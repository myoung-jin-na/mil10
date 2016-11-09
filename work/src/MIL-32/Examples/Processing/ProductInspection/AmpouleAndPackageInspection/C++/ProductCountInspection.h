//***************************************************************************************
// 
// File name: ProductCountInspection.h
//
// Synopsis: This file contains the declaration of the CHorizontalAlignment class
//           which is the inspection task used to find the horizontal alignment.
//

#ifndef PRODUCT_COUNT_INSPECTION_H
#define PRODUCT_COUNT_INSPECTION_H

#include "PatternMatchingTask.h"

class CProductCountInspection : public CPatMatchingTask
   {
   public:

      // Constructor.
      CProductCountInspection(MIL_CONST_TEXT_PTR PatModelPath, MIL_INT ExpectedNumber, MIL_INT ColorConversion = M_NONE, CInspectionTask* FixtureProvider = M_NULL, CInspectionTask* ImageProvider = M_NULL);

      // Destructor.
      virtual ~CProductCountInspection();

      // Drawing functions
      virtual void DrawGraphicalResult(MIL_ID MilGraContext, MIL_ID MilDest);
      virtual void DrawTextResult(MIL_ID MilGraContext, MIL_ID MilDest);
      
   protected:

   private:

      // The expected number of occurences to find
      MIL_INT m_ExpectedNumber;

      // Inspection function.
      virtual bool Calculate(MIL_ID MilImage);
   };

#endif // SEPARATOR_INSPECTION_H