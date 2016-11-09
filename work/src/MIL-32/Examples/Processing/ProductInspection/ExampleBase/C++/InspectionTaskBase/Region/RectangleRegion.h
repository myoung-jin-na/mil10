//***************************************************************************************
// 
// File name: RectangleRegion.h
//
// Synopsis: This file contains the declaration of the CRectangleRegion.
//

#ifndef RECTANGLE_REGION_H
#define RECTANGLE_REGION_H

#include "GraphicRegion.h"

class CRectangleRegion : public CGraphicRegion
   {
   public:

      // Constructor.
      CRectangleRegion(MIL_INT InputUnits, MIL_DOUBLE PositionX, MIL_DOUBLE PositionY, MIL_DOUBLE SizeX, MIL_DOUBLE SizeY, MIL_DOUBLE Angle, MIL_INT ControlFlag, CInspectionTask* RegionProvider = M_NULL);

      // Destructor.
      virtual ~CRectangleRegion();
      
   protected:

   private:

      // Virtual function to draw the output region.
      virtual void DrawOutputRegion(MIL_ID MilGraContext, MIL_ID MilDest);
      
      // Virtual function to draw the region in the region list
      virtual void DrawGraphicRegionInRegionList(MIL_ID MilGraContext);

      // The information of the region
      MIL_DOUBLE m_PositionX;
      MIL_DOUBLE m_PositionY;
      MIL_DOUBLE m_SizeX;
      MIL_DOUBLE m_SizeY;
      MIL_DOUBLE m_Angle;   
      MIL_INT    m_ControlFlag;
   };
#endif // RECTANGLE_REGION_H