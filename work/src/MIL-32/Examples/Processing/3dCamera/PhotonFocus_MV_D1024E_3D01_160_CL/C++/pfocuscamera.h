/*****************************************************************************/
/* 
 * File name: pfocuscamera.h
 *
 * Synopsis: Camera-specific interface
 *            
 */
#ifndef __PFOCUS_CAMERA_H
#define __PFOCUS_CAMERA_H

/* Possible return values. */
#define PFOCUS_CAMERA_OK         0
#define PFOCUS_CAMERA_NOT_OK     1

/* Camera API. */

/* Initializes communication with the camera and load parameter file,
   must be called once before grabbing. */
MIL_INT InitCamera(int Port);

/* Ask the camera configuration and fill conversion structure accordingly. */
MIL_INT FillParams(int Port, PFocusParams* pConvParams);

/* Switch the camera to 2d mode only and image ROI. */
MIL_INT SwitchCameraToImage(int     Port      ,
                            MIL_INT ROIOffsetX,
                            MIL_INT ROIOffsetY,
                            MIL_INT ROISizeX  ,
                            MIL_INT ROISizeY  );

/* Switch the camera to 2d and 3d mode and measurement ROI. */
MIL_INT SwitchCameraToMeasure(int     Port      ,
                              MIL_INT ROIOffsetX,
                              MIL_INT ROIOffsetY,
                              MIL_INT ROISizeX  ,
                              MIL_INT ROISizeY  );

/* Terminate communication with camera and destroys the camera object. */
void CloseDown(int Port);

#endif
