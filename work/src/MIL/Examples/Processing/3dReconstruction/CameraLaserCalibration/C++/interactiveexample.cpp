//***************************************************************************************
//
// File name: interactiveexample.cpp
//
// Synopsis:  Implementation for CInteractiveExample.
//

#include <mil.h>
#include "interactiveexample.h"

//*****************************************************************************
// Constructor.
//*****************************************************************************
CInteractiveExample::CInteractiveExample()
: CExampleInterface(M_SYSTEM_DEFAULT, M_GRAB),
  m_MilDigitizer(M_NULL)
   {
   // Allocate digitizer.
   MdigAlloc(GetMilSystem(), M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &m_MilDigitizer);

   if (m_MilDigitizer != M_NULL)
      {
      // Get image size from digitizer (from DCF).
      MIL_INT SizeX = MdigInquire(m_MilDigitizer, M_SIZE_X, M_NULL);
      MIL_INT SizeY = MdigInquire(m_MilDigitizer, M_SIZE_Y, M_NULL);
      SetImageSize(SizeX, SizeY);
      }
   }

//*****************************************************************************
// Destructor, free subclass-specific MIL objects.
//*****************************************************************************
CInteractiveExample::~CInteractiveExample()
   {
   if (m_MilDigitizer != M_NULL)
      MdigFree(m_MilDigitizer);
   }

//*****************************************************************************
// Indicates if all MIL objects are initialized correctly.
//*****************************************************************************
bool CInteractiveExample::IsValid() const
   {
   bool IsValid = IsValidBase();
   if (m_MilDigitizer == M_NULL)
      IsValid = false;
   return IsValid;
   }

//*****************************************************************************
// Used to pause the execution in the stand-alone version, does nothing here.
//*****************************************************************************
void CInteractiveExample::PauseInStandAloneMode() const
   {
   // Do nothing in interactive mode.
   }

//*****************************************************************************
// Explain how to adjust M_MIN_INTENSITY value.
//*****************************************************************************
void CInteractiveExample::PrintExplanationForMinIntensity() const
   {
   MosPrintf(MIL_TEXT("Activate the laser line and adjust the camera's aperture (or exposure) to\n")
             MIL_TEXT("easily distinguish the laser line from the background. Then, adjust the\n")
             MIL_TEXT("minimum intensity value to extract the laser line correctly.\n\n")

             MIL_TEXT("Extracted laser line is displayed in green.\n")
             MIL_TEXT("Use '1' and '2' to decrease/increase M_MIN_INTENSITY in increments of 10,\n")
             MIL_TEXT("or  '4' and '5' to use increments of 1.\n")
             MIL_TEXT("Press <Enter> when you are satisfied with the extraction results.\n\n"));
   }

//*****************************************************************************
// Adjust M_MIN_INTENSITY value when user use (1,2,4,5).
//*****************************************************************************
bool CInteractiveExample::AskMinIntensityAdjust(MIL_INT* pMinIntensity)
   {
   const MIL_INT BIG_INCREMENT   = 10;
   const MIL_INT SMALL_INCREMENT = 1;

   // The while loop is to empty the IO buffer, if necessary.
   bool AskQuit = false;
   while (MosKbhit())
      {
      switch (MosGetch())
         {
         case MIL_TEXT('1'):
            *pMinIntensity -= BIG_INCREMENT;
            break;

         case MIL_TEXT('2'):
            *pMinIntensity += BIG_INCREMENT;
            break;

         case MIL_TEXT('4'):
            *pMinIntensity -= SMALL_INCREMENT;
            break;

         case MIL_TEXT('5'):
            *pMinIntensity += SMALL_INCREMENT;
            break;

         case MIL_TEXT('\r'): // <Enter>
         case MIL_TEXT('\n'): // <Enter>
            AskQuit = true;
            break;
         }

      // Bring back M_MIN_INTENSITY in the range [0,255]
      if (*pMinIntensity < 0)
         *pMinIntensity = 0;
      else if (*pMinIntensity > 255)
         *pMinIntensity = 255;
      }

   MosPrintf(MIL_TEXT("\rM_MIN_INTENSITY: %3d"), *pMinIntensity);
   return AskQuit;
   }

//*****************************************************************************
// Prompts user for yes/no
//*****************************************************************************
bool CInteractiveExample::AskIfFeatureExtractionAccurate()
   {
   return AskYesNo(MIL_TEXT("Is the feature extraction accurate (y/n)? "));
   }

//*****************************************************************************
// Prompts user for yes/no
//*****************************************************************************
bool CInteractiveExample::AskIfCameraCalibrationAccurate()
   {
   return AskYesNo(MIL_TEXT("Is the camera calibration accurate (y/n)? "));
   }

//*****************************************************************************
// Prompts user for yes/no
//*****************************************************************************
bool CInteractiveExample::AskIfLineExtractionAccurate()
   {
   return AskYesNo(MIL_TEXT("Is the laser line extraction accurate (y/n)? "));
   }

//*****************************************************************************
// Prompts user for yes/no
//*****************************************************************************
bool CInteractiveExample::AskIfLaserCalibrationAccurate()
   {
   return AskYesNo(MIL_TEXT("Is the calibration of the 3d reconstruction setup accurate (y/n)? "));
   }

//*****************************************************************************
// Checks if there is a camera calibration file present in the current directory.
// If so, and if it is a valid camera calibration, asks the user if he wants to
// use it instead of calibrating the camera again.
//*****************************************************************************
MIL_ID CInteractiveExample::TryToReloadCameraCalibration(const MIL_TEXT_CHAR* CalibrationFileName) const
   {
   // This will be the calibration context to return.
   MIL_ID MilCalibration = M_NULL;

   // First, let's look if there is a calibration file in local directory.
   // Try to reload it, and if it works, ask user if he wants it.
   MappControl(M_DEFAULT, M_ERROR, M_PRINT_DISABLE);
   McalRestore(CalibrationFileName, GetMilSystem(), M_DEFAULT, &MilCalibration);
   MappControl(M_DEFAULT, M_ERROR, M_PRINT_ENABLE);

   if (MilCalibration != M_NULL)
      {
      // Calibration context has been reloaded. Verify that it is calibrated in 3d.
      bool UseReloadedCalibration = false;
      MIL_INT CalibrationStatus = McalInquire(MilCalibration, M_CALIBRATION_STATUS, M_NULL);
      MIL_INT CalibrationMode   = McalInquire(MilCalibration, M_CALIBRATION_MODE  , M_NULL);

      if ( CalibrationStatus == M_CALIBRATED &&
           (CalibrationMode == M_TSAI_BASED || CalibrationMode == M_3D_ROBOTICS) )
         {
         // Calibration is appropriate, ask user if he wants to use it.
         MosPrintf(MIL_TEXT("Camera calibration file '"));
         MosPrintf(CalibrationFileName);
         MosPrintf(MIL_TEXT("' has been found in the current\ndirectory. "));
         UseReloadedCalibration = AskYesNo(MIL_TEXT("Do you want to reload this calibration context (y/n)? "));
         }

      if (!UseReloadedCalibration)
         {
         // Calibration is not appropriate or user does not want to use it, so free it.
         McalFree(MilCalibration);
         MilCalibration = M_NULL;
         MosPrintf(SEPARATOR);
         }
      }

   return MilCalibration;
   }

//*****************************************************************************
// Grabs continuously until user presses a key.
//*****************************************************************************
void CInteractiveExample::GrabCalibrationGrid()
   {
   MosPrintf(MIL_TEXT("Put the 15x16 calibration grid at Z=0 mm.\n")
             MIL_TEXT("Press <Enter> to calibrate.\n\n"));

   MdigGrabContinuous(m_MilDigitizer, GetMilDisplayImage());
   MosGetch();
   MdigHalt(m_MilDigitizer);
   }

//*****************************************************************************
// Grabs a single image.
//*****************************************************************************
void CInteractiveExample::GrabLaserLineToAdjustIntensity()
   {
   MdigGrab(m_MilDigitizer, GetMilDisplayImage());
   }

//*****************************************************************************
// Grabs continuously until user presses a key.
//*****************************************************************************
bool CInteractiveExample::GrabCalibrationLaserLine(MIL_INT ReferencePlaneIndex, MIL_DOUBLE CalibrationDepth, bool ShouldAskIfFinished)
   {
   // Prints message according to calibration mode (to account for the minimum number of reference planes to use).
   MosPrintf(MIL_TEXT("Set up the reference plane #%d at Z=%.3g mm.\n"), ReferencePlaneIndex, CalibrationDepth);
   if (ShouldAskIfFinished)
      MosPrintf(MIL_TEXT("Press <Enter> to add this plane, or 's' to stop adding reference planes.\n\n"));
   else
      MosPrintf(MIL_TEXT("Press <Enter> to add this plane.\n\n"));

   // Grabs until a key is pressed.
   MdigGrabContinuous(m_MilDigitizer, GetMilDisplayImage());
   MIL_INT UserChar = MosGetch();
   MdigHalt(m_MilDigitizer);

   // If it is possible that there are no more plane AND the user pressed 's', indicates
   // to caller that we are ready to call M3dmapCalibrate().
   bool ReadyToCalibrate = false;
   if (ShouldAskIfFinished && UserChar == MIL_TEXT('s'))
      {
      ReadyToCalibrate = true;

      if (ReferencePlaneIndex == 1)
         {
         MosPrintf(MIL_TEXT("Since a single reference plane is used, the laser plane is assumed to be\n")
                   MIL_TEXT("vertical. Press <Enter> to continue.\n\n"));
         MosGetch();
         }
      }

   return ReadyToCalibrate;
   }

//*****************************************************************************
// Prompts user for yes/no
//*****************************************************************************
bool CInteractiveExample::AskYesNo(const MIL_TEXT_CHAR* QuestionString)
   {
   MosPrintf(QuestionString);
   while (true)
      {
      switch (MosGetch())
         {
         case MIL_TEXT('y'):
            MosPrintf(MIL_TEXT("YES\n\n"));
            return true;

         case MIL_TEXT('n'):
            MosPrintf(MIL_TEXT("NO\n\n"));
            return false;
         }
      }
   }
