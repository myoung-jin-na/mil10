//***************************************************************************************/
// 
// File name: KinectCameraInterface.h
//
// Synopsis: Declaration of the base class CKinectCameraInterface.
//

#ifndef __KINECT_CAMERA_INTERFACE_H
#define __KINECT_CAMERA_INTERFACE_H

// Defines that sets whether the real camera is used in the example.
// Here are the possible configuration:
// USE_REAL_CAMERA | CALIBRATE_WITH_REAL_CAMERA |
//         1       |              1             | All images are grabbed live
//         1       |              0             | All images are grabbed live except the calibration grid images
//         0       |          Irrelevant        | All images are restored from file
#define USE_REAL_CAMERA 0
#define CALIBRATE_WITH_REAL_CAMERA 0

// The paths to the grab folders.
#define EXAMPLE_IMAGE_PATH M_IMAGE_PATH MIL_TEXT("MicrosoftKinect\\")
static MIL_CONST_TEXT_PTR COLOR_GRID = EXAMPLE_IMAGE_PATH MIL_TEXT("ColorGrid.mim");
static MIL_CONST_TEXT_PTR IR_GRID    = EXAMPLE_IMAGE_PATH MIL_TEXT("IRGrid.mim");
static MIL_CONST_TEXT_PTR COLOR_AVI  = EXAMPLE_IMAGE_PATH MIL_TEXT("Color.avi");
static MIL_CONST_TEXT_PTR DEPTH_AVI  = EXAMPLE_IMAGE_PATH MIL_TEXT("Depth.avi");
static const MIL_DOUBLE GRAB_FPS = 30;

// Possible return values.
#define KINECT_CAMERA_OK      0
#define KINECT_CAMERA_NOT_OK  1

// Camera configuration constants.
#define KINECT_DEPTH_IMAGE_SIZE_X  640
#define KINECT_DEPTH_IMAGE_SIZE_Y  480
#define KINECT_COLOR_IMAGE_SIZE_X  640
#define KINECT_COLOR_IMAGE_SIZE_Y  480

// Enum that defines what kind of color stream to use.
enum ColorStreamTypeEnum
   {
   enColor = 0,
   enIR
   };

class CKinectCameraInterface
   {
   public:
      
      // Constructor.
      CKinectCameraInterface(MIL_ID MilSystem, BOOL ManualResetEvents);

      // Destructor.
      virtual ~CKinectCameraInterface();

      // Function to get the status of the camera.
      virtual MIL_INT GetCameraStatus() const = 0;

      // Function that initializes the camera.
      virtual MIL_INT InitCamera(ColorStreamTypeEnum ColorStreamType, bool UseDepth) = 0;

      // Function that close down the camera.
      void CloseDown();

      // Grabbing function.
      void FrameGrabberProcess(MIL_INT Operation,
                               MIL_INT OperationFlag,
                               MIL_INT NbGrab = M_DEFAULT,
                               MIL_DIG_HOOK_FUNCTION_PTR HookHandlerPtr = M_NULL,
                               void* UserDataPtr = M_NULL);

      // Converts the Kinect depth data to some MIL depth data.
      void ConvertKinectDepthToMilDepth(MIL_ID MilKinectDepthImage, MIL_ID MilDepthImage) const;
      void ConvertMilDepthToKinectDepth(MIL_ID MilKinectDepthImage, MIL_ID MilDepthImage) const;      

      // Grabbing function thread of the Kinect.
      static MIL_UINT32 MFTYPE GrabbingFunctionThread(void *UserDataPtr);
      void GrabbingFunction();
            
   protected:

      // Virtual function that processes the Kinect buffer.
      virtual void ProcessKinectBufferDepth(MIL_DIG_HOOK_FUNCTION_PTR HookHandlerPtr, void* UserDataPtr) = 0;
      virtual void ProcessKinectBufferColor(MIL_DIG_HOOK_FUNCTION_PTR HookHandlerPtr, void* UserDataPtr) = 0;
     
      // Virtual function that shuts down the camera.
      virtual void ShutdownCamera() = 0;

      // Function that stops the grab.
      MIL_INT StopGrab();

      // Accessors.
      MIL_ID GetMilSystem() const;
      HANDLE GetNextDepthFrameEvent() const;
      HANDLE GetNextColorFrameEvent() const;
            
   private:
      
      // Disallow copying.
      CKinectCameraInterface(const CKinectCameraInterface&);
      const CKinectCameraInterface& operator=(const CKinectCameraInterface&);

      HANDLE m_NextDepthFrameEvent;
      HANDLE m_NextColorFrameEvent;
      HANDLE m_StopProcEvent;

      MIL_ID m_MilSystem;
      MIL_ID m_MilGrabMutex;
      MIL_ID m_MilProcThread;
      
      // Temporary variables for each FrameGrabberProcess
      MIL_DIG_HOOK_FUNCTION_PTR m_HookHandlerPtr;
      void* m_UserDataPtr;
      MIL_INT m_NbGrab;
   };

#endif // __KINECT_CAMERA_H