//***************************************************************************************
// 
// File name: ArcRegion.h
//
// Synopsis: This file contains the declaration of the CArcRegion.
//

#ifndef ARC_REGION_H
#define ARC_REGION_H

#include "GraphicRegion.h"

class CArcRegion : public CGraphicRegion
   {
   public:

      // Constructor.
      CArcRegion(MIL_INT InputUnits, MIL_DOUBLE CenterX, MIL_DOUBLE CenterY, MIL_DOUBLE RadiusX, MIL_DOUBLE RadiusY, MIL_DOUBLE StartAngle, MIL_DOUBLE EndAngle, MIL_DOUBLE XAxisAngle, CInspectionTask* RegionProvider = M_NULL);

      // Destructor.
      virtual ~CArcRegion();
          
   protected:

   private:

      // Virtual function to draw the output region.
      virtual void DrawOutputRegion(MIL_ID MilGraContext, MIL_ID MilDest);
      
      // Virtual function to draw the region in the region list
      virtual void DrawGraphicRegionInRegionList(MIL_ID MilGraContext);

      // The information of the region
      MIL_DOUBLE m_CenterX;
      MIL_DOUBLE m_CenterY;
      MIL_DOUBLE m_RadiusX;
      MIL_DOUBLE m_RadiusY;
      MIL_DOUBLE m_StartAngle;
      MIL_DOUBLE m_EndAngle;
      MIL_DOUBLE m_XAxisAngle;
   };
#endif // ARC_REGION_H