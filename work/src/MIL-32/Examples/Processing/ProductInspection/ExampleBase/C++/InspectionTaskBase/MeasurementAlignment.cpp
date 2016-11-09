//***************************************************************************************
// 
// File name: MeasurementAlignment.cpp
//
// Synopsis: This file contains the declaration of the CMeasAlignment class
//           which is the inspection task used to align an image based on a single measurement.
//

#include <mil.h>
#include "MeasurementAlignment.h"

//*****************************************************************************
// Constants.
//*****************************************************************************

//*****************************************************************************
// Constructor.
//*****************************************************************************
CMeasAlignment::CMeasAlignment(MIL_CONST_TEXT_PTR MeasMarkerPath, MIL_INT ColorConversion /* = M_NONE */, CInspectionTask* FixtureProvider /* = M_NULL */, CInspectionTask* ImageProvider /* = M_NULL */)
 : CMeasInspectionTask(MeasMarkerPath, ColorConversion, FixtureProvider, ImageProvider)
   {
   }

//*****************************************************************************
// Destructor.
//*****************************************************************************
CMeasAlignment::~CMeasAlignment()
   {
   }

//*****************************************************************************
// Inspect.
//*****************************************************************************
bool CMeasAlignment::Calculate(MIL_ID MilImage)
   {
   // Do the measurement task.
   if(CMeasInspectionTask::Calculate(MilImage))
      {
      // Get the measurement result.
      MmeasGetResult(MilResult(), M_POSITION, &m_PosX, &m_PosY);
      MmeasGetResult(MilResult(), M_ANGLE, &m_Angle, M_NULL);

      return true;
      }
   return false;
   }

//*****************************************************************************
// Set fixture.
//*****************************************************************************
void CMeasAlignment::SetFixture()
   {
   SetOutputFixture(M_POINT_AND_ANGLE, M_NULL, M_DEFAULT, m_PosX, m_PosY, m_Angle - 90, M_DEFAULT);
   }

//*****************************************************************************
// Draw graphical result.
//*****************************************************************************
void CMeasAlignment::DrawGraphicalResult(MIL_ID MilGraContext, MIL_ID MilDest)
   {
   MgraColor(MilGraContext, M_COLOR_GREEN);
   MmeasDraw(MilGraContext, MilResult(), MilDest, M_DRAW_SEARCH_REGION + M_DRAW_SEARCH_DIRECTION, M_DEFAULT, M_RESULT);
   MgraColor(MilGraContext, M_COLOR_MAGENTA);
   MmeasDraw(MilGraContext, MilResult(), MilDest, M_DRAW_EDGE, M_DEFAULT, M_RESULT);
   }

//*****************************************************************************
// Draw text result.
//*****************************************************************************
void CMeasAlignment::DrawTextResult(MIL_ID MilGraContext, MIL_ID MilDest)
   {   
   if(IsResultValid())
      {
      MgraColor(MilGraContext, M_COLOR_GREEN);
      MgraText(MilGraContext, MilDest, 0, 0, MIL_TEXT("Measurement alignment: PASS"));
      }
   else
      {
      MgraColor(MilGraContext, M_COLOR_RED);
      MgraText(MilGraContext, MilDest, 0, 0, MIL_TEXT("Measurement alignment: FAIL"));
      }
   MoveGraphicContextYOffset(MilGraContext, 1);
   }