//***************************************************************************************
// 
// File name: LabelAlignmentInspection.h
//
// Synopsis: This file contains the declaration of the CLabelAlignmentInspection class
//           which is the inspection task used to check if the alignment of the label is good.
//

#ifndef LABEL_ALIGNMENT_INSPECTION_H
#define LABEL_ALIGNMENT_INSPECTION_H

#include "MeasurementInspectionTask.h"

class CLabelAlignmentInspection : public CMeasInspectionTask
   {
   public:  

      // Constructor.
      CLabelAlignmentInspection(MIL_CONST_TEXT_PTR MeasContextPath, MIL_DOUBLE AngleTolerance, MIL_DOUBLE ExpectedPosY, MIL_DOUBLE PosYTolerance, MIL_INT ColorConversion = M_NONE, CInspectionTask* FixtureProvider = M_NULL, CInspectionTask* ImageProvider = M_NULL);

      // Destructor.
      virtual ~CLabelAlignmentInspection();
      
      // Drawing functions.
      virtual void DrawTextResult(MIL_ID MilGraContext, MIL_ID MilDest);
      virtual void DrawGraphicalResult(MIL_ID MilGraContext, MIL_ID MilDest);

   protected:

   private:

      // Inspection function.
      virtual bool Calculate(MIL_ID MilImage);

      // Function that sets the output fixture.
      virtual void SetFixture();

      // The result of the inspection.
      MIL_DOUBLE m_PosX;
      MIL_DOUBLE m_PosY;
      MIL_DOUBLE m_Angle;
            
      // The expected values.
      MIL_DOUBLE m_AngleTolerance;
      MIL_DOUBLE m_ExpectedPosY;
      MIL_DOUBLE m_PosYTolerance;
   
   };

#endif // LABEL_ALIGNMENT_INSPECTION_H