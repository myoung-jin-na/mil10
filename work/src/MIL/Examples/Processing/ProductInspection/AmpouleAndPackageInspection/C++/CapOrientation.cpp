//***************************************************************************************
// 
// File name: CapOrientation.cpp
//
// Synopsis: This file contains the implementation of the CSimpleFixture class.
//

#include <mil.h>
#include <math.h>
#include "CapOrientation.h"

//*****************************************************************************
// Constants.
//*****************************************************************************

//*****************************************************************************
// Constructor.
//*****************************************************************************
CCapOrientation::CCapOrientation(MIL_CONST_TEXT_PTR MeasMarkerPath, MIL_DOUBLE ExpectedWidth, MIL_DOUBLE WidthTolerance, MIL_INT ColorConversion /* = M_NONE */, CInspectionTask* FixtureProvider /* = M_NULL */, CInspectionTask* ImageProvider /* = M_NULL */)
 : CMeasInspectionTask(MeasMarkerPath, ColorConversion, FixtureProvider, ImageProvider),
   m_ExpectedWidth(ExpectedWidth),
   m_WidthTolerance(WidthTolerance)
   {
   }

//*****************************************************************************
// Destructor.
//*****************************************************************************
CCapOrientation::~CCapOrientation()
   {
   }

//*****************************************************************************
// Inspect.
//*****************************************************************************
bool CCapOrientation::Calculate(MIL_ID MilImage)
   {
   // Calculate the measurement marker
   if(CMeasInspectionTask::Calculate(MilImage))
      {
      // Get the results
      MmeasGetResult(MilResult(), M_STRIPE_WIDTH, &m_Width, M_NULL);
      }
   // Even if the stripe is not found the inspection is still valid.
   return true;
   }

//*****************************************************************************
// Draw text result
//*****************************************************************************
void CCapOrientation::DrawTextResult(MIL_ID MilGraContext, MIL_ID MilDest)
   {
   if(IsResultValid() && fabs(m_ExpectedWidth - m_Width) <= m_WidthTolerance)
      {
      MgraColor(MilGraContext, M_COLOR_GREEN);
      MgraText(MilGraContext, MilDest, 0, 0, MIL_TEXT("Cap orientation: PASS"));
      }
   else
      {
      MgraColor(MilGraContext, M_COLOR_RED);
      MgraText(MilGraContext, MilDest, 0, 0, MIL_TEXT("Cap orientation: FAIL"));
      }
   MoveGraphicContextYOffset(MilGraContext, 1);  
   }

//*****************************************************************************
// Draw graphical result
//*****************************************************************************
void CCapOrientation::DrawGraphicalResult(MIL_ID MilGraContext, MIL_ID MilDest)
   {
   MgraColor(MilGraContext, M_COLOR_GREEN);
   MmeasDraw(MilGraContext, MilResult(), MilDest, M_DRAW_SEARCH_REGION + M_DRAW_SEARCH_DIRECTION, M_DEFAULT, M_RESULT);
   MgraColor(MilGraContext, M_COLOR_MAGENTA);
   MmeasDraw(MilGraContext, MilResult(), MilDest, M_DRAW_EDGES, M_DEFAULT, M_RESULT);   
   }