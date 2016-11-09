//***************************************************************************************
// 
// File name: LidIdentification.h
//
// Synopsis: This file contains the declaration of the CLidIdentification class
//           which is the inspection task used to verify that the lid is of the correct type.
//

#ifndef LID_IDENTIFICATION_H
#define LID_IDENTIFICATION_H

#include "PatternMatchingTask.h"

class CLidIdentification : public CPatMatchingTask
   {
   public:
      // Constructor.
      CLidIdentification(MIL_CONST_TEXT_PTR PatContextPath, MIL_INT ColorConversion = M_NONE, CInspectionTask* FixtureProvider = M_NULL, CInspectionTask* ImageProvider = M_NULL, CInspectionTask* RegionProvider = M_NULL);

      // Destructor.
      virtual ~CLidIdentification();
            
      // Drawing functions.
      virtual void DrawTextResult(MIL_ID MilGraContext, MIL_ID MilDest);
   };

#endif // LID_IDENTIFICATION_H