//***************************************************************************************
//
// File name: standaloneexample.h
//
// Synopsis:  Subclass implementing CExampleInterface that reloads images on disk and
//            hardcodes the answers to all user interaction.
//

#ifndef STANDALONE_EXAMPLE_H
#define STANDALONE_EXAMPLE_H

#include "exampleinterface.h"

//*****************************************************************************
// Subclass implementing CExampleInterface for the stand-alone case.
// Every grab function will reload an image from disk.
// Every "question" asked to the user will be automatically answered according
// to iteration counters to show different calibration situations.
//*****************************************************************************
class CStandAloneExample : public CExampleInterface
   {
   public:
      CStandAloneExample();
      virtual ~CStandAloneExample();

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
      MIL_INT m_CalibrationGridCounter;            // Iteration counter used in GrabCalibrationGrid().
      MIL_INT m_LaserLineToAdjustIntensityCounter; // Iteration counter used in AskMinIntensityAdjust().
      MIL_INT m_CalibrationLaserLineCounter;       // Iteration counter used in GrabCalibrationLaserLine().
      MIL_INT m_CameraCalibrationCounter;          // Iteration counter used in AskIfCameraCalibrationAccurate().
      MIL_INT m_LineExtractionCounter;             // Iteration counter used in AskIfLineExtractionAccurate().
      MIL_INT m_LaserCalibrationCounter;           // Iteration counter used in AskIfLaserCalibrationAccurate().
   };

#endif
