//***************************************************************************************
// 
// File name: CapColorIdentification.h
//
// Synopsis: This file contains the declaration of the CCapColorInspection class
//           which is the inspection task used to check if the cap color is ok.
//

#ifndef CAP_COLOR_IDENTIFICATION_H
#define CAP_COLOR_IDENTIFICATION_H

#include "ColorMatchTask.h"

class CCapColorInspection : public CColorMatchTask
   {
   public:

      // Constructor.
      CCapColorInspection(MIL_CONST_TEXT_PTR ColContextPath, const MIL_INT* ExpectedMatches, MIL_INT ColorConversion = M_NONE, CInspectionTask* FixtureProvider = M_NULL, CInspectionTask* ImageProvider = M_NULL, CInspectionTask* RegionProvider = M_NULL);

      // Destructor.
      virtual ~CCapColorInspection();
           
   protected:

   private:

      // Function to get the label of the sample.
      virtual MIL_CONST_TEXT_PTR GetProductLabel() const { return MIL_TEXT("Cap"); }
      virtual MIL_CONST_TEXT_PTR GetBestMatchLabel(MIL_INT MatchIdx) const;   
   };

#endif // CAP_COLOR_IDENTIFICATION_H