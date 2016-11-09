/***************************************************************************/
/*

    Filename:  MILBLOB.H
    Owner   :  Matrox Imaging dept.
    Revision:  10.00.2564
    Content :  This file contains the defines for the MIL blob
               analysis module. (Mblob...).

    Copyright © Matrox Electronic Systems Ltd., 1992-2014.
    All Rights Reserved

*/
/***************************************************************************/

#ifndef __MILBLOB_H__
#define __MILBLOB_H__

#if (!M_MIL_LITE) // MIL FULL ONLY

/* C++ directive if needed */
#ifdef __cplusplus
extern "C"
{
#endif

/* Binary only */

#define M_LABEL_VALUE              1L
#define M_AREA                     2L
#define M_PERIMETER                3L
#define M_FERET_X                 72L
#define M_FERET_Y                  5L
#define M_WORLD_FERET_X         1465L
#define M_WORLD_FERET_Y         1466L
#define M_WORLD_BOX_X_MIN       1479L
#define M_WORLD_BOX_Y_MIN       1480L
#define M_WORLD_BOX_X_MAX       1481L
#define M_WORLD_BOX_Y_MAX       1482L
#define M_BOX_X_MIN                6L
#define M_BOX_Y_MIN                7L
#define M_BOX_X_MAX                8L
#define M_BOX_Y_MAX                9L
#define M_BOX_AREA                90L
#define M_BOX_ASPECT_RATIO        91L
#define M_BOX_FILL_RATIO          92L
#define M_CONVEX_HULL_FILL_RATIO  93L
#define M_FIRST_POINT_X           75L
#define M_FIRST_POINT_Y          132L
#define M_AXIS_PRINCIPAL_LENGTH   12L
#define M_AXIS_SECONDARY_LENGTH   13L
#define M_FERET_MIN_DIAMETER      14L
#define M_FERET_MIN_ANGLE         15L
#define M_FERET_MAX_DIAMETER      16L
#define M_FERET_MAX_ANGLE         17L
#define M_FERET_MEAN_DIAMETER     18L
#define M_CONVEX_AREA             19L
#define M_CONVEX_PERIMETER        20L
#define M_CONVEX_HULL             80L
#define M_X_MIN_AT_Y_MIN          21L
#define M_X_MAX_AT_Y_MAX          22L
#define M_Y_MIN_AT_X_MAX          23L
#define M_Y_MAX_AT_X_MIN          24L
#define M_X_MIN_AT_Y_MAX          58L
#define M_X_MAX_AT_Y_MIN          59L
#define M_Y_MIN_AT_X_MIN          60L
#define M_Y_MAX_AT_X_MAX          61L
#define M_WORLD_X_AT_Y_MIN      1483L
#define M_WORLD_X_AT_Y_MAX      1484L
#define M_WORLD_Y_AT_X_MIN      1485L
#define M_WORLD_Y_AT_X_MAX      1486L
#define M_COMPACTNESS             25L
#define M_NUMBER_OF_HOLES         26L
#define M_FERET_ELONGATION        27L
#define M_ROUGHNESS               28L
#define M_EULER_NUMBER            47L
#define M_LENGTH          0x00002000L
#define M_BREADTH                 49L
#define M_ELONGATION              50L
#define M_INTERCEPT_0             51L
#define M_INTERCEPT_45            52L
#define M_INTERCEPT_90            53L
#define M_INTERCEPT_135           54L
#define M_NUMBER_OF_RUNS          55L
#define M_GENERAL_FERET        0x400L
#define M_TIMEOUT_END             10L //Already defined in milmod.h
#define M_MAX_BLOBS_END           57L
#define M_RETURN_PARTIAL_RESULTS  70L
#define M_STOP_CALCULATE         116L
#define M_TOTAL_NUMBER_OF_RUNS   117L
#define M_BLOB_TOUCHING_IMAGE_BORDERS  118L

#define M_FERET_AT_PRINCIPAL_AXIS_ANGLE     119L
#define M_FERET_AT_SECONDARY_AXIS_ANGLE     120L
#define M_FERET_PRINCIPAL_AXIS_ELONGATION   121L
#define M_ALL_FERETS                        122L

#define M_FERET_PERPENDICULAR_TO_MIN_DIAMETER       123L
#define M_FERET_PERPENDICULAR_TO_MAX_DIAMETER       124L
#define M_FERET_MIN_DIAMETER_ELONGATION             125L
#define M_FERET_MAX_DIAMETER_ELONGATION             126L
#define M_RECTANGULARITY                            127L

/* Greyscale only (ie, trivial for binary) */

#define M_SUM_PIXEL               29L
#define M_MIN_PIXEL               30L
#define M_MAX_PIXEL               31L
#define M_MEAN_PIXEL              32L
#define M_SIGMA_PIXEL             33L
#define M_SUM_PIXEL_SQUARED       46L

/* Binary or greyscale (might want both for a greyscale image) */

#define M_CENTER_OF_GRAVITY_X     34L
#define M_CENTER_OF_GRAVITY_Y     35L
#define M_MOMENT_X0_Y1            36L
#define M_MOMENT_X1_Y0            37L
#define M_MOMENT_X1_Y1            38L
#define M_MOMENT_X0_Y2            39L
#define M_MOMENT_X2_Y0            40L
#define M_MOMENT_CENTRAL_X1_Y1    41L
#define M_MOMENT_CENTRAL_X0_Y2    42L
#define M_MOMENT_CENTRAL_X2_Y0    43L
#define M_AXIS_PRINCIPAL_ANGLE    44L
#define M_AXIS_SECONDARY_ANGLE    45L
#define M_GENERAL_MOMENT       0x800L

/* General moment type */

#define M_ORDINARY             0x400L
#define M_CENTRAL              0x800L

/* the defines are used by the active mil BlobAnalysis.Results.Get method YV */

#define M_RUN_LENGTHS            100L
#define M_RUN_X_COORDINATES      101L
#define M_RUN_Y_COORDINATES      102L

/* Sorting options. */
#define M_NO_SORT         0L
#define M_SORT1_UP        0x02000000L
#define M_SORT2_UP        0x04000000L
#define M_SORT3_UP        0x06000000L
#define M_SORT1_DOWN      0x0A000000L
#define M_SORT2_DOWN      0x0C000000L
#define M_SORT3_DOWN      0x0E000000L

/* Chain code specific features */
#define M_NUMBER_OF_CHAINED_PIXELS 56L
#define M_CHAIN_X                  65L
#define M_CHAIN_Y                 144L
#define M_CHAIN_INDEX              67L
#define M_CHAIN_INDEXXY_PACKED     62L

/* Convex Hull specific features */
#define M_CONVEX_HULL_X                      81L
#define M_CONVEX_HULL_Y                      82L
#define M_CONVEX_HULL_XY_PACKED              83L
#define M_NUMBER_OF_CONVEX_HULL_POINTS       84L
#define M_FULL_CONVEX_HULL_X                 85L
#define M_FULL_CONVEX_HULL_Y                 86L
#define M_FULL_CONVEX_HULL_XY_PACKED         87L
#define M_NUMBER_OF_FULL_CONVEX_HULL_POINTS  88L
#define M_CONVEX_HULL_AREA                   89L
#define M_CONVEX_HULL_COG_X                  96L
#define M_CONVEX_HULL_COG_Y                  97L

#define M_BLOB_INCLUSION_STATE          0x108L

/* Short cuts for enabling multiple features */

#define M_ALL_FEATURES         0x100L   /* All except general Feret */
#define M_ALL_FEATURES_EXCEPT_CHAINS 0x105L   /* All except chains */
#define M_BOX                  0x101L
#define M_CONTACT_POINTS       0x102L
#define M_CENTER_OF_GRAVITY    0x103L
#define M_NO_FEATURES          0x104L   /* Still do label and area */
#define M_CHAIN                209L     /* Also defined in miledge.h */
#define M_CHAINS               M_CHAIN  /* Also defined in miledge.h */
#define M_WORLD_BOX            1487L
#define M_ALL_SIMPLE_FEATURES  94L

/* MblobControl() and/or MblobInquire() values */

#define M_IMAGE_TYPE               1L
#define M_BLOB_IDENTIFICATION      2L
#define M_LATTICE                  3L
#define M_FOREGROUND_VALUE         4L
#define M_PIXEL_ASPECT_RATIO       5L
#define M_NUMBER_OF_FERETS        63L
#define M_IMAGE_START_X            7L
#define M_IMAGE_START_Y            8L
#define M_RESET                    9L
#define M_IDENTIFIER_IMAGE        10L
#define M_GREY_IMAGE              11L
#define M_IDENTIFIER_START_BIT    12L
#define M_GREY_START_BIT          13L
#define M_SAVE_RUNS               14L
#define M_IDENTIFIER_TYPE         15L
#define M_MAX_LABEL               16L
#define M_SELECT_FEATURE          17L
#define M_AUTO_RESET      0x00002000L  // Already defined in mil.h
#define M_TIMEOUT               2077L  // Already defined in mil.h
#define M_MAX_BLOBS               18L
#define M_CLEAR_RESULT            19L
#define M_INPUT_SELECT_UNITS      20L
#define M_ALLOCATION_FAILURE_REAL_TEST_FLAG            971L // Also in milstr.h
#define M_RESULT_OUTPUT_UNITS   1300L // Also in milim.h
#define M_LABELLED_OLD_BEHAVIOR 1687L

// SQA Memory Leak Isolator Self Test
#define M_CHECK_SQA_MEM_LEAK_ISOLATOR           1688L

/* Blob identification values */

#define M_WHOLE_IMAGE              1L
#define M_INDIVIDUAL               2L
#define M_LABELLED                 4L
#define M_LABELED          M_LABELLED  // One "L" is american english while
                                       // two "LL" is British (ref. Thalia tech writ.).
#define M_LABELED_TOUCHING         5L

/* Enabling value. */

#define M_ENABLE               -9997L
#define M_DISABLE              -9999L

/* Lattice values */

#define M_4_CONNECTED     0x00000010L
#define M_8_CONNECTED     0x00000020L

#define M_YES                           1L //Already defined in milmeas.h
#define M_NO                            0L //Already defined in milmeas.h

#define M_INCLUDED                      0x109L
#define M_EXCLUDED                      0x110L


/* Foreground values */

#define M_NONZERO               0x80L
#define M_ZERO                 0x100L
#define M_NON_ZERO          M_NONZERO
#define M_FOREGROUND_WHITE      0x80L
#define M_FOREGROUND_BLACK     0x100L

/* Parameters for output units */
#define M_PIXEL                              0x1000L
#define M_WORLD                              0x2000L
#define M_ACCORDING_TO_CALIBRATION             1301L
#define M_ACCORDING_TO_RESULT                  1302L


/* Data types for results are no longer redefined... to avoid all the conditions and defines around MIL_INT variables (x64)*/


/* Conditional test */

#define M_ALWAYS              0L
#define M_IN_RANGE            1L
#define M_OUT_RANGE           2L
#define M_EQUAL               3L
#define M_NOT_EQUAL           4L
#define M_GREATER             5L
#define M_LESS                6L
#define M_GREATER_OR_EQUAL    7L
#define M_LESS_OR_EQUAL       8L


/* Image type values (and flags for blob_enable_feature()) */

#define M_BINARY          0x00001000L
#define M_GREYSCALE           0x0200L
#define M_GRAYSCALE       M_GREYSCALE


/* MblobReconstruct() defines */

#define M_RECONSTRUCT_FROM_SEED     1L
#define M_ERASE_BORDER_BLOBS        2L
#define M_FILL_HOLES                3L
#define M_EXTRACT_HOLES             4L
#define M_SEED_PIXELS_ALL_IN_BLOBS  1L
#define M_FOREGROUND_ZERO           2L

/* MblobDraw() defines */
#define M_DRAW_BLOBS                0x00000400L
#define M_DRAW_BLOBS_CONTOUR        0x00000100L
#define M_DRAW_HOLES                0x00002000L
#define M_DRAW_HOLES_CONTOUR        0x00004000L
#define M_DRAW_CONVEX_HULL          0x00008000L
#define M_DRAW_CONVEX_HULL_CONTOUR  0x00010000L
#define M_DRAW_BOX                  0x00000020L     // also used in m*draw
#define M_DRAW_WORLD_BOX            0x00020000L
#define M_DRAW_POSITION             0x00000040L     // also used in m*draw
#define M_DRAW_CENTER_OF_GRAVITY    0x00000080L
#define M_DRAW_FERET_MIN            0x00001000L
#define M_DRAW_FERET_MAX            0x00000008L
#define M_DRAW_FERET_BOX            0x00000010L
#define M_DRAW_AXIS                 0x00000200L     // also used in mmoddraw
#define M_DRAW_WORLD_BOX_CENTER     0x00040000L
#define M_DRAW_BOX_CENTER           0x00000001L     // also defined in milmeas.h
#define M_DRAW_WORLD_FERET_X        0x00080000L
#define M_DRAW_WORLD_FERET_Y        0x00100000L
#define M_DRAW_RELATIVE_ORIGIN_X           319L     // deprecated : Use MgraControl(... M_DRAW_OFFSET_X...)
#define M_DRAW_RELATIVE_ORIGIN_Y           320L     // deprecated : Use MgraControl(... M_DRAW_OFFSET_Y...)
#define M_DRAW_SCALE_X                    3203L     // deprecated : Use MgraControl(... M_DRAW_ZOOM_X ...)
#define M_DRAW_SCALE_Y                    3204L     // deprecated : Use MgraControl(... M_DRAW_ZOOM_Y ...)

/* MblobMerge() control flags */
#define M_TOP_BOTTOM                0x00001000L
#define M_LEFT_RIGHT                0x00002000L
#define M_MOVE                      0x00010000L     // already defined in mil.h
#define M_COPY                      0x00020000L     // already defined in mil.h
#define M_CLONE                     0x00040000L

/* Miscellaneous */
#define M_ALL_BLOBS           0x40000000L
#define M_INCLUDED_BLOBS      0x08000000L
#define M_EXCLUDED_BLOBS      0x20000000L
#define M_INCLUDE                  1L
#define M_EXCLUDE                  2L
#define M_DELETE                   3L
#define M_MERGE           0x00000040L
#define M_MIN_FERETS               2L
#define M_MAX_FERETS              64L
#define M_INCLUDE_ONLY         0x101L
#define M_EXCLUDE_ONLY         0x102L
#define M_CONTOUR              0x800L
#define M_DUMMY               0x4000L
#define M_CLEAR           0x00000001L
#define M_NO_CLEAR        0x00000002L

/* Other defines are in MIL.H */

/********************************************************************
 * Function prototypes
 ********************************************************************/
MIL_ID MFTYPE MblobAllocFeatureList(MIL_ID SystemId,MIL_ID* FeatureListIdPtr);
MIL_ID MFTYPE MblobAllocResult(MIL_ID SystemId, MIL_ID* BlobResIdPtr);
void MFTYPE MblobCalculate(MIL_ID BlobIdentImageId, MIL_ID GrayImageId,
              MIL_ID FeatureListId, MIL_ID BlobResId);

#if M_MIL_USE_64BIT
// Prototypes for 64 bits OSs
void MFTYPE MblobControlInt64(MIL_ID BlobResId, MIL_INT64  ControlType, MIL_INT64 ControlValue);
void MFTYPE MblobControlDouble(MIL_ID BlobResId, MIL_INT64  ControlType, MIL_DOUBLE ControlValue);
#else
// Prototypes for 32 bits OSs
#define MblobControlInt64  MblobControl
#define MblobControlDouble MblobControl
void MFTYPE MblobControl(MIL_ID BlobResId, MIL_INT64  ControlType, MIL_DOUBLE ControlValue);
#endif

void MFTYPE MblobFill(MIL_ID BlobResId, MIL_ID DestImageBufId, MIL_INT64  Criterion, MIL_INT Value);
void MFTYPE MblobFree(MIL_ID BlobId);
MIL_INT MFTYPE MblobGetLabel(MIL_ID BlobResId, MIL_INT XPos, MIL_INT YPos, MIL_INT* LabelVarPtr);
MIL_INT MFTYPE MblobGetNumber(MIL_ID BlobResId, MIL_INT* CountVarPtr);
void MFTYPE MblobGetResult(MIL_ID BlobResId, MIL_INT64  Feature, void* TargetArrayPtr);
void MFTYPE MblobGetResultSingle(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  Feature, void* TargetVarPtr);
void MFTYPE MblobGetRuns(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  ArrayType,
                         void* RunXPtr, void* RunYPtr, void* RunLengthPtr);
MIL_INT MFTYPE MblobInquire(MIL_ID ResOrFeatListBlobId, MIL_INT64  InquireType, void* UserVarPtr);
void MFTYPE MblobLabel(MIL_ID BlobResId, MIL_ID DestImageBufId, MIL_INT64  Mode);
void MFTYPE MblobSelect(MIL_ID BlobResId, MIL_INT64  Operation, MIL_INT64  SelectionCriterion,
                        MIL_INT64  Condition, MIL_DOUBLE CondLow, MIL_DOUBLE CondHigh);
void MFTYPE MblobSelectFeature(MIL_ID FeatureListId, MIL_INT64  Feature);
void MFTYPE MblobSelectFeret(MIL_ID FeatureListId, MIL_DOUBLE Angle);
void MFTYPE MblobSelectMoment(MIL_ID FeatureListId, MIL_INT64  MomType,
                              MIL_INT XMomOrder, MIL_INT YMomOrder);
void MFTYPE MblobReconstruct(MIL_ID SrcImageBufId, MIL_ID SeedImageBufId,
                             MIL_ID DestImageBufId, MIL_INT64  Operation, MIL_INT64  ProcMode);
void MFTYPE MblobMerge(MIL_ID SourceResult1, MIL_ID SourceResult2,
                       MIL_ID DestResult, MIL_INT64  ControlFlag);

void MFTYPE MblobDraw(MIL_ID ContextGraId, MIL_ID ResultId, MIL_ID DstImageBufOrListGraId,
                      MIL_INT64  Operation, MIL_INT Label, MIL_INT64  ControlFlag);


/* C++ directive if needed */
#ifdef __cplusplus
}
#endif

#if M_MIL_USE_64BIT
#ifdef __cplusplus

#if !M_MIL_USE_LINUX
//////////////////////////////////////////////////////////////
// MblobControl function definition when compiling c++ files
//////////////////////////////////////////////////////////////
inline void MblobControl(MIL_ID BlobResId, MIL_INT64  ControlType, int ControlValue)
   {
   MblobControlInt64(BlobResId, ControlType, ControlValue);
   };
#endif

inline void MblobControl(MIL_ID BlobResId, MIL_INT64  ControlType, MIL_INT32 ControlValue)
   {
   MblobControlInt64(BlobResId, ControlType, ControlValue);
   };

inline void MblobControl(MIL_ID BlobResId, MIL_INT64  ControlType, MIL_INT64 ControlValue)
   {
   MblobControlInt64(BlobResId, ControlType, ControlValue);
   };

inline void MblobControl(MIL_ID BlobResId, MIL_INT64  ControlType, MIL_DOUBLE ControlValue)
   {
   MblobControlDouble(BlobResId, ControlType, ControlValue);
   };
#else
//////////////////////////////////////////////////////////////
// For C file, call the default function, i.e. Int64 one
//////////////////////////////////////////////////////////////
#define MblobControl  MblobControlDouble

#endif // __cplusplus
#endif // M_MIL_USE_64BIT

#if M_MIL_USE_SAFE_TYPE

//////////////////////////////////////////////////////////////
// See milos.h for explanation about these functions.
//////////////////////////////////////////////////////////////

// ----------------------------------------------------------
// MblobGetResult

inline void MFTYPE MblobGetResultUnsafe  (MIL_ID BlobResId, MIL_INT64  Feature, void*       TargetArrayPtr);
inline void MFTYPE MblobGetResultSafeType(MIL_ID BlobResId, MIL_INT64  Feature, int         TargetArrayPtr);
inline void MFTYPE MblobGetResultSafeType(MIL_ID BlobResId, MIL_INT64  Feature, MIL_INT8*   TargetArrayPtr);
inline void MFTYPE MblobGetResultSafeType(MIL_ID BlobResId, MIL_INT64  Feature, MIL_INT16*  TargetArrayPtr);
inline void MFTYPE MblobGetResultSafeType(MIL_ID BlobResId, MIL_INT64  Feature, MIL_INT32*  TargetArrayPtr);
inline void MFTYPE MblobGetResultSafeType(MIL_ID BlobResId, MIL_INT64  Feature, MIL_INT64*  TargetArrayPtr);
inline void MFTYPE MblobGetResultSafeType(MIL_ID BlobResId, MIL_INT64  Feature, float*      TargetArrayPtr);
inline void MFTYPE MblobGetResultSafeType(MIL_ID BlobResId, MIL_INT64  Feature, MIL_DOUBLE* TargetArrayPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED                       
inline void MFTYPE MblobGetResultSafeType(MIL_ID BlobResId, MIL_INT64  Feature, MIL_UINT8*  TargetArrayPtr);
inline void MFTYPE MblobGetResultSafeType(MIL_ID BlobResId, MIL_INT64  Feature, MIL_UINT16* TargetArrayPtr);
inline void MFTYPE MblobGetResultSafeType(MIL_ID BlobResId, MIL_INT64  Feature, MIL_UINT32* TargetArrayPtr);
inline void MFTYPE MblobGetResultSafeType(MIL_ID BlobResId, MIL_INT64  Feature, MIL_UINT64* TargetArrayPtr);
#endif

// ----------------------------------------------------------
// MblobGetResultSingle

inline void MFTYPE MblobGetResultSingleUnsafe  (MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  Feature, void*       TargetVarPtr);
inline void MFTYPE MblobGetResultSingleSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  Feature, int         TargetVarPtr);
inline void MFTYPE MblobGetResultSingleSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  Feature, MIL_INT8*   TargetVarPtr);
inline void MFTYPE MblobGetResultSingleSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  Feature, MIL_INT16*  TargetVarPtr);
inline void MFTYPE MblobGetResultSingleSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  Feature, MIL_INT32*  TargetVarPtr);
inline void MFTYPE MblobGetResultSingleSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  Feature, MIL_INT64*  TargetVarPtr);
inline void MFTYPE MblobGetResultSingleSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  Feature, float*      TargetVarPtr);
inline void MFTYPE MblobGetResultSingleSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  Feature, MIL_DOUBLE* TargetVarPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED                                               
inline void MFTYPE MblobGetResultSingleSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  Feature, MIL_UINT8*  TargetVarPtr);
inline void MFTYPE MblobGetResultSingleSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  Feature, MIL_UINT16* TargetVarPtr);
inline void MFTYPE MblobGetResultSingleSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  Feature, MIL_UINT32* TargetVarPtr);
inline void MFTYPE MblobGetResultSingleSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  Feature, MIL_UINT64* TargetVarPtr);
#endif

// ----------------------------------------------------------
// MblobGetRuns

inline void MFTYPE MblobGetRunsUnsafe  (MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  ArrayType, void*       RunXPtr, void*       RunYPtr, void*       RunLengthPtr);
inline void MFTYPE MblobGetRunsSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  ArrayType, int         RunXPtr, int         RunYPtr, int         RunLengthPtr);
inline void MFTYPE MblobGetRunsSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  ArrayType, MIL_INT8*   RunXPtr, MIL_INT8*   RunYPtr, MIL_INT8*   RunLengthPtr);
inline void MFTYPE MblobGetRunsSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  ArrayType, MIL_INT16*  RunXPtr, MIL_INT16*  RunYPtr, MIL_INT16*  RunLengthPtr);
inline void MFTYPE MblobGetRunsSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  ArrayType, MIL_INT32*  RunXPtr, MIL_INT32*  RunYPtr, MIL_INT32*  RunLengthPtr);
inline void MFTYPE MblobGetRunsSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  ArrayType, MIL_INT64*  RunXPtr, MIL_INT64*  RunYPtr, MIL_INT64*  RunLengthPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED                                        
inline void MFTYPE MblobGetRunsSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  ArrayType, MIL_UINT8*  RunXPtr, MIL_UINT8*  RunYPtr, MIL_UINT8*  RunLengthPtr);
inline void MFTYPE MblobGetRunsSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  ArrayType, MIL_UINT16* RunXPtr, MIL_UINT16* RunYPtr, MIL_UINT16* RunLengthPtr);
inline void MFTYPE MblobGetRunsSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  ArrayType, MIL_UINT32* RunXPtr, MIL_UINT32* RunYPtr, MIL_UINT32* RunLengthPtr);
inline void MFTYPE MblobGetRunsSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  ArrayType, MIL_UINT64* RunXPtr, MIL_UINT64* RunYPtr, MIL_UINT64* RunLengthPtr);
#endif

// ----------------------------------------------------------
// MblobInquire

inline MIL_INT MFTYPE MblobInquireUnsafe  (MIL_ID ResOrFeatListBlobId, MIL_INT64  InquireType, void*       UserVarPtr);
inline MIL_INT MFTYPE MblobInquireSafeType(MIL_ID ResOrFeatListBlobId, MIL_INT64  InquireType, int         UserVarPtr);
inline MIL_INT MFTYPE MblobInquireSafeType(MIL_ID ResOrFeatListBlobId, MIL_INT64  InquireType, MIL_INT32*  UserVarPtr);
inline MIL_INT MFTYPE MblobInquireSafeType(MIL_ID ResOrFeatListBlobId, MIL_INT64  InquireType, MIL_INT64*  UserVarPtr);
inline MIL_INT MFTYPE MblobInquireSafeType(MIL_ID ResOrFeatListBlobId, MIL_INT64  InquireType, MIL_DOUBLE* UserVarPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MblobInquireSafeType(MIL_ID ResOrFeatListBlobId, MIL_INT64  InquireType, MIL_UINT32* UserVarPtr);
inline MIL_INT MFTYPE MblobInquireSafeType(MIL_ID ResOrFeatListBlobId, MIL_INT64  InquireType, MIL_UINT64* UserVarPtr);
#endif

// -------------------------------------------------------------------------
// MblobGetResult

inline void MFTYPE MblobGetResultSafeType(MIL_ID BlobResId, MIL_INT64  Feature, int TargetArrayPtr)
   {
   if (TargetArrayPtr)
      SafeTypeError(MIL_TEXT("MblobGetResult"));

   MblobGetResult(BlobResId, Feature, NULL);
   }

inline void MblobGetResultSafeTypeExecute(MIL_ID BlobResId, MIL_INT64  Feature, void* TargetArrayPtr, MIL_INT64  GivenType)
   {
   MIL_INT64  RequiredType = (Feature & M_HLVLDATATYPE_MASK);

   if((RequiredType != M_TYPE_MIL_INT32) && (RequiredType != M_TYPE_MIL_INT64) && (RequiredType != M_TYPE_FLOAT) && 
      (RequiredType != M_TYPE_DOUBLE) && (RequiredType != M_TYPE_CHAR) && (RequiredType != M_TYPE_SHORT)) 
      RequiredType = 0;
   if (RequiredType == 0)
      RequiredType = M_TYPE_DOUBLE;

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MblobGetResult"));

   MblobGetResult(BlobResId, Feature, TargetArrayPtr);
   }


inline void MFTYPE MblobGetResultUnsafe  (MIL_ID BlobResId, MIL_INT64  Feature, void*       TargetArrayPtr) {MblobGetResult               (BlobResId, Feature, TargetArrayPtr                  );}
inline void MFTYPE MblobGetResultSafeType(MIL_ID BlobResId, MIL_INT64  Feature, MIL_INT8*   TargetArrayPtr) {MblobGetResultSafeTypeExecute(BlobResId, Feature, TargetArrayPtr, M_TYPE_CHAR     );}
inline void MFTYPE MblobGetResultSafeType(MIL_ID BlobResId, MIL_INT64  Feature, MIL_INT16*  TargetArrayPtr) {MblobGetResultSafeTypeExecute(BlobResId, Feature, TargetArrayPtr, M_TYPE_SHORT    );}
inline void MFTYPE MblobGetResultSafeType(MIL_ID BlobResId, MIL_INT64  Feature, MIL_INT32*  TargetArrayPtr) {MblobGetResultSafeTypeExecute(BlobResId, Feature, TargetArrayPtr, M_TYPE_MIL_INT32);}
inline void MFTYPE MblobGetResultSafeType(MIL_ID BlobResId, MIL_INT64  Feature, MIL_INT64*  TargetArrayPtr) {MblobGetResultSafeTypeExecute(BlobResId, Feature, TargetArrayPtr, M_TYPE_MIL_INT64);}
inline void MFTYPE MblobGetResultSafeType(MIL_ID BlobResId, MIL_INT64  Feature, float*      TargetArrayPtr) {MblobGetResultSafeTypeExecute(BlobResId, Feature, TargetArrayPtr, M_TYPE_FLOAT    );}
inline void MFTYPE MblobGetResultSafeType(MIL_ID BlobResId, MIL_INT64  Feature, MIL_DOUBLE* TargetArrayPtr) {MblobGetResultSafeTypeExecute(BlobResId, Feature, TargetArrayPtr, M_TYPE_DOUBLE   );}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED                       
inline void MFTYPE MblobGetResultSafeType(MIL_ID BlobResId, MIL_INT64  Feature, MIL_UINT8*  TargetArrayPtr) {MblobGetResultSafeTypeExecute(BlobResId, Feature, TargetArrayPtr, M_TYPE_CHAR     );}
inline void MFTYPE MblobGetResultSafeType(MIL_ID BlobResId, MIL_INT64  Feature, MIL_UINT16* TargetArrayPtr) {MblobGetResultSafeTypeExecute(BlobResId, Feature, TargetArrayPtr, M_TYPE_SHORT    );}
inline void MFTYPE MblobGetResultSafeType(MIL_ID BlobResId, MIL_INT64  Feature, MIL_UINT32* TargetArrayPtr) {MblobGetResultSafeTypeExecute(BlobResId, Feature, TargetArrayPtr, M_TYPE_MIL_INT32);}
inline void MFTYPE MblobGetResultSafeType(MIL_ID BlobResId, MIL_INT64  Feature, MIL_UINT64* TargetArrayPtr) {MblobGetResultSafeTypeExecute(BlobResId, Feature, TargetArrayPtr, M_TYPE_MIL_INT64);}
#endif

// -------------------------------------------------------------------------
// MblobGetResultSingle

inline void MFTYPE MblobGetResultSingleSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  Feature, int TargetVarPtr)
   {
   if (TargetVarPtr)
      SafeTypeError(MIL_TEXT("MblobGetResultSingle"));

   MblobGetResultSingle(BlobResId, LabelVal, Feature, NULL);
   }

inline void MblobGetResultSingleSafeTypeExecute(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  Feature, void* TargetVarPtr, MIL_INT64  GivenType)
   {
   MIL_INT64  RequiredType = (Feature & M_HLVLDATATYPE_MASK);

   if((RequiredType != M_TYPE_MIL_INT32) && (RequiredType != M_TYPE_MIL_INT64) && (RequiredType != M_TYPE_FLOAT) && 
      (RequiredType != M_TYPE_DOUBLE) && (RequiredType != M_TYPE_CHAR) && (RequiredType != M_TYPE_SHORT)) 
      RequiredType = 0;
   if (RequiredType == 0)
      RequiredType = M_TYPE_DOUBLE;

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MblobGetResultSingle"));

   MblobGetResultSingle(BlobResId, LabelVal, Feature, TargetVarPtr);
   }

inline void MFTYPE MblobGetResultSingleUnsafe  (MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  Feature, void*       TargetVarPtr) {MblobGetResultSingle               (BlobResId, LabelVal, Feature, TargetVarPtr                  );}
inline void MFTYPE MblobGetResultSingleSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  Feature, MIL_INT8*   TargetVarPtr) {MblobGetResultSingleSafeTypeExecute(BlobResId, LabelVal, Feature, TargetVarPtr, M_TYPE_CHAR     );}
inline void MFTYPE MblobGetResultSingleSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  Feature, MIL_INT16*  TargetVarPtr) {MblobGetResultSingleSafeTypeExecute(BlobResId, LabelVal, Feature, TargetVarPtr, M_TYPE_SHORT    );}
inline void MFTYPE MblobGetResultSingleSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  Feature, MIL_INT32*  TargetVarPtr) {MblobGetResultSingleSafeTypeExecute(BlobResId, LabelVal, Feature, TargetVarPtr, M_TYPE_MIL_INT32);}
inline void MFTYPE MblobGetResultSingleSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  Feature, MIL_INT64*  TargetVarPtr) {MblobGetResultSingleSafeTypeExecute(BlobResId, LabelVal, Feature, TargetVarPtr, M_TYPE_MIL_INT64);}
inline void MFTYPE MblobGetResultSingleSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  Feature, float*      TargetVarPtr) {MblobGetResultSingleSafeTypeExecute(BlobResId, LabelVal, Feature, TargetVarPtr, M_TYPE_FLOAT    );}
inline void MFTYPE MblobGetResultSingleSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  Feature, MIL_DOUBLE* TargetVarPtr) {MblobGetResultSingleSafeTypeExecute(BlobResId, LabelVal, Feature, TargetVarPtr, M_TYPE_DOUBLE   );}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED                                               
inline void MFTYPE MblobGetResultSingleSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  Feature, MIL_UINT8*  TargetVarPtr) {MblobGetResultSingleSafeTypeExecute(BlobResId, LabelVal, Feature, TargetVarPtr, M_TYPE_CHAR     );}
inline void MFTYPE MblobGetResultSingleSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  Feature, MIL_UINT16* TargetVarPtr) {MblobGetResultSingleSafeTypeExecute(BlobResId, LabelVal, Feature, TargetVarPtr, M_TYPE_SHORT    );}
inline void MFTYPE MblobGetResultSingleSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  Feature, MIL_UINT32* TargetVarPtr) {MblobGetResultSingleSafeTypeExecute(BlobResId, LabelVal, Feature, TargetVarPtr, M_TYPE_MIL_INT32);}
inline void MFTYPE MblobGetResultSingleSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  Feature, MIL_UINT64* TargetVarPtr) {MblobGetResultSingleSafeTypeExecute(BlobResId, LabelVal, Feature, TargetVarPtr, M_TYPE_MIL_INT64);}
#endif

// ----------------------------------------------------------
// MblobGetRuns

inline void MFTYPE MblobGetRunsSafeTypeExecute(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  ArrayType, void* RunXPtr, void* RunYPtr, void* RunLengthPtr, MIL_INT64  ExpectedType)
   {
   MIL_INT64  RequiredType = (ArrayType & M_HLVLDATATYPE_MASK);

   if((RequiredType != M_TYPE_MIL_INT32) && (RequiredType != M_TYPE_MIL_INT64) && (RequiredType != M_TYPE_FLOAT) && 
      (RequiredType != M_TYPE_DOUBLE) && (RequiredType != M_TYPE_CHAR) && (RequiredType != M_TYPE_SHORT)) 
      RequiredType = 0;
   if (RequiredType == 0)
      RequiredType = M_TYPE_DOUBLE;

   if (RequiredType != ExpectedType)
      SafeTypeError(MIL_TEXT("MblobGetRuns"));

   MblobGetRuns(BlobResId, LabelVal, ArrayType, RunXPtr, RunYPtr, RunLengthPtr);
   }
inline void MFTYPE MblobGetRunsSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  ArrayType, int RunXPtr, int RunYPtr, int RunLengthPtr)
   {
   if (RunXPtr || RunYPtr || RunLengthPtr)
      SafeTypeError(MIL_TEXT("MblobGetRuns"));

   MblobGetRuns(BlobResId, LabelVal, ArrayType, NULL, NULL, NULL);
   }

inline void MFTYPE MblobGetRunsUnsafe  (MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  ArrayType, void*       RunXPtr, void*       RunYPtr, void*       RunLengthPtr) {MblobGetRuns               (BlobResId, LabelVal, ArrayType, RunXPtr, RunYPtr, RunLengthPtr                  );}
inline void MFTYPE MblobGetRunsSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  ArrayType, MIL_INT8*   RunXPtr, MIL_INT8*   RunYPtr, MIL_INT8*   RunLengthPtr) {MblobGetRunsSafeTypeExecute(BlobResId, LabelVal, ArrayType, RunXPtr, RunYPtr, RunLengthPtr, M_TYPE_CHAR     );}
inline void MFTYPE MblobGetRunsSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  ArrayType, MIL_INT16*  RunXPtr, MIL_INT16*  RunYPtr, MIL_INT16*  RunLengthPtr) {MblobGetRunsSafeTypeExecute(BlobResId, LabelVal, ArrayType, RunXPtr, RunYPtr, RunLengthPtr, M_TYPE_SHORT    );}
inline void MFTYPE MblobGetRunsSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  ArrayType, MIL_INT32*  RunXPtr, MIL_INT32*  RunYPtr, MIL_INT32*  RunLengthPtr) {MblobGetRunsSafeTypeExecute(BlobResId, LabelVal, ArrayType, RunXPtr, RunYPtr, RunLengthPtr, M_TYPE_MIL_INT32);}
inline void MFTYPE MblobGetRunsSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  ArrayType, MIL_INT64*  RunXPtr, MIL_INT64*  RunYPtr, MIL_INT64*  RunLengthPtr) {MblobGetRunsSafeTypeExecute(BlobResId, LabelVal, ArrayType, RunXPtr, RunYPtr, RunLengthPtr, M_TYPE_MIL_INT64);}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED                                        
inline void MFTYPE MblobGetRunsSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  ArrayType, MIL_UINT8*  RunXPtr, MIL_UINT8*  RunYPtr, MIL_UINT8*  RunLengthPtr) {MblobGetRunsSafeTypeExecute(BlobResId, LabelVal, ArrayType, RunXPtr, RunYPtr, RunLengthPtr, M_TYPE_CHAR     );}
inline void MFTYPE MblobGetRunsSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  ArrayType, MIL_UINT16* RunXPtr, MIL_UINT16* RunYPtr, MIL_UINT16* RunLengthPtr) {MblobGetRunsSafeTypeExecute(BlobResId, LabelVal, ArrayType, RunXPtr, RunYPtr, RunLengthPtr, M_TYPE_SHORT    );}
inline void MFTYPE MblobGetRunsSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  ArrayType, MIL_UINT32* RunXPtr, MIL_UINT32* RunYPtr, MIL_UINT32* RunLengthPtr) {MblobGetRunsSafeTypeExecute(BlobResId, LabelVal, ArrayType, RunXPtr, RunYPtr, RunLengthPtr, M_TYPE_MIL_INT32);}
inline void MFTYPE MblobGetRunsSafeType(MIL_ID BlobResId, MIL_INT LabelVal, MIL_INT64  ArrayType, MIL_UINT64* RunXPtr, MIL_UINT64* RunYPtr, MIL_UINT64* RunLengthPtr) {MblobGetRunsSafeTypeExecute(BlobResId, LabelVal, ArrayType, RunXPtr, RunYPtr, RunLengthPtr, M_TYPE_MIL_INT64);}
#endif

// ----------------------------------------------------------
// MblobInquire

inline MIL_INT MFTYPE MblobInquireSafeType (MIL_ID ResOrFeatListBlobId, MIL_INT64  InquireType, int UserVarPtr)
   {
   if (UserVarPtr)
      SafeTypeError(MIL_TEXT("MblobInquire"));

   return MblobInquire(ResOrFeatListBlobId, InquireType, NULL);
   }


inline MIL_INT MFTYPE MblobInquireSafeTypeExecute (MIL_ID ResOrFeatListBlobId, MIL_INT64  InquireType, void* UserVarPtr, MIL_INT64  GivenType)
   {
   MIL_INT64  RequiredType = (InquireType & M_HLVLDATATYPE_MASK);

   if((RequiredType != M_TYPE_MIL_INT32) && (RequiredType != M_TYPE_MIL_INT64) && (RequiredType != M_TYPE_MIL_ID) && 
      (RequiredType != M_TYPE_DOUBLE) ) 
      RequiredType = 0;
   if (RequiredType == 0)
      RequiredType = M_TYPE_DOUBLE;
   ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MblobInquire"));

   return MblobInquire(ResOrFeatListBlobId, InquireType, UserVarPtr);
   }

inline MIL_INT MFTYPE MblobInquireUnsafe  (MIL_ID ResOrFeatListBlobId, MIL_INT64  InquireType, void*       UserVarPtr) {return MblobInquire               (ResOrFeatListBlobId, InquireType, UserVarPtr                  );}
inline MIL_INT MFTYPE MblobInquireSafeType(MIL_ID ResOrFeatListBlobId, MIL_INT64  InquireType, MIL_INT32*  UserVarPtr) {return MblobInquireSafeTypeExecute(ResOrFeatListBlobId, InquireType, UserVarPtr, M_TYPE_MIL_INT32);}
inline MIL_INT MFTYPE MblobInquireSafeType(MIL_ID ResOrFeatListBlobId, MIL_INT64  InquireType, MIL_INT64*  UserVarPtr) {return MblobInquireSafeTypeExecute(ResOrFeatListBlobId, InquireType, UserVarPtr, M_TYPE_MIL_INT64);}
inline MIL_INT MFTYPE MblobInquireSafeType(MIL_ID ResOrFeatListBlobId, MIL_INT64  InquireType, MIL_DOUBLE* UserVarPtr) {return MblobInquireSafeTypeExecute(ResOrFeatListBlobId, InquireType, UserVarPtr, M_TYPE_DOUBLE   );}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MblobInquireSafeType(MIL_ID ResOrFeatListBlobId, MIL_INT64  InquireType, MIL_UINT32* UserVarPtr) {return MblobInquireSafeTypeExecute(ResOrFeatListBlobId, InquireType, UserVarPtr, M_TYPE_MIL_INT32);}
inline MIL_INT MFTYPE MblobInquireSafeType(MIL_ID ResOrFeatListBlobId, MIL_INT64  InquireType, MIL_UINT64* UserVarPtr) {return MblobInquireSafeTypeExecute(ResOrFeatListBlobId, InquireType, UserVarPtr, M_TYPE_MIL_INT64);}
#endif

#define MblobGetResult       MblobGetResultSafeType
#define MblobGetResultSingle MblobGetResultSingleSafeType
#define MblobGetRuns         MblobGetRunsSafeType
#define MblobInquire         MblobInquireSafeType

#else // #if M_MIL_USE_SAFE_TYPE

#define MblobGetResultUnsafe       MblobGetResult
#define MblobGetResultSingleUnsafe MblobGetResultSingle
#define MblobGetRunsUnsafe         MblobGetRuns
#define MblobInquireUnsafe         MblobInquire

#endif // #if M_MIL_USE_SAFE_TYPE

#endif // !M_MIL_LITE

#endif /* __MILBLOB_H__ */
