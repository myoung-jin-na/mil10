/*****************************************************************************/
/* 
 * File name: pfocus.cpp
 *
 * Synopsis: Example demonstrating how to use a PhotonFocus 3D camera to feed mil3dmap
 *           module to generate depth maps.
 *            
 */

#include "cameraconfig.h"     /* User options - change them to suit your needs. */

#include <mil.h>
#include <stdlib.h>           /* for exit().                                    */
#include "mil3dcam_pfocus.h"  /* Prototypes for conversion routines.            */
#include "pfocuscamera.h"     /* Prototypes to interact with PhotonFocus camera.*/

/*****************************************************************************
 Example description.
*****************************************************************************/
void PrintHeader()
   {
   MosPrintf(MIL_TEXT("[EXAMPLE NAME]\n")
             MIL_TEXT("PhotonFocus_MV_D1024E_3D01_160_CL\n\n")

             MIL_TEXT("[SYNOPSIS]\n")
             MIL_TEXT("This programs calibrates a 3d reconstruction setup with a PhotonFocus\n")
             MIL_TEXT("3d camera and scans an object to generate its depth map.\n\n")

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

#define NB_DIG_PROCESS_BUFFERS   2

#define NB_GRABS                 300

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
#define DEPTH_MAP_SIZE_Y         284      /* in pixels            */
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

#define EXAMPLE_IMAGE_PATH  M_IMAGE_PATH MIL_TEXT("PhotonFocus_MV_D1024E_3D01_160_CL/")

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

/* 
 * To avoid loading a video file to restore all the images grabbed by the camera during the
 * scanning phase, all the images have been preconverted by calling M3dcamPFocusToMIL and
 * the resulting lines have been saved as these two images (position and intensity).
 */
static const MIL_TEXT_CHAR* ScanPosFilename = EXAMPLE_IMAGE_PATH MIL_TEXT("ScanPosition.mim");
static const MIL_TEXT_CHAR* ScanIntFilename = EXAMPLE_IMAGE_PATH MIL_TEXT("ScanIntensity.mim");

#endif

/*****************************************************************************
 Functions declarations.
*****************************************************************************/

/* Example function declaration. */
void PFocusExample(MIL_ID MilSystem, MIL_ID MilDisplay, MIL_INT ContextType);

/* Helper function prototypes. */
void FatalError(const MIL_TEXT_CHAR* pMsg);

void SwitchToMeasure(int CameraPort, MIL_ID MilDigitizer);

void SwitchToImage(int CameraPort, MIL_ID MilDigitizer);

void DisplayLaserLine(MIL_ID  MilDisplay,
                      MIL_ID  MilPositionLine,
                      MIL_ID  MilIntensityLine,
                      MIL_ID  MilMeasImage);

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
   PFocusParams* ConvParamsPtr;
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
   PFocusExample(MilSystem, MilDisplay, M_DEPTH_CORRECTION);

   /* Run the calibrated camera example. */
   PFocusExample(MilSystem, MilDisplay, M_CALIBRATED_CAMERA_LINEAR_MOTION);

   /* Free MIL objects. */
   MdispFree(MilDisplay);
   MsysFree(MilSystem);
   MappFree(MilApplication);

   return 0;
   }

/*****************************************************************************
 Function that calibrates a 3d reconstruction setup with a PhotonFocus 3d camera
 and scans an object to generate its depth map.
*****************************************************************************/
void PFocusExample(MIL_ID MilSystem, MIL_ID MilDisplay, MIL_INT ContextType)
   {
   MIL_ID      MilDigitizer,      /* Digitizer for camera.                                */
               MilImage,          /* Image buffer identifier (for continuous grab).       */
               MilMeasImage,      /* Image buffer to hold image of meas. ROI (MimDraw).   */
               MilCalGridImage,   /* Image buffer to hold calibration grid image.         */
               MilPeakImage[NB_DIG_PROCESS_BUFFERS], /* Image buffers to hold 3d info.    */
               MilPositionLine,   /* Image buffer to hold one line of converted pos.      */
               MilIntensityLine,  /* Image buffer to hold one line of converted int.      */
               MilCalibration,    /* Calibration context identifier.                      */
               MilDepthMap,       /* Image buffer identifier (for depth map result)       */
               MilIntensityMap,   /* Image buffer identifier (for intensity map result)   */
               MilLaser,          /* 3dmap laser profiling context identifier.            */
               MilScan;           /* 3dmap result buffer identifier.                      */
   MIL_INT     n,                 /* Counter.                                             */
               Ret,               /* Return value (error code) of subfunctions.           */
               SizeBit;           /* 8 or 16 bits image, according to camera resolution.  */

   MosPrintf(MIL_TEXT("\n3D RECONSTRUCTION USING PHOTONFOCUS 3D CAMERA:\n"));
   MosPrintf(MIL_TEXT("----------------------------------------------\n\n"));
   MosPrintf(MIL_TEXT("This programs calibrates a 3d reconstruction setup with a\n"));
   MosPrintf(MIL_TEXT("PhotonFocus 3d camera and scans an object to generate its\n"));
   MosPrintf(MIL_TEXT("depth map.\n\n"));

   if (ContextType == M_DEPTH_CORRECTION)
      MosPrintf(MIL_TEXT("3dmap M_DEPTH_CORRECTION mode is used.\n\n"));
   else
      MosPrintf(MIL_TEXT("3dmap M_CALIBRATED_CAMERA_LINEAR_MOTION mode is used.\n\n"));

   /* CONVEYOR_CODE: Initialize conveyor. */

   /* Initialize the camera. */
   Ret = InitCamera(CAMERA_COM_PORT);
   if (Ret != PFOCUS_CAMERA_OK)
      FatalError(MIL_TEXT("Unable to initialize camera.\n"));

   /* Set camera in measurement mode to ensure correct inquired exposure time and ROI. */
   Ret = SwitchCameraToMeasure(CAMERA_COM_PORT,
                               PFOCUS_MEAS_OFFSET_X,
                               PFOCUS_MEAS_OFFSET_Y,
                               PFOCUS_MEAS_SIZE_X,
                               PFOCUS_MEAS_SIZE_Y);
   if (Ret != PFOCUS_CAMERA_OK)
      FatalError(MIL_TEXT("Unable to set camera parameters.\n"));

   /* Inquire camera parameters and fill in conversion structure. */
   PFocusParams ConvParams;
   Ret = FillParams(CAMERA_COM_PORT, &ConvParams);
   if (Ret != PFOCUS_CAMERA_OK)
      FatalError(MIL_TEXT("Unable to inquire camera parameters.\n"));

   SizeBit = (ConvParams.DataResolution == 8 ? 8 : 16);

#if USE_REAL_CAMERA
   MIL_INT GrabAttr = M_GRAB;
#else
   MIL_INT GrabAttr = 0;
#endif

   /* Allocate buffers to hold images returned by camera in measurement mode. */
   for (n = 0; n < NB_DIG_PROCESS_BUFFERS; ++n)
      {
      MbufAlloc2d(MilSystem, PFOCUS_MEAS_SIZE_X+PFOCUS_3D_DATA_WIDTH, PFOCUS_MEAS_SIZE_Y,
                  SizeBit+M_UNSIGNED, M_IMAGE+M_PROC+GrabAttr, &MilPeakImage[n]);
      }

   /* Allocate buffers to hold images returned by camera in image mode. */
   MbufAlloc2d(MilSystem, PFOCUS_IMAGE_SIZE_X, PFOCUS_IMAGE_SIZE_Y,
               SizeBit+M_UNSIGNED, M_IMAGE+M_PROC+GrabAttr+M_DISP, &MilImage);
   MbufAlloc2d(MilSystem, PFOCUS_IMAGE_SIZE_X, PFOCUS_IMAGE_SIZE_Y,
               SizeBit+M_UNSIGNED, M_IMAGE+M_PROC+GrabAttr+M_DISP, &MilCalGridImage);

   /* Allocate buffers to hold images returned by MimDraw(), same size as meas. ROI. */
   MIL_INT NbDetectedLaserPixels = PFOCUS_MEAS_SIZE_Y;
   if (ConvParams.StatusLine == STATUS_LINE_ENABLED)
      NbDetectedLaserPixels -= 1;

   MbufAlloc2d(MilSystem, PFOCUS_MEAS_SIZE_X, NbDetectedLaserPixels,
               SizeBit+M_UNSIGNED, M_IMAGE+M_PROC+GrabAttr+M_DISP,
               &MilMeasImage);

   /* Allocate position and intensity image returned by conversion routine. */
   MbufAlloc2d(M_DEFAULT_HOST, NbDetectedLaserPixels, 1,
               PFOCUS_DEPTH_SIZEBIT+M_UNSIGNED, M_IMAGE+M_PROC, &MilPositionLine);
   MbufAlloc2d(M_DEFAULT_HOST, NbDetectedLaserPixels, 1,
               SizeBit+M_UNSIGNED, M_IMAGE+M_PROC, &MilIntensityLine);

   /* Allocate 3dmap context and result, set some parameters. */
   M3dmapAlloc(MilSystem, M_LASER, ContextType, &MilLaser);
   M3dmapControl(MilLaser, M_DEFAULT, M_EXTRACTION_FIXED_POINT, PFOCUS_FIXED_POINT);
   M3dmapControl(MilLaser, M_DEFAULT, M_ORIENTATION           , M_1D_ROWS         );

   if (ContextType == M_CALIBRATED_CAMERA_LINEAR_MOTION)
      {
      /*
       * Compute conveyor speed in mm/frame.
       * CONVEYOR_SPEED is in um/s, ConvParams.CycleTimeMicroseconds is in us/frame.
       * ConvParams.CycleTimeMicroseconds * CONVEYOR_SPEED is in 10^-12 m/frame
       */
      double ConveyorSpeed = 1.0e-9 * ConvParams.CycleTimeMicroseconds * CONVEYOR_SPEED;

      M3dmapControl(MilLaser, M_DEFAULT, M_EXTRACTION_CHILD_OFFSET_X,
                    PFOCUS_MEAS_OFFSET_X-PFOCUS_IMAGE_OFFSET_X);
      M3dmapControl(MilLaser, M_DEFAULT, M_EXTRACTION_CHILD_OFFSET_Y,
                    PFOCUS_MEAS_OFFSET_Y-PFOCUS_IMAGE_OFFSET_Y);
      M3dmapControl(MilLaser, M_DEFAULT, M_SCAN_SPEED, ConveyorSpeed);
      }

   M3dmapAllocResult(MilSystem, M_LASER_DATA, M_DEFAULT, &MilScan);
   M3dmapControl(MilScan, M_DEFAULT, M_MAX_FRAMES, NB_GRABS);

#if USE_REAL_CAMERA
   /* Allocate digitizer according to camera data resolution. */
   switch(ConvParams.DataResolution)
      {
      case 8:
         MdigAlloc(MilSystem, M_DEFAULT, PFOCUS_8BITS_DCF , M_DEFAULT, &MilDigitizer);
         break;
   
      case 10:
         MdigAlloc(MilSystem, M_DEFAULT, PFOCUS_10BITS_DCF, M_DEFAULT, &MilDigitizer);
         break;
   
      case 12:
         MdigAlloc(MilSystem, M_DEFAULT, PFOCUS_12BITS_DCF, M_DEFAULT, &MilDigitizer);
         break;
   
      default:
         FatalError(MIL_TEXT("Unexpected camera resolution.\n"));
      }

   MosPrintf(MIL_TEXT("Adjust camera and laser position for calibration.\n\n"));

   /* Set camera and digitizer to image mode. */
   SwitchToImage(CAMERA_COM_PORT, MilDigitizer);
#else
   MilDigitizer = M_NULL;
#endif

   /* Display grabbed image. */
   MbufClear(MilImage, 0.0);
   if (SizeBit != 8)
      MdispControl(MilDisplay, M_VIEW_MODE, M_AUTO_SCALE);
   else
      MdispControl(MilDisplay, M_VIEW_MODE, M_DEFAULT);
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
      MdigHalt(MilDigitizer);

      /* Switch digitizer to measure size and camera to 3D mode only. */
      SwitchToMeasure(CAMERA_COM_PORT, MilDigitizer);

      /* Grab one laser line for 3d setup calibration. */
      MdigGrab(MilDigitizer, MilPeakImage[0]);
      
      /* Switch digitizer back to full frame and camera to 2D mode. */
      SwitchToImage(CAMERA_COM_PORT, MilDigitizer);
#else
      /* Simply reload the images that would have been grabbed above. */
      MbufLoad(CalibGridFilename, MilCalGridImage);
      MbufLoad(CalibLineFilename, MilPeakImage[0]);
#endif

      /* Allocate calibration context in 3D mode. */
      McalAlloc(MilSystem, M_TSAI_BASED, M_DEFAULT, &MilCalibration);

      /* Ensure that the correct grid corner is chosen as grid origin
         (the one in the top-right of the image, if the camera is rotated on the left. */
      McalControl(MilCalibration, M_GRID_CORNER_HINT_X, PFOCUS_IMAGE_SIZE_X-1);
      McalControl(MilCalibration, M_GRID_CORNER_HINT_Y, 0);

      /* Calibrate the camera. */
      McalGrid(MilCalibration, MilCalGridImage, 0.0, 0.0, 0.0, GRID_NB_ROWS, GRID_NB_COLS,
               GRID_ROW_SPACING, GRID_COL_SPACING, M_DEFAULT, M_DEFAULT);

      if (McalInquire(MilCalibration, M_CALIBRATION_STATUS, M_NULL) != M_CALIBRATED)
         FatalError(MIL_TEXT("Camera calibration failed.\n"));

      /* Display McalGrid() results. */
      DisplayCalibrationPoints(MilDisplay, MilCalGridImage, MilCalibration);

      /* Convert PhotonFocus format to MIL format. */
      Ret = M3dcamPFocusToMIL(MilPeakImage[0], MilPositionLine, MilIntensityLine, M_NULL, &ConvParams);
      if (Ret != PFOCUS_CAMERA_OK)
         FatalError(MIL_TEXT("Unable to convert image to MIL format.\n"));

      /* Display extracted laser line. */
      DisplayLaserLine(MilDisplay, MilPositionLine, MilIntensityLine, MilMeasImage);

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
         MdigHalt(MilDigitizer);

         /* Switch digitizer to measure size and camera to 3D mode only. */
         SwitchToMeasure(CAMERA_COM_PORT, MilDigitizer);

         /* Grab one laser line on the reference plane. */
         MdigGrab(MilDigitizer, MilPeakImage[0]);

         /* Switch digitizer back to full frame and camera to 2D mode. */
         SwitchToImage(CAMERA_COM_PORT, MilDigitizer);
#else
         /* Simply reload the images that would have been grabbed above. */
         MbufLoad(RefPlaneFilenames[n], MilPeakImage[0]);
#endif

         /* Set desired corrected depth of next reference plane. */
         M3dmapControl(MilLaser, M_DEFAULT, M_CORRECTED_DEPTH,
                       CORRECTED_DEPTHS[n]*SCALE_FACTOR);

         /* Convert PhotonFocus format to MIL format. */
         Ret = M3dcamPFocusToMIL(MilPeakImage[0], MilPositionLine, MilIntensityLine, M_NULL, &ConvParams);
         if (Ret != PFOCUS_CAMERA_OK)
            FatalError(MIL_TEXT("Unable to convert image to MIL format.\n"));


         /* Display extracted laser line. */
         DisplayLaserLine(MilDisplay, MilPositionLine, MilIntensityLine, MilMeasImage);

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
   MdigHalt(MilDigitizer);

   MosPrintf(MIL_TEXT("The object is being scanned.\n\n"));

   /* Switch digitizer to measure size and camera to 3D mode only. */
   SwitchToMeasure(CAMERA_COM_PORT, MilDigitizer);

   /* CONVEYOR_CODE: Start conveyor and wait for its speed to become constant. */

   /* Fill MdigProcess hook function structure. */
   AddScanProcFctDataStruct AddScanProcFctData;
   AddScanProcFctData.MilLaser          = MilLaser;
   AddScanProcFctData.MilScan           = MilScan;
   AddScanProcFctData.MilPositionImage  = MilPositionLine;
   AddScanProcFctData.MilIntensityImage = MilIntensityLine;
   AddScanProcFctData.ConvParamsPtr     = &ConvParams;

   /* Grab laser line data with buffering. */
   MdigProcess(MilDigitizer,
               MilPeakImage,
               NB_DIG_PROCESS_BUFFERS,
               M_SEQUENCE+M_COUNT(NB_GRABS),
               M_SYNCHRONOUS,
               &AddScanProcFct,
               &AddScanProcFctData);

   /* CONVEYOR_CODE: Stop conveyor. */

#else

   /* 
    * Here, we would need to load a video file to restore all the images grabbed by the
    * camera during the scanning phase. To simplify things, all the images have been
    * preconverted by calling M3dcamPFocusToMIL and the resulting lines have been
    * saved as two images (position and intensity).
    */

   /* Reload the two images resulting from conversion. */
   MIL_ID MilScanPosition, MilScanIntensity;
   MbufRestore(ScanPosFilename, M_DEFAULT_HOST, &MilScanPosition );
   MbufRestore(ScanIntFilename, M_DEFAULT_HOST, &MilScanIntensity);

   /* The loop is for the example only, the whole images could have been analyzed in a
      single call to M3dmapAddScan(). */
   for (n = 0; n < NB_GRABS; ++n)
      {
      /* Simulate grabbing a full frame and converting it with M3dcamPFocusToMIL().
         Simply copy the corresponding line from the restored images.                */
      MbufCopyColor2d(MilScanPosition , MilPositionLine , 0, 0, n, 0, 0, 0,
                      NbDetectedLaserPixels, 1);
      MbufCopyColor2d(MilScanIntensity, MilIntensityLine, 0, 0, n, 0, 0, 0,
                      NbDetectedLaserPixels, 1);

      /* Analyze the image to extract laser line and correct its depth. */
      M3dmapAddScan(MilLaser,
                    MilScan,
                    MilPositionLine,
                    MilIntensityLine,
                    M_NULL,
                    M_DEFAULT,
                    M_LINE_ALREADY_EXTRACTED);
      }

   MbufFree(MilScanIntensity);
   MbufFree(MilScanPosition );

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
                                                   M_IMAGE+M_PROC+M_DISP, &MilDepthMap    );

   M3dmapGetResult(MilScan, M_DEFAULT, M_INTENSITY_MAP_BUFFER_TYPE      +M_TYPE_MIL_INT,
                                                                     &IntensityMapType);
   MbufAlloc2d(MilSystem, DepthMapSizeX, DepthMapSizeY, IntensityMapType,
                                                M_IMAGE+M_PROC+M_DISP, &MilIntensityMap);

   /* Get depth map from accumulated information in the result buffer. */
   M3dmapExtract(MilScan, MilDepthMap, MilIntensityMap, M_CORRECTED_DEPTH_MAP,
                 M_DEFAULT, M_DEFAULT);

   /* Display corrected depth map. */
   MdispControl(MilDisplay, M_VIEW_MODE, M_AUTO_SCALE);
   MdispSelect(MilDisplay, MilDepthMap);

   MosPrintf(MIL_TEXT("The corrected depth map of the object is being displayed.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   /* Reset display. */
   MdispSelect(MilDisplay, M_NULL);

   /* Free all allocations. */
   CloseDown(CAMERA_COM_PORT);
   MbufFree(MilIntensityMap);
   MbufFree(MilDepthMap);
   if (MilCalibration != M_NULL)
      McalFree(MilCalibration);
   M3dmapFree(MilScan);
   M3dmapFree(MilLaser);
   MbufFree(MilIntensityLine);
   MbufFree(MilPositionLine);
   MbufFree(MilMeasImage);
   MbufFree(MilCalGridImage);
   MbufFree(MilImage);
   for (n = 0; n < NB_DIG_PROCESS_BUFFERS; ++n)
      MbufFree(MilPeakImage[n]);
   if (MilDigitizer != M_NULL)
      MdigFree(MilDigitizer);
   }

/*****************************************************************************
 Hook function for MdigProcess(). Converts the profile data returned by the PhotonFocus
 camera and feed it to 3dmap.
*****************************************************************************/
MIL_INT MFTYPE AddScanProcFct(MIL_INT HookType, MIL_ID MilEvent, void* UserDataPtr)
   {
   MIL_ID MilModifiedBuffer;
   AddScanProcFctDataStruct* pAddScanProcFctData =
      static_cast<AddScanProcFctDataStruct*>(UserDataPtr);
   MdigGetHookInfo(MilEvent, M_MODIFIED_BUFFER+M_BUFFER_ID, &MilModifiedBuffer);

   /* Convert PhotonFocus format to MIL format. */
   MIL_INT Ret = M3dcamPFocusToMIL(MilModifiedBuffer,
                                   pAddScanProcFctData->MilPositionImage,
                                   pAddScanProcFctData->MilIntensityImage,
                                   M_NULL,
                                   pAddScanProcFctData->ConvParamsPtr);
   if (Ret != PFOCUS_CAMERA_OK)
      FatalError(MIL_TEXT("Unable to convert image to MIL format.\n"));


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
 Enable PhotonFocus 3d mode and change ROI if needed, both for the camera and
 the digitizer.

   CameraPort (in)  Port number used to access PhotonFocus camera
   Digitizer  (in)  Digitizer Id.
*****************************************************************************/
void SwitchToMeasure(int CameraPort, MIL_ID MilDigitizer)
   {
   MIL_INT Ret;

   /* Switch camera to measurement mode. */
   Ret = SwitchCameraToMeasure(CameraPort,
                               PFOCUS_MEAS_OFFSET_X,
                               PFOCUS_MEAS_OFFSET_Y,
                               PFOCUS_MEAS_SIZE_X,
                               PFOCUS_MEAS_SIZE_Y);
   if (Ret != PFOCUS_CAMERA_OK)
      FatalError(MIL_TEXT("Unable to switch camera to measurement mode.\n"));

   /* Set digitizer settings. */
   MdigControl(MilDigitizer, M_SOURCE_SIZE_X, PFOCUS_MEAS_SIZE_X+PFOCUS_3D_DATA_WIDTH);
   MdigControl(MilDigitizer, M_SOURCE_SIZE_Y, PFOCUS_MEAS_SIZE_Y                     );
   }

/*****************************************************************************
 Disable PhotonFocus 3d mode and change ROI if needed, both for the camera and
 the digitizer.

   CameraPort (in)  Port number used to access PhotonFocus camera
   Digitizer (in)  Digitizer Id.
*****************************************************************************/
void SwitchToImage(int CameraPort, MIL_ID MilDigitizer)
   {
   MIL_INT Ret;

   /* Switch camera to image mode. */
   Ret = SwitchCameraToImage(CameraPort,
                             PFOCUS_IMAGE_OFFSET_X,
                             PFOCUS_IMAGE_OFFSET_Y,
                             PFOCUS_IMAGE_SIZE_X,
                             PFOCUS_IMAGE_SIZE_Y);
   if (Ret != PFOCUS_CAMERA_OK)
      FatalError(MIL_TEXT("Unable to switch camera to image mode.\n"));

   /* Set digitizer settings. */
   MdigControl(MilDigitizer, M_SOURCE_SIZE_X, PFOCUS_IMAGE_SIZE_X);
   MdigControl(MilDigitizer, M_SOURCE_SIZE_Y, PFOCUS_IMAGE_SIZE_Y);
   }

/*****************************************************************************
 Uses MimDraw() to show the image of the laser line the 3d camera has extracted.

   MilDisplay       (in)  ID of the MIL display to use.
   MilPositionLine  (in)  Single-line range image (output of M3dcamPFocusToMIL()).
   MilIntensityLine (in)  Single-line intensity image (output of M3dcamPFocusToMIL()) or M_NULL
   MilMeasImage     (out) Temporary image to use to display laser line.
*****************************************************************************/
void DisplayLaserLine(MIL_ID  MilDisplay,
                      MIL_ID  MilPositionLine,
                      MIL_ID  MilIntensityLine,
                      MIL_ID  MilMeasImage)
   {
   MIL_ID  MilPreviousImage;
   MIL_INT PreviousViewMode;
   MdispInquire(MilDisplay, M_SELECTED,  &MilPreviousImage);
   MdispInquire(MilDisplay, M_VIEW_MODE, &PreviousViewMode);

   MbufClear(MilMeasImage, 0.0);
   MimDraw(M_DEFAULT, MilPositionLine, MilIntensityLine, MilMeasImage,
           M_DRAW_PEAKS+M_1D_ROWS+M_LINES, 0, 2.0, M_FIXED_POINT+PFOCUS_FIXED_POINT);

   MdispControl(MilDisplay, M_VIEW_MODE, M_AUTO_SCALE);
   MdispSelect(MilDisplay, MilMeasImage);

   MosPrintf(MIL_TEXT("The extracted laser line is displayed.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   MdispControl(MilDisplay, M_VIEW_MODE, PreviousViewMode);
   MdispSelect(MilDisplay, MilPreviousImage);
   }

/*****************************************************************************
 Uses McalDraw() to show the camera calibration grid with crosses on the calibration
 points in the overlay.

   MilDisplay        (in) ID of the MIL display to use.
   MilCalGridImage   (in) Image of the calibration grid.
   MilCalibration    (in) Calibration object.
*****************************************************************************/
void DisplayCalibrationPoints(MIL_ID MilDisplay,
                              MIL_ID MilCalGridImage,
                              MIL_ID MilCalibration)
   {
   /* Backup display settings. */
   MIL_ID MilPreviousImage;
   MIL_DOUBLE PreviousColor;
   MdispInquire(MilDisplay, M_SELECTED, &MilPreviousImage);
   MgraInquire(M_DEFAULT, M_COLOR, &PreviousColor);

   /* Prepare overlay buffer to draw calibration points. */
   MIL_ID MilOverlay;
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
   MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);
   MdispControl(MilDisplay, M_OVERLAY, M_DISABLE);
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
