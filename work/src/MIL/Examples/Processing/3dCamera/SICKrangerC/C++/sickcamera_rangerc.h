/*****************************************************************************/
/* 
 * File name: sickcamera_rangerc.h
 *
 * Synopsis: Camera-specific interface
 *            
 */
#ifndef __SICK_CAMERA_H
#define __SICK_CAMERA_H

#include <string>

#define GRAB_TIMEOUT_MS      10000.0
#define ONE_FRAME_DELAY_MS   50

/* Forward declaration of camera class, to avoid includes. */
namespace icon
   {
   class RangerC;
   }

/* Possible return values. */
#define SICK_CAMERA_OK      0
#define SICK_CAMERA_NOT_OK  1

/* Camera API. */

/* Constructs camera object and set the pointer *ppCam to it. */
MIL_INT CreateCamera(icon::RangerC** ppCam);

/* Initializes communication with the camera and load parameter file,
   must be called once before grabbing. */
MIL_INT InitCamera(icon::RangerC* pCam);

/* Ask the camera configuration and fill conversion structure accordingly. */
MIL_INT FillParams(icon::RangerC*     pCam,
                   RangerParams*      pConvParams,
                   const std::string& rParamFilename,
                   const char*        pMeasCompName);

/* Switch the camera between area-scan (image) and line-scan (measurement) modes.
   rModeName can be IMAGE_MODE_NAME or MEASUREMENT_MODE_NAME (see cameraconfig.h). */
MIL_INT SwitchCameraMode(icon::RangerC* pCam, const std::string& rModeName);

/* Terminate communication with camera and destroys the camera object. */
void CloseDown(icon::RangerC* pCam);

#endif
