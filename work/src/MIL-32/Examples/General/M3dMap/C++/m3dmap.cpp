/*****************************************************************************/
/* 
 * File name: m3dmap.cpp
 *
 * Synopsis: This program inspects a wood surface using laser profiling to find
 *           any depth defects.
 *
 * Printable calibration grids in PDF format can be found in your
 * "Matrox Imaging/Images/" directory.
 *
 * When considering a laser-based 3D reconstruction system, the file "3D Setup Helper.xls"
 * can be used to accelerate prototyping by choosing an adequate hardware configuration
 * (angle, distance, lens, camera, ...). The file is located in your
 * "Matrox Imaging/Tools/" directory.
 *            
 */

#include <mil.h>
#include <malloc.h>
#include <math.h>

#if M_MIL_USE_WINDOWS && !M_MIL_USE_CE
   /* Include DirectX display only on Windows. */
   #include "MdispD3D.h"
#endif

/* Example functions declarations. */  
void DepthCorrectionExample(MIL_ID MilSystem, MIL_ID MilDisplay);
void CalibratedCameraExample(MIL_ID MilSystem, MIL_ID MilDisplay);

/* Utility functions declarations. */  
void PerformBlobAnalysis(MIL_ID MilSystem,
                         MIL_ID MilDisplay,
                         MIL_ID MilOverlayImage,
                         MIL_ID MilDepthMap);
void SetupColorDisplay(MIL_ID MilSystem, MIL_ID MilDisplay, MIL_INT SizeBit);
MIL_DOUBLE ComputeVolume(MIL_ID MilSystem, MIL_ID MilDepthMap);

/*****************************************************************************
 Main.
*****************************************************************************/
int MosMain(void)
   {
   MIL_ID MilApplication,     /* Application identifier. */
          MilSystem,          /* System identifier.      */
          MilDisplay;         /* Display identifier.     */

   /* Allocate defaults. */
   MappAllocDefault(M_DEFAULT, &MilApplication, &MilSystem, &MilDisplay, M_NULL, M_NULL);

   /* Run the depth correction example. */
   DepthCorrectionExample(MilSystem, MilDisplay);

   /* Run the calibrated camera example. */
   CalibratedCameraExample(MilSystem, MilDisplay);

   /* Free defaults. */    
   MappFreeDefault(MilApplication, MilSystem, MilDisplay, M_NULL, M_NULL);

   return 0;
   }

/****************************************************************************
 Depth correction example.
****************************************************************************/

/* Input sequence specifications. */
#define REFERENCE_PLANES_SEQUENCE_FILE  M_IMAGE_PATH MIL_TEXT("ReferencePlanes.avi")
#define OBJECT_SEQUENCE_FILE            M_IMAGE_PATH MIL_TEXT("ScannedObject.avi")

/* Peak detection parameters. */
#define MAX_LINE_WIDTH             8
#define MIN_INTENSITY            120

/* Calibration heights in mm. */
static const double CORRECTED_DEPTHS[] = {1.25, 2.50, 3.75, 5.00};

#define SCALE_FACTOR   10000.0 /* (depth in world units) * SCALE_FACTOR gives gray levels */

/* Annotation position. */
#define CALIB_TEXT_POS_X         400   
#define CALIB_TEXT_POS_Y          15

void DepthCorrectionExample(MIL_ID MilSystem, MIL_ID MilDisplay)
   {
   MIL_ID      MilOverlayImage,   /* Overlay image buffer identifier.              */
               MilImage,          /* Image buffer identifier (for processing).     */
               MilDepthMap,       /* Image buffer identifier (for results).        */
               MilLaser,          /* 3dmap laser profiling context identifier.     */
               MilScan;           /* 3dmap result buffer identifier.               */
   MIL_INT     SizeX,             /* Width of grabbed images.                      */
               SizeY,             /* Height of grabbed images.                     */
               NbReferencePlanes, /* Number of reference planes of known heights.  */
               NbObjectImages;    /* Number of frames for scanned objects.         */
   int         n;                 /* Counter.                                      */
   MIL_DOUBLE  FrameRate,         /* Number of grabbed frames per second (in AVI). */
               StartTime,         /* Time at the beginning of each iteration.      */
               EndTime,           /* Time after processing for each iteration.     */
               WaitTime;          /* Time to wait for next frame.                  */

   /* Inquire characteristics of the input sequences. */
   MbufDiskInquire(REFERENCE_PLANES_SEQUENCE_FILE, M_SIZE_X,           &SizeX);
   MbufDiskInquire(REFERENCE_PLANES_SEQUENCE_FILE, M_SIZE_Y,           &SizeY);
   MbufDiskInquire(REFERENCE_PLANES_SEQUENCE_FILE, M_NUMBER_OF_IMAGES, &NbReferencePlanes);
   MbufDiskInquire(REFERENCE_PLANES_SEQUENCE_FILE, M_FRAME_RATE,       &FrameRate);
   MbufDiskInquire(OBJECT_SEQUENCE_FILE,           M_NUMBER_OF_IMAGES, &NbObjectImages);

   /* Allocate buffer to hold images. */
   MbufAlloc2d(MilSystem, SizeX, SizeY,  8+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC, &MilImage);
   MbufClear(MilImage, 0.0);

   MosPrintf(MIL_TEXT("\nDEPTH ANALYSIS:\n"));
   MosPrintf(MIL_TEXT("---------------\n\n"));
   MosPrintf(MIL_TEXT("This program performs a surface inspection to detect depth\n"));
   MosPrintf(MIL_TEXT("defects on a wood surface using a laser profiling system.\n\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   /* Select display. */
   MdispSelect(MilDisplay, MilImage);

   /* Prepare for overlay annotations. */
   MdispControl(MilDisplay, M_OVERLAY, M_ENABLE);
   MdispInquire(MilDisplay, M_OVERLAY_ID, &MilOverlayImage);
   MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
   MgraColor(M_DEFAULT, M_COLOR_WHITE);

   /* Allocate 3dmap objects. */
   M3dmapAlloc(MilSystem, M_LASER, M_DEPTH_CORRECTION, &MilLaser);
   M3dmapAllocResult(MilSystem, M_LASER_DATA, M_DEFAULT, &MilScan);

   /* Set laser line extraction options. */
   M3dmapControl(MilLaser, M_DEFAULT, M_PEAK_WIDTH,    MAX_LINE_WIDTH);
   M3dmapControl(MilLaser, M_DEFAULT, M_MIN_INTENSITY, MIN_INTENSITY);

   /* Open the calibration sequence file for reading. */
   MbufImportSequence(REFERENCE_PLANES_SEQUENCE_FILE, M_DEFAULT, M_NULL, M_NULL, NULL,
                                                                   M_NULL, M_NULL, M_OPEN);

   /* Read and process all images in the input sequence. */
   MappTimer(M_DEFAULT, M_TIMER_READ+M_SYNCHRONOUS, &StartTime);

   for (n = 0; n < NbReferencePlanes; n++)
      {
      MIL_TEXT_CHAR CalibString[32];

      /* Read image from sequence. */
      MbufImportSequence(REFERENCE_PLANES_SEQUENCE_FILE, M_DEFAULT, M_LOAD, M_NULL,
                                                          &MilImage, M_DEFAULT, 1, M_READ);

      /* Annotate the image with the calibration height. */
      MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);
      MosSprintf(CalibString, 32, MIL_TEXT("Reference plane %d: %.2f mm"),
                     n+1, CORRECTED_DEPTHS[n]);
      MgraText(M_DEFAULT, MilOverlayImage, CALIB_TEXT_POS_X, CALIB_TEXT_POS_Y, CalibString);

      /* Set desired corrected depth of next reference plane. */
      M3dmapControl(MilLaser, M_DEFAULT, M_CORRECTED_DEPTH,
                                                         CORRECTED_DEPTHS[n]*SCALE_FACTOR);

      /* Analyze the image to extract laser line. */
      M3dmapAddScan(MilLaser, MilScan, MilImage, M_NULL, M_NULL, M_DEFAULT, M_DEFAULT);

      /* Wait to have a proper frame rate, if necessary. */
      MappTimer(M_DEFAULT, M_TIMER_READ+M_SYNCHRONOUS, &EndTime);
      WaitTime = (1.0/FrameRate) - (EndTime - StartTime);
      if (WaitTime > 0)
         MappTimer(M_DEFAULT, M_TIMER_WAIT, &WaitTime);
      MappTimer(M_DEFAULT, M_TIMER_READ+M_SYNCHRONOUS, &StartTime);
      }

   /* Close the calibration sequence file. */
   MbufImportSequence(REFERENCE_PLANES_SEQUENCE_FILE, M_DEFAULT, M_NULL, M_NULL, NULL,
                                                                  M_NULL, M_NULL, M_CLOSE);

   /* Calibrate the laser profiling context using reference planes of known heights. */
   M3dmapCalibrate(MilLaser, MilScan, M_NULL, M_DEFAULT);

   MosPrintf(MIL_TEXT("The laser profiling system has been calibrated using 4 reference\n"));
   MosPrintf(MIL_TEXT("planes of known heights.\n\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   MosPrintf(MIL_TEXT("The wood surface is being scanned.\n\n"));

   /* Empty all result buffer contents.
      It will now be reused for extracting corrected depths. */
   M3dmapAddScan(M_NULL, MilScan, M_NULL, M_NULL, M_NULL, M_DEFAULT, M_RESET);

   /* Open the object sequence file for reading. */
   MbufDiskInquire(OBJECT_SEQUENCE_FILE, M_FRAME_RATE, &FrameRate);
   MbufImportSequence(OBJECT_SEQUENCE_FILE, M_DEFAULT, M_NULL, M_NULL, NULL, M_NULL,
                                                                           M_NULL, M_OPEN);

   /* Read and process all images in the input sequence. */
   MappTimer(M_DEFAULT, M_TIMER_READ+M_SYNCHRONOUS, &StartTime);
   MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);

   for (n = 0; n < NbObjectImages; n++)
      {
      /* Read image from sequence. */
      MbufImportSequence(OBJECT_SEQUENCE_FILE, M_DEFAULT, M_LOAD, M_NULL, &MilImage,
                                                                     M_DEFAULT, 1, M_READ);

      /* Analyze the image to extract laser line and correct its depth. */
      M3dmapAddScan(MilLaser, MilScan, MilImage, M_NULL, M_NULL, M_DEFAULT, M_DEFAULT);

      /* Wait to have a proper frame rate, if necessary. */
      MappTimer(M_DEFAULT, M_TIMER_READ+M_SYNCHRONOUS, &EndTime);
      WaitTime = (1.0/FrameRate) - (EndTime - StartTime);
      if (WaitTime > 0)
         MappTimer(M_DEFAULT, M_TIMER_WAIT, &WaitTime);
      MappTimer(M_DEFAULT, M_TIMER_READ+M_SYNCHRONOUS, &StartTime);
      }

   /* Close the object sequence file. */
   MbufImportSequence(OBJECT_SEQUENCE_FILE, M_DEFAULT, M_NULL, M_NULL, NULL, M_NULL,
                                                                          M_NULL, M_CLOSE);

   /* Allocate the image for a partially corrected depth map. */
   MbufAlloc2d(MilSystem, SizeX, NbObjectImages, 16+M_UNSIGNED,
                                                       M_IMAGE+M_PROC+M_DISP, &MilDepthMap);

   /* Get partially corrected depth map from accumulated information in the result buffer. */
   M3dmapExtract(MilScan, MilDepthMap, M_NULL, M_CORRECTED_DEPTH_MAP, M_DEFAULT, M_DEFAULT);

   /* Show partially corrected depth map and find defects. */
   SetupColorDisplay(MilSystem, MilDisplay, MbufInquire(MilDepthMap, M_SIZE_BIT, M_NULL));
   
   /* Display partially corrected depth map. */
   MdispSelect(MilDisplay, MilDepthMap);
   MdispInquire(MilDisplay, M_OVERLAY_ID, &MilOverlayImage);

   MosPrintf(MIL_TEXT("The pseudo-color depth map of the surface is displayed.\n\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   PerformBlobAnalysis(MilSystem, MilDisplay, MilOverlayImage, MilDepthMap);

   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   /* Disassociates display LUT and clear overlay. */
   MdispLut(MilDisplay, M_DEFAULT);
   MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);

   /* Free all allocations. */
   M3dmapFree(MilScan);
   M3dmapFree(MilLaser);
   MbufFree(MilDepthMap);
   MbufFree(MilImage);
   }

/* Values used for binarization. */
#define EXPECTED_HEIGHT     3.4   /* Inspected surface should be at this height (in mm)   */
#define DEFECT_THRESHOLD    0.2   /* Max acceptable deviation from expected height (mm)   */
#define SATURATED_DEFECT    1.0   /* Deviation at which defect will appear red (in mm)    */

/* Radius of the smallest particles to keep. */
#define MIN_BLOB_RADIUS              3L

/* Pixel offset for drawing text. */
#define TEXT_H_OFFSET_1            -50
#define TEXT_V_OFFSET_1             -6
#define TEXT_H_OFFSET_2            -30
#define TEXT_V_OFFSET_2              6

/* Find defects in corrected depth map, compute max deviation and draw contours.  */
void PerformBlobAnalysis(MIL_ID MilSystem,
                         MIL_ID MilDisplay,
                         MIL_ID MilOverlayImage,
                         MIL_ID MilDepthMap)
   {
   MIL_ID      MilBinImage,         /* Binary image buffer identifier.           */
               MilBlobFeatureList,  /* Feature list identifier.                  */
               MilBlobResult;       /* Blob result buffer identifier.            */
   MIL_INT     SizeX,               /* Width of depth map.                       */
               SizeY,               /* Height of depth map.                      */
               TotalBlobs,          /* Total number of blobs.                    */
               n,                   /* Counter.                                  */
              *MinPixels;           /* Maximum height of defects.                */
   MIL_DOUBLE  DefectThreshold,     /* A gray level below it is a defect.        */
              *CogX,                /* X coordinate of center of gravity.        */
              *CogY;                /* Y coordinate of center of gravity.        */

   /* Get size of depth map. */
   MbufInquire(MilDepthMap, M_SIZE_X, &SizeX);
   MbufInquire(MilDepthMap, M_SIZE_Y, &SizeY);

   /* Allocate a binary image buffer for fast processing. */
   MbufAlloc2d(MilSystem, SizeX, SizeY,  1+M_UNSIGNED, M_IMAGE+M_PROC, &MilBinImage);

   /* Binarize image. */
   DefectThreshold = (EXPECTED_HEIGHT-DEFECT_THRESHOLD) * SCALE_FACTOR;
   MimBinarize(MilDepthMap, MilBinImage, M_FIXED+M_LESS_OR_EQUAL, DefectThreshold, M_NULL);

   /* Remove small particles. */
   MimOpen(MilBinImage, MilBinImage, MIN_BLOB_RADIUS, M_BINARY);

   /* Allocate a feature list. */ 
   MblobAllocFeatureList(MilSystem, &MilBlobFeatureList); 
  
   /* Enable the Center Of Gravity and Min Pixel features calculation. */ 
   MblobSelectFeature(MilBlobFeatureList, M_CENTER_OF_GRAVITY);
   MblobSelectFeature(MilBlobFeatureList, M_MIN_PIXEL);
 
   /* Allocate a blob result buffer. */
   MblobAllocResult(MilSystem, &MilBlobResult);
 
   /* Calculate selected features for each blob. */ 
   MblobCalculate(MilBinImage, MilDepthMap, MilBlobFeatureList, MilBlobResult); 
 
   /* Get the total number of selected blobs. */ 
   MblobGetNumber(MilBlobResult, &TotalBlobs); 
   MosPrintf(MIL_TEXT("Number of defects: %ld\n"), TotalBlobs);

   /* Read and print the blob characteristics. */ 
   if ((CogX      = (MIL_DOUBLE *)malloc(TotalBlobs*sizeof(MIL_DOUBLE))) && 
       (CogY      = (MIL_DOUBLE *)malloc(TotalBlobs*sizeof(MIL_DOUBLE))) &&
       (MinPixels = (MIL_INT    *)malloc(TotalBlobs*sizeof(MIL_INT   )))
      )
      { 
      /* Get the results. */
      MblobGetResult(MilBlobResult, M_CENTER_OF_GRAVITY_X, CogX);
      MblobGetResult(MilBlobResult, M_CENTER_OF_GRAVITY_Y, CogY);
      MblobGetResult(MilBlobResult, M_MIN_PIXEL+M_TYPE_MIL_INT, MinPixels);

      /* Draw the defects. */
      MgraColor(M_DEFAULT, M_COLOR_RED);
      MblobDraw(M_DEFAULT, MilBlobResult, MilOverlayImage,
                M_DRAW_BLOBS, M_INCLUDED_BLOBS, M_DEFAULT);
      MgraColor(M_DEFAULT, M_COLOR_WHITE);

      /* Print the depth of each blob. */
      for(n=0; n < TotalBlobs; n++)
         {
         MIL_DOUBLE    DepthOfDefect;
         MIL_TEXT_CHAR DepthString[16];

         /* Write the depth of the defect in the overlay. */
         DepthOfDefect = EXPECTED_HEIGHT - (MinPixels[n]/SCALE_FACTOR);
         MosSprintf(DepthString, 16, MIL_TEXT("%.2f mm"), DepthOfDefect);

         MosPrintf(MIL_TEXT("Defect #%d: depth =%5.2f mm\n\n"),
                   n, DepthOfDefect);
         MgraText(M_DEFAULT, MilOverlayImage, CogX[n]+TEXT_H_OFFSET_1,
                                        CogY[n]+TEXT_V_OFFSET_1, MIL_TEXT("Defect depth"));
         MgraText(M_DEFAULT, MilOverlayImage, CogX[n]+TEXT_H_OFFSET_2,
                                                     CogY[n]+TEXT_V_OFFSET_2, DepthString);
         }

      free(CogX);
      free(CogY);
      free(MinPixels);
      }
   else
      MosPrintf(MIL_TEXT("Error: Not enough memory.\n\n"));

   /* Free all allocations. */
   MblobFree(MilBlobResult);
   MblobFree(MilBlobFeatureList);
   MbufFree(MilBinImage);
   }

/* Color constants for display LUT. */
#define BLUE_HUE  171.0          /* Expected depths will be blue.   */
#define RED_HUE   0.0            /* Worst defects will be red.      */
#define FULL_SATURATION 255      /* All colors are fully saturated. */
#define HALF_LUMINANCE  128      /* All colors have half luminance. */

/* Creates a color display LUT to show defects in red. */
void SetupColorDisplay(MIL_ID MilSystem, MIL_ID MilDisplay, MIL_INT SizeBit)
   {
   MIL_ID  MilRampLut1Band,      /* LUT containing hue values.             */
           MilRampLut3Band,      /* RGB LUT used by display.               */
           MilColorImage;        /* Image used for HSL to RGB conversion.  */
   MIL_INT DefectGrayLevel,      /* Gray level under which all is red.     */
           ExpectedGrayLevel,    /* Gray level over which all is blue.     */
           NbGrayLevels;

   /* Number of possible gray levels in corrected depth map. */
   NbGrayLevels = (MIL_INT)(1 << SizeBit);

   /* Allocate 1-band LUT that will contain hue values. */
   MbufAlloc1d(MilSystem, NbGrayLevels, 8+M_UNSIGNED, M_LUT, &MilRampLut1Band);

   /* Compute limit gray values. */
   DefectGrayLevel   = (MIL_INT)((EXPECTED_HEIGHT-SATURATED_DEFECT)*SCALE_FACTOR);
   ExpectedGrayLevel = (MIL_INT)(EXPECTED_HEIGHT*SCALE_FACTOR);

   /* Create hue values for each possible gray level. */
   MgenLutRamp(MilRampLut1Band, 0, RED_HUE, DefectGrayLevel, RED_HUE);
   MgenLutRamp(MilRampLut1Band, DefectGrayLevel, RED_HUE, ExpectedGrayLevel, BLUE_HUE);
   MgenLutRamp(MilRampLut1Band, ExpectedGrayLevel, BLUE_HUE, NbGrayLevels-1, BLUE_HUE);

   /* Create a HSL image buffer. */
   MbufAllocColor(MilSystem, 3, NbGrayLevels, 1, 8+M_UNSIGNED, M_IMAGE, &MilColorImage);
   MbufClear(MilColorImage, M_RGB888(0, FULL_SATURATION, HALF_LUMINANCE));

   /* Set its H band (hue) to the LUT contents and convert the image to RGB. */
   MbufCopyColor2d(MilRampLut1Band, MilColorImage, 0, 0, 0, 0, 0, 0, NbGrayLevels, 1);
   MimConvert(MilColorImage, MilColorImage, M_HSL_TO_RGB);

   /* Create RGB LUT to give to display and copy image contents. */
   MbufAllocColor(MilSystem, 3, NbGrayLevels, 1, 8+M_UNSIGNED, M_LUT, &MilRampLut3Band);
   MbufCopy(MilColorImage, MilRampLut3Band);

   /* Associates LUT to display. */
   MdispLut(MilDisplay, MilRampLut3Band);

   /* Free all allocations. */
   MbufFree(MilRampLut1Band);
   MbufFree(MilRampLut3Band);
   MbufFree(MilColorImage);
   }

/****************************************************************************
 Calibrated camera example.
****************************************************************************/

/* Input sequence specifications. */
#define GRID_FILENAME                M_IMAGE_PATH MIL_TEXT("GridForLaser.mim")
#define LASERLINE_FILENAME           M_IMAGE_PATH MIL_TEXT("LaserLine.mim")
#define OBJECT2_SEQUENCE_FILE        M_IMAGE_PATH MIL_TEXT("Cookie.avi")

/* Camera calibration grid parameters. */
#define GRID_NB_ROWS             13
#define GRID_NB_COLS             12
#define GRID_ROW_SPACING         5.0     /* in mm                */
#define GRID_COL_SPACING         5.0     /* in mm                */

/* Laser device setup parameters. */
#define CONVEYOR_SPEED           -0.2     /* in mm/frame          */

/* Fully corrected depth map generation parameters. */
#define DEPTH_MAP_SIZE_X         480      /* in pixels            */
#define DEPTH_MAP_SIZE_Y         480      /* in pixels            */
#define GAP_DEPTH                1.5      /* in mm                */
#define SCALE_X                  0.15     /* in mm/pixel          */
#define SCALE_Y                  0.15     /* in mm/pixel          */
#define SCALE_Z                  -0.0003  /* in mm/gray level     */
#define WORLD_OFFSET_X           -12.0    /* in mm                */
#define WORLD_OFFSET_Y           4.0      /* in mm                */
#define WORLD_OFFSET_Z           0.0      /* in mm                */

/* D3D display parameters */
#define D3D_DISPLAY_SIZE_X     640
#define D3D_DISPLAY_SIZE_Y     480

/* Peak detection parameters. */
#define MAX_LINE_WIDTH_2             4
#define MIN_INTENSITY_2            100

/* Everything below (less negative than) this is considered as noise. */
#define MIN_HEIGHT_THRESHOLD -1.0 /* in mm */

void CalibratedCameraExample(MIL_ID MilSystem, MIL_ID MilDisplay)
   {
   MIL_ID      MilOverlayImage, /* Overlay image buffer identifier.          */
               MilImage,        /* Image buffer identifier (for processing). */
               MilCalibration,  /* Calibration context.                      */
               MilDepthMap,     /* Image buffer identifier (for results).    */
               MilLaser,        /* 3dmap laser profiling context identifier. */
               MilScan;         /* 3dmap result buffer identifier.           */
   MIL_INT     CalibrationStatus, /* Used to ensure if McalGrid() worked.    */
               SizeX,           /* Width of grabbed images.                  */
               SizeY,           /* Height of grabbed images.                 */
               NumberOfImages,  /* Number of frames for scanned objects.     */
               n;               /* Counter.                                  */
   MIL_DOUBLE  FrameRate,       /* Number of grabbed frames per second (in AVI). */
               StartTime,       /* Time at the beginning of each iteration.  */
               EndTime,         /* Time after processing for each iteration. */
               WaitTime,        /* Time to wait for next frame.              */
               Volume;          /* Volume of scanned object.                 */

   MosPrintf(MIL_TEXT("\n3D PROFILING AND VOLUME ANALYSIS:\n"));
   MosPrintf(MIL_TEXT("---------------------------------\n\n"));
   MosPrintf(MIL_TEXT("This program generates fully corrected 3d data of a scanned\n"));
   MosPrintf(MIL_TEXT("cookie and computes its volume. The laser profiling system uses\n"));
   MosPrintf(MIL_TEXT("a 3d-calibrated camera.\n\n"));

   /* Load grid image for camera calibration. */
   MbufRestore(GRID_FILENAME, MilSystem, &MilImage);

   /* Select display. */
   MdispSelect(MilDisplay, MilImage);

   MosPrintf(MIL_TEXT("Calibrating the camera...\n\n"));

   MbufInquire(MilImage, M_SIZE_X, &SizeX);
   MbufInquire(MilImage, M_SIZE_Y, &SizeY);

   /* Allocate calibration context in 3D mode. */
   McalAlloc(MilSystem, M_TSAI_BASED, M_DEFAULT, &MilCalibration);

   /* Calibrate the camera. */
   McalGrid(MilCalibration, MilImage, 0.0, 0.0, 0.0, GRID_NB_ROWS, GRID_NB_COLS,
            GRID_ROW_SPACING, GRID_COL_SPACING, M_DEFAULT, M_CHESSBOARD_GRID);

   McalInquire(MilCalibration, M_CALIBRATION_STATUS+M_TYPE_MIL_INT, &CalibrationStatus);
   if (CalibrationStatus != M_CALIBRATED)
      {
      McalFree(MilCalibration);
      MbufFree(MilImage);
      MosPrintf(MIL_TEXT("Camera calibration failed.\n"));
      MosPrintf(MIL_TEXT("Press <Enter> to end.\n\n"));
      MosGetch();
      return;
      }

   /* Prepare for overlay annotations. */
   MdispControl(MilDisplay, M_OVERLAY, M_ENABLE);
   MdispInquire(MilDisplay, M_OVERLAY_ID, &MilOverlayImage);
   MgraColor(M_DEFAULT, M_COLOR_GREEN);

   /* Draw camera calibration points. */
   McalDraw(M_DEFAULT, MilCalibration, MilOverlayImage, M_DRAW_IMAGE_POINTS,
                                                                     M_DEFAULT, M_DEFAULT);

   MosPrintf(MIL_TEXT("The camera was calibrated using a chessboard grid.\n\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   /* Disable overlay. */
   MdispControl(MilDisplay, M_OVERLAY, M_DISABLE);

   /* Load laser line image. */
   MbufLoad(LASERLINE_FILENAME, MilImage);

   /* Allocate 3dmap objects. */
   M3dmapAlloc(MilSystem, M_LASER, M_CALIBRATED_CAMERA_LINEAR_MOTION, &MilLaser);
   M3dmapAllocResult(MilSystem, M_LASER_DATA, M_DEFAULT, &MilScan);

   /* Set laser line extraction options. */
   M3dmapControl(MilLaser, M_DEFAULT, M_PEAK_WIDTH   , MAX_LINE_WIDTH_2);
   M3dmapControl(MilLaser, M_DEFAULT, M_MIN_INTENSITY, MIN_INTENSITY_2 );

   /* Calibrate laser profiling context. */
   M3dmapAddScan(MilLaser, MilScan, MilImage, M_NULL, M_NULL, M_DEFAULT, M_DEFAULT);
   M3dmapCalibrate(MilLaser, MilScan, MilCalibration, M_DEFAULT);

   MosPrintf(MIL_TEXT("The laser profiling system has been calibrated using the image\n"));
   MosPrintf(MIL_TEXT("of one laser line.\n\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   /* Empty all result buffer contents.
      It will now be reused for extracting 3d points. */
   M3dmapAddScan(M_NULL, MilScan, M_NULL, M_NULL, M_NULL, M_DEFAULT, M_RESET);

   /* Set speed of scanned object (speed in mm/frame is constant). */
   M3dmapControl(MilLaser, M_DEFAULT, M_SCAN_SPEED, CONVEYOR_SPEED);

   /* Inquire characteristics of the input sequence. */
   MbufDiskInquire(OBJECT2_SEQUENCE_FILE, M_NUMBER_OF_IMAGES, &NumberOfImages);
   MbufDiskInquire(OBJECT2_SEQUENCE_FILE, M_FRAME_RATE, &FrameRate);

   /* Open the object sequence file for reading. */
   MbufImportSequence(OBJECT2_SEQUENCE_FILE, M_DEFAULT, M_NULL, M_NULL, NULL, M_NULL,
                                                                           M_NULL, M_OPEN);

   MosPrintf(MIL_TEXT("The cookie is being scanned to generate 3d data.\n\n"));

   /* Read and process all images in the input sequence. */
   MappTimer(M_DEFAULT, M_TIMER_READ+M_SYNCHRONOUS, &StartTime);

   for (n = 0; n < NumberOfImages; n++)
      {
      /* Read image from sequence. */
      MbufImportSequence(OBJECT2_SEQUENCE_FILE, M_DEFAULT, M_LOAD, M_NULL, &MilImage,
                                                                     M_DEFAULT, 1, M_READ);

      /* Analyze the image to extract laser line and correct its depth. */
      M3dmapAddScan(MilLaser, MilScan, MilImage, M_NULL, M_NULL, M_DEFAULT, M_DEFAULT);

      /* Wait to have a proper frame rate, if necessary. */
      MappTimer(M_DEFAULT, M_TIMER_READ+M_SYNCHRONOUS, &EndTime);
      WaitTime = (1.0/FrameRate) - (EndTime - StartTime);
      if (WaitTime > 0)
         MappTimer(M_DEFAULT, M_TIMER_WAIT, &WaitTime);
      MappTimer(M_DEFAULT, M_TIMER_READ+M_SYNCHRONOUS, &StartTime);
      }

   /* Close the object sequence file. */
   MbufImportSequence(OBJECT2_SEQUENCE_FILE, M_DEFAULT, M_NULL, M_NULL, NULL, M_NULL,
                                                                          M_NULL, M_CLOSE);

   /* Allocate image for the fully corrected depth map. */
   MbufAlloc2d(MilSystem, DEPTH_MAP_SIZE_X, DEPTH_MAP_SIZE_Y, 16+M_UNSIGNED,
                                                       M_IMAGE+M_PROC+M_DISP, &MilDepthMap);

   /* Set fully corrected depth map generation parameters. */
   M3dmapControl(MilScan, M_DEFAULT, M_FILL_MODE,                  M_X_THEN_Y);
   M3dmapControl(MilScan, M_DEFAULT, M_FILL_SHARP_ELEVATION,       M_MIN);
   M3dmapControl(MilScan, M_DEFAULT, M_FILL_SHARP_ELEVATION_DEPTH, GAP_DEPTH);
   M3dmapControl(MilScan, M_DEFAULT, M_PIXEL_SIZE_X,               SCALE_X);
   M3dmapControl(MilScan, M_DEFAULT, M_PIXEL_SIZE_Y,               SCALE_Y);
   M3dmapControl(MilScan, M_DEFAULT, M_GRAY_LEVEL_SIZE_Z,          SCALE_Z);
   M3dmapControl(MilScan, M_DEFAULT, M_WORLD_POS_X,                WORLD_OFFSET_X);
   M3dmapControl(MilScan, M_DEFAULT, M_WORLD_POS_Y,                WORLD_OFFSET_Y);
   M3dmapControl(MilScan, M_DEFAULT, M_WORLD_POS_Z,                WORLD_OFFSET_Z);

   /* Get fully corrected depth map from accumulated information in the result buffer. */
   M3dmapExtract(MilScan, MilDepthMap, M_NULL, M_CORRECTED_DEPTH_MAP, M_DEFAULT, M_DEFAULT);

   /* Remove noise. */
   /* Set all small values to 0, so that all remaining non-zeros are part of the cookie. */
   MimClip(MilDepthMap, MilDepthMap, M_LESS,
                       (MIN_HEIGHT_THRESHOLD-WORLD_OFFSET_Z)/SCALE_Z, M_NULL, 0.0, M_NULL);

   /* Compute the volume of the cookie. */
   M3dmapStat(MilDepthMap, M_NULL, M_NULL, M_NULL, M_VOLUME, M_DEFAULT, M_DEFAULT, &Volume);

   MosPrintf(MIL_TEXT("Fully corrected 3d data of the cookie is displayed.\n\n"));

#if M_MIL_USE_WINDOWS && !M_MIL_USE_CE
   /* Try to allocate D3D display. */
   MIL_DISP_D3D_HANDLE DispHandle;
   DispHandle = MdepthD3DAlloc(MilDepthMap, M_NULL,
                               D3D_DISPLAY_SIZE_X,
                               D3D_DISPLAY_SIZE_Y,
                               M_DEFAULT,
                               M_DEFAULT,
                               M_DEFAULT,
                               M_DEFAULT,
                               M_DEFAULT,
                               M_DEFAULT,
                               0);

   if (DispHandle != NULL)
      {
      /* Hide Mil Display. */
      MdispControl(MilDisplay, M_WINDOW_SHOW, M_DISABLE );
      MdispD3DShow(DispHandle);
      MdispD3DPrintHelp(DispHandle);
      }
   else
#endif
      {
      MdispControl(MilDisplay, M_VIEW_MODE, M_AUTO_SCALE);
      MdispSelect(MilDisplay, MilDepthMap);
      }

   MosPrintf(MIL_TEXT("Volume of the cookie is %4.1f cm^3.\n\n"), -Volume/1000.0);
   MosPrintf(MIL_TEXT("Press <Enter> to end.\n\n"));
   MosGetch();

#if M_MIL_USE_WINDOWS && !M_MIL_USE_CE
   if (DispHandle != M_NULL)
      {
      MdispD3DHide(DispHandle);
      MdispD3DFree(DispHandle);
      }
#endif

   /* Free all allocations. */
   M3dmapFree(MilScan);
   M3dmapFree(MilLaser);
   McalFree(MilCalibration);
   MbufFree(MilDepthMap);
   MbufFree(MilImage);
   }
