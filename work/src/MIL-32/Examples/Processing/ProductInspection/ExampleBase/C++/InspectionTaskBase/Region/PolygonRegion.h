//***************************************************************************************
// 
// File name: PolygonRegion.h
//
// Synopsis: This file contains the declaration of the CPolygonRegion.
//

#ifndef POLYGON_REGION_H
#define POLYGON_REGION_H

#include "GraphicRegion.h"

class CPolygonRegion : public CGraphicRegion
   {
   public:

      // Constructor.
      CPolygonRegion(MIL_INT InputUnits, MIL_INT NbPoints, const MIL_DOUBLE* pPosX, const MIL_DOUBLE* pPosY, CInspectionTask* RegionProvider = M_NULL);

      // Destructor.
      virtual ~CPolygonRegion();
    
   protected:

   private:

      // Virtual function to draw the output region.
      virtual void DrawOutputRegion(MIL_ID MilGraContext, MIL_ID MilDest);
      
      // Virtual function to draw the region in the region list
      virtual void DrawGraphicRegionInRegionList(MIL_ID MilGraContext);

      // The information of the region
      MIL_INT m_NbPoints;
      const MIL_DOUBLE* m_pPosX;
      const MIL_DOUBLE* m_pPosY;
   };
#endif // POLYGON_REGION_H