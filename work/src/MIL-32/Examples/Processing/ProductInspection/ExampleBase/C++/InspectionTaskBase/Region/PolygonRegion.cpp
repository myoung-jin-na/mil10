//***************************************************************************************
// 
// File name: PolygonRegion.cpp
//
// Synopsis: This file contains the implementation of the CArcRegion class.
//

#include <mil.h>
#include "PolygonRegion.h"

//*****************************************************************************
// Constants.
//*****************************************************************************

//*****************************************************************************
// Constructor.
//*****************************************************************************
CPolygonRegion::CPolygonRegion(MIL_INT InputUnits, MIL_INT NbPoints, const MIL_DOUBLE* pPosX, const MIL_DOUBLE* pPosY, CInspectionTask* RegionProvider /* = M_NULL */)
 : CGraphicRegion(InputUnits, RegionProvider),
   m_NbPoints(NbPoints),
   m_pPosX(pPosX),
   m_pPosY(pPosY)   
   {
   }

//*****************************************************************************
// Destructor.
//*****************************************************************************
CPolygonRegion::~CPolygonRegion()
   {
   }

//*****************************************************************************
// Draw the region in the output region list
//*****************************************************************************
void CPolygonRegion::DrawGraphicRegionInRegionList(MIL_ID MilGraContext)
   {
   // Draw the polygon region.
   MgraLines(MilGraContext, GetOutputRegionList(), m_NbPoints, m_pPosX, m_pPosY, M_NULL, M_NULL, M_POLYGON + M_FILLED);
   }

//*****************************************************************************
// Draw output region function.
//*****************************************************************************
void CPolygonRegion::DrawOutputRegion(MIL_ID MilGraContext, MIL_ID MilDest)
   {
   CGraphicRegion::DrawOutputRegion(MilGraContext, MilDest);
   MgraLines(MilGraContext, MilDest, m_NbPoints, m_pPosX, m_pPosY, M_NULL, M_NULL, M_POLYGON);
   }
