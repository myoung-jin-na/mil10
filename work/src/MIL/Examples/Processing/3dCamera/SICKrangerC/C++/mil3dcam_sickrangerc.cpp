/*****************************************************************************/
/* 
 * File name: mil3dcam_sickrangerc.cpp
 *
 * Synopsis: Implements routines to convert 3rd-party 3d cameras output format to
 *           a MIL format compatible with mil3dmap.dll
 *            
 */
#include "mil.h"
#include "mil3dcam_sickrangerc.h"      /* Prototypes for conversion routines.            */

/* Helper function prototypes. */

template <bool IsYAxisFlipped>
MIL_INT DoHi3DToMIL(MIL_BUFFER_INFO RangerBuf,
                    MIL_BUFFER_INFO PositionBuf,
                    MIL_BUFFER_INFO IntensityBuf,
                    const RangerParams& rConvParams);

MIL_INT DoHorThrMaxToMIL(MIL_BUFFER_INFO RangerBuf,
                         MIL_BUFFER_INFO PositionBuf,
                         MIL_BUFFER_INFO IntensityBuf,
                         const RangerParams& rConvParams);

bool GetBufInfoAndValidate(MIL_ID MilBuf, MIL_BUFFER_INFO* ppBuf);

/*****************************************************************************
 Conversion routine.
   MilRangerImage    (in)  Image grabbed in measurement mode by the ranger camera.
   MilPositionImage  (out) Converted position image to pass to M3dmapAddScan().
   MilIntensityImage (out) Converted intensity image to pass to M3dmapAddScan().
   MilEncoderImage   (out) M_NULL for now.
   ConvParamsPtr     (in)  Structure containing conversion parameters.

 Returns CONVERSION_SUCCESS or CONVERSION_FAILURE.
*****************************************************************************/
MIL_INT MFTYPE M3dcamRangerToMIL(MIL_ID MilRangerImage,
                                 MIL_ID MilPositionImage,
                                 MIL_ID MilIntensityImage,
                                 MIL_ID MilEncoderImage,
                                 const RangerParams* ConvParamsPtr)
   {
   /*****************************************************************************/
   /* Start of ParamCheck. */

   /* Validate buffers. */

   /* Basic checks. */
   MIL_BUFFER_INFO RangerBuf, PositionBuf, IntensityBuf;
   if ( !GetBufInfoAndValidate(MilRangerImage  , &RangerBuf  ) )
      return CONVERSION_FAILURE;
   if ( !GetBufInfoAndValidate(MilPositionImage, &PositionBuf) )
      return CONVERSION_FAILURE;

   if (ConvParamsPtr->IntensitySizeBit == NO_INTENSITY_INFORMATION)
      {
      if (MilIntensityImage != M_NULL)
         return CONVERSION_FAILURE;
      IntensityBuf = NULL;
      }
   else
      {
      if ( !GetBufInfoAndValidate(MilIntensityImage, &IntensityBuf) )
         return CONVERSION_FAILURE;
      }

   /* Validate buffer SizeX, SizeY and SizeBit. */
   if (MfuncBufSizeX(RangerBuf) != ConvParamsPtr->ProfileDataSizeByte)
      return CONVERSION_FAILURE;

   if (MfuncBufSizeX(PositionBuf) != ConvParamsPtr->MeasROISizeX)
      return CONVERSION_FAILURE;

   if (MfuncBufSizeY(RangerBuf) != MfuncBufSizeY(PositionBuf))
      return CONVERSION_FAILURE;

   if (MfuncBufSizeBit(RangerBuf) != 8)
      return CONVERSION_FAILURE;

   if (MfuncBufSizeBit(PositionBuf) != ConvParamsPtr->RangeSizeBit)
      return CONVERSION_FAILURE;

   if (IntensityBuf != NULL)
      {
      if (MfuncBufSizeX(IntensityBuf) != ConvParamsPtr->MeasROISizeX)
         return CONVERSION_FAILURE;

      if (MfuncBufSizeY(RangerBuf) != MfuncBufSizeY(IntensityBuf))
         return CONVERSION_FAILURE;

      if (MfuncBufSizeBit(IntensityBuf) != ConvParamsPtr->IntensitySizeBit)
         return CONVERSION_FAILURE;
      }

   /* Validate conversion parameters according to mode. */
   if (ConvParamsPtr->RangeStreamOffset < 0)
      return CONVERSION_FAILURE;

   MIL_INT RangeWidth = ConvParamsPtr->MeasROISizeX * (ConvParamsPtr->RangeSizeBit >> 3);
   if (ConvParamsPtr->RangeStreamOffset+RangeWidth > ConvParamsPtr->ProfileDataSizeByte)
      return CONVERSION_FAILURE;

   if (ConvParamsPtr->IntensityStreamOffset != NO_INTENSITY_INFORMATION)
      {
      if (ConvParamsPtr->IntensityStreamOffset < 0)
         return CONVERSION_FAILURE;

      MIL_INT IntW = ConvParamsPtr->MeasROISizeX * (ConvParamsPtr->IntensitySizeBit >> 3);
      if (ConvParamsPtr->IntensityStreamOffset+IntW > ConvParamsPtr->ProfileDataSizeByte)
         return CONVERSION_FAILURE;
      }

   if ( ConvParamsPtr->RangeSizeBit != 8  &&
        ConvParamsPtr->RangeSizeBit != 16 )
      return CONVERSION_FAILURE;

   /* Conditions relative to SICK rangerC camera. */
   if ( ConvParamsPtr->ImageROISizeX < 256    ||
        ConvParamsPtr->ImageROISizeX > 1536   ||
        ConvParamsPtr->ImageROISizeX % 8 != 0 )
      return CONVERSION_FAILURE;

   if ( ConvParamsPtr->ImageROISizeY < 2   ||
        ConvParamsPtr->ImageROISizeY > 512 )
      return CONVERSION_FAILURE;

   if ( ConvParamsPtr->MeasROISizeX < 256    ||
        ConvParamsPtr->MeasROISizeX > 1536   ||
        ConvParamsPtr->MeasROISizeX % 8 != 0 )
      return CONVERSION_FAILURE;

   if ( ConvParamsPtr->MeasROISizeY < 16  ||
        ConvParamsPtr->MeasROISizeY > 512 )
      return CONVERSION_FAILURE;

   if (ConvParamsPtr->ProfileDataSizeByte <= 0)
      return CONVERSION_FAILURE;

   if ( ConvParamsPtr->FlipY != DO_FLIP_Y_AXIS     &&
        ConvParamsPtr->FlipY != DO_NOT_FLIP_Y_AXIS )
      return CONVERSION_FAILURE;

   if (ConvParamsPtr->CycleTimeMicroseconds <= 0)
      return CONVERSION_FAILURE;

   switch(ConvParamsPtr->MeasurementMode)
      {
      case RANGER_HOR_THR_MODE:
         {
         if ( ConvParamsPtr->IntensityStreamOffset != NO_INTENSITY_INFORMATION ||
              ConvParamsPtr->IntensitySizeBit      != NO_INTENSITY_INFORMATION )
            return CONVERSION_FAILURE;
         if ( ConvParamsPtr->FixedPoint != 1 &&
              ConvParamsPtr->FixedPoint != 2 )
            return CONVERSION_FAILURE;
         }
         break;
   
      case RANGER_HOR_MAX_MODE:
         {
         if (ConvParamsPtr->IntensitySizeBit != 8)
            return CONVERSION_FAILURE;
         if (ConvParamsPtr->FlipY != DO_FLIP_Y_AXIS)
            return CONVERSION_FAILURE;
         if (ConvParamsPtr->FixedPoint != 0)
            return CONVERSION_FAILURE;
         }
         break;
   
      case M_RANGER_Hi3D_MODE:
         {
         if (ConvParamsPtr->RangeSizeBit != 16)
            return CONVERSION_FAILURE;
         if (ConvParamsPtr->IntensitySizeBit != 16)
            return CONVERSION_FAILURE;
         if ( ConvParamsPtr->FixedPoint < 0 ||
              ConvParamsPtr->FixedPoint > 7 )
            return CONVERSION_FAILURE;
         }
         break;
   
      default:
         return CONVERSION_FAILURE;
      }

   /* End of ParamCheck. */
   /*****************************************************************************/

   /* Choose correct conversion routine according to rangerC mode. */
   if (ConvParamsPtr->MeasurementMode == M_RANGER_Hi3D_MODE)
      {
      if (ConvParamsPtr->FlipY == DO_FLIP_Y_AXIS)
         return DoHi3DToMIL<true >(RangerBuf, PositionBuf, IntensityBuf, *ConvParamsPtr);
      else
         return DoHi3DToMIL<false>(RangerBuf, PositionBuf, IntensityBuf, *ConvParamsPtr);
      }
   else
      {
      return DoHorThrMaxToMIL(RangerBuf, PositionBuf, IntensityBuf, *ConvParamsPtr);
      }
   }

/*****************************************************************************
 Converts profile data from SICK format in Hi3D mode to 3dmap format.

   RangerBuf    (in)  Image grabbed in measurement mode by the ranger camera.
   PositionBuf  (out) Converted position image to pass to M3dmapAddScan().
   IntensityBuf (out) Converted intensity image to pass to M3dmapAddScan().
   rConvParams  (in)  Structure containing conversion parameters.

   IsYAxisFlipped: indicates that returned positions are counted from the bottom (greater Y)
                   of the camera ROI instead from the top (lower Y).

 Returns CONVERSION_SUCCESS or CONVERSION_FAILURE.
*****************************************************************************/
template <bool IsYAxisFlipped>
MIL_INT DoHi3DToMIL(MIL_BUFFER_INFO RangerBuf,
                    MIL_BUFFER_INFO PositionBuf,
                    MIL_BUFFER_INFO IntensityBuf,
                    const RangerParams& rConvParams)
   {
   /* Lock buffers to ensure a valid host address. */
   MbufControl(MfuncBufId(RangerBuf   ), M_LOCK+M_READ , M_DEFAULT);
   MbufControl(MfuncBufId(PositionBuf ), M_LOCK+M_WRITE, M_DEFAULT);
   MbufControl(MfuncBufId(IntensityBuf), M_LOCK+M_WRITE, M_DEFAULT);

   MIL_INT SizeX = MfuncBufSizeX(PositionBuf); /* SizeX == measurement ROI width. */
   MIL_INT SizeY = MfuncBufSizeY(PositionBuf); /* SizeY == nb scanned lines.      */

   MIL_INT RangerPitch    = MfuncBufPitch(RangerBuf   );
   MIL_INT PositionPitch  = MfuncBufPitch(PositionBuf );
   MIL_INT IntensityPitch = MfuncBufPitch(IntensityBuf);

   /* SICK profile data buffer is initially 8-bits. */
   const MIL_UINT8* pRanger = (const MIL_UINT8*) MfuncBufHostAddress(RangerBuf);

   /* Position and intensity informations will be returned in 16 bits. */
   MIL_UINT16* pPos = (MIL_UINT16*) MfuncBufHostAddress(PositionBuf );
   MIL_UINT16* pInt = (MIL_UINT16*) MfuncBufHostAddress(IntensityBuf);

   /* If there is no Host address, fail in Hi3D mode. */
   if (pRanger == NULL || pPos == NULL || pInt == NULL)
      return CONVERSION_FAILURE;

   /* Find XSum and Sum subcomponents starting points. They are in 16-bits format. */
   const MIL_UINT16* pXSum =
      reinterpret_cast<const MIL_UINT16*>( &pRanger[rConvParams.RangeStreamOffset    ] );
   const MIL_UINT16* pSum  =
      reinterpret_cast<const MIL_UINT16*>( &pRanger[rConvParams.IntensityStreamOffset] );
   
   /* We'll move by multiple of 16 bits, not 8 bits as the original buffer. */
   RangerPitch /= 2;

   MIL_UINT32 LastY      = static_cast<MIL_UINT32>( rConvParams.MeasROISizeY - 1 );
   MIL_UINT32 FixedPoint = static_cast<MIL_UINT32>( rConvParams.FixedPoint       );

   /* For all pixels in the rangerC image... */
   for (MIL_INT y = 0; y < SizeY; ++y)
      {
      for (MIL_INT x = 0; x < SizeX; ++x)
         {
         /* Get XSum and Sum values. */
         MIL_UINT32 XSum = pXSum[x];
         MIL_UINT32  Sum =  pSum[x];

         /* Remove the 3 upper bits of Sum and append them to XSum. */
         XSum |= ((Sum & 0xE000) << 3);
         Sum &= 0x1FFF;

         /* 
          * Perform a horizontal flip at the same time, since SICK camera grabs
          * flipped images.
          */
         MIL_INT DstIdx = SizeX-1-x;

         if (Sum != 0)
            {
            /* There is valid data. */
            if (IsYAxisFlipped)
               {
               /*
                * Integer version of Round(  ( LastY - (XSum / Sum) ) << FixedPoint  ).
                * Warning: no protection against overflow in the subtraction below, will
                *          happen only if the camera itself has overflowed, so not
                *          important anyway (too late).
                */
               pPos[DstIdx] = static_cast<MIL_UINT16>( 
                     ( ((LastY*Sum - XSum) << FixedPoint) + (Sum >> 1) ) / Sum 
                  );
               }
            else
               {
               /* Integer version of Round(  (XSum / Sum) << FixedPoint  ). */
               pPos[DstIdx] = static_cast<MIL_UINT16>(
                     ( (      XSum         << FixedPoint) + (Sum >> 1) ) / Sum
                  );
               }
            }
         else
            {
            /* Data is missing. */
            pPos[DstIdx] = MIL_UINT16_MAX;
            }
         pInt[DstIdx] = static_cast<MIL_UINT16>( Sum );
         }

      /* Next line. */
      pXSum += RangerPitch;
      pSum  += RangerPitch;
      pPos  += PositionPitch;
      pInt  += IntensityPitch;
      }

   /* Unlock buffers. */
   MbufControl(MfuncBufId(RangerBuf   ), M_UNLOCK, M_DEFAULT);
   MbufControl(MfuncBufId(PositionBuf ), M_UNLOCK, M_DEFAULT);
   MbufControl(MfuncBufId(IntensityBuf), M_UNLOCK, M_DEFAULT);

   return CONVERSION_SUCCESS;
   }

/*****************************************************************************
 Converts profile data from SICK format in HorMax or HorThr mode to 3dmap format.

   RangerBuf    (in)  Image grabbed in measurement mode by the ranger camera.
   PositionBuf  (out) Converted position image to pass to M3dmapAddScan().
   IntensityBuf (out) Converted intensity image to pass to M3dmapAddScan().
   rConvParams  (in)  Structure containing conversion parameters.

 Returns CONVERSION_SUCCESS or CONVERSION_FAILURE.
*****************************************************************************/
MIL_INT DoHorThrMaxToMIL(MIL_BUFFER_INFO RangerBuf,
                         MIL_BUFFER_INFO PositionBuf,
                         MIL_BUFFER_INFO IntensityBuf,
                         const RangerParams& rConvParams)
   {
   MIL_ID MilPosition, MilIntensity;

   MIL_ID  MilDestPosition = MfuncBufId   (PositionBuf);
   MIL_INT SizeX           = MfuncBufSizeX(PositionBuf); /* SizeX == measurement ROI width.*/
   MIL_INT SizeY           = MfuncBufSizeY(PositionBuf); /* SizeY == nb scanned lines      */
   
   /* In 3dmap, missing data is represented as the maximum value of the buffer. */
   MIL_DOUBLE InvalidPos = static_cast<MIL_DOUBLE>( MfuncBufSizeBit(PositionBuf) == 8 ? MIL_UINT8_MAX : MIL_UINT16_MAX );
   
   MIL_INT RangerPitch = MfuncBufPitch(RangerBuf);
   MIL_UINT8* pRanger = (MIL_UINT8*)MfuncBufHostAddress(RangerBuf);

   /* Interprets the region of RangerBuf with range data as a buffer. */
   MbufCreate2d(M_DEFAULT_HOST, SizeX, SizeY, rConvParams.RangeSizeBit+M_UNSIGNED,
                M_IMAGE+M_PROC, M_HOST_ADDRESS+M_PITCH_BYTE, RangerPitch,
                pRanger+rConvParams.RangeStreamOffset, &MilPosition);
   
   if (rConvParams.FlipY == DO_FLIP_Y_AXIS)
      {
      /* Convert Y position upside down. */
      MIL_DOUBLE LastY = static_cast<MIL_DOUBLE>( (rConvParams.MeasROISizeY - 1) << rConvParams.FixedPoint );
      MimArith(LastY, MilPosition, MilDestPosition, M_CONST_SUB);

      /* Replace 0 (which has become LastY) by 3dmap missing data value. */
      MimClip(MilDestPosition, MilDestPosition, M_EQUAL, LastY, M_NULL, InvalidPos, M_NULL);
      }
   else
      {
      /* Replace 0 by 3dmap missing data value while copying. */
      MimClip(MilPosition, MilDestPosition, M_EQUAL, 0.0, M_NULL, InvalidPos, M_NULL);
      }

   /* 
    * Perform a horizontal flip at the same time, since SICK camera grabs
    * flipped images.
    */
   MimFlip(MilDestPosition, MilDestPosition, M_FLIP_HORIZONTAL, M_DEFAULT);
   MbufFree(MilPosition);

   /* If SICK camera provided intensity information, i.e. if not in HorThr mode... */
   if (rConvParams.IntensityStreamOffset != NO_INTENSITY_INFORMATION)
      {
      /* Interprets the region of RangerBuf with intensity data as a buffer and copy it. */
      MbufCreate2d(M_DEFAULT_HOST, SizeX, SizeY, 8+M_UNSIGNED, M_IMAGE+M_PROC,
                   M_HOST_ADDRESS+M_PITCH_BYTE, RangerPitch,
                   pRanger+rConvParams.IntensityStreamOffset, &MilIntensity);
      /* 
       * Perform a horizontal flip at the same time, since SICK camera grabs
       * flipped images.
       */
      MimFlip(MilIntensity, MfuncBufId(IntensityBuf), M_FLIP_HORIZONTAL, M_DEFAULT);
      MbufFree(MilIntensity);
      }

   return CONVERSION_SUCCESS;
   }

/*****************************************************************************
 Checks that the buffer is of the correct type, 1 band, 8- or 16-bits unsigned,
 uncompressed buffer. If so, gets the buffer information structure.

   MilBuf     (in)  Buffer to validate.
   ppBuf      (out) Pointer to buffer information structure handle.

 Returns true if succesful, false otherwise.
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