//***************************************************************************************/
// 
// File name: MatroxGatorEye.cpp  
//
// Synopsis:  This program contains an example of 3d reconstruction by interfacing with
//            a Matrox GatorEye, using the 3dmap module.
//            See the PrintHeader() function below for detailed description.
//
//
#include <mil.h>

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
             MIL_TEXT("MatroxGatorEye\n\n")

             MIL_TEXT("[SYNOPSIS]\n")
             MIL_TEXT("This programs calibrates a 3d reconstruction setup, which consists of a\n")
             MIL_TEXT("Matrox GatorEye camera and a laser. It then scans an object to generate\n")
             MIL_TEXT("its depth map. The GatorEye hardware is used to extract the laser line.\n\n")

             MIL_TEXT("Note: this example can be compiled to either acquire images using a Matrox\n")
             MIL_TEXT("GatorEye camera, or reload images from disk.\n\n")

             MIL_TEXT("[MODULES USED]\n")
             MIL_TEXT("Modules used: application, system, display, digitizer, buffer, graphic,\n")
             MIL_TEXT("              calibration, 3d reconstruction.\n\n")

             MIL_TEXT("Press <Enter> to start.\n\n"));
   MosGetch();
   }

//*****************************************************************************
// Constants.
//*****************************************************************************

// Set this constant to false to be able to run this example without actually grabbing images
// with a Matrox GatorEye camera. Images will be loaded from disk and camera configuration
// is hard-coded in the example. To try it with a camera, set this to true. You will also
// probably need to replace some comments starting with CONVEYOR_CODE.
static const bool USE_REAL_CAMERA = false;

static MIL_CONST_TEXT_PTR SYSTEM_DESCRIPTOR = (USE_REAL_CAMERA ? M_SYSTEM_DEFAULT : M_SYSTEM_HOST);

// Number of allocated grab buffers that will be used by MdigProcess().
static const MIL_INT NB_DIG_PROCESS_BUFFERS = 2;

// Calibration values for M_DEPTH_CORRECTION context type.
static const MIL_INT NB_REFERENCE_PLANES = 5;

// Number of laser data buffers to use during the scanning phase.
static const MIL_INT NB_LASER_GRABS = 6;

// Simple separator line for console output.
static const MIL_TEXT_CHAR* const SEPARATOR = MIL_TEXT("--------------------\n\n");

//*****************************************************************************
// File names

// Directory containing all images used for the example.
#define EXAMPLE_IMAGE_PATH    M_IMAGE_PATH MIL_TEXT("MatroxGatorEye/")

static const MIL_INT MAX_FILENAME_LEN = 256;

static const MIL_TEXT_CHAR* const CALIBRATION_GRID_FILE           = EXAMPLE_IMAGE_PATH MIL_TEXT("CalibGrid.mim");
static const MIL_TEXT_CHAR* const FULL_MODE_CALIBRATION_LINE_FILE = EXAMPLE_IMAGE_PATH MIL_TEXT("CalibLine.mim");

static const MIL_TEXT_CHAR* const DEPTH_ONLY_CALIBRATION_LINE_FILENAME_PATTERN = EXAMPLE_IMAGE_PATH MIL_TEXT("CalibLine%d.mim");
static const MIL_TEXT_CHAR* const LASER_GRAB_FILENAME_PATTERN                  = EXAMPLE_IMAGE_PATH MIL_TEXT("Scan%d.mim");

//*****************************************************************************
// Camera parameters

static const MIL_DOUBLE GATOREYE_EXPOSURE_TIME = 17000.0;  // in ns

// Laser line extraction options.
static const MIL_INT32 GATOREYE_FIXED_POINT   =   5;  // in bits
static const MIL_INT32 GATOREYE_MIN_INTENSITY =  80;  // in gray levels
static const MIL_INT32 GATOREYE_PEAK_WIDTH    =  10;  // in pixels

static const bool GATOREYE_OUTPUT_INTENSITY = true;
static const MIL_TEXT_PTR GATOREYE_FORMAT = (GATOREYE_OUTPUT_INTENSITY ? MIL_TEXT("PositionAndIntensity") : MIL_TEXT("Position"));

// Desired ROI when grabbing full frames, e.g. for camera calibration.
static const MIL_INT FULL_IMAGE_OFFSET_X  =    0;  // in pixels
static const MIL_INT FULL_IMAGE_OFFSET_Y  =    0;  // in pixels
static const MIL_INT FULL_IMAGE_SIZE_X    = 1280;  // in pixels
static const MIL_INT FULL_IMAGE_SIZE_Y    =  960;  // in pixels

// Desired ROI when extracting laser line.
static const MIL_INT LASER_ROI_OFFSET_X =    0;    // in pixels
static const MIL_INT LASER_ROI_OFFSET_Y =  416;    // in pixels
static const MIL_INT LASER_ROI_SIZE_X   = 1280;    // in pixels
static const MIL_INT LASER_ROI_SIZE_Y   =  320;    // in pixels

//*****************************************************************************
// Dependent constant values.

// M_GRAB attributes is necessary only if we are using a real digitizer.
static const MIL_INT GRAB_ATTR = (USE_REAL_CAMERA ? M_GRAB : 0);

// Position requires two bytes and intensity, one byte.
static const MIL_INT NB_BYTES_PER_PIXEL = (GATOREYE_OUTPUT_INTENSITY ? 3 : 2);

// How many laser data lines can fit in a single grab image?
static const MIL_INT MAX_NB_SCANS_PER_GRAB = LASER_ROI_SIZE_Y / NB_BYTES_PER_PIXEL; // integer division intended.

// How many laser data lines will be extracted (over all grabbed images)?
static const MIL_INT NB_TOTAL_LASER_LINES = NB_LASER_GRABS * MAX_NB_SCANS_PER_GRAB;

//*****************************************************************************
// Parameters for McalGrid(). Depends on the calibration grid used.

static const MIL_INT    ROW_NUMBER     = 15;
static const MIL_INT    COLUMN_NUMBER  = 16;
static const MIL_DOUBLE ROW_SPACING    = 5.0;             // in mm
static const MIL_DOUBLE COLUMN_SPACING = 5.0;             // in mm
static const MIL_INT    GRID_TYPE      = M_CIRCLE_GRID;

//*****************************************************************************
// Parameters used during 3d reconstruction setup calibration.

// Calibration heights in mm.
static const MIL_DOUBLE CORRECTED_DEPTHS[NB_REFERENCE_PLANES] = {0.0, 12.0, 24.0, 36.0, 48.0};

// (depth in world units) * SCALE_FACTOR gives gray levels. Unit: gray level per mm.
static const MIL_DOUBLE SCALE_FACTOR = 1000.0;

//*****************************************************************************
// Depth map generation parameters.

static const MIL_INT    DEPTH_MAP_SIZE_X = 640;     // in pixels
static const MIL_INT    DEPTH_MAP_SIZE_Y = 480;     // in pixels
static const MIL_DOUBLE GAP_DEPTH        = 1.5;     // in mm
static const MIL_DOUBLE CONVEYOR_SPEED   = 9.3;     // in mm/s

// Size of the DirectX display.
static const MIL_INT D3D_DISPLAY_SIZE_X = 800;      // in pixels
static const MIL_INT D3D_DISPLAY_SIZE_Y = 800;      // in pixels

//*****************************************************************************
// Functions declarations.
//*****************************************************************************

// Main example functions.
void GatorEyeExample(MIL_ID MilSystem, MIL_ID MilDisplay, MIL_ID MilDigitizer, MIL_INT ContextType);
bool Initialize(MIL_ID  MilSystem      ,
                MIL_ID  MilDisplay     ,
                MIL_ID  MilDigitizer   ,
                MIL_INT ContextType    ,
                MIL_ID* pMilCameraImage,
                MIL_ID* pMilLaser      ,
                MIL_ID* pMilScan       );
bool Calibrate(MIL_ID MilSystem     ,
               MIL_ID MilDisplay    ,
               MIL_ID MilDigitizer  ,
               MIL_ID MilCameraImage,
               MIL_ID MilLaser      ,
               MIL_ID MilScan       );
bool Scan(MIL_ID MilSystem     ,
          MIL_ID MilDisplay    ,
          MIL_ID MilDigitizer  ,
          MIL_ID MilCameraImage,
          MIL_ID MilLaser      ,
          MIL_ID MilScan       );
bool ShowResults(MIL_ID MilSystem, MIL_ID MilDisplay, MIL_ID MilScan);

// Functions used to change camera mode and adjust ROI.
void SwitchToLaserExtraction(MIL_ID MilDigitizer, MIL_INT64 NbScansPerGrab);
void SwitchToImage(MIL_ID MilDigitizer);

// Intermediate steps.
bool CalibrateCamera(MIL_ID  MilSystem      ,
                     MIL_ID  MilDisplay     ,
                     MIL_ID  MilDigitizer   ,
                     MIL_ID  MilCameraImage ,
                     MIL_ID* pMilCalibration);
void AddLaserLinesFullMode(MIL_ID MilSystem     ,
                           MIL_ID MilDisplay    ,
                           MIL_ID MilDigitizer  ,
                           MIL_ID MilCameraImage,
                           MIL_ID MilLaser      ,
                           MIL_ID MilScan       );
void AddLaserLinesDepthOnly(MIL_ID MilSystem     ,
                            MIL_ID MilDisplay    ,
                            MIL_ID MilDigitizer  ,
                            MIL_ID MilCameraImage,
                            MIL_ID MilLaser      ,
                            MIL_ID MilScan       );
void DisplayLaserLine(MIL_ID MilSystem, MIL_ID MilDisplay, MIL_ID MilScan);

// Functions used to allocate/free buffers used by the GatorEye to extract laser lines.
void AllocateBuffersForLaserExtraction(MIL_ID  MilSystem            ,
                                       MIL_INT LaserROISizeX        ,
                                       MIL_INT NbLaserLinesToExtract,
                                       MIL_INT NbGrabBuffers        ,
                                       MIL_ID* MilGrabImageArray    ,
                                       MIL_ID* MilNoPaddingImagePtr ,
                                       MIL_ID* MilPositionImagePtr  ,
                                       MIL_ID* MilIntensityImagePtr );
void FreeBuffersForLaserExtraction(MIL_INT NbGrabBuffers    ,
                                   MIL_ID* MilGrabImageArray,
                                   MIL_ID  MilNoPaddingImage,
                                   MIL_ID  MilPositionImage ,
                                   MIL_ID  MilIntensityImage);

// Function used to query the camera frame rate in laser extraction mode.
MIL_DOUBLE GetCameraFrameRate(MIL_ID MilDigitizer);

// Data structure passed to the MdigProcess() hook function.
struct AddScanProcFctDataStruct
   {
   MIL_ID MilLaser;
   MIL_ID MilScan;
   MIL_ID MilGrabImageArray[NB_DIG_PROCESS_BUFFERS];
   MIL_ID MilNoPaddingImage;
   MIL_ID MilPositionImage;
   MIL_ID MilIntensityImage;
   };

// MdigProcess() hook function.
MIL_INT MFTYPE AddScanProcFct(MIL_INT HookType, MIL_ID MilEvent, void* UserDataPtr);
void AddGatorEyeScan(const AddScanProcFctDataStruct* pAddScanProcFctData, MIL_INT BufIndex);

//*****************************************************************************
// Main.
//*****************************************************************************
int MosMain(void)
   {
   PrintHeader();

   // Allocate MIL objects. 
   MIL_ID MilApplication = MappAlloc(M_NULL, M_DEFAULT, M_NULL);
   MIL_ID MilSystem      = MsysAlloc(M_DEFAULT, SYSTEM_DESCRIPTOR, M_DEFAULT, M_DEFAULT, M_NULL);
   MIL_ID MilDisplay     = MdispAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_WINDOWED, M_NULL);
   MIL_ID MilDigitizer   = M_NULL;

   if (USE_REAL_CAMERA)
      {
      // If necessary, allocate digitizer and setup some camera options that will not
      // change in the course of the example.
      MdigAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &MilDigitizer);

      // When grabbing multiple laser lines, grab timeout must be increased (30s).
      MdigControl(MilDigitizer, M_GRAB_TIMEOUT, 50000);

      // Set exposure time so that it won't limit the frame rate during laser extraction.
      MdigControlFeature(MilDigitizer, M_FEATURE_VALUE, MIL_TEXT("ExposureTime"), M_TYPE_DOUBLE, &GATOREYE_EXPOSURE_TIME);

      // Setup laser line extraction options.
      MdigControlFeature(MilDigitizer, M_FEATURE_VALUE          , MIL_TEXT("LocatePeakFixedPointPosition"), M_TYPE_MIL_INT32  , &GATOREYE_FIXED_POINT  );
      MdigControlFeature(MilDigitizer, M_FEATURE_VALUE          , MIL_TEXT("LocatePeakMinIntensity"      ), M_TYPE_MIL_INT32  , &GATOREYE_MIN_INTENSITY);
      MdigControlFeature(MilDigitizer, M_FEATURE_VALUE          , MIL_TEXT("LocatePeakWidth"             ), M_TYPE_MIL_INT32  , &GATOREYE_PEAK_WIDTH   );
      MdigControlFeature(MilDigitizer, M_FEATURE_VALUE_AS_STRING, MIL_TEXT("LocatePeakFormat"            ), M_TYPE_ENUMERATION, GATOREYE_FORMAT        );

      // CONVEYOR_CODE: Initialize conveyor.
      }

   // Run the depth correction example.
   MosPrintf(MIL_TEXT("M_DEPTH_CORRECTION 3d reconstruction mode\n")
             MIL_TEXT("=========================================\n\n")
             
             MIL_TEXT("For this calibration mode, the following steps will be executed:\n")
             MIL_TEXT("   - Calibrating the 3d reconstruction setup.\n")
             MIL_TEXT("   - Scanning the object.\n\n"));
   GatorEyeExample(MilSystem, MilDisplay, MilDigitizer, M_DEPTH_CORRECTION);

   // Run the calibrated camera example.
   MosPrintf(MIL_TEXT("M_CALIBRATED_CAMERA_LINEAR_MOTION 3d reconstruction mode\n")
             MIL_TEXT("========================================================\n\n")
             
             MIL_TEXT("For this calibration mode, the following steps will be executed:\n")
             MIL_TEXT("   - Calibrating the camera.\n")
             MIL_TEXT("   - Calibrating the 3d reconstruction setup.\n")
             MIL_TEXT("   - Scanning the object.\n\n"));
   GatorEyeExample(MilSystem, MilDisplay, MilDigitizer, M_CALIBRATED_CAMERA_LINEAR_MOTION);

   // Free MIL objects.
   if (MilDigitizer != M_NULL)
      MdigFree(MilDigitizer);
   MdispFree(MilDisplay);
   MsysFree(MilSystem);
   MappFree(MilApplication);

   return 0;
   }

//*****************************************************************************
// Function that calibrates a 3d reconstruction setup with a Matrox GatorEye camera
// and scans an object to generate its depth map. Laser line extraction is performed
// by the camera.
//
//   MilSystem       (in)  System on which to allocate MIL objects.
//   MilDisplay      (in)  Display to use.
//   MilDigitizer    (in)  Digitizer to use to grab images using the Matrox GatorEye.
//   ContextType     (in)  3d reconstruction mode (M_DEPTH_CORRECTION or M_CALIBRATED_CAMERA_LINEAR_MOTION).
//*****************************************************************************
void GatorEyeExample(MIL_ID MilSystem, MIL_ID MilDisplay, MIL_ID MilDigitizer, MIL_INT ContextType)
   {
   // We'll check if each step below worked.
   bool EverythingIsOK;

   // Initialization phase:
   //   Reset conveyor position
   //   Allocate camera image buffer and select it to display
   //   Allocate 3dmap objects and set some parameters
   //   Set the GatorEye camera into full-frame mode
   MIL_ID MilCameraImage, MilLaser, MilScan;
   EverythingIsOK = Initialize(MilSystem, MilDisplay, MilDigitizer, ContextType,
                               &MilCameraImage, &MilLaser, &MilScan);

   // Calibration phase:
   //   Calibrate the camera if necessary (grab a calibration grid)
   //   Grab and add calibration laser lines in result buffer
   //   Calibrate the 3d reconstruction context
   if (EverythingIsOK)
      EverythingIsOK = Calibrate(MilSystem, MilDisplay, MilDigitizer, MilCameraImage, MilLaser, MilScan);

   // Scanning phase:
   //   Allocate necessary buffers for grabbing and reinterpreting GatorEye laser data
   //   Call MdigProcess() while the conveyor is moving the object under the laser line
   //   Free MdigProcess() buffers
   if (EverythingIsOK)
      EverythingIsOK = Scan(MilSystem, MilDisplay, MilDigitizer, MilCameraImage, MilLaser, MilScan);

   // Results phase:
   //   Setup 3dmap for depth map generation
   //   Allocate buffers to hold the depth map
   //   Show depth map (using D3D if applicable and possible)
   //   Reset display and free depth map buffers
   if (EverythingIsOK)
      EverythingIsOK = ShowResults(MilSystem, MilDisplay, MilScan);

   // Free MIL objects.
   M3dmapFree(MilScan);
   M3dmapFree(MilLaser);
   MbufFree(MilCameraImage);
   }

//*****************************************************************************
// Initialization phase:
//   Reset conveyor position
//   Allocate camera image buffer and select it to display
//   Allocate 3dmap objects and set some parameters
//   Set the GatorEye camera into full-frame mode
//
//   MilSystem       (in)  System on which to allocate MIL objects.
//   MilDisplay      (in)  Display to use.
//   MilDigitizer    (in)  Digitizer to use to grab images using the Matrox GatorEye.
//   ContextType     (in)  3d reconstruction mode (M_DEPTH_CORRECTION or M_CALIBRATED_CAMERA_LINEAR_MOTION).
//   pMilCameraImage (out) Pointer to image buffer to allocate.
//   pMilLaser       (out) Pointer to 3d reconstruction context to allocate.
//   pMilScan        (out) Pointer to 3d reconstruction result buffer to allocate.
//
// Returns true if no error occurred.
//*****************************************************************************
bool Initialize(MIL_ID  MilSystem      ,
                MIL_ID  MilDisplay     ,
                MIL_ID  MilDigitizer   ,
                MIL_INT ContextType    ,
                MIL_ID* pMilCameraImage,
                MIL_ID* pMilLaser      ,
                MIL_ID* pMilScan       )
   {
   bool EverythingIsOK = true;

   // CONVEYOR_CODE: Reset conveyor position.
   
   // Allocate buffer to hold images returned by the camera in full-frame mode.
   MbufAlloc2d(MilSystem, FULL_IMAGE_SIZE_X, FULL_IMAGE_SIZE_Y,
               8+M_UNSIGNED, M_IMAGE+M_PROC+GRAB_ATTR+M_DISP, pMilCameraImage);

   // Allocate 3d reconstruction context set some parameters.
   M3dmapAlloc(MilSystem, M_LASER, ContextType, pMilLaser);
   M3dmapControl(*pMilLaser, M_DEFAULT, M_EXTRACTION_FIXED_POINT, GATOREYE_FIXED_POINT);

   if (ContextType == M_CALIBRATED_CAMERA_LINEAR_MOTION)
      {
      // Query frame rate from the camera.
      MIL_DOUBLE FrameRate = GetCameraFrameRate(MilDigitizer);

      // Compute scanning speed in mm/frame.
      // CONVEYOR_SPEED is in mm/s, FrameRate is in frame/s.
      // CONVEYOR_SPEED / FrameRate is in mm/frame
      // Negative sign implies the object is moving AWAY from the camera.
      MIL_DOUBLE ScanSpeed = -CONVEYOR_SPEED / FrameRate;
      M3dmapControl(*pMilLaser, M_DEFAULT, M_SCAN_SPEED, ScanSpeed);

      // Set ROI used for laser line extraction by the GatorEye.
      M3dmapControl(*pMilLaser, M_DEFAULT, M_EXTRACTION_CHILD_OFFSET_X, LASER_ROI_OFFSET_X-FULL_IMAGE_OFFSET_X);
      M3dmapControl(*pMilLaser, M_DEFAULT, M_EXTRACTION_CHILD_OFFSET_Y, LASER_ROI_OFFSET_Y-FULL_IMAGE_OFFSET_Y);
      }

   // Allocate 3d reconstruction result buffer and set some parameters.
   M3dmapAllocResult(MilSystem, M_LASER_DATA, M_DEFAULT, pMilScan);
   M3dmapControl(*pMilScan, M_DEFAULT, M_MAX_FRAMES, NB_TOTAL_LASER_LINES);

   // Set camera and digitizer to full-frame image mode.
   SwitchToImage(MilDigitizer);

   // Display grabbed image.
   MbufClear(*pMilCameraImage, 0.0);
   if (USE_REAL_CAMERA)
      MdispSelect(MilDisplay, *pMilCameraImage);

   return EverythingIsOK;
   }

//*****************************************************************************
// Calibration phase:
//   Calibrate the camera if necessary (grab a calibration grid)
//   Grab and add calibration laser lines in result buffer
//   Calibrate the 3d reconstruction context
//
//   MilSystem       (in)  System on which to allocate MIL objects, if necessary.
//   MilDisplay      (in)  Display to use.
//   MilDigitizer    (in)  Digitizer to use to grab images using the Matrox GatorEye.
//   MilCameraImage  (in)  Image buffer selected to display.
//   MilLaser        (in)  3d reconstruction context to calibrate.
//   MilScan         (in)  3d reconstruction result buffer that will accumulate calibration data.
//
// Returns true if no error occurred.
//*****************************************************************************
bool Calibrate(MIL_ID MilSystem     ,
               MIL_ID MilDisplay    ,
               MIL_ID MilDigitizer  ,
               MIL_ID MilCameraImage,
               MIL_ID MilLaser      ,
               MIL_ID MilScan       )
   {
   bool EverythingIsOK = true;

   // Get 3d reconstruction mode.
   MIL_INT ContextType = M3dmapInquire(MilLaser, M_DEFAULT, M_LASER_CONTEXT_TYPE, M_NULL);

   // Calibrate the camera (necessary only for M_CALIBRATED_CAMERA_LINEAR_MOTION) by
   // using a calibration grid at Z=0.
   MIL_ID MilCalibration = M_NULL;
   if (ContextType == M_CALIBRATED_CAMERA_LINEAR_MOTION)
      EverythingIsOK = CalibrateCamera(MilSystem, MilDisplay, MilDigitizer, MilCameraImage, &MilCalibration);

   if (EverythingIsOK)
      {
      MosPrintf(MIL_TEXT("Calibrating the 3d reconstruction setup\n")
                MIL_TEXT("---------------------------------------\n\n"));

      // Add calibration laser lines in result buffer.
      if (ContextType == M_CALIBRATED_CAMERA_LINEAR_MOTION)
         AddLaserLinesFullMode(MilSystem, MilDisplay, MilDigitizer, MilCameraImage, MilLaser, MilScan);
      else
         AddLaserLinesDepthOnly(MilSystem, MilDisplay, MilDigitizer, MilCameraImage, MilLaser, MilScan);

      // Calibrate the 3d reconstruction context.
      M3dmapCalibrate(MilLaser, MilScan, MilCalibration, M_DEFAULT);

      if (M3dmapInquire(MilLaser, M_DEFAULT, M_CALIBRATION_STATUS, M_NULL) == M_CALIBRATED)
         {
         MosPrintf(MIL_TEXT("The calibration of the 3d reconstruction setup was successful.\n\n"));
         }
      else
         {
         MosPrintf(MIL_TEXT("The calibration of the 3d reconstruction setup has failed.\n")
                   MIL_TEXT("Press <Enter> to end.\n\n"));
         MosGetch();
         EverythingIsOK = false;
         }
      }

   // After calibrating the 3d reconstruction setup, we don't need the calibration context anymore.
   if (MilCalibration != M_NULL)
      McalFree(MilCalibration);

   return EverythingIsOK;
   }

//*****************************************************************************
// Scanning phase:
//   Allocate necessary buffers for grabbing and reinterpreting GatorEye laser data
//   Call MdigProcess() while the conveyor is moving the object under the laser line
//   Free MdigProcess() buffers
//
//   MilSystem       (in)  System on which to allocate MIL objects, if necessary.
//   MilDisplay      (in)  Display to use.
//   MilDigitizer    (in)  Digitizer to use to extract laser data using the Matrox GatorEye.
//   MilCameraImage  (in)  Image buffer selected to display.
//   MilLaser        (in)  Calibrated 3d reconstruction context.
//   MilScan         (in)  3d reconstruction result buffer that will accumulate object data.
//
// Returns true if no error occurred.
//*****************************************************************************
bool Scan(MIL_ID MilSystem     ,
          MIL_ID MilDisplay    ,
          MIL_ID MilDigitizer  ,
          MIL_ID MilCameraImage,
          MIL_ID MilLaser      ,
          MIL_ID MilScan       )
   {
   bool EverythingIsOK = true;

   MosPrintf(MIL_TEXT("Scanning the object\n")
             MIL_TEXT("-------------------\n\n"));

   // Empty all result buffer contents. It will now be reused for extracting corrected depths.
   M3dmapAddScan(M_NULL, MilScan, M_NULL, M_NULL, M_NULL, M_DEFAULT, M_RESET);

   // Fill MdigProcess() hook function structure to use during scanning.
   // We will also use this in stand-alone mode, using only BufIndex == 0.
   AddScanProcFctDataStruct AddScanProcFctData;
   AddScanProcFctData.MilLaser = MilLaser;
   AddScanProcFctData.MilScan  = MilScan;
   AllocateBuffersForLaserExtraction(MilSystem                            ,
                                     LASER_ROI_SIZE_X                     ,
                                     MAX_NB_SCANS_PER_GRAB                ,
                                     NB_DIG_PROCESS_BUFFERS               ,
                                      AddScanProcFctData.MilGrabImageArray,
                                     &AddScanProcFctData.MilNoPaddingImage,
                                     &AddScanProcFctData.MilPositionImage ,
                                     &AddScanProcFctData.MilIntensityImage);

   // Give the user the time to position the object to scan.
   if (USE_REAL_CAMERA)
      {
      // Grab a sequence of image until ENTER is pressed.
      MdigGrabContinuous(MilDigitizer, MilCameraImage);

      MosPrintf(MIL_TEXT("Adjust the object's position.\n")
                MIL_TEXT("Press <Enter> to start scanning.\n\n"));
      MosGetch();
      MdigHalt(MilDigitizer);
      }

   // Display the uncorrected depth map during scanning.
   MbufClear(AddScanProcFctData.MilPositionImage, 0.0);
   MdispSelect(MilDisplay, AddScanProcFctData.MilPositionImage);
   MdispControl(MilDisplay, M_VIEW_BIT_SHIFT, 6          );
   MdispControl(MilDisplay, M_VIEW_MODE     , M_BIT_SHIFT);

   MosPrintf(MIL_TEXT("The object is being scanned. Raw (uncorrected) depth map coming from the\n")
             MIL_TEXT("GatorEye is displayed.\n\n"));

   if (USE_REAL_CAMERA)
      {
      // Use Matrox GatorEye to perform laser line extraction.
      SwitchToLaserExtraction(MilDigitizer, MAX_NB_SCANS_PER_GRAB);

      // CONVEYOR_CODE: Start conveyor and wait for its speed to become constant.

      // Grab laser line data with buffering.
      MdigProcess(MilDigitizer,
                  AddScanProcFctData.MilGrabImageArray,
                  NB_DIG_PROCESS_BUFFERS,
                  M_SEQUENCE+M_COUNT(NB_LASER_GRABS),
                  M_SYNCHRONOUS,
                  &AddScanProcFct,
                  &AddScanProcFctData);

      // CONVEYOR_CODE: Stop conveyor.
      }
   else
      {
      MosPrintf(MIL_TEXT("GatorEye data obtained during scanning are reloaded from disk.\n\n"));

      // Simulate the MdigProcess() loop by reloading Matrox GatorEye laser data buffers.
      const MIL_INT BufIndexToUse = 0;
      for (MIL_INT ScanIndex = 0; ScanIndex < NB_LASER_GRABS; ++ScanIndex)
         {
         // Simulated grab delay of 1s.
         MosSleep(1000);

         // Reload laser data buffer from disk.
         MIL_TEXT_CHAR Filename[MAX_FILENAME_LEN];
         MosSprintf(Filename, MAX_FILENAME_LEN, LASER_GRAB_FILENAME_PATTERN, ScanIndex);
         MbufLoad(Filename, AddScanProcFctData.MilGrabImageArray[BufIndexToUse]);

         // Call hook function manually.
         AddGatorEyeScan(&AddScanProcFctData, BufIndexToUse);
         }
      }

   // Reset display.
   MdispSelect(MilDisplay, M_NULL);
   MdispControl(MilDisplay, M_VIEW_MODE, M_DEFAULT);

   // Free MIL objects in hook function structure.
   FreeBuffersForLaserExtraction(NB_DIG_PROCESS_BUFFERS              ,
                                 AddScanProcFctData.MilGrabImageArray,
                                 AddScanProcFctData.MilNoPaddingImage,
                                 AddScanProcFctData.MilPositionImage ,
                                 AddScanProcFctData.MilIntensityImage);

   return EverythingIsOK;
   }

//*****************************************************************************
// Results phase:
//   Setup 3dmap for depth map generation
//   Allocate buffers to hold the depth map
//   Show depth map (using D3D if applicable and possible)
//   Reset display and free depth map buffers
//
//   MilSystem       (in)  System on which to allocate depth map.
//   MilDisplay      (in)  Display to use (if not using D3D).
//   MilScan         (in)  3d reconstruction result buffer that contains data to show.
//
// Returns true if no error occurred.
//*****************************************************************************
bool ShowResults(MIL_ID MilSystem, MIL_ID MilDisplay, MIL_ID MilScan)
   {
   bool EverythingIsOK = true;

   // Check if current result are partially or fully corrected.
   MIL_INT CorrectionState;
   M3dmapGetResult(MilScan, M_DEFAULT, M_DEPTH_CORRECTION_STATE+M_TYPE_MIL_INT, &CorrectionState);

   // Set depth map generation parameters according to 3dmap context type.
   M3dmapControl(MilScan, M_DEFAULT, M_FILL_MODE                 , M_X_THEN_Y  );
   M3dmapControl(MilScan, M_DEFAULT, M_FILL_SHARP_ELEVATION      , M_MIN       );
   M3dmapControl(MilScan, M_DEFAULT, M_FILL_SHARP_ELEVATION_DEPTH, GAP_DEPTH   );
   M3dmapControl(MilScan, M_DEFAULT, M_EXTRACTION_SCALE_MODE     , M_AUTO_SCALE);

   // Get informations to allocate the corrected depth map buffers correctly.
   MIL_INT DepthMapSizeX, DepthMapSizeY, DepthMapType, IntensityMapType;
   if (CorrectionState == M_FULLY_CORRECTED)
      {
      DepthMapSizeX = DEPTH_MAP_SIZE_X;
      DepthMapSizeY = DEPTH_MAP_SIZE_Y;
      DepthMapType  = 16+M_UNSIGNED;
      }
   else
      {
      M3dmapGetResult(MilScan, M_DEFAULT, M_CORRECTED_DEPTH_MAP_SIZE_X     +M_TYPE_MIL_INT, &DepthMapSizeX);
      M3dmapGetResult(MilScan, M_DEFAULT, M_CORRECTED_DEPTH_MAP_SIZE_Y     +M_TYPE_MIL_INT, &DepthMapSizeY);
      M3dmapGetResult(MilScan, M_DEFAULT, M_CORRECTED_DEPTH_MAP_BUFFER_TYPE+M_TYPE_MIL_INT, &DepthMapType );
      }

   if (GATOREYE_OUTPUT_INTENSITY)
      M3dmapGetResult(MilScan, M_DEFAULT, M_INTENSITY_MAP_BUFFER_TYPE+M_TYPE_MIL_INT, &IntensityMapType);

   // Allocate depth map buffers.
   MIL_ID MilDepthMap = MbufAlloc2d(MilSystem, DepthMapSizeX, DepthMapSizeY, DepthMapType    , M_IMAGE+M_PROC+M_DISP, M_NULL);
   MIL_ID MilIntensityMap = M_NULL;
   
   if (GATOREYE_OUTPUT_INTENSITY)
      MbufAlloc2d(MilSystem, DepthMapSizeX, DepthMapSizeY, IntensityMapType, M_IMAGE+M_PROC+M_DISP, &MilIntensityMap);

   // Get corrected depth maps from accumulated information in the result buffer.
   M3dmapExtract(MilScan, MilDepthMap, MilIntensityMap, M_CORRECTED_DEPTH_MAP, M_DEFAULT, M_DEFAULT);

   // Display corrected depth map.
   bool D3DDisplayShown = false;

#if USE_D3D_DISPLAY
   MIL_DISP_D3D_HANDLE DispHandle;
   if (CorrectionState == M_FULLY_CORRECTED)
      {
      // This mode generates fully calibrated 3d data, let's try to allocate D3D display.
      DispHandle = MdepthD3DAlloc(MilDepthMap, MilIntensityMap,
                                  D3D_DISPLAY_SIZE_X, D3D_DISPLAY_SIZE_Y,
                                  M_DEFAULT, M_DEFAULT, M_DEFAULT, 
                                  M_DEFAULT, M_DEFAULT, M_DEFAULT, 0);

      if (DispHandle != NULL)
         {
         // Show D3D display, with instructions.
         MosPrintf(MIL_TEXT("The 3d model of the scanned object is displayed.\n\n"));
         MdispD3DShow(DispHandle);
         MdispD3DPrintHelp(DispHandle);
         D3DDisplayShown = true;
         }
      }
#endif
   
   // Show depth map in Mil display, if D3D is not used.
   if (!D3DDisplayShown)
      {
      MosPrintf(MIL_TEXT("The depth map of the scanned object is displayed.\n"));
      MdispControl(MilDisplay, M_VIEW_MODE, M_AUTO_SCALE);
      MdispSelect(MilDisplay, MilDepthMap);
      }

   if (CorrectionState == M_FULLY_CORRECTED)
      MosPrintf(MIL_TEXT("Press <Enter> to end.\n\n"));
   else
      MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   // Reset display and free D3D display, if necessary.
   if (!D3DDisplayShown)
      {
      MdispSelect(MilDisplay, M_NULL);
      MdispControl(MilDisplay, M_VIEW_MODE, M_DEFAULT);
      }

#if USE_D3D_DISPLAY
   if (D3DDisplayShown)
      {
      MdispD3DHide(DispHandle);
      MdispD3DFree(DispHandle);
      }
#endif

   // Free MIL objects.
   if (GATOREYE_OUTPUT_INTENSITY)
      MbufFree(MilIntensityMap);
   MbufFree(MilDepthMap);

   return EverythingIsOK;
   }

//*****************************************************************************
// Hook function for MdigProcess(). Get the profile data returned by the Matrox GatorEye
// camera and feed it to 3dmap.
//
//   HookType     (in)  Type of event that generated the call (M_MODIFIED_BUFFER).
//   MilEvent     (in)  Event identifier to pass to MdigGetHookInfo() to inquire about the hooked event.
//   UserDataPtr  (in)  User data pointer that was passed to MdigProcess(). 
//*****************************************************************************
MIL_INT MFTYPE AddScanProcFct(MIL_INT HookType, MIL_ID MilEvent, void* UserDataPtr)
   {
   const AddScanProcFctDataStruct* pAddScanProcFctData = static_cast<const AddScanProcFctDataStruct*>(UserDataPtr);

   MIL_INT BufIndex;
   MdigGetHookInfo(MilEvent, M_MODIFIED_BUFFER+M_BUFFER_INDEX, &BufIndex);

   AddGatorEyeScan(pAddScanProcFctData, BufIndex);

   return M_NULL;
   }

//*****************************************************************************
// Get the profile data returned by the Matrox GatorEye camera and feed it to 3dmap.
// Will be called inside MdigProcess() hook or directly in stand-alone mode.
//
//   pAddScanProcFctData   (in)  Structure containing all necessary objects to add a new GatorEye laser scan.
//   BufIndex              (in)  Index of the current grab buffer.
//*****************************************************************************
void AddGatorEyeScan(const AddScanProcFctDataStruct* pAddScanProcFctData, MIL_INT BufIndex)
   {
   // Copy the grabbed image into a host buffer to remove padding.
   MbufCopy(pAddScanProcFctData->MilGrabImageArray[BufIndex], pAddScanProcFctData->MilNoPaddingImage);

   // MIL is unaware that modifying MilNoPaddingImage also modifies MilPositionImage and
   // MilIntensityImage. For the purpose of this example, we want to display MilPositionImage
   // during the scan, so we must inform MIL that the image has just changed, else the
   // display won't update.
   MbufControl(pAddScanProcFctData->MilPositionImage, M_MODIFIED, M_DEFAULT);

   // Add extracted laser lines info performed by the camera to the 3d reconstruction result.
   M3dmapAddScan(pAddScanProcFctData->MilLaser,
                 pAddScanProcFctData->MilScan,
                 pAddScanProcFctData->MilPositionImage,
                 pAddScanProcFctData->MilIntensityImage,
                 M_NULL,
                 M_DEFAULT,
                 M_LINE_ALREADY_EXTRACTED);
   }

//*****************************************************************************
// Enable Matrox GatorEye laser extraction mode and change ROI if needed.
//   MilDigitizer   (in)  Digitizer Id.
//   NbScansPerGrab (in)  Number of laser lines to extract for each call to MdigGrab().
//*****************************************************************************
void SwitchToLaserExtraction(MIL_ID MilDigitizer, MIL_INT64 NbScansPerGrab)
   {
   if (USE_REAL_CAMERA)
      {
      // Stop camera.
      MdigControl(MilDigitizer, M_GC_STREAMING_STOP, M_DEFAULT         );

      // Set ROI.
      MdigControl(MilDigitizer, M_SOURCE_OFFSET_X  , 0                 );
      MdigControl(MilDigitizer, M_SOURCE_OFFSET_Y  , 0                 );
      MdigControl(MilDigitizer, M_SOURCE_SIZE_X    , LASER_ROI_SIZE_X  );
      MdigControl(MilDigitizer, M_SOURCE_SIZE_Y    , LASER_ROI_SIZE_Y  );
      MdigControl(MilDigitizer, M_SOURCE_OFFSET_X  , LASER_ROI_OFFSET_X);
      MdigControl(MilDigitizer, M_SOURCE_OFFSET_Y  , LASER_ROI_OFFSET_Y);

      // Enable laser extraction mode.
      MdigControlFeature(MilDigitizer, M_FEATURE_VALUE_AS_STRING, MIL_TEXT("LocatePeakMethod"    ), M_TYPE_ENUMERATION, MIL_TEXT("CenterOfGravity"));
      MdigControlFeature(MilDigitizer, M_FEATURE_VALUE          , MIL_TEXT("LocatePeakFrameCount"), M_TYPE_INT64      , &NbScansPerGrab            );
      }
   }

//*****************************************************************************
// Enable Matrox GatorEye full-frame image mode and change ROI if needed.
//   MilDigitizer   (in)  Digitizer Id.
//*****************************************************************************
void SwitchToImage(MIL_ID MilDigitizer)
   {
   if (USE_REAL_CAMERA)
      {
      // Stop camera.
      MdigControl(MilDigitizer, M_GC_STREAMING_STOP, M_DEFAULT         );

      // Set ROI.
      MdigControl(MilDigitizer, M_SOURCE_OFFSET_X  , 0                 );
      MdigControl(MilDigitizer, M_SOURCE_OFFSET_Y  , 0                 );
      MdigControl(MilDigitizer, M_SOURCE_SIZE_X    , FULL_IMAGE_SIZE_X  );
      MdigControl(MilDigitizer, M_SOURCE_SIZE_Y    , FULL_IMAGE_SIZE_Y  );
      MdigControl(MilDigitizer, M_SOURCE_OFFSET_X  , FULL_IMAGE_OFFSET_X);
      MdigControl(MilDigitizer, M_SOURCE_OFFSET_Y  , FULL_IMAGE_OFFSET_Y);

      // Disable laser extraction mode.
      MdigControlFeature(MilDigitizer, M_FEATURE_VALUE_AS_STRING, MIL_TEXT("LocatePeakMethod"), M_TYPE_ENUMERATION, MIL_TEXT("Off"));
      }
   }

//*****************************************************************************
// Perform camera calibration using a calibration grid.
//
//   MilSystem       (in)  System on which to allocate the calibration context.
//   MilDisplay      (in)  Display to use to show the calibration points.
//   MilDigitizer    (in)  Digitizer to use to grab the calibration grid.
//   MilCameraImage  (in)  Image to grab into, already selected to display.
//   pMilCalibration (out) Pointer to the camera calibration context to allocate and calibrate.
//
// Returns true if camera calibration succeeded, or false otherwise.
//*****************************************************************************
bool CalibrateCamera(MIL_ID  MilSystem      ,
                     MIL_ID  MilDisplay     ,
                     MIL_ID  MilDigitizer   ,
                     MIL_ID  MilCameraImage ,
                     MIL_ID* pMilCalibration)
   {
   bool EverythingIsOK = true;

   MosPrintf(MIL_TEXT("Calibrating the camera\n")
             MIL_TEXT("----------------------\n\n"));

   // First, calibrate the camera by grabbing a calibration grid at Z=0.
   if (USE_REAL_CAMERA)
      {
      // Grab a sequence of image until enter is pressed.
      MdigGrabContinuous(MilDigitizer, MilCameraImage);

      MosPrintf(MIL_TEXT("Place calibration grid at Z=0 mm (laser switched OFF).\n")
                MIL_TEXT("Press <Enter> to calibrate the camera.\n\n"));
      MosGetch();

      // Grab calibration grid image.
      MdigHalt(MilDigitizer);
      }
   else
      {
      // Simply reload the image that would have been grabbed above.
      MbufLoad(CALIBRATION_GRID_FILE, MilCameraImage);
      MosPrintf(MIL_TEXT("The image of the calibration grid has been reloaded from disk.\n\n"));
      }

   // Allocate calibration context in 3D mode.
   McalAlloc(MilSystem, M_TSAI_BASED, M_DEFAULT, pMilCalibration);

   // Calibrate the camera.
   McalGrid(*pMilCalibration, MilCameraImage, 0.0, 0.0, 0.0, ROW_NUMBER, COLUMN_NUMBER,
            ROW_SPACING, COLUMN_SPACING, M_DEFAULT, GRID_TYPE);

   if (McalInquire(*pMilCalibration, M_CALIBRATION_STATUS, M_NULL) == M_CALIBRATED)
      {
      // Calibration succeeded; display calibration points.
      if (!USE_REAL_CAMERA)
         MdispSelect(MilDisplay, MilCameraImage);

      // Enable display overlay.
      MIL_ID MilOverlay;
      MdispControl(MilDisplay, M_OVERLAY, M_ENABLE);
      MdispInquire(MilDisplay, M_OVERLAY_ID, &MilOverlay);
      MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);

      // Draw calibration points.
      MgraColor(M_DEFAULT, M_COLOR_GREEN);
      McalDraw(M_DEFAULT, *pMilCalibration, MilOverlay, M_DRAW_WORLD_POINTS, M_DEFAULT, M_DEFAULT);

      MosPrintf(MIL_TEXT("Camera calibration was successful. Calibration points are displayed in green.\n")
                MIL_TEXT("Press <Enter> to continue.\n\n"));
      MosGetch();

      // Disable display overlay.
      MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);
      MdispControl(MilDisplay, M_OVERLAY, M_DISABLE);

      if (!USE_REAL_CAMERA)
         MdispSelect(MilDisplay, M_NULL);
      }
   else
      {
      // Indicates to caller that calibration failed by freeing the calibration context.
      McalFree(*pMilCalibration);
      *pMilCalibration = M_NULL;
      EverythingIsOK = false;

      MosPrintf(MIL_TEXT("Camera calibration failed.\n")
                MIL_TEXT("Press <Enter> to end.\n\n"));
      MosGetch();
      }

   return EverythingIsOK;
   }

//*****************************************************************************
// Add all necessary laser line info to the 3d reconstruction result buffer so
// that M3dmapCalibrate() can be called.
//
//   MilSystem       (in)  System on which to allocate the grab buffers.
//   MilDisplay      (in)  Display to use to show the extracted laser lines.
//   MilDigitizer    (in)  Digitizer to use to grab the laser line images.
//   MilCameraImage  (in)  Image to grab into (full-frame mode), already selected to display.
//   MilLaser        (in)  3d reconstruction context to calibrate (M_CALIBRATED_CAMERA_LINEAR_MOTION mode).
//   MilScan         (in)  3d reconstruction result buffer that will accumulate calibration scan lines.
//*****************************************************************************
void AddLaserLinesFullMode(MIL_ID MilSystem     ,
                           MIL_ID MilDisplay    ,
                           MIL_ID MilDigitizer  ,
                           MIL_ID MilCameraImage,
                           MIL_ID MilLaser      ,
                           MIL_ID MilScan       )
   {
   // Allocate buffers used by the GatorEye to extract laser lines.
   MIL_ID MilGrabImage, MilNoPaddingImage, MilPositionImage, MilIntensityImage;
   AllocateBuffersForLaserExtraction(MilSystem          ,
                                     LASER_ROI_SIZE_X   ,
                                     1                  , // extract one laser line at a time
                                     1                  , // allocate a single grab buffer
                                     &MilGrabImage      ,
                                     &MilNoPaddingImage ,
                                     &MilPositionImage  ,
                                     &MilIntensityImage );

   // Grabbing an image of the laser line on the Z=0 plane.
   if (USE_REAL_CAMERA)
      {
      // Grab a sequence of image until enter is pressed.
      MdigGrabContinuous(MilDigitizer, MilCameraImage);

      MosPrintf(MIL_TEXT("Set up the reference plane at Z=0 mm.\n")
                MIL_TEXT("Press <Enter> to add this plane.\n\n"));
      MosGetch();
      MdigHalt(MilDigitizer);

      // Use Matrox GatorEye to perform laser line extraction.
      SwitchToLaserExtraction(MilDigitizer, 1);
      MdigGrab(MilDigitizer, MilGrabImage);

      // Switch back to full image frame.
      SwitchToImage(MilDigitizer);
      }
   else
      {
      // Simply reload the image that would have been grabbed above.
      MbufLoad(FULL_MODE_CALIBRATION_LINE_FILE, MilGrabImage);
      MosPrintf(MIL_TEXT("GatorEye data for the reference plane at Z=0 mm has been reloaded\n")
                MIL_TEXT("from disk.\n\n"));
      }

   // Copy the grabbed image into a host buffer to remove padding.
   MbufCopy(MilGrabImage, MilNoPaddingImage);

   // Add laser line information.
   M3dmapAddScan(MilLaser, MilScan, MilPositionImage, MilIntensityImage,
                 M_NULL, M_DEFAULT, M_LINE_ALREADY_EXTRACTED);

   // Display extracted laser line.
   DisplayLaserLine(MilSystem, MilDisplay, MilScan);

   MosPrintf(SEPARATOR);

   // Free MIL objects.
   FreeBuffersForLaserExtraction(1                , // free a single grab buffer
                                 &MilGrabImage    ,
                                 MilNoPaddingImage,
                                 MilPositionImage ,
                                 MilIntensityImage);
   }

//*****************************************************************************
// Add all necessary laser line info to the 3d reconstruction result buffer so
// that M3dmapCalibrate() can be called.
//
//   MilSystem       (in)  System on which to allocate the grab buffers.
//   MilDisplay      (in)  Display to use to show the extracted laser lines.
//   MilDigitizer    (in)  Digitizer to use to grab the laser line images.
//   MilCameraImage  (in)  Image to grab into (full-frame mode), already selected to display.
//   MilLaser        (in)  3d reconstruction context to calibrate  (M_DEPTH_CORRECTION mode).
//   MilScan         (in)  3d reconstruction result buffer that will accumulate calibration scan lines.
//*****************************************************************************
void AddLaserLinesDepthOnly(MIL_ID MilSystem     ,
                            MIL_ID MilDisplay    ,
                            MIL_ID MilDigitizer  ,
                            MIL_ID MilCameraImage,
                            MIL_ID MilLaser      ,
                            MIL_ID MilScan       )
   {
   // Allocate buffers used by the GatorEye to extract laser lines.
   MIL_ID MilGrabImage, MilNoPaddingImage, MilPositionImage, MilIntensityImage;
   AllocateBuffersForLaserExtraction(MilSystem          ,
                                     LASER_ROI_SIZE_X   ,
                                     1                  , // extract one laser line at a time
                                     1                  , // allocate a single grab buffer
                                     &MilGrabImage      ,
                                     &MilNoPaddingImage ,
                                     &MilPositionImage  ,
                                     &MilIntensityImage );

   for (MIL_INT ReferencePlaneIndex = 0; ReferencePlaneIndex < NB_REFERENCE_PLANES; ++ReferencePlaneIndex)
      {
      // Grabbing an image of the laser line on the next reference plane.
      if (USE_REAL_CAMERA)
         {
         // Grab a sequence of image until enter is pressed.
         MdigGrabContinuous(MilDigitizer, MilCameraImage);

         MosPrintf(MIL_TEXT("Set up the reference plane #%d at Z=%.3g mm.\n")
                   MIL_TEXT("Press <Enter> to add this plane.\n\n"),
                   ReferencePlaneIndex, CORRECTED_DEPTHS[ReferencePlaneIndex]);
         MosGetch();
         MdigHalt(MilDigitizer);

         // Use Matrox GatorEye to perform laser line extraction.
         SwitchToLaserExtraction(MilDigitizer, 1);
         MdigGrab(MilDigitizer, MilGrabImage);

         // Switch back to full image frame.
         SwitchToImage(MilDigitizer);
         }
      else
         {
         // Simply reload the image that would have been grabbed above.
         MIL_TEXT_CHAR Filename[MAX_FILENAME_LEN];
         MosSprintf(Filename, MAX_FILENAME_LEN, DEPTH_ONLY_CALIBRATION_LINE_FILENAME_PATTERN, ReferencePlaneIndex);
         MbufLoad(Filename, MilGrabImage);
         MosPrintf(MIL_TEXT("GatorEye data for the reference plane #%d at Z=%.3g mm has been\n")
                   MIL_TEXT("reloaded from disk.\n\n"),
                   ReferencePlaneIndex, CORRECTED_DEPTHS[ReferencePlaneIndex]);
         }

      // Copy the grabbed image into a host buffer to remove padding.
      MbufCopy(MilGrabImage, MilNoPaddingImage);

      // Add laser line information.
      M3dmapControl(MilLaser, M_DEFAULT, M_CORRECTED_DEPTH,
                    CORRECTED_DEPTHS[ReferencePlaneIndex]*SCALE_FACTOR);
      M3dmapAddScan(MilLaser, MilScan, MilPositionImage, MilIntensityImage,
                    M_NULL, M_DEFAULT, M_LINE_ALREADY_EXTRACTED);

      // Display extracted laser line.
      DisplayLaserLine(MilSystem, MilDisplay, MilScan);

      MosPrintf(SEPARATOR);
      }

   // Free MIL objects.
   FreeBuffersForLaserExtraction(1                , // free a single grab buffer
                                 &MilGrabImage    ,
                                 MilNoPaddingImage,
                                 MilPositionImage ,
                                 MilIntensityImage);
   }

//*****************************************************************************
// Allocate buffers used by the GatorEye to extract laser lines.
//
//   MilSystem             (in)     System on which to allocate the buffers.
//   LaserROISizeX         (in)     Width of the ROI for laser extraction; also the width of the buffers to allocate.
//   NbLaserLinesToExtract (in)     Number of laser data lines that the buffers must contain.
//   NbGrabBuffers         (in)     Number of grab buffers to allocate.
//   MilGrabImageArray     (out)    Array of images allocated for the grab operation.
//   MilNoPaddingImagePtr  (out)    Temporary image used to remove the padding of the grab image.
//   MilPositionImagePtr   (out)    Region of the image with no padding reinterpreted as laser 16-bit position.
//   MilIntensityImagePtr  (out)    Region of the image with no padding reinterpreted as laser  8-bit intensity.
//*****************************************************************************
void AllocateBuffersForLaserExtraction(MIL_ID  MilSystem            ,
                                       MIL_INT LaserROISizeX        ,
                                       MIL_INT NbLaserLinesToExtract,
                                       MIL_INT NbGrabBuffers        ,
                                       MIL_ID* MilGrabImageArray    ,
                                       MIL_ID* MilNoPaddingImagePtr ,
                                       MIL_ID* MilPositionImagePtr  ,
                                       MIL_ID* MilIntensityImagePtr )
   {
   // How many lines of raw 8-bits data do we need to store all laser data lines?
   MIL_INT LaserImageSizeY = NbLaserLinesToExtract * NB_BYTES_PER_PIXEL;

   // Compute the pitch in bytes between the start of two consecutive laser data line
   // in an image with no padding.
   MIL_INT LaserPitchByte = LaserROISizeX * NB_BYTES_PER_PIXEL;

   // Images used for grabbing in laser extraction mode. Let the digitizer use padding
   // if necessary.
   for (MIL_INT BufIndex = 0; BufIndex < NbGrabBuffers; ++BufIndex)
      {
      MbufAlloc2d(MilSystem, LaserROISizeX, LaserImageSizeY,
                  8+M_UNSIGNED, M_IMAGE+GRAB_ATTR, &MilGrabImageArray[BufIndex]);
      }

   // Special image the same size as the grabbed images, but with no padding.
   MsysControl(MilSystem, M_ALLOCATION_OVERSCAN, M_DISABLE);
   MsysControl(MilSystem, M_DEFAULT_PITCH_BYTE_MULTIPLE , 1        );
   MbufAlloc2d(MilSystem, LaserROISizeX, LaserImageSizeY,
               8+M_UNSIGNED, M_IMAGE+M_PROC, MilNoPaddingImagePtr);
   MsysControl(MilSystem, M_ALLOCATION_OVERSCAN, M_DEFAULT);
   MsysControl(MilSystem, M_DEFAULT_PITCH_BYTE_MULTIPLE , M_DEFAULT);

   // The grab image will be copied into the image with no padding. Then, this image
   // will be reinterpreted as two images: 16-bits position and 8-bits intensity images.
   // First, get the address of the buffer with no padding.
   MIL_UINT8* RawLaserLineDataPtr;
   MbufInquire(*MilNoPaddingImagePtr, M_HOST_ADDRESS, &RawLaserLineDataPtr);

   // Region of the image with no padding that is reinterpreted as the laser position.
   // Note that M_DISP here is only necessary for the optional display of the uncorrected
   // depth map during object scanning.
   MbufCreate2d(MilSystem, LaserROISizeX, NbLaserLinesToExtract, 16+M_UNSIGNED, M_IMAGE+M_PROC+M_DISP,
                M_HOST_ADDRESS+M_PITCH_BYTE, LaserPitchByte, RawLaserLineDataPtr,
                MilPositionImagePtr);

   // Region of the image with no padding that is reinterpreted as the laser intensity.
   if (GATOREYE_OUTPUT_INTENSITY)
      {
      // The 8-bit intensity comes after the 16-bit position, don't forget to offset the
      // start address!
      MbufCreate2d(MilSystem, LaserROISizeX, NbLaserLinesToExtract, 8+M_UNSIGNED, M_IMAGE+M_PROC,
                   M_HOST_ADDRESS+M_PITCH_BYTE, LaserPitchByte, RawLaserLineDataPtr+2*LaserROISizeX,
                   MilIntensityImagePtr);
      }
   else
      {
      *MilIntensityImagePtr = M_NULL;
      }
   }

//*****************************************************************************
// Free buffers allocated by AllocateBuffersForLaserExtraction().
//
//   NbGrabBuffers      (in)    Number of grab buffers to free.
//   MilGrabImageArray  (in)    Array of images allocated for the grab operation.
//   MilNoPaddingImage  (in)    Temporary image used to remove the padding of the grab image.
//   MilPositionImage   (in)    Region of the image with no padding reinterpreted as laser 16-bit position.
//   MilIntensityImage  (in)    Region of the image with no padding reinterpreted as laser  8-bit intensity.
//*****************************************************************************
void FreeBuffersForLaserExtraction(MIL_INT NbGrabBuffers    ,
                                   MIL_ID* MilGrabImageArray,
                                   MIL_ID  MilNoPaddingImage,
                                   MIL_ID  MilPositionImage ,
                                   MIL_ID  MilIntensityImage)
   {
   if (GATOREYE_OUTPUT_INTENSITY)
      MbufFree(MilIntensityImage);
   MbufFree(MilPositionImage );
   MbufFree(MilNoPaddingImage);
   for (MIL_INT BufIndex = 0; BufIndex < NbGrabBuffers; ++BufIndex)
      MbufFree(MilGrabImageArray[BufIndex]);
   }

//*****************************************************************************
// Uses 3dmap to show the image of the laser line the 3d camera has extracted.
//
//   MilSystem       (in)  System on which to allocate the buffer to draw.
//   MilDisplay      (in)  Display to use to show the extracted laser lines.
//   MilScan         (in)  3d reconstruction result buffer that contains the laser line to draw.
//*****************************************************************************
void DisplayLaserLine(MIL_ID MilSystem, MIL_ID MilDisplay, MIL_ID MilScan)
   {
   // Allocate buffer to draw laser line images returned by M3dmapDraw(), same size as laser ROI.
   MIL_ID MilLaserImage = MbufAlloc2d(MilSystem, LASER_ROI_SIZE_X, LASER_ROI_SIZE_Y,
                                      8+M_UNSIGNED, M_IMAGE+M_PROC+M_DISP, M_NULL);
   MbufClear(MilLaserImage, 0.0);

   // Backup previous image selected to display.
   MIL_ID MilPreviousImage;
   MdispInquire(MilDisplay, M_SELECTED, &MilPreviousImage);

   // Draw the extracted laser line and select it to the display.
   MgraColor(M_DEFAULT, 255.0);
   M3dmapDraw(M_DEFAULT, MilScan, MilLaserImage, M_DRAW_PEAKS_LAST, M_DEFAULT, M_DEFAULT);
   MdispSelect(MilDisplay, MilLaserImage);

   MosPrintf(MIL_TEXT("The extracted laser line is displayed.\n")
             MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   // Restore previous image selected to display.
   MdispSelect(MilDisplay, MilPreviousImage);

   // Free MIL objects.
   MbufFree(MilLaserImage);
   }

//*****************************************************************************
// Uses 3dmap to show the image of the laser line the 3d camera has extracted.
//
//   MilDigitizer    (in)  Digitizer for the Matrox GatorEye camera.
//
// Returns the camera framerate in fps (as a double)
//*****************************************************************************
MIL_DOUBLE GetCameraFrameRate(MIL_ID MilDigitizer)
   {
   MIL_DOUBLE FrameRate;
   if (USE_REAL_CAMERA)
      {
      // Switch to laser extraction settings.
      SwitchToLaserExtraction(MilDigitizer, 1);

      // Grab at least one image with the (potentially) new ROI to refresh the framerate.
      MIL_ID MilSystem;
      MdigInquire(MilDigitizer, M_OWNER_SYSTEM, &MilSystem);
      MIL_ID MilGrabImage = MbufAlloc2d(MilSystem, LASER_ROI_SIZE_X, NB_BYTES_PER_PIXEL,
                                        8+M_UNSIGNED, M_IMAGE+GRAB_ATTR, M_NULL);
      MdigGrab(MilDigitizer, MilGrabImage);
      MbufFree(MilGrabImage);

      // Now inquire the framerate.
      MdigInquireFeature(MilDigitizer, M_FEATURE_VALUE, MIL_TEXT("AcquisitionFrameRate"), M_TYPE_DOUBLE, &FrameRate);
      }
   else
      {
      // Framerate used for stand-alone mode.
      FrameRate = 55.693066;
      }
   return FrameRate;
   }
