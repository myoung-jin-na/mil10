/***************************************************************************/
/*

    Filename:  MILMEAS.H
    Owner   :  Matrox Imaging dept.
    Revision:  10.00.2564
    Content :  This file contains the defines for the MIL measurement
               module. (Mmeas...).

    Copyright © Matrox Electronic Systems Ltd., 1992-2014.
    All Rights Reserved

*/
/***************************************************************************/

#ifndef __MILMEAS_H__
#define __MILMEAS_H__

#if (!M_MIL_LITE) // MIL FULL ONLY

/* C++ directive if needed */
#ifdef __cplusplus
extern "C"
{
#endif


/**************************************************************************/
/* CAPI defines                                                           */
/**************************************************************************/

/**************************************************************************/
/* MmeasAllocMarker                                                       */
/**************************************************************************/

#define M_POINT                           1L
#define M_EDGE                            2L
#define M_STRIPE                          3L
#define M_CIRCLE                 0x00000008L   // 8   Already defined in milmod.h
#define M_EDGE_LIST                   M_EDGE
#define M_STRIPE_LIST               M_STRIPE


/**************************************************************************/
/* MmeasAllocResult                                                       */
/**************************************************************************/
#define M_CALCULATE                       1L   // Already defined in mil.h


/**************************************************************************/
/* Bitwise values that the followings cannot take                         */
/**************************************************************************/

#define M_EDGE_FIRST                      0x20000000L//0x00100000L 
#define M_EDGE_SECOND                     0x40000000L//0x00200000L  
#define M_MIN                             0x02000000L
#define M_MAX                             0x04000000L
#define M_MEAN                            0x08000000L
#define M_STANDARD_DEVIATION              0x10000000L 


/**************************************************************************/
/* MmeasInquire(), MmeasSetMarker(), MmeasGetResult(), MmeasFindMarker(), */
/* MmeasCalculate() parameters :                                          */
/**************************************************************************/

#define M_ANGLE                           0x00000800L
#define M_WIDTH                           0x00010000L // Deprecated in MIL10.0
#define M_STRIPE_WIDTH                    0x00200000L
#define M_EDGEVALUE_PEAK_WIDTH            0x00400000L
#define M_EDGE_WIDTH                      0x00000004L
#define M_POLARITY                        0x00004000L
#define M_EDGEVALUE_PEAK_CONTRAST         0x00001000L
#define M_EDGE_CONTRAST                   0x00000008L
#define M_LENGTH                          0x00002000L
#define M_LINE_EQUATION                   0x00800000L
#define M_LINE_EQUATION_SLOPE             0x00801000L
#define M_LINE_EQUATION_INTERCEPT         0x00802000L
#define M_LINE_A                          0x00003000L // also defined in milmetrol.h
#define M_LINE_B                          0x00003001L // also defined in milmetrol.h
#define M_LINE_C                          0x00003002L // also defined in milmetrol.h
#define M_EDGE_INSIDE                     0x00000080L
#define M_POSITION                        0x00000402L
#define M_POSITION_IN_MARKER                    1377L
#define M_INCLUSION_POINT                       1581L
#define M_SPACING                         0x00000200L
#define M_NUMBER                                1009L
#define M_NUMBER_MIN                            1010L
#define M_FIT_ERROR                       0x00001700L  // already defined in MilMod.h
#define M_FIT_ERROR_MAX                         1378L
#define M_SEARCH_REGION_INPUT_UNITS             1392L  // 0x00000570L 
#define M_MARKER_REFERENCE_INPUT_UNITS          1393L  // 0x00000571L 
#define M_POINT_INPUT_UNITS                     1394L  // 0x00000572L
#define M_INCLUSION_POINT_INPUT_UNITS           1459L
#define M_MAX_ASSOCIATION_DISTANCE_INPUT_UNITS  1584L
#define M_SCORE                           0x00001400L
#define M_ORIENTATION                     0x00002400L
#define M_POSITION_X                      0x00003400L  // deprecated in MIL 9.0 PP2
#define M_POSITION_Y                      0x00004400L  // deprecated in MIL 9.0 PP2
#define M_EDGE_STRENGTH                   0x00006400L
#define M_BOX_ORIGIN                      0x00009400L
#define M_BOX_SIZE                        0x0000A400L
#define M_BOX_CENTER                      0x0000B400L    // applies to edges and stripes
#define M_RING_CENTER                     M_BOX_CENTER   // applies to circles
#define M_BOX_ANGLE_MODE                  0x0000E400L
#define M_BOX_ANGLE                       0x0000F400L
#define M_MULTIPLE_POINT_ANGLE            M_BOX_ANGLE // New flag name since MIL 9.0 PP2 for point markers, need to be at the same value as M_BOX_ANGLE 
#define M_BOX_ANGLE_DELTA_NEG             0x00010400L
#define M_BOX_ANGLE_DELTA_POS             0x00011400L
#define M_BOX_ANGLE_TOLERANCE             0x00012400L
#define M_BOX_ANGLE_ACCURACY              0x00013400L
#define M_SEARCH_REGION_ANGLE_INTERPOLATION_MODE   0x00014400L
#define M_EDGEVALUE_MIN                   0x00015400L
#define M_EDGEVALUE_VAR_MIN                     1711L
#define M_MARKER_REFERENCE                0x00016400L
#define M_BOX_ANGLE_SIZE                  0x00017400L
#define M_MARKER_TYPE                     0x00018400L
#define M_CONTROL_FLAG                    0x00019400L
#define M_EDGEVALUE_PEAK_POS_MIN          0x0001A400L
#define M_EDGEVALUE_PEAK_POS_MAX          0x0001B400L
#define M_EDGE_START                            1712L
#define M_EDGE_END                              1713L
#define M_BOX_EDGE_VALUES                 0x0001C400L
#define M_ANY_ANGLE                       0x0001D400L
#define M_FOUND_BOX_ANGLE                 M_ANY_ANGLE
#define M_VALID_FLAG                      0x0001E400L
#define M_BOX_CORNER_TOP_LEFT             0x0001F400L
#define M_BOX_CORNER_TOP_RIGHT            0x00020400L
#define M_BOX_CORNER_BOTTOM_LEFT          0x00021400L
#define M_BOX_CORNER_BOTTOM_RIGHT         0x00022400L
#define M_BOX_EDGE_VALUES_NUMBER          0x00023400L
#define M_INCLUSION_POINT_INSIDE_STRIPE   0x00024400L
#define M_BOX_ANGLE_REFERENCE             0x00025400L
#define M_TOTAL_SCORE                     0x00029400L
#define M_BOX_RADIUS                      0x0002A400L
#define M_BOX_START_ANGLE                 0x0002B400L
#define M_BOX_END_ANGLE                   0x0002C400L
#define M_ALL                             0x40000000L // Already defined in Mil.h
#define M_ALL_OLD                                  0L

#define M_RING_RADII                      0x0002F400L

#define M_RESULT_OUTPUT_UNITS                   1300L    // Also in milim.h
#define M_NUMBER_OF_OUTLIERS                    1379L
#define M_SEARCH_REGION_WAS_CLIPPED             1467L

#define M_RADIUS                          0x00000040L       // Already defined in MilMod.h
#define M_RADIUS_MIN                             214L  // Already defined in MilMod.h
#define M_RADIUS_MAX                             216L  // Already defined in MilMod.h
#define M_RADIUS_VARIATION                       217L   
#define M_SUB_REGIONS_NUMBER                     218L
#define M_SUB_EDGES_NUMBER_MIN                   219L
#define M_SUB_EDGES_POSITION                     220L
#define M_SUB_EDGES_WEIGHT                       222L
#define M_SUB_EDGES_MARKER_INDEX                 223L
#define M_SUB_REGIONS_OFFSET                     224L
#define M_SUB_EDGES_STRENGTH                     227L
#define M_SUB_EDGES_CONTRAST                     228L
#define M_SUB_EDGES_FIT_ERROR                    229L
#define M_SUB_EDGES_CONTRAST2                    230L

#define M_SUB_REGIONS_SIZE                       225L
#define M_SUB_REGIONS_CHORD_ANGLE                226L                      // New in PP2
#define M_MAX_ASSOCIATION_DISTANCE              1503L

#define M_CONTROL_FLAG                    0x00019400L
#define M_ZERO_OFFSET_X                            1L
#define M_ZERO_OFFSET_Y                            2L
#define M_PIXEL_ASPECT_RATIO                       5L
#define M_PIXEL_ASPECT_RATIO_INPUT                 6L
#define M_PIXEL_ASPECT_RATIO_OUTPUT                7L
#define M_DRAW_RELATIVE_ORIGIN_X                   319L        // deprecated : Use MgraControl(... M_DRAW_OFFSET_X...)
#define M_DRAW_RELATIVE_ORIGIN_Y                   320L        // deprecated : Use MgraControl(... M_DRAW_OFFSET_Y...)
#define M_DRAW_SCALE_X                             3203L       // deprecated : Use MgraControl(... M_DRAW_ZOOM_X ...)
#define M_DRAW_SCALE_Y                             3204L       // deprecated : Use MgraControl(... M_DRAW_ZOOM_Y ...)

#define M_RESULT_TYPE                              1L

#define M_DISTANCE                        0x00080000L       // Also defined in milim.h
#define M_DISTANCE_X                      0x00081000L
#define M_DISTANCE_Y                      0x00082000L
#define M_ANGLE                           0x00000800L
#define M_LINE_EQUATION                   0x00800000L
#define M_LINE_EQUATION_SLOPE             0x00801000L
#define M_LINE_EQUATION_INTERCEPT         0x00802000L

#define M_LINE_END_POINT_FIRST            0x00000010L
#define M_LINE_END_POINT_SECOND           0x00000020L

#define M_SEARCH_REGION_CLIPPING               0x00809000L
#define M_SEARCH_REGION_CLIPPING_MIN_AREA      1690L
#define M_SEARCH_REGION_CLIPPING_MIN_WIDTH     1691L
#define M_SEARCH_REGION_CLIPPING_MIN_HEIGHT    1692L
#define M_SEARCH_REGION_CLIPPING_PRESERVE_CENTER 1693L

#define M_CIRCLE_ACCURACY                 0x00030400L
#define M_CIRCLE_INSIDE_SEARCH_REGION           1718L

/* Score flags */
#define M_MAX_POSSIBLE_VALUE              0x10000001L
#define M_STRENGTH_SCORE                  0x10100000L
#define M_RADIUS_SCORE                    0x10200000L
#define M_EDGEVALUE_PEAK_CONTRAST_SCORE   0x10300000L
#define M_EDGE_CONTRAST_SCORE             0x10400000L
#define M_POSITION_IN_MARKER_SCORE        0x10600000L
#define M_SPACING_SCORE                   0x10700000L
#define M_STRIPE_WIDTH_SCORE              0x10800000L
#define M_EDGE_INSIDE_SCORE               0x10900000L

#define M_SCORE_BACKWARD_COMPATIBILITY    0x00031400L

/**************************************************************************/
/* MmeasInquire(), MmeasSetMarker(), MmeasGetResult(), MmeasFindMarker(), */
/* MmeasCalculate() values :                                              */
/**************************************************************************/

#define M_VERTICAL                      1L
#define M_HORIZONTAL                    2L
#define M_ANY                  0x11000000L
#define M_POSITIVE                      2L
#define M_NEGATIVE                     -2L /*Must be the additive inverse of M_POSITIVE*/
#define M_OPPOSITE                      3L
#define M_SAME                         -1L /* For spacing */
#define M_NORMAL                        8L
#define M_CORRECTED                     2L
#define M_DISABLE                   -9999L
#define M_ENABLE                    -9997L
#define M_NEAREST_NEIGHBOR     0x00000040L
#define M_BILINEAR             0x00000008L
#define M_BICUBIC              0x00000010L
#define M_YES                           1L
#define M_NO                            0L
#define M_TRUE                          1L
#define M_FALSE                         0L
#define M_INVALID                      -1
#define M_FILTER_TYPE                 1046L          // Already defined in mil.h, miledge.h
#define M_SMOOTHNESS                  108L           // Already defined in milmod.h
#define M_FILTER_SMOOTHNESS           M_SMOOTHNESS   // Already defined in miledge.h
#define M_SHEN                        0x802L
#define M_PREWITT                     M_EDGE_DETECT2 // Already defined in mil.h
#define M_EULER                       0x806L
#define M_SUBPIXEL_MODE               1715L
#define M_LOCAL                       0x01000000     // Already defined in mil.h, milstr.h
#define M_GLOBAL                      0x00000008L    // Already defined in mil.h, milreg.h

#define M_MAXIMIZE_WIDTH              1694L
#define M_MAXIMIZE_HEIGHT             1695L
#define M_MAXIMIZE_AREA               M_ENABLE
#define M_ALONG_WIDTH                 1697L
#define M_ALONG_HEIGHT                1698L
#define M_NONE                        0x08000000L // Also defined in milcal.h, milcolor.h
#define M_AUTO                        444L        // Also defined in miledge.h, milmetrol.h, milmod.h, milocr.h

// Parameters for output units
#define M_PIXEL                              0x1000L
#define M_WORLD                              0x2000L
#define M_ACCORDING_TO_CALIBRATION             1301L
#define M_ACCORDING_TO_RESULT                  1302L

/**************************************************************************/
/* MmeasDraw()                                                            */
/**************************************************************************/

// Operation:
#define M_DRAW_SEARCH_REGION_CENTER             0x00000001L
#define M_DRAW_BOX_CENTER                       0x00000001L             // Deprecated in MIL 9.0 PP2 (also defined in milblob)
#define M_DRAW_LINE                             0x00000002L             
#define M_DRAW_EDGES                            0x00000004L             
#define M_DRAW_WIDTH                            0x00000008L             
#define M_DRAW_POSITION_VARIATION               0x00000010L             // Deprecated in MIL 9.0 PP1
#define M_DRAW_SEARCH_REGION                    0x00000020L             
#define M_DRAW_BOX                              0x00000020L             // Deprecated in MIL 9.0 PP2 (also defined in milblob), renamed to M_DRAW_SEARCH_REGION
#define M_DRAW_POSITION                         0x00000040L             // Also used by Mblobdraw and MpatDraw and Mmoddraw 
#define M_DRAW_EDGES_PROFILE                    0x00000080L             
#define M_DRAW_SPACING                          0x00000100L             
#define M_DRAW_WIDTH_VARIATION                  0x00000200L             // Deprecated in MIL 9.0 PP1
#define M_DRAW_SUB_POSITIONS                    0x00000400L       
#define M_DRAW_SEARCH_DIRECTION                 0X00000800L       
#define M_DRAW_INCLUSION_POINT                  0x00004000L
#define M_DRAW_EDGEVALUE_MIN_IN_PROFILE         0x00008000L             
#define M_DRAW_EDGEVALUE_PEAK_WIDTH_IN_PROFILE  0x00010000L             
#define M_DRAW_POSITION_IN_PROFILE              0x00020000L             
#define M_DRAW_IN_BOX                           0x00100000L             // Operation modifier
                                                                  
// ControlFlag:                                                   
#define M_RESULT                          0x00001000L             // Also used by MmodDraw 
#define M_MARKER                          0x000001F8L             
                                                                  
// Index:                                                         
#define M_SUB_REGION_TAG            0x20000000L                   

#define M_MEAS_DRAW_INDEX_BIT_SHIFT 14
#define M_RESULT_ALL_OCCURRENCES    M_ALL
#define M_ALL_SUBREGIONS            0x00003FFFL
#define M_ALL_OCCURRENCES           (M_RESULT_ALL_OCCURRENCES|M_ALL_SUBREGIONS)


#define M_RESULT_PER_SUBREGION(Subregion, Occurence) \
   (M_SUB_REGION_TAG | Subregion << M_MEAS_DRAW_INDEX_BIT_SHIFT | Occurence)

/**************************************************************************/
/* MmeasRestoreMarker(), MmeasSaveMarker                                  */
/**************************************************************************/
#define M_INTERACTIVE                      M_NULL // Already defined in mil.h, milcal.h, milcode.h, miledge.h, milocr.h, milpat.h, milmod.h


/**************************************************************************/
/* Utility defines                                                        */
/**************************************************************************/
#define M_INFINITE_SLOPE      MIL_INT32_MAX

// Deprecated functionalities; use MmeasSetScore() instead.
#if OldDefinesSupport
   #define M_WEIGHT_FACTOR                   0x01000000L  // Deprecated, use MmeasSetScore() instead
   MIL_DEPRECATED(M_WEIGHT_FACTOR, 1000)                        
   #define M_POSITION_VARIATION              0x00008000L  // Deprecated, use MmeasSetScore() instead
   MIL_DEPRECATED(M_POSITION_VARIATION, 1000)                   
   #define M_WIDTH_VARIATION                 0x00020000L  // Deprecated, use MmeasSetScore() instead
   MIL_DEPRECATED(M_WIDTH_VARIATION, 1000)
   #define M_SPACING_VARIATION               0x00000100L  // Deprecated, use MmeasSetScore() instead
   MIL_DEPRECATED(M_SPACING_VARIATION, 1000)
   #define M_CONTRAST_VARIATION              0x00005400L  // Deprecated, use MmeasSetScore() instead
   MIL_DEPRECATED(M_CONTRAST_VARIATION, 1000)
   #define M_EDGE_STRENGTH_VARIATION         0x00007400L  // Deprecated, use MmeasSetScore() instead
   MIL_DEPRECATED(M_EDGE_STRENGTH_VARIATION, 1000)
   #define M_EDGE_INSIDE_VARIATION           0x00008400L  // Deprecated, use MmeasSetScore() instead
   MIL_DEPRECATED(M_EDGE_INSIDE_VARIATION, 1000)
#endif

// Deprecated flag names.
#if OldDefinesSupport
   #define        M_DRAW_ARROW M_DRAW_SEARCH_DIRECTION
   MIL_DEPRECATED(M_DRAW_ARROW, 1000)
   // #define        M_FILTER M_FILTER_TYPE already defined in mil.h
   // MIL_DEPRECATED(M_FILTER, 1000)        already defined in mil.h
   #define        M_WIDTH_SCORE M_STRIPE_WIDTH_SCORE
   MIL_DEPRECATED(M_WIDTH_SCORE, 1000)
   #define        M_CONTRAST_SCORE M_EDGEVALUE_PEAK_CONTRAST_SCORE
   MIL_DEPRECATED(M_CONTRAST_SCORE, 1000)
   #define        M_SUB_EDGES_INDEX M_SUB_EDGES_MARKER_INDEX
   MIL_DEPRECATED(M_SUB_EDGES_INDEX, 1000)
   #define        M_MAJOR_AXIS_MAX M_RADIUS_MAX
   MIL_DEPRECATED(M_MAJOR_AXIS_MAX, 1000)
   #define        M_MAJOR_AXIS_MIN M_RADIUS_MIN
   MIL_DEPRECATED(M_MAJOR_AXIS_MIN, 1000)
   #define        M_MAJOR_AXIS M_RADIUS
   MIL_DEPRECATED(M_MAJOR_AXIS, 1000)
   #define        M_POSITION_MAX M_EDGEVALUE_PEAK_POS_MAX
   MIL_DEPRECATED(M_POSITION_MAX, 1000)
   #define        M_POSITION_MIN M_EDGEVALUE_PEAK_POS_MIN
   MIL_DEPRECATED(M_POSITION_MIN, 1000)
   #define        M_EDGE_THRESHOLD M_EDGEVALUE_MIN
   MIL_DEPRECATED(M_EDGE_THRESHOLD, 1000)
   #define        M_SECOND_END_POINT_LINE M_LINE_END_POINT_SECOND
   MIL_DEPRECATED(M_SECOND_END_POINT_LINE, 1000)
   #define        M_FIRST_END_POINT_LINE M_LINE_END_POINT_FIRST
   MIL_DEPRECATED(M_FIRST_END_POINT_LINE, 1000)
   #define        M_BOX_CLIPPING M_SEARCH_REGION_CLIPPING
   MIL_DEPRECATED(M_BOX_CLIPPING, 1000)
   #define        M_SUB_BOX_MAXIMUM_ANGLE M_SUB_REGIONS_CHORD_ANGLE
   MIL_DEPRECATED(M_SUB_BOX_MAXIMUM_ANGLE, 1000)
   #define        M_BOX_WAS_CLIPPED M_SEARCH_REGION_WAS_CLIPPED
   MIL_DEPRECATED(M_BOX_WAS_CLIPPED, 1000)
   #define        M_BOX_RING_RADIUS M_RING_RADII
   MIL_DEPRECATED(M_BOX_RING_RADIUS, 1000)
   #define        M_BOX_INPUT_UNITS M_SEARCH_REGION_INPUT_UNITS
   MIL_DEPRECATED(M_BOX_INPUT_UNITS, 1000)
   #define        M_BOX_ANGLE_INTERPOLATION_MODE M_SEARCH_REGION_ANGLE_INTERPOLATION_MODE
   MIL_DEPRECATED(M_BOX_ANGLE_INTERPOLATION_MODE, 1000)
   #define        M_BOX_INTERPOLATION_MODE M_SEARCH_REGION_ANGLE_INTERPOLATION_MODE
   MIL_DEPRECATED(M_BOX_INTERPOLATION_MODE, 1000)
   #define        M_SUB_EDGES_OFFSET M_SUB_REGIONS_OFFSET
   MIL_DEPRECATED(M_SUB_EDGES_OFFSET, 1000)
   #define        M_SUB_EDGES_SIZE M_SUB_REGIONS_SIZE
   MIL_DEPRECATED(M_SUB_EDGES_SIZE, 1000)
   #define        M_SUB_EDGES_NUMBER M_SUB_REGIONS_NUMBER
   MIL_DEPRECATED(M_SUB_EDGES_NUMBER, 1000)
   #define        M_POSITION_INSIDE_STRIPE M_INCLUSION_POINT_INSIDE_STRIPE
   MIL_DEPRECATED(M_POSITION_INSIDE_STRIPE, 1000)
   #define        M_CONTRAST M_EDGEVALUE_PEAK_CONTRAST
   MIL_DEPRECATED(M_CONTRAST, 1000)
   #define        M_BOX_EDGES_STRENGTH_NUMBER M_BOX_EDGE_VALUES_NUMBER
   MIL_DEPRECATED(M_BOX_EDGES_STRENGTH_NUMBER, 1000)
   #define        M_BOX_EDGES_STRENGTH M_BOX_EDGE_VALUES
   MIL_DEPRECATED(M_BOX_EDGES_STRENGTH, 1000)
#endif

/**************************************************************************/
/* Function prototypes                                                    */
/**************************************************************************/

MIL_ID MFTYPE MmeasAllocMarker  (MIL_ID     SystemId,
                                 MIL_INT64  MarkerType,
                                 MIL_INT64  ControlFlag,
                                 MIL_ID*    MarkerIdPtr);

MIL_ID MFTYPE MmeasAllocResult  (MIL_ID     SystemId,
                                 MIL_INT64  ResultType,
                                 MIL_ID*    MeasResultIdPtr);

MIL_ID MFTYPE MmeasAllocContext (MIL_ID     SystemId,
                                 MIL_INT64  ControlFlag,
                                 MIL_ID*    ContextIdPtr);

void   MFTYPE MmeasFree         (MIL_ID MeasId);

void   MFTYPE MmeasSetMarker    (MIL_ID     MarkerId,
                                 MIL_INT64  CharacteristicToSet,
                                 MIL_DOUBLE FirstValue,
                                 MIL_DOUBLE SecondValue);

void MFTYPE MmeasSetScore       (MIL_ID     MarkerId,
                                 MIL_INT64  Characteristic,
                                 MIL_DOUBLE Min,
                                 MIL_DOUBLE Low,
                                 MIL_DOUBLE High,
                                 MIL_DOUBLE Max,
                                 MIL_DOUBLE ScoreOffset,
                                 MIL_INT64  InputUnits,
                                 MIL_INT64  ControlFlag);

void MFTYPE MmeasGetScore       (MIL_ID      MarkerId,
                                 MIL_INT64   Characteristic,
                                 MIL_INT     Index,
                                 MIL_DOUBLE* Param1Ptr,
                                 MIL_DOUBLE* Param2Ptr,
                                 MIL_DOUBLE* Param3Ptr,
                                 MIL_DOUBLE* Param4Ptr,
                                 MIL_DOUBLE* Param5Ptr,
                                 MIL_INT*    InputUnitsPtr,
                                 MIL_INT64   ControlFlag);

void MFTYPE MmeasGetScoreOutputUnits(MIL_ID      MarkerId,
                                     MIL_INT64   Characteristic,
                                     MIL_INT     Index,
                                     MIL_INT64   OutputUnits,
                                     MIL_DOUBLE* Param1,
                                     MIL_DOUBLE* Param2,
                                     MIL_DOUBLE* Param3,
                                     MIL_DOUBLE* Param4,
                                     MIL_DOUBLE* Param5,
                                     MIL_INT*    InputUnitsPtr,
                                     MIL_INT64   ControlFlag);


void   MFTYPE MmeasFindMarker   (MIL_ID     ContextId,
                                 MIL_ID     ImageBufId,
                                 MIL_ID     MarkerId,
                                 MIL_INT64  MeasurementList);


void   MFTYPE MmeasCalculate    (MIL_ID     ContextId,
                                 MIL_ID     Marker1Id,
                                 MIL_ID     Marker2Id,
                                 MIL_ID     MeasResultId,
                                 MIL_INT64  MeasurementList);

void   MFTYPE MmeasGetResult    (MIL_ID      MarkerOrMeasResultId,
                                 MIL_INT64   ResultType,
                                 void*       FirstResultArrayPtr,
                                 void*       SecondResultArrayPtr);

void   MFTYPE MmeasGetResultOutputUnits(MIL_ID      MarkerOrResultId,
                                        MIL_INT64   ResultType,
                                        MIL_INT64   OutputUnits,
                                        void*       FirstResultPtr,
                                        void*       SecondResultPtr);

void MFTYPE MmeasGetResultSingle(MIL_ID      MarkerOrMeasResultId,
                                 MIL_INT64   ResultType,
                                 void*       FirstResultArrayPtr,
                                 void*       SecondResultArrayPtr,
                                 MIL_INT     ResultIndex);

void MFTYPE MmeasGetResultSingleOutputUnits(MIL_ID      MarkerOrResultId,
                                            MIL_INT64   ResultType,
                                            MIL_INT64   OutputUnits,
                                            void*       FirstResultPtr,
                                            void*       SecondResultPtr,
                                            MIL_INT     Index);

MIL_INT MFTYPE MmeasInquire   (MIL_ID     MeasId,
                               MIL_INT64  InquireType,
                               void*      FirstUserVarPtr,
                               void*      SecondUserVarPtr);

#if M_MIL_USE_64BIT
// Prototypes for 64 bits OSs
void   MFTYPE MmeasControlInt64 (MIL_ID     ContextOrResultId,
                                 MIL_INT64  ControlType,
                                 MIL_INT64  ControlValue);

void   MFTYPE MmeasControlDouble(MIL_ID     ContextOrResultId,
                                 MIL_INT64  ControlType,
                                 MIL_DOUBLE ControlValue);
#else
// Prototypes for 32 bits OSs
#define MmeasControlInt64  MmeasControl
#define MmeasControlDouble MmeasControl
void   MFTYPE MmeasControl      (MIL_ID     ContextOrResultId,
                                 MIL_INT64  ControlType,
                                 MIL_DOUBLE ControlValue);
#endif

void MFTYPE MmeasDraw           (MIL_ID     ContextGraId,
                                 MIL_ID     MarkerOrResultMeasId,
                                 MIL_ID     DstImageBufOrListGraId,
                                 MIL_INT64  Operation,
                                 MIL_INT    Index,
                                 MIL_INT64  ControlFlag);

#if M_MIL_USE_UNICODE

void   MFTYPE MmeasSaveMarkerW  (MIL_CONST_TEXT_PTR FileName,
                                 MIL_ID             MarkerId,
                                 MIL_INT64          ControlFlag);

MIL_ID MFTYPE MmeasRestoreMarkerW(MIL_CONST_TEXT_PTR FileName,
                                  MIL_ID             SystemId,
                                  MIL_INT64          ControlFlag,
                                  MIL_ID*            MarkerIdPtr);

void MFTYPE MmeasStreamW        (MIL_TEXT_PTR MemPtrOrFileName,
                                 MIL_ID       SystemId,
                                 MIL_INT64    Operation,
                                 MIL_INT64    StreamType,
                                 MIL_DOUBLE   Version,
                                 MIL_INT64    ControlFlag,
                                 MIL_ID*      MarkerIdPtr,
                                 MIL_INT*     SizeByteVarPtr);

void   MFTYPE MmeasSaveMarkerA  (const char*  FileName,
                                 MIL_ID       MarkerId,
                                 MIL_INT64    ControlFlag);

MIL_ID MFTYPE MmeasRestoreMarkerA(const char* FileName,
                                  MIL_ID      SystemId,
                                  MIL_INT64   ControlFlag,
                                  MIL_ID*     MarkerIdPtr);

void MFTYPE MmeasStreamA        (char*        MemPtrOrFileName,
                                 MIL_ID       SystemId,
                                 MIL_INT64    Operation,
                                 MIL_INT64    StreamType,
                                 MIL_DOUBLE   Version,
                                 MIL_INT64    ControlFlag,
                                 MIL_ID*      MarkerIdPtr,
                                 MIL_INT*     SizeByteVarPtr);

#if M_MIL_UNICODE_API

#define MmeasSaveMarker    MmeasSaveMarkerW
#define MmeasRestoreMarker MmeasRestoreMarkerW
#define MmeasStream        MmeasStreamW

#else

#define MmeasSaveMarker    MmeasSaveMarkerA
#define MmeasRestoreMarker MmeasRestoreMarkerA
#define MmeasStream        MmeasStreamA

#endif

#else

void   MFTYPE MmeasSaveMarker   (MIL_CONST_TEXT_PTR FileName,
                                 MIL_ID             MarkerId,
                                 MIL_INT64          ControlFlag);

MIL_ID MFTYPE MmeasRestoreMarker(MIL_CONST_TEXT_PTR FileName,
                                 MIL_ID             SystemId,
                                 MIL_INT64          ControlFlag,
                                 MIL_ID*            MarkerIdPtr);

void MFTYPE MmeasStream         (MIL_TEXT_PTR MemPtrOrFileName,
                                 MIL_ID       SystemId,
                                 MIL_INT64    Operation,
                                 MIL_INT64    StreamType,
                                 MIL_DOUBLE   Version,
                                 MIL_INT64    ControlFlag,
                                 MIL_ID*      MarkerIdPtr,
                                 MIL_INT*     SizeByteVarPtr);
#endif // M_MIL_USE_UNICODE



/* C++ directive if needed */
#ifdef __cplusplus
}
#endif

#if M_MIL_USE_64BIT
#ifdef __cplusplus
//////////////////////////////////////////////////////////////
// MmeasControl function definition when compiling c++ files
//////////////////////////////////////////////////////////////
#if !M_MIL_USE_LINUX
inline void MmeasControl (MIL_ID     ContextOrResultId,
                          MIL_INT64  ControlType,
                          int        ControlValue)
   {
   MmeasControlInt64(ContextOrResultId, ControlType, ControlValue);
   };
#endif

inline void MmeasControl (MIL_ID     ContextOrResultId,
                          MIL_INT64  ControlType,
                          MIL_INT32  ControlValue)
   {
   MmeasControlInt64(ContextOrResultId, ControlType, ControlValue);
   };

inline void MmeasControl(MIL_ID     ContextOrResultId,
                         MIL_INT64  ControlType,
                         MIL_INT64  ControlValue)
      {
      MmeasControlInt64(ContextOrResultId, ControlType, ControlValue);
      };

inline void MmeasControl (MIL_ID     ContextOrResultId,
                          MIL_INT64  ControlType,
                          MIL_DOUBLE ControlValue)
         {
         MmeasControlDouble(ContextOrResultId, ControlType, ControlValue);
         };
#else
//////////////////////////////////////////////////////////////
// For C file, call the default function, i.e. Int64 one
//////////////////////////////////////////////////////////////
#define MmeasControl  MmeasControlDouble

#endif // __cplusplus
#endif // M_MIL_USE_64BIT

#if M_MIL_USE_SAFE_TYPE

//////////////////////////////////////////////////////////////
// See milos.h for explanation about these functions.
//////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------
// MmeasGetResult

inline void MFTYPE MmeasGetResultUnsafe  (MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, void           *FirstResultArrayPtr, void           *SecondResultArrayPtr);
inline void MFTYPE MmeasGetResultSafeType(MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, MIL_INT32      *FirstResultArrayPtr, int             SecondResultArrayPtr);
inline void MFTYPE MmeasGetResultSafeType(MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, MIL_INT64      *FirstResultArrayPtr, int             SecondResultArrayPtr);
inline void MFTYPE MmeasGetResultSafeType(MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, MIL_DOUBLE     *FirstResultArrayPtr, int             SecondResultArrayPtr);
inline void MFTYPE MmeasGetResultSafeType(MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, int             FirstResultArrayPtr, int             SecondResultArrayPtr);
inline void MFTYPE MmeasGetResultSafeType(MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, MIL_INT32      *FirstResultArrayPtr, MIL_INT32      *SecondResultArrayPtr);
inline void MFTYPE MmeasGetResultSafeType(MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, MIL_INT64      *FirstResultArrayPtr, MIL_INT64      *SecondResultArrayPtr);
inline void MFTYPE MmeasGetResultSafeType(MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, MIL_DOUBLE     *FirstResultArrayPtr, MIL_DOUBLE     *SecondResultArrayPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED                           
inline void MFTYPE MmeasGetResultSafeType(MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, MIL_UINT32     *FirstResultArrayPtr, int             SecondResultArrayPtr);
inline void MFTYPE MmeasGetResultSafeType(MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, MIL_UINT64     *FirstResultArrayPtr, int             SecondResultArrayPtr);
inline void MFTYPE MmeasGetResultSafeType(MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, MIL_UINT32     *FirstResultArrayPtr, MIL_UINT32     *SecondResultArrayPtr);
inline void MFTYPE MmeasGetResultSafeType(MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, MIL_UINT64     *FirstResultArrayPtr, MIL_UINT64     *SecondResultArrayPtr);
#endif

//-------------------------------------------------------------------------------------
// MmeasGetResultSingle

inline void MFTYPE MmeasGetResultSingleUnsafe  (MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, void           *FirstResultArrayPtr, void           *SecondResultArrayPtr, MIL_INT ResultIndex);
inline void MFTYPE MmeasGetResultSingleSafeType(MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, MIL_INT32      *FirstResultArrayPtr, int             SecondResultArrayPtr, MIL_INT ResultIndex);
inline void MFTYPE MmeasGetResultSingleSafeType(MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, MIL_INT64      *FirstResultArrayPtr, int             SecondResultArrayPtr, MIL_INT ResultIndex);
inline void MFTYPE MmeasGetResultSingleSafeType(MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, MIL_DOUBLE     *FirstResultArrayPtr, int             SecondResultArrayPtr, MIL_INT ResultIndex);
inline void MFTYPE MmeasGetResultSingleSafeType(MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, int             FirstResultArrayPtr, int             SecondResultArrayPtr, MIL_INT ResultIndex);
inline void MFTYPE MmeasGetResultSingleSafeType(MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, MIL_INT32      *FirstResultArrayPtr, MIL_INT32      *SecondResultArrayPtr, MIL_INT ResultIndex);
inline void MFTYPE MmeasGetResultSingleSafeType(MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, MIL_INT64      *FirstResultArrayPtr, MIL_INT64      *SecondResultArrayPtr, MIL_INT ResultIndex);
inline void MFTYPE MmeasGetResultSingleSafeType(MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, MIL_DOUBLE     *FirstResultArrayPtr, MIL_DOUBLE     *SecondResultArrayPtr, MIL_INT ResultIndex);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED                                 
inline void MFTYPE MmeasGetResultSingleSafeType(MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, MIL_UINT32     *FirstResultArrayPtr, int             SecondResultArrayPtr, MIL_INT ResultIndex);
inline void MFTYPE MmeasGetResultSingleSafeType(MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, MIL_UINT64     *FirstResultArrayPtr, int             SecondResultArrayPtr, MIL_INT ResultIndex);
inline void MFTYPE MmeasGetResultSingleSafeType(MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, MIL_UINT32     *FirstResultArrayPtr, MIL_UINT32     *SecondResultArrayPtr, MIL_INT ResultIndex);
inline void MFTYPE MmeasGetResultSingleSafeType(MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, MIL_UINT64     *FirstResultArrayPtr, MIL_UINT64     *SecondResultArrayPtr, MIL_INT ResultIndex);
#endif

//-------------------------------------------------------------------------------------
// MmeasInquire

inline MIL_INT MFTYPE MmeasInquireUnsafe  (MIL_ID MeasId, MIL_INT64  InquireType, void           *FirstUserArrayPtr, void           *SecondUserArrayPtr);
inline MIL_INT MFTYPE MmeasInquireUnsafe  (MIL_ID MeasId, MIL_INT64  InquireType, int             FirstUserArrayPtr, int             SecondUserArrayPtr);
inline MIL_INT MFTYPE MmeasInquireSafeType(MIL_ID MeasId, MIL_INT64  InquireType, MIL_INT32      *FirstUserArrayPtr, MIL_INT32      *SecondUserArrayPtr);
inline MIL_INT MFTYPE MmeasInquireSafeType(MIL_ID MeasId, MIL_INT64  InquireType, MIL_INT64      *FirstUserArrayPtr, MIL_INT64      *SecondUserArrayPtr);
inline MIL_INT MFTYPE MmeasInquireSafeType(MIL_ID MeasId, MIL_INT64  InquireType, MIL_DOUBLE     *FirstUserArrayPtr, MIL_DOUBLE     *SecondUserArrayPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED                      
inline MIL_INT MFTYPE MmeasInquireSafeType(MIL_ID MeasId, MIL_INT64  InquireType, MIL_UINT32     *FirstUserArrayPtr, MIL_UINT32     *SecondUserArrayPtr);
inline MIL_INT MFTYPE MmeasInquireSafeType(MIL_ID MeasId, MIL_INT64  InquireType, MIL_UINT64     *FirstUserArrayPtr, MIL_UINT64     *SecondUserArrayPtr);
#endif

// -------------------------------------------------------------------------
// MmeasGetResult

inline void MFTYPE MmeasGetResultSafeType  (MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, int FirstResultArrayPtr, int SecondResultArrayPtr)
   {
   if(FirstResultArrayPtr || SecondResultArrayPtr)
      SafeTypeError(MIL_TEXT("MmeasGetResult"));

   MmeasGetResult(MarkerOrMeasResultId, ResultType, NULL, NULL);
   }

inline void MmeasGetResultSafeTypeExecute (MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, void* FirstResultArrayPtr, int SecondResultArrayPtr, MIL_INT64  FirstGivenType)
   {
   MIL_INT64  RequiredType = (ResultType & M_HLVLDATATYPE_MASK);
   if((RequiredType != M_TYPE_DOUBLE) && (RequiredType != M_TYPE_MIL_INT32) && (RequiredType != M_TYPE_MIL_INT64))
      RequiredType = 0;
   if (RequiredType == 0)
      RequiredType = M_TYPE_DOUBLE;

   if(RequiredType != FirstGivenType || SecondResultArrayPtr)
      SafeTypeError(MIL_TEXT("MmeasGetResult"));

   MmeasGetResult(MarkerOrMeasResultId, ResultType, FirstResultArrayPtr, NULL);
   }

inline void MmeasGetResultSafeTypeExecute (MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, void* FirstResultArrayPtr, void* SecondResultArrayPtr, MIL_INT64  FirstGivenType, MIL_INT64  SecondGivenType)
   {
   MIL_INT64  RequiredType = (ResultType & M_HLVLDATATYPE_MASK);
   if((RequiredType != M_TYPE_DOUBLE) && (RequiredType != M_TYPE_MIL_INT32) && (RequiredType != M_TYPE_MIL_INT64))
      RequiredType = 0;
   if (RequiredType == 0)
      RequiredType = M_TYPE_DOUBLE;

   if (RequiredType != FirstGivenType || RequiredType != SecondGivenType )
      SafeTypeError(MIL_TEXT("MmeasGetResult"));

   MmeasGetResult(MarkerOrMeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr);
   }

inline void MFTYPE MmeasGetResultUnsafe  (MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, void        *FirstResultArrayPtr, void        *SecondResultArrayPtr){MmeasGetResult               (MarkerOrMeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr);}

inline void MFTYPE MmeasGetResultSafeType(MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, MIL_INT32   *FirstResultArrayPtr, int SecondResultArrayPtr         ){MmeasGetResultSafeTypeExecute(MarkerOrMeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, M_TYPE_MIL_INT32 );}
inline void MFTYPE MmeasGetResultSafeType(MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, MIL_INT64   *FirstResultArrayPtr, int SecondResultArrayPtr         ){MmeasGetResultSafeTypeExecute(MarkerOrMeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, M_TYPE_MIL_INT64 );}
inline void MFTYPE MmeasGetResultSafeType(MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, MIL_DOUBLE  *FirstResultArrayPtr, int SecondResultArrayPtr         ){MmeasGetResultSafeTypeExecute(MarkerOrMeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, M_TYPE_DOUBLE    );}

inline void MFTYPE MmeasGetResultSafeType(MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, MIL_INT32   *FirstResultArrayPtr, MIL_INT32   *SecondResultArrayPtr){MmeasGetResultSafeTypeExecute(MarkerOrMeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, M_TYPE_MIL_INT32, M_TYPE_MIL_INT32 );}
inline void MFTYPE MmeasGetResultSafeType(MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, MIL_INT64   *FirstResultArrayPtr, MIL_INT64   *SecondResultArrayPtr){MmeasGetResultSafeTypeExecute(MarkerOrMeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, M_TYPE_MIL_INT64, M_TYPE_MIL_INT64 );}
inline void MFTYPE MmeasGetResultSafeType(MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, MIL_DOUBLE  *FirstResultArrayPtr, MIL_DOUBLE  *SecondResultArrayPtr){MmeasGetResultSafeTypeExecute(MarkerOrMeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, M_TYPE_DOUBLE   , M_TYPE_DOUBLE    );}

#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline void MFTYPE MmeasGetResultSafeType(MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, MIL_UINT32  *FirstResultArrayPtr, int SecondResultArrayPtr         ){MmeasGetResultSafeTypeExecute(MarkerOrMeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, M_TYPE_MIL_INT32 );}
inline void MFTYPE MmeasGetResultSafeType(MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, MIL_UINT64  *FirstResultArrayPtr, int SecondResultArrayPtr         ){MmeasGetResultSafeTypeExecute(MarkerOrMeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, M_TYPE_MIL_INT64 );}

inline void MFTYPE MmeasGetResultSafeType(MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, MIL_UINT32  *FirstResultArrayPtr, MIL_UINT32  *SecondResultArrayPtr){MmeasGetResultSafeTypeExecute(MarkerOrMeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, M_TYPE_MIL_INT32, M_TYPE_MIL_INT32 );}
inline void MFTYPE MmeasGetResultSafeType(MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, MIL_UINT64  *FirstResultArrayPtr, MIL_UINT64  *SecondResultArrayPtr){MmeasGetResultSafeTypeExecute(MarkerOrMeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, M_TYPE_MIL_INT64, M_TYPE_MIL_INT64 );}
#endif

// -------------------------------------------------------------------------
// MmeasGetResultSingle

inline void MFTYPE MmeasGetResultSingleSafeType  (MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, int FirstResultArrayPtr, int SecondResultArrayPtr, MIL_INT ResultIndex)
   {
   if(FirstResultArrayPtr || SecondResultArrayPtr)
      SafeTypeError(MIL_TEXT("MmeasGetResultSingle"));

   MmeasGetResultSingle(MarkerOrMeasResultId, ResultType, NULL, NULL, ResultIndex);
   }

inline void MmeasGetResultSingleSafeTypeExecute (MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, void* FirstResultArrayPtr, int SecondResultArrayPtr, MIL_INT ResultIndex, MIL_INT64  FirstGivenType)
   {
   MIL_INT64  RequiredType = (ResultType & M_HLVLDATATYPE_MASK);
   if((RequiredType != M_TYPE_DOUBLE) && (RequiredType != M_TYPE_MIL_INT32) && (RequiredType != M_TYPE_MIL_INT64))
      RequiredType = 0;
   if (RequiredType == 0)
      RequiredType = M_TYPE_DOUBLE;

   if(RequiredType != FirstGivenType || SecondResultArrayPtr)
      SafeTypeError(MIL_TEXT("MmeasGetResult"));  

   MmeasGetResultSingle(MarkerOrMeasResultId, ResultType, FirstResultArrayPtr, NULL, ResultIndex);
   }

inline void MmeasGetResultSingleSafeTypeExecute (MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, void* FirstResultArrayPtr, void* SecondResultArrayPtr, MIL_INT ResultIndex, MIL_INT64  FirstGivenType, MIL_INT64  SecondGivenType)
   {
   MIL_INT64  RequiredType = (ResultType & M_HLVLDATATYPE_MASK);
   if((RequiredType != M_TYPE_DOUBLE) && (RequiredType != M_TYPE_MIL_INT32) && (RequiredType != M_TYPE_MIL_INT64))
      RequiredType = 0;
   if (RequiredType == 0)
      RequiredType = M_TYPE_DOUBLE;

   if (RequiredType != FirstGivenType || RequiredType != SecondGivenType )
      SafeTypeError(MIL_TEXT("MmeasGetResult"));

   MmeasGetResultSingle(MarkerOrMeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, ResultIndex);
   }


inline void MFTYPE MmeasGetResultSingleUnsafe  (MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, void* FirstResultArrayPtr, void* SecondResultArrayPtr, MIL_INT ResultIndex){MmeasGetResultSingle                             (MarkerOrMeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, ResultIndex);}

inline void MFTYPE MmeasGetResultSingleSafeType(MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, MIL_INT32   *FirstResultArrayPtr, int          SecondResultArrayPtr, MIL_INT ResultIndex){MmeasGetResultSingleSafeTypeExecute(MarkerOrMeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, ResultIndex, M_TYPE_MIL_INT32);}
inline void MFTYPE MmeasGetResultSingleSafeType(MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, MIL_INT64   *FirstResultArrayPtr, int          SecondResultArrayPtr, MIL_INT ResultIndex){MmeasGetResultSingleSafeTypeExecute(MarkerOrMeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, ResultIndex, M_TYPE_MIL_INT64);}
inline void MFTYPE MmeasGetResultSingleSafeType(MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, MIL_DOUBLE  *FirstResultArrayPtr, int          SecondResultArrayPtr, MIL_INT ResultIndex){MmeasGetResultSingleSafeTypeExecute(MarkerOrMeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, ResultIndex, M_TYPE_DOUBLE   );}

inline void MFTYPE MmeasGetResultSingleSafeType(MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, MIL_INT32   *FirstResultArrayPtr, MIL_INT32   *SecondResultArrayPtr, MIL_INT ResultIndex){MmeasGetResultSingleSafeTypeExecute(MarkerOrMeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, ResultIndex, M_TYPE_MIL_INT32, M_TYPE_MIL_INT32);}
inline void MFTYPE MmeasGetResultSingleSafeType(MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, MIL_INT64   *FirstResultArrayPtr, MIL_INT64   *SecondResultArrayPtr, MIL_INT ResultIndex){MmeasGetResultSingleSafeTypeExecute(MarkerOrMeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, ResultIndex, M_TYPE_MIL_INT64, M_TYPE_MIL_INT64);}
inline void MFTYPE MmeasGetResultSingleSafeType(MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, MIL_DOUBLE  *FirstResultArrayPtr, MIL_DOUBLE  *SecondResultArrayPtr, MIL_INT ResultIndex){MmeasGetResultSingleSafeTypeExecute(MarkerOrMeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, ResultIndex, M_TYPE_DOUBLE   , M_TYPE_DOUBLE   );}

#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline void MFTYPE MmeasGetResultSingleSafeType(MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, MIL_UINT32  *FirstResultArrayPtr, int          SecondResultArrayPtr, MIL_INT ResultIndex){MmeasGetResultSingleSafeTypeExecute(MarkerOrMeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, ResultIndex, M_TYPE_MIL_INT32);}
inline void MFTYPE MmeasGetResultSingleSafeType(MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, MIL_UINT64  *FirstResultArrayPtr, int          SecondResultArrayPtr, MIL_INT ResultIndex){MmeasGetResultSingleSafeTypeExecute(MarkerOrMeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, ResultIndex, M_TYPE_MIL_INT64);}

inline void MFTYPE MmeasGetResultSingleSafeType(MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, MIL_UINT32  *FirstResultArrayPtr, MIL_UINT32  *SecondResultArrayPtr, MIL_INT ResultIndex){MmeasGetResultSingleSafeTypeExecute(MarkerOrMeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, ResultIndex, M_TYPE_MIL_INT32, M_TYPE_MIL_INT32);}
inline void MFTYPE MmeasGetResultSingleSafeType(MIL_ID MarkerOrMeasResultId, MIL_INT64  ResultType, MIL_UINT64  *FirstResultArrayPtr, MIL_UINT64  *SecondResultArrayPtr, MIL_INT ResultIndex){MmeasGetResultSingleSafeTypeExecute(MarkerOrMeasResultId, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, ResultIndex, M_TYPE_MIL_INT64, M_TYPE_MIL_INT64);}
#endif

// -------------------------------------------------------------------------
// MmeasInquire

inline MIL_INT MFTYPE MmeasInquireSafeType  (MIL_ID MeasId, MIL_INT64  InquireType, int FirstUserArrayPtr, int SecondUserArrayPtr)
   {
   if(FirstUserArrayPtr || SecondUserArrayPtr)
      SafeTypeError(MIL_TEXT("MmeasInquire"));

   return MmeasInquire(MeasId, InquireType, NULL, NULL );
   }

inline MIL_INT MFTYPE MmeasInquireSafeTypeExecute (MIL_ID MeasId, MIL_INT64  InquireType, void* FirstUserArrayPtr, void* SecondUserArrayPtr, MIL_INT64  GivenType)
   {
   MIL_INT64  RequiredType = (InquireType & M_HLVLDATATYPE_MASK);
   if((RequiredType != M_TYPE_DOUBLE) && (RequiredType != M_TYPE_MIL_INT32) && (RequiredType != M_TYPE_MIL_INT64))
      RequiredType = 0;
   if (RequiredType == 0)
      RequiredType = M_TYPE_DOUBLE;

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MmeasInquire"));

   return MmeasInquire(MeasId, InquireType, FirstUserArrayPtr, SecondUserArrayPtr);
   }

inline MIL_INT MFTYPE MmeasInquireUnsafe  (MIL_ID MarkerIdOrResultIdOrContextId, MIL_INT64  InquireType, void        *FirstUserArrayPtr, void        *SecondUserArrayPtr){return MmeasInquire               (MarkerIdOrResultIdOrContextId, InquireType, FirstUserArrayPtr, SecondUserArrayPtr);}
inline MIL_INT MFTYPE MmeasInquireSafeType(MIL_ID MarkerIdOrResultIdOrContextId, MIL_INT64  InquireType, MIL_INT32   *FirstUserArrayPtr, MIL_INT32   *SecondUserArrayPtr){return MmeasInquireSafeTypeExecute(MarkerIdOrResultIdOrContextId, InquireType, FirstUserArrayPtr, SecondUserArrayPtr, M_TYPE_MIL_INT32);}
inline MIL_INT MFTYPE MmeasInquireSafeType(MIL_ID MarkerIdOrResultIdOrContextId, MIL_INT64  InquireType, MIL_INT64   *FirstUserArrayPtr, MIL_INT64   *SecondUserArrayPtr){return MmeasInquireSafeTypeExecute(MarkerIdOrResultIdOrContextId, InquireType, FirstUserArrayPtr, SecondUserArrayPtr, M_TYPE_MIL_INT64);}
inline MIL_INT MFTYPE MmeasInquireSafeType(MIL_ID MarkerIdOrResultIdOrContextId, MIL_INT64  InquireType, MIL_DOUBLE  *FirstUserArrayPtr, MIL_DOUBLE  *SecondUserArrayPtr){return MmeasInquireSafeTypeExecute(MarkerIdOrResultIdOrContextId, InquireType, FirstUserArrayPtr, SecondUserArrayPtr, M_TYPE_DOUBLE);}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED                      
inline MIL_INT MFTYPE MmeasInquireSafeType(MIL_ID MarkerIdOrResultIdOrContextId, MIL_INT64  InquireType, MIL_UINT32  *FirstUserArrayPtr, MIL_UINT32  *SecondUserArrayPtr){return MmeasInquireSafeTypeExecute(MarkerIdOrResultIdOrContextId, InquireType, FirstUserArrayPtr, SecondUserArrayPtr, M_TYPE_MIL_INT32);}
inline MIL_INT MFTYPE MmeasInquireSafeType(MIL_ID MarkerIdOrResultIdOrContextId, MIL_INT64  InquireType, MIL_UINT64  *FirstUserArrayPtr, MIL_UINT64  *SecondUserArrayPtr){return MmeasInquireSafeTypeExecute(MarkerIdOrResultIdOrContextId, InquireType, FirstUserArrayPtr, SecondUserArrayPtr, M_TYPE_MIL_INT64);}
#endif

#define MmeasGetResult       MmeasGetResultSafeType
#define MmeasGetResultSingle MmeasGetResultSingleSafeType
#define MmeasInquire         MmeasInquireSafeType

#else // #if M_MIL_USE_SAFE_TYPE

#define MmeasGetResultUnsafe       MmeasGetResult
#define MmeasGetResultSingleUnsafe MmeasGetResultSingle
#define MmeasInquireUnsafe         MmeasInquire

#endif // #if M_MIL_USE_SAFE_TYPE

#endif // !M_MIL_LITE

#endif /* __MILMEAS_H__ */
