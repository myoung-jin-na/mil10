//***************************************************************************************
// 
// File name: ProductTypeIdentification.h
//
// Synopsis: This file contains the declaration of the CproductTypeIdentification class
//           which is the inspection task used to identify what is the type of product.
//

#ifndef PRODUCT_TYPE_IDENTIFICATION_H
#define PRODUCT_TYPE_IDENTIFICATION_H

#include "ColorMatchTask.h"

class CProductTypeIdentification : public CColorMatchTask
   {
   public:
      // Constructor.
      CProductTypeIdentification(MIL_CONST_TEXT_PTR ColContextPath, const MIL_INT* ExpectedMatches, MIL_INT ColorConversion = M_NONE, CInspectionTask* FixtureProvider = M_NULL, CInspectionTask* ImageProvider = M_NULL, CInspectionTask* RegionProvider = M_NULL);

      // Destructor.
      virtual ~CProductTypeIdentification();
           
   protected:

   private:

      // Function to get the label of the sample.
      virtual MIL_CONST_TEXT_PTR GetProductLabel() const { return MIL_TEXT("Label"); }
      virtual MIL_CONST_TEXT_PTR GetBestMatchLabel(MIL_INT MatchIdx) const;
   };

#endif // PRODUCT_TYPE_IDENTIFICATION_H