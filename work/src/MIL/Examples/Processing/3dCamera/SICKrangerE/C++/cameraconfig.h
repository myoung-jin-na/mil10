/*****************************************************************************/
// 
// File name: cameraconfig.h
//
// Synopsis: User-modifiable camera options
//            
//
#ifndef __CAMERA_CONFIG_H
#define __CAMERA_CONFIG_H


// Set this define to 0 to be able to run this example without a rangerE color camera.
// Images will be loaded from disk and camera configuration is hard-coded in example.
// To try it with a camera, set this to 1. You will also probably 
// need to replace some comments starting with CONVEYOR_CODE in SICKrangerE.cpp.

#define USE_REAL_CAMERA       0

// Choose the desired configuration for the camera (defines are below).
#define RANGER_E55_PRM        RANGER_E55_Hi3D_COLOR_PRM
#define MEAS_COMP_NAME        Hi3D_COMP_NAME

// Camera IP used to communicate with the camera.
#define CAMERA_IP       "192.168.0.12"

/*
 * Change the names below to match the component names in the XML file of the camera
 * configuration chosen (.prm file).
 */
#define CAMERA_NAME           "RangerE55"
#define IMAGE_MODE_NAME       "Image"
#define MEASUREMENT_MODE_NAME "Measurement"
#define IMAGE_COMP_NAME       "Image 1"

/* Strings to pass to FillParams() function according to camera mode. */

/* Camera parameter file. */
#define RANGER_E55_HOR_THR_PRM             "../Ranger E HorThr.prm"
#define RANGER_E55_HOR_MAX_THR_PRM         "../Ranger E HorMaxThr.prm"
#define RANGER_E55_Hi3D_PRM                "../Ranger E Hi3D.prm"
#define RANGER_E55_Hi3D_COLOR_PRM          "../Ranger E Hi3D Multi.prm"

/* Measurement component name. */
#define HOR_THR_COMP_NAME        "HorThr 1"
#define HOR_MAX_COMP_NAME        "HorMax 1"
#define HOR_MAX_THR_COMP_NAME    "HorMaxThr 1"
#define Hi3D_COMP_NAME           "Hi3D 1"
#define Hi3D_COG_COMP_NAME       "Hi3D COG"

#define GRAY_COMP_NAME           "Gray 1"
#define HIRES_GRAY_COMP_NAME     "HiresGray 1"
#define COLOR_COMP_NAME          "Color 1"
#define HIRES_COLOR_COMP_NAME    "HiresColor 1"
#define SCATTER_COMP_NAME        "Scatter 1"
#endif