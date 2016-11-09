//***************************************************************************************
// 
// File name: CapScrewInspection_CapAndContent.h
//
// Synopsis: This file contains the declaration of the CCapScrewInspection class
//           which is the inspection task used to check if the cap is well screwed.
//

#ifndef CAP_SCREW_INSPECTION_CAP_AND_CONTENT_H
#define CAP_SCREW_INSPECTION_CAP_AND_CONTENT_H

#include "MeasurementInspectionTask.h"

class CCapScrewInspection : public CMeasInspectionTask
   {
   public:

      // Constructor.
      CCapScrewInspection(MIL_CONST_TEXT_PTR MeasContextPath, MIL_DOUBLE PositionYTolerance, MIL_DOUBLE AngleTolerance, MIL_INT ColorConversion = M_NONE, CInspectionTask* FixtureProvider = M_NULL, CInspectionTask* ImageProvider = M_NULL);

      // Destructor.
      virtual ~CCapScrewInspection();
      
      // Drawing functions.
      virtual void DrawGraphicalResult(MIL_ID MilGraContext, MIL_ID MilDest);
      virtual void DrawTextResult(MIL_ID MilGraContext, MIL_ID MilDest);
            
   protected:

   private:

      // Inspection function.
      virtual bool Calculate(MIL_ID MilImage);

      // Function that sets the output fixture.
      virtual void SetFixture();

      // The inspection results.
      MIL_DOUBLE m_PosX;
      MIL_DOUBLE m_PosY;
      MIL_DOUBLE m_Angle;

      // The position tolerance of the inspection
      MIL_DOUBLE m_PositionTolerance;

      // The angle tolerance of the inspection
      MIL_DOUBLE m_AngleTolerance;   
   };

#endif // CAP_SCREW_INSPECTION_CAP_AND_CONTENT_H