/***************************************************************************/
/*

   Filename:  MilBead.h
   Owner   :  Matrox Imaging dept.
   Revision:  10.00.2564
   Content :  This file contains the defines and the prototypes for the
   MIL bead verifier. (Mbead...)

   Copyright © Matrox Electronic Systems Ltd., 1992-2014.
   All Rights Reserved

*/
/***************************************************************************/
#ifndef __MIL_BEAD_H__
#define __MIL_BEAD_H__

#if (!M_MIL_LITE) // MIL FULL ONLY

/* C++ directive if needed */
#ifdef __cplusplus
extern "C"
{
#endif

   /***************************************************************************/
   /*                        MBead CAPI defines                               */
   /***************************************************************************/
#define M_CONTEXT                                  0x08000000L // Already defined in milmod.h, milreg.h, milstr.h, milocr.h, milcol.h, ...
#define M_GENERAL                                  0x20000000L // Already defined in milmod.h, milreg.h, milstr.h, milocr.h, milcol.h, ...
#define M_ALL                                      0x40000000L // Already defined in milmod.h, milmeas.h, milocr.h,  milpat.h,  mil.h, miledge.h, milcol.h, ...

   // Control Flags for Context
#define M_TRAINING_BOX_WIDTH                       17L         // Width of the search boxes in world units.
#define M_BOX_WIDTH_MARGIN                         2L          // Additional width margin of the search boxes in %.
#define M_TRAINING_BOX_HEIGHT                      3L          // Height of the search boxes in world units.
#define M_TRAINED_BOX_SPACING                      25L         // Readable: inquire the real spacing used for verify operation.
#define M_TRAINING_BOX_SPACING                     40L         // Space between search boxes in world units.
#define M_TRAINING_BOX_INPUT_UNITS                 78L         //
#define M_TEMPLATE_INPUT_UNITS                     79L         //
#define M_SMOOTHNESS                               108L        // Sets the degree of smoothness (strength of denoising), Already defined in milmod.h, milmeas.h, miledge.h, milmetrol.h, mil.h, ...
#define M_TRAINED_WIDTH_NOMINAL                    92L         //
#define M_THRESHOLD_MODE                           33L         // Sets the threshold mode setting, Already defined in milcode.h, miledge.h, milmetrol.h, milstr.h, ...
#define M_THRESHOLD_VALUE                          953L        // Sets the threshold value, Already defined in milcode.h, milstr.h, ...
#define M_FOREGROUND_VALUE                         4L          // Set the color of the bead, Already defined in milblob.h, milcal.h, milcode.h, miledge.h, milmod.h, milocr.h, milstr.h, ...
#define M_WIDTH_NOMINAL                            5L          // Set the nominal width of the bead, Already defined in milmeas.h, milblob,h, ...
#define M_WIDTH_DELTA_NEG                          6L          // Set the delta neg tolerance of the bead's width
#define M_WIDTH_DELTA_POS                          7L          // Set the delta pos tolerance of the bead's width
#define M_OFFSET_MAX                               88L         // Set the max offset tolerance of the bead position relative to the Template, in world units.
#define M_FAIL_WARNING_OFFSET                      99L         // Set the max offset to find the bead position relative to the Template, in world units.
#define M_FOUND_OFFSET_MAX                         M_FAIL_WARNING_OFFSET  // deprecated
#define M_GAP_MAX_LENGTH                           9L          // Set the max valid gap length in world units
#define M_GAP_COVERAGE                             216L        // Retrieve the % of bead found for a Template to be valid
#define M_ACCEPTANCE                               200L        // Set the min % of valid beads for a Template to be valid, Already defined in milmod.h, milpat.h, milreg.h, milcol.h
#define M_ACCURACY                                 106L        // Set the bead verification accuracy level for a Template, Already defined in milmod.h,...
#define M_ACCURACY_MAX_ANGULAR_DEVIATION           107L
#define M_ACCURACY_MAX_ANGLE_DEVIATION             M_ACCURACY_MAX_ANGULAR_DEVIATION
#define M_ANGLE_ACCURACY_MAX_DEVIATION             M_ACCURACY_MAX_ANGULAR_DEVIATION
#define M_MEASUREMENT_MARKER                       109L
#define M_WIDTH_NOMINAL_MODE                       12L         // Set the nominal width mode of the bead
#define M_TRAINING_IMAGE_ID                        101L        // Set the training image id
#define M_ASSOCIATED_CALIBRATION                   125L        // Set the associated calibration to the training image. // Already defined in milmod.h, milmet.h,...
#define M_CLOSEST_POINT_MAX_DISTANCE               176L        // Set the max distance value for template closest operations
#define M_GLOBAL_FRAME_OFFSET_X                    1000L
#define M_GLOBAL_FRAME_OFFSET_Y                    1001L
#define M_GLOBAL_FRAME_ROTATION                    1002L
#define M_TRAINING_GLOBAL_FRAME_OFFSET_X           1003L
#define M_TRAINING_GLOBAL_FRAME_OFFSET_Y           1004L
#define M_TRAINING_GLOBAL_FRAME_ROTATION           1005L
#define M_TRAINING_IS_CALIBRATED                   1006L
#define M_POSITION_MODE                            1462L       // deprecated flag
#define M_TRAINING_PATH                            M_POSITION_MODE
#define M_TEMPLATE_INDEX_FROM_LABEL                M_INDEX_VALUE
#define M_DRAW_INDEX_OFFSET                        1014L       // Already defined in milmetrol.h
#define M_RESTORED_CALIBRATION                     1491L       // Already in milmod.h
#define M_TRAINED_GAP_MAX_LENGTH                   203L
#define M_TRAINED_WIDTH_DELTA_POS                  204L
#define M_TRAINED_WIDTH_DELTA_NEG                  205L
#define M_TRAINED_OFFSET_MAX                       206L
#define M_TRAINED_INDIVIDUAL_WIDTH_NOMINAL         207L        //
#define M_TRAINED_INDIVIDUAL_BOX_WIDTH             208L        //
#define M_TRAINED_SIZE                             209L        //
#define M_TRAINED_POSITION_X                       210L        //
#define M_TRAINED_POSITION_Y                       211L        //
#define M_INDIVIDUAL_WIDTH_NOMINAL                 212L        //
#define M_TRAINED_FAIL_WARNING_OFFSET              213L

#define M_CIRCLE                                   0x00000008L  // already defined
#define M_TEMPLATE_CIRCLE_CENTER_X                 302L
#define M_TEMPLATE_CIRCLE_CENTER_Y                 303L
#define M_TEMPLATE_CIRCLE_RADIUS                   304L

#define M_SEGMENT                                  0x00000082L  // already defined
#define M_TEMPLATE_SEGMENT_START_X                 305L
#define M_TEMPLATE_SEGMENT_START_Y                 306L
#define M_TEMPLATE_SEGMENT_END_X                   307L
#define M_TEMPLATE_SEGMENT_END_Y                   308L

#define M_POLYLINE_SEED                            444L         // = deprecated M_AUTO
#define M_POLYLINE                                 0x00000020L  // deprecated M_USER_DEFINED

// Control Flags for Context or Result
#define M_DRAW_RELATIVE_ORIGIN_X                   319L        // deprecated : Use MgraControl(... M_DRAW_OFFSET_X...)
#define M_DRAW_RELATIVE_ORIGIN_Y                   320L        // deprecated : Use MgraControl(... M_DRAW_OFFSET_Y...)
#define M_DRAW_SCALE_X                             3203L       // deprecated : Use MgraControl(... M_DRAW_ZOOM_X ...)
#define M_DRAW_SCALE_Y                             3204L       // deprecated : Use MgraControl(... M_DRAW_ZOOM_Y ...)
#define M_WORLD                                    0x2000L     // Already defined in milmeas.h,...
#define M_PIXEL                                    0x1000L     // Already defined in milmeas.h,...
#define M_ENHANCED                                 1L

   // Control values 
#define M_USER_DEFINED                             21L         // Already defined in milcal.h, milcode.h, milcolor.h, miledge.h, milmetrol.h, milocr.h, milstr.h, ...
#define M_FOREGROUND_BLACK                         0x100L      // Already defined in    milblob.h, milmod.h,  milcode.h, milocr.h, miledge.h
#define M_FOREGROUND_WHITE                         0x80L       // Already defined in    milblob.h, milmod.h,  milcode.h, milocr.h, miledge.h
#define M_AUTO                                     444L        // Already defined in mil.h, ...
#define M_AUTO_UNIFORM                             M_AUTO
#define M_AUTO_CONTINUOUS                          445
#define M_DISABLE                                 -9999L       // Already defined in milblob.h, milcal.h, milmeas.h, mil.h, ...

   // Strategy Flag
#define M_MEAS_STRATEGY                            1L          // Set the strategy to measurement

   // Inquire Flags for Context or Result
#define M_MODIFICATION_COUNT                       5010L       // To retrieve the modification counter, Already defined in mil.h, ...
#define M_NUMBER_OF_TEMPLATES                      1L          // To retrieve the number of Template in context or result
#define M_LABEL_VALUE                              1L          // Already defined in mil.h, ...
#define M_INDEX_VALUE                              M_INDEX     // Already defined in mil.h, ...
#define M_SIZE                                     0xC00L      // Retrieve the size of a template. Already defined in milmod, miledge,...
#define M_POSITION_X                               0x00003400L // Already defined in miledge.h, milcode.h, ...
#define M_POSITION_Y                               0x00004400L // Already defined in miledge.h, milcode.h, ...

   // Template function flags
#define M_BEAD_STRIPE                              1L          // Type (stripe) of template to add
#define M_BEAD_EDGE                                2L          // Type (edge) of template to add
#define M_TYPE                                     1008L       // Already defined in mil.h,...
#define M_BEAD_TYPE                                M_TYPE      // Type of the template
#define M_ADD                                      0x0000L     // Add a new Template in the context, Already defined in milcode.h, milim.h, milocr.h, ...
#define M_REPLACE                                  0x01000060L // Already defined in mil.h, milocr.h, miledge.h
#define M_INSERT                                   5L          // Insert points to an existing Template
#define M_DELETE                                   3L          // Delete a Template in the context, Already defined in milblob.h, milcode.h, milcolor.h, miledge.h, milmetrol.h, milmod.h, mil.h, ...
#define M_TRANSLATE_POINTS                         8L          // Move all points of a Template (rigid transformation only)
#define M_SCALE_POINTS                             229L        // Scale all points of a Template
#define M_ROTATE_POINTS                            210L        // Rotate all points of a Template
#define M_SET_WIDTH_NOMINAL                        11L         // To set the width nominal values
#define M_RESET_WIDTH                              0L          // Value to reset the user defined width setting for a specific bead section

   // GetNeighbors function flags
#define M_CLOSEST_TEMPLATE                         11L         // Retrieve closest Templates from a user-defined point
#define M_CLOSEST_POINT                            12L         // Retrieve closest point of a Template from a user-defined point

   // Train operation flag (bitwise)
#define M_POSITION_DISABLE                         1L          // The training is done only to search the nominal width and other values
#define M_SAVE_TRAINING_IMAGE                      2L          // To save the training image into the context
#define M_FREE_TRAINING_IMAGE                      3L          // To release the internal training image if any
#define M_TRAINING_IMAGE_SIZE_X                    15L         // Size of the training image
#define M_TRAINING_IMAGE_SIZE_Y                    16L         // Size of the training image
#define M_TRAINING_IMAGE_TYPE                      17L         // Type of the training image

   // Context status flag
#define M_COMPLETE                                 0x00000000L // Already defined in mil.h, milmod.h
#define M_PARTIAL                                  0x00000001L // Already defined in mil.h, milmod.h
#define M_NOT_TRAINED                              2L       

   // Result type Flag 
#define M_STATUS                                   0x00008002L // Already defined in milcode.h, milcolor.h, miledge.h, ...
#define M_NUMBER                                   1009L       // Already defined in mil.h, ...
#define M_GAP_TOLERANCE                            216L        // Already defined in milmetrol.h, ...
#define M_WIDTH_MIN                                74L         // 
#define M_WIDTH_MAX                                30L         // 
#define M_WIDTH_AVERAGE                            3L          // 
#define M_POSITION_X                               0x00003400L // Already defined in milmod, milpat.h, ...
#define M_POSITION_Y                               0x00004400L // Already defined in milmod, milpat.h, ...
#define M_OFFSET                                   2L
#define M_ANGLE                                    0x00000800L // Already defined in milmod, milpat.h, ...
#define M_TRAINED_BOX_ANGLE                        33L
#define M_TRAINED_BOX_WIDTH                        48L
#define M_TRAINED_BOX_HEIGHT                       49L
#define M_WIDTH_MIN_INDEX                          10L         // 
#define M_WIDTH_MAX_INDEX                          20L         // 
#define M_STATUS_GAP_TOLERANCE                     22L
#define M_STATUS_GAP_MAX                           23L
#define M_STATUS_SCORE                             24L
#define M_TRAINED_INDEX                            25L
#define M_STATUS_SEARCH                            26L           
#define M_STATUS_OFFSET                            27L
#define M_STATUS_WIDTH_MIN                         28L
#define M_STATUS_WIDTH_MAX                         29L
#define M_WIDTH_VALUE                              31L
#define M_STATUS_FOUND                             32L
#define M_SCORE                                    0x00001400L // Already defined in milmod, milpat.h, ...
#define M_STATUS_INTENSITY_MIN                     50L
#define M_STATUS_INTENSITY_MAX                     51L
#define M_INTENSITY                                52L
#define M_TRAINING_WIDTH_NOMINAL                   60L
#define M_TRAINING_WIDTH_SCALE_MIN                 61L
#define M_TRAINING_WIDTH_SCALE_MAX                 62L
#define M_INTENSITY_NOMINAL_MODE                   80L
#define M_INTENSITY_NOMINAL                        81L
#define M_INTENSITY_DELTA_POS                      82L
#define M_INTENSITY_DELTA_NEG                      83L
#define M_INTENSITY_MIN                            84L
#define M_INTENSITY_MAX                            85L
#define M_INTENSITY_MIN_INDEX                      86L
#define M_INTENSITY_MAX_INDEX                      87L
#define M_OFFSET_MAX_INDEX                         89L
#define M_NUMBER_FOUND                             90L

// General values
#define M_BEGIN                                    -1L
#define M_END                                      -2L
#define M_INFINITE                                 -1L // Also defined in Mil.h

// Result values
#define M_PASS                                     0x0002L     // Already defined in milim.h, ...
#define M_FAIL                                     4L          // Already defined in milmetrol.h, ...
#define M_FAIL_WIDTH_MIN                           7L          // 
#define M_FAIL_WIDTH_MAX                           8L          // 
#define M_FAIL_GAP_MAX                             9L          // 
#define M_FAIL_OFFSET                              10L         //
#define M_FAIL_NOT_FOUND                           11L         //
#define M_FAIL_INTENSITY_MIN                       12L         //
#define M_FAIL_INTENSITY_MAX                       13L         //

// Draw operation flags
#define M_DRAW_POSITION                            0x00000040L // Already defined in milmod.h, milpat.h, ...
#define M_DRAW_POSITION_POLYLINE                   2L          //
#define M_DRAW_INDEX                               0x00000100L // Already defines in miledge.h
#define M_DRAW_SEARCH_BOX                          6L          //    
#define M_DRAW_WIDTH                               0x00000008L // Already 
#define M_DRAW_BEAD                                4L          //
#define M_DRAW_BEAD_WIDTH_MIN                      5L          //
#define M_DRAW_BEAD_WIDTH_MAX                      6L          //
#define M_DRAW_LABEL                               0x00000001L // Already defines in miledge.h, milmetrol.h, ...
#define M_DRAW_TRAINING_IMAGE                      15L
#define M_DRAW_SEARCH_BOX_FILL                     16L
#define M_DRAW_BEAD_FILL                           17L
#define M_DRAW_GAP                                 18L
#define M_DRAW_GAP_FILL                            19L
#define M_DRAW_POSITION_INDEX                      20L
#define M_DRAW_GLOBAL_FRAME                        21L
#define M_DRAW_CURRENT_GLOBAL_FRAME                22L

// Draw option flags
#define M_USER                                     0x00000001L // Already defines in mil.h,...
#define M_TRAINED                                  12L
#define M_TRAINED_PASS                             11L
#define M_TRAINED_FAIL                             9L
#define M_ALL                                      0x40000000L // Already defines in milmetrol.h,...
#define M_PASS                                     0x0002L     // Already defines in milmetrol.h,...
#define M_FAIL                                     4L          // Already defines in milmetrol.h,...
#define M_FAIL_WIDTH_MAX                           8L 
#define M_FAIL_WIDTH_MIN                           7L 
#define M_FAIL_OFFSET                              10L 

/***************************************************************************/
/* MbeadSave()/MbeadRestore()/MbeadStream()                                   */
/***************************************************************************/
#define M_MEMORY           (M_APP_INQUIRE_SYS_START+120L) /*5120*/  // Already defined in Mil.h
#define M_INTERACTIVE                    M_NULL // Already defined in    mil.h, milcal.h, milcode.h, milmeas.h, milocr.h, milpat.h, milmod.h

#define M_SAVE                             0x100L

#define M_RESTORE                           0x10L

#define M_LOAD                                 1L
#define M_INQUIRE_SIZE_BYTE                    2L

#define M_WITH_CALIBRATION            0x00020000L // Already in mil.h



/***************************************************************************/
/* Utilities                                                               */
/***************************************************************************/

#define M_TEMPLATE_LABEL_TAG                 0x01000000L
#define M_TEMPLATE_INDEX_TAG                 0x02000000L

#define M_TEMPLATE_LABEL(i)                  (M_TEMPLATE_LABEL_TAG   | (i))
#define M_TEMPLATE_INDEX(i)                  (M_TEMPLATE_INDEX_TAG   | (i))

   //*****************************************************************************************
   // CAPI function prototypes

#if M_MIL_USE_64BIT
   // Prototypes for 64 bits OS
   void MFTYPE MbeadControlInt64(MIL_ID     ContextId,
                                 MIL_INT    LabelOrIndex,
                                 MIL_INT64  ControlType,
                                 MIL_INT64  ControlValue);

   void MFTYPE MbeadControlDouble(MIL_ID     ContextId,
                                  MIL_INT    LabelOrIndex,
                                  MIL_INT64  ControlType,
                                  MIL_DOUBLE ControlValue);
#else

   // Prototypes for 32 bits OS
#define MbeadControlInt64  MbeadControl
#define MbeadControlDouble MbeadControl

   void MFTYPE MbeadControl(MIL_ID     ContextId,
                            MIL_INT    LabelOrIndex,
                            MIL_INT64  ControlType,
                            MIL_DOUBLE ControlValue);
#endif

   MIL_ID   MFTYPE  MbeadAlloc(MIL_ID     SystemId,
                               MIL_INT64  Strategy,
                               MIL_INT64  ControlFlag,
                               MIL_ID*    ContextIdPtr);

   MIL_ID   MFTYPE  MbeadAllocResult(MIL_ID     SystemId,
                                     MIL_INT64  ControlFlag,
                                     MIL_ID*    ResultIdPtr);

   void     MFTYPE  MbeadFree(MIL_ID   ContextOrResultId);


   MIL_INT  MFTYPE  MbeadInquire(MIL_ID     ContextOrResultId,
                                 MIL_INT    LabelOrIndex,
                                 MIL_INT64  InquireType,
                                 void*      UserVarPtr);

   void     MFTYPE  MbeadTemplate(MIL_ID            ContextId,
                                  MIL_INT64         Operation,
                                  MIL_INT64         BeadType,
                                  MIL_INT           LabelOrIndex,
                                  MIL_INT           SizeOfArray,
                                  const MIL_DOUBLE* FirstArrayPtr,
                                  const MIL_DOUBLE* SecondArrayPtr,
                                  const MIL_INT*    ThirdArrayPtr,
                                  MIL_INT64         ControlFlag);

   void     MFTYPE  MbeadTrain(MIL_ID     ContextId,
                               MIL_ID     TargetImageId,
                               MIL_INT64  ControlFlag);

   void     MFTYPE  MbeadVerify(MIL_ID     ContextId,
                                MIL_ID     TargetImageId,
                                MIL_ID     ResultId,
                                MIL_INT64  ControlFlag);

   void     MFTYPE  MbeadGetResult(MIL_ID       ResultId,
                                   MIL_INT      LabelOrIndex,
                                   MIL_INT      ResultIndex,
                                   MIL_INT64    ResultType,
                                   void*        UserVarPtr);

   void     MFTYPE   MbeadGetNeighbors(MIL_ID     ContextId,
                                       MIL_INT    LabelOrIndex,
                                       MIL_DOUBLE PositionX,
                                       MIL_DOUBLE PositionY,
                                       MIL_INT*   TemplateLabelPtr,
                                       MIL_INT*   PointIndexPtr,
                                       MIL_INT64  ControlFlag);

   void     MFTYPE  MbeadDraw(MIL_ID      ContextGraId,
                              MIL_ID      ContextOrResultId,
                              MIL_ID      DstImageBufOrListGraId,
                              MIL_INT64   DrawOperation,
                              MIL_INT64   DrawOption,
                              MIL_INT     LabelOrIndex,
                              MIL_INT     Index,
                              MIL_INT64   ControlFlag);

#if M_MIL_USE_UNICODE

   MIL_ID MFTYPE MbeadRestoreW(MIL_CONST_TEXT_PTR FileName,
                               MIL_ID     SystemId,
                               MIL_INT64  ControlFlag,
                               MIL_ID*    ContextIdPtr);

   void MFTYPE MbeadSaveW(MIL_CONST_TEXT_PTR FileName,
                          MIL_ID     ContextId,
                          MIL_INT64  ControlFlag);

   void MFTYPE MbeadStreamW(MIL_TEXT_PTR MemPtrOrFileName,
                            MIL_ID     SystemId,
                            MIL_INT64  Operation,
                            MIL_INT64  StreamType,
                            MIL_DOUBLE Version,
                            MIL_INT64  ControlFlag,
                            MIL_ID*    ContextIdPtr,
                            MIL_INT*   SizeByteVarPtr);

   MIL_ID MFTYPE MbeadRestoreA(const char* FileName,
                               MIL_ID      SystemId,
                               MIL_INT64   ControlFlag,
                               MIL_ID*     ContextIdPtr);

   void MFTYPE MbeadSaveA(const char* FileName,
                          MIL_ID      ContextId,
                          MIL_INT64   ControlFlag);

   void MFTYPE MbeadStreamA(char*      MemPtrOrFileName,
                            MIL_ID     SystemId,
                            MIL_INT64  Operation,
                            MIL_INT64  StreamType,
                            MIL_DOUBLE Version,
                            MIL_INT64  ControlFlag,
                            MIL_ID*    ContextIdPtr,
                            MIL_INT*   SizeByteVarPtr);

#if M_MIL_UNICODE_API
#define MbeadSave           MbeadSaveW
#define MbeadRestore        MbeadRestoreW
#define MbeadStream         MbeadStreamW
#else
#define MbeadSave           MbeadSaveA
#define MbeadRestore        MbeadRestoreA
#define MbeadStream         MbeadStreamA
#endif

#else

   MIL_ID MFTYPE MbeadRestore(MIL_CONST_TEXT_PTR FileName,
                              MIL_ID             SystemId,
                              MIL_INT64          ControlFlag,
                              MIL_ID*            ContextIdPtr);

   void MFTYPE MbeadSave(MIL_CONST_TEXT_PTR FileName,
                         MIL_ID             ContextId,
                         MIL_INT64          ControlFlag);

   void MFTYPE MbeadStream(MIL_TEXT_PTR MemPtrOrFileName,
                           MIL_ID       SystemId,
                           MIL_INT64    Operation,
                           MIL_INT64    StreamType,
                           MIL_DOUBLE   Version,
                           MIL_INT64    ControlFlag,
                           MIL_ID*      ContextIdPtr,
                           MIL_INT*     SizeByteVarPtr);
#endif

   //*****************************************************************************************
#ifdef __cplusplus
}
#endif

#if M_MIL_USE_64BIT
#ifdef __cplusplus
//////////////////////////////////////////////////////////////
// M3dmapControl function definition when compiling c++ files
//////////////////////////////////////////////////////////////
#if !M_MIL_USE_LINUX
inline void MbeadControl(MIL_ID ContextId, MIL_INT LabelOrIndex, MIL_INT64  ControlType, int ControlValue)
   {
   MbeadControlInt64(ContextId, LabelOrIndex, ControlType, ControlValue);
   };
#endif
inline void MbeadControl(MIL_ID ContextId, MIL_INT LabelOrIndex, MIL_INT64  ControlType, MIL_INT32 ControlValue)
   {
   MbeadControlInt64(ContextId, LabelOrIndex, ControlType, ControlValue);
   };

inline void MbeadControl(MIL_ID ContextId, MIL_INT LabelOrIndex, MIL_INT64  ControlType, MIL_INT64 ControlValue)
   {
   MbeadControlInt64(ContextId, LabelOrIndex, ControlType, ControlValue);
   };

inline void MbeadControl(MIL_ID ContextId, MIL_INT LabelOrIndex, MIL_INT64  ControlType, MIL_DOUBLE ControlValue)
   {
   MbeadControlDouble(ContextId, LabelOrIndex, ControlType, ControlValue);
   };
#else
//////////////////////////////////////////////////////////////
// For C file, call the default function, i.e. Double one
//////////////////////////////////////////////////////////////
#define MbeadControl  MbeadControlDouble

#endif // __cplusplus
#endif // M_MIL_USE_64BIT



#if M_MIL_USE_SAFE_TYPE

//////////////////////////////////////////////////////////////
// See milos.h for explanation about these functions.
//////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------
//  MbeadGetResult

inline void MFTYPE MbeadGetResultUnsafe  (MIL_ID ResultId, MIL_INT LabelOrIndex, MIL_INT ResultIndex, MIL_INT64 ResultType, void           *UserVarPtr);
inline void MFTYPE MbeadGetResultSafeType(MIL_ID ResultId, MIL_INT LabelOrIndex, MIL_INT ResultIndex, MIL_INT64 ResultType, int             UserVarPtr);
inline void MFTYPE MbeadGetResultSafeType(MIL_ID ResultId, MIL_INT LabelOrIndex, MIL_INT ResultIndex, MIL_INT64 ResultType, MIL_INT32      *UserVarPtr);
inline void MFTYPE MbeadGetResultSafeType(MIL_ID ResultId, MIL_INT LabelOrIndex, MIL_INT ResultIndex, MIL_INT64 ResultType, MIL_INT64      *UserVarPtr);
inline void MFTYPE MbeadGetResultSafeType(MIL_ID ResultId, MIL_INT LabelOrIndex, MIL_INT ResultIndex, MIL_INT64 ResultType, MIL_DOUBLE     *UserVarPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED                                    
inline void MFTYPE MbeadGetResultSafeType(MIL_ID ResultId, MIL_INT LabelOrIndex, MIL_INT ResultIndex, MIL_INT64 ResultType, MIL_UINT32     *UserVarPtr);
inline void MFTYPE MbeadGetResultSafeType(MIL_ID ResultId, MIL_INT LabelOrIndex, MIL_INT ResultIndex, MIL_INT64 ResultType, MIL_UINT64     *UserVarPtr);
#endif

// ----------------------------------------------------------
// MbeadInquire

inline MIL_INT MFTYPE MbeadInquireUnsafe  (MIL_ID ContextOrResultId, MIL_INT LabelOrIndex, MIL_INT64 InquireType, void           *UserVarPtr);
inline MIL_INT MFTYPE MbeadInquireSafeType(MIL_ID ContextOrResultId, MIL_INT LabelOrIndex, MIL_INT64 InquireType, MIL_INT32      *UserVarPtr);
inline MIL_INT MFTYPE MbeadInquireSafeType(MIL_ID ContextOrResultId, MIL_INT LabelOrIndex, MIL_INT64 InquireType, MIL_INT64      *UserVarPtr);
inline MIL_INT MFTYPE MbeadInquireSafeType(MIL_ID ContextOrResultId, MIL_INT LabelOrIndex, MIL_INT64 InquireType, MIL_DOUBLE     *UserVarPtr);
inline MIL_INT MFTYPE MbeadInquireSafeType(MIL_ID ContextOrResultId, MIL_INT LabelOrIndex, MIL_INT64 InquireType, int             UserVarPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED                                      
inline MIL_INT MFTYPE MbeadInquireSafeType(MIL_ID ContextOrResultId, MIL_INT LabelOrIndex, MIL_INT64 InquireType, MIL_UINT32     *UserVarPtr);
inline MIL_INT MFTYPE MbeadInquireSafeType(MIL_ID ContextOrResultId, MIL_INT LabelOrIndex, MIL_INT64 InquireType, MIL_UINT64     *UserVarPtr);
#endif

// -------------------------------------------------------------------------
// MbeadGetResult

inline void MFTYPE MbeadGetResultSafeType (MIL_ID ResultId, MIL_INT LabelOrIndex, MIL_INT ResultIndex, MIL_INT64 ResultType, int ResultPtr)
   {
   if (ResultPtr)
      SafeTypeError(MIL_TEXT("MmbeadGetResult"));

   MbeadGetResult(ResultId, LabelOrIndex, ResultIndex, ResultType, NULL);
   }

inline void MbeadGetResultSafeTypeExecute (MIL_ID ResultId, MIL_INT LabelOrIndex, MIL_INT ResultIndex, MIL_INT64 ResultType, void *ResultArrayPtr, MIL_INT64  GivenType)
   {
   MIL_INT64  RequiredType = (ResultType & M_HLVLDATATYPE_MASK);
   if((RequiredType != M_TYPE_DOUBLE) && (RequiredType != M_TYPE_MIL_INT32) && 
      (RequiredType != M_TYPE_MIL_INT64) && (RequiredType != M_TYPE_MIL_ID))
      RequiredType = 0;
   if (RequiredType == 0)
      RequiredType = M_TYPE_DOUBLE;
   ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MbeadGetResult"));

   MbeadGetResult(ResultId, LabelOrIndex, ResultIndex, ResultType, ResultArrayPtr);
   }

inline void MFTYPE MbeadGetResultUnsafe  (MIL_ID ResultId, MIL_INT LabelOrIndex, MIL_INT ResultIndex, MIL_INT64 ResultType, void        *ResultArrayPtr){MbeadGetResult               (ResultId, LabelOrIndex, ResultIndex, ResultType, ResultArrayPtr);}
inline void MFTYPE MbeadGetResultSafeType(MIL_ID ResultId, MIL_INT LabelOrIndex, MIL_INT ResultIndex, MIL_INT64 ResultType, MIL_INT32   *ResultArrayPtr){MbeadGetResultSafeTypeExecute(ResultId, LabelOrIndex, ResultIndex, ResultType, ResultArrayPtr, M_TYPE_MIL_INT32);}
inline void MFTYPE MbeadGetResultSafeType(MIL_ID ResultId, MIL_INT LabelOrIndex, MIL_INT ResultIndex, MIL_INT64 ResultType, MIL_INT64   *ResultArrayPtr){MbeadGetResultSafeTypeExecute(ResultId, LabelOrIndex, ResultIndex, ResultType, ResultArrayPtr, M_TYPE_MIL_INT64);}
inline void MFTYPE MbeadGetResultSafeType(MIL_ID ResultId, MIL_INT LabelOrIndex, MIL_INT ResultIndex, MIL_INT64 ResultType, MIL_DOUBLE  *ResultArrayPtr){MbeadGetResultSafeTypeExecute(ResultId, LabelOrIndex, ResultIndex, ResultType, ResultArrayPtr, M_TYPE_DOUBLE);}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED                                    
inline void MFTYPE MbeadGetResultSafeType(MIL_ID ResultId, MIL_INT LabelOrIndex, MIL_INT ResultIndex, MIL_INT64 ResultType, MIL_UINT32  *ResultArrayPtr){MbeadGetResultSafeTypeExecute(ResultId, LabelOrIndex, ResultIndex, ResultType, ResultArrayPtr, M_TYPE_MIL_INT32);}
inline void MFTYPE MbeadGetResultSafeType(MIL_ID ResultId, MIL_INT LabelOrIndex, MIL_INT ResultIndex, MIL_INT64 ResultType, MIL_UINT64  *ResultArrayPtr){MbeadGetResultSafeTypeExecute(ResultId, LabelOrIndex, ResultIndex, ResultType, ResultArrayPtr, M_TYPE_MIL_INT64);}
#endif

// ----------------------------------------------------------
// MbeadInquire

inline MIL_INT MFTYPE MbeadInquireSafeType  (MIL_ID ContextOrResultId, MIL_INT LabelOrIndex, MIL_INT64 InquireType, int UserVarPtr )
   {
   if(UserVarPtr)
      SafeTypeError(MIL_TEXT("MbeadInquire"));

   return MbeadInquire(ContextOrResultId, LabelOrIndex, InquireType, NULL);
   }

inline MIL_INT MFTYPE MbeadInquireSafeTypeExecute (MIL_ID ContextOrResultId, MIL_INT LabelOrIndex, MIL_INT64 InquireType, void *UserVarPtr, MIL_INT64 GivenType)
   {
   MIL_INT64  RequiredType = (InquireType & M_HLVLDATATYPE_MASK);
   if((RequiredType != M_TYPE_DOUBLE) && (RequiredType != M_TYPE_MIL_INT32) && 
      (RequiredType != M_TYPE_MIL_INT64) && (RequiredType != M_TYPE_MIL_ID))
      RequiredType = 0;
   if (RequiredType == 0)
      RequiredType = M_TYPE_DOUBLE;
   ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MbeadInquire"));

   return MbeadInquire(ContextOrResultId, LabelOrIndex, InquireType, UserVarPtr);
   }

inline MIL_INT MFTYPE MbeadInquireUnsafe  (MIL_ID ContextOrResultId, MIL_INT LabelOrIndex, MIL_INT64 InquireType, void       *UserVarPtr) {return MbeadInquire               (ContextOrResultId, LabelOrIndex, InquireType, UserVarPtr                  );}
inline MIL_INT MFTYPE MbeadInquireSafeType(MIL_ID ContextOrResultId, MIL_INT LabelOrIndex, MIL_INT64 InquireType, MIL_INT32  *UserVarPtr) {return MbeadInquireSafeTypeExecute(ContextOrResultId, LabelOrIndex, InquireType, UserVarPtr, M_TYPE_MIL_INT32);}
inline MIL_INT MFTYPE MbeadInquireSafeType(MIL_ID ContextOrResultId, MIL_INT LabelOrIndex, MIL_INT64 InquireType, MIL_INT64  *UserVarPtr) {return MbeadInquireSafeTypeExecute(ContextOrResultId, LabelOrIndex, InquireType, UserVarPtr, M_TYPE_MIL_INT64);}
inline MIL_INT MFTYPE MbeadInquireSafeType(MIL_ID ContextOrResultId, MIL_INT LabelOrIndex, MIL_INT64 InquireType, MIL_DOUBLE *UserVarPtr) {return MbeadInquireSafeTypeExecute(ContextOrResultId, LabelOrIndex, InquireType, UserVarPtr, M_TYPE_DOUBLE   );}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED                                                                                                                                                
inline MIL_INT MFTYPE MbeadInquireSafeType(MIL_ID ContextOrResultId, MIL_INT LabelOrIndex, MIL_INT64 InquireType, MIL_UINT32 *UserVarPtr) {return MbeadInquireSafeTypeExecute(ContextOrResultId, LabelOrIndex, InquireType, UserVarPtr, M_TYPE_MIL_INT32);}
inline MIL_INT MFTYPE MbeadInquireSafeType(MIL_ID ContextOrResultId, MIL_INT LabelOrIndex, MIL_INT64 InquireType, MIL_UINT64 *UserVarPtr) {return MbeadInquireSafeTypeExecute(ContextOrResultId, LabelOrIndex, InquireType, UserVarPtr, M_TYPE_MIL_INT64);}
#endif

#define MbeadGetResult        MbeadGetResultSafeType
#define MbeadInquire          MbeadInquireSafeType

#else // #if M_MIL_USE_SAFE_TYPE

#define MbeadGetResultUnsafe        MbeadGetResult
#define MbeadInquireUnsafe          MbeadInquire

#endif // #if M_MIL_USE_SAFE_TYPE


#endif // !M_MIL_LITE

#endif // __MIL_BEAD_H__
