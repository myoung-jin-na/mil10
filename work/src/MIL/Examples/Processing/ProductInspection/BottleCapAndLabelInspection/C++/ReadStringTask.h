//***************************************************************************************
// 
// File name: ReadStringTask.h
//
// Synopsis: This file contains the declaration of the CReadStringTask class
//           which is the inspection task used to read the a string.
//

#ifndef READ_STRING_TASK_H
#define READ_STRING_TASK_H

#include "ReadStrTask.h"

class CReadStringTask : public CReadStrTask
   {
   public:

      // Constructor.
      CReadStringTask(MIL_CONST_TEXT_PTR StrContextPath, MIL_INT ColorConversion = M_NONE, CInspectionTask* FixtureProvider = M_NULL, CInspectionTask* ImageProvider = M_NULL, CInspectionTask* RegionProvider = M_NULL);

      // Destructor.
      virtual ~CReadStringTask();

      // Drawing functions.
      virtual void DrawTextResult(MIL_ID MilGraContext, MIL_ID MilDest);
      
   protected:

   private:
   };

#endif // READ_STRING_TASK_H