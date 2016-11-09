/***************************************************************************/
/*

    Filename:  MILFUNC.H
    Revision:  10.00.2564
    Content :  This file contains the prototypes for the Matrox
               Imaging Library (MIL) Mfunc user's functions.

    Copyright © Matrox Electronic Systems Ltd., 1992-2014.
    All Rights Reserved

*/
/***************************************************************************/

#ifndef __MILFUNC_H
#define __MILFUNC_H

/* C++ directive if needed */
#ifdef __cplusplus
extern "C"
{
#endif

/***************************************************************************/
/* TOOLKIT FOR PSEUDO MIL FUNCTIONS DEVELOPMENT                            */
/***************************************************************************/
typedef void (MFTYPE *MIL_FUNC_FUNCTION_PTR)(MIL_ID FunctionId);
typedef MIL_FUNC_FUNCTION_PTR MFUNCSLAVEFCTPTR;
typedef MIL_FUNC_FUNCTION_PTR MFUNCFCTPTR;
typedef MIL_FUNC_FUNCTION_PTR MFUNCSLAVEPAFCTPTR;

// we need the ASCII version even on non-Unicode version
// this is needed for transferring ASCII strings in DMIL 
MIL_DLLFUNC void MFTYPE MfuncParamMilTextA(MIL_ID FuncId, MIL_INT ParamIndex, char* ParamValuePtr, MIL_INT NbOfItems, MIL_INT Attribute);
MIL_DLLFUNC void MFTYPE MfuncParamConstMilTextA(MIL_ID FuncId, MIL_INT ParamIndex,const char* ParamValuePtr, MIL_INT NbOfItems, MIL_INT Attribute);
MIL_DLLFUNC void MFTYPE MfuncParamFilenameA(MIL_ID FuncId, MIL_INT ParamIndex, const char* ParamValuePtr, MIL_INT NbOfItems, MIL_INT Attribute);

#if M_MIL_USE_UNICODE
   MIL_ID MFTYPE MfuncAllocW(MIL_CONST_TEXT_PTR FunctionName,
                             MIL_INT ParameterNumber,
                             MIL_FUNC_FUNCTION_PTR SlaveFunctionPtr,
                             MIL_CONST_TEXT_PTR SlaveFunctionDLLName,
                             MIL_CONST_TEXT_PTR SlaveFunctionName,
                             MIL_INT SlaveFunctionOpcode,
                             MIL_INT64 InitFlag,
                             MIL_ID *FuncIdPtr);
   MIL_ID MFTYPE MfuncAllocA(const char* FunctionName,
                             MIL_INT ParameterNumber,
                             MIL_FUNC_FUNCTION_PTR SlaveFunctionPtr,
                             const char* SlaveFunctionDLLName,
                             const char* SlaveFunctionName,
                             MIL_INT SlaveFunctionOpcode,
                             MIL_INT64 InitFlag,
                             MIL_ID *FuncIdPtr);

   MIL_INT MFTYPE MfuncErrorReportW(MIL_ID FuncId,
                                    MIL_INT ErrorCode,
                                    MIL_CONST_TEXT_PTR ErrorMessage,
                                    MIL_CONST_TEXT_PTR ErrorSubMessage1,
                                    MIL_CONST_TEXT_PTR ErrorSubMessage2,
                                    MIL_CONST_TEXT_PTR ErrorSubMessage3);
   MIL_INT MFTYPE MfuncErrorReportA(MIL_ID FuncId,
                                    MIL_INT ErrorCode,
                                    const char* ErrorMessage,
                                    const char* ErrorSubMessage1,
                                    const char* ErrorSubMessage2,
                                    const char* ErrorSubMessage3);

   void MFTYPE MfuncParamMilTextW(MIL_ID FuncId, MIL_INT ParamIndex, MIL_TEXT_PTR ParamValuePtr, MIL_INT NbOfItems, MIL_INT Attribute);
   void MFTYPE MfuncParamMilTextA(MIL_ID FuncId, MIL_INT ParamIndex, char* ParamValuePtr, MIL_INT NbOfItems, MIL_INT Attribute);

   void MFTYPE MfuncParamConstMilTextW(MIL_ID FuncId, MIL_INT ParamIndex,MIL_CONST_TEXT_PTR ParamValuePtr, MIL_INT NbOfItems, MIL_INT Attribute);
   void MFTYPE MfuncParamConstMilTextA(MIL_ID FuncId, MIL_INT ParamIndex,const char* ParamValuePtr, MIL_INT NbOfItems, MIL_INT Attribute);

   void MFTYPE MfuncParamFilenameW(MIL_ID FuncId, MIL_INT ParamIndex, MIL_CONST_TEXT_PTR ParamValuePtr, MIL_INT NbOfItems, MIL_INT Attribute);
   void MFTYPE MfuncParamFilenameA(MIL_ID FuncId, MIL_INT ParamIndex, const char* ParamValuePtr, MIL_INT NbOfItems, MIL_INT Attribute);

   MIL_INT MFTYPE MfuncPrintMessageW(MIL_INT Mode,
                                     MIL_CONST_TEXT_PTR Str1,
                                     MIL_CONST_TEXT_PTR Str2,
                                     MIL_CONST_TEXT_PTR Str3,
                                     MIL_CONST_TEXT_PTR Str4,
                                     MIL_CONST_TEXT_PTR Str5);
   MIL_INT MFTYPE MfuncPrintMessageA(MIL_INT Mode,
                                     const char* Str1,
                                     const char* Str2,
                                     const char* Str3,
                                     const char* Str4,
                                     const char* Str5);
   #if M_MIL_UNICODE_API
      #define MfuncAlloc         MfuncAllocW
      #define MfuncErrorReport   MfuncErrorReportW
      #define MfuncParamMilText   MfuncParamMilTextW
      #define MfuncParamConstMilText   MfuncParamConstMilTextW
      #define MfuncParamFilename MfuncParamFilenameW
      #define MfuncPrintMessage  MfuncPrintMessageW
   #else
      #define MfuncAlloc         MfuncAllocA
      #define MfuncErrorReport   MfuncErrorReportA
      #define MfuncParamMilText   MfuncParamMilTextA
      #define MfuncParamConstMilText   MfuncParamConstMilTextA
      #define MfuncParamFilename MfuncParamFilenameA
      #define MfuncPrintMessage  MfuncPrintMessageA
   #endif
   #else
      MIL_ID MFTYPE MfuncAlloc(MIL_CONST_TEXT_PTR FunctionName,
                              MIL_INT ParameterNumber,
                              MIL_FUNC_FUNCTION_PTR SlaveFunctionPtr,
                              MIL_CONST_TEXT_PTR SlaveFunctionDLLName,
                              MIL_CONST_TEXT_PTR SlaveFunctionName,
                              MIL_INT SlaveFunctionOpcode,
                              MIL_INT64 InitFlag,
                              MIL_ID *FuncIdPtr);

      MIL_INT MFTYPE MfuncErrorReport( MIL_ID FuncId,
                                       MIL_INT ErrorCode,
                                       MIL_CONST_TEXT_PTR ErrorMessage,
                                       MIL_CONST_TEXT_PTR ErrorSubMessage1,
                                       MIL_CONST_TEXT_PTR ErrorSubMessage2,
                                       MIL_CONST_TEXT_PTR ErrorSubMessage3);

      void MFTYPE MfuncParamMilText(MIL_ID FuncId, MIL_INT ParamIndex, MIL_TEXT_PTR ParamValuePtr, MIL_INT NbOfItems, MIL_INT Attribute);

      void MFTYPE MfuncParamConstMilText(MIL_ID FuncId, MIL_INT ParamIndex,MIL_CONST_TEXT_PTR ParamValuePtr, MIL_INT NbOfItems, MIL_INT Attribute);

      void MFTYPE MfuncParamFilename(MIL_ID FuncId, MIL_INT ParamIndex, MIL_CONST_TEXT_PTR ParamValuePtr, MIL_INT NbOfItems, MIL_INT Attribute);

      MIL_INT MFTYPE MfuncPrintMessage(MIL_INT Mode, MIL_CONST_TEXT_PTR Str1,
                                       MIL_CONST_TEXT_PTR Str2,
                                       MIL_CONST_TEXT_PTR Str3,
                                       MIL_CONST_TEXT_PTR Str4,
                                       MIL_CONST_TEXT_PTR Str5);
#endif
// compatibility function
#if OldDefinesSupport
#define MfuncParamString   MfuncParamMilText
MIL_DEPRECATED(MfuncParamString, 1000)
#endif


MIL_ID MFTYPE MfuncAllocId(MIL_ID FunctionId, MIL_INT64 ObjectType, void *ObjectPtr);
MIL_INT MFTYPE MfuncParamCheck(MIL_ID FuncId);
void MFTYPE MfuncParam(MIL_ID FuncId, MIL_INT ParamIndex, const void* ParamValuePtr, MIL_UINT ParamType, MIL_INT NbOfItems, MIL_INT64 ObjectType, MIL_INT Attribute);
void MFTYPE MfuncParamMilId(MIL_ID FuncId, MIL_INT ParamIndex, MIL_ID ParamValue, MIL_INT64 ObjectType, MIL_INT Attribute);
#if OldDefinesSupport
#define MfuncParamId MfuncParamMilId // it was documented in MIL 9, so keep it
MIL_DEPRECATED(MfuncParamId, 1000)
#endif
void MFTYPE MfuncParamLong (MIL_ID FuncId, MIL_INT ParamIndex, long ParamValue);
void MFTYPE MfuncParamMilInt(MIL_ID FuncId, MIL_INT ParamIndex, MIL_INT ParamValue);
void MFTYPE MfuncParamMilInt32(MIL_ID FuncId, MIL_INT ParamIndex, MIL_INT32 ParamValue);
void MFTYPE MfuncParamMilInt64(MIL_ID FuncId, MIL_INT ParamIndex, MIL_INT64 ParamValue);
void MFTYPE MfuncParamMilUint(MIL_ID FuncId, MIL_INT ParamIndex, MIL_UINT ParamValue);
void MFTYPE MfuncParamMilUint32(MIL_ID FuncId, MIL_INT ParamIndex, MIL_UINT32 ParamValue);
void MFTYPE MfuncParamMilUint64(MIL_ID FuncId, MIL_INT ParamIndex, MIL_UINT64 ParamValue);
void MFTYPE MfuncParamMilDouble(MIL_ID FuncId, MIL_INT ParamIndex, MIL_DOUBLE ParamValue);
void MFTYPE MfuncParamDataPointer(MIL_ID FuncId, MIL_INT ParamIndex, void *ParamValuePtr, MIL_INT NbOfItems, MIL_INT Attribute);
#if OldDefinesSupport
#define MfuncParamDouble MfuncParamMilDouble // it was documented in MIL 9, so keep it
MIL_DEPRECATED(MfuncParamDouble, 1000)
#define MfuncParamPointer MfuncParamDataPointer
MIL_DEPRECATED(MfuncParamPointer, 1000)
#endif
void MFTYPE MfuncParamIdPointer(MIL_ID FuncId, MIL_INT ParamIndex, MIL_ID *ParamValuePtr, MIL_INT64 ObjectType, MIL_INT Attribute);
void MFTYPE MfuncParamValue(MIL_ID FuncId, MIL_INT ParamIndex, void *ParamValuePtr);
MIL_INT MFTYPE MfuncGetError(MIL_ID FunctId, MIL_INT ErrorType,void* ErrorVarPtr);
void MFTYPE MfuncFreeId(MIL_ID FunctionId, MIL_ID ObjectId);
void MFTYPE MfuncFree(MIL_ID FuncId);
MIL_INT MFTYPE MfuncCall(MIL_ID FuncId);
MIL_INT MFTYPE MfuncInquire(MIL_ID ObjectId, MIL_INT64 InquireType, void* UserVarPtr);
MIL_INT MFTYPE MfuncExit();
void* MFTYPE MfuncMemory(MIL_INT64 Operation, MIL_INT OperationValue, MIL_INT64 OperationFlag, void *DataPointer);
MIL_ID MFTYPE MfuncGetNextId(MIL_ID CurrentId);
void MFTYPE MfuncMDTrace(FILE** FileHandle, MIL_INT64 Action, void* DataPointer);
void MFTYPE MfuncTraceTimerRead(MIL_DOUBLE* pCurrentTime);

#if M_MIL_USE_64BIT
   void   MFTYPE MfuncControlInt64(MIL_ID ObjectId, MIL_INT64 ControlType, MIL_INT64 ControlFlag);
   #ifdef __cplusplus
      //////////////////////////////////////////////////////////////
      // MfuncControl function definition when compiling c++ files
      //////////////////////////////////////////////////////////////
      inline void MfuncControl(MIL_ID ObjectId,
                               MIL_INT64 ControlType,
                               MIL_INT64 ControlFlag)
         {
         MfuncControlInt64(ObjectId, ControlType, ControlFlag);
         };
   #else
      //////////////////////////////////////////////////////////////
      // For C file, call the default function, i.e. Int64 one
      //////////////////////////////////////////////////////////////
      #define MfuncControl MfuncControlInt64
   #endif // __cplusplus
#else
   #define MfuncControlInt64 MfuncControl
   void   MFTYPE MfuncControl(MIL_ID ObjectId, MIL_INT64 ControlType, MIL_DOUBLE ControlFlag);
#endif // M_MIL_USE_64BIT

/* The following MfuncParamValue are for getting the value by type */
void MFTYPE MfuncParamValueMilInt(MIL_ID FuncId, MIL_INT ParamIndex, MIL_INT *ParamValuePtr);
void MFTYPE MfuncParamValueMilInt32(MIL_ID FuncId, MIL_INT ParamIndex, MIL_INT32 *ParamValuePtr);
void MFTYPE MfuncParamValueMilInt64(MIL_ID FuncId, MIL_INT ParamIndex, MIL_INT64 *ParamValuePtr);
void MFTYPE MfuncParamValueMilUint(MIL_ID FuncId, MIL_INT ParamIndex, MIL_UINT *ParamValuePtr);
void MFTYPE MfuncParamValueMilUint32(MIL_ID FuncId, MIL_INT ParamIndex, MIL_UINT32 *ParamValuePtr);
void MFTYPE MfuncParamValueMilUint64(MIL_ID FuncId, MIL_INT ParamIndex, MIL_UINT64 *ParamValuePtr);

void MFTYPE MfuncParamValueMilDouble(MIL_ID FuncId, MIL_INT ParamIndex, MIL_DOUBLE *ParamValuePtr);
void MFTYPE MfuncParamValueMilId(MIL_ID FuncId, MIL_INT ParamIndex, MIL_ID *ParamValuePtr);

void MFTYPE MfuncParamValueArrayMilDouble(MIL_ID FuncId, MIL_INT ParamIndex, MIL_DOUBLE **ParamValuePtr);
void MFTYPE MfuncParamValueConstArrayMilDouble(MIL_ID FuncId, MIL_INT ParamIndex, const MIL_DOUBLE **ParamValuePtr);

#if OldDefinesSupport
#define MfuncParamValueDouble MfuncParamValueMilDouble                     // it was documented in MIL 9, so keep it
MIL_DEPRECATED(MfuncParamValueDouble, 1000)
#define MfuncParamValueId MfuncParamValueMilId                             // it was documented in MIL 9, so keep it
MIL_DEPRECATED(MfuncParamValueId, 1000)
#define MfuncParamValueArrayDouble MfuncParamValueArrayMilDouble           // it was documented in MIL 9, so keep it
MIL_DEPRECATED(MfuncParamValueArrayDouble, 1000)
#define MfuncParamValueConstArrayDouble MfuncParamValueConstArrayMilDouble // it was documented in MIL 9, so keep it
MIL_DEPRECATED(MfuncParamValueConstArrayDouble, 1000)
#endif

MIL_DLLFUNC void MFTYPE MfuncParamValueMilTextA(MIL_ID FuncId, MIL_INT ParamIndex, char* *ParamValuePtr);
MIL_DLLFUNC void MFTYPE MfuncParamValueConstMilTextA(MIL_ID FuncId, MIL_INT ParamIndex, const char* *ParamValuePtr);
MIL_DLLFUNC void MFTYPE MfuncParamValueFilenameA(MIL_ID FuncId, MIL_INT ParamIndex, const char* *ParamValuePtr);

#if M_MIL_USE_UNICODE
#if M_MIL_UNICODE_API
#define MfuncParamValueMilText       MfuncParamValueMilTextW
#define MfuncParamValueConstMilText  MfuncParamValueConstMilTextW
#define MfuncParamValueFilename     MfuncParamValueFilenameW
#else
#define MfuncParamValueMilText       MfuncParamValueMilTextA
#define MfuncParamValueConstMilText  MfuncParamValueConstMilTextA
#define MfuncParamValueFilename     MfuncParamValueFilenameA
#endif

void MFTYPE MfuncParamValueMilTextW(MIL_ID FuncId, MIL_INT ParamIndex, MIL_TEXT_PTR *ParamValuePtr);
void MFTYPE MfuncParamValueConstMilTextW(MIL_ID FuncId, MIL_INT ParamIndex, MIL_CONST_TEXT_PTR *ParamValuePtr);
void MFTYPE MfuncParamValueFilenameW(MIL_ID FuncId, MIL_INT ParamIndex, MIL_CONST_TEXT_PTR *ParamValuePtr);

void MFTYPE MfuncParamValueMilTextA(MIL_ID FuncId, MIL_INT ParamIndex, char* *ParamValuePtr);
void MFTYPE MfuncParamValueConstMilTextA(MIL_ID FuncId, MIL_INT ParamIndex, const char* *ParamValuePtr);
void MFTYPE MfuncParamValueFilenameA(MIL_ID FuncId, MIL_INT ParamIndex, const char* *ParamValuePtr);
#else

void MFTYPE MfuncParamValueMilText(MIL_ID FuncId, MIL_INT ParamIndex, MIL_TEXT_PTR *ParamValuePtr);
void MFTYPE MfuncParamValueConstMilText(MIL_ID FuncId, MIL_INT ParamIndex, MIL_CONST_TEXT_PTR *ParamValuePtr);
void MFTYPE MfuncParamValueFilename(MIL_ID FuncId, MIL_INT ParamIndex, MIL_CONST_TEXT_PTR *ParamValuePtr);

#endif

void MFTYPE MfuncParamValueArrayMilInt(MIL_ID FuncId, MIL_INT ParamIndex, MIL_INT **ParamValuePtr);
void MFTYPE MfuncParamValueArrayMilUint(MIL_ID FuncId, MIL_INT ParamIndex, MIL_UINT **ParamValuePtr);

void MFTYPE MfuncParamValueArrayMilInt32(MIL_ID FuncId, MIL_INT ParamIndex, MIL_INT32 **ParamValuePtr);
void MFTYPE MfuncParamValueArrayMilUint32(MIL_ID FuncId, MIL_INT ParamIndex, MIL_UINT32 **ParamValuePtr);
void MFTYPE MfuncParamValueArrayMilInt64(MIL_ID FuncId, MIL_INT ParamIndex, MIL_INT64 **ParamValuePtr);
void MFTYPE MfuncParamValueArrayMilUint64(MIL_ID FuncId, MIL_INT ParamIndex, MIL_UINT64 **ParamValuePtr);

void MFTYPE MfuncParamValueConstArrayMilInt32(MIL_ID FuncId, MIL_INT ParamIndex, const MIL_INT32 **ParamValuePtr);
void MFTYPE MfuncParamValueConstArrayMilUint32(MIL_ID FuncId, MIL_INT ParamIndex, const MIL_UINT32 **ParamValuePtr);
void MFTYPE MfuncParamValueConstArrayMilInt64(MIL_ID FuncId, MIL_INT ParamIndex, const MIL_INT64 **ParamValuePtr);
void MFTYPE MfuncParamValueConstArrayMilUint64(MIL_ID FuncId, MIL_INT ParamIndex, const MIL_UINT64 **ParamValuePtr);

void MFTYPE MfuncParamValueArrayMilId(MIL_ID FuncId, MIL_INT ParamIndex, MIL_ID **ParamValuePtr);
#if OldDefinesSupport
#define MfuncParamValueArrayId MfuncParamValueArrayMilId                          // it was documented in MIL 9, so keep it
MIL_DEPRECATED(MfuncParamValueArrayId, 1000)
#endif

void MFTYPE MfuncParamValuePointer(MIL_ID FuncId, MIL_INT ParamIndex, void ** ParamValuePtr);
void MFTYPE MfuncParamValueConstDataPointer(MIL_ID FuncId, MIL_INT ParamIndex, const void ** ParamValuePtr);
void MFTYPE MfuncParamValueConstArrayMilId(MIL_ID FuncId, MIL_INT ParamIndex,const MIL_ID **ParamValuePtr);
#if OldDefinesSupport
#define MfuncParamValueConstArrayId MfuncParamValueConstArrayMilId                // it was documented in MIL 9, so keep it
MIL_DEPRECATED(MfuncParamValueConstArrayId, 1000)
#endif
void MFTYPE MfuncParamValueConstArrayMilInt(MIL_ID FuncId, MIL_INT ParamIndex, const MIL_INT **ParamValuePtr);
void MFTYPE MfuncParamValueConstArrayMilUint(MIL_ID FuncId, MIL_INT ParamIndex, const MIL_UINT **ParamValuePtr);


void MFTYPE MfuncParamArrayMilInt(MIL_ID FuncId, MIL_INT ParamIndex, MIL_INT *ParamValuePtr, MIL_INT NbOfItems, MIL_INT Attribute);
void MFTYPE MfuncParamArrayMilDouble(MIL_ID FuncId, MIL_INT ParamIndex, MIL_DOUBLE *ParamValuePtr, MIL_INT NbOfItems, MIL_INT Attribute);
#if OldDefinesSupport
#define MfuncParamArrayDouble MfuncParamArrayMilDouble   // it was documented in MIL 9, so keep it
MIL_DEPRECATED(MfuncParamArrayDouble, 1000)
#endif
void MFTYPE MfuncParamArrayMilUint(MIL_ID FuncId, MIL_INT ParamIndex, MIL_UINT *ParamValuePtr, MIL_INT NbOfItems, MIL_INT Attribute);
void MFTYPE MfuncParamArrayMilId(MIL_ID FuncId, MIL_INT ParamIndex, MIL_ID *ParamValuePtr, MIL_INT NbOfItems, MIL_INT64 ObjectType, MIL_INT Attribute);
#if OldDefinesSupport
#define MfuncParamArrayId MfuncParamArrayMilId           // it was documented in MIL 9, so keep it
MIL_DEPRECATED(MfuncParamArrayId, 1000)
#endif
void MFTYPE MfuncParamArrayMilInt32(MIL_ID FuncId, MIL_INT ParamIndex, MIL_INT32 *ParamValuePtr, MIL_INT NbOfItems, MIL_INT Attribute);
void MFTYPE MfuncParamArrayMilInt64(MIL_ID FuncId, MIL_INT ParamIndex, MIL_INT64 *ParamValuePtr, MIL_INT NbOfItems, MIL_INT Attribute);
void MFTYPE MfuncParamArrayMilUint32(MIL_ID FuncId, MIL_INT ParamIndex, MIL_UINT32 *ParamValuePtr, MIL_INT NbOfItems, MIL_INT Attribute);
void MFTYPE MfuncParamArrayMilUint64(MIL_ID FuncId, MIL_INT ParamIndex, MIL_UINT64 *ParamValuePtr, MIL_INT NbOfItems, MIL_INT Attribute);

void MFTYPE MfuncParamConstArrayMilDouble(MIL_ID FuncId, MIL_INT ParamIndex, const MIL_DOUBLE *ParamValuePtr, MIL_INT NbOfItems, MIL_INT Attribute);
#if OldDefinesSupport
#define MfuncParamConstArrayDouble MfuncParamConstArrayMilDouble // it was documented in MIL 9, so keep it
MIL_DEPRECATED(MfuncParamConstArrayDouble, 1000)
#endif
void MFTYPE MfuncParamConstArrayMilInt32(MIL_ID FuncId, MIL_INT ParamIndex, const MIL_INT32 *ParamValuePtr, MIL_INT NbOfItems, MIL_INT Attribute);
void MFTYPE MfuncParamConstArrayMilInt64(MIL_ID FuncId, MIL_INT ParamIndex, const MIL_INT64 *ParamValuePtr, MIL_INT NbOfItems, MIL_INT Attribute);
void MFTYPE MfuncParamConstArrayMilUint32(MIL_ID FuncId, MIL_INT ParamIndex, const MIL_UINT32 *ParamValuePtr, MIL_INT NbOfItems, MIL_INT Attribute);
void MFTYPE MfuncParamConstArrayMilUint64(MIL_ID FuncId, MIL_INT ParamIndex, const MIL_UINT64 *ParamValuePtr, MIL_INT NbOfItems, MIL_INT Attribute);
void MFTYPE MfuncParamConstArrayMilId(MIL_ID FuncId, MIL_INT ParamIndex,const MIL_ID *ParamValuePtr, MIL_INT NbOfItems, MIL_INT64 ObjectType, MIL_INT Attribute);
#if OldDefinesSupport
#define MfuncParamConstArrayId MfuncParamConstArrayMilId // it was documented in MIL 9, so keep it
MIL_DEPRECATED(MfuncParamConstArrayId, 1000)
#endif
void MFTYPE MfuncParamConstArrayMilInt(MIL_ID FuncId, MIL_INT ParamIndex, const MIL_INT *ParamValuePtr, MIL_INT NbOfItems, MIL_INT Attribute);
void MFTYPE MfuncParamConstArrayMilUint(MIL_ID FuncId, MIL_INT ParamIndex, const MIL_UINT *ParamValuePtr, MIL_INT NbOfItems, MIL_INT Attribute);

void MFTYPE MfuncParamConstDataPointer(MIL_ID FuncId, MIL_INT ParamIndex, const void * ParamValuePtr, MIL_INT NbOfItems, MIL_INT Attribute);


/*The following Mfunc functions are obsolete*/
MIL_INT MFTYPE MfuncStart(MIL_ID FuncId);          /*Replaced by MfuncCall*/
void    MFTYPE MfuncFreeAndEnd(MIL_ID FuncId);     /*Replaced by MfuncCall*/
MIL_INT MFTYPE MfuncModified(MIL_ID ImageId);      /*Replaced by MbufControl(M_MODIFIED)*/
MIL_INT MFTYPE MfuncParamRegister(MIL_ID FuncId);  /*Parameters must always be registered*/
MIL_INT MFTYPE MfuncIdGetObjectType(MIL_ID FunctionId, MIL_ID ObjectId);               /*Replaced by MfuncInquire*/
void    MFTYPE MfuncIdSetUserPtr(MIL_ID FunctionId, MIL_ID ObjectId, void *UserPtr);   /*Replaced by MfuncControl*/
void*   MFTYPE MfuncIdGetUserPtr(MIL_ID FunctionId, MIL_ID ObjectId);                  /*Replaced by MfuncInquire*/



/* C++ directive if needed */
#ifdef __cplusplus
}
#endif

#endif
