//***************************************************************************************
// 
// File name: ProductTypeIdentification.cpp
//
// Synopsis: This file contains the implementation of the CproductTypeIdentification class
//           which is the inspection task used to identify what is the type of product.
//

#include <mil.h>
#include "ProductTypeIdentification.h"

//*****************************************************************************
// Constants.
//*****************************************************************************
static const MIL_INT NB_PRODUCT = 2; 
static MIL_CONST_TEXT_PTR PRODUCT_LABEL[NB_PRODUCT] = 
   {
   MIL_TEXT("Extra Strength"),
   MIL_TEXT("Cold")
   };


//*****************************************************************************
// Constructor.
//*****************************************************************************
CProductTypeIdentification::CProductTypeIdentification(MIL_CONST_TEXT_PTR ColContextPath, const MIL_INT* ExpectedMatches, MIL_INT ColorConversion /* = M_NONE */, CInspectionTask* FixtureProvider /* = M_NULL */, CInspectionTask* ImageProvider /* = M_NULL */, CInspectionTask* RegionProvider /* = M_NULL */)
 : CColorMatchTask(ColContextPath, ExpectedMatches, ColorConversion, FixtureProvider, ImageProvider, RegionProvider)
   {
   }

//*****************************************************************************
// Destructor.
//*****************************************************************************
CProductTypeIdentification::~CProductTypeIdentification()
   {
   }


//*****************************************************************************
// Get the best match label
//*****************************************************************************
MIL_CONST_TEXT_PTR CProductTypeIdentification::GetBestMatchLabel(MIL_INT MatchIdx) const
   {
   if(MatchIdx == -1)
      return MIL_TEXT("None");
   else
      return PRODUCT_LABEL[MatchIdx];
   }
