/************************************************************************/
/*
*
* Filename     :  MILOS.H
* Revision     :  10.00.2564
* Content      :  This file contains the defines necessary to use the
*                 Matrox Imaging Library (MIL 4.0 and up) "C" user interface.
*                 under different operating systems
* Comments     :  This file has first become necessary to support the unicode
*                 strings under Windows CE.
*
* Copyright © Matrox Electronic Systems Ltd., 1992-2014.
* All Rights Reserved
*************************************************************************/

#ifndef __MILOS_H
#define __MILOS_H

/************************************************************************/
/* Note that this header file might be included as a stand alone version*/
/************************************************************************/
#ifndef M_MIL_USE_OS
#define M_MIL_USE_WINDOWS       1
#endif

#define        M_SEEK_SET  0
#define        M_SEEK_CUR  1
#define        M_SEEK_END  2

/************************************************************************/
/* Include float.h                                                      */
/************************************************************************/
#include <float.h>

/************************************************************************/
/* MIL TYPE DEFINITIONS                                                 */
/************************************************************************/
/* GENERAL RELATED DEFINES */
/***************************/
#if M_MIL_USE_WINDOWS
   #if M_MIL_USE_CE
      typedef  long           MIL_INT;
      typedef  unsigned long  MIL_UINT;
      #define  MIL_UINT_MIN   0
      #define  MIL_UINT_MAX   4294967295UL
      #define  MIL_INT_MIN    (-2147483647L-1L)
      #define  MIL_INT_MAX    2147483647L
   #else
      #if M_MIL_USE_64BIT
#if defined(_MSC_VER) && (_MSC_VER < 1310)
         typedef  __int64              MIL_INT;
         typedef  unsigned __int64     MIL_UINT;
#else
         typedef  long long            MIL_INT;
         typedef  unsigned long long   MIL_UINT;
#endif

         #define  MIL_UINT_MIN      0ULL
         #define  MIL_UINT_MAX      18446744073709551615ULL
         #define  MIL_INT_MIN       -9223372036854775808LL
         #define  MIL_INT_MAX       9223372036854775807LL
      #else
         #if (_MSC_VER < 1400) // Visual Studio 2003
            typedef  long           MIL_INT;
            typedef  unsigned long  MIL_UINT;
         #else                 // Visual Studio 2005
            // _W64 Lets you mark variables, such that when you compile
            // with /Wp64 the compiler will report any warnings that would
            // be reported if you were compiling with a 64-bit compiler.
            typedef _W64          long MIL_INT;
            typedef _W64 unsigned long MIL_UINT;
         #endif
         #define  MIL_UINT_MIN   0UL
         #define  MIL_UINT_MAX   4294967295UL
         #define  MIL_INT_MIN    (-2147483647L-1L)
         #define  MIL_INT_MAX    2147483647L
      #endif
   #endif
   typedef                long         MIL_INT32;
   typedef const          long         MIL_CINT32;
   typedef       unsigned long         MIL_UINT32;
   typedef const unsigned long         MIL_CUINT32;
   #define  MIL_UINT32_MIN             0UL
   #define  MIL_UINT32_MAX             4294967295UL
   #define  MIL_INT32_MIN              (-2147483647L-1L)
   #define  MIL_INT32_MAX              2147483647L
   #define  MIL_UINT32_MIN_FLT         0UL               //Min precise value in 32-bit floating point.
   #define  MIL_UINT32_MAX_FLT         0xFFFFFF00UL      //Max precise value in 32-bit floating point.
   #define  MIL_INT32_MIN_FLT          (-2147483647L-1L) //Min precise value in 32-bit floating point.
   #define  MIL_INT32_MAX_FLT          0x7FFFFF80L       //Max precise value in 32-bit floating point.

#if defined(_MSC_VER) && (_MSC_VER < 1310)
   typedef                __int64      MIL_INT64;
   typedef const          __int64      MIL_CINT64;
   typedef       unsigned __int64      MIL_UINT64;
   typedef const unsigned __int64      MIL_CUINT64;
#else
   typedef                long long    MIL_INT64;
   typedef const          long long    MIL_CINT64;
   typedef       unsigned long long    MIL_UINT64;
   typedef const unsigned long long    MIL_CUINT64;
#endif
   #define  MIL_UINT64_MIN             0ULL
   #define  MIL_UINT64_MAX             18446744073709551615ULL
   #define  MIL_INT64_MIN              -9223372036854775808LL
   #define  MIL_INT64_MAX              9223372036854775807LL
   #define  MIL_UINT64_MIN_DBL         0ULL                    //Min precise value in 64-bit floating point.
   #define  MIL_UINT64_MAX_DBL         0xFFFFFFFFFFFFF800ULL   //Max precise value in 64-bit floating point.
   #define  MIL_INT64_MIN_DBL          (-9223372036854775807LL - 1LL)  //Min precise value in 64-bit floating point.
   #define  MIL_INT64_MAX_DBL          0x7FFFFFFFFFFFFC00LL    //Max precise value in 64-bit floating point.

   typedef                double       MIL_DOUBLE;
   typedef                float        MIL_FLOAT;
   typedef                char         MIL_INT8;
   typedef const          char         MIL_CINT8;
   typedef       unsigned char         MIL_UINT8;
   typedef const unsigned char         MIL_CUINT8;
   typedef                short        MIL_INT16;
   typedef const          short        MIL_CINT16;
   typedef       unsigned short        MIL_UINT16;
   typedef const unsigned short        MIL_CUINT16;
   typedef MIL_INT32                   MIL_BOOL;
   typedef const MIL_INT32             MIL_CBOOL;

   #define  MIL_UINT8_MIN              0UL
   #define  MIL_UINT8_MAX              255UL
   #define  MIL_INT8_MIN               -128L
   #define  MIL_INT8_MAX               127L
   #define  MIL_UINT16_MIN             0UL
   #define  MIL_UINT16_MAX             65535UL
   #define  MIL_INT16_MIN              -32768L
   #define  MIL_INT16_MAX              32767L
   #define  MIL_DOUBLE_MIN             DBL_MIN
   #define  MIL_DOUBLE_MAX             DBL_MAX
   #define  MIL_FLOAT_MIN              FLT_MIN
   #define  MIL_FLOAT_MAX              FLT_MAX
   #define  MIL_BOOL_MIN               0UL
   #define  MIL_BOOL_MAX               1UL

#if M_MIL_USE_UNICODE && M_MIL_UNICODE_API
   typedef       wchar_t               MIL_WCHAR;
#else
   typedef       unsigned short        MIL_WCHAR;
#endif

#elif M_MIL_USE_LINUX
   typedef               long          MIL_INT;
   typedef      unsigned long          MIL_UINT;

   #if M_MIL_USE_64BIT
   typedef                int          MIL_INT32;
   typedef const          int          MIL_CINT32;
   typedef       unsigned int          MIL_UINT32;
   typedef const unsigned int          MIL_CUINT32;

   #define  MIL_UINT_MIN               MIL_UINT64_MIN
   #define  MIL_UINT_MAX               MIL_UINT64_MAX
   #define  MIL_INT_MIN                MIL_INT64_MIN
   #define  MIL_INT_MAX                MIL_INT64_MAX
   #else // M_MIL_USE_64BIT
   typedef                long         MIL_INT32;
   typedef const          long         MIL_CINT32;
   typedef       unsigned long         MIL_UINT32;
   typedef const unsigned long         MIL_CUINT32;

   #define  MIL_UINT_MIN               MIL_UINT32_MIN
   #define  MIL_UINT_MAX               MIL_UINT32_MAX
   #define  MIL_INT_MIN                MIL_INT32_MIN
   #define  MIL_INT_MAX                MIL_INT32_MAX
   #endif

   #define  MIL_UINT32_MIN             0
   #define  MIL_UINT32_MAX             4294967295U
   #define  MIL_INT32_MIN              (-MIL_INT32_MAX - 1) // -2147483648L does not yield the right value
   #define  MIL_INT32_MAX              2147483647
   #define  MIL_UINT32_MIN_FLT         0U                //Min precise value in 32-bit floating point.
   #define  MIL_UINT32_MAX_FLT         0xFFFFFF00U       //Max precise value in 32-bit floating point.
   #define  MIL_INT32_MIN_FLT          (-2147483647-1)   //Min precise value in 32-bit floating point.
   #define  MIL_INT32_MAX_FLT          0x7FFFFF80       //Max precise value in 32-bit floating point.

   #if M_MIL_USE_64BIT
      typedef                long       MIL_INT64;
      typedef const          long       MIL_CINT64;
      typedef       unsigned long       MIL_UINT64;
      typedef const unsigned long       MIL_CUINT64;
   #else
      typedef                long long MIL_INT64;
      typedef const          long long MIL_CINT64;
      typedef       unsigned long long MIL_UINT64;
      typedef const unsigned long long MIL_CUINT64;
   #endif
   #define  MIL_UINT64_MIN             0ULL
   #define  MIL_UINT64_MAX             18446744073709551615ULL
   #define  MIL_INT64_MAX              9223372036854775807LL
   #define  MIL_INT64_MIN              (-MIL_INT64_MAX - 1LL) // Using -9223372036854775808LL directly causes weird warnings.
   #define  MIL_UINT64_MIN_DBL         0ULL                    //Min precise value in 64-bit floating point.
   #define  MIL_UINT64_MAX_DBL         0xFFFFFFFFFFFFF800ULL   //Max precise value in 64-bit floating point.
   #define  MIL_INT64_MIN_DBL          (-9223372036854775807LL - 1LL)  //Min precise value in 64-bit floating point.
   #define  MIL_INT64_MAX_DBL          0x7FFFFFFFFFFFFC00LL    //Max precise value in 64-bit floating point.

   typedef                double       MIL_DOUBLE;
   typedef                float        MIL_FLOAT;
   typedef                char         MIL_INT8;
   typedef const          char         MIL_CINT8;
   typedef       unsigned char         MIL_UINT8;
   typedef const unsigned char         MIL_CUINT8;
   typedef                short        MIL_INT16;
   typedef const          short        MIL_CINT16;
   typedef       unsigned short        MIL_UINT16;
   typedef const unsigned short        MIL_CUINT16;
   typedef MIL_INT32                   MIL_BOOL;
   typedef const MIL_INT32             MIL_CBOOL;

   #define  MIL_UINT8_MIN              0UL
   #define  MIL_UINT8_MAX              255UL
   #define  MIL_INT8_MIN               -128L
   #define  MIL_INT8_MAX               127L
   #define  MIL_UINT16_MIN             0UL
   #define  MIL_UINT16_MAX             65535UL
   #define  MIL_INT16_MIN              -32768L
   #define  MIL_INT16_MAX              32767L
   #define  MIL_DOUBLE_MIN             DBL_MIN
   #define  MIL_DOUBLE_MAX             DBL_MAX
   #define  MIL_FLOAT_MIN              FLT_MIN
   #define  MIL_FLOAT_MAX              FLT_MAX
   #define  MIL_BOOL_MIN               0UL
   #define  MIL_BOOL_MAX               1UL

   typedef       unsigned short        MIL_WCHAR;

#else
   #error Operating system not supported in Milos.h!
#endif

typedef MIL_UINT64               MIL_DATA_PTR;

typedef MIL_DATA_PTR             MIL_PHYS_DATA_PTR;
typedef void*                    MIL_HOST_DATA_PTR;
typedef const void*              MIL_CONST_HOST_DATA_PTR;

typedef MIL_UINT                 MIL_ADDR;   // this type is used to cast a pointer to an integer type for arithmetic conversion
                                             // for example void *NewPtr = (void*)((MIL_ADDR)OldPtr + Offset);

typedef unsigned char            MIL_BYTE;

typedef MIL_UINT64               MIL_MTXDMA_HANDLE;

///////////////////////////////////////////////////////////////////////////
// Data pointer macro definitions                                        //
///////////////////////////////////////////////////////////////////////////
#define M_INT_DATA_PTR_SIZE   MIL_UINT64

#if M_MIL_USE_64BIT
#define M_CAST_MIL_HOST_PTR_TO_MIL_DATA_PTR(PTR)   ((MIL_DATA_PTR)(PTR))
#define M_CAST_MIL_DATA_PTR_TO_MIL_HOST_PTR(PTR)   ((MIL_HOST_DATA_PTR)(PTR))

#define M_CAST_MIL_HOST_PTR_TO_MIL_DATA_PTR_CONST(PTR) ((MIL_CONST_DATA_PTR)(PTR))
#define M_CAST_MIL_DATA_PTR_TO_MIL_HOST_PTR_CONST(PTR) ((MIL_CONST_HOST_DATA_PTR)(PTR))

#else
#define M_CAST_MIL_HOST_PTR_TO_MIL_DATA_PTR(PTR)     ((MIL_DATA_PTR)(PTR))
#define M_CAST_MIL_DATA_PTR_TO_MIL_HOST_PTR(PTR)     ((MIL_HOST_DATA_PTR)((MIL_ADDR)(PTR)))

#define M_CAST_MIL_HOST_PTR_TO_MIL_DATA_PTR_CONST(PTR)     ((MIL_CONST_DATA_PTR)(PTR))
#define M_CAST_MIL_DATA_PTR_TO_MIL_HOST_PTR_CONST(PTR)     ((MIL_CONST_HOST_DATA_PTR)((MIL_ADDR)(PTR)))
#endif

#define M_CAST_MIL_PHYS_PTR_TO_MIL_DATA_PTR(PTR)  (PTR)
#define M_CAST_MIL_DATA_PTR_TO_MIL_PHYS_PTR(PTR)  (PTR)

#define M_IS_PTR_OK_FOR_PLATFORM(X) ((sizeof(X) == 4) || (sizeof(X) == sizeof(void*)) || ((((MIL_INT64)X)&MAKE_INT64(0xFFFFFFFF00000000))==0))

typedef    MIL_UINT64 MIL_ADDRESS64;

#if M_MIL_USE_INT64_ID
   #define M_MILID_TO_DOUBLE(X) (MIL_DOUBLE)(X)
   #define M_MDID_TO_DOUBLE(X)  (MIL_DOUBLE)(X)
#else
   #define M_MILID_TO_DOUBLE(X) X
   #define M_MDID_TO_DOUBLE(X)  X
#endif


/***************************/
/* STRING RELATED DEFINES  */
/***************************/
#if M_MIL_USE_UNICODE && M_MIL_UNICODE_API
   #define MIL_TEXT(quote)        L##quote
   typedef wchar_t                MIL_TEXT_CHAR;
#elif M_MIL_USE_WINDOWS || M_MIL_USE_LINUX
   #define MIL_TEXT(quote)        quote
   typedef char                   MIL_TEXT_CHAR;
#else
   #error Operating system not supported in Milos.h!
#endif

#define MT                          MIL_TEXT
#if defined(OldDefinesSupport) && OldDefinesSupport 
   MIL_DEPRECATED(MT, 1000)      //Please use MIL_TEXT()
#endif
   
#define MIL_FUNC_NAME(Func)    Func
typedef const char*            MIL_FUNC_NAME_PTR;  // In Windows (not CE), the GetProcAddress function is not Unicode compliant

typedef MIL_TEXT_CHAR*               MIL_TEXT_PTR;
typedef const MIL_TEXT_CHAR*         MIL_CONST_TEXT_PTR;

/***************************************/
/* OLD DEFINES THAT SHOULD NOT BE USED */
/***************************************/
#if defined(OldDefinesSupport) && OldDefinesSupport

#define MIL_INTADDR  MIL_UINT             // Please, use MIL_UINT
MIL_DEPRECATED(MIL_INTADDR, 1000)
#define MIL_UINTADDR MIL_UINT             // Please, use MIL_UINT
MIL_DEPRECATED(MIL_UINTADDR, 1000)
#define MILTCHAR     MIL_TEXT_CHAR        // Please, use MIL_TEXT_CHAR
MIL_DEPRECATED(MILTCHAR, 1000)
#define LPMILSTR     MIL_TEXT_PTR         // Please, use MIL_TEXT_PTR
//MIL_DEPRECATED(LPMILSTR)
#define LPCMILSTR    MIL_CONST_TEXT_PTR   // Please, use MIL_CONST_TEXT_PTR
//MIL_DEPRECATED(LPCMILSTR)
#define BUFATTRTYPE  MIL_INT64            // Please, use MIL_INT64
MIL_DEPRECATED(BUFATTRTYPE, 1000)

#endif   // defined(OldDefinesSupport) && OldDefinesSupport


/************************************************************************/
/* Define UNREFERENCED_PARAMETER                                        */
/************************************************************************/
#ifndef UNREFERENCED_PARAMETER
#if M_MIL_USE_LINUX
#define UNREFERENCED_PARAMETER(P) (void)(P)
#else
#define UNREFERENCED_PARAMETER(P) (P)
#endif
#endif

/************************************************************************/
/* HANDLES RELATED DEFINITIONS                                          */
/************************************************************************/
#if   M_MIL_USE_WINDOWS
#if (defined(_INC_WINDOWS) || defined(_WINDOWS_) || defined(__WINDOWS_H) || defined(__WINDOWS__))
typedef HWND                   MIL_WINDOW_HANDLE;
typedef HDC                    MIL_DC_HANDLE;
#else
typedef void*                   MIL_WINDOW_HANDLE;
typedef void*                   MIL_DC_HANDLE;
#endif
#else
// Window is typedef-ed to XID, which is typedef-ed to
// unsigned long
typedef unsigned long          MIL_WINDOW_HANDLE;
typedef MIL_INT                MIL_DC_HANDLE;
#endif


/*************************************************************************/
/** Include string.h                                                     */
/*************************************************************************/
#if !M_LINUX_KERNEL
#include <string.h>
#endif

// Regular string functions' definition
#if (M_MIL_USE_UNICODE && M_MIL_UNICODE_API)
#if !M_WINDOWS_NT_KERNEL_MODE && !M_WINDOWS_CE_KERNEL_MODE
   #define     MOs_ltoa          _ltow
   #define     MOs_itoa          _itow
   #define     MOs_ultoa         _ultow
   #define     MOs_atoi          _wtoi
   #define     MOs_atol          _wtol
   #define     MOs_ANSIatol      atol
   #define     MOs_atof          _wtof
   #define     MOs_strtol        wcstol
   #define     MOs_strtoul       wcstoul
   #define     MOs_ANSIstrtoul   strtoul
   #define     MOs_strtoui64     _wcstoui64
   #define     MOs_strcpy        wcscpy
   #define     MOs_ANSIstrcpy    strcpy
   #define     MOs_strncpy       wcsncpy
   #define     MOs_ANSIstrncpy   strncpy
   #define     MOs_strcmp        wcscmp
   #define     MOs_ANSIstrcmp    strcmp
   #define     MOs_stricmp       _wcsicmp
   #define     MOs_ANSIstricmp   _stricmp
   #define     MOs_strncmp       wcsncmp
   #define     MOs_ANSIstrncmp   strncmp
   #define     MOs_ANSIstrnicmp  _strnicmp
   #define     MOs_strcat        wcscat
   #define     MOs_ANSIstrcat    strcat
   #define     MOs_strncat       wcsncat
   #define     MOs_strchr        wcschr
   #define     MOs_strrchr       wcsrchr
   #define     MOs_ANSIstrchr    strchr
   #if M_MIL_USE_64BIT
      #define  MOs_strlen       (MIL_UINT32)wcslen //size_t is an __int64 under Win64,
      #define  MOs_strnlen      (MIL_UINT32)wcsnlen //size_t is an __int64 under Win64,
   #else
      #define  MOs_strlen        wcslen
      #define  MOs_strnlen       wcsnlen
   #endif
   #if M_MIL_USE_64BIT
      #define  MOs_ANSIstrlen    (MIL_UINT32)strlen //size_t is an __int64 under Win64,
      #define  MOs_ANSIstrnlen   (MIL_UINT32)strnlen //size_t is an __int64 under Win64,
   #else
      #define  MOs_ANSIstrlen    strlen
      #define  MOs_ANSIstrnlen   strnlen
   #endif
   #define     MOs_sprintf       swprintf
   #define     MOs_ANSIsprintf   sprintf
   #define     MOs_vsprintf      vswprintf
   #define     MOs_sscanf        swscanf
   #define     MOs_ANSIsscanf    sscanf
   #define     MOs_fscanf        fwscanf
   #define     MOs_fprintf       fwprintf
   #define     MOs_ANSIfprintf   fprintf
   #define     MOs_ANSIvfprintf  vfprintf
   #define     MOs_vfprintf      vfwprintf
   #define     MOs_strtod        wcstod
   #define     MOs_strupr        _wcsupr
   #define     MOs_getchar       getwchar
   #define     MOs_ctime         _wctime
   #define     MOs_ftime         _ftime_s
   #define     MOs_localtime     localtime
   #define     MOs_strlwr        _wcslwr
   #define     MOs_printf        wprintf
   #define     MOs_vprintf       vwprintf
// #define     MOs_vscanf        vwscanf  NOT SUPPORTED ON ALL PLATFORMS
   #define     MOs_fputs         fputws
   #define     MOs_ANSIfputs     fputs
   #define     MOs_fputc         fputc
   #define     MOs_strtok        wcstok
   #define     MOs_ANSIstrtok    strtok
   #define     MOs_ungetc        ungetwc
   #define     MOs_tolower       towlower
   #define     MOs_ANSItolower   tolower
   #define     MOs_toupper       towupper
   #define     MOs_strspn        wcsspn
   #define     MOs_strcspn       wcscspn
   #define     MOs_ANSIstrcspn   strcspn
   #define     MOs_asctime       _wasctime
   #define     MOs_ANSIasctime   asctime
   #define     MOs_strstr        wcsstr
   #define     MOs_ANSIstrstr    strstr
   #define     MOs_splitpath     _wsplitpath
   #define     MOs_makepath      _wmakepath
   #define     MOs_strnset       _wcsnset
   #define     MOs_strpbrk       wcspbrk

#if M_MIL_USING_SAFE_CRT
   #define     MOs_sprintf_s              swprintf_s

   #define     MOs_ANSIsprintf_s          sprintf_s

   #define     MOs_printf_s               wprintf_s
   #define     MOs_scanf_s                wscanf_s
   #define     MOs_sscanf_s               swscanf_s
   #define     MOs_ANSIsscanf_s           sscanf_s
   #define     MOs_fscanf_s               fwscanf_s
   #define     MOs_strncpy_s              wcsncpy_s
   #define     MOs_ANSIstrncpy_s          strncpy_s
   #define     MOs_asctime_s              _wasctime_s
   #define     MOs_ANSIasctime_s          asctime_s
   #define     MOs_strcpy_s               wcscpy_s
   #define     MOs_ANSIstrcpy_s           strcpy_s
   #define     MOs_getenv_s               _wgetenv_s
   #define     MOs_ANSIgetenv_s           getenv_s
   #define     MOs_strcat_s               wcscat_s
   #define     MOs_ANSIstrcat_s           strcat_s
   #if M_MIL_USE_CE
       #define     MOs_fopen_s(A,B,C)         _wfopen_s(A,B,C)
       #define     MOs_ANSIfopen_s(A,B,C)     _fopen_s(A,B,C)
   #else
#include <share.h>
       #define     MOs_fopen_s(A,B,C)         ((((*A) =_wfsopen(B,C,_SH_DENYNO)) != NULL) ? (0) : (1))
       #define     MOs_ANSIfopen_s(A,B,C)     ((((*A) =_fsopen(B,C,_SH_DENYNO)) != NULL) ? (0) : (1))
   #endif
   #define     MOs_strncat_s              wcsncat_s
   #define     MOs_fcvt_s                 _fcvt_s
   #define     MOs_strtok_s               wcstok_s
   #define     MOs_ANSIstrtok_s           strtok_s
   #define     MOs_vsprintf_s             vswprintf_s
   #define     MOs_ANSIvsprintf_s         vsprintf_s
   #define     MOs_splitpath_s            _wsplitpath_s
   #define     MOs_makepath_s             _wmakepath_s
   #define     MOs_strnset_s              _wcsnset_s
   #define     MOs_strlwr_s               _wcslwr_s
   #define     MOs_strupr_s               _wcsupr_s
   #define     MOs_strnicmp               _wcsnicmp
   #define     MOs_ctime_s                _wctime_s
   #define     MOs_ftime_s                _ftime_s
   #define     MOs_localtime_s            localtime_s
   #define     MOs_mbstowcs_s             mbstowcs_s
   #define     MOs_wcstombs_s             wcstombs_s
#else    // Safe version of CRT map to unsafe version
   #define     MOs_sprintf_s              _snwprintf
   
   #define     MOs_ANSIsprintf_s          _snprintf

   #define     MOs_printf_s                        wprintf
   #define     MOs_sscanf_s                        swscanf
   #define     MOs_ANSIsscanf_s                    sscanf
   #define     MOs_fscanf_s                        fwscanf
   #define     MOs_strncpy_s(A, B, C, D)           wcsncpy(A, C, D)
   #define     MOs_ANSIstrncpy_s(A, B, C, D)       strncpy(A, C, D)
   #define     MOs_asctime_s                       _wasctime
   #define     MOs_ANSIasctime_s                   asctime
   #define     MOs_strcpy_s(A, B, C)               wcscpy(A, C)
   #define     MOs_ANSIstrcpy_s(A, B, C)           strcpy(A, C)
   #define     MOs_getenv_s(A, B, C, D)            (MOs_getenv(D)) ? wcscpy(B, MOs_getenv(D)) : wcscpy(B, MT("\0"))
   #define     MOs_ANSIgetenv_s(A, B, C, D)        (MOs_ANSIgetenv(D)) ? strcpy(B, MOs_ANSIgetenv(D)) : strcpy(B, "\0")
   #define     MOs_strcat_s(A, B, C)               wcscat(A, C)
   #define     MOs_ANSIstrcat_s(A, B, C)           strcat(A, C)
   #if M_MIL_USE_CE
      #define     MOs_fopen_s(A,B,C)               _wfopen_s(A,B,C)
      #define     MOs_ANSIfopen_s(A,B,C)           _fopen_s(A,B,C)
   #else
      #define     MOs_fopen_s(A,B,C)               (((*(A) =_wfsopen(B,C,_SH_DENYNO)) != NULL) ? (0) : (1))
      #define     MOs_ANSIfopen_s(A,B,C)           (((*(A) =_fsopen(B,C,_SH_DENYNO)) != NULL) ? (0) : (1))
   #endif
   #define     MOs_strncat_s(A, B, C, D)           wcsncat(A, C, D)
   #define     MOs_strtok_s(A, B, C)               wcstok(A, (C, B))
   #define     MOs_ANSIstrtok_s(A, B, C)           strtok(A, (C, B))
   #define     MOs_vsprintf_s(A, B, C, D)          vswprintf(A, C, D)
   #define     MOs_ANSIvsprintf_s(A, B, C, D)      vsprintf(A, C, D)
   #define     MOs_splitpath_s(A,B,C,D,E,F,G,H,I)  _wsplitpath(A,B,D,F,H)
   #define     MOs_makepath_s(A,B,C,D,E,F)         _wmakepath(A,C,D,E,F)
   #define     MOs_strnset_s(A,B,C,D)              _wcsnset(A,C,D)
   #define     MOs_strupr_s(A,B)                   _wcsupr(A)
   #define     MOs_strnicmp                        wcsnicmp
   #define     MOs_ctime_s(A,B,C)                  wcscpy(A, _wctime(C))
   #define     MOs_ftime_s                         _ftime
   #define     MOs_localtime_s(A, B)               A = localtime(B)
   #if M_MIL_USE_WINDOWS
      #define     MOs_strlwr_s(A, B)                  _wcslwr(A)
      #define     MOs_strupr_s(A, B)                  _wcsupr(A)
   #endif
   #define     MOs_mbstowcs_s(A,B,C,D,E)              (((*(A) =mbstowcs(B,D,E)) != NULL) ? (0) : (1))
   #define     MOs_wcstombs_s(A,B,C,D,E)              (((*(A) =wcstombs(B,D,E)) != NULL) ? (0) : (1))
#endif

   #if M_MIL_USE_CE
   //In CE, because of the memory allocation/deallocation, wcsdup must not be used.
      #ifdef __cplusplus
      inline wchar_t * MOs_strdup(const wchar_t * wcsSource)
      {
           wchar_t * wcsCopy = (wchar_t *)malloc(sizeof(wchar_t)*(wcslen(wcsSource) + 1));
           if(wcsCopy)
               MOs_strcpy_s(wcsCopy, wcslen(wcsSource) + 1, wcsSource);
           return wcsCopy;
      }
      #endif //__cplusplus
   #else
      #define MOs_strdup _wcsdup
   #endif //M_MIL_USE_CE

   #if (M_MIL_USE_UNICODE)
      #if (M_MIL_USING_SAFE_CRT)
         // Functions used by the Mosxxx functions (which are visible by MIL clients) must support both charset API's
         #define     MOs_strcpy_sA              strcpy_s
         #define     MOs_vsprintf_sA            vsprintf_s
         #define     MOs_strcat_sA              strcat_s
         #define     MOs_strlwr_sA              _strlwr_s
         #define     MOs_strupr_sA              _strupr_s
#if M_MIL_USE_CE
         #define     MOs_unlink(X)              (DeleteFile(X)?0:-1)
#else
         #define     MOs_unlink                 _wunlink
#endif

      #else
         #if M_MIL_USE_CE
            #define     MOs_strlwr_sA(A, B)     _strlwr(A)
            #define     MOs_strupr_sA(A, B)     _strupr(A)
         #else
            #define     MOs_strlwr_sA           strlwr
            #define     MOs_strupr_sA           strupr
         #endif   // M_MIL_USE_CE
         #define     MOs_strcpy_sA(A, B, C)      strcpy(A, C)
         #define     MOs_vsprintf_sA(A, B, C, D) vsprintf(A, C, D)
         #define     MOs_strcat_sA(A, B, C)      strcat(A, C)
         #define     MOs_strlwrA                strlwr
         #define     MOs_struprA                strupr
         #define     MOs_strcpyA                strcpy
         #define     MOs_vsprintfA              vsprintf
         #define     MOs_strcatA                strcat
         #define     MOs_strlwrA                strlwr
         #define     MOs_struprA                strupr
         #define     MOs_unlink                 wunlink
      #endif
         #define     MOs_vprintfA               vprintf
         #define     MOs_strcmpA                strcmp
   #endif

#endif // !M_WINDOWS_NT_KERNEL_MODE && !M_WINDOWS_CE_KERNEL_MODE
#elif (M_MIL_USE_WINDOWS)
#if !M_WINDOWS_NT_KERNEL_MODE && !M_WINDOWS_CE_KERNEL_MODE
   #define     MOs_strcat        strcat
   #define     MOs_ANSIstrcat    strcat
   #define     MOs_atoi          atoi
   #define     MOs_atol          atol
   #define     MOs_ANSIatol      atol
   #define     MOs_atof          atof
   #define     MOs_strtol        strtol
   #define     MOs_strtoul       strtoul
   #define     MOs_ANSIstrtoul   strtoul
   #define     MOs_strtoui64     _strtoui64
   #define     MOs_strcpy        strcpy
   #define     MOs_ANSIstrcpy    strcpy
   #define     MOs_strncpy       strncpy
   #define     MOs_ANSIstrncpy   strncpy
   #define     MOs_strcmp        strcmp
   #define     MOs_ANSIstrcmp    strcmp
   #define     MOs_strncmp       strncmp
   #define     MOs_ANSIstrncmp   strncmp
   #define     MOs_ANSIstrnicmp  _strnicmp
   #define     MOs_strncat       strncat
   #define     MOs_strchr        strchr
   #define     MOs_strrchr       strrchr
   #define     MOs_ANSIstrchr    strchr
   #define     MOs_sprintf       sprintf
   #define     MOs_ANSIsprintf   sprintf
   #define     MOs_vsprintf      vsprintf
   #define     MOs_strstr        strstr
   #define     MOs_ANSIstrstr    strstr
   #define     MOs_sscanf        sscanf
   #define     MOs_ANSIsscanf    sscanf
   #define     MOs_fscanf        fscanf
   #define     MOs_strtod        strtod
   #define     MOs_strlwr        strlwr
   #define     MOs_strtok        strtok
   #define     MOs_ANSIstrtok    strtok
   #define     MOs_ungetc        ungetc
   #define     MOs_tolower       tolower
   #define     MOs_ANSItolower   tolower
   #define     MOs_tcscpy        _tcscpy
   #define     MOs_tcslen        _tcslen
   #define     MOs_tcsncpy       _tcsncpy
   #define     MOs_toupper       toupper
   #define     MOs_strspn        strspn
   #define     MOs_strcspn       strcspn
   #define     MOs_ANSIstrcspn   strcspn
   #define     MOs_asctime       asctime
   #define     MOs_ANSIasctime   asctime
   #define     MOs_getchar       getchar
   #define     MOs_ctime         ctime
   #define     MOs_ftime         _ftime
   #define     MOs_localtime     localtime
   #define     MOs_splitpath     _splitpath
   #define     MOs_makepath      _makepath
   #define     MOs_strnset       _strnset
   #define     MOs_strpbrk       strpbrk

#if M_MIL_USING_SAFE_CRT
   #define     MOs_sprintf_s              sprintf_s
   
   #define     MOs_ANSIsprintf_s          sprintf_s

   #define     MOs_printf_s               printf_s
   #define     MOs_scanf_s                scanf_s
   #define     MOs_sscanf_s               sscanf_s
   #define     MOs_ANSIsscanf_s           sscanf_s
   #define     MOs_fscanf_s               fscanf_s
   #define     MOs_strncpy_s              strncpy_s
   #define     MOs_ANSIstrncpy_s          strncpy_s
   #define     MOs_asctime_s              asctime_s
   #define     MOs_ANSIasctime_s          asctime_s
   #define     MOs_ctime_s                ctime_s
   #define     MOs_ftime_s                _ftime_s
   #define     MOs_strcpy_s               strcpy_s
   #define     MOs_ANSIstrcpy_s           strcpy_s
   #define     MOs_getenv_s               getenv_s
   #define     MOs_ANSIgetenv_s           getenv_s
   #define     MOs_strcat_s               strcat_s
   #define     MOs_ANSIstrcat_s           strcat_s
   #if M_MIL_USE_CE
       #define     MOs_fopen_s(A,B,C)         _wfopen_s(A,B,C)
       #define     MOs_ANSIfopen_s(A,B,C)     _fopen_s(A,B,C)
   #else
       #define     MOs_fopen_s(A,B,C)         (((*(A) = _fsopen(B,C,_SH_DENYNO)) != NULL) ? (0) : (1))
       #define     MOs_ANSIfopen_s(A,B,C)     (((*(A) = _fsopen(B,C,_SH_DENYNO)) != NULL) ? (0) : (1))
   #endif
   #define     MOs_strncat_s              strncat_s
   #define     MOs_fcvt_s                 _fcvt_s
   #define     MOs_strtok_s               strtok_s
   #define     MOs_ANSIstrtok_s           strtok_s
   #define     MOs_vsprintf_s             vsprintf_s
   #define     MOs_ANSIvsprintf_s         vsprintf_s
   #define     MOs_splitpath_s            _splitpath_s
   #define     MOs_makepath_s             _makepath_s
   #define     MOs_strnset_s              _strnset_s
   #define     MOs_strlwr_s               _strlwr_s
   #define     MOs_strupr_s               _strupr_s
   #define     MOs_stricmp                _stricmp
   #define     MOs_ANSIstricmp            _stricmp
   #define     MOs_strnicmp               _strnicmp
   #define     MOs_unlink                 _unlink
   #define     MOs_localtime_s            localtime_s
   #define     MOs_mbstowcs_s             mbstowcs_s
   #define     MOs_wcstombs_s             wcstombs_s
#else
   #define     MOs_sprintf_s              _snprintf
   
   #define     MOs_ANSIsprintf_s          _snprintf

   #define     MOs_printf_s                        printf
   #define     MOs_sscanf_s                        sscanf
   #define     MOs_ANSIsscanf_s                    sscanf
   #define     MOs_fscanf_s                        fscanf
   #define     MOs_strncpy_s(A, B, C, D)           strncpy(A, C, D)
   #define     MOs_ANSIstrncpy_s(A, B, C, D)       strncpy(A, C, D)
   #define     MOs_asctime_s                       asctime_s
   #define     MOs_ANSIasctime_s                   asctime_s
   #define     MOs_ctime_s(A,B,C)                  strcpy((A), ctime(C))
   #define     MOs_ftime_s                         _ftime
   #define     MOs_strcpy_s(A, B, C)               strcpy(A, C)
   #define     MOs_ANSIstrcpy_s(A, B, C)           strcpy(A, C)
   #define     MOs_getenv_s(A, B, C, D)           (getenv(D)) ? strcpy(B, getenv(D)) : strcpy(B, "\0")
   #define     MOs_ANSIgetenv_s(A, B, C, D)       (getenv(D)) ? strcpy(B, getenv(D)) : strcpy(B, "\0")
   #define     MOs_strcat_s(A, B, C)               strcat(A, C)
   #define     MOs_ANSIstrcat_s(A, B, C)           strcat(A, C)
   #define     MOs_fopen_s(A, B, C)                (((*(A)=fopen(B, C)) != NULL) ? (0) : (1))
   #define     MOs_ANSIfopen_s(A, B, C)            (((*(A)=fopen(B, C)) != NULL) ? (0) : (1))
   #define     MOs_strncat_s(A, B, C, D)           strncat(A, C, D)
   #define     MOs_fcvt_s                          _fcvt
   #define     MOs_strtok_s(A, B, C)               strtok(A, (C, B))
   #define     MOs_ANSIstrtok_s(A, B, C)           strtok(A, (C, B))
   #define     MOs_vsprintf_s(A, B, C, D)          vsprintf(A, C, D)
   #define     MOs_ANSIvsprintf_s(A, B, C, D)      vsprintf(A, C, D)
   #define     MOs_splitpath_s(A,B,C,D,E,F,G,H,I)  _splitpath(A,B,D,F,H)
   #define     MOs_makepath_s(A,B,C,D,E,F)         _makepath(A,C,D,E,F)
   #define     MOs_strnset_s(A,B,C,D)              _strnset(A,C,D)
   #define     MOs_stricmp                         stricmp
   #define     MOs_ANSIstricmp                     stricmp
   #define     MOs_unlink                          unlink
   #define     MOs_localtime_s(A, B)               A = localtime(B)

   #define     MOs_strnicmp                        strnicmp

   #if M_MIL_USE_WINDOWS
      #define     MOs_strlwr_s(A, B)                  _strlwr(A)
      #define     MOs_strupr_s(A, B)                  _strupr(A)
   #endif
   #define     MOs_mbstowcs_s(A,B,C,D,E)              (((*(A) =mbstowcs(B,D,E)) != NULL) ? (0) : (1))
   #define     MOs_wcstombs_s(A,B,C,D,E)              (((*(A) =wcstombs(B,D,E)) != NULL) ? (0) : (1))
#endif

   #if M_MIL_USE_64BIT
      #define MOs_strlen       (MIL_UINT32)strlen //size_t is an __int64 under Win64,
   #else
      #define     MOs_strlen   strlen
   #endif
   #if M_MIL_USE_64BIT
      #define     MOs_ANSIstrlen    (MIL_UINT32)strlen //size_t is an __int64 under Win64,
   #else
      #define     MOs_ANSIstrlen    strlen
   #endif
   #define     MOs_ltoa          _ltoa
   #define     MOs_itoa          _itoa
   #define     MOs_ultoa         _ultoa
   #define     MOs_fprintf       fprintf
   #define     MOs_ANSIfprintf   fprintf
   #define     MOs_ANSIvfprintf  vfprintf
   #define     MOs_vfprintf      vfprintf
   #define     MOs_printf        printf
   #define     MOs_vprintf       vprintf
//      #define     MOs_vscanf      vscanf NOT SUPPORTED ON ALL PLATFORMS
   #if M_MIL_USING_SAFE_CRT
      #define     MOs_printf_s                        printf_s
   #else
      #define     MOs_printf_s                        printf
   #endif
   #if M_MIL_USE_WINDOWS
      #define     MOs_strdup   _strdup
      #define     MOs_strupr   strupr
   #else
      #define     MOs_strdup   strdup
      #define     MOs_strupr(Str) Str
   #endif

#endif // !M_WINDOWS_NT_KERNEL_MODE && !M_WINDOWS_CE_KERNEL_MODE
#elif M_MIL_USE_LINUX
#if !M_LINUX_KERNEL
   // forward declarations
   MIL_TEXT_PTR strupr( MIL_TEXT_PTR String1 );
   #ifdef __cplusplus
   extern "C"
   {
   #endif
   MIL_TEXT_PTR MilLinstrlwr(MIL_TEXT_PTR String1);
   #ifdef __cplusplus
   }
   #endif

//   #define     MOs_ltoa        Not implemented under Linux
//   #define     MOs_itoa        Not implemented under Linux
//   #define     MOs_ultoa       Not implemented under Linux
   #define     MOs_atoi          atoi
   #define     MOs_atol          atol
   #define     MOs_ANSIatol      atol
   #define     MOs_atof          atof
   #define     MOs_strtol        strtol
   #define     MOs_strtoul       strtoul
   #define     MOs_ANSIstrtoul   strtoul
   #define     MOs_strtoui64     strtoull
   #define     MOs_strcpy        strcpy
   #define     MOs_ANSIstrcpy    strcpy
   #define     MOs_strncpy       strncpy
   #define     MOs_ANSIstrncpy   strncpy
   #define     MOs_strcmp        strcmp
   #define     MOs_ANSIstrcmp    strcmp
   #define     MOs_stricmp       strcasecmp
   #define     MOs_ANSIstricmp   strcasecmp
   #define     MOs_strncmp       strncmp
   #define     MOs_ANSIstrncmp   strncmp
   #define     MOs_strnicmp      strncasecmp
   #define     MOs_ANSIstrnicmp  strncasecmp
   #define     MOs_strcat        strcat
   #define     MOs_ANSIstrcat    strcat
   #define     MOs_strncat       strncat
   #define     MOs_strchr        strchr
   #define     MOs_strrchr       strrchr
   #define     MOs_ANSIstrchr    strchr
   #define     MOs_strlen        strlen
   #define     MOs_ANSIstrlen    strlen
   #define     MOs_strnlen        strnlen
   #define     MOs_ANSIstrnlen    strnlen
   #define     MOs_fprintf       fprintf
   #define     MOs_ANSIfprintf   fprintf
   #define     MOs_ANSIvfprintf  vfprintf
   #define     MOs_vfprintf      vfprintf
   #define     MOs_printf        printf
   #define     MOs_printf_s      printf
   #define     MOs_vprintf       vprintf
//   #define     MOs_vscanf  vscanf NOT SUPPORTED ON ALL PLATFORMS
   #define     MOs_sprintf       sprintf
   #define     MOs_ANSIsprintf   sprintf
   #define     MOs_vsprintf      vsprintf
   #define     MOs_strstr        strstr
   #define     MOs_ANSIstrstr    strstr
   #define     MOs_sscanf        sscanf
   #define     MOs_ANSIsscanf    sscanf
   #define     MOs_fscanf        fscanf
   #define     MOs_strtod        strtod
   #define     MOs_strupr        strupr
   #define     MOs_strupr_s(A,B) strupr(A)
   #define     MOs_strdup        strdup
   #define     MOs_strlwr        MilLinstrlwr
   #define     MOs_strlwr_s(A,B) MilLinstrlwr(A)
   #define     MOs_strtok        strtok
   #define     MOs_ANSIstrtok    strtok
   #define     MOs_ungetc        ungetc
   #define     MOs_tolower       tolower
   #define     MOs_ANSItolower   tolower
   #define     MOs_tcscpy
   #define     MOs_tcslen
   #define     MOs_tcsncpy
   #define     MOs_toupper       toupper
   #define     MOs_strspn        strspn
   #define     MOs_strcspn       strcspn
   #define     MOs_ANSIstrcspn   strcspn
   #define     MOs_asctime       asctime
   #define     MOs_asctime_s(A,B,C) strcpy((A), asctime(C))
   #define     MOs_ANSIasctime   asctime
   #define     MOs_getchar       getchar
   #define     MOs_ctime         ctime
   #define     MOs_ftime         ftime
   #define     MOs_ctime_s(A,B,C) strcpy((A), ctime(C))
   #define     MOs_localtime     localtime
   #define     MOs_localtime_s(A,B) localtime_r(B, A)
//   #define     MOs_sscanf_s      Not implemented under Linux
//   #define     MOs_ANSIsscanf_s  Not implemented under Linux
//   #define     MOs_fscanf_s      Not implemented under Linux
//   #define     MOs_splitpath     Not implemented under Linux
//   #define     MOs_makepath      Not implemented under Linux
//   #define     MOs_strnset       Not implemented under Linux
   #define     MOs_strpbrk       strpbrk
   #define     MOs_sprintf_s                       snprintf
   
   #define     MOs_ANSIsprintf_s                   snprintf
  #if defined(_LARGE_FILES) || defined(_LARGEFILE64_SOURCE)
   #define     MOs_fopen_s(A, B, C)                (((*(A)=fopen64(B, C)) != NULL) ? (0) : (1))
   #define     MOs_ANSIfopen_s(A, B, C)            (((*(A)=fopen64(B, C)) != NULL) ? (0) : (1))
   #define     MOs_fseek                           fseeko64
  #else
   #define     MOs_fopen_s(A, B, C)                (((*(A)=fopen(B, C)) != NULL) ? (0) : (1))
   #define     MOs_ANSIfopen_s(A, B, C)            (((*(A)=fopen(B, C)) != NULL) ? (0) : (1))
   #define     MOs_fseek                           fseek
  #endif
   #define     MOs_getenv_s(A, B, C, D)                \
                  do {                                 \
                     char *tmp = getenv(D);            \
                     *(A) = (tmp) ? strlen(tmp)+1 : 0; \
                     if (B && tmp && (*(A) <= C))      \
                        strcpy(B, tmp);                \
                  } while(0)

   #define     MOs_ANSIgetenv_s(A, B, C, D)            \
                  do {                                 \
                     char *tmp = getenv(D);            \
                     *(A) = (tmp) ? strlen(tmp)+1 : 0; \
                     if (B && tmp && (*(A) <= C))      \
                        strcpy(B, tmp);                \
                  } while(0)

   #define     MOs_strcpy_s(A, B, C)               strncpy(A, C, B)
   #define     MOs_ANSIstrcpy_s(A, B, C)           strncpy(A, C, B)
   #define     MOs_strcat_s(A, B, C)               strncat(A, C, B)
   #define     MOs_ANSIstrcat_s(A, B, C)           strncat(A, C, B)
   #define     MOs_vsprintf_s(A, B, C, D)          vsnprintf(A, B, C, D)
   #define     MOs_ANSIvsprintf_s(A, B, C, D)      vsnprintf(A, B, C, D)
   #define     MOs_strtok_s(A, B, C)               strtok(A, (C, B))
   #define     MOs_ANSIstrtok_s(A, B, C)           strtok(A, (C, B))
   #define     MOs_unlink                          unlink
#endif // !M_LINUX_KERNEL

#else
   #error Operating system not supported in Milos.h!
#endif


/************************************************************************/
/* FILES RELATED DEFINITIONS                                            */
/************************************************************************/
#if !M_LINUX_KERNEL
#include <stdio.h>   /* required for definition of file */

#if M_MIL_USE_WINDOWS || !M_MIL_USE_TIFF_OPTIMISATION
   typedef FILE* MIL_FILE;
   #define MIL_EOF      EOF
#endif

#if (M_MIL_USE_UNICODE && M_MIL_UNICODE_API)
   #define     MOs_hypot   _hypot

   #define     MOs_getenv       _wgetenv
   #define     MOs_ANSIgetenv   getenv
#if M_MIL_USE_CE
   #define MOs_mkdir(Folder) (::CreateDirectory(Folder, NULL)? 0:-1)
   #define MOs_rmdir(Folder) (::RemoveDirectory(Folder)? 0:-1)
#else
   #define     MOs_mkdir     _wmkdir
   #define     MOs_rmdir     _wrmdir
#endif
   #define     MOs_chmod     _wchmod
   #define     MOs_fopen     _wfopen
   #define     MOs_ANSIfopen fopen
   #define     MOs_fclose    fclose
   #define     MOs_fwrite    fwrite
   #define     MOs_fread     fread
#if M_MIL_USE_64BIT
   #define     MOs_fseek    _fseeki64
#else
   #define     MOs_fseek    fseek
#endif
   #define     MOs_ftell       ftell
   #define     MOs_feof        feof
   #define     MOs_ferror      ferror
   #define     MOs_fflush      fflush
   #define     MOs_fgetc       fgetc
   #define     MOs_fgets       fgetws
   #define     MOs_ANSIfgets   fgets
   #define     MOs_clearerr    clearerr
   #define     MOs_remove(n)   (DeleteFile(n) ? 0 : 1)
   #define     MOs_rename(n,m) (MoveFile(n,m) ? 0 : -1)
   #define     MOs_copyfile(n,m,b) CopyFile(n,m,b)
#elif ((M_MIL_USE_WINDOWS) || (!M_MIL_USE_TIFF_OPTIMISATION)) && !M_MIL_USE_LINUX
   #if !M_MIL_USE_CE
     #include <direct.h>         //for _rmdir and _mkdir
     #include <io.h>             //for _chmod
   #endif

   #define     MOs_rmdir       _rmdir
   #define     MOs_mkdir       _mkdir
   #define     MOs_chmod       _chmod
   #define     MOs_fopen       fopen
   #define     MOs_ANSIfopen   fopen
   #define     MOs_fclose      fclose
   #define     MOs_fwrite      fwrite
   #define     MOs_fread       fread
#if M_MIL_USE_64BIT
   #define     MOs_fseek    _fseeki64
#else
   #define     MOs_fseek    fseek
#endif
   #define     MOs_ftell     ftell
   #define     MOs_feof      feof
   #define     MOs_ferror    ferror
   #define     MOs_fflush    fflush
   #define     MOs_fgetc     fgetc
   #define     MOs_fgets     fgets
   #define     MOs_ANSIfgets fgets
   #define     MOs_clearerr  clearerr
   #define     MOs_remove    remove
   #define     MOs_rename    rename
   #define     MOs_copyfile  CopyFile
#if _MSC_VER >= 1400
   #define     MOs_hypot   _hypot
#else
   #define     MOs_hypot   hypot
#endif
   #define     MOs_getenv      getenv
   #define     MOs_ANSIgetenv  getenv
   #define     MOs_putenv      putenv
   #define     MOs_fputs       fputs
   #define     MOs_fputc       fputc
#elif M_MIL_USE_LINUX
  #if defined(_LARGE_FILES) || defined(_LARGEFILE64_SOURCE)
   #define     MOs_fopen        fopen64
   #define     MOs_ANSIfopen    fopen64
   #define     MOs_fseek        fseeko64
   #define     MOs_ftell        ftello64
  #else
   #define     MOs_fopen        fopen
   #define     MOs_ANSIfopen    fopen
   #define     MOs_fseek        fseek
   #define     MOs_ftell        ftell
  #endif
   #define     MOs_mkdir(A)     mkdir(A, 0755)
   #define     MOs_rmdir        rmdir
   #define     MOs_chmod        chmod
   #define     MOs_fclose       fclose
   #define     MOs_fwrite       fwrite
   #define     MOs_fread        fread
   #define     MOs_feof         feof
   #define     MOs_ferror       ferror
   #define     MOs_fflush       fflush
   #define     MOs_fgetc        fgetc
   #define     MOs_fgets        fgets
   #define     MOs_ANSIfgets    fgets
   #define     MOs_ANSIfputs    fputs
   #define     MOs_fputs        fputs
   #define     MOs_clearerr     clearerr
   #define     MOs_remove       remove
   #define     MOs_rename       rename
   #define     MOs_copyfile     LinCopyFile
   #define     MOs_hypot        hypot
   #define     MOs_getenv       getenv
   #define     MOs_ANSIgetenv   getenv
   #define     MOs_putenv       putenv
   #define     MOs_fputs        fputs
   #define     MOs_fputc        fputc

#else
   #error Operating system not supported in Milos.h!
#endif

#endif // M_LINUX_KERNEL


/************************************************************************/
/* C++ EXCEPTIONS SUPPORT                                               */
/************************************************************************/

#if !(M_MIL_USE_CE)
   #define MOs_try                        try
   #define MOs_catch(x)                   catch(x)
   #define MOs_throw                      throw
#else
   #define MOs_try                        if (true)            // Execute try block
   #define MOs_catch(x)                   else if (false)      // Do not execute catch block
   #define MOs_throw                                           // Do nothing
#endif


// Macro usage are deprecated, please use MosXXX() functions if equivalent exist.
#if defined(M_MIL_WARN_ON_DEPRECATED_MOS) && M_MIL_WARN_ON_DEPRECATED_MOS
   MIL_DEPRECATED(MOs_ltoa, 1000)
   MIL_DEPRECATED(MOs_itoa, 1000)
   MIL_DEPRECATED(MOs_ultoa, 1000)
   MIL_DEPRECATED(MOs_atoi, 1000)
   MIL_DEPRECATED(MOs_atol, 1000)
   MIL_DEPRECATED(MOs_ANSIatol, 1000)
   MIL_DEPRECATED(MOs_atof, 1000)
   MIL_DEPRECATED(MOs_strtol, 1000)
   MIL_DEPRECATED(MOs_strtoul, 1000)
   MIL_DEPRECATED(MOs_ANSIstrtoul, 1000)
   MIL_DEPRECATED(MOs_strtoui64, 1000)
   MIL_DEPRECATED(MOs_strcpy, 1000)
   MIL_DEPRECATED(MOs_ANSIstrcpy, 1000)
   MIL_DEPRECATED(MOs_strncpy, 1000)
   MIL_DEPRECATED(MOs_ANSIstrncpy, 1000)
   MIL_DEPRECATED(MOs_strcmp, 1000)
   MIL_DEPRECATED(MOs_ANSIstrcmp, 1000)
   MIL_DEPRECATED(MOs_stricmp, 1000)
   MIL_DEPRECATED(MOs_ANSIstricmp, 1000)
   MIL_DEPRECATED(MOs_strncmp, 1000)
   MIL_DEPRECATED(MOs_ANSIstrncmp, 1000)
   MIL_DEPRECATED(MOs_ANSIstrnicmp, 1000)
   MIL_DEPRECATED(MOs_strcat, 1000)
   MIL_DEPRECATED(MOs_ANSIstrcat, 1000)
   MIL_DEPRECATED(MOs_strncat, 1000)
   MIL_DEPRECATED(MOs_strchr, 1000)
   MIL_DEPRECATED(MOs_strrchr, 1000)
   MIL_DEPRECATED(MOs_ANSIstrchr, 1000)
   MIL_DEPRECATED(MOs_strlen, 1000)
   MIL_DEPRECATED(MOs_strnlen, 1000)
   MIL_DEPRECATED(MOs_ANSIstrlen, 1000)
   MIL_DEPRECATED(MOs_ANSIstrnlen, 1000)
   MIL_DEPRECATED(MOs_sprintf, 1000)
   MIL_DEPRECATED(MOs_ANSIsprintf, 1000)
   MIL_DEPRECATED(MOs_vsprintf, 1000)
   MIL_DEPRECATED(MOs_sscanf, 1000)
   MIL_DEPRECATED(MOs_ANSIsscanf, 1000)
   MIL_DEPRECATED(MOs_fscanf, 1000)
   MIL_DEPRECATED(MOs_fprintf, 1000)
   MIL_DEPRECATED(MOs_ANSIfprintf, 1000)
   MIL_DEPRECATED(MOs_ANSIvfprintf, 1000)
   MIL_DEPRECATED(MOs_vfprintf, 1000)
   MIL_DEPRECATED(MOs_strtod, 1000)
   MIL_DEPRECATED(MOs_strupr, 1000)
   MIL_DEPRECATED(MOs_getchar, 1000)
   MIL_DEPRECATED(MOs_ctime, 1000)
   MIL_DEPRECATED(MOs_ftime, 1000)
   MIL_DEPRECATED(MOs_localtime, 1000)
   MIL_DEPRECATED(MOs_strlwr, 1000)
   MIL_DEPRECATED(MOs_printf, 1000)
   MIL_DEPRECATED(MOs_vprintf, 1000)
   MIL_DEPRECATED(MOs_fputs, 1000)
   MIL_DEPRECATED(MOs_ANSIfputs, 1000)
   MIL_DEPRECATED(MOs_fputc, 1000)
   MIL_DEPRECATED(MOs_strtok, 1000)
   MIL_DEPRECATED(MOs_ANSIstrtok, 1000)
   MIL_DEPRECATED(MOs_ungetc, 1000)
   MIL_DEPRECATED(MOs_tolower, 1000)
   MIL_DEPRECATED(MOs_ANSItolower, 1000)
   MIL_DEPRECATED(MOs_toupper, 1000)
   MIL_DEPRECATED(MOs_strspn, 1000)
   MIL_DEPRECATED(MOs_strcspn, 1000)
   MIL_DEPRECATED(MOs_ANSIstrcspn, 1000)
   MIL_DEPRECATED(MOs_asctime, 1000)
   MIL_DEPRECATED(MOs_ANSIasctime, 1000)
   MIL_DEPRECATED(MOs_strstr, 1000)
   MIL_DEPRECATED(MOs_ANSIstrstr, 1000)
   MIL_DEPRECATED(MOs_splitpath, 1000)
   MIL_DEPRECATED(MOs_makepath, 1000)
   MIL_DEPRECATED(MOs_strnset, 1000)
   MIL_DEPRECATED(MOs_strpbrk, 1000)
   MIL_DEPRECATED(MOs_sprintf_s, 1000)
   MIL_DEPRECATED(MOs_ANSIsprintf_s, 1000)
   MIL_DEPRECATED(MOs_printf_s, 1000)
   MIL_DEPRECATED(MOs_sscanf_s, 1000)
   MIL_DEPRECATED(MOs_ANSIsscanf_s, 1000)
   MIL_DEPRECATED(MOs_fscanf_s, 1000)
   MIL_DEPRECATED(MOs_strncpy_s, 1000)
   MIL_DEPRECATED(MOs_ANSIstrncpy_s, 1000)
   MIL_DEPRECATED(MOs_asctime_s, 1000)
   MIL_DEPRECATED(MOs_ANSIasctime_s, 1000)
   MIL_DEPRECATED(MOs_strcpy_s, 1000)
   MIL_DEPRECATED(MOs_ANSIstrcpy_s, 1000)
   MIL_DEPRECATED(MOs_getenv_s, 1000)
   MIL_DEPRECATED(MOs_ANSIgetenv_s, 1000)
   MIL_DEPRECATED(MOs_strcat_s, 1000)
   MIL_DEPRECATED(MOs_ANSIstrcat_s, 1000)
   MIL_DEPRECATED(MOs_fopen_s, 1000)
   MIL_DEPRECATED(MOs_ANSIfopen_s, 1000)
   MIL_DEPRECATED(MOs_strncat_s, 1000)
   MIL_DEPRECATED(MOs_fcvt_s, 1000)
   MIL_DEPRECATED(MOs_strtok_s, 1000)
   MIL_DEPRECATED(MOs_ANSIstrtok_s, 1000)
   MIL_DEPRECATED(MOs_vsprintf_s, 1000)
   MIL_DEPRECATED(MOs_ANSIvsprintf_s, 1000)
   MIL_DEPRECATED(MOs_splitpath_s, 1000)
   MIL_DEPRECATED(MOs_makepath_s, 1000)
   MIL_DEPRECATED(MOs_strnset_s, 1000)
   MIL_DEPRECATED(MOs_strlwr_s, 1000)
   MIL_DEPRECATED(MOs_strupr_s, 1000)
   MIL_DEPRECATED(MOs_strnicmp, 1000)
   MIL_DEPRECATED(MOs_ctime_s, 1000)
   MIL_DEPRECATED(MOs_ftime_s, 1000)
   MIL_DEPRECATED(MOs_localtime_s, 1000)
   MIL_DEPRECATED(MOs_mbstowcs_s, 1000)
   MIL_DEPRECATED(MOs_wcstombs_s, 1000)
   MIL_DEPRECATED(MOs_strdup, 1000)
   MIL_DEPRECATED(MOs_strlwr_sA, 1000)
   MIL_DEPRECATED(MOs_strupr_sA, 1000)
   MIL_DEPRECATED(MOs_strcpy_sA, 1000)
   MIL_DEPRECATED(MOs_vsprintf_sA, 1000)
   MIL_DEPRECATED(MOs_strcat_sA, 1000)
   MIL_DEPRECATED(MOs_strlwrA, 1000)
   MIL_DEPRECATED(MOs_struprA, 1000)
   MIL_DEPRECATED(MOs_strcpyA, 1000)
   MIL_DEPRECATED(MOs_vsprintfA, 1000)
   MIL_DEPRECATED(MOs_strcatA, 1000)
   MIL_DEPRECATED(MOs_strlwrA, 1000)
   MIL_DEPRECATED(MOs_struprA, 1000)
   MIL_DEPRECATED(MOs_unlink, 1000)
   MIL_DEPRECATED(MOs_vprintfA, 1000)
   MIL_DEPRECATED(MOs_strcmpA, 1000)
   MIL_DEPRECATED(MOs_hypot, 1000)
   MIL_DEPRECATED(MOs_getenv, 1000)
   MIL_DEPRECATED(MOs_ANSIgetenv, 1000)
   MIL_DEPRECATED(MOs_mkdir, 1000)
   MIL_DEPRECATED(MOs_rmdir, 1000)
   MIL_DEPRECATED(MOs_chmod, 1000)
   MIL_DEPRECATED(MOs_fopen, 1000)
   MIL_DEPRECATED(MOs_ANSIfopen, 1000)
   MIL_DEPRECATED(MOs_fclose, 1000)
   MIL_DEPRECATED(MOs_fwrite, 1000)
   MIL_DEPRECATED(MOs_fread, 1000)
   MIL_DEPRECATED(MOs_fseek, 1000)
   MIL_DEPRECATED(MOs_ftell, 1000)
   MIL_DEPRECATED(MOs_feof, 1000)
   MIL_DEPRECATED(MOs_ferror, 1000)
   MIL_DEPRECATED(MOs_fflush, 1000)
   MIL_DEPRECATED(MOs_fgetc, 1000)
   MIL_DEPRECATED(MOs_fgets, 1000)
   MIL_DEPRECATED(MOs_ANSIfgets, 1000)
   MIL_DEPRECATED(MOs_clearerr, 1000)
   MIL_DEPRECATED(MOs_remove, 1000)
   MIL_DEPRECATED(MOs_rename, 1000)
   MIL_DEPRECATED(MOs_copyfile, 1000)
   MIL_DEPRECATED(MOs_putenv, 1000)
   MIL_DEPRECATED(MOs_try, 1000)
   MIL_DEPRECATED(MOs_catch, 1000)
   MIL_DEPRECATED(MOs_throw, 1000)
#endif

/************************************************************************/
/* SAFETYPE FUNCTIONS ON/OFF                                            */
/************************************************************************/

/* 
The SafeType functions overload the C functions that receive a void*
argument.

A C++ overload is defined for each possible type of pointer that can be
received. The type of data received is compared with the expected data
type according to MIL documentation.

Two types of FALSE POSITIVE errors can occur:
1. Compile time error 

   If a wrapper has been build around a MIL function and the MIL
   function is called via a void*, for ex:
      void MyBufInquire(MIL_ID Id, MIL_INT64 InquireType, void *Ptr) 
         {
         MbufInquire(Id, InquireType, Ptr);
         }

   If the following syntax is used to access an array on the stack:
      {
      double AllScores[MAX_NUM_OCCURENCES];
      MmodGetResult(ResultId, M_ALL, M_SCORE+M_TYPE_DOUBLE, &AllScores) ;
      }
   The only thing to do is to remove the " & ".

   In these situations, the error will be
   "none of the ?? overloads could convert all the argument types"

2. Run time error: the inline function SafeTypeError is called.

   If the application voluntarily passes a pointer of the wrong 
   type of data, for example:
      {
      char *Ptr = new char[sizeof(double)];
      MmodGetResult(ResultId, 0, M_SCORE+M_TYPE_DOUBLE, Ptr);
      double Score = (double*)Ptr;
      }
   It is possible to remove this error by casting Ptr to the correct
   type if available.

TO DEACTIVATE THE SAFE TYPE OVERLOADS AND GET RID OF COMPILE-TIME
OR RUN-TIME ERRORS:

1. Deactivate globally by using
   #define M_MIL_USE_SAFE_TYPE 0
   before including mil.h

OR

2. Deactivate locally by adding "Unsafe" as a suffix to the 
   function name.

*/

/* Safe type is activated by default in C++ and in Debug.
   It is deactivated in Release or in C. */
#ifndef M_MIL_USE_SAFE_TYPE
   #if (!__cplusplus)
      #define M_MIL_USE_SAFE_TYPE 0 /* In C, there is no multiple overload of a function. */
   #elif M_MIL_USE_LINUX
      #define M_MIL_USE_SAFE_TYPE 0 /* PPC compiler produce an error when passing NULL to a void* and try to discriminate between pointer types. */
   #elif M_WINDOWS_NT_KERNEL_MODE || M_WINDOWS_CE_KERNEL_MODE
      #define M_MIL_USE_SAFE_TYPE 0 /* Driver compilation defines double to MIL_INT64, causing ambiguous calls. */
   #elif defined(NDEBUG)
      #define M_MIL_USE_SAFE_TYPE 0 /* Disable SafeType in Release to avoid added overhead. */
   #else
      #define M_MIL_USE_SAFE_TYPE 1 /* Enable SafeType in Debug. */
   #endif
#endif

#if M_MIL_USE_SAFE_TYPE && !__cplusplus
#error M_MIL_USE_SAFE_TYPE can only be activated with C++ compilation.
#endif

// Even if the MIL documentation does not officially support pointers to "unsigned"
// integers, many applications do it anyway without problems. We support it here to
// avoid too many false errors.
#ifndef M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED
#define M_MIL_SAFE_TYPE_SUPPORTS_UNSIGNED 1
#endif

// It would be safer to ensure that when asking for pointers we only fill 
// double pointers (ex: void**), but too much code exists that probably fills a 
// pointer into an integer of the correct type.
#ifndef M_MIL_SAFE_TYPE_STRONG_POINTER_CHECK
#define M_MIL_SAFE_TYPE_STRONG_POINTER_CHECK 0
#endif

#if M_MIL_SAFE_TYPE_STRONG_POINTER_CHECK
   // An integer will not be accepted to hold pointers.
   #define M_SAFE_TYPE_CHECK_PTR   M_TYPE_PTR
#else
   // An integer of the correct size will be accepted to hold pointers.
   #if M_MIL_USE_64BIT
      #define M_SAFE_TYPE_CHECK_PTR   M_TYPE_MIL_INT64
   #else
      #define M_SAFE_TYPE_CHECK_PTR   M_TYPE_MIL_INT32
   #endif
#endif

// With VisualStudio compilers, we may have to add the definition of pointers to 
// wchar_t since it is different from unsigned short
#if (M_MIL_USE_UNICODE || M_MIL_USE_CE) && defined(_NATIVE_WCHAR_T_DEFINED)
#define M_MIL_SAFE_TYPE_ADD_WCHAR_T 1
#endif

#endif /* #ifndef __MILOS_H */
