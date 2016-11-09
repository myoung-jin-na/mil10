//***************************************************************************************/
// 
// File name: KinectCameraStandalone.cpp
//
// Synopsis:  Implementation of the CKinectCameraStandalone class, a class that emulates
//            the Microsoft Kinect.
//  

#include <mil.h>
#include <Windows.h> 
#include "KinectCameraStandalone.h"

//*****************************************************************************
// Constructor.
//*****************************************************************************
CKinectCameraStandalone::CKinectCameraStandalone(MIL_ID MilSystem)
   : CKinectCameraInterface(MilSystem, FALSE),
     m_SimulatedGrabActive(false),
     m_ColorEventTimer(0),
     m_DepthEventTimer(0),
     m_AviNbImages(0),
     m_AviIndex(0),
     m_MilKinectDepthImage(M_NULL),
     m_MilKinectColorImage(M_NULL)
   {
   // Get the number of images of the avi.
   MbufDiskInquire(COLOR_AVI, M_NUMBER_OF_IMAGES, &m_AviNbImages);
   }

//*****************************************************************************
// Function to get the allocation status of the camera.
//*****************************************************************************
MIL_INT CKinectCameraStandalone::GetCameraStatus() const
   {
   return KINECT_CAMERA_OK;
   }

//*****************************************************************************
// Destructor.
//*****************************************************************************
CKinectCameraStandalone::~CKinectCameraStandalone()
   {
   // Close down the camera.
   CloseDown();

   // Free the images if necessary.
   if(m_MilKinectColorImage)
      MbufFree(m_MilKinectColorImage);
   if(m_MilKinectDepthImage)
      MbufFree(m_MilKinectDepthImage);
   }

void CKinectCameraStandalone::ShutdownCamera()
   {
   // Signal the end of the simulated grab.
   timeKillEvent(m_ColorEventTimer);
   timeKillEvent(m_DepthEventTimer);
   m_SimulatedGrabActive = false;

   // Close the sequences.
   MbufImportSequence(COLOR_AVI, M_DEFAULT, M_NULL, M_NULL, M_NULL, 0, M_NULL, M_CLOSE);
   MbufImportSequence(DEPTH_AVI, M_DEFAULT, M_NULL, M_NULL, M_NULL, 0, M_NULL, M_CLOSE);
   }

//*****************************************************************************
// Initialization function.
//*****************************************************************************
MIL_INT CKinectCameraStandalone::InitCamera(ColorStreamTypeEnum ColorStreamType, bool UseDepth)
   {
   if(!m_SimulatedGrabActive)
      {
      // Open the sequences. 
      MbufImportSequence(COLOR_AVI, M_DEFAULT, M_NULL, M_NULL, M_NULL, 0, M_NULL, M_OPEN);
      MbufImportSequence(DEPTH_AVI, M_DEFAULT, M_NULL, M_NULL, M_NULL, 0, M_NULL, M_OPEN);

      // Allocate the images by restoring the first frame of the sequences.
      MbufImportSequence(COLOR_AVI, M_DEFAULT, M_RESTORE, GetMilSystem(), &m_MilKinectColorImage, 0, 1, M_READ);
      MbufImportSequence(DEPTH_AVI, M_DEFAULT, M_RESTORE, GetMilSystem(), &m_MilKinectDepthImage, 0, 1, M_READ);
      
      // Start the simulated camera by starting the multimedia timers.
      m_ColorEventTimer = timeSetEvent((UINT)(1.0/GRAB_FPS * 1000), 1, (LPTIMECALLBACK)GetNextColorFrameEvent(),
                                       NULL, TIME_PERIODIC + TIME_CALLBACK_EVENT_SET);
      m_DepthEventTimer = timeSetEvent((UINT)(1.0/GRAB_FPS * 1000), 1, (LPTIMECALLBACK)GetNextDepthFrameEvent(),
                                       NULL, TIME_PERIODIC + TIME_CALLBACK_EVENT_SET);
      m_SimulatedGrabActive = true;
      }

   return KINECT_CAMERA_OK;
   }

//*****************************************************************************
// Function that processes the Kinect buffer.
//*****************************************************************************
void CKinectCameraStandalone::ProcessKinectBufferColor(MIL_DIG_HOOK_FUNCTION_PTR HookHandlerPtr, void* UserDataPtr)
   {
   ProcessKinectBuffer(COLOR_AVI, m_MilKinectColorImage, HookHandlerPtr, UserDataPtr);
   m_AviIndex = (m_AviIndex == m_AviNbImages-1) ? 0 : m_AviIndex + 1;
   }

void CKinectCameraStandalone::ProcessKinectBufferDepth(MIL_DIG_HOOK_FUNCTION_PTR HookHandlerPtr, void* UserDataPtr)
   {
   ProcessKinectBuffer(DEPTH_AVI, m_MilKinectDepthImage, HookHandlerPtr, UserDataPtr);
   }

void CKinectCameraStandalone::ProcessKinectBuffer(MIL_CONST_TEXT_PTR AviPath,
                                                  MIL_ID MilKinectImage,
                                                  MIL_DIG_HOOK_FUNCTION_PTR HookHandlerPtr,
                                                  void* UserDataPtr)
   {
   MbufImportSequence(AviPath, M_DEFAULT, M_LOAD, M_NULL, &MilKinectImage, m_AviIndex, 1, M_READ);

   // Call the Mil hook function. 
   (*HookHandlerPtr)(0, MilKinectImage, UserDataPtr);
   }