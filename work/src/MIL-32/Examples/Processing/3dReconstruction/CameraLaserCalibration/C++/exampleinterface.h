//***************************************************************************************
//
// File name: exampleinterface.h
//
// Synopsis:  Base class that manages MIL application, system and digitizer. Also provides
//            an interface for grabbing images and prompting the user.
//

#ifndef EXAMPLE_INTERFACE_H
#define EXAMPLE_INTERFACE_H

// Directory containing all images used for the example.
#define EXAMPLE_IMAGE_PATH    M_IMAGE_PATH MIL_TEXT("CameraLaserCalibration/")

// Simple separator line for console output.
static const MIL_TEXT_CHAR* const SEPARATOR = MIL_TEXT("--------------------\n\n");

//*****************************************************************************
// Base class for the example. Provides an interface to manage interaction with
// the user, which includes image grabbing/reloading and IO (asking questions
// to the user and providing informational messages).
//*****************************************************************************
class CExampleInterface
   {
   public:
      enum EOverlayImage
         {
         eCalibrateCameraImage = 0,
         eAdjustMinIntensityImage,
         eCalibrateLaserImage,
         eNbOverlayImages // used only to know the number of images
         };

      static bool AskInteractiveExample();

      CExampleInterface(MIL_CONST_TEXT_PTR SystemDescriptor, MIL_INT64 BufferAttribute);
      virtual ~CExampleInterface();

      MIL_INT AskCalibrationMode();
      void    ShowDisplay();
      void    HideDisplay();
      void    CopyInOverlay(EOverlayImage eOverlayImage);

      virtual bool    IsValid() const = 0;

      virtual void    PauseInStandAloneMode() const = 0;
      virtual void    PrintExplanationForMinIntensity() const = 0;

      virtual bool    AskMinIntensityAdjust(MIL_INT* pMinIntensity) = 0;
      virtual bool    AskIfFeatureExtractionAccurate() = 0;
      virtual bool    AskIfCameraCalibrationAccurate() = 0;
      virtual bool    AskIfLineExtractionAccurate() = 0;
      virtual bool    AskIfLaserCalibrationAccurate() = 0;

      virtual MIL_ID  TryToReloadCameraCalibration(const MIL_TEXT_CHAR* CalibrationFileName) const = 0;

      virtual void    GrabCalibrationGrid() = 0;
      virtual void    GrabLaserLineToAdjustIntensity() = 0;
      virtual bool    GrabCalibrationLaserLine(MIL_INT CalibrationPlane, MIL_DOUBLE CalibrationDepth, bool ShouldAskIfFinished) = 0;

      inline MIL_ID   GetMilSystem() const;
      inline MIL_ID   GetMilDisplay() const;
      inline MIL_ID   GetMilDisplayImage() const;
      inline MIL_ID   GetMilOverlayImage() const;

   protected:
      void SetImageSize(MIL_INT SizeX, MIL_INT SizeY);
      bool IsValidBase() const;

      inline MIL_INT GetCalibrationMode() const;

   private:
      // Disallowed operators
      CExampleInterface(const CExampleInterface&);
      CExampleInterface& operator=(const CExampleInterface&);

      void DeterminePictureSize(MIL_INT* pPictureSizeX, MIL_INT* pPictureSizeY) const;

      MIL_INT64 m_BufferAttribute;  // M_GRAB or M_NULL (used with MbufAlloc2d()).
      MIL_INT   m_SizeX;            // Width of camera image.
      MIL_INT   m_SizeY;            // Height of camera image.
      MIL_INT   m_OverlayOffsetX;   // OffsetX used with MbufCopyClip() to paste an image in the overlay.
      MIL_INT   m_OverlayOffsetY;   // OffsetY used with MbufCopyClip() to paste an image in the overlay.
      MIL_INT   m_CalibrationMode;  // M_DEPTH_CORRECTION or M_CALIBRATED_CAMERA_LINEAR_MOTION.

      MIL_ID    m_MilApplication;   // ID obtained with MappAlloc().
      MIL_ID    m_MilSystem;        // ID obtained with MsysAlloc(M_DEFAULT, ).
      MIL_ID    m_MilDisplay;       // ID obtained with MdispAlloc().
      MIL_ID    m_MilDisplayImage;  // ID obtained with MbufAlloc2d().
      MIL_ID    m_MilOverlayImage;  // ID obtained with MdispInquire(), must not be freed.

      MIL_ID    m_MilOverlayPictures[eNbOverlayImages];  // Array of image ID to be pasted in the overlay.
   };

//*****************************************************************************
// Returns system identifier to use.
//*****************************************************************************
inline MIL_ID CExampleInterface::GetMilSystem() const
   {
   return m_MilSystem;
   }

//*****************************************************************************
// Returns display identifier to use.
//*****************************************************************************
inline MIL_ID CExampleInterface::GetMilDisplay() const
   {
   return m_MilDisplay;
   }

//*****************************************************************************
// Returns image identifier selected to display.
//*****************************************************************************
inline MIL_ID CExampleInterface::GetMilDisplayImage() const
   {
   return m_MilDisplayImage;
   }

//*****************************************************************************
// Returns image identifier of the overlay.
//*****************************************************************************
inline MIL_ID CExampleInterface::GetMilOverlayImage() const
   {
   return m_MilOverlayImage;
   }

//*****************************************************************************
// Returns mode used with M3dmapAlloc().
//*****************************************************************************
inline MIL_INT CExampleInterface::GetCalibrationMode() const
   {
   return m_CalibrationMode;
   }

#endif
