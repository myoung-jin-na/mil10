/***************************************************************************/
/*

    Filename:  MilEdge.h
    Owner   :  Matrox Imaging dept.
    Revision:  10.00.2564
    Content :  This file contains the defines and the prototypes for the
               MIL edge and line crest extractor. (Medge...).

    Copyright © Matrox Electronic Systems Ltd., 1992-2014.
    All Rights Reserved

*/
/***************************************************************************/

#ifndef __MILEDGEEXTR_H__
#define __MILEDGEEXTR_H__

#if (!M_MIL_LITE) // MIL FULL ONLY

/* C++ directive if needed */
#ifdef __cplusplus
extern "C"
{
#endif

/***************************************************************************/
/*                      MilEdge CAPI defines                               */
/***************************************************************************/


/***************************************************************************/
/* MedgeAlloc()                                                            */
/***************************************************************************/
#define M_CONTOUR                            0x800L      // Already defined in    milblob.h
#define M_CREST                              0x801L

/***************************************************************************/
/* MedgeControl()                                                          */
/***************************************************************************/

/* Global to context behavior */
#define M_CHAIN_ALL_NEIGHBORS                888L
#define M_CHAINING_METHOD                    1111L
#define M_MAGNITUDE_TYPE                     10L
#define M_NORM                               4000L
#define M_SQR_NORM                           4001L
#define M_FILTER_TYPE                        1046L       // Already defined in    mil.h
#define M_KERNEL_WIDTH                       109L        // Already defined in    milmod.h
#define M_KERNEL_SIZE                        301L        // Already defined in    milmod.h
#define M_FILTER_MODE                        122L        // Already defined in    milmod.h
#define M_KERNEL_DEPTH                       368L        // Already defined in    milmod.h
#define M_KERNEL                             0x00400000L // Already defined in    milmod.h, mil.h
#define M_RECURSIVE                          0x00040000  // Already defined in    mildev.h
#define M_SHEN                               0x802L
#define M_EXPONENTIAL                        2           // Already defined in    mil.h
#define M_GAUSSIAN                           0x805L
#define M_SOBEL                              M_EDGE_DETECT  // Already defined in mil.h
#define M_PREWITT                            M_EDGE_DETECT2 // Already defined in mil.hL
#define M_FREI_CHEN                          M_ID_OFFSET_OF_DEFAULT_KERNEL + 9L
#define M_DERICHE                            M_ID_OFFSET_OF_DEFAULT_KERNEL + 10L
#define M_SMOOTHNESS                         108L         // Already defined in   milmod.h, milmeas.h, mil.h
#define M_FILTER_SMOOTHNESS                  M_SMOOTHNESS // Already defined in   mil.h, milmeas.h
#define M_ACCURACY                           106L        // Already defined in    milmod.h
#define M_ANGLE_ACCURACY                     M_SEARCH_ANGLE_ACCURACY // Already defined in    milpat.h
#define M_THRESHOLD_TYPE                     3000L
#define M_THRESHOLD_MODE                     33L
#define M_THRESHOLD_VALUE_LOW                3001L
#define M_THRESHOLD_VALUE_HIGH               3002L
#define M_THRESHOLD_LOW                      M_THRESHOLD_VALUE_LOW
#define M_THRESHOLD_HIGH                     M_THRESHOLD_VALUE_HIGH
#define M_USER_DEFINED                       21L         // Already defined in    mil.h, milocr.h
#define M_DETAIL_LEVEL                       111L        // Already defined in    milmod.h
#define M_NO_HYSTERESIS                      20L
#define M_FULL_HYSTERESIS                    25L
#define M_HYSTERESIS                         31L
#define M_SAVE_DERIVATIVES                   11L
#define M_SAVE_ANGLE                         12L
#define M_SAVE_MAGNITUDE                     13L
#define M_SAVE_MASK                          39L
#define M_SAVE_CHAIN_MAGNITUDE               59L
#define M_SAVE_CHAIN_ANGLE                   60L
#define M_SAVE_IMAGE                         61L
#define M_MASK_SIZE_X                        0x0000001DL
#define M_MASK_SIZE_Y                        0x0000001EL
#define M_FILL_GAP_DISTANCE                  44L
#define M_FILL_GAP_ANGLE                     45L
#define M_FILL_GAP_POLARITY                  46L
#define M_FILL_GAP_CONTINUITY                38L
#define M_FILL_GAP_CANDIDATE                 91L
#define M_EXTRACTION_ANGLE                   47L
#define M_EXTRACTION_TOLERANCE               48L
#define M_EXTRACTION_USE_POLARITY            49L
#define M_EXTRACTION_SCALE                   58L
#define M_EXTRACTION_SCALE_X                 94L
#define M_EXTRACTION_SCALE_Y                 95L

// Typical control values
#define M_LOW                                1L          // Already defined in    milcode.h, milmod.h, milocr, milpat.h, mil.h
#define M_MEDIUM                             2L          // Already defined in    milcode.h, milmod.h, milocr, milpat.h
#define M_HIGH                               3L          // Already defined in    milcode.h, milmod.h, milocr, milpat.h, mil.h
#define M_VERY_HIGH                          4L          // Already defined in    milcode.h, milmod.h, milocr, milpat.h
#define M_MEDIUM2                            11L
#define M_HIGH2                              12L

//Flag values for M_CHAINING_METHOD
#define M_CHAIN_FAST                         1L
#define M_CHAIN_MAX_MAGNITUDE                2L
#define M_CHAIN_NEAREST_MAGNITUDE            3L
#define M_CHAIN_NEAREST_ANGLE                4L
#define M_CHAIN_DIR                          5L
#define M_CHAIN_DIR_MAX_MAGNITUDE            6L
#define M_CHAIN_DIR_NEAREST_MAGNITUDE        7L
#define M_CHAIN_DIR_NEAREST_ANGLE            8L

// Parameters for output units
#define M_PIXEL                              0x1000L
#define M_WORLD                              0x2000L
#define M_ACCORDING_TO_CALIBRATION             1301L
#define M_ACCORDING_TO_RESULT                  1302L

// Utilities
#define M_FLOAT_MODE                         70L
#define M_ALIGNEMENT_MODE                    51L
#define M_TIMEOUT                            2077L       // Already defined in    milmod.h, mil.h

/* Global to context features */
#define M_BOX_X_MIN                          6L          // Already defined in    milblob.h
#define M_BOX_Y_MIN                          7L          // Already defined in    milblob.h
#define M_BOX_X_MAX                          8L          // Already defined in    milblob.h
#define M_BOX_Y_MAX                          9L          // Already defined in    milblob.M
#define M_X_MIN_AT_Y_MIN                     21L         // Already defined in    milblob.h
#define M_X_MAX_AT_Y_MAX                     22L         // Already defined in    milblob.h
#define M_Y_MIN_AT_X_MAX                     23L         // Already defined in    milblob.h
#define M_Y_MAX_AT_X_MIN                     24L         // Already defined in    milblob.h
#define M_CHAIN_APPROXIMATION                0x00010200L 
#define M_APPROXIMATION_TOLERANCE            26L
#define M_LINE                               0x00000080L // Already defined in    milmod.h
#define M_ARC                                0x00000081L
#define M_CENTER_OF_GRAVITY_X                34L         // Already defined in    milblob.h
#define M_CENTER_OF_GRAVITY_Y                35L         // Already defined in    milblob.h
#define M_POSITION                           0x00000402L // Already defined in    milmeas.h
#define M_POSITION_X                         0x00003400L // Already defined in    milmeas.h, milcode.h, milmod.h, milpat.h, mil.h
#define M_POSITION_Y                         0x00004400L // Already defined in    milblob.h, milcode.h, milmod.h, milpat.h, mil.h
#define M_MOMENT_ELONGATION                  50L
#define M_MOMENT_ELONGATION_ANGLE            999L
#define M_SIZE                               0xC00L      // Already defined in    milmod.h
#define M_FAST_LENGTH                        52L
#define M_TORTUOSITY                         76L
#define M_CLOSURE                            77L
#define M_STRENGTH                           55L
#define M_AVERAGE_STRENGTH                   57L
#define M_BOX                                0x101L      // Already defined in    milblob.h
#define M_CONTACT_POINTS                     0x102L      // Already defined in    milblob.h
#define M_CENTER_OF_GRAVITY                  0x103L      // Already defined in    milblob.h
#define M_LENGTH                             0x00002000L // Already defined in    milblob.h, milcal.h, milmeas.h, milmod.h, mil.h
#define M_FERET_X                            72L         // Already defined in    milblob.h
#define M_FERET_Y                            5L          // Already defined in    milblob.h
#define M_FERET                              69L
#define M_FERET_BOX                          M_FERET
#define M_FERET_MIN_ANGLE                    15L         // Already defined in    milblob.h
#define M_FERET_MAX_ANGLE                    17L         // Already defined in    milblob.h
#define M_FERET_MIN_DIAMETER                 14L         // Already defined in    milblob.h
#define M_FERET_MAX_DIAMETER                 16L         // Already defined in    milblob.h
#define M_FERET_MEAN_DIAMETER                18L         // Already defined in    milblob.h
#define M_FERET_ELONGATION                   27L         // Already defined in    milblob.h
#define M_NUMBER_OF_FERETS                   63L         // Already defined in    milblob.h
#define M_CONVEX_PERIMETER                   20L         // Already defined in    milblob.h
#define M_GENERAL_FERET                      0x400L      // Already defined in    milblob.h
#define M_LINE_FIT                           80L
#define M_CIRCLE_FIT                         25L
#define M_ELLIPSE_FIT                        97L
#define M_FERET_ANGLE_SEARCH_START           90L
#define M_FERET_ANGLE_SEARCH_END             92L
#define M_GENERAL_FERET_ANGLE                62L
#define M_ALL_FEATURES                       0x100L      // Already defined in    milblob.h
#define M_FIRST_FERET_INDEX                  640L
#define M_SECOND_FERET_INDEX                 257L
#define M_FERET_INDEX                        (M_FIRST_FERET_INDEX|M_SECOND_FERET_INDEX)
#define M_NEAREST_EDGEL_X                    37L
#define M_NEAREST_EDGEL_Y                    264L
#define M_NEAREST_EDGEL                      (M_NEAREST_EDGEL_X|M_NEAREST_EDGEL_Y)

/* Specific to M_CREST context behavior */
#define M_FOREGROUND_VALUE                   4L          // Already defined in    milblob.h, milmod.h,  milcal.h,  milcode.h, milocr.h
#define M_FOREGROUND_BLACK                   0x100L      // Already defined in    milblob.h, milmod.h,  milcode.h, milocr.h
#define M_FOREGROUND_WHITE                   0x80L       // Already defined in    milblob.h, milmod.h,  milcode.h, milocr.h
#define M_ANY                                0x11000000L // Already defined in    milmod.h,  milcode.h, milocr.h,  milmeas.h, mil.h
#define M_SAVE_CHAIN_WIDTH                   28L
#define M_TOPOLOGY                           28L
#define M_AVERAGE_WIDTH                      104L

/* Sorting keys */
#define M_SORT1_UP                           0x02000000L // Already defined in    milblob.h
#define M_SORT1_DOWN                         0x0A000000L // Already defined in    milblob.h
#define M_SORT2_UP                           0x04000000L // Already defined in    milblob.h
#define M_SORT2_DOWN                         0x0C000000L // Already defined in    milblob.h
#define M_SORT3_UP                           0x06000000L // Already defined in    milblob.h
#define M_SORT3_DOWN                         0x0E000000L // Already defined in    milblob.h
#define M_NO_SORT                            0L          // Already defined in    milblob.h

/* Statistic flags */
#define M_MIN                                0x02000000L // Already defined in    milmeas.h, mil.h
#define M_MAX                                0x04000000L // Already defined in    milmeas.h, mil.h
#define M_MEAN                               0x08000000L // Already defined in    milmeas.h, mil.h, milocr.h
#define M_MIN_ABS                            0x00200000L // Already defined in    mil.h
#define M_MAX_ABS                            0x00800000L // Already defined in    mil.h
#define M_STANDARD_DEVIATION                 0x10000000L // Already defined in    milmeas.h, mil.h

/* Global to result behavior */
#define M_DRAW_RELATIVE_ORIGIN_X             319L        // deprecated : Use MgraControl(... M_DRAW_OFFSET_X...)
#define M_DRAW_RELATIVE_ORIGIN_Y             320L        // deprecated : Use MgraControl(... M_DRAW_OFFSET_Y...)
#define M_DRAW_SCALE_X                       3203L       // deprecated : Use MgraControl(... M_DRAW_ZOOM_X ...)
#define M_DRAW_SCALE_Y                       3204L       // deprecated : Use MgraControl(... M_DRAW_ZOOM_Y ...)
#define M_DRAW_CROSS_SIZE                    115L
#define M_MODEL_FINDER_COMPATIBLE            19L
#define M_REGISTRATION_COMPATIBLE            81L
#define M_METROLOGY_COMPATIBLE               98L
#define M_CODE_READER_COMPATIBLE             99L
#define M_RESULT_OUTPUT_UNITS 	            1300L


/***************************************************************************/
/* MedgeInquire()                                                          */
/***************************************************************************/

/* To Context */
#define M_OWNER_SYSTEM                       1101L       // Already defined in    mil.h, milmod.h
#define M_MODIFICATION_COUNT                 5010L       // Already defined in    mil.h, milmod.h
#define M_CONTEXT_TYPE                       M_OBJECT_TYPE // Already defined in    mil.h, milmod.h
#define M_FILTER_POWER                       1049L
#define M_OVERSCAN                           53L         // Already defined in    mil.h, milmod.h
#define M_OVERSCAN_REPLACE_VALUE             54L         // Already defined in    mil.h, milmod.h
#define M_MIRROR                             0x01000061L // Already defined in    mil.h
#define M_REPLACE                            0x01000060L // Already defined in    mil.h, milocr.h

/***************************************************************************/
/* MedgeSelect()                                                           */
/***************************************************************************/
#define M_INCLUDE                            1L          // Already defined in    milblob.h
#define M_INCLUDE_ONLY                       0x101L      // Already defined in    milblob.h
#define M_EXCLUDE                            2L          // Already defined in    milblob.h
#define M_EXCLUDE_ONLY                       0x102L      // Already defined in    milblob.h
#define M_DELETE                             3L          // Already defined in    milblob.h, milmod.h
#define M_GROUP                              4L
#define M_UNGROUP                            5L
#define M_ALL_EDGES                          0x00200000L
#define M_INCLUDED_EDGES                     0x00400000L
#define M_EXCLUDED_EDGES                     0x00800000L
#define M_INCLUDED_EDGE                      M_INCLUDED_EDGES
#define M_EXCLUDED_EDGE                      M_EXCLUDED_EDGES

#define M_IN_RANGE                           1L          // Already defined in    milblob.h, mil.h
#define M_OUT_RANGE                          2L          // Already defined in    milblob.h, mil.h
#define M_EQUAL                              3L          // Already defined in    milblob.h, mil.h
#define M_NOT_EQUAL                          4L          // Already defined in    milblob.h, mil.h
#define M_GREATER                            5L          // Already defined in    milblob.h, mil.h
#define M_LESS                               6L          // Already defined in    milblob.h, mil.h
#define M_GREATER_OR_EQUAL                   7L          // Already defined in    milblob.h, mil.h
#define M_LESS_OR_EQUAL                      8L          // Already defined in    milblob.h, mil.h

#define M_INSIDE_BOX                         300L
#define M_INSIDE_CHAIN                       301L
#define M_OUTSIDE_BOX                        302L
#define M_OUTSIDE_CHAIN                      303L
#define M_EQUAL_CHAIN                        0x40
#define M_INSIDE_OR_EQUAL_BOX                (M_INSIDE_BOX|M_EQUAL_CHAIN)
#define M_INSIDE_OR_EQUAL_CHAIN              (M_INSIDE_CHAIN|M_EQUAL_CHAIN)
#define M_OUTSIDE_OR_EQUAL_BOX               (M_OUTSIDE_BOX|M_EQUAL_CHAIN)
#define M_OUTSIDE_OR_EQUAL_CHAIN             (M_OUTSIDE_CHAIN|M_EQUAL_CHAIN)
#define M_CROP_CHAIN                         304L
#define M_NEAREST_NEIGHBOR                   0x00000040L // Already defined in    milmeas.h, mil.h
#define M_ALL_NEAREST_NEIGHBORS              0x00000080L 
#define M_NEAREST_NEIGHBOR_RADIUS            116L
#define M_BOUNDARY                           M_NULL
#define M_PROXIMITY                          M_NULL
#define M_STATUS                             0x00008002L // Already defined in    milcode.h
#define M_FEATURE                            M_NULL

/***************************************************************************/
/* MedgeGetResult()                                                        */
/***************************************************************************/
#define M_LABEL_VALUE                        1L          // Already defined in    milblob.h
#define M_SPECIFIC_EDGE                      M_LABEL_VALUE
#define M_THRESHOLD_VALUES                   3003L
#define M_THRESHOLDS                         M_THRESHOLD_VALUES
#define M_NUMBER_OF_CHAINS                   0x769L
#define M_CHAIN_MAGNITUDE                    36L
#define M_CHAIN_ANGLE                        258L
#define M_IMAGE_ID                           0x04000000L    // Already defined in    milmod.h (0x04000000L)
#define M_MASK_ID                            (M_IMAGE_ID | 0x80000L)
#define M_ANGLE_ID                           0x20000000L
#define M_MAGNITUDE_ID                       0x8800L
#define M_FIRST_DERIVATIVE_X_ID              0x10000L
#define M_FIRST_DERIVATIVE_Y_ID              0x8000L
#define M_SECOND_DERIVATIVE_X_ID             0x4000L
#define M_SECOND_DERIVATIVE_Y_ID             0x1000L
#define M_CROSS_DERIVATIVE_ID                0x1000000L
#define M_FIRST_DERIVATIVES_ID               (M_FIRST_DERIVATIVE_X_ID  | M_FIRST_DERIVATIVE_Y_ID)
#define M_SECOND_DERIVATIVES_ID              (M_SECOND_DERIVATIVE_X_ID | M_SECOND_DERIVATIVE_Y_ID)
#define M_NUMBER_OF_CHAINED_PIXELS           56L         // Already defined in    milblob.h, milmod.h
#define M_NUMBER_OF_CHAINED_EDGELS           M_NUMBER_OF_CHAINED_PIXELS
#define M_CHAIN_CODE                         58L         // Already defined in    milblob.h
#define M_FIRST_POINT_X                      75L         // Already defined in    milblob.h
#define M_FIRST_POINT_Y                      132L
#define M_FIRST_POINT                        (M_FIRST_POINT_X|M_FIRST_POINT_Y)
#define M_CHAIN_INDEX                        67L         // Already defined in    milblob.h, milmod.h
#define M_CHAIN_X                            65L         // Already defined in    milblob.h, milmod.h
#define M_CHAIN_Y                            144L        // Already defined in    milblob.h, milmod.h
#define M_CHAIN                              209L        // Previously (M_CHAIN_X|M_CHAIN_Y), also defined in milblob.h
#define M_CHAINS                             M_CHAIN     // Already defined in    milblob.h
#define M_VERTICES_CHAIN_INDEX               71L
#define M_VERTICES_INDEX                     79L
#define M_VERTEX_X                           68L                                
#define M_VERTEX_Y                           40L
#define M_VERTEX                             (M_VERTEX_X|M_VERTEX_Y)
#define M_VERTICES_X                         M_VERTEX_X
#define M_VERTICES_Y                         M_VERTEX_Y
#define M_VERTICES                           M_VERTEX
#define M_VERTEX_ANGLE                       64L
#define M_VERTEX_MAGNITUDE                   63L
#define M_NUMBER_OF_VERTICES                 73L
#define M_NUMBER_OF_VERTEXES                 M_NUMBER_OF_VERTICES
#define M_NUMBER_OF_VERTEX                   M_NUMBER_OF_VERTICES
#define M_BULGE                              145L
#define M_BULGES                             M_BULGE
#define M_LINE_FIT_A                         82L
#define M_LINE_FIT_B                         83L
#define M_LINE_FIT_C                         31L
#define M_LINE_FIT_ERROR                     32L
#define M_LINE_FIT_COVERAGE                  49L
#define M_CIRCLE_FIT_CENTER_X                41L
#define M_CIRCLE_FIT_CENTER_Y                42L
#define M_CIRCLE_FIT_RADIUS                  43L
#define M_CIRCLE_FIT_ERROR                   47L
#define M_CIRCLE_FIT_COVERAGE                48L 
#define M_ELLIPSE_FIT_CENTER_X               85L
#define M_ELLIPSE_FIT_CENTER_Y               86L
#define M_ELLIPSE_FIT_MINOR_AXIS             87L
#define M_ELLIPSE_FIT_MAJOR_AXIS             88L
#define M_ELLIPSE_FIT_ANGLE                  89L
#define M_ELLIPSE_FIT_COVERAGE               96L
#define M_ELLIPSE_FIT_ERROR                  74L
#define M_EDGE_STATUS                        90L

/***************************************************************************/
/* MedgeGetNeighbors()                                                     */
/***************************************************************************/
#define M_NEIGHBOR_MINIMUM_SPACING           117L
#define M_NEIGHBOR_MAXIMUM_NUMBER            118L
#define M_NEIGHBOR_ANGLE                     122L
#define M_NEIGHBOR_ANGLE_TOLERANCE           123L
#define M_SEARCH_RADIUS_MAX                  120L
#define M_SEARCH_RADIUS_MIN                  121L
#define M_SEARCH_ANGLE                       0x00000100L // Already defines in    milpat.h, milcode.h
#define M_SEARCH_ANGLE_TOLERANCE             0x00000800L // Already defines in    milpat.h, milcode.h
#define M_SEARCH_ANGLE_SIGN                  126L
#define M_GET_EDGELS                         1L
#define M_GET_SUBEDGELS                      2L

// TEMPORARY BCKWARD SUPPORT UNTIL APPROVEMENT FORM TECH-WRITT
#define M_NEIGHBOR_POLARITY                          M_NEIGHBOR_ANGLE
#define M_NEIGHBOR_POLARITY_TOLERANCE                M_NEIGHBOR_ANGLE_TOLERANCE
#define M_NEIGHBOR_SEARCH_RADIUS_MAX                 M_SEARCH_RADIUS_MAX
#define M_NEIGHBOR_SEARCH_RADIUS_MIN                 M_SEARCH_RADIUS_MIN     
#define M_NEIGHBOR_DIRECTION                         M_SEARCH_ANGLE
#define M_NEIGHBOR_DIRECTION_TOLERANCE               M_SEARCH_ANGLE_TOLERANCE
#define M_NEIGHBOR_DIRECTION_SIGN                    M_SEARCH_ANGLE_SIGN


/***************************************************************************/
/* MedgeDraw()                                                             */
/***************************************************************************/
#define M_DRAW_LABEL                         0x00000001L
#define M_DRAW_IMAGE                         0x00000002L       // Already defines in    milmod.h, milpat.h
#define M_DRAW_EDGES                         0x00000004L       // Already defines in    milmod.h, milmeas.h 
#define M_DRAW_EDGE                          M_DRAW_EDGES
#define M_DRAW_FERET_MAX                     0x00000008L       // Already defined in    milblob.h
#define M_DRAW_FERET_BOX                     0x00000010L       // Already defined in    milblob.h
#define M_DRAW_BOX                           0x00000020L       // Already defined in    milblob.h, milmod.h, milmeas.h, milpat.h
#define M_DRAW_POSITION                      0x00000040L       // Already defined in    milblob.h, milmod.h, milmeas.h, milpat.h
#define M_DRAW_CENTER_OF_GRAVITY             0x00000080L       // Already defined in    milblob.h
#define M_DRAW_INDEX                         0x00000100L
#define M_DRAW_VERTICES                      0x00000400L
#define M_DRAW_VERTEXES                      M_DRAW_VERTICES
#define M_DRAW_VERTEX                        M_DRAW_VERTICES
#define M_DRAW_SEGMENTS                      0x00000800L
#define M_DRAW_FERET_MIN                     0x00001000L       // Already defined in    milblob.h
#define M_DRAW_GENERAL_FERET                 0x00100000L

#define M_DRAW_BUFFER_MASK                   0x3E00000L
#define M_DRAW_FIRST_DERIVATIVE_Y            0xA00000L
#define M_DRAW_MAGNITUDE                     0xE00000L
#define M_DRAW_SECOND_DERIVATIVE_X           0x1200000L
#define M_DRAW_SECOND_DERIVATIVE_Y           0x1600000L
#define M_DRAW_ANGLE                         0x1A00000L
#define M_DRAW_CROSS_DERIVATIVE              0x1E00000L
#define M_DRAW_MASK                          0x200000L
#define M_DRAW_CIRCLE_FIT                    0x20000L
#define M_DRAW_FIRST_DERIVATIVE_X            0x600000L
#define M_DRAW_EDGELS                        0x2000L
#define M_DRAW_ACTIVEMIL_INDEX               0x4000L
#define M_DRAW_VALUE                         0x8000L
#define M_DRAW_NEAREST_EDGELS                0x10000L
#define M_DRAW_LINE_FIT                      0x200L
#define M_DRAW_ELLIPSE_FIT                   0x40000L

/***************************************************************************/
/* MedgeStream                                                             */
/***************************************************************************/
#define M_SAVE                             0x100L        // Already defines in    milmod.h, milocr.h
#define M_RESTORE                           0x10L
#define M_LOAD                                 1L        // Already defines in    milmod.h, milmeas.h, mil.h
#define M_INQUIRE_SIZE_BYTE                    2L        // Already defines in    milmod.h

/***************************************************************************/
/* Complementary definitions                                               */
/***************************************************************************/
#define M_ENABLE                             -9997L      // Already defined in    milblob.h, milcal.h, milmeas.h, mil.h
#define M_DISABLE                            -9999L      // Already defined in    milblob.h, milcal.h, milmeas.h, mil.h
#define M_PACKED                             0x00020000L // Already defined in    mil.h
#define M_AVAILABLE                          MAKE_INT64(0x0000400000000000) // Also defined in mil.h
#define M_SUPPORTED                          MAKE_INT64(0x0000800000000000) // Also defined in mil.h
#define M_TYPE_LABEL                         0x01000000L
#define M_TYPE_INDEX                         0x02000000L
#define M_ALL                                0x40000000L // Already defined in    milmod.h, milmeas.h, milocr.h,  milpat.h,  mil.h
#define M_DEFAULT                            0x10000000L // Already defined in    milmod.h, milocr.h,  milcode.h, mil.h
#define M_TYPE_MIL_ANGLE                     4L
#define M_TYPE_ANGLE                         8L
#define M_TRUE                               1L          // Already defined in    mil.h, milmeas.h
#define M_FALSE                              0L          // Already defined in    mil.h, milmeas.h
#define M_NO_CHECK                           0x40000000L
#define M_SAME                               -1L         // Already defined in    milmod.h, milmeas.h, milocr.h
#define M_SAME_OR_REVERSE                    5L          // Already defined in    milmod.h
#define M_SAME_EDGE                          67L
#define M_REVERSE                            4L          // Already defined in    milmod.h, mil.h
#define M_ANY                                0x11000000L // Already defined in    milmod.h, milmeas.h, milcode.h, milocr.h, mil.h
#define M_AUTO                               444L        // Already defined in    milocr.h, milmod.h
#define M_SIZE_X                             1536L // Already defined in    milcode.h, mil.h
#define M_SIZE_Y                             1537L // Already defined in    milcode.h, mil.h
#define M_SIZE_BAND                          1005L       // Already defined in    mil.h
#define M_TYPE                               1008L       // Already defined in    mil.h
#define M_SIGN                               1014L       // Already defined in    mil.h
#define M_SIZE_BIT                           1007L       // Already defined in    mil.h
#define M_WORLD                              0x2000L     // Already defined in    milcal.h
#define M_FULL_ANGLE                         78L
#define M_SIZE_BYTE                          5061L       // Already defined in    milmod.h, milcode.h, mil.h
#define M_NATIVE_EDGE                        93L
#define M_INTERNAL                           0x000E0000L // Already defined in    mil.h
#define M_EXTERNAL                           0x00040000L // Already defined in    mil.h
#define M_UNKNOWN                            -9999L      // Already defined in    mil.h, milpat.h  
#define M_MEMORY                             (M_APP_INQUIRE_SYS_START+120L) /*5120*/ // Already defined in milmod.h, milcode.h, mil.h
#define M_EDGE_CONTOUR                       (M_EDGE_OBJECT | 0x00000001L) // Already defined in    mil.h
#define M_EDGE_CONTEXT                       (M_EDGE_OBJECT | 0x00000002L) // Already defined in    mil.h
#define M_EDGE_RESULT                        (M_EDGE_OBJECT | 0x00000004L) // Already defined in    mil.h
#define M_EDGE_RESULT_BUFFER                 M_EDGE_RESULT
#define M_INTERACTIVE                        M_NULL // Already defined in mil.h, milcal.h, milcode.h, milmeas.h, milocr.h, milpat.h, milmod.h

#define M_USE_MODULE_DRAW                    false

// Deprecated flag names.
#if OldDefinesSupport
   // #define       M_FILTER M_FILTER_TYPE already defined in mil.h
   // MIL_DEPRECATED(M_FILTER, 1000)       already defined in mil.h
#endif

/***************************************************************************/
/*                 MilEdge CAPI function prototypes                        */
/***************************************************************************/

/***************************************************************************/
/* MedgeAlloc()                                                            */
/***************************************************************************/
MIL_ID MFTYPE  MedgeAlloc         (MIL_ID       SystemId,
                                   MIL_INT64    EdgeFinderType,
                                   MIL_INT64    ControlFlag,
                                   MIL_ID*      ContextIdPtr);

/***************************************************************************/
/* MedgeAllocResult()                                                      */
/***************************************************************************/
MIL_ID MFTYPE  MedgeAllocResult   (MIL_ID       SystemId,
                                   MIL_INT64    ControlFlag,
                                   MIL_ID*      EdgeResultIdPtr);

/***************************************************************************/
/* MedgeControl()                                                          */
/***************************************************************************/
#if M_MIL_USE_64BIT
// Prototypes for 64 bits OSs
void MFTYPE    MedgeControlInt64  (MIL_ID       ContextOrResultId,
                                   MIL_INT64    ControlType,
                                   MIL_INT64    ControlValue);
void MFTYPE    MedgeControlDouble (MIL_ID       ContextOrResultId,
                                   MIL_INT64    ControlType,
                                   MIL_DOUBLE   ControlValue);
#else
// Prototypes for 32 bits OSs
#define MedgeControlInt64  MedgeControl
#define MedgeControlDouble MedgeControl
void MFTYPE    MedgeControl       (MIL_ID       ContextOrResultId,
                                   MIL_INT64    ControlType,
                                   MIL_DOUBLE   ControlValue);
#endif
/***************************************************************************/
/* MedgeInquire()                                                          */
/***************************************************************************/
MIL_INT MFTYPE    MedgeInquire       (MIL_ID       ContextOrResultId,
                                      MIL_INT64    InquireType,
                                      void*        UserVarPtr);

/***************************************************************************/
/* MedgeSelect()                                                           */
/***************************************************************************/
void MFTYPE    MedgeSelect        (MIL_ID       EdgeResultId,
                                   MIL_INT64    Operation,
                                   MIL_INT64    SelectionCriterion,
                                   MIL_INT64    Condition,
                                   MIL_DOUBLE   Param1,
                                   MIL_DOUBLE   Param2);

/***************************************************************************/
/* MedgeGetResult()                                                        */
/***************************************************************************/
void MFTYPE    MedgeGetResult     (MIL_ID       EdgeResultId,
                                   MIL_INT      EdgeIndexOrLabelValue,
                                   MIL_INT64    ResultType,
                                   void*        FirstResultArrayPtr,
                                   void*        SecondResultArrayPtr);

/***************************************************************************/
/* MedgeDraw()                                                             */
/***************************************************************************/
void MFTYPE    MedgeDraw          (MIL_ID       ContextGraId,
                                   MIL_ID       ResultEdgeId,
                                   MIL_ID       DstImageBufOrListGraId,
                                   MIL_INT64    Operation,
                                   MIL_INT      IndexOrLabel,
                                   MIL_INT64    ControlFlag);

/***************************************************************************/
/* MedgeCalculate()                                                        */
/***************************************************************************/
void MFTYPE    MedgeCalculate     (MIL_ID       ContextId,
                                   MIL_ID       SourceImageId,
                                   MIL_ID       SourceDeriv1Id,
                                   MIL_ID       SourceDeriv2Id,
                                   MIL_ID       SourceDeriv3Id,
                                   MIL_ID       EdgeResultId,
                                   MIL_INT64    ControlFlag);

/***************************************************************************/
/* MedgeMask()                                                             */
/***************************************************************************/
void MFTYPE    MedgeMask          (MIL_ID       ContextId,
                                   MIL_ID       MaskImageId,
                                   MIL_INT64    ControlFlag);

/***************************************************************************/
/* MedgeGetNeighbors()                                                     */
/***************************************************************************/
void MFTYPE    MedgeGetNeighbors  (MIL_ID            EdgeResultId,
                                   MIL_INT           SizeOfArray,
                                   const MIL_DOUBLE* SrcArrayXPtr,
                                   const MIL_DOUBLE* SrcArrayYPtr,
                                   const MIL_DOUBLE* SrcArrayAnglePtr,
                                   MIL_DOUBLE*       DstArrayXPtr,
                                   MIL_DOUBLE*       DstArrayYPtr,
                                   MIL_INT*          DstArrayIndexPtr,
                                   MIL_INT*          DstArrayLabelPtr,
                                   MIL_INT64         ControlFlag);

/***************************************************************************/
/* MedgePut()                                                              */
/***************************************************************************/
void MFTYPE    MedgePut          (MIL_ID            EdgeResultId,
                                  MIL_INT           NbEdgels,
                                  const MIL_INT*    ArrayIndexPtr,
                                  const MIL_DOUBLE* ArrayXPtr,
                                  const MIL_DOUBLE* ArrayYPtr,
                                  const MIL_DOUBLE* ArrayAnglePtr,
                                  const MIL_DOUBLE* ArrayMagnitudePtr,
                                  MIL_INT64         ControlFlag);

/***************************************************************************/
/* MedgeFree()                                                             */
/***************************************************************************/
void MFTYPE    MedgeFree          (MIL_ID       ObjectId);

#if M_MIL_USE_UNICODE

/***************************************************************************/
/* MedgeSave() Unicode                                                     */
/***************************************************************************/
void MFTYPE    MedgeSaveW         (MIL_CONST_TEXT_PTR FileName,
                                   MIL_ID       ContextOrResultId,
                                   MIL_INT64    ControlFlag);

/***************************************************************************/
/* MedgeRestore() Unicode                                                  */
/***************************************************************************/
MIL_ID  MFTYPE MedgeRestoreW      (MIL_CONST_TEXT_PTR FileName,
                                   MIL_ID       SystemId,
                                   MIL_INT64    ControlFlag,
                                   MIL_ID*      ContextIdPtr);

/***************************************************************************/
/* MedgeStream() Unicode                                                   */
/***************************************************************************/
void MFTYPE    MedgeStreamW      (MIL_TEXT_PTR MemPtrOrFileName,
                                  MIL_ID       SystemId,
                                  MIL_INT64    Operation,
                                  MIL_INT64    StreamType,
                                  MIL_DOUBLE   Version,
                                  MIL_INT64    ControlFlag,
                                  MIL_ID*      ContextOrResultIdPtr,
                                  MIL_INT*     SizeByteVarPtr);

/***************************************************************************/
/* MedgeSave() ASCII                                                       */
/***************************************************************************/
void MFTYPE    MedgeSaveA         (const char*  FileName,
                                   MIL_ID       ContextOrResultId,
                                   MIL_INT64    ControlFlag);

/***************************************************************************/
/* MedgeRestore() ASCII                                                    */
/***************************************************************************/
MIL_ID  MFTYPE MedgeRestoreA      (const char*  FileName,
                                   MIL_ID       SystemId,
                                   MIL_INT64    ControlFlag,
                                   MIL_ID*      ContextIdPtr);

/***************************************************************************/
/* MedgeStream() ASCII                                                     */
/***************************************************************************/
void MFTYPE    MedgeStreamA      (char*      MemPtrOrFileName,
                                  MIL_ID     SystemId,
                                  MIL_INT64  Operation,
                                  MIL_INT64  StreamType,
                                  MIL_DOUBLE Version,
                                  MIL_INT64  ControlFlag,
                                  MIL_ID*    ContextOrResultIdPtr,
                                  MIL_INT*   SizeByteVarPtr);

#if M_MIL_UNICODE_API

#define MedgeRestore MedgeRestoreW 
#define MedgeSave    MedgeSaveW 
#define MedgeStream  MedgeStreamW 

#else

#define MedgeRestore MedgeRestoreA
#define MedgeSave    MedgeSaveA
#define MedgeStream  MedgeStreamA

#endif //M_MIL_UNICODE_API

#else

/***************************************************************************/
/* MedgeSave()                                                             */
/***************************************************************************/
void MFTYPE    MedgeSave          (MIL_CONST_TEXT_PTR FileName,
                                   MIL_ID       ContextOrResultId,
                                   MIL_INT64    ControlFlag);

/***************************************************************************/
/* MedgeRestore()                                                          */
/***************************************************************************/
MIL_ID  MFTYPE MedgeRestore       (MIL_CONST_TEXT_PTR FileName,
                                   MIL_ID       SystemId,
                                   MIL_INT64    ControlFlag,
                                   MIL_ID*      ContextIdPtr);

/***************************************************************************/
/* MedgeStream()                                                           */
/***************************************************************************/
void MFTYPE    MedgeStream       (MIL_TEXT_PTR MemPtrOrFileName,
                                  MIL_ID       SystemId,
                                  MIL_INT64    Operation,
                                  MIL_INT64    StreamType,
                                  MIL_DOUBLE   Version,
                                  MIL_INT64    ControlFlag,
                                  MIL_ID*      ContextOrResultIdPtr,
                                  MIL_INT*     SizeByteVarPtr);

#endif


/*************/
/* Utilities */
/*************/
#define MedgeCalculateDefault(ContextId, SourceImageId, EdgeResultId)   MedgeCalculate(ContextId, SourceImageId, M_NULL, M_NULL, M_NULL, EdgeResultId, M_DEFAULT);

/* C++ directive if needed */
#ifdef __cplusplus
}
#endif
#if M_MIL_USE_64BIT
#ifdef __cplusplus
//////////////////////////////////////////////////////////////
// MedgeControl function definition when compiling c++ files
//////////////////////////////////////////////////////////////
#if !M_MIL_USE_LINUX
inline void MedgeControl (MIL_ID       ContextOrResultId, 
                          MIL_INT64      ControlType, 
                          int            ControlValue)
   {
   MedgeControlInt64(ContextOrResultId, ControlType, ControlValue);
   };
#endif

inline void MedgeControl (MIL_ID       ContextOrResultId, 
                          MIL_INT64       ControlType, 
                          MIL_INT32       ControlValue)
   {
   MedgeControlInt64(ContextOrResultId, ControlType, ControlValue);
   };

inline void MedgeControl (MIL_ID       ContextOrResultId, 
                          MIL_INT64       ControlType, 
                          MIL_INT64       ControlValue)
   {
   MedgeControlInt64(ContextOrResultId, ControlType, ControlValue);
   };

inline void MedgeControl (MIL_ID       ContextOrResultId, 
                          MIL_INT64    ControlType, 
                          MIL_DOUBLE   ControlValue)
   {
   MedgeControlDouble(ContextOrResultId, ControlType, ControlValue);
   };

#else
//////////////////////////////////////////////////////////////
// For C file, call the default function, i.e. Int64 one
//////////////////////////////////////////////////////////////
#define MedgeControl  MedgeControlDouble

#endif // __cplusplus
#endif // M_MIL_USE_64BIT

#if M_MIL_USE_SAFE_TYPE

//////////////////////////////////////////////////////////////
// See milos.h for explanation about these functions.
//////////////////////////////////////////////////////////////

// ----------------------------------------------------------
// MedgeGetResult

inline void MFTYPE MedgeGetResultUnsafe  (MIL_ID EdgeResultId, MIL_INT EdgeIndexOrLabelValue, MIL_INT64  ResultType, void            *FirstResultArrayPtr, void           *SecondResultArrayPtr);
inline void MFTYPE MedgeGetResultSafeType(MIL_ID EdgeResultId, MIL_INT EdgeIndexOrLabelValue, MIL_INT64  ResultType, int              FirstResultArrayPtr, int             SecondResultArrayPtr);
inline void MFTYPE MedgeGetResultSafeType(MIL_ID EdgeResultId, MIL_INT EdgeIndexOrLabelValue, MIL_INT64  ResultType, MIL_INT32       *FirstResultArrayPtr, MIL_INT32      *SecondResultArrayPtr);
inline void MFTYPE MedgeGetResultSafeType(MIL_ID EdgeResultId, MIL_INT EdgeIndexOrLabelValue, MIL_INT64  ResultType, MIL_INT64       *FirstResultArrayPtr, MIL_INT64      *SecondResultArrayPtr);
inline void MFTYPE MedgeGetResultSafeType(MIL_ID EdgeResultId, MIL_INT EdgeIndexOrLabelValue, MIL_INT64  ResultType, MIL_DOUBLE      *FirstResultArrayPtr, MIL_DOUBLE     *SecondResultArrayPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED                                                         
inline void MFTYPE MedgeGetResultSafeType(MIL_ID EdgeResultId, MIL_INT EdgeIndexOrLabelValue, MIL_INT64  ResultType, MIL_UINT32      *FirstResultArrayPtr, MIL_UINT32     *SecondResultArrayPtr);
inline void MFTYPE MedgeGetResultSafeType(MIL_ID EdgeResultId, MIL_INT EdgeIndexOrLabelValue, MIL_INT64  ResultType, MIL_UINT64      *FirstResultArrayPtr, MIL_UINT64     *SecondResultArrayPtr);
#endif

// ----------------------------------------------------------
// MedgeInquire

inline MIL_INT MFTYPE MedgeInquireUnsafe  (MIL_ID ContextOrResultId, MIL_INT64  InquireType, void         *UserVarPtr);
inline MIL_INT MFTYPE MedgeInquireSafeType(MIL_ID ContextOrResultId, MIL_INT64  InquireType, int           UserVarPtr);
inline MIL_INT MFTYPE MedgeInquireSafeType(MIL_ID ContextOrResultId, MIL_INT64  InquireType, MIL_INT32    *UserVarPtr);
inline MIL_INT MFTYPE MedgeInquireSafeType(MIL_ID ContextOrResultId, MIL_INT64  InquireType, MIL_INT64    *UserVarPtr);
inline MIL_INT MFTYPE MedgeInquireSafeType(MIL_ID ContextOrResultId, MIL_INT64  InquireType, MIL_DOUBLE   *UserVarPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MedgeInquireSafeType(MIL_ID ContextOrResultId, MIL_INT64  InquireType, MIL_UINT32   *UserVarPtr);
inline MIL_INT MFTYPE MedgeInquireSafeType(MIL_ID ContextOrResultId, MIL_INT64  InquireType, MIL_UINT64   *UserVarPtr);
#endif

// -------------------------------------------------------------------------
// MedgeGetResult

inline void MFTYPE MedgeGetResultSafeType(MIL_ID EdgeResultId, MIL_INT EdgeIndexOrLabelValue, MIL_INT64  ResultType, int FirstResultArrayPtr,int SecondResultArrayPtr)
   {
   if (SecondResultArrayPtr || FirstResultArrayPtr)
      SafeTypeError(MIL_TEXT("MedgeGetResult"));

   MedgeGetResult(EdgeResultId, EdgeIndexOrLabelValue, ResultType, NULL, NULL);
   }

inline void MedgeGetResultSafeTypeExecute (MIL_ID EdgeResultId, MIL_INT EdgeIndexOrLabelValue, MIL_INT64  ResultType, void           *FirstResultArrayPtr, void           *SecondResultArrayPtr, MIL_INT64  FirstGivenType, MIL_INT64  SecondGivenType)
   {
   MIL_INT64  RequiredType = ResultType & M_HLVLDATATYPE_MASK;
   if((RequiredType != M_TYPE_MIL_ID) && (RequiredType != M_TYPE_MIL_INT32) && (RequiredType != M_TYPE_MIL_INT64) && (RequiredType != M_TYPE_DOUBLE))
      RequiredType = 0;
   if (RequiredType == 0)
      RequiredType = M_TYPE_DOUBLE;
   ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

   if (RequiredType != FirstGivenType || RequiredType != SecondGivenType)
      SafeTypeError(MIL_TEXT("MedgeGetResult"));

   MedgeGetResult(EdgeResultId, EdgeIndexOrLabelValue, ResultType, FirstResultArrayPtr, SecondResultArrayPtr);
   }

inline void MFTYPE MedgeGetResultUnsafe  (MIL_ID EdgeResultId, MIL_INT EdgeIndexOrLabelValue, MIL_INT64  ResultType, void        *FirstResultArrayPtr, void        *SecondResultArrayPtr) {MedgeGetResult               (EdgeResultId, EdgeIndexOrLabelValue, ResultType, FirstResultArrayPtr, SecondResultArrayPtr);}
inline void MFTYPE MedgeGetResultSafeType(MIL_ID EdgeResultId, MIL_INT EdgeIndexOrLabelValue, MIL_INT64  ResultType, MIL_INT32   *FirstResultArrayPtr, MIL_INT32   *SecondResultArrayPtr) {MedgeGetResultSafeTypeExecute(EdgeResultId, EdgeIndexOrLabelValue, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, M_TYPE_MIL_INT32, M_TYPE_MIL_INT32);}
inline void MFTYPE MedgeGetResultSafeType(MIL_ID EdgeResultId, MIL_INT EdgeIndexOrLabelValue, MIL_INT64  ResultType, MIL_INT64   *FirstResultArrayPtr, MIL_INT64   *SecondResultArrayPtr) {MedgeGetResultSafeTypeExecute(EdgeResultId, EdgeIndexOrLabelValue, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, M_TYPE_MIL_INT64, M_TYPE_MIL_INT64);}
inline void MFTYPE MedgeGetResultSafeType(MIL_ID EdgeResultId, MIL_INT EdgeIndexOrLabelValue, MIL_INT64  ResultType, MIL_DOUBLE  *FirstResultArrayPtr, MIL_DOUBLE  *SecondResultArrayPtr) {MedgeGetResultSafeTypeExecute(EdgeResultId, EdgeIndexOrLabelValue, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, M_TYPE_DOUBLE   , M_TYPE_DOUBLE   );}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED                                                         
inline void MFTYPE MedgeGetResultSafeType(MIL_ID EdgeResultId, MIL_INT EdgeIndexOrLabelValue, MIL_INT64  ResultType, MIL_UINT32   *FirstResultArrayPtr, MIL_UINT32  *SecondResultArrayPtr) {MedgeGetResultSafeTypeExecute(EdgeResultId, EdgeIndexOrLabelValue, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, M_TYPE_MIL_INT32, M_TYPE_MIL_INT32);}
inline void MFTYPE MedgeGetResultSafeType(MIL_ID EdgeResultId, MIL_INT EdgeIndexOrLabelValue, MIL_INT64  ResultType, MIL_UINT64   *FirstResultArrayPtr, MIL_UINT64  *SecondResultArrayPtr) {MedgeGetResultSafeTypeExecute(EdgeResultId, EdgeIndexOrLabelValue, ResultType, FirstResultArrayPtr, SecondResultArrayPtr, M_TYPE_MIL_INT64, M_TYPE_MIL_INT64);}
#endif

// ----------------------------------------------------------
// MedgeInquire

inline MIL_INT MFTYPE MedgeInquireSafeType(MIL_ID ContextOrResultId, MIL_INT64  InquireType, int UserVarPtr)
   {
   if (UserVarPtr)
      SafeTypeError(MIL_TEXT("MedgeInquire"));

   return MedgeInquire(ContextOrResultId, InquireType, NULL);
   }

inline MIL_INT MFTYPE MedgeInquireSafeTypeExecute (MIL_ID ContextOrResultId, MIL_INT64  InquireType, void  *UserVarPtr, MIL_INT64  GivenType)
   {
   MIL_INT64  RequiredType = InquireType & M_HLVLDATATYPE_MASK;
   if((RequiredType != M_TYPE_MIL_ID) && (RequiredType != M_TYPE_MIL_INT32) && (RequiredType != M_TYPE_MIL_INT64) && (RequiredType != M_TYPE_DOUBLE))
      RequiredType = 0;
   if (RequiredType == 0)
      RequiredType = M_TYPE_DOUBLE;
   ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MedgeInquire"));

   return MedgeInquire(ContextOrResultId, InquireType, UserVarPtr);
   }

inline MIL_INT MFTYPE MedgeInquireUnsafe  (MIL_ID ContextOrResultId, MIL_INT64  InquireType, void        *UserVarPtr) {return MedgeInquire               (ContextOrResultId, InquireType, UserVarPtr                  );}
inline MIL_INT MFTYPE MedgeInquireSafeType(MIL_ID ContextOrResultId, MIL_INT64  InquireType, MIL_INT32   *UserVarPtr) {return MedgeInquireSafeTypeExecute(ContextOrResultId, InquireType, UserVarPtr, M_TYPE_MIL_INT32);}
inline MIL_INT MFTYPE MedgeInquireSafeType(MIL_ID ContextOrResultId, MIL_INT64  InquireType, MIL_INT64   *UserVarPtr) {return MedgeInquireSafeTypeExecute(ContextOrResultId, InquireType, UserVarPtr, M_TYPE_MIL_INT64);}
inline MIL_INT MFTYPE MedgeInquireSafeType(MIL_ID ContextOrResultId, MIL_INT64  InquireType, MIL_DOUBLE  *UserVarPtr) {return MedgeInquireSafeTypeExecute(ContextOrResultId, InquireType, UserVarPtr, M_TYPE_DOUBLE   );}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED                                
inline MIL_INT MFTYPE MedgeInquireSafeType(MIL_ID ContextOrResultId, MIL_INT64  InquireType, MIL_UINT32  *UserVarPtr) {return MedgeInquireSafeTypeExecute(ContextOrResultId, InquireType, UserVarPtr, M_TYPE_MIL_INT32);}
inline MIL_INT MFTYPE MedgeInquireSafeType(MIL_ID ContextOrResultId, MIL_INT64  InquireType, MIL_UINT64  *UserVarPtr) {return MedgeInquireSafeTypeExecute(ContextOrResultId, InquireType, UserVarPtr, M_TYPE_MIL_INT64);}
#endif

#define MedgeGetResult       MedgeGetResultSafeType
#define MedgeInquire         MedgeInquireSafeType

#else // #if M_MIL_USE_SAFE_TYPE

#define MedgeGetResultUnsafe       MedgeGetResult
#define MedgeInquireUnsafe         MedgeInquire

#endif // #if M_MIL_USE_SAFE_TYPE

#endif // !M_MIL_LITE

#endif /* __MILEDGEEXTR_H__ */
