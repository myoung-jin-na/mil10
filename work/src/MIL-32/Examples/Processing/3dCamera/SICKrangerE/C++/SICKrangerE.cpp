/*****************************************************************************/
/* 
 * File name: SICKrangerE.cpp
 *
 * Synopsis: Example demonstrating how to use a SICK ranger camera to feed mil3dmap
 *           module to generate depth maps.
 *            
 */

#include "cameraconfig.h"           /* User options - change them to suit your needs. */

#include <mil.h>
#include <math.h>
#include "mil3dcam_sickrangere.h"   /* Prototypes for conversion routines.            */
#include "sickcamera_rangere.h"     /* Prototypes to interact with ranger camera.     */

#include <algorithm>
using std::lower_bound;

#if M_MIL_USE_WINDOWS && !M_MIL_USE_CE
 /* Include DirectX display only on Windows. */
#include "MdispD3D.h"
#endif
/*****************************************************************************
 Example description.
*****************************************************************************/
void PrintHeader()
   {
   MosPrintf(MIL_TEXT("[EXAMPLE NAME]\n")
             MIL_TEXT("SICKrangerE\n\n")

             MIL_TEXT("[SYNOPSIS]\n")
             MIL_TEXT("This programs calibrates a 3d reconstruction setup with a SICK\n")
             MIL_TEXT("rangerE camera and scans an object to generate its depth map.\n\n")

             MIL_TEXT("[MODULES USED]\n")
             MIL_TEXT("Modules used: application, system, display, digitizer, buffer, graphic,\n")
             MIL_TEXT("              image processing, calibration, 3d reconstruction.\n\n")

             MIL_TEXT("Press <Enter> to start.\n\n"));

   MosGetch();
   }

/*****************************************************************************
 Constants.
*****************************************************************************/
/* Useful define */
#define PI 3.14159265358979323846

/* All the values below should be adjusted to suit your needs. */
#define MAX_DISPLAY_SIZE_X       1024.0
#define MAX_DISPLAY_SIZE_Y       768.0

#define DEPTH_NB_GRAB            10
#define CALIBRATED_NB_GRAB       16
#define NB_SCANS_PER_GRAB        256

/* Calibration values for M_DEPTH_CORRECTION context type. */
#define NUMBER_OF_CALIBRATION_DEPTHS  5
static const double CORRECTED_DEPTHS[NUMBER_OF_CALIBRATION_DEPTHS] =
   {0.0, 5.2, 11.0, 16.8, 22.6};   /* calibration heights in mm. */

#define SCALE_FACTOR   1000.0 /* (depth in world units) * SCALE_FACTOR gives gray levels */

/* Camera parameters */
#define PRINCIPAL_POINT_X        768
#define PRINCIPAL_POINT_Y        227

/* Calibration grid parameters. */
#define GRID_NB_ROWS             15
#define GRID_NB_COLS             22
#define GRID_ROW_SPACING         5.0     /* in mm                */
#define GRID_COL_SPACING         5.0     /* in mm                */

/* Depth map generation parameters. */
#define GAP_DEPTH                3.0      /* in mm                */

#define SCALE_X                  0.1      /* in mm/pixel          */
#define SCALE_Y                  0.1      /* in mm/pixel          */
#define SCALE_Z                  -0.001   /* in mm/gray level     */
#define WORLD_OFFSET_X           0.0    /* in mm */
#define WORLD_OFFSET_Y           0.0    /* in mm */
#define WORLD_OFFSET_Z           5.0    /* in mm                */

#define ERODE_MASK_WIDTH         2
#define DEPTH_CLEANING_RANK      7

#define INVALID_COLOR            30

/* Size of the depth map in pixels */
static const MIL_INT DEPTH_MAP_SIZE_X = (MIL_INT)(GRID_COL_SPACING*GRID_NB_COLS/SCALE_X);
static const MIL_INT DEPTH_MAP_SIZE_Y = 725;                                             

#define CONVEYOR_SPEED           5000.0  /* microns per second, 5cm/s. */

/* D3D display parameters */
#define D3D_DISPLAY_SIZE_X     640
#define D3D_DISPLAY_SIZE_Y     480

/* Useful to iterate over all bands. */
static const MIL_INT BAND_COLOR[3] = {M_RED, M_GREEN, M_BLUE};

/* Filenames. */
#if (!USE_REAL_CAMERA)

#define EXAMPLE_IMAGE_PATH  M_IMAGE_PATH MIL_TEXT("SICKrangerE/")

/* Images used for calibration in M_DEPTH_CORRECTION mode. */
static MIL_CONST_TEXT_PTR RefPlaneFilenames[NUMBER_OF_CALIBRATION_DEPTHS] =
   {
   EXAMPLE_IMAGE_PATH MIL_TEXT("calibplane0.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("calibplane1.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("calibplane2.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("calibplane3.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("calibplane4.mim")
   };

/* Images used for calibration in M_CALIBRATED_CAMERA_LINEAR_MOTION mode. */
static MIL_CONST_TEXT_PTR CalibGridFilename = EXAMPLE_IMAGE_PATH MIL_TEXT("CalibGrid.mim");
static MIL_CONST_TEXT_PTR CalibLineFilename = EXAMPLE_IMAGE_PATH MIL_TEXT("CalibLine.mim");

/* Images used for runtime scan of an object (in both modes). */
static MIL_CONST_TEXT_PTR ScanFilenames[CALIBRATED_NB_GRAB] =
   {
   EXAMPLE_IMAGE_PATH MIL_TEXT("scan0.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("scan1.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("scan2.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("scan3.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("scan4.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("scan5.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("scan6.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("scan7.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("scan8.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("scan9.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("scan10.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("scan11.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("scan12.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("scan13.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("scan14.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("scan15.mim")
   };

#endif

/*****************************************************************************
 Functions declarations.
*****************************************************************************/

/* Example function declaration. */
template <MIL_INT ContextType>
void RangerEExample(MIL_ID MilSystem, MIL_ID MilDisplay, MIL_ID MilDepthMapDisplay);

/* Helper function prototypes. */
void FatalError(const MIL_TEXT_CHAR* pMsg);
void CleanDepthMap(MIL_ID MilDepthMap);
void CalculateColorBandsAverage(MIL_ID MilColorImage, MIL_DOUBLE BandAverage[3]);
void MoveRelativeCoordToLaserCoord(MIL_ID MilLaser, MIL_ID MilCalibration);
void Remap8BitImage(MIL_ID MilImage);

/* Allocation of result image buffers */
void AllocateScanResultImageBuffers(MIL_INT AllocCondition, MIL_INT SizeBand, MIL_INT SizeX, MIL_INT SizeY, MIL_INT SizeBit, MIL_INT NbGrabs, MIL_ID* pMilResultImage, MIL_ID* pMilResultChilds, MIL_ID* pMilResultOneLine);
void FreeScanResultImageBuffers(MIL_INT FreeCondition, MIL_INT NbGrabs, MIL_ID MilResultImage, MIL_ID* pMilResultChilds, MIL_ID MilResultOneLine);

/* Display functions prototypes */
void DisplayLaserLine(MIL_ID  MilDisplay,
                      MIL_ID  MilPositionLine,
                      MIL_ID  MilIntensityLine,
                      MIL_ID  MilMeasImage,
                      MIL_INT FixedPoint);

void DisplayRelativeCoordinateSystem(MIL_ID MilDisplay, 
                                     MIL_ID MilGraList,
                                     MIL_ID MilCalibration,
                                     MIL_ID MilPositionLine,
                                     MIL_ID MilIntensityLine,
                                     MIL_ID MilMeasImage,
                                     MIL_INT FixedPoint,
                                     MIL_INT LaserExtractionOffsetX,
                                     MIL_INT LaserExtractionOffsetY);

void DisplayCalibrationPoints(MIL_ID MilDisplay,
                              MIL_ID MilGraList,
                              MIL_ID MilCalGridImage,
                              MIL_ID MilCalibration);

MIL_DOUBLE SetDepthMapDisplay(MIL_ID MilDepthMapDisplay, MIL_INT DisplayOffsetX, MIL_INT DisplayOffsetY, MIL_INT DepthmapSizeX, MIL_INT DepthMapSizeY);

/* User's processing function prototype. */
MIL_INT MFTYPE AddScanProcFct(MIL_INT HookType, MIL_ID MilRangerImage, void* UserDataPtr);
MIL_INT MFTYPE CopyFct(MIL_INT HookType, MIL_ID MilRangerImage, void* UserDataPtr);
MIL_INT MFTYPE HorFlipFct(MIL_INT HookType, MIL_ID MilRangerImage, void* UserDataPtr);

/* Ray tracing functions */
void CreateColorMap(MIL_ID MilSystem, MIL_ID MilDepthMap, MIL_ID MilCalibration, MIL_ID MilColorImage, double ConveyorSpeed, MIL_DOUBLE PixelXIncrement, MIL_INT MeasROIOffsetX, MIL_DOUBLE ColorLinesOffset, MIL_ID* pMilColorMap); 

/* User's start stage thread function */
MIL_UINT32 MFTYPE StartConveyorFct(void *UserDataPtr);

/* User's processing function hook data structure. */
struct AddScanProcFctDataStruct
   {
   MIL_ID        MilLaser;
   MIL_ID        MilScan;
   MIL_ID        MilLaserPositionImage;
   MIL_ID        MilLaserIntensityImage;
   MIL_ID*       pMilLaserScatterChilds;
   MIL_ID*       pMilGrayChilds;
   MIL_ID*       pMilHiresGrayChilds;
   MIL_ID*       pMilColorChilds;
   MIL_ID*       pMilHiresColorChilds;
   MIL_ID*       pMilScatterChilds;
   RangerParams* ConvParamsPtr;
   MIL_INT       GrabIndex;
   MIL_INT       NbGrabs;
   };

/* Start conveyor event data structure */
struct StartConveyorFctDataStruct
   {
   /* CONVEYOR_CODE: Add conveyor related objects here */   

   MIL_ID MilStartStageEvent;
   };

/*****************************************************************************
 Main.
*****************************************************************************/
int MosMain(void)
   {
   /* Print example description. */
   PrintHeader();

#if USE_REAL_CAMERA
   MIL_CONST_TEXT_PTR SystemDescriptor = M_SYSTEM_DEFAULT;
#else
   MIL_CONST_TEXT_PTR SystemDescriptor = M_SYSTEM_HOST;
#endif
   
   /* Allocate application, system and display. */
   MIL_ID MilApplication = MappAlloc(M_NULL, M_DEFAULT, M_NULL);
   MIL_ID MilSystem = MsysAlloc(M_DEFAULT, SystemDescriptor, M_DEFAULT, M_DEFAULT, M_NULL);
   MIL_ID MilDisplay = MdispAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_WINDOWED, M_NULL);
   MIL_ID MilDepthMapDisplay = MdispAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_WINDOWED, M_NULL);
   
   /* Run the depth correction example. */
   RangerEExample<M_DEPTH_CORRECTION>(MilSystem, MilDisplay, MilDepthMapDisplay);

   /* Run the calibrated camera example. */
   RangerEExample<M_CALIBRATED_CAMERA_LINEAR_MOTION>(MilSystem, MilDisplay, MilDepthMapDisplay);

   /* Free MIL objects. */
   MdispFree(MilDepthMapDisplay);
   MdispFree(MilDisplay);
   MsysFree(MilSystem);   
   MappFree(MilApplication);

   return 0;
   }



/*****************************************************************************
 Function that calibrates a 3d reconstruction setup with a SICK rangerE camera
 and scans an object to generate its depth map and if possible its color map.
*****************************************************************************/
template <MIL_INT ContextType>
void RangerEExample(MIL_ID MilSystem, MIL_ID MilDisplay, MIL_ID MilDepthMapDisplay)
   {
   const MIL_INT NB_GRABS = ContextType == M_CALIBRATED_CAMERA_LINEAR_MOTION ? CALIBRATED_NB_GRAB : DEPTH_NB_GRAB;
   const MIL_INT NB_SCANS_IN_RESULT = (NB_GRABS * NB_SCANS_PER_GRAB);
   
   MIL_ID      MilGraList = 0,         /* Graphic list associated with the display  */

               MilImage,               /* Image buffer identifier (for continuous grab).               */
               MilMeasImage,           /* Image buffer to hold image of meas. ROI (MimDraw).           */
               MilCalGridImage,        /* Image buffer to hold calibration grid image.                 */
               MilOneLineImage,        /* Image buffer to hold one laser line.                         */
               MilManyLinesImage,      /* Image buffers to hold lines.                                 */
               MilLaserPositionImage,  /* Image buffer to hold converted position results.             */
               MilLaserIntensityImage, /* Image buffer to hold converted laser line intensity results. */
               MilLaserScatterImage,   /* Image buffer to hold converted laser line scatter results.   */
               MilGrayImage,           /* Image buffer to hold converted gray results.                 */
               MilHiresGrayImage,      /* Image buffer to hold converted hires gray results.           */
               MilColorImage,          /* Image buffer to hold converted color results.                */
               MilHiresColorImage,     /* Image buffer to hold converted hires color results.          */
               MilScatterImage,        /* Image buffer to hold converted scatter results.              */
               
               MilLaserPositionLine,   /* Image buffer to hold one line of converted int.        */
               MilLaserIntensityLine,  /* Image buffer to hold one line of converted int.        */
               MilLaserScatterLine,    /* Image buffer to hold one laser line scatter results.   */
               MilGrayLine,            /* Image buffer to hold one gray results.                 */
               MilHiresGrayLine,       /* Image buffer to hold one hires gray results.           */
               MilColorLine,           /* Image buffer to hold one color results.                */
               MilHiresColorLine,      /* Image buffer to hold one hires color results.          */
               MilScatterLine,         /* Image buffer to hold one scatter results.              */

               MilLaserScatterChilds[NB_GRABS], /* Child buffers to hold converted laser line scatter results.   */
               MilGrayChilds[NB_GRABS],         /* Child buffers to hold converted gray results.                 */
               MilHiresGrayChilds[NB_GRABS],    /* Child buffers to hold converted hires gray results.           */
               MilColorChilds[NB_GRABS],        /* Child buffers to hold converted color results.                */
               MilHiresColorChilds[NB_GRABS],   /* Child buffers to hold converted hires color results.          */
               MilScatterChilds[NB_GRABS],      /* Child buffers to hold converted scatter results.              */
               
               MilCalibration,                  /* Calibration context identifier.                              */
               MilDepthMap,                     /* Image buffer identifier (for depth map result)               */
               MilIntensityMap,                 /* Image buffer identifier (for intensity map result)           */
               MilColorMap = M_NULL,            /* Image buffer identifier (for color map result)               */
               MilColorMapDisplay = M_NULL,     /* Color map display identifier                                 */
               MilLaser,                        /* 3dmap laser profiling context identifier.                    */
               MilScan;                         /* 3dmap result buffer identifier.                              */
                          
   MIL_INT     DepthMapDisplayOffsetY = 0,      /* The display window offsets of the depth map display          */
               DepthMapDisplayOffsetX = 0,         
               n,                               /* Counter.                                                     */
               Ret;                             /* Return value (error code) of subfunctions.                   */

   MosPrintf(MIL_TEXT("\n3D RECONSTRUCTION USING SICK RANGERE:\n"));
   MosPrintf(MIL_TEXT("------------------------------------\n\n"));
   MosPrintf(MIL_TEXT("This programs calibrates a 3d reconstruction setup with a SICK\n"));
   MosPrintf(MIL_TEXT("rangerE camera and scans an object to generate its depth map and\n"));
   MosPrintf(MIL_TEXT("if available, its color map.\n\n"));

   if (ContextType == M_DEPTH_CORRECTION)
      MosPrintf(MIL_TEXT("3dmap M_DEPTH_CORRECTION mode is used.\n\n"));
   else
      MosPrintf(MIL_TEXT("3dmap M_CALIBRATED_CAMERA_LINEAR_MOTION mode is used.\n\n"));

   /* CONVEYOR_CODE: Initialize conveyor. */
   
   /* Create a sick camera system and initialize it */
   SSickCameraSystem CameraSystem;
   SSickCameraSystem* pCameraSystem = &CameraSystem;
   Ret = CreateCameraSystem(pCameraSystem);
   if (Ret != SICK_CAMERA_OK)
      FatalError(MIL_TEXT("Unable to create camera system.\n"));

   Ret = InitCameraSystem(pCameraSystem);
   if (Ret != SICK_CAMERA_OK)
      FatalError(MIL_TEXT("Unable to initialize camera system.\n"));

   /* Create the structures for the different types of grab */
   FrameGrabberHookStruct FGImageHookStruct = {MilSystem, &HorFlipFct, &MilImage};
   FrameGrabberHookStruct FGMeasOneLineHookStruct = {MilSystem, &CopyFct, &MilOneLineImage};
   FrameGrabberHookStruct FGMeasColorHookStruct = {MilSystem, &CopyFct, &MilManyLinesImage};
   
   /* Inquire camera parameters and fill in conversion structure. */
   RangerParams ConvParams;
   Ret = FillParams(pCameraSystem, &ConvParams, RANGER_E55_PRM);
   if (Ret != SICK_CAMERA_OK)
      FatalError(MIL_TEXT("Unable to inquire camera parameters.\n"));

   /* Allocate buffers to hold images returned by SICK camera in measurement mode. */
   MbufAlloc2d(MilSystem, ConvParams.ProfileDataSizeByte, NB_SCANS_PER_GRAB,
                  8+M_UNSIGNED, M_IMAGE+M_PROC, &MilManyLinesImage);
   
   /* Allocate buffers to hold images returned by SICK camera in image mode. */
   MbufAlloc2d(MilSystem, ConvParams.ImageROISizeX, ConvParams.ImageROISizeY,
               8+M_UNSIGNED, M_IMAGE+M_PROC+M_DISP, &MilImage);
   MbufAlloc2d(MilSystem, ConvParams.ImageROISizeX, ConvParams.ImageROISizeY,
               8+M_UNSIGNED, M_IMAGE+M_PROC+M_DISP, &MilCalGridImage);

   /* Allocate buffers to hold images returned by MimDraw(), same size as meas. ROI. */
   MIL_INT MimDrawSizeBit = (ConvParams.LaserIntensitySizeBit == NO_INFORMATION ?
                             8 : ConvParams.LaserIntensitySizeBit);
   MbufAlloc2d(MilSystem, ConvParams.MeasROISizeX, ConvParams.LaserROISizeY,
               MimDrawSizeBit+M_UNSIGNED, M_IMAGE+M_PROC+M_DISP,
               &MilMeasImage);

   /* Allocate position image returned by conversion routine and fed to 3dmap. */
   MbufAlloc2d(M_DEFAULT_HOST, ConvParams.MeasROISizeX, NB_SCANS_PER_GRAB,
               ConvParams.RangeSizeBit+M_UNSIGNED, M_IMAGE+M_PROC, &MilLaserPositionImage);

   /*
    * When calibrating, we won't need the full images since we want to grab only one
    * laser line. So, we create 1-line child buffers for SICK measurement output buffer and
    * for conversion routine buffers. 
    */
   MbufChild2d(MilManyLinesImage     , 0, 0, ConvParams.ProfileDataSizeByte, 1, &MilOneLineImage );
   MbufChild2d(MilLaserPositionImage , 0, 0, ConvParams.MeasROISizeX       , 1, &MilLaserPositionLine );

   /* If camera mode returns laser intensity information, we also allocate the related buffers. */
   if(ConvParams.LaserIntensitySizeBit != NO_INFORMATION)
      {
      MbufAlloc2d(M_DEFAULT_HOST, ConvParams.MeasROISizeX, NB_SCANS_PER_GRAB,
                  ConvParams.LaserIntensitySizeBit+M_UNSIGNED, M_IMAGE+M_PROC, &MilLaserIntensityImage);
      MbufChild2d(MilLaserIntensityImage, 0, 0, ConvParams.MeasROISizeX, 1, &MilLaserIntensityLine);
      }   
   
   /* Allocate all the buffers for the other measurements that are not directly linked with 3dmap. */
   AllocateScanResultImageBuffers(ConvParams.LaserScatterSizeBit, 1, ConvParams.MeasROISizeX, NB_SCANS_IN_RESULT, ConvParams.LaserScatterSizeBit, NB_GRABS, &MilLaserScatterImage, MilLaserScatterChilds, &MilLaserScatterLine);
   
   AllocateScanResultImageBuffers(ConvParams.GrayStreamOffset      , 1, ConvParams.MeasROISizeX    , NB_SCANS_IN_RESULT, 8, NB_GRABS, &MilGrayImage      , MilGrayChilds      , &MilGrayLine);
   AllocateScanResultImageBuffers(ConvParams.HiResGrayStreamOffset , 1, ConvParams.MeasROISizeX * 2, NB_SCANS_IN_RESULT, 8, NB_GRABS, &MilHiresGrayImage , MilHiresGrayChilds , &MilHiresGrayLine);
   AllocateScanResultImageBuffers(ConvParams.ColorStreamOffset     , 3, ConvParams.MeasROISizeX    , NB_SCANS_IN_RESULT, 8, NB_GRABS, &MilColorImage     , MilColorChilds     , &MilColorLine);
   AllocateScanResultImageBuffers(ConvParams.HiResColorStreamOffset, 3, ConvParams.MeasROISizeX * 2, NB_SCANS_IN_RESULT, 8, NB_GRABS, &MilHiresColorImage, MilHiresColorChilds, &MilHiresColorLine);
   AllocateScanResultImageBuffers(ConvParams.ScatterStreamOffset   , 1, ConvParams.MeasROISizeX    , NB_SCANS_IN_RESULT, 8, NB_GRABS, &MilScatterImage   , MilScatterChilds   , &MilScatterLine);
   
   /* Allocate 3dmap context and result, set some parameters. */
   M3dmapAlloc(MilSystem, M_LASER, ContextType, &MilLaser);
   M3dmapControl(MilLaser, M_DEFAULT, M_EXTRACTION_FIXED_POINT, ConvParams.FixedPoint);

   double ConveyorSpeed;
   if (ContextType == M_CALIBRATED_CAMERA_LINEAR_MOTION)
      {
      /*
       * Compute conveyor speed in mm/frame.
       * CONVEYOR_SPEED is in um/s, ConvParams.CycleTimeMicroseconds is in us/frame.
       * ConvParams.CycleTimeMicroseconds * CONVEYOR_SPEED is in 10^-12 m/frame
       */
      /* The conveyor is moving in the negative y direction in the example. */
      ConveyorSpeed = 1.0e-9 * ConvParams.CycleTimeMicroseconds * (-CONVEYOR_SPEED);

      M3dmapControl(MilLaser, M_DEFAULT, M_EXTRACTION_CHILD_OFFSET_X, ConvParams.ROIOffsetX);
      M3dmapControl(MilLaser, M_DEFAULT, M_EXTRACTION_CHILD_OFFSET_Y, ConvParams.ROIOffsetY);
      M3dmapControl(MilLaser, M_DEFAULT, M_SCAN_SPEED               , ConveyorSpeed        );

      /* Allocate a graphic list and associate it with the display */
      MgraAllocList(MilSystem, M_DEFAULT, &MilGraList);
      MdispControl(MilDisplay, M_ASSOCIATED_GRAPHIC_LIST_ID, MilGraList);
      }

   M3dmapAllocResult(MilSystem, M_LASER_DATA, M_DEFAULT, &MilScan);
   M3dmapControl(MilScan, M_DEFAULT, M_MAX_FRAMES, NB_SCANS_IN_RESULT);
      
#if USE_REAL_CAMERA
   MosPrintf(MIL_TEXT("Adjust camera and laser position for calibration.\n\n"));
#endif

   /* Display grabbed image. */
   MbufClear(MilImage, 0.0);
   if (ConvParams.ImageROISizeX > MAX_DISPLAY_SIZE_X)
      MdispZoom(MilDisplay, 0.5, 0.5);
   MdispSelect(MilDisplay, MilImage);

   /* Laser calibration steps are different, depending on the 3dmap context type. */
   if (ContextType == M_CALIBRATED_CAMERA_LINEAR_MOTION)
      {
#if USE_REAL_CAMERA
      /* Grab a sequence of image until enter is pressed. */
      SwitchGrabMode(pCameraSystem, IMAGE_MODE_NAME, 1, M_ASYNCHRONOUS, &FGImageHookStruct);
      FrameGrabberProcess(pCameraSystem, M_START);
      
      MosPrintf(MIL_TEXT("Place calibration grid, switch laser OFF.\n"));
      MosPrintf(MIL_TEXT("Press <Enter> to grab.\n\n"));
      MosGetch();

      /* Grab calibration grid image. */
      FrameGrabberProcess(pCameraSystem, M_STOP);
      MbufCopy(MilImage, MilCalGridImage);
      Remap8BitImage(MilCalGridImage);
            
      /* Grab a sequence of image until enter is pressed. */
      FrameGrabberProcess(pCameraSystem, M_START);

      MosPrintf(MIL_TEXT("Switch laser ON.\n"));
      MosPrintf(MIL_TEXT("Press <Enter> to grab.\n\n"));
      MosGetch();
      
      /* Stop the grab */
      FrameGrabberProcess(pCameraSystem, M_STOP);

      /* Switch the camera to measurement mode */
      MosPrintf(MIL_TEXT("Switching grab mode to measurement...\n"));
      SwitchGrabMode(pCameraSystem, MEASUREMENT_MODE_NAME, 1, M_SYNCHRONOUS, &FGMeasOneLineHookStruct);

      /* Grab one laser line for 3d setup calibration. */     
      MosPrintf(MIL_TEXT("Grabbing the measurement data...\n\n"));
      FrameGrabberProcess(pCameraSystem, M_SEQUENCE, 1);  

#else
      /* Simply reload the images that would have been grabbed above. */
      MbufLoad(CalibGridFilename, MilCalGridImage);
      MbufLoad(CalibLineFilename, MilOneLineImage);
      MbufCopy(MilCalGridImage, MilImage);
#endif

      /* Allocate calibration context in 3D mode. */
      McalAlloc(MilSystem, M_TSAI_BASED, M_DEFAULT, &MilCalibration);

      /* Set the principal point */
      McalControl(MilCalibration, M_PRINCIPAL_POINT_X, PRINCIPAL_POINT_X);
      McalControl(MilCalibration, M_PRINCIPAL_POINT_Y, PRINCIPAL_POINT_Y);

      /* Calibrate the camera. */
      MosPrintf(MIL_TEXT("Calibrating the camera...\n"));
      McalGrid(MilCalibration, MilCalGridImage, 0.0, 0.0, 0.0, GRID_NB_ROWS, GRID_NB_COLS,
               GRID_ROW_SPACING, GRID_COL_SPACING, M_DEFAULT, M_CHESSBOARD_GRID);

      if (McalInquire(MilCalibration, M_CALIBRATION_STATUS, M_NULL) != M_CALIBRATED)
         FatalError(MIL_TEXT("Camera calibration failed.\n"));

      /* Display McalGrid() results. */
      DisplayCalibrationPoints(MilDisplay, MilGraList, MilCalGridImage, MilCalibration);

      /* Convert SICK format to MIL format. */
      M3dcamRangerToMIL(MilOneLineImage, MilLaserPositionLine, MilLaserIntensityLine,
                        MilLaserScatterLine, MilGrayLine, MilHiresGrayLine, MilColorLine,
                        MilHiresColorLine, MilScatterLine, M_NULL, &ConvParams);

      /* Display extracted laser line. */
      DisplayLaserLine(MilDisplay, MilLaserPositionLine, MilLaserIntensityLine,
                       MilMeasImage, ConvParams.FixedPoint);

      /* Add laser line informations. */
      M3dmapAddScan(MilLaser, MilScan, MilLaserPositionLine, MilLaserIntensityLine,
                    M_NULL, M_DEFAULT, M_LINE_ALREADY_EXTRACTED);
      }

   else
      {
      /* ContextType == M_DEPTH_CORRECTION */
      MilCalibration = M_NULL;

      for(n = 0; n < NUMBER_OF_CALIBRATION_DEPTHS; n++)
         {
         MosPrintf(MIL_TEXT("Current plane: %d (%4.2f mm).\n"), n, CORRECTED_DEPTHS[n]);

#if USE_REAL_CAMERA
         /* Grab a sequence of image until enter is pressed. */
         SwitchGrabMode(pCameraSystem, IMAGE_MODE_NAME, 1, M_ASYNCHRONOUS, &FGImageHookStruct);
         FrameGrabberProcess(pCameraSystem, M_START);

         MosPrintf(MIL_TEXT("Press <Enter> to grab.\n\n"));
         MosGetch();

         /* Stop the grab */
         FrameGrabberProcess(pCameraSystem, M_STOP);

         /* Switch the camera to measurement mode */
         MosPrintf(MIL_TEXT("Switching grab mode to measurement...\n"));
         SwitchGrabMode(pCameraSystem, MEASUREMENT_MODE_NAME, 1, M_SYNCHRONOUS, &FGMeasOneLineHookStruct);

         /* Grab one laser line for 3d setup calibration. */  
         MosPrintf(MIL_TEXT("Grabbing the measurement data...\n\n"));
         FrameGrabberProcess(pCameraSystem, M_SEQUENCE, 1);

#else
         /* Simply reload the images that would have been grabbed above. */
         MbufLoad(RefPlaneFilenames[n], MilOneLineImage);
#endif

         /* Set desired corrected depth of next reference plane. */
         M3dmapControl(MilLaser, M_DEFAULT, M_CORRECTED_DEPTH,
                       CORRECTED_DEPTHS[n]*SCALE_FACTOR);

         /* Convert SICK format to MIL format. */
         M3dcamRangerToMIL(MilOneLineImage, MilLaserPositionLine, MilLaserIntensityLine,
                           MilLaserScatterLine, MilGrayLine, MilHiresGrayLine, MilColorLine,
                           MilHiresColorLine, MilScatterLine, M_NULL, &ConvParams);         

         /* Display extracted laser line. */
         DisplayLaserLine(MilDisplay, MilLaserPositionLine, MilLaserIntensityLine,
                          MilMeasImage, ConvParams.FixedPoint);

         /* Add laser line informations. */
         M3dmapAddScan(MilLaser, MilScan, MilLaserPositionLine, MilLaserIntensityLine,
                       M_NULL, M_DEFAULT, M_LINE_ALREADY_EXTRACTED);
         }
      }

   /* Calibrate the 3d reconstruction context. */
   MosPrintf(MIL_TEXT("Calibrating 3dmap...\n"));
   M3dmapCalibrate(MilLaser, MilScan, MilCalibration, M_DEFAULT);

   if(M3dmapInquire(MilLaser, M_DEFAULT, M_CALIBRATION_STATUS, M_NULL) != M_CALIBRATED)
      FatalError(MIL_TEXT("Laser calibration failed.\n"));

   MosPrintf(MIL_TEXT("The 3d reconstruction setup has been calibrated.\n\n"));

   if(ContextType == M_CALIBRATED_CAMERA_LINEAR_MOTION)
      {
      /* Move the coordinate system of the calibration on the 3dmap coordinate system */
      MoveRelativeCoordToLaserCoord(MilLaser, MilCalibration);
              
      /* Display the relative coordinate system */
      DisplayRelativeCoordinateSystem(MilDisplay, MilGraList, MilCalibration, MilLaserPositionLine, MilLaserIntensityLine, MilCalGridImage, ConvParams.FixedPoint, ConvParams.ROIOffsetX, ConvParams.ROIOffsetY);


#if (USE_REAL_CAMERA)    
      if(ConvParams.ColorStreamOffset != NO_INFORMATION || ConvParams.HiResColorStreamOffset != NO_INFORMATION)
         {
         /* Grab a sequence of image until enter is pressed*/
         SwitchGrabMode(pCameraSystem, IMAGE_MODE_NAME, 1, M_ASYNCHRONOUS, &FGImageHookStruct);
         FrameGrabberProcess(pCameraSystem, M_START);

         /* Set the grab image on the display */
         MdispControl(MilDisplay, M_UPDATE, M_DISABLE);
         MdispControl(MilDisplay, M_VIEW_MODE, M_DEFAULT);
         MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);
         MgraClear(M_DEFAULT, MilGraList);
         MdispSelect(MilDisplay, MilImage);
         MdispControl(MilDisplay, M_UPDATE, M_ENABLE);

         MosPrintf(MIL_TEXT("Color gain and white balance calibration\n\n"));
         MosPrintf(MIL_TEXT("Put a white object in front of the color line region of the camera\n"));
         MosPrintf(MIL_TEXT("Press <Enter> to grab.\n\n"));
         MosGetch();

         /* Stop the grab */
         FrameGrabberProcess(pCameraSystem, M_STOP);

         /* Switch the camera to measurement mode */
         MosPrintf(MIL_TEXT("Switching grab mode to measurement...\n"));
         SwitchGrabMode(pCameraSystem, MEASUREMENT_MODE_NAME, NB_SCANS_PER_GRAB, M_SYNCHRONOUS, &FGMeasColorHookStruct);

         /* Grab one color frame for color gain setup */
         MosPrintf(MIL_TEXT("Grabbing the measurement data for gain correction...\n"));
         FrameGrabberProcess(pCameraSystem, M_SEQUENCE, 1);

         /* Convert SICK format to MIL format. */
         M3dcamRangerToMIL(MilManyLinesImage, MilLaserPositionImage, MilLaserIntensityImage,
                           MilLaserScatterChilds[0], MilGrayChilds[0], MilHiresGrayChilds[0],
                           MilColorChilds[0], MilHiresColorChilds[0], MilScatterChilds[0],
                           M_NULL, &ConvParams);

         
         /* Calculate the bands average color */
         MIL_DOUBLE BandAverage[3];
         MIL_ID MilColorMeasChildsToUse = ConvParams.HiResColorStreamOffset != NO_INFORMATION ? MilHiresColorChilds[0] : MilColorChilds[0];
         CalculateColorBandsAverage(MilColorMeasChildsToUse, BandAverage);

         /* Set the color gains */
         SetColorGain(pCameraSystem, BandAverage[0], BandAverage[1], BandAverage[2], ConvParams.HiResColorStreamOffset != NO_INFORMATION);

         /* Grab another frame for the white balancing*/
         MosPrintf(MIL_TEXT("Grabbing the measurement data for white balance...\n"));
         FrameGrabberProcess(pCameraSystem, M_SEQUENCE, 1);

         /* Convert SICK format to MIL format. */
         M3dcamRangerToMIL(MilManyLinesImage, MilLaserPositionImage, MilLaserIntensityImage,
                           MilLaserScatterChilds[0], MilGrayChilds[0], MilHiresGrayChilds[0],
                           MilColorChilds[0], MilHiresColorChilds[0], MilScatterChilds[0],
                           M_NULL, &ConvParams);

         /* Calculate the band average color */
         CalculateColorBandsAverage(MilColorMeasChildsToUse, BandAverage);

         /* Set the white balance coefficient in the camera */
         SetWhiteBalancing(pCameraSystem, BandAverage[0], BandAverage[1], BandAverage[2], ConvParams.HiResColorStreamOffset != NO_INFORMATION);

         MosPrintf(MIL_TEXT("The gain and white balance coefficient were successfully calculated.\n"));
         MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
         MosGetch();
         }
#endif
      }
   
   /* Empty all result buffer contents.
      It will now be reused for extracting corrected depths. */
   M3dmapAddScan(M_NULL, MilScan, M_NULL, M_NULL, M_NULL, M_DEFAULT, M_RESET);


#if (USE_REAL_CAMERA)
   /* Grab a sequence of image until ENTER is pressed. */
   SwitchGrabMode(pCameraSystem, IMAGE_MODE_NAME, 1, M_ASYNCHRONOUS, &FGImageHookStruct);
   FrameGrabberProcess(pCameraSystem, M_START);

   MosPrintf(MIL_TEXT("Adjust object position for scanning.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to grab.\n\n"));
   MosGetch();

   // Stop the grab.
   FrameGrabberProcess(pCameraSystem, M_STOP);   

   /* CONVEYOR_CODE: Start the conveyor start thread */
   StartConveyorFctDataStruct StartConveyorDataStruct;
   StartConveyorDataStruct.MilStartStageEvent = MthrAlloc(MilSystem, M_EVENT, M_DEFAULT, M_NULL, M_NULL, M_NULL);
   MIL_ID MilStartStageThread = MthrAlloc(MilSystem, M_THREAD, M_DEFAULT, &StartConveyorFct, &StartConveyorDataStruct, M_NULL);
      
   /* Fill MdigProcess hook function structure. */
   AddScanProcFctDataStruct AddScanProcFctData;
   AddScanProcFctData.MilLaser               = MilLaser;
   AddScanProcFctData.MilScan                = MilScan;
   AddScanProcFctData.MilLaserPositionImage  = MilLaserPositionImage;
   AddScanProcFctData.MilLaserIntensityImage = MilLaserIntensityImage;
   AddScanProcFctData.pMilLaserScatterChilds = MilLaserScatterChilds;
   AddScanProcFctData.pMilGrayChilds         = MilGrayChilds;
   AddScanProcFctData.pMilHiresGrayChilds    = MilHiresGrayChilds;
   AddScanProcFctData.pMilColorChilds        = MilColorChilds;
   AddScanProcFctData.pMilHiresColorChilds   = MilHiresColorChilds;
   AddScanProcFctData.pMilScatterChilds      = MilScatterChilds;
   AddScanProcFctData.ConvParamsPtr          = &ConvParams;
   AddScanProcFctData.GrabIndex              = 0;
   AddScanProcFctData.NbGrabs                = NB_GRABS;
   FrameGrabberHookStruct FGMeasHookStruct = {MilSystem, &AddScanProcFct, &AddScanProcFctData};

   /* Switch the camera to measurement mode */
   MosPrintf(MIL_TEXT("Switching grab mode to measurement...\n\n"));
   SwitchGrabMode(pCameraSystem, MEASUREMENT_MODE_NAME, NB_SCANS_PER_GRAB, M_SYNCHRONOUS, &FGMeasHookStruct);
   
   /* Grab laser line data with buffering. */
   MosPrintf(MIL_TEXT("The object is being scanned.\n\n"));
   FrameGrabberProcess(pCameraSystem, M_SEQUENCE, NB_GRABS,
                       StartConveyorDataStruct.MilStartStageEvent);

   MthrFree(MilStartStageThread);
   MthrFree(StartConveyorDataStruct.MilStartStageEvent);
   
   /* CONVEYOR_CODE: Stop conveyor. */
   
#else

   for (n = 0; n < NB_GRABS; ++n)
      {
      /* Reload profile data generated by SICK camera. */
      MbufLoad(ScanFilenames[n], MilManyLinesImage);

      /* Convert SICK format to MIL format. */
      M3dcamRangerToMIL(MilManyLinesImage, MilLaserPositionImage, MilLaserIntensityImage,
                        MilLaserScatterChilds[n], MilGrayChilds[n], MilHiresGrayChilds[n],
                        MilColorChilds[n], MilHiresColorChilds[n], MilScatterChilds[n],
                        M_NULL, &ConvParams);
      
      /* Analyze the image to extract laser line and correct its depth. */
      M3dmapAddScan(MilLaser, MilScan, MilLaserPositionImage,
                    MilLaserIntensityImage, M_NULL,
                    M_DEFAULT, M_LINE_ALREADY_EXTRACTED);
      }

#endif

   /* Set depth map generation parameters according to 3dmap context type. */
   MIL_INT DepthMapSizeX, DepthMapSizeY, DepthMapType, IntensityMapType;

   if (ContextType == M_CALIBRATED_CAMERA_LINEAR_MOTION)
      {
      M3dmapControl(MilScan, M_DEFAULT, M_PIXEL_SIZE_X     , SCALE_X       );
      M3dmapControl(MilScan, M_DEFAULT, M_PIXEL_SIZE_Y     , SCALE_Y       );
      M3dmapControl(MilScan, M_DEFAULT, M_GRAY_LEVEL_SIZE_Z, SCALE_Z       );
      M3dmapControl(MilScan, M_DEFAULT, M_WORLD_POS_X      , WORLD_OFFSET_X);
      M3dmapControl(MilScan, M_DEFAULT, M_WORLD_POS_Y      , WORLD_OFFSET_Y);
      M3dmapControl(MilScan, M_DEFAULT, M_WORLD_POS_Z      , WORLD_OFFSET_Z);

      DepthMapSizeX = DEPTH_MAP_SIZE_X;
      DepthMapSizeY = DEPTH_MAP_SIZE_Y;
      DepthMapType  = 16+M_UNSIGNED;
      }
   else
      {
      M3dmapGetResult(MilScan, M_DEFAULT, M_CORRECTED_DEPTH_MAP_SIZE_X     +M_TYPE_MIL_INT,
                                                                        &DepthMapSizeX   );
      M3dmapGetResult(MilScan, M_DEFAULT, M_CORRECTED_DEPTH_MAP_SIZE_Y     +M_TYPE_MIL_INT,
                                                                        &DepthMapSizeY   );
      M3dmapGetResult(MilScan, M_DEFAULT, M_CORRECTED_DEPTH_MAP_BUFFER_TYPE+M_TYPE_MIL_INT,
                                                                        &DepthMapType    );
      }

   /* Allocate images for depth map. */
   MbufAlloc2d(MilSystem, DepthMapSizeX, DepthMapSizeY, DepthMapType    ,
                                                   M_IMAGE+M_PROC+M_DISP, &MilDepthMap   );

   if (ConvParams.LaserIntensitySizeBit != NO_INFORMATION)
      {
      M3dmapGetResult(MilScan, M_DEFAULT, M_INTENSITY_MAP_BUFFER_TYPE      +M_TYPE_MIL_INT,
                                                                        &IntensityMapType);
      MbufAlloc2d(MilSystem, DepthMapSizeX, DepthMapSizeY, IntensityMapType,
                                                  M_IMAGE+M_PROC+M_DISP, &MilIntensityMap);
      }
   else
      {
      MilIntensityMap = M_NULL;
      }

   /* Get depth map from accumulated information in the result buffer. */
   M3dmapExtract(MilScan, MilDepthMap, MilIntensityMap, M_CORRECTED_DEPTH_MAP,
                 M_DEFAULT, M_DEFAULT);

   /* Clean the depth map */
   CleanDepthMap(MilDepthMap);
      
   /* If we have a calibrated camera and a color image we want to create a color image over the depth map */
   if(ContextType == M_CALIBRATED_CAMERA_LINEAR_MOTION && (ConvParams.ColorStreamOffset != NO_INFORMATION || ConvParams.HiResColorStreamOffset != NO_INFORMATION))
      {
      /* If high resolution color is available */
      if(ConvParams.HiResColorStreamOffset != NO_INFORMATION)
         {
         Remap8BitImage(MilHiresColorImage);
         CreateColorMap(MilSystem, MilDepthMap, MilCalibration, MilHiresColorImage, ConveyorSpeed, 0.5, ConvParams.MeasROIOffsetX, -40.5, &MilColorMap);
         }
      else
         {
         Remap8BitImage(MilColorImage);
         CreateColorMap(MilSystem, MilDepthMap, MilCalibration, MilColorImage, ConveyorSpeed, 1.0, ConvParams.MeasROIOffsetX, 0, &MilColorMap);   
         }
      }

   /* Fill the gaps of the depth map */    
   M3dmapControl(MilScan, M_DEFAULT, M_FILL_MODE                 , M_Y_THEN_X);
   M3dmapControl(MilScan, M_DEFAULT, M_FILL_SHARP_ELEVATION      , M_MIN     );
   M3dmapControl(MilScan, M_DEFAULT, M_FILL_SHARP_ELEVATION_DEPTH, GAP_DEPTH );
   M3dmapExtract(MilScan, MilDepthMap, M_NULL, M_FILL_MISSING_DATA_ONLY, M_DEFAULT, M_DEFAULT);
   
   /* Set the depth map display view mode */
   if(DepthMapType != 8+M_UNSIGNED)
      MdispControl(MilDepthMapDisplay, M_VIEW_MODE, M_AUTO_SCALE);

   /* Stop displaying the original display */
   MdispSelect(MilDisplay, M_NULL);
   
#if M_MIL_USE_WINDOWS && !M_MIL_USE_CE
   MIL_DISP_D3D_HANDLE DispD3DHandle = M_NULL;
   if(ContextType == M_CALIBRATED_CAMERA_LINEAR_MOTION)
      {
       /* Try to allocate D3D display. */      
      DispD3DHandle = MdepthD3DAlloc(MilDepthMap, MilColorMap,
                                    D3D_DISPLAY_SIZE_X,
                                    D3D_DISPLAY_SIZE_Y,
                                    M_DEFAULT,
                                    M_DEFAULT,
                                    M_DEFAULT,
                                    M_DEFAULT,
                                    M_DEFAULT,
                                    M_DEFAULT,
                                    M_NULL);

      if(DispD3DHandle != NULL)
         {
         /* Send a message to window so that the default mode is a point cloud */
         MdispD3DShow(DispD3DHandle);
         MdispD3DPrintHelp(DispD3DHandle);
         DepthMapDisplayOffsetX = D3D_DISPLAY_SIZE_X;
         }

      /* Allocate the color map display */
      MdispAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &MilColorMapDisplay);
      MIL_DOUBLE Scale = SetDepthMapDisplay(MilColorMapDisplay, DepthMapDisplayOffsetX, 0, DepthMapSizeX, DepthMapSizeY);
      DepthMapDisplayOffsetY = (MIL_INT)((MIL_DOUBLE)DepthMapSizeY * Scale);
      MdispSelect(MilColorMapDisplay, MilColorMap);
      }
#endif   

   SetDepthMapDisplay(MilDepthMapDisplay, DepthMapDisplayOffsetX, DepthMapDisplayOffsetY, DepthMapSizeX, DepthMapSizeY);
   MdispSelect(MilDepthMapDisplay, MilDepthMap);
      
         
   MosPrintf(MIL_TEXT("The corrected depth map of the object is being displayed.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   /* Deselect the depth map */
   MdispSelect(MilDepthMapDisplay, M_NULL);
      
#if M_MIL_USE_WINDOWS && !M_MIL_USE_CE
   if (DispD3DHandle != M_NULL)
      {
      MdispD3DHide(DispD3DHandle);
      MdispD3DFree(DispD3DHandle);
      }
#endif

   /* Free all allocations. */
   if (MilLaserIntensityLine != M_NULL)
      {
      MbufFree(MilLaserIntensityLine);
      MbufFree(MilLaserIntensityImage);
      }
   FreeScanResultImageBuffers(ConvParams.LaserScatterSizeBit, NB_GRABS, MilLaserScatterImage, MilLaserScatterChilds, MilLaserScatterLine);

   FreeScanResultImageBuffers(ConvParams.GrayStreamOffset      , NB_GRABS, MilGrayImage      , MilGrayChilds      , MilGrayLine);
   FreeScanResultImageBuffers(ConvParams.HiResGrayStreamOffset , NB_GRABS, MilHiresGrayImage , MilHiresGrayChilds , MilHiresGrayLine);
   FreeScanResultImageBuffers(ConvParams.ColorStreamOffset     , NB_GRABS, MilColorImage     , MilColorChilds     , MilColorLine);
   FreeScanResultImageBuffers(ConvParams.HiResColorStreamOffset, NB_GRABS, MilHiresColorImage, MilHiresColorChilds, MilHiresColorLine);
   FreeScanResultImageBuffers(ConvParams.ScatterStreamOffset   , NB_GRABS, MilScatterImage   , MilScatterChilds   , MilScatterLine);

   /* Close down the frame grabber and camera */
   CloseDown(pCameraSystem);
   
   if(MilGraList)
      MgraFree(MilGraList);

   if(MilColorMap != M_NULL)
      MbufFree(MilColorMap);

   if(MilColorMapDisplay != M_NULL)
      MdispFree(MilColorMapDisplay);

   if (MilIntensityMap != M_NULL)
      MbufFree(MilIntensityMap);
   
   MbufFree(MilDepthMap);
   
   if (MilCalibration != M_NULL)
      McalFree(MilCalibration);
   
   M3dmapFree(MilScan);
   M3dmapFree(MilLaser);
   
   MbufFree(MilLaserPositionLine);
   MbufFree(MilLaserPositionImage);
   MbufFree(MilMeasImage);
   MbufFree(MilCalGridImage);
   MbufFree(MilImage);
   MbufFree(MilOneLineImage);
   MbufFree(MilManyLinesImage);
   
   MdispControl(MilDisplay, M_VIEW_MODE, M_DEFAULT);
   }
/*****************************************************************************
Hook function for FrameGrabberProcess(). Copy the ranger image data to the buffer.

   HookType         (in)  Type of hook.
   MilRangerImage   (in)  Image outputted by the Ranger Camera.
   UserDataPtr      (out) Data of the user. Should be a pointer to the destination image. 
*****************************************************************************/
MIL_INT MFTYPE CopyFct(MIL_INT HookType, MIL_ID MilRangerImage, void* UserDataPtr)
   {
   MIL_ID* pDestMilImage = (MIL_ID*)UserDataPtr;

   /* Copy the image from the ranger to the Mil image */
   MbufCopy(MilRangerImage, *pDestMilImage);

   return M_NULL;
   }

/*****************************************************************************
Hook function for FrameGrabberProcess(). Flip the ranger image data to the buffer.

   HookType         (in)  Type of hook.
   MilRangerImage   (in)  Image outputted by the Ranger Camera.
   UserDataPtr      (out) Data of the user. Should be a pointer to the destination image. 
*****************************************************************************/
MIL_INT MFTYPE HorFlipFct(MIL_INT HookType, MIL_ID MilRangerImage, void* UserDataPtr)
   {
   MIL_ID* pDestMilImage = (MIL_ID*)UserDataPtr;

   /* Flip the image from the ranger to the Mil image */
   MimFlip(MilRangerImage, *pDestMilImage, M_FLIP_HORIZONTAL, M_DEFAULT);
   
   return M_NULL;
   }
/*****************************************************************************
 Hook function for FrameGrabberProcess(). Converts the profile data returned by the ranger camera
 and feed it to 3dmap.

   HookType         (in)  Type of hook.
   MilRangerImage   (in)  Image outputted by the Ranger Camera.
   UserDataPtr      (out) Data of the user. Should be a pointer to the AddScanProcFuncDataStruct. 
*****************************************************************************/
MIL_INT MFTYPE AddScanProcFct(MIL_INT HookType, MIL_ID MilRangerImage, void* UserDataPtr)
   {
   AddScanProcFctDataStruct* pAddScanProcFctData =
      static_cast<AddScanProcFctDataStruct*>(UserDataPtr);
   
   MIL_INT GrabIndex = pAddScanProcFctData->GrabIndex;
   
   /* Convert SICK format to MIL format. */
   M3dcamRangerToMIL(MilRangerImage,
                     pAddScanProcFctData->MilLaserPositionImage,
                     pAddScanProcFctData->MilLaserIntensityImage,
                     pAddScanProcFctData->pMilLaserScatterChilds[GrabIndex],
                     pAddScanProcFctData->pMilGrayChilds[GrabIndex],
                     pAddScanProcFctData->pMilHiresGrayChilds[GrabIndex],
                     pAddScanProcFctData->pMilColorChilds[GrabIndex],
                     pAddScanProcFctData->pMilHiresColorChilds[GrabIndex],
                     pAddScanProcFctData->pMilScatterChilds[GrabIndex],
                     M_NULL,
                     pAddScanProcFctData->ConvParamsPtr);

   pAddScanProcFctData->GrabIndex = (GrabIndex + 1) % pAddScanProcFctData->NbGrabs;

   /* Analyze the image to extract laser line and correct its depth. */
   M3dmapAddScan(pAddScanProcFctData->MilLaser,
                 pAddScanProcFctData->MilScan,
                 pAddScanProcFctData->MilLaserPositionImage,
                 pAddScanProcFctData->MilLaserIntensityImage,
                 M_NULL,
                 M_DEFAULT,
                 M_LINE_ALREADY_EXTRACTED);

   return M_NULL;
   }

/*****************************************************************************
 Thread function that starts the conveyor.
  
   UserDataPtr      (out) Data of the user. Should be a pointer to the StartConveyorFctDataStruct. 
*****************************************************************************/
MIL_UINT32 MFTYPE StartConveyorFct(void *UserDataPtr)
   {
   StartConveyorFctDataStruct* pStartConveyorData = (StartConveyorFctDataStruct*) UserDataPtr;

   /* Wait for the grab to start to start the conveyor */
   MthrWait(pStartConveyorData->MilStartStageEvent, M_EVENT_WAIT, M_NULL);

   /* CONVEYOR_CODE: Put your conveyor start code here */
   
   return 0;
   }

/*****************************************************************************
 Function that allocates images for a specific measurements.

    AllocCondition      (in)  Condition that determines if the images need to be created.
    SizeBand            (in)  The size band of the buffer to allocate.
    SizeX               (in)  The size X of the buffer.
    SizeBit             (in)  The size bit of the buffer. 
    pMilResultImage     (out) The complete result image.
    pMilResultChilds    (out) The childs of the result image that holds individual scans.
    pMilResultOneLine   (out) The child of one line of the result image.
*****************************************************************************/
void AllocateScanResultImageBuffers(MIL_INT AllocCondition,
                                    MIL_INT SizeBand,
                                    MIL_INT SizeX,
                                    MIL_INT SizeY,
                                    MIL_INT SizeBit,
                                    MIL_INT NbGrabs,
                                    MIL_ID* pMilResultImage,
                                    MIL_ID* pMilResultChilds,
                                    MIL_ID* pMilResultOneLine)
   {
   if(AllocCondition != NO_INFORMATION)
      {
      MbufAllocColor(M_DEFAULT_HOST, SizeBand, SizeX, SizeY, SizeBit+M_UNSIGNED, M_IMAGE+M_PROC, pMilResultImage);
      MbufChild2d(*pMilResultImage, 0, 0, SizeX, 1, pMilResultOneLine);
      for(MIL_INT GrabIdx = 0, OffsetY = 0; GrabIdx < NbGrabs; GrabIdx++, OffsetY += NB_SCANS_PER_GRAB)
         MbufChild2d(*pMilResultImage, 0, OffsetY, SizeX, NB_SCANS_PER_GRAB, &pMilResultChilds[GrabIdx]);
      }
   else
      {
      *pMilResultImage = M_NULL;
      *pMilResultOneLine = M_NULL;
      for(MIL_INT GrabIdx = 0; GrabIdx < NbGrabs; GrabIdx++)
         pMilResultChilds[GrabIdx]  = M_NULL;
      }
   }

/*****************************************************************************
 Function that frees images for a specific measurements.

 FreeCondition       (in) Condition that determines if the images need to be freed.
 MilResultImage      (in) The complete result image.
 pMilResultChilds    (in) The childs of the result image that holds individual scans.
 MilResultOneLine    (in) The child of one line of the result image.
*****************************************************************************/
void FreeScanResultImageBuffers(MIL_INT FreeCondition, MIL_INT NbGrabs, MIL_ID MilResultImage, MIL_ID* pMilResultChilds, MIL_ID MilResultOneLine)
   {
   if(FreeCondition != NO_INFORMATION)
      {
      for(MIL_INT GrabIdx = 0; GrabIdx < NbGrabs; GrabIdx++)
         MbufFree(pMilResultChilds[GrabIdx]);
      MbufFree(MilResultOneLine);
      MbufFree(MilResultImage);
      }
   }

/*****************************************************************************
Creates the color map over the depth map using the color image, depth map and calibration.

   MilSystem        (in)  ID of the system to use.
   MilDisplay       (in)  ID of the MIL display to use.
   MilCalibration   (in)  ID of the camera calibration of 3dmap.
   MilColorImage    (in)  Color line scan image. 
   ConveyorSpeed    (in)  The speed of the conveyor in mm/frame.
   PixelXIncrement  (in)  The increment in of pixel coordinate between two pixel index(0.5 for Hires).
   MeasROIOffsetS   (in)  The offsetX in pixel coordinates of the measurement ROI.
   ColorLinesOffset (in)  The offset in Y in pixel coordinates of the blue color line.
   pMilColorMap     (out) The created color map
*****************************************************************************/
void CreateColorMap(MIL_ID MilSystem,
                    MIL_ID MilDepthMap,
                    MIL_ID MilCalibration,
                    MIL_ID MilColorImage,
                    double ConveyorSpeed,
                    MIL_DOUBLE PixelXIncrement,
                    MIL_INT MeasROIOffsetX,
                    MIL_DOUBLE ColorLinesOffset,
                    MIL_ID* pMilColorMap) 
   {
   /* Get the size of the depth map */
   MIL_INT DepthMapSizeX = MbufInquire(MilDepthMap, M_SIZE_X, M_NULL);
   MIL_INT DepthMapSizeY = MbufInquire(MilDepthMap, M_SIZE_Y, M_NULL);

   /* Get the number of pixels in X */
   MIL_INT NbPixelX = MbufInquire(MilColorImage, M_SIZE_X, M_NULL);   
 
   /* Allocate the color map image */
   MbufAllocColor(MilSystem, 3, DepthMapSizeX, DepthMapSizeY, 8+M_UNSIGNED, M_IMAGE+M_PROC+M_DISP, pMilColorMap);
   MbufClear(*pMilColorMap, 0);

   /* Get the host address of the depth map buffer and its pitch */
   MIL_UINT16* pDepthMap = (MIL_UINT16*) MbufInquire(MilDepthMap, M_HOST_ADDRESS, M_NULL);
   MIL_INT DepthMapPitch = MbufInquire(MilDepthMap, M_PITCH, M_NULL);

   /* Get the position of the camera in the relative coordinate system */
   MIL_DOUBLE WorldCamPosX, WorldCamPosY, WorldCamPosZ;
   McalGetCoordinateSystem(MilCalibration, M_CAMERA_COORDINATE_SYSTEM, M_RELATIVE_COORDINATE_SYSTEM,
                           M_TRANSLATION, M_NULL, &WorldCamPosX, &WorldCamPosY, &WorldCamPosZ, M_NULL);   

   /* Allocate the arrays */
   MIL_DOUBLE* pBandLineXPixel = new MIL_DOUBLE[NbPixelX];
   MIL_DOUBLE* pBandLineYPixel = new MIL_DOUBLE[NbPixelX];
   MIL_DOUBLE* LinesWorldVecData = new MIL_DOUBLE[3*NbPixelX];
   MIL_DOUBLE* pBandLineXWorldVec = LinesWorldVecData;
   MIL_DOUBLE* pBandLineYWorldVec = LinesWorldVecData + NbPixelX;
   MIL_DOUBLE* pBandLineZWorldVec = LinesWorldVecData + 2*NbPixelX;
   MIL_DOUBLE* pBandLineXZWorldVecXZRatio = new MIL_DOUBLE[NbPixelX];
   
   /* Fill the x coordinate */
   MIL_DOUBLE PixelX = (MIL_DOUBLE)MeasROIOffsetX + (0.5 * PixelXIncrement) - 0.5;
   for(MIL_INT PixelIdx = 0; PixelIdx < NbPixelX; PixelIdx++, PixelX += PixelXIncrement)
      pBandLineXPixel[PixelIdx] = PixelX;      

   /* Allocate the warp luts */
   MIL_ID MilWarpLutX = MbufAlloc2d(MilSystem, DepthMapSizeX, DepthMapSizeY, 32+M_SIGNED, M_LUT, M_NULL);
   MIL_ID MilWarpLutY = MbufAlloc2d(MilSystem, DepthMapSizeX, DepthMapSizeY, 32+M_SIGNED, M_LUT, M_NULL);

   /* Get the data pointer to the warp luts and the pitch */
   MIL_INT32* pWarpLutXData = (MIL_INT32*)MbufInquire(MilWarpLutX, M_HOST_ADDRESS, M_NULL);
   MIL_INT32* pWarpLutYData = (MIL_INT32*)MbufInquire(MilWarpLutY, M_HOST_ADDRESS, M_NULL);
   MIL_INT WarpLutPitch = MbufInquire(MilWarpLutX, M_PITCH, M_NULL);

   /* Calculate the number of fixed point and the lut mult factor*/
   MIL_INT NbFixedPoint = 8;
   MIL_DOUBLE LutMultFactor = (MIL_DOUBLE)(1 << NbFixedPoint);

   /* Get the color position in the color image of each value of the depth map */
   MIL_UINT16* pCurDepthMap;
   MIL_INT32* pCurWarpLutXData;
   MIL_INT32* pCurWarpLutYData;
   for(MIL_INT BandIdx = 0; BandIdx < 3; BandIdx++)
      {
      /* Get a child of the color map buffer and of the color buffer */
      MIL_ID MilColorBand = MbufChildColor(MilColorImage, BAND_COLOR[BandIdx], M_NULL);
      MIL_ID MilColorMapBand = MbufChildColor(*pMilColorMap, BAND_COLOR[BandIdx], M_NULL);

      /* Fill the y coordinate of the pixel of the line */
      MIL_DOUBLE BandYPixel = (8 - BandIdx * 4) + ColorLinesOffset; /* Blue = 0 or -40.5, Green = 4 or -36.5, Red = 8 or -32.5 */
      for(MIL_INT PixelIdx = 0; PixelIdx < NbPixelX; PixelIdx++)
         pBandLineYPixel[PixelIdx] = BandYPixel;
         
      /* Get the unit direction vector of all the pixels of the color line */
      McalTransformCoordinate3dList(MilCalibration,
                                    M_PIXEL_COORDINATE_SYSTEM,
                                    M_RELATIVE_COORDINATE_SYSTEM,
                                    NbPixelX,
                                    pBandLineXPixel,
                                    pBandLineYPixel,
                                    M_NULL,
                                    pBandLineXWorldVec,
                                    pBandLineYWorldVec,
                                    pBandLineZWorldVec,
                                    M_UNIT_DIRECTION_VECTOR);
      
      /* Get the ratio Vx/Vz */
      for(MIL_INT PixelIdx = 0; PixelIdx < NbPixelX; PixelIdx++)
         pBandLineXZWorldVecXZRatio[PixelIdx] =  pBandLineXWorldVec[PixelIdx]/pBandLineZWorldVec[PixelIdx];
         
      for(MIL_INT y = 0; y < DepthMapSizeY; y++)
         {
         /* Get the current pointer of the luts and of the depth map at the beginning of the current line*/
         pCurDepthMap = &pDepthMap[DepthMapPitch*y];
         pCurWarpLutXData = &pWarpLutXData[WarpLutPitch*y];
         pCurWarpLutYData = &pWarpLutYData[WarpLutPitch*y];
         for(MIL_INT x = 0; x < DepthMapSizeX; x++, pCurDepthMap++, pCurWarpLutXData++, pCurWarpLutYData++)
            {
            /* If we have invalid data, set the position to be outside of the color image */
            if(*pCurDepthMap == MIL_UINT16_MAX)
               {
               *pCurWarpLutXData = -1;
               *pCurWarpLutYData = -1;
               }
            else
               {
               /* Get the world position of the depth map pixel */
               MIL_DOUBLE WorldX = x * SCALE_X + WORLD_OFFSET_X;
               MIL_DOUBLE WorldY = y * SCALE_Y + WORLD_OFFSET_Y;
               MIL_DOUBLE WorldZ = (*pCurDepthMap) * SCALE_Z + WORLD_OFFSET_Z;

               /* Get the direction vector in X and Z between the camera position axis and the depth
                  position. In the 3dmap coordinate system, the scanning causes the camera coordinate
                  system to move along the Y axis. */
               MIL_DOUBLE CamToDepthVx = WorldX - WorldCamPosX;
               MIL_DOUBLE CamToDepthVz = WorldZ - WorldCamPosZ;
               MIL_DOUBLE CamToDepthVxVzRatio = CamToDepthVx/CamToDepthVz;

               /* Find the pixel index whose Vx/Vz ratio is closest to the CamToDepthVxVzRatio.
                  Use lower_bound binary search algorithm to get the index of the closest ratio */
               MIL_DOUBLE* pClosestRatio = lower_bound(pBandLineXZWorldVecXZRatio, pBandLineXZWorldVecXZRatio + NbPixelX, CamToDepthVxVzRatio);
               MIL_INT ClosestRatioIdx = pClosestRatio - pBandLineXZWorldVecXZRatio;
                              
               /* We now know the unit vector from the camera to the depth point and the X to get
                  from the MilColorImage. We must now calculate the world position Y of the camera
                  when that point was seen by the camera color line. For simplicity, we will take 
                  only the z component of the depth point to camera vector to calculate relative
                  Y position of the camera with regards to the depth point which gives us the 
                  real world position of the camera when the point was grabbed */
               MIL_DOUBLE Scale = CamToDepthVz / pBandLineZWorldVec[ClosestRatioIdx];
               MIL_DOUBLE RealWorldCamPosY = WorldY - pBandLineYWorldVec[ClosestRatioIdx] * Scale;

               /* Get the rounded color line idx, i.e. the frame that saw that world point in the color band line */
               MIL_DOUBLE ColorLineIdx = ((RealWorldCamPosY - WorldCamPosY) / -ConveyorSpeed);

               /* Set the luts */
               *pCurWarpLutXData = (MIL_INT32)(ClosestRatioIdx * LutMultFactor);
               *pCurWarpLutYData = (MIL_INT32)(ColorLineIdx * LutMultFactor);
               }
            }
         }

      /* The warp lut is filled, we can now warp the band */
      MimWarp(MilColorBand, MilColorMapBand, MilWarpLutX, MilWarpLutY, M_WARP_LUT + M_FIXED_POINT + NbFixedPoint, M_BILINEAR);

      /* Free the band childs */
      MbufFree(MilColorBand);
      MbufFree(MilColorMapBand);
      }   

   /* Put some invalid data color in the invalid data regions */
   MIL_ID MilInvalidDataColor = MbufAllocColor(MilSystem, 3, DEPTH_MAP_SIZE_X, DEPTH_MAP_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_PROC, M_NULL);
   MbufClear(MilInvalidDataColor, INVALID_COLOR);
   MbufCopyCond(MilInvalidDataColor, *pMilColorMap, MilDepthMap, M_EQUAL, MIL_UINT16_MAX);
   MbufFree(MilInvalidDataColor);

   /* Free the arrays */
   delete [] pBandLineXPixel;
   delete [] pBandLineYPixel;
   delete [] LinesWorldVecData;
   delete [] pBandLineXZWorldVecXZRatio;

   /* Free the warp lut */
   MbufFree(MilWarpLutX);
   MbufFree(MilWarpLutY);
   }

/*****************************************************************************
 Uses MimDraw() to show the image of the laser line the SICK rangerE has extracted.

   MilDisplay       (in)  ID of the MIL display to use.
   MilPositionLine  (in)  Single-line range image (output of M3dcamRangerToMIL()).
   MilIntensityLine (in)  Single-line intensity image (output of M3dcamRangerToMIL()) or M_NULL.
   MilMeasImage     (out) Temporary image to use to display laser line.
   FixedPoint       (in)  Fixed point used for MilPositionLine (taken from RangerParams struct).
*****************************************************************************/
void DisplayLaserLine(MIL_ID  MilDisplay,
                      MIL_ID  MilPositionLine,
                      MIL_ID  MilIntensityLine,
                      MIL_ID  MilMeasImage,
                      MIL_INT FixedPoint)
   {
   MIL_ID MilPreviousImage;
   MdispInquire(MilDisplay, M_SELECTED, &MilPreviousImage);

   MbufClear(MilMeasImage, 0.0);
   MimDraw(M_DEFAULT, MilPositionLine, MilIntensityLine, MilMeasImage,
           M_DRAW_PEAKS+M_1D_COLUMNS+M_LINES, 0, 2.0, M_FIXED_POINT+FixedPoint);

   if (MbufInquire(MilMeasImage, M_SIZE_BIT, M_NULL) != 8)
      MdispControl(MilDisplay, M_VIEW_MODE, M_AUTO_SCALE);
   MdispSelect(MilDisplay, MilMeasImage);

   MosPrintf(MIL_TEXT("The extracted laser line is displayed.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   MdispControl(MilDisplay, M_VIEW_MODE, M_DEFAULT);
   MdispSelect(MilDisplay, MilPreviousImage);
   }

/*****************************************************************************
 Uses MimDraw() to show the image of the laser line the SICK rangerE has extracted
 with a drawing of the relative coordinate system moved to the 3dmap coordinate system.

   MilDisplay             (in)  ID of the MIL display to use.
   MilGraList             (in)  ID of the MIL graphic list associated to the display.
   MilCalibration         (in)  ID of the moved camera calibration.
   MilPositionLine        (in)  Single-line range image (output of M3dcamRangerToMIL()).
   MilIntensityLine       (in)  Single-line intensity image (output of M3dcamRangerToMIL()) or M_NULL
   MilMeasImage           (in)  Image where the laser line was extracted.
   FixedPoint             (in)  Fixed point used for MilPositionLine (taken from RangerParams struct).
   LaserExtractionOffsetX (in)  Offset in X of the laser extraction ROI.
   LaserExtractionOffsetY (in)  Offset in Y of the laser extraction ROI.
*****************************************************************************/
void DisplayRelativeCoordinateSystem(MIL_ID MilDisplay, 
                                    MIL_ID MilGraList,
                                    MIL_ID MilCalibration,
                                    MIL_ID MilPositionLine,
                                    MIL_ID MilIntensityLine,
                                    MIL_ID MilMeasImage,
                                    MIL_INT FixedPoint,
                                    MIL_INT LaserExtractionOffsetX,
                                    MIL_INT LaserExtractionOffsetY)
   {
   MIL_ID MilOverlay = MdispInquire(MilDisplay, M_OVERLAY_ID, M_NULL);
   MIL_ID MilOverlayLaserChild = MbufChild2d(MilMeasImage,
                                             LaserExtractionOffsetX,
                                             LaserExtractionOffsetY,
                                             MbufInquire(MilMeasImage, M_SIZE_X, M_NULL) - LaserExtractionOffsetX,
                                             MbufInquire(MilMeasImage, M_SIZE_Y, M_NULL) - LaserExtractionOffsetY,
                                             M_NULL);

   /* Disable the display updates. */
   MdispControl(MilDisplay, M_UPDATE, M_DISABLE);

   /* Draw the laser line. */
   MimDraw(M_DEFAULT, MilPositionLine, MilIntensityLine, MilOverlayLaserChild,
           M_DRAW_PEAKS+M_1D_COLUMNS+M_LINES, 0, 2.0, M_FIXED_POINT+FixedPoint);
   
   MbufFree(MilOverlayLaserChild);
     
   /* Draw the coordinate system in the graphic list. */
   MgraColor(M_DEFAULT, M_COLOR_CYAN);
   MgraClear(M_DEFAULT, MilGraList);
   McalDraw(M_DEFAULT, MilCalibration, MilGraList, M_DRAW_RELATIVE_COORDINATE_SYSTEM, M_DEFAULT, M_DEFAULT);

   /* Select the measurement image. */
   if (MbufInquire(MilMeasImage, M_SIZE_BIT, M_NULL) != 8)
      MdispControl(MilDisplay, M_VIEW_MODE, M_AUTO_SCALE);
   MdispSelect(MilDisplay, MilMeasImage);

   /* Enable the display updates */
   MdispControl(MilDisplay, M_UPDATE, M_ENABLE);

   MosPrintf(MIL_TEXT("The 3dmap coordinate system is displayed over the laser line image.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();
   }

/*****************************************************************************
 Uses McalDraw() to show the camera calibration grid with crosses on the calibration
 points in the overlay.

   MilDisplay      (in) ID of the MIL display to use.
   MilGraList      (in) ID of the MIL graphic list associated to the display.
   MilCalGridImage (in) Image of the calibration grid.
   MilCalibration  (in) Calibration context.
*****************************************************************************/
void DisplayCalibrationPoints(MIL_ID MilDisplay,
                              MIL_ID MilGraList,
                              MIL_ID MilCalGridImage,
                              MIL_ID MilCalibration)
   {
   /* Empty the graphic list and select the cal grid image. */
   MgraClear(M_DEFAULT, MilGraList);
   MdispSelect(MilDisplay, MilCalGridImage);
   
   /* Draw calibration points. */
   MgraColor(M_DEFAULT, M_COLOR_GREEN);
   McalDraw(M_DEFAULT, MilCalibration, MilGraList, M_DRAW_WORLD_POINTS,
      M_DEFAULT, M_DEFAULT);

   MosPrintf(MIL_TEXT("Camera calibration points are being displayed.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   MgraClear(M_DEFAULT, MilGraList);
   }

/*****************************************************************************
 Function that sets the display zoom of the depth map display to fit the 
 constrained set by the user

   MilDisplay      (in) ID of the MIL display to use.
   DepthmapSizeX   (in) Size of the depth map image.
   DepthMapSizeY   (in) Size of the depth map image.
*****************************************************************************/
MIL_DOUBLE SetDepthMapDisplay(MIL_ID MilDepthMapDisplay, MIL_INT DisplayOffsetX, MIL_INT DisplayOffsetY, MIL_INT DepthMapSizeX, MIL_INT DepthMapSizeY)
   {
   MIL_INT DisplaySpanX = DisplayOffsetX + DepthMapSizeX;
   MIL_INT DisplaySpanY = DisplayOffsetY + DepthMapSizeY;
   MIL_DOUBLE ScaleX = DisplaySpanX > MAX_DISPLAY_SIZE_X ? (MAX_DISPLAY_SIZE_X - DisplayOffsetX)/DepthMapSizeX : 1.0;
   MIL_DOUBLE ScaleY = DisplaySpanY > MAX_DISPLAY_SIZE_Y ? (MAX_DISPLAY_SIZE_Y - DisplayOffsetY)/DepthMapSizeY : 1.0;
   MIL_DOUBLE Scale = ScaleX < ScaleY ? ScaleX : ScaleY;

   /* Set the zoom factor */
   MdispZoom(MilDepthMapDisplay, Scale, Scale);

   /* Set the window position */
   MdispControl(MilDepthMapDisplay, M_WINDOW_INITIAL_POSITION_X, DisplayOffsetX);
   MdispControl(MilDepthMapDisplay, M_WINDOW_INITIAL_POSITION_Y, DisplayOffsetY);

   return Scale;
   }

/*****************************************************************************
 Function that cleans the depth map by applying a median filter. An erosion is
 done on the data touching the invalid data.

   MilDepthMap (in) Depth map image that will be cleaned.
*****************************************************************************/
void CleanDepthMap(MIL_ID MilDepthMap)
   {
   /* Get the system of the depth map */
   MIL_ID MilSystem = MbufInquire(MilDepthMap, M_OWNER_SYSTEM, M_NULL);

   /* Get the size and depth of the depth map */
   MIL_INT DepthSizeX = MbufInquire(MilDepthMap, M_SIZE_X, M_NULL);
   MIL_INT DepthSizeY = MbufInquire(MilDepthMap, M_SIZE_Y, M_NULL);
   MIL_INT DepthType = MbufInquire(MilDepthMap, M_TYPE, M_NULL);
   MIL_DOUBLE DepthMaxValue;
   MbufInquire(MilDepthMap, M_MAX, &DepthMaxValue);

   /* Allocate masks for the data to erode and the data to apply median */
   MIL_ID MilErodeMaskImage = MbufAlloc2d(MilSystem, DepthSizeX, DepthSizeY, DepthType, M_IMAGE+M_PROC, M_NULL);
   MIL_ID MilMedianMaskImage = MbufAlloc2d(MilSystem, DepthSizeX, DepthSizeY, DepthType, M_IMAGE+M_PROC, M_NULL);
   MIL_ID MilMedianDilateMaskImage = MbufAlloc2d(MilSystem, DepthSizeX, DepthSizeY, DepthType, M_IMAGE+M_PROC, M_NULL);

   /* Allocate the median and erode image */
   MIL_ID MilRankImage  = MbufAlloc2d(MilSystem, DepthSizeX, DepthSizeY, DepthType, M_IMAGE+M_PROC, M_NULL);
   MIL_ID MilErodeImage = MbufAlloc2d(MilSystem, DepthSizeX, DepthSizeY, DepthType, M_IMAGE+M_PROC, M_NULL);

   /* Get the valid data mask */
   MimBinarize(MilDepthMap, MilMedianMaskImage, M_EQUAL, (MIL_DOUBLE)DepthMaxValue, M_NULL);
   MimRank(MilMedianMaskImage, MilMedianMaskImage, M_5X5_RECT, M_MEDIAN, M_BINARY);
     
   /* Get the erode data mask */
   MimDilate(MilMedianMaskImage, MilMedianDilateMaskImage, ERODE_MASK_WIDTH, M_BINARY);
   MimArith(MilMedianDilateMaskImage, MilMedianMaskImage, MilErodeMaskImage, M_SUB);

   /* Get the rank and eroded image */
   MimRank(MilDepthMap, MilRankImage, M_5X5_RECT, DEPTH_CLEANING_RANK, M_GRAYSCALE);
   MimErode(MilDepthMap, MilErodeImage, 1, M_GRAYSCALE);

   /* Put the data back in the depth map */   
   MimArith(MilDepthMap, MilMedianMaskImage, MilDepthMap, M_OR);
   MbufCopyCond(MilRankImage, MilDepthMap, MilMedianMaskImage, M_NOT_EQUAL, (MIL_DOUBLE)DepthMaxValue);
   MbufCopyCond(MilErodeImage, MilDepthMap, MilErodeMaskImage, M_EQUAL, (MIL_DOUBLE)DepthMaxValue);

   /* Free the buffers */
   MbufFree(MilRankImage);
   MbufFree(MilMedianMaskImage);
   MbufFree(MilMedianDilateMaskImage);
   MbufFree(MilErodeImage);
   MbufFree(MilErodeMaskImage);
   }

/*****************************************************************************
 Function that moves the relative coordinate system of the camera on the 3dmap
 coordinate system. This function assumes that the laser plane is perpendicular
 to the conveyor plane.

 MilLaser       (in) The calibrated 3dmap context.
 MiLCalibration (in) The camera calibration used to calibrate the 3dmap context.
*****************************************************************************/
void MoveRelativeCoordToLaserCoord(MIL_ID MilLaser, MIL_ID MilCalibration)
   {
   MIL_DOUBLE PlaneA, PlaneB, PlaneC, PlaneD;
   M3dmapInquire(MilLaser, M_DEFAULT, M_LASER_PLANE_A, &PlaneA);
   M3dmapInquire(MilLaser, M_DEFAULT, M_LASER_PLANE_B, &PlaneB);
   M3dmapInquire(MilLaser, M_DEFAULT, M_LASER_PLANE_C, &PlaneC);
   M3dmapInquire(MilLaser, M_DEFAULT, M_LASER_PLANE_D, &PlaneD);

   if(PlaneB == 0)
      FatalError(MIL_TEXT("The laser plane is not good."));

   if(PlaneC != 0)
      FatalError(MIL_TEXT("The laser plane should be perpendicular to the conveyor"));

   MIL_DOUBLE LaserVx = PlaneB;
   MIL_DOUBLE LaserVy = -PlaneA;

   MIL_DOUBLE StartPointX = 0.0;
   MIL_DOUBLE StartPointY = -PlaneD / PlaneB;

   MIL_DOUBLE Dot = LaserVy * -StartPointY;

   MIL_DOUBLE RelativeOriginX = Dot * LaserVx + StartPointX;
   MIL_DOUBLE RelativeOriginY = Dot * LaserVy + StartPointY;

   MIL_DOUBLE RelativeAngle = atan2(PlaneA, PlaneB) * 180 / PI;
   McalRelativeOrigin(MilCalibration, RelativeOriginX, RelativeOriginY, 0, RelativeAngle, M_ASSIGN);
   }

/*****************************************************************************
 Function that calculates the bands average values.

   MilColorImage       (in)  The color image.
   BandAverage         (out) The average of the 3 bands.
*****************************************************************************/
void CalculateColorBandsAverage(MIL_ID MilColorImage, MIL_DOUBLE BandAverage[3])
   {
   MIL_ID MilStatResult = MimAllocResult(MbufInquire(MilColorImage, M_OWNER_SYSTEM, M_NULL), M_DEFAULT, M_STAT_LIST, M_NULL);
   for(MIL_INT BandIdx = 0; BandIdx < 3; BandIdx++)
      {
      MIL_ID MilColorBand = MbufChildColor(MilColorImage, BAND_COLOR[BandIdx], M_NULL);
      MimStat(MilColorBand, MilStatResult, M_MEAN, M_NULL, M_NULL, M_NULL);
      MimGetResult(MilStatResult, M_MEAN + M_TYPE_DOUBLE, &BandAverage[BandIdx]);
      MbufFree(MilColorBand);
      }
   MimFree(MilStatResult);
   }

/*****************************************************************************
 Function that remaps the image to the full dynamic range.

    MilImage       (in)  The image that will be remapped.
*****************************************************************************/
void Remap8BitImage(MIL_ID MilImage)
   {
   // Work on the same system of the MilImage.
   MIL_ID MilSystem = MbufInquire(MilImage, M_OWNER_SYSTEM, M_NULL);

   // Alloc extreme result buffer.
   MIL_ID MilExtremeResult = MimAllocResult(MilSystem, 2, M_EXTREME_LIST, M_NULL);

   // Get the number of bands of the buffer.
   MIL_INT NbBands = MbufInquire(MilImage, M_SIZE_BAND, M_NULL);

   // Get the maximum and minimum value of the src buffer.
   MIL_INT ExtremeValues[2] = {MIL_INT_MAX, -MIL_INT_MAX};
   for(MIL_INT BandIdx = 0; BandIdx < NbBands; BandIdx++)
      {      
      // Get the current band.
      MIL_ID MilImageBand = MbufChildColor(MilImage, BAND_COLOR[BandIdx], M_NULL);
      
      // FInd the extremes of the band.
      MIL_INT CurExtremeValues[2];
      MimFindExtreme(MilImageBand, MilExtremeResult, M_MIN_VALUE+M_MAX_VALUE);
      MimGetResult(MilExtremeResult, M_VALUE+M_TYPE_MIL_INT, CurExtremeValues);
      
      // Update the overall min and max.
      if(CurExtremeValues[0] < ExtremeValues[0])
         ExtremeValues[0] = CurExtremeValues[0];
      if(CurExtremeValues[1] > ExtremeValues[1])
         ExtremeValues[1] = CurExtremeValues[1];

      // Free the band child.
      MbufFree(MilImageBand);
      }  

   // Alloc the LUT.
   MIL_ID MilLut = MbufAlloc1d(MilSystem, ExtremeValues[1]+1, 8+M_UNSIGNED, M_LUT, M_NULL);

   // Generate the ramp.
   MgenLutRamp(MilLut, 0, 0, ExtremeValues[0], 0);
   MgenLutRamp(MilLut, ExtremeValues[0], 0, ExtremeValues[1], 255);
   
   // Apply the mapping.
   MimLutMap(MilImage, MilImage, MilLut);

   // Free the buffers.
   MimFree(MilExtremeResult);
   MbufFree(MilLut);
   }

/*****************************************************************************
 Simple function that terminates the program abruptly when an error occurs.
*****************************************************************************/
void FatalError(const MIL_TEXT_CHAR* pMsg)
   {
   MosPrintf(pMsg);
   MosPrintf(MIL_TEXT("\nPress <Enter> to end.\n"));
   MosGetch();
   exit(-1);
   }