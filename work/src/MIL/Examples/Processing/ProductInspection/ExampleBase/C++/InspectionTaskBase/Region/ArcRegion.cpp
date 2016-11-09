//***************************************************************************************
// 
// File name: ArcRegion.cpp
//
// Synopsis: This file contains the implementation of the CArcRegion class.
//

#include <mil.h>
#include "ArcRegion.h"

//*****************************************************************************
// Constants.
//*****************************************************************************

//*****************************************************************************
// Constructor.
//*****************************************************************************
CArcRegion::CArcRegion(MIL_INT InputUnits, MIL_DOUBLE CenterX, MIL_DOUBLE CenterY, MIL_DOUBLE RadiusX, MIL_DOUBLE RadiusY, MIL_DOUBLE StartAngle, MIL_DOUBLE EndAngle, MIL_DOUBLE XAxisAngle, CInspectionTask* RegionProvider /* = M_NULL */)
 : CGraphicRegion(InputUnits, RegionProvider),
   m_CenterX(CenterX),
   m_CenterY(CenterY),
   m_RadiusX(RadiusX),
   m_RadiusY(RadiusY),
   m_StartAngle(StartAngle),
   m_EndAngle(EndAngle),
   m_XAxisAngle(XAxisAngle)
   {
   }

//*****************************************************************************
// Destructor.
//*****************************************************************************
CArcRegion::~CArcRegion()
   {
   }

//*****************************************************************************
// Draw the region in the output region list
//*****************************************************************************
void CArcRegion::DrawGraphicRegionInRegionList(MIL_ID MilGraContext)
   {
   // Draw the arc region.
   MgraArcAngle(MilGraContext, GetOutputRegionList(), m_CenterX, m_CenterY, m_RadiusX, m_RadiusY, m_StartAngle, m_EndAngle, m_XAxisAngle, M_SECTOR+M_FILLED);
   }

//*****************************************************************************
// Draw output region function.
//*****************************************************************************
void CArcRegion::DrawOutputRegion(MIL_ID MilGraContext, MIL_ID MilDest)
   {
   CGraphicRegion::DrawOutputRegion(MilGraContext, MilDest);
   MgraArcAngle(MilGraContext, MilDest, m_CenterX, m_CenterY, m_RadiusX, m_RadiusY, m_StartAngle, m_EndAngle, m_XAxisAngle, M_SECTOR);
   }
