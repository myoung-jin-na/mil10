//***************************************************************************************/
//
// File name: StereoCalibration.cpp
//
// Synopsis:  This program contains an example of stereo calibration using milcal and
//            mil3dmap modules. See the PrintHeader() function below for detailed description.
//
// Printable calibration grids in PDF format can be found in your "Matrox Imaging/Images/"
// directory.
//
#include <mil.h>
#include <math.h>

// DirectX display is only available under Windows.
#if M_MIL_USE_WINDOWS && !M_MIL_USE_CE
   #define USE_D3D_DISPLAY 1
#else
   #define USE_D3D_DISPLAY 0
#endif

#if USE_D3D_DISPLAY
   // Include DirectX display only under Windows.
   #include "MdispD3D.h"
#endif

//****************************************************************************
// Example description.
//****************************************************************************
void PrintHeader()
   {
   MosPrintf(MIL_TEXT("[EXAMPLE NAME]\n")
             MIL_TEXT("StereoCalibration\n\n")

             MIL_TEXT("[SYNOPSIS]\n")
             MIL_TEXT("This example demonstrates how to calibrate multiple cameras and how to use the\n")
             MIL_TEXT("calibration for estimating the 3D position of features using stereo\n")
             MIL_TEXT("triangulation.\n\n")

             MIL_TEXT("Calibration phase:\n")
             MIL_TEXT("Each camera is calibrated using its own calibration grid. The positions and\n")
             MIL_TEXT("orientations of the calibration grids with respect to each other are used in\n")
             MIL_TEXT("order to have all the calibrations sharing a common absolute coordinate system.\n\n")

             MIL_TEXT("Stereo triangulation phase:\n")
             MIL_TEXT("Images taken by each camera are analyzed to measure feature positions in pixel\n")
             MIL_TEXT("coordinates. Stereo triangulation is performed to calculate the 3D positions of\n")
             MIL_TEXT("these features in the world. The calculated distance between some features is\n")
             MIL_TEXT("compared with the expected distance between these features.\n\n")

             MIL_TEXT("[MODULES USED]\n")
             MIL_TEXT("Modules used: application, system, display, digitizer, buffer, graphic,\n")
             MIL_TEXT("              image processing, calibration, 3d reconstruction, blob, edge.\n\n"));

   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n"));

   MosGetch();
   MosPrintf(MIL_TEXT("\n"));
   }

// Functions declarations.
struct SCalibrationData;

bool CalibrateImage(MIL_ID  MilCalibration,
                    MIL_ID  MilImage,
                    MIL_ID  MilDisplay,
                    const SCalibrationData& rData);

void DisplayCalibratedImages(const MIL_ID* MilImageArray, MIL_ID MilDispay,
                             MIL_INT NumberOfCameras);

bool ExtractStereoPointsInImageUsingEdge(MIL_ID MilImage, MIL_ID MilDisplay,
                                         MIL_DOUBLE* ImagePointsXArray, MIL_DOUBLE* ImagePointsYArray,
                                         MIL_INT ExpectedNumberOfPoints,
                                         MIL_INT SortFeature);

bool ExtractStereoPointsInImageUsingBlob(MIL_ID MilImage, MIL_ID MilDisplay,
                                         MIL_DOUBLE* ImagePointsXArray, MIL_DOUBLE* ImagePointsYArray,
                                         MIL_INT ExpectedNumberOfPoints,
                                         MIL_INT SortFeature);

void DisplayStereoData(const MIL_ID* MilImageArray, MIL_ID MilDispay,
                       const MIL_DOUBLE* ImagePointsXArray, const MIL_DOUBLE* ImagePointsYArray,
                       MIL_INT NumberOfCameras, MIL_INT NumberOfPoints);

void CalculateWorldPointsError(MIL_ID MilDisplay,
                               const MIL_DOUBLE* WorldPointsXArray,
                               const MIL_DOUBLE* WorldPointsYArray,
                               const MIL_DOUBLE* WorldPointsZArray,
                               const MIL_DOUBLE* WorldRMSErrorArray,
                               MIL_INT NumberOfPoints);

void CopyPictureInOverlay(MIL_CONST_TEXT_PTR PictureFilename, MIL_ID MilDisplay);

// Macros and constants definitions.

// Set this to 0 if you wish to remove the pictures pasted in the overlay.
#define ENABLE_PICTURES_IN_OVERLAY 1

// Directory containing all images used for the example.
#define EXAMPLE_IMAGE_PATH    M_IMAGE_PATH MIL_TEXT("StereoCalibration/")

// Number of cameras.
static const MIL_INT NUMBER_OF_CAMERAS = 2;

// Images used only for annotations.
static const MIL_TEXT_CHAR GRIDS_FILENAME[] = EXAMPLE_IMAGE_PATH MIL_TEXT("Grids.mim");
static const MIL_TEXT_CHAR MEASURED_ERRORS_FILENAME[] = EXAMPLE_IMAGE_PATH MIL_TEXT("MeasuredErrors.mim");

static const MIL_TEXT_CHAR WHOLE_SETUP_PICTURE_FILENAME[] = EXAMPLE_IMAGE_PATH MIL_TEXT("PictureCal01.mim");
static const MIL_TEXT_CHAR* const CALIBRATION_PICTURE_FILENAMES[NUMBER_OF_CAMERAS] =
   {
      EXAMPLE_IMAGE_PATH MIL_TEXT("PictureCal0.mim"),
      EXAMPLE_IMAGE_PATH MIL_TEXT("PictureCal1.mim")
   };
static const MIL_TEXT_CHAR* const STEREO_PICTURE_FILENAMES[NUMBER_OF_CAMERAS] =
   {
      EXAMPLE_IMAGE_PATH MIL_TEXT("PictureStereo0.mim"),
      EXAMPLE_IMAGE_PATH MIL_TEXT("PictureStereo1.mim")
   };

// All necessary informations to calibrate the cameras.
struct SCalibrationData
   {
   MIL_INT              m_CameraIndex;
   const MIL_TEXT_CHAR* m_ImageFile;
   MIL_DOUBLE           m_GridCornerHintX;
   MIL_DOUBLE           m_GridCornerHintY;
   MIL_DOUBLE           m_RelativePositionX;
   MIL_DOUBLE           m_RelativePositionY;
   MIL_DOUBLE           m_RelativePositionZ;
   MIL_DOUBLE           m_RelativeRotationX;
   MIL_DOUBLE           m_RelativeRotationY;
   MIL_DOUBLE           m_RelativeRotationZ;
   };

static const SCalibrationData CALIBRATION_DATA[NUMBER_OF_CAMERAS] =
   {
      // Camera                                                                     Translation          Rotation
      //  Idx     Image filename                                HintX   HintY    X      Y       Z      X    Y    Z
      {    0    , EXAMPLE_IMAGE_PATH MIL_TEXT("CalImage0.mim"), M_NONE, M_NONE,  0,    79.0,    0,     0,   0,   0},
      {    1    , EXAMPLE_IMAGE_PATH MIL_TEXT("CalImage1.mim"), M_NONE, M_NONE,  0,  -132.0,    0,    90,   0,   0}
   };

// Information about sets of images used for stereo calculation
// and about some characteristics of the features extraction
// in these images.
struct SStereoSetData
   {
   const MIL_TEXT_CHAR* m_ImageFiles[NUMBER_OF_CAMERAS];
   MIL_INT m_EdgeSortFeature;
   MIL_INT m_BlobSortFeature;
   };

static const MIL_INT NUMBER_OF_STEREO_SETS = 3;
static const SStereoSetData STEREO_SETS[NUMBER_OF_STEREO_SETS] =
   {
      {{
      EXAMPLE_IMAGE_PATH MIL_TEXT("StereoImage0_Camera0.mim"),
      EXAMPLE_IMAGE_PATH MIL_TEXT("StereoImage0_Camera1.mim") }, M_ELLIPSE_FIT_CENTER_X, M_CENTER_OF_GRAVITY_X},
      {{
      EXAMPLE_IMAGE_PATH MIL_TEXT("StereoImage1_Camera0.mim"),
      EXAMPLE_IMAGE_PATH MIL_TEXT("StereoImage1_Camera1.mim") }, M_ELLIPSE_FIT_CENTER_Y, M_CENTER_OF_GRAVITY_Y},
      {{
      EXAMPLE_IMAGE_PATH MIL_TEXT("StereoImage2_Camera0.mim"),
      EXAMPLE_IMAGE_PATH MIL_TEXT("StereoImage2_Camera1.mim") }, M_ELLIPSE_FIT_CENTER_Y, M_CENTER_OF_GRAVITY_Y},
   };

// File name of an image used to frame the images to be pasted in the overlay.
static const MIL_TEXT_CHAR* const FRAME_IMAGE_FILE = EXAMPLE_IMAGE_PATH MIL_TEXT("frame.mim");

// Parameters for McalGrid(). Depends on the calibration grid used.
static const MIL_INT    ROW_NUMBER     = 16;
static const MIL_INT    COLUMN_NUMBER  = 15;
static const MIL_DOUBLE ROW_SPACING    = 5.0;           // in mm
static const MIL_DOUBLE COLUMN_SPACING = 5.0;           // in mm
static const MIL_INT    GRID_TYPE      = M_CHESSBOARD_GRID+M_Y_AXIS_DOWN;

// Colors used to draw in the overlay.
static const MIL_DOUBLE PIXEL_COLOR             = M_COLOR_GREEN;
static const MIL_DOUBLE WORLD_COLOR             = M_COLOR_RED;
static const MIL_DOUBLE COORDINATE_SYSTEM_COLOR = M_COLOR_CYAN;

// Transparency color of the frame image.
static const MIL_DOUBLE FRAME_TRANSPARENT_COLOR   = M_RGB888(227,0,227);

// Size of the border in the image (used when pasting picture in the overlay).
static const MIL_INT FRAME_BORDER = 5; // in pixels

// Maximum ratio of picture to paste in the overlay with respect to image size.
static const MIL_DOUBLE MAX_PICTURE_RATIO = 0.25;

// Characteristics of the stereo features extracted.
static const MIL_INT    NUMBER_OF_STEREO_POINTS = 6;
static const MIL_DOUBLE DISTANCE_BETWEEN_STEREO_POINTS = 10.0; // in mm

// Size of the DirectX display.
static const MIL_INT D3D_DISPLAY_SIZE_X = 600;
static const MIL_INT D3D_DISPLAY_SIZE_Y = 600;

// Size of string buffers.
static const MIL_INT MAX_CHAR_LENGTH = 256;

// Position of text for calibration grid annotations.
static const MIL_DOUBLE CAL_GRID_TEXT_POS_X = 315.0;
static const MIL_DOUBLE CAL_GRID_TEXT_POS_Y =   5.0;

//*****************************************************************************
// Main.
//*****************************************************************************
int MosMain(void)
   {
   // Allocate MIL objects.
   MIL_ID MilApplication = MappAlloc(M_NULL, M_DEFAULT, M_NULL);
   MIL_ID MilSystem      = MsysAlloc(M_DEFAULT, M_SYSTEM_HOST, M_DEFAULT, M_DEFAULT, M_NULL);
   MIL_ID MilDisplay     = MdispAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, M_NULL);

   MdispControl(MilDisplay, M_OVERLAY, M_ENABLE);

   PrintHeader();

   MIL_ID MilGridsImage = MbufRestore(GRIDS_FILENAME, MilSystem, M_NULL);
   MdispSelect(MilDisplay, MilGridsImage);
   CopyPictureInOverlay(WHOLE_SETUP_PICTURE_FILENAME, MilDisplay);

   MosPrintf(MIL_TEXT("==========================================\n")
             MIL_TEXT("Calibration phase.\n\n")

             MIL_TEXT("General view of the grids used during calibration phase.\n\n"));

   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   MbufFree(MilGridsImage);

   // Declaration of one calibration context and one image for each camera.
   MIL_ID MilCalibrationArray[NUMBER_OF_CAMERAS];
   MIL_ID MilImageArray[NUMBER_OF_CAMERAS];

   // Loop to calibrate all cameras.
   bool Success = true;
   MIL_INT CameraIndex;
   for (CameraIndex = 0; CameraIndex < NUMBER_OF_CAMERAS; CameraIndex++)
      {
      // Restore image grabbed by the camera.
      MilImageArray[CameraIndex] = MbufRestore(CALIBRATION_DATA[CameraIndex].m_ImageFile, MilSystem, M_NULL);
      MdispSelect(MilDisplay, MilImageArray[CameraIndex]);

      CopyPictureInOverlay(CALIBRATION_PICTURE_FILENAMES[CameraIndex], MilDisplay);

      MosPrintf(MIL_TEXT("Calibrating the image acquired by camera #%d...\n"), CameraIndex);

      // Calibrate one camera using one image grabbed by this camera.
      MilCalibrationArray[CameraIndex] = McalAlloc(MilSystem, M_TSAI_BASED, M_DEFAULT, M_NULL);

      Success = CalibrateImage(MilCalibrationArray[CameraIndex],
                               MilImageArray[CameraIndex],
                               MilDisplay,
                               CALIBRATION_DATA[CameraIndex]);

      if (!Success)
         break;
      }

   if (Success)
      {
      DisplayCalibratedImages(MilImageArray, MilDisplay, NUMBER_OF_CAMERAS);
      }

   MosPrintf(MIL_TEXT("==========================================\n")
             MIL_TEXT("Stereo triangulation phase.\n\n"));

   MIL_INT StereoSetIndex;
   for (StereoSetIndex = 0; StereoSetIndex < NUMBER_OF_STEREO_SETS; StereoSetIndex++)
      {
      if (!Success)
         break;

      MosPrintf(MIL_TEXT("---------------------------------\n")
                MIL_TEXT("Stereo images sequence #%d.\n\n"), StereoSetIndex);

      MIL_DOUBLE ImagePointsXArray[NUMBER_OF_CAMERAS*NUMBER_OF_STEREO_POINTS];
      MIL_DOUBLE ImagePointsYArray[NUMBER_OF_CAMERAS*NUMBER_OF_STEREO_POINTS];

      for (CameraIndex = 0; CameraIndex < NUMBER_OF_CAMERAS; CameraIndex++)
         {
         MosPrintf(MIL_TEXT("--------------------\n"));

         // Restore the image of stereo features.
         MbufFree(MilImageArray[CameraIndex]);
         MilImageArray[CameraIndex] = MbufRestore(STEREO_SETS[StereoSetIndex].m_ImageFiles[CameraIndex], MilSystem, M_NULL);
         McalAssociate(MilCalibrationArray[CameraIndex], MilImageArray[CameraIndex], M_DEFAULT);

         // Re-setup display for this image.
         MdispSelect(MilDisplay, MilImageArray[CameraIndex]);
         MdispControl(MilDisplay, M_WINDOW_SHOW, M_ENABLE );

         CopyPictureInOverlay(STEREO_PICTURE_FILENAMES[CameraIndex], MilDisplay);

         MosPrintf(MIL_TEXT("Analyzing the image acquired by camera #%d.\n"), CameraIndex);

         // Extract stereo points in one of the image.
         MIL_DOUBLE* ImagePointsForOneCameraXArray = ImagePointsXArray + CameraIndex*NUMBER_OF_STEREO_POINTS;
         MIL_DOUBLE* ImagePointsForOneCameraYArray = ImagePointsYArray + CameraIndex*NUMBER_OF_STEREO_POINTS;

         // First try with edge.
         Success = ExtractStereoPointsInImageUsingEdge(MilImageArray[CameraIndex],
                                                       MilDisplay,
                                                       ImagePointsForOneCameraXArray,
                                                       ImagePointsForOneCameraYArray,
                                                       NUMBER_OF_STEREO_POINTS,
                                                       STEREO_SETS[StereoSetIndex].m_EdgeSortFeature);

         // Second try with blob.
         if (!Success)
            Success = ExtractStereoPointsInImageUsingBlob(MilImageArray[CameraIndex],
                                                          MilDisplay,
                                                          ImagePointsForOneCameraXArray,
                                                          ImagePointsForOneCameraYArray,
                                                          NUMBER_OF_STEREO_POINTS,
                                                          STEREO_SETS[StereoSetIndex].m_BlobSortFeature);

         if (!Success)
            break;
         }

      if (!Success)
         break;

      // Calculate the world positions of the points
      // using stereo triangulation.
      MIL_DOUBLE WorldPointsXArray[NUMBER_OF_STEREO_POINTS];
      MIL_DOUBLE WorldPointsYArray[NUMBER_OF_STEREO_POINTS];
      MIL_DOUBLE WorldPointsZArray[NUMBER_OF_STEREO_POINTS];
      MIL_DOUBLE WorldRMSErrorsArray[NUMBER_OF_STEREO_POINTS];
      M3dmapTriangulate(MilImageArray, ImagePointsXArray, ImagePointsYArray,
                        WorldPointsXArray, WorldPointsYArray, WorldPointsZArray, WorldRMSErrorsArray,
                        NUMBER_OF_CAMERAS, NUMBER_OF_STEREO_POINTS,
                        M_ABSOLUTE_COORDINATE_SYSTEM, M_DEFAULT);

      // Display 3D representation of stereo calculation.
      DisplayStereoData(MilImageArray, MilDisplay,
                        ImagePointsXArray, ImagePointsYArray,
                        NUMBER_OF_CAMERAS, NUMBER_OF_STEREO_POINTS);

      // Calculate and display the errors.
      CalculateWorldPointsError(MilDisplay,
                                WorldPointsXArray,
                                WorldPointsYArray,
                                WorldPointsZArray,
                                WorldRMSErrorsArray,
                                NUMBER_OF_STEREO_POINTS);
      }

   // Free MIL objects.
   for (CameraIndex = 0; CameraIndex < NUMBER_OF_CAMERAS; CameraIndex++)
      {
      if (MilImageArray[CameraIndex] != M_NULL)
         MbufFree(MilImageArray[CameraIndex]);
      if (MilCalibrationArray[CameraIndex] != M_NULL)
         McalFree(MilCalibrationArray[CameraIndex]);
      }

   MdispFree(MilDisplay);

   MosPrintf(MIL_TEXT("%d sets of images have been processed.\n\n"), NUMBER_OF_STEREO_SETS);
   MosPrintf(MIL_TEXT("Press <Enter> to end.\n"));
   MosGetch();

   MsysFree(MilSystem);

   MappFree(MilApplication);

   return 0;
   }

//*****************************************************************************
// Calibrate one camera using one image
// using a grid positioned at some position and angle specified by
// an SCalibrationData structure.
// Returns true if calibration is successful.
//*****************************************************************************
bool CalibrateImage(MIL_ID  MilCalibration,
                    MIL_ID  MilImage,
                    MIL_ID  MilDisplay,
                    const SCalibrationData& rData)
   {
   bool CalibrationIsSuccessful = false;

   // Set hint for calibration.
   McalControl(MilCalibration, M_GRID_CORNER_HINT_X, rData.m_GridCornerHintX);
   McalControl(MilCalibration, M_GRID_CORNER_HINT_Y, rData.m_GridCornerHintY);

   // The calibration grid is in the relative coordinate system
   // that is itself at some position and angle in the absolute
   // coordinate system.
   McalControl(MilCalibration, M_CALIBRATION_PLANE, M_RELATIVE_COORDINATE_SYSTEM);

   McalSetCoordinateSystem(MilCalibration,
                           M_RELATIVE_COORDINATE_SYSTEM,
                           M_ABSOLUTE_COORDINATE_SYSTEM,
                           M_ROTATION_XYZ+M_ASSIGN,
                           M_NULL,
                           rData.m_RelativeRotationX,
                           rData.m_RelativeRotationY,
                           rData.m_RelativeRotationZ,
                           M_DEFAULT);

   McalSetCoordinateSystem(MilCalibration,
                           M_RELATIVE_COORDINATE_SYSTEM,
                           M_RELATIVE_COORDINATE_SYSTEM,
                           M_TRANSLATION,
                           M_NULL,
                           rData.m_RelativePositionX,
                           rData.m_RelativePositionY,
                           rData.m_RelativePositionZ,
                           M_DEFAULT);

   // Calibrate.
   McalGrid(MilCalibration, MilImage,
            0.0, 0.0, 0.0,                   // GridOffset
            ROW_NUMBER,   COLUMN_NUMBER,
            ROW_SPACING,  COLUMN_SPACING,
            M_DEFAULT, GRID_TYPE);

   // Verify calibration status.
   MIL_INT CalibrationStatus = McalInquire(MilCalibration, M_CALIBRATION_STATUS, M_NULL);

   if (CalibrationStatus == M_CALIBRATED)
      {
      MIL_ID MilOverlayImage;
      MdispInquire(MilDisplay, M_OVERLAY_ID, &MilOverlayImage);

      // Draw in the overlay
      // - image points
      // - world points
      // - relative coordinate system.
      MgraColor(M_DEFAULT, PIXEL_COLOR);
      McalDraw(M_DEFAULT, MilCalibration, MilOverlayImage, M_DRAW_IMAGE_POINTS, M_DEFAULT, M_DEFAULT);
      MgraColor(M_DEFAULT, WORLD_COLOR);
      McalDraw(M_DEFAULT, MilCalibration, MilOverlayImage, M_DRAW_WORLD_POINTS, M_DEFAULT, M_DEFAULT);
      MgraColor(M_DEFAULT, COORDINATE_SYSTEM_COLOR);
      McalDraw(M_DEFAULT, M_NULL, MilOverlayImage, M_DRAW_RELATIVE_COORDINATE_SYSTEM+M_DRAW_AXES, M_DEFAULT, M_DEFAULT);

      MIL_TEXT_CHAR StrMsg[MAX_CHAR_LENGTH];
      MosSprintf(StrMsg, MAX_CHAR_LENGTH, MIL_TEXT("Relative coordinate system of camera #%d"), rData.m_CameraIndex);
      MgraColor(M_DEFAULT, COORDINATE_SYSTEM_COLOR);
      MgraText(M_DEFAULT, MilOverlayImage, CAL_GRID_TEXT_POS_X, CAL_GRID_TEXT_POS_Y, StrMsg);

      // Display message
      MosPrintf(MIL_TEXT("Calibration successful.\n\n"));
      MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
      MosGetch();

      // Clear overlay.
      MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);

      CalibrationIsSuccessful = true;
      }
   else
      {
      // Display message
      MosPrintf(MIL_TEXT("Calibration failed.\n\n"));
      MosPrintf(MIL_TEXT("Press <Enter> to end.\n\n"));
      MosGetch();
      }

   return CalibrationIsSuccessful;
   }

//*****************************************************************************
// Draw a 3D representation of calibrated images along with the camera
// positions.
//*****************************************************************************
void DisplayCalibratedImages(const MIL_ID* MilImageArray, MIL_ID MilDisplay, MIL_INT NumberOfCameras)
   {
 #if USE_D3D_DISPLAY
   MIL_DISP_D3D_HANDLE DispD3DHandle = McalD3DAlloc(MilImageArray, NumberOfCameras,
                                                    D3D_DISPLAY_SIZE_X, D3D_DISPLAY_SIZE_Y, 0);

   if (DispD3DHandle != NULL)
      {
      // Hide Mil Display.
      MdispControl(MilDisplay, M_WINDOW_SHOW, M_DISABLE);
      MdispD3DShow(DispD3DHandle);

      // Display message
      MosPrintf(MIL_TEXT("--------------------\n")
                MIL_TEXT("Displaying a 3D representation of the scene with the calibration images\n")
                MIL_TEXT("and the camera positions and orientations.\n\n"));

      MdispD3DPrintHelp(DispD3DHandle);

      MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
      MosGetch();

      MdispD3DHide(DispD3DHandle);
      MdispD3DFree(DispD3DHandle);
      }
   else
#endif
      {
      // Display message
      MosPrintf(MIL_TEXT("Failed to display 3D representation.\n\n"));
      MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
      MosGetch();
      }
  }

//*****************************************************************************
// Extract features using blob in one image in order to do stereo.
// Returns true if all features were extracted.
//*****************************************************************************
bool ExtractStereoPointsInImageUsingBlob(MIL_ID MilImage, MIL_ID MilDisplay,
                                         MIL_DOUBLE* ImagePointsXArray, MIL_DOUBLE* ImagePointsYArray,
                                         MIL_INT ExpectedNumberOfPoints,
                                         MIL_INT SortFeature)
   {
   bool ExtractionIsSuccessful = false;

   // Re-setup display for this image.
   MdispSelect(MilDisplay, MilImage);
   MdispControl(MilDisplay, M_WINDOW_SHOW, M_ENABLE );

   // Binarize image using auto threshold.
   MimBinarize(MilImage, MilImage, M_BIMODAL+M_GREATER_OR_EQUAL, M_NULL, M_NULL);

   // Allocate and configure blob objects to extract centers of
   // gravity.
   MIL_ID MilSystem = MbufInquire(MilImage, M_OWNER_SYSTEM, M_NULL);

   MIL_ID MilBlobFeatureList = MblobAllocFeatureList(MilSystem, M_NULL);
   MIL_ID MilBlobResult = MblobAllocResult(MilSystem, M_NULL);

   MblobSelectFeature(MilBlobFeatureList, M_CENTER_OF_GRAVITY);
   MblobSelectFeature(MilBlobFeatureList, SortFeature + M_SORT1_UP);

   MblobControl(MilBlobResult, M_FOREGROUND_VALUE, M_ZERO);

   // Extract the centers of gravity using blobs.
   MblobCalculate(MilImage, M_NULL, MilBlobFeatureList, MilBlobResult);

   // Verify if the expected number of blobs have been extracted.
   MIL_INT Number = MblobGetNumber(MilBlobResult, M_NULL);
   if (Number == ExpectedNumberOfPoints)
      {
      // The stereo points are the center of gravity of the blobs.
      MblobControl(MilBlobResult, M_RESULT_OUTPUT_UNITS, M_PIXEL);
      MblobGetResult(MilBlobResult, M_CENTER_OF_GRAVITY_X, ImagePointsXArray);
      MblobGetResult(MilBlobResult, M_CENTER_OF_GRAVITY_Y, ImagePointsYArray);

      // Draw extracted points.
      MIL_ID MilOverlayImage;
      MdispInquire(MilDisplay, M_OVERLAY_ID, &MilOverlayImage);
      MgraColor(M_DEFAULT, M_COLOR_GREEN);
      MblobDraw(M_DEFAULT, MilBlobResult, MilOverlayImage, M_DRAW_CENTER_OF_GRAVITY, M_DEFAULT, M_DEFAULT);

      // Display message
      MosPrintf(MIL_TEXT("Extracted stereo points from the image using blobs centers of gravity,\n")
                MIL_TEXT("as displayed in green.\n"));
      MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));

      MosGetch();

      // Clear overlay.
      MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);

      ExtractionIsSuccessful = true;
      }
   else
      {
      MosPrintf(MIL_TEXT("Failed to extract stereo points from image.\n\n"));
      MosPrintf(MIL_TEXT("Press <Enter> to end.\n\n"));
      }

   MblobFree(MilBlobResult);
   MblobFree(MilBlobFeatureList);

   return ExtractionIsSuccessful;
   }

//*****************************************************************************
// Extract features using edge in one image in order to do stereo.
// Returns true if all features were extracted.
//*****************************************************************************
bool ExtractStereoPointsInImageUsingEdge(MIL_ID MilImage, MIL_ID MilDisplay,
                                         MIL_DOUBLE* ImagePointsXArray, MIL_DOUBLE* ImagePointsYArray,
                                         MIL_INT ExpectedNumberOfPoints,
                                         MIL_INT SortFeature)
   {
   bool ExtractionIsSuccessful = false;

   // Allocate and configure edge objects to extract ellipse centers.
   MIL_ID MilSystem = MbufInquire(MilImage, M_OWNER_SYSTEM, M_NULL);

   MIL_ID MilEdgeContext = MedgeAlloc(MilSystem, M_CONTOUR, M_DEFAULT, M_NULL);
   MIL_ID MilEdgeResult = MedgeAllocResult(MilSystem, M_DEFAULT, M_NULL);

   MedgeControl(MilEdgeContext, M_ELLIPSE_FIT, M_ENABLE);
   MedgeControl(MilEdgeContext, SortFeature + M_SORT1_UP, M_ENABLE);

   MedgeControl(MilEdgeContext, M_THRESHOLD_MODE, M_VERY_HIGH);

   // Extract the centers of gravity using blobs.
   MedgeCalculate(MilEdgeContext, MilImage, M_NULL, M_NULL, M_NULL, MilEdgeResult, M_DEFAULT);

   // Verify if the expected number of blobs have been extracted.
   MIL_INT Number;
   MedgeGetResult(MilEdgeResult, M_ALL, M_NUMBER_OF_CHAINS+M_TYPE_MIL_INT, &Number, M_NULL);
   if (Number == ExpectedNumberOfPoints)
      {
      // The stereo points are the center of gravity of the blobs.
      MedgeControl(MilEdgeResult, M_RESULT_OUTPUT_UNITS, M_PIXEL);
      MedgeGetResult(MilEdgeResult, M_ALL, M_ELLIPSE_FIT_CENTER_X, ImagePointsXArray, M_NULL);
      MedgeGetResult(MilEdgeResult, M_ALL, M_ELLIPSE_FIT_CENTER_Y, ImagePointsYArray, M_NULL);

      // Draw extracted points.
      MIL_ID MilOverlayImage;
      MdispInquire(MilDisplay, M_OVERLAY_ID, &MilOverlayImage);
      MgraColor(M_DEFAULT, M_COLOR_GREEN);
      MedgeDraw(M_DEFAULT, MilEdgeResult, MilOverlayImage, M_DRAW_ELLIPSE_FIT, M_DEFAULT, M_DEFAULT);

      // Display message
      MosPrintf(MIL_TEXT("Extracted stereo points from then image using an ellipse fit on the edge\n")
                MIL_TEXT("contours, as displayed in green.\n\n"));
      MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));

      MosGetch();

      // Clear overlay.
      MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);

      ExtractionIsSuccessful = true;
      }
   else
      {
      MosPrintf(MIL_TEXT("Failed to extract stereo points from image.\n\n"));
      MosPrintf(MIL_TEXT("Press <Enter> to end.\n\n"));
      }

   MedgeFree(MilEdgeResult);
   MedgeFree(MilEdgeContext);

   return ExtractionIsSuccessful;
   }

//*****************************************************************************
// Draw a 3D representation of stereo points.
//*****************************************************************************
void DisplayStereoData(const MIL_ID* MilImageArray, MIL_ID MilDisplay,
                       const MIL_DOUBLE* ImagePointsXArray, const MIL_DOUBLE* ImagePointsYArray,
                       MIL_INT NumberOfCameras, MIL_INT NumberOfPoints)
   {
#if USE_D3D_DISPLAY
   MIL_DISP_D3D_HANDLE DispD3DHandle = MstereoD3DAlloc(MilImageArray,
      ImagePointsXArray, ImagePointsYArray,
      NumberOfCameras, NumberOfPoints,
      M_ABSOLUTE_COORDINATE_SYSTEM,
      D3D_DISPLAY_SIZE_X, D3D_DISPLAY_SIZE_Y, 0);

   if (DispD3DHandle != NULL)
      {
      /* Hide Mil Display. */
      MdispControl(MilDisplay, M_WINDOW_SHOW, M_DISABLE );
      MdispD3DShow(DispD3DHandle);

      // Display message
      MosPrintf(MIL_TEXT("--------------------\n")
                MIL_TEXT("Displaying a 3D representation of:\n")
                MIL_TEXT("- The camera positions.\n")
                MIL_TEXT("- The stereo images (symbolically positioned in front of each camera).\n")
                MIL_TEXT("- The stereo triangulation lines, in red.\n")
                MIL_TEXT("- The calculated world points, in white.\n\n"));

      MdispD3DPrintHelp(DispD3DHandle);

      MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
      MosGetch();

      MdispD3DHide(DispD3DHandle);
      MdispD3DFree(DispD3DHandle);
      }
   else
#endif
      {
      // Display message
      MosPrintf(MIL_TEXT("Failed to display 3D representation.\n\n"));
      MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
      MosGetch();
      }
   }

//*****************************************************************************
// Calculate and display the errors between the calculated stereo world
// points and the theoretical points.
//*****************************************************************************
void CalculateWorldPointsError(MIL_ID MilDisplay,
                               const MIL_DOUBLE* WorldPointsXArray,
                               const MIL_DOUBLE* WorldPointsYArray,
                               const MIL_DOUBLE* WorldPointsZArray,
                               const MIL_DOUBLE* WorldRMSErrorArray,
                               MIL_INT NumberOfPoints)
   {
   MIL_DOUBLE SumRMSError = 0.0;
   MIL_DOUBLE MaxRMSError = 0.0;

   MosPrintf(MIL_TEXT("--------------------\n")
             MIL_TEXT("Stereo lines calculated from features in each image might not intersect\n")
             MIL_TEXT("in 3D space. The distance between a calculated world point\n")
             MIL_TEXT("and the stereo lines is displayed in red in the schematic image.\n")
             MIL_TEXT("For each world point, the mean distance is calculated (noted RMS Error):\n"));

   MIL_INT i;
   for (i = 0; i < NumberOfPoints; i++)
      {
      MosPrintf(MIL_TEXT("   Point %d: %5.2f mm\n"), i, WorldRMSErrorArray[i]);

      SumRMSError += WorldRMSErrorArray[i];
      if (WorldRMSErrorArray[i] > MaxRMSError)
         MaxRMSError = WorldRMSErrorArray[i];
      }

   MosPrintf(MIL_TEXT("\n"));
   MosPrintf(MIL_TEXT("   Average: %5.2f mm\n"), SumRMSError / static_cast<MIL_DOUBLE>(NumberOfPoints));
   MosPrintf(MIL_TEXT("   Maximum: %5.2f mm\n\n"), MaxRMSError);

   if (NumberOfPoints >= 2)
      {
      // Calculate distance between the two extreme points.
      MIL_DOUBLE ExtremeDX = WorldPointsXArray[NumberOfPoints-1] - WorldPointsXArray[0];
      MIL_DOUBLE ExtremeDY = WorldPointsYArray[NumberOfPoints-1] - WorldPointsYArray[0];
      MIL_DOUBLE ExtremeDZ = WorldPointsZArray[NumberOfPoints-1] - WorldPointsZArray[0];
      MIL_DOUBLE CalculatedDistance = sqrt(ExtremeDX*ExtremeDX + ExtremeDY*ExtremeDY + ExtremeDZ*ExtremeDZ);
      MIL_DOUBLE ExpectedDistance = static_cast<MIL_DOUBLE>(NumberOfPoints-1) * DISTANCE_BETWEEN_STEREO_POINTS;
      MIL_DOUBLE AbsoluteError = fabs(CalculatedDistance - ExpectedDistance);
      MIL_DOUBLE RelativeError = AbsoluteError / ExpectedDistance;
      MosPrintf(MIL_TEXT("The distance between the two extreme features (in green) is calculated and\n")
                MIL_TEXT("compared to the expected value (according to the CAD of the features):\n"));
      MosPrintf(MIL_TEXT("   Calculated: %5.2f mm\n"), CalculatedDistance);
      MosPrintf(MIL_TEXT("   Expected:   %5.2f mm\n"), ExpectedDistance);
      MosPrintf(MIL_TEXT("   Error:      %5.2f mm (%5.1f %%)\n\n"), AbsoluteError, RelativeError*100.0);
      }

   // Display image explaining what errors are measured.
   MIL_ID MilSystem = MdispInquire(MilDisplay, M_OWNER_SYSTEM, M_NULL);
   MIL_ID MilMeasuredErrorsImage = MbufRestore(MEASURED_ERRORS_FILENAME, MilSystem, M_NULL);
   MdispSelect(MilDisplay, MilMeasuredErrorsImage);
   MdispControl(MilDisplay, M_WINDOW_SHOW, M_ENABLE );

   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   MbufFree(MilMeasuredErrorsImage);
   }

void CopyPictureInOverlay(MIL_CONST_TEXT_PTR PictureFilename, MIL_ID MilDisplay)
   {
#if ENABLE_PICTURES_IN_OVERLAY
   // Set up images to be pasted in the overlay.
   // Determine size of the images.

   // Initialize size with actual picture size.
   MIL_INT PictureSizeX = MbufDiskInquire(FRAME_IMAGE_FILE, M_SIZE_X, M_NULL);
   MIL_INT PictureSizeY = MbufDiskInquire(FRAME_IMAGE_FILE, M_SIZE_Y, M_NULL);

   MIL_DOUBLE ResizeFactor = 1.0;

   // Inquire size of the display.
   MIL_ID MilOverlayImage;
   MdispInquire(MilDisplay, M_OVERLAY_ID, &MilOverlayImage);
   MIL_INT DisplaySizeX = MbufInquire(MilOverlayImage, M_SIZE_X, M_NULL);
   MIL_INT DisplaySizeY = MbufInquire(MilOverlayImage, M_SIZE_Y, M_NULL);

   // Both PictureSizeX and PictureSizeY must be smaller than 1/4 the camera image size.
   if (PictureSizeX > DisplaySizeX * MAX_PICTURE_RATIO)
      {
      ResizeFactor = DisplaySizeX * MAX_PICTURE_RATIO / PictureSizeX;
      }
   if (PictureSizeY > DisplaySizeY * MAX_PICTURE_RATIO)
      {
      MIL_DOUBLE ResizeFactorY = DisplaySizeY * MAX_PICTURE_RATIO / PictureSizeY;
      if (ResizeFactorY < ResizeFactor)
         ResizeFactor = ResizeFactorY;
      }

   PictureSizeX = static_cast<MIL_INT>( PictureSizeX * ResizeFactor );
   PictureSizeY = static_cast<MIL_INT>( PictureSizeY * ResizeFactor );

   // Load frame image for overlay and resize it to picture size.
   MIL_ID MilSystem = MdispInquire(MilDisplay, M_OWNER_SYSTEM, M_NULL);
   MIL_ID MilFrameOriginalImage = MbufRestore(FRAME_IMAGE_FILE, MilSystem, M_NULL);
   MIL_ID MilFrameImage = MbufAllocColor(MilSystem, 3, PictureSizeX, PictureSizeY, 8+M_UNSIGNED, M_IMAGE+M_PROC+M_DISP, M_NULL);
   MimResize(MilFrameOriginalImage, MilFrameImage, M_FILL_DESTINATION, M_FILL_DESTINATION, M_NEAREST_NEIGHBOR);
   MbufFree(MilFrameOriginalImage);

   // Load and resize image.
   MIL_ID MilOriginalPicture = MbufRestore(PictureFilename, MilSystem, M_NULL);
   MIL_ID MilPicture = MbufAllocColor(MilSystem, 3, PictureSizeX, PictureSizeY, 8+M_UNSIGNED, M_IMAGE+M_PROC+M_DISP, M_NULL);
   MimResize(MilOriginalPicture, MilPicture, M_FILL_DESTINATION, M_FILL_DESTINATION, M_BICUBIC+M_REGULAR);
   MbufFree(MilOriginalPicture);

   // Adds the frame to the overlay images. Only pixels that are not transparent are copied.
   MbufCopyCond(MilFrameImage, MilPicture, MilFrameImage, M_NOT_EQUAL, FRAME_TRANSPARENT_COLOR);

   // Free the frame image.
   MbufFree(MilFrameImage);

   // Compute position to copy the overlay images (bottom-right corner of overlay).
   MIL_INT OverlayOffsetX = DisplaySizeX - PictureSizeX - FRAME_BORDER;
   MIL_INT OverlayOffsetY = DisplaySizeY - PictureSizeY - FRAME_BORDER;

   MbufCopyClip(MilPicture, MilOverlayImage, OverlayOffsetX, OverlayOffsetY);

   MbufFree(MilPicture);
#endif
   }
