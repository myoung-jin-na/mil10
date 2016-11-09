//***************************************************************************************/
// 
// File name: FoodInspection_Mango.cpp  
//
// Synopsis:  This program contains an example of mango food inspection using the color module.
//            See the PrintHeader() function below for detailed description.
//
//
#include <mil.h>

//****************************************************************************
// Example description.
//****************************************************************************
void PrintHeader()
   {
   MosPrintf(MIL_TEXT("[EXAMPLE NAME]\n")
             MIL_TEXT("FoodInspection_Mango\n\n")

             MIL_TEXT("[SYNOPSIS]\n")
             MIL_TEXT("This example shows how to determine the ripeness of a mango using color\n")
             MIL_TEXT("matching. In the example, a mango is considered ripe when the area of the\n")
             MIL_TEXT("pixels that matched with the green sample is less than a certain percentage\n")
             MIL_TEXT("of the total matching area of the mango.\n\n")
  
             MIL_TEXT("[MODULES USED]\n")
             MIL_TEXT("Modules used: Application, system, image processing, color\n\n")
  
             MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();
   }

// Paths definitions.
#define EXAMPLE_IMAGE_PATH M_IMAGE_PATH MIL_TEXT("FoodInspection_Mango/")

static const MIL_INT NB_IMAGES = 10;
static MIL_CONST_TEXT_PTR NUMBERED_IMAGE_FILE_NAME = EXAMPLE_IMAGE_PATH MIL_TEXT("Mango_%i.jpg");

static MIL_CONST_TEXT_PTR COLOR_CONTEXT_PATH = EXAMPLE_IMAGE_PATH MIL_TEXT("MangoColor.mcol");

// Color samples informations.
static const MIL_INT NB_COLOR_SAMPLES = 5;
static MIL_CONST_TEXT_PTR SAMPLE_STRING[NB_COLOR_SAMPLES] = 
   {
   MIL_TEXT("Dark Red"),
   MIL_TEXT("Bright Red"),
   MIL_TEXT("Yellow"),
   MIL_TEXT("Orange"),
   MIL_TEXT("Green")
   };

static const MIL_INT GREEN_INDEX = 4;

// Constants.
static const MIL_INT NB_ERODE_ITER = 10;
static const MIL_DOUBLE GREEN_THRESHOLD = 30;
static const MIL_INT PASS_FAIL_OFFSET = 30;

// Result table parameters.
static const MIL_INT COLUMN_WIDTH = 16;

// Functions declarations. 
void DrawSample(MIL_ID MilColContext, MIL_ID MilDest, MIL_UINT SampleIdx, MIL_INT OffsetX, MIL_INT OffsetY, MIL_INT SamplePatchSize);
void PrintResultTable(MIL_DOUBLE SamplesScores[NB_COLOR_SAMPLES]);

//*****************************************************************************
// Main.
//*****************************************************************************
int MosMain(void)
   {
   // Allocate MIL objects.
   MIL_ID MilApplication = MappAlloc(M_NULL, M_DEFAULT, &MilApplication);
   MIL_ID MilSystem = MsysAlloc(M_DEFAULT, M_SYSTEM_HOST, M_DEFAULT, M_DEFAULT, &MilSystem);
   MIL_ID MilDisplay = MdispAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &MilDisplay);

   // Allocate a graphic list and associate it with the display.
   MIL_ID MilGraList = MgraAllocList(MilSystem, M_DEFAULT, M_NULL);
   MdispControl(MilDisplay, M_ASSOCIATED_GRAPHIC_LIST_ID, MilGraList);

   // Restore the color matching context and allocate a color result.
   MIL_ID MilColContext = McolRestore(COLOR_CONTEXT_PATH, MilSystem, M_DEFAULT, M_NULL);
   MIL_ID MilColResult = McolAllocResult(MilSystem, M_DEFAULT, M_DEFAULT, M_NULL);

   // Preprocess the color context.
   McolPreprocess(MilColContext, M_DEFAULT);
   
   MosPrintf(MIL_TEXT("FOOD INSPECTION MANGO:\n"));
   MosPrintf(MIL_TEXT("---------------------------------------\n\n"));

   // Print the header.
   PrintHeader();

   // Get the size of the images.
   MIL_TEXT_CHAR ImageFilePath[512];
   MosSprintf(ImageFilePath, 512, NUMBERED_IMAGE_FILE_NAME, 0);
   MIL_INT ImageSizeX = MbufDiskInquire(ImageFilePath, M_SIZE_X, M_NULL);
   MIL_INT ImageSizeY = MbufDiskInquire(ImageFilePath, M_SIZE_Y, M_NULL);

   // Allocate the display image and the child for the image.
   MIL_INT SamplePatchSize = ImageSizeY / NB_COLOR_SAMPLES;
   MIL_ID MilDisplayImage = MbufAllocColor(MilSystem, 3, 2*ImageSizeX + SamplePatchSize, ImageSizeY, 8+M_UNSIGNED, M_IMAGE+M_PROC+M_DISP, M_NULL);
   MIL_ID MilInspectionImage = MbufChild2d(MilDisplayImage, 0, 0, ImageSizeX, ImageSizeY, M_NULL);
   MIL_ID MilInspectionBlue = MbufChildColor2d(MilInspectionImage, M_BLUE, 1, 1, ImageSizeX-2, ImageSizeY-2, M_NULL);
   MIL_ID MilResultImage = MbufChild2d(MilDisplayImage, ImageSizeX, 0, ImageSizeX, ImageSizeY, M_NULL);

   // Restore the initial inspection image and clear the result image.
   MbufLoad(ImageFilePath, MilInspectionImage);
   MbufClear(MilResultImage, 0);

   // Allocate the binary and area image.
   MIL_ID MilBinaryImage = MbufAlloc2d(MilSystem, ImageSizeX, ImageSizeY, 1+M_UNSIGNED, M_IMAGE+M_PROC, M_NULL);
   MIL_ID MilBinaryChild = MbufChild2d(MilBinaryImage, 1, 1, ImageSizeX-2, ImageSizeY-2, M_NULL);
   MIL_ID MilAreaImage = MbufAlloc2d(MilSystem, ImageSizeX, ImageSizeY, 1+M_UNSIGNED, M_IMAGE+M_PROC, M_NULL);
   
   // Set the horizontal text alignment.
   MgraControl(M_DEFAULT, M_TEXT_ALIGN_HORIZONTAL, M_CENTER);

   // Draw the title of the images in the graphic list.
   MgraColor(M_DEFAULT, M_COLOR_RED);
   MgraControl(M_DEFAULT, M_FONT, M_FONT_DEFAULT_MEDIUM);
   MgraText(M_DEFAULT, MilGraList, ImageSizeX / 2, 5, MIL_TEXT("Inspection Image"));
   MgraText(M_DEFAULT, MilGraList, 3*ImageSizeX / 2, 5, MIL_TEXT("Result Image"));

   // Draw the color sample in the display image.
   MIL_UINT SampleIdx;
   MIL_INT OffsetY = 0;
   MgraControl(M_DEFAULT, M_TEXT_ALIGN_VERTICAL, M_CENTER);
   MgraControl(M_DEFAULT, M_FONT, M_FONT_DEFAULT_SMALL);
   MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
      
   for(SampleIdx = 0; SampleIdx < NB_COLOR_SAMPLES-1; SampleIdx++, OffsetY += SamplePatchSize)
      {
      DrawSample(MilColContext, MilDisplayImage, SampleIdx, 2*ImageSizeX, OffsetY, SamplePatchSize);
      }
   DrawSample(MilColContext, MilDisplayImage, SampleIdx, 2*ImageSizeX, OffsetY, ImageSizeY - OffsetY);
   MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_OPAQUE);

   // Select the image on the display.
   MdispSelect(MilDisplay, MilDisplayImage);

   // Pause to show the samples.
   MosPrintf(MIL_TEXT("The average colors of the samples used to qualify the\n")
             MIL_TEXT("ripeness of the mangos are displayed. The maximum green\n")
             MIL_TEXT("area allowed for a mango to pass is set at %.2f%%.\n")
             MIL_TEXT("We will now proceed to qualify some mangos.\n\n")
             MIL_TEXT("Press <Enter> to continue.\n\n"),
             GREEN_THRESHOLD);
   MosGetch();

   // Set the text alignment for the pass fail.
   MgraControl(M_DEFAULT, M_TEXT_ALIGN_HORIZONTAL, M_RIGHT);
   MgraControl(M_DEFAULT, M_TEXT_ALIGN_VERTICAL, M_BOTTOM);
   MgraControl(M_DEFAULT, M_FONT, M_FONT_DEFAULT_LARGE);

   for(MIL_UINT ImageIdx = 0; ImageIdx < NB_IMAGES; ImageIdx++)
      {
      // Disable the display updates.
      MdispControl(MilDisplay, M_UPDATE, M_DISABLE);

      // Clear the result image.
      MbufClear(MilResultImage, 0);
     
      // Restore the image.
      MosSprintf(ImageFilePath, 512, NUMBERED_IMAGE_FILE_NAME, ImageIdx);
      MbufLoad(ImageFilePath, MilInspectionImage);

      // Print a message.
      MosPrintf(MIL_TEXT("Inspecting mango image %i\n\n"), ImageIdx);

      // Create the area image by binarizing the blue band. Use a child to have a black border
      // around the processed image thus not having to manage the overscan.
      MimBinarize(MilInspectionBlue, MilBinaryChild, M_BIMODAL+M_LESS, M_NULL, M_NULL);

      // An erosion is done to remove the border of the blob whose color is not as reliable.
      MimErode(MilBinaryImage, MilAreaImage, NB_ERODE_ITER, M_BINARY);

      // Match the color samples.
      McolMatch(MilColContext, MilInspectionImage, M_DEFAULT, MilAreaImage, MilColResult, M_DEFAULT);

      // Get the score of the samples.
      MIL_DOUBLE SamplesScores[NB_COLOR_SAMPLES];
      McolGetResult(MilColResult, M_DEFAULT, M_ALL, M_SCORE, SamplesScores);

      // Draw the result of the color matching.
      McolDraw(M_DEFAULT, MilColResult, MilResultImage, M_DRAW_PIXEL_MATCH_USING_COLOR, M_DEFAULT, M_DEFAULT, M_DEFAULT);
      
      // Draw the status.
      bool IsNotRipe = SamplesScores[GREEN_INDEX] > GREEN_THRESHOLD;
      MgraColor(M_DEFAULT, IsNotRipe ? M_COLOR_RED : M_COLOR_GREEN);
      MgraText(M_DEFAULT, MilDisplayImage, 2*ImageSizeX - PASS_FAIL_OFFSET, ImageSizeY - PASS_FAIL_OFFSET, IsNotRipe ? MIL_TEXT("FAIL") : MIL_TEXT("PASS"));

      // Enable the display updates.
      MdispControl(MilDisplay, M_UPDATE, M_ENABLE);

      // Print the scores of the samples.
      PrintResultTable(SamplesScores);

      // Print the status of the mango.
      MosPrintf(MIL_TEXT("Mango ripeness : %s\n\n"), IsNotRipe ? MIL_TEXT("FAIL") : MIL_TEXT("PASS"));
      MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
      MosGetch();
      }

   // Free the images.
   MbufFree(MilBinaryChild);
   MbufFree(MilBinaryImage);
   MbufFree(MilAreaImage);
   MbufFree(MilResultImage);
   MbufFree(MilInspectionBlue);
   MbufFree(MilInspectionImage);
   MbufFree(MilDisplayImage);
   
   // Free color.
   McolFree(MilColResult);
   McolFree(MilColContext);

   // Free the graphic list.
   MgraFree(MilGraList);

   // Free MIL objects.     
   MdispFree(MilDisplay);
   MsysFree(MilSystem);
   MappFree(MilApplication);

   return 0;
   }

//*****************************************************************************
// Function to draw a patch of the average value of a color matching sample.
//*****************************************************************************
void DrawSample(MIL_ID MilColContext, MIL_ID MilDest, MIL_UINT SampleIdx, MIL_INT OffsetX, MIL_INT OffsetY, MIL_INT SamplePatchSize)
   {
   MIL_INT SampleColorR = McolInquire(MilColContext, M_SAMPLE_INDEX(SampleIdx), M_SAMPLE_AVERAGE_COLOR_BAND_0, M_NULL);
   MIL_INT SampleColorG = McolInquire(MilColContext, M_SAMPLE_INDEX(SampleIdx), M_SAMPLE_AVERAGE_COLOR_BAND_1, M_NULL);
   MIL_INT SampleColorB = McolInquire(MilColContext, M_SAMPLE_INDEX(SampleIdx), M_SAMPLE_AVERAGE_COLOR_BAND_2, M_NULL);
   MgraColor(M_DEFAULT, M_RGB888(SampleColorR, SampleColorG, SampleColorB));
   MgraRectAngle(M_DEFAULT, MilDest, OffsetX, OffsetY, SamplePatchSize, SamplePatchSize, 0, M_CORNER_AND_DIMENSION+M_FILLED);
   MgraColor(M_DEFAULT, M_COLOR_BLACK);
   MgraText(M_DEFAULT, MilDest, OffsetX + SamplePatchSize/2, OffsetY + SamplePatchSize/2, SAMPLE_STRING[SampleIdx]);
   }

//*****************************************************************************
// Function to print a table of the samples matching scores. 
//*****************************************************************************
void PrintResultTable(MIL_DOUBLE SamplesScores[NB_COLOR_SAMPLES])
   {
   MosPrintf(MIL_TEXT("+----------------+----------------+----------------+\n")
             MIL_TEXT("|  Sample Index  |   Sample Tag   |    Score(%%)    |\n"));

   for(MIL_INT SampleIdx = 0; SampleIdx < NB_COLOR_SAMPLES; SampleIdx++)
      {
      MosPrintf(MIL_TEXT("+----------------+----------------+----------------+\n")
                MIL_TEXT("|%*i|%*s|%*.2f|\n"),
                COLUMN_WIDTH,
                SampleIdx,
                COLUMN_WIDTH,
                SAMPLE_STRING[SampleIdx],
                COLUMN_WIDTH,
                SamplesScores[SampleIdx]);  
      }

   MosPrintf(MIL_TEXT("+----------------+----------------+----------------+\n\n"));
   }