/*****************************************************************************/
/* 
 * File name: cameraconfig.h
 *
 * Synopsis: User-modifiable camera options
 *            
 */
#ifndef __CAMERA_CONFIG_H
#define __CAMERA_CONFIG_H

/*
 * Set this define to 0 to be able to run this example without a CameraLink frame-grabber
 * or a PhotonFocus camera. Images will be loaded from disk and camera configuration is
 * hard-coded in example. To try it with a camera, set this to 1. You will also probably 
 * need to replace some comments starting with CONVEYOR_CODE in pfocus.cpp.
 */
#define USE_REAL_CAMERA          0

/* These are three possible DCF files, according to sensor resolution. */
#define PFOCUS_8BITS_DCF         MIL_TEXT("pfocus8bits.dcf")
#define PFOCUS_10BITS_DCF        MIL_TEXT("pfocus10bits.dcf")
#define PFOCUS_12BITS_DCF        MIL_TEXT("pfocus12bits.dcf")

/* COM port used over CameraLink to communicate with camera. */
#define CAMERA_COM_PORT          2 /* 2 means com3. */

/* Desired ROI when grabbing full frames, e.g. for camera calibration. */
#define PFOCUS_IMAGE_OFFSET_X    0
#define PFOCUS_IMAGE_OFFSET_Y    0
#define PFOCUS_IMAGE_SIZE_X      1024
#define PFOCUS_IMAGE_SIZE_Y      1024

/* Desired ROI when extracting laser line, MROI not supported. */
#define PFOCUS_MEAS_OFFSET_X     0
#define PFOCUS_MEAS_OFFSET_Y     0
#define PFOCUS_MEAS_SIZE_X       1024
#define PFOCUS_MEAS_SIZE_Y       1024

#endif