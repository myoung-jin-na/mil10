//*************************************************************************************
//
// File name: MultiViewAnnotations.cpp
//
// Synopsis:  This program contains an example of advanced graphic annotations
//            when using an application that has multiple camera views. 
//            It also demonstrates the fixturing capabilities and the use
//            of McalWrap.
//            See the PrintHeader() function below for details.
//

#include <mil.h>
#include <map>
#include <math.h>

using namespace std;

//***************************************************************************
// Example description.
//***************************************************************************
void PrintHeader()
   {
   MosPrintf(MIL_TEXT("[EXAMPLE NAME]\n"));
   MosPrintf(MIL_TEXT("MultiViewAnnotations\n\n"));
   MosPrintf(MIL_TEXT("[SYNOPSIS]\n"));
   MosPrintf(MIL_TEXT("This example shows the capabilities of advanced graphic annotations\n")
             MIL_TEXT("when using an application that has multiple camera views. The resulting\n")
             MIL_TEXT("subpixel annotations are drawn using the corresponding color of the most\n")
             MIL_TEXT("suitable camera view with which to perform the processing operation.\n")
             MIL_TEXT("Annotations can be panned, zoomed, and seen from any other camera point\n")
             MIL_TEXT("of view by interactively selecting a different camera.\n\n"));

   MosPrintf(MIL_TEXT("[MODULES USED]\n"));
   MosPrintf(MIL_TEXT("Application, system, display, buffer, graphic, \n")
             MIL_TEXT("image processing, calibration, model finder, \n")
             MIL_TEXT("measurement, metrology, ocr.\n\n"));
   }


// Macros
#define EXAMPLE_IMAGE_PATH M_IMAGE_PATH MIL_TEXT("MultiViewAnnotations/")

// Constants
static MIL_CONST_TEXT_PTR IMAGE1       = EXAMPLE_IMAGE_PATH MIL_TEXT("ImageView1.mim");
static MIL_CONST_TEXT_PTR IMAGE2       = EXAMPLE_IMAGE_PATH MIL_TEXT("ImageView2.mim");
static MIL_CONST_TEXT_PTR IMAGE3       = EXAMPLE_IMAGE_PATH MIL_TEXT("ImageView3.mim");
static MIL_CONST_TEXT_PTR IMAGE4       = EXAMPLE_IMAGE_PATH MIL_TEXT("ImageView4.mim");

static MIL_CONST_TEXT_PTR IMAGESKETCH  = EXAMPLE_IMAGE_PATH MIL_TEXT("Sketch.mim");

static MIL_CONST_TEXT_PTR IMAGEGRID1   = EXAMPLE_IMAGE_PATH MIL_TEXT("CalGrid1.mim");
static MIL_CONST_TEXT_PTR IMAGEGRID2   = EXAMPLE_IMAGE_PATH MIL_TEXT("CalGrid2.mim");
static MIL_CONST_TEXT_PTR IMAGEGRID3   = EXAMPLE_IMAGE_PATH MIL_TEXT("CalGrid3.mim");
static MIL_CONST_TEXT_PTR IMAGEGRID4   = EXAMPLE_IMAGE_PATH MIL_TEXT("CalGrid4.mim");

static MIL_CONST_TEXT_PTR IMAGEGRID3D1 = EXAMPLE_IMAGE_PATH MIL_TEXT("CalGrid1AtAngle.mim");

static MIL_CONST_TEXT_PTR MODCONTEXT   = EXAMPLE_IMAGE_PATH MIL_TEXT("ModelFinderContext.mmf");
static MIL_CONST_TEXT_PTR MEASCONTEXT  = EXAMPLE_IMAGE_PATH MIL_TEXT("MeasMarker.edg");
static MIL_CONST_TEXT_PTR OCRCONTEXT   = EXAMPLE_IMAGE_PATH MIL_TEXT("OCR_Context.mfo");
static MIL_CONST_TEXT_PTR METCONTEXT   = EXAMPLE_IMAGE_PATH MIL_TEXT("MetrologyContext.met");

static const MIL_DOUBLE NB_PIXELS_PER_DEGREE    = 10.0; // in pix / deg
static const MIL_DOUBLE NB_PIXELS_PER_RADIAL_MM = 30.0; // in pix / mm
static const MIL_DOUBLE TO_RAD                  = 3.14159265358989 / 180.0;
static const MIL_INT    NB_FIXED_POINT          = 4;
static const MIL_INT    MAX_CHAR_LENGTH         = 256;
static const MIL_INT    NB_ANGLE_STEP           = 5;
static const MIL_DOUBLE ANGLE_STEP              = 60.0;


/* Calibration Grid specifications. */
#define GRID_NB_ROWS               15L
#define GRID_NB_COLUMNS            16L
#define GRID_ROW_SPACING           5L
#define GRID_COLUMN_SPACING        5L
#define GRID_THICKNESS             2L

/* Images specifications. */
#define SRC_IMAGE_WIDTH            920L
#define SRC_IMAGE_HEIGHT           920L
#define DISPLAYED_IMAGE_WIDTH      512L
#define DISPLAYED_IMAGE_HEIGHT     512L
#define OFFSET_BETWEEN_IMAGES      6L

static const MIL_DOUBLE SRC_TO_DISP_ZOOM = (MIL_DOUBLE)DISPLAYED_IMAGE_WIDTH / (MIL_DOUBLE)SRC_IMAGE_WIDTH;

/* Selectable button positions. */
#define BUTTON1_POSX              234L
#define BUTTON1_POSY              150L
#define BUTTON2_POSX              389L
#define BUTTON2_POSY              328L
#define BUTTON3_POSX              345L
#define BUTTON3_POSY              213L
#define BUTTON4_POSX              68L
#define BUTTON4_POSY              310L

/* Selectable button radius. */
#define BUTTON_RADIUS              11L

/* Child specifications. */
#define CHILD_OFFSETX              146L
#define CHILD_OFFSETY              90L
#define CHILD_SIZEX                495L
#define CHILD_SIZEY                617L

#define STRING_LENGTH              16L

// Function declarations.
void ShowMultiViewAnnotations(MIL_ID MilSystem, MIL_ID MilDisplay1, MIL_ID MilDisplay2);

void CreateCalibration(MIL_ID MilSystem, MIL_ID& MilCalibration1, MIL_ID& MilCalibration2,
                       MIL_ID& MilCalibration3, MIL_ID& MilCalibration4);

void AddSelectableButton(MIL_ID MilGraphicContext, MIL_ID MilGraphicList, MIL_ID MilImage,
                         MIL_INT PosX, MIL_INT PosY,
                         map<MIL_INT, MIL_ID>& MapImageToDisplay,
                         map<MIL_INT, MIL_DOUBLE>& MapInitialZoom, MIL_INT* pRegionLabel);

void InitCameraViewDisplay(MIL_ID MilGraphicContextCameraView,
                           MIL_ID MilGraphicListCameraView, MIL_ID MilGraphicListSketch,
                           MIL_INT GlobalRegionLabel, bool ShowCalibrationAxis);

void ModelFinderProcess(MIL_ID MilSystem, MIL_ID MilGraphicContext, MIL_ID MilGraphicList,
                        MIL_ID MilImage, MIL_ID& MilModResult);

void MeasProcess(MIL_ID MilSystem, MIL_ID MilGraphicContext, MIL_ID MilGraphicList,
                 MIL_ID MilImage, MIL_DOUBLE& CircleCenterX,MIL_DOUBLE& CircleCenterY,
                 MIL_DOUBLE& CircleRadius);

void MetrologyProcess(MIL_ID MilSystem, MIL_ID MilGraphicContext, MIL_ID MilGraphicList,
                      const map<MIL_ID,MIL_DOUBLE>& ImageToProcessMapColor,
                      MIL_DOUBLE PosX, MIL_DOUBLE PosY);

bool MetrologyProcessStep(MIL_ID MilMetContext, MIL_ID MilMetResult,
                          MIL_INT ToleranceIndex, MIL_DOUBLE Angle, MIL_ID MilImage);

void CreatePolarImage(MIL_ID MilSystem, MIL_ID MilImageSource, MIL_ID& MilPolarImage,
                      MIL_ID& WrapCal,MIL_DOUBLE OffsetX, MIL_DOUBLE OffsetY,
                      MIL_DOUBLE StartAngle, MIL_DOUBLE EndAngle,
                      MIL_DOUBLE RMin, MIL_DOUBLE RMax);

void CreateLUTs(MIL_ID MilSystem, MIL_ID MilCalibration, MIL_ID* pMilLutX,
                MIL_ID* pMilLutY, MIL_DOUBLE OffsetX, MIL_DOUBLE OffsetY,
                MIL_DOUBLE StartAngle, MIL_DOUBLE EndAngle,
                MIL_DOUBLE MinRadius, MIL_DOUBLE MaxRadius);

void OCRProcess(MIL_ID MilSystem, MIL_ID MilGraphicContext, MIL_ID MilGraphicList,
                MIL_ID MilImage);

MIL_INT MFTYPE HookHandler(MIL_INT HookType, MIL_ID EventId, void* UserDataPtr);


/* Data structure to handle parameters for the hook function. */
struct SHookUserData
   {
   MIL_ID  MilDisplay;

   const map<MIL_INT, MIL_ID>*     pMapImageToDisplay;
   const map<MIL_INT, MIL_DOUBLE>* pMapInitialZoom;
   };

//****************************************************************************
// Main.
//****************************************************************************
int MosMain(void)
   {
   MIL_ID MilApplication,        /* Application identifier.    */
          MilSystem,             /* System identifier.         */
          MilDisplay1,           /* First display identifier.  */
          MilDisplay2;           /* Second display identifier. */

   /* Allocate MIL objects. */
   MappAlloc(M_NULL, M_DEFAULT, &MilApplication);
   MsysAlloc(M_DEFAULT, M_SYSTEM_HOST, M_DEFAULT, M_DEFAULT, &MilSystem);

   /* Allocate one display for the set-up view. */
   MdispAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &MilDisplay1);
   MdispControl(MilDisplay1, M_TITLE, M_PTR_TO_DOUBLE(MIL_TEXT("Set-up")));

   /* Allocate another display for the camera view. */
   MdispAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &MilDisplay2);
   MdispControl(MilDisplay2, M_TITLE, M_PTR_TO_DOUBLE(MIL_TEXT("Camera View")));

   /* Print example description. */
   PrintHeader();

   /* Run the example. */
   ShowMultiViewAnnotations(MilSystem, MilDisplay1, MilDisplay2);

   /* Free MIL objects. */
   MdispFree(MilDisplay1);
   MdispFree(MilDisplay2);
   MsysFree(MilSystem);
   MappFree(MilApplication);

   return 0;
   }

//****************************************************************************************
// Perform processing in one view and display the resulting annotations
// in all other views.
//****************************************************************************************
void ShowMultiViewAnnotations(MIL_ID MilSystem, MIL_ID MilDisplay1, MIL_ID MilDisplay2)
   {
   MIL_ID MilImage1,         /* First view image buffer identifier.                      */
          MilImage2,         /* Second view image buffer identifier.                     */
          MilImage3,         /* Third view image buffer identifier.                      */
          MilImage4,         /* Fourth view image buffer identifier.                     */
          MilImage4Child,    /* Child image buffer identifier.                           */
          MilCorrectedImage4,/* Corrected image buffer identifier.                       */
          MilPolarImage,     /* Polar transform image buffer identifier.                 */
          MilCalibration1,   /* Calibration context identifier for the first view image. */
          MilCalibration2,   /* Calibration context identifier for the second view image.*/
          MilCalibration3,   /* Calibration context identifier for the third view image. */
          MilCalibration4,   /* Calibration context identifier for the fourth view image.*/
          MilWrapCal1,       /* Calibration context identifier for polar transform image.*/
          MilImageSketch,    /* Sketch image buffer identifier.                          */
          MilGraphicList1,   /* Graphics list identifier for the sketch image.           */
          MilGraphicContext1,/* Graphics context identifier for the sketch image.        */
          MilGraphicList2,   /* Graphics list identifier for the camera view image.      */
          MilGraphicContext2,/* Graphics context identifier for the camera view image.   */
          MilModResult;      /* Model finder result buffer identifier.                   */

   MIL_DOUBLE CircleCenterX, /* X-coordinate of the measured circle.                     */
              CircleCenterY, /* Y-coordinate of the measured circle.                     */
              CircleRadius;  /* Radius coordinate of the measured circle.                */

   MIL_INT    RegionLabel,       /* Label value of the region.                           */
              GlobalRegionLabel; /* Label value of the global view region.               */

   map<MIL_ID, MIL_DOUBLE> ImageToProcessMapColor; /* Images and color map.              */
   map<MIL_INT, MIL_ID> MapImageToDisplay;         /* Graphic labels and images map.     */
   map<MIL_INT, MIL_DOUBLE> MapInitialZoom;/* Graphic labels and initial zoom value map. */

   SHookUserData DataStructure;          /* Hook function data structure.                */

   MosPrintf(MIL_TEXT("\nCalibration process, please wait"));

   /* Restore the camera view images. */
   MbufRestore(IMAGE1, MilSystem, &MilImage1);
   MbufRestore(IMAGE2, MilSystem, &MilImage2);
   MbufRestore(IMAGE3, MilSystem, &MilImage3);
   MbufRestore(IMAGE4, MilSystem, &MilImage4);

   /* Create the calibration context for each camera view image. */
   CreateCalibration(MilSystem, MilCalibration1, MilCalibration2, MilCalibration3,
                     MilCalibration4);

   MosPrintf(MIL_TEXT("Done!\n\n"));

   /* Associate calibrations to the camera view images. */
   McalAssociate(MilCalibration1, MilImage1, M_DEFAULT);
   McalAssociate(MilCalibration2, MilImage2, M_DEFAULT);
   McalAssociate(MilCalibration3, MilImage3, M_DEFAULT);
   McalAssociate(MilCalibration4, MilImage4, M_DEFAULT);

   /* Restore the sketch image. */
   MbufRestore(IMAGESKETCH, MilSystem, &MilImageSketch);

   /* Disable zoom control and window resizing control for sketch image display. */
   MdispControl(MilDisplay1, M_WINDOW_RESIZE, M_DISABLE);
   MdispControl(MilDisplay1, M_MOUSE_USE, M_DISABLE);
   MdispControl(MilDisplay1, M_KEYBOARD_USE, M_DISABLE);

   /* Set initial zoom value and initial position for the camera view image display. */
   MdispZoom(MilDisplay2, SRC_TO_DISP_ZOOM, SRC_TO_DISP_ZOOM);
   MdispControl(MilDisplay2, M_WINDOW_INITIAL_POSITION_X, DISPLAYED_IMAGE_WIDTH + OFFSET_BETWEEN_IMAGES);

   /* Associate images to displays. */
   MdispSelect(MilDisplay1, MilImageSketch);
   MdispSelect(MilDisplay2, MilImage1);

   /* Allocate graphic list objects. */
   MgraAllocList(MilSystem, M_DEFAULT, &MilGraphicList1);
   MgraAlloc(MilSystem, &MilGraphicContext1);
  
   MgraAllocList(MilSystem, M_DEFAULT, &MilGraphicList2);
   MgraAlloc(MilSystem, &MilGraphicContext2);

   /* Associate the graphic list to the display for annotations. */
   MdispControl(MilDisplay1, M_ASSOCIATED_GRAPHIC_LIST_ID, MilGraphicList1);
   MdispControl(MilDisplay2, M_ASSOCIATED_GRAPHIC_LIST_ID, MilGraphicList2);

   /* Enable the interactive mode. */
   MdispControl(MilDisplay1, M_GRAPHIC_LIST_INTERACTIVE, M_ENABLE);

   /* Disable the editable mode for the next annotations to the graphics list. */
   MgraControl(MilGraphicContext1, M_EDITABLE, M_DISABLE);

   /* Add selectable circular regions. */
   AddSelectableButton(MilGraphicContext1, MilGraphicList1, MilImage1, BUTTON1_POSX,
                       BUTTON1_POSY, MapImageToDisplay, MapInitialZoom, &RegionLabel);
   GlobalRegionLabel = RegionLabel;
   AddSelectableButton(MilGraphicContext1, MilGraphicList1, MilImage2, BUTTON2_POSX,
                       BUTTON2_POSY, MapImageToDisplay, MapInitialZoom, &RegionLabel);
   AddSelectableButton(MilGraphicContext1, MilGraphicList1, MilImage3, BUTTON3_POSX,
                       BUTTON3_POSY, MapImageToDisplay, MapInitialZoom, &RegionLabel);
   AddSelectableButton(MilGraphicContext1, MilGraphicList1, MilImage4, BUTTON4_POSX,
                       BUTTON4_POSY, MapImageToDisplay, MapInitialZoom, &RegionLabel);

   /* Set the selectable color of the circular regions. */
   MgraControlList(MilGraphicList1, M_LIST, M_DEFAULT, M_SELECTED_COLOR,
                   M_COLOR_DARK_GREEN);
   /* Disable multiple circular region selection. */
   MgraControlList(MilGraphicList1, M_LIST, M_DEFAULT, M_MULTIPLE_SELECTION, M_DISABLE);

   /* Associate each processable image to one color. */
   ImageToProcessMapColor[MilImage1] = M_COLOR_CYAN;
   ImageToProcessMapColor[MilImage2] = M_COLOR_RED;
   ImageToProcessMapColor[MilImage3] = M_COLOR_YELLOW;
   ImageToProcessMapColor[MilImage4] = M_COLOR_MAGENTA;

   /* Set Input Unit to M_WORLD for camera view annotations. */
   MgraControl(MilGraphicContext2, M_INPUT_UNITS, M_WORLD);

   /* Initialize the hook data structure, then associate the hook function to    */
   /* the "M_GRAPHIC_SELECTION_MODIFIED" event. The hook function will be called */
   /* with any region selected by the user.                                      */
   DataStructure.MilDisplay = MilDisplay2;
   DataStructure.pMapImageToDisplay = &MapImageToDisplay;
   DataStructure.pMapInitialZoom = &MapInitialZoom;
  
   MgraHookFunction(MilGraphicList1, M_GRAPHIC_SELECTION_MODIFIED,
                    &HookHandler, &DataStructure);
 
   if(MdispInquire(MilDisplay1, M_DISPLAY_TYPE, M_NULL) == M_AUXILIARY)
      {
      MosPrintf(MIL_TEXT("***NOTE: This example does not implement the interactivity\n"));
      MosPrintf(MIL_TEXT("         with an auxiliary display.\n\n"));
      }

   /* Initialize the camera view display with the global view by selecting the Global region label. */
   MgraControlList(MilGraphicList1, M_GRAPHIC_LABEL(GlobalRegionLabel), M_DEFAULT, 
                   M_GRAPHIC_SELECTED, M_TRUE);

   /* Draw the relative coordinate system. */
   MgraColor(MilGraphicContext2, M_COLOR_DARK_GREEN);
   McalDraw(MilGraphicContext2, M_NULL, MilGraphicList2,M_DRAW_RELATIVE_COORDINATE_SYSTEM, 
      M_DEFAULT, M_DEFAULT);

   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   /* Reinitialize the camera view display. */
   InitCameraViewDisplay(MilGraphicContext2, MilGraphicList2, MilGraphicList1,
                         GlobalRegionLabel, true);
  
   /* Find the piece in the global view and draw its edges. */
   MgraColor(MilGraphicContext2, ImageToProcessMapColor[MilImage1]);
   ModelFinderProcess(MilSystem, MilGraphicContext2, MilGraphicList2, MilImage1,
                      MilModResult);

   MosPrintf(MIL_TEXT("The Model Finder tool locates the 2D model of an object in the\n"));
   MosPrintf(MIL_TEXT("calibration plane of the image acquired by the blue camera.\n\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   /* Reinitialize the camera view display. */
   InitCameraViewDisplay(MilGraphicContext2, MilGraphicList2, MilGraphicList1,
                         GlobalRegionLabel, true);
 
   /* Compute the corrected image from the fourth view*/
   MbufChild2d(MilImage4, CHILD_OFFSETX, CHILD_OFFSETY, CHILD_SIZEX, CHILD_SIZEY,
               &MilImage4Child);
   MilCorrectedImage4 = MbufClone(MilImage4, M_DEFAULT, M_DEFAULT, M_DEFAULT, M_DEFAULT,
                                  M_DEFAULT, M_DEFAULT, &MilCorrectedImage4);
   MbufClear(MilCorrectedImage4, 0);
   McalTransformImage(MilImage4Child, MilCorrectedImage4, M_DEFAULT, M_DEFAULT,
                      M_FULL_CORRECTION, M_DEFAULT);

   /* Apply a fixture offset to the calibration of the following images */
   McalFixture(MilImage1, M_NULL, M_MOVE_RELATIVE, M_RESULT_MOD, MilModResult, 0,
               M_DEFAULT, M_DEFAULT, M_DEFAULT);
   McalFixture(MilImage2, M_NULL, M_MOVE_RELATIVE, M_RESULT_MOD, MilModResult, 0,
               M_DEFAULT, M_DEFAULT, M_DEFAULT);
   McalFixture(MilImage3, M_NULL, M_MOVE_RELATIVE, M_RESULT_MOD, MilModResult, 0,
               M_DEFAULT, M_DEFAULT, M_DEFAULT);
   McalFixture(MilImage4, M_NULL, M_MOVE_RELATIVE, M_RESULT_MOD, MilModResult, 0,
               M_DEFAULT, M_DEFAULT, M_DEFAULT);
   McalFixture(MilCorrectedImage4, M_NULL, M_MOVE_RELATIVE, M_RESULT_MOD, MilModResult, 0,
               M_DEFAULT, M_DEFAULT, M_DEFAULT);

   /* Measure the circular region of the piece and draw it. */
   MgraColor(MilGraphicContext2, ImageToProcessMapColor[MilImage4]);
   MeasProcess(MilSystem, MilGraphicContext2, MilGraphicList2, MilCorrectedImage4,
               CircleCenterX, CircleCenterY, CircleRadius);
  
   MosPrintf(MIL_TEXT("The Measurement tool is used to establish a circle in the\n"));
   MosPrintf(MIL_TEXT("calibrated view acquired from the magenta camera.\n\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   /* Reinitialize the camera view display. */
   InitCameraViewDisplay(MilGraphicContext2, MilGraphicList2, MilGraphicList1,
                         GlobalRegionLabel, true);
    
   /* Validate hole measures on the piece and draw the results. */
   MetrologyProcess(MilSystem, MilGraphicContext2, MilGraphicList2, ImageToProcessMapColor,
                    CircleCenterX, CircleCenterY);

   /* Remove the circular distortion: compute a polar transform image and   */
   /* its associated calibration from the measured circle parameter.        */
   McalAlloc(MilSystem, M_LINEAR_INTERPOLATION, M_DEFAULT, &MilWrapCal1);
   CreatePolarImage(MilSystem, MilImage4, MilPolarImage, MilWrapCal1, CircleCenterX,
                    CircleCenterY, 20, 70, 0.5*CircleRadius, 1.1*CircleRadius);

   MosPrintf(MIL_TEXT("The Metrology tool is used to validate geometric tolerances\n"));
   MosPrintf(MIL_TEXT("in multiple calibrated views.\n\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

    /* Reinitialize the camera view display. */
   InitCameraViewDisplay(MilGraphicContext2, MilGraphicList2, MilGraphicList1,
                         GlobalRegionLabel, false);
  
   /* Read the text on the piece and draw the result. */
   MgraColor(MilGraphicContext2, ImageToProcessMapColor[MilImage4]);
   MapImageToDisplay[RegionLabel] = MilPolarImage;
   MapInitialZoom[RegionLabel] = 1.0;
   OCRProcess(MilSystem, MilGraphicContext2, MilGraphicList2, MilPolarImage);

   MosPrintf(MIL_TEXT("The OCR tool is used to read a product number in the unwarped\n"));
   MosPrintf(MIL_TEXT("calibrated view acquired from the magenta camera.\n\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();
   /* Disassociate the hook function to the "M_GRAPHIC_SELECTION_MODIFIED" event. */
   MgraHookFunction(MilGraphicList1, M_GRAPHIC_SELECTION_MODIFIED+M_UNHOOK,
                    &HookHandler, &DataStructure);

   /* Free all allocation. */
   MbufFree(MilPolarImage);
   MbufFree(MilImage4Child);
   MbufFree(MilImage1);
   MbufFree(MilImage2);
   MbufFree(MilImage3);
   MbufFree(MilImage4);
   MbufFree(MilCorrectedImage4);
   MbufFree(MilImageSketch);
   McalFree(MilCalibration1);
   McalFree(MilCalibration2);
   McalFree(MilCalibration3);
   McalFree(MilCalibration4);
   McalFree(MilWrapCal1);
   MgraFree(MilGraphicContext1);
   MgraFree(MilGraphicList1);
   MgraFree(MilGraphicContext2);
   MgraFree(MilGraphicList2);
   MmodFree(MilModResult);
   }


void CreateCalibration(MIL_ID MilSystem, MIL_ID& MilCalibration1, MIL_ID& MilCalibration2,
                       MIL_ID& MilCalibration3, MIL_ID& MilCalibration4)
   {
   MIL_ID MilImageGrid1,   /* First view calibration grid image buffer identifier.      */
          MilImageGrid2,   /* Second view calibration grid image buffer identifier.     */
          MilImageGrid3,   /* Third view calibration grid image buffer identifier.      */
          MilImageGrid4,   /* Fourth view calibration grid image buffer identifier.     */
          MilImageGrid3D1; /* First view depth calibration grid image buffer identifier.*/

   /* Allocate 3D calibration contexts. */
   McalAlloc(MilSystem, M_TSAI_BASED, M_DEFAULT, &MilCalibration1);
   McalAlloc(MilSystem, M_TSAI_BASED, M_DEFAULT, &MilCalibration2);
   McalAlloc(MilSystem, M_TSAI_BASED, M_DEFAULT, &MilCalibration3);
   McalAlloc(MilSystem, M_TSAI_BASED, M_DEFAULT, &MilCalibration4);

   /* Restore all grid images. */
   MbufRestore(IMAGEGRID1  , MilSystem, &MilImageGrid1);
   MbufRestore(IMAGEGRID3D1, MilSystem, &MilImageGrid3D1);
   MbufRestore(IMAGEGRID2  , MilSystem, &MilImageGrid2);
   MbufRestore(IMAGEGRID3  , MilSystem, &MilImageGrid3);
   MbufRestore(IMAGEGRID4  , MilSystem, &MilImageGrid4);

   /* Use the relative coordinate system as the camera's calibration plane. */
   McalControl(MilCalibration1, M_CALIBRATION_PLANE, M_RELATIVE_COORDINATE_SYSTEM);
   McalControl(MilCalibration2, M_CALIBRATION_PLANE, M_RELATIVE_COORDINATE_SYSTEM);
   McalControl(MilCalibration3, M_CALIBRATION_PLANE, M_RELATIVE_COORDINATE_SYSTEM);
   McalControl(MilCalibration4, M_CALIBRATION_PLANE, M_RELATIVE_COORDINATE_SYSTEM);

   /* Offset the relative coordinate system from the absolute coordinate system by */
   /* the height of the grid. */
   McalSetCoordinateSystem(MilCalibration1, M_RELATIVE_COORDINATE_SYSTEM,
                           M_ABSOLUTE_COORDINATE_SYSTEM, M_TRANSLATION, M_NULL, 0.0, 0.0,
                           -GRID_THICKNESS, M_DEFAULT);
   McalSetCoordinateSystem(MilCalibration2, M_RELATIVE_COORDINATE_SYSTEM,
                           M_ABSOLUTE_COORDINATE_SYSTEM, M_TRANSLATION, M_NULL, 0.0, 0.0,
                           -GRID_THICKNESS, M_DEFAULT);
   McalSetCoordinateSystem(MilCalibration3, M_RELATIVE_COORDINATE_SYSTEM,
                           M_ABSOLUTE_COORDINATE_SYSTEM, M_TRANSLATION, M_NULL, 0.0, 0.0,
                           -GRID_THICKNESS, M_DEFAULT);
   McalSetCoordinateSystem(MilCalibration4, M_RELATIVE_COORDINATE_SYSTEM,
                           M_ABSOLUTE_COORDINATE_SYSTEM, M_TRANSLATION, M_NULL, 0.0, 0.0,
                           -GRID_THICKNESS, M_DEFAULT);

   /* Calibrate each camera with the image of the grid and its world description.      */
   /*                                                                                  */
   /* For the first view the camera, the optical axis of the camera is almost          */
   /* perpendicular to the camera calibration plane. We have to process in two steps   */
   /* to perform the 3D calibration:                                                   */
   /* First calibrate using the depth calibration grid.                                */
   McalGrid(MilCalibration1, MilImageGrid3D1, M_NULL, M_NULL, M_NULL, GRID_NB_ROWS,
            GRID_NB_COLUMNS, GRID_ROW_SPACING, GRID_COLUMN_SPACING, M_DEFAULT, 
            M_CHESSBOARD_GRID);
   /* Then we calibrate with "M_DISPLACE_CAMERA_COORD" using the first view            */
   /* calibration grid as we were displacing the camera coordinate system.             */                                                  
   McalGrid(MilCalibration1, MilImageGrid1, M_NULL, M_NULL, M_NULL, GRID_NB_ROWS,
            GRID_NB_COLUMNS, GRID_ROW_SPACING, GRID_COLUMN_SPACING,
            M_DISPLACE_CAMERA_COORD,M_CHESSBOARD_GRID);

   MosPrintf(MIL_TEXT("."));

   McalGrid(MilCalibration2, MilImageGrid2, M_NULL, M_NULL, M_NULL, GRID_NB_ROWS,
            GRID_NB_COLUMNS, GRID_ROW_SPACING, GRID_COLUMN_SPACING, M_DEFAULT,
            M_CHESSBOARD_GRID);

   MosPrintf(MIL_TEXT("."));

   McalGrid(MilCalibration3, MilImageGrid3, M_NULL, M_NULL, M_NULL, GRID_NB_ROWS,
            GRID_NB_COLUMNS, GRID_ROW_SPACING, GRID_COLUMN_SPACING, M_DEFAULT, 
            M_CHESSBOARD_GRID);

   MosPrintf(MIL_TEXT("."));

   McalGrid(MilCalibration4, MilImageGrid4, M_NULL, M_NULL, M_NULL, GRID_NB_ROWS,
            GRID_NB_COLUMNS, GRID_ROW_SPACING, GRID_COLUMN_SPACING, M_DEFAULT,
            M_CHESSBOARD_GRID);

   MosPrintf(MIL_TEXT("."));

   /* Reset the relative coordinate system directly on the working surface. */
   McalSetCoordinateSystem(MilCalibration1, M_RELATIVE_COORDINATE_SYSTEM,
                           M_ABSOLUTE_COORDINATE_SYSTEM, M_IDENTITY, M_NULL,
                           M_DEFAULT, M_DEFAULT, M_DEFAULT, M_DEFAULT);
   McalSetCoordinateSystem(MilCalibration2, M_RELATIVE_COORDINATE_SYSTEM,
                           M_ABSOLUTE_COORDINATE_SYSTEM, M_IDENTITY, M_NULL,
                           M_DEFAULT, M_DEFAULT, M_DEFAULT, M_DEFAULT);
   McalSetCoordinateSystem(MilCalibration3, M_RELATIVE_COORDINATE_SYSTEM,
                           M_ABSOLUTE_COORDINATE_SYSTEM, M_IDENTITY, M_NULL,
                           M_DEFAULT, M_DEFAULT, M_DEFAULT, M_DEFAULT);
   McalSetCoordinateSystem(MilCalibration4, M_RELATIVE_COORDINATE_SYSTEM,
                           M_ABSOLUTE_COORDINATE_SYSTEM, M_IDENTITY, M_NULL,
                           M_DEFAULT, M_DEFAULT, M_DEFAULT, M_DEFAULT);

  /* Free all allocations. */
  MbufFree(MilImageGrid1);
  MbufFree(MilImageGrid2);
  MbufFree(MilImageGrid3);
  MbufFree(MilImageGrid4);
  MbufFree(MilImageGrid3D1);
  }


void AddSelectableButton(MIL_ID MilGraphicContext, MIL_ID MilGraphicList, MIL_ID MilImage,
                         MIL_INT PosX, MIL_INT PosY,
                         map<MIL_INT, MIL_ID>&  MapImageToDisplay,
                         map<MIL_INT, MIL_DOUBLE>& MapInitialZoom, MIL_INT* pRegionLabel)
   {
   /* Add the circle graphic primitive to the graphic list. */
   MgraColor(MilGraphicContext, M_COLOR_BRIGHT_GRAY);
   MgraArcFill(MilGraphicContext, MilGraphicList, PosX, PosY, BUTTON_RADIUS,
               BUTTON_RADIUS, 0, 360 );

   /* Retrieve the label of the circle graphic primitive. */
   MgraInquireList(MilGraphicList, M_LIST, M_DEFAULT, M_LAST_LABEL, pRegionLabel); 
   /* Associate the corresponding image view to the graphic primitive label. */
   MapImageToDisplay[*pRegionLabel] = MilImage;
   /* Associate the corresponding initial zoom to the graphic primitive label. */
   MapInitialZoom   [*pRegionLabel] = SRC_TO_DISP_ZOOM;
   }


void InitCameraViewDisplay(MIL_ID MilGraphicContextCameraView,
                           MIL_ID MilGraphicListCameraView, MIL_ID MilGraphicListSketch,
                           MIL_INT GlobalRegionLabel, bool ShowCalibrationAxis)
   {
   /* Clear all previous annotations in the camera view graphic list. */
   MgraClear(MilGraphicContextCameraView, MilGraphicListCameraView);
   /* Clear object selection to the sketch graphic list. */
   MgraControlList(MilGraphicListSketch, M_ALL, M_DEFAULT, M_GRAPHIC_SELECTED, M_FALSE);
   /* Select the graphic object corresponding to the global view. */
   MgraControlList(MilGraphicListSketch, M_GRAPHIC_LABEL(GlobalRegionLabel), M_DEFAULT, 
                   M_GRAPHIC_SELECTED, M_TRUE);

   if(ShowCalibrationAxis)
      {
      /* Draw the relative coordinate system axis. */
      MgraColor(MilGraphicContextCameraView, M_COLOR_DARK_GREEN);
      McalDraw(MilGraphicContextCameraView, M_NULL, MilGraphicListCameraView, 
               M_DRAW_RELATIVE_COORDINATE_SYSTEM + M_DRAW_FRAME, M_DEFAULT, M_DEFAULT);
      }
   }


void ModelFinderProcess(MIL_ID MilSystem, MIL_ID MilGraphicContext, MIL_ID MilGraphicList,
                        MIL_ID MilImage, MIL_ID& MilModResult)
   {
   MIL_ID     MilModContext; /* Search context.      */
   MIL_DOUBLE PosX, PosY;    /* Coordinate position. */

   /* Restore the search context. */
   MmodRestore(MODCONTEXT, MilSystem, M_WITH_CALIBRATION, &MilModContext);
   /* Allocate a result buffer. */
   MmodAllocResult(MilSystem, M_DEFAULT, &MilModResult);

   /* Preprocess the search context. */
   MmodPreprocess(MilModContext, M_DEFAULT);   
   /* Find the model. */  
   MmodFind(MilModContext, MilImage, MilModResult);

   /* Get the position of the first founded occurrence. */
   MmodGetResult(MilModResult, 0, M_POSITION_X, &PosX);
   MmodGetResult(MilModResult, 0, M_POSITION_Y, &PosY);

   /* Draw the edges of the found occurrence in the graphic list. */
   MmodDraw(MilGraphicContext, MilModResult,MilGraphicList,
            M_DRAW_EDGES, M_ALL, M_DEFAULT);

   /* Free all allocations. */
   MmodFree(MilModContext);
   }


void MeasProcess(MIL_ID MilSystem, MIL_ID MilGraphicContext, MIL_ID MilGraphicList,
                 MIL_ID MilImage, MIL_DOUBLE& CircleCenterX, MIL_DOUBLE& CircleCenterY,
                 MIL_DOUBLE& CircleRadius)
   {
   MIL_ID MilMeasMarker; /* Circle marker identifier. */

   /* Restore the circle marker. */
   MmeasRestoreMarker(MEASCONTEXT,MilSystem,M_DEFAULT,&MilMeasMarker);
   /* Set the center of the ring search region. */
   MmeasSetMarker(MilMeasMarker, M_SEARCH_REGION_INPUT_UNITS, M_WORLD, M_NULL);
   /*Set the inner and outer radius of the ring search region. */
   MmeasSetMarker(MilMeasMarker, M_RING_CENTER, 0, 0);
   MmeasSetMarker(MilMeasMarker, M_RING_RADII, 24, 26);

   /* Find the circle and then measure and compute all the measurements. */
   MmeasFindMarker(M_DEFAULT,MilImage,MilMeasMarker,M_DEFAULT);

   /* Draw the found circle in the graphic list. */
   MmeasDraw(MilGraphicContext, MilMeasMarker, MilGraphicList, M_DRAW_EDGES, M_DEFAULT,
             M_RESULT);
   /* Get the center and the radius of the measured circle. */
   MmeasGetResult(MilMeasMarker, M_POSITION, &CircleCenterX, &CircleCenterY);
   MmeasGetResult(MilMeasMarker, M_RADIUS  , &CircleRadius , M_NULL        );

   /* Free all allocations. */
   MmeasFree(MilMeasMarker);
   }


void MetrologyProcess(MIL_ID MilSystem, MIL_ID MilGraphicContext, MIL_ID MilGraphicList,
                      const map<MIL_ID,MIL_DOUBLE>& ImageToProcessMapColor,
                      MIL_DOUBLE PosX, MIL_DOUBLE PosY)
   {
   MIL_ID     MilMetContext,                   /* Metrology context identifier.        */
              MilMetResult;                    /* Metrology result buffer identifier.  */
  
   MIL_DOUBLE AngleToProcess,                  /* Angle to process.                    */
              NumberOfTolerance;               /* Number of tolerances.                */ 
   MIL_INT    AngleIndex,                      /* Angle index.                         */
              ToleranceIndex;                  /* Tolerance index.                     */

   bool       success;                         /* Success flag.                        */
   map<MIL_ID,MIL_DOUBLE>::const_iterator
              iterImage;                       /* Image iterator.                      */

   /* Allocate a metrology result buffer. */
   MmetAllocResult(MilSystem, M_DEFAULT, &MilMetResult);
   /* Restore the metrology context. */
   MmetRestore(METCONTEXT, MilSystem, M_WITH_CALIBRATION, &MilMetContext);

   /* Set the position of the global frame. */
   MmetControl(MilMetContext, M_GLOBAL_FRAME, M_POSITION_X, PosX);
   MmetControl(MilMetContext, M_GLOBAL_FRAME, M_POSITION_Y, PosY);

   /* Get the number of tolerances. */
   MmetInquire(MilMetContext, M_DEFAULT, M_NUMBER_OF_TOLERANCES, &NumberOfTolerance);

   /* Initialize the success flag and the first angle value to process. */
   success = false;
   AngleToProcess = 0.0;

   /* For each angle. */
   for(AngleIndex = 0; AngleIndex < NB_ANGLE_STEP; AngleIndex++)
      {
      /* For each tolerance. */
      for(ToleranceIndex = 0; ToleranceIndex < NumberOfTolerance; ToleranceIndex++)
         {
         iterImage = ImageToProcessMapColor.begin();
         /* Try for each image. */
         while(iterImage != ImageToProcessMapColor.end())
            {
            /* Calculate metrology and check tolerance status. */
            success = MetrologyProcessStep(MilMetContext, MilMetResult, ToleranceIndex,
                                           AngleToProcess, iterImage->first);
            if(success)
               { 
               /* Draw tolerance annotations. */
               MIL_DOUBLE AnnotationColor = iterImage->second;
               MgraColor(MilGraphicContext, AnnotationColor);
               MmetDraw(MilGraphicContext, MilMetResult, MilGraphicList,
                        M_DRAW_TOLERANCE_FEATURES, M_TOLERANCE_INDEX(ToleranceIndex),
                        M_DEFAULT);
               MmetDraw(MilGraphicContext, MilMetResult, MilGraphicList, M_DRAW_TOLERANCE, 
                        M_TOLERANCE_INDEX(ToleranceIndex), M_DEFAULT);  

               break;
               }
            iterImage++;
            }
         }
       AngleToProcess += ANGLE_STEP;
      }
  
   /* Free all allocations. */
   MmetFree(MilMetContext);
   MmetFree(MilMetResult);
}


bool MetrologyProcessStep(MIL_ID MilMetContext, MIL_ID MilMetResult,
                          MIL_INT ToleranceIndex, MIL_DOUBLE Angle, MIL_ID MilImage)
   {
   MIL_DOUBLE Status; /* Tolerance status. */

   /* Set the angle of the global frame. */
   MmetControl( MilMetContext, M_GLOBAL_FRAME, M_ANGLE, Angle);
   /* Calculate. */
   MmetCalculate(MilMetContext, MilImage, MilMetResult, M_DEFAULT);
   /* Get tolerance status. */
   MmetGetResult(MilMetResult, M_TOLERANCE_INDEX(ToleranceIndex), M_STATUS, &Status);

   if(Status == M_PASS)
      return true;
   else
      return false;
   }


void CreatePolarImage(MIL_ID MilSystem, MIL_ID MilImageSource, MIL_ID& MilPolarImage,
                      MIL_ID& WrapCal, MIL_DOUBLE OffsetX, MIL_DOUBLE OffsetY,
                      MIL_DOUBLE StartAngle, MIL_DOUBLE EndAngle,
                      MIL_DOUBLE RMin, MIL_DOUBLE RMax)
   {
   MIL_ID  MilLutX, MilLutY;        /* LUT buffer identifiers. */
   MIL_INT PolarSizeX, PolarSizeY;  /* Polar image size. */

   /* Create the corresponding LUTs for the specified polar transform. */
   CreateLUTs(MilSystem, MilImageSource, &MilLutX, &MilLutY, OffsetX, OffsetY, StartAngle,
              EndAngle, RMin, RMax);

   /* Get the polar image size. */
   MbufInquire(MilLutX, M_SIZE_X, &PolarSizeX);
   MbufInquire(MilLutX, M_SIZE_Y, &PolarSizeY);

   /* Allocate a buffer for the polar image. */
   MilPolarImage = MbufAlloc2d(MilSystem, PolarSizeX, PolarSizeY, 8+M_UNSIGNED,
                               M_IMAGE+M_PROC+M_DISP, M_NULL);
   MbufClear(MilPolarImage, 0.0);

   /* Warp the source image into the polar image buffer using the LUTs. */
   MimWarp(MilImageSource, MilPolarImage, MilLutX, MilLutY, 
           M_WARP_LUT + M_FIXED_POINT + NB_FIXED_POINT, M_BICUBIC);

   /* Warp the source calibration to the calibration context of the polar image using the LUTs. */
   McalWarp(MilImageSource, WrapCal, MilLutX, MilLutY, M_NULL, M_NULL, M_NULL, M_NULL,
            50, 50, M_WARP_LUT+M_FIXED_POINT+NB_FIXED_POINT, M_DEFAULT);

   /* Associate the computed calibration context to the polar image. */
   McalAssociate(WrapCal, MilPolarImage, M_DEFAULT);

   /* Free all allocations. */
   MbufFree(MilLutX);
   MbufFree(MilLutY);
   }


void CreateLUTs(MIL_ID MilSystem, MIL_ID MilCalibration, MIL_ID* pMilLutX,
                MIL_ID* pMilLutY, MIL_DOUBLE OffsetX, MIL_DOUBLE OffsetY, 
                MIL_DOUBLE StartAngle, MIL_DOUBLE EndAngle,
                MIL_DOUBLE MinRadius, MIL_DOUBLE MaxRadius)
   {
   MIL_INT SizeX, SizeY; /* LUT size. */

   /* Allocate the LUTs. */
   SizeX = static_cast<MIL_INT>(( (EndAngle - StartAngle) * NB_PIXELS_PER_DEGREE ));
   SizeY = static_cast<MIL_INT>(ceil( (MaxRadius - MinRadius) * NB_PIXELS_PER_RADIAL_MM ));

   if(SizeX > SRC_IMAGE_WIDTH)
      {
      SizeY = MIL_INT(ceil(MIL_DOUBLE(SizeY*SRC_IMAGE_WIDTH) / SizeX));
      SizeX = SRC_IMAGE_WIDTH;
      }

   MbufAlloc2d(MilSystem, SizeX, SizeY, 32+M_SIGNED, M_LUT, pMilLutX);
   MbufAlloc2d(MilSystem, SizeX, SizeY, 32+M_SIGNED, M_LUT, pMilLutY);

   /* Prepare. */
   MIL_INT NbPixels = SizeX * SizeY;
   MIL_DOUBLE* LutCoordsX   = new MIL_DOUBLE[NbPixels];
   MIL_DOUBLE* LutCoordsY   = new MIL_DOUBLE[NbPixels];
   MIL_INT32*  LutIntCoords = new MIL_INT32 [NbPixels];

   /* Calculate world points. */
   MIL_DOUBLE* pCurX = LutCoordsX;
   MIL_DOUBLE* pCurY = LutCoordsY;

   MIL_DOUBLE StepR = (MaxRadius - MinRadius) / SizeY; 
   MIL_DOUBLE StartTheta = -EndAngle * TO_RAD;
   MIL_DOUBLE StepTheta =(EndAngle - StartAngle) * TO_RAD / SizeX;

   MIL_DOUBLE CurR = MinRadius + 0.5*StepR;
   for (MIL_INT y = 0; y < SizeY; ++y)
      {
      MIL_DOUBLE CurTheta = StartTheta - 0.5*StepTheta;
      for (MIL_INT x = 0; x < SizeX; ++x)
         {
         *pCurX++ = CurR * cos(CurTheta) +OffsetX;
         *pCurY++ = CurR * sin(CurTheta) +OffsetY;
         CurTheta -= StepTheta;
         }
      CurR += StepR;
      }

   /* Calculates pixels. */
   McalTransformCoordinateList(MilCalibration, M_WORLD_TO_PIXEL, NbPixels,
                               LutCoordsX, LutCoordsY, LutCoordsX, LutCoordsY);

   MIL_DOUBLE Factor = static_cast<MIL_DOUBLE>(1 << NB_FIXED_POINT);

   /* Fill LUT X. */
   for (MIL_INT i = 0; i < NbPixels; ++i)
      {
      MIL_DOUBLE MultValue = Factor * LutCoordsX[i];
      LutIntCoords[i] = static_cast<MIL_INT32>(MultValue > 0.0 ? MultValue + 0.5 : MultValue - 0.5);
      }

   MbufPut(*pMilLutX, LutIntCoords);

   /* Fill LUT Y. */
   for (MIL_INT i = 0; i < NbPixels; ++i)
      {
      MIL_DOUBLE MultValue = Factor * LutCoordsY[i];
      LutIntCoords[i] = static_cast<MIL_INT32>(MultValue > 0.0 ? MultValue + 0.5 : MultValue - 0.5);
      }

   MbufPut(*pMilLutY, LutIntCoords);

   /* Free all allocations. */
   delete [] LutIntCoords;
   delete [] LutCoordsY;
   delete [] LutCoordsX;
   }


void OCRProcess(MIL_ID MilSystem, MIL_ID MilGraphicContext, MIL_ID MilGraphicList,
                MIL_ID MilImage)
  {
  MIL_ID        MilOcrContext,         /* OCR context identifier.        */
                MilOcrResult;          /* OCR result buffer identifier.  */

  MIL_TEXT_CHAR String[STRING_LENGTH]; /* Array of characters to read.   */
  MIL_DOUBLE    PosX  [STRING_LENGTH]; /* Array of character X-positions.*/
  MIL_DOUBLE    PosY  [STRING_LENGTH]; /* Array of character Y-positions.*/
  MIL_DOUBLE    SizeX [STRING_LENGTH]; /* Array of character X-size.     */
  MIL_DOUBLE    SizeY [STRING_LENGTH]; /* Array of character Y-size.     */

   /* Restore the OCR context. */
   MocrRestoreFont(OCRCONTEXT, M_DEFAULT, MilSystem, &MilOcrContext);
   /* Allocate an OCR result buffer. */
   MocrAllocResult(MilSystem, M_DEFAULT, &MilOcrResult);
   /* Read the string. */
   MocrReadString(MilImage, MilOcrContext, MilOcrResult);
   /* Draw box and position annotations for each read char. */
   MocrDraw(MilGraphicContext, MilOcrResult, MilGraphicList, M_DRAW_STRING_CHAR_BOX,
            M_DEFAULT, M_NULL, M_DEFAULT);
   MocrDraw(MilGraphicContext, MilOcrResult, MilGraphicList, M_DRAW_STRING_CHAR_POSITION,
            M_DEFAULT, M_NULL, M_DEFAULT);
   MocrGetResult(MilOcrResult, M_STRING, String);
   /* Get char position and size in world unit. */
   MocrControl(MilOcrResult, M_RESULT_OUTPUT_UNITS, M_WORLD);
   MocrGetResult(MilOcrResult, M_CHAR_POSITION_X, PosX);
   MocrGetResult(MilOcrResult, M_CHAR_POSITION_Y, PosY);
   MocrGetResult(MilOcrResult, M_CHAR_SIZE_X, SizeX);
   MocrGetResult(MilOcrResult, M_CHAR_SIZE_Y, SizeY);
   /* Draw the read font in the graphic list. */
   MgraFont(MilGraphicContext, M_FONT_DEFAULT_MEDIUM);
   MgraText(MilGraphicContext, MilGraphicList,
            PosX[0]-1.5*SizeX[0], PosY[0]-1.5*SizeY[0],
            String);

   MocrFree(MilOcrContext);
   MocrFree(MilOcrResult);
   }


MIL_INT MFTYPE HookHandler(MIL_INT HookType, MIL_ID EventId, void* UserDataPtr)
   {
   const SHookUserData& UserData = *((const SHookUserData*)(UserDataPtr));

   /* Check if the object selected is valid. */
   MIL_INT ObjectLabelSelected;
   MgraGetHookInfo(EventId, M_GRAPHIC_LABEL_VALUE, &ObjectLabelSelected);

   if(ObjectLabelSelected == M_NO_LABEL)
      {
      MgraGetHookInfo(EventId, M_GRAPHIC_LABEL_VALUE_DESELECTED, &ObjectLabelSelected);
      }

   if(ObjectLabelSelected != M_NO_LABEL)
      {
      /* Reset zoom factor. */
      MIL_DOUBLE Zoom = UserData.pMapInitialZoom->find(ObjectLabelSelected)->second;
      MdispZoom(UserData.MilDisplay, Zoom, Zoom);

      /* Set corresponding view image to the display according to the selected label. */
      MIL_ID ImageToDisplay = UserData.pMapImageToDisplay->find(ObjectLabelSelected)->second;
      MdispSelect(UserData.MilDisplay, ImageToDisplay);
      }

   return 0;
   }
