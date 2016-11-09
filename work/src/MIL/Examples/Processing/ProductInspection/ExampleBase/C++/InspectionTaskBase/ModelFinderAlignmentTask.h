//***************************************************************************************
// 
// File name: ModelFinderAlignmentTask.h
//
// Synopsis: This file contains the declaration of the CModAlignmentTask class
//           which is the base class for any alignment task.
//

#ifndef MODEL_FINDER_ALIGMNENT_TASK_H
#define MODEL_FINDER_ALIGMNENT_TASK_H

#include "HighLevelInspectionTask.h"

class  CModAlignmentTask: public CHighLevelInspectionTask
   {
   public:

      // Constructor.
      CModAlignmentTask(MIL_CONST_TEXT_PTR ModContextPath, MIL_INT ColorConversion = M_NONE, CInspectionTask* FixtureProvider = M_NULL, CInspectionTask* ImageProvider = M_NULL);

      // Destructor.
      virtual ~CModAlignmentTask();
      
      // Inspection function.
      virtual bool Calculate(MIL_ID MilImage);
      
      // Drawing functions.
      virtual void DrawGraphicalResult(MIL_ID MilGraContext, MIL_ID MilDest);
      virtual void DrawTextResult(MIL_ID MilGraContext, MIL_ID MilDest){};
      
      // Get number function.
      MIL_INT GetNumberFound() const {return m_NumberFound;}

   protected:
                 
   private:      

      // Function to restore the context.
      virtual void Restore(MIL_ID MilSystem, MIL_CONST_TEXT_PTR ContextPath, MIL_ID *pMilContext, MIL_ID *pMilResult)
         {
         MmodRestore(ContextPath, MilSystem, M_DEFAULT, pMilContext);
         MmodAllocResult(MilSystem, M_DEFAULT, pMilResult);
         }

      // Function to preprocess the context.
      virtual void Preprocess()
         {
         MmodPreprocess(MilContext(), M_DEFAULT);
         }

      // Function that sets the output fixture.
      virtual void SetFixture();

      // The number of occurences found.
      MIL_INT m_NumberFound;
   };
#endif // MODEL_FINDER_ALIGMNENT_TASK_H