/*****************************************************************************/
/* 
 * File name: sickframegrabber.h
 *
 * Synopsis: SIck framegrabber-specific interface
 *            
 */
#ifndef __SICK_FRAMEGRABBER_H
#define __SICK_FRAMEGRABBER_H

#include <string>

/* Forward declaration of framegrabber class, to avoid includes. */
namespace icon
   {
   class FrameGrabber;
   class EthernetCamera;
   }

/* Possible return values. */
#define SICK_FRAME_GRABBER_OK      0
#define SICK_FRAME_GRABBER_NOT_OK  1



/* FrameGrabber API. */

/* Constructs framegrabber object and set the pointer *ppFrameGrab to it. */
MIL_INT CreateFrameGrabber(icon::FrameGrabber** ppFrameGrab);

/* Initializes the frame grabber with some of the camera parameters
   and some other frame grabber specific parameters */
MIL_INT InitFrameGrabber(icon::FrameGrabber* pFrameGrab, icon::EthernetCamera* pCam);

/* Grabbing function */
void FrameGrabberProcess(icon::FrameGrabber* pFrameGrab,
                         icon::EthernetCamera* pCam,
                         MIL_INT Operation,
                         MIL_ID StartStageEvent = M_NULL);

/* Terminate communication with camera and destroys the framegrabber object. */
void CloseDown(icon::FrameGrabber* pFrameGrab, icon::EthernetCamera* pCam);

/* Switch the camera between area-scan (image) and line-scan (measurement) modes.
   rModeName can be IMAGE_MODE_NAME or MEASUREMENT_MODE_NAME (see cameraconfig.h). */
MIL_INT SwitchGrabMode(icon::FrameGrabber* pFrameGrab,
                         icon::EthernetCamera* pCam,
                         const std::string& rModeName,
                         MIL_UINT NbScansPerGrab = 1,
                         MIL_INT GrabType = M_NULL,
                         FrameGrabberHookStruct* pFrameGrabberHook = NULL);

#endif
