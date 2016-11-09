//***************************************************************************************
// 
// File name: RectangleRegion.cpp
//
// Synopsis: This file contains the implementation of the CRectangleRegion class.
//

#include <mil.h>
#include "RectangleRegion.h"

//*****************************************************************************
// Constants.
//*****************************************************************************

//*****************************************************************************
// Constructor.
//*****************************************************************************
CRectangleRegion::CRectangleRegion(MIL_INT InputUnits, MIL_DOUBLE PositionX, MIL_DOUBLE PositionY, MIL_DOUBLE SizeX, MIL_DOUBLE SizeY, MIL_DOUBLE Angle, MIL_INT ControlFlag, CInspectionTask* RegionProvider /* = M_NULL */)
 : CGraphicRegion(InputUnits, RegionProvider),
   m_PositionX(PositionX),
   m_PositionY(PositionY),
   m_SizeX(SizeX),
   m_SizeY(SizeY),
   m_Angle(Angle),
   m_ControlFlag(ControlFlag)
   {
   }

//*****************************************************************************
// Destructor.
//*****************************************************************************
CRectangleRegion::~CRectangleRegion()
   {
   }

//*****************************************************************************
// Draw the region in the output region list.
//*****************************************************************************
void CRectangleRegion::DrawGraphicRegionInRegionList(MIL_ID MilGraContext)
   {
   // Draw the rectangle region.
   MgraRectAngle(MilGraContext, GetOutputRegionList(), m_PositionX, m_PositionY, m_SizeX, m_SizeY, m_Angle, m_ControlFlag + M_FILLED);
   }

//*****************************************************************************
// Draw output region function.
//*****************************************************************************
void CRectangleRegion::DrawOutputRegion(MIL_ID MilGraContext, MIL_ID MilDest)
   {
   CGraphicRegion::DrawOutputRegion(MilGraContext, MilDest);
   MgraRectAngle(MilGraContext, MilDest, m_PositionX, m_PositionY, m_SizeX, m_SizeY, m_Angle, m_ControlFlag);
   }
