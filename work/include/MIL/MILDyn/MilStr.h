/***************************************************************************/
/*

    Filename:  MilStr.h
    Owner   :  Matrox Imaging dept.
    Revision:  10.00.2564
    Content :  This file contains the defines and the prototypes for the
               MIL String Reader module. (Mstr...).

    Copyright © Matrox Electronic Systems Ltd., 1992-2014.
    All Rights Reserved

*/
/***************************************************************************/

#ifndef __MILSTR_H__
#define __MILSTR_H__

#if (!M_MIL_LITE) // MIL FULL ONLY

/* C++ directive if needed */
#ifdef __cplusplus
extern "C"
{
#endif


/***************************************************************************/
/*                      MilStr CAPI defines                                */
/***************************************************************************/

/***************************************************************************/
/* Index values                                                            */
/***************************************************************************/
#define M_FONT_MASK                             0x00800000L
#define M_STRING_MASK                           0x00400000L
#define M_REAL_INDEX_MASK                       0x000000FFL

#define M_DEFAULT                               0x10000000L // Also defined in mil.h
#define M_CONTEXT                               0x08000000L
#define M_ALL                                   0x40000000L // Also defined in Milmod.h
#define M_GENERAL                               0x20000000L // Also defined in Milmod.h

#define M_FONT_INDEX(i)                 (M_FONT_MASK | (i))
#define M_STRING_INDEX(i)             (M_STRING_MASK | (i))
#define M_CHAR_LABEL(i)                 (M_FONT_MASK | (i))

/***************************************************************************/
/* MstrAlloc()                                                             */
/***************************************************************************/

/* Object types */
#define M_FEATURE_BASED                         0x00002000L //Obsolete
#define M_FONT_BASED                            0x00002000L
#define M_FONTLESS                              0x00004000L


/***************************************************************************/
/* MstrControl() and MstrInquire()                                         */
/***************************************************************************/

/* ControlType and InquireType for context */
#define M_TRAINING_ERROR                                 2L
#define M_STRING_ADD                                   700L
#define M_STRING_DELETE                                701L
#define M_FONT_ADD                                     702L
#define M_FONT_DELETE                                  703L

#define M_STRING_NUMBER                                 74L // Also defined in milocr.h
#define M_SPEED					                           8L // Also defined in milpat.h
#define M_MINIMUM_CONTRAST                             600L
#define M_ENCODING                              0x00008005L // Also defined in milcode.h
#define M_TIMEOUT                                     2077L // already define in mil.h
#define M_STOP_READ                                    610L 
#define M_STRING_SEPARATOR                             910L 
#define M_SPACE_CHARACTER                              911L
#define M_SEARCH_SKEW_ANGLE                            950L
#define M_SEARCH_CHAR_ANGLE                            951L
#define M_SEARCH_STRING_ANGLE                          952L
#define M_THRESHOLD_MODE                                33L // Already define in miledge.h
#define M_THRESHOLD_VALUE                              953L
#define M_LOCAL_WITH_RESEGMENTATION                    954L
#define M_LOCAL                                  0x01000000 // Already define in mil.h, milmeas.h
#define M_THICKEN_CHAR                                  84L // Already define in milocr.h

#define M_NUMBER_OF_STRING_MODELS                      900L
#define M_NUMBER_OF_FONTS                              901L
#define M_OWNER_SYSTEM                                1101L // Also defined in mil.h
#define M_PREPROCESSED                                  14L // Also defined in MilPat.h
#define M_MODIFICATION_COUNT                          5010L // Also defined in mil.h
#define M_CONTEXT_TYPE                        M_OBJECT_TYPE  
#define M_SCORE_TYPE                                    37L // Already defined in milpat.h

#define M_DRAW_LAST_SIZE_X                             905L
#define M_DRAW_LAST_SIZE_Y                             906L

#define M_MEMORY_LIMIT                                 908L

#define M_NUMBER_OF_NODES_MAX                          955L
#define M_NUMBER_OF_LEARNERS_MAX                       956L
#define M_NUMBER_OF_LEARNERS                           957L
#define M_MATRIX_OF_CONFUSION_SIZE                     958L
#define M_MATRIX_OF_CONFUSION                          959L
#define M_TRAIN_SCORE_MIN                              960L
#define M_TRAIN_SCORE_MAX                              961L
#define M_TRAIN_SCORE_MEAN                             962L
#define M_TRAIN_SCORE_VARIANCE                         963L
#define M_TRAIN_SCORE_MEDIAN                           964L
#define M_CONTRAST_HYSTERESIS                          965L
#define M_RESEGMENTATION_METHOD                        966L
#define M_TRAIN_FROM_DISK                              967L
#define M_ENABLE_CHAR                                  968L
#define M_DISABLE_CHAR                                 969L
#define M_CHAR_STATUS                                  970L

#define M_ALLOCATION_FAILURE_REAL_TEST_FLAG            971L // Also in milblob.h

#define M_MATRIX_OF_COST_SIZE                          972L
#define M_MATRIX_OF_COST                               973L
#define M_IMPORT_PARAMETERS                            974L
#define M_IMPORT_TRAINING                              975L

#define M_PRESEARCH_LONG_STRING                        976L
#define M_AGGRESSIVE_FILTERING                         977L

//Operation for context
#define M_DRAW_CHAR                                    907L

#define M_STOP_EXPERT                                  611L
#define M_STOP_TRAINING                                612L // Also defined in    milclas.h

/*String Type pass to MstrControll() with M_STRING_ADD*/
#define M_USER_DEFINED                                  21L // Also defined in Mil.h
#define M_UNIVERSAL                              0x00001000

/* ControlType and InquireType for string model and font */
#define M_STRING_USER_LABEL                       228L   // -
#define M_STRING_MODEL_USER_LABEL  M_STRING_USER_LABEL   // This is a result only
#define M_FONT_USER_LABEL                         229L   // -
#define M_CHAR_FONT_USER_LABEL       M_FONT_USER_LABEL   // This is a result only

/* Inquire only                                          */
#define M_STRING_INDEX_FROM_LABEL            230L
#define M_FONT_INDEX_FROM_LABEL              231L

/* ControlType and InquireType for string model */
#define M_STRING_NUMBER                                 74L // Also defined in milocr.h
#define M_STRING_SIZE_MIN                              500L
#define M_STRING_SIZE_MAX                               62L // Also defined in milocr.h
#define M_STRING_ANGLE                                  87L // Also defined in milocr.h
#define M_STRING_ANGLE_DELTA_NEG                       106L // Also defined in milocr.h
#define M_STRING_ANGLE_DELTA_POS                       107L // Also defined in milocr.h
#define M_STRING_SCALE                                 502L
#define M_STRING_SCALE_MIN_FACTOR                      503L
#define M_STRING_SCALE_MAX_FACTOR                      504L
#define M_STRING_ASPECT_RATIO                          505L
#define M_STRING_ASPECT_RATIO_MIN_FACTOR               506L
#define M_STRING_ASPECT_RATIO_MAX_FACTOR               507L
#define M_FOREGROUND_VALUE                               4L // Also defined in milocr.h
#define M_CHAR_ACCEPTANCE                               25L // Also defined in milocr.h
#define M_CHAR_SIMILARITY_ACCEPTANCE                    26L
#define M_CHAR_HOMOGENEITY_ACCEPTANCE                   27L
#define M_STRING_ACCEPTANCE                             24L // Also defined in milocr.h
#define M_STRING_CERTAINTY                             526L
#define M_STRING_TARGET_ACCEPTANCE                     527L
#define M_STRING_TARGET_CERTAINTY                      528L
#define M_CHAR_SCALE_MIN_FACTOR                        509L
#define M_CHAR_SCALE_MAX_FACTOR                        510L
#define M_CHAR_ASPECT_RATIO_MIN_FACTOR                 511L
#define M_CHAR_ASPECT_RATIO_MAX_FACTOR                 512L
#define M_CHAR_MAX_BASELINE_DEVIATION                  513L
#define M_SPACE_MAX_CONSECUTIVE                        514L
#define M_STRING_TYPE                                  515L
#define M_NUMBER_OF_CONSTRAINTS                        516L

#define M_CONSTRAINT_POSITION                          520L
#define M_CONSTRAINT_TYPE                       0x00008000L // Also defined in milocr.h
#define M_CONSTRAINT_FONT                              524L
#define M_CONSTRAINT_ALLOC_SIZE                        521L
#if (M_MIL_USE_UNICODE && !M_MIL_UNICODE_API && !M_COMPILING_MILDLL)
#define M_CONSTRAINT                           (0x04000000L | M_CLIENT_ASCII_MODE) // Also defined in milocr.h
#else
#define M_CONSTRAINT                            0x04000000L // Also defined in milocr.h
#endif
#define M_DEFAULT_CONSTRAINT_ALLOC_SIZE                522L
#define M_DEFAULT_CONSTRAINT_FONT                      525L
#define M_DEFAULT_CONSTRAINT_TYPE                      523L
#define M_DEFAULT_CONSTRAINT                    0x08000000L

#define M_CHECK_DIGIT                           0x0000000EL //Already defined in milcode.h

/* Mask for constraint index */
#define M_CONSTRAINT_INDEX_MASK                 0x000000FFL



/* ControlType and InquireType for font */
#define M_FONT_TYPE                                     31L // Also defined in milocr.h
#define M_SPACE_WIDTH                                  800L
#define M_SPACE_SIZE                          M_SPACE_WIDTH
#define M_DRAW_BOX_MARGIN_X                            801L
#define M_DRAW_BOX_MARGIN_Y                            802L
#define M_CHAR_M_WIDTH                                 803L  
#define M_CHAR_M_HEIGHT                                804L
#define M_CHAR_M_THICKNESS                             821L
#define M_SPACE_WIDTH_VALUE                            805L

#define M_NUMBER_OF_CHARS                              814L
#define M_CHAR_VALUE                                   815L
#define M_CHAR_TYPE                                    816L
#define M_CHAR_BASELINE                                  4L
#define M_CHAR_SIZE_X                                   37L // Also defined in milocr.h
#define M_CHAR_SIZE_Y                                   38L // Also defined in milocr.h
#define M_CHAR_THICKNESS                                39L // Also defined in milocr.h

#define M_DEFINITION_OFFSET_X                          817L
#define M_DEFINITION_OFFSET_Y                          818L

#define M_REF_CHAR_SIZE_Y                              827L
#define M_MIN_CHAR_SIZE_X                              822L
#define M_MAX_CHAR_SIZE_X                              823L
#define M_MIN_CHAR_SIZE_Y                              824L
#define M_MAX_CHAR_SIZE_Y                              825L
#define M_REF_CHAR_THICKNESS                           826L

/* Inquire type for report string (MstrExpert) */
#define M_REPORT_STRING                                819L

/* Inquire only for font */
#define M_CHAR_BASELINE_VALUE                          5L
#define M_CHAR_TYPE_VALUE                              820L

/* InquireType modifiers */
#define M_DEFAULT                               0x10000000L // Also defined in mil.h
#define M_SUPPORTED                             MAKE_INT64(0x0000800000000000) // Also defined in mil.h
// Data types are no longer duplicated to avoid the copy of all the conditions related to MIL_INT types (x64)

// Controls/inquires for results
#define M_RESULT_OUTPUT_UNITS          1300L // Also in milim.h

/* ControlValue  */
#define M_DEFAULT                               0x10000000L // Also defined in mil.h
#define M_ORIGINAL                              0x00000199L // Already define in MilPat.h
#define M_ALL                                   0x40000000L // Also defined in Milmod.h
#define M_DISABLE                                    -9999L // Also defined in mil.h
#define M_ENABLE                                     -9997L // Also defined in mil.h

#define M_USER_DEFINED                                  21L // Also defined in Mil.h

#define M_FOREGROUND_WHITE                            0x80L // Also defined in MilBlob.h 
#define M_FOREGROUND_BLACK                           0x100L // Also defined in MilBlob.h 
#define M_FOREGROUND_BLACK_OR_WHITE                  0x200L
#define M_FOREGROUND_ANY        M_FOREGROUND_WHITE+M_FOREGROUND_BLACK // Also defined in milmod.h


#define M_LOW                                            1L // Also defined in Milocr.h
#define M_MEDIUM                                         2L // Also defined in Milocr.h
#define M_HIGH                                           3L // Also defined in Milocr.h

#define M_ASCII                                          1L
#define M_UNICODE                                        2L

#define M_NONE                                  0x08000000L // Also defined in milcal.h, milcolor.h
#define M_AUTO_COMPUTE                          0x08000001L

#define M_MIN_CHAR_WIDTH                        0x01000000L
#define M_MEAN_CHAR_WIDTH                       0x02000000L
#define M_MAX_CHAR_WIDTH                        0x03000000L
#define M_QUARTER_MAX_CHAR_WIDTH                0x04000000L
#define M_INFINITE                              -1L // Also defined in Mil.h

// Score Type...
#define M_GREYSCALE                                 0x0200L // Already defined in mil.h
#define M_GRAYSCALE                             M_GREYSCALE // Already defined in mil.h
#define M_FEATURE_BASED                         0x00002000L

// To remove user label
#define M_NO_LABEL                              0x20000000L   // Also defined in milreg.h

// Supported check digit type
#define M_ISO6346_CONTAINER                              1L

// M_STRING_ANGLE value
#define M_ACCORDING_TO_REGION                   0x40000000L  // Also defined in mil.h

/***************************************************************************/
/* MstrGetResult()                                                         */
/***************************************************************************/

#define M_STRING_NUMBER                                 74L // Also defined in milocr.h
#define M_CHAR_NUMBER                                   32L // Also defined in milocr.h
#define M_TIMEOUT_END                                   10L // Also defined in milmod.h
#define M_TEXT_SIZE                                     17L 
#define M_TEXT_ALLOC_SIZE_BYTE                          18L 
#define M_CONTEXT_ID                            0x00001010L // Also defined in milmod.h
#if (M_MIL_USE_UNICODE && !M_MIL_UNICODE_API && !M_COMPILING_MILDLL)
#define M_TEXT                                       (16L | M_CLIENT_ASCII_MODE) // Also defined in milocr.h
#else
#define M_TEXT                                          16L
#endif  
#define M_STRING_SIZE        MAKE_INT64(0x0000050000000000) // Also defined in milocr.h
#define M_STRING_LENGTH      M_STRING_SIZE                  // Also defined in milocr.h (must be bitwise - 2 bit)
#define M_STRING_ALLOC_SIZE_BYTE                       591L // Also defined in milocr.h
#define M_FORMATTED_STRING_ALLOC_SIZE_BYTE             592L 
#if (M_MIL_USE_UNICODE && !M_MIL_UNICODE_API && !M_COMPILING_MILDLL)
#define M_STRING                                      (3L | M_CLIENT_ASCII_MODE)   // Also defined in milocr.h
#else
#define M_STRING                                         3L // Also defined in milocr.h
#endif
 // Also defined in milocr.h
#define M_STRING_MODEL_INDEX                           585L
#define M_STRING_ANGLE                                  87L // Also defined in milocr.h
#define M_CHAR_ANGLE                                   794L
#define M_STRING_CHAR_ANGLE                    M_CHAR_ANGLE
#define M_SKEW_ANGLE                                   795L
#define M_STRING_SCALE                                 502L
#define M_STRING_ASPECT_RATIO                          505L
#define M_STRING_POSITION_X                            586L
#define M_STRING_POSITION_Y                            587L
#define M_STRING_SCORE                          0x00001400L // Also defined in milocr.h
#define M_STRING_TARGET_SCORE                          900L 
#define M_STRING_CHAR_SCORE_MIN                        588L
#define M_STRING_CHAR_SCORE_MAX                        589L
#define M_STRING_FOREGROUND_VALUE                      508L
#define M_FORMATTED_STRING_SIZE                        792L
#define M_FORMATTED_STRING                             793L
#define M_MEMORY_LIMIT_REACHED                         913L

#define M_STRING_BOX_UL_X                              914L
#define M_STRING_BOX_UL_Y                              915L
#define M_STRING_BOX_UR_X                              916L
#define M_STRING_BOX_UR_Y                              917L
#define M_STRING_BOX_BR_X                              918L
#define M_STRING_BOX_BR_Y                              919L
#define M_STRING_BOX_BL_X                              920L
#define M_STRING_BOX_BL_Y                              921L

#define M_CHAR_VALUE                                   815L
#define M_CHAR_FONT                                    785L
#define M_CHAR_INDEX                                   790L
#define M_CHAR_SCORE                                     5L // Also defined in milocr.h
#define M_CHAR_HOMOGENEITY_SCORE                       788L
#define M_CHAR_SIMILARITY_SCORE                        789L
#define M_CHAR_SCALE                                   786L
#define M_CHAR_ASPECT_RATIO                            787L
#define M_CHAR_POSITION_X                                6L // Also defined in milocr.h
#define M_CHAR_POSITION_Y                                7L // Also defined in milocr.h
#define M_CHAR_CONSECUTIVE_SPACE                       791L
#define M_CHAR_BASELINE_DEVIATION                      912L

#define M_CHAR_BOX_UL_X                              922L
#define M_CHAR_BOX_UL_Y                              923L
#define M_CHAR_BOX_UR_X                              924L
#define M_CHAR_BOX_UR_Y                              925L
#define M_CHAR_BOX_BR_X                              926L
#define M_CHAR_BOX_BR_Y                              927L
#define M_CHAR_BOX_BL_X                              928L
#define M_CHAR_BOX_BL_Y                              929L


#define M_REPORT_NUMBER_OF_ERRORS                      796L
#define M_REPORT_NUMBER_OF_WARNINGS                    797L
#define M_REPORT_NUMBER_OF_OPTIMIZATIONS               798L
#define M_REPORT_ERRORS                                799L
#define M_REPORT_WARNINGS                              800L
#define M_REPORT_OPTIMIZATIONS                         801L

#define M_A_FORWARD                             0x00004001L // Also defined in milmod.h
#define M_B_FORWARD                             0x00004002L // Also defined in milmod.h
#define M_C_FORWARD                             0x00004003L // Also defined in milmod.h
#define M_D_FORWARD                             0x00004004L // Also defined in milmod.h
#define M_E_FORWARD                             0x00004005L // Also defined in milmod.h
#define M_F_FORWARD                             0x00004006L // Also defined in milmod.h
#define M_A_REVERSE                             0x00004011L // Also defined in milmod.h
#define M_B_REVERSE                             0x00004012L // Also defined in milmod.h
#define M_C_REVERSE                             0x00004013L // Also defined in milmod.h
#define M_D_REVERSE                             0x00004014L // Also defined in milmod.h
#define M_E_REVERSE                             0x00004015L // Also defined in milmod.h
#define M_F_REVERSE                             0x00004016L // Also defined in milmod.h


#define M_TARGET_STRING_INDEX                   0x04000000L
#define M_IMAGE_INDEX                           0x08000000L

/* ResultType modifiers */
#define M_DEFAULT                               0x10000000L // Also defined in mil.h
#define M_AVAILABLE                             MAKE_INT64(0x0000400000000000) // Also defined in mil.h
#define M_SUPPORTED                             MAKE_INT64(0x0000800000000000) // Also defined in mil.h

// Data types are no longer duplicated to avoid the copy of all the conditions related to MIL_INT types (x64)

/***************************************************************************/
/* MstrDraw()                                                              */
/***************************************************************************/

/* Operation */
#define M_DRAW_STRING                           0x00000001L
#define M_DRAW_STRING_CONTOUR                   0x00000002L
#define M_DRAW_STRING_BOX                       0x00000004L
#define M_DRAW_STRING_CHAR_BOX                  0x00000008L
#define M_DRAW_STRING_CHAR_POSITION             0x00000010L
#define M_DRAW_MIL_FONT_STRING                  0x00000020L

/***************************************************************************/
/* MstrEditFont()                                                          */
/***************************************************************************/

/* Operation */
#define M_CHAR_ADD                                       1L
#define M_CHAR_DELETE                                    2L
#define M_CHAR_NORMALIZE                                 3L
#define M_CHAR_BASELINE                                  4L
#define M_CHAR_SORT                                      5L
#define M_CHAR_TYPE                                    816L

/* Operation mode */
#define M_DEFAULT                               0x10000000L // Also defined in mil.h
#define M_USER_DEFINED                                  21L // Also defined in Mil.h
#if (M_MIL_USE_UNICODE && !M_MIL_UNICODE_API && !M_COMPILING_MILDLL)
#define M_SYSTEM_FONT                                   (12L | M_CLIENT_ASCII_MODE)
#else
#define M_SYSTEM_FONT                                   12L 
#endif
#define M_SEMI_M13_88                                  0x2L // Also defined in milocr.h
#define M_SEMI_M12_92                                  0x1L // Also defined in milocr.h
#define M_OCR_A                                         10L
#define M_OCR_B                                         11L
#define M_ASCENDING                                      1L
#define M_DESCENDING                                     2L

/* Operation mode modifiers */
#define M_SINGLE                                0x01000000L // Also defined in Mil.h

#define M_FOREGROUND_WHITE                            0x80L // Also defined in MilBlob.h 
#define M_FOREGROUND_BLACK                           0x100L // Also defined in MilBlob.h 

#define M_NO_OVERWRITE                               0x200L

/* Operation parameters 1 */
#define M_NONE                                  0x08000000L
#define M_REGULAR                               0x00020000L // Also defined in Mil.h
#define M_PUNCTUATION                                    1L



/***************************************************************************/
/* MstrSetConstraint()                                                     */
/***************************************************************************/

/* Constraint Type */
#define M_DEFAULT                               0x10000000L // Also defined in mil.h
#define M_ANY                                   0x11000000L // Also defined in Mil.h
#define M_LETTER                                0x00020000L // Also defined in milocr.h
#define M_DIGIT                                 0x00040000L // Also defined in milocr.h
#define M_UPPERCASE                             0x00010000L // Also defined in milocr.h
#define M_LOWERCASE                             0x00008000L // Also defined in milocr.h
#define M_FONT_MASK                             0x00800000L
#define M_REAL_INDEX_MASK                       0x000000FFL


/***************************************************************************/
/* MstrExpert()                                                            */
/***************************************************************************/

/* Control Flag */
#define M_ERRORS                                0x00000001L
#define M_WARNINGS                                     306L // Also defined in milmod.h
#define M_OPTIMIZATIONS                         0x00000004L
#define M_FIX_ERRORS                            0x20000000L
/***************************************************************************/
/*                 Reports definitions                                     */
/***************************************************************************/
#define M_MAX_REPORT_STRING_LENGTH                     128L

////////////////////////////////////////////////////////////////////////////////////////////////////
// MstrTrain
////////////////////////////////////////////////////////////////////////////////////////////////////
#define M_CONTINUE_TRAINING                               1L
#define M_COMPUTE_CONFUSION_MATRIX                        2L
#define M_TRAIN_META_CLASSES                              3L

////////////////////////////////////////////////////////////////////////////////////////////////////
// MstrHookTrainfunc
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef MIL_INT (MFTYPE *MIL_STR_HOOK_TRAIN_FUNCTION_PTR)(MIL_INT         HookType,
                                                          MIL_TEXT_CHAR   CurChar,
                                                          MIL_DOUBLE      TrainingError,
                                                          MIL_DOUBLE      ValidationError,
                                                          void*           pUserData);


#define M_TRAIN_NEW_CHAR      0L
#define M_TRAINING_START      1L
#define M_TRAINING_END        2L
#define M_TRAINING_ITERATION  3L
#define M_EXTRACTING_FEATURES 4L
#define M_PREPROCESS_START    5L
#define M_PREPROCESS_END      6L

/***************************************************************************/
/*                 MilStr CAPI function prototypes                         */
/***************************************************************************/

MIL_ID MFTYPE MstrAlloc         (MIL_ID      SystemId,
                                 MIL_INT64   ContextType,
                                 MIL_INT64   ControlFlag,
                                 MIL_ID*     ObjectIdPtr);

MIL_ID MFTYPE MstrAllocResult   (MIL_ID      SystemId,
                                 MIL_INT64   ControlFlag,
                                 MIL_ID*     ObjectIdPtr);

void MFTYPE MstrFree            (MIL_ID      ObjectId);

#if M_MIL_USE_64BIT
// Prototypes for 64 bits OSs
void MFTYPE MstrControlInt64    (MIL_ID      ContextOrResultId,
                                 MIL_INT     Index,
                                 MIL_INT64   ControlType,
                                 MIL_INT64   ControlValue);

void MFTYPE MstrControlDouble   (MIL_ID      ContextOrResultId,
                                 MIL_INT     Index,
                                 MIL_INT64   ControlType,
                                 MIL_DOUBLE  ControlValue);
#else
// Prototypes for 32 bits OSs
#define MstrControlInt64  MstrControl
#define MstrControlDouble MstrControl
void MFTYPE MstrControl         (MIL_ID      ContextOrResultId,
                                 MIL_INT     Index,
                                 MIL_INT64   ControlType,
                                 MIL_DOUBLE  ControlValue);

#endif

MIL_INT MFTYPE MstrInquire      (MIL_ID      ContextOrResultId,
                                 MIL_INT     Index,
                                 MIL_INT64   InquireType,
                                 void*       UserVarPtr);

void MFTYPE MstrEditFont        (MIL_ID   ContextId,
                                 MIL_INT     FontIndex,
                                 MIL_INT64   Operation,
                                 MIL_INT64   OperationMode,
                                 MIL_INT     Param1,
                                 const void*    Param2Ptr,
                                 const void*    Param3Ptr);

void MFTYPE MstrSetConstraint   (MIL_ID   ContextId,
                                 MIL_INT     StringIndex,
                                 MIL_INT     CharPos,
                                 MIL_INT64   ConstraintType,
                                 const void*    CharListPtr);

void MFTYPE MstrPreprocess      (MIL_ID      ContextId,
                                 MIL_INT64   ControlFlag);

void MFTYPE MstrRead            (MIL_ID      ContextId,
                                 MIL_ID      TargetImageId,
                                 MIL_ID      ResultId);

void MFTYPE MstrGetResult       (MIL_ID      ResultId,
                                 MIL_INT     Index,
                                 MIL_INT64   ResultType,
                                 void*       ResultArrayPtr);

void MFTYPE MstrDraw            (MIL_ID   ContextGraId,
                                 MIL_ID   ContextOrResultStrId,
                                 MIL_ID   DstImageBufOrListGraId,
                                 MIL_INT64  Operation,
                                 MIL_INT  Index,
                                 const void*    CharListPtr,
                                 MIL_INT64  ControlFlag);

void MFTYPE MstrExpert          (MIL_ID         ContextId,
                                 const MIL_ID*  ImageArrayPtr,
                                 const void*    TargetStringArrayPtr,
                                 MIL_INT        NbImages,
                                 MIL_ID         ResultId,
                                 MIL_INT64      ControlFlag);

MIL_ID MFTYPE MstrTrain(         MIL_ID               TrainingBaseId,
                                 MIL_ID               ValidationBaseId,
                                 MIL_CONST_TEXT_PTR   CharSet,
                                 MIL_ID               SystemId,
                                 MIL_INT64            ControlFlag,
                                 MIL_ID               ContextId);


void MFTYPE MstrHookTrainFunction(MIL_ID                          ContextId,
                                  MIL_STR_HOOK_TRAIN_FUNCTION_PTR HookHandlerPtr,
                                  void*                           UserDataPtr);

MIL_ID MFTYPE MstrGenerateTrainingSet(MIL_ID*    ContextIdArrayPtr,
                                      MIL_INT    ArraySize,
                                      MIL_ID     DatabaseId,
                                      MIL_INT64  ControlFlag);

#if M_MIL_USE_UNICODE

void MFTYPE MstrSaveA            (const char*   FileName,
                                  MIL_ID  ContextId,
                                  MIL_INT64  ControlFlag);

MIL_ID MFTYPE MstrRestoreA       (const char* FileName,
                                  MIL_ID      SystemId,
                                  MIL_INT64   ControlFlag,
                                  MIL_ID*     ContextIdPtr);

void MFTYPE MstrStreamA          (char*         MemPtrOrFileName,
                                  MIL_ID        SystemId,
                                  MIL_INT64     Operation,
                                  MIL_INT64     StreamType,
                                  MIL_DOUBLE    Version,
                                  MIL_INT64     ControlFlag,
                                  MIL_ID*       ObjectIdPtr,
                                  MIL_INT*      SizeByteVarPtr);

void MFTYPE MstrSaveW            (MIL_CONST_TEXT_PTR  FileName,
                                  MIL_ID        ContextId,
                                  MIL_INT64     ControlFlag);

MIL_ID MFTYPE MstrRestoreW       (MIL_CONST_TEXT_PTR  FileName,
                                  MIL_ID        SystemId,
                                  MIL_INT64     ControlFlag,
                                  MIL_ID*       ContextIdPtr);

void MFTYPE MstrStreamW          (MIL_TEXT_PTR  MemPtrOrFileName,
                                  MIL_ID        SystemId,
                                  MIL_INT64     Operation,
                                  MIL_INT64     StreamType,
                                  MIL_DOUBLE    Version,
                                  MIL_INT64     ControlFlag,
                                  MIL_ID*       ObjectIdPtr,
                                  MIL_INT       *SizeByteVarPtr);


#if M_MIL_UNICODE_API

#define MstrSave           MstrSaveW
#define MstrRestore        MstrRestoreW
#define MstrStream         MstrStreamW

#else

#define MstrSave           MstrSaveA
#define MstrRestore        MstrRestoreA
#define MstrStream         MstrStreamA
#endif

#else

void MFTYPE MstrSave            (MIL_CONST_TEXT_PTR  FileName,
                                 MIL_ID        ContextId,
                                 MIL_INT64     ControlFlag);

MIL_ID MFTYPE MstrRestore       (MIL_CONST_TEXT_PTR  FileName,
                                 MIL_ID        SystemId,
                                 MIL_INT64     ControlFlag,
                                 MIL_ID*       ContextIdPtr);

void MFTYPE MstrStream          (MIL_TEXT_PTR  MemPtrOrFileName,
                                 MIL_ID        SystemId,
                                 MIL_INT64     Operation,
                                 MIL_INT64     StreamType,
                                 MIL_DOUBLE    Version,
                                 MIL_INT64     ControlFlag,
                                 MIL_ID*       ObjectIdPtr,
                                 MIL_INT*      SizeByteVarPtr);

#endif // M_MIL_USE_UNICODE

/* C++ directive if needed */
#ifdef __cplusplus
}
#endif

#if M_MIL_USE_64BIT
#ifdef __cplusplus
//////////////////////////////////////////////////////////////
// MstrControl function definition when compiling c++ files
//////////////////////////////////////////////////////////////
#if !M_MIL_USE_LINUX
inline void MstrControl         (MIL_ID   ContextOrResultId,
                                 MIL_INT     Index,
                                 MIL_INT64   ControlType,
                                 MIL_INT32   ControlValue)
   {
   MstrControlInt64(ContextOrResultId, Index, ControlType, ControlValue);
   };
#endif

inline void MstrControl         (MIL_ID   ContextOrResultId,
                                 MIL_INT     Index,
                                 MIL_INT64   ControlType,
                                 int   ControlValue)
   {
   MstrControlInt64(ContextOrResultId, Index, ControlType, ControlValue);
   };

inline void MstrControl         (MIL_ID   ContextOrResultId,
                                 MIL_INT     Index,
                                 MIL_INT64   ControlType,
                                 MIL_INT64   ControlValue)
   {
   MstrControlInt64(ContextOrResultId, Index, ControlType, ControlValue);
   };

inline void MstrControl         (MIL_ID      ContextOrResultId,
                                 MIL_INT     Index,
                                 MIL_INT64   ControlType,
                                 MIL_DOUBLE  ControlValue)
   {
   MstrControlDouble(ContextOrResultId, Index, ControlType, ControlValue);
   };
#else

//////////////////////////////////////////////////////////////
// For C file, call the default function, i.e. Int64 one
//////////////////////////////////////////////////////////////
#define MstrControl  MstrControlDouble

#endif // __cplusplus
#endif // M_MIL_USE_64BIT

#if M_MIL_USE_SAFE_TYPE

//////////////////////////////////////////////////////////////
// See milos.h for explanation about these functions.
//////////////////////////////////////////////////////////////

// ----------------------------------------------------------
// MstrInquire

inline MIL_INT MFTYPE MstrInquireUnsafe  (MIL_ID ContextId, MIL_INT Index, MIL_INT64  InquireType, void           *UserVarPtr);
inline MIL_INT MFTYPE MstrInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT64  InquireType, int             UserVarPtr);
inline MIL_INT MFTYPE MstrInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT64  InquireType, MIL_INT8       *UserVarPtr);
inline MIL_INT MFTYPE MstrInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT64  InquireType, MIL_INT16      *UserVarPtr);
inline MIL_INT MFTYPE MstrInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT64  InquireType, MIL_INT32      *UserVarPtr);
inline MIL_INT MFTYPE MstrInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT64  InquireType, MIL_INT64      *UserVarPtr);
inline MIL_INT MFTYPE MstrInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT64  InquireType, float          *UserVarPtr);
inline MIL_INT MFTYPE MstrInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT64  InquireType, MIL_DOUBLE     *UserVarPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED                                      
inline MIL_INT MFTYPE MstrInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT64  InquireType, MIL_UINT8      *UserVarPtr);
inline MIL_INT MFTYPE MstrInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT64  InquireType, MIL_UINT16     *UserVarPtr);
inline MIL_INT MFTYPE MstrInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT64  InquireType, MIL_UINT32     *UserVarPtr);
inline MIL_INT MFTYPE MstrInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT64  InquireType, MIL_UINT64     *UserVarPtr);
#endif                                                                     
#if M_MIL_SAFE_TYPE_ADD_WCHAR_T                                            
inline MIL_INT MFTYPE MstrInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT64  InquireType, wchar_t        *UserVarPtr);
#endif

// ----------------------------------------------------------
// MstrGetResult

inline void MFTYPE MstrGetResultUnsafe  (MIL_ID ResultId, MIL_INT Index, MIL_INT64  ResultType, void           *ResultArrayPtr);
inline void MFTYPE MstrGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT64  ResultType, int             ResultArrayPtr);
inline void MFTYPE MstrGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT64  ResultType, MIL_INT8       *ResultArrayPtr);
inline void MFTYPE MstrGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT64  ResultType, MIL_INT16      *ResultArrayPtr);
inline void MFTYPE MstrGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT64  ResultType, MIL_INT32      *ResultArrayPtr);
inline void MFTYPE MstrGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT64  ResultType, MIL_INT64      *ResultArrayPtr);
inline void MFTYPE MstrGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT64  ResultType, float          *ResultArrayPtr);
inline void MFTYPE MstrGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT64  ResultType, MIL_DOUBLE     *ResultArrayPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED                                    
inline void MFTYPE MstrGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT64  ResultType, MIL_UINT8      *ResultArrayPtr);
inline void MFTYPE MstrGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT64  ResultType, MIL_UINT16     *ResultArrayPtr);
inline void MFTYPE MstrGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT64  ResultType, MIL_UINT32     *ResultArrayPtr);
inline void MFTYPE MstrGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT64  ResultType, MIL_UINT64     *ResultArrayPtr);
#endif                                                                   
#if M_MIL_SAFE_TYPE_ADD_WCHAR_T                                          
inline void MFTYPE MstrGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT64  ResultType, wchar_t        *ResultArrayPtr);
#endif

// ----------------------------------------------------------
// MstrInquire

inline MIL_INT64  MstrInquireRequiredType(MIL_ID ContextId, MIL_INT64  InquireType)
   {
   MIL_INT Encoding;
   switch (InquireType)
      {
      case M_REPORT_STRING:
         return M_TYPE_TEXT_CHAR;
         break;

      case M_CHAR_VALUE:
      case M_CONSTRAINT:
      case M_DEFAULT_CONSTRAINT:
         Encoding = MstrInquire(ContextId, M_CONTEXT, M_ENCODING+M_TYPE_MIL_INT, NULL);
         if (Encoding == M_ASCII || Encoding == M_DEFAULT)
            return M_TYPE_CHAR;
         else
            return M_TYPE_SHORT;
         break;

      default:
         return M_TYPE_DOUBLE;
         break;
      }
   }

inline MIL_INT MFTYPE MstrInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT64  InquireType, int ValuePtr)
   {
   if (ValuePtr)
      SafeTypeError(MIL_TEXT("MmodInquire"));

   return MstrInquire(ContextId, Index, InquireType, NULL);
   }

inline MIL_INT MFTYPE MstrInquireExecute (MIL_ID ContextId, MIL_INT Index, MIL_INT64  InquireType, void        *ValuePtr, MIL_INT64  GivenType)
   {
   MIL_INT64  RequiredType = (InquireType & M_HLVLDATATYPE_MASK);
   if((RequiredType != M_TYPE_MIL_INT32) && (RequiredType != M_TYPE_MIL_INT64) && (RequiredType != M_TYPE_DOUBLE) &&
      (RequiredType != M_TYPE_CHAR) && (RequiredType != M_TYPE_SHORT) && 
      (RequiredType != M_TYPE_MIL_ID) && (RequiredType != M_TYPE_FLOAT) && (RequiredType != M_TYPE_TEXT_CHAR))
      RequiredType = 0;
   if (RequiredType == 0)
      RequiredType = MstrInquireRequiredType(ContextId, InquireType);

   ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

#if M_MIL_UNICODE_API
   if(GivenType    == M_TYPE_SHORT) { GivenType    = M_TYPE_TEXT_CHAR; }
   if(RequiredType == M_TYPE_SHORT) { RequiredType = M_TYPE_TEXT_CHAR; }
#else
   if(GivenType    == M_TYPE_CHAR) { GivenType    = M_TYPE_TEXT_CHAR; }
   if(RequiredType == M_TYPE_CHAR) { RequiredType = M_TYPE_TEXT_CHAR; }
#endif

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MstrInquire"));

   return MstrInquire(ContextId, Index, InquireType, ValuePtr);
   }

inline MIL_INT MFTYPE MstrInquireUnsafe  (MIL_ID ContextId, MIL_INT Index, MIL_INT64  InquireType, void        *ValuePtr) {return MstrInquire       (ContextId, Index, InquireType, ValuePtr                  );}
inline MIL_INT MFTYPE MstrInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT64  InquireType, MIL_INT8    *ValuePtr) {return MstrInquireExecute(ContextId, Index, InquireType, ValuePtr, M_TYPE_CHAR);}
inline MIL_INT MFTYPE MstrInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT64  InquireType, MIL_INT16   *ValuePtr) {return MstrInquireExecute(ContextId, Index, InquireType, ValuePtr, M_TYPE_SHORT);}
inline MIL_INT MFTYPE MstrInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT64  InquireType, MIL_INT32   *ValuePtr) {return MstrInquireExecute(ContextId, Index, InquireType, ValuePtr, M_TYPE_MIL_INT32);}
inline MIL_INT MFTYPE MstrInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT64  InquireType, MIL_INT64   *ValuePtr) {return MstrInquireExecute(ContextId, Index, InquireType, ValuePtr, M_TYPE_MIL_INT64);}
inline MIL_INT MFTYPE MstrInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT64  InquireType, float       *ValuePtr) {return MstrInquireExecute(ContextId, Index, InquireType, ValuePtr, M_TYPE_FLOAT);}
inline MIL_INT MFTYPE MstrInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT64  InquireType, MIL_DOUBLE  *ValuePtr) {return MstrInquireExecute(ContextId, Index, InquireType, ValuePtr, M_TYPE_DOUBLE   );}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED                                      
inline MIL_INT MFTYPE MstrInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT64  InquireType, MIL_UINT8   *ValuePtr) {return MstrInquireExecute(ContextId, Index, InquireType, ValuePtr, M_TYPE_CHAR);}
inline MIL_INT MFTYPE MstrInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT64  InquireType, MIL_UINT16  *ValuePtr) {return MstrInquireExecute(ContextId, Index, InquireType, ValuePtr, M_TYPE_SHORT);}
inline MIL_INT MFTYPE MstrInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT64  InquireType, MIL_UINT32  *ValuePtr) {return MstrInquireExecute(ContextId, Index, InquireType, ValuePtr, M_TYPE_MIL_INT32);}
inline MIL_INT MFTYPE MstrInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT64  InquireType, MIL_UINT64  *ValuePtr) {return MstrInquireExecute(ContextId, Index, InquireType, ValuePtr, M_TYPE_MIL_INT64);}
#endif                                                                     
#if M_MIL_SAFE_TYPE_ADD_WCHAR_T                                            
inline MIL_INT MFTYPE MstrInquireSafeType(MIL_ID ContextId, MIL_INT Index, MIL_INT64  InquireType, wchar_t     *ValuePtr) {return MstrInquireExecute(ContextId, Index, InquireType, ValuePtr, M_TYPE_MIL_TEXT);}
#endif

// ----------------------------------------------------------
// MstrGetResult

inline MIL_INT64  MstrGetResultRequiredType(MIL_ID ResultId, MIL_INT64  ResultType)
   {
   double Encoding;
   switch (ResultType)
      {
      case M_REPORT_ERRORS:
      case M_REPORT_WARNINGS:
         return M_TYPE_MIL_INT;
         break;

      case M_TEXT:
      case M_FORMATTED_STRING:
      case M_STRING:
      case M_CHAR_VALUE:
         MstrGetResult(ResultId, M_GENERAL, M_ENCODING, &Encoding);
         if (Encoding == M_ASCII || Encoding == M_DEFAULT)
            return M_TYPE_CHAR;
         else
            return M_TYPE_SHORT;
         break;

      default:
         return M_TYPE_DOUBLE;
         break;
      }
   }

inline void MFTYPE MstrGetResultSafeType (MIL_ID ResultId, MIL_INT Index, MIL_INT64  ResultType, int  ValuePtr)
   {
   if (ValuePtr)
      SafeTypeError(MIL_TEXT("MstrGetResult"));

   MstrGetResult(ResultId, Index, ResultType, NULL);
   }

inline void MstrGetResultExecute (MIL_ID ResultId, MIL_INT Index, MIL_INT64  ResultType, void        *ValuePtr, MIL_INT64  GivenType)
   {
   // Contrary to other SafeType implementation of the function, we have no
   // choice to do the true call before verifying the type.
   // This is because there is a possibility that the MstrGetResultRequiredType
   // would do a MstrGetResult to know the required type.
   // If the client application had queued some results, the MstrGetResult
   // would obtain the whole queue of results instead of only the result
   // needed by MstrGetResultRequiredType.
   // So let the client application obtain its data and flush the queue
   // before proceeding for type validation.
   MstrGetResult(ResultId, Index, ResultType, ValuePtr);

   MIL_INT64  RequiredType = (ResultType & M_HLVLDATATYPE_MASK);
   if((RequiredType != M_TYPE_MIL_INT32) && (RequiredType != M_TYPE_MIL_INT64) && (RequiredType != M_TYPE_DOUBLE) &&
      (RequiredType != M_TYPE_CHAR) && (RequiredType != M_TYPE_SHORT) && 
      (RequiredType != M_TYPE_MIL_ID) && (RequiredType != M_TYPE_FLOAT) && (RequiredType != M_TYPE_TEXT_CHAR))
      RequiredType = 0;
   if (RequiredType == 0)
      RequiredType = MstrGetResultRequiredType(ResultId, ResultType);

   ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

#if M_MIL_UNICODE_API      
      if(GivenType    == M_TYPE_SHORT) { GivenType    = M_TYPE_TEXT_CHAR; }
      if(RequiredType == M_TYPE_SHORT) { RequiredType = M_TYPE_TEXT_CHAR; }
#else
      if(GivenType    == M_TYPE_CHAR) { GivenType    = M_TYPE_TEXT_CHAR; }
      if(RequiredType == M_TYPE_CHAR) { RequiredType = M_TYPE_TEXT_CHAR; }
#endif

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MstrGetResult"));
   }

inline void MFTYPE MstrGetResultUnsafe  (MIL_ID ResultId, MIL_INT Index, MIL_INT64  ResultType, void        *ValuePtr) {MstrGetResult       (ResultId, Index, ResultType, ValuePtr                  );}
inline void MFTYPE MstrGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT64  ResultType, MIL_INT8    *ValuePtr) {MstrGetResultExecute(ResultId, Index, ResultType, ValuePtr, M_TYPE_CHAR);}
inline void MFTYPE MstrGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT64  ResultType, MIL_INT16   *ValuePtr) {MstrGetResultExecute(ResultId, Index, ResultType, ValuePtr, M_TYPE_SHORT);}
inline void MFTYPE MstrGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT64  ResultType, MIL_INT32   *ValuePtr) {MstrGetResultExecute(ResultId, Index, ResultType, ValuePtr, M_TYPE_MIL_INT32);}
inline void MFTYPE MstrGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT64  ResultType, MIL_INT64   *ValuePtr) {MstrGetResultExecute(ResultId, Index, ResultType, ValuePtr, M_TYPE_MIL_INT64);}
inline void MFTYPE MstrGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT64  ResultType, float       *ValuePtr) {MstrGetResultExecute(ResultId, Index, ResultType, ValuePtr, M_TYPE_FLOAT);}
inline void MFTYPE MstrGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT64  ResultType, MIL_DOUBLE  *ValuePtr) {MstrGetResultExecute(ResultId, Index, ResultType, ValuePtr, M_TYPE_DOUBLE   );}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED                                    
inline void MFTYPE MstrGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT64  ResultType, MIL_UINT8   *ValuePtr) {MstrGetResultExecute(ResultId, Index, ResultType, ValuePtr, M_TYPE_CHAR);}
inline void MFTYPE MstrGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT64  ResultType, MIL_UINT16  *ValuePtr) {MstrGetResultExecute(ResultId, Index, ResultType, ValuePtr, M_TYPE_SHORT);}
inline void MFTYPE MstrGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT64  ResultType, MIL_UINT32  *ValuePtr) {MstrGetResultExecute(ResultId, Index, ResultType, ValuePtr, M_TYPE_MIL_INT32);}
inline void MFTYPE MstrGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT64  ResultType, MIL_UINT64  *ValuePtr) {MstrGetResultExecute(ResultId, Index, ResultType, ValuePtr, M_TYPE_MIL_INT64);}
#endif
#if M_MIL_SAFE_TYPE_ADD_WCHAR_T
inline void MFTYPE MstrGetResultSafeType(MIL_ID ResultId, MIL_INT Index, MIL_INT64  ResultType, wchar_t     *ValuePtr) {MstrGetResultExecute(ResultId, Index, ResultType, ValuePtr, M_TYPE_MIL_TEXT);}
#endif

#define MstrGetResult        MstrGetResultSafeType
#define MstrInquire          MstrInquireSafeType

#else // #if M_MIL_USE_SAFE_TYPE

#define MstrGetResultUnsafe        MstrGetResult
#define MstrInquireUnsafe          MstrInquire

#endif // #if M_MIL_USE_SAFE_TYPE

#endif // !M_MIL_LITE

#endif /* __MILSTR_H__ */
