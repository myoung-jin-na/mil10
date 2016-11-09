////////////////////////////////////////////////////////////////////////////////
//! 
//! \file  milcolor.h 
//! 
//! \brief This file contains the defines and the prototypes for the
//!         MIL color module. (Mcol...). 
//! 
//! AUTHOR:  Matrox Imaging dept.
//!
//! COPYRIGHT NOTICE:
//! Copyright © Matrox Electronic Systems Ltd., 1992-2014. 
//! All Rights Reserved 
//  Revision:  10.00.2564
//////////////////////////////////////////////////////////////////////////////// 

#ifndef __MILCOLOR_H__
#define __MILCOLOR_H__

#if (!M_MIL_LITE) // MIL FULL ONLY

/* C++ directive if needed */
#ifdef __cplusplus
extern "C"
   {
#endif

/***************************************************************************/
/*                       MilColor CAPI defines                             */
/***************************************************************************/
#define M_CONTEXT                                  0x08000000L // Already defined in    milmod.h, milreg.h, milstr.h, milocr.h
#define M_GENERAL                                  0x20000000L // Already defined in    milmod.h, milreg.h, milstr.h, milocr.h
#define M_ALL                                      0x40000000L // Already defined in    milmod.h, milmeas.h, milocr.h,  milpat.h,  mil.h, miledge.h

// High level result types modifiers
#define M_AVAILABLE                                MAKE_INT64(0x0000400000000000) // Also defined in mil.h
#define M_SUPPORTED                                MAKE_INT64(0x0000800000000000) // Also defined in mil.h

// Allocation Macros
#define M_SAMPLE_LABEL_TAG                         0x01000000L  // must not clash with M_ALL, M_GENERAL, M_CONTEXT
#define M_SAMPLE_INDEX_TAG                         0x02000000L  // must not clash with M_ALL, M_GENERAL, M_CONTEXT

#define M_COLOR_ITEM_TAG                           0x04000000L

#define M_SAMPLE_LABEL(lbl)                        ((lbl) | M_SAMPLE_LABEL_TAG)
#define M_SAMPLE_INDEX(idx)                        ((idx) | M_SAMPLE_INDEX_TAG)

#define M_SAMPLE_LABEL_ITEM(lbl, sublbl)           ((lbl | (sublbl << 12)) | M_COLOR_ITEM_TAG | M_SAMPLE_LABEL_TAG)
#define M_SAMPLE_INDEX_ITEM(lbl, sublbl)           ((lbl | (sublbl << 12)) | M_COLOR_ITEM_TAG | M_SAMPLE_INDEX_TAG)

// Color Macros
#define M_COLOR888(Component0, Component1, Component2)      M_RGB888(Component0, Component1, Component2)

// Allocation Flag
#define M_COLOR_MATCHING_RESULT                    0x00001000L // used in McolAllocResult
#define M_COLOR_MATCHING                           0x00000100L // used in McolAlloc
#define M_COLOR_PROJECTION                         0x00000200L // used in McolAlloc

// Control Flags for Context
#define M_DISTANCE_TYPE                            1L          // used in matching context   (read-only)
#define M_PREPROCESSED                             14L         // used in matching context      *also defined in milpat.h, milmod.h, milmetrol.h, milstr.h
#define M_DISTANCE_TOLERANCE_MODE                  20L         // used in matching context
#define M_DISTANCE_TOLERANCE_METHOD                M_DISTANCE_TOLERANCE_MODE  // keep synonym because PP1 EA released (TBR in MIL10)
#define M_OUTLIER_LABEL                            30L         // used in matching context
#define M_OUTLIER_DRAW_COLOR                       31L         // used in matching context
#define M_BACKGROUND_DRAW_COLOR                    32L         // used in matching context
#define M_NUMBER_OF_SAMPLES                        35L         // used in matching context
#define M_CONVERSION_MODE                          40L         // used in matching context   (read-only)
#define M_PRECONVERT_MODE                          M_CONVERSION_MODE // keep synonym because PP1 EA released (TBR in MIL10)
#define M_MATCH_MODE                               50L         // used in matching context   (read-only)
#define M_MATCH_METHOD                             M_MATCH_MODE // keep synonym because PP1 EA released (TBR in MIL10)
#define M_BAND_MODE                                90L         // used in matching context
#define M_ACCEPTANCE                               200L        // used in matching context      *also used in milpat.h, milmod.h
#define M_ACCEPTANCE_RELEVANCE                     202L        // used in matching context
#define M_RELEVANCE_ACCEPTANCE                     M_ACCEPTANCE_RELEVANCE // keep synonym because PP1 EA released (TBR in MIL10)
#define M_TIE_EPSILON                              205L        // used in matching context
#define M_SAMPLE_BUFFER_FORMAT                     210L        // used in matching context      *undocumented internal flag
#define M_DISTANCE_IMAGE_NORMALIZE                 215L        // used in matching context and matching result
#define M_DISTANCE_PARAM_1                         221L        // used in matching context
#define M_DISTANCE_PARAM_2                         222L        // used in matching context
#define M_DISTANCE_PARAM_3                         223L        // used in matching context
#define M_DISTANCE_PARAM_4                         224L        // used in matching context
#define M_DISTANCE_PARAM_5                         225L        // used in matching context
#define M_ENCODING                          0x00008005L        // used in matching context     *also used in milstr.h, milcode.h
#define M_CONVERSION_GAMMA                         226L        // used in matching context
#define M_PRECONVERT_GAMMA                         M_CONVERSION_GAMMA  // keep synonym because PP1 EA released (TBR in MIL10)
#define M_NB_BINS_BAND_0                          1420L        // used in matching context
#define M_NB_BINS_BAND_1                          1421L        // used in matching context
#define M_NB_BINS_BAND_2                          1422L        // used in matching context
#define M_KERNEL_SIZE                             301L         // used in matching context      *also used in miledge.h, milmod.h
// Encoding controls per-band
#define M_SCALE_BAND_0                             241L        // used in matching context
#define M_SCALE_BAND_1                             242L        // used in matching context
#define M_SCALE_BAND_2                             243L        // used in matching context
#define M_OFFSET_BAND_0                            244L        // used in matching context
#define M_OFFSET_BAND_1                            245L        // used in matching context
#define M_OFFSET_BAND_2                            246L        // used in matching context
// Internal controls
#define M_SCORE_MAX_DIST_PARAM                     250L        // used in matching context
#define M_SCORE_K_PARAM                            252L        // used in matching context
#define M_TRIPLET_ITEM_SIZE                       1624L        // used for draw of sample mosaic

// Controls for output images (read-only)
// --> To be deprecated !
#define M_LABEL_PIXEL_IMAGE_SIZE_BIT               352L        // used in matching result and matching context
#define M_LABEL_AREA_IMAGE_SIZE_BIT                372L        // used in matching result and matching context
// <--

// Alloc-time controls (read-only)
#define M_COLOR_SPACE                              230L        // used in matching context   (read-only)

// Supported color spaces
#define M_RGB                                      8L        // used in McolAlloc             *also defined in mil.h
#define M_CIELAB                                  41L        // used in McolAlloc
#define M_HSL                                      2L        // used in McolAlloc
#define M_YUV                                      4L        // used in McolAlloc             *also defined in mil.h

// Control/Inquire Flags for Samples
#define M_SAMPLE_8BIT_AVERAGE_COLOR_BAND_0        1424L        // used in inquiring samples (read-only)
#define M_SAMPLE_8BIT_AVERAGE_COLOR_BAND_1        1425L        // used in inquiring samples (read-only)
#define M_SAMPLE_8BIT_AVERAGE_COLOR_BAND_2        1426L        // used in inquiring samples (read-only)
#define M_SAMPLE_AVERAGE_COLOR_BAND_0             1427L        // used in inquiring samples (read-only)
#define M_SAMPLE_AVERAGE_COLOR_BAND_1             1428L        // used in inquiring samples (read-only)
#define M_SAMPLE_AVERAGE_COLOR_BAND_2             1429L        // used in inquiring samples (read-only)
#define M_SAMPLE_PCA_VECTOR1_1                    1430L        // used in inquiring samples (read-only)
#define M_SAMPLE_PCA_VECTOR1_2                    1431L        // used in inquiring samples (read-only)
#define M_SAMPLE_PCA_VECTOR1_3                    1432L        // used in inquiring samples (read-only)
#define M_SAMPLE_PCA_VECTOR2_1                    1433L        // used in inquiring samples (read-only)
#define M_SAMPLE_PCA_VECTOR2_2                    1434L        // used in inquiring samples (read-only)
#define M_SAMPLE_PCA_VECTOR2_3                    1435L        // used in inquiring samples (read-only)
#define M_SAMPLE_PCA_VECTOR3_1                    1436L        // used in inquiring samples (read-only)
#define M_SAMPLE_PCA_VECTOR3_2                    1437L        // used in inquiring samples (read-only)
#define M_SAMPLE_PCA_VECTOR3_3                    1438L        // used in inquiring samples (read-only)
#define M_SAMPLE_PCA_VAR1                         1439L        // used in inquiring samples (read-only)
#define M_SAMPLE_PCA_VAR2                         1440L        // used in inquiring samples (read-only)
#define M_SAMPLE_PCA_VAR3                         1441L        // used in inquiring samples (read-only)
#define M_DISTANCE_TOLERANCE                       120L        // used in matching samples 
#define M_SAMPLE_TYPE                              121L        // used in matching samples (read-only) 
#define M_SAMPLE_IMAGE_SIZE_X                      122L        // used in matching samples (read-only)
#define M_SAMPLE_IMAGE_SIZE_Y                      123L        // used in matching samples (read-only)
#define M_SAMPLE_IMAGE_SIZE_BIT                    124L        // used in matching samples (read-only)
#define M_SAMPLE_IMAGE_SIZE_BAND                   125L        // used in matching samples (read-only)
#define M_SAMPLE_IMAGE_SIGN                        126L        // used in matching samples (read-only)
#define M_SAMPLE_IMAGE_TYPE                        127L        // used in matching samples (read-only)
#define M_SAMPLE_MASK_SIZE_X                       128L        // used in matching samples (read-only)
#define M_SAMPLE_MASK_SIZE_Y                       129L        // used in matching samples (read-only)
#define M_SAMPLE_MASK_SIZE_BIT                     130L        // used in matching samples (read-only)
#define M_SAMPLE_MASK_SIZE_BAND                    131L        // used in matching samples (read-only)
#define M_SAMPLE_MASK_SIGN                         132L        // used in matching samples (read-only)
#define M_SAMPLE_MASK_TYPE                         133L        // used in matching samples (read-only)
#define M_SAMPLE_MASKED                            135L        // used in matching samples (read-only)
#define M_SAMPLE_COLOR_BAND_0                      140L        // used in matching samples (read-only)
#define M_SAMPLE_COLOR_BAND_1                      141L        // used in matching samples (read-only)
#define M_SAMPLE_COLOR_BAND_2                      142L        // used in matching samples (read-only)
#define M_MATCH_SAMPLE_COLOR_BAND_0                145L        // used in matching samples (read-only)
#define M_MATCH_SAMPLE_COLOR_BAND_1                146L        // used in matching samples (read-only)
#define M_MATCH_SAMPLE_COLOR_BAND_2                147L        // used in matching samples (read-only)
#define M_NUMBER_OF_COLOR_ITEMS                    1585L       // used in inquiring samples (read-only)
#define M_SAMPLE_MOSAIC_SIZE_X                     1610L       // used in inquiring samples (read-only)
#define M_SAMPLE_MOSAIC_SIZE_Y                     1611L       // used in inquiring samples (read-only)
#define M_SAMPLE_MOSAIC_SIZE_BIT                   1612L       // used in inquiring samples (read-only)
#define M_SAMPLE_MOSAIC_SIZE_BAND                  1613L       // used in inquiring samples (read-only)
#define M_SAMPLE_MOSAIC_SIGN                       1614L       // used in inquiring samples (read-only)
#define M_SAMPLE_MOSAIC_TYPE                       1615L       // used in inquiring samples (read-only)
#define M_SAMPLE_MOSAIC_DONT_CARE_SIZE_X           1616L       // used in inquiring samples (read-only)
#define M_SAMPLE_MOSAIC_DONT_CARE_SIZE_Y           1617L       // used in inquiring samples (read-only)
#define M_SAMPLE_MOSAIC_DONT_CARE_SIZE_BIT         1618L       // used in inquiring samples (read-only)
#define M_SAMPLE_MOSAIC_DONT_CARE_SIZE_BAND        1619L       // used in inquiring samples (read-only)
#define M_SAMPLE_MOSAIC_DONT_CARE_SIGN             1620L       // used in inquiring samples (read-only)
#define M_SAMPLE_MOSAIC_DONT_CARE_TYPE             1621L       // used in inquiring samples (read-only)
#define M_HISTOGRAM_FREQUENCY_THRESHOLD            1627L       // used in matching samples 

// Control/Inquire Flags for Context
#define M_SAMPLE_LUT_SIZE_X                        150L        // used in matching context (read-only)
#define M_SAMPLE_LUT_SIZE_Y                        151L        // used in matching context (read-only)
#define M_SAMPLE_LUT_SIZE_BIT                      152L        // used in matching context (read-only)
#define M_SAMPLE_LUT_SIZE_BAND                     153L        // used in matching context (read-only)
#define M_SAMPLE_LUT_SIGN                          154L        // used in matching context (read-only)
#define M_SAMPLE_LUT_TYPE                          155L        // used in matching context (read-only)
#define M_SAMPLE_ABSOLUTE_TOLERANCE                160L        // used in matching context (read-only)
#define M_PIXEL_SCORE_TOLERANCE                   1442L        // used in matching context

// Control Values
#define M_NONE                                     0x08000000L // value for M_CONVERSION_MODE  *also defined in milcal.h, 
#define M_EUCLIDEAN                                2L          // value for M_DISTANCE_TYPE
#define M_MANHATTAN                                3L          // value for M_DISTANCE_TYPE
#define M_DELTA_E                                  4L          // value for M_DISTANCE_TYPE
#define M_MAHALANOBIS_SAMPLE                       5L          // value for M_DISTANCE_TYPE
#define M_MAHALANOBIS                              M_MAHALANOBIS_SAMPLE
#define M_MAHALANOBIS_TARGET                       8L          // value for M_DISTANCE_TYPE
#define M_EUCLIDEAN_SQR                            22L         // value for M_DISTANCE_TYPE
#define M_MANHATTAN_SQR                            23L         // value for M_DISTANCE_TYPE
#define M_DELTA_E_SQR                              24L         // value for M_DISTANCE_TYPE
#define M_MAHALANOBIS_SAMPLE_SQR                   25L         // value for M_DISTANCE_TYPE
#define M_MAHALANOBIS_TARGET_SQR                   28L         // value for M_DISTANCE_TYPE
#define M_ABSOLUTE                                 1L          // value for M_DISTANCE_TOLERANCE_MODE  *also defined in milpat.h, 
#define M_RELATIVE                                 21L         // value for M_DISTANCE_TOLERANCE_MODE
#define M_SAMPLE_STDDEV                            33L         // value for M_DISTANCE_TOLERANCE_MODE
#define M_AUTO                                     444L        // value for M_DISTANCE_TOLERANCE *also defined in miledge.h, milmetrol.h, milmod.h, milocr.h
#define M_NONE                                     0x08000000L // value for M_CONVERSION_MODE   *also defined in milstr.h, milcal.h
#define M_CIELAB                                   41L         // value for M_CONVERSION_MODE
#define M_MIN_DIST_VOTE                            51L         // value for M_MATCH_MODE
#define M_STAT_MIN_DIST                            52L         // value for M_MATCH_MODE
#define M_HISTOGRAM_MATCHING                      1443L        // value for M_MATCH_MODE
#define M_HISTOGRAM_VOTE                          1626L        // value for M_MATCH_MODE
#define M_HISTOGRAM_CLASSIFY_PIXEL                1444L        // value for M_MATCH_MODE
#define M_ALL_BAND                                 -1L         // value for M_BAND_MODE              *also defined in MIL.H
#define M_ALL_BANDS                                M_ALL_BAND  // value for M_BAND_MODE              *also defined in MIL.H
#define M_TRANSPARENT                              0x01000059L // value for M_BACKGROUND_DRAW_COLOR  *also defined in MIL.H
#define M_CIELCH                                   1468L       // value for M_CONVERSION_MODE
#define M_CMC_PERCEPTIBILITY                       1469L       // value for M_DISTANCE_TYPE
#define M_CMC_ACCEPTABILITY                        1470L       // value for M_DISTANCE_TYPE
#define M_CIE94_GRAPHIC_ARTS                       1471L       			// value for M_DISTANCE_TYPE
#define M_CIEDE94_GRAPHIC_ARTS                     M_CIE94_GRAPHIC_ARTS // old erroneous name released in 9.0 PP2, kept for backward compatibility (TBR in MIL10)
#define M_CIE94_TEXTILE                            1472L       			// value for M_DISTANCE_TYPE
#define M_CIEDE94_TEXTILE                          M_CIE94_TEXTILE		// old erroneous name released in 9.0 PP2, kept for backward compatibility (TBR in MIL10)
#define M_CIEDE2000                                1473L       // value for M_DISTANCE_TYPE
#define M_CMC_PERCEPTIBILITY_SQR                   1474L       // value for M_DISTANCE_TYPE
#define M_CMC_ACCEPTABILITY_SQR                    1475L       // value for M_DISTANCE_TYPE
#define M_CIE94_GRAPHIC_ARTS_SQR                   1476L       			    // value for M_DISTANCE_TYPE
#define M_CIEDE94_GRAPHIC_ARTS_SQR                 M_CIE94_GRAPHIC_ARTS_SQR // old erroneous name released in 9.0 PP2, kept for backward compatibility (TBR in MIL10)
#define M_CIE94_TEXTILE_SQR                        1477L       			    // value for M_DISTANCE_TYPE
#define M_CIEDE94_TEXTILE_SQR                      M_CIE94_TEXTILE_SQR		 // old erroneous name released in 9.0 PP2, kept for backward compatibility (TBR in MIL10)
#define M_CIEDE2000_SQR                            1478L       // value for M_DISTANCE_TYPE
// To work independently from RGB space we define new names for bands
#define M_COLOR_BAND_0                             0x00000100  // value for M_BAND_MODE & McolDistance
#define M_COLOR_BAND_1                             0x00000200  // value for M_BAND_MODE & McolDistance
#define M_COLOR_BAND_2                             0x00000400  // value for M_BAND_MODE & McolDistance
#define M_MAX_NORMALIZE                            0           // value for M_DISTANCE_IMAGE_NORMALIZE
#define M_NO_NORMALIZE                             -1.0        // value for M_DISTANCE_IMAGE_NORMALIZE
// Encodings
#define M_ENCODING_START                           60L
#define M_8BIT                                     M_ENCODING_START+8L          // used in matching context
#define M_10BIT                                    M_ENCODING_START+10L         // used in matching context
#define M_12BIT                                    M_ENCODING_START+12L         // used in matching context
#define M_14BIT                                    M_ENCODING_START+14L         // used in matching context
#define M_16BIT                                    M_ENCODING_START+16L         // used in matching context
#define M_USER_DEFINED                             21L                          // used in matching context      *also defined in MIL.H

// Control/Inquire Flags for Matching Result
#define M_GENERATE_PIXEL_MATCH                     300L        // used in matching result
#define M_GENERATE_SAMPLE_COLOR_LUT                305L        // used in matching result
#define M_GENERATE_DISTANCE_IMAGE                  310L        // used in matching result
#define M_SAVE_AREA_IMAGE                          315L        // used in matching result
#define M_DRAW_RELATIVE_ORIGIN_X                   319L        // deprecated : Use MgraControl(... M_DRAW_OFFSET_X...)
#define M_DRAW_RELATIVE_ORIGIN_Y                   320L        // deprecated : Use MgraControl(... M_DRAW_OFFSET_Y...)
#define M_DISTANCE_IMAGE_NORMALIZE                 215L        // used in matching context and macthing result
// --> To be deprecated in MIL10.0
#define M_GENERATE_LABEL_PIXEL_IMAGE               M_GENERATE_PIXEL_MATCH
// <--


// Result type Flag for Matching Result
#define M_AREA_IMAGE_SIZE_X                        330L        // used in matching result
#define M_AREA_IMAGE_SIZE_Y                        331L        // used in matching result
#define M_AREA_IMAGE_SIZE_BIT                      332L        // used in matching result
#define M_AREA_IMAGE_SIZE_BAND                     333L        // used in matching result  // (TBR: confusing, can't we just assume 1-band?)
#define M_AREA_IMAGE_SIGN                          334L        // used in matching result
#define M_AREA_IMAGE_TYPE                          335L        // used in matching result

#define M_DISTANCE_IMAGE_SIZE_X                    340L        // used in matching result
#define M_DISTANCE_IMAGE_SIZE_Y                    341L        // used in matching result
#define M_DISTANCE_IMAGE_SIZE_BIT                  342L        // used in matching result
#define M_DISTANCE_IMAGE_SIZE_BAND                 343L        // used in matching result  // (TBR: confusing, can't we just assume 1-band?)
#define M_DISTANCE_IMAGE_SIGN                      344L        // used in matching result
#define M_DISTANCE_IMAGE_TYPE                      345L        // used in matching result

#define M_PIXEL_MATCH_IMAGE_SIZE_X                 1447L        // used in matching result
#define M_PIXEL_MATCH_IMAGE_SIZE_Y                 1448L        // used in matching result
#define M_AREA_MATCH_IMAGE_SIZE_X                  1449L        // used in matching result
#define M_AREA_MATCH_IMAGE_SIZE_Y                  1450L        // used in matching result
#define M_SAMPLE_LABEL_SIZE_BIT                    1451L        // used in matching result and matching context
#define M_SAMPLE_LABEL_SIZE_BAND                   1452L        // used in matching result // (TBR: confusing, can't we just assume 1-band?)
#define M_SAMPLE_LABEL_SIGN                        1453L        // used in matching result
#define M_SAMPLE_LABEL_TYPE                        1454L        // used in matching result
#define M_SAMPLE_COLOR_SIZE_BIT                    1455L        // used in matching result
#define M_SAMPLE_COLOR_SIZE_BAND                   1456L        // used in matching result
#define M_SAMPLE_COLOR_SIGN                        1457L        // used in matching result
#define M_SAMPLE_COLOR_TYPE                        1458L        // used in matching result
//////////////////////////////////////////////////////////////////////////
// --> Flags to be deprecated in MIL 10.0 !
#define M_LABEL_PIXEL_IMAGE_SIZE_X                 350L        // used in matching result
#define M_LABEL_PIXEL_IMAGE_SIZE_Y                 351L        // used in matching result
#define M_LABEL_PIXEL_IMAGE_SIZE_BIT               352L        // used in matching result and matching context
#define M_LABEL_PIXEL_IMAGE_SIZE_BAND              353L        // used in matching result
#define M_LABEL_PIXEL_IMAGE_SIGN                   354L        // used in matching result
#define M_LABEL_PIXEL_IMAGE_TYPE                   355L        // used in matching result

#define M_COLORED_LABEL_PIXEL_IMAGE_SIZE_X         360L        // used in matching result
#define M_COLORED_LABEL_PIXEL_IMAGE_SIZE_Y         361L        // used in matching result
#define M_COLORED_LABEL_PIXEL_IMAGE_SIZE_BIT       362L        // used in matching result
#define M_COLORED_LABEL_PIXEL_IMAGE_SIZE_BAND      363L        // used in matching result
#define M_COLORED_LABEL_PIXEL_IMAGE_SIGN           364L        // used in matching result
#define M_COLORED_LABEL_PIXEL_IMAGE_TYPE           365L        // used in matching result

#define M_LABEL_AREA_IMAGE_SIZE_X                  370L        // used in matching result
#define M_LABEL_AREA_IMAGE_SIZE_Y                  371L        // used in matching result 
#define M_LABEL_AREA_IMAGE_SIZE_BIT                372L        // used in matching result and matching context
#define M_LABEL_AREA_IMAGE_SIZE_BAND               373L        // used in matching result
#define M_LABEL_AREA_IMAGE_SIGN                    374L        // used in matching result
#define M_LABEL_AREA_IMAGE_TYPE                    375L        // used in matching result

#define M_COLORED_LABEL_AREA_IMAGE_SIZE_X          380L        // used in matching result
#define M_COLORED_LABEL_AREA_IMAGE_SIZE_Y          381L        // used in matching result
#define M_COLORED_LABEL_AREA_IMAGE_SIZE_BIT        382L        // used in matching result
#define M_COLORED_LABEL_AREA_IMAGE_SIZE_BAND       383L        // used in matching result
#define M_COLORED_LABEL_AREA_IMAGE_SIGN            384L        // used in matching result
#define M_COLORED_LABEL_AREA_IMAGE_TYPE            385L        // used in matching result
// <--
//////////////////////////////////////////////////////////////////////////
#define M_MAX_DISTANCE                             12L         // used in matching result
#define M_LABEL_VALUE                              1L          // used in matching result       *also defined in miledge.h, milblob.h, milmetrol.h
#define M_SAMPLE_LABEL_VALUE                       M_LABEL_VALUE
#define M_AREA_LABEL_VALUE                         802L        // used in matching result
#define M_BEST_MATCH_INDEX                         804L        // used in matching result
#define M_BEST_MATCH_LABEL                         805L        // used in matching result
#define M_COLOR_DISTANCE                           806L        // used in matching result
#define M_SCORE                                    0x00001400L // used in matching result *also defined in milpat.h
#define M_NUMBER_OF_AREAS                          832L        // used in matching context
#define M_OUTLIER_COVERAGE                         808L        // used in matching result
#define M_STATUS                                   0x00008002L // used in matching result Already defined in milcode.h (in decimal: 32770)
#define M_SAMPLE_MATCH_STATUS                      809L        // used in matching result
#define M_NB_BEST_MATCH_SAMPLE                     810L        // used in matching result
#define M_NB_MATCH_SAMPLE                          811L        // used in matching result
#define M_NB_NO_MATCH_SAMPLE                       812L        // used in matching result
#define M_MATCH_INDEX                              820L        // used in matching result
#define M_MATCH_LABEL                              822L        // used in matching result
#define M_NO_MATCH_INDEX                           824L        // used in matching result
#define M_NO_MATCH_LABEL                           826L        // used in matching result
#define M_SAMPLE_COVERAGE                          830L        // used in matching result
#define M_SAMPLE_PIXEL_COUNT                       832L        // used in matching result
#define M_SCORE_RELEVANCE                          834L        // used in matching result
#define M_RELEVANCE_SCORE                          M_SCORE_RELEVANCE // keep synonym because PP1 EA released (TBR in MIL10)
#define M_AREA_PIXEL_COUNT                         838L        // used in matching result

// Result values
#define M_SUCCESS                                  0x00000000L // Already defined in milreg.h, used as status result
#define M_FAILURE                                  0x00000001L // Already defined in milreg.h, used as status result
#define M_TIE                                      0x00000002L // used as status result
#define M_MATCH                                    6L          // Already defined in milim.h, used as sample match status result
#define M_NO_MATCH                                 7L          // used as sample match status result

// Operation Flag
#define M_DELETE                                   3L          // used in McolDefine *also defined in milmod.h, miledge.h, milblob.h, milmetrol.h
#define M_IMAGE                                    0x00000004L // used in McolDefine *also defined in mil.h, milmod.h
#define M_TRIPLET                                  8L          // used in McolDefine
#define M_ADD_COLOR_TO_SAMPLE                      0x00001000L // used in McolDefine as combination flag
#define M_DONT_CARE                                0x00008000L // used in McolMask   *also defined in mil.h, milpat.h, milmod.h

// Draw flags
#define M_DRAW_PIXEL_MATCH_USING_LABEL             400L        // used in McolDraw and McolMatch
#define M_DRAW_PIXEL_MATCH_USING_COLOR             402L        // used in McolDraw and McolMatch
#define M_DRAW_AREA_MATCH_USING_LABEL              403L        // used in McolDraw and McolMatch
#define M_DRAW_AREA_MATCH_USING_COLOR              404L        // used in McolDraw and McolMatch
#define M_DRAW_DISTANCE                            405L        // used in McolDraw
#define M_DRAW_AREA                                410L        // used in McolDraw
#define M_DRAW_SAMPLE                              415L        // used in McolDraw
#define M_DRAW_SAMPLE_MASK                         416L        // used in McolDraw
#define M_DRAW_SAMPLE_COLOR_LUT                    420L        // used in McolDraw
#define M_DRAW_PIXEL_SCORE_IMAGE                   M_DRAW_DISTANCE      // TBM
#define M_DRAW_SAMPLE_MOSAIC                       1622L       // used in McolDraw
#define M_DRAW_SAMPLE_MOSAIC_DONT_CARE             1623L       // used in McolDraw

//////////////////////////////////////////////////////////////////////////
//  --> Flags to be deprecated in MIL 10.0 !
#define M_DRAW_LABEL_PIXEL_IMAGE                   M_DRAW_PIXEL_MATCH_USING_LABEL
#define M_DRAW_COLORED_LABEL_PIXEL_IMAGE           M_DRAW_PIXEL_MATCH_USING_COLOR
#define M_DRAW_LABEL_AREA_IMAGE                    M_DRAW_AREA_MATCH_USING_LABEL 
#define M_DRAW_COLORED_LABEL_AREA_IMAGE            M_DRAW_AREA_MATCH_USING_COLOR 
#define M_DRAW_DISTANCE_IMAGE                      M_DRAW_DISTANCE               
#define M_DRAW_AREA_IMAGE                          M_DRAW_AREA                   
#define M_DRAW_SAMPLE_IMAGE                        M_DRAW_SAMPLE                 
#define M_DRAW_SAMPLE_MASK_IMAGE                   M_DRAW_SAMPLE_MASK  
// <--
//////////////////////////////////////////////////////////////////////////

// Draw combination flags
#define M_INVERTED_COLORS                          0x00100000L // used in McolDraw and McolMatch

// Projection flags
#define M_COLOR_SEPARATION                         600L        // used in McolProject
#define M_PRINCIPAL_COMPONENT_PROJECTION           605L        // used in McolProject
#define M_COVARIANCE                               610L        // used in McolProject (McolSetMethod)
#define M_PRINCIPAL_COMPONENTS                     615L        // used in McolProject
#define M_MASK_CONTRAST_ENHANCEMENT                620L        // used in McolProject
#define M_BACKGROUND_LABEL                          43L        // used in McolProject
#define M_SELECTED_LABEL                           103L        // used in McolProject
#define M_REJECTED_LABEL                            73L        // used in McolProject
#define M_SOURCE_LABEL                             163L        // used in McolProject
#define M_BRIGHT_LABEL                             253L        // used in McolProject
#define M_DARK_LABEL                                83L        // used in McolProject
// McolProject result statuses
#define M_NO_BACKGROUND_DEFINED                    650L        // used in McolProject
#define M_NO_SELECTED_DEFINED                      652L        // used in McolProject
#define M_NO_REJECTED_DEFINED                      654L        // used in McolProject
#define M_REJECTED_EQUAL_SELECTED                  660L        // used in McolProject
#define M_REJECTED_EQUAL_BACKGROUND                662L        // used in McolProject
#define M_SELECTED_EQUAL_BACKGROUND                664L        // used in McolProject
#define M_3_COLORS_COLLINEAR                       668L        // used in McolProject
#define M_NO_SOURCE_DEFINED                        670L        // used in McolProject
#define M_UNSTABLE_POLARITY                        672L        // used in McolProject
#define M_UNSTABLE_PRINCIPAL_COMPONENT_2           680L        // used in McolProject
#define M_UNSTABLE_PRINCIPAL_COMPONENTS_12         682L        // used in McolProject
#define M_UNSTABLE_PRINCIPAL_COMPONENTS_012        684L        // used in McolProject
/***************************************************************************/
/*               MilColor CAPI function prototypes                         */
/***************************************************************************/

MIL_ID MFTYPE McolAlloc(MIL_ID      SystemId,
                        MIL_INT64   ContextType,
                        MIL_INT64   SourceColorSpace,
                        MIL_ID      ColorSpaceProfileId,
                        MIL_INT64   ControlFlag,
                        MIL_ID*     ContextIdPtr);

MIL_ID MFTYPE McolAllocResult(MIL_ID     SystemId, 
                              MIL_INT64  ResultType, 
                              MIL_INT64  ControlFlag, 
                              MIL_ID*    ResultIdPtr);

void   MFTYPE McolDefine(MIL_ID     ContextId,
                         MIL_ID     SrcImageId,
                         MIL_INT    UserLabelOrIndex,
                         MIL_INT64  ColorSampleType,
                         MIL_DOUBLE Param1,
                         MIL_DOUBLE Param2,
                         MIL_DOUBLE Param3,
                         MIL_DOUBLE Param4);

void MFTYPE McolPreprocess(MIL_ID      ContextId,
                           MIL_INT64   ControlFlag);

void MFTYPE McolMatch(MIL_ID     ContextId,
                      MIL_ID     TargetColorImageId,
                      MIL_ID     TargetColorProfileId,
                      MIL_ID     AreaIdentifierImageId,
                      MIL_ID     ColorResultOrDestImageId,
                      MIL_INT64  ControlFlag);

void MFTYPE McolFree(MIL_ID ContextIdOrResultId);

MIL_INT MFTYPE McolInquire(MIL_ID     ContextOrResultId,
                           MIL_INT    IndexOrLabel,
                           MIL_INT64  InquireType,
                           void      *UserVarPtr);

void MFTYPE McolGetResult(MIL_ID     ResultId,
                          MIL_INT    AreaLabel,
                          MIL_INT    ColorSampleIndexOrLabel,
                          MIL_INT64  ResultType,
                          void      *ResultArrayPtr);

void MFTYPE McolDistance(MIL_ID     Src1Id,
                         MIL_ID     Src2Id,
                         MIL_ID     DestId,
                         MIL_ID     MaskId,
                         MIL_ID     DistanceAttributesArrayId,
                         MIL_INT64  DistType,
                         MIL_DOUBLE NormalizeValue,
                         MIL_INT64  ControlFlag);

void MFTYPE McolDraw(MIL_ID     GraphContId,
                     MIL_ID     ContextOrResultId,
                     MIL_ID     DestImageId,
                     MIL_INT64  DrawOperation,
                     MIL_INT    AreaLabel,
                     MIL_INT    SampleIndexOrLabel,
                     MIL_INT64  ControlFlag);

void MFTYPE McolMask(MIL_ID     ContextId,
                     MIL_INT    ColorSampleIndexOrLabel,
                     MIL_ID     MaskBufferId,
                     MIL_INT64  MaskType,
                     MIL_INT64  ControlFlag);

void MFTYPE McolSetMethod(MIL_ID     ContextId,
                          MIL_INT64  OperationMode,
                          MIL_INT64  DistanceType,
                          MIL_INT64  ConversionMode,
                          MIL_INT64  ControlFlag);

void MFTYPE McolProject(MIL_ID      Src1ImageId,
                        MIL_ID      Src2ImageOrArrayId,
                        MIL_ID      DestImageOrArrayId,
                        MIL_ID      DestMaskImageId,
                        MIL_INT64   Operation,
                        MIL_INT64   ControlFlag,
                        MIL_INT*    StatusPtr);

#if M_MIL_USE_64BIT
// Prototypes for 64 bits OSs
void MFTYPE McolControlInt64    (MIL_ID      ContextId,
                                 MIL_INT     IndexOrLabel,
                                 MIL_INT64   ControlType,
                                 MIL_INT64   ControlValue);

void MFTYPE McolControlDouble   (MIL_ID      ContextId,
                                 MIL_INT     IndexOrLabel,
                                 MIL_INT64   ControlType,
                                 MIL_DOUBLE  ControlValue);

#else
// Prototypes for 32 bits OSs
#define McolControlInt64  McolControl
#define McolControlDouble McolControl
void MFTYPE McolControl         (MIL_ID      ContextId,
                                 MIL_INT     IndexOrLabel,
                                 MIL_INT64   ControlType,
                                 MIL_DOUBLE  ControlValue);
#endif

#if M_MIL_USE_UNICODE

MIL_ID MFTYPE McolRestoreW(MIL_CONST_TEXT_PTR FileName,
                           MIL_ID SystemId,
                           MIL_INT64  ControlFlag,
                           MIL_ID *ContextIdPtr);

void MFTYPE McolSaveW(MIL_CONST_TEXT_PTR FileName,
                      MIL_ID ContextId,
                      MIL_INT64  ControlFlag);

void MFTYPE McolStreamW(MIL_TEXT_PTR   MemPtrOrFileName,
                        MIL_ID         SystemId,
                        MIL_INT64      Operation,
                        MIL_INT64      StreamType,
                        MIL_DOUBLE     Version,
                        MIL_INT64      ControlFlag,
                        MIL_ID*        ContextIdPtr,
                        MIL_INT*       SizeByteVarPtr);

MIL_ID MFTYPE McolRestoreA(const char* FileName,
                           MIL_ID      SystemId,
                           MIL_INT64   ControlFlag,
                           MIL_ID*     ContextIdPtr);

void MFTYPE McolSaveA(const char* FileName,
                      MIL_ID      ContextId,
                      MIL_INT64   ControlFlag);

void MFTYPE McolStreamA(char*      MemPtrOrFileName,
                        MIL_ID     SystemId,
                        MIL_INT64  Operation,
                        MIL_INT64  StreamType,
                        MIL_DOUBLE Version,
                        MIL_INT64  ControlFlag,
                        MIL_ID*    ContextIdPtr,
                        MIL_INT*   SizeByteVarPtr);

#if M_MIL_UNICODE_API
#define McolSave           McolSaveW
#define McolRestore        McolRestoreW
#define McolStream         McolStreamW
#else
#define McolSave           McolSaveA
#define McolRestore        McolRestoreA
#define McolStream         McolStreamA
#endif

#else

MIL_ID MFTYPE McolRestore(MIL_CONST_TEXT_PTR FileName,
                          MIL_ID SystemId,
                          MIL_INT64  ControlFlag,
                          MIL_ID *ContextIdPtr);

void MFTYPE McolSave(MIL_CONST_TEXT_PTR FileName,
                     MIL_ID ContextId,
                     MIL_INT64  ControlFlag);

void MFTYPE McolStream(MIL_TEXT_PTR MemPtrOrFileName,
                       MIL_ID       SystemId,
                       MIL_INT64    Operation,
                       MIL_INT64    StreamType,
                       MIL_DOUBLE   Version,
                       MIL_INT64    ControlFlag,
                       MIL_ID*      ContextIdPtr,
                       MIL_INT*     SizeByteVarPtr);
#endif

   /* C++ directive if needed */
#ifdef __cplusplus
   }
#endif

#if M_MIL_USE_64BIT
#ifdef __cplusplus
//////////////////////////////////////////////////////////////
// McolControl function definition when compiling c++ files
//////////////////////////////////////////////////////////////
#if !M_MIL_USE_LINUX
inline void McolControl(MIL_ID     ContextId, 
                        MIL_INT    IndexOrLabel,
                        MIL_INT64  ControlType, 
                        int        ControlValue)
   {
   McolControlInt64(ContextId, IndexOrLabel, ControlType, ControlValue);
   };
#endif

inline void McolControl(MIL_ID      ContextId, 
                        MIL_INT     IndexOrLabel,
                        MIL_INT64   ControlType, 
                        MIL_INT32   ControlValue)
   {
   McolControlInt64(ContextId, IndexOrLabel, ControlType, ControlValue);
   }
inline void McolControl(MIL_ID      ContextId, 
                        MIL_INT     IndexOrLabel,
                        MIL_INT64   ControlType, 
                        MIL_INT64   ControlValue)
   {
   McolControlInt64(ContextId, IndexOrLabel, ControlType, ControlValue);
   }
inline void McolControl(MIL_ID      ContextId, 
                        MIL_INT     IndexOrLabel,
                        MIL_INT64   ControlType, 
                        MIL_DOUBLE  ControlValue)
   {
   McolControlDouble(ContextId, IndexOrLabel, ControlType, ControlValue);
   }

#else
//////////////////////////////////////////////////////////////
// For C file, call the default function, i.e. Double one
//////////////////////////////////////////////////////////////
#define McolControl  McolControlDouble

#endif // __cplusplus
#endif // M_MIL_USE_64BIT

#if M_MIL_USE_SAFE_TYPE

   //////////////////////////////////////////////////////////////
   // See milos.h for explanation about these functions.
   //////////////////////////////////////////////////////////////

   // -------------------------------------------------------------------------
   // McolGetResult

   inline void MFTYPE McolGetResultSafeType (MIL_ID ResultId, MIL_INT AreaLabel, MIL_INT ColorSampleIndexOrLabel, MIL_INT64  ResultType, int ResultArrayPtr)
      {
      if (ResultArrayPtr != 0)
         SafeTypeError(MIL_TEXT("McolGetResult"));

      McolGetResult(ResultId, AreaLabel, ColorSampleIndexOrLabel, ResultType, NULL);
      }

   inline void McolGetResultSafeTypeExecute (MIL_ID ResultId, MIL_INT AreaLabel, MIL_INT ColorSampleIndexOrLabel, MIL_INT64  ResultType, void  *ResultArrayPtr, MIL_INT64  GivenType)
      {
      MIL_INT64  RequiredType = (ResultType & M_HLVLDATATYPE_MASK);
      if((RequiredType != M_TYPE_DOUBLE) && (RequiredType != M_TYPE_MIL_INT32) && (RequiredType != M_TYPE_MIL_INT64) && 
         (RequiredType != M_TYPE_MIL_ID) && (RequiredType != M_TYPE_FLOAT) && (RequiredType != M_TYPE_CHAR) && (RequiredType != M_TYPE_SHORT))
         RequiredType = 0;
      if (RequiredType == 0)
         RequiredType = M_TYPE_DOUBLE;
      ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

      if (RequiredType != GivenType)
         SafeTypeError(MIL_TEXT("McolGetResult"));

      McolGetResult(ResultId, AreaLabel, ColorSampleIndexOrLabel, ResultType, ResultArrayPtr);
      }

   inline void MFTYPE McolGetResultUnsafe  (MIL_ID ResultId, MIL_INT AreaLabel, MIL_INT ColorSampleIndexOrLabel, MIL_INT64  ResultType, void        *ResultArrayPtr){McolGetResult               (ResultId, AreaLabel, ColorSampleIndexOrLabel, ResultType, ResultArrayPtr);}
   inline void MFTYPE McolGetResultSafeType(MIL_ID ResultId, MIL_INT AreaLabel, MIL_INT ColorSampleIndexOrLabel, MIL_INT64  ResultType, MIL_INT8    *ResultArrayPtr){McolGetResultSafeTypeExecute(ResultId, AreaLabel, ColorSampleIndexOrLabel, ResultType, ResultArrayPtr, M_TYPE_CHAR     );}
   inline void MFTYPE McolGetResultSafeType(MIL_ID ResultId, MIL_INT AreaLabel, MIL_INT ColorSampleIndexOrLabel, MIL_INT64  ResultType, MIL_INT16   *ResultArrayPtr){McolGetResultSafeTypeExecute(ResultId, AreaLabel, ColorSampleIndexOrLabel, ResultType, ResultArrayPtr, M_TYPE_SHORT    );}
   inline void MFTYPE McolGetResultSafeType(MIL_ID ResultId, MIL_INT AreaLabel, MIL_INT ColorSampleIndexOrLabel, MIL_INT64  ResultType, MIL_INT32   *ResultArrayPtr){McolGetResultSafeTypeExecute(ResultId, AreaLabel, ColorSampleIndexOrLabel, ResultType, ResultArrayPtr, M_TYPE_MIL_INT32);}
   inline void MFTYPE McolGetResultSafeType(MIL_ID ResultId, MIL_INT AreaLabel, MIL_INT ColorSampleIndexOrLabel, MIL_INT64  ResultType, MIL_INT64   *ResultArrayPtr){McolGetResultSafeTypeExecute(ResultId, AreaLabel, ColorSampleIndexOrLabel, ResultType, ResultArrayPtr, M_TYPE_MIL_INT64);}
   inline void MFTYPE McolGetResultSafeType(MIL_ID ResultId, MIL_INT AreaLabel, MIL_INT ColorSampleIndexOrLabel, MIL_INT64  ResultType, float       *ResultArrayPtr){McolGetResultSafeTypeExecute(ResultId, AreaLabel, ColorSampleIndexOrLabel, ResultType, ResultArrayPtr, M_TYPE_FLOAT    );}
   inline void MFTYPE McolGetResultSafeType(MIL_ID ResultId, MIL_INT AreaLabel, MIL_INT ColorSampleIndexOrLabel, MIL_INT64  ResultType, MIL_DOUBLE  *ResultArrayPtr){McolGetResultSafeTypeExecute(ResultId, AreaLabel, ColorSampleIndexOrLabel, ResultType, ResultArrayPtr, M_TYPE_DOUBLE   );}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED                                                                       
   inline void MFTYPE McolGetResultSafeType(MIL_ID ResultId, MIL_INT AreaLabel, MIL_INT ColorSampleIndexOrLabel, MIL_INT64  ResultType, MIL_UINT8   *ResultArrayPtr){McolGetResultSafeTypeExecute(ResultId, AreaLabel, ColorSampleIndexOrLabel, ResultType, ResultArrayPtr, M_TYPE_CHAR     );}
   inline void MFTYPE McolGetResultSafeType(MIL_ID ResultId, MIL_INT AreaLabel, MIL_INT ColorSampleIndexOrLabel, MIL_INT64  ResultType, MIL_UINT16  *ResultArrayPtr){McolGetResultSafeTypeExecute(ResultId, AreaLabel, ColorSampleIndexOrLabel, ResultType, ResultArrayPtr, M_TYPE_SHORT    );}
   inline void MFTYPE McolGetResultSafeType(MIL_ID ResultId, MIL_INT AreaLabel, MIL_INT ColorSampleIndexOrLabel, MIL_INT64  ResultType, MIL_UINT32  *ResultArrayPtr){McolGetResultSafeTypeExecute(ResultId, AreaLabel, ColorSampleIndexOrLabel, ResultType, ResultArrayPtr, M_TYPE_MIL_INT32);}
   inline void MFTYPE McolGetResultSafeType(MIL_ID ResultId, MIL_INT AreaLabel, MIL_INT ColorSampleIndexOrLabel, MIL_INT64  ResultType, MIL_UINT64  *ResultArrayPtr){McolGetResultSafeTypeExecute(ResultId, AreaLabel, ColorSampleIndexOrLabel, ResultType, ResultArrayPtr, M_TYPE_MIL_INT64);}
#endif

   // ----------------------------------------------------------
   // McolInquire

   inline MIL_INT MFTYPE McolInquireSafeType  (MIL_ID ContextId, MIL_INT IndexOrLabel, MIL_INT64  InquireType, int UserVarPtr)
      {
      if(UserVarPtr != 0)
         SafeTypeError(MIL_TEXT("McolInquire"));

      return McolInquire(ContextId, IndexOrLabel, InquireType, NULL );
      }

   inline MIL_INT MFTYPE McolInquireSafeTypeExecute (MIL_ID ContextId, MIL_INT IndexOrLabel, MIL_INT64  InquireType, void *UserVarPtr, MIL_INT64  GivenType)
      {
      MIL_INT64  RequiredType = (InquireType & M_HLVLDATATYPE_MASK);
      if((RequiredType != M_TYPE_DOUBLE) && (RequiredType != M_TYPE_MIL_INT32) && (RequiredType != M_TYPE_MIL_INT64) && 
         (RequiredType != M_TYPE_MIL_ID) && (RequiredType != M_TYPE_FLOAT) && (RequiredType != M_TYPE_CHAR) && (RequiredType != M_TYPE_SHORT))
         RequiredType = 0;
      if (RequiredType == 0)
         RequiredType = M_TYPE_DOUBLE;
      ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

      if (RequiredType != GivenType)
         SafeTypeError(MIL_TEXT("McolInquire"));

      return McolInquire(ContextId, IndexOrLabel, InquireType, UserVarPtr);
      }

   inline MIL_INT MFTYPE McolInquireUnsafe  (MIL_ID ContextId, MIL_INT IndexOrLabel, MIL_INT64  InquireType, void       *UserVarPtr) {return McolInquire               (ContextId, IndexOrLabel, InquireType, UserVarPtr                  );}
   inline MIL_INT MFTYPE McolInquireSafeType(MIL_ID ContextId, MIL_INT IndexOrLabel, MIL_INT64  InquireType, MIL_INT8   *UserVarPtr) {return McolInquireSafeTypeExecute(ContextId, IndexOrLabel, InquireType, UserVarPtr, M_TYPE_CHAR     );}
   inline MIL_INT MFTYPE McolInquireSafeType(MIL_ID ContextId, MIL_INT IndexOrLabel, MIL_INT64  InquireType, MIL_INT16  *UserVarPtr) {return McolInquireSafeTypeExecute(ContextId, IndexOrLabel, InquireType, UserVarPtr, M_TYPE_SHORT    );}
   inline MIL_INT MFTYPE McolInquireSafeType(MIL_ID ContextId, MIL_INT IndexOrLabel, MIL_INT64  InquireType, MIL_INT32  *UserVarPtr) {return McolInquireSafeTypeExecute(ContextId, IndexOrLabel, InquireType, UserVarPtr, M_TYPE_MIL_INT32);}
   inline MIL_INT MFTYPE McolInquireSafeType(MIL_ID ContextId, MIL_INT IndexOrLabel, MIL_INT64  InquireType, MIL_INT64  *UserVarPtr) {return McolInquireSafeTypeExecute(ContextId, IndexOrLabel, InquireType, UserVarPtr, M_TYPE_MIL_INT64);}
   inline MIL_INT MFTYPE McolInquireSafeType(MIL_ID ContextId, MIL_INT IndexOrLabel, MIL_INT64  InquireType, float      *UserVarPtr) {return McolInquireSafeTypeExecute(ContextId, IndexOrLabel, InquireType, UserVarPtr, M_TYPE_FLOAT    );}
   inline MIL_INT MFTYPE McolInquireSafeType(MIL_ID ContextId, MIL_INT IndexOrLabel, MIL_INT64  InquireType, MIL_DOUBLE *UserVarPtr) {return McolInquireSafeTypeExecute(ContextId, IndexOrLabel, InquireType, UserVarPtr, M_TYPE_DOUBLE   );}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED                                                                                                                                  
   inline MIL_INT MFTYPE McolInquireSafeType(MIL_ID ContextId, MIL_INT IndexOrLabel, MIL_INT64  InquireType, MIL_UINT8  *UserVarPtr) {return McolInquireSafeTypeExecute(ContextId, IndexOrLabel, InquireType, UserVarPtr, M_TYPE_CHAR     );}
   inline MIL_INT MFTYPE McolInquireSafeType(MIL_ID ContextId, MIL_INT IndexOrLabel, MIL_INT64  InquireType, MIL_UINT16 *UserVarPtr) {return McolInquireSafeTypeExecute(ContextId, IndexOrLabel, InquireType, UserVarPtr, M_TYPE_SHORT    );}
   inline MIL_INT MFTYPE McolInquireSafeType(MIL_ID ContextId, MIL_INT IndexOrLabel, MIL_INT64  InquireType, MIL_UINT32 *UserVarPtr) {return McolInquireSafeTypeExecute(ContextId, IndexOrLabel, InquireType, UserVarPtr, M_TYPE_MIL_INT32);}
   inline MIL_INT MFTYPE McolInquireSafeType(MIL_ID ContextId, MIL_INT IndexOrLabel, MIL_INT64  InquireType, MIL_UINT64 *UserVarPtr) {return McolInquireSafeTypeExecute(ContextId, IndexOrLabel, InquireType, UserVarPtr, M_TYPE_MIL_INT64);}
#endif

#define McolGetResult        McolGetResultSafeType
#define McolInquire          McolInquireSafeType

#else // #if M_MIL_USE_SAFE_TYPE

#define McolGetResultUnsafe        McolGetResult
#define McolInquireUnsafe          McolInquire

#endif // #if M_MIL_USE_SAFE_TYPE

#endif // !M_MIL_LITE

#endif /* __MILCOLOR_H__ */

