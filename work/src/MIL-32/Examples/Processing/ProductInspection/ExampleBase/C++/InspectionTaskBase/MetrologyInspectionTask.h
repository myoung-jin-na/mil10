//***************************************************************************************
// 
// File name: MetrologyInspectionTask.h
//
// Synopsis: This file contains the declaration of the CMetInspectionTask class
//           which is the base inspection task that uses metrology.

#ifndef METROLOGY_INSPECTION_TASK_H
#define METROLOGY_INSPECTION_TASK_H

#include "HighLevelInspectionTask.h"

class CMetInspectionTask : public CHighLevelInspectionTask
   {
   public:

      // Constructor.
      CMetInspectionTask(MIL_CONST_TEXT_PTR MetContextPath, MIL_INT ColorConversion = M_NONE, CInspectionTask* FixtureProvider = M_NULL, CInspectionTask* ImageProvider = M_NULL);

      // Destructor.
      virtual ~CMetInspectionTask();

      // Inspection function.
      virtual bool Calculate(MIL_ID MilImage);
      
      // Drawing functions.
      virtual void DrawGraphicalResult(MIL_ID MilGraContext, MIL_ID MilDest);
      virtual void DrawTextResult(MIL_ID MilGraContext, MIL_ID MilDest) = 0;
      
   protected:

      // Drawing functions.
      void DrawFeatureByName(MIL_ID MilGraContext, MIL_CONST_TEXT_PTR FeatureName, MIL_ID MilDest, MIL_DOUBLE Color);
      void DrawToleranceByName(MIL_ID MilGraContext, MIL_CONST_TEXT_PTR ToleranceName, MIL_ID MilDest, MIL_DOUBLE PassColor, MIL_DOUBLE FailColor, MIL_DOUBLE WarningColor);
     
   private:

      // Function to restore the context.
      virtual void Restore(MIL_ID MilSystem, MIL_CONST_TEXT_PTR ContextPath, MIL_ID *pMilContext, MIL_ID *pMilResult)
         {
         MmetRestore(ContextPath, MilSystem, M_DEFAULT, pMilContext);
         MmetAllocResult(MilSystem, M_DEFAULT, pMilResult);
         }
   };

#endif // METROLOGY_INSPECTION_TASK_H