/***************************************************************************/
/*                                                                         */
/*                                                                         */
/*  Filename:  MILSETUP.H                                                  */
/*  Revision:  10.00.2564                                                    */
/*  Content :  This file contains definitions for specifying the target    */
/*             compile environment and the default state to set for        */
/*             MIL (Matrox Imaging Library). It also defines the           */
/*             MappAllocDefault() and MappFreeDefault() macros.            */
/*                                                                         */
/*  Copyright © Matrox Electronic Systems Ltd., 1992-2014.                */
/*  All Rights Reserved.                                                   */
/*                                                                         */
/*                                                                         */
/***************************************************************************/
#ifndef __MILSETUP_H__
#define __MILSETUP_H__

/************************************************************************/
/* MIL COMPILE VERSION                                                  */
/* These defines are for Matrox internal purpose only and are NOT meant */
/* to be edited by the client.                                          */
/************************************************************************/
#if !defined(MIL_COMPILE_VERSION_HEX) || (MIL_COMPILE_VERSION_HEX== 0)
   #ifdef MIL_COMPILE_VERSION_HEX
      #undef MIL_COMPILE_VERSION_HEX
   #endif
   #define MIL_COMPILE_VERSION_HEX 0x1000
#endif

// Let's put the hex value in decimal.
#define MIL_COMPILE_VERSION ((((MIL_COMPILE_VERSION_HEX >> 12) & 0xf) * 1000) + (((MIL_COMPILE_VERSION_HEX >> 8) & 0xf) * 100) + (((MIL_COMPILE_VERSION_HEX >> 4) & 0xf) * 10) + (((MIL_COMPILE_VERSION_HEX) & 0xf) ))


/************************************************************************/
/* COMPILATION FLAG                                                     */
/* One and only one flag must be active                                 */
/************************************************************************/
#ifndef M_MIL_USE_OS
   #define M_MIL_USE_OS               1

#ifndef M_MIL_USE_XEON_PHI
#if defined(__MIC__)
   #define M_MIL_USE_XEON_PHI            1
#else
   #define M_MIL_USE_XEON_PHI            0
#endif
#endif

   #if defined(__linux__)
      #define M_MIL_USE_WINDOWS       0
      #define M_MIL_USE_PHARLAP       0
      #define M_MIL_USE_LINUX         1
      #define M_MIL_USE_CE            0
      #define M_MIL_USE_NEW_STDHEADERS 1
      #ifdef __x86_64__
         #define M_MIL_USE_64BIT      1
      #else
         #define M_MIL_USE_64BIT      0
      #endif

      #ifndef M_LINUX_KERNEL
         #define M_LINUX_KERNEL       0
      #endif

      #ifdef __INTEL_COMPILER
         #define M_MIL_USE_LINTEL     1
      #endif
   #elif defined(UNDER_CE)
      #define M_MIL_USE_WINDOWS       1
      #define M_MIL_USE_PHARLAP       0
      #define M_MIL_USE_LINUX         0
      #define M_MIL_USE_CE            1
      #define M_MIL_USE_64BIT         0

      #if (_MSC_VER >= 1400) && !defined(M_MIL_USING_SAFE_CRT)
         #define M_MIL_USING_SAFE_CRT       1
      #endif
   #else
      #define M_MIL_USE_WINDOWS       1
      #ifndef M_MIL_USE_PHARLAP
         #define M_MIL_USE_PHARLAP    0
      #endif
      #define M_MIL_USE_LINUX         0
      #define M_MIL_USE_CE            0
      #ifdef _WIN64
         #define M_MIL_USE_64BIT      1
      #else
         #define M_MIL_USE_64BIT      0
      #endif

      #if (_MSC_VER >= 1400) && !defined(M_MIL_USING_SAFE_CRT)
         #define M_MIL_USING_SAFE_CRT       1
      #endif
   #endif

   #if defined(__POWERPC__)
#error "PowerPC not supported"
   #endif
#endif

#if !(M_MIL_USE_WINDOWS || M_MIL_USE_LINUX)
#error "You must set at least one compilation flag."
#endif

#if ( (M_MIL_USE_WINDOWS && M_MIL_USE_LINUX) )
#error "You must select one and only one compilation flag."
#endif

// Defined M_MIL_LITE to avoid declaration of highlvl safe type functions
// while compiling MIL dll
#if M_COMPILING_MIL_LITE && !defined M_MIL_LITE
#define M_MIL_LITE 1
#endif

/************************************************************************/
/*This flag indicates if MIL has been compiled without UNICODE support  */
/*If MIL supports UNICODE, user can define UNICODE to use UNICODE API   */
/************************************************************************/
#if !defined(M_MIL_USE_UNICODE) && !M_MIL_USE_LINUX
   #define M_MIL_USE_UNICODE 1
#elif !defined (M_MIL_USE_UNICODE)
   #define M_MIL_USE_UNICODE 0
#endif

#if  M_MIL_USE_UNICODE
   #if defined(UNICODE)
      #define M_MIL_UNICODE_API 1
   #else
      #define M_MIL_UNICODE_API 0
   #endif
#endif

#if M_MIL_USE_LINUX || M_MIL_UNICODE_API
#define M_MIL_USE_TTF_UNICODE 1
#endif

#define MIL_DLLFUNC

/************************************************************************/
/*This flag enable or disable deprecated defines.                       */
/************************************************************************/
#if !defined(M_MIL_WARN_ON_DEPRECATED)
#if (defined(_MSC_VER) && (_MSC_VER >= 1400)) || defined(__INTEL_COMPILER)
#define M_MIL_WARN_ON_DEPRECATED 1
#else
#define M_MIL_WARN_ON_DEPRECATED 0
#endif
#endif

#if M_MIL_WARN_ON_DEPRECATED
   // Define true (1) or false (2) depends on MIL_COMPILE_VERSION and START_VERSION.
   #if MIL_COMPILE_VERSION >= 1000
   #define MIL_COMPILE_VERSION_HIGHER_OR_EQUAL_TO_1000 1
   #else
   #define MIL_COMPILE_VERSION_HIGHER_OR_EQUAL_TO_1000 0
   #endif
   #if MIL_COMPILE_VERSION >= 1010
   #define MIL_COMPILE_VERSION_HIGHER_OR_EQUAL_TO_1010 1
   #else
   #define MIL_COMPILE_VERSION_HIGHER_OR_EQUAL_TO_1010 0
   #endif
   #if MIL_COMPILE_VERSION >= 1020
   #define MIL_COMPILE_VERSION_HIGHER_OR_EQUAL_TO_1020 1
   #else
   #define MIL_COMPILE_VERSION_HIGHER_OR_EQUAL_TO_1020 0
   #endif
   #if MIL_COMPILE_VERSION >= 1030
   #define MIL_COMPILE_VERSION_HIGHER_OR_EQUAL_TO_1030 1
   #else
   #define MIL_COMPILE_VERSION_HIGHER_OR_EQUAL_TO_1030 0
   #endif
   #if MIL_COMPILE_VERSION >= 1040
   #define MIL_COMPILE_VERSION_HIGHER_OR_EQUAL_TO_1040 1
   #else
   #define MIL_COMPILE_VERSION_HIGHER_OR_EQUAL_TO_1040 0
   #endif
   #if MIL_COMPILE_VERSION >= 1100
   #define MIL_COMPILE_VERSION_HIGHER_OR_EQUAL_TO_1100 1
   #else
   #define MIL_COMPILE_VERSION_HIGHER_OR_EQUAL_TO_1100 0
   #endif

   #define PRIMITIVE_CAT(X, ...)                     X ## __VA_ARGS__
   #define MIL_COMPILE_VERSION_HIGHER_OR_EQUAL_TO(X) PRIMITIVE_CAT(MIL_COMPILE_VERSION_HIGHER_OR_EQUAL_TO_, X)
   #define IIF(X)                                    PRIMITIVE_CAT(IIF_, X)
   #define IIF_0(X, ...)                             __VA_ARGS__
   #define IIF_1(X, ...)                             X

   #define MIL_DEPRECATED(IDENTIFIER, START_VERSION) IIF(MIL_COMPILE_VERSION_HIGHER_OR_EQUAL_TO(START_VERSION)) (__pragma(deprecated(#IDENTIFIER)), /*nothing*/)
#else
   #define MIL_DEPRECATED(IDENTIFIER, START_VERSION)
#endif


#ifndef M_MIL_WARN_ON_DEPRECATED_MOS
   #if defined(OldDefinesSupport) && OldDefinesSupport
      #define M_MIL_WARN_ON_DEPRECATED_MOS 1
   #else
      #define M_MIL_WARN_ON_DEPRECATED_MOS 0
   #endif
#endif

/************************************************************************/
/* DEFAULT STATE INITIALIZATION FLAG                                    */
/************************************************************************/

//Defines for INSTALLDIR support
#define INSTALL_DIR MIL_TEXT("///INSTALLDIR///")
#define M_TEMP_DIR MIL_TEXT("///MILTEMPDIRDIR///")

#if   M_MIL_USE_LINUX
   #define M_IMAGE_PATH               INSTALL_DIR MIL_TEXT("images/")
   #define M_CONTEXT_PATH             INSTALL_DIR MIL_TEXT("contexts/")
#elif M_MIL_USE_CE
   #define M_IMAGE_PATH               MIL_TEXT("\\Userdisk\\mil\\images\\")
   #define M_CONTEXT_PATH             MIL_TEXT("\\Userdisk\\mil\\contexts\\")
#else
   #define M_IMAGE_PATH               INSTALL_DIR MIL_TEXT("images\\")
   #define M_CONTEXT_PATH             INSTALL_DIR MIL_TEXT("contexts\\")
#endif

/************************************************************************/
/* DEFAULT SYSTEM SPECIFICATIONS                                        */
/************************************************************************/

/*DO NOT EDIT: you should change the default values with the Mil Configuration utility*/
#if OldDefinesSupport
   #define M_DEF_SYSTEM_NUM           M_DEFAULT
   MIL_DEPRECATED(M_DEF_SYSTEM_NUM, 1000)
   #define M_SYSTEM_SETUP             M_DEF_SYSTEM_TYPE
   MIL_DEPRECATED(M_SYSTEM_SETUP, 1000)
   #define M_DEF_SYSTEM_TYPE          M_SYSTEM_DEFAULT
   MIL_DEPRECATED(M_DEF_SYSTEM_TYPE, 1000)
#endif

/************************************************************************/
/* DEFAULT DIGITIZER SPECIFICATIONS                                     */
/************************************************************************/

/*DO NOT EDIT: you should change the default values with the Mil Configuration utility*/
#if OldDefinesSupport
   #define M_DEF_DIGITIZER_INIT       M_DEFAULT
   MIL_DEPRECATED(M_DEF_DIGITIZER_INIT, 1000)
   #define M_DEF_DIGITIZER_NUM        M_DEFAULT
   MIL_DEPRECATED(M_DEF_DIGITIZER_NUM, 1000)
   #define M_CAMERA_SETUP             M_DEF_DIGITIZER_FORMAT
   MIL_DEPRECATED(M_CAMERA_SETUP, 1000)
   #define M_DEF_DIGITIZER_FORMAT     MIL_TEXT("M_DEFAULT")
   MIL_DEPRECATED(M_DEF_DIGITIZER_FORMAT, 1000)
#endif

/************************************************************************/
/* DEFAULT DISPLAY SPECIFICATIONS                                       */
/************************************************************************/

/*DO NOT EDIT: you should change the default values with the Mil Configuration utility*/
#if OldDefinesSupport
   #define M_DEF_DISPLAY_FORMAT               MIL_TEXT("M_DEFAULT")
   #define M_DISPLAY_SETUP                    M_DEF_DISPLAY_FORMAT
   MIL_DEPRECATED(M_DISPLAY_SETUP, 1000)
   MIL_DEPRECATED(M_DEF_DISPLAY_FORMAT, 1000)
   #define M_DEF_DISPLAY_INIT                 M_DEFAULT
   MIL_DEPRECATED(M_DEF_DISPLAY_INIT, 1000)
   #define M_DEF_DISPLAY_NUM                  M_DEFAULT
   MIL_DEPRECATED(M_DEF_DISPLAY_NUM, 1000)
#endif

/************************************************************************/
/* DEFAULT IMAGE BUFFER SPECIFICATIONS                                  */
/************************************************************************/
#if OldDefinesSupport
   #define M_DEF_IMAGE_NUMBANDS_MIN   1
   MIL_DEPRECATED(M_DEF_IMAGE_NUMBANDS_MIN, 1000)
   #define M_DEF_IMAGE_SIZE_X_MIN     50
   MIL_DEPRECATED(M_DEF_IMAGE_SIZE_X_MIN, 1000)
   #define M_DEF_IMAGE_SIZE_Y_MIN     50
   MIL_DEPRECATED(M_DEF_IMAGE_SIZE_Y_MIN, 1000)
   #define M_DEF_IMAGE_SIZE_X_MAX     4000000
   MIL_DEPRECATED(M_DEF_IMAGE_SIZE_X_MAX, 1000)
   #define M_DEF_IMAGE_SIZE_Y_MAX     4000000
   MIL_DEPRECATED(M_DEF_IMAGE_SIZE_Y_MAX, 1000)
   #define M_DEF_IMAGE_TYPE           (8+M_UNSIGNED)
   MIL_DEPRECATED(M_DEF_IMAGE_TYPE, 1000)
   #define M_DEF_IMAGE_ATTRIBUTE_MIN  (M_IMAGE+M_PROC)
   MIL_DEPRECATED(M_DEF_IMAGE_ATTRIBUTE_MIN, 1000)
#endif

/************************************************************************/
/* Enables DMIL monitoring auto-publish mode. 0 == Disabled             */
/************************************************************************/
#define M_DEF_AUTO_PUBLISH  0

/***************************************************************************/
/* LocalBufferAllocDefault - Local macro to allocate a default MIL buffer: */
/*                                                                         */
/* MIL_ID *SystemIdVarPtr;                                                 */
/* MIL_ID *DisplayIdVarPtr;                                                */
/* MIL_ID *ImageIdVarPtr;                                                  */
/*                                                                         */
/***************************************************************************/
#define LocalBufferAllocDefault(SystemIdVarPtr,DisplayIdVarPtr,DigitizerIdVarPtr,ImageIdVarPtr) \
{                                                                            \
  /* local variables */                                                      \
  MIL_INT   m_def_image_numbands = M_DEFAULT;                                \
  MIL_INT   m_def_image_size_x   = M_DEFAULT;                                \
  MIL_INT   m_def_image_size_y   = M_DEFAULT;                                \
  MIL_INT   m_def_image_type     = 8+M_UNSIGNED;                             \
  MIL_INT64 m_def_image_attribute= M_IMAGE+M_PROC;                           \
  MIL_ID*   LNullPtr             = M_NULL;                                   \
  MIL_ID    LNull                = M_NULL;                                   \
                                                                             \
  /* determines the needed size band, x, y and attribute */            \
  if ((((MIL_ID *)(DigitizerIdVarPtr))   != LNullPtr)   &&                   \
      ((*((MIL_ID *)(DigitizerIdVarPtr)))!= LNull))                          \
     {                                                                       \
     m_def_image_size_x =                                                    \
        MdigInquire(*((MIL_ID *)(DigitizerIdVarPtr)),M_SIZE_X,M_NULL);       \
     m_def_image_size_y =                                                    \
        MdigInquire(*((MIL_ID *)(DigitizerIdVarPtr)),M_SIZE_Y,M_NULL);       \
     m_def_image_attribute |= M_GRAB;                                        \
     m_def_image_numbands =                                                  \
        MdigInquire(*(MIL_ID *)DigitizerIdVarPtr,M_SIZE_BAND,M_NULL);        \
     }                                                                       \
                                                                             \
                                                                             \
  /* determines the needed attribute */                                      \
  if ((((MIL_ID *)(DisplayIdVarPtr))   != LNullPtr)   &&                     \
      ((*((MIL_ID *)(DisplayIdVarPtr)))!= LNull))                            \
     {                                                                       \
     m_def_image_attribute |= M_DISP;                                        \
     }                                                                       \
                                                                             \
  /* allocates a monochromatic or color image buffer */                      \
  MbufAllocColor(*((MIL_ID *)(SystemIdVarPtr)),                              \
                 m_def_image_numbands,                                       \
                 m_def_image_size_x,                                         \
                 m_def_image_size_y,                                         \
                 m_def_image_type,                                           \
                 m_def_image_attribute,                                      \
                 ((MIL_ID *)(ImageIdVarPtr)));                               \
                                                                             \
  /* clear and display the image buffer */                                   \
  if (((DisplayIdVarPtr) !=LNullPtr) &&                                      \
      ((*((MIL_ID *)(DisplayIdVarPtr)))!=LNull)  &&                          \
      ((*((MIL_ID *)(ImageIdVarPtr)))  !=LNull))                             \
     {                                                                       \
     MbufClear(*((MIL_ID *)(ImageIdVarPtr)), 0);                             \
     MdispSelect(*((MIL_ID *)(DisplayIdVarPtr)),                             \
                 *((MIL_ID *)(ImageIdVarPtr)));                              \
     }                                                                       \
}

/**************************************************************************/
/* MappAllocDefault - macro to allocate default MIL objects:              */
/*                                                                        */
/* MIL_UINT64 InitFlag;                                                   */
/* MIL_ID*    ContextAppIdPtr;                                            */
/* MIL_ID*    SystemIdVarPtr;                                             */
/* MIL_ID*    DisplayIdVarPtr;                                            */
/* MIL_ID*    DigitizerIdVarPtr;                                          */
/* MIL_ID*    ImageIdVarPtr;                                              */
/*                                                                        */
/* Note:                                                                  */
/*       An application must be allocated before a system.                */
/*       An system must be allocated before a display, digitizer or image.*/
/*                                                                        */
/**************************************************************************/
#define MappAllocDefault(InitFlag,ContextAppIdPtr,SystemIdVarPtr,DisplayIdVarPtr,DigitizerIdVarPtr,ImageIdVarPtr) \
{                                                                            \
  /* local variables */                                                      \
  MIL_ID *LNullPtr       = M_NULL;                                           \
  MIL_ID LNull           = M_NULL;                                           \
  MIL_INT DefAutoPublish = M_DEF_AUTO_PUBLISH;                               \
                                                                             \
  /*Make sure variables are initialized to M_NULL if a call fails*/          \
  if (((MIL_ID *)(ContextAppIdPtr)) != LNullPtr)                             \
            (*(MIL_ID *)(ContextAppIdPtr)) = M_NULL;                         \
  if (((MIL_ID *)(SystemIdVarPtr)) != LNullPtr)                              \
            (*(MIL_ID *)(SystemIdVarPtr)) = M_NULL;                          \
  if (((MIL_ID *)(DisplayIdVarPtr)) != LNullPtr)                             \
            (*(MIL_ID *)(DisplayIdVarPtr)) = M_NULL;                         \
  if (((MIL_ID *)(DigitizerIdVarPtr)) != LNullPtr)                           \
            (*(MIL_ID *)(DigitizerIdVarPtr)) = M_NULL;                       \
  if (((MIL_ID *)(ImageIdVarPtr)) != LNullPtr)                               \
            (*(MIL_ID *)(ImageIdVarPtr)) = M_NULL;                           \
  /* allocate a MIL application. */                                          \
  if (((MIL_ID *)(ContextAppIdPtr)) != LNullPtr)                             \
     MappAlloc(MIL_TEXT("M_DEFAULT"), InitFlag,((MIL_ID *)(ContextAppIdPtr)));\
                                                                             \
  /* Enables DMIL monitoring mode */                                         \
  if(DefAutoPublish != 0)                                                    \
     {                                                                       \
     MappControl(M_DEFAULT, M_DMIL_CONNECTION, M_DMIL_MONITOR);              \
     MappControl(M_DEFAULT, M_DMIL_AUTO_PUBLISH_ALL, M_ENABLE);              \
     }                                                                       \
                                                                             \
  /* allocate a system */                                                    \
  if ((((MIL_ID *)(SystemIdVarPtr))    != LNullPtr) &&                       \
      (((MIL_ID *)(ContextAppIdPtr))   != LNullPtr) &&                       \
      ((*((MIL_ID *)(ContextAppIdPtr)))!= LNull))                            \
     MsysAlloc(M_DEFAULT,                                                    \
               M_SYSTEM_DEFAULT,                                             \
               M_DEFAULT,                                                    \
               (InitFlag),                                                   \
               ((MIL_ID *)(SystemIdVarPtr)));                                \
                                                                             \
  /* allocate a display */                                                   \
  if ((((MIL_ID *)(DisplayIdVarPtr))   != LNullPtr) &&                       \
      (((MIL_ID *)(SystemIdVarPtr))    != LNullPtr) &&                       \
      ((*((MIL_ID *)(SystemIdVarPtr))) != LNull))                            \
     MdispAlloc(*((MIL_ID *)(SystemIdVarPtr)),                               \
               M_DEFAULT,                                                    \
               MIL_TEXT("M_DEFAULT"),                                        \
               M_DEFAULT,                                                    \
               ((MIL_ID *)(DisplayIdVarPtr)));                               \
                                                                             \
  /* allocate a digitizer */                                                 \
  if ((((MIL_ID *)(DigitizerIdVarPtr)) != LNullPtr) &&                       \
      (((MIL_ID *)(SystemIdVarPtr))    != LNullPtr) &&                       \
      ((*((MIL_ID *)(SystemIdVarPtr))) != LNull))                            \
     MdigAlloc(*((MIL_ID *)(SystemIdVarPtr)),                                \
               M_DEFAULT,                                                    \
               MIL_TEXT("M_DEFAULT"),                                        \
               M_DEFAULT,                                                    \
               ((MIL_ID *)(DigitizerIdVarPtr)));                             \
                                                                             \
                                                                             \
  /* allocate an image buffer */                                             \
  if ((((MIL_ID *)(ImageIdVarPtr))     != LNullPtr) &&                       \
      (((MIL_ID *)(SystemIdVarPtr))    != LNullPtr) &&                       \
      ((*((MIL_ID *)(SystemIdVarPtr))) != LNull))                            \
     LocalBufferAllocDefault(((MIL_ID *)(SystemIdVarPtr)),                   \
                             ((MIL_ID *)(DisplayIdVarPtr)),                  \
                             ((MIL_ID *)(DigitizerIdVarPtr)),                \
                             ((MIL_ID *)(ImageIdVarPtr)));                   \
                                                                             \
}


/************************************************************************/
/* MappFreeDefault - macro to free default MIL objects:                 */
/*                                                                      */
/* MIL_ID ContextAppId;                                                 */
/* MIL_ID SystemId;                                                     */
/* MIL_ID DisplayId;                                                    */
/* MIL_ID DigitizerId;                                                  */
/* MIL_ID ImageId;                                                      */
/*                                                                      */
/************************************************************************/
#define MappFreeDefault(ContextAppId,SystemId,DisplayId,DigitizerId,BufferId) \
{                                                                           \
  MIL_ID NullId = M_NULL;                                                   \
                                                                            \
  /* free the image buffer */                                               \
  if ((BufferId) != NullId)                                                 \
     {                                                                      \
     MbufFree((BufferId));                                                  \
     }                                                                      \
                                                                            \
  /* free digitizer */                                                      \
  if ((DigitizerId) != NullId)                                              \
     {                                                                      \
     MdigFree((DigitizerId));                                               \
     }                                                                      \
                                                                            \
  /* free the display */                                                    \
  if ((DisplayId) != NullId)                                                \
     {                                                                      \
     MdispFree((DisplayId));                                                \
     }                                                                      \
                                                                            \
  /* free the system */                                                     \
  if ((SystemId) != NullId)                                                 \
     {                                                                      \
     MsysFree((SystemId));                                                  \
     }                                                                      \
                                                                            \
  /* free the application */                                                \
  if ((ContextAppId) != NullId)                                             \
     {                                                                      \
     MappFree((ContextAppId));                                              \
     }                                                                      \
}

#endif /*__MILSETUP_H__*/
