///*************************************************************************************
//
// File name: FoodInspection_Cereal.cpp
//
// Synopsis:  This program contains an example of food inspection using the color module.
//            See the PrintHeader() function below for detailed description.
//
//
#include <mil.h>

///***************************************************************************
// Example description.
///***************************************************************************
void PrintHeader()
   {
   MosPrintf(MIL_TEXT("[EXAMPLE NAME]\n"));
   MosPrintf(MIL_TEXT("FoodInspection_Cereal\n\n"));

   MosPrintf(MIL_TEXT("[SYNOPSIS]\n"));
   MosPrintf(MIL_TEXT("This example performs food inspection for cereal on a conveyor.\n"));
   MosPrintf(MIL_TEXT("4 types of cereal are expected, in the following order\n"));
   MosPrintf(MIL_TEXT("(from left to right): \n"));
   MosPrintf(MIL_TEXT("   +-----------+-----------+-----------+-----------+   \n"));
   MosPrintf(MIL_TEXT("   | Kids'     | Red       | Tropical  | Honey     |   \n"));
   MosPrintf(MIL_TEXT("   | Mix       | Berry     | Mix       | Nut       |   \n"));
   MosPrintf(MIL_TEXT("   | cereal    | cereal    | cereal    | cereal    |   \n"));
   MosPrintf(MIL_TEXT("   +-----------+-----------+-----------+-----------+   \n"));
   MosPrintf(MIL_TEXT("The Measurement tool is used to locate the 4 regions\n"));
   MosPrintf(MIL_TEXT("that contain cereal. The Color Matching tool is used\n"));
   MosPrintf(MIL_TEXT("to identify the type of cereal in each region.\n\n"));
   MosPrintf(MIL_TEXT("If the type of cereal matches the expected product,\n"));
   MosPrintf(MIL_TEXT("it is a Pass; otherwise, it is a Fail.\n\n"));

   MosPrintf(MIL_TEXT("[MODULES USED]\n"));
   MosPrintf(MIL_TEXT("Modules used: application, system, display, measurement, color.\n\n"));

   // Wait for a key to be pressed.
   MosPrintf(MIL_TEXT("Press <Enter> to start.\n"));
   MosGetch();

   MosPrintf(MIL_TEXT("\nFOOD INSPECTION USING COLOR MATCHING:\n"));
   MosPrintf(MIL_TEXT("---------------------------------------\n"));
   }

// Functions declaration.
MIL_ID AllocateAndDefineStripeMarker(MIL_ID MilSystem);

bool LocateConveyorAndDrawAreas(MIL_ID MilTargetImage,    MIL_ID MilMeasVStripe,
                                MIL_ID MilAreaLabelImage, MIL_ID MilOverlayImage,
                                MIL_DOUBLE* AreaPositionXTable, MIL_DOUBLE* AreaPositionYTable);

void DisplayResults(MIL_ID MilColorResult, MIL_ID MilOverlayImage,
                    const MIL_DOUBLE* AreaPositionXTable, const MIL_DOUBLE* AreaPositionYTable);

template <class T> inline T Max(T x, T y) { return ( (x >= y) ? x : y ); }

// Macros definition.
#define EXAMPLE_IMAGE_PATH M_IMAGE_PATH MIL_TEXT("FoodInspection_Cereal/")

// Constants definition.
static const MIL_INT  MAX_CHAR_LENGTH = 256;

static const MIL_INT  NUM_IMAGES      =   7;
static const MIL_INT  NUM_SAMPLES     =   4;
static const MIL_INT  NUM_AREAS       =   4;

static MIL_CONST_TEXT_PTR TARGET_IMAGE_LIST_FILENAME[NUM_IMAGES] =
   {
   EXAMPLE_IMAGE_PATH MIL_TEXT("TargetCereals0.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("TargetCereals1.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("TargetCereals2.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("TargetCereals3.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("TargetCereals4.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("TargetCereals5.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("TargetCereals6.mim")
   };

static MIL_CONST_TEXT_PTR SAMPLE_IMAGE_LIST_FILENAME[NUM_SAMPLES] =
   {
   EXAMPLE_IMAGE_PATH MIL_TEXT("SampleCereals0.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("SampleCereals1.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("SampleCereals2.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("SampleCereals3.mim")
   };

static const MIL_INT SAMPLE_STRIP_MARGIN =   5;
static const MIL_INT SAMPLE_ROI_SIZE     = 200;
static const MIL_INT SAMPLE_CHILD_SIZE   = SAMPLE_ROI_SIZE + (2 * SAMPLE_STRIP_MARGIN);

// Area width and height
static const MIL_DOUBLE AREA_WIDTH   = 90.0;
static const MIL_DOUBLE AREA_HEIGHT  = 440.0;

// Measurement stripe parameters
static const MIL_DOUBLE STRIPE_BOX_CENTER_X = 320.0;
static const MIL_DOUBLE STRIPE_BOX_CENTER_Y = 240.0;
static const MIL_DOUBLE STRIPE_BOX_SIZE_X   = 630.0;
static const MIL_DOUBLE STRIPE_BOX_SIZE_Y   = 300.0;
static const MIL_DOUBLE FILTER_SMOOTHNESS   = 80.0;
static const MIL_DOUBLE MIN_STRIPE_WIDTH    = 90.0;
static const MIL_DOUBLE MAX_STRIPE_WIDTH    = 140.0;

// Global constants.
static const MIL_UINT8          AREA_LABELS[NUM_AREAS]    = { 80, 100, 120, 140 };

static const MIL_CONST_TEXT_PTR AREA_NAMES[NUM_AREAS]     = {
                                                              MIL_TEXT("First "),
                                                              MIL_TEXT("Second"),
                                                              MIL_TEXT("Third "),
                                                              MIL_TEXT("Fourth")
                                                            };

static const MIL_CONST_TEXT_PTR SAMPLE_NAMES[NUM_SAMPLES] = {
                                                              MIL_TEXT("Kids' Mix   "),
                                                              MIL_TEXT("Red Berry   "),
                                                              MIL_TEXT("Tropical Mix"),
                                                              MIL_TEXT("Honey Nut   ")
                                                            };

// Expected sequence of samples.
static const MIL_INT EXPECTED_SAMPLE_IDX_SEQ[NUM_AREAS]   = { 0, 1, 2, 3 };

///****************************************************************************
// Main.
///****************************************************************************
int MosMain(void)
   {
   // Allocate MIL objects.
   MIL_ID MilApplication = MappAlloc(M_NULL, M_DEFAULT, M_NULL);
   MIL_ID MilSystem      = MsysAlloc(M_DEFAULT, M_SYSTEM_HOST, M_DEFAULT, M_DEFAULT, M_NULL);
   MIL_ID MilDisplay     = MdispAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, M_NULL);

   // Print the example synopsis.
   PrintHeader();

   // Get target buffer size.
   MIL_INT ImageSizeX = MbufDiskInquire(TARGET_IMAGE_LIST_FILENAME[0], M_SIZE_X, M_NULL);
   MIL_INT ImageSizeY = MbufDiskInquire(TARGET_IMAGE_LIST_FILENAME[0], M_SIZE_Y, M_NULL);

   // Allocate area buffer.
   MIL_ID MilAreaLabelImage = MbufAlloc2d(MilSystem, ImageSizeX, ImageSizeY, 8+M_UNSIGNED, M_IMAGE+M_PROC, M_NULL);

   // Allocate a color buffer and select it to display.
   MIL_INT DispColorImageSizeX = Max(ImageSizeX, (NUM_SAMPLES * SAMPLE_CHILD_SIZE));
   MIL_INT DispColorImageSizeY = ImageSizeY + SAMPLE_CHILD_SIZE;
   MIL_ID MilDispColorImage = MbufAllocColor(MilSystem, 3, DispColorImageSizeX, DispColorImageSizeY, 8+M_UNSIGNED, M_IMAGE+M_DISP, M_NULL);
   MbufClear(MilDispColorImage, 0);

   MdispSelect(MilDisplay, MilDispColorImage);

   // Prepare display for overlay annotations
   MdispControl(MilDisplay, M_OVERLAY, M_ENABLE);
   MIL_INT MilOverlayImage = MdispInquire(MilDisplay, M_OVERLAY_ID, M_NULL);

   // Allocate an RGB color matching context.
   MIL_ID MilColorContext = McolAlloc(MilSystem, M_COLOR_MATCHING, M_RGB, M_DEFAULT, M_DEFAULT, M_NULL);

   // Define all color samples.
   MIL_INT i;
   for(i = 0; i < NUM_SAMPLES ; i++)
      {
      // Restore the sample image.
      MIL_ID MilSampleImage = MbufRestore(SAMPLE_IMAGE_LIST_FILENAME[i], MilSystem, M_NULL);

      // Add the sample into the context.
      McolDefine(MilColorContext, MilSampleImage, M_DEFAULT, M_IMAGE, 
                 M_DEFAULT, M_DEFAULT, M_DEFAULT, M_DEFAULT);

      // Copy the samples inside the bottom of the display image.
      MIL_ID MilDisplayChild = MbufChild2d(MilDispColorImage, 
                                           i * (SAMPLE_ROI_SIZE + SAMPLE_STRIP_MARGIN) + SAMPLE_STRIP_MARGIN,
                                           ImageSizeY + SAMPLE_STRIP_MARGIN,
                                           SAMPLE_ROI_SIZE,
                                           SAMPLE_ROI_SIZE,
                                           M_NULL);
      MbufCopy(MilSampleImage, MilDisplayChild);

      // Draw the sample name.
      MIL_TEXT_CHAR SampleString[MAX_CHAR_LENGTH];
      MosSprintf(SampleString, MAX_CHAR_LENGTH, MIL_TEXT("#%d %s"), i, SAMPLE_NAMES[i]);
      MgraColor(M_DEFAULT, M_COLOR_YELLOW);
      MgraText(M_DEFAULT, MilDisplayChild, 4, 2, SampleString);

      // Free the buffers.
      MbufFree(MilDisplayChild);
      MbufFree(MilSampleImage);
      }

   // Set the context operation mode (histogram because we have multi-colored samples).
   McolSetMethod(MilColorContext, M_HISTOGRAM_MATCHING, M_DEFAULT, M_DEFAULT, M_DEFAULT);

   // Preprocess the context.
   McolPreprocess(MilColorContext, M_DEFAULT);

   // Allocate a color matching result.
   MIL_ID MilColorResult = McolAllocResult(MilSystem, M_COLOR_MATCHING_RESULT, M_DEFAULT, M_NULL);

   // Define the measurement marker.
   MIL_ID MilMeasVStripe = AllocateAndDefineStripeMarker(MilSystem);

   // Loop on all target images.
   for(i = 0; i < NUM_IMAGES ; i++)
      {
      // Restore the target image.
      MIL_ID MilTargetImage = MbufRestore(TARGET_IMAGE_LIST_FILENAME[i], MilSystem, M_NULL);

      // Clear the overlay.
      MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);

      // Copy the target into the display buffer.
      MbufCopy(MilTargetImage, MilDispColorImage);

      // Temporarily disable the display update.
      MdispControl(MilDisplay, M_UPDATE, M_DISABLE);

      MIL_DOUBLE AreaPositionXTable[NUM_AREAS],    // X coordinate of top-left area corner
                 AreaPositionYTable[NUM_AREAS];    // Y coordinate of top-left area corner

      // Perform a measurement operation to define the target's areas for color matching.
      if(LocateConveyorAndDrawAreas(MilTargetImage, MilMeasVStripe, MilAreaLabelImage, MilOverlayImage,
                                    AreaPositionXTable, AreaPositionYTable))
         {
         // Perform the color matching within the areas.
         McolMatch(MilColorContext, MilTargetImage, M_DEFAULT, MilAreaLabelImage, MilColorResult, M_DEFAULT);

         // Display the match results.
         MosPrintf(MIL_TEXT("MATCH RESULTS ( Image %02d ):\n"), i);
         DisplayResults(MilColorResult, MilOverlayImage, AreaPositionXTable, AreaPositionYTable);
         }
      else
         {
         MosPrintf(MIL_TEXT("Error: No tray found.\n\n"));
         }

      // Enable back the display update.
      MdispControl(MilDisplay, M_UPDATE, M_ENABLE);

      MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
      MosGetch();

      // Free the target image.
      MbufFree(MilTargetImage);
      }

   MdispSelect(MilDisplay, M_NULL);

   MosPrintf(MIL_TEXT("%d images were inspected.\n\n"), i);
   MosPrintf(MIL_TEXT("Press <Enter> to end.\n"));
   MosGetch();

   // Free color objects.
   McolFree(MilColorResult);
   McolFree(MilColorContext);

   // Free measurement objects.
   MmeasFree(MilMeasVStripe);

   // Free allocated buffers.
   MbufFree(MilAreaLabelImage);
   MbufFree(MilDispColorImage);

   // Free MIL objects.
   MdispFree(MilDisplay);
   MsysFree(MilSystem);
   MappFree(MilApplication);

   return 0;
   }
//***************************************************************************
// Allocate a measurement marker and set it to find the vertical stripes
//***************************************************************************
MIL_ID AllocateAndDefineStripeMarker(MIL_ID MilSystem)
   {
   MIL_ID StripeMarker = MmeasAllocMarker(MilSystem, M_STRIPE, M_DEFAULT, M_NULL);

   // Set the stripe orientation.
   MmeasSetMarker(StripeMarker, M_ORIENTATION, M_VERTICAL, M_NULL);
   
   // Set the number of stripes to find.
   MmeasSetMarker(StripeMarker, M_NUMBER, (double) NUM_AREAS, M_NULL);

   // Set the stripe edge polarity.
   MmeasSetMarker(StripeMarker, M_POLARITY, M_POSITIVE, M_OPPOSITE);

   // Set a Shen filter and adjust its smoothness.
   MmeasSetMarker(StripeMarker, M_FILTER_TYPE, M_SHEN, M_NULL);
   MmeasSetMarker(StripeMarker, M_FILTER_SMOOTHNESS, FILTER_SMOOTHNESS, M_NULL);

   // Set the measurement search region, in pixels.
   MmeasSetMarker(StripeMarker, M_BOX_CENTER, STRIPE_BOX_CENTER_X, STRIPE_BOX_CENTER_Y);
   MmeasSetMarker(StripeMarker, M_BOX_SIZE, STRIPE_BOX_SIZE_X, STRIPE_BOX_SIZE_Y);

   // Set the stripe accepted width, in pixels.
   MmeasSetScore(StripeMarker, M_STRIPE_WIDTH_SCORE, MIN_STRIPE_WIDTH, MIN_STRIPE_WIDTH, MAX_STRIPE_WIDTH, MAX_STRIPE_WIDTH, 
                 M_DEFAULT, M_PIXEL, M_DEFAULT);   
   
   return StripeMarker;
   }
//***************************************************************************
// Use the measurement tool to locate each object position, then draw
// the corresponding area label image.
//***************************************************************************
bool LocateConveyorAndDrawAreas(MIL_ID MilTargetImage,
                                MIL_ID MilMeasVStripe,
                                MIL_ID MilAreaLabelImage,
                                MIL_ID MilOverlayImage,
                                MIL_DOUBLE* AreaPositionXTable,
                                MIL_DOUBLE* AreaPositionYTable)
   {
   bool AreasFound = false;

   // Use a grayscale version of the color image for measurement: the green band.
   MIL_ID MilGreenBand;
   MbufChildColor(MilTargetImage, M_GREEN, &MilGreenBand);

   // Find the vertical stripes.
   MmeasFindMarker(M_DEFAULT, MilGreenBand, MilMeasVStripe, M_POSITION+M_STRIPE_WIDTH);

   MbufFree(MilGreenBand);

   // Check if stripes were found.
   MIL_INT Number;
   MmeasGetResult(MilMeasVStripe, M_NUMBER + M_TYPE_MIL_INT, &Number, M_NULL);

   if(NUM_AREAS == Number)
      {
      AreasFound = true;

      MbufClear(MilAreaLabelImage, 0);

      // Draw the found stripes.
      MgraColor(M_DEFAULT, M_COLOR_MAGENTA);
      MmeasDraw(M_DEFAULT, MilMeasVStripe, MilOverlayImage, M_DRAW_WIDTH, M_DEFAULT, M_DEFAULT);

      for(MIL_INT i=0; i < Number; i++)
         {
         // Get the (X,Y) position of the stripe center.
         MIL_DOUBLE X, Y;
         MmeasGetResultSingle(MilMeasVStripe, M_POSITION, &X, &Y, i);

         // Draw the areas as filled regions in the MilAreaLabelImage with respect
         // to the found position (needed for McolMatch).
         MgraColor(M_DEFAULT, AREA_LABELS[i]);
         MgraRectAngle(M_DEFAULT, MilAreaLabelImage, X, Y, AREA_WIDTH, AREA_HEIGHT, 0.0, 
                       M_CENTER_AND_DIMENSION+M_FILLED);

         // Draw the areas contour as annotations in the overlay.
         MgraColor(M_DEFAULT, M_COLOR_BLUE);
         MgraRectAngle(M_DEFAULT, MilOverlayImage, X, Y, AREA_WIDTH, AREA_HEIGHT, 0.0, 
                       M_CENTER_AND_DIMENSION);

         // Store the top-left corner of the area
         AreaPositionXTable[i] = X;
         AreaPositionYTable[i] = Y;
         }
      }

   return AreasFound;
   }

//***************************************************************************
// Display the color match results as text and overlay annotations.
//***************************************************************************
void DisplayResults(MIL_ID MilColorResult, MIL_ID MilOverlayImage,
                    const MIL_DOUBLE* AreaPositionXTable, const MIL_DOUBLE* AreaPositionYTable)
   {
   // Text alignment.
   MgraControl(M_DEFAULT, M_TEXT_ALIGN_HORIZONTAL, M_CENTER);
   MgraControl(M_DEFAULT, M_TEXT_ALIGN_VERTICAL, M_CENTER);

   // Retrieve needed results.
   MIL_INT  NumAreas;                     // Total number of areas.
   McolGetResult(MilColorResult, M_GENERAL, M_GENERAL, M_NUMBER_OF_AREAS + M_TYPE_MIL_INT, &NumAreas);

   MIL_INT* AreaLabelsTable      = new MIL_INT[NumAreas]; // Area labels.
   MIL_INT* BestMatchSampleTable = new MIL_INT[NumAreas]; // Best match sample for each area.
   McolGetResult(MilColorResult, M_ALL, M_GENERAL, M_AREA_LABEL_VALUE + M_TYPE_MIL_INT, AreaLabelsTable);
   McolGetResult(MilColorResult, M_ALL, M_GENERAL, M_BEST_MATCH_INDEX + M_TYPE_MIL_INT, BestMatchSampleTable);

   // For each area, retrieve, verify and display results.
   bool ExpectedSampleSequenceOK = true;
   for(MIL_INT i = 0; i < NumAreas; i++)
      {
      MIL_INT ExpectedSampleIndex = EXPECTED_SAMPLE_IDX_SEQ[i];
      MIL_INT MatchedSampleIndex = BestMatchSampleTable[i];

      MIL_DOUBLE MatchedSampleScore;
      McolGetResult(MilColorResult, AreaLabelsTable[i], M_SAMPLE_INDEX(BestMatchSampleTable[i]), M_SCORE, &MatchedSampleScore);

      // Set annotation according to status of color matching.
      double TextColor;
      MIL_TEXT_CHAR AreaStatus[MAX_CHAR_LENGTH];
      if(MatchedSampleIndex == ExpectedSampleIndex)
         {
         TextColor = M_COLOR_GREEN;
         MosSprintf(AreaStatus, MAX_CHAR_LENGTH, MIL_TEXT("PASS"));
         }
      else
         {
         ExpectedSampleSequenceOK = false;

         TextColor = M_COLOR_RED;
         MosSprintf(AreaStatus, MAX_CHAR_LENGTH, MIL_TEXT("FAIL"));
         }

      // Display the results as text.
      MgraColor(M_DEFAULT, TextColor);
      MgraText(M_DEFAULT, MilOverlayImage, AreaPositionXTable[i], AreaPositionYTable[i], AreaStatus);
      MosPrintf(MIL_TEXT("  %s area, expected: #%d, match: #%d (%5.2f%%): %s\n"),
                AREA_NAMES[i], ExpectedSampleIndex, MatchedSampleIndex, MatchedSampleScore, AreaStatus);
      }

   if(ExpectedSampleSequenceOK)
      {
      MosPrintf(MIL_TEXT("*** Overall status: PASS ***\n\n"));
      }
   else
      {
      MosPrintf(MIL_TEXT("*** Overall status: FAIL ***\n\n"));
      }

   // Delete tables.
   delete [] AreaLabelsTable;
   delete [] BestMatchSampleTable;
   }