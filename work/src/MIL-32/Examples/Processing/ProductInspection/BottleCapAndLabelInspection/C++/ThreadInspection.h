//***************************************************************************************
// 
// File name: ThreadInspection.h
//
// Synopsis: This file contains the declaration of the CThreadInspection class
//           which is the inspection task used to check if the thread of the bottle is ok.
//

#ifndef THREAD_INSPECTION_H
#define THREAD_INSPECTION_H

#include "MetrologyInspectionTask.h"

class CThreadInspection : public CMetInspectionTask
   {
   public:

      // Construc tor.
      CThreadInspection(MIL_CONST_TEXT_PTR MetContextPath, MIL_INT ColorConversion = M_NONE, CInspectionTask* FixtureProvider = M_NULL, CInspectionTask* ImageProvider = M_NULL);

      // Destructor.
      virtual ~CThreadInspection();

      // Drawing functions.
      virtual void DrawGraphicalResult(MIL_ID MilGraContext, MIL_ID MilDest);
      virtual void DrawTextResult(MIL_ID MilGraContext, MIL_ID MilDest);

   protected:

   private:   
   };

#endif // THREAD_INSPECTION_H