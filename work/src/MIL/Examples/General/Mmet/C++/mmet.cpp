/*****************************************************************************/
/* 
 * File name: Mmet.cpp
 *
 * Synopsis:  This program uses the MIL Metrology module to measure geometric 
 *            features and to validate tolerance relationships between features.
 */

#include <mil.h>

/* Example selection. */
#define RUN_SIMPLE_IMAGE_EXAMPLE     M_YES
#define RUN_COMPLETE_IMAGE_EXAMPLE   M_YES

/* Example functions declarations. */  
void SimpleImageExample(MIL_ID MilSystem, MIL_ID MilDisplay);
void CompleteImageExample(MIL_ID MilSystem, MIL_ID MilDisplay);

/*****************************************************************************
 Main.
 *****************************************************************************/
int MosMain(void)
   {
   MIL_ID MilApplication,     /* Application identifier. */
          MilSystem,          /* System Identifier.      */
          MilDisplay;         /* Display identifier.     */

   /* Allocate defaults. */
   MappAllocDefault(M_DEFAULT, &MilApplication, &MilSystem, &MilDisplay, M_NULL, M_NULL);

   /* Print module name. */
   MosPrintf(MIL_TEXT("\nMETROLOGY MODULE:\n"));
   MosPrintf(MIL_TEXT("-------------------\n\n"));

   #if (RUN_SIMPLE_IMAGE_EXAMPLE)
   SimpleImageExample(MilSystem, MilDisplay);
   #endif

   #if (RUN_COMPLETE_IMAGE_EXAMPLE)
   CompleteImageExample(MilSystem, MilDisplay);
   #endif

   /* Free defaults. */    
   MappFreeDefault(MilApplication, MilSystem, MilDisplay, M_NULL, M_NULL);
   
   return 0;
   }

/*****************************************************************************
Simple example. 
*****************************************************************************/
/* Source MIL image file specification. */ 
#define METROL_SIMPLE_IMAGE_FILE M_IMAGE_PATH MIL_TEXT("SingleModel.mim")

/* Region parameters */
#define TOP_RING_POSITION_X        240
#define TOP_RING_POSITION_Y        155
#define TOP_RING_START_RADIUS        2
#define TOP_RING_END_RADIUS         15 

#define MIDDLE_RING_POSITION_X     240
#define MIDDLE_RING_POSITION_Y     190
#define MIDDLE_RING_START_RADIUS     2
#define MIDDLE_RING_END_RADIUS      15

#define BOTTOM_RECT_POSITION_X     320
#define BOTTOM_RECT_POSITION_Y     265
#define BOTTOM_RECT_WIDTH          170
#define BOTTOM_RECT_HEIGHT          20
#define BOTTOM_RECT_ANGLE          180

/* Tolerance parameters */
#define PERPENDICULARITY_MIN       0.5
#define PERPENDICULARITY_MAX       0.5

/* Color definitions */
#define FAIL_COLOR       M_RGB888(255,0,0)
#define PASS_COLOR       M_RGB888(0,255,0)
#define REGION_COLOR     M_RGB888(0,100,255)
#define FEATURE_COLOR    M_RGB888(255,0,255)

void SimpleImageExample(MIL_ID MilSystem, MIL_ID MilDisplay)
   {
   MIL_ID MilImage,                    /* Image buffer identifier. */
          GraphicList,                 /* Graphic list identifier. */
          MilMetrolContext,            /* Metrology Context        */
          MilMetrolResult;             /* Metrology Result         */

   MIL_DOUBLE  Status,
               Value;

   MIL_INT FeatureIndexForTopConstructedPoint    = M_FEATURE_INDEX(1); 
   MIL_INT FeatureIndexForMiddleConstructedPoint = M_FEATURE_INDEX(2); 
   MIL_INT FeatureIndexForConstructedSegment[2]  = { M_FEATURE_INDEX(3),M_FEATURE_INDEX(4) };
   MIL_INT FeatureIndexForTolerance[2]           = { M_FEATURE_INDEX(5),M_FEATURE_INDEX(6) };

   /* Restore and display the source image. */
   MbufRestore(METROL_SIMPLE_IMAGE_FILE, MilSystem, &MilImage);
   MdispSelect(MilDisplay, MilImage);

   /* Allocate a graphic list to hold the subpixel annotations to draw. */
   MgraAllocList(MilSystem, M_DEFAULT, &GraphicList);

   /* Associate the graphic list to the display for annotations. */
   MdispControl(MilDisplay, M_ASSOCIATED_GRAPHIC_LIST_ID, GraphicList);

   /* Allocate metrology context and result. */
   MmetAlloc(MilSystem, M_DEFAULT, &MilMetrolContext);
   MmetAllocResult(MilSystem, M_DEFAULT, &MilMetrolResult);

   /* Add a first measured circle feature to context and set its search region */
   MmetAddFeature(MilMetrolContext, M_MEASURED, M_CIRCLE, M_DEFAULT, 
                  M_DEFAULT, M_NULL, M_NULL, M_DEFAULT, M_DEFAULT);

   MmetSetRegion(MilMetrolContext, M_FEATURE_INDEX(1), M_DEFAULT, M_RING, 
                 TOP_RING_POSITION_X, TOP_RING_POSITION_Y, TOP_RING_START_RADIUS, 
                 TOP_RING_END_RADIUS, M_NULL, M_NULL);

   /* Add a second measured circle feature to context and set its search region */
   MmetAddFeature(MilMetrolContext, M_MEASURED, M_CIRCLE, M_DEFAULT,
                  M_DEFAULT, M_NULL, M_NULL, M_DEFAULT, M_DEFAULT);

   MmetSetRegion(MilMetrolContext, M_FEATURE_INDEX(2), M_DEFAULT, M_RING, 
                 MIDDLE_RING_POSITION_X, MIDDLE_RING_POSITION_Y,  
                 MIDDLE_RING_START_RADIUS,MIDDLE_RING_END_RADIUS, M_NULL, M_NULL);

   /* Add a first constructed point feature to context */
   MmetAddFeature(MilMetrolContext, M_CONSTRUCTED, M_POINT, M_DEFAULT,
                  M_CENTER, &FeatureIndexForTopConstructedPoint, M_NULL, 1, M_DEFAULT);

   /* Add a second constructed point feature to context */
   MmetAddFeature(MilMetrolContext, M_CONSTRUCTED, M_POINT, M_DEFAULT,
                  M_CENTER, &FeatureIndexForMiddleConstructedPoint, M_NULL, 1, M_DEFAULT);

   /* Add a constructed segment feature to context passing through the two points */
   MmetAddFeature(MilMetrolContext, M_CONSTRUCTED, M_SEGMENT, M_DEFAULT,
                  M_CONSTRUCTION, FeatureIndexForConstructedSegment, M_NULL, 2, M_DEFAULT);

   /* Add a first segment feature to context and set its search region */
   MmetAddFeature(MilMetrolContext, M_MEASURED, M_SEGMENT, M_DEFAULT,
                  M_DEFAULT, M_NULL, M_NULL, M_DEFAULT, M_DEFAULT);

   MmetSetRegion(MilMetrolContext, M_FEATURE_INDEX(6), M_DEFAULT, M_RECTANGLE, 
                 BOTTOM_RECT_POSITION_X, BOTTOM_RECT_POSITION_Y, BOTTOM_RECT_WIDTH, 
                 BOTTOM_RECT_HEIGHT, BOTTOM_RECT_ANGLE, M_NULL);

   /* Add perpendicularity tolerance */
   MmetAddTolerance(MilMetrolContext, M_PERPENDICULARITY, M_DEFAULT, PERPENDICULARITY_MIN, 
                    PERPENDICULARITY_MAX, FeatureIndexForTolerance, M_NULL, 2, M_DEFAULT);

   /* Calculate */
   MmetCalculate(MilMetrolContext, MilImage, MilMetrolResult, M_DEFAULT);

   /* Draw region */
   MgraColor(M_DEFAULT, REGION_COLOR);
   MmetDraw(M_DEFAULT, MilMetrolResult, GraphicList, M_DRAW_REGION, M_DEFAULT, 
                                                                        M_DEFAULT);
   MosPrintf(MIL_TEXT("Regions used to calculate measured features:\n"));
   MosPrintf(MIL_TEXT("- two measured circles\n"));
   MosPrintf(MIL_TEXT("- one measured segment\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   /* Clear annotations. */
   MgraClear(M_DEFAULT, GraphicList);

   MgraColor(M_DEFAULT, FEATURE_COLOR);
   MmetDraw(M_DEFAULT, MilMetrolResult, GraphicList, M_DRAW_FEATURE, M_DEFAULT, 
                                                                         M_DEFAULT);
   MosPrintf(MIL_TEXT("Calculated features:\n"));

   MmetGetResult(MilMetrolResult, M_FEATURE_INDEX(1), M_RADIUS, &Value);
   MosPrintf(MIL_TEXT("- first measured circle:  radius=%.2f\n"), Value);

   MmetGetResult(MilMetrolResult, M_FEATURE_INDEX(2), M_RADIUS, &Value);
   MosPrintf(MIL_TEXT("- second measured circle: radius=%.2f\n"), Value);

   MmetGetResult(MilMetrolResult, M_FEATURE_INDEX(5), M_LENGTH, &Value);
   MosPrintf(MIL_TEXT("- constructed segment between the two circle centers: ")
             MIL_TEXT("length=%.2f\n"), Value);

   MmetGetResult(MilMetrolResult, M_FEATURE_INDEX(6), M_LENGTH, &Value);
   MosPrintf(MIL_TEXT("- measured segment: length=%.2f\n"), Value);   

   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   /* Get angularity tolerance status and value*/
   MmetGetResult(MilMetrolResult, M_TOLERANCE_INDEX(0), M_STATUS, &Status);
   MmetGetResult(MilMetrolResult, M_TOLERANCE_INDEX(0), M_VALUE, &Value);

   if(Status==M_PASS)
      {
      MgraColor(M_DEFAULT, PASS_COLOR);
      MosPrintf(MIL_TEXT("Perpendicularity between the two segments: %.2f ")
                MIL_TEXT("degrees.\n"), Value);
      }
   else
      {
      MgraColor(M_DEFAULT, FAIL_COLOR);
      MosPrintf(MIL_TEXT("Perpendicularity between the two segments - Fail.\n"));
      }
   MmetDraw(M_DEFAULT, MilMetrolResult, GraphicList, M_DRAW_TOLERANCE, 
                                           M_TOLERANCE_INDEX(0), M_DEFAULT);   

   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   /* Free all allocations. */
   MgraFree(GraphicList);
   MmetFree(MilMetrolResult);
   MmetFree(MilMetrolContext);
   MbufFree(MilImage);
   }

/*****************************************************************************
 Complete example. 
 *****************************************************************************/
/* Source MIL image, calibration and model finder context file specification. */ 
#define METROL_CALIBRATION_FILE   M_IMAGE_PATH MIL_TEXT("Hook.mca")
#define METROL_COMPLETE_IMAGE_FILE M_IMAGE_PATH MIL_TEXT("Hook.tif")
#define METROL_MODEL_FINDER_FILE  M_IMAGE_PATH MIL_TEXT("Hook.mmf")

/* Region parameters */
#define CIRCLE1_LABEL            1
#define RING1_POSITION_X         1.10
#define RING1_POSITION_Y         0.80
#define RING1_START_RADIUS       0.20
#define RING1_END_RADIUS         0.50

#define CIRCLE2_LABEL            2
#define RING2_POSITION_X         1.10
#define RING2_POSITION_Y         3.00
#define RING2_START_RADIUS       0.10
#define RING2_END_RADIUS         0.40

#define SEGMENT1_LABEL           3
#define RECT1_POSITION_X         0.10
#define RECT1_POSITION_Y         2.40
#define RECT1_WIDTH              1.40
#define RECT1_HEIGHT             0.30
#define RECT1_ANGLE              90.0

#define SEGMENT2_LABEL           4
#define RECT2_POSITION_X         0.90
#define RECT2_POSITION_Y         2.80
#define RECT2_WIDTH              0.40
#define RECT2_HEIGHT             0.20
#define RECT2_ANGLE             165.0

#define POINT1_LABEL             5
#define SEG1_START_X             1.60
#define SEG1_START_Y             1.50
#define SEG1_END_X               1.60
#define SEG1_END_Y               2.40

/* Tolerance parameters */
#define MIN_DISTANCE_LABEL       1
#define ANGULARITY_LABEL         2
#define MAX_DISTANCE_LABEL       3

#define MIN_DISTANCE_VALUE_MIN   1.40
#define MIN_DISTANCE_VALUE_MAX   1.60
#define MAX_DISTANCE_VALUE_MIN   0.40
#define MAX_DISTANCE_VALUE_MAX   0.60
#define ANGULARITY_VALUE_MIN     65.0
#define ANGULARITY_VALUE_MAX     75.0

void CompleteImageExample(MIL_ID MilSystem, MIL_ID MilDisplay)
   {
   MIL_ID MilImage,                        /* Image buffer identifier. */
          GraphicList,                     /* Graphic list identifier. */
          MilCalibration,                  /* Calibration context      */
          MilMetrolContext,                /* Metrology Context        */
          MilMetrolResult,                 /* Metrology Result         */
          MilModelFinderContext,           /* Model Finder Context     */
          MilModelFinderResult;            /* Model Finder Result      */
   
   MIL_DOUBLE Status,
              Value;

   MIL_INT MinDistanceFeatureLabels[2]  = { CIRCLE1_LABEL , CIRCLE2_LABEL  };
   MIL_INT AngularityFeatureLabels[2]   = { SEGMENT1_LABEL, SEGMENT2_LABEL };
   MIL_INT MaxDistanceFeatureLabels[2]  = { POINT1_LABEL  , POINT1_LABEL   };
   MIL_INT MaxDistanceFeatureIndices[2] = { 0             , 1              };

   /* Restore and display the source image. */
   MbufRestore(METROL_COMPLETE_IMAGE_FILE, MilSystem, &MilImage);
   MdispSelect(MilDisplay, MilImage);

    /* Restore and associate calibration context to source image */   
   McalRestore(METROL_CALIBRATION_FILE, MilSystem, M_DEFAULT, &MilCalibration);
   McalAssociate(MilCalibration, MilImage, M_DEFAULT);

   /* Allocate a graphic list to hold the subpixel annotations to draw. */
   MgraAllocList(MilSystem, M_DEFAULT, &GraphicList);

   /* Associate the graphic list to the display for annotations. */
   MdispControl(MilDisplay, M_ASSOCIATED_GRAPHIC_LIST_ID, GraphicList);

   /* Allocate metrology context and result. */
   MmetAlloc(MilSystem, M_DEFAULT, &MilMetrolContext);
   MmetAllocResult(MilSystem, M_DEFAULT, &MilMetrolResult);

   /* Add a first measured circle feature to context and set its search region */
   MmetAddFeature(MilMetrolContext, M_MEASURED, M_CIRCLE, CIRCLE1_LABEL, 
                  M_DEFAULT, M_NULL, M_NULL, M_DEFAULT, M_DEFAULT);

   MmetSetRegion(MilMetrolContext, M_FEATURE_LABEL(CIRCLE1_LABEL), M_DEFAULT, M_RING, 
                 RING1_POSITION_X, RING1_POSITION_Y, RING1_START_RADIUS, RING1_END_RADIUS, 
                 M_NULL, M_NULL);

   /* Add a second measured circle feature to context and set its search region */
   MmetAddFeature(MilMetrolContext, M_MEASURED, M_CIRCLE, CIRCLE2_LABEL,
                  M_DEFAULT, M_NULL, M_NULL, M_DEFAULT, M_DEFAULT);
   
   MmetSetRegion(MilMetrolContext, M_FEATURE_LABEL(CIRCLE2_LABEL), M_DEFAULT, M_RING, 
                 RING2_POSITION_X, RING2_POSITION_Y, RING2_START_RADIUS, RING2_END_RADIUS, 
                 M_NULL, M_NULL);

   /* Add a first measured segment feature to context and set its search region */
   MmetAddFeature(MilMetrolContext, M_MEASURED, M_SEGMENT, SEGMENT1_LABEL,
                  M_DEFAULT, M_NULL, M_NULL, M_DEFAULT, M_DEFAULT);

   MmetSetRegion(MilMetrolContext, M_FEATURE_LABEL(SEGMENT1_LABEL), M_DEFAULT, M_RECTANGLE, 
                 RECT1_POSITION_X, RECT1_POSITION_Y, RECT1_WIDTH, RECT1_HEIGHT, 
                 RECT1_ANGLE, M_NULL);

   MmetControl(MilMetrolContext, M_FEATURE_LABEL(SEGMENT1_LABEL), M_EDGEL_ANGLE_RANGE, 10);

   /* Add a second measured segment feature to context and set its search region */
   MmetAddFeature(MilMetrolContext, M_MEASURED, M_SEGMENT, SEGMENT2_LABEL,
                  M_INNER_FIT, M_NULL, M_NULL, M_DEFAULT, M_DEFAULT);

   MmetSetRegion(MilMetrolContext, M_FEATURE_LABEL(SEGMENT2_LABEL), M_DEFAULT, M_RECTANGLE, 
                 RECT2_POSITION_X, RECT2_POSITION_Y, RECT2_WIDTH, RECT2_HEIGHT, 
                 RECT2_ANGLE, M_NULL);

   /* Add a second measured segment feature to context and set its search region */
   MmetAddFeature(MilMetrolContext, M_MEASURED, M_POINT, POINT1_LABEL,
                  M_DEFAULT, M_NULL, M_NULL, M_DEFAULT, M_DEFAULT);

   MmetSetRegion(MilMetrolContext, M_FEATURE_LABEL(POINT1_LABEL), M_DEFAULT, M_SEGMENT, 
                 SEG1_START_X, SEG1_START_Y, SEG1_END_X, SEG1_END_Y, 
                 M_NULL, M_NULL);

   /*Set the polarity and the maximum number of points to detect along the segment region*/
   MmetControl(MilMetrolContext, M_FEATURE_LABEL(POINT1_LABEL), 
                     M_EDGEL_RELATIVE_ANGLE, M_SAME_OR_REVERSE);
   MmetControl(MilMetrolContext, M_FEATURE_LABEL(POINT1_LABEL), M_NUMBER_MAX, 2);

   /* Add minimum distance tolerance */
   MmetAddTolerance(MilMetrolContext, M_DISTANCE_MIN, MIN_DISTANCE_LABEL, 
                    MIN_DISTANCE_VALUE_MIN, MIN_DISTANCE_VALUE_MAX,  
                    MinDistanceFeatureLabels, M_NULL, 2, M_DEFAULT);

   /* Add angularity tolerance */
   MmetAddTolerance(MilMetrolContext, M_ANGULARITY, ANGULARITY_LABEL, 
                    ANGULARITY_VALUE_MIN, ANGULARITY_VALUE_MAX,  
                    AngularityFeatureLabels, M_NULL, 2, M_DEFAULT);

   
   /* Add maximum distance tolerance */
   MmetAddTolerance(MilMetrolContext, M_DISTANCE_MAX, MAX_DISTANCE_LABEL, 
                    MAX_DISTANCE_VALUE_MIN, MAX_DISTANCE_VALUE_MAX,  
                    MaxDistanceFeatureLabels, MaxDistanceFeatureIndices, 2, M_DEFAULT);

   /* Calculate */
   MmetCalculate(MilMetrolContext, MilImage, MilMetrolResult, M_DEFAULT);

   /* Draw features */
   MgraColor(M_DEFAULT, REGION_COLOR);
   MmetDraw(M_DEFAULT, MilMetrolResult, GraphicList, M_DRAW_REGION, M_DEFAULT, 
                                                                        M_DEFAULT);
   MosPrintf(MIL_TEXT("Regions used to calculate measured features:\n"));
   MosPrintf(MIL_TEXT("- two measured circle features\n"));
   MosPrintf(MIL_TEXT("- two measured segment features\n"));
   MosPrintf(MIL_TEXT("- one measured points feature\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   /* Clear annotations. */
   MgraClear(M_DEFAULT, GraphicList);

   MgraColor(M_DEFAULT, FEATURE_COLOR);
   MmetDraw(M_DEFAULT, MilMetrolResult, GraphicList, M_DRAW_FEATURE, M_DEFAULT, 
                                                                         M_DEFAULT);
   MosPrintf(MIL_TEXT("Calculated features:\n"));

   MmetGetResult(MilMetrolResult, M_FEATURE_LABEL(CIRCLE1_LABEL), M_RADIUS, &Value);
   MosPrintf(MIL_TEXT("- first measured circle:   radius=%.2fmm\n"), Value);

   MmetGetResult(MilMetrolResult, M_FEATURE_LABEL(CIRCLE2_LABEL), M_RADIUS, &Value);
   MosPrintf(MIL_TEXT("- second measured circle:  radius=%.2fmm\n"), Value);

   MmetGetResult(MilMetrolResult, M_FEATURE_LABEL(SEGMENT1_LABEL), M_LENGTH, &Value);
   MosPrintf(MIL_TEXT("- first measured segment:  length=%.2fmm\n"), Value);

   MmetGetResult(MilMetrolResult, M_FEATURE_LABEL(SEGMENT2_LABEL), M_LENGTH, &Value);
   MosPrintf(MIL_TEXT("- second measured segment: length=%.2fmm\n"), Value);

   MosPrintf(MIL_TEXT("- two measured points\n"));

   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   /* Get angularity tolerance status and value */
   MmetGetResult(MilMetrolResult, M_TOLERANCE_LABEL(ANGULARITY_LABEL), M_STATUS, &Status);
   MmetGetResult(MilMetrolResult, M_TOLERANCE_LABEL(ANGULARITY_LABEL), M_VALUE, &Value);

   if(Status==M_PASS)
      {
      MgraColor(M_DEFAULT, PASS_COLOR);
      MosPrintf(MIL_TEXT("Angularity value: %.2f degrees.\n"), Value);
      }
   else
      {
      MgraColor(M_DEFAULT, FAIL_COLOR);
      MosPrintf(MIL_TEXT("Angularity value - Fail.\n"));
      }
   MmetDraw(M_DEFAULT, MilMetrolResult, GraphicList, M_DRAW_TOLERANCE, 
                            M_TOLERANCE_LABEL(ANGULARITY_LABEL), M_DEFAULT);   

   /* Get min distance tolerance status and value */
   MmetGetResult(MilMetrolResult, M_TOLERANCE_LABEL(MIN_DISTANCE_LABEL), M_STATUS, 
                                                                         &Status);
   MmetGetResult(MilMetrolResult, M_TOLERANCE_LABEL(MIN_DISTANCE_LABEL), M_VALUE, 
                                                                         &Value);

   if(Status==M_PASS)
      {
      MgraColor(M_DEFAULT, PASS_COLOR);
      MosPrintf(MIL_TEXT("Min distance tolerance value: %.2f mm.\n"), Value);
      }
   else
      {
      MgraColor(M_DEFAULT, FAIL_COLOR);
      MosPrintf(MIL_TEXT("Min distance tolerance value - Fail.\n"));
      }
   MmetDraw(M_DEFAULT, MilMetrolResult, GraphicList, M_DRAW_TOLERANCE, 
                           M_TOLERANCE_LABEL(MIN_DISTANCE_LABEL), M_DEFAULT);   

   /* Get max distance tolerance status and value */
   MmetGetResult(MilMetrolResult, M_TOLERANCE_LABEL(MAX_DISTANCE_LABEL), M_STATUS, 
                                                                         &Status);
   MmetGetResult(MilMetrolResult, M_TOLERANCE_LABEL(MAX_DISTANCE_LABEL), M_VALUE, 
                                                                         &Value);

   if(Status==M_PASS)
      {
      MgraColor(M_DEFAULT, PASS_COLOR);
      MosPrintf(MIL_TEXT("Max distance tolerance value: %.2f mm.\n"), Value);
      }
   else
      {
      MgraColor(M_DEFAULT, FAIL_COLOR);
      MosPrintf(MIL_TEXT("Max distance tolerance value - Fail.\n"));
      }
   MmetDraw(M_DEFAULT, MilMetrolResult, GraphicList, M_DRAW_TOLERANCE, 
                            M_TOLERANCE_LABEL(MAX_DISTANCE_LABEL), M_DEFAULT);   
   
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   /* Clear annotations. */
   MgraClear(M_DEFAULT, GraphicList);

   /* Restore the model finder context and calibrate it */
   MmodRestore(METROL_MODEL_FINDER_FILE, MilSystem, M_DEFAULT, &MilModelFinderContext);
   MmodControl(MilModelFinderContext, 0, M_ASSOCIATED_CALIBRATION, MilCalibration);

   /* Allocate a result buffer */
   MmodAllocResult(MilSystem, M_DEFAULT, &MilModelFinderResult);

   /* Find object occurrence */
   MmodPreprocess(MilModelFinderContext, M_DEFAULT);
   MmodFind(MilModelFinderContext, MilImage, MilModelFinderResult);

   /* Get number of found occurrences */
   MmodGetResult(MilModelFinderResult, M_GENERAL, M_NUMBER, &Value);

   if(Value==1)
      {
      MmodDraw(M_DEFAULT, MilModelFinderResult, GraphicList, 
                      M_DRAW_POSITION+M_DRAW_BOX, M_DEFAULT, M_DEFAULT);
      MosPrintf(MIL_TEXT("Found occurrence using MIL Model Finder.\n"));
      MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
      MosGetch();

      /* Clear annotations. */
      MgraClear(M_DEFAULT, GraphicList);

      /* Set the new context position */
      MmetSetPosition(MilMetrolContext, M_DEFAULT, M_RESULT, MilModelFinderResult, 
                                                    0, M_NULL, M_NULL, M_DEFAULT);

      /* Calculate */
      MmetCalculate(MilMetrolContext, MilImage, MilMetrolResult, M_DEFAULT);

      /* Draw features */
      MgraColor(M_DEFAULT, REGION_COLOR);
      MmetDraw(M_DEFAULT, MilMetrolResult, GraphicList, 
                          M_DRAW_REGION, M_DEFAULT, M_DEFAULT);
      MosPrintf(MIL_TEXT("Regions used to calculate measured ")
                MIL_TEXT("features at the new location.\n"));
      MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
      MosGetch();

      /* Clear annotations. */
      MgraClear(M_DEFAULT, GraphicList);

      MgraColor(M_DEFAULT, FEATURE_COLOR);
      MmetDraw(M_DEFAULT, MilMetrolResult, GraphicList, 
                              M_DRAW_FEATURE, M_DEFAULT, M_DEFAULT);
      MosPrintf(MIL_TEXT("Calculated features.\n"));

      MmetGetResult(MilMetrolResult, M_FEATURE_LABEL(CIRCLE1_LABEL), M_RADIUS, &Value);
      MosPrintf(MIL_TEXT("- first measured circle:   radius=%.2fmm\n"), Value);

      MmetGetResult(MilMetrolResult, M_FEATURE_LABEL(CIRCLE2_LABEL), M_RADIUS, &Value);
      MosPrintf(MIL_TEXT("- second measured circle:  radius=%.2fmm\n"), Value);

      MmetGetResult(MilMetrolResult, M_FEATURE_LABEL(SEGMENT1_LABEL), M_LENGTH, &Value);
      MosPrintf(MIL_TEXT("- first measured segment:  length=%.2fmm\n"), Value);

      MmetGetResult(MilMetrolResult, M_FEATURE_LABEL(SEGMENT2_LABEL), M_LENGTH, &Value);
      MosPrintf(MIL_TEXT("- second measured segment: length=%.2fmm\n"), Value);

      MosPrintf(MIL_TEXT("- two measured points\n"));

      MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
      MosGetch();

      /* Get angularity tolerance status and value */
      MmetGetResult(MilMetrolResult, M_TOLERANCE_LABEL(ANGULARITY_LABEL), M_STATUS, 
                                                                          &Status);
      MmetGetResult(MilMetrolResult, M_TOLERANCE_LABEL(ANGULARITY_LABEL), M_VALUE, 
                                                                          &Value);

      if(Status==M_PASS)
         {
         MgraColor(M_DEFAULT, PASS_COLOR);
         MosPrintf(MIL_TEXT("Angularity value: %.2f degrees.\n"), Value);
         }
      else
         {
         MgraColor(M_DEFAULT, FAIL_COLOR);
         MosPrintf(MIL_TEXT("Angularity value - Fail.\n"));
         }
      MmetDraw(M_DEFAULT, MilMetrolResult, GraphicList, M_DRAW_TOLERANCE, 
                                   M_TOLERANCE_LABEL(ANGULARITY_LABEL), M_DEFAULT);   

      /* Get min distance tolerance status and value */
      MmetGetResult(MilMetrolResult, M_TOLERANCE_LABEL(MIN_DISTANCE_LABEL), M_STATUS, 
                                                                            &Status);
      MmetGetResult(MilMetrolResult, M_TOLERANCE_LABEL(MIN_DISTANCE_LABEL), M_VALUE, 
                                                                            &Value);

      if(Status==M_PASS)
         {
         MgraColor(M_DEFAULT, PASS_COLOR);
         MosPrintf(MIL_TEXT("Min distance tolerance value: %.2f mm.\n"), Value);
         }
      else
         {
         MgraColor(M_DEFAULT, FAIL_COLOR);
         MosPrintf(MIL_TEXT("Min distance tolerance value - Fail.\n"));
         }
      MmetDraw(M_DEFAULT, MilMetrolResult, GraphicList, M_DRAW_TOLERANCE, 
                                  M_TOLERANCE_LABEL(MIN_DISTANCE_LABEL), M_DEFAULT);   

      /* Get max distance tolerance status and value */
      MmetGetResult(MilMetrolResult, M_TOLERANCE_LABEL(MAX_DISTANCE_LABEL), M_STATUS, 
                                                                            &Status);
      MmetGetResult(MilMetrolResult, M_TOLERANCE_LABEL(MAX_DISTANCE_LABEL), M_VALUE, 
                                                                            &Value);

      if(Status==M_PASS)
         {
         MgraColor(M_DEFAULT, PASS_COLOR);
         MosPrintf(MIL_TEXT("Max distance tolerance value: %.2f mm.\n"), Value);
         }
      else
         {
         MgraColor(M_DEFAULT, FAIL_COLOR);
         MosPrintf(MIL_TEXT("Max distance tolerance value - Fail.\n"));
         }
      MmetDraw(M_DEFAULT, MilMetrolResult, GraphicList, M_DRAW_TOLERANCE, 
                                  M_TOLERANCE_LABEL(MAX_DISTANCE_LABEL), M_DEFAULT);
      
      MosPrintf(MIL_TEXT("Press <Enter> to quit.\n\n"));
      MosGetch();
      }
   else
      {
      MosPrintf(MIL_TEXT("Occurrence not found.\n"));
      MosPrintf(MIL_TEXT("Press <Enter> to quit.\n\n"));
      MosGetch();
      }

   /* Free all allocations. */
   MgraFree(GraphicList);
   MmodFree(MilModelFinderContext);
   MmodFree(MilModelFinderResult);
   MmetFree(MilMetrolResult);
   MmetFree(MilMetrolContext);
   McalFree(MilCalibration);
   MbufFree(MilImage);
   }
