//***************************************************************************************/
// 
// File name: KinectCameraStandalone.h
//
// Synopsis:  Declaration of the CKinectCameraStandalone class, a class that emulates
//            the Microsoft Kinect.
//  

#ifndef __KINECT_CAMERA_STANDALONE_H
#define __KINECT_CAMERA_STANDALONE_H

#include "KinectCameraInterface.h"

class CKinectCameraStandalone : public CKinectCameraInterface
   {
   public:
      
      // Constructor.
      CKinectCameraStandalone(MIL_ID MilSystem);

      // Destructor.
      virtual ~CKinectCameraStandalone();

      // Function to get the status of the camera.
      virtual MIL_INT GetCameraStatus() const;

      // Initialize the camera.
      virtual MIL_INT InitCamera(ColorStreamTypeEnum ColorStreamType, bool UseDepth);
            
   protected:  
   private:

      // Function to shutdown the camera.
      virtual void ShutdownCamera();

      // Function that processes the Kinect buffer.
      virtual void ProcessKinectBufferDepth(MIL_DIG_HOOK_FUNCTION_PTR HookHandlerPtr, void* UserDataPtr);
      virtual void ProcessKinectBufferColor(MIL_DIG_HOOK_FUNCTION_PTR HookHandlerPtr, void* UserDataPtr);
      void ProcessKinectBuffer(MIL_CONST_TEXT_PTR AviPath,
                               MIL_ID MilKinectImage,
                               MIL_DIG_HOOK_FUNCTION_PTR HookHandlerPtr,
                               void* UserDataPtr);
      
      // The restored kinect images
      MIL_ID m_MilKinectDepthImage;
      MIL_ID m_MilKinectColorImage;

      bool m_SimulatedGrabActive;

      // Microsoft multimedia timers. 
      UINT m_ColorEventTimer;
      UINT m_DepthEventTimer;
           
      MIL_INT m_AviNbImages;
      MIL_INT m_AviIndex;
   };

#endif // __KINECT_CAMERA_H