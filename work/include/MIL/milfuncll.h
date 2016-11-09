//MfuncBuf DTK functions

/***************************************************************************/
/*

    Filename:  MILFUNC.H
    Revision:  10.00.2564

    Content :  This file contains the prototypes for the Matrox
               Imaging Library (MIL) MfuncBuf DTK functions.

    Copyright © Matrox Electronic Systems Ltd., 1992-2014.
    All Rights Reserved

*/
/***************************************************************************/

#ifndef __MFUNCBUF_H
#define __MFUNCBUF_H

/* C++ directive if needed */
#ifdef __cplusplus
extern "C"
{
#endif

#ifndef __MILBUFFERINFO_H__
#if !M_MIL_USE_CPP_FUNC_BUF
typedef void* MIL_BUFFER_INFO;
MIL_DOUBLE MFTYPE MfuncBufMaxValue(MIL_BUFFER_INFO BufferInfoHandle);
MIL_DOUBLE MFTYPE MfuncBufMinValue(MIL_BUFFER_INFO BufferInfoHandle);
MIL_ID MFTYPE MfuncBufDriverId(MIL_BUFFER_INFO BufferInfoPtr);
void* MFTYPE MfuncBufHostAddress(MIL_BUFFER_INFO BufferInfoHandle);
void* MFTYPE MfuncBufHostAddressBand(MIL_BUFFER_INFO BufferInfoHandle, MIL_INT Band);
MIL_UINT64  MFTYPE MfuncBufPhysicalAddressBand(MIL_BUFFER_INFO BufferInfoHandle, MIL_INT Band);
MIL_UINT64 MFTYPE MfuncBufPhysicalAddress(MIL_BUFFER_INFO BufferInfoHandle);
MIL_INT MFTYPE MfuncBufSizeX(MIL_BUFFER_INFO BufferInfoHandle);
MIL_INT MFTYPE MfuncBufSizeY(MIL_BUFFER_INFO BufferInfoHandle);
MIL_INT MFTYPE MfuncBufSizeBand(MIL_BUFFER_INFO BufferInfoHandle);
MIL_INT MFTYPE MfuncBufSizeBit(MIL_BUFFER_INFO BufferInfoHandle);
MIL_INT MFTYPE MfuncBufType(MIL_BUFFER_INFO BufferInfoHandle);
MIL_INT MFTYPE MfuncBufPitch(MIL_BUFFER_INFO BufferInfoHandle);
MIL_INT MFTYPE MfuncBufPitchByte(MIL_BUFFER_INFO BufferInfoHandle);
MIL_INT64 MFTYPE MfuncBufAttribute(MIL_BUFFER_INFO BufferInfoHandle);
MIL_INT64 MFTYPE MfuncBufFormat(MIL_BUFFER_INFO BufferInfoHandle);
MIL_INT64 MFTYPE MfuncBufFlags(MIL_BUFFER_INFO BufInfoPtr);
MIL_INT MFTYPE MfuncBufLockable(MIL_BUFFER_INFO BufInfoPtr);
MIL_INT MFTYPE MfuncBufParentOffsetX(MIL_BUFFER_INFO BufferInfoHandle);
MIL_INT MFTYPE MfuncBufParentOffsetY(MIL_BUFFER_INFO BufferInfoHandle);
MIL_INT MFTYPE MfuncBufParentOffsetBand(MIL_BUFFER_INFO BufferInfoHandle);
MIL_INT MFTYPE MfuncBufAncestorOffsetX(MIL_BUFFER_INFO BufferInfoHandle);
MIL_INT MFTYPE MfuncBufAncestorOffsetY(MIL_BUFFER_INFO BufferInfoHandle);
MIL_INT MFTYPE MfuncBufAncestorOffsetBand(MIL_BUFFER_INFO BufferInfoHandle);
MIL_INT MFTYPE MfuncBufAncestorOffsetBit(MIL_BUFFER_INFO BufferInfoHandle);
MIL_INT MFTYPE MfuncBufNativeId(MIL_BUFFER_INFO BufInfoPtr);
MIL_ID MFTYPE MfuncBufId(MIL_BUFFER_INFO BufferInfoHandle);
MIL_ID MFTYPE MfuncBufHostId(MIL_BUFFER_INFO BufInfoPtr);
MIL_ID MFTYPE MfuncBufParentId(MIL_BUFFER_INFO BufferInfoHandle);
MIL_ID MFTYPE MfuncBufAncestorId(MIL_BUFFER_INFO BufferInfoHandle);
MIL_ID MFTYPE MfuncBufOwnerSystemId(MIL_BUFFER_INFO BufferInfoHandle);
MIL_INT MFTYPE MfuncBufOwnerSystemType(MIL_BUFFER_INFO BufferInfoHandle);
#endif 
#else
MIL_DOUBLE MFTYPE MfuncBufMaxValue(MIL_BUFFER_INFO BufferInfoHandle);
MIL_DOUBLE MFTYPE MfuncBufMinValue(MIL_BUFFER_INFO BufferInfoHandle);
MIL_ID MFTYPE MfuncBufDriverId(MIL_BUFFER_INFO BufferInfoPtr);
#endif 

#ifdef __cplusplus
}
#endif


#endif
