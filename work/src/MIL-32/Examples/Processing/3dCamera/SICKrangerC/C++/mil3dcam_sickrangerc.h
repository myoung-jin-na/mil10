/*****************************************************************************/
/* 
 * File name: mil3dcam.h
 *
 * Synopsis: Declares prototypes to convert 3rd-party 3d cameras output format to
 *           a MIL format compatible with mil3dmap.dll
 *            
 */
#ifndef __MIL3DCAM_SICK_RANGER_C_H__
#define __MIL3DCAM_SICK_RANGER_C_H__

#ifdef __cplusplus
extern "C"
   {
#endif

/*****************************************************************************
 Struct: RangerParams
         Options to specify when using a SICK Ranger camera.
*****************************************************************************/
typedef struct
   {
   MIL_INT MeasurementMode;         /* Indicates camera mode.                             */

   MIL_INT RangeStreamOffset;       /* Offset in bytes to start of range data.            */
   MIL_INT IntensityStreamOffset;   /* Offset in bytes to start of intensity data.        */
   
   MIL_INT RangeSizeBit;     /* SizeBit of input range component AND converted range.     */
   MIL_INT IntensitySizeBit; /* SizeBit of the intensity returned by conversion routine.  */

   MIL_INT ImageROISizeX;           /* X Size (in pixels) of ROI in image mode.           */
   MIL_INT ImageROISizeY;           /* Y Size (in pixels) of ROI in image mode.           */
   MIL_INT MeasROISizeX;            /* X Size (in pixels) of ROI in measurement mode.     */
   MIL_INT MeasROISizeY;            /* Y Size (in pixels) of ROI in measurement mode.     */

   MIL_INT ProfileDataSizeByte;     /* X Size (in bytes) of profiles returned in meas.    */

   MIL_INT FlipY;                   /* Must we flip the image to conform to 3dmap.        */

   MIL_INT FixedPoint;              /* Number of bits used for sub-pixeling (for 3dmap).  */

   MIL_INT ROIOffsetX;              /* X Offset (in pixels) between image and meas ROIs.  */
   MIL_INT ROIOffsetY;              /* Y Offset (in pixels) between image and meas ROIs.  */
   MIL_INT CycleTimeMicroseconds;   /* Time in micro-seconds between two grabs in meas.   */

   // MISSING: Encoder parameters

   } RangerParams;

/* Measurement modes values. */
#define RANGER_HOR_THR_MODE    1
#define RANGER_HOR_MAX_MODE    2
#define M_RANGER_Hi3D_MODE       3

/* Intensity stream offset special value when there is no intensity information. */
#define NO_INTENSITY_INFORMATION  -1

/* FlipY values. */
#define DO_NOT_FLIP_Y_AXIS     0
#define DO_FLIP_Y_AXIS         1

/* Default fixed point for Hi3D mode. */
/* = 16 - log_2(MaxSizeY), where MaxSizeY = 512 for ranger C55. */
#define RANGER_C55_DEFAULT_FIXED_POINT  7

/* Return values for conversion routine. */
#define CONVERSION_SUCCESS    0
#define CONVERSION_FAILURE    -1

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
                                 const RangerParams* ConvParamsPtr);

#ifdef __cplusplus
   }
#endif

#endif
