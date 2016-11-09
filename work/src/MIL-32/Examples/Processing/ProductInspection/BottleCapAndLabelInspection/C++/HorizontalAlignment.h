//***************************************************************************************
// 
// File name: HorizontalAlignment.h
//
// Synopsis: This file contains the declaration of the CHorizontalAlignment class
//           which is the inspection task used to find the horizontal alignment.
//

#ifndef HORIZONTAL_ALIGNMENT_H
#define HORIZONTAL_ALIGNMENT_H

#include "MeasurementInspectionTask.h"

class CHorizontalAlignment : public CMeasInspectionTask
   {
   public:

      // Constructor.
      CHorizontalAlignment(MIL_CONST_TEXT_PTR MeasMarkerPath, MIL_INT ColorConversion = M_NONE, CInspectionTask* FixtureProvider = M_NULL, CInspectionTask* ImageProvider = M_NULL);

      // Destructor.
      virtual ~CHorizontalAlignment();

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
   };

#endif // HORIZONTAL_ALIGNMENT_H