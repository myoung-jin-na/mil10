//***************************************************************************************
// 
// File name: GraphicRegion.cpp
//
// Synopsis: This file contains the implementation of the CGraphicRegion class.
//

#include <mil.h>
#include "GraphicRegion.h"

//*****************************************************************************
// Constants.
//*****************************************************************************

//*****************************************************************************
// Constructor.
//*****************************************************************************
CGraphicRegion::CGraphicRegion(MIL_INT InputUnits, CInspectionTask* RegionProvider /* = M_NULL */)
 : CInspectionTask(M_NONE, M_NULL, M_NULL, RegionProvider),
   m_RegionInputUnits(InputUnits)
   {
   }

//*****************************************************************************
// Destructor.
//*****************************************************************************
CGraphicRegion::~CGraphicRegion()
   {
   }


//*****************************************************************************
// Init.
//*****************************************************************************
void CGraphicRegion::Init(MIL_ID MilSystem, MIL_INT ImageSizeX /* = 0 */, MIL_INT ImageSizeY /* = 0 */)
   {
   // Initialize the base classes.
   CInspectionTask::Init(MilSystem, ImageSizeX, ImageSizeY);

   // Allocate the region list if necessary.
   CInspectionTask::AllocateRegionGraList(MilSystem);
   
   // If there is a region provider copy the graphics list
   MIL_INT CurNbGraphics = 0;
   if(HasRegionProvider())
      {
      MgraCopy(GetInputRegionList(), GetOutputRegionList(), M_COPY, M_DEFAULT, M_ALL, M_NULL, M_NULL, M_DEFAULT);
      MgraInquireList(GetInputRegionList(), M_LIST, M_DEFAULT, M_NUMBER_OF_GRAPHICS + M_TYPE_MIL_INT, &CurNbGraphics);
      }

   // Allocate a graphic context.
   MIL_ID MilGraContext = MgraAlloc(MilSystem, M_NULL);

   // Set it to the correct input units.
   MgraControl(MilGraContext, M_INPUT_UNITS, (MIL_DOUBLE)m_RegionInputUnits);

   // Set the drawing color of the next primitive.
   MgraColor(MilGraContext, (MIL_DOUBLE)(CurNbGraphics + 1));

   // Draw the region.
   DrawGraphicRegionInRegionList(MilGraContext);

   // Free the graphic context
   MgraFree(MilGraContext);
   }

//*****************************************************************************
// Inspect.
//*****************************************************************************
bool CGraphicRegion::Inspect(MIL_ID MilImage)
   {
   return true;
   }

//*****************************************************************************
// Draw output region function.
//*****************************************************************************
void CGraphicRegion::DrawOutputRegion(MIL_ID MilGraContext, MIL_ID MilDest)
   {
   // Draw the input region since the graphic region are combined   
   DrawInputRegion(MilGraContext, MilDest);
   }

//*****************************************************************************
// Draw text result.
//*****************************************************************************
void CGraphicRegion::DrawTextResult(MIL_ID MilGraContext, MIL_ID MilDest)
   {   
   }

//*****************************************************************************
// Draw graphical result.
//*****************************************************************************
void CGraphicRegion::DrawGraphicalResult(MIL_ID MilGraContext, MIL_ID MilDest)
   {
   }

