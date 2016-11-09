//***************************************************************************************
// 
// File name: HorizontalAndAngleAlignment.cpp
//
// Synopsis: This file contains the implementation of the CHorizontalALignment class
//           which is the class that manages the use of a region in an inspection task.
//

#include <mil.h>
#include "HorizontalAndAngleAlignment.h"

//*****************************************************************************
// Constants.
//*****************************************************************************

//*****************************************************************************
// Constructor.
//*****************************************************************************
CHorizontalAndAngleAlignment::CHorizontalAndAngleAlignment(MIL_CONST_TEXT_PTR MeasMarkerPath, MIL_INT ColorConversion /* = M_NONE */, CInspectionTask* FixtureProvider /* = M_NULL */, CInspectionTask* ImageProvider /* = M_NULL */)
 : CMeasInspectionTask(MeasMarkerPath, ColorConversion, FixtureProvider, ImageProvider)
   {
   }

//*****************************************************************************
// Destructor.
//*****************************************************************************
CHorizontalAndAngleAlignment::~CHorizontalAndAngleAlignment()
   {
   }

//*****************************************************************************
// Inspect.
//*****************************************************************************
bool CHorizontalAndAngleAlignment::Calculate(MIL_ID MilImage)
   {
   // Do the measurement task
   if(CMeasInspectionTask::Calculate(MilImage))
      {
      // Get the measurement results.
      
      MmeasGetResult(MilResult(), M_ANGLE, &m_Angle, M_NULL);
      MmeasGetResult(MilResult(), M_POSITION, &m_PosX, &m_PosY);

      // Put the angle between -PI and PI
      PutAngleBetweenMinus180And180(m_Angle);

      return true;     
      }  
   return false;
   }

//*****************************************************************************
// Set fixture.
//*****************************************************************************
void CHorizontalAndAngleAlignment::SetFixture()
   {
   MIL_DOUBLE LineA, LineC;
   MmeasGetResult(MilResult(), M_LINE_A, &LineA, M_NULL);
   MmeasGetResult(MilResult(), M_LINE_C, &LineC, M_NULL);
   
   // Create the output fixture intersection of the line and the y = 0(according to the source fixture) vertical axis
   SetOutputFixture(M_POINT_AND_ANGLE, M_NULL, M_DEFAULT, -LineC/LineA, 0, m_Angle-90, M_DEFAULT);
   }

//*****************************************************************************
// Draw graphical result
//*****************************************************************************
void CHorizontalAndAngleAlignment::DrawGraphicalResult(MIL_ID MilGraContext, MIL_ID MilDest)
   {
   MgraColor(MilGraContext, M_COLOR_GREEN);
   MmeasDraw(MilGraContext, MilResult(), MilDest, M_DRAW_SEARCH_REGION + M_DRAW_SEARCH_DIRECTION, M_DEFAULT, M_RESULT);
   MgraColor(MilGraContext, M_COLOR_MAGENTA);
   MmeasDraw(MilGraContext, MilResult(), MilDest, M_DRAW_POSITION, M_DEFAULT, M_RESULT);
   }

//*****************************************************************************
// Draw text result
//*****************************************************************************
void CHorizontalAndAngleAlignment::DrawTextResult(MIL_ID MilGraContext, MIL_ID MilDest)
   {   
   if(IsResultValid())
      {
      MgraColor(MilGraContext, M_COLOR_GREEN);
      MgraText(MilGraContext, MilDest, 0, 0, MIL_TEXT("Horizontal alignment: PASS"));
      }
   else
      {
      MgraColor(MilGraContext, M_COLOR_RED);
      MgraText(MilGraContext, MilDest, 0, 0, MIL_TEXT("Horizontal alignment: FAIL"));
      }
   MoveGraphicContextYOffset(MilGraContext, 1);
   }