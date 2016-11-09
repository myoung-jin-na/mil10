//***************************************************************************************
// 
// File name: MetrologyAlignment.h
//
// Synopsis: This file contains the declaration of the CMetrologyAlignment class
//           which is a task to measure some metrology features and output a fixture

#ifndef METROLOGY_ALIGNMENT_H
#define METROLOGY_ALIGNMENT_H

#include "MetrologyInspectionTask.h"

class CMetrologyAlignment : public CMetInspectionTask
   {
   public:

      // Constructor.
      CMetrologyAlignment(MIL_CONST_TEXT_PTR MetContextPath, MIL_INT ColorConversion = M_NONE, CInspectionTask* FixtureProvider = M_NULL, CInspectionTask* ImageProvider = M_NULL);

      // Destructor.
      virtual ~CMetrologyAlignment();

      // Inspection function.
      virtual bool Calculate(MIL_ID MilImage);
      
      // Drawing functions.
      virtual void DrawGraphicalResult(MIL_ID MilGraContext, MIL_ID MilDest);
      virtual void DrawTextResult(MIL_ID MilGraContext, MIL_ID MilDest);
      
   protected:

   private:

      // Function that sets the output fixture.
      virtual void SetFixture();
   };

#endif // METROLOGY_ALIGNMENT_H