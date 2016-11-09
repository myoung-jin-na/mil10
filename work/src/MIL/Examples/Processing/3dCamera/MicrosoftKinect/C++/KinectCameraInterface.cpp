//***************************************************************************************/
// 
// File name: KinectCameraInterface.cpp
//
// Synopsis:  Implementation of the base class CKinectCameraInterface.
//            

#include <mil.h>
#include <Windows.h>
#include "KinectCameraInterface.h"

// Useful defines.
#define ARRAY_COUNT(x) (sizeof(x)/sizeof(x[0]))
#define NUI_IMAGE_PLAYER_INDEX_SHIFT 3           // already defined in NuiApi.h

// Constants.
static const MIL_INT KINECT_CAMERA_GRAB_TIMEOUT = 100;

//*****************************************************************************
// Constructor.
//*****************************************************************************
CKinectCameraInterface::CKinectCameraInterface(MIL_ID MilSystem, BOOL ManualResetEvents)
   :m_MilSystem(MilSystem),
    m_MilProcThread(M_NULL),
    m_MilGrabMutex(M_NULL),
    m_StopProcEvent(NULL),
    m_NextColorFrameEvent(NULL),
    m_NextDepthFrameEvent(NULL)
   {
   // Allocate the stop event.
   m_StopProcEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

   // Allocate the next frame events.
   m_NextColorFrameEvent = CreateEvent(NULL, ManualResetEvents, FALSE, NULL);
   m_NextDepthFrameEvent = CreateEvent(NULL, ManualResetEvents, FALSE, NULL);
   
   // Allocate the grab mutex.
   MthrAlloc(MilSystem, M_MUTEX, M_DEFAULT, M_NULL, M_NULL, &m_MilGrabMutex);
   }

//*****************************************************************************
// Destructor.
//*****************************************************************************
CKinectCameraInterface::~CKinectCameraInterface()
   {  
   // Close the events.
   if(m_NextColorFrameEvent)
      {
      CloseHandle(m_NextColorFrameEvent);
      m_NextColorFrameEvent = NULL;
      }
   if(m_NextDepthFrameEvent)
      {
      CloseHandle(m_NextDepthFrameEvent);
      m_NextDepthFrameEvent = NULL;
      }
   if(m_StopProcEvent)
      {
      CloseHandle(m_StopProcEvent);
      m_StopProcEvent = NULL;
      }  

   // Free the grab mutex.
   if(m_MilGrabMutex)
      {
      MthrFree(m_MilGrabMutex);
      m_MilGrabMutex = M_NULL;
      }
   }

//*****************************************************************************
// Function that closes down the camera.
//*****************************************************************************
void CKinectCameraInterface::CloseDown()
   {
   // Stop the grab.
   StopGrab();

   // Shutdown the camera.
   ShutdownCamera();
   }

//*****************************************************************************
// Function that starts the grab.
//*****************************************************************************
void CKinectCameraInterface::FrameGrabberProcess(MIL_INT Operation,
                                                 MIL_INT OperationFlag,
                                                 MIL_INT NbGrab /* = M_DEFAULT */,
                                                 MIL_DIG_HOOK_FUNCTION_PTR HookHandlerPtr /* = M_NULL */,
                                                 void* UserDataPtr /* = M_NULL */)
   {
   // Stop the grab.
   StopGrab();

   if(Operation == M_STOP)
      return;
      
   // Lock this section.
   MthrControl(m_MilGrabMutex, M_LOCK, M_DEFAULT);

   // Set the process hook.
   m_HookHandlerPtr = HookHandlerPtr;
   m_UserDataPtr = UserDataPtr;
   m_NbGrab = (Operation != M_SEQUENCE || NbGrab == M_DEFAULT ) ? M_INFINITE : NbGrab; 

   // Start the thread.
   MthrAlloc(m_MilSystem, M_THREAD, M_DEFAULT, GrabbingFunctionThread, this, &m_MilProcThread);

   // Wait for the stop of the grab or the end of the sequence.
   if(OperationFlag & M_SYNCHRONOUS)
      MthrWait(m_MilProcThread, M_THREAD_END_WAIT, M_NULL);

   // Unlock this section.
   MthrControl(m_MilGrabMutex, M_UNLOCK, M_DEFAULT);
   }

//*****************************************************************************
// Static grabbing function that will call the Kinect camera grabbing function.
//*****************************************************************************
MIL_UINT32 MFTYPE CKinectCameraInterface::GrabbingFunctionThread(void *UserDataPtr)
   {
   CKinectCameraInterface* pKinectCamera = (CKinectCameraInterface*) UserDataPtr;
   pKinectCamera->GrabbingFunction();
   return 1;
   }

//*****************************************************************************
// Kinect camera grabbing function.
//*****************************************************************************
void CKinectCameraInterface::GrabbingFunction()
   {
   HANDLE Events[] = {m_StopProcEvent,
                      m_NextColorFrameEvent,
                      m_NextDepthFrameEvent};
   int    nEventIdx;

   // Main thread loop. Continues until NbGrab is reached or a stop event is triggered.
   bool ContinueProcessing = true;
   MIL_INT GrabCount = 0;
   while (ContinueProcessing &&  (m_NbGrab == M_INFINITE || GrabCount < m_NbGrab))
      {
      // Wait for any of the events to be signaled
      nEventIdx = WaitForMultipleObjects(ARRAY_COUNT(Events), Events, FALSE, KINECT_CAMERA_GRAB_TIMEOUT);

      // Process signal events
      switch (nEventIdx)
         {
      case WAIT_TIMEOUT:
         continue;

         // If the stop event, stop looping and exit.
      case WAIT_OBJECT_0:
         ContinueProcessing = false;
         continue;

      case WAIT_OBJECT_0 + 1:
         ProcessKinectBufferColor(m_HookHandlerPtr, m_UserDataPtr);
         GrabCount += m_NbGrab != M_INFINITE ? 1 : 0;
         break;

      case WAIT_OBJECT_0 + 2:
         ProcessKinectBufferDepth(m_HookHandlerPtr, m_UserDataPtr);
         break;     

      default:
         break;
         }
      }
   }

//*****************************************************************************
// Function to stop the grab.
//*****************************************************************************
MIL_INT CKinectCameraInterface::StopGrab()
   {
   // Signal the thread to stop and wait for its end.
   if(m_MilProcThread)
      {
      SetEvent(m_StopProcEvent);
      MthrWait(m_MilProcThread, M_THREAD_END_WAIT, M_NULL);
      MthrFree(m_MilProcThread);
      m_MilProcThread = M_NULL;
      }

   return KINECT_CAMERA_OK;
   }

//*****************************************************************************
// Function that converts the Kinect depth buffer into a Mil depth buffer.
// The Kinect buffer contains player label data in the lowest order bits.
//*****************************************************************************
void CKinectCameraInterface::ConvertKinectDepthToMilDepth(MIL_ID MilKinectDepthImage, MIL_ID MilDepthImage) const
   {
   MimShift(MilKinectDepthImage, MilDepthImage, -NUI_IMAGE_PLAYER_INDEX_SHIFT);
   }

void CKinectCameraInterface::ConvertMilDepthToKinectDepth(MIL_ID MilKinectDepthImage, MIL_ID MilDepthImage) const
   {
   MimShift(MilDepthImage, MilKinectDepthImage, NUI_IMAGE_PLAYER_INDEX_SHIFT);
   }

//*****************************************************************************
// Accessors.
//*****************************************************************************
MIL_ID CKinectCameraInterface::GetMilSystem() const
   {
   return m_MilSystem;
   }

HANDLE CKinectCameraInterface::GetNextDepthFrameEvent() const
   {
   return m_NextDepthFrameEvent;
   }

HANDLE CKinectCameraInterface::GetNextColorFrameEvent() const
   {
   return m_NextColorFrameEvent;
   }

