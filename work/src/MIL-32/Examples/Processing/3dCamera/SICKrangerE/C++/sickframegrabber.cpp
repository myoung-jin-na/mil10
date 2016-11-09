/*****************************************************************************/
/* 
 * File name: sickframegrabber.cpp
 *
 * Synopsis: All frame grabber-specific code
 *            
 */

#include "cameraconfig.h"           /* User options - change them to suit your needs. */

#include <mil.h>
#include "mil3dcam_sickrangere.h"   /* Parameter struct definition.                   */
#include "sickcamera_rangere.h"     /* Prototypes to interact with ranger camera.     */
#include "sickframegrabber.h"       /* Prototypes to interact with ranger camera.     */

#if !USE_REAL_CAMERA



#else

#include "icon_auto_link.h"
#include "icon_api.h"         /* Include the icon api                           */
#include "ethernetcamera.h"   /* Ethernet camera specific prototypes            */
#include "framegrabber.h"     /* Framegrabber specific prototypes               */
#include "dataformat.h"       /* Icon-specific structure                        */

/* ICON API name spaces. */
using namespace std;
using namespace icon;


/*****************************************************************************
 API function implementations.
*****************************************************************************/




/*****************************************************************************
 Helper functions.
*****************************************************************************/
/* Frame grabber processing hook function */
void ProcessCallback(IconBuffer *pBuffer, FrameGrabber* pFrameGrab, void* UserData)
   {
   // Get the FrameGrabberHookStruct
   FrameGrabberHookStruct* pFrameGrabHookStruct = (FrameGrabberHookStruct*) UserData;
       
   // Create a mil image on the ranger data
   MIL_ID MilRangerImage = MbufCreate2d(pFrameGrabHookStruct->MilSystem,
                                        pBuffer->getWidth(),
                                        pBuffer->getHeight(),
                                        8 + M_UNSIGNED,
                                        M_IMAGE + M_PROC,
                                        M_HOST_ADDRESS + M_PITCH_BYTE,
                                        pBuffer->getWidth(),
                                        pBuffer->getWritePointer(0),
                                        M_NULL);
        
   // Call the Mil hook function
   (*(pFrameGrabHookStruct->HookHandlerPtr))(0, MilRangerImage, pFrameGrabHookStruct->UserDataPtr);

   // Free the created buffer
   MbufFree(MilRangerImage);
   }

/* Start grab function */
MIL_INT StartGrab(FrameGrabber* pFrameGrab, RangerE* pCam, MIL_ID StartStageEvent /* = M_NULL */)
   {
   // Get the index of a free frame grabber
   MIL_INT Index  = GetNewActiveIndex();
   if(Index != -1)
      {
      MIL_INT Ret;

      // Set the frame grabber as active
      ActiveFrameGrabber[Index] = pFrameGrab;
            
      // Start the frame grabber grab
      Ret = pFrameGrab->startGrab();
      if(Ret != FrameGrabber::E_ALL_OK)
         return SICK_FRAME_GRABBER_NOT_OK;

      // Start the camera
      Ret = pCam->start();
      if(Ret != Camera::E_ALL_OK)
         return SICK_CAMERA_NOT_OK;

      // Start the conveyor and wait for its speed to become constant
      if(StartStageEvent)
         MthrControl(StartStageEvent, M_EVENT_SET, M_SIGNALED);
      MosSleep(STAGE_STARTUP_DELAY_MS);      
      }
   else
      return SICK_CAMERA_NOT_OK;

   return SICK_FRAME_GRABBER_OK;
   }

/* Stop grab function */
MIL_INT StopGrab(FrameGrabber* pFrameGrab, RangerE* pCam)
   {
   MIL_INT Ret;
   MIL_INT Ret2;

   // Remove from active
   if(RemoveActiveFrameGrabber(pFrameGrab))
      {
      // Stop the grab
      Ret = pFrameGrab->stopGrab();
      if(Ret != FrameGrabber::E_ALL_OK)
         return SICK_FRAME_GRABBER_NOT_OK;

      // Stop the camera
      int Status;
      Ret2 = pCam->checkCamStatus(Status);
      if(Ret2 != Camera::E_ALL_OK)
         return SICK_CAMERA_NOT_OK;

      if(Status != RangerE::Stopped)
         {
         Ret = pCam->stop();
         if(Ret != Camera::E_ALL_OK)
            return SICK_CAMERA_NOT_OK;
         }    
      }
   
   return SICK_FRAME_GRABBER_OK;
   }

/* Function to get the new active index in the list */
MIL_INT GetNewActiveIndex()
   {
   for(MIL_INT i = 0; i < MAX_FRAME_GRABBER; i++)
      {
      if(ActiveFrameGrabber[i] == NULL)
         return i;
      }
   return -1;
   }

bool IsActive(FrameGrabber* pFrameGrab)
   {
   for(MIL_INT i = 0; i < MAX_FRAME_GRABBER; i++)
      {
      if(ActiveFrameGrabber[i] == pFrameGrab)
         return true;
      }
   return false;
   }

/* Function to remove the active frame grabber */
bool RemoveActiveFrameGrabber(FrameGrabber* pFrameGrab)
   {
   for(MIL_INT i = 0; i < MAX_FRAME_GRABBER; i++)
      {
      if(ActiveFrameGrabber[i] == pFrameGrab)
         {
         ActiveFrameGrabber[i] = NULL;
         return true;
         }
      }
   return false;
   }

#endif
