//***************************************************************************************
// 
// File name: GraphicRegion.h
//
// Synopsis: This file contains the declaration of the CRectangleRegion.
//

#ifndef GRAPHIC_REGION_H
#define GRAPHIC_REGION_H

#include "../InspectionTask.h"

class CGraphicRegion : public CInspectionTask
   {
   public:

      // Constructor.
      CGraphicRegion(MIL_INT InputUnits, CInspectionTask* RegionProvider = M_NULL);

      // Destructor.
      virtual ~CGraphicRegion();

      // Initialization function.
      virtual void Init(MIL_ID MilSystem, MIL_INT ImageSizeX = 0, MIL_INT ImageSizeY = 0);    

      // Inspection function.
      virtual bool Inspect(MIL_ID MilImage);
           
      // Drawing functions.
      virtual void DrawGraphicalResult(MIL_ID MilGraContext, MIL_ID MilDest);
      virtual void DrawTextResult(MIL_ID MilGraContext, MIL_ID MilDest);

   protected:

      // Function to draw the region in the output region list
      virtual void DrawGraphicRegionInRegionList(MIL_ID MilGraContext) = 0;
      
      // Virtual function to draw the output region.
      virtual void DrawOutputRegion(MIL_ID MilGraContext, MIL_ID MilDest);

   private:

      MIL_INT m_RegionInputUnits;
          
   };

#endif // GRAPHIC_REGION_H