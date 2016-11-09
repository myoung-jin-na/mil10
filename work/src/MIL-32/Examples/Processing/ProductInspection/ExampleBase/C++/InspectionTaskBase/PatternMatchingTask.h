//***************************************************************************************
// 
// File name: PatternMatchingTask.h
//
// Synopsis: This file contains the declaration of the CPatMatchingTask class
//           which is the base class for any pattern matching task.
//

#ifndef PATTERN_MATCHING_TASK_H
#define PATTERN_MATCHING_TASK_H

#include "HighLevelInspectionTask.h"

class CPatMatchingTask: public CHighLevelInspectionTask
   {
   public:

      // Constructor.
      CPatMatchingTask(MIL_CONST_TEXT_PTR PatContextPath, MIL_INT ColorConversion = M_NONE, CInspectionTask* FixtureProvider = M_NULL, CInspectionTask* ImageProvider = M_NULL);

      // Destructor.
      virtual ~CPatMatchingTask();

      // Inspection function.
      virtual bool Calculate(MIL_ID MilImage);
                  
      // Drawing functions
      virtual void DrawGraphicalResult(MIL_ID MilGraContext, MIL_ID MilDest);
      virtual void DrawTextResult(MIL_ID MilGraContext, MIL_ID MilDest){};
      
      // Get number function.
      MIL_INT GetNumberFound() const {return m_NumberFound;}

   protected:
           
   private:

      // Function to restore the context
      virtual void Restore(MIL_ID MilSystem, MIL_CONST_TEXT_PTR ContextPath, MIL_ID *pMilContext, MIL_ID *pMilResult)
         {
         MpatRestore(MilSystem, ContextPath, pMilContext);
         MpatAllocResult(MilSystem, M_DEFAULT, pMilResult);
         }

      // Function to preprocess the context
      virtual void Preprocess()
         {
         MpatPreprocModel(M_NULL, MilContext(), M_DEFAULT);
         }

      // Function that sets the output fixture.
      virtual void SetFixture();

      // The number of occurences found
      MIL_INT m_NumberFound;
   };

#endif // PATTERN_MATCHING_TASK_H