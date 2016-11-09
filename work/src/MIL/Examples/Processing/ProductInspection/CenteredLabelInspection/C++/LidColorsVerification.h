//***************************************************************************************
// 
// File name: LidColorsVerification.h
//
// Synopsis: This file contains the declaration of the CLidColorsVerification class
//           which is the inspection task used to verify the colors of the lid.
//

#ifndef LID_COLORS_VERIFICATION_H
#define LID_COLORS_VERIFICATION_H

#include "ColorMatchTask.h"

class CLidColorsVerification : public CColorMatchTask
   {
   public:
      // Constructor.
      CLidColorsVerification(MIL_CONST_TEXT_PTR ColContextPath, const MIL_INT* ExpectedMatches, MIL_INT ColorConversion = M_NONE, CInspectionTask* FixtureProvider = M_NULL, CInspectionTask* ImageProvider = M_NULL, CInspectionTask* RegionProvider = M_NULL);

      // Destructor.
      virtual ~CLidColorsVerification();
           
   protected:

   private:

      // Function to get the label of the sample.
      virtual MIL_CONST_TEXT_PTR GetProductLabel() const { return MIL_TEXT("Lid"); }
      virtual MIL_CONST_TEXT_PTR GetBestMatchLabel(MIL_INT MatchIdx) const;
   };

#endif // LID_COLORS_VERIFICATION_H