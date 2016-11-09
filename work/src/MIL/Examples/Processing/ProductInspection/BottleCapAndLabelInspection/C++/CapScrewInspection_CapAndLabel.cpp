//***************************************************************************************
// 
// File name: CapScrewInspection_CapAndLabel.cpp
//
// Synopsis: This file contains the implementation of the CCapScrewInspection class
//           which is the inspection task used to check if the cap is well screwed.
//

#include <mil.h>
#include <math.h>
#include "CapScrewInspection_CapAndLabel.h"

//*****************************************************************************
// Constants.
//*****************************************************************************

//*****************************************************************************
// Constructor.
//*****************************************************************************
CCapScrewInspection::CCapScrewInspection(MIL_CONST_TEXT_PTR MeasContextPath, MIL_DOUBLE PositionYTolerance, MIL_DOUBLE AngleTolerance, MIL_INT ColorConversion /* = M_NONE */, CInspectionTask* FixtureProvider /* = M_NULL */, CInspectionTask* ImageProvider /* = M_NULL */)
 : CMeasInspectionTask(MeasContextPath, ColorConversion, FixtureProvider, ImageProvider),
   m_AngleTolerance(AngleTolerance),
   m_PositionTolerance(PositionYTolerance)
   {
   }

//*****************************************************************************
// Destructor.
//*****************************************************************************
CCapScrewInspection::~CCapScrewInspection()
   {
   }  

//*****************************************************************************
// Inspect.
//*****************************************************************************
bool CCapScrewInspection::Calculate(MIL_ID MilImage)
   {
   // Do the measurement task.
   if(CMeasInspectionTask::Calculate(MilImage))
      {
      MmeasGetResult(MilResult(), M_POSITION, &m_PosX, &m_PosY);
      MmeasGetResult(MilResult(), M_ANGLE, &m_Angle, M_NULL);

      // Put the angle fro -Pi to PI.
      PutAngleBetweenMinus180And180(m_Angle);
      
      return true;
      }   
   return false;
   }

//*****************************************************************************
// Set fixture.
//*****************************************************************************
void CCapScrewInspection::SetFixture()
   {
   SetOutputFixture(M_POINT_AND_ANGLE, M_NULL, M_DEFAULT, m_PosX, m_PosY, m_Angle, M_DEFAULT);
   }

//*****************************************************************************
// Draw text result.
//*****************************************************************************
void CCapScrewInspection::DrawTextResult(MIL_ID MilGraContext, MIL_ID MilDest)
   {
   MIL_DOUBLE AngleDiff = fabs(m_Angle);

   if(IsResultValid() && (AngleDiff <= m_AngleTolerance) && fabs(m_PosY) <= m_PositionTolerance)
      {
      MgraColor(MilGraContext, M_COLOR_GREEN);
      MgraText(MilGraContext, MilDest, 0, 0, MIL_TEXT("Cap screw: PASS"));
      }
   else
      {
      MgraColor(MilGraContext, M_COLOR_RED);
      MgraText(MilGraContext, MilDest, 0, 0, MIL_TEXT("Cap screw: FAIL"));
      }
   MoveGraphicContextYOffset(MilGraContext, 1);  
   }

//*****************************************************************************
// Draw graphical result.
//*****************************************************************************
void CCapScrewInspection::DrawGraphicalResult(MIL_ID MilGraContext, MIL_ID MilDest)
   {
   MgraColor(MilGraContext, M_COLOR_GREEN);
   MmeasDraw(MilGraContext, MilResult(), MilDest, M_DRAW_SEARCH_REGION + M_DRAW_SEARCH_DIRECTION, M_DEFAULT, M_RESULT);
   MgraColor(MilGraContext, M_COLOR_MAGENTA);
   MmeasDraw(MilGraContext, MilResult(), MilDest, M_DRAW_EDGES, M_DEFAULT, M_RESULT);   
   }