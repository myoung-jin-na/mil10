//***************************************************************************************
// 
// File name: CapColorIdentification.cpp
//
// Synopsis: This file contains the implementation of the CCapColorInspection class
//           which is the inspection task used to check if the cap color is ok.
//

#include <mil.h>
#include "CapColorIdentification.h"

//*****************************************************************************
// Constants.
//*****************************************************************************
static const MIL_INT NB_PRODUCT = 2; 
static MIL_CONST_TEXT_PTR PRODUCT_LABEL[NB_PRODUCT] = 
   {
   MIL_TEXT("Red"),
   MIL_TEXT("White")
   };

//*****************************************************************************
// Constructor.
//*****************************************************************************
CCapColorInspection::CCapColorInspection(MIL_CONST_TEXT_PTR ColContextPath, const MIL_INT* ExpectedMatches, MIL_INT ColorConversion /* = M_NONE */, CInspectionTask* FixtureProvider /* = M_NULL */, CInspectionTask* ImageProvider /* = M_NULL */, CInspectionTask* RegionProvider /* = M_NULL */)
 : CColorMatchTask(ColContextPath, ExpectedMatches, ColorConversion, FixtureProvider, ImageProvider, RegionProvider)
   {
   }

//*****************************************************************************
// Destructor.
//*****************************************************************************
CCapColorInspection::~CCapColorInspection()
   {
   }


//*****************************************************************************
// Get the best match label
//*****************************************************************************
MIL_CONST_TEXT_PTR CCapColorInspection::GetBestMatchLabel(MIL_INT MatchIdx) const
   {
   if(MatchIdx == -1)
      return MIL_TEXT("None");
   else
      return PRODUCT_LABEL[MatchIdx];
   }

