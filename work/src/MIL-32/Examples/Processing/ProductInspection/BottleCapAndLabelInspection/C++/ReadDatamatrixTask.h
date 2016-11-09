//***************************************************************************************
// 
// File name: ReadDatamatrixTask.h
//
// Synopsis: This file contains the declaration of the CReadDatamatrixTask class
//           which is the inspection task used to read a datamatrix.
//

#ifndef READ_DATAMATRIX_TASK_H
#define READ_DATAMATRIX_TASK_H

#include "ReadCodeTask.h"

class CReadDatamatrixTask : public CReadCodeTask
   {
   public:

      // Constructor.
      CReadDatamatrixTask(MIL_CONST_TEXT_PTR CodeContextPath, MIL_INT ColorConversion = M_NONE, CInspectionTask* FixtureProvider = M_NULL, CInspectionTask* ImageProvider = M_NULL, CInspectionTask* RegionProvider = M_NULL);

      // Destructor.
      virtual ~CReadDatamatrixTask();
            
      // Drawing functions.
      virtual void DrawTextResult(MIL_ID MilGraContext, MIL_ID MilDest);
      
   protected:

   private: 
   };

#endif // READ_DATAMATRIX_TASK_H