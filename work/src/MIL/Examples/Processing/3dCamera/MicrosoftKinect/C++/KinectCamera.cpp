//***************************************************************************************/
// 
// File name: KinectCamera.cpp
//
// Synopsis:  Implementation of the CKinectCamera class, a class that manages
//            the Microsoft Kinect.
//

#include <mil.h>
#include <Windows.h>
#include "KinectCamera.h"

#if USE_REAL_CAMERA

// Include the header for the Kinect. Define the static_assert function for compiler < VS2010
#pragma comment(lib, "Kinect10.lib")
#if _MSC_VER < 1600
#define static_assert(a, b)
#endif
#include "NuiApi.h"

// Constants.
static const NUI_IMAGE_RESOLUTION KINECT_COLOR_IMAGE_RESOLUTION = NUI_IMAGE_RESOLUTION_640x480;
static const NUI_IMAGE_RESOLUTION KINECT_DEPTH_IMAGE_RESOLUTION = NUI_IMAGE_RESOLUTION_640x480;
static const MIL_INT KINECT_COLOR_NB_FRAMES = 2;
static const MIL_INT KINECT_DEPTH_NB_FRAMES = 2;
static const MIL_INT KINECT_IR_NB_FRAMES = 2;

//*****************************************************************************
// Constructor.
//*****************************************************************************
CKinectCamera::CKinectCamera(MIL_ID MilSystem)
   : CKinectCameraInterface(MilSystem, TRUE),
     m_SensorStatus(KINECT_CAMERA_OK)
   {
   // Create the sensor.
   if(FAILED(NuiCreateSensorByIndex(0, &m_pNuiSensor)))
      m_SensorStatus = KINECT_CAMERA_NOT_OK;  
   }

//*****************************************************************************
// Function to get the allocation status of the camera.
//*****************************************************************************
MIL_INT CKinectCamera::GetCameraStatus() const
   {
   return m_SensorStatus;
   }

//*****************************************************************************
// Destructor.
//*****************************************************************************
CKinectCamera::~CKinectCamera()
   {
   // Close down the camera.
   CloseDown();
   }

void CKinectCamera::ShutdownCamera()
   {
   // Shut down the sensor.
   if(m_pNuiSensor)
      m_pNuiSensor->NuiShutdown();

   // Release the sensor. This should also close the image streams handles.
   if(m_pNuiSensor)
      {
      m_pNuiSensor->Release();
      m_pNuiSensor = NULL;
      }
   }

//*****************************************************************************
// Initialization function.
//*****************************************************************************
MIL_INT CKinectCamera::InitCamera(ColorStreamTypeEnum ColorStreamType, bool UseDepth)
   {
   if(m_SensorStatus == KINECT_CAMERA_NOT_OK)
      return m_SensorStatus;

   // Set whether to use the IR camera.
   m_ColorStreamType = ColorStreamType;

   // Open/Close the IR Emitter. 
   if(FAILED(m_pNuiSensor->NuiSetForceInfraredEmitterOff(!UseDepth)))
      return KINECT_CAMERA_NOT_OK;  

   // ReInitialize the Kinect sensor.
   if(FAILED(m_pNuiSensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_COLOR + (UseDepth ? NUI_INITIALIZE_FLAG_USES_DEPTH : 0))))
      return KINECT_CAMERA_NOT_OK;

   // Open the color/infrared streams of the Kinect.
   if(FAILED(m_pNuiSensor->NuiImageStreamOpen(m_ColorStreamType == enColor? NUI_IMAGE_TYPE_COLOR : NUI_IMAGE_TYPE_COLOR_INFRARED,
                                              KINECT_COLOR_IMAGE_RESOLUTION,
                                              0,
                                              KINECT_COLOR_NB_FRAMES,
                                              GetNextColorFrameEvent(),
                                              &m_ColorStreamHandle)))
      return KINECT_CAMERA_NOT_OK;

   // Open the depth stream(do not open if in IR mode).
   if(UseDepth)
      {
      if(FAILED(m_pNuiSensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_DEPTH,
                                                 KINECT_DEPTH_IMAGE_RESOLUTION,
                                                 0,
                                                 KINECT_DEPTH_NB_FRAMES,
                                                 GetNextDepthFrameEvent(),
                                                 &m_DepthStreamHandle)))
            return KINECT_CAMERA_NOT_OK;

      // Put the depth steam in near mode.
      if(FAILED(NuiImageStreamSetImageFrameFlags(m_DepthStreamHandle, NUI_IMAGE_STREAM_FLAG_ENABLE_NEAR_MODE)))
         return KINECT_CAMERA_NOT_OK;
      }   
   return KINECT_CAMERA_OK;
   }

//*****************************************************************************
// Function that processes the Kinect buffer.
//*****************************************************************************
void CKinectCamera::ProcessKinectBufferColor(MIL_DIG_HOOK_FUNCTION_PTR HookHandlerPtr, void* UserDataPtr)
   {
   if(m_ColorStreamType == enColor)
      ProcessKinectBuffer(m_ColorStreamHandle, HookHandlerPtr, UserDataPtr, 3, 8+M_UNSIGNED, M_BGR32 + M_PACKED);
   else
      ProcessKinectBuffer(m_ColorStreamHandle, HookHandlerPtr, UserDataPtr, 1, 16+M_UNSIGNED, M_NULL);   
      
   }

void CKinectCamera::ProcessKinectBufferDepth(MIL_DIG_HOOK_FUNCTION_PTR HookHandlerPtr, void* UserDataPtr)
   {
   ProcessKinectBuffer(m_DepthStreamHandle, HookHandlerPtr, UserDataPtr, 1, 16+M_UNSIGNED, M_NULL);
   }

void CKinectCamera::ProcessKinectBuffer(HANDLE NuiImageStream,
                                        MIL_DIG_HOOK_FUNCTION_PTR HookHandlerPtr,
                                        void* UserDataPtr,
                                        MIL_INT NbBands,
                                        MIL_INT Type,
                                        MIL_INT Attribute)
   {
   NUI_IMAGE_FRAME NuiImageFrame;
   NUI_LOCKED_RECT NuiLockedRect;
   NUI_SURFACE_DESC NuiSurfaceDesc;

   // Get the image frame.
   if(FAILED(m_pNuiSensor->NuiImageStreamGetNextFrame(NuiImageStream, 0,  &NuiImageFrame)))
      return;

   // Get the surface descriptor.
   NuiImageFrame.pFrameTexture->GetLevelDesc(0, &NuiSurfaceDesc);

   // Get the locked rect of the image frame.
   NuiImageFrame.pFrameTexture->LockRect(0, &NuiLockedRect, NULL, 0);   

   // Create a mil image on the kinect data. 
   MIL_ID MilKinectImage = MbufCreateColor(GetMilSystem(),
                                           NbBands,
                                           NuiSurfaceDesc.Width,
                                           NuiSurfaceDesc.Height,
                                           Type,
                                           M_IMAGE + M_PROC + Attribute,
                                           M_HOST_ADDRESS + M_PITCH_BYTE,
                                           NuiLockedRect.Pitch,
                                           (void**)(&NuiLockedRect.pBits),
                                           M_NULL);

   // Call the Mil hook function. 
   (*HookHandlerPtr)(0, MilKinectImage, UserDataPtr);

   // Free the created buffer.
   MbufFree(MilKinectImage);

   // Release the locked rect of the image frame.
   NuiImageFrame.pFrameTexture->UnlockRect(0);

   // Release the image frame.
   m_pNuiSensor->NuiImageStreamReleaseFrame(NuiImageStream, &NuiImageFrame);
   }

#endif