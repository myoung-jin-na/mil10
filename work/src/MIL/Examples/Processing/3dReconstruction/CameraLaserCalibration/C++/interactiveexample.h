//***************************************************************************************
//
// File name: interactiveexample.h
//
// Synopsis:  Subclass implementing CExampleInterface that uses live grabbing and the
//            console to prompt the user.
//

#ifndef INTERACTIVE_EXAMPLE_H
#define INTERACTIVE_EXAMPLE_H

#include "exampleinterface.h"

//*****************************************************************************
// Subclass implementing CExampleInterface for the interactive case.
// Every grab function will actually grab an image using a MIL digitizer.
// Every question will be asked to the user with MosGetch().
//*****************************************************************************
class CInteractiveExample : public CExampleInterface
   {
   public:
      CInteractiveExample();
      virtual ~CInteractiveExample();

      virtual bool    IsValid() const;

      virtual void    PauseInStandAloneMode() const;
      virtual void    PrintExplanationForMinIntensity() const;

      virtual bool    AskMinIntensityAdjust(MIL_INT* pMinIntensity);
      virtual bool    AskIfFeatureExtractionAccurate();
      virtual bool    AskIfCameraCalibrationAccurate();
      virtual bool    AskIfLineExtractionAccurate();
      virtual bool    AskIfLaserCalibrationAccurate();

      virtual MIL_ID  TryToReloadCameraCalibration(const MIL_TEXT_CHAR* CalibrationFileName) const;

      virtual void    GrabCalibrationGrid();
      virtual void    GrabLaserLineToAdjustIntensity();
      virtual bool    GrabCalibrationLaserLine(MIL_INT ReferencePlaneIndex, MIL_DOUBLE CalibrationDepth, bool ShouldAskIfFinished);

   private:
      static bool AskYesNo(const MIL_TEXT_CHAR* QuestionString);

      MIL_ID m_MilDigitizer;  // ID obtained with MdigAlloc().
   };

#endif
