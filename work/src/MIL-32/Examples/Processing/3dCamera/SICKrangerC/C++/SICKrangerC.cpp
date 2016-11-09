/*****************************************************************************/
/* 
 * File name: SICKrangerC.cpp
 *
 * Synopsis: Example demonstrating how to use a SICK ranger camera to feed mil3dmap
 *           module to generate depth maps.
 *            
 */

#include "cameraconfig.h"           /* User options - change them to suit your needs. */

#include <mil.h>
#include "mil3dcam_sickrangerc.h"   /* Prototypes for conversion routines.            */
#include "sickcamera_rangerc.h"     /* Prototypes to interact with ranger camera.     */

/*****************************************************************************
 Example description.
*****************************************************************************/
void PrintHeader()
   {
   MosPrintf(MIL_TEXT("[EXAMPLE NAME]\n")
             MIL_TEXT("SICKrangerC\n\n")

             MIL_TEXT("[SYNOPSIS]\n")
             MIL_TEXT("This programs calibrates a 3d reconstruction setup with a SICK\n")
             MIL_TEXT("rangerC camera and scans an object to generate its depth map.\n\n")

             MIL_TEXT("[MODULES USED]\n")
             MIL_TEXT("Modules used: application, system, display, digitizer, buffer, graphic,\n")
             MIL_TEXT("              image processing, calibration, 3d reconstruction.\n\n")

             MIL_TEXT("Press <Enter> to start.\n\n"));

   MosGetch();
   }

/*****************************************************************************
 Constants.
*****************************************************************************/

/* All the values below should be adjusted to suit your needs. */

#define MAX_DISPLAY_SIZE_X       1024

#define NB_DIG_PROCESS_BUFFERS   2

#define NB_GRABS                 2
#define NB_SCANS_PER_GRAB        512
#define NB_SCANS_IN_RESULT       (NB_SCANS_PER_GRAB*NB_GRABS)

/* Calibration values for M_DEPTH_CORRECTION context type. */
#define NUMBER_OF_CALIBRATION_DEPTHS  5
static const double CORRECTED_DEPTHS[NUMBER_OF_CALIBRATION_DEPTHS] =
   {0.0, 12.0, 24.0, 36.0, 48.0};   /* calibration heights in mm. */

#define SCALE_FACTOR   1000.0 /* (depth in world units) * SCALE_FACTOR gives gray levels */

/* Calibration grid parameters. */
#define GRID_NB_ROWS             12
#define GRID_NB_COLS             13
#define GRID_ROW_SPACING         10.0     /* in mm                */
#define GRID_COL_SPACING         10.0     /* in mm                */

/* Depth map generation parameters. */
#define DEPTH_MAP_SIZE_X         512      /* in pixels            */
#define DEPTH_MAP_SIZE_Y         320      /* in pixels            */
#define GAP_DEPTH                1.5      /* in mm                */

#define SCALE_X                  0.3      /* in mm/pixel          */
#define SCALE_Y                  0.3      /* in mm/pixel          */
#define SCALE_Z                  -0.001   /* in mm/gray level     */
#define WORLD_OFFSET_X           -15.0    /* in mm */
#define WORLD_OFFSET_Y           (-DEPTH_MAP_SIZE_Y*SCALE_Y) /* in mm */
#define WORLD_OFFSET_Z           0.0      /* in mm                */

#define CONVEYOR_SPEED           20000.0  /* microns per second, 20cm/s. */

/* Filenames. */
#if !USE_REAL_CAMERA

#define EXAMPLE_IMAGE_PATH  M_IMAGE_PATH MIL_TEXT("SICKrangerC/")

/* Images used for calibration in M_DEPTH_CORRECTION mode. */
static const MIL_TEXT_CHAR* RefPlaneFilenames[NUMBER_OF_CALIBRATION_DEPTHS] =
   {
   EXAMPLE_IMAGE_PATH MIL_TEXT("calibplane0.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("calibplane1.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("calibplane2.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("calibplane3.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("calibplane4.mim")
   };

/* Images used for calibration in M_CALIBRATED_CAMERA_LINEAR_MOTION mode. */
static const MIL_TEXT_CHAR* CalibGridFilename = EXAMPLE_IMAGE_PATH MIL_TEXT("CalibGrid.mim");
static const MIL_TEXT_CHAR* CalibLineFilename = EXAMPLE_IMAGE_PATH MIL_TEXT("CalibLine.mim");

/* Images used for runtime scan of an object (in both modes). */
static const MIL_TEXT_CHAR* ScanFilenames[NB_GRABS] =
   {
   EXAMPLE_IMAGE_PATH MIL_TEXT("scan0.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("scan1.mim")
   };

#endif

/*****************************************************************************
 Functions declarations.
*****************************************************************************/

/* Example function declaration. */
void RangerCExample(MIL_ID MilSystem, MIL_ID MilDisplay, MIL_INT ContextType);

/* Helper function prototypes. */
void FatalError(const MIL_TEXT_CHAR* pMsg);

void SwithToMeasure(MIL_ID  MilSystem,
                    icon::RangerC* pCam,
                    MIL_INT SizeX,
                    MIL_INT SizeY,
                    MIL_ID* MilDigitizerPtr);

void SwithToImage(MIL_ID MilSystem, icon::RangerC* pCam, MIL_ID* MilDigitizerPtr);

void DisplayLaserLine(MIL_ID  MilDisplay,
                      MIL_ID  MilPositionLine,
                      MIL_ID  MilIntensityLine,
                      MIL_ID  MilMeasImage,
                      MIL_INT FixedPoint);

void DisplayCalibrationPoints(MIL_ID MilDisplay,
                              MIL_ID MilCalGridImage,
                              MIL_ID MilCalibration);

/* User's processing function prototype. */
MIL_INT MFTYPE AddScanProcFct(MIL_INT HookType, MIL_ID MilEvent, void* UserDataPtr);

/* User's processing function hook data structure. */
struct AddScanProcFctDataStruct
   {
   MIL_ID        MilLaser;
   MIL_ID        MilScan;
   MIL_ID        MilPositionImage;
   MIL_ID        MilIntensityImage;
   RangerParams* ConvParamsPtr;
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

   /* Run the depth correction example. */
   RangerCExample(MilSystem, MilDisplay, M_DEPTH_CORRECTION);

   /* Run the calibrated camera example. */
   RangerCExample(MilSystem, MilDisplay, M_CALIBRATED_CAMERA_LINEAR_MOTION);

   /* Free MIL objects. */
   MdispFree(MilDisplay);
   MsysFree(MilSystem);
   MappFree(MilApplication);

   return 0;
   }

/*****************************************************************************
 Function that calibrates a 3d reconstruction setup with a SICK rangerC camera
 and scans an object to generate its depth map.
*****************************************************************************/
void RangerCExample(MIL_ID MilSystem, MIL_ID MilDisplay, MIL_INT ContextType)
   {
   MIL_ID      MilDigitizer,      /* Digitizer for SICK camera.                        */
               MilImage,          /* Image buffer identifier (for continuous grab).    */
               MilMeasImage,      /* Image buffer to hold image of meas. ROI (MimDraw).*/
               MilCalGridImage,   /* Image buffer to hold calibration grid image.      */
               MilTwoLinesImage,  /* Image buffer to hold two laser lines.             */
               MilOneLineImage,   /* Image buffer to hold one laser line.              */
               MilManyLinesImage[NB_DIG_PROCESS_BUFFERS], /* Image buffers to hold lines.*/
               MilPositionImage,  /* Image buffer to hold converted position results.  */
               MilIntensityImage, /* Image buffer to hold converted intensity results. */
               MilPositionLine,   /* Image buffer to hold one line of converted pos.   */
               MilIntensityLine,  /* Image buffer to hold one line of converted int.   */
               MilCalibration,    /* Calibration context identifier.                   */
               MilDepthMap,       /* Image buffer identifier (for depth map result)    */
               MilIntensityMap,   /* Image buffer identifier (for intensity map result)*/
               MilLaser,          /* 3dmap laser profiling context identifier.         */
               MilScan;           /* 3dmap result buffer identifier.                   */
   MIL_INT     n,                 /* Counter.                                          */
               Ret;               /* Return value (error code) of subfunctions.        */

   MosPrintf(MIL_TEXT("\n3D RECONSTRUCTION USING SICK RANGERC:\n"));
   MosPrintf(MIL_TEXT("------------------------------------\n\n"));
   MosPrintf(MIL_TEXT("This programs calibrates a 3d reconstruction setup with a SICK\n"));
   MosPrintf(MIL_TEXT("rangerC camera and scans an object to generate its depth map.\n\n"));

   if (ContextType == M_DEPTH_CORRECTION)
      MosPrintf(MIL_TEXT("3dmap M_DEPTH_CORRECTION mode is used.\n\n"));
   else
      MosPrintf(MIL_TEXT("3dmap M_CALIBRATED_CAMERA_LINEAR_MOTION mode is used.\n\n"));

   /* CONVEYOR_CODE: Initialize conveyor. */

   /* Create a camera object and initialize it. */
   icon::RangerC* pCam;
   Ret = CreateCamera(&pCam);
   if (Ret != SICK_CAMERA_OK)
      FatalError(MIL_TEXT("Unable to create camera object.\n"));

   Ret = InitCamera(pCam);
   if (Ret != SICK_CAMERA_OK)
      FatalError(MIL_TEXT("Unable to initialize camera.\n"));

   /* Inquire camera parameters and fill in conversion structure. */
   RangerParams ConvParams;
   Ret = FillParams(pCam, &ConvParams, RANGER_C55_PRM, MEAS_COMP_NAME);
   if (Ret != SICK_CAMERA_OK)
      FatalError(MIL_TEXT("Unable to inquire camera parameters.\n"));

#if USE_REAL_CAMERA
   MIL_INT GrabAttr = M_GRAB;
#else
   MIL_INT GrabAttr = 0;
#endif

   /* Allocate buffers to hold images returned by SICK camera in measurement mode. */
   for (n = 0; n < NB_DIG_PROCESS_BUFFERS; ++n)
      {
      MbufAlloc2d(MilSystem, ConvParams.ProfileDataSizeByte, NB_SCANS_PER_GRAB,
                  8+M_UNSIGNED, M_IMAGE+M_PROC+GrabAttr, &MilManyLinesImage[n]);
      }

   /* Allocate buffers to hold images returned by SICK camera in image mode. */
   MbufAlloc2d(MilSystem, ConvParams.ImageROISizeX, ConvParams.ImageROISizeY,
               8+M_UNSIGNED, M_IMAGE+M_PROC+GrabAttr+M_DISP, &MilImage);
   MbufAlloc2d(MilSystem, ConvParams.ImageROISizeX, ConvParams.ImageROISizeY,
               8+M_UNSIGNED, M_IMAGE+M_PROC+GrabAttr+M_DISP, &MilCalGridImage);

   /* Allocate buffers to hold images returned by MimDraw(), same size as meas. ROI. */
   MIL_INT MimDrawSizeBit = (ConvParams.IntensitySizeBit == NO_INTENSITY_INFORMATION ?
                             8 : ConvParams.IntensitySizeBit);
   MbufAlloc2d(MilSystem, ConvParams.MeasROISizeX, ConvParams.MeasROISizeY,
               MimDrawSizeBit+M_UNSIGNED, M_IMAGE+M_PROC+M_DISP,
               &MilMeasImage);

   /* Allocate position image returned by conversion routine and fed to 3dmap. */
   MbufAlloc2d(M_DEFAULT_HOST, ConvParams.MeasROISizeX, NB_SCANS_PER_GRAB,
               ConvParams.RangeSizeBit+M_UNSIGNED, M_IMAGE+M_PROC, &MilPositionImage);

   /*
    * When calibrating, we won't need the full images since we want to grab only one
    * laser line. So, we create 1-line child buffers for SICK measurement output buffer and
    * for conversion routine buffers. However, because of a hardware limitation of the
    * Solios framegrabber, it is not possible to grab less than 2 lines, so we also create
    * a 2-lines child buffer for grabbing purposes.
    */
   MbufChild2d(MilManyLinesImage[0], 0, 0, ConvParams.ProfileDataSizeByte, 2, &MilTwoLinesImage);
   MbufChild2d(MilManyLinesImage[0], 0, 0, ConvParams.ProfileDataSizeByte, 1, &MilOneLineImage );
   MbufChild2d(MilPositionImage    , 0, 0, ConvParams.MeasROISizeX       , 1, &MilPositionLine );

   /* If camera mode returns intensity information, we also allocate the related buffers. */
   if (ConvParams.IntensitySizeBit != NO_INTENSITY_INFORMATION)
      {
      MbufAlloc2d(M_DEFAULT_HOST, ConvParams.MeasROISizeX, NB_SCANS_PER_GRAB,
                  ConvParams.IntensitySizeBit+M_UNSIGNED, M_IMAGE+M_PROC, &MilIntensityImage);
      MbufChild2d(MilIntensityImage, 0, 0, ConvParams.MeasROISizeX, 1, &MilIntensityLine);
      }
   else
      {
      MilIntensityImage = M_NULL;
      MilIntensityLine  = M_NULL;
      }

   /* Allocate 3dmap context and result, set some parameters. */
   M3dmapAlloc(MilSystem, M_LASER, ContextType, &MilLaser);
   M3dmapControl(MilLaser, M_DEFAULT, M_EXTRACTION_FIXED_POINT, ConvParams.FixedPoint);

   if (ContextType == M_CALIBRATED_CAMERA_LINEAR_MOTION)
      {
      /*
       * Compute conveyor speed in mm/frame.
       * CONVEYOR_SPEED is in um/s, ConvParams.CycleTimeMicroseconds is in us/frame.
       * ConvParams.CycleTimeMicroseconds * CONVEYOR_SPEED is in 10^-12 m/frame
       */
      double ConveyorSpeed = 1.0e-9 * ConvParams.CycleTimeMicroseconds * CONVEYOR_SPEED;

      M3dmapControl(MilLaser, M_DEFAULT, M_EXTRACTION_CHILD_OFFSET_X, ConvParams.ROIOffsetX);
      M3dmapControl(MilLaser, M_DEFAULT, M_EXTRACTION_CHILD_OFFSET_Y, ConvParams.ROIOffsetY);
      M3dmapControl(MilLaser, M_DEFAULT, M_SCAN_SPEED               , ConveyorSpeed        );
      }

   M3dmapAllocResult(MilSystem, M_LASER_DATA, M_DEFAULT, &MilScan);
   M3dmapControl(MilScan, M_DEFAULT, M_MAX_FRAMES, NB_SCANS_IN_RESULT);

   /* Set SICK camera in image mode. */
   Ret = SwitchCameraMode(pCam, IMAGE_MODE_NAME);
   if (Ret != SICK_CAMERA_OK)
      FatalError(MIL_TEXT("Unable to switch camera to image mode.\n"));

#if USE_REAL_CAMERA
   /* Allocate digitizer in area scan mode, for continuous grab. */
   MdigAlloc(MilSystem, M_DEFAULT, IMAGE_DCF, M_DEFAULT, &MilDigitizer);
   /* SICK camera grabs flipped images. */
   MdigControl(MilDigitizer, M_GRAB_DIRECTION_X, M_REVERSE);
   MosPrintf(MIL_TEXT("Adjust camera and laser position for calibration.\n\n"));
#else
   MilDigitizer = M_NULL;
#endif

   /* Display grabbed image. */
   MbufClear(MilImage, 0.0);
   if (ConvParams.ImageROISizeX > MAX_DISPLAY_SIZE_X)
      MdispZoom(MilDisplay, 0.5, 0.5);
   MdispSelect(MilDisplay, MilImage);

   /* Calibration steps are different, depending on the 3dmap context type. */
   if (ContextType == M_CALIBRATED_CAMERA_LINEAR_MOTION)
      {
#if USE_REAL_CAMERA
      /* Grab a sequence of image until enter is pressed. */
      MdigGrabContinuous(MilDigitizer, MilImage);

      MosPrintf(MIL_TEXT("Place calibration grid, switch laser OFF.\n"));
      MosPrintf(MIL_TEXT("Press <Enter> to grab.\n\n"));
      MosGetch();

      /* Grab calibration grid image. */
      MdigHalt(MilDigitizer);
      MbufCopy(MilImage, MilCalGridImage);

      /* Grab a sequence of image until enter is pressed. */
      MdigGrabContinuous(MilDigitizer, MilImage);

      MosPrintf(MIL_TEXT("Switch laser ON.\n"));
      MosPrintf(MIL_TEXT("Press <Enter> to grab.\n\n"));
      MosGetch();

      /* Switch digitizer to line-scan and camera to measurement mode. */
      SwithToMeasure(MilSystem,
                     pCam,
                     ConvParams.ProfileDataSizeByte,
                     2,
                     &MilDigitizer);

      /* Grab one laser line for 3d setup calibration. */
      MdigGrab(MilDigitizer, MilTwoLinesImage);
      
      /* Switch digitizer back to area-scan and camera to image mode. */
      SwithToImage(MilSystem, pCam, &MilDigitizer);
#else
      /* Simply reload the images that would have been grabbed above. */
      MbufLoad(CalibGridFilename, MilCalGridImage);
      MbufLoad(CalibLineFilename, MilOneLineImage);
#endif

      /* Allocate calibration context in 3D mode. */
      McalAlloc(MilSystem, M_TSAI_BASED, M_DEFAULT, &MilCalibration);

      /* Calibrate the camera. */
      McalGrid(MilCalibration, MilCalGridImage, 0.0, 0.0, 0.0, GRID_NB_ROWS, GRID_NB_COLS,
               GRID_ROW_SPACING, GRID_COL_SPACING, M_DEFAULT, M_DEFAULT);

      if (McalInquire(MilCalibration, M_CALIBRATION_STATUS, M_NULL) != M_CALIBRATED)
         FatalError(MIL_TEXT("Camera calibration failed.\n"));

      /* Display McalGrid() results. */
      DisplayCalibrationPoints(MilDisplay, MilCalGridImage, MilCalibration);

      /* Convert SICK format to MIL format. */
      M3dcamRangerToMIL(MilOneLineImage, MilPositionLine, MilIntensityLine, M_NULL, &ConvParams);

      /* Display extracted laser line. */
      DisplayLaserLine(MilDisplay, MilPositionLine, MilIntensityLine,
                       MilMeasImage, ConvParams.FixedPoint);

      /* Add laser line informations. */
      M3dmapAddScan(MilLaser, MilScan, MilPositionLine, MilIntensityLine,
                    M_NULL, M_DEFAULT, M_LINE_ALREADY_EXTRACTED);
      }
   else
      {
      /* ContextType == M_DEPTH_CORRECTION */
      MilCalibration = M_NULL;

      for (n = 0; n < NUMBER_OF_CALIBRATION_DEPTHS; n++)
         {
         MosPrintf(MIL_TEXT("Current plane: %d (%4.2f mm).\n"), n, CORRECTED_DEPTHS[n]);

#if USE_REAL_CAMERA
         /* Grab a sequence of image until enter is pressed. */
         MdigGrabContinuous(MilDigitizer, MilImage);

         MosPrintf(MIL_TEXT("Press <Enter> to grab.\n\n"));
         MosGetch();

         /* Switch digitizer to line-scan and camera to measurement mode. */
         SwithToMeasure(MilSystem,
                        pCam,
                        ConvParams.ProfileDataSizeByte,
                        2,
                        &MilDigitizer);

         /* Grab one laser line on the reference plane. */
         MdigGrab(MilDigitizer, MilTwoLinesImage);

         /* Switch digitizer back to area-scan and camera to image mode. */
         SwithToImage(MilSystem, pCam, &MilDigitizer);
#else
         /* Simply reload the images that would have been grabbed above. */
         MbufLoad(RefPlaneFilenames[n], MilOneLineImage);
#endif

         /* Set desired corrected depth of next reference plane. */
         M3dmapControl(MilLaser, M_DEFAULT, M_CORRECTED_DEPTH,
                       CORRECTED_DEPTHS[n]*SCALE_FACTOR);

         /* Convert SICK format to MIL format. */
         M3dcamRangerToMIL(MilOneLineImage, MilPositionLine, MilIntensityLine, M_NULL, &ConvParams);

         /* Display extracted laser line. */
         DisplayLaserLine(MilDisplay, MilPositionLine, MilIntensityLine,
                          MilMeasImage, ConvParams.FixedPoint);

         /* Add laser line informations. */
         M3dmapAddScan(MilLaser, MilScan, MilPositionLine, MilIntensityLine,
                       M_NULL, M_DEFAULT, M_LINE_ALREADY_EXTRACTED);
         }
      }

   /* Calibrate the 3d reconstruction context. */
   M3dmapCalibrate(MilLaser, MilScan, MilCalibration, M_DEFAULT);

   if (M3dmapInquire(MilLaser, M_DEFAULT, M_CALIBRATION_STATUS, M_NULL) != M_CALIBRATED)
      FatalError(MIL_TEXT("Laser calibration failed.\n"));

   MosPrintf(MIL_TEXT("The 3d reconstruction setup has been calibrated.\n\n"));

   /* Empty all result buffer contents.
      It will now be reused for extracting corrected depths. */
   M3dmapAddScan(M_NULL, MilScan, M_NULL, M_NULL, M_NULL, M_DEFAULT, M_RESET);

#if USE_REAL_CAMERA

   /* Grab a sequence of image until ENTER is pressed. */
   MdigGrabContinuous(MilDigitizer, MilImage); 

   MosPrintf(MIL_TEXT("Adjust object position for scanning.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to grab.\n\n"));
   MosGetch();

   MosPrintf(MIL_TEXT("The object is being scanned.\n\n"));

   /* Switch digitizer to line-scan and camera to measurement mode. */
   SwithToMeasure(MilSystem,
                  pCam,
                  ConvParams.ProfileDataSizeByte,
                  NB_SCANS_PER_GRAB,
                  &MilDigitizer);

   /* CONVEYOR_CODE: Start conveyor and wait for its speed to become constant. */

   /* Fill MdigProcess hook function structure. */
   AddScanProcFctDataStruct AddScanProcFctData;
   AddScanProcFctData.MilLaser          = MilLaser;
   AddScanProcFctData.MilScan           = MilScan;
   AddScanProcFctData.MilPositionImage  = MilPositionImage;
   AddScanProcFctData.MilIntensityImage = MilIntensityImage;
   AddScanProcFctData.ConvParamsPtr     = &ConvParams;

   /* Grab laser line data with buffering. */
   MdigProcess(MilDigitizer,
               MilManyLinesImage,
               NB_DIG_PROCESS_BUFFERS,
               M_SEQUENCE+M_COUNT(NB_GRABS),
               M_SYNCHRONOUS,
               &AddScanProcFct,
               &AddScanProcFctData);

   /* CONVEYOR_CODE: Stop conveyor. */

#else

   for (n = 0; n < NB_GRABS; ++n)
      {
      /* Reload profile data generated by SICK camera. */
      MbufLoad(ScanFilenames[n], MilManyLinesImage[0]);

      /* Convert SICK format to MIL format. */
      M3dcamRangerToMIL(MilManyLinesImage[0],
                        MilPositionImage,
                        MilIntensityImage,
                        M_NULL,
                        &ConvParams);

      /* Analyze the image to extract laser line and correct its depth. */
      M3dmapAddScan(MilLaser,
                    MilScan,
                    MilPositionImage,
                    MilIntensityImage,
                    M_NULL,
                    M_DEFAULT,
                    M_LINE_ALREADY_EXTRACTED);
      }

#endif

   /* Set depth map generation parameters according to 3dmap context type. */
   MIL_INT DepthMapSizeX, DepthMapSizeY, DepthMapType, IntensityMapType;

   M3dmapControl(MilScan, M_DEFAULT, M_FILL_MODE                 , M_X_THEN_Y);
   M3dmapControl(MilScan, M_DEFAULT, M_FILL_SHARP_ELEVATION      , M_MIN     );
   M3dmapControl(MilScan, M_DEFAULT, M_FILL_SHARP_ELEVATION_DEPTH, GAP_DEPTH );

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

   if (ConvParams.IntensitySizeBit != NO_INTENSITY_INFORMATION)
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

   /* Display corrected depth map. */
   if (DepthMapType != 8+M_UNSIGNED)
      MdispControl(MilDisplay, M_VIEW_MODE, M_AUTO_SCALE);
   if (DepthMapSizeX > MAX_DISPLAY_SIZE_X)
      MdispZoom(MilDisplay, 0.5, 0.5);
   else
      MdispZoom(MilDisplay, 1.0, 1.0);
   MdispSelect(MilDisplay, MilDepthMap);

   MosPrintf(MIL_TEXT("The corrected depth map of the object is being displayed.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   /* Reset display. */
   MdispSelect(MilDisplay, M_NULL);
   MdispControl(MilDisplay, M_VIEW_MODE, M_DEFAULT);
   MdispZoom(MilDisplay, 1.0, 1.0);

   /* Free all allocations. */
   if (MilIntensityLine != M_NULL)
      {
      MbufFree(MilIntensityLine);
      MbufFree(MilIntensityImage);
      }

   CloseDown(pCam);
   if (MilIntensityMap != M_NULL)
      MbufFree(MilIntensityMap);
   MbufFree(MilDepthMap);
   if (MilCalibration != M_NULL)
      McalFree(MilCalibration);
   M3dmapFree(MilScan);
   M3dmapFree(MilLaser);
   MbufFree(MilPositionLine);
   MbufFree(MilPositionImage);
   MbufFree(MilMeasImage);
   MbufFree(MilCalGridImage);
   MbufFree(MilImage);
   MbufFree(MilOneLineImage);
   MbufFree(MilTwoLinesImage);
   for (n = 0; n < NB_DIG_PROCESS_BUFFERS; ++n)
      MbufFree(MilManyLinesImage[n]);
   if (MilDigitizer != M_NULL)
      MdigFree(MilDigitizer);
   }

/*****************************************************************************
 Hook function for MdigProcess(). Converts the profile data returned by the ranger camera
 and feed it to 3dmap.
*****************************************************************************/
MIL_INT MFTYPE AddScanProcFct(MIL_INT HookType, MIL_ID MilEvent, void* UserDataPtr)
   {
   MIL_ID MilModifiedBuffer;
   AddScanProcFctDataStruct* pAddScanProcFctData =
      static_cast<AddScanProcFctDataStruct*>(UserDataPtr);
   MdigGetHookInfo(MilEvent, M_MODIFIED_BUFFER+M_BUFFER_ID, &MilModifiedBuffer);

   /* Convert SICK format to MIL format. */
   M3dcamRangerToMIL(MilModifiedBuffer,
                     pAddScanProcFctData->MilPositionImage,
                     pAddScanProcFctData->MilIntensityImage,
                     M_NULL,
                     pAddScanProcFctData->ConvParamsPtr);

   /* Analyze the image to extract laser line and correct its depth. */
   M3dmapAddScan(pAddScanProcFctData->MilLaser,
                 pAddScanProcFctData->MilScan,
                 pAddScanProcFctData->MilPositionImage,
                 pAddScanProcFctData->MilIntensityImage,
                 M_NULL,
                 M_DEFAULT,
                 M_LINE_ALREADY_EXTRACTED);

   return M_NULL;
   }

/*****************************************************************************
 Halts digitizer from continuous grab, reallocates it with line-scan DCF and
 switch SICK rangerC camera to measurement mode.

   MilSystem       (in)      ID of the system on which the digitizer is allocated.
   pCam            (in/out)  Pointer to camera object.
   SizeX           (in)      X size of the image to grab.
   SizeY           (in)      Y size of the image to grab.
   MilDigitizerPtr (in/out)  Pointer to allocated digitizer, to reallocate.
*****************************************************************************/
void SwithToMeasure(MIL_ID  MilSystem,
                    icon::RangerC* pCam,
                    MIL_INT SizeX,
                    MIL_INT SizeY,
                    MIL_ID* MilDigitizerPtr)
   {
   MIL_INT Ret;

   /* 
    * Stop grabbing data, MdigHalt() function will wait for the end of the current
    * frame before returning. If Camera is stopped when executing this command the
    * program will wait forever... so we add a sleep here.
    */
   MdigHalt(*MilDigitizerPtr);
   MosSleep(ONE_FRAME_DELAY_MS);
   MdigFree(*MilDigitizerPtr);

   /* Switch camera to measurement mode. */
   Ret = SwitchCameraMode(pCam, MEASUREMENT_MODE_NAME);
   if (Ret != SICK_CAMERA_OK)
      FatalError(MIL_TEXT("Unable to switch camera to measurement mode.\n"));

   /* Switch digitizer DCF to line-scan mode. */
   MdigAlloc(MilSystem, M_DEFAULT, MEAS_DCF, M_DEFAULT, MilDigitizerPtr);
   MdigControl(*MilDigitizerPtr, M_SOURCE_SIZE_X, SizeX);
   MdigControl(*MilDigitizerPtr, M_SOURCE_SIZE_Y, SizeY);
   MdigControl(*MilDigitizerPtr, M_GRAB_TIMEOUT, GRAB_TIMEOUT_MS);
   }

/*****************************************************************************
 Reallocates digitizre with area-scan DCF and switch SICK rangerC camera to
 measurement mode.

   MilSystem       (in)      ID of the system on which the digitizer is allocated.
   pCam            (in/out)  Pointer to camera object.
   MilDigitizerPtr (in/out)  Pointer to allocated digitizer, to reallocate.
*****************************************************************************/
void SwithToImage(MIL_ID  MilSystem, icon::RangerC* pCam, MIL_ID* MilDigitizerPtr)
   {
   MIL_INT Ret;

   MdigFree(*MilDigitizerPtr);

   /* Switch camera back to image mode. */
   Ret = SwitchCameraMode(pCam, IMAGE_MODE_NAME);
   if (Ret != SICK_CAMERA_OK)
      FatalError(MIL_TEXT("Unable to switch camera to image mode.\n"));

   /* Switch digitizer DCF back to area-scan mode. */
   MdigAlloc(MilSystem, M_DEFAULT, IMAGE_DCF, M_DEFAULT, MilDigitizerPtr);

   /* SICK camera grabs flipped images. */
   MdigControl(*MilDigitizerPtr, M_GRAB_DIRECTION_X, M_REVERSE);
   }

/*****************************************************************************
 Uses MimDraw() to show the image of the laser line the SICK rangerC has extracted.

   MilDisplay       (in)  ID of the MIL display to use.
   MilPositionLine  (in)  Single-line range image (output of M3dcamRangerToMIL()).
   MilIntensityLine (in)  Single-line intensity image (output of M3dcamRangerToMIL()) or M_NULL
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
 Uses McalDraw() to show the camera calibration grid with crosses on the calibration
 points in the overlay.

   MilDisplay      (in) ID of the MIL display to use.
   MilCalGridImage (in) Image of the calibration grid.
   MilCalibration  (in) Calibration object.
*****************************************************************************/
void DisplayCalibrationPoints(MIL_ID MilDisplay,
                              MIL_ID MilCalGridImage,
                              MIL_ID MilCalibration)
   {
   /* Backup display settings. */
   MIL_ID MilPreviousImage;
   MIL_DOUBLE PreviousZoom, PreviousColor;
   MdispInquire(MilDisplay, M_SELECTED, &MilPreviousImage);
   MdispInquire(MilDisplay, M_ZOOM_FACTOR_X, &PreviousZoom);
   MgraInquire(M_DEFAULT, M_COLOR, &PreviousColor);

   /* Prepare overlay buffer to draw calibration points. */
   MIL_ID MilOverlay;
   MdispSelect(MilDisplay, M_NULL);
   MdispZoom(MilDisplay, 1.0, 1.0);
   MdispSelect(MilDisplay, MilCalGridImage);
   MdispControl(MilDisplay, M_OVERLAY, M_ENABLE);
   MdispInquire(MilDisplay, M_OVERLAY_ID, &MilOverlay);
   MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);

   /* Draw calibration points. */
   MgraColor(M_DEFAULT, M_COLOR_GREEN);
   McalDraw(M_DEFAULT, MilCalibration, MilOverlay, M_DRAW_WORLD_POINTS,
      M_DEFAULT, M_DEFAULT);

   MosPrintf(MIL_TEXT("Camera calibration points are being displayed.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   /* Restore previous settings. */
   MgraColor(M_DEFAULT, PreviousColor);
   MdispSelect(MilDisplay, M_NULL);
   MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);
   MdispControl(MilDisplay, M_OVERLAY, M_DISABLE);
   MdispZoom(MilDisplay, PreviousZoom, PreviousZoom);
   MdispSelect(MilDisplay, MilPreviousImage);
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