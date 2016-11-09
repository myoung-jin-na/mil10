//***************************************************************************************/
// 
// File name: CircleMeasurement.cpp  
//
// Synopsis:  This program contains an example of circle measurement using the measurement
//            module. See the PrintHeader() function below for detailed description.
//
//
#include <mil.h>

//****************************************************************************
// Example description.
//****************************************************************************
void PrintHeader()   
   {
   MosPrintf(MIL_TEXT("[EXAMPLE NAME]\n"));
   MosPrintf(MIL_TEXT("CircleMeasurement\n\n"));

   MosPrintf(MIL_TEXT("[SYNOPSIS]\n"));
   MosPrintf(MIL_TEXT("This example shows how to use some controls and score functions\n"));
   MosPrintf(MIL_TEXT("to measure four circles.\n\n"));

   MosPrintf(MIL_TEXT("[MODULES USED]\n"));
   MosPrintf(MIL_TEXT("Modules used: application, system, display, buffer, graphic, measurement.\n\n"));

   MosPrintf(MIL_TEXT("Press <Enter> to start.\n\n"));
   MosGetch();
   }

// Circle measurement function prototypes.
void MeasureCircle(MIL_ID MilSystem, MIL_ID MilDisplay, MIL_INT ImageIndex);

void IntroMessageForCircle0();
void IntroMessageForCircle1();
void IntroMessageForCircle2();
void IntroMessageForCircle3();

void SetupForCircle1(MIL_ID MilCircleMarker);
void SetupForCircle2(MIL_ID MilCircleMarker);
void SetupForCircle3(MIL_ID MilCircleMarker);

void FindMarkerAndShowResults(MIL_ID MilDisplay,
                              MIL_ID MilImage,
                              MIL_ID MilOverlayImage,
                              MIL_ID MilCircleMarker);

//****************************************************************************
// Constants.
//****************************************************************************

// Filenames.
#define EXAMPLE_IMAGE_PATH    M_IMAGE_PATH MIL_TEXT("CircleMeasurement/")

static const MIL_INT NB_IMAGES = 4;
static const MIL_TEXT_CHAR* IMAGE_FILENAME[NB_IMAGES] =
   {
   EXAMPLE_IMAGE_PATH MIL_TEXT("circle0.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("circle1.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("circle2.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("circle3.mim")
   };

// Draw colors.
static const MIL_DOUBLE POSITION_COLOR      = M_COLOR_YELLOW;
static const MIL_DOUBLE SUB_POSITIONS_COLOR = M_COLOR_GREEN;
static const MIL_DOUBLE RING_COLOR          = M_COLOR_BLUE;
static const MIL_DOUBLE EDGES_COLOR         = M_COLOR_RED;

// Search settings.
static const MIL_DOUBLE POLARITY         [NB_IMAGES] = {M_ANY, M_NEGATIVE, M_NEGATIVE, M_POSITIVE};
static const MIL_DOUBLE RING_CENTER_POS_X[NB_IMAGES] = {228.0,      333.0,      260.0,      270.0};
static const MIL_DOUBLE RING_CENTER_POS_Y[NB_IMAGES] = {282.0,      240.0,      230.0,      225.0};
static const MIL_DOUBLE RING_INNER_RADIUS[NB_IMAGES] = { 15.0,       50.0,       20.0,       50.0};
static const MIL_DOUBLE RING_OUTER_RADIUS[NB_IMAGES] = {150.0,      235.0,      200.0,      200.0};

// Maximum number of pixels of deviation from the circle radius.
static const MIL_DOUBLE MAX_DEVIATION = 2.0;

// Edge threshold to use in noisy images
static const MIL_DOUBLE HIGHER_EDGEVALUE = 5.0;

//*****************************************************************************
// Main.
//*****************************************************************************
int MosMain(void)
   {
   // Allocate MIL objects. 
   MIL_ID MilApplication = MappAlloc(M_NULL, M_DEFAULT, M_NULL);
   MIL_ID MilSystem      = MsysAlloc(M_DEFAULT, M_SYSTEM_HOST, M_DEFAULT, M_DEFAULT, M_NULL);
   MIL_ID MilDisplay     = MdispAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, M_NULL);

   PrintHeader();

   for (MIL_INT ImageIndex = 0; ImageIndex < NB_IMAGES; ++ImageIndex)
      MeasureCircle(MilSystem, MilDisplay, ImageIndex);

   // Free MIL objects.
   MdispFree(MilDisplay);
   MsysFree(MilSystem);
   MappFree(MilApplication);

   return 0;
   }

//***************************************************************************
// Create and setup a circle marker to measure the center and radius of a
// circle in a given image.
//***************************************************************************
void MeasureCircle(MIL_ID MilSystem, MIL_ID MilDisplay, MIL_INT ImageIndex)
   {
   MosPrintf(MIL_TEXT("CIRCLE %d\n")
             MIL_TEXT("========\n\n"),
             ImageIndex);

   switch (ImageIndex)
      {
      case 0: IntroMessageForCircle0(); break;
      case 1: IntroMessageForCircle1(); break;
      case 2: IntroMessageForCircle2(); break;
      case 3: IntroMessageForCircle3(); break;
      }

   // Restore and display the source image.
   MIL_ID MilImage = MbufRestore(IMAGE_FILENAME[ImageIndex], MilSystem, M_NULL);
   MdispSelect(MilDisplay, MilImage);

   // Prepare for overlay annotation.
   MdispControl(MilDisplay, M_OVERLAY, M_ENABLE);
   MIL_ID MilOverlayImage = MdispInquire(MilDisplay, M_OVERLAY_ID, M_NULL);

   // Allocate circle marker and setup search region and polarity.
   MIL_ID MilCircleMarker = MmeasAllocMarker(MilSystem, M_CIRCLE, M_DEFAULT, M_NULL);

   MmeasSetMarker(MilCircleMarker, M_POLARITY   , POLARITY[ImageIndex]         , M_NULL                       );
   MmeasSetMarker(MilCircleMarker, M_RING_CENTER, RING_CENTER_POS_X[ImageIndex], RING_CENTER_POS_Y[ImageIndex]);
   MmeasSetMarker(MilCircleMarker, M_RING_RADII , RING_INNER_RADIUS[ImageIndex], RING_OUTER_RADIUS[ImageIndex]);

   if (ImageIndex != 0)
      {
      MosPrintf(MIL_TEXT("If you measure the circle with default settings, results are not as expected.\n\n"));
      }

   // Measure circle and print results.
   FindMarkerAndShowResults(MilDisplay, MilImage, MilOverlayImage, MilCircleMarker);

   // Pause to show results.
   if (ImageIndex == 0)
      {
      MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
      }
   else
      {
      MosPrintf(MIL_TEXT("Press <Enter> to measure again, using custom settings.\n\n"));
      }
   MosGetch();

   // Clear the overlay image.
   MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);

   // Except for the first case, measure again using new settings.
   if (ImageIndex != 0)
      {
      MosPrintf(MIL_TEXT("----------\n\n"));

      // Set up controls and score functions that are specific to each circle.
      switch (ImageIndex)
         {
         case 1: SetupForCircle1(MilCircleMarker); break;
         case 2: SetupForCircle2(MilCircleMarker); break;
         case 3: SetupForCircle3(MilCircleMarker); break;
         }

      // Measure circle and print results.
      FindMarkerAndShowResults(MilDisplay, MilImage, MilOverlayImage, MilCircleMarker);

      // Pause to show results.
      if (ImageIndex < NB_IMAGES-1)
         MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
      else
         MosPrintf(MIL_TEXT("Press <Enter> to end.\n\n"));
      MosGetch();

      // Clear the overlay image.
      MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);
      }

   // Free all allocations.
   MmeasFree(MilCircleMarker);
   MbufFree(MilImage);
   }

//***************************************************************************
// Print explanations for circle 0
//***************************************************************************
void IntroMessageForCircle0()
   {
   MosPrintf(MIL_TEXT("In simple cases, only the search region (in blue) needs to be set, using\n")
             MIL_TEXT("M_RING_CENTER and M_RING_RADII; all other settings can be left to their\n")
             MIL_TEXT("default.\n\n"));
   }

//***************************************************************************
// Print explanations for circle 1
//***************************************************************************
void IntroMessageForCircle1()
   {
   MosPrintf(MIL_TEXT("In this case, the contour of the outer circle forms a double edge; however,\n")
             MIL_TEXT("the circle must be measured along its outer edge only.\n\n"));
   }

//***************************************************************************
// Specific circle marker setup to measure circle 1.
//***************************************************************************
void SetupForCircle1(MIL_ID MilCircleMarker)
   {
   // Ensures that all edges contributing to the measurement are near the circle.
   MmeasSetMarker(MilCircleMarker, M_MAX_ASSOCIATION_DISTANCE, MAX_DEVIATION, M_NULL);

   MosPrintf(MIL_TEXT("For more accurate measurements, the following setting is used:\n\n")
             MIL_TEXT("   M_MAX_ASSOCIATION_DISTANCE\n")
             MIL_TEXT("      This ensures that all edges contributing to the measurement are near\n")
             MIL_TEXT("      the circle.\n\n"));
   }

//***************************************************************************
// Print explanations for circle 2
//***************************************************************************
void IntroMessageForCircle2()
   {
   MosPrintf(MIL_TEXT("The object in this image is textured; therefore, many edges could be detected\n")
             MIL_TEXT("inside the circle. It is also an incomplete circle, so edges on the straight\n")
             MIL_TEXT("side must not be considered.\n\n"));
   }

//***************************************************************************
// Specific circle marker setup to measure circle 2.
//***************************************************************************
void SetupForCircle2(MIL_ID MilCircleMarker)
   {
   // Ensures all edges contributing to measurement are near the circle.
   MmeasSetMarker(MilCircleMarker, M_MAX_ASSOCIATION_DISTANCE, MAX_DEVIATION, M_NULL);

   // Only consider stronger edges, useful in noisy images.
   MmeasSetMarker(MilCircleMarker, M_EDGEVALUE_MIN, HIGHER_EDGEVALUE, M_NULL);

   MosPrintf(MIL_TEXT("For more accurate measurements, the following settings are used:\n\n")
             MIL_TEXT("   M_MAX_ASSOCIATION_DISTANCE\n")
             MIL_TEXT("      This ensures that all edges contributing to the measurement are near\n")
             MIL_TEXT("      the circle.\n")
             MIL_TEXT("   M_EDGEVALUE_MIN\n")
             MIL_TEXT("      This ensures that only stronger edges are considered, which is useful\n")
             MIL_TEXT("      in noisy images.\n\n"));
   }

//***************************************************************************
// Print explanations for circle 3
//***************************************************************************
void IntroMessageForCircle3()
   {
   MosPrintf(MIL_TEXT("This image contains concentric circles. A criterion must be added to\n")
             MIL_TEXT("select a specific circle; in this case, the innermost circle.\n\n"));
   }

//***************************************************************************
// Specific circle marker setup to measure circle 3.
//***************************************************************************
void SetupForCircle3(MIL_ID MilCircleMarker)
   {
   // Ensures that all edges contributing to the measurement are near the circle.
   MmeasSetMarker(MilCircleMarker, M_MAX_ASSOCIATION_DISTANCE, MAX_DEVIATION, M_NULL);

   // Removes strength criterion (replaces it by a flat function).
   MmeasSetScore(MilCircleMarker, M_STRENGTH_SCORE,
                 0, 0, M_MAX_POSSIBLE_VALUE, M_MAX_POSSIBLE_VALUE,  // flat score function
                 M_DEFAULT, M_DEFAULT, M_DEFAULT);

   // Adds radius criterion, chooses smallest circle (decreasing ramp function).
   MmeasSetScore(MilCircleMarker, M_RADIUS_SCORE,
                 0, 0, 0, M_MAX_POSSIBLE_VALUE,   // decreasing ramp score function
                 M_DEFAULT, M_DEFAULT, M_DEFAULT);

   MosPrintf(MIL_TEXT("For more accurate measurements, the following settings are used:\n\n")
             MIL_TEXT("   M_MAX_ASSOCIATION_DISTANCE\n")
             MIL_TEXT("      This ensures that all edges contributing to the measurement are near\n")
             MIL_TEXT("      the circle.\n")
             MIL_TEXT("   M_STRENGTH_SCORE\n")
             MIL_TEXT("      This removes the strength criterion (replaces it by a flat function).\n")
             MIL_TEXT("   M_RADIUS_SCORE\n")
             MIL_TEXT("      This adds a radius criterion, which chooses the smallest circle\n")
             MIL_TEXT("      (decreasing ramp function).\n\n"));
   }

//***************************************************************************
// Measure circle and print results.
//***************************************************************************
void FindMarkerAndShowResults(MIL_ID MilDisplay,
                              MIL_ID MilImage,
                              MIL_ID MilOverlayImage,
                              MIL_ID MilCircleMarker)
   {
   // Draw the contour of the measurement search region.
   MgraColor(M_DEFAULT, RING_COLOR);
   MmeasDraw(M_DEFAULT, MilCircleMarker, MilOverlayImage, M_DRAW_SEARCH_REGION, M_DEFAULT, M_MARKER);

   // Find the circle and measure its position and radius.
   MmeasFindMarker(M_DEFAULT, MilImage, MilCircleMarker, M_DEFAULT);

   // If occurrence is found, show the results.
   MIL_INT NumberOccurrencesFound;
   MmeasGetResult(MilCircleMarker, M_NUMBER+M_TYPE_MIL_INT, &NumberOccurrencesFound, M_NULL);

   if (NumberOccurrencesFound >= 1)
      {
      MIL_DOUBLE CircleCenterX, CircleCenterY, CircleRadius;
      MmeasGetResult(MilCircleMarker, M_POSITION, &CircleCenterX, &CircleCenterY);
      MmeasGetResult(MilCircleMarker, M_RADIUS  , &CircleRadius , M_NULL        );

      // Draw a cross on the center of the found circle.
      MgraColor(M_DEFAULT, POSITION_COLOR);
      MmeasDraw(M_DEFAULT, MilCircleMarker, MilOverlayImage, M_DRAW_POSITION, M_DEFAULT, M_RESULT);

      // Draw the found circle itself.
      MgraColor(M_DEFAULT, EDGES_COLOR);
      MmeasDraw(M_DEFAULT, MilCircleMarker, MilOverlayImage, M_DRAW_EDGES, M_DEFAULT, M_RESULT);

      // Draw the edges used to fit the circle.
      MgraColor(M_DEFAULT, SUB_POSITIONS_COLOR);
      MmeasDraw(M_DEFAULT, MilCircleMarker, MilOverlayImage, M_DRAW_SUB_POSITIONS, M_DEFAULT, M_RESULT);

      // Print the result.
      MosPrintf(MIL_TEXT("The circle was found (in red):\n")
                MIL_TEXT("   Center position (in yellow): (%.2f, %.2f)\n")
                MIL_TEXT("   Radius: %.2f\n\n"),
                CircleCenterX, CircleCenterY, CircleRadius);
      }
   else
      {
      // Print the result.
      MosPrintf(MIL_TEXT("The circle was not found.\n\n"));
      }
   }
