//***************************************************************************************/
//
// File name: AdvancedMeasurement.cpp
//
// Synopsis:  This program contains an example of various advanced measurements
//            using the measurement module.
//            See the PrintHeader() function below for detailed description.
//
//
#include <mil.h>

//****************************************************************************
// Example description.
//****************************************************************************
void PrintHeader()
   {
   MosPrintf(MIL_TEXT("[EXAMPLE NAME]\n"));
   MosPrintf(MIL_TEXT("AdvancedMeasurement\n\n"));

   MosPrintf(MIL_TEXT("[SYNOPSIS]\n"));
   MosPrintf(MIL_TEXT("This example shows how to use some controls and score functions\n"));
   MosPrintf(MIL_TEXT("to measure edges or stripes in different scenarios.\n\n"));

   MosPrintf(MIL_TEXT("[MODULES USED]\n"));
   MosPrintf(MIL_TEXT("Modules used: application, system, display, buffer, graphic, measurement.\n\n"));

   MosPrintf(MIL_TEXT("Press <Enter> to start.\n\n"));
   MosGetch();
   }

// Advanced measurement function prototypes.
void Measure(MIL_ID MilSystem, MIL_ID MilDisplay, MIL_INT ImageIndex);

void IntroMessageForMeasure0();
void IntroMessageForMeasure1();
void IntroMessageForMeasure2();
void IntroMessageForMeasure3();
void IntroMessageForMeasure4();
void IntroMessageForMeasure5();
void IntroMessageForMeasure6();
void IntroMessageForMeasure7();

void SetupForMarker0(MIL_ID MilMarker);
void SetupForMarker1(MIL_ID MilMarker);
void SetupForMarker2(MIL_ID MilMarker);
void SetupForMarker3(MIL_ID MilMarker);
void SetupForMarker4(MIL_ID MilMarker);
void SetupForMarker5(MIL_ID MilMarker);
void SetupForMarker6(MIL_ID MilMarker);
void SetupForMarker7(MIL_ID MilMarker);

void SetCalibrationMarker5(MIL_ID MilImage);

void DrawForMarker4(MIL_ID  MilOverlayImage, MIL_ID  MilMarker);
void DrawForMarker6(MIL_ID  MilOverlayImage, MIL_ID  MilMarker);
void DrawDotAtPosition(MIL_ID  MilOverlayImage, MIL_DOUBLE PositionX, MIL_DOUBLE PositionY);
void DrawLegend(MIL_ID MilOverlayImage, bool DrawGlobalEdge, bool DrawSubPositions);

void FindMarkerAndShowResults(MIL_ID  MilImage,
                              MIL_ID  MilOverlayImage,
                              MIL_ID  MilMarker,
                              MIL_INT DrawRegionType,
                              MIL_INT ImageIndex);

//****************************************************************************
// Constants.
//****************************************************************************

// Filenames.
#define EXAMPLE_IMAGE_PATH    M_IMAGE_PATH MIL_TEXT("AdvancedMeasurement/")

static const MIL_INT NB_IMAGES = 8;
static const MIL_TEXT_CHAR* IMAGE_FILENAME[NB_IMAGES] =
   {
   EXAMPLE_IMAGE_PATH MIL_TEXT("measurement0.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("measurement0.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("measurement0.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("measurement0.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("measurement4.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("measurement5.mim"),
   M_IMAGE_PATH       MIL_TEXT("lead.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("measurement7.mim")
   };

static const MIL_TEXT_CHAR* EXEMPLE_NAME[NB_IMAGES] =
   {
   MIL_TEXT("Find bottom edge"),
   MIL_TEXT("Find top edge"),
   MIL_TEXT("Find the widest stripe"),
   MIL_TEXT("Find the widest stripe subpositions"),
   MIL_TEXT("Find the stripe that contains the inclusion point"),
   MIL_TEXT("Find the evenly spaced edges"),
   MIL_TEXT("Find the stripe with a maximum association distance"),
   MIL_TEXT("Find the edge with search region clipping")
   };

// Draw colors.
static const MIL_DOUBLE POSITION_COLOR      = M_COLOR_YELLOW;
static const MIL_DOUBLE SUB_POSITIONS_COLOR = M_COLOR_GREEN;
static const MIL_DOUBLE BOX_COLOR           = M_COLOR_BLUE;
static const MIL_DOUBLE EDGES_COLOR         = M_COLOR_RED;
static const MIL_DOUBLE SINGLE_POINT_COLOR  = M_RGB888(255, 102,   0);
static const MIL_DOUBLE ERROR_COLOR         = M_COLOR_RED;

static const MIL_DOUBLE ERROR_TEXT_POS_X    = 30.0;
static const MIL_DOUBLE ERROR_TEXT_POS_Y    = 30.0;

// Search settings.                                     //   Marker 0        Marker 1        Marker 2          Marker 3          Marker 4        Marker 5       Marker 6       Marker 7
static const MIL_INT    MARKER_TYPE         [NB_IMAGES] = {    M_EDGE,         M_EDGE,       M_STRIPE,        M_STRIPE,         M_STRIPE,         M_EDGE,      M_STRIPE,        M_EDGE };
static const MIL_DOUBLE FIRST_EDGE_POLARITY [NB_IMAGES] = {     M_ANY,          M_ANY,          M_ANY,           M_ANY,            M_ANY,     M_NEGATIVE,    M_NEGATIVE,    M_POSITIVE };
static const MIL_DOUBLE SECOND_EDGE_POLARITY[NB_IMAGES] = { M_DEFAULT,      M_DEFAULT,      M_DEFAULT,       M_DEFAULT,       M_OPPOSITE,      M_DEFAULT,    M_NEGATIVE,     M_DEFAULT };
static const MIL_DOUBLE BOX_CENTER_POS_X    [NB_IMAGES] = {       388,            388,            388,             388,              244,            320,           159,           331 };
static const MIL_DOUBLE BOX_CENTER_POS_Y    [NB_IMAGES] = {       335,            335,            335,             335,              357,            247,           332,           260 };
static const MIL_DOUBLE BOX_SIZE_X          [NB_IMAGES] = {       261,            261,            261,             261,              459,            503,           190,           485 };
static const MIL_DOUBLE BOX_SIZE_Y          [NB_IMAGES] = {       572,            572,            572,             572,               63,             82,           250,           156 };
static const MIL_DOUBLE BOX_ANGLE           [NB_IMAGES] = {        90,             90,             90,              90,                0,              0,            15,           300 };
static const MIL_DOUBLE SUB_REGIONS_NUMBER  [NB_IMAGES] = { M_DEFAULT,      M_DEFAULT,      M_DEFAULT,              20,        M_DEFAULT,      M_DEFAULT,     M_DEFAULT,     M_DEFAULT };
static const MIL_DOUBLE NB_MARKERS          [NB_IMAGES] = { M_DEFAULT,      M_DEFAULT,      M_DEFAULT,       M_DEFAULT,        M_DEFAULT,          M_ALL,     M_DEFAULT,     M_DEFAULT };
static const MIL_DOUBLE EDGEVALUE_MIN      [NB_IMAGES] = { M_DEFAULT,      M_DEFAULT,      M_DEFAULT,       M_DEFAULT,              1.5,      M_DEFAULT,     M_DEFAULT,     M_DEFAULT };

// Inclusion point setting
static const MIL_DOUBLE INCLUSION_POINT_X = 240.0;
static const MIL_DOUBLE INCLUSION_POINT_Y = 347.0;

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
      Measure(MilSystem, MilDisplay, ImageIndex);

   // Free MIL objects.
   MdispFree(MilDisplay);
   MsysFree(MilSystem);
   MappFree(MilApplication);

   return 0;
   }

//***************************************************************************
// Create and setup a marker to measure edges or stripes markers, in each
// image, using score functions.
//***************************************************************************
void Measure(MIL_ID MilSystem, MIL_ID MilDisplay, MIL_INT ImageIndex)
   {
   MosPrintf(MIL_TEXT("=========================================================\n")
             MIL_TEXT("%d - %s\n")
             MIL_TEXT("=========================================================\n\n"),
             ImageIndex,
             EXEMPLE_NAME[ImageIndex]);

   switch (ImageIndex)
      {
      case 0: IntroMessageForMeasure0(); break;
      case 1: IntroMessageForMeasure1(); break;
      case 2: IntroMessageForMeasure2(); break;
      case 3: IntroMessageForMeasure3(); break;
      case 4: IntroMessageForMeasure4(); break;
      case 5: IntroMessageForMeasure5(); break;
      case 6: IntroMessageForMeasure6(); break;
      case 7: IntroMessageForMeasure7(); break;
      }

   // Restore and display the source image.
   MIL_ID MilImage = MbufRestore(IMAGE_FILENAME[ImageIndex], MilSystem, M_NULL);
   MdispSelect(MilDisplay, MilImage);

   // Prepare for overlay annotation.
   MdispControl(MilDisplay, M_OVERLAY, M_ENABLE);
   MIL_ID MilOverlayImage = MdispInquire(MilDisplay, M_OVERLAY_ID, M_NULL);

   // Allocate edge or stripe marker and setup search region and polarity.
   MIL_ID MilMarker = MmeasAllocMarker(MilSystem, MARKER_TYPE[ImageIndex], M_DEFAULT, M_NULL);

   MmeasSetMarker(MilMarker, M_POLARITY          , FIRST_EDGE_POLARITY[ImageIndex], SECOND_EDGE_POLARITY[ImageIndex]);
   MmeasSetMarker(MilMarker, M_BOX_CENTER        , BOX_CENTER_POS_X   [ImageIndex], BOX_CENTER_POS_Y[ImageIndex]    );
   MmeasSetMarker(MilMarker, M_BOX_SIZE          , BOX_SIZE_X         [ImageIndex], BOX_SIZE_Y[ImageIndex]          );
   MmeasSetMarker(MilMarker, M_BOX_ANGLE         , BOX_ANGLE          [ImageIndex], M_NULL                          );
   MmeasSetMarker(MilMarker, M_SUB_REGIONS_NUMBER, SUB_REGIONS_NUMBER [ImageIndex], M_NULL                          );
   MmeasSetMarker(MilMarker, M_NUMBER            , NB_MARKERS         [ImageIndex], M_NULL                          );
   MmeasSetMarker(MilMarker, M_EDGEVALUE_MIN     , EDGEVALUE_MIN     [ImageIndex], M_NULL                          );

   // Measure and print results.
   FindMarkerAndShowResults(MilImage, MilOverlayImage, MilMarker, M_MARKER, ImageIndex);

   // Pause to show results.
   MosPrintf(MIL_TEXT("Press <Enter> to measure again, using custom settings.\n\n"));
   MosGetch();

   // Clear the overlay image.
   MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);

   MosPrintf(MIL_TEXT("----------\n\n"));

   // Set up controls and score functions that are specific to each marker.
   switch (ImageIndex)
      {
      case 0: SetupForMarker0(MilMarker); break;
      case 1: SetupForMarker1(MilMarker); break;
      case 2: SetupForMarker2(MilMarker); break;
      case 3: SetupForMarker3(MilMarker); break;
      case 4: SetupForMarker4(MilMarker); break;
      case 5: SetupForMarker5(MilMarker); break;
      case 6: SetupForMarker6(MilMarker); break;
      case 7: SetupForMarker7(MilMarker); break;
      }

   // Measure and print results.
   FindMarkerAndShowResults(MilImage, MilOverlayImage, MilMarker, M_RESULT, ImageIndex);

   // Pause to show results.
   if (ImageIndex < NB_IMAGES-1)
      MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n\n"));
   else
      MosPrintf(MIL_TEXT("Press <Enter> to end.\n\n"));
   MosGetch();

   // Clear the overlay image.
   MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);

   // Free all allocations.
   MmeasFree(MilMarker);
   MbufFree(MilImage);
   }

//***************************************************************************
// Print explanations for marker 0.
//***************************************************************************
void IntroMessageForMeasure0()
   {
   MosPrintf(MIL_TEXT("[Objective]\n")
             MIL_TEXT("Find the bottom edge of the bacon.\n\n"));

   MosPrintf(MIL_TEXT("[Search with default values]\n")
             MIL_TEXT("The found marker (in yellow) is the edge with the greatest score.\n")
             MIL_TEXT("The edge is not found at the expected position.\n\n"));
   }

//***************************************************************************
// Specific marker setup 0.
//***************************************************************************
void SetupForMarker0(MIL_ID MilMarker)
   {
   MosPrintf(MIL_TEXT("[Search with modified marker]\n"));
   MosPrintf(MIL_TEXT("To get the bottom edge, the following settings are used:\n\n")
             MIL_TEXT("   M_STRENGTH_SCORE\n")
             MIL_TEXT("      This removes the strength criterion (it is replaced by a flat function).\n")
             MIL_TEXT("   M_POSITION_IN_MARKER_SCORE\n")
             MIL_TEXT("      This adds a position criterion, to select the edge closest to\n")
             MIL_TEXT("      the box origin.\n")
             MIL_TEXT("      (a decreasing ramp function is used).\n\n"));

   // Remove strength score function
   MmeasSetScore(MilMarker, M_STRENGTH_SCORE          , 0, 0, M_MAX_POSSIBLE_VALUE, M_MAX_POSSIBLE_VALUE, M_DEFAULT, M_DEFAULT, M_DEFAULT);

   // Set the position score function to find the bottom edge
   MmeasSetScore(MilMarker, M_POSITION_IN_MARKER_SCORE, 0, 0, 0                   , M_MAX_POSSIBLE_VALUE, M_DEFAULT, M_DEFAULT, M_DEFAULT);
   }

//***************************************************************************
// Print explanations for marker 1.
//***************************************************************************
void IntroMessageForMeasure1()
   {
   MosPrintf(MIL_TEXT("[Objective]\n")
             MIL_TEXT("Find the top edge of the bacon.\n\n"));

   MosPrintf(MIL_TEXT("[Search with default values]\n")
             MIL_TEXT("The found marker (in yellow) is the edge with the greatest score.\n")
             MIL_TEXT("The edge is not found at the expected position.\n\n"));
   }

//***************************************************************************
// Specific marker setup 1.
//***************************************************************************
void SetupForMarker1(MIL_ID MilMarker)
   {
   MosPrintf(MIL_TEXT("[Search with modified maker]\n"));
   MosPrintf(MIL_TEXT("To get the top edge, the following settings are used:\n\n")
             MIL_TEXT("   M_STRENGTH_SCORE\n")
             MIL_TEXT("      This removes the strength criterion (it is replaced by a flat function).\n")
             MIL_TEXT("   M_POSITION_IN_MARKER_SCORE\n")
             MIL_TEXT("      This adds a position criterion, to select the edge closest to\n")
             MIL_TEXT("      the box origin.\n")
             MIL_TEXT("      (an increasing ramp function is used).\n\n"));

   // Remove strength score function.
   MmeasSetScore(MilMarker, M_STRENGTH_SCORE          , 0, 0                   , M_MAX_POSSIBLE_VALUE, M_MAX_POSSIBLE_VALUE, M_DEFAULT, M_DEFAULT, M_DEFAULT);

   // Set the position score function to find the top edge.
   MmeasSetScore(MilMarker, M_POSITION_IN_MARKER_SCORE, 0, M_MAX_POSSIBLE_VALUE, M_MAX_POSSIBLE_VALUE, M_MAX_POSSIBLE_VALUE, M_DEFAULT, M_DEFAULT, M_DEFAULT);
   }

//***************************************************************************
// Print explanations for marker 2.
//***************************************************************************
void IntroMessageForMeasure2()
   {
   MosPrintf(MIL_TEXT("[Objective]\n")
             MIL_TEXT("Find the widest stripe of the bacon.\n\n"));

   MosPrintf(MIL_TEXT("[Search with default values]\n")
             MIL_TEXT("The found marker (in yellow) is composed of the two edges with\n")
             MIL_TEXT("the greatest scores.\n")
             MIL_TEXT("The stripe is not found at the expected position.\n\n"));
   }

//***************************************************************************
// Specific marker setup 2.
//***************************************************************************
void SetupForMarker2(MIL_ID MilMarker)
   {
   MosPrintf(MIL_TEXT("To get the widest possible stripe, the following settings are used:\n\n")
             MIL_TEXT("   M_STRENGTH_SCORE\n")
             MIL_TEXT("      This removes the strength criterion (it is replaced by a flat function).\n")
             MIL_TEXT("   M_STRIPE_WIDTH_SCORE\n")
             MIL_TEXT("      This adds a width criterion, which chooses the widest stripe.\n")
             MIL_TEXT("      (an increasing ramp function is used).\n\n"));

   // Remove strength score function.
   MmeasSetScore(MilMarker, M_STRENGTH_SCORE, 0, 0                   , M_MAX_POSSIBLE_VALUE, M_MAX_POSSIBLE_VALUE, M_DEFAULT, M_DEFAULT, M_DEFAULT);

   // Set the width score function to find the widest stripe.
   MmeasSetScore(MilMarker, M_STRIPE_WIDTH_SCORE   , 0, M_MAX_POSSIBLE_VALUE, M_MAX_POSSIBLE_VALUE, M_MAX_POSSIBLE_VALUE, M_DEFAULT, M_DEFAULT, M_DEFAULT);
   }

//***************************************************************************
// Print explanations for marker 3.
//***************************************************************************
void IntroMessageForMeasure3()
   {
   MosPrintf(MIL_TEXT("[Objective]\n")
             MIL_TEXT("Find the widest stripe subpositions of the bacon.\n\n"));

   MosPrintf(MIL_TEXT("[Search with default values]\n")
             MIL_TEXT("The number of subregions is set to 20.\n")
             MIL_TEXT("The subedges (in green) are the ones with maximal edge scores.\n")
             MIL_TEXT("The found stripe's subpositions are not as expected.\n\n"));
   }

//***************************************************************************
// Specific marker setup 3.
//***************************************************************************
void SetupForMarker3(MIL_ID MilMarker)
   {
   MosPrintf(MIL_TEXT("To get the widest possible stripe, the following settings are used:\n\n")
             MIL_TEXT("   M_STRENGTH_SCORE\n")
             MIL_TEXT("      This removes the strength criterion (it is replaced by a flat function).\n")
             MIL_TEXT("   M_STRIPE_WIDTH_SCORE\n")
             MIL_TEXT("      This adds a width criterion, which chooses the widest stripe.\n")
             MIL_TEXT("      (an increasing ramp function is used).\n\n"));

   // Remove strength score function.
   MmeasSetScore(MilMarker, M_STRENGTH_SCORE, 0, 0                   , M_MAX_POSSIBLE_VALUE, M_MAX_POSSIBLE_VALUE, M_DEFAULT, M_DEFAULT, M_DEFAULT);

   // Set the width score function to find the widest stripe.
   MmeasSetScore(MilMarker, M_STRIPE_WIDTH_SCORE   , 0, M_MAX_POSSIBLE_VALUE, M_MAX_POSSIBLE_VALUE, M_MAX_POSSIBLE_VALUE, M_DEFAULT, M_DEFAULT, M_DEFAULT);
   }

//***************************************************************************
// Print explanations for marker 4.
//***************************************************************************
void IntroMessageForMeasure4()
   {
   MosPrintf(MIL_TEXT("[Objective]\n")
             MIL_TEXT("Find the white bead containing the orange dot.\n\n"));

   MosPrintf(MIL_TEXT("[Search with default values]\n")
             MIL_TEXT("The found marker (in yellow) is composed of the two edges with\n")
             MIL_TEXT("the greatest scores.\n")
             MIL_TEXT("The found stripe is not as expected.\n\n"));
   }

//***************************************************************************
// Specific marker setup 4.
//***************************************************************************
void SetupForMarker4(MIL_ID MilMarker)
   {
   MosPrintf(MIL_TEXT("To perform the correct measurements, the following settings are used:\n\n")
             MIL_TEXT("   M_INCLUSION_POINT\n")
             MIL_TEXT("      This value specifies the point that must be included in\n")
             MIL_TEXT("      the result (orange dot).\n")
             MIL_TEXT("   M_INCLUSION_POINT_INSIDE_STRIPE (set to M_YES)\n")
             MIL_TEXT("      This forces to return a stripe containing the inclusion point\n")
             MIL_TEXT("      (M_INCLUSION_POINT).\n")
             MIL_TEXT("   M_STRENGTH_SCORE\n")
             MIL_TEXT("      This removes the strength criterion (it is replaced by a flat function).\n")
             MIL_TEXT("   M_STRIPE_WIDTH_SCORE\n")
             MIL_TEXT("      This adds a width criterion, which chooses the smallest stripe.\n")
             MIL_TEXT("      (a decreasing ramp function is used).\n\n"));


   // Set and enable the inclusion point to find the white stripe.
   MmeasSetMarker(MilMarker, M_INCLUSION_POINT              , INCLUSION_POINT_X, INCLUSION_POINT_Y);
   MmeasSetMarker(MilMarker, M_INCLUSION_POINT_INSIDE_STRIPE, M_YES, M_NULL);

   // Remove strength score function.
   MmeasSetScore(MilMarker, M_STRENGTH_SCORE, 0, 0, M_MAX_POSSIBLE_VALUE, M_MAX_POSSIBLE_VALUE, M_DEFAULT, M_DEFAULT, M_DEFAULT);

   // Set the width score function to find the stripe with the smallest width.
   MmeasSetScore(MilMarker, M_STRIPE_WIDTH_SCORE   , 0, 0, 0                   , M_MAX_POSSIBLE_VALUE, M_DEFAULT, M_PIXEL  , M_DEFAULT);
   }

//***************************************************************************
// Additional drawings for marker 4.
//***************************************************************************
void DrawResultForMarker4(MIL_ID MilOverlayImage, MIL_ID  MilMarker)
   {
   // Draw the point
   DrawDotAtPosition(MilOverlayImage, INCLUSION_POINT_X, INCLUSION_POINT_Y);
   }

//***************************************************************************
// Print explanations for marker 5.
//***************************************************************************
void IntroMessageForMeasure5()
   {
   MosPrintf(MIL_TEXT("[Objective]\n")
             MIL_TEXT("Find the evenly spaced edges by 1/4\".\n\n"));

   MosPrintf(MIL_TEXT("[Search with default values]\n")
             MIL_TEXT("The expected number of edges is M_ALL.\n")
             MIL_TEXT("With these settings, all the edges in the search region are found,\n")
             MIL_TEXT("however they do not respect the expected spacing.\n\n"));
   }

//***************************************************************************
// Specific marker setup 5.
//***************************************************************************
void SetupForMarker5(MIL_ID MilMarker)
   {
   MosPrintf(MIL_TEXT("To perform the correct measurements, the following settings are used:\n\n")
             MIL_TEXT("   M_STRENGTH_SCORE\n")
             MIL_TEXT("      This removes the strength criterion (it is replaced by a flat function).\n")
             MIL_TEXT("   M_SPACING_SCORE with M_WORLD input units\n")
             MIL_TEXT("      This adds a spacing criterion, which chooses the correctly spaced edges\n")
             MIL_TEXT("      (bandpass function).\n")
             MIL_TEXT("   Note that McalUniform was used to set a scale only calibration.\n")
             MIL_TEXT("   This allows for a spacing criterion in world input units\n\n"));

   MIL_DOUBLE ExpectedSpacing = 1.0 / 4.0;
   MIL_DOUBLE DeltaSpacing = 0.01;

   // Remove strength score function.
   MmeasSetScore(MilMarker, M_STRENGTH_SCORE, 0, 0, M_MAX_POSSIBLE_VALUE, M_MAX_POSSIBLE_VALUE, M_DEFAULT, M_DEFAULT, M_DEFAULT);

   // Set the spacing score function to find the edges spaced by 1/4".
   MmeasSetScore(MilMarker, M_SPACING_SCORE, ExpectedSpacing - DeltaSpacing, ExpectedSpacing - DeltaSpacing, ExpectedSpacing + DeltaSpacing, ExpectedSpacing + DeltaSpacing, M_DEFAULT, M_WORLD, M_DEFAULT);
   }

//***************************************************************************
// Set calibration for marker 5.
//***************************************************************************
void SetCalibrationMarker5(MIL_ID MilImage)
   {
   MIL_DOUBLE NbPixelsPerInch = 473.0;
   MIL_DOUBLE PixelScale = (1.0 / NbPixelsPerInch);
   McalUniform(MilImage, 0.0, 0.0, PixelScale, PixelScale, 0.0, M_DEFAULT);
   }

//***************************************************************************
// Print explanations for marker 6.
//***************************************************************************
void IntroMessageForMeasure6()
   {
   MosPrintf(MIL_TEXT("[Objective]\n")
             MIL_TEXT("Find the stripe and associate the correct subedges\n")
             MIL_TEXT("using M_MAX_ASSOCIATION_DISTANCE.\n\n"));

   MosPrintf(MIL_TEXT("[Search with default values]\n")
             MIL_TEXT("The found marker (in yellow) is composed of the two edges with\n")
             MIL_TEXT("the greatest scores.\n")
             MIL_TEXT("The subedges (in green) are the ones that maximize the score\n")
             MIL_TEXT("in each subregion.\n")
             MIL_TEXT("One subedge per region is associated to each edges (orange dots).\n")
             MIL_TEXT("The subedges are used to compute the edge's line equation and the\n")
             MIL_TEXT("edge's line angle (in red).\n")
             MIL_TEXT("The first and second edges lines always cross their positions.\n")
             MIL_TEXT("The edges positions (orange dots) are correct, however their associated\n")
             MIL_TEXT("subedges are not. Subsequently, the edges don't have a correct angle.\n\n"));
   }

//***************************************************************************
// Specific marker setup 6.
//***************************************************************************
void SetupForMarker6(MIL_ID MilMarker)
   {
   MosPrintf(MIL_TEXT("To get the correct angle for the exterior edges, the following setting is used:\n\n")
             MIL_TEXT("   M_MAX_ASSOCIATION_DISTANCE\n")
             MIL_TEXT("      This forces the maximum allowed distance from the found exterior edges\n")
             MIL_TEXT("      (orange dots) to the associated subedges (in green).\n\n"));

   MIL_DOUBLE MaxAssociationDistance = 5.0;

   // Set maximum association distance to set the correct subedges.
   MmeasSetMarker(MilMarker, M_MAX_ASSOCIATION_DISTANCE, MaxAssociationDistance, M_NULL);
   }

//***************************************************************************
// Additional drawings for marker 6.
//***************************************************************************
void DrawResultForMarker6(MIL_ID MilOverlayImage, MIL_ID MilMarker)
   {
   // Get the first edge position
   MIL_DOUBLE FirstPositionX;
   MIL_DOUBLE FirstPositionY;
   MmeasGetResultSingle(MilMarker, M_POSITION + M_EDGE_FIRST, &FirstPositionX, &FirstPositionY, 0);

   // Get the second edge position
   MIL_DOUBLE SecondPositionX;
   MIL_DOUBLE SecondPositionY;
   MmeasGetResultSingle(MilMarker, M_POSITION + M_EDGE_SECOND, &SecondPositionX, &SecondPositionY, 0);

   // Draw both edge points
   DrawDotAtPosition(MilOverlayImage,FirstPositionX , FirstPositionY);
   DrawDotAtPosition(MilOverlayImage,SecondPositionX, SecondPositionY);
   }

//***************************************************************************
// Print explanations for marker 7.
//***************************************************************************
void IntroMessageForMeasure7()
   {
   MosPrintf(MIL_TEXT("[Objective]\n")
             MIL_TEXT("Find the edge even if one or more corners of the search region\n")
             MIL_TEXT("fall outside the image limits.\n\n"));

   MosPrintf(MIL_TEXT("[Search with default values]\n"));
   MosPrintf(MIL_TEXT("All settings are left to default except the search region (in blue).\n")
             MIL_TEXT("No results are found because the search region is partly outside the image.\n\n"));
   }

//***************************************************************************
// Specific marker setup 7.
//***************************************************************************
void SetupForMarker7(MIL_ID MilMarker)
   {
   MosPrintf(MIL_TEXT("To be able to perform a measurement, the following setting is used:\n\n")
             MIL_TEXT("   M_SEARCH_REGION_CLIPPING\n")
             MIL_TEXT("      This enables the module to clip the search region within\n")
             MIL_TEXT("      the image limits.\n\n"));

   // Enable search region clipping.
   MmeasSetMarker(MilMarker, M_SEARCH_REGION_CLIPPING, M_ENABLE, M_NULL);
   }

//***************************************************************************
// Measure and print results.
//***************************************************************************
void FindMarkerAndShowResults(MIL_ID  MilImage,
                              MIL_ID  MilOverlayImage,
                              MIL_ID  MilMarker,
                              MIL_INT DrawRegionType,
                              MIL_INT ImageIndex)
   {
   switch (ImageIndex)
      {
      case 5: SetCalibrationMarker5(MilImage);
      }

   // Find the marker and compute all applicable measurements.
   MmeasFindMarker(M_DEFAULT, MilImage, MilMarker, M_DEFAULT);

   // Draw the contour of the measurement region.
   MgraColor(M_DEFAULT, BOX_COLOR);
   MmeasDraw(M_DEFAULT, MilMarker, MilOverlayImage, M_DRAW_SEARCH_REGION, M_DEFAULT, DrawRegionType);

   // Draw the search region orientation
   MmeasDraw(M_DEFAULT, MilMarker, MilOverlayImage, M_DRAW_SEARCH_DIRECTION, M_DEFAULT, DrawRegionType);

   // If occurrence is found, show the results.
   MIL_INT NumberOccurrencesFound;
   MmeasGetResult(MilMarker, M_NUMBER+M_TYPE_MIL_INT, &NumberOccurrencesFound, M_NULL);

   if (NumberOccurrencesFound >= 1)
      {
      bool DrawGlobalEdge   = false;
      bool DrawSubPositions = false;

      switch (ImageIndex)
         {
         case 0:
         case 1:
         case 2:
         case 4:
         case 5:
         case 7:
            {
            DrawGlobalEdge = true;
            }
            break;

         case 3:
            {
            DrawSubPositions = true;
            }
            break;

         case 6:
            {
            DrawGlobalEdge   = true;
            DrawSubPositions = true;
            }
            break;
         }

      DrawLegend(MilOverlayImage, DrawGlobalEdge, DrawSubPositions);

      if(DrawGlobalEdge)
         {
         // Draw the found marker edges.
         MgraColor(M_DEFAULT, EDGES_COLOR);
         MmeasDraw(M_DEFAULT, MilMarker, MilOverlayImage, M_DRAW_EDGES, M_DEFAULT, M_RESULT);

         // Draw a cross at the found position.
         MgraColor(M_DEFAULT, POSITION_COLOR);
         MmeasDraw(M_DEFAULT, MilMarker, MilOverlayImage, M_DRAW_POSITION, M_DEFAULT, M_RESULT);
         }

      if(DrawSubPositions)
         {
         // Draw the sub edges position used to fit the marker.
         MgraColor(M_DEFAULT, SUB_POSITIONS_COLOR);
         MmeasDraw(M_DEFAULT, MilMarker, MilOverlayImage, M_DRAW_SUB_POSITIONS, M_DEFAULT, M_RESULT);
         }

      switch (ImageIndex)
         {
         case 4: DrawResultForMarker4(MilOverlayImage, MilMarker); break;
         case 6: DrawResultForMarker6(MilOverlayImage, MilMarker); break;
         }
      }
   else
      {
      // Print the result.
      MgraColor(M_DEFAULT, ERROR_COLOR);
      MgraText(M_DEFAULT, MilOverlayImage, ERROR_TEXT_POS_X, ERROR_TEXT_POS_Y, MIL_TEXT("The marker was not found."));
      }
   }

//***************************************************************************
// Draw dots at position.
//***************************************************************************
void DrawDotAtPosition(MIL_ID MilOverlayImage, MIL_DOUBLE PositionX, MIL_DOUBLE PositionY)
   {
   MgraColor(M_DEFAULT, SINGLE_POINT_COLOR);
   MgraArcFill(M_DEFAULT, MilOverlayImage, PositionX, PositionY, 3, 3, 0, 360);
   }

//***************************************************************************
// Draw the example legend.
//***************************************************************************
void DrawLegend(MIL_ID MilOverlayImage, bool DrawGlobalEdge, bool DrawSubPositions)
   {
   MIL_DOUBLE TextPosX = 10.0;
   MIL_DOUBLE TextPosY = 10.0;
   MIL_DOUBLE StepY  = 15.0;

   MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
   MgraColor(M_DEFAULT, BOX_COLOR);
   MgraText(M_DEFAULT, MilOverlayImage, TextPosX, TextPosY, MIL_TEXT("Measurement Search Region - Blue"));
   TextPosY += StepY;

   if(DrawGlobalEdge)
      {
      MgraColor(M_DEFAULT, POSITION_COLOR);
      MgraText(M_DEFAULT, MilOverlayImage, TextPosX, TextPosY, MIL_TEXT("Marker Position           - Yellow"));
      TextPosY += StepY;

      MgraColor(M_DEFAULT, EDGES_COLOR);
      MgraText(M_DEFAULT, MilOverlayImage, TextPosX, TextPosY, MIL_TEXT("Edges                     - Red"));
      TextPosY += StepY;
      }

   if(DrawSubPositions)
      {
      MgraColor(M_DEFAULT, SUB_POSITIONS_COLOR);
      MgraText(M_DEFAULT, MilOverlayImage, TextPosX, TextPosY, MIL_TEXT("Subedges Positions        - Green"));
      TextPosY += StepY;
      }
   }