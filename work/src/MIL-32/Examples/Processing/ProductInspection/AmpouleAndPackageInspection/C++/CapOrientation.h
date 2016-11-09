//***************************************************************************************
// 
// File name: CapOrientation.h
//
// Synopsis: This file contains the declaration of the CHorizontalAlignment class
//           which is the inspection task used to find the horizontal alignment.
//

#ifndef CAP_ORIENTATION_H
#define CAP_ORIENTATION_H

#include "MeasurementInspectionTask.h"

class CCapOrientation : public CMeasInspectionTask
   {
   public:

      // Constructor.
      CCapOrientation(MIL_CONST_TEXT_PTR MeasMarkerPath, MIL_DOUBLE ExpectedWidth, MIL_DOUBLE WidthTolerance, MIL_INT ColorConversion = M_NONE, CInspectionTask* FixtureProvider = M_NULL, CInspectionTask* ImageProvider = M_NULL);

      // Destructor.
      virtual ~CCapOrientation();

      // Drawing functions
      virtual void DrawGraphicalResult(MIL_ID MilGraContext, MIL_ID MilDest);
      virtual void DrawTextResult(MIL_ID MilGraContext, MIL_ID MilDest);
      
   protected:

   private:

      // Inspection function.
      virtual bool Calculate(MIL_ID MilImage);

      // Result of the measurement
      MIL_DOUBLE m_Width;
      MIL_DOUBLE m_ExpectedWidth;
      MIL_DOUBLE m_WidthTolerance;
   };

#endif // SEPARATOR_INSPECTION_H