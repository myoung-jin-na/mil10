//***************************************************************************************/
// 
// File name: KinectCamera.h
//
// Synopsis: Declaration of the CKinectCamera class a class that manages
//           the Microsoft Kinect.
// 

#ifndef __KINECT_CAMERA_H
#define __KINECT_CAMERA_H

#include "KinectCameraInterface.h"

#if USE_REAL_CAMERA

// Forward declaration of camera class, to avoid includes.
struct INuiSensor;

class CKinectCamera : public CKinectCameraInterface
   {
   public:
      
      // Constructor.
      CKinectCamera(MIL_ID MilSystem);

      // Destructor.
      virtual ~CKinectCamera();

      // Function to get the status of the camera.
      virtual MIL_INT GetCameraStatus() const;

      // Function to initialize the camera.
      virtual MIL_INT InitCamera(ColorStreamTypeEnum ColorStreamType, bool UseDepth);      
           
   protected:
   private:

      // Function to shutdown the camera.
      virtual void ShutdownCamera();

      // Function that processes the Kinect buffer.
      virtual void ProcessKinectBufferDepth(MIL_DIG_HOOK_FUNCTION_PTR HookHandlerPtr, void* UserDataPtr);
      virtual void ProcessKinectBufferColor(MIL_DIG_HOOK_FUNCTION_PTR HookHandlerPtr, void* UserDataPtr);
      void ProcessKinectBuffer(HANDLE NuiImageStream,
                               MIL_DIG_HOOK_FUNCTION_PTR HookHandlerPtr,
                               void* UserDataPtr,
                               MIL_INT NbBands,
                               MIL_INT Type,
                               MIL_INT Attribute);
     
      // The status of the Kinect.
      MIL_INT m_SensorStatus;

      // The streams and pointer to the NuiSensor.
      INuiSensor* m_pNuiSensor;
      HANDLE m_ColorStreamHandle;
      HANDLE m_DepthStreamHandle;
      
      // Indicates if the color image stream is the IR camera or color camera stream.
      ColorStreamTypeEnum m_ColorStreamType;
   };

#endif

#endif // __KINECT_CAMERA_H