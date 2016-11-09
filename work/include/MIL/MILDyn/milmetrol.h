/***************************************************************************/
/*

   Filename:  MilMetrol.h
   Owner   :  Matrox Imaging dept.
   Revision:  10.00.2564
   Content :  This file contains the defines and the prototypes for the
   MIL metrology finder. (Mmet...)

   Copyright © Matrox Electronic Systems Ltd., 1992-2014.
   All Rights Reserved

*/
/***************************************************************************/

#ifndef __MILMETROLFINDER_H__
#define __MILMETROLFINDER_H__

#if (!M_MIL_LITE) // MIL FULL ONLY

/* C++ directive if needed */
#ifdef __cplusplus
extern "C"
   {
#endif


/***************************************************************************/
/*                       MilMet CAPI defines                               */
/***************************************************************************/


/***************************************************************************/
/* MmetControl()                                                           */
/***************************************************************************/
#define M_ALL_FEATURES                       0x100L                  // Already defined in    milblob.h, miledge.h
#define M_MEASURED_FEATURES                  0x102L
#define M_CONSTRUCTED_FEATURES               0x103L
#define M_ALL_TOLERANCES                     0x200L
#define M_ALL_PASS_TOLERANCES                0x202L
#define M_ALL_WARNING_TOLERANCES             0x204L
#define M_ALL_FAIL_TOLERANCES                0x206L
#define M_OUTPUT_FRAME                       0x104L
#define M_GENERAL                            0x20000000L             // Already defined in milocr.h, milmod.h, milstr.h, milreg.h
#define M_STATISTICAL                        3L                      // Already defined in mil.h
#define M_STATISTICS                         M_STATISTICAL

// Operations' parameters
#define M_FIT_DISTANCE_MAX                   101L
#define M_METHOD                             103L                    // Already defined in milocr.h
#define M_OPERATION                          M_METHOD
#define M_FIT_ITERATIONS_MAX                 104L
#define M_OCCURRENCE                         105L
#define M_INDEX_VALUE                        M_INDEX
#define M_ANGLE_VALUE                        M_ANGLE
#define M_CONSTRUCTION_DATA                  107L                    // (deprecated)
#define M_EDGEL_TYPE                         M_CONSTRUCTION_DATA
#define M_POSITION                           0x00000402L             // Already defined in milmeas.h, miledge.h
#define M_POSITION_VALUE                     M_POSITION
#define M_DATA_ANGLE_TOLERANCE               110L                    // (deprecated)
#define M_DATA_ANGLE_RANGE                   M_DATA_ANGLE_TOLERANCE  // (deprected)
#define M_DATA_ANGLE_POLARITY                111L                    // (deprecated)
#define M_EDGEL_ANGLE_RANGE                  M_DATA_ANGLE_RANGE
#define M_EDGEL_RELATIVE_ANGLE               M_DATA_ANGLE_POLARITY
#define M_FIT_COVERAGE_MIN                   112L
#define M_FIT_VARIATION_MAX                  113L
#define M_SECTOR_DIVIDE_VALUE                114L
#define M_COPY_OFFSET_X                      115L                    // (deprecated)
#define M_CLONE_OFFSET_X                     M_COPY_OFFSET_X
#define M_COPY_OFFSET_Y                      116L                    // (deprecated)
#define M_CLONE_OFFSET_Y                     M_COPY_OFFSET_Y
#define M_COPY_SCALE                         117L                    // (deprecated)
#define M_CLONE_SCALE                        117L
#define M_COPY_ANGLE                         118L                    // (deprecated)
#define M_CLONE_ANGLE                        M_COPY_ANGLE
#define M_DATA_ALL                           0L                      // (deprecated)
#define M_DATA_FITTED                        2L                      // (deprecated)
#define M_ALL_EDGELS                         M_DATA_ALL
#define M_ACTIVE_EDGELS                      1719L
#define M_DATA_FIT                           M_ACTIVE_EDGELS
#define M_FITTED_EDGELS                      M_DATA_FITTED
#define M_VISIBLE                            1533L
#define M_DRAWABLE                           M_VISIBLE               // (deprecated)

// result controls
#define M_RESULT_OUTPUT_UNITS                1300L // Also in milim.h

// extraction parameters
#define M_SMOOTHNESS                         108L                    // Already defined in    milmod.h, milmeas.h, mil.h, miledge.h
#define M_FILTER_SMOOTHNESS                  M_SMOOTHNESS            // Already defined in    mil.h, milmeas.h, miledge.h
#define M_THRESHOLD_MODE                     33L                     // Already defined in    miledge.h
#define M_FLOAT_MODE                         70L                     // Already defined in    miledge.h
#define M_FILTER_TYPE                        1046L                   // Already defined in    mil.h, miledge.h
#define M_CONTEXT_TYPE                       M_OBJECT_TYPE           // Already defined in    mil.h, milmod.h, milstr.h, miledge.h
#define M_RESULT_TYPE                        1L                      // Already defined in    milmeas.h, milim.h
#define M_FILTER_MODE                        122L                    // Already defined in    mil.h, milmod.h, milmeas.h, miledge.h
#define M_KERNEL_WIDTH                       109L                    // Already defined in    milmod.h, miledge.h
#define M_KERNEL_DEPTH                       368L                    // Already defined in    milmod.h, miledge.h
#define M_MAGNITUDE_TYPE                     10L                     // Already defined in    miledge.h
#define M_CHAIN_ALL_NEIGHBORS                888L                    
#define M_THRESHOLD_VALUE_HIGH               3002L                   // Already defined in    miledge.h
#define M_THRESHOLD_VALUE_LOW                3001L                   // Already defined in    miledge.h
#define M_THRESHOLD_TYPE                     3000L                   // Already defined in    miledge.h
#define M_EXTRACTION_SCALE                   58L                     // Already defined in    miledge.h
#define M_EDGEL_SELECTION_RANK               119L

// extraction values
#define M_LOW                                1L                      // Already defined in    milcode.h, milmod.h, milocr, milpat.h, mil.h, miledge.h
#define M_MEDIUM                             2L                      // Already defined in    milcode.h, milmod.h, milocr, milpat.h, miledge.h
#define M_HIGH                               3L                      // Already defined in    milcode.h, milmod.h, milocr, milpat.h, mil.h, miledge.h
#define M_VERY_HIGH                          4L                      // Already defined in    milcode.h, milmod.h, milocr, milpat.h, miledge.h
#define M_USER_DEFINED                       21L                     // Already defined in    mil.h, milocr.h, miledge.h
#define M_SHEN                               0x802L                  // Already defined in    miledge.h
#define M_SOBEL                              M_EDGE_DETECT           // Already defined in    mil.h, miledge.h
#define M_PREWITT                            M_EDGE_DETECT2          // Already defined in    mil.h, miledge.h
#define M_FREI_CHEN                          M_ID_OFFSET_OF_DEFAULT_KERNEL + 9L
#define M_DERICHE                            M_ID_OFFSET_OF_DEFAULT_KERNEL + 10L
#define M_CONTOUR                            0x800L                  // Already defined in    milblob.h, miledge.h
#define M_CREST                              0x801L                  // Already defined in    miledge.h
#define M_KERNEL                             0x00400000L             // Already defined in    milmod.h, mil.h, miledge.h
#define M_RECURSIVE                          0x00040000              // Already defined in    mildev.h, miledge.h
#define M_AUTO                               444L                    // Already defined in    milocr.h, milmod.h, miledge.h
#define M_SQR_NORM                           4001L                   // Already defined in    miledge.h
#define M_NORM                               4000L                   // Already defined in    miledge.h
#define M_HYSTERESIS                         31L                     // Already defined in    miledge.h
#define M_NO_HYSTERESIS                      20L                     // Already defined in    miledge.h
#define M_FULL_HYSTERESIS                    25L                     // Already defined in    miledge.h
#define M_REGION_ACCURACY_HIGH               99L                     

// Model
#define M_TEMPLATE_REFERENCE_ID              50L
#define M_ASSOCIATED_CALIBRATION             125L                    // Already defined in    mil.h, milmod.h, milcal.h
#define M_RESTORED_CALIBRATION               1491L                   // Already defined in    milmod.h
#define M_TEMPLATE_REFERENCE_SIZE_X          55L
#define M_TEMPLATE_REFERENCE_SIZE_Y          56L
#define M_TEMPLATE_REFERENCE_SIZE_BAND       57L
#define M_TEMPLATE_REFERENCE_TYPE            M_OBJECT_TYPE           // Already defined in milmod.h

#define M_SORT                               0x00200000L             // Already defined in milocr.h
#define M_SORT_UP                            1L
#define M_SORT_DOWN                          2L

// Parameters for output units
#define M_PIXEL                              0x1000L
#define M_WORLD                              0x2000L
#define M_ACCORDING_TO_CALIBRATION             1301L
#define M_ACCORDING_TO_RESULT                  1302L

/***************************************************************************/
/* MmetInquire()                                                           */
/***************************************************************************/
// General
#define M_OWNER_SYSTEM                          1101L                // Already defined in    mil.h, milmod.h, miledge.h
#define M_MODIFICATION_COUNT                    5010L                // Already defined in    mil.h, milmod.h, miledge.h
#define M_NUMBER_OF_FEATURES                    1002L
#define M_NUMBER_OF_MEASURED_FEATURES           1003L
#define M_NUMBER_OF_CONSTRUCTED_FEATURES        1004L
#define M_NUMBER_OF_TOLERANCES                  1005L
#define M_NUMBER_MIN                            1010L                // Already defined in    milmeas.h
#define M_NUMBER_MAX                            1011L
#define M_NUMBER_OF_CONSTRUCTION_FEATURE_LABEL  1012L // deprecated MIL9PP2
#define M_NUMBER_OF_CONSTRUCTION_FEATURE_INDEX  1013L // deprecated MIL9PP2
#define M_BASE_FEATURES_ARRAY_SIZE              M_NUMBER_OF_CONSTRUCTION_FEATURE_LABEL  
#define M_BASE_SUBFEATURES_ARRAY_SIZE           M_NUMBER_OF_CONSTRUCTION_FEATURE_INDEX
#define M_DRAW_INDEX_OFFSET                     1014L
#define M_DEPENDENCIES                          1015L
#define M_VERIFY_IS_CONSTRUCTED_ON_FEATURE      1496L
#define M_FEATURE_TO_VERIFY_IS_CONSTRUCTED_ON   1497L
#define M_INVALID                               -1

// Feature
#define M_FEATURE_LABEL_VALUE                M_LABEL_VALUE           // Already defined in    milblob.h, miledge.h
#define M_FEATURE_TYPE                       MAKE_INT64(0x0000000000040000)
#define M_FEATURE_GEOMETRY                   903L
#define M_FEATURE_METHOD                     M_OPERATION
#define M_FEATURE_OPERATION                  M_OPERATION
#define M_CONSTRUCTION_FEATURE_LABEL_TAG     0x1000000L
#define M_CONSTRUCTION_FEATURE_INDEX_TAG     0x2000000L
#define M_CONSTRUCTION_FEATURE_LABEL(i)      (M_CONSTRUCTION_FEATURE_LABEL_TAG|(i)) // deprecated M9PP2
#define M_CONSTRUCTION_FEATURE_INDEX(i)      (M_CONSTRUCTION_FEATURE_INDEX_TAG|(i)) // deprecated M9PP2
#define M_BASE_FEATURE_LABEL(i)              (M_CONSTRUCTION_FEATURE_LABEL_TAG|(i))
#define M_BASE_FEATURE_INDEX(i)              (M_CONSTRUCTION_FEATURE_INDEX_TAG|(i))
#define M_BASE_SUBFEATURE_INDEX(i)           (M_CONSTRUCTION_FEATURE_INDEX_TAG|(i))
#define M_TRANSFORMATION_TYPE                3L                      // Already defined in milreg.h
#define M_TRANSFORMATION_PARAM1              4L
#define M_TRANSFORMATION_PARAM2              5L
#define M_TRANSFORMATION_PARAM3              6L
#define M_TRANSFORMATION_PARAM4              7L

// Tolerance
#define M_TOLERANCE_LABEL_VALUE              M_LABEL_VALUE           // Already defined in    milblob.h, miledge.h
#define M_TOLERANCE_TYPE                     902L

// Frames
#define M_REFERENCE_FRAME                    205L
#define M_GLOBAL_FRAME                       (M_FEATURE_LABEL_TAG|M_DEFAULT)

// Region
#define M_DERIVED_GEOMETRY_REGION            206L // allocation type
#define M_REGION_GEOMETRY                    206L
#define M_REGION_POSITION_X                  207L
#define M_REGION_POSITION_Y                  208L
#define M_REGION_WIDTH                       209L
#define M_REGION_HEIGHT                      210L
#define M_REGION_ANGLE                        46L                    // Already defined in    milbga.h
#define M_REGION_RADIUS_START                212L
#define M_REGION_RADIUS_END                  213L
#define M_REGION_ANGLE_START                 214L
#define M_REGION_ANGLE_END                   215L
#define M_REGION_START_X                     218L
#define M_REGION_START_Y                     219L
#define M_REGION_END_X                       220L
#define M_REGION_END_Y                       221L
#define M_REGION_RADIUS                      222L
#define M_RADIAL                             777L

#define M_REGION_POSITION_X_TYPE             1727L
#define M_REGION_POSITION_Y_TYPE             1728L
#define M_REGION_WIDTH_TYPE                  1729L
#define M_REGION_HEIGHT_TYPE                 1730L
#define M_REGION_ANGLE_TYPE                  1731L
#define M_REGION_RADIUS_START_TYPE           1732L
#define M_REGION_RADIUS_END_TYPE             1733L
#define M_REGION_ANGLE_START_TYPE            1734L
#define M_REGION_ANGLE_END_TYPE              1735L
#define M_REGION_START_X_TYPE                1736L
#define M_REGION_START_Y_TYPE                1737L
#define M_REGION_END_X_TYPE                  1738L
#define M_REGION_END_Y_TYPE                  1739L
#define M_REGION_RADIUS_TYPE                 1740L
#define M_RADIAL_TYPE                        1741L
#define M_REGION_POSITION                    1775L
#define M_REGION_POSITION_TYPE               1776L
#define M_REGION_START                       1777L
#define M_REGION_POSITION_START              M_REGION_START
#define M_REGION_END                         1778L
#define M_REGION_POSITION_END                M_REGION_END
#define M_REGION_START_TYPE                  1779L
#define M_REGION_POSITION_START_TYPE         M_REGION_START_TYPE
#define M_REGION_END_TYPE                    1780L
#define M_REGION_POSITION_END_TYPE           M_REGION_END_TYPE
#define M_REGION_POSITION_START_X            M_REGION_START_X
#define M_REGION_POSITION_END_X              M_REGION_END_X
#define M_REGION_POSITION_START_Y            M_REGION_START_Y
#define M_REGION_POSITION_END_Y              M_REGION_END_Y
#define M_FROM_REGION_GEOMETRY               1785L // deprecated
#define M_FROM_DERIVED_GEOMETRY_REGION       M_FROM_REGION_GEOMETRY

/***************************************************************************/
/* MmetGetResult()                                                         */
/***************************************************************************/
#define M_LABEL_VALUE                        1L                      // Already defined in    milblob.h, miledge.h
#define M_INDEX_FROM_LABEL                   229L                    // Already defined in    milmod.h

#define M_POSITION_X                         0x00003400L             // Already defined in    miledge.h, milreg.h, milmod.h, milcode.h, milmeas.h, milpat.h, milim.h
#define M_POSITION_Y                         0x00004400L             // Already defined in    miledge.h, milreg.h, milmod.h, milcode.h, milmeas.h, milpat.h, milim.h
#define M_POSITION_START_X                   9L                      // Already defined in    milpat.h
#define M_POSITION_START_Y                   10L                     // Already defined in    milpat.h
#define M_POSITION_END_X                     0x00003402L
#define M_POSITION_END_Y                     0x00004402L
#define M_ANGLE                              0x00000800L             // Already defined in    milreg.h, milmod.h, milcode.h, milcal.h, milmeas.h, milpat.h, mil.h
#define M_ANGLE_START                        0x00000801L
#define M_ANGLE_END                          0x00000802L
#define M_LENGTH                             0x00002000L             // Already defined in    miledge.h, milmod.h, milcal.h, milmeas.h, milblob.h, mil.h
#define M_RADIUS                             0x00000040L             // Already defined in    milmod.h, milmeas.h
#define M_LINE_A                             0x00003000L
#define M_LINE_B                             0x00003001L
#define M_LINE_C                             0x00003002L
#define M_COVERAGE                           216L

#define M_CIRCLE_CENTER_X                    M_POSITION_X
#define M_CIRCLE_CENTER_Y                    M_POSITION_Y
#define M_CIRCLE_RADIUS                      M_RADIUS
#define M_CIRCLE_LENGTH                      M_LENGTH
#define M_CIRCLE_COVERAGE                    M_COVERAGE
#define M_CIRCLE_DIAMETER                    217L

#define M_LOCAL_FRAME_POSITION_X             M_POSITION_X
#define M_LOCAL_FRAME_POSITION_Y             M_POSITION_Y
#define M_LOCAL_FRAME_ANGLE                  M_ANGLE
#define M_LOCAL_FRAME_GRID_SPACING           0x00003002L                                             

#define M_POINT_POSITION_X                   M_POSITION_X
#define M_POINT_POSITION_Y                   M_POSITION_Y
#define M_POINT_ANGLE                        M_ANGLE
#define M_NUMBER_OF_POINTS                   M_NUMBER

#define M_NUMBER_OF_DATA                     M_NUMBER                // (deprecated)
#define M_DATA_POSITION_X                    M_POSITION_X            // (deprecated)
#define M_DATA_POSITION_Y                    M_POSITION_Y            // (deprecated)
#define M_NUMBER_OF_EDGELS                   M_NUMBER
#define M_EDGELS_POSITION_X                  M_POSITION_X
#define M_EDGELS_POSITION_Y                  M_POSITION_Y

#define M_LINE_PARAM_A                       M_LINE_A
#define M_LINE_PARAM_B                       M_LINE_B
#define M_LINE_PARAM_C                       M_LINE_C
#define M_LINE_ANGLE                         M_ANGLE

#define M_SEGMENT_START_X                    M_POSITION_START_X
#define M_SEGMENT_START_Y                    M_POSITION_START_Y
#define M_SEGMENT_END_X                      M_POSITION_END_X
#define M_SEGMENT_END_Y                      M_POSITION_END_Y
#define M_SEGMENT_ANGLE                      M_ANGLE
#define M_SEGMENT_LENGTH                     M_LENGTH
#define M_SEGMENT_COVERAGE                   M_COVERAGE

#define M_ARC_CENTER_X                       M_POSITION_X
#define M_ARC_CENTER_Y                       M_POSITION_Y
#define M_ARC_RADIUS                         M_RADIUS
#define M_ARC_ANGLE_START                    M_ANGLE_START
#define M_ARC_ANGLE_END                      M_ANGLE_END
#define M_ARC_LENGTH                         M_LENGTH
#define M_ARC_ANGLE                          M_ANGLE
#define M_ARC_START_X                        M_POSITION_START_X
#define M_ARC_START_Y                        M_POSITION_START_Y
#define M_ARC_END_X                          M_POSITION_END_X
#define M_ARC_END_Y                          M_POSITION_END_Y
#define M_ARC_COVERAGE                       M_COVERAGE

#define M_TOLERANCE_VALUE                    M_VALUE

#define M_NUMBER_OF_FEATURES_PASS            M_NUMBER_OF_FEATURES+100
#define M_NUMBER_OF_FEATURE_FAIL             M_NUMBER_OF_FEATURES+101
#define M_NUMBER_OF_FEATURES_FAIL            M_NUMBER_OF_FEATURE_FAIL
#define M_NUMBER_OF_TOLERANCES_PASS          M_NUMBER_OF_TOLERANCES+100
#define M_NUMBER_OF_TOLERANCES_FAIL          M_NUMBER_OF_TOLERANCES+101
#define M_NUMBER_OF_TOLERANCES_WARNING       M_NUMBER_OF_TOLERANCES+102

#define M_MEASURED_DATA_SIZE                 0x00003000L
#define M_MEASURED_DATA_COORDX               0x00003001L        
#define M_MEASURED_DATA_COORDY               0x00003002L
#define M_MEASURED_USED_DATA_SIZE            0x00003003L
#define M_MEASURED_USED_DATA_COORDX          0x00003004L        
#define M_MEASURED_USED_DATA_COORDY          0x00003005L
#define M_MEASURED_DATA_ANGLE                0x00003006L
#define M_MEASURED_USED_DATA_ANGLE           0x00003007L

/***************************************************************************/
/* MmetDraw()                                                             */
/***************************************************************************/
#define M_DRAW_RELATIVE_ORIGIN_X                   319L        // deprecated : Use MgraControl(... M_DRAW_OFFSET_X...)
#define M_DRAW_RELATIVE_ORIGIN_Y                   320L        // deprecated : Use MgraControl(... M_DRAW_OFFSET_Y...)
#define M_DRAW_SCALE_X                             3203L       // deprecated : Use MgraControl(... M_DRAW_ZOOM_X ...)
#define M_DRAW_SCALE_Y                             3204L       // deprecated : Use MgraControl(... M_DRAW_ZOOM_Y ...)
#define M_DRAW_CROSS_SIZE                    115L                    // Already defined in    miledge.h
#define M_DRAW_SIZE                          M_DRAW_CROSS_SIZE

#define M_DRAW_LABEL                         0x00000001L             // Already defines in    miledge.h
#define M_DRAW_IMAGE                         0x00000002L             // Already defines in    milmod.h, milpat.h, miledge.h
#define M_DRAW_TEMPLATE_REFERENCE            5L
#define M_DRAW_REGION                        0x00000010L             //|M_DRAW_LABEL=17
#define M_DRAW_FEATURE                       6L                      //|M_DRAW_LABEL=7
#define M_DRAW_TOLERANCE                     8L                      //|M_DRAW_LABEL=9
#define M_DRAW_DATA_ALL                      10L                     // (deprecated)
#define M_DRAW_EDGELS                        0x2000L                 // Already defines in    miledge.h
#define M_DRAW_ALL_EDGELS                    M_DRAW_EDGELS
#define M_DRAW_ACTIVE_EDGELS                 0x00100000L
#define M_DRAW_DATA                          M_DRAW_ACTIVE_EDGELS    // (deprecated)
#define M_DRAW_FITTED_EDGELS                 15L
#define M_DRAW_REFERENCE_FEATURES            12L                     // deprecated flag name
#define M_DRAW_TOLERANCE_FEATURES            M_DRAW_REFERENCE_FEATURES 
#define M_DRAW_FRAME_GRID                    13L
#define M_DRAW_NAME                          14L
#define M_DRAW_ASSOCIATED_CALIBRATION      1495L
/***************************************************************************/
/* MmetStream                                                              */
/***************************************************************************/
#define M_SAVE                             0x100L                    // Already defines in    milmod.h, milocr.h, miledge.h
#define M_RESTORE                           0x10L                    // Already defined in    miledge.h
#define M_LOAD                                 1L                    // Already defines in    milmod.h, milmeas.h, mil.h, miledge.h
#define M_INQUIRE_SIZE_BYTE                    2L                    // Already defines in    milmod.h, miledge.h
#define M_MEMORY                           (M_APP_INQUIRE_SYS_START+120L) /*5120*/ //Only 2 bits set. Used in bitwise operations 
#define M_WITH_CALIBRATION            0x00020000L                    // Already defined in    mil.h, milmod.h

/***************************************************************************/
/* MmetAddFeature                                                          */
/***************************************************************************/
// Feature type
#define M_MODIFY                             999L
#define M_MEASURED                           1L
#define M_CONSTRUCTED                        2L


// Geometry
#define M_LINE                               0x00000080L             // Already defined in    miledge.h, milmod
#define M_CIRCLE                             0x00000008L             // Already defined in    milmod.h, milmeas.h
#define M_POINT                              1L                      // Already defined in    milmeas.h
#define M_STRIPE                             3L                      // Already defined in    milmeas.h
#define M_DATA                               4L                      // (deprecated)
#define M_EDGEL                              M_DATA
#define M_ARC                                0x00000081L             // Already defined in    miledge.h
#define M_SEGMENT                            0x00000082L    
#define M_LOCAL_FRAME                        0x00000083L    

// BuildOperation
#define M_FIT                                0x0020L                 // Already defined in    milcal.h
#define M_PARAMETRIC                         0x0021L
#define M_INTERSECTION                       0x0022L
#define M_CONSTRUCTION                       0x0023L
#define M_MIDDLE                             0x0024L
#define M_CLOSEST                            0x0025L
#define M_FARTHEST                           0x0026L                 //(deprecated)
#define M_MAX_DISTANCE_POINT                 M_FARTHEST
#define M_PARALLEL                           0x0027L
#define M_PERPENDICULAR                      0x0028L
#define M_OUTER_FIT                          0x0029L
#define M_INNER_FIT                          0x0030L
#define M_POSITION_ABSOLUTE                  0x0031L
#define M_POSITION_RELATIVE                  0x0032L
#define M_TANGENT                            0x0033L
#define M_SECTOR_RELATIVE                    0x0035L
#define M_BISECTOR                           0x0036L
#define M_POSITION_START                     0x0037L
#define M_POSITION_END                       0x0038L
#define M_ANGLE_ABSOLUTE                     0x0039L
#define M_ANGLE_RELATIVE                     0x0040L
#define M_COPY_DATA                          0x0041L              // (deprecated)
#define M_COPY_FEATURE_EDGELS                M_COPY_DATA
#define M_COPY                               0x00020000L          // already defined in mil.h, milblob.h (deprecated)
#define M_CLONE_FEATURE                      M_COPY
#define M_CENTER                             0x00000080L          // already defined in milim.h
#define M_ANGLE                              0x00000800L          // already defined in mil.h
#define M_EXTENDED_INTERSECTION              0x0042L

/***************************************************************************/
/* MmetAddTolerance                                                        */
/***************************************************************************/
// Tolerance type
#define M_PERPENDICULARITY                   1L
#define M_LENGTH                             0x00002000L          // Already defined in       miledge.h, milblob.h, milcal.h, milmeas.h, milmod.h, mil.h
#define M_DISTANCE_MIN                       2L
#define M_DISTANCE_MAX                       0x00004900L          // Already defined in       milcode.h
#define M_ANGULARITY                         3L
#define M_PARALLELISM                        4L
#define M_ROUNDNESS                          5L
#define M_STRAIGHTNESS                       6L
#define M_CONCENTRICITY                      7L
#define M_POSITION_X                         0x00003400L          // Already defined in       milmeas.h, milcode.h, milmod.h, milpat.h, mil.h
#define M_POSITION_Y                         0x00004400L          // Already defined in       milblob.h, milcode.h, milmod.h, milpat.h, mil.h

// Attributes
#define M_REFERENCE_LABEL                    9L
#define M_VERIFIED_LABEL                     3L
#define M_VALUE_MIN                          4L
#define M_VALUE_MAX                          5L
#define M_VALUE_WARNING_MIN                  7L
#define M_VALUE_WARNING_MAX                  8L
#define M_GAIN                               5124L               // Already defined in mil.h
#define M_OFFSET                             2L                  // Already defined in milpat.h

/***************************************************************************/
/* MmetSetRegion                                                           */
/***************************************************************************/
#define M_RECTANGLE                          0x00000040L             // Already defined in    milmod.h
#define M_RING                               0x00000100L             // Already defined in    milmod.h
#define M_RING_SECTOR                        0x00000200L
#define M_INFINITE                           -1L                     // Also defined in Mil.h
#define M_SEGMENT                            0x00000082L
#define M_ARC                                0x00000081L             // Already defined in    miledge.h
#define M_FROM_GRAPHIC_LIST                  0x08000000L             // Bitwise with other geometry (can be added)

/***************************************************************************/
/* MmetSetPosition                                                         */
/***************************************************************************/
#define M_RESULT                             0x00001000L             // Already defined in    milmod.h, milmeas.h
#define M_GEOMETRIC                          0x00002000L             // Already defined in    milmod.h
#define M_FORWARD_COEFFICIENTS               0x00010000L

/***************************************************************************/
/* MmetName()                                                              */
/***************************************************************************/
#define M_SET_NAME                           1L
#define M_GET_NAME                           2L
#define M_GET_FEATURE_LABEL                  3L
#define M_GET_TOLERANCE_LABEL                4L
#define M_GET_FEATURE_INDEX                  5L
#define M_GET_TOLERANCE_INDEX                6L

/***************************************************************************/
/* General definitions                                                     */
/***************************************************************************/
#define M_ENABLE                             -9997L                  // Already defined in    milblob.h, milcal.h, milmeas.h, mil.h, miledge.h
#define M_DISABLE                            -9999L                  // Already defined in    milblob.h, milcal.h, milmeas.h, mil.h, miledge.h
#define M_PACKED                             0x00020000L             // Already defined in    mil.h, miledge.h
#define M_AVAILABLE                          MAKE_INT64(0x0000400000000000) // Also defined in mil.h
#define M_SUPPORTED                          MAKE_INT64(0x0000800000000000) // Also defined in mil.h
#define M_ALL                                0x40000000L             // Already defined in    milmod.h, milmeas.h, milocr.h,  milpat.h,  mil.h, miledge.h
#define M_DEFAULT                            0x10000000L             // Already defined in    milmod.h, milocr.h,  milcode.h, mil.h, miledge.h
#define M_DELETE                             3L                      // Already defined in    milmod.h, miledge.h, milblob.h
#define M_SAME                               -1L                     // Already defined in    milmod.h, milmeas.h, milocr.h, miledge.h
#define M_SAME_OR_REVERSE                    5L                      // Already defined in    milmod.h, miledge.h
#define M_REVERSE                            4L                      // Already defined in    milmod.h, mil.h, miledge.h
#define M_TRUE                               1L                      // Already defined in    mil.h, milmeas.h, miledge.h
#define M_FALSE                              0L                      // Already defined in    mil.h, milmeas.h, miledge.h
#define M_PASS                               0x0002L                 // Already defined in    milim.h
#define M_FAIL                               4L
#define M_WARNING                            3L
#define M_IMAGE_FRAME                        0L
#define M_RESET                              9L                      // Already defined in    milblob.h, milmod.h

#define M_CONTEXT                            0x08000000L             // Already defined in    milmod.h, milreg.h, milstr.h, milocr.h

#define M_MET_OBJECT                         (M_USER_ATTRIBUTE       | MAKE_INT64(0x0000000100000000))// Already defined in    mil.h
#define M_MET_CONTEXT                        (M_MET_OBJECT | 0x00000001L ) // Already defined in    mil.h
#define M_MET_RESULT                         (M_MET_OBJECT | 0x00000002L ) // Already defined in    mil.h
#define M_MET_GEOMETRY                       (M_MET_OBJECT | 0x00000004L ) // Already defined in    mil.h
#define M_INTERACTIVE                        M_NULL                  // Already defined in    mil.h, milcal.h, milcode.h, milmeas.h, milocr.h, milpat.h, milmod.h, miledge.h

/***************************************************************************/
/* Utilities                                                               */
/***************************************************************************/

#define M_COORDINATE_SYSTEM                  123L
#define M_FEATURE_LABEL_TAG                  0x01000000L
#define M_FEATURE_INDEX_TAG                  0x02000000L
#define M_TOLERANCE_INDEX_TAG                0x04000000L
#define M_TOLERANCE_LABEL_TAG                0x20000000L

#define M_FEATURE_LABEL(i)                   (M_FEATURE_LABEL_TAG         | (i))
#define M_FEATURE_INDEX(i)                   (M_FEATURE_INDEX_TAG         | (i))
#define M_TOLERANCE_LABEL(i)                 (M_TOLERANCE_LABEL_TAG       | (i))
#define M_TOLERANCE_INDEX(i)                 (M_TOLERANCE_INDEX_TAG       | (i))

#define M_METROLOGY_COMPATIBLE               98L                  // Already defined in    miledge.h

#define M_VALUE                              0L
#define M_MIN                                0x02000000L          // Already defined in    milmeas.h, mil.h, miledge.h
#define M_MAX                                0x04000000L          // Already defined in    milmeas.h, mil.h, miledge.h
#define M_MEAN                               0x08000000L          // Already defined in    milmeas.h, mil.h, milocr.h, miledge.h
#define M_MIN_ABS                            0x00200000L          // Already defined in    mil.h, miledge.h
#define M_MAX_ABS                            0x00800000L          // Already defined in    mil.h, miledge.h
#define M_VARIANCE                           0x01000000L
#define M_SKEWNESS                           0x20000000L
#define M_LAST                               0x20000000L          // Already defined in    mil.h

// Deprecated flag names.
#if OldDefinesSupport
   // #define        M_MAJOR_AXIS M_RADIUS already defined in milmeas.h
   // MIL_DEPRECATED(M_MAJOR_AXIS, 1000)   already defined in milmeas.h
#endif

/***************************************************************************/
/*               MilMetrol CAPI function prototypes                        */
/***************************************************************************/

/***************************************************************************/
/* MmetAlloc()                                                             */
/***************************************************************************/
MIL_ID MFTYPE  MmetAlloc            (MIL_ID       SysId,
                                     MIL_INT64    Type,
                                     MIL_ID*      ContextOrRegionObjectIdPtr);

/***************************************************************************/
/* MmetAllocResult()                                                       */
/***************************************************************************/
MIL_ID MFTYPE  MmetAllocResult      (MIL_ID       SysId,
                                     MIL_INT64    ControlFlag,
                                     MIL_ID*      ResultIdPtr);

/***************************************************************************/
/* MmetControl()                                                           */
/***************************************************************************/
#if M_MIL_USE_64BIT
// Prototypes for 64 bits OSs
void MFTYPE    MmetControlInt64     (MIL_ID       MetId,
                                     MIL_INT      LabelOrIndex,
                                     MIL_INT64    ControlType,
                                     MIL_INT64    ControlValue);

void MFTYPE    MmetControlDouble    (MIL_ID       MetId,
                                     MIL_INT      LabelOrIndex,
                                     MIL_INT64    ControlType,
                                     MIL_DOUBLE   ControlValue);

#else
// Prototypes for 32 bits OSs
#define MmetControlInt64  MmetControl
#define MmetControlDouble MmetControl

void MFTYPE    MmetControl          (MIL_ID       MetId,
                                     MIL_INT      LabelOrIndex,
                                     MIL_INT64    ControlType,
                                     MIL_DOUBLE   ControlValue);

#endif

/***************************************************************************/
/* MmetInquire()                                                           */
/***************************************************************************/
MIL_INT MFTYPE    MmetInquire       (MIL_ID       MetId,
                                     MIL_INT      LabelOrIndex,
                                     MIL_INT64    InquireType,
                                     void*        UserVarPtr);

/***************************************************************************/
/* MmetSetPosition()                                                       */
/***************************************************************************/
#if M_MIL_USE_64BIT
// Prototypes for 64 bits OSs
void MFTYPE    MmetSetPositionInt64 (MIL_ID      ContextId,
                                     MIL_INT     FeatureLabelOrIndex,
                                     MIL_INT64   TransformationType,
                                     MIL_INT64   Param1,
                                     MIL_DOUBLE  Param2,
                                     MIL_DOUBLE  Param3,
                                     MIL_DOUBLE  Param4,
                                     MIL_INT64   ControlFlag);

void MFTYPE    MmetSetPositionDouble(MIL_ID      ContextId,
                                     MIL_INT     FeatureLabelOrIndex,
                                     MIL_INT64   TransformationType,
                                     MIL_DOUBLE  Param1,
                                     MIL_DOUBLE  Param2,
                                     MIL_DOUBLE  Param3,
                                     MIL_DOUBLE  Param4,
                                     MIL_INT64   ControlFlag);
#else
// Prototypes for 32 bits OSs
#define MmetSetPositionInt64  MmetSetPosition
#define MmetSetPositionDouble MmetSetPosition
void MFTYPE    MmetSetPosition      (MIL_ID      ContextId,
                                     MIL_INT     FeatureLabelOrIndex,
                                     MIL_INT64   TransformationType,
                                     MIL_DOUBLE  Param1,
                                     MIL_DOUBLE  Param2,
                                     MIL_DOUBLE  Param3,
                                     MIL_DOUBLE  Param4,
                                     MIL_INT64   ControlFlag);
#endif
/***************************************************************************/
/* MmetMeasuredRegion()                                                    */
/***************************************************************************/
#if M_MIL_USE_64BIT
// Prototypes for 64 bits OSs
void MFTYPE    MmetSetRegionInt64        (MIL_ID      ContextId,
                                          MIL_INT     FeatureLabelOrIndex,
                                          MIL_INT     ReferenceFrameLabelOrIndex,
                                          MIL_INT64   Geometry,
                                          MIL_INT64   Param1,
                                          MIL_DOUBLE  Param2,
                                          MIL_DOUBLE  Param3,
                                          MIL_DOUBLE  Param4,
                                          MIL_DOUBLE  Param5,
                                          MIL_DOUBLE  Param6);

void MFTYPE    MmetSetRegionDouble        (MIL_ID      ContextId,
                                           MIL_INT     FeatureLabelOrIndex,
                                           MIL_INT     ReferenceFrameLabelOrIndex,
                                           MIL_INT64   Geometry,
                                           MIL_DOUBLE  Param1,
                                           MIL_DOUBLE  Param2,
                                           MIL_DOUBLE  Param3,
                                           MIL_DOUBLE  Param4,
                                           MIL_DOUBLE  Param5,
                                           MIL_DOUBLE  Param6);
#else
// Prototypes for 32 bits OSs
#define MmetSetRegionInt64  MmetSetRegion
#define MmetSetRegionDouble MmetSetRegion

void MFTYPE    MmetSetRegion        (MIL_ID      ContextId,
                                     MIL_INT     FeatureLabelOrIndex,
                                     MIL_INT     ReferenceFrameLabelOrIndex,
                                     MIL_INT64   Geometry,
                                     MIL_DOUBLE  Param1,
                                     MIL_DOUBLE  Param2,
                                     MIL_DOUBLE  Param3,
                                     MIL_DOUBLE  Param4,
                                     MIL_DOUBLE  Param5,
                                     MIL_DOUBLE  Param6);

#endif
/***************************************************************************/
/* MmetCalculate()                                                         */
/***************************************************************************/
void MFTYPE    MmetCalculate        (MIL_ID      ContextId,
                                     MIL_ID      TargetBufId,
                                     MIL_ID      ResultId,
                                     MIL_INT64   ControlFlag);

/***************************************************************************/
/* MmetGetResult()                                                         */
/***************************************************************************/
void MFTYPE    MmetGetResult        (MIL_ID      ResultId,
                                     MIL_INT     LabelOrIndex,
                                     MIL_INT64   ResultType,
                                     void*       ResultArrayPtr);

/***************************************************************************/
/* MmetGetResultOutputUnits()                                              */
/***************************************************************************/
void MFTYPE MmetGetResultOutputUnits(MIL_ID     ResultId,
                                     MIL_INT    LabelOrIndex,
                                     MIL_INT64  ResultType,
                                     MIL_INT64  OutputUnits,
                                     MIL_INT64  ForcedOutputFrame,
                                     void*      ResultArrayPtr);

/***************************************************************************/
/* MmetAddFeature()                                                        */
/***************************************************************************/
void MFTYPE    MmetAddFeature       (MIL_ID           ContextId,
                                     MIL_INT64        FeatureType,
                                     MIL_INT64        Geometry,
                                     MIL_INT          FeatureLabel,
                                     MIL_INT64        BuildOperation,
                                     const MIL_INT   *FeatureLabelArrayPtr,
                                     const MIL_INT   *SubFeatureIndexArrayPtr,
                                     MIL_INT          SizeOfArray,
                                     MIL_INT64        ControlFlag);

/***************************************************************************/
/* MmetAddTolerance()                                                      */
/***************************************************************************/
void MFTYPE    MmetAddTolerance     (MIL_ID          ContextId,
                                     MIL_INT64       ToleranceType,
                                     MIL_INT         ToleranceLabel,
                                     MIL_DOUBLE      ValueMin,
                                     MIL_DOUBLE      ValueMax,
                                     const MIL_INT*  FeatureLabelArrayPtr,
                                     const MIL_INT*  SubFeatureIndexArrayPtr,
                                     MIL_INT         SizeOfArray,
                                     MIL_INT64       ControlFlag);

/***************************************************************************/
/* MmetDraw()                                                              */
/***************************************************************************/
void MFTYPE    MmetDraw             (MIL_ID      ContextGraId,
                                     MIL_ID      ContextOrResultMetId,
                                     MIL_ID      DstImageBufOrListGraId,
                                     MIL_INT64   Operation,
                                     MIL_INT     LabelOrIndex,
                                     MIL_INT64   ControlFlag);

/***************************************************************************/
/* MmetFree()                                                              */
/***************************************************************************/
void MFTYPE    MmetFree             (MIL_ID       MetId);

#if M_MIL_USE_UNICODE
/***************************************************************************/
/* MmetSave() Unicode                                                      */ 
/***************************************************************************/
void MFTYPE    MmetSaveW            (MIL_CONST_TEXT_PTR FileName,
                                     MIL_ID       ContextId,
                                     MIL_INT64    ControlFlag);

/***************************************************************************/
/* MmetRestore() Unicode                                                   */
/***************************************************************************/
MIL_ID  MFTYPE MmetRestoreW         (MIL_CONST_TEXT_PTR FileName,
                                     MIL_ID       SysId,
                                     MIL_INT64    ControlFlag,
                                     MIL_ID*      ContextIdPtr);

/***************************************************************************/
/* MmetStream() Unicode                                                    */
/***************************************************************************/
void MFTYPE    MmetStreamW          (MIL_TEXT_PTR MemPtrOrFileName,
                                     MIL_ID       SysId,
                                     MIL_INT64    Operation,
                                     MIL_INT64    StreamType,
                                     MIL_DOUBLE   Version,
                                     MIL_INT64    ControlFlag,
                                     MIL_ID*      ObjectIdPtr,
                                     MIL_INT*     SizeByteVarPtr);

/***************************************************************************/
/* MmetName() Unicode                                                      */
/***************************************************************************/
void MFTYPE    MmetNameW            (MIL_ID        ContextOrResultId,
                                     MIL_INT64     Operation,
                                     MIL_INT       LabelOrIndex,
                                     MIL_TEXT_PTR  String,
                                     MIL_INT*      ValuePtr,
                                     MIL_INT64     ControlFlag);

/***************************************************************************/
/* MmetSave() ASCII                                                        */ 
/***************************************************************************/
void MFTYPE    MmetSaveA            (const char*   FileName,
                                     MIL_ID        ContextId,
                                     MIL_INT64     ControlFlag);

/***************************************************************************/
/* MmetRestore() ASCII                                                     */ 
/***************************************************************************/
MIL_ID  MFTYPE MmetRestoreA         (const char*  FileName,
                                     MIL_ID       SysId,
                                     MIL_INT64    ControlFlag,
                                     MIL_ID*      ContextIdPtr);

/***************************************************************************/
/* MmetStream() ASCII                                                      */ 
/***************************************************************************/
void MFTYPE    MmetStreamA          (char*        MemPtrOrFileName,
                                     MIL_ID       SysId,
                                     MIL_INT64    Operation,
                                     MIL_INT64    StreamType,
                                     MIL_DOUBLE   Version,
                                     MIL_INT64    ControlFlag,
                                     MIL_ID*      ObjectIdPtr,
                                     MIL_INT*     SizeByteVarPtr);

/***************************************************************************/
/* MmetName() ASCII                                                        */
/***************************************************************************/
void MFTYPE    MmetNameA            (MIL_ID        ContextOrResultId,
                                     MIL_INT64     Operation,
                                     MIL_INT       LabelOrIndex,
                                     char*         String,
                                     MIL_INT*      ValuePtr,
                                     MIL_INT64     ControlFlag);

#if M_MIL_UNICODE_API

#define MmetSave        MmetSaveW
#define MmetRestore     MmetRestoreW
#define MmetStream      MmetStreamW
#define MmetName        MmetNameW

#else

#define MmetSave        MmetSaveA
#define MmetRestore     MmetRestoreA
#define MmetStream      MmetStreamA
#define MmetName        MmetNameA

#endif

#else

/***************************************************************************/
/* MmetSave()                                                              */
/***************************************************************************/
void MFTYPE    MmetSave             (MIL_CONST_TEXT_PTR FileName,
                                     MIL_ID       ContextId,
                                     MIL_INT64    ControlFlag);

/***************************************************************************/
/* MmetRestore()                                                           */
/***************************************************************************/
MIL_ID  MFTYPE MmetRestore          (MIL_CONST_TEXT_PTR FileName,
                                     MIL_ID       SysId,
                                     MIL_INT64    ControlFlag,
                                     MIL_ID*      ContextIdPtr);

/***************************************************************************/
/* MmetStream()                                                            */
/***************************************************************************/
void MFTYPE    MmetStream           (MIL_TEXT_PTR MemPtrOrFileName,
                                     MIL_ID       SysId,
                                     MIL_INT64    Operation,
                                     MIL_INT64    StreamType,
                                     MIL_DOUBLE   Version,
                                     MIL_INT64    ControlFlag,
                                     MIL_ID*      ContextIdPtr,
                                     MIL_INT*     SizeByteVarPtr);


/***************************************************************************/
/* MmetName()                                                              */
/***************************************************************************/
void MFTYPE    MmetName            (MIL_ID         ContextOrResultId,
                                    MIL_INT64      Operation,
                                    MIL_INT        LabelOrIndex,
                                    MIL_TEXT_PTR   String,
                                    MIL_INT*       ValuePtr,
                                    MIL_INT64      ControlFlag);

#endif

   /* C++ directive if needed */
#ifdef __cplusplus
   }
#endif

#if M_MIL_USE_64BIT
#ifdef __cplusplus
//////////////////////////////////////////////////////////////
// MmetControl function definition when compiling c++ files
//////////////////////////////////////////////////////////////
#if !M_MIL_USE_LINUX
inline void  MmetControl           (MIL_ID       MetId, 
                                    MIL_INT      LabelOrIndex,
                                    MIL_INT64    ControlType, 
                                    int          ControlValue)
   {
   MmetControlInt64(MetId, LabelOrIndex, ControlType, ControlValue);   
   };
#endif
inline void  MmetControl           (MIL_ID       MetId, 
                                    MIL_INT      LabelOrIndex,
                                    MIL_INT64    ControlType, 
                                    MIL_INT32    ControlValue)
   {
   MmetControlInt64(MetId, LabelOrIndex, ControlType, ControlValue);   
   };
inline void  MmetControl           (MIL_ID       MetId, 
                                    MIL_INT      LabelOrIndex,
                                    MIL_INT64    ControlType, 
                                    MIL_INT64    ControlValue)
   {
   MmetControlInt64(MetId, LabelOrIndex, ControlType, ControlValue);   
   };
inline void  MmetControl           (MIL_ID       MetId, 
                                    MIL_INT      LabelOrIndex,
                                    MIL_INT64    ControlType, 
                                    MIL_DOUBLE   ControlValue)
   {
   MmetControlDouble(MetId, LabelOrIndex, ControlType, ControlValue);   
   };
//////////////////////////////////////////////////////////////
// MmetSetRegion function definition when compiling c++ files
//////////////////////////////////////////////////////////////
#if !M_MIL_USE_LINUX
inline void    MmetSetRegion        (MIL_ID      ContextId,
                                     MIL_INT     FeatureLabelOrIndex,
                                     MIL_INT     ReferenceFrameLabelOrIndex,
                                     MIL_INT64   Geometry,
                                     int         Param1,
                                     MIL_DOUBLE  Param2,
                                     MIL_DOUBLE  Param3,
                                     MIL_DOUBLE  Param4,
                                     MIL_DOUBLE  Param5,
                                     MIL_DOUBLE  Param6)
   {
   MmetSetRegionInt64(ContextId, FeatureLabelOrIndex, ReferenceFrameLabelOrIndex, Geometry, Param1, Param2, Param3, Param4, Param5, Param6);
   };
#endif
inline void    MmetSetRegion        (MIL_ID      ContextId,
                                     MIL_INT     FeatureLabelOrIndex,
                                     MIL_INT     ReferenceFrameLabelOrIndex,
                                     MIL_INT64   Geometry,
                                     MIL_INT32   Param1,
                                     MIL_DOUBLE  Param2,
                                     MIL_DOUBLE  Param3,
                                     MIL_DOUBLE  Param4,
                                     MIL_DOUBLE  Param5,
                                     MIL_DOUBLE  Param6)
   {
   MmetSetRegionInt64(ContextId, FeatureLabelOrIndex, ReferenceFrameLabelOrIndex, Geometry, Param1, Param2, Param3, Param4, Param5, Param6);
   };
inline void    MmetSetRegion        (MIL_ID      ContextId,
                                     MIL_INT     FeatureLabelOrIndex,
                                     MIL_INT     ReferenceFrameLabelOrIndex,
                                     MIL_INT64   Geometry,
                                     MIL_INT64   Param1,
                                     MIL_DOUBLE  Param2,
                                     MIL_DOUBLE  Param3,
                                     MIL_DOUBLE  Param4,
                                     MIL_DOUBLE  Param5,
                                     MIL_DOUBLE  Param6)
   {
   MmetSetRegionInt64(ContextId, FeatureLabelOrIndex, ReferenceFrameLabelOrIndex, Geometry, Param1, Param2, Param3, Param4, Param5, Param6);
   };
inline void    MmetSetRegion        (MIL_ID      ContextId,
                                     MIL_INT     FeatureLabelOrIndex,
                                     MIL_INT     ReferenceFrameLabelOrIndex,
                                     MIL_INT64   Geometry,
                                     MIL_DOUBLE  Param1,
                                     MIL_DOUBLE  Param2,
                                     MIL_DOUBLE  Param3,
                                     MIL_DOUBLE  Param4,
                                     MIL_DOUBLE  Param5,
                                     MIL_DOUBLE  Param6)
   {
   MmetSetRegionDouble(ContextId, FeatureLabelOrIndex, ReferenceFrameLabelOrIndex, Geometry, Param1, Param2, Param3, Param4, Param5, Param6);
   };
//////////////////////////////////////////////////////////////
// MmetSetPosition function definition when compiling c++ files
//////////////////////////////////////////////////////////////
#if !M_MIL_USE_LINUX
inline void    MmetSetPosition      (MIL_ID      ContextId, 
                                     MIL_INT     FeatureLabelOrIndex,
                                     MIL_INT64   TransformationType, 
                                     int         Param1, 
                                     MIL_DOUBLE  Param2, 
                                     MIL_DOUBLE  Param3, 
                                     MIL_DOUBLE  Param4,
                                     MIL_INT64   ControlFlag)
   {
   MmetSetPositionInt64(ContextId, FeatureLabelOrIndex, TransformationType, Param1, Param2, Param3, Param4, ControlFlag);
   };
#endif

inline void    MmetSetPosition      (MIL_ID      ContextId, 
                                     MIL_INT     FeatureLabelOrIndex,
                                     MIL_INT64   TransformationType, 
                                     MIL_INT32   Param1, 
                                     MIL_DOUBLE  Param2, 
                                     MIL_DOUBLE  Param3, 
                                     MIL_DOUBLE  Param4,
                                     MIL_INT64   ControlFlag)
   {
   MmetSetPositionInt64(ContextId, FeatureLabelOrIndex, TransformationType, Param1, Param2, Param3, Param4, ControlFlag);
   };
inline void    MmetSetPosition      (MIL_ID      ContextId, 
                                     MIL_INT     FeatureLabelOrIndex,
                                     MIL_INT64   TransformationType, 
                                     MIL_INT64   Param1, 
                                     MIL_DOUBLE  Param2, 
                                     MIL_DOUBLE  Param3, 
                                     MIL_DOUBLE  Param4,
                                     MIL_INT64   ControlFlag)
   {
   MmetSetPositionInt64(ContextId, FeatureLabelOrIndex, TransformationType, Param1, Param2, Param3, Param4, ControlFlag);
   };
inline void    MmetSetPosition      (MIL_ID      ContextId, 
                                     MIL_INT     FeatureLabelOrIndex,
                                     MIL_INT64   TransformationType, 
                                     MIL_DOUBLE  Param1, 
                                     MIL_DOUBLE  Param2, 
                                     MIL_DOUBLE  Param3, 
                                     MIL_DOUBLE  Param4,
                                     MIL_INT64   ControlFlag)
   {
   MmetSetPositionDouble(ContextId, FeatureLabelOrIndex, TransformationType, Param1, Param2, Param3, Param4, ControlFlag);
   };

#else
   //////////////////////////////////////////////////////////////
   // For C file, call the default function, i.e. Double one
   //////////////////////////////////////////////////////////////
#define MmetControl     MmetControlDouble
#define MmetSetPosition MmetSetPositionDouble
#define MmetSetRegion   MmetSetRegionDouble

#endif // __cplusplus
#endif // M_MIL_USE_64BIT

#if M_MIL_USE_SAFE_TYPE

//////////////////////////////////////////////////////////////
// See milos.h for explanation about these functions.
//////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------
//  MmetGetResult

inline void MFTYPE MmetGetResultUnsafe  (MIL_ID ResultId, MIL_INT Index, MIL_INT64  ResultType, void           *ResultArrayPtr);
inline void MFTYPE MmetGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT64  ResultType, int             ResultArrayPtr);
inline void MFTYPE MmetGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT64  ResultType, MIL_INT32      *ResultArrayPtr);
inline void MFTYPE MmetGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT64  ResultType, MIL_INT64      *ResultArrayPtr);
inline void MFTYPE MmetGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT64  ResultType, MIL_DOUBLE     *ResultArrayPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED                                    
inline void MFTYPE MmetGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT64  ResultType, MIL_UINT32     *ResultArrayPtr);
inline void MFTYPE MmetGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT64  ResultType, MIL_UINT64     *ResultArrayPtr);
#endif

// ----------------------------------------------------------
// MmetInquire

inline MIL_INT MFTYPE MmetInquireUnsafe  (MIL_ID ContextId, MIL_INT Index, MIL_INT64  InquireType, void           *ResultArrayPtr);
inline MIL_INT MFTYPE MmetInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT64  InquireType, MIL_INT32      *ResultArrayPtr);
inline MIL_INT MFTYPE MmetInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT64  InquireType, MIL_INT64      *ResultArrayPtr);
inline MIL_INT MFTYPE MmetInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT64  InquireType, MIL_DOUBLE     *ResultArrayPtr);
inline MIL_INT MFTYPE MmetInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT64  InquireType, int             ResultArrayPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED                                      
inline MIL_INT MFTYPE MmetInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT64  InquireType, MIL_UINT32     *ResultArrayPtr);
inline MIL_INT MFTYPE MmetInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT64  InquireType, MIL_UINT64     *ResultArrayPtr);
#endif

// -------------------------------------------------------------------------
// MmetGetResult

inline void MFTYPE MmetGetResultSafeType (MIL_ID ResultId, MIL_INT Index, MIL_INT64  ResultType, int ResultPtr)
   {
   if (ResultPtr)
      SafeTypeError(MIL_TEXT("MmetGetResult"));

   MmetGetResult(ResultId, Index, ResultType, NULL);
   }

inline void MmetGetResultSafeTypeExecute (MIL_ID ResultId, MIL_INT Index, MIL_INT64  ResultType, void           *ResultArrayPtr, MIL_INT64  GivenType)
   {
   MIL_INT64  RequiredType = ResultType & M_HLVLDATATYPE_MASK;
   if((RequiredType != M_TYPE_DOUBLE) && (RequiredType != M_TYPE_MIL_INT32) && (RequiredType != M_TYPE_MIL_INT64) && (RequiredType != M_TYPE_MIL_ID))
      RequiredType = 0;
   if (RequiredType == 0)
      RequiredType = M_TYPE_DOUBLE;
   ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MmetGetResult"));

   MmetGetResult(ResultId, Index, ResultType, ResultArrayPtr);
   }


inline void MFTYPE MmetGetResultUnsafe  (MIL_ID ResultId, MIL_INT Index, MIL_INT64  ResultType, void        *ResultArrayPtr){MmetGetResult               (ResultId, Index, ResultType, ResultArrayPtr);}
inline void MFTYPE MmetGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT64  ResultType, MIL_INT32   *ResultArrayPtr){MmetGetResultSafeTypeExecute(ResultId, Index, ResultType, ResultArrayPtr, M_TYPE_MIL_INT32);}
inline void MFTYPE MmetGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT64  ResultType, MIL_INT64   *ResultArrayPtr){MmetGetResultSafeTypeExecute(ResultId, Index, ResultType, ResultArrayPtr, M_TYPE_MIL_INT64);}
inline void MFTYPE MmetGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT64  ResultType, MIL_DOUBLE  *ResultArrayPtr){MmetGetResultSafeTypeExecute(ResultId, Index, ResultType, ResultArrayPtr, M_TYPE_DOUBLE);}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED                                    
inline void MFTYPE MmetGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT64  ResultType, MIL_UINT32  *ResultArrayPtr){MmetGetResultSafeTypeExecute(ResultId, Index, ResultType, ResultArrayPtr, M_TYPE_MIL_INT32);}
inline void MFTYPE MmetGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT64  ResultType, MIL_UINT64  *ResultArrayPtr){MmetGetResultSafeTypeExecute(ResultId, Index, ResultType, ResultArrayPtr, M_TYPE_MIL_INT64);}
#endif

// ----------------------------------------------------------
// MmetInquire

inline MIL_INT MFTYPE MmetInquireSafeType  (MIL_ID ContextId, MIL_INT Index, MIL_INT64  InquireType,int UserVarPtr )
   {
   if(UserVarPtr)
      SafeTypeError(MIL_TEXT("MmetInquire"));

   return MmetInquire(ContextId, Index, InquireType, NULL );
   }

inline MIL_INT MFTYPE MmetInquireSafeTypeExecute (MIL_ID ContextId, MIL_INT Index, MIL_INT64  InquireType, void *UserVarPtr, MIL_INT64  GivenType)
   {
   MIL_INT64  RequiredType = InquireType & M_HLVLDATATYPE_MASK;
   if((RequiredType != M_TYPE_DOUBLE) && (RequiredType != M_TYPE_MIL_INT32) && (RequiredType != M_TYPE_MIL_INT64) && (RequiredType != M_TYPE_MIL_ID))
      RequiredType = 0;
   if (RequiredType == 0)
      RequiredType = M_TYPE_DOUBLE;
   ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MmetInquire"));

   return MmetInquire(ContextId, Index, InquireType, UserVarPtr);
   }

inline MIL_INT MFTYPE MmetInquireUnsafe  (MIL_ID ContextId, MIL_INT Index, MIL_INT64  InquireType, void       *UserVarPtr) {return MmetInquire               (ContextId, Index, InquireType, UserVarPtr                  );}
inline MIL_INT MFTYPE MmetInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT64  InquireType, MIL_INT32  *UserVarPtr) {return MmetInquireSafeTypeExecute(ContextId, Index, InquireType, UserVarPtr, M_TYPE_MIL_INT32);}
inline MIL_INT MFTYPE MmetInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT64  InquireType, MIL_INT64  *UserVarPtr) {return MmetInquireSafeTypeExecute(ContextId, Index, InquireType, UserVarPtr, M_TYPE_MIL_INT64);}
inline MIL_INT MFTYPE MmetInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT64  InquireType, MIL_DOUBLE *UserVarPtr) {return MmetInquireSafeTypeExecute(ContextId, Index, InquireType, UserVarPtr, M_TYPE_DOUBLE   );}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED                                      
inline MIL_INT MFTYPE MmetInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT64  InquireType, MIL_UINT32 *UserVarPtr) {return MmetInquireSafeTypeExecute(ContextId, Index, InquireType, UserVarPtr, M_TYPE_MIL_INT32);}
inline MIL_INT MFTYPE MmetInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT64  InquireType, MIL_UINT64 *UserVarPtr) {return MmetInquireSafeTypeExecute(ContextId, Index, InquireType, UserVarPtr, M_TYPE_MIL_INT64);}
#endif

#define MmetGetResult        MmetGetResultSafeType
#define MmetInquire          MmetInquireSafeType

#else // #if M_MIL_USE_SAFE_TYPE

#define MmetGetResultUnsafe        MmetGetResult
#define MmetInquireUnsafe          MmetInquire

#endif // #if M_MIL_USE_SAFE_TYPE

#endif // !M_MIL_LITE

#endif /* __MILMETROLFINDER_H__ */
