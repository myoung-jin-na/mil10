/************************************************************************/
/*
*
* Filename     :  mbasictypes.h
* Revision     :  10.00.2564
* Content      :  This file contains the definitions of basic types 
*                 for Matrox Imaging.
*
*
* Copyright © Matrox Electronic Systems Ltd., 1992-2014.
* All Rights Reserved
*************************************************************************/
#ifndef __MATROX_IMAGING_BASIC_TYPES__
#define __MATROX_IMAGING_BASIC_TYPES__     1


#if defined(_MSC_VER) && (_MSC_VER < 1310)
   typedef       unsigned __int64   M_UINT64;
#else
   typedef       unsigned long long M_UINT64;
#endif
   typedef       unsigned int       M_UINT32;
   typedef       unsigned short     M_UINT16;
   typedef       unsigned char      M_UINT8;

#if defined(_MSC_VER) && (_MSC_VER < 1310)
   typedef       __int64            M_INT64;
#else
   typedef       long long          M_INT64;
#endif
   typedef       int                M_INT32;
   typedef       short              M_INT16;
   typedef       char               M_INT8;

#endif 
