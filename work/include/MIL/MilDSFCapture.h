/************************************************************************/
/*
*
* Filename     :  MilCaptureBase.H
* Revision     :  10.00.2564
* Content      :  This file contains the interface structure required for 
*                 MIL Capture DirectShow Filter.
*
* Comments     :  Some defines may be here but not yet
*                 implemented in the library.
*
* Copyright © Matrox Electronic Systems Ltd., 1992-2014.
* All Rights Reserved
*************************************************************************/

#pragma once
#pragma warning(disable:4995)

#include <initguid.h>
#include "MilDSFCaptureGUID.h"

#include <mil.h>

enum MEDIASUBTYPE_FORMAT
   {
   MF_UNKNOWN = -1,
   MF_RGB32,
   MF_YUV16,
   MF_NV12,
   MF_RGB24,
   MF_ARGB32,
   };

enum CAPTURE_BUFFER_FORMAT
   {
   CBF_UNKNOWN = -1,
   CBF_BGR32,
   CBF_YUV16_YUYV,
   CBF_MONO8,
   };

enum PinDbgParam
   {
   PRINT_PIN_FRAME_COUNTER,
   PRINT_PIN_TIMESTAMP,
   };

/////////////////////////////////////////////////////////////////
//
// Interface         : IMilCapture
//
// Synopsis          : Access the system
//
// Comments          : for the functions begining with "sys" they are
//                      equivalent to Msys from Mil but you do not
//                      need to place the MIL_ID of the system
//
/////////////////////////////////////////////////////////////////
DECLARE_INTERFACE_(IMilCapture, IBaseFilter)
   {
   // USE THIS METHOD BEFORE ADDING THE FILTER
#if UNICODE
#define setSystemInfo setSystemInfoW
#else
#define setSystemInfo setSystemInfoA
#endif

   STDMETHOD(setSystemInfoA)
      ( THIS_
      const char* systemName, // [in] system descriptor string
      MIL_INT devNum // [in] system Number
      )  PURE;

   STDMETHOD(setSystemInfoW)
      ( THIS_
      const wchar_t* systemName, // [in] system descriptor string
      MIL_INT devNum // [in] system Number
      )  PURE;

   STDMETHOD(getAppID)
      ( THIS_
      MIL_ID* appID  // [out] adress to contain the returning application MIL_ID
      )  PURE;

   // -------------------------------------------------------------------------
   // Mil System exposed function
   // -------------------------------------------------------------------------
   // Same Call as Mil MsysControl without SystemId
   STDMETHOD(sysControl)
      ( THIS_
      MIL_INT64 ControlType,
      MIL_INT ControlValue
      )  PURE;

   // Same Call as Mil MsysGetHookInfo without SystemId
   STDMETHOD(sysGetHookInfo)
      ( THIS_
      MIL_ID EventId,
      MIL_INT64 InquireType,
      MIL_INT* UserVarPtr
      )  PURE;

   // Same Call as Mil MsysHookFunction without SystemId
   STDMETHOD(sysHookFunction)
      ( THIS_
      MIL_INT HookType,
      MIL_SYS_HOOK_FUNCTION_PTR HookHandlerPtr,
      void* UserDataPtr
      )  PURE;

   // *************************************************************************
   // Multiple definition for sysInquire to support all output format
   // *************************************************************************
   // Same Call as Mil MsysInquire without SystemId
   STDMETHOD(sysInquire)
      ( THIS_
      MIL_INT64 InquireType,
      MIL_INT32* UserVarPtr 
      )  PURE;

   // Same Call as Mil MsysInquire without SystemId
   STDMETHOD(sysInquire)
      ( THIS_
      MIL_INT64 InquireType,
      MIL_INT64* UserVarPtr
      )  PURE;

   // Same Call as Mil MsysInquire without SystemId
   STDMETHOD(sysInquire)
      ( THIS_
      MIL_INT64 InquireType,
      MIL_TEXT_CHAR* UserVarPtr
      )  PURE;

   // Same Call as Mil MsysInquire without SystemId
   STDMETHOD(sysInquire)
      ( THIS_
      MIL_INT64 InquireType,
      MIL_DOUBLE* UserVarPtr
      )  PURE;

#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
   // Same Call as Mil MsysInquire without SystemId
   STDMETHOD(sysInquire)
      ( THIS_
      MIL_INT64 InquireType,
      MIL_UINT32* UserVarPtr
      )  PURE;

   // Same Call as Mil MsysInquire without SystemId
   STDMETHOD(sysInquire)
      ( THIS_
      MIL_INT64 InquireType,
      MIL_UINT64* UserVarPtr
      )  PURE;
#endif // M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED

   };


/////////////////////////////////////////////////////////////////
//
// Interface         : IMilCapturePin
//
// Synopsis          : Access the digitizer
//
// Comments          : for the functions begining with "dig" they are
//                      equivalent to Mdig call from Mil but you do not
//                      need to place the MIL_ID of the Digitizer
//
/////////////////////////////////////////////////////////////////
DECLARE_INTERFACE_(IMilCapturePin, IPin)
   {
#if UNICODE
#define set_DigitizerInfo set_DigitizerInfoW
#else
#define set_DigitizerInfo set_DigitizerInfoA
#endif

   STDMETHOD(set_DigitizerInfoW)
      ( THIS_
      MIL_INT DevNum,   // [in] Digitizer Number
      MIL_INT64 ChNum,  // [in] Digitizer channel Number
      const wchar_t* DCFPath    // [in] complete string to represent the DCF
      )  PURE;

   STDMETHOD(set_DigitizerInfoA)
      ( THIS_
      MIL_INT DevNum,   // [in] Digitizer Number
      MIL_INT64 ChNum,  // [in] Digitizer channel Number
      const char* DCFPath    // [in] complete string to represent the DCF
      )  PURE;

   STDMETHOD(get_FIFOSize)
      ( THIS_
      MIL_INT* FIFOSize // [out] Number of buffer to allocate for MdigProcess
      )  PURE;

   STDMETHOD(set_FIFOSize)
      ( THIS_
      MIL_INT FIFOSize // [in] Number of buffer to allocate for MdigProcess
      )  PURE;

   STDMETHOD(is_DigAllocated)
      ( THIS_
      )  PURE;

   STDMETHOD(set_DebugParam)
      ( THIS_
      PinDbgParam DebugOptionType, // [in] Debug parameter ID
      MIL_INT DebugOptionValue // [in] Value to set
      )  PURE;

   STDMETHOD(get_DebugParam)
      ( THIS_
      PinDbgParam DebugOptionType, // [in] Debug parameter ID
      MIL_INT* DebugOptionValue // [out] Value of the parameter
      )  PURE;

   // Function moves specified MediaSubType Format to desired rank
   STDMETHOD(Set_PinMediaSubTypePriority)
      ( THIS_
      MEDIASUBTYPE_FORMAT mfID,  // [in] Pin MediaSubtype format to move
      MIL_INT priorityIdx        // [in] position indexed at 0 of the rank in priority
      )  PURE;

   STDMETHOD(Get_PinMediaSubTypeAtPriority)
      ( THIS_
      MIL_INT priorityIdx,       // [in] position indexed at 0 of the rank in priority
      MEDIASUBTYPE_FORMAT* mfID  // [out] Pin MediaSubtype format pointer
      )  PURE;

   // Function moves specified Capture Buffer Format to desired rank
   STDMETHOD(Set_CaptureBufferFormatPriority)
      ( THIS_
      CAPTURE_BUFFER_FORMAT cbfID,  // [in] Capture Buffer Format to move
      MIL_INT priorityIdx           // [in] position indexed at 0 of the rank in priority
      )  PURE;

   STDMETHOD(Get_CaptureBufferFormatAtPriority)
      ( THIS_
      MIL_INT priorityIdx,          // [in] position indexed at 0 of the rank in priority
      CAPTURE_BUFFER_FORMAT* cbfID  // [out] Capture Buffer Format pointer
      )  PURE;


   // -------------------------------------------------------------------------
   // Mil Digitizer exposed function
   // -------------------------------------------------------------------------
   // Same Call as Mil MdigChannel without DigId
   __declspec(deprecated) STDMETHOD(digChannel)
      ( THIS_
      MIL_INT64 channel
      )  PURE;

   // Same Call as Mil MdigControl without DigId
   STDMETHOD(digControl)
      ( THIS_
      MIL_INT64  ControlType,
      MIL_DOUBLE ControlValue
      )  PURE;

   // Same Call as Mil MdigControl without DigId
   STDMETHOD(digControl)
      ( THIS_
      MIL_INT64  ControlType,
      MIL_INT32 ControlValue
      )  PURE;

   // Same Call as Mil MdigControl without DigId
   STDMETHOD(digControl)
      ( THIS_
      MIL_INT64  ControlType,
      MIL_INT64 ControlValue
      )  PURE;

#if UNICODE
#define digControlFeature digControlFeatureW
#else
#define digControlFeature digControlFeatureA
#endif

   // Same Call as Mil MdigControlFeature without DigitizerId
   STDMETHOD(digControlFeatureA)
      ( THIS_
      MIL_INT64 ControlFlag,
      const char* FeatureName,
      MIL_INT64 FeatureDataType,
      MIL_TEXT_CHAR* FeatureValuePtr
      )  PURE;

   // Same Call as Mil MdigControlFeature without DigitizerId
   STDMETHOD(digControlFeatureA)
      ( THIS_
      MIL_INT64 ControlFlag,
      const char* FeatureName,
      MIL_INT64 FeatureDataType,
      MIL_INT64* FeatureValuePtr
      )  PURE;

   // Same Call as Mil MdigControlFeature without DigitizerId
   STDMETHOD(digControlFeatureA)
      ( THIS_
      MIL_INT64 ControlFlag,
      const char* FeatureName,
      MIL_INT64 FeatureDataType,
      MIL_DOUBLE* FeatureValuePtr
      )  PURE;

   // Same Call as Mil MdigControlFeature without DigitizerId
   STDMETHOD(digControlFeatureA)
      ( THIS_
      MIL_INT64 ControlFlag,
      const char* FeatureName,
      MIL_INT64 FeatureDataType,
      MIL_INT32* FeatureValuePtr
      )  PURE;

   // Same Call as Mil MdigControlFeature without DigitizerId
   STDMETHOD(digControlFeatureA)
      ( THIS_
      MIL_INT64 ControlFlag,
      const char* FeatureName,
      MIL_INT64 FeatureDataType,
      MIL_UINT8* FeatureValuePtr
      )  PURE;

   // Same Call as Mil MdigControlFeature without DigitizerId
   STDMETHOD(digControlFeatureA)
      ( THIS_
      MIL_INT64 ControlFlag,
      const char* FeatureName,
      MIL_INT64 FeatureDataType,
      int FeatureValue
      )  PURE;

   STDMETHOD(digControlFeatureW)
      ( THIS_
      MIL_INT64 ControlFlag,
      const wchar_t* FeatureName,
      MIL_INT64 FeatureDataType,
      MIL_TEXT_CHAR* FeatureValuePtr
      )  PURE;

   // Same Call as Mil MdigControlFeature without DigitizerId
   STDMETHOD(digControlFeatureW)
      ( THIS_
      MIL_INT64 ControlFlag,
      const wchar_t* FeatureName,
      MIL_INT64 FeatureDataType,
      MIL_INT64* FeatureValuePtr
      )  PURE;

   // Same Call as Mil MdigControlFeature without DigitizerId
   STDMETHOD(digControlFeatureW)
      ( THIS_
      MIL_INT64 ControlFlag,
      const wchar_t* FeatureName,
      MIL_INT64 FeatureDataType,
      MIL_DOUBLE* FeatureValuePtr
      )  PURE;

   // Same Call as Mil MdigControlFeature without DigitizerId
   STDMETHOD(digControlFeatureW)
      ( THIS_
      MIL_INT64 ControlFlag,
      const wchar_t* FeatureName,
      MIL_INT64 FeatureDataType,
      MIL_INT32* FeatureValuePtr
      )  PURE;

   // Same Call as Mil MdigControlFeature without DigitizerId
   STDMETHOD(digControlFeatureW)
      ( THIS_
      MIL_INT64 ControlFlag,
      const wchar_t* FeatureName,
      MIL_INT64 FeatureDataType,
      MIL_UINT8* FeatureValuePtr
      )  PURE;

   // Same Call as Mil MdigControlFeature without DigitizerId
   STDMETHOD(digControlFeatureW)
      ( THIS_
      MIL_INT64 ControlFlag,
      const wchar_t* FeatureName,
      MIL_INT64 FeatureDataType,
      int FeatureValue
      )  PURE;

   // *************************************************************************
   // Multiple definition for digGetHookInfo to support all output format
   // *************************************************************************
   // Same Call as Mil MdigGetHookInfo without DigId
   STDMETHOD(digGetHookInfo)
      ( THIS_
      MIL_ID EventId,
      MIL_INT64 InfoType,
      MIL_DOUBLE* UserVarPtr
      )  PURE;

   // Same Call as Mil MdigGetHookInfo
   STDMETHOD(digGetHookInfo)
      ( THIS_
      MIL_ID EventId,
      MIL_INT64 InfoType,
      MIL_INT* UserVarPtr
      )  PURE;

   // Same Call as Mil MdigHokfunction without DigId
   STDMETHOD(digHookFunction)
      ( THIS_
      MIL_INT HookType,
      MIL_DIG_HOOK_FUNCTION_PTR HookHandlerPtr,
      void* UserDataPtr
      )  PURE;

   // *************************************************************************
   // Multiple definition for digInquire to support all output format
   // *************************************************************************
   // Same Call as Mil MdigInquire without DigId
   STDMETHOD(digInquire)
      ( THIS_
      MIL_INT64 InquireType,
      MIL_INT32* UserVarPtr
      )  PURE;

   // Same Call as Mil MdigInquire without DigId
   STDMETHOD(digInquire)
      ( THIS_
      MIL_INT64 InquireType,
      MIL_INT64* UserVarPtr
      )  PURE;

   // Same Call as Mil MdigInquire without DigId
   STDMETHOD(digInquire)
      ( THIS_
      MIL_INT64 InquireType,
      MIL_TEXT_CHAR* UserVarPtr
      )  PURE;

   // Same Call as Mil MdigInquire without DigId
   STDMETHOD(digInquire)
      ( THIS_
      MIL_INT64 InquireType,
      MIL_DOUBLE* UserVarPtr
      )  PURE;

#if M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
   // Same Call as Mil MdigInquire without DigId
   STDMETHOD(digInquire)
      ( THIS_
      MIL_INT64 InquireType,
      MIL_UINT32* UserVarPtr
      )  PURE;

   // Same Call as Mil MdigInquire without DigId
   STDMETHOD(digInquire)
      ( THIS_
      MIL_INT64 InquireType,
      MIL_UINT64* UserVarPtr
      )  PURE;
#endif // M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED

#if UNICODE
#define digInquireFeature digInquireFeatureW
#else
#define digInquireFeature digInquireFeatureA
#endif

   // Same Call as Mil MdigInquireFeature without DigitizerId
   STDMETHOD(digInquireFeatureA)
      ( THIS_
      MIL_INT64 InquireFlag,
      const char* FeatureName,
      MIL_INT64 FeatureDataType,
      MIL_TEXT_CHAR* FeatureValuePtr
      )  PURE;

   STDMETHOD(digInquireFeatureA)
      ( THIS_
      MIL_INT64 InquireFlag,
      const char* FeatureName,
      MIL_INT64 FeatureDataType,
      MIL_INT64 *FeatureValuePtr
      )  PURE;

   STDMETHOD(digInquireFeatureA)
      ( THIS_
      MIL_INT64 InquireFlag,
      const char* FeatureName,
      MIL_INT64 FeatureDataType,
      MIL_DOUBLE *FeatureValuePtr
      )  PURE;

   STDMETHOD(digInquireFeatureA)
      ( THIS_
      MIL_INT64 InquireFlag,
      const char* FeatureName,
      MIL_INT64 FeatureDataType,
      MIL_INT32 *FeatureValuePtr
      )  PURE;

   STDMETHOD(digInquireFeatureA)
      ( THIS_
      MIL_INT64 InquireFlag,
      const char* FeatureName,
      MIL_INT64 FeatureDataType,
      MIL_UINT8 *FeatureValuePtr
      )  PURE;

   // Same Call as Mil MdigInquireFeature without DigitizerId
   STDMETHOD(digInquireFeatureW)
      ( THIS_
      MIL_INT64 InquireFlag,
      const wchar_t* FeatureName,
      MIL_INT64 FeatureDataType,
      MIL_TEXT_CHAR* FeatureValuePtr
      )  PURE;

   STDMETHOD(digInquireFeatureW)
      ( THIS_
      MIL_INT64 InquireFlag,
      const wchar_t* FeatureName,
      MIL_INT64 FeatureDataType,
      MIL_INT64 *FeatureValuePtr
      )  PURE;

   STDMETHOD(digInquireFeatureW)
      ( THIS_
      MIL_INT64 InquireFlag,
      const wchar_t* FeatureName,
      MIL_INT64 FeatureDataType,
      MIL_DOUBLE *FeatureValuePtr
      )  PURE;

   STDMETHOD(digInquireFeatureW)
      ( THIS_
      MIL_INT64 InquireFlag,
      const wchar_t* FeatureName,
      MIL_INT64 FeatureDataType,
      MIL_INT32 *FeatureValuePtr
      )  PURE;

   STDMETHOD(digInquireFeatureW)
      ( THIS_
      MIL_INT64 InquireFlag,
      const wchar_t* FeatureName,
      MIL_INT64 FeatureDataType,
      MIL_UINT8 *FeatureValuePtr
      )  PURE;

   // Same Call as Mil MdigReference without DigId
   __declspec(deprecated) STDMETHOD(digReference)
      ( THIS_
      MIL_INT64  ReferenceType,
      MIL_DOUBLE ReferenceLevel
      )  PURE;
   };
