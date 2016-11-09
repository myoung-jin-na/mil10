//***************************************************************************************
// 
// File name: MeasurementInspectionTask.h
//
// Synopsis: This file contains the declaration of the CMeasInspectionTask class.
//           
//

#ifndef MEASUREMENT_INSPECTION_TASK_H
#define MEASUREMENT_INSPECTION_TASK_H

#include "HighLevelInspectionTask.h"

class CMeasInspectionTask : public CHighLevelInspectionTask
   {
   public:

      // Constructor.
      CMeasInspectionTask(MIL_CONST_TEXT_PTR MeasContextPath, MIL_INT ColorConversion = M_NONE, CInspectionTask* FixtureProvider = M_NULL, CInspectionTask* ImageProvider = M_NULL);

      // Destructor.
      virtual ~CMeasInspectionTask();
      
      // Inspection function.
      virtual bool Calculate(MIL_ID MilImage);

      // Drawing functions.
      virtual void DrawGraphicalResult(MIL_ID MilGraContext, MIL_ID MilDest) = 0;
      virtual void DrawTextResult(MIL_ID MilGraContext, MIL_ID MilDest) = 0;

      // Get number function.
      MIL_INT GetNumberFound() const {return m_NumberFound;}
      
   protected:
                 
   private:

      // Function to restore the context
      virtual void Restore(MIL_ID MilSystem, MIL_CONST_TEXT_PTR ContextPath, MIL_ID *pMilContext, MIL_ID *pMilResult)
         {
         MmeasRestoreMarker(ContextPath, MilSystem, M_DEFAULT, pMilResult);
         }

      // The number of markers found.
      MIL_INT m_NumberFound;
   };

#endif // MEASUREMENT_INSPECTION_TASK_H