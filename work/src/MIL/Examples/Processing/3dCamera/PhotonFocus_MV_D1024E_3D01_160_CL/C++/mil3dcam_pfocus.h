/*****************************************************************************/
/* 
 * File name: mil3dcam_pfocus.h
 *
 * Synopsis: Declares prototypes to convert 3rd-party 3d cameras output format to
 *           a MIL format compatible with mil3dmap.dll
 *            
 */
#ifndef __MIL3DCAM_PFOCUS_H__
#define __MIL3DCAM_PFOCUS_H__

#ifdef __cplusplus
extern "C"
   {
#endif

/* Width, in pixels, of 3D data. */
#define PFOCUS_3D_DATA_WIDTH     8

/* 3D information is always returned in 16 bits. */
#define PFOCUS_DEPTH_SIZEBIT     16

/* 3D information always have 6 bits of sub-pixel accuracy. */
#define PFOCUS_FIXED_POINT       6

/*****************************************************************************
 Struct: PhotonFocusParams
         Options to specify when using a PhotonFocus 3D camera.
*****************************************************************************/
typedef struct
   {
   MIL_INT DataResolution;          /* 8, 10 or 12 bits.                                  */
   MIL_INT StatusLine;              /* Is the last line in the image replaced with status.*/
   MIL_INT CycleTimeMicroseconds;   /* Time in micro-seconds between two grabs.           */
   } PFocusParams;

/* Possible values for StatusLine. */
#define STATUS_LINE_ENABLED         1
#define STATUS_LINE_DISABLED        2

/* Return values for conversion routine. */
#define CONVERSION_SUCCESS          0
#define CONVERSION_FAILURE         -1

/*****************************************************************************
 Conversion routine.
   MilPFocusImage    (in)  Image grabbed in measurement mode by the PhotonFocus 3d camera.
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
                                 const PFocusParams* ConvParamsPtr);

#ifdef __cplusplus
   }
#endif

#endif
