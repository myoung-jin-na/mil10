/////////////////////////////////////////////////////////////////
//
// Filename          :  MILIM.H
// Revision          :  10.00.2564
//
// Copyright © Matrox Electronic Systems Ltd., 1992-2014.
// All Rights Reserved
//
/////////////////////////////////////////////////////////////////

#ifndef __MILIM_H__
#define __MILIM_H__

#define M_PROC_CURRENT_VERSION   M_PROC_VERSION_100

/************************************************************************/
/* MimAlloc()                                                           */
/************************************************************************/
#define M_DEINTERLACE_CONTEXT                         1L
#define M_THRESHOLD_CONTEXT                           2L
#define M_FLAT_FIELD_CONTEXT                          3L
#define M_DEAD_PIXEL_CONTEXT                          4L
#define M_REARRANGE_CONTEXT                           5L
#define M_STAT_MULTIPLE_CONTEXT                       6L
#define M_MATCH_CONTEXT                               7L
#define M_PROJECT_SHAPE_CONTEXT                       8L

/************************************************************************/
/* MimGetResult()                                                       */
/************************************************************************/
#define M_VALUE                                       0L
#define M_POSITION_X                                  0x00003400L
#define M_POSITION_Y                                  0x00004400L
#define M_NB_EVENT                                    5L
#define M_NUMBER                                      1009L    // Already defined elsewhere
#define M_TOTAL_NUMBER                                1010L    // Already defined elsewhere

/************************************************************************/
/* MimGetResult/MimHistogram()                                          */
/************************************************************************/
#define M_CUMULATIVE_VALUE                            1L
#define M_PERCENTAGE                                  2L // to be used as a complement to M_VALUE and M_CUMULATIVE_VALUE
#define M_PERCENTILE_VALUE                            0x10L // can also be used as a complement to M_GREATER/... with MimBinarize
#define M_PERCENTILE_VALUE_RESULT_SIZE                101L // 0 to 100%

/************************************************************************/
/* MimGetResult2d()/MimStatMultiple                                     */
/************************************************************************/
#define M_MAX                                         0x04000000L    //Already defined elsewhere
#define M_MAX_ABS                                     0x00800000L    //Already defined elsewhere
#define M_MEAN                                        0x08000000L    //Already defined elsewhere
#define M_MIN                                         0x02000000L    //Already defined elsewhere
#define M_MIN_ABS                                     0x00200000L    //Already defined elsewhere
#define M_STANDARD_DEVIATION                          0x10000000L    //Already defined elsewhere
#define M_SUM                                         0x00004000L    //Already defined elsewhere
#define M_SUM_ABS                                     0x00000008L    //Already defined elsewhere
#define M_SUM_OF_SQUARES                              0x00000400L    //Already defined elsewhere

/************************************************************************/
/* MimControl()/MimInquire()                                            */
/************************************************************************/
#define M_IN_IM_INQUIRE_DOUBLE_RANGE(X)               (((X) & M_MASK_MIM_INQUIRE_TYPE) == M_DARK_CONST || \
                                                       ((X) & M_MASK_MIM_INQUIRE_TYPE) == M_OFFSET_CONST || \
                                                       ((X) & M_MASK_MIM_INQUIRE_TYPE) == M_FLAT_CONST || \
                                                       ((X) & M_MASK_MIM_INQUIRE_TYPE) == M_GAIN_CONST || \
                                                       ((X) & M_MASK_MIM_INQUIRE_TYPE) == M_MAX_SCORE)

#define M_IN_IM_INQUIRE_MIL_INT64_RANGE(X)            (((X) & M_MASK_MIM_INQUIRE_TYPE) == M_EXTENDED_CONTEXT_TYPE || \
                                                       ((X) & M_MASK_MIM_INQUIRE_TYPE) == M_DEAD_PIXELS_IMAGE_ATTRIBUTE || \
                                                       ((X) & M_MASK_MIM_INQUIRE_TYPE) == M_MODEL_IMAGE_ATTRIBUTE || \
                                                       ((X) & M_MASK_MIM_INQUIRE_TYPE) == M_MASK_IMAGE_ATTRIBUTE)

#if M_MIL_USE_64BIT

#define M_IM_INQUIRE_MUST_HAVE_USER_PTR(X)            M_IN_IM_INQUIRE_DOUBLE_RANGE(X)

#else

#define M_IM_INQUIRE_MUST_HAVE_USER_PTR(X)            (M_IN_IM_INQUIRE_DOUBLE_RANGE(X) || M_IN_IM_INQUIRE_MIL_INT64_RANGE(X))

#endif

#define M_IN_IM_CONTROL_MIL_ID_RANGE(X)               ((X) == M_DARK_IMAGE || \
                                                       (X) == M_OFFSET_IMAGE || \
                                                       (X) == M_FLAT_IMAGE || \
                                                       (X) == M_DEAD_PIXELS || \
                                                       (X) == M_MODEL_IMAGE || \
                                                       (X) == M_MASK_IMAGE)

#define M_IM_CONTROL_ALLOW_NULL_ID(X)                 ((X) == M_DARK_IMAGE || \
                                                       (X) == M_OFFSET_IMAGE || \
                                                       (X) == M_FLAT_IMAGE || \
                                                       (X) == M_MASK_IMAGE)

// To avoid eventual clashes with M_TYPE_XXX, we must use the mask before checking the value itself.
#define M_MASK_MIM_INQUIRE_TYPE                       0x0001FFFF
#define M_RESULT_SIZE                                 0L
#define M_RESULT_TYPE                                 1L
#define M_DEINTERLACE_TYPE                            2L
#define M_DISCARD_FIELD                               3L
#define M_FIRST_FIELD                                 4L
#define M_MOTION_DETECT_NUM_FRAMES                    5L
#define M_MOTION_DETECT_THRESHOLD                     6L
#define M_MOTION_DETECT_REFERENCE_FRAME               7L
#define M_MOTION_DETECT_OUTPUT                        8L
#define M_SOURCE_FIRST_IMAGE                          9L
#define M_SOURCE_START_IMAGE                          M_SOURCE_FIRST_IMAGE
#define M_EXTENDED_CONTEXT_TYPE                       10L

// Inquire and control types for M_FLAT_FIELD_CONTEXT.
#define M_DARK_IMAGE                                  200L
#define M_OFFSET_IMAGE                                201L
#define M_FLAT_IMAGE                                  202L

#define M_DARK_CONST                                  203L
#define M_OFFSET_CONST                                204L
#define M_FLAT_CONST                                  205L
#define M_GAIN_CONST                                  206L

#define M_DARK_IMAGE_NB_BANDS                         207L
#define M_DARK_IMAGE_WIDTH                            208L
#define M_DARK_IMAGE_HEIGHT                           209L
#define M_DARK_IMAGE_TYPE                             210L

#define M_OFFSET_IMAGE_NB_BANDS                       212L
#define M_OFFSET_IMAGE_WIDTH                          213L
#define M_OFFSET_IMAGE_HEIGHT                         214L
#define M_OFFSET_IMAGE_TYPE                           215L

#define M_FLAT_IMAGE_NB_BANDS                         217L
#define M_FLAT_IMAGE_WIDTH                            218L
#define M_FLAT_IMAGE_HEIGHT                           219L
#define M_FLAT_IMAGE_TYPE                             220L

#define M_INVALID_CONST                               0x40000000L

// Inquire and control types for M_REARRANGE_CONTEXT.
#define M_X_SOURCE                                    300L
#define M_Y_SOURCE                                    301L
#define M_XY_SOURCE                                   302L
#define M_X_DESTINATION                               303L
#define M_Y_DESTINATION                               304L
#define M_XY_DESTINATION                              305L
#define M_X_SIZE                                      306L
#define M_Y_SIZE                                      307L
#define M_XY_SIZE                                     308L

#define M_X_SOURCE_ARRAY_SIZE                         309L
#define M_Y_SOURCE_ARRAY_SIZE                         310L
#define M_XY_SOURCE_ARRAY_SIZE                        311L
#define M_X_DESTINATION_ARRAY_SIZE                    312L
#define M_Y_DESTINATION_ARRAY_SIZE                    313L
#define M_XY_DESTINATION_ARRAY_SIZE                   314L
#define M_X_SIZE_ARRAY_SIZE                           315L
#define M_Y_SIZE_ARRAY_SIZE                           316L
#define M_XY_SIZE_ARRAY_SIZE                          317L

// Inquire and control types for M_DEAD_PIXEL_CONTEXT.
#define M_DEAD_PIXELS                                 80L
#define M_X_DEAD_PIXELS                               81L
#define M_Y_DEAD_PIXELS                               82L
#define M_XY_DEAD_PIXELS                              83L

#define M_X_DEAD_PIXELS_ARRAY_SIZE                    84L
#define M_Y_DEAD_PIXELS_ARRAY_SIZE                    85L
#define M_XY_DEAD_PIXELS_ARRAY_SIZE                   86L
#define M_DEAD_PIXELS_IMAGE_NB_BANDS                  87L      //Inquire only
#define M_DEAD_PIXELS_IMAGE_WIDTH                     88L      //Inquire only
#define M_DEAD_PIXELS_IMAGE_HEIGHT                    89L      //Inquire only
#define M_DEAD_PIXELS_IMAGE_TYPE                      91L      //Inquire only
#define M_DEAD_PIXELS_IMAGE_ATTRIBUTE                 92L      //Inquire only


// Inquire and control types for M_STAT_MULTIPLE_CONTEXT.
#define M_TYPE                                        1008L    //Already defined elsewhere
#define M_SIZE_X                                      1536L    //Already defined elsewhere
#define M_SIZE_Y                                      1537L    //Already defined elsewhere
#define M_2D                                          90L
   //See MimStat for M_TYPE possible values.

// Inquire and control types for M_MATCH_CONTEXT.
#define M_MODEL_IMAGE                                 1019L 
#define M_MASK_IMAGE                                  1020L       //Already defined in milreg.h
#define M_MAX_SCORE                                   100L        
#define M_SCORE_TYPE                                  37L         //Already defined elsewhere
#define M_MODEL_STEP                                  33L         //Already defined elsewhere
#define M_MODEL_SIZE_X                                101L        //Inquire only
#define M_MODEL_SIZE_Y                                102L        //Inquire only
#define M_MODE                                        103L        //Already defined elsewhere
#define M_MODEL_BUF_TYPE                              104L        //Inquire only
#define M_CORRELATE_NORMALIZED                        1L          //Mode value.
#define M_CORRELATE                                   2L          //Mode value.
#define M_ABS_SUM_OF_DIFFERENCES                      3L          //Mode value.
#define M_MAX_DEPTH                                   MIL_UINT32_MAX  //M_MAX_SCORE value.
#define M_NORM                                        4000L       //Already defined elsewhere, ScoreType value.
#define M_SQR_NORM                                    4001L       //Already defined elsewhere, ScoreType value.
#define M_NORM_CLIP                                   4002L       //ScoreType value.
#define M_NORM_CLIP_SQR                               4003L       //ScoreType value.

#define M_MODEL_IMAGE_NB_BANDS                        105L        //Inquire only
#define M_MODEL_IMAGE_WIDTH                           106L        //Inquire only
#define M_MODEL_IMAGE_HEIGHT                          107L        //Inquire only
#define M_MODEL_IMAGE_TYPE                            108L        //Inquire only
#define M_MODEL_IMAGE_ATTRIBUTE                       109L        //Inquire only

#define M_MASK_IMAGE_NB_BANDS                         110L        //Inquire only
#define M_MASK_IMAGE_WIDTH                            111L        //Inquire only
#define M_MASK_IMAGE_HEIGHT                           112L        //Inquire only
#define M_MASK_IMAGE_TYPE                             113L        //Inquire only
#define M_MASK_IMAGE_ATTRIBUTE                        114L        //Inquire only

//Common to many objects.
#define M_MODE                                        103L

// Already defined in another header file.
#define M_LINES                                       0x00000080L

#define M_RECTS                                       0x00000100L

#define M_AVERAGE                                     0x00000020L
#define M_DISCARD                                     0x00000021L
#define M_BOB                                         0x00000022L
#define M_ADAPTIVE_AVERAGE                            0x00000023L
#define M_ADAPTIVE_DISCARD                            0x00000024L
#define M_ADAPTIVE_BOB                                0x00000025L
#define M_ADAPTATIVE_AVERAGE                          M_ADAPTIVE_AVERAGE
#define M_ADAPTATIVE_DISCARD                          M_ADAPTIVE_DISCARD
#define M_ADAPTATIVE_BOB                              M_ADAPTIVE_BOB
#define M_ODD_FIELD                                   1L
#define M_EVEN_FIELD                                  2L
#define M_FIRST_FRAME                                 0x40000001L
#define M_CENTER_FRAME                                0x40000002L
#define M_LAST_FRAME                                  0x40000003L
#define M_RESULT_OUTPUT_UNITS 	                     1300L

// Control values for M_RESULT_OUTPUT_UNITS
#define M_PIXEL                              0x1000L
#define M_WORLD                              0x2000L
#define M_ACCORDING_TO_CALIBRATION             1301L
#define M_ACCORDING_TO_RESULT                  1302L
/************************************************************************/
/* MimFindExtreme()                                                     */
/************************************************************************/
#define M_MIN_VALUE                                   MAKE_INT64(0x0000010000000000)
#define M_MAX_VALUE                                   MAKE_INT64(0x0000020000000000)
#define M_MIN_ABS_VALUE                               4L
#define M_MAX_ABS_VALUE                               8L

/************************************************************************/
/* MimStat()                                                            */
/************************************************************************/
#define M_MIN_ABS                                     0x00200000L    //Already defined elsewhere
#define M_MAX_ABS                                     0x00800000L    //Already defined elsewhere
#define M_SUM_ABS                                     0x00000008L    //Already defined elsewhere
#define M_SUM_OF_SQUARES                              0x00000400L    //Already defined elsewhere
#define M_SUM                                         0x00004000L    //Already defined in milocr.h
#define M_MEAN                                        0x08000000L    //Already defined in milmeas.h
#define M_STANDARD_DEVIATION                          0x10000000L    //Already defined in milmeas.h
#define M_NUMBER                                      1009L          // Already defined elsewhere 0x000003F1
#define M_ANGULAR_DATA_MEAN                           0x00000004L
#define M_ANGULAR_DATA_COHERENCE                      0x00000800L
#define M_ALL_STAT                                    (M_MAX|M_MAX_ABS|M_MEAN|M_MIN|M_MIN_ABS|M_NUMBER|M_STANDARD_DEVIATION|M_SUM|M_SUM_ABS|M_SUM_OF_SQUARES)

/************************************************************************/
/* MimArith()                                                           */
/************************************************************************/
#define M_CONSTANT                                    0x8000L
#define M_FIXED_POINT                                 0x00004000L
#define M_FLOAT_PROC                                  0x40000000L
#define M_LOGICAL                                     0x08000000L
#define M_ADD                                         0x0000L
#define M_ADD_CONST                                   (M_ADD | M_CONSTANT)
#define M_SUB                                         0x0001L
#define M_SUB_CONST                                   (M_SUB | M_CONSTANT)
#define M_NEG_SUB                                     0x000AL
#define M_CONST_SUB                                   (M_NEG_SUB | M_CONSTANT)
#define M_SUB_ABS                                     0x0011L
#define M_MIN                                         0x02000000L
#define M_MIN_CONST                                   (M_MIN | M_CONSTANT)
#define M_MAX                                         0x04000000L
#define M_MAX_CONST                                   (M_MAX | M_CONSTANT)
#define M_OR                                          0x0016L
#define M_OR_CONST                                    (M_OR  | M_CONSTANT)
#define M_AND                                         0x0017L
#define M_AND_CONST                                   (M_AND | M_CONSTANT)
#define M_XOR                                         0x0018L
#define M_XOR_CONST                                   (M_XOR | M_CONSTANT)
#define M_NOR                                         0x0019L
#define M_NOR_CONST                                   (M_NOR | M_CONSTANT)
#define M_NAND                                        0x001AL
#define M_NAND_CONST                                  (M_NAND | M_CONSTANT)
#define M_XNOR                                        0x001BL
#define M_XNOR_CONST                                  (M_XNOR | M_CONSTANT)
#define M_NOT                                         0x0014L
#define M_NEG                                         0x0023L
#define M_ABS                                         0x000CL
#define M_PASS                                        0x0002L
#define M_CONST_PASS                                  (M_PASS | M_CONSTANT)
#define M_MULT                                        0x0100L
#define M_MULT_CONST                                  (M_MULT | M_CONSTANT)
#define M_DIV                                         0x0101L
#define M_DIV_CONST                                   (M_DIV | M_CONSTANT)
#define M_INV_DIV                                     0x0102L
#define M_CONST_DIV                                   (M_INV_DIV | M_CONSTANT)
#define M_SQRT                                        0x0103L
#define M_SQUARE_ROOT                                 M_SQRT
#define M_SQUARE                                      0x00000020L // Already defined in milmod.h
#define M_CUBE                                        0x0104L
#define M_LN                                          0x0105L
#define M_LOG10                                       0x0106L
#define M_LOG2                                        0x0107L
#define M_EXP_CONST                                   0x0108L
#define M_CONST_EXP                                   0x0109L
#define M_EXP                                         0x010AL  //Also defined in mil.h for MgenLutFunction
#define M_LOG_CONST                                   0x010BL
#define M_CONST_LOG                                   0x010CL
#define M_LOG                                         0x010DL  //Also defined in mil.h for MgenLutFunction

//Note that those are valid only with M_LOGICAL, avoiding conflict with other flags.
#define M_EQUAL                                       3L
#define M_NOT_EQUAL                                   4L
#define M_GREATER                                     5L
#define M_LESS                                        6L
#define M_GREATER_OR_EQUAL                            7L
#define M_LESS_OR_EQUAL                               8L

#define M_EQUAL_CONST                                 (M_EQUAL | M_CONSTANT)
#define M_NOT_EQUAL_CONST                             (M_NOT_EQUAL | M_CONSTANT)
#define M_GREATER_CONST                               (M_GREATER | M_CONSTANT)
#define M_LESS_CONST                                  (M_LESS | M_CONSTANT)
#define M_GREATER_OR_EQUAL_CONST                      (M_GREATER_OR_EQUAL | M_CONSTANT)
#define M_LESS_OR_EQUAL_CONST                         (M_LESS_OR_EQUAL | M_CONSTANT)

/************************************************************************/
/* MimArithMultiple()                                                   */
/************************************************************************/
#define M_OFFSET_GAIN                                 0x0000L
#define M_WEIGHTED_AVERAGE                            0x0001L
#define M_MULTIPLY_ACCUMULATE_1                       0x0002L
#define M_MULTIPLY_ACCUMULATE_2                       0x0004L

/************************************************************************/
/* MimFlip()                                                            */
/************************************************************************/
#define M_FLIP_VERTICAL                               1L
#define M_FLIP_HORIZONTAL                             2L

/************************************************************************/
/* MimBinarize(), MimClip(), MimLocateEvent(), MimStat()                */
/* Some of these defines are also define in milutil.h                   */
/************************************************************************/
#define M_IN_RANGE                                    1L
#define M_OUT_RANGE                                   2L
#define M_EQUAL                                       3L
#define M_NOT_EQUAL                                   4L
#define M_GREATER                                     5L
#define M_LESS                                        6L
#define M_GREATER_OR_EQUAL                            7L
#define M_LESS_OR_EQUAL                               8L
#define M_MASK                                        0x00001000L
#define M_ALL                                         0x40000000L
#define M_OPERATOR_MASK                               0x4000000FL

/************************************************************************/
/* These flags are specific to MimLocateEvent()                         */
/************************************************************************/

//The encoding for extremum mode is done on 11 bits, with the 8 lower bits
//indicating we use > or >= (0 or 1) for a specific neighbor, the bit 8 is used
//to select minimum or maximum (0 or 1) and bit 9 for the special case strickly
//greater than any one and greater or equal to all others. Bit 10 is indicating
//that one mode is selected. Note that for the neighbor position, we use the
//same convention than MimConnectMap.
// n3 n2 n1
// n4 X  n0
// n5 n6 n7
#define M_LOCATE_EVENT_EXTREMUM_MODE_SHIFT      17L
#define M_LOCAL_MAX_STRICT_LOW                  (0x0700L << M_LOCATE_EVENT_EXTREMUM_MODE_SHIFT) //Greater than at least one and greater or equal to the others.
#define M_LOCAL_MAX_STRICT_HIGH                 (0x0500L << M_LOCATE_EVENT_EXTREMUM_MODE_SHIFT) //Greater than all
#define M_LOCAL_MAX_NOT_STRICT                  (0x05FFL << M_LOCATE_EVENT_EXTREMUM_MODE_SHIFT) //Greater or equal to all
#define M_LOCAL_MAX_STRICT_MEDIUM               (0x0587L << M_LOCATE_EVENT_EXTREMUM_MODE_SHIFT) //Greater right
#define M_LOCAL_MIN_STRICT_LOW                  (0x0600L << M_LOCATE_EVENT_EXTREMUM_MODE_SHIFT) //Less than at least one and less or equal to the others.
#define M_LOCAL_MIN_STRICT_HIGH                 (0x0400L << M_LOCATE_EVENT_EXTREMUM_MODE_SHIFT) //Less than all
#define M_LOCAL_MIN_NOT_STRICT                  (0x04FFL << M_LOCATE_EVENT_EXTREMUM_MODE_SHIFT) //Less or equal to all
#define M_LOCAL_MIN_STRICT_MEDIUM               (0x0487L << M_LOCATE_EVENT_EXTREMUM_MODE_SHIFT) //Less right
#define M_LOCATE_EVENT_EXTREMUM_MODE_MASK       (0x07FFL << M_LOCATE_EVENT_EXTREMUM_MODE_SHIFT)
#define M_LOCATE_EVENT_EXTREMUM_MIN_OR_MAX_MASK (0x0100L << M_LOCATE_EVENT_EXTREMUM_MODE_SHIFT)

/************************************************************************/
/* MimBinarize()                                                        */
/************************************************************************/
// reserve bits 4-7 to the binarization method
#define M_BINARIZATION_METHOD_MASK                    0x0F0L
//#define M_PERCENTILE_VALUE                            0x10L // already defined at the beginning of the file
#define M_TRIANGLE_BISECTION_DARK                     0x20L
#define M_TRIANGLE_BISECTION_BRIGHT                   0x30L
#define M_BIMODAL                                     0x40L
#define M_FIXED                                       0x50L
#define M_DISCRETE_RANGE                              0x1000L
#define M_RETURN_VALUE_AS_FLOAT_IN_INT                0x2000L

/************************************************************************/
/* MimConvolve()                                                        */
/************************************************************************/
#define M_ID_OFFSET_OF_DEFAULT_KERNEL                 (0x00100000L|M_PSEUDO_ID)
#define M_ID_OFFSET_OF_EDGE_DETECTVAR                 (0x00100100L|M_PSEUDO_ID)
#define M_SMOOTH                                      (M_ID_OFFSET_OF_DEFAULT_KERNEL +  0L)
#define M_LAPLACIAN_EDGE                              (M_ID_OFFSET_OF_DEFAULT_KERNEL +  1L)
#define M_LAPLACIAN_EDGE2                             (M_ID_OFFSET_OF_DEFAULT_KERNEL +  2L)
#define M_SHARPEN                                     (M_ID_OFFSET_OF_DEFAULT_KERNEL +  3L)
#define M_SHARPEN2                                    (M_ID_OFFSET_OF_DEFAULT_KERNEL +  4L)
#define M_HORIZ_EDGE                                  (M_ID_OFFSET_OF_DEFAULT_KERNEL +  5L)
#define M_VERT_EDGE                                   (M_ID_OFFSET_OF_DEFAULT_KERNEL +  6L)
#define M_EDGE_DETECT                                 (M_ID_OFFSET_OF_DEFAULT_KERNEL +  7L)
#define M_EDGE_DETECT2                                (M_ID_OFFSET_OF_DEFAULT_KERNEL +  8L)
#define M_EDGE_DETECT_SQR                             (M_ID_OFFSET_OF_DEFAULT_KERNEL +  9L)
#define M_FIRST_DERIVATIVE_X                          (M_ID_OFFSET_OF_DEFAULT_KERNEL + 10L)
#define M_FIRST_DERIVATIVE_Y                          (M_ID_OFFSET_OF_DEFAULT_KERNEL + 11L)
#define M_SECOND_DERIVATIVE_X                         (M_ID_OFFSET_OF_DEFAULT_KERNEL + 12L)
#define M_SECOND_DERIVATIVE_Y                         (M_ID_OFFSET_OF_DEFAULT_KERNEL + 13L)
#define M_SECOND_DERIVATIVE_XY                        (M_ID_OFFSET_OF_DEFAULT_KERNEL + 14L)
#define M_EDGE_DETECTVAR(Val)                         (M_ID_OFFSET_OF_EDGE_DETECTVAR + Val)

#define M_OVERSCAN_DISABLE                            0x00080000L
#define M_OVERSCAN_FAST                               0x00040000L

#define M_SHEN_PREDEFINED_KERNEL                      (0x10000000L|M_ID_OFFSET_OF_DEFAULT_KERNEL)
#define M_DERICHE_PREDEFINED_KERNEL                   (0x20000000L|M_ID_OFFSET_OF_DEFAULT_KERNEL)
#define M_SHEN_PREDEFINED_KERNEL_INVALID_TYPE         (0x01000000L|M_SHEN_PREDEFINED_KERNEL)
#define M_SHEN_PREDEFINED_KERNEL_INVALID_FACTOR       (0x02000000L|M_SHEN_PREDEFINED_KERNEL)
#define M_DERICHE_PREDEFINED_KERNEL_INVALID_TYPE      (0x01000000L|M_DERICHE_PREDEFINED_KERNEL)
#define M_DERICHE_PREDEFINED_KERNEL_INVALID_FACTOR    (0x02000000L|M_DERICHE_PREDEFINED_KERNEL)

#define M_SHEN_FILTER(FilterOperation, FilterSmoothness)  (((((FilterOperation) & M_ID_OFFSET_OF_DEFAULT_KERNEL) != M_ID_OFFSET_OF_DEFAULT_KERNEL) && (FilterOperation!=M_DEFAULT))   ? M_SHEN_PREDEFINED_KERNEL_INVALID_TYPE :   \
   ((((FilterOperation) & (~(M_ID_OFFSET_OF_DEFAULT_KERNEL | 0xFF))) != 0) && (FilterOperation!=M_DEFAULT))                    ? M_SHEN_PREDEFINED_KERNEL_INVALID_TYPE :   \
   (((((FilterSmoothness) < 0) || ((FilterSmoothness) > 100))) && (FilterSmoothness!=M_DEFAULT))                               ? M_SHEN_PREDEFINED_KERNEL_INVALID_FACTOR : \
   M_SHEN_PREDEFINED_KERNEL | ((FilterOperation==M_DEFAULT) ? 0x80 : FilterOperation) | ((FilterSmoothness==M_DEFAULT)?(0xFF00):(((long) FilterSmoothness) << 8))          )

#define M_DERICHE_FILTER(FilterOperation, FilterSmoothness) (((((FilterOperation) & M_ID_OFFSET_OF_DEFAULT_KERNEL) != M_ID_OFFSET_OF_DEFAULT_KERNEL) && (FilterOperation!=M_DEFAULT)) ? M_DERICHE_PREDEFINED_KERNEL_INVALID_TYPE :   \
   ((((FilterOperation) & (~(M_ID_OFFSET_OF_DEFAULT_KERNEL | 0xFF))) != 0) && (FilterOperation!=M_DEFAULT))                  ? M_DERICHE_PREDEFINED_KERNEL_INVALID_TYPE :   \
   (((((FilterSmoothness) < 0) || ((FilterSmoothness) > 100))) && (FilterSmoothness!=M_DEFAULT))                             ? M_DERICHE_PREDEFINED_KERNEL_INVALID_FACTOR : \
   M_DERICHE_PREDEFINED_KERNEL | ((FilterOperation==M_DEFAULT) ? 0x80 : FilterOperation) | ((FilterSmoothness==M_DEFAULT)?(0xFF00):(((long) FilterSmoothness) << 8))        )

#define M_SHEN                               0x802L
#define M_DERICHE                            M_ID_OFFSET_OF_DEFAULT_KERNEL + 10L

/************************************************************************/
/* MimEdgeDetect()                                                      */
/************************************************************************/
#define M_SOBEL                                       M_EDGE_DETECT
#define M_PREWITT                                     M_EDGE_DETECT2
#define M_NOT_WRITE_ANGLE                             1L
#define M_NOT_WRITE_INT                               2L
#define M_FAST_ANGLE                                  4L
#define M_FAST_GRADIENT                               8L
#define M_FAST_EDGE_DETECT                            (M_FAST_ANGLE + M_FAST_GRADIENT)
#define M_REGULAR_ANGLE                               16L
#define M_REGULAR_GRADIENT                            64L
#define M_REGULAR_EDGE_DETECT                         (M_REGULAR_ANGLE + M_REGULAR_GRADIENT)
#define M_USE_RESERVED_ANGLE_VALUE                    128L

/************************************************************************/
/* MimRank()                                                            */
/************************************************************************/
#define M_MEDIAN                                      0x10000L
#define M_3X3_RECT                                    (M_ID_OFFSET_OF_DEFAULT_KERNEL + 20L)
#define M_3X3_CROSS                                   (M_ID_OFFSET_OF_DEFAULT_KERNEL + 21L)
#define M_5X5_RECT                                    (M_ID_OFFSET_OF_DEFAULT_KERNEL + 30L)

/************************************************************************/
/* MimMorphic(), ...                                                    */
/************************************************************************/
#define M_ERODE                                       1L
#define M_DILATE                                      2L
#define M_THIN                                        3L
#define M_THICK                                       4L
#define M_HIT_OR_MISS                                 5L
#define M_MATCH                                       6L
#define M_AREA_OPEN                                   7L
#define M_AREA_CLOSE                                  8L
#define M_TOP_HAT                                     9L
#define M_BOTTOM_HAT                                 10L
#define M_LEVEL                                      11L
#define M_HIT_THRESHOLD                              17L

// These flags are already defined elsewhere. We use the same values here.
#define M_OPEN                                       0x00000010L
#define M_CLOSE                                      0x00000020L

/************************************************************************/
/* MimErode()                                                           */
/************************************************************************/
#define M_BINARY_ULTIMATE                             1L

/************************************************************************/
/* MimThin()                                                            */
/************************************************************************/
#define M_TO_SKELETON                                -1L
#define M_BINARY2                                     1L
#define M_BINARY3                                     2L
#define M_OVERSCAN_REPLACE_MAX                        0x10
#define M_OVERSCAN_REPLACE_MIN                        0x20

/************************************************************************/
/* MimThick()                                                           */
/************************************************************************/
#define M_TO_IDEMPOTENCE                              M_TO_SKELETON

/************************************************************************/
/* MimDistance()                                                        */
/************************************************************************/
#define M_CHAMFER_3_4                                 0x01
#define M_CITY_BLOCK                                  0x02
#define M_CHESSBOARD                                  0x04
#define M_FORWARD                                     0x01
#define M_BACKWARD                                    0x02
#define M_OVERSCAN_TO_DO                              0x04
#define M_BOTH                                        0x07

/************************************************************************/
/* MimWatershed()                                                        */
/************************************************************************/
#define M_WATERSHED                                   0x0001
#define M_BASIN                                       0x0002
#define M_MINIMA_FILL                                 0x0004
#define M_MAXIMA_FILL                                 0x0008

#define M_4_CONNECTED                                 0x00000010L
#define M_8_CONNECTED                                 0x00000020L
#define M_REGULAR                                     0x00020000L
#define M_STRAIGHT_WATERSHED                          0x0080
#define M_SKIP_LAST_LEVEL                             0x0100
#define M_FILL_SOURCE                                 0x0200
#define M_LABELLED_MARKER                             0x0400
#define M_LABELED_MARKER                              M_LABELLED_MARKER

#define M_METHOD_1                                    0x1000
#define M_METHOD_2                                    0x2000

#define M_BASIN_4_CONNECTED                           0x4000
#define M_BASIN_8_CONNECTED                           0x8000

/************************************************************************/
/* MimProject()                                                         */
/************************************************************************/
#define M_0_DEGREE                                      0.0
#define M_90_DEGREE                                    90.0
#define M_180_DEGREE                                  180.0
#define M_270_DEGREE                                  270.0

/************************************************************************/
/* MimResize(), MimTranslate() and MimRotate()                          */
/************************************************************************/
#define M_OVERSCAN_DISABLE                            0x00080000L
#define M_OVERSCAN_ENABLE                             0x00008000L
#define M_OVERSCAN_FAST                               0x00040000L
#define M_INTERPOLATE                                 0x00000004L
#define M_BILINEAR                                    0x00000008L
#define M_BICUBIC                                     0x00000010L
#define M_AVERAGE                                     0x00000020L
#define M_NEAREST_NEIGHBOR                            0x00000040L
#define M_OVERSCAN_CLEAR                              0x00000080L
#define M_FIT_ALL_ANGLE                               0x00000100L
#define M_BINARY                                      0x00001000L
#define M_FAST                                        0x00002000L
#define M_REGULAR                                     0x00020000L

/************************************************************************/
/* MimResize                                                            */
/************************************************************************/
#define M_FILL_DESTINATION                            -1 // DO NOT MODIFY, WITHOUT CODE MODIFICATION

/************************************************************************/
/* MimHistogramEqualize()                                               */
/************************************************************************/
#define M_UNIFORM                                     1
#define M_EXPONENTIAL                                 2
#define M_RAYLEIGH                                    3
#define M_HYPER_CUBE_ROOT                             4
#define M_HYPER_LOG                                   5

/************************************************************************/
/* MimConvert()                                                         */
/************************************************************************/
#define M_REGULAR                                     0x00020000L
#define M_FAST                                        0x00002000L
#define M_MASK                                        0x00001000L

#define M_ID_OFFSET_OF_DEFAULT_CONVERT                (0x00200000L|M_PSEUDO_ID)

#define M_RGB_TO_HSL                                  (M_ID_OFFSET_OF_DEFAULT_CONVERT + 1L)     // Same as Mil.h
#define M_RGB_TO_HLS                                  M_RGB_TO_HSL                              // Obsolete
#define M_RGB_TO_L                                    (M_ID_OFFSET_OF_DEFAULT_CONVERT + 2L)     // Same as Mil.h
#define M_HSL_TO_RGB                                  (M_ID_OFFSET_OF_DEFAULT_CONVERT + 3L)     // Same as Mil.h
#define M_HLS_TO_RGB                                  M_HSL_TO_RGB                              // Obsolete
#define M_L_TO_RGB                                    (M_ID_OFFSET_OF_DEFAULT_CONVERT + 4L)     // Same as Mil.h
#define M_RGB_TO_Y                                    (M_ID_OFFSET_OF_DEFAULT_CONVERT + 5L)     // Same as Mil.h
#define M_RGB_TO_H                                    (M_ID_OFFSET_OF_DEFAULT_CONVERT + 6L)     // Same as Mil.h

#define M_RGB_TO_YUV16                                (M_ID_OFFSET_OF_DEFAULT_CONVERT + 101L)   // Same as Mil.h
#define M_RGB_TO_YCBCR                                (M_ID_OFFSET_OF_DEFAULT_CONVERT + 7L)
#define M_RGB_TO_YCRCB                                M_RGB_TO_YCBCR                            // Obsolete
#define M_RGB_TO_YCLIP                                (M_ID_OFFSET_OF_DEFAULT_CONVERT + 8L)
#define M_YUV16_TO_RGB                                (M_ID_OFFSET_OF_DEFAULT_CONVERT + 301L)   // Same as Mil.h
#define M_YCBCR_TO_RGB                                (M_ID_OFFSET_OF_DEFAULT_CONVERT + 9L)
#define M_YCRCB_TO_RGB                                M_YCBCR_TO_RGB                            // Obsolete

#define M_MATRIX_ID(x)                                (x)                                       // Obsolete from MIL 9

// If the conversion type is set to M_DEFAULT, the conversion will be
//  choosen according to the type of buffers passed to the function.
#define M_DEFAULT                                    0x10000000L

// These can be added to the conversion type to speed-up the
//   process ( M_FAST ) or have a better precision ( M_REGULAR ).
//   Not all conversion are affected by this flag.
#define M_REGULAR                                     0x00020000L
#define M_FAST                                        0x00002000L

/************************************************************************/
/* MimWarp()                                                            */
/************************************************************************/
/* 8 bits reserved for number of fractional bits */
#define M_WARP_MATRIX                                 0x00100000L
#define M_WARP_POLYNOMIAL                             0x00200000L
#define M_WARP_LUT                                    0x00400000L
#define M_OVERSCAN_ENABLE                             0x00008000L
#define M_OVERSCAN_DISABLE                            0x00080000L
#define M_OVERSCAN_FAST                               0x00040000L
#define M_OVERSCAN_CLEAR                              0x00000080L
#define M_FIXED_POINT                                 0x00004000L

/************************************************************************/
/* MimTransform()                                                       */
/************************************************************************/
#define M_FFT                                         1L
#define M_DCT8X8                                      2L
#define M_DCT8x8                                      2L  //Obsolete
#define M_DCT                                         3L
//#define M_FFT_OLD                                   4L  // Removed to flag any use by clients
#define M_POLAR                                       5L
#define M_FFT_DEPRECATED                              6L  // To be removed before next update

#define M_FORWARD                                     0x01
#define M_REVERSE                                     4L
#define M_NORMALIZED                                  0x00000002L
#define M_NORMALIZE                                   M_NORMALIZED
#define M_1D_ROWS                                     0x00000010L
#define M_1D_COLUMNS                                  0x00000020L
#define M_REAL                                        0x00000040L
#define M_CENTER                                      0x00000080L
#define M_MAGNITUDE                                   0x00000100L
#define M_PHASE                                       0x00000200L
#define M_LOG_SCALE                                   0x00000400L
#define M_FAST_PHASE                                  0x00000800L
#define M_SQUARE_MAGNITUDE                            0x00001000L
#define M_NORMALIZE_PHASE                             0x00002000L
#define M_NORMALIZE_PHASE_255                         0x00008000L
#define M_CLOCKWISE_PHASE                             0x00010000L
#define M_USE_CORDIC                                  0x00020000L
#define M_INTERNAL_FFT_NEW                            0x00000008L

/************************************************************************/
/* MimPolarTransform()                                                  */
/************************************************************************/
#define M_RECTANGULAR_TO_POLAR                        0x00000001L
#define M_POLAR_TO_RECTANGULAR                        0x00000002L

/************************************************************************/
/* MimDraw()                                                       */
/************************************************************************/
#define M_DRAW_PEAKS                                  0x01000000L
#define M_DRAW_DARK_IMAGE                             0x02000000L
#define M_DRAW_OFFSET_IMAGE                           0x04000000L
#define M_DRAW_FLAT_IMAGE                             0x08000000L
#define M_DRAW_XY_SOURCE                              0x10000000L
#define M_DRAW_XY_DESTINATION                         0x20000000L
#define M_DRAW_XY_SIZE                                0x40000000L
#define M_DOTS                                        0x00000040L
#define M_LINES                                       0x00000080L
#define M_1D_ROWS                                     0x00000010L    // Already defined elsewhere
#define M_1D_COLUMNS                                  0x00000020L    // Already defined elsewhere
#define M_DRAW_STAT_RESULT                            0x00000100L
#define M_DRAW_MODEL                                  0x00001000L
#define M_DRAW_MASK                                   0x200000L      // Already defined elsewhere
#define M_DRAW_PATH                                   0x00000200L
#define M_DRAW_DEAD_PIXELS                            0x00000400L


/************************************************************************/
/* MimStatMultiple()                                                    */
/************************************************************************/
#define M_CALCULATE                                   1L          // Already defined elsewhere
#define M_PREPROCESS                                  0x00000002L // Already defined elsewhere
#define M_REMOVE                                      0x00000004L
#define M_RESET_EXTREMES                               0x00000008L

/************************************************************************/
/* MimFlatField(), MimDeadPixelCorrection(), MimRearrange() and         */
/* MimStatMultiple()                                                    */
/************************************************************************/
#define M_PREPROCESS                                  0x00000002L

/************************************************************************/
/* MimProjectShape(), MgenLutWarp()                                     */
/************************************************************************/
#define M_OPTIMAL_SIZE_X                              0x1L
#define M_OPTIMAL_SIZE_Y                              0x2L
#define M_PATH_LIST                                   0x3L
#define M_X_PATH_LIST                                 0x4L
#define M_Y_PATH_LIST                                 0x5L
#define M_XY_PATH_LIST                                0x6L
#define M_PATH_WIDTH_LIST                             0x7L

/************************************************************************/
/* MimStatMultiple()                                                    */
/* Note that those should be temporary flags that should be replace     */
/* with more generic M_TYPE_XXX in MIL 10.0.                            */
/************************************************************************/
#define M_RESULT_TYPE_MIL_INT64                       0x00000100L
#define M_RESULT_TYPE_FLOAT                           0x00000200L
#define M_RESULT_TYPE_DOUBLE                          0x00000300L
#define M_RESULT_TYPE_MASK                            0x00000300L

#ifdef __cplusplus

inline MIL_INT64 ExtractTypeFromValue(MIL_INT64 Value,
                                      MIL_INT64 Type1,
                                      MIL_INT64 Type2 = 0,
                                      MIL_INT64 Type3 = 0,
                                      MIL_INT64 Type4 = 0,
                                      MIL_INT64 Type5 = 0,
                                      MIL_INT64 Type6 = 0,
                                      MIL_INT64 Type7 = 0,
                                      MIL_INT64 Type8 = 0)
   {
   MIL_INT64 Type = 0;

   Type = Value & M_HLVLDATATYPE_MASK;

   // No type specified.
   if (!Type)
      {
      goto End;
      }

   // If the extracted type doesn't match one of the allowed types, set it to 0.
   if (Type != Type1 &&
       Type != Type2 &&
       Type != Type3 &&
       Type != Type4 &&
       Type != Type5 &&
       Type != Type6 &&
       Type != Type7 &&
       Type != Type8)
      {
      Type = 0;
      }

   End:

   return Type;
   }

#endif

/* C++ directive if needed */
#ifdef __cplusplus
extern "C"
{
#endif


      /* -------------------------------------------------------------- */
      /* -------------------------------------------------------------- */

      /* POINT TO POINT : */

      /* -------------------------------------------------------------- */

#if M_MIL_USE_64BIT
   #define MimArith(Src1, Src2, Dest, Op) MimArithDouble(M_MILID_TO_DOUBLE(Src1), M_MILID_TO_DOUBLE(Src2), Dest, Op)
#else
   #define MimArithDouble MimArith
#endif
void MFTYPE    MimArithDouble       (MIL_DOUBLE Src1ImageBufIdOrConst,
                                     MIL_DOUBLE Src2ImageBufIdOrConst,
                                     MIL_ID DstImageBufId,
                                     MIL_INT64 Operation);

#if M_MIL_USE_64BIT
   #define MimArithMultiple(Src1, Src2, Src3, Src4, Src5, Dest, Op, OpFlag) MimArithMultipleDouble(M_MILID_TO_DOUBLE(Src1), M_MILID_TO_DOUBLE(Src2), M_MILID_TO_DOUBLE(Src3), Src4, Src5, Dest, Op, OpFlag)
#else
   #define MimArithMultipleDouble MimArithMultiple
#endif
void MFTYPE    MimArithMultipleDouble(MIL_DOUBLE Src1ImageBufId,
                                      MIL_DOUBLE Src2ImageBufIdOrConst,
                                      MIL_DOUBLE Src3ImageBufIdOrConst,
                                      MIL_DOUBLE Src4Const,
                                      MIL_DOUBLE Src5Const,
                                      MIL_ID DstImageBufId,
                                      MIL_INT64  Operation,
                                      MIL_INT64  OperationFlag);

void MFTYPE    MimLutMap            (MIL_ID SrcImageBufId,
                                     MIL_ID DstImageBufId,
                                     MIL_ID LutBufId);

void MFTYPE    MimShift             (MIL_ID SrcImageBufId,
                                     MIL_ID DstImageBufId,
                                     MIL_INT NbBitsToShift);

MIL_INT MFTYPE    MimBinarizeDouble    (MIL_ID     SrcImageBufId,
                                        MIL_ID     DstImageBufId,
                                        MIL_INT64  ConditionAndThreshMode,
                                        MIL_DOUBLE LowParam,
                                        MIL_DOUBLE HighParam);

void MFTYPE    MimClipDouble        (MIL_ID     SrcImageBufId,
                                     MIL_ID     DstImageBufId,
                                     MIL_INT64  Condition,
                                     MIL_DOUBLE CondLow,
                                     MIL_DOUBLE CondHigh,
                                     MIL_DOUBLE WriteLow,
                                     MIL_DOUBLE WriteHigh);

void MFTYPE    MimFlatField         (MIL_ID FlatFieldContextImId,
                                     MIL_ID SrcImageBufId,
                                     MIL_ID DstImageBufId,
                                     MIL_INT64 ControlFlag);


      /* -------------------------------------------------------------- */
      /* -------------------------------------------------------------- */

      /* NEIGHBOURHOOD : */

      /* -------------------------------------------------------------- */

void MFTYPE    MimDeinterlace       (MIL_ID DeinterlaceContextImId,
                                     const MIL_ID *SrcImageArrayPtr,
                                     const MIL_ID *DstImageArrayPtr,
                                     MIL_INT SrcImageCount,
                                     MIL_INT DstImageCount,
                                     MIL_INT64 ControlFlag);


      /* -------------------------------------------------------------- */
      /* -------------------------------------------------------------- */

      /* GEOMETRICAL: */

      /* -------------------------------------------------------------- */

void MFTYPE    MimResize             (MIL_ID     SrcImageBufId,
                                      MIL_ID     DstImageBufId,
                                      MIL_DOUBLE ScaleFactorX,
                                      MIL_DOUBLE ScaleFactorY,
                                      MIL_INT64  InterpolationMode);

void MFTYPE    MimFlip               (MIL_ID SrcImageBufId,
                                      MIL_ID DstImageBufId,
                                      MIL_INT64 Operation,
                                      MIL_INT64 OpFlag);

      /* -------------------------------------------------------------- */
      /* -------------------------------------------------------------- */

      /* STATISTICAL: */

      /* -------------------------------------------------------------- */

void MFTYPE    MimHistogram         (MIL_ID SrcImageBufId,
                                     MIL_ID HistResultImId);

void MFTYPE    MimHistogramEqualizeDouble(MIL_ID     SrcImageBufId,
                                          MIL_ID     DstImageBufId,
                                          MIL_INT64  Operation,
                                          MIL_DOUBLE Alpha,
                                          MIL_DOUBLE Min,
                                          MIL_DOUBLE Max);

void MFTYPE    MimFindExtreme       (MIL_ID SrcImageBufId,
                                     MIL_ID ExtremeResultImId,
                                     MIL_INT64 ExtremeType);

MIL_ID MFTYPE  MimAllocResult       (MIL_ID SysId,
                                     MIL_INT NbEntries,
                                     MIL_INT64 ResultType,
                                     MIL_ID* ResultImIdPtr);

void MFTYPE    MimFree              (MIL_ID ContextOrResultImId);

MIL_INT MFTYPE MimInquire           (MIL_ID ContextOrResultImId,
                                     MIL_INT64 InquireType,
                                     void* UserVarPtr);

void MFTYPE    MimGetResult2d       (MIL_ID ResultImId,
                                     MIL_INT OffsetX,
                                     MIL_INT OffsetY,
                                     MIL_INT SizeX,
                                     MIL_INT SizeY,
                                     MIL_INT64  ResultType,
                                     MIL_INT64  ControlFlag,
                                     void* UserArrayPtr);

void MFTYPE    MimGetResult1d       (MIL_ID ResultImId,
                                     MIL_INT OffEntry,
                                     MIL_INT NbEntries,
                                     MIL_INT64 ResultType,
                                     void* UserArrayPtr);

void MFTYPE    MimGetResult         (MIL_ID ResultImId,
                                     MIL_INT64 ResultType,
                                     void* UserArrayPtr);

MIL_ID MFTYPE  MimAlloc             (MIL_ID SysId,
                                     MIL_INT64 ContextType,
                                     MIL_INT64 ControlFlag,
                                     MIL_ID* ContextImIdPtr);

#if M_MIL_USE_64BIT
// Prototypes for 64 bits OSs

void MFTYPE    MimControlInt64      (MIL_ID ContextOrResultImId,
                                     MIL_INT64 ControlType,
                                     MIL_INT64 ControlValue);

void MFTYPE    MimControlDouble     (MIL_ID ContextOrResultImId,
                                     MIL_INT64 ControlType,
                                     MIL_DOUBLE ControlValue);
#else
// Prototypes for 32 bits OSs

#define MimControlDouble   MimControl
#define MimControlInt64    MimControl
void MFTYPE    MimControl           (MIL_ID     ContextOrResultImId,
                                     MIL_INT64  ControlType,
                                     MIL_DOUBLE ControlValue);
#endif

MIL_INT MFTYPE MimGet               (MIL_ID     ContextImId,
                                     MIL_INT64  GetType,
                                     MIL_INT    ArraySize,
                                     void *     Param1Ptr,
                                     void *     Param2Ptr,
                                     MIL_INT64  ControlFlag);

void MFTYPE    MimPut               (MIL_ID      ContextImId,
                                     MIL_INT64   PutType,
                                     MIL_INT     ArraySize,
                                     const void *Param1Ptr,
                                     const void *Param2Ptr,
                                     MIL_INT64   ControlFlag);

      /* -------------------------------------------------------------- */
      /* -------------------------------------------------------------- */

      /* CONVERSION: */

      /* -------------------------------------------------------------- */

void MFTYPE    MimConvert           (MIL_ID SrcImageBufId,
                                     MIL_ID DstImageBufId,
                                     MIL_ID ArrayBufIdOrConversionType);

      /* -------------------------------------------------------------- */
      /* -------------------------------------------------------------- */

      /* BUF: */

      /* -------------------------------------------------------------- */
void MFTYPE    MimRearrange         (MIL_ID RearrangeContextImId,
                                     MIL_ID SrcImageBufId,
                                     MIL_ID DstImageBufId,
                                     MIL_INT64 ControlFlag);
#if M_MIL_USE_UNICODE

MIL_ID MFTYPE MimRestoreA           (const char* FileName,
                                     MIL_ID      SysId,
                                     MIL_INT64   ControlFlag,
                                     MIL_ID*     ContextImIdPtr);
void MFTYPE MimSaveA                (const char* FileName,
                                     MIL_ID      ContextImId,
                                     MIL_INT64   ControlFlag);
void MFTYPE MimStreamA              (char*      MemPtrOrFileName,
                                     MIL_ID     SysId,
                                     MIL_INT64  Operation,
                                     MIL_INT64  StreamType,
                                     MIL_DOUBLE Version,
                                     MIL_INT64  ControlFlag,
                                     MIL_ID*    ContextImIdPtr,
                                     MIL_INT*   SizeByteVarPtr);

MIL_ID MFTYPE MimRestoreW           (MIL_CONST_TEXT_PTR FileName,
                                     MIL_ID SysId,
                                     MIL_INT64  ControlFlag,
                                     MIL_ID *ContextImIdPtr);
void MFTYPE MimSaveW                (MIL_CONST_TEXT_PTR FileName,
                                     MIL_ID ContextImId,
                                     MIL_INT64  ControlFlag);
void MFTYPE MimStreamW              (MIL_TEXT_PTR MemPtrOrFileName,
                                     MIL_ID       SysId,
                                     MIL_INT64    Operation,
                                     MIL_INT64    StreamType,
                                     MIL_DOUBLE   Version,
                                     MIL_INT64    ControlFlag,
                                     MIL_ID*      ContextImIdPtr,
                                     MIL_INT*     SizeByteVarPtr);

#if M_MIL_UNICODE_API
#define MimRestore   MimRestoreW
#define MimSave      MimSaveW
#define MimStream    MimStreamW
#else
#define MimRestore   MimRestoreA
#define MimSave      MimSaveA
#define MimStream    MimStreamA
#endif

#else

MIL_ID MFTYPE MimRestore            (MIL_CONST_TEXT_PTR FileName,
                                     MIL_ID SysId,
                                     MIL_INT64  ControlFlag,
                                     MIL_ID *ContextImIdPtr);

void MFTYPE MimSave                 (MIL_CONST_TEXT_PTR FileName,
                                     MIL_ID ContextImId,
                                     MIL_INT64  ControlFlag);

void MFTYPE MimStream               (MIL_TEXT_PTR  MemPtrOrFileName,
                                     MIL_ID        SysId,
                                     MIL_INT64     Operation,
                                     MIL_INT64     StreamType,
                                     MIL_DOUBLE    Version,
                                     MIL_INT64     ControlFlag,
                                     MIL_ID*       ContextImIdPtr,
                                     MIL_INT*      SizeByteVarPtr);
#endif   //#if M_MIL_USE_UNICODE

#if (!M_MIL_LITE)

      /* -------------------------------------------------------------- */
      /* -------------------------------------------------------------- */

      /* NEIGHBOURHOOD : */

      /* -------------------------------------------------------------- */

void MFTYPE    MimConvolve          (MIL_ID SrcImageBufId,
                                     MIL_ID DstImageBufId,
                                     MIL_ID KernelBufId);

void MFTYPE    MimRank              (MIL_ID SrcImageBufId,
                                     MIL_ID DstImageBufId,
                                     MIL_ID StructElemBufId,
                                     MIL_INT Rank,
                                     MIL_INT64 ProcMode);

void MFTYPE    MimEdgeDetectMIL_INT (MIL_ID SrcImageBufId,
                                     MIL_ID DstIntensityImageBufId,
                                     MIL_ID DstAngleImageBufId,
                                     MIL_ID KernelId,
                                     MIL_INT64 ControlFlag,
                                     MIL_INT Threshold);

void MFTYPE    MimMatch             (MIL_ID MatchContextImId,
                                     MIL_ID SrcImageBufId,
                                     MIL_ID DstImageBufId,
                                     MIL_INT64 ControlFlag);
      /* -------------------------------------------------------------- */
      /* -------------------------------------------------------------- */

      /* MORPHOLOGICAL: */

      /* -------------------------------------------------------------- */

void MFTYPE    MimLabel             (MIL_ID SrcImageBufId,
                                     MIL_ID DstImageBufId,
                                     MIL_INT64 ProcMode);

void MFTYPE    MimConnectMap        (MIL_ID SrcImageBufId,
                                     MIL_ID DstImageBufId,
                                     MIL_ID LutBufId);

void MFTYPE    MimDilate            (MIL_ID SrcImageBufId,
                                     MIL_ID DstImageBufId,
                                     MIL_INT NbIteration,
                                     MIL_INT64 ProcMode);

void MFTYPE    MimErode             (MIL_ID SrcImageBufId,
                                     MIL_ID DstImageBufId,
                                     MIL_INT NbIteration,
                                     MIL_INT64 ProcMode);

void MFTYPE    MimClose             (MIL_ID SrcImageBufId,
                                     MIL_ID DstImageBufId,
                                     MIL_INT NbIteration,
                                     MIL_INT64 ProcMode);

void MFTYPE    MimOpen              (MIL_ID SrcImageBufId,
                                     MIL_ID DstImageBufId,
                                     MIL_INT NbIteration,
                                     MIL_INT64 ProcMode);

void MFTYPE    MimMorphic           (MIL_ID SrcImageBufId,
                                     MIL_ID DstImageBufId,
                                     MIL_ID StructElemBufId,
                                     MIL_INT64 Operation,
                                     MIL_INT NbIterationOrArea,
                                     MIL_INT64 ProcMode);

void MFTYPE    MimThin              (MIL_ID SrcImageBufId,
                                     MIL_ID DstImageBufId,
                                     MIL_INT NbIteration,
                                     MIL_INT64 ProcMode);

void MFTYPE    MimThick             (MIL_ID SrcImageBufId,
                                     MIL_ID DstImageBufId,
                                     MIL_INT NbIteration,
                                     MIL_INT64 ProcMode);

void MFTYPE    MimDistance          (MIL_ID SrcImageBufId,
                                     MIL_ID DstImageBufId,
                                     MIL_INT64 DistanceTransform);

void MFTYPE    MimWatershed         (MIL_ID SrcImageBufId,
                                     MIL_ID MarkerImageBufId,
                                     MIL_ID DstImageBufId,
                                     MIL_INT MinVariation,
                                     MIL_INT64 ControlFlag);

void MFTYPE    MimZoneOfInfluence   (MIL_ID SrcImageBufId,
                                     MIL_ID DstImageBufId,
                                     MIL_INT64 OperationFlag);

void MFTYPE    MimDeadPixelCorrection(MIL_ID DeadPixelContextImId,
                                      MIL_ID SrcImageBufId,
                                      MIL_ID DstImageBufId,
                                      MIL_INT64 ControlFlag);

      /* -------------------------------------------------------------- */
      /* -------------------------------------------------------------- */

      /* GEOMETRICAL: */

      /* -------------------------------------------------------------- */

void MFTYPE    MimRotate             (MIL_ID     SrcImageBufId,
                                      MIL_ID     DstImageBufId,
                                      MIL_DOUBLE Angle,
                                      MIL_DOUBLE SrcCenX,
                                      MIL_DOUBLE SrcCenY,
                                      MIL_DOUBLE DstCenX,
                                      MIL_DOUBLE DstCenY,
                                      MIL_INT64  InterpolationMode);

void MFTYPE    MimTranslate          (MIL_ID     SrcImageBufId,
                                      MIL_ID     DstImageBufId,
                                      MIL_DOUBLE DisplacementX,
                                      MIL_DOUBLE DisplacementY,
                                      MIL_INT64  InterpolationMode);

void MFTYPE    MimWarp               (MIL_ID SrcImageBufId,
                                      MIL_ID DstImageBufId,
                                      MIL_ID WarpParam1BufId,
                                      MIL_ID WarpParam2BufId,
                                      MIL_INT64 OperationMode,
                                      MIL_INT64 InterpolationMode);

void MFTYPE    MimPolarTransform (MIL_ID      SrcImageBufId,
                                  MIL_ID      DstImageBufId,
                                  MIL_DOUBLE  CenterPosX,
                                  MIL_DOUBLE  CenterPosY,
                                  MIL_DOUBLE  StartRadius,
                                  MIL_DOUBLE  EndRadius,
                                  MIL_DOUBLE  StartAngle,
                                  MIL_DOUBLE  EndAngle,
                                  MIL_INT64   OperationMode,
                                  MIL_INT64   InterpolationMode,
                                  MIL_DOUBLE *DstSizeXPtr,
                                  MIL_DOUBLE *DstSizeYPtr);

      /* -------------------------------------------------------------- */
      /* -------------------------------------------------------------- */

      /* STATISTICAL: */

      /* -------------------------------------------------------------- */

void MFTYPE    MimProject           (MIL_ID SrcImageBufId,
                                     MIL_ID ProjResultImId,
                                     MIL_DOUBLE ProjectionAngle);

#if M_MIL_USE_64BIT
   #define MimStat(Src, Result, Type, Condition, CondLow, CondHigh) MimStatDouble(Src, Result, Type, Condition, M_MILID_TO_DOUBLE(CondLow), CondHigh)
#else
   #define MimStatDouble MimStat
#endif
void MFTYPE    MimStatDouble        (MIL_ID SrcImageBufId,
                                     MIL_ID StatResultImId,
                                     MIL_INT64 StatType,
                                     MIL_INT64 Condition,
                                     MIL_DOUBLE CondLow,
                                     MIL_DOUBLE CondHigh);

void MFTYPE    MimStatMultiple      (MIL_ID StatMultipleContextImId,
                                     MIL_ID SrcImageBufId,
                                     MIL_ID StatMultipleResultImId,
                                     MIL_INT64 ControlFlag);

MIL_INT MFTYPE MimLocateEventDouble (MIL_ID     SrcImageBufId,
                                     MIL_ID     EventResultImId,
                                     MIL_INT64  Condition,
                                     MIL_DOUBLE CondLow,
                                     MIL_DOUBLE CondHigh);

void MFTYPE    MimLocatePeak1dDouble(MIL_ID     SrcImageBufId,
                                     MIL_ID     DstPositionImageBufId,
                                     MIL_ID     DstIntensityImageBufId,
                                     MIL_INT    DstY,
                                     MIL_INT    PeakWidth,
                                     MIL_DOUBLE MinIntensity,
                                     MIL_INT64  ControlFlag,
                                     MIL_DOUBLE ControlValue);

MIL_INT MFTYPE MimCountDifference   (MIL_ID Src1ImageBufId,
                                     MIL_ID Src2ImageBufId,
                                     MIL_ID CountResultImId);

      /* -------------------------------------------------------------- */
      /* -------------------------------------------------------------- */

      /* TRANSFORM: */

      /* -------------------------------------------------------------- */

void MFTYPE MimTransform            (MIL_ID SrcImageRBufId,
                                     MIL_ID SrcImageIBufId,
                                     MIL_ID DstImageRBufId,
                                     MIL_ID DstImageIBufId,
                                     MIL_INT64 TransformType,
                                     MIL_INT64 ControlFlag);

      /* -------------------------------------------------------------- */
      /* -------------------------------------------------------------- */

      /* DRAW: */

      /* -------------------------------------------------------------- */

void MFTYPE MimDrawDouble           (MIL_ID     ContextGraId,
                                     MIL_ID     Src1MilId,
                                     MIL_ID     Src2MilId,
                                     MIL_ID     DstImageBufOrListGraId,
                                     MIL_INT64  Operation,
                                     MIL_DOUBLE Param1,
                                     MIL_DOUBLE Param2,
                                     MIL_INT64  ControlFlag);

#endif // (!MIL_LITE)


#ifdef __cplusplus
} // extern "C"
#endif

#if M_MIL_USE_64BIT
#ifdef __cplusplus
//////////////////////////////////////////////////////////////
// MbufControl function definition when compiling c++ files
//////////////////////////////////////////////////////////////
#if !M_MIL_USE_LINUX
inline void MimControl(MIL_ID ContextOrResultImId,
                       MIL_INT64 ControlType,
                       int ControlValue)
   {
   MimControlInt64(ContextOrResultImId, ControlType, ControlValue);
   };
#endif

inline void MimControl(MIL_ID ContextOrResultImId,
                       MIL_INT64 ControlType,
                       MIL_INT32 ControlValue)
   {
   MimControlInt64(ContextOrResultImId, ControlType, ControlValue);
   };

inline void MimControl(MIL_ID ContextOrResultImId,
                       MIL_INT64 ControlType,
                       MIL_INT64 ControlValue)
   {
   MimControlInt64(ContextOrResultImId, ControlType, ControlValue);
   };

inline void MimControl(MIL_ID ContextOrResultImId,
                       MIL_INT64 ControlType,
                       MIL_DOUBLE ControlValue)
   {
   MimControlDouble(ContextOrResultImId, ControlType, ControlValue);
   };

#else
//////////////////////////////////////////////////////////////
// For C file, call the default function, i.e. Int64 one
//////////////////////////////////////////////////////////////
#define MimControl  MimControlInt64

#endif // __cplusplus
#endif // M_MIL_USE_64BIT

#if(defined(__cplusplus))

//////////////////////////////////////////////////////////////
// MbufBinarize function definition when compiling c++ files
//////////////////////////////////////////////////////////////
inline MIL_INT MFTYPE MimBinarize(MIL_ID     SrcImageBufId,
                                  MIL_ID     DstImageBufId,
                                  MIL_INT64  ConditionAndThreshMode,
                                  MIL_DOUBLE LowParam,
                                  MIL_DOUBLE HighParam)
   {
   return   MimBinarizeDouble(SrcImageBufId, DstImageBufId, ConditionAndThreshMode, LowParam, HighParam);
   }

inline void MFTYPE MimClip(MIL_ID     SrcImageBufId,
                           MIL_ID     DstImageBufId,
                           MIL_INT64  Condition,
                           MIL_DOUBLE CondLow,
                           MIL_DOUBLE CondHigh,
                           MIL_DOUBLE WriteLow,
                           MIL_DOUBLE WriteHigh)
   {
   MimClipDouble(SrcImageBufId, DstImageBufId, Condition, CondLow, CondHigh, WriteLow, WriteHigh);
   }

inline void MFTYPE MimHistogramEqualize(MIL_ID      SrcImageBufId,
                                        MIL_ID      DstImageBufId,
                                        MIL_INT64   EqualizationType,
                                        MIL_DOUBLE  Alpha,
                                        MIL_DOUBLE  Min,
                                        MIL_DOUBLE  Max)
   {
   MimHistogramEqualizeDouble(SrcImageBufId, DstImageBufId, EqualizationType, Alpha, Min, Max);
   }

#if (!M_MIL_LITE)

inline void MFTYPE MimDraw(MIL_ID     ContextGraId,
                           MIL_ID     Src1MilId,
                           MIL_ID     Src2MilId,
                           MIL_ID     DstImageBufOrListGraId,
                           MIL_INT64  Operation,
                           MIL_DOUBLE Param1,
                           MIL_DOUBLE Param2,
                           MIL_INT64  ControlFlag)
   {
   MimDrawDouble(ContextGraId, Src1MilId, Src2MilId, DstImageBufOrListGraId, Operation, Param1, Param2, ControlFlag);
   }

inline void MFTYPE MimEdgeDetect(MIL_ID SrcImageBufId,
                                 MIL_ID DstIntensityImageBufId,
                                 MIL_ID DstAngleImageBufId,
                                 MIL_ID KernelId,
                                 MIL_INT64 ControlFlag,
                                 MIL_INT Threshold)
   {
   MimEdgeDetectMIL_INT(SrcImageBufId, DstIntensityImageBufId, DstAngleImageBufId, KernelId, ControlFlag, Threshold);
   }

inline MIL_INT MFTYPE MimLocateEvent(MIL_ID     SrcImageBufId,
                                     MIL_ID     EventResultImId,
                                     MIL_INT64  Condition,
                                     MIL_DOUBLE CondLow,
                                     MIL_DOUBLE CondHigh)
   {
   return MimLocateEventDouble(SrcImageBufId, EventResultImId, Condition, CondLow, CondHigh);
   }

inline void MFTYPE MimLocatePeak1d(MIL_ID     SrcImageBufId,
                                   MIL_ID     DstPositionImageBufId,
                                   MIL_ID     DstIntensityImageBufId,
                                   MIL_INT    DstY,
                                   MIL_INT    PeakWidth,
                                   MIL_DOUBLE MinIntensity,
                                   MIL_INT64  ControlFlag,
                                   MIL_DOUBLE ControlValue)
   {
   MimLocatePeak1dDouble(SrcImageBufId, DstPositionImageBufId, DstIntensityImageBufId, DstY, PeakWidth, MinIntensity, ControlFlag, ControlValue);
   }

#endif

#else //#if(!defined(__cplusplus))

#define  MimBinarize                   MimBinarizeDouble
#define  MimClip                       MimClipDouble
#define  MimHistogramEqualize          MimHistogramEqualizeDouble

#if (!M_MIL_LITE)

#define  MimDraw                       MimDrawDouble
#define  MimEdgeDetect                 MimEdgeDetectMIL_INT
#define  MimLocateEvent                MimLocateEventDouble
#define  MimLocatePeak1d               MimLocatePeak1dDouble

#endif

#endif   //#if(defined(__cplusplus))

#if M_MIL_USE_SAFE_TYPE

//////////////////////////////////////////////////////////////
// See milos.h for explanation about these functions.
//////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------
// MimGetResult

inline void MFTYPE MimGetResultUnsafe  (MIL_ID ResultImId, MIL_INT64  ResultType, void           *UserArrayPtr);
inline void MFTYPE MimGetResultSafeType(MIL_ID ResultImId, MIL_INT64  ResultType, int             UserArrayPtr);
inline void MFTYPE MimGetResultSafeType(MIL_ID ResultImId, MIL_INT64  ResultType, MIL_INT32      *UserArrayPtr);
inline void MFTYPE MimGetResultSafeType(MIL_ID ResultImId, MIL_INT64  ResultType, MIL_INT64      *UserArrayPtr);
inline void MFTYPE MimGetResultSafeType(MIL_ID ResultImId, MIL_INT64  ResultType, float          *UserArrayPtr);
inline void MFTYPE MimGetResultSafeType(MIL_ID ResultImId, MIL_INT64  ResultType, MIL_DOUBLE     *UserArrayPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline void MFTYPE MimGetResultSafeType(MIL_ID ResultImId, MIL_INT64  ResultType, MIL_UINT32     *UserArrayPtr);
inline void MFTYPE MimGetResultSafeType(MIL_ID ResultImId, MIL_INT64  ResultType, MIL_UINT64     *UserArrayPtr);
#endif

//-------------------------------------------------------------------------------------
// MimGetResult1d

inline void MFTYPE MimGetResult1dUnsafe  (MIL_ID ResultImId, MIL_INT OffEntry, MIL_INT NbEntries, MIL_INT64  ResultType, void           *UserArrayPtr);
inline void MFTYPE MimGetResult1dSafeType(MIL_ID ResultImId, MIL_INT OffEntry, MIL_INT NbEntries, MIL_INT64  ResultType, int             UserArrayPtr);
inline void MFTYPE MimGetResult1dSafeType(MIL_ID ResultImId, MIL_INT OffEntry, MIL_INT NbEntries, MIL_INT64  ResultType, MIL_INT32      *UserArrayPtr);
inline void MFTYPE MimGetResult1dSafeType(MIL_ID ResultImId, MIL_INT OffEntry, MIL_INT NbEntries, MIL_INT64  ResultType, MIL_INT64      *UserArrayPtr);
inline void MFTYPE MimGetResult1dSafeType(MIL_ID ResultImId, MIL_INT OffEntry, MIL_INT NbEntries, MIL_INT64  ResultType, float          *UserArrayPtr);
inline void MFTYPE MimGetResult1dSafeType(MIL_ID ResultImId, MIL_INT OffEntry, MIL_INT NbEntries, MIL_INT64  ResultType, MIL_DOUBLE     *UserArrayPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline void MFTYPE MimGetResult1dSafeType(MIL_ID ResultImId, MIL_INT OffEntry, MIL_INT NbEntries, MIL_INT64  ResultType, MIL_UINT32     *UserArrayPtr);
inline void MFTYPE MimGetResult1dSafeType(MIL_ID ResultImId, MIL_INT OffEntry, MIL_INT NbEntries, MIL_INT64  ResultType, MIL_UINT64     *UserArrayPtr);
#endif

//-------------------------------------------------------------------------------------
// MimGetResult2d

inline void MFTYPE MimGetResult2dUnsafe  (MIL_ID ResultImId, MIL_INT OffsetX, MIL_INT OffsetY, MIL_INT SizeX, MIL_INT SizeY, MIL_INT64  ResultType, MIL_INT64  ControlFlag, void           *UserArrayPtr);
inline void MFTYPE MimGetResult2dSafeType(MIL_ID ResultImId, MIL_INT OffsetX, MIL_INT OffsetY, MIL_INT SizeX, MIL_INT SizeY, MIL_INT64  ResultType, MIL_INT64  ControlFlag, int             UserArrayPtr);
inline void MFTYPE MimGetResult2dSafeType(MIL_ID ResultImId, MIL_INT OffsetX, MIL_INT OffsetY, MIL_INT SizeX, MIL_INT SizeY, MIL_INT64  ResultType, MIL_INT64  ControlFlag, MIL_INT32      *UserArrayPtr);
inline void MFTYPE MimGetResult2dSafeType(MIL_ID ResultImId, MIL_INT OffsetX, MIL_INT OffsetY, MIL_INT SizeX, MIL_INT SizeY, MIL_INT64  ResultType, MIL_INT64  ControlFlag, MIL_INT64      *UserArrayPtr);
inline void MFTYPE MimGetResult2dSafeType(MIL_ID ResultImId, MIL_INT OffsetX, MIL_INT OffsetY, MIL_INT SizeX, MIL_INT SizeY, MIL_INT64  ResultType, MIL_INT64  ControlFlag, float          *UserArrayPtr);
inline void MFTYPE MimGetResult2dSafeType(MIL_ID ResultImId, MIL_INT OffsetX, MIL_INT OffsetY, MIL_INT SizeX, MIL_INT SizeY, MIL_INT64  ResultType, MIL_INT64  ControlFlag, MIL_DOUBLE     *UserArrayPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline void MFTYPE MimGetResult2dSafeType(MIL_ID ResultImId, MIL_INT OffsetX, MIL_INT OffsetY, MIL_INT SizeX, MIL_INT SizeY, MIL_INT64  ResultType, MIL_INT64  ControlFlag, MIL_UINT32     *UserArrayPtr);
inline void MFTYPE MimGetResult2dSafeType(MIL_ID ResultImId, MIL_INT OffsetX, MIL_INT OffsetY, MIL_INT SizeX, MIL_INT SizeY, MIL_INT64  ResultType, MIL_INT64  ControlFlag, MIL_UINT64     *UserArrayPtr);
#endif

// ----------------------------------------------------------
// MimInquire

inline MIL_INT MFTYPE MimInquireUnsafe  (MIL_ID ContextOrResultImId, MIL_INT64  ParamToInquire, void        * ValuePtr);
inline MIL_INT MFTYPE MimInquireSafeType(MIL_ID ContextOrResultImId, MIL_INT64  ParamToInquire, int           ValuePtr);
inline MIL_INT MFTYPE MimInquireSafeType(MIL_ID ContextOrResultImId, MIL_INT64  ParamToInquire, MIL_INT32   * ValuePtr);
inline MIL_INT MFTYPE MimInquireSafeType(MIL_ID ContextOrResultImId, MIL_INT64  ParamToInquire, MIL_INT64   * ValuePtr);
inline MIL_INT MFTYPE MimInquireSafeType(MIL_ID ContextOrResultImId, MIL_INT64  ParamToInquire, MIL_DOUBLE  * ValuePtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MimInquireSafeType(MIL_ID ContextOrResultImId, MIL_INT64  ParamToInquire, MIL_UINT32  * ValuePtr);
inline MIL_INT MFTYPE MimInquireSafeType(MIL_ID ContextOrResultImId, MIL_INT64  ParamToInquire, MIL_UINT64  * ValuePtr);
#endif

// -------------------------------------------------------------------------
// MimGetResult

inline MIL_INT64  MimGetResultRequiredType(MIL_ID ResultImId, MIL_INT64  ResultType)
   {
   switch (ResultType)
      {
      case M_NB_EVENT:
      case M_POSITION_X:
      case M_POSITION_Y:
         {
         // For these results, the doc specifies that MIL_INT is returned
         // except when allocated with 32+M_FLOAT, where a float is returned.
         MIL_INT  Type  = MbufInquire(ResultImId, M_TYPE, M_NULL);
         if (Type == 32+M_FLOAT)
            return M_TYPE_FLOAT;
         else if(Type == 64+M_FLOAT)
            return M_TYPE_MIL_DOUBLE;
         else
            return M_TYPE_MIL_INT;
         }
         break;
      case M_VALUE:
         {
         // For M_VALUE, the doc specifies that MIL_INT is returned
         // except for M_PROJ_LIST, M_EXTREME_LIST, M_EVENT_LIST
         // allocated with 32+M_FLOAT, where a float is returned.
         MIL_INT IdResultType = MimInquire(ResultImId, M_RESULT_TYPE, NULL);
         switch (IdResultType)
            {
            case M_PROJ_LIST:
            case M_EXTREME_LIST:
            case M_EVENT_LIST:
               {
               MIL_INT  Type  = MbufInquire(ResultImId, M_TYPE, M_NULL);
               if(Type == 32+M_FLOAT)
                  return M_TYPE_FLOAT;
               else if(Type == 64+M_FLOAT)
                  return M_TYPE_MIL_DOUBLE;
               else
                  return M_TYPE_MIL_INT;
               }
               break;
            default:
               return M_TYPE_MIL_INT;
               break;
            }
         }
         break;

      case M_VALUE+M_PERCENTAGE:
      case M_CUMULATIVE_VALUE:
      case M_CUMULATIVE_VALUE+M_PERCENTAGE:
      case M_PERCENTILE_VALUE:
         return M_TYPE_MIL_INT;
         break;

      default:
         return M_TYPE_DOUBLE;
         break;
      }
   }

inline void MFTYPE MimGetResultSafeType(MIL_ID ResultImId, MIL_INT64 ResultType, int  UserArrayPtr)
   {
   if (UserArrayPtr)
      SafeTypeError(MIL_TEXT("MimGetResult"));

   MimGetResult(ResultImId, ResultType, NULL);
   }

inline void MimGetResultSafeTypeExecute (MIL_ID ResultImId, MIL_INT64 ResultType, void *UserArrayPtr, MIL_INT64  GivenType)
   {
   MIL_INT64  RequiredType = ExtractTypeFromValue(ResultType, M_TYPE_DOUBLE, M_TYPE_MIL_INT32, M_TYPE_MIL_INT64, M_TYPE_FLOAT);
   if (RequiredType == 0)
      RequiredType = MimGetResultRequiredType(ResultImId, ResultType);

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MimGetResult"));

   MimGetResult(ResultImId, ResultType, UserArrayPtr);
   }


inline void MFTYPE MimGetResultUnsafe  (MIL_ID ResultImId, MIL_INT64  ResultType, void        *UserArrayPtr) {MimGetResult               (ResultImId, ResultType, UserArrayPtr);}
inline void MFTYPE MimGetResultSafeType(MIL_ID ResultImId, MIL_INT64  ResultType, MIL_INT32   *UserArrayPtr) {MimGetResultSafeTypeExecute(ResultImId, ResultType, UserArrayPtr, M_TYPE_MIL_INT32);}
inline void MFTYPE MimGetResultSafeType(MIL_ID ResultImId, MIL_INT64  ResultType, MIL_INT64   *UserArrayPtr) {MimGetResultSafeTypeExecute(ResultImId, ResultType, UserArrayPtr, M_TYPE_MIL_INT64);}
inline void MFTYPE MimGetResultSafeType(MIL_ID ResultImId, MIL_INT64  ResultType, float       *UserArrayPtr) {MimGetResultSafeTypeExecute(ResultImId, ResultType, UserArrayPtr, M_TYPE_FLOAT);}
inline void MFTYPE MimGetResultSafeType(MIL_ID ResultImId, MIL_INT64  ResultType, MIL_DOUBLE  *UserArrayPtr) {MimGetResultSafeTypeExecute(ResultImId, ResultType, UserArrayPtr, M_TYPE_DOUBLE);}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline void MFTYPE MimGetResultSafeType(MIL_ID ResultImId, MIL_INT64  ResultType, MIL_UINT32  *UserArrayPtr) {MimGetResultSafeTypeExecute(ResultImId, ResultType, UserArrayPtr, M_TYPE_MIL_INT32);}
inline void MFTYPE MimGetResultSafeType(MIL_ID ResultImId, MIL_INT64  ResultType, MIL_UINT64  *UserArrayPtr) {MimGetResultSafeTypeExecute(ResultImId, ResultType, UserArrayPtr, M_TYPE_MIL_INT64);}
#endif

// -------------------------------------------------------------------------
// MimGetResult1d

inline void MFTYPE MimGetResult1dSafeType(MIL_ID ResultImId, MIL_INT OffEntry, MIL_INT NbEntries, MIL_INT64  ResultType, int UserArrayPtr)
   {
   if (UserArrayPtr)
      SafeTypeError(MIL_TEXT("MimGetResult1d"));

   MimGetResult1d(ResultImId, OffEntry, NbEntries, ResultType, NULL);
   }

inline void MimGetResult1dSafeTypeExecute (MIL_ID ResultImId, MIL_INT OffEntry, MIL_INT NbEntries, MIL_INT64  ResultType, void *UserArrayPtr, MIL_INT64  GivenType)
   {
   MIL_INT64  RequiredType = ExtractTypeFromValue(ResultType, M_TYPE_DOUBLE, M_TYPE_MIL_INT32, M_TYPE_MIL_INT64, M_TYPE_FLOAT);
   if (RequiredType == 0)
      RequiredType = MimGetResultRequiredType(ResultImId, ResultType);

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MimGetResult1d"));

   MimGetResult1d(ResultImId, OffEntry, NbEntries, ResultType, UserArrayPtr);
   }

inline void MFTYPE MimGetResult1dUnsafe  (MIL_ID ResultImId, MIL_INT OffEntry, MIL_INT NbEntries, MIL_INT64  ResultType, void        *UserArrayPtr){MimGetResult1d               (ResultImId, OffEntry, NbEntries, ResultType, UserArrayPtr);}
inline void MFTYPE MimGetResult1dSafeType(MIL_ID ResultImId, MIL_INT OffEntry, MIL_INT NbEntries, MIL_INT64  ResultType, MIL_INT32   *UserArrayPtr){MimGetResult1dSafeTypeExecute(ResultImId, OffEntry, NbEntries, ResultType, UserArrayPtr, M_TYPE_MIL_INT32);}
inline void MFTYPE MimGetResult1dSafeType(MIL_ID ResultImId, MIL_INT OffEntry, MIL_INT NbEntries, MIL_INT64  ResultType, MIL_INT64   *UserArrayPtr){MimGetResult1dSafeTypeExecute(ResultImId, OffEntry, NbEntries, ResultType, UserArrayPtr, M_TYPE_MIL_INT64);}
inline void MFTYPE MimGetResult1dSafeType(MIL_ID ResultImId, MIL_INT OffEntry, MIL_INT NbEntries, MIL_INT64  ResultType, float       *UserArrayPtr){MimGetResult1dSafeTypeExecute(ResultImId, OffEntry, NbEntries, ResultType, UserArrayPtr, M_TYPE_FLOAT);}
inline void MFTYPE MimGetResult1dSafeType(MIL_ID ResultImId, MIL_INT OffEntry, MIL_INT NbEntries, MIL_INT64  ResultType, MIL_DOUBLE  *UserArrayPtr){MimGetResult1dSafeTypeExecute(ResultImId, OffEntry, NbEntries, ResultType, UserArrayPtr, M_TYPE_DOUBLE);}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline void MFTYPE MimGetResult1dSafeType(MIL_ID ResultImId, MIL_INT OffEntry, MIL_INT NbEntries, MIL_INT64  ResultType, MIL_UINT32  *UserArrayPtr){MimGetResult1dSafeTypeExecute(ResultImId, OffEntry, NbEntries, ResultType, UserArrayPtr, M_TYPE_MIL_INT32);}
inline void MFTYPE MimGetResult1dSafeType(MIL_ID ResultImId, MIL_INT OffEntry, MIL_INT NbEntries, MIL_INT64  ResultType, MIL_UINT64  *UserArrayPtr){MimGetResult1dSafeTypeExecute(ResultImId, OffEntry, NbEntries, ResultType, UserArrayPtr, M_TYPE_MIL_INT64);}
#endif

// -------------------------------------------------------------------------
// MimGetResult2d

inline void MFTYPE MimGetResult2dSafeType(MIL_ID ResultImId, MIL_INT OffsetX, MIL_INT OffsetY, MIL_INT SizeX, MIL_INT SizeY, MIL_INT64  ResultType, MIL_INT64  ControlFlag, int UserArrayPtr)
   {
   if(UserArrayPtr)
      SafeTypeError(MIL_TEXT("MimGetResult2d"));

   MimGetResult2d(ResultImId, OffsetX, OffsetY, SizeX, SizeY, ResultType, ControlFlag, NULL);
   }

inline void MimGetResult2dSafeTypeExecute (MIL_ID ResultImId, MIL_INT OffsetX, MIL_INT OffsetY, MIL_INT SizeX, MIL_INT SizeY, MIL_INT64  ResultType, MIL_INT64  ControlFlag, void *UserArrayPtr, MIL_INT64  GivenType)
   {
   MIL_INT64  RequiredType = ExtractTypeFromValue(ResultType, M_TYPE_DOUBLE, M_TYPE_MIL_INT32, M_TYPE_MIL_INT64, M_TYPE_FLOAT);
   if (RequiredType == 0)
      RequiredType = MimGetResultRequiredType(ResultImId, ResultType);

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MimGetResult2d"));

   MimGetResult2d(ResultImId, OffsetX, OffsetY, SizeX, SizeY, ResultType, ControlFlag, UserArrayPtr);
   }

inline void MFTYPE MimGetResult2dUnsafe  (MIL_ID ResultImId, MIL_INT OffsetX, MIL_INT OffsetY, MIL_INT SizeX, MIL_INT SizeY, MIL_INT64  ResultType, MIL_INT64  ControlFlag, void        *UserArrayPtr){MimGetResult2d               (ResultImId, OffsetX, OffsetY, SizeX, SizeY, ResultType, ControlFlag, UserArrayPtr);}
inline void MFTYPE MimGetResult2dSafeType(MIL_ID ResultImId, MIL_INT OffsetX, MIL_INT OffsetY, MIL_INT SizeX, MIL_INT SizeY, MIL_INT64  ResultType, MIL_INT64  ControlFlag, MIL_INT32   *UserArrayPtr){MimGetResult2dSafeTypeExecute(ResultImId, OffsetX, OffsetY, SizeX, SizeY, ResultType, ControlFlag, UserArrayPtr, M_TYPE_MIL_INT32);}
inline void MFTYPE MimGetResult2dSafeType(MIL_ID ResultImId, MIL_INT OffsetX, MIL_INT OffsetY, MIL_INT SizeX, MIL_INT SizeY, MIL_INT64  ResultType, MIL_INT64  ControlFlag, MIL_INT64   *UserArrayPtr){MimGetResult2dSafeTypeExecute(ResultImId, OffsetX, OffsetY, SizeX, SizeY, ResultType, ControlFlag, UserArrayPtr, M_TYPE_MIL_INT64);}
inline void MFTYPE MimGetResult2dSafeType(MIL_ID ResultImId, MIL_INT OffsetX, MIL_INT OffsetY, MIL_INT SizeX, MIL_INT SizeY, MIL_INT64  ResultType, MIL_INT64  ControlFlag, float       *UserArrayPtr){MimGetResult2dSafeTypeExecute(ResultImId, OffsetX, OffsetY, SizeX, SizeY, ResultType, ControlFlag, UserArrayPtr, M_TYPE_FLOAT);}
inline void MFTYPE MimGetResult2dSafeType(MIL_ID ResultImId, MIL_INT OffsetX, MIL_INT OffsetY, MIL_INT SizeX, MIL_INT SizeY, MIL_INT64  ResultType, MIL_INT64  ControlFlag, MIL_DOUBLE  *UserArrayPtr){MimGetResult2dSafeTypeExecute(ResultImId, OffsetX, OffsetY, SizeX, SizeY, ResultType, ControlFlag, UserArrayPtr, M_TYPE_DOUBLE);}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline void MFTYPE MimGetResult2dSafeType(MIL_ID ResultImId, MIL_INT OffsetX, MIL_INT OffsetY, MIL_INT SizeX, MIL_INT SizeY, MIL_INT64  ResultType, MIL_INT64  ControlFlag, MIL_UINT32  *UserArrayPtr){MimGetResult2dSafeTypeExecute(ResultImId, OffsetX, OffsetY, SizeX, SizeY, ResultType, ControlFlag, UserArrayPtr, M_TYPE_MIL_INT32);}
inline void MFTYPE MimGetResult2dSafeType(MIL_ID ResultImId, MIL_INT OffsetX, MIL_INT OffsetY, MIL_INT SizeX, MIL_INT SizeY, MIL_INT64  ResultType, MIL_INT64  ControlFlag, MIL_UINT64  *UserArrayPtr){MimGetResult2dSafeTypeExecute(ResultImId, OffsetX, OffsetY, SizeX, SizeY, ResultType, ControlFlag, UserArrayPtr, M_TYPE_MIL_INT64);}
#endif

// ----------------------------------------------------------
// MimInquire

inline MIL_INT64  MimInquireRequiredType(MIL_INT64 InquireType)
   {
   MIL_INT64  DataType = ExtractTypeFromValue(InquireType, M_TYPE_DOUBLE, M_TYPE_MIL_ID, M_TYPE_MIL_INT32, M_TYPE_MIL_INT64);

   if (DataType)
      return DataType;

   if (M_IN_IM_INQUIRE_DOUBLE_RANGE(InquireType))
      return M_TYPE_DOUBLE;

   if (M_IN_IM_INQUIRE_MIL_INT64_RANGE(InquireType))
      return M_TYPE_MIL_INT64;

   return M_TYPE_MIL_INT;
   }

inline MIL_INT MFTYPE MimInquireSafeType(MIL_ID ContextOrResultId, MIL_INT64  InquireType, int UserVarPtr)
   {
   bool MustHaveUserPointer = M_IM_INQUIRE_MUST_HAVE_USER_PTR(InquireType);

   if (UserVarPtr || MustHaveUserPointer)
      SafeTypeError(MIL_TEXT("MimInquire"));

   return MimInquire(ContextOrResultId, InquireType, NULL);
   }

inline MIL_INT MFTYPE MimInquireSafeTypeExecute (MIL_ID ContextOrResultId, MIL_INT64  InquireType, void  *UserVarPtr, MIL_INT64  GivenType)
   {
   MIL_INT64  RequiredType = MimInquireRequiredType(InquireType);
   ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MimInquire"));

   return MimInquire(ContextOrResultId, InquireType, UserVarPtr);
   }

inline MIL_INT MFTYPE MimInquireUnsafe  (MIL_ID ContextOrResultId, MIL_INT64  InquireType, void        *UserVarPtr) {return MimInquire               (ContextOrResultId, InquireType, UserVarPtr                  );}
inline MIL_INT MFTYPE MimInquireSafeType(MIL_ID ContextOrResultId, MIL_INT64  InquireType, MIL_INT32   *UserVarPtr) {return MimInquireSafeTypeExecute(ContextOrResultId, InquireType, UserVarPtr, M_TYPE_MIL_INT32);}
inline MIL_INT MFTYPE MimInquireSafeType(MIL_ID ContextOrResultId, MIL_INT64  InquireType, MIL_INT64   *UserVarPtr) {return MimInquireSafeTypeExecute(ContextOrResultId, InquireType, UserVarPtr, M_TYPE_MIL_INT64);}
inline MIL_INT MFTYPE MimInquireSafeType(MIL_ID ContextOrResultId, MIL_INT64  InquireType, MIL_DOUBLE  *UserVarPtr) {return MimInquireSafeTypeExecute(ContextOrResultId, InquireType, UserVarPtr, M_TYPE_DOUBLE   );}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MimInquireSafeType(MIL_ID ContextOrResultId, MIL_INT64  InquireType, MIL_UINT32  *UserVarPtr) {return MimInquireSafeTypeExecute(ContextOrResultId, InquireType, UserVarPtr, M_TYPE_MIL_INT32);}
inline MIL_INT MFTYPE MimInquireSafeType(MIL_ID ContextOrResultId, MIL_INT64  InquireType, MIL_UINT64  *UserVarPtr) {return MimInquireSafeTypeExecute(ContextOrResultId, InquireType, UserVarPtr, M_TYPE_MIL_INT64);}
#endif

#define MimGetResult         MimGetResultSafeType
#define MimGetResult1d       MimGetResult1dSafeType
#define MimGetResult2d       MimGetResult2dSafeType
#define MimInquire           MimInquireSafeType

#else // #if M_MIL_USE_SAFE_TYPE

#define MimGetResultUnsafe         MimGetResult
#define MimGetResult1dUnsafe       MimGetResult1d
#define MimGetResult2dUnsafe       MimGetResult2d
#define MimInquireUnsafe           MimInquire

#endif // #if M_MIL_USE_SAFE_TYPE


#endif //__MILIM_H__
