/*****************************************************************************/
/* 
 * File name: mil3dcam_sickrangere.h
 *
 * Synopsis: Declares prototypes to convert 3rd-party 3d cameras output format to
 *           a MIL format compatible with mil3dmap.dll
 *            
 */
#ifndef __MIL3DCAM_SICK_RANGER_E_H__
#define __MIL3DCAM_SICK_RANGER_E_H__

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
   /* General measurement information */
   MIL_INT MeasROISizeX;            /* X Size (in pixels) of ROI in measurement mode.     */
   MIL_INT MeasROIOffsetX;          /* X Offset (in pixels) of ROI in measurement mode.     */

   MIL_INT GrayStreamOffset;                /* Offset in bytes to start gray component        */ 
   MIL_INT HiResGrayStreamOffset;           /* Offset in bytes to start Hires gray component  */ 
   MIL_INT ColorStreamOffset;               /* Offset in bytes to start color component       */ 
   MIL_INT HiResColorStreamOffset;          /* Offset in bytes to start Hires color component */ 
   MIL_INT ScatterStreamOffset;             /* Offset in bytes to start scatter component     */ 
   
   MIL_INT ProfileDataSizeByte;     /* X Size (in bytes) of profiles returned in meas.    */   

   /* Laser component information */
   MIL_INT LaserROISizeY;
   MIL_INT LaserROIOffsetY;

   MIL_INT LaserMeasurementMode;         /* Indicates laser measurement mode.      */

   MIL_INT RangeSizeBit;                 /* SizeBit of input range component AND converted range.     */
   MIL_INT LaserIntensitySizeBit;        /* SizeBit of the intensity returned by conversion routine.  */
   MIL_INT LaserScatterSizeBit;          /* SizeBit of the scatter returned by the conversion routine */

   MIL_INT RangeStreamOffset;            /* Offset in bytes to start of Range data.        */
   MIL_INT LaserIntensityStreamOffset;   /* Offset in bytes to start of intensity data.        */
   MIL_INT LaserScatterStreamOffset;     /* Offset in bytes to start of scatter data           */

   MIL_INT EnableScatter;           /* Sets whether the scatter will be calculated in Hi3D*/

   MIL_INT FlipY;                   /* Must we flip the image to conform to 3dmap.        */

   MIL_INT FixedPoint;              /* Number of bits used for sub-pixeling (for 3dmap).  */

   /* Image component information */
   MIL_INT ImageROISizeX;           /* X Size (in pixels) of ROI in image mode.           */
   MIL_INT ImageROIOffsetX;         /* X Offset (in pixels) of ROI in image mode.         */
   MIL_INT ImageROISizeY;           /* Y Size (in pixels) of ROI in image mode.           */
   MIL_INT ImageROIOffsetY;         /* Y Offset (in pixels) of ROI in image mode.         */
   
   
   MIL_INT ROIOffsetX;              /* X Offset (in pixels) between image and laser meas ROIs.  */
   MIL_INT ROIOffsetY;              /* Y Offset (in pixels) between image and laser meas ROIs.  */
   MIL_INT CycleTimeMicroseconds;   /* Time in micro-seconds between two grabs in meas.   */

   // MISSING: Encoder parameters

   } RangerParams;

/* Laser measurement modes values. */
#define RANGER_HOR_THR_MODE             1
#define RANGER_HOR_MAX_MODE             2
#define RANGER_HOR_MAX_AND_THRES_MODE   3
#define M_RANGER_Hi3D_MODE                4
#define M_RANGER_Hi3D_COG_MODE            5

/* Other measurement modes */
#define M_RANGER_GRAY_MODE              0x01
#define M_RANGER_HIRES_GRAY_MODE        0x02
#define M_RANGER_COLOR_MODE             0x04
#define M_RANGER_HIRES_COLOR_MODE       0x08  
#define M_RANGER_SCATTER_MODE           0x10

/* Special value for stream offset when the measurement is not available */
#define NO_INFORMATION  -1

/* FlipY values. */
#define DO_NOT_FLIP_Y_AXIS     0
#define DO_FLIP_Y_AXIS         1

/* Enable scatter value */
#define M_SCATTER_DISABLE        0
#define M_SCATTER_ENABLE         1

/* Default fixed point for Hi3D COG mode. */
/* = 16 - log_2(MaxSizeY), where MaxSizeY = 512 for ranger E55. */
#define RANGER_E55_DEFAULT_COG_FIXED_POINT  7

/* Fixed point value for th Hi3D mode */
#define RANGER_E55_DEFAULT_HI3D_FIXED_POINT 4

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
                                 MIL_ID MilLaserPositionImage,
                                 MIL_ID MilLaserIntensityImage,
                                 MIL_ID MilLaserScatterImage,
                                 MIL_ID MilGrayImage,
                                 MIL_ID MilHiResGrayImage,
                                 MIL_ID MilColorImage,
                                 MIL_ID MilHiResColorImage,
                                 MIL_ID MilScatterImage,
                                 MIL_ID MilEncoderImage,
                                 const RangerParams* ConvParamsPtr);

#ifdef __cplusplus
   }
#endif

#endif
