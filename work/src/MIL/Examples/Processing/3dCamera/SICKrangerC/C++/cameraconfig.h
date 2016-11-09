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
 * or a rangerC camera. Images will be loaded from disk and camera configuration is
 * hard-coded in example. To try it with a camera, set this to 1. You will also probably 
 * need to replace some comments starting with CONVEYOR_CODE in SICKrangerC.cpp.
 */
#define USE_REAL_CAMERA       0

/* Choose the desired configuration for the camera (defines are below). */
#define RANGER_C55_PRM        RANGER_C55_HOR_THR_PRM
#define MEAS_COMP_NAME        HOR_THR_COMP_NAME

/* COM port used over CameraLink to communicate with camera. */
#define CAMERA_COM_PORT       "com3"

/* These are the two DCF files needed: one to grab in area-scan, one to grab in line-scan.*/
#define IMAGE_DCF             MIL_TEXT("sick_image_33.dcf")
#define MEAS_DCF              MIL_TEXT("sick_meas_33.dcf")

/*
 * Change the names below to match the component names in the XML file of the camera
 * configuration chosen (.prm file).
 */
#define CAMERA_NAME           "RangerC55"
#define IMAGE_MODE_NAME       "Image"
#define MEASUREMENT_MODE_NAME "Measurement"
#define IMAGE_COMP_NAME       "Image 1"

/* Strings to pass to FillParams() function according to camera mode. */

/* Camera parameter file. */
#define RANGER_C55_HOR_THR_PRM   "RangerC55_33_HorThr.prm"
#define RANGER_C55_HOR_MAX_PRM   "RangerC55_33_HorMax.prm"
#define RANGER_C55_Hi3D_PRM      "RangerC55_33_Hi3D.prm"

/* Measurement component name. */
#define HOR_THR_COMP_NAME        "HorThr 1"
#define HOR_MAX_COMP_NAME        "HorMax 1"
#define Hi3D_COMP_NAME           "Hi3D 1"

#endif