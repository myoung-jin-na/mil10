/*****************************************************************************/
/* 
 * File name: sickcamera_rangere.h
 *
 * Synopsis: Camera-specific interface
 *            
 */
#ifndef __SICK_CAMERA_H
#define __SICK_CAMERA_H

#include <string>

/* Forward declaration of camera class, to avoid includes. */
namespace icon
   {
   class EthernetCamera;
   class FrameGrabber;
   }

/* Camera frame grabber system struct */
typedef struct  
   {
   icon::EthernetCamera* pCam;
   icon::FrameGrabber*   pFrameGrab;

   }SSickCameraSystem;

/* FrameGrabber hook structure */
typedef struct 
   {
   MIL_ID MilSystem;
   MIL_DIG_HOOK_FUNCTION_PTR HookHandlerPtr;
   void* UserDataPtr;
   }FrameGrabberHookStruct;

/* Possible return values. */
#define SICK_CAMERA_OK      0
#define SICK_CAMERA_NOT_OK  1

/* Camera API. */

/* Constructs camera object and frame grabber object. */
MIL_INT CreateCameraSystem(SSickCameraSystem* pCameraSystem);

/* Initializes frame grabber, communication with the camera and load parameter file,
   must be called once before grabbing. */
MIL_INT InitCameraSystem(SSickCameraSystem* pCameraSystem);

/* Ask the camera configuration and fill conversion structure accordingly. */
MIL_INT FillParams(SSickCameraSystem* pCameraSystem,
                   RangerParams*          pConvParams,
                   const std::string&     rParamFilename);

/* Set the white balancing of the color component */
MIL_INT SetWhiteBalancing(SSickCameraSystem* pCameraSystem, MIL_DOUBLE RedMean, MIL_DOUBLE GreenMean, MIL_DOUBLE BlueMean, bool UseHighRes);

/* Set the color gains */
MIL_INT SetColorGain(SSickCameraSystem* pCameraSystem, MIL_DOUBLE RedMean, MIL_DOUBLE GreenMean, MIL_DOUBLE BlueMean, bool UseHighRes);

/* Grabbing function */
void FrameGrabberProcess(SSickCameraSystem* pCameraSystem,
                         MIL_INT Operation,
                         MIL_INT NbGrab = M_DEFAULT,
                         MIL_ID StartStageEvent = M_NULL);

/* Terminate communication with camera and destroys the framegrabber object. */
void CloseDown(SSickCameraSystem* pCameraSystem);

/* Switch the camera between area-scan (image) and line-scan (measurement) modes.
   rModeName can be IMAGE_MODE_NAME or MEASUREMENT_MODE_NAME (see cameraconfig.h). */
MIL_INT SwitchGrabMode(SSickCameraSystem* pCameraSystem,
                       const std::string& rModeName,
                       MIL_UINT NbScansPerGrab = 1,
                       MIL_INT GrabType = M_NULL,
                       FrameGrabberHookStruct* pFrameGrabberHook = NULL);

#endif
