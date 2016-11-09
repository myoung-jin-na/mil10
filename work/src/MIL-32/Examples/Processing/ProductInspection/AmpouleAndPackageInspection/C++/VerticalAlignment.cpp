//***************************************************************************************
// 
// File name: VerticalAlignment.cpp
//
// Synopsis: This file contains the implementation of the CVerticalAlignment class
//           which is the class that manages the use of a region in an inspection task.
//

#include <mil.h>
#include "VerticalAlignment.h"

//*****************************************************************************
// Constants.
//*****************************************************************************

//*****************************************************************************
// Constructor.
//*****************************************************************************
CVerticalAlignment::CVerticalAlignment(MIL_CONST_TEXT_PTR MeasMarkerPath, MIL_INT ColorConversion /* = M_NONE */, CInspectionTask* FixtureProvider /* = M_NULL */, CInspectionTask* ImageProvider /* = M_NULL */)
 : CMeasInspectionTask(MeasMarkerPath, ColorConversion, FixtureProvider, ImageProvider)
   {
   }

//*****************************************************************************
// Destructor.
//*****************************************************************************
CVerticalAlignment::~CVerticalAlignment()
   {
   }

//*****************************************************************************
// Inspect.
//*****************************************************************************
bool CVerticalAlignment::Calculate(MIL_ID MilImage)
   {
   // Do the measurement task
   if(CMeasInspectionTask::Calculate(MilImage))
      {
      // Get the measurement result
      MIL_DOUBLE PosX;
      MmeasGetResult(MilResult(), M_POSITION, &PosX, &m_PosY);

      return true;
      }  
   return false;
   }

//*****************************************************************************
// Set fixture.
//*****************************************************************************
void CVerticalAlignment::SetFixture()
   {
   SetOutputFixture(M_POINT_AND_ANGLE, M_NULL, M_DEFAULT, 0, m_PosY, 0, M_DEFAULT);
   }

//*****************************************************************************
// Draw graphical result
//*****************************************************************************
void CVerticalAlignment::DrawGraphicalResult(MIL_ID MilGraContext, MIL_ID MilDest)
   {
   MgraColor(MilGraContext, M_COLOR_GREEN);
   MmeasDraw(MilGraContext, MilResult(), MilDest, M_DRAW_SEARCH_REGION + M_DRAW_SEARCH_DIRECTION, M_DEFAULT, M_MARKER);
   MgraColor(MilGraContext, M_COLOR_MAGENTA);
   MmeasDraw(MilGraContext, MilResult(), MilDest, M_DRAW_POSITION, M_DEFAULT, M_RESULT);
   }

//*****************************************************************************
// Draw text result
//*****************************************************************************
void CVerticalAlignment::DrawTextResult(MIL_ID MilGraContext, MIL_ID MilDest)
   {   
   if(IsResultValid())
      {
      MgraColor(MilGraContext, M_COLOR_GREEN);
      MgraText(MilGraContext, MilDest, 0, 0, MIL_TEXT("Vertical alignment: PASS"));
      }
   else
      {
      MgraColor(MilGraContext, M_COLOR_RED);
      MgraText(MilGraContext, MilDest, 0, 0, MIL_TEXT("Vertical alignment: FAIL"));
      }
   MoveGraphicContextYOffset(MilGraContext, 1);
   }