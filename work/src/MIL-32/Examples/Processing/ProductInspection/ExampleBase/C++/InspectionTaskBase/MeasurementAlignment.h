//***************************************************************************************
// 
// File name: MeasurementAlignment.h
//
// Synopsis: This file contains the declaration of the CMeasAlignment class
//           which is the inspection task used to align an image based on a single measurement.
//

#ifndef MEAS_ALIGNMENT_H
#define MEAS_ALIGNMENT_H

#include "MeasurementInspectionTask.h"

class CMeasAlignment : public CMeasInspectionTask
   {
   public:

      // Constructor.
      CMeasAlignment(MIL_CONST_TEXT_PTR MeasMarkerPath, MIL_INT ColorConversion = M_NONE, CInspectionTask* FixtureProvider = M_NULL, CInspectionTask* ImageProvider = M_NULL);

      // Destructor.
      virtual ~CMeasAlignment();

      // Drawing functions.
      virtual void DrawGraphicalResult(MIL_ID MilGraContext, MIL_ID MilDest);
      virtual void DrawTextResult(MIL_ID MilGraContext, MIL_ID MilDest);
      
   protected:

   private:

      // Inspection function.
      virtual bool Calculate(MIL_ID MilImage);

      // Function that sets the output fixture.
      virtual void SetFixture();

      // Result of the measurement.
      MIL_DOUBLE m_PosX;
      MIL_DOUBLE m_PosY;
      MIL_DOUBLE m_Angle;
   };

#endif // HORIZONTAL_ALIGNMENT_H