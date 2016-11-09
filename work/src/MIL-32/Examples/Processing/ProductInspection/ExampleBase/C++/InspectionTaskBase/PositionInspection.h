//***************************************************************************************
// 
// File name: PositionInspection.h
//
// Synopsis: This file contains the declaration of the CPositionInspection class
//           which is a measurement derived class that verifies if the position 
//           of the measurement is good.
//

#ifndef POSITION_INSPECTION_H
#define POSITION_INSPECTION_H

#include "MeasurementInspectionTask.h"

class CPositionInspection : public CMeasInspectionTask
   {
   public:

      // Constructor.
      CPositionInspection(MIL_CONST_TEXT_PTR MeasMarkerPath, MIL_CONST_TEXT_PTR PositionLabel, MIL_DOUBLE ExpectedPosition, MIL_DOUBLE PosTolerance, MIL_INT ColorConversion = M_NONE, CInspectionTask* FixtureProvider = M_NULL, CInspectionTask* ImageProvider = M_NULL);

      // Destructor.
      virtual ~CPositionInspection();

      // Drawing functions.
      virtual void DrawGraphicalResult(MIL_ID MilGraContext, MIL_ID MilDest);
      virtual void DrawTextResult(MIL_ID MilGraContext, MIL_ID MilDest);
      
   protected:

   private:

      // Inspection function.
      virtual bool Calculate(MIL_ID MilImage);

      // Result of the measurement
      MIL_DOUBLE m_Pos;
      
      // The expected value
      MIL_DOUBLE m_ExpectedPosition;
      MIL_DOUBLE m_PosTolerance;

      // The label of the position inspection
      MIL_TEXT_PTR m_PositionLabel;
   };

#endif // POSITION_INSPECTION_H