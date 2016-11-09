/***************************************************************************/
/*

    Filename:  MILPROTO.H
    Owner   :  Matrox Imaging dept.
    Revision:  10.00.2564
    Content :  This file contains the prototypes for the Matrox
               Imaging Library (MIL) C user's functions.

    Copyright © Matrox Electronic Systems Ltd., 1992-2014.
    All Rights Reserved

*/
/***************************************************************************/

#ifndef __MILPROTO_H
#define __MILPROTO_H

#if M_MIL_USE_LINUX
#include <stdarg.h>
#endif

/* C++ directive if needed */
#ifdef __cplusplus
extern "C"
{
#endif

/***************************************************************************/
/* HOOK HANDLER PROTOTYPE                                                  */
/***************************************************************************/
typedef MIL_INT (MFTYPE* MIL_HOOK_FUNCTION_PTR)(MIL_INT HookType,
                                                MIL_ID EventId,
                                                void* UserData);


/***************************************************************************/
/* GRAPHIC MODULE:                                                         */
/***************************************************************************/

      /* -------------------------------------------------------------- */
      /* -------------------------------------------------------------- */

      /* CONTROL: */

      /* -------------------------------------------------------------- */

MIL_ID MFTYPE  MgraAlloc            (MIL_ID SysId,
                                     MIL_ID* ContextGraIdPtr);

MIL_ID MFTYPE  MgraAllocList        (MIL_ID SysId,
                                     MIL_INT64 ListGraType,
                                     MIL_ID *ListGraIdPtr);

void MFTYPE    MgraFree             (MIL_ID ObjectId);

void MFTYPE    MgraColorDouble      (MIL_ID ContextGraId,
                                     MIL_DOUBLE ForegroundColor);

void MFTYPE    MgraBackColorDouble  (MIL_ID ContextGraId,
                                     MIL_DOUBLE BackgroundColor);
#if M_MIL_USE_UNICODE
void MFTYPE    MgraFontW            (MIL_ID ContextGraId,
                                     MIL_INT64  FontName);
void MFTYPE    MgraFontA            (MIL_ID ContextGraId,
                                     MIL_INT64  FontName);
#if M_MIL_UNICODE_API
#define MgraFont           MgraFontW
#else
#define MgraFont           MgraFontA
#endif

#else
void MFTYPE    MgraFont             (MIL_ID     ContextGraId,
                                     MIL_INT64  FontName);
#endif

void* MFTYPE MgraFontGetLarge(void);
void* MFTYPE MgraFontGetSmall(void);
void* MFTYPE MgraFontGetMedium(void);

void MFTYPE    MgraFontScale        (MIL_ID ContextGraId,
                                     MIL_DOUBLE XFontScale,
                                     MIL_DOUBLE YFontScale);

MIL_INT    MFTYPE    MgraInquire    (MIL_ID ContextGraId,
                                     MIL_INT64 InquireType,
                                     void* UserVarPtr);
#if M_MIL_USE_64BIT
// Prototypes for 64 bits OSs

void MFTYPE    MgraControlListInt64 (MIL_ID GraListId,
                                     MIL_INT LabelOrIndex,
                                     MIL_INT SubIndex,
                                     MIL_INT64 ControlType,
                                     MIL_INT64 ControlValue);

void MFTYPE    MgraControlListDouble(MIL_ID GraListId,
                                     MIL_INT LabelOrIndex,
                                     MIL_INT SubIndex,
                                     MIL_INT64 ControlType,
                                     MIL_DOUBLE ControlValue);

void MFTYPE    MgraControlInt64 (MIL_ID ContextGraId,
                                 MIL_INT64 ControlType,
                                 MIL_INT64 ControlValue);

void MFTYPE    MgraControlDouble(MIL_ID ContextGraId,
                                 MIL_INT64 ControlType,
                                 MIL_DOUBLE ControlValue);
#else
// Prototypes for 32 bits OSs

#define MgraControlListDouble MgraControlList
#define MgraControlListInt64  MgraControlList
void MFTYPE    MgraControlList      (MIL_ID GraListId,
                                     MIL_INT LabelOrIndex,
                                     MIL_INT SubIndex,
                                     MIL_INT64 ControlType,
                                     MIL_DOUBLE ControlValue);

#define MgraControlDouble MgraControl
#define MgraControlInt64  MgraControl
void MFTYPE    MgraControl (MIL_ID ContextGraId,
                            MIL_INT64 ControlType,
                            MIL_DOUBLE ControlValue);
#endif

MIL_INT MFTYPE MgraInquireList      (MIL_ID GraListId,
                                     MIL_INT LabelOrIndex,
                                     MIL_INT SubIndex,
                                     MIL_INT64 InquireType,
                                     void* UserVarPtr);

void MFTYPE MgraGetNeighbors        (MIL_ID        GraphListId            ,
                                     MIL_ID        DisplayId              ,
                                     MIL_ID        CalOrImageId           ,
                                     MIL_INT       LabelOrIndex           ,
                                     MIL_INT64     Operation              ,
                                     MIL_DOUBLE    PosX                   ,
                                     MIL_DOUBLE    PosY                   ,
                                     MIL_DOUBLE    Param1                 ,
                                     MIL_DOUBLE    Param2                 ,
                                     MIL_INT64     ResultOutputUnits      ,
                                     MIL_INT       MaxNumNeighbors        ,
                                     MIL_INT*      NumNeighborsFoundPtr   ,
                                     MIL_INT*      ObjectLabelOrIndexArray,
                                     MIL_DOUBLE*   ObjectPosXArray        ,
                                     MIL_DOUBLE*   ObjectPosYArray        ,
                                     MIL_INT64     ControlFlag            );

void MFTYPE MgraGetBoundingBox      (MIL_ID        GraphListId,
                                     MIL_ID        CalOrImageId,
                                     MIL_INT64     LabelOrIndex,
                                     MIL_INT64     Units,
                                     MIL_DOUBLE*   MinXPtr,
                                     MIL_DOUBLE*   MinYPtr,
                                     MIL_DOUBLE*   MaxXPtr,
                                     MIL_DOUBLE*   MaxYPtr,
                                     MIL_INT64     ControlFlag);

void MFTYPE MgraDraw                (MIL_ID        GraListId,
                                     MIL_ID        DestImageId,
                                     MIL_INT64     ControlFlag);

void MFTYPE MgraDrawZoom            (MIL_ID        GraphListId,
                                     MIL_ID        ImageId,
                                     MIL_ID        CalibrationId,
                                     MIL_DOUBLE    ScaleX,
                                     MIL_DOUBLE    ScaleY,
                                     MIL_DOUBLE    OffsetX,
                                     MIL_DOUBLE    OffsetY,
                                     MIL_INT       SizeX,
                                     MIL_INT       SizeY,
                                     MIL_INT64     ControlFlag);

typedef MIL_HOOK_FUNCTION_PTR MIL_GRA_HOOK_FUNCTION_PTR;

void MFTYPE MgraHookFunction        (MIL_ID GraListId,
                                     MIL_INT HookType,
                                     MIL_GRA_HOOK_FUNCTION_PTR HookHandlerPtr,
                                     void* UserDataPtr);

MIL_INT MFTYPE MgraGetHookInfo      (MIL_ID EventId,
                                     MIL_INT64 InfoType,
                                     void* UserVarPtr);

void MFTYPE MgraMessage             (MIL_ID   GraListId,
                                     MIL_INT  MessageType,
                                     MIL_INT  MessageParam1,
                                     MIL_INT  MessageParam2,
                                     MIL_INT  MessageParam3,
                                     MIL_INT  CombinationKeys,
                                     MIL_ID   CalOrImageId,
                                     MIL_DOUBLE DisplayOffX,
                                     MIL_DOUBLE DisplayOffY,
                                     MIL_DOUBLE DisplayScaleX,
                                     MIL_DOUBLE DisplayScaleY,
                                     MIL_INT64 ControlFlag,
                                     MIL_INT* StatusPtr);

void MFTYPE MgraCopy                (MIL_ID         SrcListGraId           ,
                                     MIL_ID         DstListGraId           ,
                                     MIL_INT64      Operation              ,
                                     MIL_INT        InsertLocation         ,
                                     MIL_INT        NumGraphics            ,
                                     const MIL_INT* SrcIndexOrLabelArrayPtr,
                                     MIL_INT*       DstLabelArrayPtr       ,
                                     MIL_INT64      ControlFlag            );

void MFTYPE MgraProcess             (MIL_ID SrcGraphListId,
                                     MIL_ID DstGraphListId,
                                     MIL_ID CalOrImageId,
                                     MIL_INT SrcLabelOrIndex,
                                     MIL_INT DstLabelOrIndex,
                                     MIL_INT64 Operation,
                                     MIL_DOUBLE Param);

#if M_MIL_USE_UNICODE
void MFTYPE MgraSaveW(MIL_CONST_TEXT_PTR  FileName,
                      MIL_ID              GraphListId,
                      MIL_INT64           ControlFlag);

MIL_ID MFTYPE MgraRestoreW(MIL_CONST_TEXT_PTR FileName,
                           MIL_ID             SystemId,
                           MIL_INT64          ControlFlag,
                           MIL_ID*            GraphListIdPtr);

void MFTYPE MgraStreamW(MIL_TEXT_PTR MemPtrOrFileName,
                        MIL_ID       SystemId,
                        MIL_INT64    Operation,
                        MIL_INT64    StreamType,
                        MIL_DOUBLE   Version,
                        MIL_INT64    ControlFlag,
                        MIL_ID*      GraphListIdPtr,
                        MIL_INT*     SizeByteVarPtr);

void MFTYPE MgraSaveA(const char*  FileName,
                      MIL_ID       GraphListId,
                      MIL_INT64    ControlFlag);

MIL_ID MFTYPE MgraRestoreA(const char* FileName,
                           MIL_ID      SystemId,
                           MIL_INT64   ControlFlag,
                           MIL_ID*     GraphListIdPtr);

void MFTYPE MgraStreamA(const char*  MemPtrOrFileName,
                        MIL_ID       SystemId,
                        MIL_INT64    Operation,
                        MIL_INT64    StreamType,
                        MIL_DOUBLE   Version,
                        MIL_INT64    ControlFlag,
                        MIL_ID*      ObjectIdPtr,
                        MIL_INT*     GraphListIdPtr);
#if M_MIL_UNICODE_API
#define MgraSave           MgraSaveW
#define MgraRestore        MgraRestoreW
#define MgraStream         MgraStreamW
#else
#define MgraSave           MgraSaveA
#define MgraRestore        MgraRestoreA
#define MgraStream         MgraStreamA
#endif

#else
void MFTYPE MgraSave(MIL_CONST_TEXT_PTR  FileName,
                     MIL_ID              GraphListId,
                     MIL_INT64           ControlFlag);

MIL_ID MFTYPE MgraRestore(MIL_CONST_TEXT_PTR FileName,
                          MIL_ID             SystemId,
                          MIL_INT64          ControlFlag,
                          MIL_ID*            GraphListIdPtr);

void MFTYPE MgraStream(MIL_TEXT_PTR MemPtrOrFileName,
                       MIL_ID       SystemId,
                       MIL_INT64    Operation,
                       MIL_INT64    StreamType,
                       MIL_DOUBLE   Version,
                       MIL_INT64    ControlFlag,
                       MIL_ID*      GraphListIdPtr,
                       MIL_INT*     SizeByteVarPtr);

#endif

      /* -------------------------------------------------------------- */
      /* -------------------------------------------------------------- */

       /* DRAWING : */

      /* -------------------------------------------------------------- */

/* -------------------------------------------------------------- */
#if M_MIL_USE_64BIT
   void MFTYPE    MgraDotDouble        (MIL_ID ContextGraId,
                                        MIL_ID DstImageBufOrListGraId,
                                        MIL_DOUBLE XPos,
                                        MIL_DOUBLE YPos);

   void MFTYPE    MgraDotDoubleEx      (MIL_ID ContextGraId,
                                        MIL_ID DstImageBufOrListGraId,
                                        MIL_DOUBLE XPos,
                                        MIL_DOUBLE YPos,
                                        MIL_ID NativeCalibration,
                                        MIL_INT64 NativeUnits,
                                        MIL_INT64 ConvertMode);

   void MFTYPE    MgraDotInt64         (MIL_ID ContextGraId,
                                        MIL_ID DstImageBufOrListGraId,
                                        MIL_INT64 XPos,
                                        MIL_INT64 YPos);

   void MFTYPE    MgraDotInt64Ex       (MIL_ID ContextGraId,
                                        MIL_ID DstImageBufOrListGraId,
                                        MIL_INT64 XPos,
                                        MIL_INT64 YPos,
                                        MIL_ID NativeCalibration,
                                        MIL_INT64 NativeUnits,
                                        MIL_INT64 ConvertMode);
#else
   void MFTYPE    MgraDot              (MIL_ID ContextGraId,
                                        MIL_ID DstImageBufOrListGraId,
                                        MIL_DOUBLE XPos,
                                        MIL_DOUBLE YPos);

   void MFTYPE    MgraDotEx            (MIL_ID ContextGraId,
                                        MIL_ID DstImageBufOrListGraId,
                                        MIL_DOUBLE XPos,
                                        MIL_DOUBLE YPos,
                                        MIL_ID NativeCalibration,
                                        MIL_INT64 NativeUnits,
                                        MIL_INT64 ConvertMode);

   #define MgraDotDouble MgraDot
   #define MgraDotInt64  MgraDot
   #define MgraDotDoubleEx MgraDotEx
   #define MgraDotInt64Ex  MgraDotEx  
#endif


/* -------------------------------------------------------------- */
void MFTYPE    MgraDotsDouble       (MIL_ID ContextGraId,
                                     MIL_ID DstImageBufOrListGraId,
                                     MIL_INT  NumberOfDots,
                                     const MIL_DOUBLE *PosXArrayPtr,
                                     const MIL_DOUBLE *PosYArrayPtr,
                                     MIL_INT64 ControlFlag);

void MFTYPE    MgraDotsInt64        (MIL_ID ContextGraId,
                                     MIL_ID DstImageBufOrListGraId,
                                     MIL_INT  NumberOfDots,
                                     const MIL_INT64 *PosXArrayPtr,
                                     const MIL_INT64 *PosYArrayPtr,
                                     MIL_INT64 ControlFlag);

void MFTYPE    MgraDotsInt32        (MIL_ID ContextGraId,
                                     MIL_ID DstImageBufOrListGraId,
                                     MIL_INT  NumberOfDots,
                                     const MIL_INT32 *PosXArrayPtr,
                                     const MIL_INT32 *PosYArrayPtr,
                                     MIL_INT64 ControlFlag);

void MFTYPE    MgraDotsDoubleEx     (MIL_ID ContextGraId,
                                     MIL_ID DstImageBufOrListGraId,
                                     MIL_INT  NumberOfDots,
                                     const MIL_DOUBLE *PosXArrayPtr,
                                     const MIL_DOUBLE *PosYArrayPtr,
                                     MIL_INT64 ControlFlag,
                                     MIL_ID NativeCalibration,
                                     MIL_INT64 NativeUnits,
                                     MIL_INT64 ConvertMode);

void MFTYPE    MgraDotsInt64Ex      (MIL_ID ContextGraId,
                                     MIL_ID DstImageBufOrListGraId,
                                     MIL_INT  NumberOfDots,
                                     const MIL_INT64 *PosXArrayPtr,
                                     const MIL_INT64 *PosYArrayPtr,
                                     MIL_INT64 ControlFlag,
                                     MIL_ID NativeCalibration,
                                     MIL_INT64 NativeUnits,
                                     MIL_INT64 ConvertMode);

void MFTYPE    MgraDotsInt32Ex      (MIL_ID ContextGraId,
                                     MIL_ID DstImageBufOrListGraId,
                                     MIL_INT  NumberOfDots,
                                     const MIL_INT32 *PosXArrayPtr,
                                     const MIL_INT32 *PosYArrayPtr,
                                     MIL_INT64 ControlFlag,
                                     MIL_ID NativeCalibration,
                                     MIL_INT64 NativeUnits,
                                     MIL_INT64 ConvertMode);

/* -------------------------------------------------------------- */
void MFTYPE    MgraSymbolsDouble    (MIL_ID ContextGraId,
                                     MIL_ID DestImageBufIdOrGraListId,
                                     MIL_INT  NumberOfSymbols,
                                     const MIL_DOUBLE *XPosArray,
                                     const MIL_DOUBLE *YPosArray,
                                     const MIL_DOUBLE *AngleArray,
                                     MIL_DOUBLE Dimension,
                                     MIL_INT64 SymbolType,
                                     MIL_INT64 ControlFlag);

void MFTYPE    MgraSymbolsDoubleEx  (MIL_ID ContextGraId,
                                     MIL_ID DestImageBufIdOrGraListId,
                                     MIL_INT  NumberOfSymbols,
                                     const MIL_DOUBLE *XPosArray,
                                     const MIL_DOUBLE *YPosArray,
                                     const MIL_DOUBLE *AngleArray,
                                     MIL_DOUBLE Dimension,
                                     MIL_INT64 SymbolType,
                                     MIL_INT64 ControlFlag,
                                     MIL_INT64 AngleUnits,
                                     MIL_ID NativeCalibration,
                                     MIL_INT64 NativeUnits,
                                     MIL_INT64 ConvertMode);

/* -------------------------------------------------------------- */
#if M_MIL_USE_64BIT
   void MFTYPE    MgraLineDouble       (MIL_ID ContextGraId,
                                        MIL_ID DstImageBufOrListGraId,
                                        MIL_DOUBLE XStart,
                                        MIL_DOUBLE YStart,
                                        MIL_DOUBLE XEnd,
                                        MIL_DOUBLE YEnd);

   void MFTYPE    MgraLineDoubleEx     (MIL_ID ContextGraId,
                                        MIL_ID DstImageBufOrListGraId,
                                        MIL_DOUBLE XStart,
                                        MIL_DOUBLE YStart,
                                        MIL_DOUBLE XEnd,
                                        MIL_DOUBLE YEnd,
                                        MIL_ID NativeCalibration,
                                        MIL_INT64 NativeUnits,
                                        MIL_INT64 ConvertMode);

   void MFTYPE    MgraLineInt64        (MIL_ID ContextGraId,
                                        MIL_ID DstImageBufOrListGraId,
                                        MIL_INT64 XStart,
                                        MIL_INT64 YStart,
                                        MIL_INT64 XEnd,
                                        MIL_INT64 YEnd);

   void MFTYPE    MgraLineInt64Ex      (MIL_ID ContextGraId,
                                        MIL_ID DstImageBufOrListGraId,
                                        MIL_INT64 XStart,
                                        MIL_INT64 YStart,
                                        MIL_INT64 XEnd,
                                        MIL_INT64 YEnd,
                                        MIL_ID NativeCalibration,
                                        MIL_INT64 NativeUnits,
                                        MIL_INT64 ConvertMode);

#else
   void MFTYPE    MgraLine             (MIL_ID ContextGraId,
                                        MIL_ID DstImageBufOrListGraId,
                                        MIL_DOUBLE XStart,
                                        MIL_DOUBLE YStart,
                                        MIL_DOUBLE XEnd,
                                        MIL_DOUBLE YEnd);

   void MFTYPE    MgraLineEx           (MIL_ID ContextGraId,
                                        MIL_ID DstImageBufOrListGraId,
                                        MIL_DOUBLE XStart,
                                        MIL_DOUBLE YStart,
                                        MIL_DOUBLE XEnd,
                                        MIL_DOUBLE YEnd,
                                        MIL_ID NativeCalibration,
                                        MIL_INT64 NativeUnits,
                                        MIL_INT64 ConvertMode);

   #define MgraLineDouble MgraLine
   #define MgraLineInt64  MgraLine
   #define MgraLineDoubleEx MgraLineEx
   #define MgraLineInt64Ex  MgraLineEx
#endif

/* -------------------------------------------------------------- */
void MFTYPE    MgraLinesDouble      (MIL_ID ContextGraId,
                                     MIL_ID DstImageBufOrListGraId,
                                     MIL_INT NumberOfLinesOrVertices,
                                     const MIL_DOUBLE *XPtr,
                                     const MIL_DOUBLE *YPtr,
                                     const MIL_DOUBLE *X2Ptr,
                                     const MIL_DOUBLE *Y2Ptr,
                                     MIL_INT64 ControlFlag);
void MFTYPE    MgraLinesInt64       (MIL_ID ContextGraId,
                                     MIL_ID DstImageBufOrListGraId,
                                     MIL_INT NumberOfLinesOrVertices,
                                     const MIL_INT64 *XPtr,
                                     const MIL_INT64 *YPtr,
                                     const MIL_INT64 *X2Ptr,
                                     const MIL_INT64 *Y2Ptr,
                                     MIL_INT64 ControlFlag);
void MFTYPE    MgraLinesInt32       (MIL_ID ContextGraId,
                                     MIL_ID DstImageBufOrListGraId,
                                     MIL_INT NumberOfLinesOrVertices,
                                     const MIL_INT32 *XPtr,
                                     const MIL_INT32 *YPtr,
                                     const MIL_INT32 *X2Ptr,
                                     const MIL_INT32 *Y2Ptr,
                                     MIL_INT64 ControlFlag);

void MFTYPE    MgraLinesDoubleEx    (MIL_ID ContextGraId,
                                     MIL_ID DstImageBufOrListGraId,
                                     MIL_INT NumberOfLinesOrVertices,
                                     const MIL_DOUBLE *XPtr,
                                     const MIL_DOUBLE *YPtr,
                                     const MIL_DOUBLE *X2Ptr,
                                     const MIL_DOUBLE *Y2Ptr,
                                     MIL_INT64 ControlFlag,
                                     MIL_ID NativeCalibration,
                                     MIL_INT64 NativeUnits,
                                     MIL_INT64 ConvertMode);

void MFTYPE    MgraLinesInt64Ex     (MIL_ID ContextGraId,
                                     MIL_ID DstImageBufOrListGraId,
                                     MIL_INT NumberOfLinesOrVertices,
                                     const MIL_INT64 *XPtr,
                                     const MIL_INT64 *YPtr,
                                     const MIL_INT64 *X2Ptr,
                                     const MIL_INT64 *Y2Ptr,
                                     MIL_INT64 ControlFlag,
                                     MIL_ID NativeCalibration,
                                     MIL_INT64 NativeUnits,
                                     MIL_INT64 ConvertMode);

void MFTYPE    MgraLinesInt32Ex     (MIL_ID ContextGraId,
                                     MIL_ID DstImageBufOrListGraId,
                                     MIL_INT NumberOfLinesOrVertices,
                                     const MIL_INT32 *XPtr,
                                     const MIL_INT32 *YPtr,
                                     const MIL_INT32 *X2Ptr,
                                     const MIL_INT32 *Y2Ptr,
                                     MIL_INT64 ControlFlag,
                                     MIL_ID NativeCalibration,
                                     MIL_INT64 NativeUnits,
                                     MIL_INT64 ConvertMode);

/* -------------------------------------------------------------- */
#if M_MIL_USE_64BIT
   void MFTYPE    MgraArcDouble        (MIL_ID ContextGraId,
                                        MIL_ID DstImageBufOrListGraId,
                                        MIL_DOUBLE XCenter,
                                        MIL_DOUBLE YCenter,
                                        MIL_DOUBLE XRad,
                                        MIL_DOUBLE YRad,
                                        MIL_DOUBLE StartAngle,
                                        MIL_DOUBLE EndAngle);

   void MFTYPE    MgraArcDoubleEx      (MIL_ID ContextGraId,
                                        MIL_ID DstImageBufOrListGraId,
                                        MIL_DOUBLE XCenter,
                                        MIL_DOUBLE YCenter,
                                        MIL_DOUBLE XRad,
                                        MIL_DOUBLE YRad,
                                        MIL_DOUBLE StartAngle,
                                        MIL_DOUBLE EndAngle,
                                        MIL_ID NativeCalibration,
                                        MIL_INT64 NativeUnits,
                                        MIL_INT64 ConvertMode);

   void MFTYPE    MgraArcInt64         (MIL_ID ContextGraId,
                                        MIL_ID DstImageBufOrListGraId,
                                        MIL_INT64 XCenter,
                                        MIL_INT64 YCenter,
                                        MIL_INT64 XRad,
                                        MIL_INT64 YRad,
                                        MIL_DOUBLE StartAngle,
                                        MIL_DOUBLE EndAngle);

   void MFTYPE    MgraArcInt64Ex       (MIL_ID ContextGraId,
                                        MIL_ID DstImageBufOrListGraId,
                                        MIL_INT64 XCenter,
                                        MIL_INT64 YCenter,
                                        MIL_INT64 XRad,
                                        MIL_INT64 YRad,
                                        MIL_DOUBLE StartAngle,
                                        MIL_DOUBLE EndAngle,
                                        MIL_ID NativeCalibration,
                                        MIL_INT64 NativeUnits,
                                        MIL_INT64 ConvertMode);

#else
   void MFTYPE    MgraArc              (MIL_ID ContextGraId,
                                        MIL_ID DstImageBufOrListGraId,
                                        MIL_DOUBLE XCenter,
                                        MIL_DOUBLE YCenter,
                                        MIL_DOUBLE XRad,
                                        MIL_DOUBLE YRad,
                                        MIL_DOUBLE StartAngle,
                                        MIL_DOUBLE EndAngle);

   void MFTYPE    MgraArcEx            (MIL_ID ContextGraId,
                                        MIL_ID DstImageBufOrListGraId,
                                        MIL_DOUBLE XCenter,
                                        MIL_DOUBLE YCenter,
                                        MIL_DOUBLE XRad,
                                        MIL_DOUBLE YRad,
                                        MIL_DOUBLE StartAngle,
                                        MIL_DOUBLE EndAngle,
                                        MIL_ID NativeCalibration,
                                        MIL_INT64 NativeUnits,
                                        MIL_INT64 ConvertMode);

   #define MgraArcDouble MgraArc
   #define MgraArcInt64  MgraArc
   #define MgraArcDoubleEx MgraArcEx
   #define MgraArcInt64Ex  MgraArcEx


#endif

/* -------------------------------------------------------------- */
#if M_MIL_USE_64BIT
   void MFTYPE    MgraArcFillDouble    (MIL_ID ContextGraId,
                                        MIL_ID DstImageBufOrListGraId,
                                        MIL_DOUBLE XCenter,
                                        MIL_DOUBLE YCenter,
                                        MIL_DOUBLE XRad,
                                        MIL_DOUBLE YRad,
                                        MIL_DOUBLE StartAngle,
                                        MIL_DOUBLE EndAngle);

   void MFTYPE    MgraArcFillDoubleEx  (MIL_ID ContextGraId,
                                        MIL_ID DstImageBufOrListGraId,
                                        MIL_DOUBLE XCenter,
                                        MIL_DOUBLE YCenter,
                                        MIL_DOUBLE XRad,
                                        MIL_DOUBLE YRad,
                                        MIL_DOUBLE StartAngle,
                                        MIL_DOUBLE EndAngle,
                                        MIL_ID NativeCalibration,
                                        MIL_INT64 NativeUnits,
                                        MIL_INT64 ConvertMode);

   void MFTYPE    MgraArcFillInt64     (MIL_ID ContextGraId,
                                        MIL_ID DstImageBufOrListGraId,
                                        MIL_INT64 XCenter,
                                        MIL_INT64 YCenter,
                                        MIL_INT64 XRad,
                                        MIL_INT64 YRad,
                                        MIL_DOUBLE StartAngle,
                                        MIL_DOUBLE EndAngle);

   void MFTYPE    MgraArcFillInt64Ex   (MIL_ID ContextGraId,
                                        MIL_ID DstImageBufOrListGraId,
                                        MIL_INT64 XCenter,
                                        MIL_INT64 YCenter,
                                        MIL_INT64 XRad,
                                        MIL_INT64 YRad,
                                        MIL_DOUBLE StartAngle,
                                        MIL_DOUBLE EndAngle,
                                        MIL_ID NativeCalibration,
                                        MIL_INT64 NativeUnits,
                                        MIL_INT64 ConvertMode);

#else
   void MFTYPE    MgraArcFill          (MIL_ID ContextGraId,
                                        MIL_ID DstImageBufOrListGraId,
                                        MIL_DOUBLE XCenter,
                                        MIL_DOUBLE YCenter,
                                        MIL_DOUBLE XRad,
                                        MIL_DOUBLE YRad,
                                        MIL_DOUBLE StartAngle,
                                        MIL_DOUBLE EndAngle);

   void MFTYPE    MgraArcFillEx        (MIL_ID ContextGraId,
                                        MIL_ID DstImageBufOrListGraId,
                                        MIL_DOUBLE XCenter,
                                        MIL_DOUBLE YCenter,
                                        MIL_DOUBLE XRad,
                                        MIL_DOUBLE YRad,
                                        MIL_DOUBLE StartAngle,
                                        MIL_DOUBLE EndAngle,
                                        MIL_ID NativeCalibration,
                                        MIL_INT64 NativeUnits,
                                        MIL_INT64 ConvertMode);

   #define MgraArcFillDouble MgraArcFill
   #define MgraArcFillInt64  MgraArcFill
   #define MgraArcFillDoubleEx MgraArcFillEx
   #define MgraArcFillInt64Ex  MgraArcFillEx
#endif

#if M_MIL_USE_64BIT
   void MFTYPE MgraArcAngleDouble       (MIL_ID ContextGraId,
                                         MIL_ID DstImageBufOrListGraId,
                                         MIL_DOUBLE XCenter,
                                         MIL_DOUBLE YCenter,
                                         MIL_DOUBLE XRad,
                                         MIL_DOUBLE YRad,
                                         MIL_DOUBLE StartAngle,
                                         MIL_DOUBLE EndAngle,
                                         MIL_DOUBLE XAxisAngle,
                                         MIL_INT64 ControlFlag);

   void MFTYPE MgraArcAngleDoubleEx     (MIL_ID ContextGraId,
                                         MIL_ID DstImageBufOrListGraId,
                                         MIL_DOUBLE XCenter,
                                         MIL_DOUBLE YCenter,
                                         MIL_DOUBLE XRad,
                                         MIL_DOUBLE YRad,
                                         MIL_DOUBLE StartAngle,
                                         MIL_DOUBLE EndAngle,
                                         MIL_DOUBLE XAxisAngle,
                                         MIL_INT64 ControlFlag,
                                         MIL_ID NativeCalibration,
                                         MIL_INT64 NativeUnits,
                                         MIL_INT64 ConvertMode);

   void MFTYPE MgraArcAngleInt64        (MIL_ID ContextGraId,
                                         MIL_ID DstImageBufOrListGraId,
                                         MIL_INT64 XCenter,
                                         MIL_INT64 YCenter,
                                         MIL_INT64 XRad,
                                         MIL_INT64 YRad,
                                         MIL_DOUBLE StartAngle,
                                         MIL_DOUBLE EndAngle,
                                         MIL_DOUBLE XAxisAngle,
                                         MIL_INT64 ControlFlag);

   void MFTYPE MgraArcAngleInt64Ex      (MIL_ID ContextGraId,
                                         MIL_ID DstImageBufOrListGraId,
                                         MIL_INT64 XCenter,
                                         MIL_INT64 YCenter,
                                         MIL_INT64 XRad,
                                         MIL_INT64 YRad,
                                         MIL_DOUBLE StartAngle,
                                         MIL_DOUBLE EndAngle,
                                         MIL_DOUBLE XAxisAngle,
                                         MIL_INT64 ControlFlag,
                                         MIL_ID NativeCalibration,
                                         MIL_INT64 NativeUnits,
                                         MIL_INT64 ConvertMode);

#else
   void MFTYPE MgraArcAngle             (MIL_ID ContextGraId,
                                         MIL_ID DstImageBufOrListGraId,
                                         MIL_DOUBLE XCenter,
                                         MIL_DOUBLE YCenter,
                                         MIL_DOUBLE XRad,
                                         MIL_DOUBLE YRad,
                                         MIL_DOUBLE StartAngle,
                                         MIL_DOUBLE EndAngle,
                                         MIL_DOUBLE XAxisAngle,
                                         MIL_INT64 ControlFlag);

   void MFTYPE MgraArcAngleEx           (MIL_ID ContextGraId,
                                         MIL_ID DstImageBufOrListGraId,
                                         MIL_DOUBLE XCenter,
                                         MIL_DOUBLE YCenter,
                                         MIL_DOUBLE XRad,
                                         MIL_DOUBLE YRad,
                                         MIL_DOUBLE StartAngle,
                                         MIL_DOUBLE EndAngle,
                                         MIL_DOUBLE XAxisAngle,
                                         MIL_INT64 ControlFlag,
                                         MIL_ID NativeCalibration,
                                         MIL_INT64 NativeUnits,
                                         MIL_INT64 ConvertMode);
   #define MgraArcAngleDouble MgraArcAngle
   #define MgraArcAngleInt64  MgraArcAngle
   #define MgraArcAngleDoubleEx MgraArcAngleEx
   #define MgraArcAngleInt64Ex  MgraArcAngleEx
#endif

/* -------------------------------------------------------------- */
#if M_MIL_USE_64BIT
   void MFTYPE MgraRingSectorDouble     (MIL_ID     ContextGraId             ,
                                         MIL_ID     DestImageBufIdOrGraListId,
                                         MIL_DOUBLE XCenter                  ,
                                         MIL_DOUBLE YCenter                  ,
                                         MIL_DOUBLE XInnerRadius             ,
                                         MIL_DOUBLE YInnerRadius             ,
                                         MIL_DOUBLE XOuterRadius             ,
                                         MIL_DOUBLE YOuterRadius             ,
                                         MIL_DOUBLE StartAngle               ,
                                         MIL_DOUBLE EndAngle                 ,
                                         MIL_DOUBLE XAxisAngle               ,
                                         MIL_INT64  ControlFlag              );

   void MFTYPE MgraRingSectorDoubleEx   (MIL_ID     ContextGraId             ,
                                         MIL_ID     DestImageBufIdOrGraListId,
                                         MIL_DOUBLE XCenter                  ,
                                         MIL_DOUBLE YCenter                  ,
                                         MIL_DOUBLE XInnerRadius             ,
                                         MIL_DOUBLE YInnerRadius             ,
                                         MIL_DOUBLE XOuterRadius             ,
                                         MIL_DOUBLE YOuterRadius             ,
                                         MIL_DOUBLE StartAngle               ,
                                         MIL_DOUBLE EndAngle                 ,
                                         MIL_DOUBLE XAxisAngle               ,
                                         MIL_INT64  ControlFlag              ,
                                         MIL_ID     NativeCalibration        ,
                                         MIL_INT64  NativeUnits              ,
                                         MIL_INT64  ConvertMode          );

   void MFTYPE MgraRingSectorInt64      (MIL_ID     ContextGraId             ,
                                         MIL_ID     DestImageBufIdOrGraListId,
                                         MIL_INT64  XCenter                  ,
                                         MIL_INT64  YCenter                  ,
                                         MIL_INT64  XInnerRadius             ,
                                         MIL_INT64  YInnerRadius             ,
                                         MIL_INT64  XOuterRadius             ,
                                         MIL_INT64  YOuterRadius             ,
                                         MIL_DOUBLE StartAngle               ,
                                         MIL_DOUBLE EndAngle                 ,
                                         MIL_DOUBLE XAxisAngle               ,
                                         MIL_INT64  ControlFlag              );

   void MFTYPE MgraRingSectorInt64Ex    (MIL_ID     ContextGraId             ,
                                         MIL_ID     DestImageBufIdOrGraListId,
                                         MIL_INT64  XCenter                  ,
                                         MIL_INT64  YCenter                  ,
                                         MIL_INT64  XInnerRadius             ,
                                         MIL_INT64  YInnerRadius             ,
                                         MIL_INT64  XOuterRadius             ,
                                         MIL_INT64  YOuterRadius             ,
                                         MIL_DOUBLE StartAngle               ,
                                         MIL_DOUBLE EndAngle                 ,
                                         MIL_DOUBLE XAxisAngle               ,
                                         MIL_INT64  ControlFlag              ,
                                         MIL_ID     NativeCalibration        ,
                                         MIL_INT64  NativeUnits              ,
                                         MIL_INT64  ConvertMode          );

#else
   void MFTYPE MgraRingSector           (MIL_ID     ContextGraId             ,
                                         MIL_ID     DestImageBufIdOrGraListId,
                                         MIL_DOUBLE XCenter                  ,
                                         MIL_DOUBLE YCenter                  ,
                                         MIL_DOUBLE XInnerRadius             ,
                                         MIL_DOUBLE YInnerRadius             ,
                                         MIL_DOUBLE XOuterRadius             ,
                                         MIL_DOUBLE YOuterRadius             ,
                                         MIL_DOUBLE StartAngle               ,
                                         MIL_DOUBLE EndAngle                 ,
                                         MIL_DOUBLE XAxisAngle               ,
                                         MIL_INT64  ControlFlag              );

   void MFTYPE MgraRingSectorEx         (MIL_ID     ContextGraId             ,
                                         MIL_ID     DestImageBufIdOrGraListId,
                                         MIL_DOUBLE XCenter                  ,
                                         MIL_DOUBLE YCenter                  ,
                                         MIL_DOUBLE XInnerRadius             ,
                                         MIL_DOUBLE YInnerRadius             ,
                                         MIL_DOUBLE XOuterRadius             ,
                                         MIL_DOUBLE YOuterRadius             ,
                                         MIL_DOUBLE StartAngle               ,
                                         MIL_DOUBLE EndAngle                 ,
                                         MIL_DOUBLE XAxisAngle               ,
                                         MIL_INT64  ControlFlag              ,
                                         MIL_ID     NativeCalibration        ,
                                         MIL_INT64  NativeUnits              ,
                                         MIL_INT64  ConvertMode          );
   #define MgraRingSectorDouble   MgraRingSector
   #define MgraRingSectorInt64    MgraRingSector
   #define MgraRingSectorDoubleEx MgraRingSectorEx
   #define MgraRingSectorInt64Ex  MgraRingSectorEx
#endif

/* -------------------------------------------------------------- */
#if M_MIL_USE_64BIT
   void MFTYPE    MgraRectDouble       (MIL_ID ContextGraId,
                                        MIL_ID DstImageBufOrListGraId,
                                        MIL_DOUBLE XStart,
                                        MIL_DOUBLE YStart,
                                        MIL_DOUBLE XEnd,
                                        MIL_DOUBLE YEnd);

   void MFTYPE    MgraRectDoubleEx     (MIL_ID ContextGraId,
                                        MIL_ID DstImageBufOrListGraId,
                                        MIL_DOUBLE XStart,
                                        MIL_DOUBLE YStart,
                                        MIL_DOUBLE XEnd,
                                        MIL_DOUBLE YEnd,
                                        MIL_ID NativeCalibration,
                                        MIL_INT64 NativeUnits,
                                        MIL_INT64 ConvertMode);

   void MFTYPE    MgraRectInt64        (MIL_ID ContextGraId,
                                        MIL_ID DstImageBufOrListGraId,
                                        MIL_INT64 XStart,
                                        MIL_INT64 YStart,
                                        MIL_INT64 XEnd,
                                        MIL_INT64 YEnd);

   void MFTYPE    MgraRectInt64Ex      (MIL_ID ContextGraId,
                                        MIL_ID DstImageBufOrListGraId,
                                        MIL_INT64 XStart,
                                        MIL_INT64 YStart,
                                        MIL_INT64 XEnd,
                                        MIL_INT64 YEnd,
                                        MIL_ID NativeCalibration,
                                        MIL_INT64 NativeUnits,
                                        MIL_INT64 ConvertMode);

#else
   void MFTYPE    MgraRect             (MIL_ID ContextGraId,
                                        MIL_ID DstImageBufOrListGraId,
                                        MIL_DOUBLE XStart,
                                        MIL_DOUBLE YStart,
                                        MIL_DOUBLE XEnd,
                                        MIL_DOUBLE YEnd);

   void MFTYPE    MgraRectEx           (MIL_ID ContextGraId,
                                        MIL_ID DstImageBufOrListGraId,
                                        MIL_DOUBLE XStart,
                                        MIL_DOUBLE YStart,
                                        MIL_DOUBLE XEnd,
                                        MIL_DOUBLE YEnd,
                                        MIL_ID NativeCalibration,
                                        MIL_INT64 NativeUnits,
                                        MIL_INT64 ConvertMode);
   #define MgraRectDouble MgraRect
   #define MgraRectInt64  MgraRect
   #define MgraRectDoubleEx MgraRectEx
   #define MgraRectInt64Ex  MgraRectEx
#endif

/* -------------------------------------------------------------- */
#if M_MIL_USE_64BIT
   void MFTYPE    MgraRectFillDouble   (MIL_ID ContextGraId,
                                        MIL_ID DstImageBufOrListGraId,
                                        MIL_DOUBLE XStart,
                                        MIL_DOUBLE YStart,
                                        MIL_DOUBLE XEnd,
                                        MIL_DOUBLE YEnd);

   void MFTYPE    MgraRectFillDoubleEx (MIL_ID ContextGraId,
                                        MIL_ID DstImageBufOrListGraId,
                                        MIL_DOUBLE XStart,
                                        MIL_DOUBLE YStart,
                                        MIL_DOUBLE XEnd,
                                        MIL_DOUBLE YEnd,
                                        MIL_ID NativeCalibration,
                                        MIL_INT64 NativeUnits,
                                        MIL_INT64 ConvertMode);

   void MFTYPE    MgraRectFillInt64    (MIL_ID ContextGraId,
                                        MIL_ID DstImageBufOrListGraId,
                                        MIL_INT64 XStart,
                                        MIL_INT64 YStart,
                                        MIL_INT64 XEnd,
                                        MIL_INT64 YEnd);

   void MFTYPE    MgraRectFillInt64Ex  (MIL_ID ContextGraId,
                                        MIL_ID DstImageBufOrListGraId,
                                        MIL_INT64 XStart,
                                        MIL_INT64 YStart,
                                        MIL_INT64 XEnd,
                                        MIL_INT64 YEnd,
                                        MIL_ID NativeCalibration,
                                        MIL_INT64 NativeUnits,
                                        MIL_INT64 ConvertMode);

#else
   void MFTYPE    MgraRectFill         (MIL_ID ContextGraId,
                                        MIL_ID DstImageBufOrListGraId,
                                        MIL_DOUBLE XStart,
                                        MIL_DOUBLE YStart,
                                        MIL_DOUBLE XEnd,
                                        MIL_DOUBLE YEnd);

   void MFTYPE    MgraRectFillEx       (MIL_ID ContextGraId,
                                        MIL_ID DstImageBufOrListGraId,
                                        MIL_DOUBLE XStart,
                                        MIL_DOUBLE YStart,
                                        MIL_DOUBLE XEnd,
                                        MIL_DOUBLE YEnd,
                                        MIL_ID NativeCalibration,
                                        MIL_INT64 NativeUnits,
                                        MIL_INT64 ConvertMode);
   #define MgraRectFillDouble MgraRectFill
   #define MgraRectFillInt64  MgraRectFill
   #define MgraRectFillDoubleEx MgraRectFillEx
   #define MgraRectFillInt64Ex  MgraRectFillEx
#endif


#if M_MIL_USE_64BIT
   void MFTYPE MgraRectAngleDouble     (MIL_ID ContextGraId,
                                        MIL_ID DestImageBufIdOrGraListId,
                                        MIL_DOUBLE XPos,
                                        MIL_DOUBLE YPos,
                                        MIL_DOUBLE Width,
                                        MIL_DOUBLE Height,
                                        MIL_DOUBLE Angle,
                                        MIL_INT64 ControlFlag);

   void MFTYPE MgraRectAngleDoubleEx   (MIL_ID ContextGraId,
                                        MIL_ID DestImageBufIdOrGraListId,
                                        MIL_DOUBLE XPos,
                                        MIL_DOUBLE YPos,
                                        MIL_DOUBLE Width,
                                        MIL_DOUBLE Height,
                                        MIL_DOUBLE Angle,
                                        MIL_INT64 ControlFlag,
                                        MIL_ID NativeCalibration,
                                        MIL_INT64 NativeUnits,
                                        MIL_INT64 ConvertMode);

   void MFTYPE MgraRectAngleInt64      (MIL_ID ContextGraId,
                                        MIL_ID DestImageBufIdOrGraListId,
                                        MIL_INT64 XPos,
                                        MIL_INT64 YPos,
                                        MIL_INT64 Width,
                                        MIL_INT64 Height,
                                        MIL_DOUBLE Angle,
                                        MIL_INT64 ControlFlag);

   void MFTYPE MgraRectAngleInt64Ex    (MIL_ID ContextGraId,
                                        MIL_ID DestImageBufIdOrGraListId,
                                        MIL_INT64 XPos,
                                        MIL_INT64 YPos,
                                        MIL_INT64 Width,
                                        MIL_INT64 Height,
                                        MIL_DOUBLE Angle,
                                        MIL_INT64 ControlFlag,
                                        MIL_ID NativeCalibration,
                                        MIL_INT64 NativeUnits,
                                        MIL_INT64 ConvertMode);

#else
   void MFTYPE MgraRectAngle           (MIL_ID ContextGraId,
                                        MIL_ID DestImageBufIdOrGraListId,
                                        MIL_DOUBLE XPos,
                                        MIL_DOUBLE YPos,
                                        MIL_DOUBLE Width,
                                        MIL_DOUBLE Height,
                                        MIL_DOUBLE Angle,
                                        MIL_INT64 ControlFlag);

   void MFTYPE MgraRectAngleEx         (MIL_ID ContextGraId,
                                        MIL_ID DestImageBufIdOrGraListId,
                                        MIL_DOUBLE XPos,
                                        MIL_DOUBLE YPos,
                                        MIL_DOUBLE Width,
                                        MIL_DOUBLE Height,
                                        MIL_DOUBLE Angle,
                                        MIL_INT64 ControlFlag,
                                        MIL_ID NativeCalibration,
                                        MIL_INT64 NativeUnits,
                                        MIL_INT64 ConvertMode);
   #define MgraRectAngleDouble MgraRectAngle
   #define MgraRectAngleInt64  MgraRectAngle
   #define MgraRectAngleDoubleEx MgraRectAngleEx
   #define MgraRectAngleInt64Ex  MgraRectAngleEx
#endif

/* -------------------------------------------------------------- */
#if M_MIL_USE_64BIT
   void MFTYPE    MgraFillDouble       (MIL_ID ContextGraId,
                                        MIL_ID DstImageBufId,
                                        MIL_DOUBLE XStart,
                                        MIL_DOUBLE YStart);

   void MFTYPE    MgraFillDoubleEx     (MIL_ID ContextGraId,
                                        MIL_ID DstImageBufId,
                                        MIL_DOUBLE XStart,
                                        MIL_DOUBLE YStart,
                                        MIL_ID NativeCalibration,
                                        MIL_INT64 NativeUnits,
                                        MIL_INT64 ConvertMode);

   void MFTYPE    MgraFillInt64        (MIL_ID ContextGraId,
                                        MIL_ID DstImageBufId,
                                        MIL_INT64 XStart,
                                        MIL_INT64 YStart);

   void MFTYPE    MgraFillInt64Ex      (MIL_ID ContextGraId,
                                        MIL_ID DstImageBufId,
                                        MIL_INT64 XStart,
                                        MIL_INT64 YStart,
                                        MIL_ID NativeCalibration,
                                        MIL_INT64 NativeUnits,
                                        MIL_INT64 ConvertMode);

#else
   void MFTYPE    MgraFill             (MIL_ID ContextGraId,
                                        MIL_ID DstImageBufId,
                                        MIL_DOUBLE XStart,
                                        MIL_DOUBLE YStart);

   void MFTYPE    MgraFillEx           (MIL_ID ContextGraId,
                                        MIL_ID DstImageBufId,
                                        MIL_DOUBLE XStart,
                                        MIL_DOUBLE YStart,
                                        MIL_ID NativeCalibration,
                                        MIL_INT64 NativeUnits,
                                        MIL_INT64 ConvertMode);
   #define MgraFillDouble MgraFill
   #define MgraFillInt64  MgraFill
   #define MgraFillDoubleEx MgraFillEx
   #define MgraFillInt64Ex  MgraFillEx
#endif

/* -------------------------------------------------------------- */
void MFTYPE    MgraClear            (MIL_ID ContextGraId,
                                     MIL_ID DstImageBufOrListGraId);

/* -------------------------------------------------------------- */
#if M_MIL_USE_64BIT

   #if M_MIL_USE_UNICODE
      void MFTYPE    MgraTextADouble      (MIL_ID ContextGraId,
                                           MIL_ID DstImageBufOrListGraId,
                                           MIL_DOUBLE XStart,
                                           MIL_DOUBLE YStart,
                                           const char* StringPtr);

      void MFTYPE    MgraTextADoubleEx    (MIL_ID ContextGraId,
                                           MIL_ID DstImageBufOrListGraId,
                                           MIL_DOUBLE XStart,
                                           MIL_DOUBLE YStart,
                                           const char* StringPtr,
                                           MIL_ID NativeCalibration,
                                           MIL_INT64 NativeUnits,
                                           MIL_INT64 ConvertMode);

      void MFTYPE    MgraTextAInt64       (MIL_ID ContextGraId,
                                           MIL_ID DstImageBufOrListGraId,
                                           MIL_INT64 XStart,
                                           MIL_INT64 YStart,
                                           const char* StringPtr);

      void MFTYPE    MgraTextAInt64Ex     (MIL_ID ContextGraId,
                                           MIL_ID DstImageBufOrListGraId,
                                           MIL_INT64 XStart,
                                           MIL_INT64 YStart,
                                           const char* StringPtr,
                                           MIL_ID NativeCalibration,
                                           MIL_INT64 NativeUnits,
                                           MIL_INT64 ConvertMode);

      void MFTYPE    MgraTextWDouble      (MIL_ID ContextGraId,
                                           MIL_ID DstImageBufOrListGraId,
                                           MIL_DOUBLE XStart,
                                           MIL_DOUBLE YStart,
                                           MIL_CONST_TEXT_PTR StringPtr);

      void MFTYPE    MgraTextWDoubleEx    (MIL_ID ContextGraId,
                                           MIL_ID DstImageBufOrListGraId,
                                           MIL_DOUBLE XStart,
                                           MIL_DOUBLE YStart,
                                           MIL_CONST_TEXT_PTR StringPtr,
                                           MIL_ID NativeCalibration,
                                           MIL_INT64 NativeUnits,
                                           MIL_INT64 ConvertMode);

      void MFTYPE    MgraTextWInt64       (MIL_ID ContextGraId,
                                           MIL_ID DstImageBufOrListGraId,
                                           MIL_INT64 XStart,
                                           MIL_INT64 YStart,
                                           MIL_CONST_TEXT_PTR StringPtr);

      void MFTYPE    MgraTextWInt64Ex     (MIL_ID ContextGraId,
                                           MIL_ID DstImageBufOrListGraId,
                                           MIL_INT64 XStart,
                                           MIL_INT64 YStart,
                                           MIL_CONST_TEXT_PTR StringPtr,
                                           MIL_ID NativeCalibration,
                                           MIL_INT64 NativeUnits,
                                           MIL_INT64 ConvertMode);


      // MgraTextW and MgraTextA themselves will be defined at the end of the
      // file in C++ function (or #define in C) mapping to on of the 4 flavors
      // of MgraText.
      #if M_MIL_UNICODE_API
         #define MgraText MgraTextW
         #define MgraTextEx MgraTextWEx
      #else
         #define MgraText MgraTextA
         #define MgraTextEx MgraTextAEx
      #endif

   #else // #if M_MIL_USE_UNICODE
      void MFTYPE    MgraTextDouble       (MIL_ID ContextGraId,
                                           MIL_ID DstImageBufOrListGraId,
                                           MIL_DOUBLE XStart,
                                           MIL_DOUBLE YStart,
                                           MIL_CONST_TEXT_PTR StringPtr);

      void MFTYPE    MgraTextDoubleEx     (MIL_ID ContextGraId,
                                           MIL_ID DstImageBufOrListGraId,
                                           MIL_DOUBLE XStart,
                                           MIL_DOUBLE YStart,
                                           MIL_CONST_TEXT_PTR StringPtr,
                                           MIL_ID NativeCalibration,
                                           MIL_INT64 NativeUnits,
                                           MIL_INT64 ConvertMode);

      void MFTYPE    MgraTextInt64        (MIL_ID ContextGraId,
                                           MIL_ID DstImageBufOrListGraId,
                                           MIL_INT64 XStart,
                                           MIL_INT64 YStart,
                                           MIL_CONST_TEXT_PTR StringPtr);

      void MFTYPE    MgraTextInt64Ex      (MIL_ID ContextGraId,
                                           MIL_ID DstImageBufOrListGraId,
                                           MIL_INT64 XStart,
                                           MIL_INT64 YStart,
                                           MIL_CONST_TEXT_PTR StringPtr,
                                           MIL_ID NativeCalibration,
                                           MIL_INT64 NativeUnits,
                                           MIL_INT64 ConvertMode);

      #define MgraTextA MgraText
      #define MgraTextAEx MgraTextEx

   #endif // #if M_MIL_USE_UNICODE #else

#else // #if M_MIL_USE_64BIT

   #if M_MIL_USE_UNICODE
      void MFTYPE    MgraTextA            (MIL_ID ContextGraId,
                                           MIL_ID DstImageBufOrListGraId,
                                           MIL_DOUBLE XStart,
                                           MIL_DOUBLE YStart,
                                           const char* StringPtr);

      void MFTYPE    MgraTextAEx          (MIL_ID ContextGraId,
                                           MIL_ID DstImageBufOrListGraId,
                                           MIL_DOUBLE XStart,
                                           MIL_DOUBLE YStart,
                                           const char* StringPtr,
                                           MIL_ID NativeCalibration,
                                           MIL_INT64 NativeUnits,
                                           MIL_INT64 ConvertMode);

      void MFTYPE    MgraTextW            (MIL_ID ContextGraId,
                                           MIL_ID DstImageBufOrListGraId,
                                           MIL_DOUBLE XStart,
                                           MIL_DOUBLE YStart,
                                           MIL_CONST_TEXT_PTR StringPtr);

      void MFTYPE    MgraTextWEx          (MIL_ID ContextGraId,
                                           MIL_ID DstImageBufOrListGraId,
                                           MIL_DOUBLE XStart,
                                           MIL_DOUBLE YStart,
                                           MIL_CONST_TEXT_PTR StringPtr,
                                           MIL_ID NativeCalibration,
                                           MIL_INT64 NativeUnits,
                                           MIL_INT64 ConvertMode);

      #if M_MIL_UNICODE_API
         #define MgraText MgraTextW
         #define MgraTextEx MgraTextWEx
      #else
         #define MgraText MgraTextA
         #define MgraTextEx MgraTextAEx
      #endif

   #else // #if M_MIL_USE_UNICODE
      void MFTYPE    MgraText             (MIL_ID ContextGraId,
                                           MIL_ID DstImageBufOrListGraId,
                                           MIL_DOUBLE XStart,
                                           MIL_DOUBLE YStart,
                                           MIL_CONST_TEXT_PTR StringPtr);

      void MFTYPE    MgraTextEx           (MIL_ID ContextGraId,
                                           MIL_ID DstImageBufOrListGraId,
                                           MIL_DOUBLE XStart,
                                           MIL_DOUBLE YStart,
                                           MIL_CONST_TEXT_PTR StringPtr,
                                           MIL_ID NativeCalibration,
                                           MIL_INT64 NativeUnits,
                                           MIL_INT64 ConvertMode);

      #define MgraTextA MgraText
      #define MgraTextAEx MgraTextEx
   #endif // #if M_MIL_USE_UNICODE #else

#endif // #if M_MIL_USE_64BIT #else

void MFTYPE    MgraImage            (MIL_ID ContextGraId,
                                     MIL_ID ImageId,
                                     MIL_ID DestImageId,
                                     MIL_INT64 ControlFlag);

void MFTYPE    MgraImageEx          (MIL_ID ContextGraId,
                                     MIL_ID ImageId,
                                     MIL_ID DestImageId,
                                     MIL_INT64 ControlFlag,
                                     MIL_ID NativeCalibration,
                                     MIL_INT64 NativeUnits,
                                     MIL_INT64 ConvertMode);


#define M_NO_ERROR_TO_REPORT      0
#define M_ERROR_ALREADY_REPORTED  1
typedef MIL_INT (MFTYPE* MIL_GRA_CUSTOM_FUNCTION_PTR)(MIL_ID        ContextGraId,
                                                      MIL_ID        DestImageId,
                                                      MIL_INT64     DrawType,
                                                      const void*   DrawData,
                                                      const MIL_INT SizeX,
                                                      const MIL_INT SizeY,
                                                      MIL_ID        NativeCalibration,
                                                      MIL_INT64     NativeConversion,
                                                      MIL_INT64     ConvertMode,
                                                      MIL_INT64     ControlFlag);

void MFTYPE    MgraCustom           (MIL_ID ContextGraId,
                                     MIL_ID DestImageOrGraphicListId,
                                     MIL_INT64 DrawType,
                                     MIL_GRA_CUSTOM_FUNCTION_PTR CustomDrawPtr,
                                     const void* DrawData,
                                     MIL_INT DrawDataSizeByte,
                                     MIL_INT64 ControlFlag);

void MFTYPE    MgraCustomEx         (MIL_ID ContextGraId,
                                     MIL_ID DestImageOrGraphicListId,
                                     MIL_INT64 DrawType,
                                     MIL_GRA_CUSTOM_FUNCTION_PTR CustomDrawPtr,
                                     const void* DrawData,
                                     MIL_INT DrawDataSizeByte,
                                     MIL_INT64 ControlFlag,
                                     MIL_ID NativeCalibration,
                                     MIL_INT64 NativeUnits,
                                     MIL_INT64 ConvertMode);

void MFTYPE    MgraInteractive      (MIL_ID ContextGraId,
                                     MIL_ID ListGraId,
                                     MIL_INT64 GraphicType,
                                     MIL_INT64 InitFlag,
                                     MIL_INT64 ControlFlag);

/* -------------------------------------------------------------- */


      /* -------------------------------------------------------------- */

/***************************************************************************/
/* SEQUENCE MODULE:                                                        */
/***************************************************************************/

      /* -------------------------------------------------------------- */

typedef MIL_HOOK_FUNCTION_PTR MIL_SEQ_HOOK_FUNCTION_PTR;

#if !M_MIL_USE_LINUX || M_COMPILING_MILDLL

MIL_ID MFTYPE MseqAlloc(MIL_ID SystemId,
                        MIL_INT64 SequenceType,
                        MIL_INT64 Operation,
                        MIL_UINT32 OutputFormat,
                        MIL_INT64 InitFlag,
                        MIL_ID* ContextSeqIdPtr);

void MFTYPE MseqFree(MIL_ID ContextSeqId);

void MFTYPE MseqDefine(MIL_ID ContextSeqId,
                       MIL_INT SequenceIndex,
                       MIL_INT64 SequenceType,
                       const void *Param1Ptr,
                       MIL_DOUBLE Param2);

#if M_MIL_USE_64BIT
// Prototypes for 64 bits OSs

void MFTYPE MseqControlInt64( MIL_ID ContextSeqId,
                              MIL_INT SequenceIndex,
                              MIL_INT64 ControlType,
                              MIL_INT64 ControlValue);

void MFTYPE MseqControlDouble(MIL_ID ContextSeqId,
                              MIL_INT SequenceIndex,
                              MIL_INT64 ControlType,
                              MIL_DOUBLE ControlValue);
#else
// Prototypes for 32 bits OSs

#define MseqControlDouble MseqControl
#define MseqControlInt64  MseqControl
void MFTYPE MseqControl(   MIL_ID ContextSeqId,
                           MIL_INT SequenceIndex,
                           MIL_INT64 ControlType,
                           MIL_DOUBLE ControlValue);
#endif

MIL_INT MFTYPE MseqInquire( MIL_ID ContextSeqId,
                              MIL_INT SequenceIndex,
                              MIL_INT64 InquireType,
                              void* UserVarPtr);

void MFTYPE MseqHookFunction( MIL_ID ContextSeqId,
                              MIL_INT HookType,
                              MIL_SEQ_HOOK_FUNCTION_PTR HookHandlerPtr,
                              void* UserDataPtr);

MIL_INT MFTYPE MseqGetHookInfo(  MIL_ID EventId,
                              MIL_INT64 InfoType,
                              void* UserVarPtr);

void MFTYPE MseqFeed(MIL_ID ContextSeqId,
                     MIL_ID ImageBufId,
                     MIL_INT64 InitFlag);

void MFTYPE MseqProcess(MIL_ID ContextSeqId,
                        MIL_INT64 Command,
                        MIL_INT64 CommandFlag);
#endif 
      /* -------------------------------------------------------------- */

/***************************************************************************/
/* DATA GENERATION MODULE:                                                 */
/***************************************************************************/

      /* -------------------------------------------------------------- */
      /* -------------------------------------------------------------- */

      /* DATA BUFFERS: */

      /* -------------------------------------------------------------- */

void MFTYPE    MgenLutRamp          (MIL_ID LutBufId,
                                     MIL_INT StartIndex,
                                     MIL_DOUBLE StartValue,
                                     MIL_INT EndIndex,
                                     MIL_DOUBLE EndValue);

void MFTYPE    MgenLutFunction      (MIL_ID LutBufId,
                                     MIL_INT Func,
                                     MIL_DOUBLE a,
                                     MIL_DOUBLE b,
                                     MIL_DOUBLE c,
                                     MIL_INT StartIndex,
                                     MIL_DOUBLE StartXValue,
                                     MIL_INT EndIndex);

void MFTYPE    MgenWarpParameter    (MIL_ID     SrcLutOrArrayBufId,
                                     MIL_ID     DstXLutOrArrayBufId,
                                     MIL_ID     DstYLutBufId,
                                     MIL_INT    OperationMode,
                                     MIL_INT    Transform,
                                     MIL_DOUBLE Val1,
                                     MIL_DOUBLE Val2);

      /* -------------------------------------------------------------- */
      /* -------------------------------------------------------------- */

/***************************************************************************/
/* DATA BUFFERS MODULE:                                                    */
/***************************************************************************/

      /* -------------------------------------------------------------- */
      /* -------------------------------------------------------------- */

      /* CREATION: */

      /* -------------------------------------------------------------- */

MIL_ID MFTYPE  MbufAlloc1d          (MIL_ID SystemId,
                                     MIL_INT SizeX,
                                     MIL_INT Type,
                                     MIL_INT64 Attribute,
                                     MIL_ID* BufIdPtr);

MIL_ID MFTYPE  MbufAlloc2d          (MIL_ID SystemId,
                                     MIL_INT SizeX,
                                     MIL_INT SizeY,
                                     MIL_INT Type,
                                     MIL_INT64 Attribute,
                                     MIL_ID* BufIdPtr);

MIL_ID MFTYPE  MbufAllocColor       (MIL_ID SystemId,
                                     MIL_INT SizeBand,
                                     MIL_INT SizeX,
                                     MIL_INT SizeY,
                                     MIL_INT Type,
                                     MIL_INT64 Attribute,
                                     MIL_ID* BufIdPtr);

MIL_ID MFTYPE  MbufChild1d          (MIL_ID ParentBufId,
                                     MIL_INT OffX,
                                     MIL_INT SizeX,
                                     MIL_ID* BufIdPtr);

MIL_ID MFTYPE  MbufChild2d          (MIL_ID  ParentBufId,
                                     MIL_INT OffX,
                                     MIL_INT OffY,
                                     MIL_INT SizeX,
                                     MIL_INT SizeY,
                                     MIL_ID* BufIdPtr);

MIL_ID MFTYPE  MbufChildColor       (MIL_ID  ParentBufId,
                                     MIL_INT Band,
                                     MIL_ID* BufIdPtr);

MIL_ID MFTYPE  MbufChildColor2d     (MIL_ID  ParentBufId,
                                     MIL_INT Band,
                                     MIL_INT OffX,
                                     MIL_INT OffY,
                                     MIL_INT SizeX,
                                     MIL_INT SizeY,
                                     MIL_ID* BufIdPtr);

void  MFTYPE  MbufChildMove         (MIL_ID  BufId,
                                     MIL_INT OffsetX,
                                     MIL_INT OffsetY,
                                     MIL_INT SizeX,
                                     MIL_INT SizeY,
                                     MIL_INT64 ControlFlag);

MIL_ID MFTYPE  MbufCreateColor      (MIL_ID SystemId,
                                     MIL_INT SizeBand,
                                     MIL_INT SizeX,
                                     MIL_INT SizeY,
                                     MIL_INT Type,
                                     MIL_INT64 Attribute,
                                     MIL_INT64 ControlFlag,
                                     MIL_INT Pitch,
                                     void* *ArrayOfDataPtr,
                                     MIL_ID* BufIdPtr);

MIL_ID MFTYPE  MbufClone            (MIL_ID SrcBufId,
                                     MIL_ID SysId,
                                     MIL_INT SizeX,
                                     MIL_INT SizeY,
                                     MIL_INT Type,
                                     MIL_INT64 Attribute,
                                     MIL_INT64 ControlFlag,
                                     MIL_ID* VarBufIdPtr);

#ifdef M_USE_MBUFBLIT
/*void MFTYPE  MbufBlit                (MIL_ID  SrcBufId,
                                              MIL_ID  DstBufId,
                                              LPMBLTDESC pBltDesc);*/
#define MbufBlit(srcID,dstID,pBltDesc)        MbufTransfer(srcID,                \
                                                           dstID,                \
                                                           0,                    \
                                                           0,                    \
                                                           M_DEFAULT,            \
                                                           M_DEFAULT,            \
                                                           M_DEFAULT,            \
                                                           0,                    \
                                                           0,                    \
                                                           M_DEFAULT,            \
                                                           M_DEFAULT,            \
                                                           M_DEFAULT,            \
                                                           M_DEFAULT,            \
                                                           M_MTX0_METHOD,        \
                                                           M_DEFAULT,            \
                                                           pBltDesc)
#endif


MIL_ID MFTYPE  MbufCreate2dFunc      (MIL_ID SystemId,
                                      MIL_INT SizeX,
                                      MIL_INT SizeY,
                                      MIL_INT Type,
                                      MIL_INT64 Attribute,
                                      MIL_INT64 ControlFlag,
                                      MIL_INT Pitch,
                                      MIL_DATA_PTR DataPtr,
                                      MIL_ID* BufIdPtr);

#if M_MIL_USE_64BIT
#define CAST_TO_64BIT_WITHOUT_SIGN_EXTENSION(X) ((MIL_DATA_PTR)(X))
#else
#define CAST_TO_64BIT_WITHOUT_SIGN_EXTENSION(X) ((sizeof(X) == sizeof(MIL_DATA_PTR))? ((MIL_DATA_PTR)(X)):((MIL_DATA_PTR)((MIL_ADDR)(X)))) 
#endif

#define MbufCreate2d(SystemId, SizeX, SizeY, Type, Attribute, ControlFlag, Pitch, DataPtr, IdVarPtr) \
        MbufCreate2dFunc(SystemId, SizeX, SizeY, Type, Attribute, ControlFlag, Pitch, CAST_TO_64BIT_WITHOUT_SIGN_EXTENSION(DataPtr), IdVarPtr)

void MFTYPE    MbufFree             (MIL_ID BufId);


      /* -------------------------------------------------------------- */
      /* -------------------------------------------------------------- */

      /* ACCESS: */

      /* -------------------------------------------------------------- */

void MFTYPE    MbufClearDouble      (MIL_ID DstImageBufId,
                                     MIL_DOUBLE Color);

void MFTYPE    MbufCopy             (MIL_ID SrcBufId,
                                     MIL_ID DestBufId);

void MFTYPE    MbufCopyColor        (MIL_ID SrcBufId,
                                     MIL_ID DestBufId,
                                     MIL_INT Band);

void MFTYPE    MbufCopyColor2d      (MIL_ID  SrcBufId,
                                     MIL_ID  DstBufId,
                                     MIL_INT SrcBand,
                                     MIL_INT SrcOffX,
                                     MIL_INT SrcOffY,
                                     MIL_INT DstBand,
                                     MIL_INT DstOffX,
                                     MIL_INT DstOffY,
                                     MIL_INT SizeX,
                                     MIL_INT SizeY);

void MFTYPE    MbufCopyClip         (MIL_ID SrcBufId,
                                     MIL_ID DestBufId,
                                     MIL_INT DestOffX,
                                     MIL_INT DestOffY);

void MFTYPE    MbufCopyMask         (MIL_ID SrcBufId,
                                     MIL_ID DestBufId,
                                     MIL_INT64 MaskValue);

void MFTYPE    MbufCopyCondDouble   (MIL_ID SrcBufId,
                                     MIL_ID DestBufId,
                                     MIL_ID CondBufId,
                                     MIL_INT64 Condition,
                                     MIL_DOUBLE CondValue);

void MFTYPE    MbufClearCondDouble  (MIL_ID BufId,
                                     MIL_DOUBLE RedOrMonoVal,
                                     MIL_DOUBLE GreenVal,
                                     MIL_DOUBLE BlueVal,
                                     MIL_ID CondBufId,
                                     MIL_INT64 Condition,
                                     MIL_DOUBLE CondValue);

void MFTYPE    MbufGetLine          (MIL_ID ImageBufId,
                                     MIL_INT StartX,
                                     MIL_INT StartY,
                                     MIL_INT EndX,
                                     MIL_INT EndY,
                                     MIL_INT64 Mode,
                                     MIL_INT* NbPixelsPtr,
                                     void* UserArrayPtr);

void MFTYPE MbufLink                (MIL_ID SourceBufId,
                                     MIL_ID TargetBufId,
                                     MIL_INT64 LinkOperation,
                                     MIL_INT ControlFlag);


void MFTYPE    MbufPut1d            (MIL_ID DestBufId,
                                     MIL_INT OffX,
                                     MIL_INT SizeX,
                                     const void* UserArrayPtr);

void MFTYPE    MbufPut2d            (MIL_ID DestBufId,
                                     MIL_INT OffX,
                                     MIL_INT OffY,
                                     MIL_INT SizeX,
                                     MIL_INT SizeY,
                                     const void* UserArrayPtr);

void MFTYPE    MbufPutColor         (MIL_ID DestBufId,
                                     MIL_INT64 DataFormat,
                                     MIL_INT Band,
                                     const void* UserArrayPtr);

void MFTYPE    MbufPutColor2d       (MIL_ID DestBufId,
                                     MIL_INT64 DataFormat,
                                     MIL_INT Band,
                                     MIL_INT OffX,
                                     MIL_INT OffY,
                                     MIL_INT SizeX,
                                     MIL_INT SizeY,
                                     const void* UserArrayPtr);

void MFTYPE    MbufPutLine          (MIL_ID ImageBufId,
                                     MIL_INT StartX,
                                     MIL_INT StartY,
                                     MIL_INT EndX,
                                     MIL_INT EndY,
                                     MIL_INT64 Mode,
                                     MIL_INT* NbPixelsPtr,
                                     const void* UserArrayPtr);

void MFTYPE    MbufPut              (MIL_ID DestBufId,
                                     const void* UserArrayPtr);

void MFTYPE    MbufGet1d            (MIL_ID  SrcBufId,
                                     MIL_INT OffX,
                                     MIL_INT SizeX,
                                     void   *UserArrayPtr);

void MFTYPE    MbufGet2d            (MIL_ID  SrcBufId,
                                     MIL_INT OffX,
                                     MIL_INT OffY,
                                     MIL_INT SizeX,
                                     MIL_INT SizeY,
                                     void   *UserArrayPtr);

void MFTYPE    MbufGetColor         (MIL_ID SrcBufId,
                                     MIL_INT64 DataFormat,
                                     MIL_INT Band,
                                     void* UserArrayPtr);

void MFTYPE    MbufGet              (MIL_ID SrcBufId,
                                     void* UserArrayPtr);

void MFTYPE    MbufGetColor2d       (MIL_ID    SrcBufId,
                                     MIL_INT64 DataFormat,
                                     MIL_INT   Band,
                                     MIL_INT   OffX,
                                     MIL_INT   OffY,
                                     MIL_INT   SizeX,
                                     MIL_INT   SizeY,
                                     void     *UserArrayPtr);

MIL_INT MFTYPE MbufGetArc              (MIL_ID     ImageBufId,
                                        MIL_INT    XCenter,
                                        MIL_INT    YCenter,
                                        MIL_INT    XRad,
                                        MIL_INT    YRad,
                                        MIL_DOUBLE StartAngle,
                                        MIL_DOUBLE EndAngle,
                                        void*      UserArrayPtr,
                                        MIL_INT*   NbPixelsPtr);

MIL_INT MFTYPE MbufInquire          (MIL_ID BufId,
                                     MIL_INT64 InquireType,
                                     void* UserVarPtr);

#if M_MIL_USE_UNICODE
MIL_INT MFTYPE MbufDiskInquireA     (const char* FileName,
                                     MIL_INT64 InquireType,
                                     void* UserVarPtr);
void MFTYPE    MbufSaveA            (const char* FileName,
                                     MIL_ID BufId);
MIL_ID MFTYPE  MbufRestoreA         (const char* FileName,
                                     MIL_ID SystemId,
                                     MIL_ID* BufIdPtr);
void MFTYPE    MbufLoadA            (const char* FileName,
                                     MIL_ID BufId);
MIL_ID MFTYPE  MbufImportA          (const char* FileName,
                                     MIL_INT64 FileFormat,
                                     MIL_INT64 Operation,
                                     MIL_ID SystemId,
                                     MIL_ID* BufIdPtr);
void MFTYPE    MbufExportA          (const char* FileName,
                                     MIL_INT64 FileFormat,
                                     MIL_ID SrcBufId);
void MFTYPE    MbufExportSequenceA  (const char* FileName,
                                     MIL_INT64 FileFormat,
                                     const MIL_ID *BufArrayPtrOrSystemIdPtr,
                                     MIL_INT NumberOfImages,
                                     MIL_DOUBLE FrameRate,
                                     MIL_INT64 ControlFlag);
void MFTYPE    MbufImportSequenceA  (const char* FileName,
                                     MIL_INT64 FileFormat,
                                     MIL_INT64 Operation,
                                     MIL_ID SystemId,
                                     MIL_ID *BufArrayPtr,
                                     MIL_INT StartImage,
                                     MIL_INT NumberOfImages,
                                     MIL_INT64 ControlFlag);

MIL_INT MFTYPE MbufDiskInquireW     (MIL_CONST_TEXT_PTR FileName,
                                     MIL_INT64 InquireType,
                                     void* UserVarPtr);

void MFTYPE    MbufSaveW            (MIL_CONST_TEXT_PTR FileName,
                                     MIL_ID BufId);
MIL_ID MFTYPE  MbufRestoreW         (MIL_CONST_TEXT_PTR FileName,
                                     MIL_ID SystemId,
                                     MIL_ID* BufIdPtr);
void MFTYPE    MbufLoadW            (MIL_CONST_TEXT_PTR FileName,
                                     MIL_ID BufId);
MIL_ID MFTYPE  MbufImportW          (MIL_CONST_TEXT_PTR FileName,
                                     MIL_INT64 FileFormat,
                                     MIL_INT64 Operation,
                                     MIL_ID SystemId,
                                     MIL_ID* BufIdPtr);
void MFTYPE    MbufExportW          (MIL_CONST_TEXT_PTR FileName,
                                     MIL_INT64 FileFormat,
                                     MIL_ID SrcBufId);
void MFTYPE    MbufExportSequenceW  (MIL_CONST_TEXT_PTR FileName,
                                     MIL_INT64 FileFormat,
                                     const MIL_ID *BufArrayPtrOrSystemIdPtr,
                                     MIL_INT NumberOfImages,
                                     MIL_DOUBLE FrameRate,
                                     MIL_INT64 ControlFlag);
void MFTYPE    MbufImportSequenceW  (MIL_CONST_TEXT_PTR FileName,
                                     MIL_INT64 FileFormat,
                                     MIL_INT64 Operation,
                                     MIL_ID SystemId,
                                     MIL_ID *BufArrayPtr,
                                     MIL_INT StartImage,
                                     MIL_INT NumberOfImages,
                                     MIL_INT64 ControlFlag);
#if M_MIL_UNICODE_API
#define MbufExportSequence MbufExportSequenceW
#define MbufImportSequence MbufImportSequenceW
#define MbufDiskInquire    MbufDiskInquireW
#define MbufSave           MbufSaveW
#define MbufRestore        MbufRestoreW
#define MbufLoad           MbufLoadW
#define MbufImport         MbufImportW
#define MbufExport         MbufExportW
#else
#define MbufExportSequence MbufExportSequenceA
#define MbufImportSequence MbufImportSequenceA
#define MbufDiskInquire    MbufDiskInquireA
#define MbufSave           MbufSaveA
#define MbufRestore        MbufRestoreA
#define MbufLoad           MbufLoadA
#define MbufImport         MbufImportA
#define MbufExport         MbufExportA
#endif
#else

MIL_INT     MFTYPE    MbufDiskInquire(MIL_CONST_TEXT_PTR FileName,
                                      MIL_INT64 InquireType,
                                      void* UserVarPtr);

void MFTYPE    MbufSave             (MIL_CONST_TEXT_PTR FileName,
                                     MIL_ID BufId);
MIL_ID MFTYPE  MbufRestore          (MIL_CONST_TEXT_PTR FileName,
                                     MIL_ID SystemId,
                                     MIL_ID* BufIdPtr);
void MFTYPE    MbufLoad             (MIL_CONST_TEXT_PTR FileName,
                                     MIL_ID BufId);
MIL_ID MFTYPE  MbufImport           (MIL_CONST_TEXT_PTR FileName,
                                     MIL_INT64 FileFormat,
                                     MIL_INT64 Operation,
                                     MIL_ID SystemId,
                                     MIL_ID* BufIdPtr);
void MFTYPE    MbufExport           (MIL_CONST_TEXT_PTR FileName,
                                     MIL_INT64 FileFormat,
                                     MIL_ID SrcBufId);
void MFTYPE    MbufExportSequence   (MIL_CONST_TEXT_PTR FileName,
                                     MIL_INT64 FileFormat,
                                     const MIL_ID *BufArrayPtrOrSystemIdPtr,
                                     MIL_INT NumberOfImages,
                                     MIL_DOUBLE FrameRate,
                                     MIL_INT64 ControlFlag);
void MFTYPE    MbufImportSequence   (MIL_CONST_TEXT_PTR FileName,
                                     MIL_INT64 FileFormat,
                                     MIL_INT64 Operation,
                                     MIL_ID SystemId,
                                     MIL_ID *BufArrayPtr,
                                     MIL_INT StartImage,
                                     MIL_INT NumberOfImages,
                                     MIL_INT64 ControlFlag);

#endif

void MFTYPE    MbufControlAreaDouble(MIL_ID  BufId,
                                     MIL_INT OffsetX,
                                     MIL_INT OffsetY,
                                     MIL_INT SizeX,
                                     MIL_INT SizeY,
                                     MIL_INT Band,
                                     MIL_INT64 ControlType,
                                     MIL_DOUBLE ControlValue);
#define MbufModified2d(BufId, OffsetX, OffsetY, SizeX, SizeY) \
        MbufControlAreaDouble(BufId, OffsetX, OffsetY, SizeX, SizeY, M_ALL_BAND, M_MODIFIED, M_DEFAULT)


void MFTYPE    MbufCompression      (MIL_ID SrcImageId,
                                     MIL_ID DestImageId,
                                     MIL_INT ControlValue,
                                     MIL_INT64 OperationFlag);

void MFTYPE    MbufBayer            (MIL_ID SrcImageBufId,
                                     MIL_ID DestImageBufId,
                                     MIL_ID CoefOrExpId,
                                     MIL_INT64 ControlFlag);

void MFTYPE    MbufTransfer         (MIL_ID     SrcBufId,
                                     MIL_ID     DestBufId,
                                     MIL_INT    SrcOffX,
                                     MIL_INT    SrcOffY,
                                     MIL_INT    SrcSizeX,
                                     MIL_INT    SrcSizeY,
                                     MIL_INT    SrcBand,
                                     MIL_INT    DestOffX,
                                     MIL_INT    DestOffY,
                                     MIL_INT    DestSizeX,
                                     MIL_INT    DestSizeY,
                                     MIL_INT    DestBand,
                                     MIL_INT64  TransferFunction,
                                     MIL_INT64  TransferType,
                                     MIL_INT64  OperationFlag,
                                     void      *ExtraParameterPtr);

void MFTYPE    MbufTransferDouble   (MIL_ID     SrcBufId,
                                     MIL_ID     DestBufId,
                                     MIL_DOUBLE SrcOffX,
                                     MIL_DOUBLE SrcOffY,
                                     MIL_DOUBLE SrcSizeX,
                                     MIL_DOUBLE SrcSizeY,
                                     MIL_INT    SrcBand,
                                     MIL_INT    DestOffX,
                                     MIL_INT    DestOffY,
                                     MIL_INT    DestSizeX,
                                     MIL_INT    DestSizeY,
                                     MIL_INT    DestBand,
                                     MIL_INT64  TransferFunction,
                                     MIL_INT64  TransferType,
                                     MIL_INT64  OperationFlag,
                                     void      *ExtraParameterPtr);

      /* -------------------------------------------------------------- */
      /* -------------------------------------------------------------- */

      /* CONTROL: */

      /* -------------------------------------------------------------- */

void MFTYPE    MbufControlNeighborhoodMIL_INT(MIL_ID BufId,
                                              MIL_INT64 OperationControlType,
                                              MIL_INT OperationValue);

#if M_MIL_USE_64BIT
// Prototypes for 64 bits OSs

void MFTYPE    MbufControlInt64        (MIL_ID BufId,
                                        MIL_INT64 ControlType,
                                        MIL_INT64 ControlValue);
void MFTYPE    MbufControlDouble       (MIL_ID BufId,
                                        MIL_INT64 ControlType,
                                        MIL_DOUBLE ControlValue);
#else
// Prototypes for 32 bits OSs

#define MbufControlDouble MbufControl
#define MbufControlInt64  MbufControl
void MFTYPE    MbufControl             (MIL_ID BufId,
                                        MIL_INT64 ControlType,
                                        MIL_DOUBLE ControlValue);
#endif


void MFTYPE    MbufSetRegionDouble     (MIL_ID BufId,
                                        MIL_ID ImageOrGraphicListId,
                                        MIL_INT64 Label,
                                        MIL_INT64 Operation,
                                        MIL_DOUBLE Param);

      /* -------------------------------------------------------------- */
      /* -------------------------------------------------------------- */

      /* HOOK: */

      /* -------------------------------------------------------------- */

typedef MIL_HOOK_FUNCTION_PTR MBUFHOOKFCTPTR;
typedef MIL_HOOK_FUNCTION_PTR MIL_BUF_HOOK_FUNCTION_PTR;

void    MFTYPE    MbufHookFunction     (MIL_ID BufferId,
                                        MIL_INT HookType,
                                        MIL_BUF_HOOK_FUNCTION_PTR HookHandlerPtr,
                                        void* UserDataPtr);

MIL_INT MFTYPE    MbufGetHookInfo      (MIL_ID EventId,
                                        MIL_INT64 InfoType,
                                        void* UserVarPtr);


      /* -------------------------------------------------------------- */
      /* -------------------------------------------------------------- */

/***************************************************************************/
/* I/O DEVICES:                                                            */
/***************************************************************************/

      /* -------------------------------------------------------------- */
      /* -------------------------------------------------------------- */

      /* CREATION: */

      /* -------------------------------------------------------------- */

typedef MIL_HOOK_FUNCTION_PTR MDIGHOOKFCTPTR;
typedef MIL_HOOK_FUNCTION_PTR MIL_DIG_HOOK_FUNCTION_PTR;

void MFTYPE    MdigHookFunction     (MIL_ID DigId,
                                     MIL_INT HookType,
                                     MIL_DIG_HOOK_FUNCTION_PTR HookHandlerPtr,
                                     void* UserDataPtr);
#if M_MIL_USE_UNICODE
MIL_ID MFTYPE  MdigAllocA           (MIL_ID SystemId,
                                     MIL_INT DigNum,
                                     const char* DataFormat,
                                     MIL_INT64 InitFlag,
                                     MIL_ID* DigIdPtr);

MIL_ID MFTYPE  MdigAllocW           (MIL_ID SystemId,
                                     MIL_INT DigNum,
                                     MIL_CONST_TEXT_PTR DataFormat,
                                     MIL_INT64 InitFlag,
                                     MIL_ID* DigIdPtr);
#if M_MIL_UNICODE_API
#define MdigAlloc MdigAllocW
#else
#define MdigAlloc MdigAllocA
#endif
#else
MIL_ID MFTYPE  MdigAlloc            (MIL_ID SystemId,
                                     MIL_INT DigNum,
                                     MIL_CONST_TEXT_PTR DataFormat,
                                     MIL_INT64 InitFlag,
                                     MIL_ID* DigIdPtr);
#endif

void MFTYPE    MdigFree             (MIL_ID DigId);


      /* -------------------------------------------------------------- */
      /* -------------------------------------------------------------- */

      /* CONTROL: */

      /* -------------------------------------------------------------- */

void MFTYPE    MdigChannel         (MIL_ID DigId,
                                    MIL_INT64 Channel);
MIL_DEPRECATED(MdigChannel, 1000)

void MFTYPE    MdigReference       (MIL_ID  DigId,
                                    MIL_INT64 ReferenceType,
                                    MIL_DOUBLE ReferenceLevel);
MIL_DEPRECATED(MdigReference, 1000)

void MFTYPE    MdigLut             (MIL_ID DigId,
                                    MIL_ID LutBufId);
MIL_DEPRECATED(MdigLut, 1000)

void MFTYPE    MdigHalt            (MIL_ID DigId);

MIL_INT MFTYPE MdigInquire         (MIL_ID DigId,
                                    MIL_INT64 InquireType,
                                    void* UserVarPtr);

#if M_MIL_USE_64BIT
// Prototypes for 64 bits OSs

void MFTYPE    MdigControlInt64    (MIL_ID DigId,
                                    MIL_INT64 ControlType,
                                    MIL_INT64 ControlValue);

void MFTYPE    MdigControlDouble   (MIL_ID DigId,
                                    MIL_INT64 ControlType,
                                    MIL_DOUBLE ControlValue);

#else
// Prototypes for 32 bits OSs

#define MdigControlDouble MdigControl
#define MdigControlInt64  MdigControl
void MFTYPE    MdigControl         (MIL_ID DigId,
                                    MIL_INT64 ControlType,
                                    MIL_DOUBLE ControlValue);
#endif

void MFTYPE    MdigGrabWait        (MIL_ID DigId,
                                    MIL_INT64 ControlFlag);

      /* -------------------------------------------------------------- */
      /* -------------------------------------------------------------- */

       /* ACCESS: */

      /* -------------------------------------------------------------- */

void MFTYPE    MdigGrab            (MIL_ID DigId,
                                    MIL_ID DestImageBufId);

void MFTYPE    MdigGrabContinuous  (MIL_ID DigId,
                                    MIL_ID DestImageBufId);

void MFTYPE    MdigProcess         (MIL_ID DigId,
                                    const MIL_ID* DestImageArrayPtr,
                                    MIL_INT ImageCount,
                                    MIL_INT64 Operation,
                                    MIL_INT64 OperationFlag,
                                    MIL_DIG_HOOK_FUNCTION_PTR HookHandlerPtr,
                                    void* UserDataPtr);

MIL_INT MFTYPE    MdigGetHookInfo  (MIL_ID EventId,
                                    MIL_INT64 InfoType,
                                    void* UserVarPtr);

typedef MIL_INT (MFTYPE *MIL_FOCUS_HOOK_FUNCTION_PTR)(MIL_INT HookType,
                                                      MIL_INT position,
                                                      void* UserDataPtr);
typedef MIL_FOCUS_HOOK_FUNCTION_PTR MFOCUSHOOKFCTPTR;

void MFTYPE    MdigFocus           (MIL_ID DigId,
                                    MIL_ID DestImageBufId,
                                    MIL_ID FocusImageRegionBufId,
                                    MIL_FOCUS_HOOK_FUNCTION_PTR FocusHookPtr,
                                    void* UserDataPtr,
                                    MIL_INT MinPosition,
                                    MIL_INT StartPosition,
                                    MIL_INT MaxPosition,
                                    MIL_INT MaxPositionVariation,
                                    MIL_INT64 ProcMode,
                                    MIL_INT *ResultPtr);

#if M_MIL_USE_UNICODE
void MFTYPE MdigControlFeatureW    (MIL_ID DigId,
                                    MIL_INT64 ControlType,
                                    MIL_CONST_TEXT_PTR FeatureName,
                                    MIL_INT64 FeatureType,
                                    const void* UserVarPtr);

void MFTYPE MdigInquireFeatureW    (MIL_ID DigId,
                                    MIL_INT64 InquireType,
                                    MIL_CONST_TEXT_PTR FeatureName,
                                    MIL_INT64 FeatureType,
                                    void* UserVarPtr);

void MFTYPE MdigControlFeatureA    (MIL_ID DigId,
                                    MIL_INT64 ControlType,
                                    const char* FeatureName,
                                    MIL_INT64 FeatureType,
                                    const void* UserVarPtr);

void MFTYPE MdigInquireFeatureA    (MIL_ID DigId,
                                    MIL_INT64 InquireType,
                                    const char* FeatureName,
                                    MIL_INT64 FeatureType,
                                    void* UserVarPtr);

#if M_MIL_UNICODE_API
#define MdigControlFeature MdigControlFeatureW
#define MdigInquireFeature MdigInquireFeatureW
#else
#define MdigControlFeature MdigControlFeatureA
#define MdigInquireFeature MdigInquireFeatureA
#endif
#else

void MFTYPE MdigControlFeature    (MIL_ID DigId,
                                   MIL_INT64 ControlType,
                                   MIL_CONST_TEXT_PTR FeatureName,
                                   MIL_INT64 FeatureType,
                                   const void* UserVarPtr);

void MFTYPE MdigInquireFeature    (MIL_ID DigId,
                                   MIL_INT64 InquireType,
                                   MIL_CONST_TEXT_PTR FeatureName,
                                   MIL_INT64 FeatureType,
                                   void* UserVarPtr);
#endif



      /* -------------------------------------------------------------- */

/***************************************************************************/
/* SYSTEM MODULE:                                                          */
/***************************************************************************/

typedef MIL_HOOK_FUNCTION_PTR MSYSHOOKFCTPTR;
typedef MIL_HOOK_FUNCTION_PTR MIL_SYS_HOOK_FUNCTION_PTR;

void MFTYPE MsysHookFunction        (MIL_ID                    SystemId,
                                     MIL_INT                   HookType,
                                     MIL_SYS_HOOK_FUNCTION_PTR HookHandlerPtr,
                                     void*                     UserDataPtr);

MIL_INT MFTYPE  MsysGetHookInfo     (MIL_ID     SystemId,
                                     MIL_ID     EventId,
                                     MIL_INT64  InquireType,
                                     void*      UserVarPtr);

#if M_MIL_USE_UNICODE
MIL_ID MFTYPE MsysAllocW            (MIL_ID             ContextAppId,
                                     MIL_CONST_TEXT_PTR SystemDescriptor,
                                     MIL_INT            SystemNum,
                                     MIL_INT64          InitFlag,
                                     MIL_ID*            SystemIdPtr);

MIL_ID MFTYPE MsysAllocA            (MIL_ID             ContextAppId,
                                     const char*        SystemDescriptor,
                                     MIL_INT            SystemNum,
                                     MIL_INT64          InitFlag,
                                     MIL_ID*            SystemIdPtr);

#if M_MIL_UNICODE_API
#define MsysAlloc MsysAllocW
#else
#define MsysAlloc MsysAllocA
#endif

#else
MIL_ID MFTYPE MsysAlloc             (MIL_ID             ContextAppId,
                                     MIL_CONST_TEXT_PTR SystemDescriptor,
                                     MIL_INT            SystemNum,
                                     MIL_INT64          InitFlag,
                                     MIL_ID*            SystemIdPtr);
#endif

void MFTYPE  MsysFree               (MIL_ID     SystemId);

MIL_INT MFTYPE  MsysInquire         (MIL_ID     SystemId,
                                     MIL_INT64  InquireType,
                                     void*      UserVarPtr);

MIL_DLLFUNC void      MFTYPE  MsysControlDouble(MIL_ID     SystemId,
                                                MIL_INT64  ControlType,
                                                MIL_DOUBLE ControlValue);

MIL_DLLFUNC void      MFTYPE  MsysControlInt64(MIL_ID     SystemId,
                                               MIL_INT64  ControlType,
                                               MIL_INT64  ControlValue);

MIL_DLLFUNC void MFTYPE MsysConfigAccess        (MIL_ID     SystemId,
                                     MIL_INT    VendorId,
                                     MIL_INT    DeviceId,
                                     MIL_INT    DeviceNum,
                                     MIL_INT64  OperationFlag,
                                     MIL_INT64  OperationType,
                                     MIL_INT    Offset,
                                     MIL_INT    Size,
                                     void*      UserArrayPtr);

MIL_DLLFUNC MIL_ID    MFTYPE  MsysIoAlloc(MIL_ID SystemId,
                                          MIL_INT64 IoObjectNum,
                                          MIL_INT64 InitFlag,
                                          MIL_INT64 InitValue,
                                          MIL_ID* IoObjectPtr);

MIL_DLLFUNC void      MFTYPE  MsysIoFree(MIL_ID IoObjectId);

MIL_DLLFUNC MIL_INT   MFTYPE  MsysIoInquire(MIL_ID IoObjectId,
                                            MIL_INT64 InquireType,
                                            void* UserVarPtr);

#if M_MIL_USE_64BIT
// Prototypes for 64 bits OSs

MIL_DLLFUNC void      MFTYPE  MsysIoControlInt64(MIL_ID IoObjectId,
                                                 MIL_INT64 ControlType,
                                                 MIL_INT64 ControlValue);

MIL_DLLFUNC void      MFTYPE  MsysIoControlDouble(MIL_ID IoObjectId,
                                                  MIL_INT64 ControlType,
                                                  MIL_DOUBLE ControlValue);
#else
// Prototypes for 32 bits OSs

#define MsysIoControlDouble MsysIoControl
#define MsysIoControlInt64  MsysIoControl
MIL_DLLFUNC void      MFTYPE  MsysIoControl(MIL_ID IoObjectId,
                                            MIL_INT64 ControlType,
                                            MIL_DOUBLE ControlValue);
#endif

MIL_DLLFUNC MIL_INT   MFTYPE  MsysIoCommandRegister(MIL_ID IoObjectId,
                                                    MIL_INT64 Operation,
                                                    MIL_INT64 ReferenceStamp,
                                                    MIL_DOUBLE DelayFromReferenceStamp,
                                                    MIL_DOUBLE Duration,
                                                    MIL_INT64 BitToOperate,
                                                    void* CommandStatusPtr);

/***************************************************************************/
/* APPLICATION MODULE:                                                     */
/***************************************************************************/

typedef MIL_HOOK_FUNCTION_PTR MAPPHOOKFCTPTR;
typedef MIL_HOOK_FUNCTION_PTR MIL_APP_HOOK_FUNCTION_PTR;

#if M_MIL_USE_UNICODE
   MIL_ID MFTYPE     MappAllocA              (const char* ServerDescription,
                                              MIL_INT64 InitFlag,
                                              MIL_ID* ContextAppIdPtr);

   MIL_ID MFTYPE     MappAllocW              (MIL_CONST_TEXT_PTR ServerDescription,
                                              MIL_INT64 InitFlag,
                                              MIL_ID* ContextAppIdPtr);
   #if !M_COMPILING_MILDLL
      #if M_MIL_UNICODE_API
      #define MappAlloc MappAllocW
      #else
      #define MappAlloc MappAllocA
      #endif
   #endif
#else
MIL_ID MFTYPE     MappAlloc               (MIL_CONST_TEXT_PTR ServerDescription,
                                           MIL_INT64 InitFlag,
                                           MIL_ID* ContextAppIdPtr);
#endif


void MFTYPE       MappFree                (MIL_ID ContextAppId);


void MFTYPE       MappControl             (MIL_ID ContextAppId,
                                           MIL_INT64 ControlType,
                                           MIL_INT ControlValue);

MIL_INT MFTYPE    MappGetError            (MIL_ID ContextAppId,
                                           MIL_INT64 ErrorType,
                                           void* ErrorPtr);

MIL_INT MFTYPE    MappGetHookInfo         (MIL_ID ContextAppId,
                                           MIL_ID EventId,
                                           MIL_INT64 InfoType,
                                           void* UserVarPtr);

void MFTYPE       MappHookFunction        (MIL_ID ContextAppId,
                                           MIL_INT HookType,
                                           MIL_APP_HOOK_FUNCTION_PTR HookHandlerPtr,
                                           void* UserDataPtr);

MIL_INT MFTYPE    MappInquire             (MIL_ID ContextAppId,
                                           MIL_INT64 InquireType,
                                           void* UserVarPtr);

MIL_DOUBLE MFTYPE MappTimer               (MIL_ID ContextAppId,
                                           MIL_INT64 ControlType,
                                           MIL_DOUBLE *TimePtr);

void MFTYPE       MappTimerVoid           (MIL_ID ContextAppId,
                                           MIL_INT64 Mode,
                                           MIL_DOUBLE *Time);

MIL_INT MFTYPE    MappInquireMp           (MIL_ID ContextAppId,
                                           MIL_INT64 InquireType,
                                           MIL_INT64 TypeFlag,
                                           MIL_INT64 TypeValue,
                                           void* UserVarPtr);

void    MFTYPE    MappControlMp           (MIL_ID ContextAppId,
                                           MIL_INT64 ControlType,
                                           MIL_INT64 TypeFlag,
                                           MIL_INT64 TypeValue,
                                           void* ValuePtr);


#if M_MIL_USE_UNICODE
void MFTYPE       MappOpenConnectionA     (const char* ConnectionDescriptor,
                                           MIL_INT64 InitFlag,
                                           MIL_INT64 ControlFlag,
                                           MIL_ID* RemoteContextAppIdPtr);

void MFTYPE       MappOpenConnectionW     (MIL_CONST_TEXT_PTR ConnectionDescriptor,
                                           MIL_INT64 InitFlag,
                                           MIL_INT64 ControlFlag,
                                           MIL_ID* RemoteContextAppIdPtr);

void   MFTYPE     MappFileOperationW      (MIL_ID Comp1ContextAppId, 
                                           MIL_CONST_TEXT_PTR Comp1FileName, 
                                           MIL_ID Comp2ContextAppId, 
                                           MIL_CONST_TEXT_PTR Comp2FileName, 
                                           MIL_INT64 Operation, 
                                           MIL_INT64 OperationFlag, 
                                           void *OperationDataPtr);

void   MFTYPE     MappFileOperationA      (MIL_ID Comp1ContextAppId, 
                                           const char* Comp1FileName, 
                                           MIL_ID Comp2ContextAppId, 
                                           const char* Comp2FileName, 
                                           MIL_INT64 Operation, 
                                           MIL_INT64 OperationFlag, 
                                           void *OperationDataPtr);

void   MFTYPE     MappTraceA              (MIL_ID ContextAppId,
                                           MIL_INT64 TraceType,
                                           MIL_INT64 TraceTag,
                                           MIL_INT64 TraceValue,
                                           const char* TraceString);

void   MFTYPE     MappTraceW              (MIL_ID ContextAppId,
                                           MIL_INT64 TraceType,
                                           MIL_INT64 TraceTag,
                                           MIL_INT64 TraceValue,
                                           MIL_CONST_TEXT_PTR TraceString);

#if M_MIL_UNICODE_API
   #define MappOpenConnection MappOpenConnectionW
   #define MappTrace MappTraceW
   #if M_MIL_USE_SAFE_TYPE
      #define MappFileOperation MappFileOperationSafeTypeW
   #else
      #define MappFileOperation MappFileOperationW
   #endif
#else
   #define MappOpenConnection MappOpenConnectionA
   #define MappTrace MappTraceA
   #if M_MIL_USE_SAFE_TYPE
      #define MappFileOperation MappFileOperationSafeTypeA
   #else
      #define MappFileOperation MappFileOperationA
   #endif
#endif
#else
void MFTYPE       MappOpenConnection      (MIL_CONST_TEXT_PTR ConnectionDescriptor,
                                           MIL_INT64 InitFlag,
                                           MIL_INT64 ControlFlag,
                                           MIL_ID* RemoteContextAppIdPtr);


void   MFTYPE     MappFileOperation       (MIL_ID Comp1ContextAppId, 
                                           MIL_CONST_TEXT_PTR Comp1FileName, 
                                           MIL_ID Comp2ContextAppId, 
                                           MIL_CONST_TEXT_PTR Comp2FileName, 
                                           MIL_INT64 Operation, 
                                           MIL_INT64 OperationFlag, 
                                           void *OperationDataPtr);

void   MFTYPE     MappTrace               (MIL_ID ContextAppId,
                                           MIL_INT64 TraceType,
                                           MIL_INT64 TraceTag,
                                           MIL_INT64 TraceValue,
                                           MIL_CONST_TEXT_PTR TraceString);
#endif

void MFTYPE       MappCloseConnection     (MIL_ID RemoteContextAppId);

MIL_INT MFTYPE    MappInquireConnection   (MIL_ID ContextAppId,
                                           MIL_INT64 InquireType,
                                           MIL_INT64 ControlFlag,
                                           MIL_INT64 ExtraFlag,
                                           void* UserVarPtr);


MIL_INT MFTYPE    MappInquireObject       (MIL_ID ContextAppId,
                                           MIL_ID ObjectId,
                                           MIL_INT64 InquireType,
                                           void* UserVarPtr);

#if M_MIL_USE_64BIT
// Prototypes for 64 bits OSs
void MFTYPE       MappControlObjectDouble (MIL_ID ContextAppId,
                                           MIL_ID ObjectId,
                                           MIL_INT64 ControlType,
                                           MIL_DOUBLE ControlValue);

void MFTYPE       MappControlObjectInt64  (MIL_ID ContextAppId,
                                           MIL_ID ObjectId,
                                           MIL_INT64 ControlType,
                                           MIL_INT64 ControlValue);
#else
// Prototypes for 32 bits OSs
#define MappControlObjectDouble MappControlObject
#define MappControlObjectInt64  MappControlObject

void MFTYPE       MappControlObject       (MIL_ID ContextAppId,
                                           MIL_ID ObjectId,
                                           MIL_INT64 ControlType,
                                           MIL_DOUBLE ControlValue);
#endif


/***************************************************************************/
/* OBJ MODULE:                                                              */
/***************************************************************************/
void MFTYPE MobjControl(MIL_ID ObjectId, MIL_INT64 ControlType, void* ControlPtr);
MIL_INT MFTYPE MobjInquire(MIL_ID ObjectId, MIL_INT64 InquireType, void* UserVarPtr);


/***************************************************************************/
/* THREAD FUNCTIONS:                                                       */
/***************************************************************************/
typedef MIL_UINT32 (MFTYPE *MIL_THREAD_FUNCTION_PTR)(void* UserDataPtr);
typedef MIL_THREAD_FUNCTION_PTR MTHREADFCTPTR;

MIL_ID MFTYPE  MthrAlloc         (MIL_ID SystemId,
                                  MIL_INT64 ObjectType,
                                  MIL_INT64 ControlFlag,
                                  MIL_THREAD_FUNCTION_PTR ThreadFctPtr,
                                  void *UserDataPtr,
                                  MIL_ID *ThreadEventOrMutexIdPtr);
#if M_MIL_USE_64BIT
// Prototypes for 64 bits OSs

void MFTYPE    MthrControlInt64  (MIL_ID ThreadEventOrMutexId,
                                  MIL_INT64 ControlType,
                                  MIL_INT64 ControlValue);

void MFTYPE    MthrControlDouble (MIL_ID ThreadEventOrMutexId,
                                  MIL_INT64 ControlType,
                                  MIL_DOUBLE ControlValue);
#else
// Prototypes for 32 bits OSs

#define MthrControlDouble MthrControl
#define MthrControlInt64  MthrControl
void MFTYPE    MthrControl       (MIL_ID ThreadEventOrMutexId,
                                  MIL_INT64 ControlType,
                                  MIL_DOUBLE ControlValue);
#endif

void MFTYPE    MthrFree          (MIL_ID ThreadEventorMutexId);

MIL_INT MFTYPE MthrInquire       (MIL_ID ThreadEventOrMutexId,
                                  MIL_INT64 InquireType,
                                  void *UserVarPtr);

MIL_INT MFTYPE MthrWait          (MIL_ID ThreadOrEventId,
                                  MIL_INT64 WaitOption,
                                  MIL_INT *StatePtr);

MIL_INT MFTYPE MthrWaitMultiple  (const MIL_ID *EventArrayIdPtr,
                                  MIL_INT EventArraySize,
                                  MIL_INT64 WaitOption,
                                  MIL_INT *StatePtr);

MIL_INT MFTYPE MthrInquireMp      (MIL_ID ThrId,
                                   MIL_INT64 InquireType,
                                   MIL_INT64 TypeFlag,
                                   MIL_INT64 TypeValue,
                                   void* ResultPtr);

void    MFTYPE MthrControlMp      (MIL_ID ThrId,
                                   MIL_INT64 ControlType,
                                   MIL_INT64 TypeFlag,
                                   MIL_INT64 TypeValue,
                                   void* ValuePtr);


/***************************************************************************/
/* OS MODULE:                                                              */
/***************************************************************************/
// Main declaration
#if M_MIL_UNICODE_API && !M_MIL_USE_LINUX
#define MosMain wmain
#define MosWinMain wWinMain
#else
#define MosMain main
#define MosWinMain WinMain
#endif


MIL_INT MFTYPE MosGetch();
MIL_INT MFTYPE MosKbhit();
MIL_INT MFTYPE MosGetchar();
MIL_INT MFTYPE MosGetMilInt();
MIL_INT MFTYPE MosAbs(MIL_INT val);
void MFTYPE MosSleep(MIL_INT ms);

// String manipulation
#if M_MIL_USE_UNICODE
   MIL_INT MFTYPE MosStrlenA(const char* str);
   char* MFTYPE MosStruprA(char* str);   
   char* MFTYPE MosStrlwrA(char* str);
   MIL_INT MFTYPE MosStrcmpA(const char* string1, const char* string2);
   char* MFTYPE MosStrcatA(char* dest, MIL_INT destsize, const char* source);
   MIL_INT MosPrintfA(const char* format, ...);
   MIL_INT MosVprintfA(const char* format, va_list args);
   MIL_INT MosVsprintfA(char* buf, MIL_INT bufsize, const char* format, va_list args);
   MIL_INT MosSprintfA(char* buf, MIL_INT bufsize, const char* format, ...);
   char* MFTYPE MosStrcpyA(char* dest, MIL_INT destsize, const char* source);
#endif

#if M_MIL_USE_LINUX
   #define MosStrlenA MosStrlen
   #define MosStruprA MosStrupr   
   #define MosStrlwrA MosStrlwr
   #define MosStrcmpA MosStrcmp
   #define MosStrcatA MosStrcat
   #define MosPrintfA MosPrintf
   #define MosVprintfA MosVprintf
   #define MosVsprintfA MosVsprintf
   #define MosSprintfA MosSprintf
   #define MosStrcpyA MosStrcpy
#endif

#if M_MIL_USE_UNICODE
   MIL_INT MFTYPE MosStrlenW(const wchar_t* str);
   wchar_t* MFTYPE MosStruprW(wchar_t* str);   
   wchar_t* MFTYPE MosStrlwrW(wchar_t* str);
   MIL_INT MFTYPE MosStrcmpW(const wchar_t* string1, const wchar_t* string2);
   wchar_t* MFTYPE MosStrcatW(wchar_t* dest, MIL_INT destsize, const wchar_t* source);
   MIL_INT MosPrintfW(const wchar_t* format, ...);
   MIL_INT MosVprintfW(const wchar_t* format, va_list args);
   MIL_INT MosVsprintfW(wchar_t* buf, MIL_INT bufsize, const wchar_t* format, va_list args);
   MIL_INT MosSprintfW(wchar_t* buf, MIL_INT bufsize, const wchar_t* format, ...);
   wchar_t* MFTYPE MosStrcpyW(wchar_t* dest, MIL_INT destsize, const wchar_t* source);

   #if M_MIL_UNICODE_API
      #define MosStrlen    MosStrlenW
      #define MosStrupr    MosStruprW 
      #define MosStrlwr    MosStrlwrW
      #define MosStrcmp    MosStrcmpW
      #define MosStrcat    MosStrcatW
      #define MosPrintf    MosPrintfW
      #define MosVprintf   MosVprintfW
      #define MosVsprintf  MosVsprintfW
      #define MosSprintf   MosSprintfW
      #define MosStrcpy    MosStrcpyW
   #else
      #define MosStrlen    MosStrlenA
      #define MosStrupr    MosStruprA
      #define MosStrlwr    MosStrlwrA
      #define MosStrcmp    MosStrcmpA
      #define MosStrcat    MosStrcatA
      #define MosPrintf    MosPrintfA
      #define MosVprintf   MosVprintfA
      #define MosVsprintf  MosVsprintfA
      #define MosSprintf   MosSprintfA
      #define MosStrcpy    MosStrcpyA
   #endif
#else
   MIL_INT MFTYPE MosStrlen(MIL_CONST_TEXT_PTR str);
   MIL_TEXT_PTR MFTYPE MosStrupr(MIL_TEXT_PTR str);
   MIL_TEXT_PTR MFTYPE MosStrlwr(MIL_TEXT_PTR str);
   MIL_INT MFTYPE MosStrcmp(MIL_CONST_TEXT_PTR string1, MIL_CONST_TEXT_PTR string2);
   MIL_TEXT_PTR MFTYPE MosStrcat(MIL_TEXT_PTR dest, MIL_INT destsize, MIL_CONST_TEXT_PTR source);   
   MIL_TEXT_PTR MFTYPE MosStrcpy(MIL_TEXT_PTR dest, MIL_INT destsize, MIL_CONST_TEXT_PTR source);
   MIL_INT MosSprintf(MIL_TEXT_PTR buf, MIL_INT bufsize, MIL_CONST_TEXT_PTR format, ...);
   MIL_INT MosPrintf(MIL_CONST_TEXT_PTR format, ...);   
   MIL_INT MosVprintf(MIL_CONST_TEXT_PTR format, va_list args);
   MIL_INT MosVsprintf(MIL_TEXT_PTR buf, MIL_INT bufsize, MIL_CONST_TEXT_PTR format, va_list args );
#endif


/***************************************************************************/
/* MISC FUNCTIONS:                                                         */
/***************************************************************************/
MIL_UINT MFTYPE  MPtrToInt           (void *pvPtr);

/* C++ directive if needed */
#ifdef __cplusplus
}
#endif

/***************************************************************************/
/* TOOLKIT FOR PSEUDO MIL FUNCTIONS DEVELOPMENT                            */
/***************************************************************************/
#include <milfunc.h> // milfunc.h handles the extern "C" and has some C++ code for safe-type support


#if !M_MIL_USE_64BIT
typedef struct _MILBUFFERINFO {
    long   StructSize;      // Size of the structure, in bytes. This member must be initialized before the structure is used. 
    long   SizeX;
    long   SizeY;
    long   SizeBand;
    long   SizeBit;
    long   Type;
    long   Attribute;
    long   Format;
    long   OffsetX;
    long   OffsetY;
    long   OffsetBand;
    long   AncestorOffsetX;
    long   AncestorOffsetY;
    long   AncestorOffsetBand;
    long   AncestorOffsetBit;
    long   Pitch;
    long   PitchByte;
    void*  HostPtr;
    void*  BandHostPtr[3];
    MIL_PHYS_DATA_PTR  PhysPtr;
    MIL_ID Id;
    MIL_ID ParentId;
    MIL_ID AncestorId;
    long   NativeId;
} MILBUFFERINFOOLD,* LPMILBUFFERINFOOLD;
#endif


#ifdef __cplusplus

inline MIL_ID MsysAlloc(MIL_CONST_TEXT_PTR SystemDescriptor,
                        MIL_INT SystemNum,
                        MIL_INT64 InitFlag,
                        MIL_ID* SystemIdPtr)
   {
   return MsysAlloc(M_DEFAULT,
                    SystemDescriptor,
                    SystemNum,
                    InitFlag,
                    SystemIdPtr);
   }

#if !M_COMPILING_MILDLL
inline MIL_ID MappAlloc(MIL_INT64 InitFlag,
                        MIL_ID* ContextAppIdPtr)
   {
   return MappAlloc(MIL_TEXT("M_DEFAULT"),
                    InitFlag,
                    ContextAppIdPtr);
   }
#endif

inline void MappControl(MIL_INT64 ControlType,
                        MIL_INT ControlFlag)
   {
   MappControl(M_DEFAULT, ControlType, ControlFlag);
   }

inline MIL_INT MappGetError(MIL_INT64 ErrorType,
                            void* ErrorPtr)
   {
   return MappGetError(M_DEFAULT, ErrorType, ErrorPtr);
   }

inline MIL_INT MappGetHookInfo(MIL_ID EventId,
                               MIL_INT64 InfoType,
                               void* UserVarPtr)
   {
   return MappGetHookInfo(M_DEFAULT, EventId, InfoType, UserVarPtr);
   }

inline void MappHookFunction(MIL_INT HookType,
                             MIL_APP_HOOK_FUNCTION_PTR HookHandlerPtr,
                             void* UserDataPtr)
   {
   MappHookFunction(M_DEFAULT, HookType, HookHandlerPtr, UserDataPtr);
   }

inline MIL_INT MappInquire(MIL_INT64 InquireType,
                           void* UserVarPtr)
   {
   return MappInquire(M_DEFAULT, InquireType, UserVarPtr);
   }

#if !M_LINUX_KERNEL
inline MIL_DOUBLE MappTimer(MIL_INT64 ControlType,
                            MIL_DOUBLE* TimePtr)
   {
   return MappTimer(M_DEFAULT, ControlType, TimePtr);
   }
#endif

inline void MappTimerVoid(MIL_INT64 Mode,
                          MIL_DOUBLE* Time)
   {
   MappTimerVoid(M_DEFAULT, Mode, Time);
   }
#endif



#if M_MIL_USE_64BIT
#ifdef __cplusplus
inline void MappControlObject(MIL_ID ContextAppId,
                              MIL_ID ObjectId,
                              MIL_INT64 ControlType,
                              MIL_INT32 ControlValue)
   {
   MappControlObjectInt64(ContextAppId, ObjectId, ControlType, ControlValue);
   };

inline void MappControlObject(MIL_ID ContextAppId,
                              MIL_ID ObjectId,
                              MIL_INT64 ControlType,
                              MIL_INT64 ControlValue)
   {
   MappControlObjectInt64(ContextAppId, ObjectId, ControlType, ControlValue);
   };

inline void MappControlObject(MIL_ID ContextAppId,
                              MIL_ID ObjectId,
                              MIL_INT64 ControlType,
                              MIL_DOUBLE ControlValue)
   {
   MappControlObjectDouble(ContextAppId, ObjectId, ControlType, ControlValue);
   };

struct MCppParameter
   {
#if !M_MIL_USE_LINUX
   MCppParameter(int          Param) { m_Param = (double)Param; }
   MCppParameter(unsigned int Param) { m_Param = (double)Param; }
#endif
   MCppParameter(MIL_INT8     Param) { m_Param = (double)Param; }
   MCppParameter(MIL_UINT8    Param) { m_Param = (double)Param; }
   MCppParameter(MIL_INT16    Param) { m_Param = (double)Param; }
   MCppParameter(MIL_UINT16   Param) { m_Param = (double)Param; }
   MCppParameter(MIL_INT32    Param) { m_Param = (double)Param; }
   MCppParameter(MIL_UINT32   Param) { m_Param = (double)Param; }
   MCppParameter(MIL_INT64    Param) { m_Param = (double)Param; }
   MCppParameter(MIL_UINT64   Param) { m_Param = (double)Param; }
   MCppParameter(float        Param) { m_Param = (double)Param; }
   MCppParameter(MIL_DOUBLE   Param) { m_Param = (double)Param; }

   double m_Param;
   };

//////////////////////////////////////////////////////////////
// MgraDot function definition when compiling c++ files
//////////////////////////////////////////////////////////////
inline void MgraDot(MIL_ID ContextGraId,
                    MIL_ID DstImageBufOrListGraId,
                    const MCppParameter& XPos,
                    const MCppParameter& YPos)
   {
   MgraDotDouble(ContextGraId, DstImageBufOrListGraId, XPos.m_Param, YPos.m_Param);
   }

inline void MgraDotEx(MIL_ID ContextGraId,
                      MIL_ID DstImageBufOrListGraId,
                      const MCppParameter& XPos,
                      const MCppParameter& YPos,
                      MIL_ID NativeCalibration,
                      MIL_INT64 NativeUnits,
                      MIL_INT64 ConvertMode)
   {
   MgraDotDoubleEx(ContextGraId, DstImageBufOrListGraId, XPos.m_Param, YPos.m_Param, NativeCalibration, NativeUnits, ConvertMode);
   }

//////////////////////////////////////////////////////////////
// MgraLine function definition when compiling c++ files
//////////////////////////////////////////////////////////////

inline void MgraLine(MIL_ID ContextGraId,
                     MIL_ID DstImageBufOrListGraId,
                     const MCppParameter& XStart,
                     const MCppParameter& YStart,
                     const MCppParameter& XEnd,
                     const MCppParameter& YEnd)
   {
   MgraLineDouble(ContextGraId, DstImageBufOrListGraId, XStart.m_Param, YStart.m_Param, XEnd.m_Param, YEnd.m_Param);
   }

inline void MgraLineEx(MIL_ID ContextGraId,
                     MIL_ID DstImageBufOrListGraId,
                     const MCppParameter& XStart,
                     const MCppParameter& YStart,
                     const MCppParameter& XEnd,
                     const MCppParameter& YEnd,
                     MIL_ID NativeCalibration,
                     MIL_INT64 NativeUnits,
                     MIL_INT64 ConvertMode)
   {
   MgraLineDoubleEx(ContextGraId, DstImageBufOrListGraId, XStart.m_Param, YStart.m_Param, XEnd.m_Param, YEnd.m_Param, NativeCalibration, NativeUnits, ConvertMode);
   }

//////////////////////////////////////////////////////////////
// MgraArc function definition when compiling c++ files
//////////////////////////////////////////////////////////////
inline void MgraArc(MIL_ID ContextGraId,
                    MIL_ID DstImageBufOrListGraId,
                    const MCppParameter& XCenter,
                    const MCppParameter& YCenter,
                    const MCppParameter& XRad,
                    const MCppParameter& YRad,
                    MIL_DOUBLE StartAngle,
                    MIL_DOUBLE EndAngle)
   {
   MgraArcDouble(ContextGraId, DstImageBufOrListGraId, XCenter.m_Param, YCenter.m_Param, XRad.m_Param, YRad.m_Param, StartAngle, EndAngle);
   }

inline void MgraArcEx(MIL_ID ContextGraId,
                    MIL_ID DstImageBufOrListGraId,
                    const MCppParameter& XCenter,
                    const MCppParameter& YCenter,
                    const MCppParameter& XRad,
                    const MCppParameter& YRad,
                    MIL_DOUBLE StartAngle,
                    MIL_DOUBLE EndAngle,
                    MIL_ID NativeCalibration,
                    MIL_INT64 NativeUnits,
                    MIL_INT64 ConvertMode)
   {
   MgraArcDoubleEx(ContextGraId, DstImageBufOrListGraId, XCenter.m_Param, YCenter.m_Param, XRad.m_Param, YRad.m_Param, StartAngle, EndAngle, NativeCalibration, NativeUnits, ConvertMode);
   }

//////////////////////////////////////////////////////////////
// MgraArcFill function definition when compiling c++ files
//////////////////////////////////////////////////////////////
inline void MgraArcFill(MIL_ID ContextGraId,
                        MIL_ID DstImageBufOrListGraId,
                        const MCppParameter& XCenter,
                        const MCppParameter& YCenter,
                        const MCppParameter& XRad,
                        const MCppParameter& YRad,
                        MIL_DOUBLE StartAngle,
                        MIL_DOUBLE EndAngle)
   {
   MgraArcFillDouble(ContextGraId, DstImageBufOrListGraId, XCenter.m_Param, YCenter.m_Param, XRad.m_Param, YRad.m_Param, StartAngle, EndAngle);
   }

inline void MgraArcFillEx(MIL_ID ContextGraId,
                        MIL_ID DstImageBufOrListGraId,
                        const MCppParameter& XCenter,
                        const MCppParameter& YCenter,
                        const MCppParameter& XRad,
                        const MCppParameter& YRad,
                        MIL_DOUBLE StartAngle,
                        MIL_DOUBLE EndAngle,
                        MIL_ID NativeCalibration,
                        MIL_INT64 NativeUnits,
                        MIL_INT64 ConvertMode)
   {
   MgraArcFillDoubleEx(ContextGraId, DstImageBufOrListGraId, XCenter.m_Param, YCenter.m_Param, XRad.m_Param, YRad.m_Param, StartAngle, EndAngle, NativeCalibration, NativeUnits, ConvertMode);
   }

//////////////////////////////////////////////////////////////
// MgraArcAngle function definition when compiling c++ files
//////////////////////////////////////////////////////////////
inline void MgraArcAngle(MIL_ID ContextGraId,
                        MIL_ID DstImageBufOrListGraId,
                        const MCppParameter& XCenter,
                        const MCppParameter& YCenter,
                        const MCppParameter& XRad,
                        const MCppParameter& YRad,
                        MIL_DOUBLE StartAngle,
                        MIL_DOUBLE EndAngle,
                        MIL_DOUBLE XAxisAngle,
                        MIL_INT64  ControlFlag)
   {
   MgraArcAngleDouble(ContextGraId, DstImageBufOrListGraId, XCenter.m_Param, YCenter.m_Param, XRad.m_Param, YRad.m_Param, StartAngle, EndAngle, XAxisAngle, ControlFlag);
   }

inline void MgraArcAngleEx(MIL_ID ContextGraId,
                        MIL_ID DstImageBufOrListGraId,
                        const MCppParameter& XCenter,
                        const MCppParameter& YCenter,
                        const MCppParameter& XRad,
                        const MCppParameter& YRad,
                        MIL_DOUBLE StartAngle,
                        MIL_DOUBLE EndAngle,
                        MIL_DOUBLE XAxisAngle,
                        MIL_INT64 ControlFlag,
                        MIL_ID NativeCalibration,
                        MIL_INT64 NativeUnits,
                        MIL_INT64 ConvertMode)
   {
   MgraArcAngleDoubleEx(ContextGraId, DstImageBufOrListGraId, XCenter.m_Param, YCenter.m_Param, XRad.m_Param, YRad.m_Param, StartAngle, EndAngle, XAxisAngle, ControlFlag, NativeCalibration, NativeUnits, ConvertMode);
   }

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
// MgraRingSector function definition when compiling c++ files
//////////////////////////////////////////////////////////////
inline void MFTYPE MgraRingSector(MIL_ID     ContextGraId             ,
                                  MIL_ID     DestImageBufIdOrGraListId,
                                  const MCppParameter&  XCenter       ,
                                  const MCppParameter&  YCenter       ,
                                  const MCppParameter&  XInnerRadius  ,
                                  const MCppParameter&  YInnerRadius  ,
                                  const MCppParameter&  XOuterRadius  ,
                                  const MCppParameter&  YOuterRadius  ,
                                  MIL_DOUBLE StartAngle               ,
                                  MIL_DOUBLE EndAngle                 ,
                                  MIL_DOUBLE XAxisAngle               ,
                                  MIL_INT64  ControlFlag              )
   {
   MgraRingSectorDouble(ContextGraId, DestImageBufIdOrGraListId, XCenter.m_Param, YCenter.m_Param, XInnerRadius.m_Param, YInnerRadius.m_Param, XOuterRadius.m_Param, YOuterRadius.m_Param, StartAngle, EndAngle, XAxisAngle, ControlFlag);
   }

inline void MFTYPE MgraRingSectorEx(MIL_ID     ContextGraId             ,
                                    MIL_ID     DestImageBufIdOrGraListId,
                                    const MCppParameter&  XCenter       ,
                                    const MCppParameter&  YCenter       ,
                                    const MCppParameter&  XInnerRadius  ,
                                    const MCppParameter&  YInnerRadius  ,
                                    const MCppParameter&  XOuterRadius  ,
                                    const MCppParameter&  YOuterRadius  ,
                                    MIL_DOUBLE StartAngle               ,
                                    MIL_DOUBLE EndAngle                 ,
                                    MIL_DOUBLE XAxisAngle               ,
                                    MIL_INT64  ControlFlag              ,
                                    MIL_ID     NativeCalibration        ,
                                    MIL_INT64  NativeUnits              ,
                                    MIL_INT64  ConvertMode          )
   {
   MgraRingSectorDoubleEx(ContextGraId, DestImageBufIdOrGraListId, XCenter.m_Param, YCenter.m_Param, XInnerRadius.m_Param, YInnerRadius.m_Param, XOuterRadius.m_Param, YOuterRadius.m_Param, StartAngle, EndAngle, XAxisAngle, ControlFlag, NativeCalibration, NativeUnits, ConvertMode);
   }

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
// MgraRect function definition when compiling c++ files
//////////////////////////////////////////////////////////////
inline void MgraRect(MIL_ID ContextGraId,
                     MIL_ID DstImageBufOrListGraId,
                     const MCppParameter& XStart,
                     const MCppParameter& YStart,
                     const MCppParameter& XEnd,
                     const MCppParameter& YEnd)
   {
   MgraRectDouble(ContextGraId, DstImageBufOrListGraId, XStart.m_Param, YStart.m_Param, XEnd.m_Param, YEnd.m_Param);
   }

inline void MgraRectEx(MIL_ID ContextGraId,
                      MIL_ID DstImageBufOrListGraId,
                      const MCppParameter& XStart,
                      const MCppParameter& YStart,
                      const MCppParameter& XEnd,
                      const MCppParameter& YEnd,
                      MIL_ID NativeCalibration,
                      MIL_INT64 NativeUnits,
                      MIL_INT64 ConvertMode)
   {
   MgraRectDoubleEx(ContextGraId, DstImageBufOrListGraId, XStart.m_Param, YStart.m_Param, XEnd.m_Param, YEnd.m_Param, NativeCalibration, NativeUnits, ConvertMode);
   }

//////////////////////////////////////////////////////////////
// MgraRectAngle function definition when compiling c++ files
//////////////////////////////////////////////////////////////
inline void MgraRectAngle(MIL_ID ContextGraId,
                          MIL_ID DestImageBufIdOrGraListId,
                          const MCppParameter& XPos,
                          const MCppParameter& YPos,
                          const MCppParameter& Width,
                          const MCppParameter& Height,
                          MIL_DOUBLE Angle,
                          MIL_INT64 ControlFlag)
   {
   MgraRectAngleDouble(ContextGraId, DestImageBufIdOrGraListId, 
      XPos.m_Param, YPos.m_Param, 
      Width.m_Param, Height.m_Param, 
      Angle, ControlFlag);
   }

inline void MgraRectAngleEx(MIL_ID ContextGraId,
                          MIL_ID DestImageBufIdOrGraListId,
                          const MCppParameter& XPos,
                          const MCppParameter& YPos,
                          const MCppParameter& Width,
                          const MCppParameter& Height,
                          MIL_DOUBLE Angle,
                          MIL_INT64 ControlFlag,
                          MIL_ID NativeCalibration,
                          MIL_INT64 NativeUnits,
                          MIL_INT64 ConvertMode)
   {
   MgraRectAngleDoubleEx(ContextGraId, DestImageBufIdOrGraListId, 
      XPos.m_Param, YPos.m_Param, 
      Width.m_Param, Height.m_Param, 
      Angle, ControlFlag, NativeCalibration, NativeUnits, ConvertMode);
   }

//////////////////////////////////////////////////////////////
// MgraRectFill function definition when compiling c++ files
//////////////////////////////////////////////////////////////
inline void MgraRectFill(MIL_ID ContextGraId,
                         MIL_ID DstImageBufOrListGraId,
                         const MCppParameter& XStart,
                         const MCppParameter& YStart,
                         const MCppParameter& XEnd,
                         const MCppParameter& YEnd)
   {
   MgraRectFillDouble(ContextGraId, DstImageBufOrListGraId, XStart.m_Param, YStart.m_Param, XEnd.m_Param, YEnd.m_Param);
   }

inline void MgraRectFillEx(MIL_ID ContextGraId,
                         MIL_ID DstImageBufOrListGraId,
                         const MCppParameter& XStart,
                         const MCppParameter& YStart,
                         const MCppParameter& XEnd,
                         const MCppParameter& YEnd,
                         MIL_ID NativeCalibration,
                         MIL_INT64 NativeUnits,
                         MIL_INT64 ConvertMode)
   {
   MgraRectFillDoubleEx(ContextGraId, DstImageBufOrListGraId, XStart.m_Param, YStart.m_Param, XEnd.m_Param, YEnd.m_Param, NativeCalibration, NativeUnits, ConvertMode);
   }

//////////////////////////////////////////////////////////////
// MgraFill function definition when compiling c++ files
//////////////////////////////////////////////////////////////
inline void MgraFill(MIL_ID ContextGraId,
                     MIL_ID ImageId,
                     const MCppParameter& XStart,
                     const MCppParameter& YStart)
   {
   MgraFillDouble(ContextGraId, ImageId, XStart.m_Param, YStart.m_Param);
   }

inline void MgraFillEx(MIL_ID ContextGraId,
                       MIL_ID ImageId,
                       const MCppParameter& XStart,
                       const MCppParameter& YStart,
                       MIL_ID NativeCalibration,
                       MIL_INT64 NativeUnits,
                       MIL_INT64 ConvertMode)
   {
   MgraFillDoubleEx(ContextGraId, ImageId, XStart.m_Param, YStart.m_Param, NativeCalibration, NativeUnits, ConvertMode);
   }

//////////////////////////////////////////////////////////////
// MgraTextW function definition when compiling c++ files
//////////////////////////////////////////////////////////////
#if M_MIL_USE_UNICODE

   inline void MgraTextA(MIL_ID ContextGraId,
                         MIL_ID DstImageBufOrListGraId,
                         const MCppParameter& XStart,
                         const MCppParameter& YStart,
                         const char* StringPtr)
      {
      MgraTextADouble(ContextGraId, DstImageBufOrListGraId, XStart.m_Param, YStart.m_Param, StringPtr);
      }

   inline void MgraTextAEx(MIL_ID ContextGraId,
                         MIL_ID DstImageBufOrListGraId,
                         const MCppParameter& XStart,
                         const MCppParameter& YStart,
                         const char* StringPtr,
                         MIL_ID NativeCalibration,
                         MIL_INT64 NativeUnits,
                         MIL_INT64 ConvertMode)
      {
      MgraTextADoubleEx(ContextGraId, DstImageBufOrListGraId, XStart.m_Param, YStart.m_Param, StringPtr, NativeCalibration, NativeUnits, ConvertMode);
      }

   inline void MgraTextW(MIL_ID ContextGraId,
                         MIL_ID DstImageBufOrListGraId,
                         const MCppParameter& XStart,
                         const MCppParameter& YStart,
                         MIL_CONST_TEXT_PTR StringPtr)
      {
      MgraTextWDouble(ContextGraId, DstImageBufOrListGraId, XStart.m_Param, YStart.m_Param, StringPtr);
      }

   inline void MgraTextWEx(MIL_ID ContextGraId,
                         MIL_ID DstImageBufOrListGraId,
                         const MCppParameter& XStart,
                         const MCppParameter& YStart,
                         MIL_CONST_TEXT_PTR StringPtr,
                         MIL_ID NativeCalibration,
                         MIL_INT64 NativeUnits,
                         MIL_INT64 ConvertMode)
      {
      MgraTextWDoubleEx(ContextGraId, DstImageBufOrListGraId, XStart.m_Param, YStart.m_Param, StringPtr, NativeCalibration, NativeUnits, ConvertMode);
      }

#else // #if M_MIL_USE_UNICODE

   inline void MgraText(MIL_ID ContextGraId,
                        MIL_ID DstImageBufOrListGraId,
                        const MCppParameter& XStart,
                        const MCppParameter& YStart,
                        MIL_CONST_TEXT_PTR StringPtr)
      {
      MgraTextDouble(ContextGraId, DstImageBufOrListGraId, XStart.m_Param, YStart.m_Param, StringPtr);
      }

   inline void MgraTextEx(MIL_ID ContextGraId,
                          MIL_ID DstImageBufOrListGraId,
                          const MCppParameter& XStart,
                          const MCppParameter& YStart,
                          MIL_CONST_TEXT_PTR StringPtr,
                          MIL_ID NativeCalibration,
                          MIL_INT64 NativeUnits,
                          MIL_INT64 ConvertMode)
      {
      MgraTextDoubleEx(ContextGraId, DstImageBufOrListGraId, XStart.m_Param, YStart.m_Param, StringPtr, NativeCalibration, NativeUnits, ConvertMode);
      }

#endif // #if M_MIL_USE_UNICODE #else

#if !M_MIL_USE_LINUX || M_COMPILING_MILDLL
inline void MseqControl(MIL_ID ContextSeqId,
                        MIL_INT SequenceIndex,
                        MIL_INT64 ControlType,
                        int ControlValue)
      {
      MseqControlInt64(ContextSeqId, SequenceIndex, ControlType, ControlValue);
      }
#if !M_MIL_USE_LINUX
inline void MseqControl(MIL_ID ContextSeqId,
                        MIL_INT SequenceIndex,
                        MIL_INT64 ControlType,
                        MIL_INT32 ControlValue)
   {
   MseqControlInt64(ContextSeqId, SequenceIndex, ControlType, ControlValue);
   }
#endif
inline void MseqControl(MIL_ID ContextSeqId,
                        MIL_INT SequenceIndex,
                        MIL_INT64 ControlType,
                        MIL_INT64 ControlValue)
   {
   MseqControlInt64(ContextSeqId, SequenceIndex, ControlType, ControlValue);
   }

inline void MseqControl(MIL_ID ContextSeqId,
                        MIL_INT SequenceIndex,
                        MIL_INT64 ControlType,
                        MIL_DOUBLE ControlValue)
   {
   MseqControlDouble(ContextSeqId, SequenceIndex, ControlType, ControlValue);
   }
#endif 


//////////////////////////////////////////////////////////////
// MbufControl function definition when compiling c++ files
//////////////////////////////////////////////////////////////
#if !M_MIL_USE_LINUX
inline void MbufControl(MIL_ID BufId,
                        MIL_INT64 ControlType,
                        int ControlValue)
   {
   MbufControlInt64(BufId, ControlType, ControlValue);
   };
#endif

inline void MbufControl(MIL_ID BufId,
                        MIL_INT64 ControlType,
                        MIL_INT32 ControlValue)
   {
   MbufControlInt64(BufId, ControlType, ControlValue);
   };

inline void MbufControl(MIL_ID BufId,
                        MIL_INT64 ControlType,
                        MIL_INT64 ControlValue)
   {
   MbufControlInt64(BufId, ControlType, ControlValue);
   };

inline void MbufControl(MIL_ID BufId,
                        MIL_INT64 ControlType,
                        MIL_DOUBLE ControlValue)
   {
   MbufControlDouble(BufId, ControlType, ControlValue);
   };

//////////////////////////////////////////////////////////////
// MdigControl function definition when compiling c++ files
//////////////////////////////////////////////////////////////
#if !M_MIL_USE_LINUX
inline void MdigControl(MIL_ID DigId,
                        MIL_INT64 ControlType,
                        int ControlValue)
   {
   MdigControlInt64(DigId, ControlType, ControlValue);
   };
#endif

inline void MdigControl(MIL_ID DigId,
                        MIL_INT64 ControlType,
                        MIL_INT32 ControlValue)
   {
   MdigControlInt64(DigId, ControlType, ControlValue);
   };

inline void MdigControl(MIL_ID DigId,
                        MIL_INT64 ControlType,
                        MIL_INT64 ControlValue)
   {
   MdigControlInt64(DigId, ControlType, ControlValue);
   };

inline void MdigControl(MIL_ID DigId,
                        MIL_INT64 ControlType,
                        MIL_DOUBLE ControlValue)
   {
   MdigControlDouble(DigId, ControlType, ControlValue);
   };

//////////////////////////////////////////////////////////////
// MsysIoControl function definition when compiling c++ files
//////////////////////////////////////////////////////////////
#if !M_MIL_USE_LINUX
inline void MsysIoControl(MIL_ID IoObjectId,
                          MIL_INT64 ControlType,
                          int ControlValue)
   {
   MsysIoControlInt64(IoObjectId, ControlType, ControlValue);
   };
#endif

inline void MsysIoControl(MIL_ID IoObjectId,
                          MIL_INT64 ControlType,
                          MIL_INT32 ControlValue)
   {
   MsysIoControlInt64(IoObjectId, ControlType, ControlValue);
   };

inline void MsysIoControl(MIL_ID IoObjectId,
                          MIL_INT64 ControlType,
                          MIL_INT64 ControlValue)
   {
   MsysIoControlInt64(IoObjectId, ControlType, ControlValue);
   };

inline void MsysIoControl(MIL_ID IoObjectId,
                          MIL_INT64 ControlType,
                          MIL_DOUBLE ControlValue)
   {
   MsysIoControlDouble(IoObjectId, ControlType, ControlValue);
   };

//////////////////////////////////////////////////////////////
// MthrControl function definition when compiling c++ files
//////////////////////////////////////////////////////////////
#if !M_MIL_USE_LINUX
inline void MthrControl(MIL_ID ThreadEventOrMutexId,
                        MIL_INT64 ControlType,
                        int ControlValue)
   {
   MthrControlInt64(ThreadEventOrMutexId, ControlType, ControlValue);
   };
#endif

inline void MthrControl(MIL_ID ThreadEventOrMutexId,
                        MIL_INT64 ControlType,
                        MIL_INT32 ControlValue)
   {
   MthrControlInt64(ThreadEventOrMutexId, ControlType, ControlValue);
   };

inline void MthrControl(MIL_ID ThreadEventOrMutexId,
                        MIL_INT64 ControlType,
                        MIL_INT64 ControlValue)
   {
   MthrControlInt64(ThreadEventOrMutexId, ControlType, ControlValue);
   };

inline void MthrControl(MIL_ID ThreadEventOrMutexId,
                        MIL_INT64 ControlType,
                        MIL_DOUBLE ControlValue)
   {
   MthrControlDouble(ThreadEventOrMutexId, ControlType, ControlValue);
   };

//////////////////////////////////////////////////////////////
// MgraControlList function definition when compiling c++ files
//////////////////////////////////////////////////////////////
#if !M_MIL_USE_LINUX
inline void MFTYPE    MgraControlList      (MIL_ID GraListId,
                                            MIL_INT LabelOrIndex,
                                            MIL_INT SubIndex,
                                            MIL_INT64 ControlType,
                                            int ControlValue)
   {
   MgraControlListInt64(GraListId, LabelOrIndex, SubIndex, ControlType, ControlValue);
   }
#endif

inline void MFTYPE    MgraControlList      (MIL_ID GraListId,
                                            MIL_INT LabelOrIndex,
                                            MIL_INT SubIndex,
                                            MIL_INT64 ControlType,
                                            MIL_INT32 ControlValue)
   {
   MgraControlListInt64(GraListId, LabelOrIndex, SubIndex, ControlType, ControlValue);
   }

inline void MFTYPE    MgraControlList      (MIL_ID GraListId,
                                            MIL_INT LabelOrIndex,
                                            MIL_INT SubIndex,
                                            MIL_INT64 ControlType,
                                            MIL_INT64 ControlValue)
   {
   MgraControlListInt64(GraListId, LabelOrIndex, SubIndex, ControlType, ControlValue);
   }

inline void MFTYPE    MgraControlList      (MIL_ID GraListId,
                                            MIL_INT LabelOrIndex,
                                            MIL_INT SubIndex,
                                            MIL_INT64 ControlType,
                                            MIL_DOUBLE ControlValue)
   {
   MgraControlListDouble(GraListId, LabelOrIndex, SubIndex, ControlType, ControlValue);
   }

//////////////////////////////////////////////////////////////
// MgraControl function definition when compiling c++ files
//////////////////////////////////////////////////////////////
#if !M_MIL_USE_LINUX
inline void MFTYPE    MgraControl (MIL_ID GraListId,
                                   MIL_INT64 ControlType,
                                   int ControlValue)
   {
   MgraControlInt64(GraListId, ControlType, ControlValue);
   }
#endif

inline void MFTYPE    MgraControl (MIL_ID GraListId,
                                   MIL_INT64 ControlType,
                                   MIL_INT32 ControlValue)
   {
   MgraControlInt64(GraListId, ControlType, ControlValue);
   }

inline void MFTYPE    MgraControl (MIL_ID GraListId,
                                   MIL_INT64 ControlType,
                                   MIL_INT64 ControlValue)
   {
   MgraControlInt64(GraListId, ControlType, ControlValue);
   }

inline void MFTYPE    MgraControl (MIL_ID GraListId,
                                   MIL_INT64 ControlType,
                                   MIL_DOUBLE ControlValue)
   {
   MgraControlDouble(GraListId, ControlType, ControlValue);
   }

#else
//////////////////////////////////////////////////////////////
// For C file, call the default function, i.e. Int64 one
//////////////////////////////////////////////////////////////
#define MgraDot         MgraDotInt64
#define MgraDotEx       MgraDotInt64Ex
#define MgraLine        MgraLineInt64
#define MgraLineEx      MgraLineInt64Ex
#define MgraArc         MgraArcInt64
#define MgraArcEx       MgraArcInt64Ex
#define MgraArcFill     MgraArcFillInt64
#define MgraArcFillEx   MgraArcFillInt64Ex
#define MgraRect        MgraRectInt64
#define MgraRectEx      MgraRectInt64Ex
#define MgraRectFill    MgraRectFillInt64
#define MgraRectFillEx  MgraRectFillInt64Ex
#define MgraRectAngle   MgraRectAngleInt64
#define MgraRectAngleEx MgraRectAngleInt64Ex
#if M_MIL_USE_UNICODE 
#define MgraFontW       MgraFont
#define MgraTextW       MgraTextWInt64
#define MgraTextWEx     MgraTextWInt64Ex
#define MgraTextA       MgraTextAInt64
#define MgraTextAEx     MgraTextAInt64Ex
#else
#define MgraText        MgraTextInt64
#define MgraTextEx      MgraTextInt64Ex
#endif
#define MseqControl     MseqControlInt64
#define MbufControl     MbufControlInt64
#define MdigControl     MdigControlInt64
#define MthrControl     MthrControlInt64
#define MgraControlList MgraControlListInt64

#endif // __cplusplus
#endif // M_MIL_USE_64BIT

#if(defined(__cplusplus))

//////////////////////////////////////////////////////////////
// MbufControl function definition when compiling c++ files
//////////////////////////////////////////////////////////////
inline void MFTYPE   MbufClear  (MIL_ID DstImageBufId,
                                 MIL_DOUBLE Color)
   {
   MbufClearDouble(DstImageBufId, Color);
   }

//////////////////////////////////////////////////////////////
// MbufControlNeighborhood function definition when compiling c++ files
//////////////////////////////////////////////////////////////
inline void MFTYPE MbufControlNeighborhood(MIL_ID BufferId,
                                           MIL_INT64 Flag,
                                           MIL_INT Value)
   {
   MbufControlNeighborhoodMIL_INT(BufferId, Flag, Value);
   }

//////////////////////////////////////////////////////////////
// MbufControlArea function definition when compiling c++ files
//////////////////////////////////////////////////////////////
inline void MFTYPE   MbufControlArea(MIL_ID  BufId,
                                     MIL_INT OffsetX,
                                     MIL_INT OffsetY,
                                     MIL_INT SizeX,
                                     MIL_INT SizeY,
                                     MIL_INT Band,
                                     MIL_INT64 ControlType,
                                     MIL_DOUBLE ControlValue)
   {
   MbufControlAreaDouble(BufId, OffsetX, OffsetY, SizeX, SizeY, Band, ControlType, ControlValue);
   }

//////////////////////////////////////////////////////////////
// MbufClearCond function definition when compiling c++ files
//////////////////////////////////////////////////////////////
inline void MFTYPE    MbufClearCond (MIL_ID BufId,
                                     MIL_DOUBLE RedOrMonoVal,
                                     MIL_DOUBLE GreenVal,
                                     MIL_DOUBLE BlueVal,
                                     MIL_ID CondBufId,
                                     MIL_INT64 Condition,
                                     MIL_DOUBLE CondValue)
   {
   MbufClearCondDouble(BufId, RedOrMonoVal, GreenVal, BlueVal, CondBufId, Condition, CondValue);
   }

//////////////////////////////////////////////////////////////
// MbufCopyCond function definition when compiling c++ files
//////////////////////////////////////////////////////////////
inline void MFTYPE    MbufCopyCond  (MIL_ID SrcBufId,
                                     MIL_ID DestBufId,
                                     MIL_ID CondBufId,
                                     MIL_INT64 Condition,
                                     MIL_DOUBLE CondValue)
   {
   MbufCopyCondDouble(SrcBufId, DestBufId, CondBufId, Condition, CondValue);
   }

//////////////////////////////////////////////////////////////
// MbufSetRegion function definition when compiling c++ files
//////////////////////////////////////////////////////////////
inline void MFTYPE    MbufSetRegion    (MIL_ID BufId,
                                        MIL_ID ImageOrGraphicListId,
                                        MIL_INT64 Label,
                                        MIL_INT64 Operation,
                                        MIL_DOUBLE Param)
   {
   MbufSetRegionDouble(BufId, ImageOrGraphicListId, Label, Operation, Param);
   }

//////////////////////////////////////////////////////////////
// MgraBackColor function definition when compiling c++ files
//////////////////////////////////////////////////////////////
inline void MFTYPE    MgraBackColor (MIL_ID ContextGraId,
                                     MIL_DOUBLE BackgroundColor)
   {
   MgraBackColorDouble(ContextGraId, BackgroundColor);
   }

//////////////////////////////////////////////////////////////
// MgraColor function definition when compiling c++ files
//////////////////////////////////////////////////////////////
inline void MFTYPE   MgraColor      (MIL_ID ContextGraId,
                                     MIL_DOUBLE ForegroundColor)
   {
   MgraColorDouble(ContextGraId, ForegroundColor);
   }

//////////////////////////////////////////////////////////////
// MsysControl function definition when compiling c++ files
//////////////////////////////////////////////////////////////
#if !M_COMPILING_MILDLL
#if !M_MIL_USE_LINUX || !M_MIL_USE_64BIT
inline void MsysControl(MIL_ID SystemId,
                        MIL_INT64 ControlType,
                        int ControlValue)
   {
   MsysControlInt64(SystemId, ControlType, ControlValue);
   };
#endif

inline void MsysControl(MIL_ID SystemId,
                        MIL_INT64 ControlType,
                        MIL_INT32 ControlValue)
   {
   MsysControlInt64(SystemId, ControlType, ControlValue);
   };

inline void MsysControl(MIL_ID SystemId,
                        MIL_INT64 ControlType,
                        MIL_INT64 ControlValue)
   {
   MsysControlInt64(SystemId, ControlType, ControlValue);
   };

inline void MsysControl(MIL_ID SystemId,
                        MIL_INT64 ControlType,
                        MIL_UINT32 ControlValue)
   {
   MsysControlInt64(SystemId, ControlType, ControlValue);
   };

inline void MsysControl(MIL_ID SystemId,
                        MIL_INT64 ControlType,
                        MIL_DOUBLE ControlValue)
   {
   MsysControlDouble(SystemId, ControlType, ControlValue);
   };
#endif //#if !M_COMPILING_MILDLL

#else //#if(!defined(__cplusplus))

#define  MbufClear                     MbufClearDouble
#define  MbufControlNeighborhood       MbufControlNeighborhoodMIL_INT
#define  MbufControlArea               MbufControlAreaDouble
#define  MbufClearCond                 MbufClearCondDouble
#define  MbufCopyCond                  MbufCopyCondDouble
#define  MbufSetRegion                 MbufSetRegionDouble
#define  MgraBackColor                 MgraBackColorDouble
#define  MgraColor                     MgraColorDouble
#define  MgraControl                   MgraControlDouble
#define  MsysControl                   MsysControlDouble

#endif   //#if(defined(__cplusplus))

// These functions have 3 versions of Mgra API even in 32 bits.
#if (defined __cplusplus)

//////////////////////////////////////////////////////////////
// MgraDots function definition when compiling c++ files
//////////////////////////////////////////////////////////////
inline void MgraDots(MIL_ID ContextGraId,
                     MIL_ID DstImageBufOrListGraId,
                     MIL_INT  NumberOfDots,
                     const MIL_INT32 *PosXArrayPtr,
                     const MIL_INT32 *PosYArrayPtr,
                     MIL_INT64 ControlFlag)
   {
   MgraDotsInt32(ContextGraId, DstImageBufOrListGraId, NumberOfDots, PosXArrayPtr, PosYArrayPtr, ControlFlag);
   }

inline void MgraDotsEx(MIL_ID ContextGraId,
                       MIL_ID DstImageBufOrListGraId,
                       MIL_INT  NumberOfDots,
                       const MIL_INT32 *PosXArrayPtr,
                       const MIL_INT32 *PosYArrayPtr,
                       MIL_INT64 ControlFlag,
                       MIL_ID NativeCalibration,
                       MIL_INT64 NativeUnits,
                       MIL_INT64 ConvertMode)
   {
   MgraDotsInt32Ex(ContextGraId, DstImageBufOrListGraId, NumberOfDots, PosXArrayPtr, PosYArrayPtr, ControlFlag, NativeCalibration, NativeUnits, ConvertMode);
   }

inline void MgraDots(MIL_ID ContextGraId,
                     MIL_ID DstImageBufOrListGraId,
                     MIL_INT  NumberOfDots,
                     const MIL_INT64 *PosXArrayPtr,
                     const MIL_INT64 *PosYArrayPtr,
                     MIL_INT64 ControlFlag)
   {
   MgraDotsInt64(ContextGraId, DstImageBufOrListGraId, NumberOfDots, PosXArrayPtr, PosYArrayPtr, ControlFlag);
   }

inline void MgraDotsEx(MIL_ID ContextGraId,
                       MIL_ID DstImageBufOrListGraId,
                       MIL_INT  NumberOfDots,
                       const MIL_INT64 *PosXArrayPtr,
                       const MIL_INT64 *PosYArrayPtr,
                       MIL_INT ControlFlag,
                       MIL_ID NativeCalibration,
                       MIL_INT64 NativeUnits,
                       MIL_INT64 ConvertMode)
   {
   MgraDotsInt64Ex(ContextGraId, DstImageBufOrListGraId, NumberOfDots, PosXArrayPtr, PosYArrayPtr, ControlFlag, NativeCalibration, NativeUnits, ConvertMode);
   }

inline void MgraDots(MIL_ID ContextGraId,
                     MIL_ID DstImageBufOrListGraId,
                     MIL_INT  NumberOfDots,
                     const MIL_DOUBLE *PosXArrayPtr,
                     const MIL_DOUBLE *PosYArrayPtr,
                     MIL_INT64 ControlFlag)
   {
   MgraDotsDouble(ContextGraId, DstImageBufOrListGraId, NumberOfDots, PosXArrayPtr, PosYArrayPtr, ControlFlag);
   }

inline void MgraDotsEx(MIL_ID ContextGraId,
                       MIL_ID DstImageBufOrListGraId,
                       MIL_INT  NumberOfDots,
                       const MIL_DOUBLE *PosXArrayPtr,
                       const MIL_DOUBLE *PosYArrayPtr,
                       MIL_INT ControlFlag,
                       MIL_ID NativeCalibration,
                       MIL_INT64 NativeUnits,
                       MIL_INT64 ConvertMode)
   {
   MgraDotsDoubleEx(ContextGraId, DstImageBufOrListGraId, NumberOfDots, PosXArrayPtr, PosYArrayPtr, ControlFlag, NativeCalibration, NativeUnits, ConvertMode);
   }

//////////////////////////////////////////////////////////////
// MgraLines function definition when compiling c++ files
//////////////////////////////////////////////////////////////
inline void MgraLines(MIL_ID ContextGraId,
                      MIL_ID DstImageBufOrListGraId,
                      MIL_INT NumberOfLinesOrVertices,
                      const MIL_INT32 *XPtr,
                      const MIL_INT32 *YPtr,
                      const MIL_INT32 *X2Ptr,
                      const MIL_INT32 *Y2Ptr,
                      MIL_INT64 ControlFlag)
   {
   MgraLinesInt32(ContextGraId, DstImageBufOrListGraId, NumberOfLinesOrVertices, XPtr, YPtr, X2Ptr, Y2Ptr, ControlFlag);
   }

inline void MgraLinesEx(MIL_ID ContextGraId,
                        MIL_ID DstImageBufOrListGraId,
                        MIL_INT NumberOfLinesOrVertices,
                        const MIL_INT32 *XPtr,
                        const MIL_INT32 *YPtr,
                        const MIL_INT32 *X2Ptr,
                        const MIL_INT32 *Y2Ptr,
                        MIL_INT64 ControlFlag,
                        MIL_ID NativeCalibration,
                        MIL_INT64 NativeUnits,
                        MIL_INT64 ConvertMode)
   {
   MgraLinesInt32Ex(ContextGraId, DstImageBufOrListGraId, NumberOfLinesOrVertices, XPtr, YPtr, X2Ptr, Y2Ptr, ControlFlag, NativeCalibration, NativeUnits, ConvertMode);
   }

inline void MgraLines(MIL_ID ContextGraId,
                      MIL_ID DstImageBufOrListGraId,
                      MIL_INT NumberOfLinesOrVertices,
                      const MIL_INT64 *XPtr,
                      const MIL_INT64 *YPtr,
                      const MIL_INT64 *X2Ptr,
                      const MIL_INT64 *Y2Ptr,
                      MIL_INT64 ControlFlag)
   {
   MgraLinesInt64(ContextGraId, DstImageBufOrListGraId, NumberOfLinesOrVertices, XPtr, YPtr, X2Ptr, Y2Ptr, ControlFlag);
   }

inline void MgraLinesEx(MIL_ID ContextGraId,
                        MIL_ID DstImageBufOrListGraId,
                        MIL_INT NumberOfLinesOrVertices,
                        const MIL_INT64 *XPtr,
                        const MIL_INT64 *YPtr,
                        const MIL_INT64 *X2Ptr,
                        const MIL_INT64 *Y2Ptr,
                        MIL_INT64 ControlFlag,
                        MIL_ID NativeCalibration,
                        MIL_INT64 NativeUnits,
                        MIL_INT64 ConvertMode)
   {
   MgraLinesInt64Ex(ContextGraId, DstImageBufOrListGraId, NumberOfLinesOrVertices, XPtr, YPtr, X2Ptr, Y2Ptr, ControlFlag, NativeCalibration, NativeUnits, ConvertMode);
   }

inline void MgraLines(MIL_ID ContextGraId,
                      MIL_ID DstImageBufOrListGraId,
                      MIL_INT NumberOfLinesOrVertices,
                      const MIL_DOUBLE *XPtr,
                      const MIL_DOUBLE *YPtr,
                      const MIL_DOUBLE *X2Ptr,
                      const MIL_DOUBLE *Y2Ptr,
                      MIL_INT64 ControlFlag)
   {
   MgraLinesDouble(ContextGraId, DstImageBufOrListGraId, NumberOfLinesOrVertices, XPtr, YPtr, X2Ptr, Y2Ptr, ControlFlag);
   }

inline void MgraLinesEx(MIL_ID ContextGraId,
                        MIL_ID DstImageBufOrListGraId,
                        MIL_INT NumberOfLinesOrVertices,
                        const MIL_DOUBLE *XPtr,
                        const MIL_DOUBLE *YPtr,
                        const MIL_DOUBLE *X2Ptr,
                        const MIL_DOUBLE *Y2Ptr,
                        MIL_INT64 ControlFlag,
                        MIL_ID NativeCalibration,
                        MIL_INT64 NativeUnits,
                        MIL_INT64 ConvertMode)
   {
   MgraLinesDoubleEx(ContextGraId, DstImageBufOrListGraId, NumberOfLinesOrVertices, XPtr, YPtr, X2Ptr, Y2Ptr, ControlFlag, NativeCalibration, NativeUnits, ConvertMode);
   }

#else //#if(defined(__cplusplus))

//////////////////////////////////////////////////////////////
// For C file, call the default function, i.e. MIL_INT one
// for backward compatibility. For MgraSymbols, we have no
// backward compatibility issues, prefer MgraSymbolsDouble.
//////////////////////////////////////////////////////////////
#if M_MIL_USE_64BIT
   #define MgraDots     MgraDotsInt64
   #define MgraDotsEx   MgraDotsInt64Ex
   #define MgraLines    MgraLinesInt64
   #define MgraLinesEx  MgraLinesInt64Ex   
#else
   #define MgraDots     MgraDotsInt32
   #define MgraDotsEx   MgraDotsInt32Ex
   #define MgraLines    MgraLinesInt32
   #define MgraLinesEx  MgraLinesInt32Ex
#endif

#endif // #if(defined(__cplusplus)) #else

/* For compatibility with possible future MgraSymbols
   functions receiving arrays of other types. */
#define MgraSymbols    MgraSymbolsDouble
#define MgraSymbolsEx  MgraSymbolsDoubleEx

//////////////////////////////////////////////////////////////
// SafeType functions.
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
// See milos.h for explanation about these functions.
//////////////////////////////////////////////////////////////

#if (defined __cplusplus)
inline MIL_INT64 MgraInquireRequiredType(MIL_INT64 InquireType)
   {
   MIL_INT64 RequiredType = (InquireType & M_HLVLDATATYPE_MASK);

   if (RequiredType != 0)
      return RequiredType;

   if ( (InquireType & (M_AVAILABLE|M_SUPPORTED)) != 0 )
      return M_TYPE_MIL_INT;

   // Here, only the +M_DEFAULT combo flag is left; remove it.
   MIL_INT64 StrippedInquireType = ( InquireType & ~(M_HLVLDATATYPE_MASK|M_AVAILABLE|M_SUPPORTED|M_DEFAULT) );

   if (M_IN_GRA_INQUIRE_MIL_ID_RANGE(StrippedInquireType))
      return M_TYPE_MIL_ID;

   if (M_IN_GRA_INQUIRE_DOUBLE_RANGE(StrippedInquireType))
      return M_TYPE_DOUBLE;

   return M_TYPE_MIL_INT;
   }
#endif //#if (defined __cplusplus)

// M_MIL_USE_SAFE_TYPE defined in milos.h.
#if M_MIL_USE_SAFE_TYPE

inline void SafeTypeError(MIL_CONST_TEXT_PTR FunctionName);

// Definition of the error function called at each error of bad type
// of data passed to a void* argument.
//
// Application can define their own SafeTypeError function and
// disable this one by adding #define M_MIL_SAFE_TYPE_ERROR_DEFINED 1
// before including mil.h.

#if !M_MIL_SAFE_TYPE_ERROR_DEFINED
#define M_MIL_SAFE_TYPE_ERROR_DEFINED 1

#ifndef M_MIL_USE_SAFE_TYPE_COMPILE_ERROR_ONLY
inline void SafeTypeError(MIL_CONST_TEXT_PTR FunctionName)
   {
   MfuncPrintMessage(
      M_RESP_OK,
      MIL_TEXT("SafeType error in "), 
      FunctionName,
      MIL_TEXT(".\nPointer type is wrong.\n"),
      MIL_TEXT("\n"), MIL_TEXT("\n"));
   }
#else
inline void SafeTypeError(MIL_CONST_TEXT_PTR FunctionName)
   {
   }
#endif

#endif

inline void ReplaceTypeMilIdByTypeMilIntXX(MIL_INT64 *DataType)
   {
#if M_MIL_USE_INT64_ID
   if (*DataType == M_TYPE_MIL_ID)
      *DataType = M_TYPE_MIL_INT64;
#else
   if (*DataType == M_TYPE_MIL_ID)
      *DataType = M_TYPE_MIL_INT32;
#endif
   }

// ----------------------------------------------------------
// MappGetError

inline MIL_INT MFTYPE MappGetErrorUnsafe  (MIL_INT64 ErrorType, void           *ErrorPtr);
inline MIL_INT MFTYPE MappGetErrorSafeType(MIL_INT64 ErrorType, int             ErrorPtr);
inline MIL_INT MFTYPE MappGetErrorSafeType(MIL_INT64 ErrorType, MIL_TEXT_CHAR  *ErrorPtr);
inline MIL_INT MFTYPE MappGetErrorSafeType(MIL_INT64 ErrorType, MIL_INT        *ErrorPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MappGetErrorSafeType(MIL_INT64 ErrorType, MIL_UINT       *ErrorPtr);
#endif
inline MIL_INT MFTYPE MappGetErrorUnsafe  (MIL_ID ContextAppId, MIL_INT64 ErrorType, void           *ErrorPtr);
inline MIL_INT MFTYPE MappGetErrorSafeType(MIL_ID ContextAppId, MIL_INT64 ErrorType, int             ErrorPtr);
inline MIL_INT MFTYPE MappGetErrorSafeType(MIL_ID ContextAppId, MIL_INT64 ErrorType, MIL_TEXT_CHAR  *ErrorPtr);
inline MIL_INT MFTYPE MappGetErrorSafeType(MIL_ID ContextAppId, MIL_INT64 ErrorType, MIL_INT        *ErrorPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MappGetErrorSafeType(MIL_ID ContextAppId, MIL_INT64 ErrorType, MIL_UINT       *ErrorPtr);
#endif


//---------------------------------------------------------------------------------
// MappGetHookInfo
inline MIL_INT MFTYPE MappGetHookInfoUnsafe  (MIL_ID EventId, MIL_INT64 InfoType, void               *ResultPtr);
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, int                 ResultPtr);
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_INT8           *ResultPtr);
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_INT16          *ResultPtr);
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_INT32          *ResultPtr);
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_INT64          *ResultPtr);
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_DOUBLE         *ResultPtr);
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, void              **ResultPtr);
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, const void        **ResultPtr);
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_ID            **ResultPtr);
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, const MIL_ID      **ResultPtr);
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_TEXT_PTR       *ResultPtr);
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_CONST_TEXT_PTR *ResultPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_UINT8          *ResultPtr);
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_UINT16         *ResultPtr);
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_UINT32         *ResultPtr);
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_UINT64         *ResultPtr);
#endif
#if M_MIL_SAFE_TYPE_ADD_WCHAR_T
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, wchar_t            *ResultPtr);
#endif
inline MIL_INT MFTYPE MappGetHookInfoUnsafe  (MIL_ID ContextAppId, MIL_ID EventId, MIL_INT64 InfoType, void               *ResultPtr);
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID ContextAppId, MIL_ID EventId, MIL_INT64 InfoType, int                 ResultPtr);
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID ContextAppId, MIL_ID EventId, MIL_INT64 InfoType, MIL_INT8           *ResultPtr);
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID ContextAppId, MIL_ID EventId, MIL_INT64 InfoType, MIL_INT16          *ResultPtr);
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID ContextAppId, MIL_ID EventId, MIL_INT64 InfoType, MIL_INT32          *ResultPtr);
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID ContextAppId, MIL_ID EventId, MIL_INT64 InfoType, MIL_INT64          *ResultPtr);
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID ContextAppId, MIL_ID EventId, MIL_INT64 InfoType, MIL_DOUBLE         *ResultPtr);
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID ContextAppId, MIL_ID EventId, MIL_INT64 InfoType, void              **ResultPtr);
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID ContextAppId, MIL_ID EventId, MIL_INT64 InfoType, const void        **ResultPtr);
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID ContextAppId, MIL_ID EventId, MIL_INT64 InfoType, MIL_ID            **ResultPtr);
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID ContextAppId, MIL_ID EventId, MIL_INT64 InfoType, const MIL_ID      **ResultPtr);
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID ContextAppId, MIL_ID EventId, MIL_INT64 InfoType, MIL_TEXT_PTR       *ResultPtr);
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID ContextAppId, MIL_ID EventId, MIL_INT64 InfoType, MIL_CONST_TEXT_PTR *ResultPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID ContextAppId, MIL_ID EventId, MIL_INT64 InfoType, MIL_UINT8          *ResultPtr);
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID ContextAppId, MIL_ID EventId, MIL_INT64 InfoType, MIL_UINT16         *ResultPtr);
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID ContextAppId, MIL_ID EventId, MIL_INT64 InfoType, MIL_UINT32         *ResultPtr);
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID ContextAppId, MIL_ID EventId, MIL_INT64 InfoType, MIL_UINT64         *ResultPtr);
#endif
#if M_MIL_SAFE_TYPE_ADD_WCHAR_T
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID ContextAppId, MIL_ID EventId, MIL_INT64 InfoType, wchar_t            *ResultPtr);
#endif


//---------------------------------------------------------------
// MappInquire
inline MIL_INT MFTYPE MappInquireUnsafe  (MIL_INT64 InquireType, void           *UserVarPtr);
inline MIL_INT MFTYPE MappInquireSafeType(MIL_INT64 InquireType, int             UserVarPtr);
inline MIL_INT MFTYPE MappInquireSafeType(MIL_INT64 InquireType, MIL_TEXT_CHAR  *UserVarPtr);
inline MIL_INT MFTYPE MappInquireSafeType(MIL_INT64 InquireType, MIL_INT        *UserVarPtr);
inline MIL_INT MFTYPE MappInquireSafeType(MIL_INT64 InquireType, MIL_DOUBLE     *UserVarPtr);
#if !M_MIL_USE_64BIT
inline MIL_INT MFTYPE MappInquireSafeType(MIL_INT64 InquireType, MIL_INT64      *UserVarPtr);
#endif
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MappInquireSafeType(MIL_INT64 InquireType, MIL_UINT        *UserVarPtr);
#endif
inline MIL_INT MFTYPE MappInquireUnsafe  (MIL_ID ContextAppId, MIL_INT64 InquireType, void           *UserVarPtr);
inline MIL_INT MFTYPE MappInquireSafeType(MIL_ID ContextAppId, MIL_INT64 InquireType, int             UserVarPtr);
inline MIL_INT MFTYPE MappInquireSafeType(MIL_ID ContextAppId, MIL_INT64 InquireType, MIL_TEXT_CHAR  *UserVarPtr);
inline MIL_INT MFTYPE MappInquireSafeType(MIL_ID ContextAppId, MIL_INT64 InquireType, MIL_INT        *UserVarPtr);
inline MIL_INT MFTYPE MappInquireSafeType(MIL_ID ContextAppId, MIL_INT64 InquireType, MIL_DOUBLE     *UserVarPtr);
#if !M_MIL_USE_64BIT
inline MIL_INT MFTYPE MappInquireSafeType(MIL_ID ContextAppId, MIL_INT64 InquireType, MIL_INT64      *UserVarPtr);
#endif
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MappInquireSafeType(MIL_ID ContextAppId, MIL_INT64 InquireType, MIL_UINT        *UserVarPtr);
#endif


#if M_MIL_USE_UNICODE
inline void MFTYPE MappFileOperationUnsafeA(    MIL_ID             Comp1ContextAppId,
                                                const char*        Comp1FileName,
                                                MIL_ID             Comp2ContextAppId,
                                                const char*        Comp2FileName,
                                                MIL_INT64          Operation,
                                                MIL_INT64          OperationFlag,
                                                void*              OperationDataPtr);
inline void MFTYPE MappFileOperationUnsafeW(    MIL_ID             Comp1ContextAppId,
                                                MIL_CONST_TEXT_PTR Comp1FileName,
                                                MIL_ID             Comp2ContextAppId,
                                                MIL_CONST_TEXT_PTR Comp2FileName,
                                                MIL_INT64          Operation,
                                                MIL_INT64          OperationFlag,
                                                void*              OperationDataPtr);

inline void MFTYPE MappFileOperationSafeTypeW(  MIL_ID             Comp1ContextAppId,
                                                MIL_CONST_TEXT_PTR Comp1FileName,
                                                MIL_ID             Comp2ContextAppId,
                                                MIL_CONST_TEXT_PTR Comp2FileName,
                                                MIL_INT64          Operation,
                                                MIL_INT64          OperationFlag,
                                                int                OperationDataPtr);
inline void MFTYPE MappFileOperationSafeTypeW(  MIL_ID             Comp1ContextAppId,
                                                int                Comp1FileName,
                                                MIL_ID             Comp2ContextAppId,
                                                MIL_CONST_TEXT_PTR Comp2FileName,
                                                MIL_INT64          Operation,
                                                MIL_INT64          OperationFlag,
                                                int                OperationDataPtr);
inline void MFTYPE MappFileOperationSafeTypeW(  MIL_ID             Comp1ContextAppId,
                                                MIL_CONST_TEXT_PTR Comp1FileName,
                                                MIL_ID             Comp2ContextAppId,
                                                int                Comp2FileName,
                                                MIL_INT64          Operation,
                                                MIL_INT64          OperationFlag,
                                                int                OperationDataPtr);
inline void MFTYPE MappFileOperationSafeTypeW(  MIL_ID             Comp1ContextAppId,
                                                MIL_CONST_TEXT_PTR Comp1FileName,
                                                MIL_ID             Comp2ContextAppId,
                                                int                Comp2FileName,
                                                MIL_INT64          Operation,
                                                MIL_INT64          OperationFlag,
                                                MIL_INT*           OperationDataPtr);

inline void MFTYPE MappFileOperationSafeTypeA(  MIL_ID             Comp1ContextAppId,
                                                const char*        Comp1FileName,
                                                MIL_ID             Comp2ContextAppId,
                                                const char*        Comp2FileName,
                                                MIL_INT64          Operation,
                                                MIL_INT64          OperationFlag,
                                                int                OperationDataPtr);
inline void MFTYPE MappFileOperationSafeTypeA(  MIL_ID             Comp1ContextAppId,
                                                int                Comp1FileName,
                                                MIL_ID             Comp2ContextAppId,
                                                const char*        Comp2FileName,
                                                MIL_INT64          Operation,
                                                MIL_INT64          OperationFlag,
                                                int                OperationDataPtr);
inline void MFTYPE MappFileOperationSafeTypeA(  MIL_ID             Comp1ContextAppId,
                                                const char*        Comp1FileName,
                                                MIL_ID             Comp2ContextAppId,
                                                int                Comp2FileName,
                                                MIL_INT64          Operation,
                                                MIL_INT64          OperationFlag,
                                                int                OperationDataPtr);
inline void MFTYPE MappFileOperationSafeTypeA(  MIL_ID             Comp1ContextAppId,
                                                const char*        Comp1FileName,
                                                MIL_ID             Comp2ContextAppId,
                                                int                Comp2FileName,
                                                MIL_INT64          Operation,
                                                MIL_INT64          OperationFlag,
                                                MIL_INT*           OperationDataPtr);
#else
inline void MFTYPE MappFileOperationUnsafe(     MIL_ID             Comp1ContextAppId,
                                                MIL_CONST_TEXT_PTR Comp1FileName,
                                                MIL_ID             Comp2ContextAppId,
                                                MIL_CONST_TEXT_PTR Comp2FileName,
                                                MIL_INT64          Operation,
                                                MIL_INT64          OperationFlag,
                                                void*              OperationDataPtr);

inline void MFTYPE MappFileOperationSafeType(   MIL_ID             Comp1ContextAppId,
                                                MIL_CONST_TEXT_PTR Comp1FileName,
                                                MIL_ID             Comp2ContextAppId,
                                                MIL_CONST_TEXT_PTR Comp2FileName,
                                                MIL_INT64          Operation,
                                                MIL_INT64          OperationFlag,
                                                int                OperationDataPtr);
inline void MFTYPE MappFileOperationSafeType(   MIL_ID             Comp1ContextAppId,
                                                int                Comp1FileName,
                                                MIL_ID             Comp2ContextAppId,
                                                MIL_CONST_TEXT_PTR Comp2FileName,
                                                MIL_INT64          Operation,
                                                MIL_INT64          OperationFlag,
                                                int                OperationDataPtr);
inline void MFTYPE MappFileOperationSafeType(   MIL_ID             Comp1ContextAppId,
                                                MIL_CONST_TEXT_PTR Comp1FileName,
                                                MIL_ID             Comp2ContextAppId,
                                                int                Comp2FileName,
                                                MIL_INT64          Operation,
                                                MIL_INT64          OperationFlag,
                                                int                OperationDataPtr);
inline void MFTYPE MappFileOperationSafeType(   MIL_ID             Comp1ContextAppId,
                                                MIL_CONST_TEXT_PTR Comp1FileName,
                                                MIL_ID             Comp2ContextAppId,
                                                int                Comp2FileName,
                                                MIL_INT64          Operation,
                                                MIL_INT64          OperationFlag,
                                                MIL_INT*           OperationDataPtr);
#endif

// ----------------------------------------------------------
// MbufDiskInquire

#if M_MIL_USE_UNICODE
inline MIL_INT MFTYPE MbufDiskInquireUnsafeA  (const char*        FileName, MIL_INT64 InquireType, void        *UserVarPtr);
inline MIL_INT MFTYPE MbufDiskInquireSafeTypeA(const char*        FileName, MIL_INT64 InquireType, int          UserVarPtr);
inline MIL_INT MFTYPE MbufDiskInquireSafeTypeA(const char*        FileName, MIL_INT64 InquireType, MIL_INT     *UserVarPtr);
inline MIL_INT MFTYPE MbufDiskInquireSafeTypeA(const char*        FileName, MIL_INT64 InquireType, MIL_DOUBLE  *UserVarPtr);
inline MIL_INT MFTYPE MbufDiskInquireUnsafeW  (MIL_CONST_TEXT_PTR FileName, MIL_INT64 InquireType, void        *UserVarPtr);
inline MIL_INT MFTYPE MbufDiskInquireSafeTypeW(MIL_CONST_TEXT_PTR FileName, MIL_INT64 InquireType, int          UserVarPtr);
inline MIL_INT MFTYPE MbufDiskInquireSafeTypeW(MIL_CONST_TEXT_PTR FileName, MIL_INT64 InquireType, MIL_INT     *UserVarPtr);
inline MIL_INT MFTYPE MbufDiskInquireSafeTypeW(MIL_CONST_TEXT_PTR FileName, MIL_INT64 InquireType, MIL_DOUBLE  *UserVarPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MbufDiskInquireSafeTypeA(const char*        FileName, MIL_INT64 InquireType, MIL_UINT    *UserVarPtr);
inline MIL_INT MFTYPE MbufDiskInquireSafeTypeW(MIL_CONST_TEXT_PTR FileName, MIL_INT64 InquireType, MIL_UINT    *UserVarPtr);
#endif
#else
inline MIL_INT MFTYPE MbufDiskInquireUnsafe   (MIL_CONST_TEXT_PTR FileName, MIL_INT64 InquireType, void        *UserVarPtr);
inline MIL_INT MFTYPE MbufDiskInquireSafeType (MIL_CONST_TEXT_PTR FileName, MIL_INT64 InquireType, int          UserVarPtr);
inline MIL_INT MFTYPE MbufDiskInquireSafeType (MIL_CONST_TEXT_PTR FileName, MIL_INT64 InquireType, MIL_INT     *UserVarPtr);
inline MIL_INT MFTYPE MbufDiskInquireSafeType (MIL_CONST_TEXT_PTR FileName, MIL_INT64 InquireType, MIL_DOUBLE  *UserVarPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MbufDiskInquireSafeType (MIL_CONST_TEXT_PTR FileName, MIL_INT64 InquireType, MIL_UINT    *UserVarPtr);
#endif
#endif

// ----------------------------------------------------------
// MbufGetHookInfo

inline MIL_INT MFTYPE MbufGetHookInfoUnsafe  (MIL_ID EventId, MIL_INT64 InfoType, void            *ResultPtr);
inline MIL_INT MFTYPE MbufGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, int              ResultPtr);
inline MIL_INT MFTYPE MbufGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_INT         *ResultPtr);
inline MIL_INT MFTYPE MbufGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_DOUBLE      *ResultPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MbufGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_UINT        *ResultPtr);
#endif

// ----------------------------------------------------------
// MbufInquire

inline MIL_INT MFTYPE MbufInquireUnsafe  (MIL_ID BufId, MIL_INT64 InquireType,       void            *ResultPtr);
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType,       int              ResultPtr);
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType,       MIL_INT32       *ResultPtr);
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType,       MIL_INT64       *ResultPtr);
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType,       MIL_DOUBLE      *ResultPtr);
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType,       MIL_TEXT_CHAR   *ResultPtr);
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType,       char           **ResultPtr);
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType, const char           **ResultPtr);
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType,       signed char    **ResultPtr);
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType, const signed char    **ResultPtr);
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType,       unsigned char  **ResultPtr);
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType, const unsigned char  **ResultPtr);
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType,       MIL_INT16      **ResultPtr);
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType, const MIL_INT16      **ResultPtr);
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType,       MIL_UINT16     **ResultPtr);
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType, const MIL_UINT16     **ResultPtr);
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType,       MIL_INT32      **ResultPtr);
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType, const MIL_INT32      **ResultPtr);
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType,       MIL_UINT32     **ResultPtr);
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType, const MIL_UINT32     **ResultPtr);
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType,       float          **ResultPtr);
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType, const float          **ResultPtr);
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType,       MIL_INT64      **ResultPtr);
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType, const MIL_INT64      **ResultPtr);
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType,       MIL_UINT64     **ResultPtr);
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType, const MIL_UINT64     **ResultPtr);
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType,       MIL_DOUBLE     **ResultPtr);
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType, const MIL_DOUBLE     **ResultPtr);
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType,       void           **ResultPtr);
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType, const void           **ResultPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType,       MIL_UINT32      *ResultPtr);
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType,       MIL_UINT64      *ResultPtr);
#endif

#if M_MIL_USE_WINDOWS
#if M_MIL_USE_DIRECTX_SERVICE
struct IDirectDrawSurface;
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType, IDirectDrawSurface   **ResultPtr);
#endif // M_MIL_USE_DIRECTX_SERVICE
struct tagBITMAPINFO;
typedef tagBITMAPINFO BITMAPINFO;
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType, BITMAPINFO           **ResultPtr);
#endif // M_MIL_USE_WINDOWS

// ----------------------------------------------------------
// MdigGetHookInfo

inline MIL_INT MFTYPE MdigGetHookInfoUnsafe  (MIL_ID EventId, MIL_INT64 InfoType, void           *ResultPtr);
inline MIL_INT MFTYPE MdigGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, int             ResultPtr);
inline MIL_INT MFTYPE MdigGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_INT        *ResultPtr);
inline MIL_INT MFTYPE MdigGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_DOUBLE     *ResultPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MdigGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_UINT       *ResultPtr);
#endif

// ----------------------------------------------------------

// MdigInquire

inline MIL_INT MFTYPE MdigInquireUnsafe  (MIL_ID DigId, MIL_INT64 InquireType, void           *ResultPtr);
inline MIL_INT MFTYPE MdigInquireSafeType(MIL_ID DigId, MIL_INT64 InquireType, int             ResultPtr);
inline MIL_INT MFTYPE MdigInquireSafeType(MIL_ID DigId, MIL_INT64 InquireType, MIL_TEXT_CHAR  *ResultPtr);
inline MIL_INT MFTYPE MdigInquireSafeType(MIL_ID DigId, MIL_INT64 InquireType, MIL_INT32      *ResultPtr);
inline MIL_INT MFTYPE MdigInquireSafeType(MIL_ID DigId, MIL_INT64 InquireType, MIL_INT64      *ResultPtr);
inline MIL_INT MFTYPE MdigInquireSafeType(MIL_ID DigId, MIL_INT64 InquireType, MIL_DOUBLE     *ResultPtr);
inline MIL_INT MFTYPE MdigInquireSafeType(MIL_ID DigId, MIL_INT64 InquireType, MIL_ID         *ResultPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MdigInquireSafeType(MIL_ID DigId, MIL_INT64 InquireType, MIL_UINT32     *ResultPtr);
inline MIL_INT MFTYPE MdigInquireSafeType(MIL_ID DigId, MIL_INT64 InquireType, MIL_UINT64     *ResultPtr);
#endif

// ----------------------------------------------------------

// MdigInquireFeature

#if M_MIL_USE_UNICODE

inline void MFTYPE MdigInquireFeatureUnsafeW  (MIL_ID DigId, MIL_INT64 InquireType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, void            *UserVarPtr);
inline void MFTYPE MdigInquireFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 InquireType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, MIL_TEXT_CHAR   *UserVarPtr);
inline void MFTYPE MdigInquireFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 InquireType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, MIL_INT64       *UserVarPtr);
inline void MFTYPE MdigInquireFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 InquireType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, MIL_DOUBLE      *UserVarPtr);
inline void MFTYPE MdigInquireFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 InquireType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, MIL_INT32       *UserVarPtr);
inline void MFTYPE MdigInquireFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 InquireType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, MIL_UINT8       *UserVarPtr);
inline void MFTYPE MdigInquireFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 InquireType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, bool            *UserVarPtr);
inline void MFTYPE MdigInquireFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 InquireType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, int              UserVarPtr);
inline void MFTYPE MdigInquireFeatureUnsafeA  (MIL_ID DigId, MIL_INT64 InquireType, const char*        FeatureName, MIL_INT64 FeatureType, void            *UserVarPtr);
inline void MFTYPE MdigInquireFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 InquireType, const char*        FeatureName, MIL_INT64 FeatureType, MIL_TEXT_CHAR   *UserVarPtr);
inline void MFTYPE MdigInquireFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 InquireType, const char*        FeatureName, MIL_INT64 FeatureType, MIL_INT64       *UserVarPtr);
inline void MFTYPE MdigInquireFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 InquireType, const char*        FeatureName, MIL_INT64 FeatureType, MIL_DOUBLE      *UserVarPtr);
inline void MFTYPE MdigInquireFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 InquireType, const char*        FeatureName, MIL_INT64 FeatureType, MIL_INT32       *UserVarPtr);
inline void MFTYPE MdigInquireFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 InquireType, const char*        FeatureName, MIL_INT64 FeatureType, MIL_UINT8       *UserVarPtr);
inline void MFTYPE MdigInquireFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 InquireType, const char*        FeatureName, MIL_INT64 FeatureType, bool            *UserVarPtr);
inline void MFTYPE MdigInquireFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 InquireType, const char*        FeatureName, MIL_INT64 FeatureType, int              UserVarPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline void MFTYPE MdigInquireFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 InquireType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, MIL_UINT64      *UserVarPtr);
inline void MFTYPE MdigInquireFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 InquireType, const char*        FeatureName, MIL_INT64 FeatureType, MIL_UINT64      *UserVarPtr);
#endif

#else

inline void MFTYPE MdigInquireFeatureUnsafe  (MIL_ID DigId, MIL_INT64 InquireType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, void             *UserVarPtr);
inline void MFTYPE MdigInquireFeatureSafeType(MIL_ID DigId, MIL_INT64 InquireType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, MIL_TEXT_CHAR    *UserVarPtr);
inline void MFTYPE MdigInquireFeatureSafeType(MIL_ID DigId, MIL_INT64 InquireType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, MIL_INT64        *UserVarPtr);
inline void MFTYPE MdigInquireFeatureSafeType(MIL_ID DigId, MIL_INT64 InquireType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, MIL_DOUBLE       *UserVarPtr);
inline void MFTYPE MdigInquireFeatureSafeType(MIL_ID DigId, MIL_INT64 InquireType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, MIL_INT32        *UserVarPtr);
inline void MFTYPE MdigInquireFeatureSafeType(MIL_ID DigId, MIL_INT64 InquireType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, MIL_UINT8        *UserVarPtr);
inline void MFTYPE MdigInquireFeatureSafeType(MIL_ID DigId, MIL_INT64 InquireType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, bool             *UserVarPtr);
inline void MFTYPE MdigInquireFeatureSafeType(MIL_ID DigId, MIL_INT64 InquireType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, int               UserVarPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline void MFTYPE MdigInquireFeatureSafeType(MIL_ID DigId, MIL_INT64 InquireType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, MIL_UINT64       *UserVarPtr);
#endif

#endif
// ----------------------------------------------------------

// MdigControlFeature
#if M_MIL_USE_UNICODE

inline void MFTYPE MdigControlFeatureUnsafeW  (MIL_ID DigId, MIL_INT64 ControlType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, const void            *UserVarPtr);
inline void MFTYPE MdigControlFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 ControlType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, const MIL_TEXT_CHAR   *UserVarPtr);
inline void MFTYPE MdigControlFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 ControlType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, const MIL_INT64       *UserVarPtr);
inline void MFTYPE MdigControlFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 ControlType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, const MIL_DOUBLE      *UserVarPtr);
inline void MFTYPE MdigControlFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 ControlType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, const MIL_INT32       *UserVarPtr);
inline void MFTYPE MdigControlFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 ControlType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, const MIL_UINT8       *UserVarPtr);
inline void MFTYPE MdigControlFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 ControlType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, const bool            *UserVarPtr);
inline void MFTYPE MdigControlFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 ControlType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType,       int              UserVarPtr);
inline void MFTYPE MdigControlFeatureUnsafeA  (MIL_ID DigId, MIL_INT64 ControlType, const char*        FeatureName, MIL_INT64 FeatureType, const void            *UserVarPtr);
inline void MFTYPE MdigControlFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 ControlType, const char*        FeatureName, MIL_INT64 FeatureType, const MIL_TEXT_CHAR   *UserVarPtr);
inline void MFTYPE MdigControlFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 ControlType, const char*        FeatureName, MIL_INT64 FeatureType, const MIL_INT64       *UserVarPtr);
inline void MFTYPE MdigControlFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 ControlType, const char*        FeatureName, MIL_INT64 FeatureType, const MIL_DOUBLE      *UserVarPtr);
inline void MFTYPE MdigControlFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 ControlType, const char*        FeatureName, MIL_INT64 FeatureType, const MIL_INT32       *UserVarPtr);
inline void MFTYPE MdigControlFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 ControlType, const char*        FeatureName, MIL_INT64 FeatureType, const MIL_UINT8       *UserVarPtr);
inline void MFTYPE MdigControlFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 ControlType, const char*        FeatureName, MIL_INT64 FeatureType, const bool            *UserVarPtr);
inline void MFTYPE MdigControlFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 ControlType, const char*        FeatureName, MIL_INT64 FeatureType,       int              UserVarPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline void MFTYPE MdigControlFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 ControlType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, const MIL_UINT64      *UserVarPtr);
inline void MFTYPE MdigControlFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 ControlType, const char*        FeatureName, MIL_INT64 FeatureType, const MIL_UINT64      *UserVarPtr);
#endif

#else

inline void MFTYPE MdigControlFeatureUnsafe  (MIL_ID DigId, MIL_INT64 ControlType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, const void             *UserVarPtr);
inline void MFTYPE MdigControlFeatureSafeType(MIL_ID DigId, MIL_INT64 ControlType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, const MIL_TEXT_CHAR    *UserVarPtr);
inline void MFTYPE MdigControlFeatureSafeType(MIL_ID DigId, MIL_INT64 ControlType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, const MIL_INT64        *UserVarPtr);
inline void MFTYPE MdigControlFeatureSafeType(MIL_ID DigId, MIL_INT64 ControlType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, const MIL_DOUBLE       *UserVarPtr);
inline void MFTYPE MdigControlFeatureSafeType(MIL_ID DigId, MIL_INT64 ControlType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, const MIL_INT32        *UserVarPtr);
inline void MFTYPE MdigControlFeatureSafeType(MIL_ID DigId, MIL_INT64 ControlType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, const MIL_UINT8        *UserVarPtr);
inline void MFTYPE MdigControlFeatureSafeType(MIL_ID DigId, MIL_INT64 ControlType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, const bool             *UserVarPtr);
inline void MFTYPE MdigControlFeatureSafeType(MIL_ID DigId, MIL_INT64 ControlType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType,       int               UserVarPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline void MFTYPE MdigControlFeatureSafeType(MIL_ID DigId, MIL_INT64 ControlType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, const MIL_UINT64       *UserVarPtr);
#endif
#endif
// ----------------------------------------------------------
// MfpgaGetHookInfo

inline MIL_INT MFTYPE MfpgaGetHookInfoUnsafe  (MIL_ID  EventId, MIL_INT64 InfoType, void             *UserVarPtr);
inline MIL_INT MFTYPE MfpgaGetHookInfoSafeType(MIL_ID  EventId, MIL_INT64 InfoType, int               UserVarPtr);
inline MIL_INT MFTYPE MfpgaGetHookInfoSafeType(MIL_ID  EventId, MIL_INT64 InfoType, MIL_INT32        *UserVarPtr);
inline MIL_INT MFTYPE MfpgaGetHookInfoSafeType(MIL_ID  EventId, MIL_INT64 InfoType, MIL_INT64        *UserVarPtr);
inline MIL_INT MFTYPE MfpgaGetHookInfoSafeType(MIL_ID  EventId, MIL_INT64 InfoType, MIL_DOUBLE       *UserVarPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MfpgaGetHookInfoSafeType(MIL_ID  EventId, MIL_INT64 InfoType, MIL_UINT32       *UserVarPtr);
inline MIL_INT MFTYPE MfpgaGetHookInfoSafeType(MIL_ID  EventId, MIL_INT64 InfoType, MIL_UINT64       *UserVarPtr);
#endif

// ----------------------------------------------------------
// MfpgaInquire

inline MIL_INT MFTYPE MfpgaInquireUnsafe  (MIL_ID  MilSystemId, MIL_INT FpgaDeviceNum, MIL_INT64 InquireType, void             *UserVarPtr);
inline MIL_INT MFTYPE MfpgaInquireSafeType(MIL_ID  MilSystemId, MIL_INT FpgaDeviceNum, MIL_INT64 InquireType, int               UserVarPtr);
inline MIL_INT MFTYPE MfpgaInquireSafeType(MIL_ID  MilSystemId, MIL_INT FpgaDeviceNum, MIL_INT64 InquireType, MIL_INT32        *UserVarPtr);
inline MIL_INT MFTYPE MfpgaInquireSafeType(MIL_ID  MilSystemId, MIL_INT FpgaDeviceNum, MIL_INT64 InquireType, MIL_INT64        *UserVarPtr);
inline MIL_INT MFTYPE MfpgaInquireSafeType(MIL_ID  MilSystemId, MIL_INT FpgaDeviceNum, MIL_INT64 InquireType, MIL_TEXT_CHAR    *UserVarPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MfpgaInquireSafeType(MIL_ID  MilSystemId, MIL_INT FpgaDeviceNum, MIL_INT64 InquireType, MIL_UINT32       *UserVarPtr);
inline MIL_INT MFTYPE MfpgaInquireSafeType(MIL_ID  MilSystemId, MIL_INT FpgaDeviceNum, MIL_INT64 InquireType, MIL_UINT64       *UserVarPtr);
#endif

// ----------------------------------------------------------
// MfuncInquire

inline MIL_INT MFTYPE MfuncInquireUnsafe  (MIL_ID ObjectId, MIL_INT64 InquireType, void         *UserVarPtr);
inline MIL_INT MFTYPE MfuncInquireSafeType(MIL_ID ObjectId, MIL_INT64 InquireType, int           UserVarPtr);
inline MIL_INT MFTYPE MfuncInquireSafeType(MIL_ID ObjectId, MIL_INT64 InquireType, MIL_INT32    *UserVarPtr);
inline MIL_INT MFTYPE MfuncInquireSafeType(MIL_ID ObjectId, MIL_INT64 InquireType, MIL_INT64    *UserVarPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MfuncInquireSafeType(MIL_ID ObjectId, MIL_INT64 InquireType, MIL_UINT64   *UserVarPtr);
#endif

// ----------------------------------------------------------
// MgraInquire

inline MIL_INT MFTYPE MgraInquireUnsafe  (MIL_ID ContextGraId, MIL_INT64 InquireType, void         *UserVarPtr);
inline MIL_INT MFTYPE MgraInquireSafeType(MIL_ID ContextGraId, MIL_INT64 InquireType, int           UserVarPtr);
inline MIL_INT MFTYPE MgraInquireSafeType(MIL_ID ContextGraId, MIL_INT64 InquireType, MIL_INT8     *UserVarPtr);
inline MIL_INT MFTYPE MgraInquireSafeType(MIL_ID ContextGraId, MIL_INT64 InquireType, MIL_INT16    *UserVarPtr);
inline MIL_INT MFTYPE MgraInquireSafeType(MIL_ID ContextGraId, MIL_INT64 InquireType, MIL_INT32    *UserVarPtr);
inline MIL_INT MFTYPE MgraInquireSafeType(MIL_ID ContextGraId, MIL_INT64 InquireType, MIL_INT64    *UserVarPtr);
inline MIL_INT MFTYPE MgraInquireSafeType(MIL_ID ContextGraId, MIL_INT64 InquireType, float        *UserVarPtr);
inline MIL_INT MFTYPE MgraInquireSafeType(MIL_ID ContextGraId, MIL_INT64 InquireType, MIL_DOUBLE   *UserVarPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MgraInquireSafeType(MIL_ID ContextGraId, MIL_INT64 InquireType, MIL_UINT8    *UserVarPtr);
inline MIL_INT MFTYPE MgraInquireSafeType(MIL_ID ContextGraId, MIL_INT64 InquireType, MIL_UINT16   *UserVarPtr);
inline MIL_INT MFTYPE MgraInquireSafeType(MIL_ID ContextGraId, MIL_INT64 InquireType, MIL_UINT32   *UserVarPtr);
inline MIL_INT MFTYPE MgraInquireSafeType(MIL_ID ContextGraId, MIL_INT64 InquireType, MIL_UINT64   *UserVarPtr);
#endif

// ----------------------------------------------------------
// MgraInquireList

inline MIL_INT MFTYPE MgraInquireListUnsafe  (MIL_ID GraListId, MIL_INT Index, MIL_INT SubIndex, MIL_INT64 InquireType, void         *UserVarPtr);
inline MIL_INT MFTYPE MgraInquireListSafeType(MIL_ID GraListId, MIL_INT Index, MIL_INT SubIndex, MIL_INT64 InquireType, int           UserVarPtr);
inline MIL_INT MFTYPE MgraInquireListSafeType(MIL_ID GraListId, MIL_INT Index, MIL_INT SubIndex, MIL_INT64 InquireType, MIL_INT8     *UserVarPtr);
inline MIL_INT MFTYPE MgraInquireListSafeType(MIL_ID GraListId, MIL_INT Index, MIL_INT SubIndex, MIL_INT64 InquireType, MIL_INT16    *UserVarPtr);
inline MIL_INT MFTYPE MgraInquireListSafeType(MIL_ID GraListId, MIL_INT Index, MIL_INT SubIndex, MIL_INT64 InquireType, MIL_INT32    *UserVarPtr);
inline MIL_INT MFTYPE MgraInquireListSafeType(MIL_ID GraListId, MIL_INT Index, MIL_INT SubIndex, MIL_INT64 InquireType, MIL_INT64    *UserVarPtr);
inline MIL_INT MFTYPE MgraInquireListSafeType(MIL_ID GraListId, MIL_INT Index, MIL_INT SubIndex, MIL_INT64 InquireType, float        *UserVarPtr);
inline MIL_INT MFTYPE MgraInquireListSafeType(MIL_ID GraListId, MIL_INT Index, MIL_INT SubIndex, MIL_INT64 InquireType, MIL_DOUBLE   *UserVarPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MgraInquireListSafeType(MIL_ID GraListId, MIL_INT Index, MIL_INT SubIndex, MIL_INT64 InquireType, MIL_UINT8    *UserVarPtr);
inline MIL_INT MFTYPE MgraInquireListSafeType(MIL_ID GraListId, MIL_INT Index, MIL_INT SubIndex, MIL_INT64 InquireType, MIL_UINT16   *UserVarPtr);
inline MIL_INT MFTYPE MgraInquireListSafeType(MIL_ID GraListId, MIL_INT Index, MIL_INT SubIndex, MIL_INT64 InquireType, MIL_UINT32   *UserVarPtr);
inline MIL_INT MFTYPE MgraInquireListSafeType(MIL_ID GraListId, MIL_INT Index, MIL_INT SubIndex, MIL_INT64 InquireType, MIL_UINT64   *UserVarPtr);
#endif

// ----------------------------------------------------------
// MsysGetHookInfo

inline MIL_INT MFTYPE MsysGetHookInfoUnsafe  (MIL_ID SystemId, MIL_ID EventId, MIL_INT64 InquireType, void            *ResultPtr);
inline MIL_INT MFTYPE MsysGetHookInfoSafeType(MIL_ID SystemId, MIL_ID EventId, MIL_INT64 InquireType, int              ResultPtr);
inline MIL_INT MFTYPE MsysGetHookInfoSafeType(MIL_ID SystemId, MIL_ID EventId, MIL_INT64 InquireType, MIL_INT32       *ResultPtr);
inline MIL_INT MFTYPE MsysGetHookInfoSafeType(MIL_ID SystemId, MIL_ID EventId, MIL_INT64 InquireType, MIL_INT64       *ResultPtr);
inline MIL_INT MFTYPE MsysGetHookInfoSafeType(MIL_ID SystemId, MIL_ID EventId, MIL_INT64 InquireType, MIL_DOUBLE      *ResultPtr);
inline MIL_INT MFTYPE MsysGetHookInfoSafeType(MIL_ID SystemId, MIL_ID EventId, MIL_INT64 InquireType, MIL_TEXT_PTR     ResultPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MsysGetHookInfoSafeType(MIL_ID SystemId, MIL_ID EventId, MIL_INT64 InquireType, MIL_UINT32      *ResultPtr);
inline MIL_INT MFTYPE MsysGetHookInfoSafeType(MIL_ID SystemId, MIL_ID EventId, MIL_INT64 InquireType, MIL_UINT64      *ResultPtr);
#endif

// ----------------------------------------------------------
// MsysInquire

inline MIL_INT MFTYPE MsysInquireUnsafe  (MIL_ID SystemId, MIL_INT64 InquireType, void            *ResultPtr);
inline MIL_INT MFTYPE MsysInquireSafeType(MIL_ID SystemId, MIL_INT64 InquireType, int              ResultPtr);
inline MIL_INT MFTYPE MsysInquireSafeType(MIL_ID SystemId, MIL_INT64 InquireType, MIL_INT32       *ResultPtr);
inline MIL_INT MFTYPE MsysInquireSafeType(MIL_ID SystemId, MIL_INT64 InquireType, MIL_INT64       *ResultPtr);
inline MIL_INT MFTYPE MsysInquireSafeType(MIL_ID SystemId, MIL_INT64 InquireType, MIL_TEXT_CHAR   *ResultPtr);
inline MIL_INT MFTYPE MsysInquireSafeType(MIL_ID SystemId, MIL_INT64 InquireType, MIL_DOUBLE      *ResultPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MsysInquireSafeType(MIL_ID SystemId, MIL_INT64 InquireType, MIL_UINT32      *ResultPtr);
inline MIL_INT MFTYPE MsysInquireSafeType(MIL_ID SystemId, MIL_INT64 InquireType, MIL_UINT64      *ResultPtr);
#endif

// ----------------------------------------------------------
// MsysIoInquire

inline MIL_INT MFTYPE MsysIoInquireUnsafe  (MIL_ID IoObjectId, MIL_INT64 InquireType, void            *UserVarPtr);
inline MIL_INT MFTYPE MsysIoInquireSafeType(MIL_ID IoObjectId, MIL_INT64 InquireType, int              UserVarPtr);
inline MIL_INT MFTYPE MsysIoInquireSafeType(MIL_ID IoObjectId, MIL_INT64 InquireType, MIL_INT32       *UserVarPtr);
inline MIL_INT MFTYPE MsysIoInquireSafeType(MIL_ID IoObjectId, MIL_INT64 InquireType, MIL_INT64       *UserVarPtr);
inline MIL_INT MFTYPE MsysIoInquireSafeType(MIL_ID IoObjectId, MIL_INT64 InquireType, MIL_TEXT_CHAR   *UserVarPtr);
inline MIL_INT MFTYPE MsysIoInquireSafeType(MIL_ID IoObjectId, MIL_INT64 InquireType, MIL_DOUBLE      *UserVarPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MsysIoInquireSafeType(MIL_ID IoObjectId, MIL_INT64 InquireType, MIL_UINT32      *UserVarPtr);
inline MIL_INT MFTYPE MsysIoInquireSafeType(MIL_ID IoObjectId, MIL_INT64 InquireType, MIL_UINT64      *UserVarPtr);
#endif

// ----------------------------------------------------------
// MsysIoCommandRegister
inline MIL_INT MFTYPE MsysIoCommandRegisterUnsafe  (MIL_ID IoObjectId, MIL_INT64 Operation, MIL_INT64 ReferenceStamp, MIL_DOUBLE DelayFromReferenceStamp, MIL_DOUBLE Duration, MIL_INT64 BitToOperate, void*    CommandStatusPtr);
inline MIL_INT MFTYPE MsysIoCommandRegisterSafeType(MIL_ID IoObjectId, MIL_INT64 Operation, MIL_INT64 ReferenceStamp, MIL_DOUBLE DelayFromReferenceStamp, MIL_DOUBLE Duration, MIL_INT64 BitToOperate, int      CommandStatusPtr);
inline MIL_INT MFTYPE MsysIoCommandRegisterSafeType(MIL_ID IoObjectId, MIL_INT64 Operation, MIL_INT64 ReferenceStamp, MIL_DOUBLE DelayFromReferenceStamp, MIL_DOUBLE Duration, MIL_INT64 BitToOperate, MIL_INT* CommandStatusPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MsysIoCommandRegisterSafeType(MIL_ID IoObjectId, MIL_INT64 Operation, MIL_INT64 ReferenceStamp, MIL_DOUBLE DelayFromReferenceStamp, MIL_DOUBLE Duration, MIL_INT64 BitToOperate, MIL_UINT* CommandStatusPtr);
#endif

//--------------------------------------------------------------------
// MthrInquire

inline MIL_INT MFTYPE MthrInquireUnsafe  (MIL_ID ThreadEventOrMutexId, MIL_INT64 InquireType, void      *UserVarPtr);
inline MIL_INT MFTYPE MthrInquireSafeType(MIL_ID ThreadEventOrMutexId, MIL_INT64 InquireType, int        UserVarPtr);
inline MIL_INT MFTYPE MthrInquireSafeType(MIL_ID ThreadEventOrMutexId, MIL_INT64 InquireType, MIL_INT   *UserVarPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MthrInquireSafeType(MIL_ID ThreadEventOrMutexId, MIL_INT64 InquireType, MIL_UINT  *UserVarPtr);
#endif

//--------------------------------------------------------------------
// MseqGetHookInfo
inline MIL_INT MFTYPE MseqGetHookInfoUnsafe  (MIL_ID EventId, MIL_INT64 InfoType, void            *UserVarPtr);
inline MIL_INT MFTYPE MseqGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, int             *UserVarPtr);
inline MIL_INT MFTYPE MseqGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_INT         *UserVarPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MseqGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_UINT        *UserVarPtr);
#endif

// ----------------------------------------------------------
// MseqInquire

inline MIL_INT MFTYPE MseqInquireUnsafe  (MIL_ID ContextSeqId, MIL_INT SequenceIndex, MIL_INT64 InquireType, void            *UserVarPtr);
inline MIL_INT MFTYPE MseqInquireSafeType(MIL_ID ContextSeqId, MIL_INT SequenceIndex, MIL_INT64 InquireType, int             *UserVarPtr);
inline MIL_INT MFTYPE MseqInquireSafeType(MIL_ID ContextSeqId, MIL_INT SequenceIndex, MIL_INT64 InquireType, MIL_INT32       *UserVarPtr);
inline MIL_INT MFTYPE MseqInquireSafeType(MIL_ID ContextSeqId, MIL_INT SequenceIndex, MIL_INT64 InquireType, MIL_INT64       *UserVarPtr);
inline MIL_INT MFTYPE MseqInquireSafeType(MIL_ID ContextSeqId, MIL_INT SequenceIndex, MIL_INT64 InquireType, MIL_TEXT_CHAR   *UserVarPtr);
inline MIL_INT MFTYPE MseqInquireSafeType(MIL_ID ContextSeqId, MIL_INT SequenceIndex, MIL_INT64 InquireType, MIL_DOUBLE      *UserVarPtr);
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MseqInquireSafeType(MIL_ID ContextSeqId, MIL_INT SequenceIndex, MIL_INT64 InquireType, MIL_UINT32      *UserVarPtr);
inline MIL_INT MFTYPE MseqInquireSafeType(MIL_ID ContextSeqId, MIL_INT SequenceIndex, MIL_INT64 InquireType, MIL_UINT64      *UserVarPtr);
#endif



// -------------------------------------------------------------------------
// MappGetError

inline MIL_INT64 MappGetErrorRequiredType(MIL_INT64 ErrorType)
   {
   if (((ErrorType&~M_CLIENT_ASCII_MODE) & M_MESSAGE) == M_MESSAGE)
      return M_TYPE_TEXT_CHAR;

   return M_TYPE_MIL_INT;
   }

inline MIL_INT MFTYPE MappGetErrorSafeType(MIL_INT64 ErrorType, int ErrorPtr)
   {
   if (ErrorPtr || (((ErrorType&~M_CLIENT_ASCII_MODE) & M_MESSAGE) == M_MESSAGE))
      SafeTypeError(MIL_TEXT("MappGetError"));

   return MappGetError(ErrorType, NULL);
   }

inline MIL_INT MFTYPE MappGetErrorSafeTypeExecute (MIL_INT64 ErrorType, void *ErrorPtr, MIL_INT64 GivenType)
   {
   MIL_INT64 RequiredType = MappGetErrorRequiredType(ErrorType);
   ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MappGetError"));

   return MappGetError(ErrorType, ErrorPtr);
   }

inline MIL_INT MFTYPE MappGetErrorUnsafe  (MIL_INT64 ErrorType, void           *ErrorPtr) {return MappGetError               (ErrorType, ErrorPtr                  );}
inline MIL_INT MFTYPE MappGetErrorSafeType(MIL_INT64 ErrorType, MIL_TEXT_CHAR  *ErrorPtr) {return MappGetErrorSafeTypeExecute(ErrorType, ErrorPtr, M_TYPE_TEXT_CHAR);}
inline MIL_INT MFTYPE MappGetErrorSafeType(MIL_INT64 ErrorType, MIL_INT        *ErrorPtr) {return MappGetErrorSafeTypeExecute(ErrorType, ErrorPtr, M_TYPE_MIL_INT  );}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MappGetErrorSafeType(MIL_INT64 ErrorType, MIL_UINT       *ErrorPtr) {return MappGetErrorSafeTypeExecute(ErrorType, ErrorPtr, M_TYPE_MIL_INT);}
#endif
inline MIL_INT MFTYPE MappGetErrorSafeType(MIL_ID ContextAppId, MIL_INT64 ErrorType, int ErrorPtr)
   {
   if (ErrorPtr || ((ErrorType & M_MESSAGE) == M_MESSAGE))
      SafeTypeError(MIL_TEXT("MappGetError"));

   return MappGetError(ContextAppId, ErrorType, NULL);
   }

inline MIL_INT MFTYPE MappGetErrorSafeTypeExecute (MIL_ID ContextAppId, MIL_INT64 ErrorType, void  *ErrorPtr, MIL_INT64 GivenType)
   {
   MIL_INT64 RequiredType = MappGetErrorRequiredType(ErrorType);
   ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MappGetError"));

   return MappGetError(ContextAppId, ErrorType, ErrorPtr);
   }

inline MIL_INT MFTYPE MappGetErrorUnsafe  (MIL_ID ContextAppId, MIL_INT64 ErrorType, void           *ErrorPtr) {return MappGetError               (ContextAppId, ErrorType, ErrorPtr                  );}
inline MIL_INT MFTYPE MappGetErrorSafeType(MIL_ID ContextAppId, MIL_INT64 ErrorType, MIL_TEXT_CHAR  *ErrorPtr) {return MappGetErrorSafeTypeExecute(ContextAppId, ErrorType, ErrorPtr, M_TYPE_TEXT_CHAR);}
inline MIL_INT MFTYPE MappGetErrorSafeType(MIL_ID ContextAppId, MIL_INT64 ErrorType, MIL_INT        *ErrorPtr) {return MappGetErrorSafeTypeExecute(ContextAppId, ErrorType, ErrorPtr, M_TYPE_MIL_INT  );}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MappGetErrorSafeType(MIL_ID ContextAppId, MIL_INT64 ErrorType, MIL_UINT       *ErrorPtr) {return MappGetErrorSafeTypeExecute(ContextAppId, ErrorType, ErrorPtr, M_TYPE_MIL_INT);}
#endif


// ----------------------------------------------------------
// MappGetHookInfo


inline MIL_INT64 MappGetHookInfoRequiredType(MIL_ID EventId, MIL_INT64 InfoType)
   {
   if (((InfoType&~M_CLIENT_ASCII_MODE) & M_MESSAGE) == M_MESSAGE)
      return M_TYPE_TEXT_CHAR;

   if((InfoType & M_PARAM_VALUE) == M_PARAM_VALUE)
      {
      MIL_INT64 InquireParamType = ((InfoType & ~M_PARAM_VALUE) + M_EXTENDED_PARAM_TYPE);
      MIL_INT64 ParamType;
      MappGetHookInfo(EventId, InquireParamType, &ParamType);
      if (ParamType == M_TYPE_ARRAY_ID_PTR || ParamType == M_TYPE_PTR || ParamType == M_TYPE_FILENAME || ParamType == M_TYPE_STRING )
         ParamType = M_SAFE_TYPE_CHECK_PTR;
      return ParamType;
      }
   
   return M_TYPE_MIL_INT;
   }

inline MIL_INT MFTYPE MappGetHookInfoSafeTypeExecute (MIL_ID EventId, MIL_INT64 InfoType, void        *ValuePtr, MIL_INT64 GivenType)
   {
   MIL_INT64 RequiredType = MappGetHookInfoRequiredType(EventId, InfoType);
   ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MappGetHookInfo"));

   return MappGetHookInfo(EventId, InfoType, ValuePtr);
   }

inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, int ResultPtr)
   {
   if (ResultPtr)
      SafeTypeError(MIL_TEXT("MappGetHookInfo"));

   return MappGetHookInfo(EventId, InfoType, NULL);
   }

inline MIL_INT MFTYPE MappGetHookInfoUnsafe  (MIL_ID EventId, MIL_INT64 InfoType, void            *ResultPtr) {return MappGetHookInfo               (EventId, InfoType, ResultPtr)                    ;}
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_INT8        *ResultPtr) {return MappGetHookInfoSafeTypeExecute(EventId, InfoType, ResultPtr, M_TYPE_CHAR       );}
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_INT16       *ResultPtr) {return MappGetHookInfoSafeTypeExecute(EventId, InfoType, ResultPtr, M_TYPE_SHORT      );}
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_INT32       *ResultPtr) {return MappGetHookInfoSafeTypeExecute(EventId, InfoType, ResultPtr, M_TYPE_MIL_INT32  );}
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_INT64       *ResultPtr) {return MappGetHookInfoSafeTypeExecute(EventId, InfoType, ResultPtr, M_TYPE_MIL_INT64  );}
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_DOUBLE      *ResultPtr) {return MappGetHookInfoSafeTypeExecute(EventId, InfoType, ResultPtr, M_TYPE_DOUBLE     );}
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, void           **ResultPtr) {return MappGetHookInfoSafeTypeExecute(EventId, InfoType, ResultPtr, M_SAFE_TYPE_CHECK_PTR);}
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, const void     **ResultPtr) {return MappGetHookInfoSafeTypeExecute(EventId, InfoType, ResultPtr, M_SAFE_TYPE_CHECK_PTR);}
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_ID         **ResultPtr) {return MappGetHookInfoSafeTypeExecute(EventId, InfoType, ResultPtr, M_SAFE_TYPE_CHECK_PTR);}
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, const MIL_ID   **ResultPtr) {return MappGetHookInfoSafeTypeExecute(EventId, InfoType, ResultPtr, M_SAFE_TYPE_CHECK_PTR);}
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_TEXT_PTR    *ResultPtr) {return MappGetHookInfoSafeTypeExecute(EventId, InfoType, ResultPtr, M_SAFE_TYPE_CHECK_PTR);}
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_CONST_TEXT_PTR *ResultPtr) {return MappGetHookInfoSafeTypeExecute(EventId, InfoType, ResultPtr, M_SAFE_TYPE_CHECK_PTR);}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_UINT8       *ResultPtr) {return MappGetHookInfoSafeTypeExecute(EventId, InfoType, ResultPtr, M_TYPE_CHAR       );}
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_UINT16      *ResultPtr) {return MappGetHookInfoSafeTypeExecute(EventId, InfoType, ResultPtr, M_TYPE_SHORT      );}
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_UINT32      *ResultPtr) {return MappGetHookInfoSafeTypeExecute(EventId, InfoType, ResultPtr, M_TYPE_MIL_INT32  );}
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_UINT64      *ResultPtr) {return MappGetHookInfoSafeTypeExecute(EventId, InfoType, ResultPtr, M_TYPE_MIL_INT64  );}
#endif
#if M_MIL_SAFE_TYPE_ADD_WCHAR_T
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, wchar_t         *ResultPtr) {return MappGetHookInfoSafeTypeExecute(EventId, InfoType, ResultPtr, M_TYPE_MIL_TEXT   );}
#endif

inline MIL_INT MFTYPE MappGetHookInfoSafeTypeExecute(MIL_ID ContextAppId, MIL_ID EventId, MIL_INT64 InfoType, void        *ValuePtr, MIL_INT64 GivenType)
   {
   MIL_INT64 RequiredType = MappGetHookInfoRequiredType(EventId, InfoType);
   ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MappGetHookInfo"));

   return MappGetHookInfo(ContextAppId, EventId, InfoType, ValuePtr);
   }

inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID ContextAppId, MIL_ID EventId, MIL_INT64 InfoType, int ResultPtr)
   {
   if (ResultPtr)
      SafeTypeError(MIL_TEXT("MappGetHookInfo"));

   return MappGetHookInfo(ContextAppId, EventId, InfoType, NULL);
   }

inline MIL_INT MFTYPE MappGetHookInfoUnsafe  (MIL_ID ContextAppId, MIL_ID EventId, MIL_INT64 InfoType, void            *ResultPtr) {return MappGetHookInfo               (ContextAppId, EventId, InfoType, ResultPtr)                    ;}
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID ContextAppId, MIL_ID EventId, MIL_INT64 InfoType, MIL_INT8        *ResultPtr) {return MappGetHookInfoSafeTypeExecute(ContextAppId, EventId, InfoType, ResultPtr, M_TYPE_CHAR       );}
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID ContextAppId, MIL_ID EventId, MIL_INT64 InfoType, MIL_INT16       *ResultPtr) {return MappGetHookInfoSafeTypeExecute(ContextAppId, EventId, InfoType, ResultPtr, M_TYPE_SHORT      );}
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID ContextAppId, MIL_ID EventId, MIL_INT64 InfoType, MIL_INT32       *ResultPtr) {return MappGetHookInfoSafeTypeExecute(ContextAppId, EventId, InfoType, ResultPtr, M_TYPE_MIL_INT32  );}
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID ContextAppId, MIL_ID EventId, MIL_INT64 InfoType, MIL_INT64       *ResultPtr) {return MappGetHookInfoSafeTypeExecute(ContextAppId, EventId, InfoType, ResultPtr, M_TYPE_MIL_INT64  );}
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID ContextAppId, MIL_ID EventId, MIL_INT64 InfoType, MIL_DOUBLE      *ResultPtr) {return MappGetHookInfoSafeTypeExecute(ContextAppId, EventId, InfoType, ResultPtr, M_TYPE_DOUBLE     );}
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID ContextAppId, MIL_ID EventId, MIL_INT64 InfoType, void           **ResultPtr) {return MappGetHookInfoSafeTypeExecute(ContextAppId, EventId, InfoType, ResultPtr, M_SAFE_TYPE_CHECK_PTR);}
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID ContextAppId, MIL_ID EventId, MIL_INT64 InfoType, const void     **ResultPtr) {return MappGetHookInfoSafeTypeExecute(ContextAppId, EventId, InfoType, ResultPtr, M_SAFE_TYPE_CHECK_PTR);}
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID ContextAppId, MIL_ID EventId, MIL_INT64 InfoType, MIL_ID         **ResultPtr) {return MappGetHookInfoSafeTypeExecute(ContextAppId, EventId, InfoType, ResultPtr, M_SAFE_TYPE_CHECK_PTR);}
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID ContextAppId, MIL_ID EventId, MIL_INT64 InfoType, const MIL_ID   **ResultPtr) {return MappGetHookInfoSafeTypeExecute(ContextAppId, EventId, InfoType, ResultPtr, M_SAFE_TYPE_CHECK_PTR);}
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID ContextAppId, MIL_ID EventId, MIL_INT64 InfoType, MIL_TEXT_PTR    *ResultPtr) {return MappGetHookInfoSafeTypeExecute(ContextAppId, EventId, InfoType, ResultPtr, M_SAFE_TYPE_CHECK_PTR);}
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID ContextAppId, MIL_ID EventId, MIL_INT64 InfoType, MIL_CONST_TEXT_PTR *ResultPtr) {return MappGetHookInfoSafeTypeExecute(ContextAppId, EventId, InfoType, ResultPtr, M_SAFE_TYPE_CHECK_PTR);}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID ContextAppId, MIL_ID EventId, MIL_INT64 InfoType, MIL_UINT8       *ResultPtr) {return MappGetHookInfoSafeTypeExecute(ContextAppId, EventId, InfoType, ResultPtr, M_TYPE_CHAR       );}
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID ContextAppId, MIL_ID EventId, MIL_INT64 InfoType, MIL_UINT16      *ResultPtr) {return MappGetHookInfoSafeTypeExecute(ContextAppId, EventId, InfoType, ResultPtr, M_TYPE_SHORT      );}
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID ContextAppId, MIL_ID EventId, MIL_INT64 InfoType, MIL_UINT32      *ResultPtr) {return MappGetHookInfoSafeTypeExecute(ContextAppId, EventId, InfoType, ResultPtr, M_TYPE_MIL_INT32  );}
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID ContextAppId, MIL_ID EventId, MIL_INT64 InfoType, MIL_UINT64      *ResultPtr) {return MappGetHookInfoSafeTypeExecute(ContextAppId, EventId, InfoType, ResultPtr, M_TYPE_MIL_INT64  );}
#endif
#if M_MIL_SAFE_TYPE_ADD_WCHAR_T
inline MIL_INT MFTYPE MappGetHookInfoSafeType(MIL_ID ContextAppId, MIL_ID EventId, MIL_INT64 InfoType, wchar_t         *ResultPtr) {return MappGetHookInfoSafeTypeExecute(ContextAppId, EventId, InfoType, ResultPtr, M_TYPE_MIL_TEXT   );}
#endif


//-----------------------------------------------------------------------------
// MappInquire

inline MIL_INT64 MappInquireRequiredType(MIL_INT64 InquireType)
   {
   if (M_IN_APP_INQUIRE_STRING_RANGE(InquireType))
      {
      return M_TYPE_TEXT_CHAR;
      }


   if(M_IN_APP_INQUIRE_DOUBLE_RANGE(InquireType))
      {
      return M_TYPE_DOUBLE;
      }

   if(M_IN_APP_INQUIRE_MIL_ID_RANGE(InquireType))
      {
      return M_TYPE_MIL_ID;
      }

   if(M_IN_APP_INQUIRE_MIL_INT64_RANGE(InquireType))
      {
      return M_TYPE_MIL_INT64;
      }

   return M_TYPE_MIL_INT;
   }


inline MIL_INT MFTYPE MappInquireSafeType(MIL_INT64 InquireType, int UserVarPtr)
   {
   if (UserVarPtr)
      SafeTypeError(MIL_TEXT("MappInquire"));

   return MappInquire(InquireType, NULL);
   }

inline MIL_INT MFTYPE MappInquireSafeTypeExecute (MIL_INT64 InquireType, void        *ValuePtr, MIL_INT64 GivenType)
   {
   MIL_INT64 RequiredType = MappInquireRequiredType(InquireType);
   ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MappInquire"));

   return MappInquire(InquireType, ValuePtr);
   }

inline MIL_INT MFTYPE MappInquireUnsafe  (MIL_INT64 InquireType, void           *UserVarPtr) {return MappInquire               (InquireType, UserVarPtr                  );}
inline MIL_INT MFTYPE MappInquireSafeType(MIL_INT64 InquireType, MIL_TEXT_CHAR  *UserVarPtr) {return MappInquireSafeTypeExecute(InquireType, UserVarPtr, M_TYPE_TEXT_CHAR);}
inline MIL_INT MFTYPE MappInquireSafeType(MIL_INT64 InquireType, MIL_INT        *UserVarPtr) {return MappInquireSafeTypeExecute(InquireType, UserVarPtr, M_TYPE_MIL_INT  );}
inline MIL_INT MFTYPE MappInquireSafeType(MIL_INT64 InquireType, MIL_DOUBLE     *UserVarPtr) {return MappInquireSafeTypeExecute(InquireType, UserVarPtr, M_TYPE_DOUBLE   );}
#if !M_MIL_USE_64BIT
inline MIL_INT MFTYPE MappInquireSafeType(MIL_INT64 InquireType, MIL_INT64      *UserVarPtr) {return MappInquireSafeTypeExecute(InquireType, UserVarPtr, M_TYPE_MIL_INT64);}
#endif
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MappInquireSafeType(MIL_INT64 InquireType, MIL_UINT       *UserVarPtr) {return MappInquireSafeTypeExecute(InquireType, UserVarPtr, M_TYPE_MIL_INT  );}
#endif
inline MIL_INT MFTYPE MappInquireSafeType(MIL_ID ContextAppId, MIL_INT64 InquireType, int UserVarPtr)
   {
   if (UserVarPtr)
      SafeTypeError(MIL_TEXT("MappInquire"));

   return MappInquire(ContextAppId, InquireType, NULL);
   }

inline MIL_INT MFTYPE MappInquireSafeTypeExecute(MIL_ID ContextAppId, MIL_INT64 InquireType, void        *ValuePtr, MIL_INT64 GivenType)
   {
   MIL_INT64 RequiredType = MappInquireRequiredType(InquireType);
   ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MappInquire"));

   return MappInquire(ContextAppId, InquireType, ValuePtr);
   }

inline MIL_INT MFTYPE MappInquireUnsafe  (MIL_ID ContextAppId, MIL_INT64 InquireType, void           *UserVarPtr) {return MappInquire               (ContextAppId, InquireType, UserVarPtr                  );}
inline MIL_INT MFTYPE MappInquireSafeType(MIL_ID ContextAppId, MIL_INT64 InquireType, MIL_TEXT_CHAR  *UserVarPtr) {return MappInquireSafeTypeExecute(ContextAppId, InquireType, UserVarPtr, M_TYPE_TEXT_CHAR);}
inline MIL_INT MFTYPE MappInquireSafeType(MIL_ID ContextAppId, MIL_INT64 InquireType, MIL_INT        *UserVarPtr) {return MappInquireSafeTypeExecute(ContextAppId, InquireType, UserVarPtr, M_TYPE_MIL_INT  );}
inline MIL_INT MFTYPE MappInquireSafeType(MIL_ID ContextAppId, MIL_INT64 InquireType, MIL_DOUBLE     *UserVarPtr) {return MappInquireSafeTypeExecute(ContextAppId, InquireType, UserVarPtr, M_TYPE_DOUBLE   );}
#if !M_MIL_USE_64BIT
inline MIL_INT MFTYPE MappInquireSafeType(MIL_ID ContextAppId, MIL_INT64 InquireType, MIL_INT64      *UserVarPtr) {return MappInquireSafeTypeExecute(ContextAppId, InquireType, UserVarPtr, M_TYPE_MIL_INT64);}
#endif
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MappInquireSafeType(MIL_ID ContextAppId, MIL_INT64 InquireType, MIL_UINT       *UserVarPtr) {return MappInquireSafeTypeExecute(ContextAppId, InquireType, UserVarPtr, M_TYPE_MIL_INT  );}
#endif

#if M_MIL_USE_UNICODE
// ----------------------------------------------------------
// MappFileOperation
inline void MFTYPE MappFileOperationUnsafeA(    MIL_ID                  Comp1ContextAppId,
                                                const char*             Comp1FileName,
                                                MIL_ID                  Comp2ContextAppId,
                                                const char*             Comp2FileName,
                                                MIL_INT64               Operation,
                                                MIL_INT64               OperationFlag,
                                                void*                   OperationDataPtr)
   {
   MappFileOperationA(Comp1ContextAppId, Comp1FileName, Comp2ContextAppId, Comp2FileName, Operation, OperationFlag, OperationDataPtr);
   }
inline void MFTYPE MappFileOperationUnsafeW(    MIL_ID                  Comp1ContextAppId,
                                                MIL_CONST_TEXT_PTR      Comp1FileName,
                                                MIL_ID                  Comp2ContextAppId,
                                                MIL_CONST_TEXT_PTR      Comp2FileName,
                                                MIL_INT64               Operation,
                                                MIL_INT64               OperationFlag,
                                                void*                   OperationDataPtr)
   {
   MappFileOperationW(Comp1ContextAppId, Comp1FileName, Comp2ContextAppId, Comp2FileName, Operation, OperationFlag, OperationDataPtr);
   }
inline void MFTYPE MappFileOperationSafeTypeW(  MIL_ID                  Comp1ContextAppId,
                                                MIL_CONST_TEXT_PTR      Comp1FileName,
                                                MIL_ID                  Comp2ContextAppId,
                                                MIL_CONST_TEXT_PTR      Comp2FileName,
                                                MIL_INT64               Operation,
                                                MIL_INT64               OperationFlag,
                                                int                     OperationDataPtr)
   {
   if (OperationDataPtr)
      SafeTypeError(MIL_TEXT("MappFileOperation"));

   MappFileOperationW(Comp1ContextAppId, Comp1FileName, Comp2ContextAppId, Comp2FileName, Operation, OperationFlag, NULL);
   }
inline void MFTYPE MappFileOperationSafeTypeW(  MIL_ID                  Comp1ContextAppId,
                                                int                     Comp1FileName,
                                                MIL_ID                  Comp2ContextAppId,
                                                MIL_CONST_TEXT_PTR      Comp2FileName,
                                                MIL_INT64               Operation,
                                                MIL_INT64               OperationFlag,
                                                int                     OperationDataPtr)
   {
   if (Comp1FileName || OperationDataPtr)
      SafeTypeError(MIL_TEXT("MappFileOperation"));

   MappFileOperationW(Comp1ContextAppId, NULL, Comp2ContextAppId, Comp2FileName, Operation, OperationFlag, NULL);
   }
inline void MFTYPE MappFileOperationSafeTypeW(  MIL_ID                 Comp1ContextAppId,
                                                MIL_CONST_TEXT_PTR     Comp1FileName,
                                                MIL_ID                 Comp2ContextAppId,
                                                int                    Comp2FileName,
                                                MIL_INT64               Operation,
                                                MIL_INT64               OperationFlag,
                                                int                     OperationDataPtr)
   {
   if (Comp2FileName || OperationDataPtr)
      SafeTypeError(MIL_TEXT("MappFileOperation"));

   MappFileOperationW(Comp1ContextAppId, Comp1FileName, Comp2ContextAppId, NULL, Operation, OperationFlag, NULL);
   }
inline void MFTYPE MappFileOperationSafeTypeW(  MIL_ID                     Comp1ContextAppId,
                                                MIL_CONST_TEXT_PTR         Comp1FileName,
                                                MIL_ID                     Comp2ContextAppId,
                                                int                        Comp2FileName,
                                                MIL_INT64                  Operation,
                                                MIL_INT64                  OperationFlag,
                                                MIL_INT*                   OperationDataPtr)
   {
   if(Comp2FileName)
      SafeTypeError(MIL_TEXT("MappFileOperation"));

   MappFileOperationW(Comp1ContextAppId, Comp1FileName, Comp2ContextAppId, NULL, Operation, OperationFlag, (void*)OperationDataPtr);
   }
inline void MFTYPE MappFileOperationSafeTypeA(  MIL_ID                  Comp1ContextAppId,
                                                const char*             Comp1FileName,
                                                MIL_ID                  Comp2ContextAppId,
                                                const char*             Comp2FileName,
                                                MIL_INT64               Operation,
                                                MIL_INT64               OperationFlag,
                                                int                     OperationDataPtr)
   {
   if (OperationDataPtr)
      SafeTypeError(MIL_TEXT("MappFileOperation"));

   MappFileOperationA(Comp1ContextAppId, Comp1FileName, Comp2ContextAppId, Comp2FileName, Operation, OperationFlag, NULL);
   }
inline void MFTYPE MappFileOperationSafeTypeA(  MIL_ID                  Comp1ContextAppId,
                                                int                     Comp1FileName,
                                                MIL_ID                  Comp2ContextAppId,
                                                const char*             Comp2FileName,
                                                MIL_INT64               Operation,
                                                MIL_INT64               OperationFlag,
                                                int                     OperationDataPtr)
   {
   if (Comp1FileName || OperationDataPtr)
      SafeTypeError(MIL_TEXT("MappFileOperation"));

   MappFileOperationA(Comp1ContextAppId, NULL, Comp2ContextAppId, Comp2FileName, Operation, OperationFlag, NULL);
   }
inline void MFTYPE MappFileOperationSafeTypeA(  MIL_ID                  Comp1ContextAppId,
                                                const char*             Comp1FileName,
                                                MIL_ID                  Comp2ContextAppId,
                                                int                     Comp2FileName,
                                                MIL_INT64               Operation,
                                                MIL_INT64               OperationFlag,
                                                int                     OperationDataPtr)
   {
   if (Comp2FileName || OperationDataPtr)
      SafeTypeError(MIL_TEXT("MappFileOperation"));

   MappFileOperationA(Comp1ContextAppId, Comp1FileName, Comp2ContextAppId, NULL, Operation, OperationFlag, NULL);
   }
inline void MFTYPE MappFileOperationSafeTypeA(  MIL_ID                     Comp1ContextAppId,
                                                const char*                Comp1FileName,
                                                MIL_ID                     Comp2ContextAppId,
                                                int                        Comp2FileName,
                                                MIL_INT64                  Operation,
                                                MIL_INT64                  OperationFlag,
                                                MIL_INT*                   OperationDataPtr)
   {
   if(Comp2FileName)
      SafeTypeError(MIL_TEXT("MappFileOperation"));

   MappFileOperationA(Comp1ContextAppId, Comp1FileName, Comp2ContextAppId, NULL, Operation, OperationFlag, (void*)OperationDataPtr);
   }


#if M_MIL_UNICODE_API
#define MappFileOperationUnsafe      MappFileOperationUnsafeW
#else
#define MappFileOperationUnsafe      MappFileOperationUnsafeA
#endif
#else

inline void MFTYPE MappFileOperationUnsafe(     MIL_ID                  Comp1ContextAppId,
                                                const char*             Comp1FileName,
                                                MIL_ID                  Comp2ContextAppId,
                                                const char*             Comp2FileName, 
                                                MIL_INT64               Operation,
                                                MIL_INT64               OperationFlag,
                                                void*                   OperationDataPtr)
   {
   MappFileOperation(Comp1ContextAppId, Comp1FileName, Comp2ContextAppId, Comp2FileName, Operation, OperationFlag, OperationDataPtr);
   }

inline void MFTYPE MappFileOperationSafeType(   MIL_ID                  Comp1ContextAppId,
                                                const char*             Comp1FileName,
                                                MIL_ID                  Comp2ContextAppId,
                                                const char*             Comp2FileName,
                                                MIL_INT64               Operation,
                                                MIL_INT64               OperationFlag,
                                                int                     OperationDataPtr)
   {
   if (OperationDataPtr)
      SafeTypeError(MIL_TEXT("MappFileOperation"));

   MappFileOperation(Comp1ContextAppId, Comp1FileName, Comp2ContextAppId, Comp2FileName, Operation, OperationFlag, NULL);
   }
inline void MFTYPE MappFileOperationSafeType(   MIL_ID                  Comp1ContextAppId,
                                                int                     Comp1FileName,
                                                MIL_ID                  Comp2ContextAppId,
                                                const char*             Comp2FileName,
                                                MIL_INT64               Operation,
                                                MIL_INT64               OperationFlag,
                                                int                     OperationDataPtr)
   {
   if (FileName || OperationDataPtr)
      SafeTypeError(MIL_TEXT("MappFileOperation"));

   MappFileOperation(Comp1ContextAppId, NULL, Comp2ContextAppId, Comp2FileName, Operation, OperationFlag, NULL);
   }
inline void MFTYPE MappFileOperationSafeTypeA(  MIL_ID                  Comp1ContextAppId,
                                                const char*             Comp1FileName,
                                                MIL_ID                  Comp2ContextAppId,
                                                int                     Comp2FileName,
                                                MIL_INT64               Operation,
                                                MIL_INT64               OperationFlag,
                                                int                     OperationDataPtr)
   {
   if (Comp2FileName || OperationDataPtr)
      SafeTypeError(MIL_TEXT("MappFileOperation"));

   MappFileOperation(Comp1ContextAppId, Comp1FileName, Comp2ContextAppId, NULL, Operation, OperationFlag, NULL);
   }

inline void MFTYPE MappFileOperationSafeType(   MIL_ID                     Comp1ContextAppId,
                                                const char*                Comp1FileName,
                                                MIL_ID                     Comp2ContextAppId,
                                                int                        Comp2FileName,
                                                MIL_INT64                  Operation,
                                                MIL_INT64                  OperationFlag,
                                                MIL_INT*                   OperationDataPtr)
   {
   if(Comp2FileName)
      SafeTypeError(MIL_TEXT("MappFileOperation"));

   MappFileOperation(Comp1ContextAppId, Comp1FileName, Comp2ContextAppId, NULL, Operation, OperationFlag, (void*)OperationDataPtr);
   }
#endif //M_MIL_USE_UNICODE
// ----------------------------------------------------------
// MbufDiskInquire

inline MIL_INT64 MbufDiskInquireRequiredType(MIL_INT64 InquireType)
   {
   if (
      InquireType == M_ASPECT_RATIO ||
      InquireType == M_FRAME_RATE
      )
      return M_TYPE_DOUBLE;

   return M_TYPE_MIL_INT;
   }

#if M_MIL_USE_UNICODE

inline MIL_INT MFTYPE MbufDiskInquireSafeTypeA(const char*        FileName, MIL_INT64 InquireType, int UserVarPtr)
   {
   if (UserVarPtr)
      SafeTypeError(MIL_TEXT("MbufDiskInquireA"));

   return MbufDiskInquireA(FileName, InquireType, NULL);
   }

inline MIL_INT MFTYPE MbufDiskInquireSafeTypeW(MIL_CONST_TEXT_PTR FileName, MIL_INT64 InquireType, int UserVarPtr)
   {
   if (UserVarPtr)
      SafeTypeError(MIL_TEXT("MbufDiskInquireW"));

   return MbufDiskInquireW(FileName, InquireType, NULL);
   }

inline MIL_INT MFTYPE MbufDiskInquireSafeTypeAExecute(const char*        FileName, MIL_INT64 InquireType, void     *UserVarPtr, MIL_INT64 GivenType)
   {
   MIL_INT64 RequiredType = MbufDiskInquireRequiredType(InquireType);
   ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MbufDiskInquireSafeTypeA"));

   return MbufDiskInquireA(FileName, InquireType, UserVarPtr);;
   }

inline MIL_INT MFTYPE MbufDiskInquireSafeTypeWExecute(MIL_CONST_TEXT_PTR     FileName, MIL_INT64 InquireType, void     *UserVarPtr, MIL_INT64 GivenType)
   {
   MIL_INT64 RequiredType = MbufDiskInquireRequiredType(InquireType);
   ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MbufDiskInquireSafeTypeW"));

   return MbufDiskInquireW(FileName, InquireType, UserVarPtr);;
   }

inline MIL_INT MFTYPE MbufDiskInquireUnsafeA  (const char*        FileName, MIL_INT64 InquireType, void        *UserVarPtr) {return MbufDiskInquireA(FileName, InquireType, UserVarPtr);}
inline MIL_INT MFTYPE MbufDiskInquireSafeTypeA(const char*        FileName, MIL_INT64 InquireType, MIL_INT     *UserVarPtr) {return MbufDiskInquireSafeTypeAExecute(FileName, InquireType, UserVarPtr, M_TYPE_MIL_INT);}
inline MIL_INT MFTYPE MbufDiskInquireSafeTypeA(const char*        FileName, MIL_INT64 InquireType, MIL_DOUBLE  *UserVarPtr) {return MbufDiskInquireSafeTypeAExecute(FileName, InquireType, UserVarPtr, M_TYPE_DOUBLE );}

inline MIL_INT MFTYPE MbufDiskInquireUnsafeW  (MIL_CONST_TEXT_PTR FileName, MIL_INT64 InquireType, void        *UserVarPtr) {return MbufDiskInquireW(FileName, InquireType, UserVarPtr);}
inline MIL_INT MFTYPE MbufDiskInquireSafeTypeW(MIL_CONST_TEXT_PTR FileName, MIL_INT64 InquireType, MIL_INT     *UserVarPtr) {return MbufDiskInquireSafeTypeWExecute(FileName, InquireType, UserVarPtr, M_TYPE_MIL_INT);}
inline MIL_INT MFTYPE MbufDiskInquireSafeTypeW(MIL_CONST_TEXT_PTR FileName, MIL_INT64 InquireType, MIL_DOUBLE  *UserVarPtr) {return MbufDiskInquireSafeTypeWExecute(FileName, InquireType, UserVarPtr, M_TYPE_DOUBLE );}

#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MbufDiskInquireSafeTypeA(const char*        FileName, MIL_INT64 InquireType, MIL_UINT    *UserVarPtr) {return MbufDiskInquireSafeTypeAExecute(FileName, InquireType, UserVarPtr, M_TYPE_MIL_INT);}
inline MIL_INT MFTYPE MbufDiskInquireSafeTypeW(MIL_CONST_TEXT_PTR FileName, MIL_INT64 InquireType, MIL_UINT    *UserVarPtr) {return MbufDiskInquireSafeTypeWExecute(FileName, InquireType, UserVarPtr, M_TYPE_MIL_INT);}
#endif

#if M_MIL_UNICODE_API
#define MbufDiskInquireUnsafe      MbufDiskInquireUnsafeW
#else
#define MbufDiskInquireUnsafe      MbufDiskInquireUnsafeA
#endif

#else

inline MIL_INT MFTYPE MbufDiskInquireSafeType(MIL_CONST_TEXT_PTR FileName, MIL_INT64 InquireType, int UserVarPtr)
   {
   if (UserVarPtr != NULL)
      SafeTypeError(MIL_TEXT("MbufDiskInquire"));

   return MbufDiskInquire(FileName, InquireType, NULL);
   }

inline MIL_INT MFTYPE MbufDiskInquireSafeTypeExecute(MIL_CONST_TEXT_PTR     FileName, MIL_INT64 InquireType, void     *UserVarPtr, MIL_INT64 GivenType)
   {
   MIL_INT64 RequiredType = MbufDiskInquireRequiredType(InquireType);
   ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MbufDiskInquireSafeType"));

   return MbufDiskInquire(FileName, InquireType, UserVarPtr);;
   }
inline MIL_INT MFTYPE MbufDiskInquireUnsafe  (MIL_CONST_TEXT_PTR FileName, MIL_INT64 InquireType, void        *UserVarPtr) {return MbufDiskInquire(FileName, InquireType, UserVarPtr);}
inline MIL_INT MFTYPE MbufDiskInquireSafeType(MIL_CONST_TEXT_PTR FileName, MIL_INT64 InquireType, MIL_INT     *UserVarPtr) {return MbufDiskInquireSafeTypeExecute(FileName, InquireType, UserVarPtr, M_TYPE_MIL_INT);}
inline MIL_INT MFTYPE MbufDiskInquireSafeType(MIL_CONST_TEXT_PTR FileName, MIL_INT64 InquireType, MIL_DOUBLE  *UserVarPtr) {return MbufDiskInquireSafeTypeExecute(FileName, InquireType, UserVarPtr, M_TYPE_DOUBLE );}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MbufDiskInquireSafeType(MIL_CONST_TEXT_PTR FileName, MIL_INT64 InquireType, MIL_UINT    *UserVarPtr) {return MbufDiskInquireSafeTypeExecute(FileName, InquireType, UserVarPtr, M_TYPE_MIL_INT);}
#endif

#endif
// ----------------------------------------------------------
// MbufGetHookInfo


inline MIL_INT64 MbufGetHookInfoRequiredType(MIL_INT64 InfoType)
   {
   switch (InfoType)
      {
      case M_MODIFIED_BUFFER+M_BUFFER_ID:
         return M_TYPE_MIL_ID;
         break;

      case M_MODIFIED_BUFFER+M_GRAB_TIME_STAMP:
      case M_MODIFIED_BUFFER+M_TIMER_VALUE:
      case M_MODIFIED_BUFFER+M_TIMER_VALUE+M_TIMER1:
      case M_MODIFIED_BUFFER+M_TIMER_VALUE+M_TIMER2:
      case M_MODIFIED_BUFFER+M_TIMER_VALUE+M_TIMER3:
      case M_MODIFIED_BUFFER+M_TIMER_VALUE+M_TIMER4:
      case M_MODIFIED_BUFFER+M_TIMER_VALUE+M_TIMER5:
      case M_MODIFIED_BUFFER+M_TIMER_VALUE+M_TIMER6:
      case M_MODIFIED_BUFFER+M_TIMER_VALUE+M_TIMER7:
      case M_MODIFIED_BUFFER+M_TIMER_VALUE+M_TIMER8:

        return M_TYPE_DOUBLE;
        break;
      }

   return M_TYPE_MIL_INT;
   }

inline MIL_INT MFTYPE MbufGetHookInfoSafeTypeExecute (MIL_ID EventId, MIL_INT64 InfoType, void        *ValuePtr, MIL_INT64 GivenType)
   {
   MIL_INT64 RequiredType = MbufGetHookInfoRequiredType(InfoType);
   ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MbufGetHookInfo"));

   return MbufGetHookInfo(EventId, InfoType, ValuePtr);
   }

inline MIL_INT MFTYPE MbufGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, int ResultPtr)
   {
   if (ResultPtr)
      SafeTypeError(MIL_TEXT("MbufGetHookInfo"));

   return MbufGetHookInfo(EventId, InfoType, NULL);
   }

inline MIL_INT MFTYPE MbufGetHookInfoUnsafe  (MIL_ID EventId, MIL_INT64 InfoType, void            *ResultPtr) {return MbufGetHookInfo               (EventId, InfoType, ResultPtr)                  ;}
inline MIL_INT MFTYPE MbufGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_INT         *ResultPtr) {return MbufGetHookInfoSafeTypeExecute(EventId, InfoType, ResultPtr, M_TYPE_MIL_INT  );}
inline MIL_INT MFTYPE MbufGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_DOUBLE      *ResultPtr) {return MbufGetHookInfoSafeTypeExecute(EventId, InfoType, ResultPtr, M_TYPE_DOUBLE   );}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MbufGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_UINT        *ResultPtr) {return MbufGetHookInfoSafeTypeExecute(EventId, InfoType, ResultPtr, M_TYPE_MIL_INT  );}
#endif

// ----------------------------------------------------------
// MbufInquire


inline MIL_INT64 MbufInquireRequiredType(MIL_INT64 InquireType)
   {
   MIL_INT64 InquireTypeNoBand = InquireType &~ (M_RED|M_GREEN|M_BLUE|M_Y|M_U|M_V);
   switch (InquireTypeNoBand)
      {
      case M_HOST_ADDRESS           :
      case M_HOST_ADDRESS_REMOTE    :
      case M_PHYSICAL_ADDRESS       :
      case M_PHYSICAL_ADDRESS_REMOTE:
         return M_SAFE_TYPE_CHECK_PTR;
         break;
      }

   if (M_IN_BUF_INQUIRE_MIL_ID_RANGE(InquireType))
      return M_TYPE_MIL_ID;

   if (M_IN_BUF_INQUIRE_DOUBLE_RANGE(InquireType))
      return M_TYPE_DOUBLE;

   if (M_IN_BUF_INQUIRE_MIL_INT64_RANGE(InquireType))
      return M_TYPE_MIL_INT64;

   // In Windows headers, HANDLE is declared as a void*.
   if (InquireType == M_DC_HANDLE || InquireType == M_DIB_HANDLE)
      return M_SAFE_TYPE_CHECK_PTR;

   return M_TYPE_MIL_INT;
   }

inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType, int ResultPtr)
   {
   if (ResultPtr || M_BUF_INQUIRE_MUST_HAVE_USER_PTR(InquireType))  
      SafeTypeError(MIL_TEXT("MbufInquire"));

   return MbufInquire(BufId, InquireType, NULL);
   }

inline MIL_INT MFTYPE MbufInquireSafeTypeExecute (MIL_ID BufId, MIL_INT64 InquireType, void        *ValuePtr, MIL_INT64 GivenType)
   {
   MIL_INT64 RequiredType = MbufInquireRequiredType(InquireType);
   ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MbufInquire"));

   return MbufInquire(BufId, InquireType, ValuePtr);
   }

inline MIL_INT MFTYPE MbufInquireUnsafe  (MIL_ID BufId, MIL_INT64 InquireType,       void            *ResultPtr) {return MbufInquire               (BufId, InquireType, ResultPtr);}
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType,       MIL_INT32       *ResultPtr) {return MbufInquireSafeTypeExecute(BufId, InquireType, ResultPtr, M_TYPE_MIL_INT32);}
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType,       MIL_INT64       *ResultPtr) {return MbufInquireSafeTypeExecute(BufId, InquireType, ResultPtr, M_TYPE_MIL_INT64);}
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType,       MIL_DOUBLE      *ResultPtr) {return MbufInquireSafeTypeExecute(BufId, InquireType, ResultPtr, M_TYPE_DOUBLE   );}
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType,       MIL_TEXT_CHAR   *ResultPtr) {return MbufInquireSafeTypeExecute(BufId, InquireType, ResultPtr, M_TYPE_TEXT_CHAR);}
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType,       char           **ResultPtr) {return MbufInquireSafeTypeExecute(BufId, InquireType, ResultPtr, M_SAFE_TYPE_CHECK_PTR);}
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType, const char           **ResultPtr) {return MbufInquireSafeTypeExecute(BufId, InquireType, ResultPtr, M_SAFE_TYPE_CHECK_PTR);}
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType,       signed char    **ResultPtr) {return MbufInquireSafeTypeExecute(BufId, InquireType, ResultPtr, M_SAFE_TYPE_CHECK_PTR);}
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType, const signed char    **ResultPtr) {return MbufInquireSafeTypeExecute(BufId, InquireType, ResultPtr, M_SAFE_TYPE_CHECK_PTR);}
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType,       unsigned char  **ResultPtr) {return MbufInquireSafeTypeExecute(BufId, InquireType, ResultPtr, M_SAFE_TYPE_CHECK_PTR);}
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType, const unsigned char  **ResultPtr) {return MbufInquireSafeTypeExecute(BufId, InquireType, ResultPtr, M_SAFE_TYPE_CHECK_PTR);}
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType,       MIL_INT16      **ResultPtr) {return MbufInquireSafeTypeExecute(BufId, InquireType, ResultPtr, M_SAFE_TYPE_CHECK_PTR);}
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType, const MIL_INT16      **ResultPtr) {return MbufInquireSafeTypeExecute(BufId, InquireType, ResultPtr, M_SAFE_TYPE_CHECK_PTR);}
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType,       MIL_UINT16     **ResultPtr) {return MbufInquireSafeTypeExecute(BufId, InquireType, ResultPtr, M_SAFE_TYPE_CHECK_PTR);}
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType, const MIL_UINT16     **ResultPtr) {return MbufInquireSafeTypeExecute(BufId, InquireType, ResultPtr, M_SAFE_TYPE_CHECK_PTR);}
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType,       MIL_INT32      **ResultPtr) {return MbufInquireSafeTypeExecute(BufId, InquireType, ResultPtr, M_SAFE_TYPE_CHECK_PTR);}
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType, const MIL_INT32      **ResultPtr) {return MbufInquireSafeTypeExecute(BufId, InquireType, ResultPtr, M_SAFE_TYPE_CHECK_PTR);}
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType,       MIL_UINT32     **ResultPtr) {return MbufInquireSafeTypeExecute(BufId, InquireType, ResultPtr, M_SAFE_TYPE_CHECK_PTR);}
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType, const MIL_UINT32     **ResultPtr) {return MbufInquireSafeTypeExecute(BufId, InquireType, ResultPtr, M_SAFE_TYPE_CHECK_PTR);}
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType,       float          **ResultPtr) {return MbufInquireSafeTypeExecute(BufId, InquireType, ResultPtr, M_SAFE_TYPE_CHECK_PTR);}
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType, const float          **ResultPtr) {return MbufInquireSafeTypeExecute(BufId, InquireType, ResultPtr, M_SAFE_TYPE_CHECK_PTR);}
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType,       MIL_INT64      **ResultPtr) {return MbufInquireSafeTypeExecute(BufId, InquireType, ResultPtr, M_SAFE_TYPE_CHECK_PTR);}
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType, const MIL_INT64      **ResultPtr) {return MbufInquireSafeTypeExecute(BufId, InquireType, ResultPtr, M_SAFE_TYPE_CHECK_PTR);}
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType,       MIL_UINT64     **ResultPtr) {return MbufInquireSafeTypeExecute(BufId, InquireType, ResultPtr, M_SAFE_TYPE_CHECK_PTR);}
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType, const MIL_UINT64     **ResultPtr) {return MbufInquireSafeTypeExecute(BufId, InquireType, ResultPtr, M_SAFE_TYPE_CHECK_PTR);}
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType,       MIL_DOUBLE     **ResultPtr) {return MbufInquireSafeTypeExecute(BufId, InquireType, ResultPtr, M_SAFE_TYPE_CHECK_PTR);}
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType, const MIL_DOUBLE     **ResultPtr) {return MbufInquireSafeTypeExecute(BufId, InquireType, ResultPtr, M_SAFE_TYPE_CHECK_PTR);}
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType,       void           **ResultPtr) {return MbufInquireSafeTypeExecute(BufId, InquireType, ResultPtr, M_SAFE_TYPE_CHECK_PTR);}
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType, const void           **ResultPtr) {return MbufInquireSafeTypeExecute(BufId, InquireType, ResultPtr, M_SAFE_TYPE_CHECK_PTR);}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType,       MIL_UINT32      *ResultPtr) {return MbufInquireSafeTypeExecute(BufId, InquireType, ResultPtr, M_TYPE_MIL_INT32);}
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType,       MIL_UINT64      *ResultPtr) {return MbufInquireSafeTypeExecute(BufId, InquireType, ResultPtr, M_TYPE_MIL_INT64);}
#endif

#ifdef __MILBUFFERINFO_H__
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType,       MilBufferInfo **ResultPtr)
   {
   if (InquireType != M_BUFFER_INFO)
      SafeTypeError(MIL_TEXT("MbufInquire"));

   return MbufInquireUnsafe(BufId, InquireType, ResultPtr);
   }

inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType, const MilBufferInfo **ResultPtr)
   {
   if (InquireType != M_BUFFER_INFO)
      SafeTypeError(MIL_TEXT("MbufInquire"));

   return MbufInquireUnsafe(BufId, InquireType, ResultPtr);
   }
#endif

#if M_MIL_USE_WINDOWS
#if M_MIL_USE_DIRECTX_SERVICE
inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType, IDirectDrawSurface   **ResultPtr)
   {
   if (InquireType != M_DDRAW_SURFACE)
      SafeTypeError(MIL_TEXT("MbufInquire"));

   return MbufInquireUnsafe(BufId, InquireType, ResultPtr);
   }
#endif

inline MIL_INT MFTYPE MbufInquireSafeType(MIL_ID BufId, MIL_INT64 InquireType, BITMAPINFO           **ResultPtr)
   {
   if (InquireType != M_BITMAPINFO)
      SafeTypeError(MIL_TEXT("MbufInquire"));

   return MbufInquireUnsafe(BufId, InquireType, ResultPtr);
   }
#endif


// ----------------------------------------------------------
// MdigGetHookInfo

inline MIL_INT64 MdigGetHookInfoRequiredType(MIL_INT64 InfoType)
   {
   if((M_IN_DIG_GET_HOOK_INFO_DOUBLE_RANGE(InfoType)))
      {
      return M_TYPE_DOUBLE;
      }
   else if (((InfoType & ~M_MODIFIED_BUFFER) == M_BUFFER_ID) || ((InfoType & ~M_MODIFIED_BUFFER) == 0))
      {
      return M_TYPE_MIL_ID;
      }
   else
      {
      return M_TYPE_MIL_INT;   
      }
   }

inline MIL_INT MFTYPE MdigGetHookInfoSafeTypeExecute (MIL_ID EventId, MIL_INT64 InfoType, void        *ValuePtr, MIL_INT64 GivenType)
   {
   MIL_INT64 RequiredType = MdigGetHookInfoRequiredType(InfoType);
   ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MdigGetHookInfo"));

   return MdigGetHookInfo(EventId, InfoType, ValuePtr);
   }

inline MIL_INT MFTYPE MdigGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, int ResultPtr)
   {
   if (ResultPtr)
      SafeTypeError(MIL_TEXT("MdigGetHookInfo"));

   return MdigGetHookInfo(EventId, InfoType, NULL);
   }

inline MIL_INT MFTYPE MdigGetHookInfoUnsafe  (MIL_ID EventId, MIL_INT64 InfoType, void           *ResultPtr) {return MdigGetHookInfo               (EventId, InfoType, ResultPtr)                  ;}
inline MIL_INT MFTYPE MdigGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_INT        *ResultPtr) {return MdigGetHookInfoSafeTypeExecute(EventId, InfoType, ResultPtr, M_TYPE_MIL_INT  );}
inline MIL_INT MFTYPE MdigGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_DOUBLE     *ResultPtr) {return MdigGetHookInfoSafeTypeExecute(EventId, InfoType, ResultPtr, M_TYPE_DOUBLE   );}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MdigGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_UINT       *ResultPtr) {return MdigGetHookInfoSafeTypeExecute(EventId, InfoType, ResultPtr, M_TYPE_MIL_INT  );}
#endif

// ----------------------------------------------------------
// MdigInquireFeature

inline MIL_INT64 MdigInquireFeatureRequiredType(MIL_INT64 InquireType, MIL_INT64 FeatureType)
   {
   MIL_INT64 Type = 0;

   if((M_GET_INTERMOD_VALUE_BITS(InquireType) == M_STRING_SIZE) || (InquireType & M_FEATURE_SIZE))
      Type = M_TYPE_MIL_INT;
   else if(InquireType & M_LENGTH)
      {
      if(FeatureType == M_TYPE_REGISTER)
         Type = M_TYPE_INT64;
      else
         Type = M_TYPE_MIL_INT;
      }
   else if(M_FEATURE_OP(InquireType) == M_FEATURE_PRESENT)
      Type = M_TYPE_MIL_INT32;
   else if(M_FEATURE_OP(InquireType) == M_FEATURE_VALUE_AS_STRING)
      Type = M_TYPE_TEXT_CHAR;
   else if(M_FEATURE_OP(InquireType) == M_FEATURE_NAME)
      Type = M_TYPE_TEXT_CHAR;
   else if(M_FEATURE_OP(InquireType) == M_FEATURE_DISPLAY_NAME)
      Type = M_TYPE_TEXT_CHAR;
   else if(M_FEATURE_OP(InquireType) == M_FEATURE_TOOLTIP)
      Type = M_TYPE_TEXT_CHAR;
   else if(M_FEATURE_OP(InquireType) == M_FEATURE_DESCRIPTION)
      Type = M_TYPE_TEXT_CHAR;
   else if(M_FEATURE_OP(InquireType) == M_FEATURE_TYPE)
      Type = M_TYPE_INT64;
   else if(M_FEATURE_OP(InquireType) == M_ELEMENT_COUNT)
      Type = M_TYPE_MIL_INT;
   else if(M_FEATURE_OP(InquireType) == M_FEATURE_ACCESS_MODE)
      Type = M_TYPE_INT64;
   else if(M_FEATURE_OP(InquireType) == M_FEATURE_VISIBILITY)
      Type = M_TYPE_INT64;
   else if(M_FEATURE_OP(InquireType) == M_FEATURE_CACHING_MODE)
      Type = M_TYPE_INT64;
   else if(M_FEATURE_OP(InquireType) == M_FEATURE_STREAMABLE)
      Type = M_TYPE_INT64;
   else if(M_FEATURE_OP(InquireType) == M_FEATURE_DEPRECATED)
      Type = M_TYPE_INT64;
   else if(M_FEATURE_OP(InquireType) == M_FEATURE_REPRESENTATION)
      Type = M_TYPE_INT64;
   else if(M_FEATURE_OP(InquireType) == M_FEATURE_MIN)
      {
      if(FeatureType == M_TYPE_DOUBLE)
         Type = M_TYPE_DOUBLE;
      else if(FeatureType == M_TYPE_MIL_INT32)
         Type = M_TYPE_MIL_INT32;
      else
         Type = M_TYPE_INT64;
      }
   else if(M_FEATURE_OP(InquireType) == M_FEATURE_MAX)
      {
      if(FeatureType == M_TYPE_DOUBLE)
         Type = M_TYPE_DOUBLE;
      else if(FeatureType == M_TYPE_MIL_INT32)
         Type = M_TYPE_MIL_INT32;
      else
         Type = M_TYPE_INT64;
      }
   else if(M_FEATURE_OP(InquireType) == M_FEATURE_INCREMENT)
      {
      if(FeatureType == M_TYPE_DOUBLE)
         Type = M_TYPE_DOUBLE;
      else if(FeatureType == M_TYPE_MIL_INT32)
         Type = M_TYPE_MIL_INT32;
      else
         Type = M_TYPE_INT64;
      }
   else if(M_FEATURE_ENUM(InquireType) == M_GC_NODES)
      {
      if((InquireType & (~M_SUBFEATURE_INDEX_MASK)) == M_SUBFEATURE_COUNT)
         Type = M_TYPE_MIL_INT;
      else if((InquireType & (~M_SUBFEATURE_INDEX_MASK)) == M_SUBFEATURE_NAME)
         Type = M_TYPE_TEXT_CHAR;
      else if((InquireType & (~M_SUBFEATURE_INDEX_MASK)) == (M_SUBFEATURE_NAME+M_STRING_SIZE))
         Type = M_TYPE_MIL_INT;
      else if((InquireType & (~M_SUBFEATURE_INDEX_MASK)) == M_SUBFEATURE_TYPE)
         Type = M_TYPE_INT64;
      }
   else if(M_FEATURE_ENUM(InquireType) == M_FEATURE_ENUM_ENTRY)
      {
      if((InquireType & (~M_FEATURE_ENUM_ENTRY_INDEX_MASK)) == M_FEATURE_ENUM_ENTRY_COUNT)
         Type = M_TYPE_MIL_INT;
      else if((InquireType & (~M_FEATURE_ENUM_ENTRY_INDEX_MASK)) == M_FEATURE_ENUM_ENTRY_NAME)
         Type = M_TYPE_TEXT_CHAR;
      else if((InquireType & (~M_FEATURE_ENUM_ENTRY_INDEX_MASK)) == (M_FEATURE_ENUM_ENTRY_NAME+M_STRING_SIZE))
         Type = M_TYPE_MIL_INT;
      else if((InquireType & (~M_FEATURE_ENUM_ENTRY_INDEX_MASK)) == M_FEATURE_ENUM_ENTRY_VALUE)
         Type = M_TYPE_INT64;
      }
   
   if(Type == 0)
      {
      switch(FeatureType)
         {
         case M_TYPE_STRING:
         case M_TYPE_STRING_ENUMERATION:
         case M_TYPE_REGISTER_FROM_STRING:
         case M_TYPE_VALUE_FROM_STRING:
            Type = M_TYPE_TEXT_CHAR;
            break;
         case M_TYPE_ENUMERATION:
            Type = M_TYPE_INT64;
            break;
         case M_TYPE_INTEGER_ENUMERATION:
            Type = M_TYPE_INT64;
            break;
         case M_TYPE_BOOLEAN:
         case M_TYPE_MIL_INT32:
            Type = M_TYPE_MIL_INT32;
            break;
         case M_TYPE_INT64:
            Type = M_TYPE_INT64;
            break;
         case M_TYPE_DOUBLE:
            Type = M_TYPE_DOUBLE;
            break;
         default:
            if((FeatureType & M_TYPE_REGISTER) == M_TYPE_REGISTER)
               Type = M_TYPE_REGISTER;
            else
               Type = 0;
            break;
         }
      }

   return Type;
   }


#if M_MIL_USE_UNICODE
inline void MFTYPE MdigInquireFeatureExecuteW (MIL_ID DigId, MIL_INT64 InquireType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, void  *UserVarPtr, MIL_INT64 GivenType)
   {
   MIL_INT64 RequiredType = MdigInquireFeatureRequiredType(InquireType, FeatureType);

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MdigInquireFeature"));

   MdigInquireFeatureW(DigId, InquireType, FeatureName, FeatureType, UserVarPtr);
   }

inline void MFTYPE MdigInquireFeatureExecuteA (MIL_ID DigId, MIL_INT64 InquireType, const char* FeatureName, MIL_INT64 FeatureType, void  *UserVarPtr, MIL_INT64 GivenType)
   {
   MIL_INT64 RequiredType = MdigInquireFeatureRequiredType(InquireType, FeatureType);

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MdigInquireFeature"));

   MdigInquireFeatureA(DigId, InquireType, FeatureName, FeatureType, UserVarPtr);
   }

inline void MFTYPE MdigInquireFeatureUnsafeW  (MIL_ID DigId, MIL_INT64 InquireType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, void           *UserVarPtr)
   {
   MdigInquireFeatureW(DigId, InquireType, FeatureName, FeatureType, UserVarPtr);
   }
inline void MFTYPE MdigInquireFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 InquireType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, MIL_TEXT_CHAR  *UserVarPtr)
   {
   MdigInquireFeatureExecuteW(DigId, InquireType, FeatureName, FeatureType, UserVarPtr, M_TYPE_TEXT_CHAR);
   }
inline void MFTYPE MdigInquireFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 InquireType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, MIL_INT64      *UserVarPtr)
   {
   MdigInquireFeatureExecuteW(DigId, InquireType, FeatureName, FeatureType, UserVarPtr, M_TYPE_INT64);
   }
inline void MFTYPE MdigInquireFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 InquireType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, MIL_DOUBLE     *UserVarPtr)
   {
   MdigInquireFeatureExecuteW(DigId, InquireType, FeatureName, FeatureType, UserVarPtr, M_TYPE_DOUBLE);
   }
inline void MFTYPE MdigInquireFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 InquireType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, MIL_INT32      *UserVarPtr)
   {
   MdigInquireFeatureExecuteW(DigId, InquireType, FeatureName, FeatureType, UserVarPtr, M_TYPE_MIL_INT32);
   }
inline void MFTYPE MdigInquireFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 InquireType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, MIL_UINT8      *UserVarPtr)
   {
   MdigInquireFeatureExecuteW(DigId, InquireType, FeatureName, FeatureType, UserVarPtr, M_TYPE_REGISTER);
   }
inline void MFTYPE MdigInquireFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 InquireType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, bool           *UserVarPtr)
   {
   SafeTypeError(MIL_TEXT("MdigInquireFeature"));

   MdigInquireFeatureW(DigId, InquireType, FeatureName, FeatureType, UserVarPtr);
   }
inline void MFTYPE MdigInquireFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 InquireType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, int             UserVarPtr)
   {
   if(UserVarPtr != 0)
      SafeTypeError(MIL_TEXT("MdigInquireFeature"));

   MdigInquireFeatureW(DigId, InquireType, FeatureName, FeatureType, M_NULL);
   }
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline void MFTYPE MdigInquireFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 InquireType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, MIL_UINT64     *UserVarPtr)
   {
   MdigInquireFeatureExecuteW(DigId, InquireType, FeatureName, FeatureType, UserVarPtr, M_TYPE_INT64);
   }
#endif
inline void MFTYPE MdigInquireFeatureUnsafeA  (MIL_ID DigId, MIL_INT64 InquireType, const char* FeatureName, MIL_INT64 FeatureType, void           *UserVarPtr)
   {
   MdigInquireFeatureA(DigId, InquireType, FeatureName, FeatureType, UserVarPtr);
   }
inline void MFTYPE MdigInquireFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 InquireType, const char* FeatureName, MIL_INT64 FeatureType, MIL_TEXT_CHAR  *UserVarPtr)
   {
   MdigInquireFeatureExecuteA(DigId, InquireType, FeatureName, FeatureType, UserVarPtr, M_TYPE_TEXT_CHAR);
   }
inline void MFTYPE MdigInquireFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 InquireType, const char* FeatureName, MIL_INT64 FeatureType, MIL_INT64      *UserVarPtr)
   {
   MdigInquireFeatureExecuteA(DigId, InquireType, FeatureName, FeatureType, UserVarPtr, M_TYPE_INT64);
   }
inline void MFTYPE MdigInquireFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 InquireType, const char* FeatureName, MIL_INT64 FeatureType, MIL_DOUBLE     *UserVarPtr)
   {
   MdigInquireFeatureExecuteA(DigId, InquireType, FeatureName, FeatureType, UserVarPtr, M_TYPE_DOUBLE);
   }
inline void MFTYPE MdigInquireFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 InquireType, const char* FeatureName, MIL_INT64 FeatureType, MIL_INT32      *UserVarPtr)
   {
   MdigInquireFeatureExecuteA(DigId, InquireType, FeatureName, FeatureType, UserVarPtr, M_TYPE_MIL_INT32);
   }
inline void MFTYPE MdigInquireFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 InquireType, const char* FeatureName, MIL_INT64 FeatureType, MIL_UINT8      *UserVarPtr)
   {
   MdigInquireFeatureExecuteA(DigId, InquireType, FeatureName, FeatureType, UserVarPtr, M_TYPE_REGISTER);
   }
inline void MFTYPE MdigInquireFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 InquireType, const char* FeatureName, MIL_INT64 FeatureType, bool           *UserVarPtr)
   {
   SafeTypeError(MIL_TEXT("MdigInquireFeature"));

   MdigInquireFeatureA(DigId, InquireType, FeatureName, FeatureType, UserVarPtr);
   }
inline void MFTYPE MdigInquireFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 InquireType, const char* FeatureName, MIL_INT64 FeatureType, int             UserVarPtr)
   {
   if(UserVarPtr != 0)
      SafeTypeError(MIL_TEXT("MdigInquireFeature"));

   MdigInquireFeatureA(DigId, InquireType, FeatureName, FeatureType, M_NULL);
   }
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline void MFTYPE MdigInquireFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 InquireType, const char* FeatureName, MIL_INT64 FeatureType, MIL_UINT64     *UserVarPtr)
   {
   MdigInquireFeatureExecuteA(DigId, InquireType, FeatureName, FeatureType, UserVarPtr, M_TYPE_INT64);
   }
#endif

#if M_MIL_UNICODE_API
#define MdigInquireFeatureUnsafe MdigInquireFeatureUnsafeW
#else
#define MdigInquireFeatureUnsafe MdigInquireFeatureUnsafeA
#endif

#else

inline void MFTYPE MdigInquireFeatureExecute (MIL_ID DigId, MIL_INT64 InquireType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, void  *UserVarPtr, MIL_INT64 GivenType)
   {
   MIL_INT64 RequiredType = MdigInquireFeatureRequiredType(InquireType, FeatureType);

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MdigInquireFeature"));

   MdigInquireFeature(DigId, InquireType, FeatureName, FeatureType, UserVarPtr);
   }
inline void MFTYPE MdigInquireFeatureUnsafe  (MIL_ID DigId, MIL_INT64 InquireType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, void           *UserVarPtr)
   {
   MdigInquireFeature(DigId, InquireType, FeatureName, FeatureType, UserVarPtr);
   }
inline void MFTYPE MdigInquireFeatureSafeType(MIL_ID DigId, MIL_INT64 InquireType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, MIL_TEXT_CHAR  *UserVarPtr)
   {
   MdigInquireFeatureExecute(DigId, InquireType, FeatureName, FeatureType, UserVarPtr, M_TYPE_TEXT_CHAR);
   }
inline void MFTYPE MdigInquireFeatureSafeType(MIL_ID DigId, MIL_INT64 InquireType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, MIL_INT64      *UserVarPtr)
   {
   MdigInquireFeatureExecute(DigId, InquireType, FeatureName, FeatureType, UserVarPtr, M_TYPE_INT64);
   }
inline void MFTYPE MdigInquireFeatureSafeType(MIL_ID DigId, MIL_INT64 InquireType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, MIL_DOUBLE     *UserVarPtr)
   {
   MdigInquireFeatureExecute(DigId, InquireType, FeatureName, FeatureType, UserVarPtr, M_TYPE_DOUBLE);
   }
inline void MFTYPE MdigInquireFeatureSafeType(MIL_ID DigId, MIL_INT64 InquireType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, MIL_INT32      *UserVarPtr)
   {
   MdigInquireFeatureExecute(DigId, InquireType, FeatureName, FeatureType, UserVarPtr, M_TYPE_MIL_INT32);
   }
inline void MFTYPE MdigInquireFeatureSafeType(MIL_ID DigId, MIL_INT64 InquireType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, MIL_UINT8      *UserVarPtr)
   {
   MdigInquireFeatureExecute(DigId, InquireType, FeatureName, FeatureType, UserVarPtr, M_TYPE_REGISTER);
   }
inline void MFTYPE MdigInquireFeatureSafeType(MIL_ID DigId, MIL_INT64 InquireType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, bool           *UserVarPtr)
   {
   SafeTypeError(MIL_TEXT("MdigInquireFeature"));

   MdigInquireFeature(DigId, InquireType, FeatureName, FeatureType, UserVarPtr);
   }
inline void MFTYPE MdigInquireFeatureSafeType(MIL_ID DigId, MIL_INT64 InquireType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, int             UserVarPtr)
   {
   if(UserVarPtr != 0)
      SafeTypeError(MIL_TEXT("MdigInquireFeature"));

   MdigInquireFeature(DigId, InquireType, FeatureName, FeatureType, UserVarPtr);
   }
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline void MFTYPE MdigInquireFeatureSafeType(MIL_ID DigId, MIL_INT64 InquireType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, MIL_UINT64     *UserVarPtr)
   {
   MdigInquireFeatureExecute(DigId, InquireType, FeatureName, FeatureType, UserVarPtr, M_TYPE_INT64);
   }
#endif

#endif

// ----------------------------------------------------------
// MdigControlFeature

inline MIL_INT64 MdigControlFeatureRequiredType(MIL_INT64 ControlType, MIL_INT64 FeatureType)
   {
   MIL_INT64 Type = 0;

   if(M_FEATURE_OP(ControlType) == M_FEATURE_VALUE_AS_STRING)
      Type = M_TYPE_TEXT_CHAR;
   else
      {
      switch(FeatureType)
         {
         case M_TYPE_STRING:
         case M_TYPE_STRING_ENUMERATION:
         case M_TYPE_REGISTER_FROM_STRING:
         case M_TYPE_VALUE_FROM_STRING:
            Type = M_TYPE_TEXT_CHAR;
            break;
         case M_TYPE_ENUMERATION:
            Type = M_TYPE_INT64;
            break;
         case M_TYPE_INTEGER_ENUMERATION:
            Type = M_TYPE_INT64;
            break;
         case M_TYPE_INT64:
            Type = M_TYPE_INT64;
            break;
         case M_TYPE_DOUBLE:
            Type = M_TYPE_DOUBLE;
            break;
         case M_TYPE_MIL_INT32:
         case M_TYPE_BOOLEAN:
            Type = M_TYPE_MIL_INT32;
            break;
         case M_TYPE_COMMAND:
            Type = M_TYPE_COMMAND;
            break;
         default:
            if((FeatureType & M_TYPE_REGISTER) == M_TYPE_REGISTER)
               Type = M_TYPE_REGISTER;
            else
               Type = 0;
            break;
         }
      }

   return Type;
   }


#if M_MIL_USE_UNICODE
inline void MFTYPE MdigControlFeatureExecuteW (MIL_ID DigId, MIL_INT64 ControlType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, const void  *UserVarPtr, MIL_INT64 GivenType)
   {
   MIL_INT64 RequiredType = MdigControlFeatureRequiredType(ControlType, FeatureType);

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MdigControlFeature"));

   MdigControlFeatureW(DigId, ControlType, FeatureName, FeatureType, UserVarPtr);
   }

inline void MFTYPE MdigControlFeatureExecuteW (MIL_ID DigId, MIL_INT64 ControlType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, int UserVarPtr, MIL_INT64 GivenType)
   {
   MIL_INT64 RequiredType = MdigControlFeatureRequiredType(ControlType, FeatureType);

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MdigControlFeature"));
   else if (UserVarPtr)
      SafeTypeError(MIL_TEXT("MdigControlFeature"));

   MdigControlFeatureW(DigId, ControlType, FeatureName, FeatureType, (const void*)UserVarPtr);
   }

inline void MFTYPE MdigControlFeatureExecuteA (MIL_ID DigId, MIL_INT64 ControlType, const char* FeatureName, MIL_INT64 FeatureType, const void  *UserVarPtr, MIL_INT64 GivenType)
   {
   MIL_INT64 RequiredType = MdigControlFeatureRequiredType(ControlType, FeatureType);

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MdigControlFeature"));

   MdigControlFeatureA(DigId, ControlType, FeatureName, FeatureType, UserVarPtr);
   }

inline void MFTYPE MdigControlFeatureExecuteA (MIL_ID DigId, MIL_INT64 ControlType, const char* FeatureName, MIL_INT64 FeatureType, int UserVarPtr, MIL_INT64 GivenType)
   {
   MIL_INT64 RequiredType = MdigControlFeatureRequiredType(ControlType, FeatureType);

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MdigControlFeature"));
   else if(UserVarPtr)
      SafeTypeError(MIL_TEXT("MdigControlFeature"));

   MdigControlFeatureA(DigId, ControlType, FeatureName, FeatureType, (const void*)UserVarPtr);
   }

inline void MFTYPE MdigControlFeatureUnsafeW  (MIL_ID DigId, MIL_INT64 ControlType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, const void            *UserVarPtr)
   {
   MdigControlFeatureW(DigId, ControlType, FeatureName, FeatureType, UserVarPtr);
   }
inline void MFTYPE MdigControlFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 ControlType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, const MIL_TEXT_CHAR   *UserVarPtr)
   {
   MdigControlFeatureExecuteW(DigId, ControlType, FeatureName, FeatureType, UserVarPtr, M_TYPE_TEXT_CHAR);
   }
inline void MFTYPE MdigControlFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 ControlType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, const MIL_INT64       *UserVarPtr)
   {
   MdigControlFeatureExecuteW(DigId, ControlType, FeatureName, FeatureType, UserVarPtr, M_TYPE_INT64);
   }
inline void MFTYPE MdigControlFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 ControlType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, const MIL_DOUBLE      *UserVarPtr)
   {
   MdigControlFeatureExecuteW(DigId, ControlType, FeatureName, FeatureType, UserVarPtr, M_TYPE_DOUBLE);
   }
inline void MFTYPE MdigControlFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 ControlType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, const MIL_INT32       *UserVarPtr)
   {
   MdigControlFeatureExecuteW(DigId, ControlType, FeatureName, FeatureType, UserVarPtr, M_TYPE_MIL_INT32);
   }
inline void MFTYPE MdigControlFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 ControlType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, const MIL_UINT8       *UserVarPtr)
   {
   MdigControlFeatureExecuteW(DigId, ControlType, FeatureName, FeatureType, UserVarPtr, M_TYPE_REGISTER);
   }
inline void MFTYPE MdigControlFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 ControlType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, const bool            *UserVarPtr)
   {
   SafeTypeError(MIL_TEXT("MdigControlFeature"));

   MdigControlFeatureW(DigId, ControlType, FeatureName, FeatureType, UserVarPtr);
   }
inline void MFTYPE MdigControlFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 ControlType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType,       int              UserVarPtr)
   {
   MdigControlFeatureExecuteW(DigId, ControlType, FeatureName, FeatureType, UserVarPtr, FeatureType);
   }
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline void MFTYPE MdigControlFeatureSafeTypeW(MIL_ID DigId, MIL_INT64 ControlType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, const MIL_UINT64      *UserVarPtr)
   {
   MdigControlFeatureExecuteW(DigId, ControlType, FeatureName, FeatureType, UserVarPtr, M_TYPE_INT64);
   }
#endif

inline void MFTYPE MdigControlFeatureUnsafeA (MIL_ID DigId, MIL_INT64 ControlType, const char* FeatureName, MIL_INT64 FeatureType, const void                    *UserVarPtr)
   {
   MdigControlFeatureA(DigId, ControlType, FeatureName, FeatureType, UserVarPtr);
   }
inline void MFTYPE MdigControlFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 ControlType, const char* FeatureName, MIL_INT64 FeatureType, const MIL_TEXT_CHAR          *UserVarPtr)
   {
   MdigControlFeatureExecuteA(DigId, ControlType, FeatureName, FeatureType, UserVarPtr, M_TYPE_TEXT_CHAR);
   }
inline void MFTYPE MdigControlFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 ControlType, const char* FeatureName, MIL_INT64 FeatureType, const MIL_INT64              *UserVarPtr)
   {
   MdigControlFeatureExecuteA(DigId, ControlType, FeatureName, FeatureType, UserVarPtr, M_TYPE_INT64);
   }
inline void MFTYPE MdigControlFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 ControlType, const char* FeatureName, MIL_INT64 FeatureType, const MIL_DOUBLE             *UserVarPtr)
   {
   MdigControlFeatureExecuteA(DigId, ControlType, FeatureName, FeatureType, UserVarPtr, M_TYPE_DOUBLE);
   }
inline void MFTYPE MdigControlFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 ControlType, const char* FeatureName, MIL_INT64 FeatureType, const MIL_INT32              *UserVarPtr)
   {
   MdigControlFeatureExecuteA(DigId, ControlType, FeatureName, FeatureType, UserVarPtr, M_TYPE_MIL_INT32);
   }
inline void MFTYPE MdigControlFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 ControlType, const char* FeatureName, MIL_INT64 FeatureType, const MIL_UINT8              *UserVarPtr)
   {
   MdigControlFeatureExecuteA(DigId, ControlType, FeatureName, FeatureType, UserVarPtr, M_TYPE_REGISTER);
   }
inline void MFTYPE MdigControlFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 ControlType, const char* FeatureName, MIL_INT64 FeatureType, const bool                   *UserVarPtr)
   {
   SafeTypeError(MIL_TEXT("MdigControlFeature"));

   MdigControlFeatureA(DigId, ControlType, FeatureName, FeatureType, UserVarPtr);
   }
inline void MFTYPE MdigControlFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 ControlType, const char* FeatureName, MIL_INT64 FeatureType,       int                     UserVarPtr)
   {
   MdigControlFeatureExecuteA(DigId, ControlType, FeatureName, FeatureType, UserVarPtr, FeatureType);
   }
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline void MFTYPE MdigControlFeatureSafeTypeA(MIL_ID DigId, MIL_INT64 ControlType, const char* FeatureName, MIL_INT64 FeatureType, const MIL_UINT64             *UserVarPtr)
   {
   MdigControlFeatureExecuteA(DigId, ControlType, FeatureName, FeatureType, UserVarPtr, M_TYPE_INT64);
   }
#endif

#if M_MIL_UNICODE_API
#define MdigControlFeatureUnsafe MdigControlFeatureUnsafeW
#else
#define MdigControlFeatureUnsafe MdigControlFeatureUnsafeA
#endif

#else
inline void MFTYPE MdigControlFeatureExecute (MIL_ID DigId, MIL_INT64 ControlType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, const void  *UserVarPtr, MIL_INT64 GivenType)
   {
   MIL_INT64 RequiredType = MdigControlFeatureRequiredType(ControlType, FeatureType);

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MdigControlFeature"));

   MdigControlFeature(DigId, ControlType, FeatureName, FeatureType, UserVarPtr);
   }

inline void MFTYPE MdigControlFeatureExecute (MIL_ID DigId, MIL_INT64 ControlType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, int  UserVarPtr, MIL_INT64 GivenType)
   {
   MIL_INT64 RequiredType = MdigControlFeatureRequiredType(ControlType, FeatureType);

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MdigControlFeature"));
   else if(UserVarPtr)
      SafeTypeError(MIL_TEXT("MdigControlFeature"));

   MdigControlFeature(DigId, ControlType, FeatureName, FeatureType, (const void*)UserVarPtr);
   }

inline void MFTYPE MdigControlFeatureUnsafe  (MIL_ID DigId, MIL_INT64 ControlType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, const void             *UserVarPtr)
   {
   MdigControlFeature(DigId, ControlType, FeatureName, FeatureType, UserVarPtr);
   }
inline void MFTYPE MdigControlFeatureSafeType(MIL_ID DigId, MIL_INT64 ControlType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, const MIL_TEXT_CHAR    *UserVarPtr)
   {
   MdigControlFeatureExecute(DigId, ControlType, FeatureName, FeatureType, UserVarPtr, M_TYPE_TEXT_CHAR);
   }
inline void MFTYPE MdigControlFeatureSafeType(MIL_ID DigId, MIL_INT64 ControlType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, const MIL_INT64        *UserVarPtr)
   {
   MdigControlFeatureExecute(DigId, ControlType, FeatureName, FeatureType, UserVarPtr, M_TYPE_INT64);
   }
inline void MFTYPE MdigControlFeatureSafeType(MIL_ID DigId, MIL_INT64 ControlType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, const MIL_DOUBLE       *UserVarPtr)
   {
   MdigControlFeatureExecute(DigId, ControlType, FeatureName, FeatureType, UserVarPtr, M_TYPE_DOUBLE);
   }
inline void MFTYPE MdigControlFeatureSafeType(MIL_ID DigId, MIL_INT64 ControlType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, const MIL_INT32        *UserVarPtr)
   {
   MdigControlFeatureExecute(DigId, ControlType, FeatureName, FeatureType, UserVarPtr, M_TYPE_MIL_INT32);
   }
inline void MFTYPE MdigControlFeatureSafeType(MIL_ID DigId, MIL_INT64 ControlType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, const MIL_UINT8        *UserVarPtr)
   {
   MdigControlFeatureExecute(DigId, ControlType, FeatureName, FeatureType, UserVarPtr, M_TYPE_REGISTER);
   }
inline void MFTYPE MdigControlFeatureSafeType(MIL_ID DigId, MIL_INT64 ControlType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, const bool             *UserVarPtr)
   {
   SafeTypeError(MIL_TEXT("MdigControlFeature"));

   MdigControlFeature(DigId, ControlType, FeatureName, FeatureType, UserVarPtr);
   }
inline void MFTYPE MdigControlFeatureSafeType(MIL_ID DigId, MIL_INT64 ControlType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType,       int               UserVarPtr)
   {
   MdigControlFeatureExecute(DigId, ControlType, FeatureName, FeatureType, UserVarPtr, FeatureType);
   }
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline void MFTYPE MdigControlFeatureSafeType(MIL_ID DigId, MIL_INT64 ControlType, MIL_CONST_TEXT_PTR FeatureName, MIL_INT64 FeatureType, const MIL_UINT64       *UserVarPtr)
   {
   MdigControlFeatureExecute(DigId, ControlType, FeatureName, FeatureType, UserVarPtr, M_TYPE_INT64);
   }
#endif
#endif

// ----------------------------------------------------------
// MdigInquire

inline MIL_INT64 MdigInquireRequiredType(MIL_INT64 InquireType)
   {
   if ( M_IN_DIG_INQUIRE_STRING_RANGE(InquireType) )
      return M_TYPE_TEXT_CHAR;

   if ( M_IN_DIG_INQUIRE_DOUBLE_RANGE(InquireType) )
      return M_TYPE_DOUBLE;

   if ( M_IN_DIG_INQUIRE_MIL_INT64_RANGE(InquireType) )
      return M_TYPE_MIL_INT64;

   return M_TYPE_MIL_INT;
   }

inline MIL_INT MFTYPE MdigInquireSafeType(MIL_ID DigId, MIL_INT64 InquireType, int UserVarPtr)
   {
   if (UserVarPtr)
      SafeTypeError(MIL_TEXT("MdigInquire"));

   return MdigInquire(DigId, InquireType, NULL);
   }

inline MIL_INT MFTYPE MdigInquireExecute (MIL_ID DigId, MIL_INT64 InquireType, void        *ValuePtr, MIL_INT64 GivenType)
   {
   MIL_INT64 RequiredType = MdigInquireRequiredType(InquireType);
   ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MdigInquire"));

   return MdigInquire(DigId, InquireType, ValuePtr);
   }

inline MIL_INT MFTYPE MdigInquireUnsafe  (MIL_ID DigId, MIL_INT64 InquireType, void           *UserVarPtr) {return MdigInquire       (DigId, InquireType, UserVarPtr                  );}
inline MIL_INT MFTYPE MdigInquireSafeType(MIL_ID DigId, MIL_INT64 InquireType, MIL_TEXT_CHAR  *UserVarPtr) {return MdigInquireExecute(DigId, InquireType, UserVarPtr, M_TYPE_TEXT_CHAR);}
inline MIL_INT MFTYPE MdigInquireSafeType(MIL_ID DigId, MIL_INT64 InquireType, MIL_INT32      *UserVarPtr) {return MdigInquireExecute(DigId, InquireType, UserVarPtr, M_TYPE_MIL_INT32);}
inline MIL_INT MFTYPE MdigInquireSafeType(MIL_ID DigId, MIL_INT64 InquireType, MIL_INT64      *UserVarPtr) {return MdigInquireExecute(DigId, InquireType, UserVarPtr, M_TYPE_MIL_INT64);}
inline MIL_INT MFTYPE MdigInquireSafeType(MIL_ID DigId, MIL_INT64 InquireType, MIL_DOUBLE     *UserVarPtr) {return MdigInquireExecute(DigId, InquireType, UserVarPtr, M_TYPE_DOUBLE   );}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MdigInquireSafeType(MIL_ID DigId, MIL_INT64 InquireType, MIL_UINT32     *UserVarPtr) {return MdigInquireExecute(DigId, InquireType, UserVarPtr, M_TYPE_MIL_INT32);}
inline MIL_INT MFTYPE MdigInquireSafeType(MIL_ID DigId, MIL_INT64 InquireType, MIL_UINT64     *UserVarPtr) {return MdigInquireExecute(DigId, InquireType, UserVarPtr, M_TYPE_MIL_INT64);}
#endif

#ifdef __MILFPGA_H__
// ----------------------------------------------------------
// MfpgaGetHookInfo

inline MIL_INT64 MfpgaGetHookInfoRequiredType(MIL_INT64 InfoType)
   {
   switch (InfoType)
      {
      case M_TIME_STAMP:
         return M_TYPE_DOUBLE;
         break;
      }

   return M_TYPE_MIL_INT;
   }

inline MIL_INT MFTYPE MfpgaGetHookInfoSafeType(MIL_ID  EventId, MIL_INT64 InfoType, int UserVarPtr)
   {
   if (UserVarPtr != NULL)
      SafeTypeError(MIL_TEXT("MfpgaGetHookInfo"));

   return MfpgaGetHookInfo(EventId, InfoType, NULL);
   }

inline MIL_INT MFTYPE MfpgaGetHookInfoSafeTypeExecute (MIL_ID  EventId, MIL_INT64 InfoType, void        *UserVarPtr, MIL_INT64 GivenType)
   {
   MIL_INT64 RequiredType = MfpgaGetHookInfoRequiredType(InfoType);
   ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MfpgaGetHookInfo"));

   return MfpgaGetHookInfo(EventId, InfoType, UserVarPtr);
   }

inline MIL_INT MFTYPE MfpgaGetHookInfoUnsafe  (MIL_ID  EventId, MIL_INT64 InfoType, void         *UserVarPtr) {return MfpgaGetHookInfo               (EventId, InfoType, UserVarPtr                  );}
inline MIL_INT MFTYPE MfpgaGetHookInfoSafeType(MIL_ID  EventId, MIL_INT64 InfoType, MIL_INT32    *UserVarPtr) {return MfpgaGetHookInfoSafeTypeExecute(EventId, InfoType, UserVarPtr, M_TYPE_MIL_INT32);}
inline MIL_INT MFTYPE MfpgaGetHookInfoSafeType(MIL_ID  EventId, MIL_INT64 InfoType, MIL_INT64    *UserVarPtr) {return MfpgaGetHookInfoSafeTypeExecute(EventId, InfoType, UserVarPtr, M_TYPE_MIL_INT64);}
inline MIL_INT MFTYPE MfpgaGetHookInfoSafeType(MIL_ID  EventId, MIL_INT64 InfoType, MIL_DOUBLE   *UserVarPtr) {return MfpgaGetHookInfoSafeTypeExecute(EventId, InfoType, UserVarPtr, M_TYPE_DOUBLE   );}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MfpgaGetHookInfoSafeType(MIL_ID  EventId, MIL_INT64 InfoType, MIL_UINT32   *UserVarPtr) {return MfpgaGetHookInfoSafeTypeExecute(EventId, InfoType, UserVarPtr, M_TYPE_MIL_INT32);}
inline MIL_INT MFTYPE MfpgaGetHookInfoSafeType(MIL_ID  EventId, MIL_INT64 InfoType, MIL_UINT64   *UserVarPtr) {return MfpgaGetHookInfoSafeTypeExecute(EventId, InfoType, UserVarPtr, M_TYPE_MIL_INT64);}
#endif

// ----------------------------------------------------------
// MfpgaInquire

inline MIL_INT64 MfpgaInquireRequiredType(MIL_INT64 InquireType)
   {
   if(M_IN_FPGA_INQUIRE_STRING_RANGE(InquireType))
      {
      return M_TYPE_TEXT_CHAR;
      }

   return M_TYPE_MIL_INT;
   }

inline MIL_INT MFTYPE MfpgaInquireSafeType(MIL_ID  MilSystemId, MIL_INT FpgaDeviceNum, MIL_INT64 InquireType, int UserVarPtr)
   {
   if (UserVarPtr != NULL)
      SafeTypeError(MIL_TEXT("MfpgaInquire"));

   return MfpgaInquire(MilSystemId, FpgaDeviceNum, InquireType, NULL);
   }

inline MIL_INT MFTYPE MfpgaInquireSafeTypeExecute (MIL_ID  MilSystemId, MIL_INT FpgaDeviceNum, MIL_INT64 InquireType, void        *UserVarPtr, MIL_INT64 GivenType)
   {
   MIL_INT64 RequiredType = MfpgaInquireRequiredType(InquireType);
   ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MfpgaInquire"));

   return MfpgaInquire(MilSystemId, FpgaDeviceNum, InquireType, UserVarPtr);
   }

inline MIL_INT MFTYPE MfpgaInquireUnsafe  (MIL_ID  MilSystemId, MIL_INT FpgaDeviceNum, MIL_INT64 InquireType, void            *UserVarPtr) {return MfpgaInquire               (MilSystemId, FpgaDeviceNum, InquireType, UserVarPtr                  );}
inline MIL_INT MFTYPE MfpgaInquireSafeType(MIL_ID  MilSystemId, MIL_INT FpgaDeviceNum, MIL_INT64 InquireType, MIL_INT32       *UserVarPtr) {return MfpgaInquireSafeTypeExecute(MilSystemId, FpgaDeviceNum, InquireType, UserVarPtr, M_TYPE_MIL_INT32);}
inline MIL_INT MFTYPE MfpgaInquireSafeType(MIL_ID  MilSystemId, MIL_INT FpgaDeviceNum, MIL_INT64 InquireType, MIL_INT64       *UserVarPtr) {return MfpgaInquireSafeTypeExecute(MilSystemId, FpgaDeviceNum, InquireType, UserVarPtr, M_TYPE_MIL_INT64);}
inline MIL_INT MFTYPE MfpgaInquireSafeType(MIL_ID  MilSystemId, MIL_INT FpgaDeviceNum, MIL_INT64 InquireType, MIL_TEXT_CHAR   *UserVarPtr) {return MfpgaInquireSafeTypeExecute(MilSystemId, FpgaDeviceNum, InquireType, UserVarPtr, M_TYPE_TEXT_CHAR);}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MfpgaInquireSafeType(MIL_ID  MilSystemId, MIL_INT FpgaDeviceNum, MIL_INT64 InquireType, MIL_UINT32      *UserVarPtr) {return MfpgaInquireSafeTypeExecute(MilSystemId, FpgaDeviceNum, InquireType, UserVarPtr, M_TYPE_MIL_INT32);}
inline MIL_INT MFTYPE MfpgaInquireSafeType(MIL_ID  MilSystemId, MIL_INT FpgaDeviceNum, MIL_INT64 InquireType, MIL_UINT64      *UserVarPtr) {return MfpgaInquireSafeTypeExecute(MilSystemId, FpgaDeviceNum, InquireType, UserVarPtr, M_TYPE_MIL_INT64);}
#endif

#endif // #ifdef __MILFPGA_H__

// ----------------------------------------------------------
// MfuncInquire

inline MIL_INT64 MfuncInquireRequiredType(MIL_INT64 InquireType)
   {
   if (InquireType == M_OBJECT_TYPE_EXTENDED)
      return M_TYPE_MIL_INT64;

   return M_TYPE_MIL_INT;
   }

inline MIL_INT MFTYPE MfuncInquireSafeTypeExecute (MIL_ID ObjectId, MIL_INT64 InquireType, void        *UserVarPtr, MIL_INT64 GivenType)
   {
   MIL_INT64 RequiredType = MfuncInquireRequiredType(InquireType);
   ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MfuncInquire"));

   return MfuncInquire(ObjectId, InquireType, UserVarPtr);
   }

inline MIL_INT MFTYPE MfuncInquireSafeType(MIL_ID ObjectId, MIL_INT64 InquireType, int UserVarPtr)
   {
   if (UserVarPtr)
      SafeTypeError(MIL_TEXT("MfuncInquire"));

   return MfuncInquire(ObjectId, InquireType, NULL);
   }

inline MIL_INT MFTYPE MfuncInquireUnsafe  (MIL_ID ObjectId, MIL_INT64 InquireType, void        *UserVarPtr) {return MfuncInquire               (ObjectId, InquireType, UserVarPtr                  );}
inline MIL_INT MFTYPE MfuncInquireSafeType(MIL_ID ObjectId, MIL_INT64 InquireType, MIL_INT32   *UserVarPtr) {return MfuncInquireSafeTypeExecute(ObjectId, InquireType, UserVarPtr, M_TYPE_MIL_INT32);}
inline MIL_INT MFTYPE MfuncInquireSafeType(MIL_ID ObjectId, MIL_INT64 InquireType, MIL_INT64   *UserVarPtr) {return MfuncInquireSafeTypeExecute(ObjectId, InquireType, UserVarPtr, M_TYPE_MIL_INT64);}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MfuncInquireSafeType(MIL_ID ObjectId, MIL_INT64 InquireType, MIL_UINT32  *UserVarPtr) {return MfuncInquireSafeTypeExecute(ObjectId, InquireType, UserVarPtr, M_TYPE_MIL_INT32);}
inline MIL_INT MFTYPE MfuncInquireSafeType(MIL_ID ObjectId, MIL_INT64 InquireType, MIL_UINT64  *UserVarPtr) {return MfuncInquireSafeTypeExecute(ObjectId, InquireType, UserVarPtr, M_TYPE_MIL_INT64);}
#endif

// ----------------------------------------------------------
// MgraInquire

inline MIL_INT MFTYPE MgraInquireSafeType(MIL_ID GraContextId, MIL_INT64 InquireType, int UserVarPtr)
   {
   if (UserVarPtr)
      SafeTypeError(MIL_TEXT("MgraInquire"));

   return MgraInquire(GraContextId, InquireType, NULL);
   }

inline MIL_INT MFTYPE MgraInquireSafeTypeExecute (MIL_ID ContextGraId, MIL_INT64 InquireType, void        *UserVarPtr, MIL_INT64 GivenType)
   {
   MIL_INT64 RequiredType = MgraInquireRequiredType(InquireType);
   ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MgraInquire"));

   return MgraInquire(ContextGraId, InquireType, UserVarPtr);
   }

inline MIL_INT MFTYPE MgraInquireUnsafe  (MIL_ID ContextGraId, MIL_INT64 InquireType, void        *UserVarPtr) {return MgraInquire               (ContextGraId, InquireType, UserVarPtr                  );}
inline MIL_INT MFTYPE MgraInquireSafeType(MIL_ID ContextGraId, MIL_INT64 InquireType, MIL_INT8    *UserVarPtr) {return MgraInquireSafeTypeExecute(ContextGraId, InquireType, UserVarPtr, M_TYPE_CHAR     );}
inline MIL_INT MFTYPE MgraInquireSafeType(MIL_ID ContextGraId, MIL_INT64 InquireType, MIL_INT16   *UserVarPtr) {return MgraInquireSafeTypeExecute(ContextGraId, InquireType, UserVarPtr, M_TYPE_SHORT    );}
inline MIL_INT MFTYPE MgraInquireSafeType(MIL_ID ContextGraId, MIL_INT64 InquireType, MIL_INT32   *UserVarPtr) {return MgraInquireSafeTypeExecute(ContextGraId, InquireType, UserVarPtr, M_TYPE_MIL_INT32);}
inline MIL_INT MFTYPE MgraInquireSafeType(MIL_ID ContextGraId, MIL_INT64 InquireType, MIL_INT64   *UserVarPtr) {return MgraInquireSafeTypeExecute(ContextGraId, InquireType, UserVarPtr, M_TYPE_MIL_INT64);}
inline MIL_INT MFTYPE MgraInquireSafeType(MIL_ID ContextGraId, MIL_INT64 InquireType, float       *UserVarPtr) {return MgraInquireSafeTypeExecute(ContextGraId, InquireType, UserVarPtr, M_TYPE_FLOAT    );}
inline MIL_INT MFTYPE MgraInquireSafeType(MIL_ID ContextGraId, MIL_INT64 InquireType, MIL_DOUBLE  *UserVarPtr) {return MgraInquireSafeTypeExecute(ContextGraId, InquireType, UserVarPtr, M_TYPE_DOUBLE   );}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MgraInquireSafeType(MIL_ID ContextGraId, MIL_INT64 InquireType, MIL_UINT8   *UserVarPtr) {return MgraInquireSafeTypeExecute(ContextGraId, InquireType, UserVarPtr, M_TYPE_CHAR     );}
inline MIL_INT MFTYPE MgraInquireSafeType(MIL_ID ContextGraId, MIL_INT64 InquireType, MIL_UINT16  *UserVarPtr) {return MgraInquireSafeTypeExecute(ContextGraId, InquireType, UserVarPtr, M_TYPE_SHORT    );}
inline MIL_INT MFTYPE MgraInquireSafeType(MIL_ID ContextGraId, MIL_INT64 InquireType, MIL_UINT32  *UserVarPtr) {return MgraInquireSafeTypeExecute(ContextGraId, InquireType, UserVarPtr, M_TYPE_MIL_INT32);}
inline MIL_INT MFTYPE MgraInquireSafeType(MIL_ID ContextGraId, MIL_INT64 InquireType, MIL_UINT64  *UserVarPtr) {return MgraInquireSafeTypeExecute(ContextGraId, InquireType, UserVarPtr, M_TYPE_MIL_INT64);}
#endif

// ----------------------------------------------------------
// MgraInquireList

inline MIL_INT64 MgraInquireListRequiredType(MIL_INT64 InquireType)
   {
   return MgraInquireRequiredType(InquireType);
   }

inline MIL_INT MFTYPE MgraInquireListSafeType(MIL_ID GraListId, MIL_INT Index, MIL_INT SubIndex, MIL_INT64 InquireType, int                 UserVarPtr)
   {
   if (UserVarPtr)
      SafeTypeError(MIL_TEXT("MgraInquireList"));

   return MgraInquireList(GraListId, Index, SubIndex, InquireType, NULL);
   }

inline MIL_INT MFTYPE MgraInquireListSafeTypeExecute (MIL_ID GraListId, MIL_INT Index, MIL_INT SubIndex, MIL_INT64 InquireType, void        *UserVarPtr, MIL_INT64 GivenType)
   {
   MIL_INT64 RequiredType = MgraInquireListRequiredType(InquireType);
   ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MgraInquireList"));

   return MgraInquireList(GraListId, Index, SubIndex, InquireType, UserVarPtr);
   }

inline MIL_INT MFTYPE MgraInquireListUnsafe  (MIL_ID GraListId, MIL_INT Index, MIL_INT SubIndex, MIL_INT64 InquireType, void         *UserVarPtr) {return MgraInquireList               (GraListId, Index, SubIndex, InquireType, UserVarPtr);}
inline MIL_INT MFTYPE MgraInquireListSafeType(MIL_ID GraListId, MIL_INT Index, MIL_INT SubIndex, MIL_INT64 InquireType, MIL_INT8     *UserVarPtr) {return MgraInquireListSafeTypeExecute(GraListId, Index, SubIndex, InquireType, UserVarPtr, M_TYPE_CHAR     );}
inline MIL_INT MFTYPE MgraInquireListSafeType(MIL_ID GraListId, MIL_INT Index, MIL_INT SubIndex, MIL_INT64 InquireType, MIL_INT16    *UserVarPtr) {return MgraInquireListSafeTypeExecute(GraListId, Index, SubIndex, InquireType, UserVarPtr, M_TYPE_SHORT    );}
inline MIL_INT MFTYPE MgraInquireListSafeType(MIL_ID GraListId, MIL_INT Index, MIL_INT SubIndex, MIL_INT64 InquireType, MIL_INT32    *UserVarPtr) {return MgraInquireListSafeTypeExecute(GraListId, Index, SubIndex, InquireType, UserVarPtr, M_TYPE_MIL_INT32);}
inline MIL_INT MFTYPE MgraInquireListSafeType(MIL_ID GraListId, MIL_INT Index, MIL_INT SubIndex, MIL_INT64 InquireType, MIL_INT64    *UserVarPtr) {return MgraInquireListSafeTypeExecute(GraListId, Index, SubIndex, InquireType, UserVarPtr, M_TYPE_MIL_INT64);}
inline MIL_INT MFTYPE MgraInquireListSafeType(MIL_ID GraListId, MIL_INT Index, MIL_INT SubIndex, MIL_INT64 InquireType, float        *UserVarPtr) {return MgraInquireListSafeTypeExecute(GraListId, Index, SubIndex, InquireType, UserVarPtr, M_TYPE_FLOAT    );}
inline MIL_INT MFTYPE MgraInquireListSafeType(MIL_ID GraListId, MIL_INT Index, MIL_INT SubIndex, MIL_INT64 InquireType, MIL_DOUBLE   *UserVarPtr) {return MgraInquireListSafeTypeExecute(GraListId, Index, SubIndex, InquireType, UserVarPtr, M_TYPE_DOUBLE   );}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MgraInquireListSafeType(MIL_ID GraListId, MIL_INT Index, MIL_INT SubIndex, MIL_INT64 InquireType, MIL_UINT8    *UserVarPtr) {return MgraInquireListSafeTypeExecute(GraListId, Index, SubIndex, InquireType, UserVarPtr, M_TYPE_CHAR     );}
inline MIL_INT MFTYPE MgraInquireListSafeType(MIL_ID GraListId, MIL_INT Index, MIL_INT SubIndex, MIL_INT64 InquireType, MIL_UINT16   *UserVarPtr) {return MgraInquireListSafeTypeExecute(GraListId, Index, SubIndex, InquireType, UserVarPtr, M_TYPE_SHORT    );}
inline MIL_INT MFTYPE MgraInquireListSafeType(MIL_ID GraListId, MIL_INT Index, MIL_INT SubIndex, MIL_INT64 InquireType, MIL_UINT32   *UserVarPtr) {return MgraInquireListSafeTypeExecute(GraListId, Index, SubIndex, InquireType, UserVarPtr, M_TYPE_MIL_INT32);}
inline MIL_INT MFTYPE MgraInquireListSafeType(MIL_ID GraListId, MIL_INT Index, MIL_INT SubIndex, MIL_INT64 InquireType, MIL_UINT64   *UserVarPtr) {return MgraInquireListSafeTypeExecute(GraListId, Index, SubIndex, InquireType, UserVarPtr, M_TYPE_MIL_INT64);}
#endif

// ----------------------------------------------------------
// MsysGetHookInfo

inline MIL_INT64 MsysGetHookInfoRequiredType(MIL_INT64 InquireType)
   {
   MIL_INT64 Type = M_TYPE_MIL_INT;
   switch(InquireType)
      {
      case M_TIME_STAMP:
         Type = M_TYPE_MIL_DOUBLE;
         break;
      case M_GC_IP_ADDRESS:
      case M_GC_MAC_ADDRESS:
         Type = M_TYPE_MIL_INT64;
         break;
      case M_GC_USER_NAME:
      case M_GC_IP_ADDRESS_STRING:
      case M_GC_MAC_ADDRESS_STRING:
         Type = M_TYPE_STRING_PTR;
         break;
      default:
         Type = M_TYPE_MIL_INT;
         break;
      }
   return Type;
   }

inline MIL_INT MFTYPE MsysGetHookInfoSafeType(MIL_ID SystemId, MIL_ID EventId, MIL_INT64 InquireType, int ResultPtr)
   {
   if (ResultPtr)
      SafeTypeError(MIL_TEXT("MsysGetHookInfo"));

   return MsysGetHookInfo(SystemId,EventId, InquireType, NULL);
   }

inline MIL_INT MFTYPE MsysGetHookInfoSafeTypeExecute (MIL_ID SystemId, MIL_ID EventId, MIL_INT64 InquireType, void        *ValuePtr, MIL_INT64 GivenType)
   {
   MIL_INT64 RequiredType = MsysGetHookInfoRequiredType(InquireType);
   ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MsysGetHookInfo"));

   return MsysGetHookInfo(SystemId, EventId, InquireType, ValuePtr);
   }

inline MIL_INT MFTYPE MsysGetHookInfoUnsafe  (MIL_ID SystemId, MIL_ID EventId, MIL_INT64 InquireType, void            *ResultPtr) {return MsysGetHookInfo               (SystemId, EventId, InquireType, ResultPtr);}
inline MIL_INT MFTYPE MsysGetHookInfoSafeType(MIL_ID SystemId, MIL_ID EventId, MIL_INT64 InquireType, MIL_INT32       *ResultPtr) {return MsysGetHookInfoSafeTypeExecute(SystemId, EventId, InquireType, ResultPtr, M_TYPE_MIL_INT32);}
inline MIL_INT MFTYPE MsysGetHookInfoSafeType(MIL_ID SystemId, MIL_ID EventId, MIL_INT64 InquireType, MIL_INT64       *ResultPtr) {return MsysGetHookInfoSafeTypeExecute(SystemId, EventId, InquireType, ResultPtr, M_TYPE_MIL_INT64);}
inline MIL_INT MFTYPE MsysGetHookInfoSafeType(MIL_ID SystemId, MIL_ID EventId, MIL_INT64 InquireType, MIL_DOUBLE      *ResultPtr) {return MsysGetHookInfoSafeTypeExecute(SystemId, EventId, InquireType, ResultPtr, M_TYPE_MIL_DOUBLE);}
inline MIL_INT MFTYPE MsysGetHookInfoSafeType(MIL_ID SystemId, MIL_ID EventId, MIL_INT64 InquireType, MIL_TEXT_PTR     ResultPtr) {return MsysGetHookInfoSafeTypeExecute(SystemId, EventId, InquireType, ResultPtr, M_TYPE_STRING_PTR);}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MsysGetHookInfoSafeType(MIL_ID SystemId, MIL_ID EventId, MIL_INT64 InquireType, MIL_UINT32      *ResultPtr) {return MsysGetHookInfoSafeTypeExecute(SystemId, EventId, InquireType, ResultPtr, M_TYPE_MIL_INT32);}
inline MIL_INT MFTYPE MsysGetHookInfoSafeType(MIL_ID SystemId, MIL_ID EventId, MIL_INT64 InquireType, MIL_UINT64      *ResultPtr) {return MsysGetHookInfoSafeTypeExecute(SystemId, EventId, InquireType, ResultPtr, M_TYPE_MIL_INT64);}
#endif

// ----------------------------------------------------------
// MsysInquire


inline MIL_INT64 MsysInquireRequiredType(MIL_INT64 InquireType)
   {
   if (M_IN_SYS_INQUIRE_MIL_ID_RANGE(InquireType))
      return M_TYPE_MIL_ID;

   if (M_IN_SYS_INQUIRE_STRING_RANGE(InquireType))
      return M_TYPE_TEXT_CHAR;

   if (M_IN_SYS_INQUIRE_MIL_INT64_RANGE(InquireType))
      return M_TYPE_MIL_INT64;

   if (M_IN_SYS_INQUIRE_DOUBLE_RANGE(InquireType))
      return M_TYPE_MIL_DOUBLE;

   return M_TYPE_MIL_INT;
   }

inline MIL_INT MFTYPE MsysInquireSafeType(MIL_ID SystemId, MIL_INT64 InquireType, int ResultPtr)
   {
   if (ResultPtr || M_SYS_INQUIRE_MUST_HAVE_USER_PTR(InquireType))
      SafeTypeError(MIL_TEXT("MsysInquire"));

   return MsysInquire(SystemId, InquireType, NULL);
   }

inline MIL_INT MFTYPE MsysInquireSafeTypeExecute (MIL_ID SystemId, MIL_INT64 InquireType, void        *ValuePtr, MIL_INT64 GivenType)
   {
   MIL_INT64 RequiredType = MsysInquireRequiredType(InquireType);
   ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MsysInquire"));

   return MsysInquire(SystemId, InquireType, ValuePtr);
   }

inline MIL_INT MFTYPE MsysInquireUnsafe  (MIL_ID SystemId, MIL_INT64 InquireType, void            *ResultPtr) {return MsysInquire               (SystemId, InquireType, ResultPtr);}
inline MIL_INT MFTYPE MsysInquireSafeType(MIL_ID SystemId, MIL_INT64 InquireType, MIL_INT32       *ResultPtr) {return MsysInquireSafeTypeExecute(SystemId, InquireType, ResultPtr, M_TYPE_MIL_INT32);}
inline MIL_INT MFTYPE MsysInquireSafeType(MIL_ID SystemId, MIL_INT64 InquireType, MIL_INT64       *ResultPtr) {return MsysInquireSafeTypeExecute(SystemId, InquireType, ResultPtr, M_TYPE_MIL_INT64);}
inline MIL_INT MFTYPE MsysInquireSafeType(MIL_ID SystemId, MIL_INT64 InquireType, MIL_TEXT_CHAR   *ResultPtr) {return MsysInquireSafeTypeExecute(SystemId, InquireType, ResultPtr, M_TYPE_TEXT_CHAR);}
inline MIL_INT MFTYPE MsysInquireSafeType(MIL_ID SystemId, MIL_INT64 InquireType, MIL_DOUBLE      *ResultPtr) {return MsysInquireSafeTypeExecute(SystemId, InquireType, ResultPtr, M_TYPE_MIL_DOUBLE);}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MsysInquireSafeType(MIL_ID SystemId, MIL_INT64 InquireType, MIL_UINT32      *ResultPtr) {return MsysInquireSafeTypeExecute(SystemId, InquireType, ResultPtr, M_TYPE_MIL_INT32);}
inline MIL_INT MFTYPE MsysInquireSafeType(MIL_ID SystemId, MIL_INT64 InquireType, MIL_UINT64      *ResultPtr) {return MsysInquireSafeTypeExecute(SystemId, InquireType, ResultPtr, M_TYPE_MIL_INT64);}
#endif

// ----------------------------------------------------------
// MsysIoInquire


inline MIL_INT64 MsysIoInquireRequiredType(MIL_INT64 InquireType)
   {
   if (M_IN_SYS_IO_INQUIRE_MIL_ID_RANGE(InquireType))
      return M_TYPE_MIL_ID;

   if (M_IN_SYS_IO_INQUIRE_STRING_RANGE(InquireType))
      return M_TYPE_TEXT_CHAR;

   if (M_IN_SYS_IO_INQUIRE_MIL_INT64_RANGE(InquireType))
      return M_TYPE_MIL_INT64;

   if (M_IN_SYS_IO_INQUIRE_DOUBLE_RANGE(InquireType))
      return M_TYPE_MIL_DOUBLE;

   return M_TYPE_MIL_INT;
   }

inline MIL_INT MFTYPE MsysIoInquireSafeType(MIL_ID IoObjectId, MIL_INT64 InquireType, int UserVarPtr)
   {
   if (UserVarPtr || M_SYS_IO_INQUIRE_MUST_HAVE_USER_PTR(InquireType))
      SafeTypeError(MIL_TEXT("MsysIoInquire"));

   return MsysIoInquire(IoObjectId, InquireType, NULL);
   }

inline MIL_INT MFTYPE MsysIoInquireSafeTypeExecute (MIL_ID IoObjectId, MIL_INT64 InquireType, void* UserVarPtr, MIL_INT64 GivenType)
   {
   MIL_INT64 RequiredType = MsysIoInquireRequiredType(InquireType);
   ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MsysIoInquire"));

   return MsysIoInquire(IoObjectId, InquireType, UserVarPtr);
   }

inline MIL_INT MFTYPE MsysIoInquireUnsafe  (MIL_ID IoObjectId, MIL_INT64 InquireType, void            *UserVarPtr) {return MsysIoInquire               (IoObjectId, InquireType, UserVarPtr);}
inline MIL_INT MFTYPE MsysIoInquireSafeType(MIL_ID IoObjectId, MIL_INT64 InquireType, MIL_INT32       *UserVarPtr) {return MsysIoInquireSafeTypeExecute(IoObjectId, InquireType, UserVarPtr, M_TYPE_MIL_INT32);}
inline MIL_INT MFTYPE MsysIoInquireSafeType(MIL_ID IoObjectId, MIL_INT64 InquireType, MIL_INT64       *UserVarPtr) {return MsysIoInquireSafeTypeExecute(IoObjectId, InquireType, UserVarPtr, M_TYPE_MIL_INT64);}
inline MIL_INT MFTYPE MsysIoInquireSafeType(MIL_ID IoObjectId, MIL_INT64 InquireType, MIL_TEXT_CHAR   *UserVarPtr) {return MsysIoInquireSafeTypeExecute(IoObjectId, InquireType, UserVarPtr, M_TYPE_TEXT_CHAR);}
inline MIL_INT MFTYPE MsysIoInquireSafeType(MIL_ID IoObjectId, MIL_INT64 InquireType, MIL_DOUBLE      *UserVarPtr) {return MsysIoInquireSafeTypeExecute(IoObjectId, InquireType, UserVarPtr, M_TYPE_MIL_DOUBLE);}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MsysIoInquireSafeType(MIL_ID IoObjectId, MIL_INT64 InquireType, MIL_UINT32      *UserVarPtr) {return MsysIoInquireSafeTypeExecute(IoObjectId, InquireType, UserVarPtr, M_TYPE_MIL_INT32);}
inline MIL_INT MFTYPE MsysIoInquireSafeType(MIL_ID IoObjectId, MIL_INT64 InquireType, MIL_UINT64      *UserVarPtr) {return MsysIoInquireSafeTypeExecute(IoObjectId, InquireType, UserVarPtr, M_TYPE_MIL_INT64);}
#endif

// ----------------------------------------------------------
// MsysIoCommandRegister

inline MIL_INT64 MsysIoCommandRegisterRequiredType(MIL_INT64 Operation)
   {
   UNREFERENCED_PARAMETER(Operation);
   return M_TYPE_MIL_INT;
   }

inline MIL_INT MFTYPE MsysIoCommandRegisterSafeType(MIL_ID IoObjectId, MIL_INT64 Operation, MIL_INT64 ReferenceStamp, MIL_DOUBLE DelayFromReferenceStamp, MIL_DOUBLE Duration, MIL_INT64 BitToOperate, int CommandStatusPtr)
   {
   if (CommandStatusPtr != 0)
      SafeTypeError(MIL_TEXT("MsysIoCommandRegister"));

   return MsysIoCommandRegister(IoObjectId, Operation, ReferenceStamp, DelayFromReferenceStamp, Duration, BitToOperate, NULL);
   }

inline MIL_INT MFTYPE MsysIoCommandRegisterExecute(MIL_ID IoObjectId, MIL_INT64 Operation, MIL_INT64 ReferenceStamp, MIL_DOUBLE DelayFromReferenceStamp, MIL_DOUBLE Duration, MIL_INT64 BitToOperate, void* CommandStatusPtr, MIL_INT64 GivenType)
   {
   MIL_INT64 RequiredType = MsysIoCommandRegisterRequiredType(Operation);
   ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MsysIoCommandRegister"));

   return MsysIoCommandRegister(IoObjectId, Operation, ReferenceStamp, DelayFromReferenceStamp, Duration, BitToOperate, CommandStatusPtr);
   }

inline MIL_INT MFTYPE MsysIoCommandRegisterUnsafe  (MIL_ID IoObjectId, MIL_INT64 Operation, MIL_INT64 ReferenceStamp, MIL_DOUBLE DelayFromReferenceStamp, MIL_DOUBLE Duration, MIL_INT64 BitToOperate, void*    CommandStatusPtr)  {return MsysIoCommandRegister(IoObjectId, Operation, ReferenceStamp, DelayFromReferenceStamp, Duration, BitToOperate, CommandStatusPtr);}
inline MIL_INT MFTYPE MsysIoCommandRegisterSafeType(MIL_ID IoObjectId, MIL_INT64 Operation, MIL_INT64 ReferenceStamp, MIL_DOUBLE DelayFromReferenceStamp, MIL_DOUBLE Duration, MIL_INT64 BitToOperate, MIL_INT* CommandStatusPtr)  {return MsysIoCommandRegisterExecute(IoObjectId, Operation, ReferenceStamp, DelayFromReferenceStamp, Duration, BitToOperate, CommandStatusPtr, M_TYPE_MIL_INT);}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MsysIoCommandRegisterSafeType(MIL_ID IoObjectId, MIL_INT64 Operation, MIL_INT64 ReferenceStamp, MIL_DOUBLE DelayFromReferenceStamp, MIL_DOUBLE Duration, MIL_INT64 BitToOperate, MIL_UINT* CommandStatusPtr) {return MsysIoCommandRegisterExecute(IoObjectId, Operation, ReferenceStamp, DelayFromReferenceStamp, Duration, BitToOperate, CommandStatusPtr, M_TYPE_MIL_INT);}
#endif

//-------------------------------------------------------------------------------
// MthrInquire

inline MIL_INT64 MthrInquireRequiredType(MIL_INT64 InquireType)
   {
   if(InquireType == M_NATIVE_ID)
      return M_TYPE_MIL_ID;
   return M_TYPE_MIL_INT;
   }

inline MIL_INT MFTYPE MthrInquireSafeType(MIL_ID ThreadEventOrMutexId, MIL_INT64 InquireType, int UserVarPtr)
   {
   if (UserVarPtr != 0)
      SafeTypeError(MIL_TEXT("MthrInquire"));

   return MthrInquire(ThreadEventOrMutexId, InquireType, NULL);
   }

inline MIL_INT MFTYPE MthrInquireExecute (MIL_ID ThreadEventOrMutexId, MIL_INT64 InquireType, void        *ValuePtr, MIL_INT64 GivenType)
   {
   MIL_INT64 RequiredType = MthrInquireRequiredType(InquireType);
   ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MthrInquire"));

   return MthrInquire(ThreadEventOrMutexId, InquireType, ValuePtr);
   }

inline MIL_INT MFTYPE MthrInquireUnsafe  (MIL_ID ThreadEventOrMutexId, MIL_INT64 InquireType, void         *UserVarPtr) {return MthrInquire     (  ThreadEventOrMutexId, InquireType, UserVarPtr                  );}
inline MIL_INT MFTYPE MthrInquireSafeType(MIL_ID ThreadEventOrMutexId, MIL_INT64 InquireType, MIL_INT      *UserVarPtr) {return MthrInquireExecute(ThreadEventOrMutexId, InquireType, UserVarPtr, M_TYPE_MIL_INT  );}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MthrInquireSafeType(MIL_ID ThreadEventOrMutexId, MIL_INT64 InquireType, MIL_UINT     *UserVarPtr) {return MthrInquireExecute(ThreadEventOrMutexId, InquireType, UserVarPtr, M_TYPE_MIL_INT  );}
#endif

// ----------------------------------------------------------
// MseqGetHookInfo

inline MIL_INT64 MseqGetHookInfoRequiredType(MIL_INT64 InfoType)
   {
   switch (InfoType)
      {
      case M_MODIFIED_BUFFER+M_BUFFER_ID:
         return M_TYPE_MIL_ID;
         break;
      }

   return M_TYPE_MIL_INT;
   }

inline MIL_INT MFTYPE MseqGetHookInfoSafeTypeExecute (MIL_ID EventId, MIL_INT64 InfoType, void* UserVarPtr, MIL_INT64 GivenType)
   {
   MIL_INT64 RequiredType = MseqGetHookInfoRequiredType(InfoType);
   ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MseqGetHookInfo"));

   return MseqGetHookInfo(EventId, InfoType, UserVarPtr);
   }

inline MIL_INT MFTYPE MseqGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, int UserVarPtr)
   {
   if (UserVarPtr)
      SafeTypeError(MIL_TEXT("MseqGetHookInfo"));

   return MseqGetHookInfo(EventId, InfoType, NULL);
   }

inline MIL_INT MFTYPE MseqGetHookInfoUnsafe  (MIL_ID EventId, MIL_INT64 InfoType, void            *UserVarPtr) {return MseqGetHookInfo               (EventId, InfoType, UserVarPtr)                  ;}
inline MIL_INT MFTYPE MseqGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_INT         *UserVarPtr) {return MseqGetHookInfoSafeTypeExecute(EventId, InfoType, UserVarPtr, M_TYPE_MIL_INT  );}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MseqGetHookInfoSafeType(MIL_ID EventId, MIL_INT64 InfoType, MIL_UINT        *UserVarPtr) {return MseqGetHookInfoSafeTypeExecute(EventId, InfoType, UserVarPtr, M_TYPE_MIL_INT  );}
#endif


// ----------------------------------------------------------
// MseqInquire


inline MIL_INT64 MseqInquireRequiredType(MIL_INT64 InquireType)
   {
   if (M_IN_SEQ_INQUIRE_STRING_RANGE(InquireType))
      return M_TYPE_TEXT_CHAR;

   if (M_IN_SEQ_INQUIRE_MIL_INT64_RANGE(InquireType))
      return M_TYPE_MIL_INT64;

   if (M_IN_SEQ_INQUIRE_MIL_DOUBLE_RANGE(InquireType))
      return M_TYPE_MIL_DOUBLE;

   return M_TYPE_MIL_INT;
   }

inline MIL_INT MFTYPE MseqInquireSafeType(MIL_ID ContextSeqId, MIL_INT SequenceIndex, MIL_INT64 InquireType, int UserVarPtr)
   {
   if (UserVarPtr || M_IN_SEQ_INQUIRE_MUST_HAVE_PTR(InquireType))
      SafeTypeError(MIL_TEXT("MseqInquire"));

   return MseqInquire(ContextSeqId, SequenceIndex, InquireType, NULL);
   }

inline MIL_INT MFTYPE MseqInquireSafeTypeExecute(MIL_ID ContextSeqId, MIL_INT SequenceIndex, MIL_INT64 InquireType, void* UserVarPtr, MIL_INT64 GivenType)
   {
   MIL_INT64 RequiredType = MseqInquireRequiredType(InquireType);
   ReplaceTypeMilIdByTypeMilIntXX(&RequiredType);

   if (RequiredType != GivenType)
      SafeTypeError(MIL_TEXT("MseqInquire"));

   return MseqInquire(ContextSeqId, SequenceIndex, InquireType, UserVarPtr);
   }

inline MIL_INT MFTYPE MseqInquireUnsafe  (MIL_ID ContextSeqId, MIL_INT SequenceIndex, MIL_INT64 InquireType, void            *UserVarPtr) {return MseqInquire               (ContextSeqId, SequenceIndex, InquireType, UserVarPtr);}
inline MIL_INT MFTYPE MseqInquireSafeType(MIL_ID ContextSeqId, MIL_INT SequenceIndex, MIL_INT64 InquireType, MIL_INT32       *UserVarPtr) {return MseqInquireSafeTypeExecute(ContextSeqId, SequenceIndex, InquireType, UserVarPtr, M_TYPE_MIL_INT32);}
inline MIL_INT MFTYPE MseqInquireSafeType(MIL_ID ContextSeqId, MIL_INT SequenceIndex, MIL_INT64 InquireType, MIL_INT64       *UserVarPtr) {return MseqInquireSafeTypeExecute(ContextSeqId, SequenceIndex, InquireType, UserVarPtr, M_TYPE_MIL_INT64);}
inline MIL_INT MFTYPE MseqInquireSafeType(MIL_ID ContextSeqId, MIL_INT SequenceIndex, MIL_INT64 InquireType, MIL_TEXT_CHAR   *UserVarPtr) {return MseqInquireSafeTypeExecute(ContextSeqId, SequenceIndex, InquireType, UserVarPtr, M_TYPE_TEXT_CHAR);}
inline MIL_INT MFTYPE MseqInquireSafeType(MIL_ID ContextSeqId, MIL_INT SequenceIndex, MIL_INT64 InquireType, MIL_DOUBLE      *UserVarPtr) {return MseqInquireSafeTypeExecute(ContextSeqId, SequenceIndex, InquireType, UserVarPtr, M_TYPE_MIL_DOUBLE);}
#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
inline MIL_INT MFTYPE MseqInquireSafeType(MIL_ID ContextSeqId, MIL_INT SequenceIndex, MIL_INT64 InquireType, MIL_UINT32      *UserVarPtr) {return MseqInquireSafeTypeExecute(ContextSeqId, SequenceIndex, InquireType, UserVarPtr, M_TYPE_MIL_INT32);}
inline MIL_INT MFTYPE MseqInquireSafeType(MIL_ID ContextSeqId, MIL_INT SequenceIndex, MIL_INT64 InquireType, MIL_UINT64      *UserVarPtr) {return MseqInquireSafeTypeExecute(ContextSeqId, SequenceIndex, InquireType, UserVarPtr, M_TYPE_MIL_INT64);}
#endif


#define MappGetError         MappGetErrorSafeType
#define MappGetHookInfo      MappGetHookInfoSafeType
#define MappInquire          MappInquireSafeType

#if M_MIL_USE_UNICODE
#define MbufDiskInquireW     MbufDiskInquireSafeTypeW
#define MbufDiskInquireA     MbufDiskInquireSafeTypeA
#else
#define MbufDiskInquire      MbufDiskInquireSafeType
#endif
#define MbufGetHookInfo      MbufGetHookInfoSafeType
#define MbufInquire          MbufInquireSafeType

#define MdigGetHookInfo      MdigGetHookInfoSafeType
#define MdigInquire          MdigInquireSafeType

#if M_MIL_USE_UNICODE
#define MdigInquireFeatureW   MdigInquireFeatureSafeTypeW
#define MdigControlFeatureW   MdigControlFeatureSafeTypeW
#define MdigInquireFeatureA   MdigInquireFeatureSafeTypeA
#define MdigControlFeatureA   MdigControlFeatureSafeTypeA
#else
#define MdigInquireFeature   MdigInquireFeatureSafeType
#define MdigControlFeature   MdigControlFeatureSafeType
#endif

//#define MfpgaGetHookInfo     MfpgaGetHookInfoSafeType
//#define MfpgaInquire         MfpgaInquireSafeType

// MfuncInquireSafeType is not activated because too many undocumented InquireTypes with various pointer types.
//#define MfuncInquire         MfuncInquireSafeType

#define MgraInquire          MgraInquireSafeType
#define MgraInquireList      MgraInquireListSafeType

#define MsysGetHookInfo      MsysGetHookInfoSafeType
#define MsysInquire          MsysInquireSafeType
#define MsysIoInquire        MsysIoInquireSafeType
#define MsysIoCommandRegister MsysIoCommandRegisterSafeType

#define MthrInquire          MthrInquireSafeType

#define MseqGetHookInfo      MseqGetHookInfoSafeType
#define MseqInquire          MseqInquireSafeType

#else // #if M_MIL_USE_SAFE_TYPE

// Definitions of Unsafe functions are available even if M_MIL_USE_SAFE_TYPE
// is deactivated in order to allow applications that have been transformed
// to use it to compile properly.

#define MappGetErrorUnsafe         MappGetError
#define MappGetHookInfoUnsafe      MappGetHookInfo
#define MappInquireUnsafe          MappInquire
#define MappFileOperationUnsafe    MappFileOperation

#define MbufCreate2dUnsafe         MbufCreate2d
#define MbufCreateColorUnsafe      MbufCreateColor
#if M_MIL_USE_UNICODE
#define MbufDiskInquireUnsafeW     MbufDiskInquireW
#define MbufDiskInquireUnsafeA     MbufDiskInquireA
#if M_MIL_UNICODE_API
#define MbufDiskInquireUnsafe      MbufDiskInquireW
#else
#define MbufDiskInquireUnsafe      MbufDiskInquireA
#endif
#else
#define MbufDiskInquireUnsafe      MbufDiskInquire
#endif
#if M_MIL_USE_UNICODE
#define MdigInquireFeatureUnsafeW   MdigInquireFeatureW
#define MdigControlFeatureUnsafeW   MdigControlFeatureW
#define MdigInquireFeatureUnsafeA   MdigInquireFeatureA
#define MdigControlFeatureUnsafeA   MdigControlFeatureA
#if M_MIL_UNICODE_API
#define MdigInquireFeatureUnsafe   MdigInquireFeatureW
#define MdigControlFeatureUnsafe   MdigControlFeatureW
#else
#define MdigInquireFeatureUnsafe   MdigInquireFeatureA
#define MdigControlFeatureUnsafe   MdigControlFeatureA
#endif
#else
#define MdigInquireFeatureUnsafe   MdigInquireFeature
#define MdigControlFeatureUnsafe   MdigControlFeature
#endif
#define MbufGet1dUnsafe            MbufGet1d
#define MbufGet2dUnsafe            MbufGet2d
#define MbufGetUnsafe              MbufGet
#define MbufGetArcUnsafe           MbufGetArc
#define MbufGetColor2dUnsafe       MbufGetColor2d
#define MbufGetColorUnsafe         MbufGetColor
#define MbufGetHookInfoUnsafe      MbufGetHookInfo
#define MbufGetLineUnsafe          MbufGetLine
#define MbufInquireUnsafe          MbufInquire
#define MbufPut1dUnsafe            MbufPut1d
#define MbufPut2dUnsafe            MbufPut2d
#define MbufPutUnsafe              MbufPut
#define MbufPutColor2dUnsafe       MbufPutColor2d
#define MbufPutColorUnsafe         MbufPutColor
#define MbufPutLineUnsafe          MbufPutLine

#define MdigGetHookInfoUnsafe      MdigGetHookInfo
#define MdigInquireUnsafe          MdigInquire

#define MfpgaGetHookInfoUnsafe     MfpgaGetHookInfo
#define MfpgaInquireUnsafe         MfpgaInquire

#define MfuncInquireUnsafe         MfuncInquire

#define MgraInquireUnsafe          MgraInquire
#define MgraInquireListUnsafe      MgraInquireList

#define MsysGetHookInfoUnsafe      MsysGetHookInfo
#define MsysInquireUnsafe          MsysInquire
#define MsysIoInquireUnsafe        MsysIoInquire
#define MsysIoCommandRegisterUnsafe MsysIoCommandRegister

#define MthrInquireUnsafe          MthrInquire

#define MseqGetHookInfoUnsafe      MseqGetHookInfo
#define MseqInquireUnsafe          MseqInquire

#endif // #if M_MIL_USE_SAFE_TYPE #else

// For backward compatibility to the deprecated function name
#define MbufControlRegion MbufControlArea

#endif
