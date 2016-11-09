/*****************************************************************************/
/* 
 * File name: mil3dcam_pfocus.cpp
 *
 * Synopsis: Implements routines to convert 3rd-party 3d cameras output format to
 *           a MIL format compatible with mil3dmap.dll
 *            
 */
#include "mil.h"
#include "mil3dcam_pfocus.h"         /* Prototypes for conversion routines.            */

/* Helper function prototypes. */
bool GetBufInfoAndValidate(MIL_ID MilBuf, MIL_BUFFER_INFO* ppBuf);

template <class T>
void DoConversion(const void* pVoidSrc , void*   pVoidPos, void*   pVoidInt,
                  MIL_INT     SrcOffset, MIL_INT SrcPitch, MIL_INT SizeLine);

/*****************************************************************************
 Conversion routine.
   MilPFocusImage    (in)  Image grabbed in measurement mode by the PhotonFocus camera.
   MilPositionImage  (out) Converted position image to pass to M3dmapAddScan().
   MilIntensityImage (out) Converted intensity image to pass to M3dmapAddScan().
   MilEncoderImage   (out) M_NULL for now.
   ConvParamsPtr     (in)  Structure containing conversion parameters.

 Returns CONVERSION_SUCCESS or CONVERSION_FAILURE.
*****************************************************************************/
MIL_INT MFTYPE M3dcamPFocusToMIL(MIL_ID MilPFocusImage,
                                 MIL_ID MilPositionImage,
                                 MIL_ID MilIntensityImage,
                                 MIL_ID MilEncoderImage,
                                 const PFocusParams* ConvParamsPtr)
   {
   /*****************************************************************************/
   /* Start of ParamCheck. */

   /* Validate conversion parameters. */
   if ( ConvParamsPtr->DataResolution !=  8 &&
        ConvParamsPtr->DataResolution != 10 &&
        ConvParamsPtr->DataResolution != 12 )
      return CONVERSION_FAILURE;

   if ( ConvParamsPtr->StatusLine != STATUS_LINE_ENABLED  && 
        ConvParamsPtr->StatusLine != STATUS_LINE_DISABLED )
      return CONVERSION_FAILURE;

   if (ConvParamsPtr->CycleTimeMicroseconds <= 0)
      return CONVERSION_FAILURE;

   /* Basic buffer checks. */
   MIL_BUFFER_INFO PFocusBuf, PositionBuf, IntensityBuf;
   if ( !GetBufInfoAndValidate(MilPFocusImage   , &PFocusBuf   ) )
      return CONVERSION_FAILURE;
   if ( !GetBufInfoAndValidate(MilPositionImage , &PositionBuf ) )
      return CONVERSION_FAILURE;
   if ( !GetBufInfoAndValidate(MilIntensityImage, &IntensityBuf) )
      return CONVERSION_FAILURE;

   /* Validate buffer SizeX, SizeY and SizeBit. */
   if (MfuncBufSizeX(PFocusBuf) < PFOCUS_3D_DATA_WIDTH)
      return CONVERSION_FAILURE;

   MIL_INT ExpectedSrcSizeY = MfuncBufSizeX(PositionBuf);
   if (ConvParamsPtr->StatusLine == STATUS_LINE_ENABLED)
      ExpectedSrcSizeY += 1;
   if (MfuncBufSizeY(PFocusBuf) != ExpectedSrcSizeY)
      return CONVERSION_FAILURE;

   if (ConvParamsPtr->DataResolution == 8)
      {
      if (MfuncBufSizeBit(PFocusBuf) != 8 || MfuncBufSizeBit(IntensityBuf) != 8)
         return CONVERSION_FAILURE;
      }
   else
      {
      if (MfuncBufSizeBit(PFocusBuf) != 16 || MfuncBufSizeBit(IntensityBuf) != 16)
         return CONVERSION_FAILURE;
      }

   if (MfuncBufSizeX(PositionBuf) != MfuncBufSizeX(IntensityBuf))
      return CONVERSION_FAILURE;

   if (MfuncBufSizeY(PositionBuf) != 1 || MfuncBufSizeY(IntensityBuf) != 1)
      return CONVERSION_FAILURE;

   if (MfuncBufSizeBit(PositionBuf) != 16)
      return CONVERSION_FAILURE;

   /* End of ParamCheck. */
   /*****************************************************************************/

   /* Lock buffers to ensure a valid host address. */
   MbufControl(MfuncBufId(PFocusBuf   ), M_LOCK+M_READ , M_DEFAULT);
   MbufControl(MfuncBufId(PositionBuf ), M_LOCK+M_WRITE, M_DEFAULT);
   MbufControl(MfuncBufId(IntensityBuf), M_LOCK+M_WRITE, M_DEFAULT);

   MIL_INT SizeLine = MfuncBufSizeX(PositionBuf);
   MIL_INT SrcPitch = MfuncBufPitch(PFocusBuf);

   /* Pointer to source image, will be cast later according to data resolution. */
   const void* pVoidSrc = (const void*) MfuncBufHostAddress(PFocusBuf);

   /* Position and intensity pointers, will be cast later according to data resolution. */
   void* pVoidPos = (void*) MfuncBufHostAddress(PositionBuf );
   void* pVoidInt = (void*) MfuncBufHostAddress(IntensityBuf);

   /* If there is no Host address, fail. */
   if (pVoidSrc == NULL || pVoidPos == NULL || pVoidInt == NULL)
      return CONVERSION_FAILURE;

   /* Align source pointer to start of 3d data, if necessary. 3d data is always located
      at 8 pixels from the end of the image. */
   MIL_INT SrcOffset = MfuncBufSizeX(PFocusBuf) - PFOCUS_3D_DATA_WIDTH;

   if (ConvParamsPtr->DataResolution == 8)
      {
      // Src pointer is a MIL_UINT8, so intensity map will be MIL_UINT8 too
      DoConversion<MIL_UINT8 >(pVoidSrc , pVoidPos, pVoidInt,
                               SrcOffset, SrcPitch, SizeLine);
      }
   else
      {
      // Src pointer is a MIL_UINT16, so intensity map will be MIL_UINT16 too
      DoConversion<MIL_UINT16>(pVoidSrc , pVoidPos, pVoidInt,
                               SrcOffset, SrcPitch, SizeLine);
      }

   /* Unlock buffers. */
   MbufControl(MfuncBufId(PFocusBuf   ), M_UNLOCK, M_DEFAULT);
   MbufControl(MfuncBufId(PositionBuf ), M_UNLOCK, M_DEFAULT);
   MbufControl(MfuncBufId(IntensityBuf), M_UNLOCK, M_DEFAULT);

   return CONVERSION_SUCCESS;
   }

/*****************************************************************************
 Performs the actual conversion, taking into account the data resolution as a
 template parameter. T will be MIL_UINT8 for 8-bits resolution and MIL_UINT16
 for 16-bits resolution.

   pVoidSrc       (in)  Pointer to start of input image (not necessarily 3d data).
   pVoidPos       (out) Pointer to start of output position  image.
   pVoidInt       (out) Pointer to start of output intensity image.
   SrcOffset      (in)  Number of pixels from pVoidSrc to start of 3d data.
   SrcPitch       (in)  Number of pixels (incl. padding) between lines (in input image).
   SizeLine       (in)  Width of output images, same as height of input image (maybe -1).

*****************************************************************************/
template <class T>
void DoConversion(const void* pVoidSrc , void*   pVoidPos, void*   pVoidInt,
                  MIL_INT     SrcOffset, MIL_INT SrcPitch, MIL_INT SizeLine)
   {
   /* Choose appropriate pointer types according to resolution. */
   const T   * pSrc = static_cast<const T   *>(pVoidSrc) + SrcOffset;
   MIL_UINT16* pPos = static_cast<MIL_UINT16*>(pVoidPos);
   T         * pInt = static_cast<T         *>(pVoidInt);

   for (MIL_INT i = 0; i < SizeLine; ++i)
      {
      if (pSrc[2] != 0) /* if width is not 0, there is a peak. */
         {
         pPos[i] = ( (static_cast<MIL_UINT16>(pSrc[0]) << 8) | pSrc[1] );
         pInt[i] = pSrc[3];
         }
      else
         {
         /* Missing data. */
         pPos[i] = MIL_UINT16_MAX;
         pInt[i] = 0;
         }
      pSrc += SrcPitch;
      }
   }

/*****************************************************************************
 Checks that the buffer is of the correct type, 1 band, 8- or 16-bits unsigned,
 uncompressed buffer. If so, gets the buffer information structure.

   MilBuf     (in)  Buffer to validate.
   ppBuf      (out) Pointer to buffer information structure handle.

 Returns true if successful, false otherwise.
*****************************************************************************/
bool GetBufInfoAndValidate(MIL_ID MilBuf, MIL_BUFFER_INFO* ppBuf)
   {
   if (MilBuf != M_NULL)
      {
      MIL_UINT64 ObjectType;
      MfuncInquire(MilBuf, M_OBJECT_TYPE_EXTENDED, &ObjectType);

      if (ObjectType != M_IMAGE)
         return false;

      MIL_BUFFER_INFO pBuf;
      MfuncInquire(MilBuf, M_BUFFER_INFO, &pBuf);

      MIL_INT ImgSizeBand = MfuncBufSizeBand(pBuf);
      MIL_INT ImgType = MfuncBufType(pBuf);
      MIL_INT ImgSign = ImgType & (M_SIGNED | M_UNSIGNED | M_FLOAT);
      MIL_INT ImgSizeBit = MfuncBufSizeBit(pBuf);
      MIL_INT64 ImgFormat = MfuncBufFormat(pBuf);

      if ( ImgSizeBand == 1                        &&
           ImgSign     == M_UNSIGNED               &&
           (ImgSizeBit == 8 || ImgSizeBit == 16)   &&
           (ImgFormat & M_COMPRESS) != M_COMPRESS  &&
           (ImgFormat & M_PACKED  ) != M_PACKED    &&
           (ImgFormat & M_FLIP    ) != M_FLIP      )
         {
         *ppBuf = pBuf;
         return true;
         }
      else
         {
         return false;
         }
      }
   else
      {
      return false;
      }
   }
