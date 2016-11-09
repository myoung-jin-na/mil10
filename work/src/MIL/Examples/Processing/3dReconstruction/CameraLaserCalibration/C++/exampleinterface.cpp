//***************************************************************************************
//
// File name: exampleinterface.cpp
//
// Synopsis:  Implementation for CExampleInterface (implementation common to subclasses).
//

#include <mil.h>
#include "exampleinterface.h"

//*****************************************************************************
// Constants.
//*****************************************************************************

// Set this to 0 if you wish to remove the images pasted in the overlay.
#define ENABLE_IMAGES_IN_OVERLAY 1

// File names of images that will be pasted in the overlay.
static const MIL_TEXT_CHAR* const OVERLAY_IMAGE_FILES[] =
   {
   EXAMPLE_IMAGE_PATH MIL_TEXT("CalibrateCameraImage.mim"),    // corresponds to CExampleInterface::eCalibrateCameraImage
   EXAMPLE_IMAGE_PATH MIL_TEXT("AdjustMinIntensityImage.mim"), // corresponds to CExampleInterface::eAdjustMinIntensityImage
   EXAMPLE_IMAGE_PATH MIL_TEXT("CalibrateLaserImage.mim")      // corresponds to CExampleInterface::eCalibrateLaserImage
   };

// File name of an image used to frame the images to be pasted in the overlay.
static const MIL_TEXT_CHAR* const FRAME_IMAGE_FILE =
   EXAMPLE_IMAGE_PATH MIL_TEXT("frame.mim");

// Transparency color of the overlay.
static const MIL_DOUBLE DEFAULT_TRANSPARENT_COLOR = M_RGB888(16,0,0);

// Transparency color of the frame image.
static const MIL_DOUBLE FRAME_TRANSPARENT_COLOR   = M_RGB888(227,0,227);

// Size of the border in the camera image (used when pasting in the overlay).
static const MIL_INT FRAME_BORDER = 5; // in pixels

// Maximum ratio of picture to paste in the overlay with respect to camera image size.
static const MIL_DOUBLE MAX_PICTURE_RATIO = 0.25;

//*****************************************************************************
// Constructor. Allocates MIL application, system and display.
//*****************************************************************************
CExampleInterface::CExampleInterface(MIL_CONST_TEXT_PTR SystemDescriptor, MIL_INT64 BufAttr)
: m_BufferAttribute(BufAttr),
  m_SizeX          (   0   ),
  m_SizeY          (   0   ),
  m_OverlayOffsetX (   0   ),
  m_OverlayOffsetY (   0   ),
  m_CalibrationMode(   0   ),
  m_MilApplication (M_NULL ),
  m_MilSystem      (M_NULL ),
  m_MilDisplay     (M_NULL ),
  m_MilDisplayImage(M_NULL ),
  m_MilOverlayImage(M_NULL )
   {
   // Allocate application.
   MappAlloc(M_NULL, M_DEFAULT, &m_MilApplication);

   // Allocate system (SystemDescriptor depends on subclass).
   MsysAlloc(M_DEFAULT, SystemDescriptor, M_DEFAULT, M_DEFAULT, &m_MilSystem);

   // Allocate display.
   MdispAlloc(m_MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &m_MilDisplay);
   for (MIL_INT i = 0 ; i < eNbOverlayImages; i++)
      {
      m_MilOverlayPictures[i] = M_NULL;
      }
   }

//*****************************************************************************
// Destructor, free all MIL objects.
//*****************************************************************************
CExampleInterface::~CExampleInterface()
   {
   for (MIL_INT i = 0; i < eNbOverlayImages; ++i)
      {
      if (m_MilOverlayPictures[i] != M_NULL)
         MbufFree(m_MilOverlayPictures[i]);
      }
   if (m_MilDisplayImage != M_NULL)
      MbufFree(m_MilDisplayImage);
   if (m_MilDisplay != M_NULL)
      MdispFree(m_MilDisplay);
   if (m_MilSystem != M_NULL)
      MsysFree(m_MilSystem);
   if (m_MilApplication != M_NULL)
      MappFree(m_MilApplication);
   }

//*****************************************************************************
// Prompts the user for the example type to use (will determine which subclass
// will be instantiated in main code).
//*****************************************************************************
bool CExampleInterface::AskInteractiveExample()
   {
   MosPrintf(MIL_TEXT("You can run this example with images:\n")
             MIL_TEXT("   1. Reloaded from disk     (STAND-ALONE)\n")
             MIL_TEXT("   2. Grabbed using a camera (INTERACTIVE)\n\n")
             MIL_TEXT("Your choice (1 or 2): "));

   MIL_INT Choice = 0;
   while (Choice == 0)
      {
      switch (MosGetch())
         {
         case MIL_TEXT('1'):
            Choice = 1;
            MosPrintf(MIL_TEXT("1. STAND-ALONE\n\n"));
            break;

         case MIL_TEXT('2'):
            Choice = 2;
            MosPrintf(MIL_TEXT("2. INTERACTIVE\n\n"));
            break;
         }
      }

   bool IsInteractive = (Choice == 2);
   return IsInteractive;
   }

//*****************************************************************************
// Prompts the user for the 3d reconstruction context type to use.
//*****************************************************************************
MIL_INT CExampleInterface::AskCalibrationMode()
   {
   MosPrintf(MIL_TEXT("Choose a calibration mode:\n")
             MIL_TEXT("   1. M_DEPTH_CORRECTION\n")
             MIL_TEXT("   2. M_CALIBRATED_CAMERA_LINEAR_MOTION\n\n")
             MIL_TEXT("Your choice (1 or 2): "));

   MIL_INT CalibrationMode = M_NULL;
   while (CalibrationMode == M_NULL)
      {
      switch (MosGetch())
         {
         case MIL_TEXT('1'):
            CalibrationMode = M_DEPTH_CORRECTION;
            MosPrintf(MIL_TEXT("1. M_DEPTH_CORRECTION\n\n")

                      MIL_TEXT("For this calibration mode, the following steps will be executed:\n")
                      MIL_TEXT("   - Setting up the laser line extraction parameters.\n")
                      MIL_TEXT("   - Calibrating the 3d reconstruction setup.\n\n"));
            break;

         case MIL_TEXT('2'):
            CalibrationMode = M_CALIBRATED_CAMERA_LINEAR_MOTION;
            MosPrintf(MIL_TEXT("2. M_CALIBRATED_CAMERA_LINEAR_MOTION\n\n")

                      MIL_TEXT("For this calibration mode, the following steps will be executed:\n")
                      MIL_TEXT("   - Calibrating the camera.\n")
                      MIL_TEXT("   - Setting up the laser line extraction parameters.\n")
                      MIL_TEXT("   - Calibrating the 3d reconstruction setup.\n\n"));
            break;
         }
      }

   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   m_CalibrationMode = CalibrationMode;
   return CalibrationMode;
   }

//*****************************************************************************
// Show the display window and enables its overlay. If it is the first time the
// function is called, the display image will also be allocated. Also, all images
// that will be pasted in the overlay are loaded from disk and "framed".
//*****************************************************************************
void CExampleInterface::ShowDisplay()
   {
   // Only do this on the first call.
   if (m_MilDisplayImage == M_NULL)
      {
      // Allocate display image (m_BufferAttribute - M_NULL or M_GRAB - depends on subclass).
      MbufAlloc2d(m_MilSystem, m_SizeX, m_SizeY, 8+M_UNSIGNED, M_IMAGE+M_PROC+M_DISP+m_BufferAttribute, &m_MilDisplayImage);
      MbufClear(m_MilDisplayImage, 0.0);

      // Set overlay transparent color.
      MdispControl(m_MilDisplay, M_TRANSPARENT_COLOR, DEFAULT_TRANSPARENT_COLOR);

      // Set up images to be pasted in the overlay.
      // Determine size of the images.
      MIL_INT PictureSizeX, PictureSizeY;
      DeterminePictureSize(&PictureSizeX, &PictureSizeY);

      // Load frame image for overlay and resize it to picture size.
      MIL_ID MilFrameOriginalImage = MbufRestore(FRAME_IMAGE_FILE, m_MilSystem, M_NULL);
      MIL_ID MilFrameImage = MbufAllocColor(m_MilSystem, 3, PictureSizeX, PictureSizeY, 8+M_UNSIGNED, M_IMAGE+M_PROC+M_DISP, M_NULL);
      MimResize(MilFrameOriginalImage, MilFrameImage, M_FILL_DESTINATION, M_FILL_DESTINATION, M_NEAREST_NEIGHBOR);
      MbufFree(MilFrameOriginalImage);

      // Load overlay images that will be used later, resize them and frame them.
      for (MIL_INT i = 0; i < eNbOverlayImages; ++i)
         {
         // Load and resize image.
         MIL_ID MilOriginalImage = MbufRestore(OVERLAY_IMAGE_FILES[i], m_MilSystem, M_NULL);
         MbufAllocColor(m_MilSystem, 3, PictureSizeX, PictureSizeY, 8+M_UNSIGNED, M_IMAGE+M_PROC+M_DISP, &m_MilOverlayPictures[i]);
         MimResize(MilOriginalImage, m_MilOverlayPictures[i], M_FILL_DESTINATION, M_FILL_DESTINATION, M_BICUBIC+M_REGULAR);
         MbufFree(MilOriginalImage);

         // Adds the frame to the overlay images. Only pixels that are not transparent are copied.
         MbufCopyCond(MilFrameImage, m_MilOverlayPictures[i], MilFrameImage, M_NOT_EQUAL, FRAME_TRANSPARENT_COLOR);
         }

      // Free the frame image.
      MbufFree(MilFrameImage);

      // Precompute position to copy the overlay images (top-right corner of overlay).
      m_OverlayOffsetX = m_SizeX - PictureSizeX - FRAME_BORDER;
      m_OverlayOffsetY = FRAME_BORDER;
      }

   // Select image to display.
   MdispSelect(m_MilDisplay, m_MilDisplayImage);

   // Prepare for overlay annotations.
   MdispControl(m_MilDisplay, M_OVERLAY, M_ENABLE);
   MdispInquire(m_MilDisplay, M_OVERLAY_ID, &m_MilOverlayImage);
   }

//*****************************************************************************
// Disables the overlay and hide the display window.
//*****************************************************************************
void CExampleInterface::HideDisplay()
   {
   // Disable overlay.
   m_MilOverlayImage = M_NULL;
   MdispControl(m_MilDisplay, M_OVERLAY, M_DISABLE);

   // Hide display.
   MdispSelect(m_MilDisplay, M_NULL);
   }

//*****************************************************************************
// Used in main code to paste an image in the overlay according to the current
// action the user should be doing.
//*****************************************************************************
void CExampleInterface::CopyInOverlay(EOverlayImage eOverlayImage)
   {
#if ENABLE_IMAGES_IN_OVERLAY
   MbufCopyClip(m_MilOverlayPictures[eOverlayImage], m_MilOverlayImage, m_OverlayOffsetX, m_OverlayOffsetY);
#endif
   }

//*****************************************************************************
// Used by subclasses to set image size.
//*****************************************************************************
void CExampleInterface::SetImageSize(MIL_INT SizeX, MIL_INT SizeY)
   {
   m_SizeX = SizeX;
   m_SizeY = SizeY;
   }

//*****************************************************************************
// Returns true if MIL objects in the base class have been allocated successfully.
//*****************************************************************************
bool CExampleInterface::IsValidBase() const
   {
   bool IsValid = ( m_MilApplication != M_NULL &&
                    m_MilSystem      != M_NULL &&
                    m_MilDisplay     != M_NULL );
   return IsValid;
   }

//*****************************************************************************
// Chooses the size of the images to paste in the overlay according to the
// camera image size.
//*****************************************************************************
void CExampleInterface::DeterminePictureSize(MIL_INT* pPictureSizeX, MIL_INT* pPictureSizeY) const
   {
   // Initialize size with actual picture size.
   MIL_INT PictureSizeX = MbufDiskInquire(FRAME_IMAGE_FILE, M_SIZE_X, M_NULL);
   MIL_INT PictureSizeY = MbufDiskInquire(FRAME_IMAGE_FILE, M_SIZE_Y, M_NULL);

   MIL_DOUBLE ResizeFactor = 1.0;

   // Both PictureSizeX and PictureSizeY must be smaller than 1/4 the camera image size.
   if (PictureSizeX > m_SizeX * MAX_PICTURE_RATIO)
      {
      ResizeFactor = m_SizeX * MAX_PICTURE_RATIO / PictureSizeX;
      }
   if (PictureSizeY > m_SizeY * MAX_PICTURE_RATIO)
      {
      MIL_DOUBLE ResizeFactorY = m_SizeY * MAX_PICTURE_RATIO / PictureSizeY;
      if (ResizeFactorY < ResizeFactor)
         ResizeFactor = ResizeFactorY;
      }

   // Fill output variables.
   *pPictureSizeX = static_cast<MIL_INT>( PictureSizeX * ResizeFactor );
   *pPictureSizeY = static_cast<MIL_INT>( PictureSizeY * ResizeFactor );
   }
