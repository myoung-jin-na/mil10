//***************************************************************************************
// 
// File name: LabelAlignmentInspection.cpp
//
// Synopsis: This file contains the implementation of the CLabelAlignmentInspection class
//           which is the inspection task used to check if the alignment of the label is good.
//

#include <mil.h>
#include <math.h>
#include "LabelAlignmentInspection.h"

//*****************************************************************************
// Constants.
//*****************************************************************************

//*****************************************************************************
// Constructor.
//*****************************************************************************
CLabelAlignmentInspection::CLabelAlignmentInspection(MIL_CONST_TEXT_PTR MeasContextPath, MIL_DOUBLE AngleTolerance, MIL_DOUBLE ExpectedPosY, MIL_DOUBLE PosYTolerance, MIL_INT ColorConversion /* = M_NONE */, CInspectionTask* FixtureProvider /* = M_NULL */, CInspectionTask* ImageProvider /* = M_NULL */)
 : CMeasInspectionTask(MeasContextPath, ColorConversion, FixtureProvider, ImageProvider),
   m_AngleTolerance(AngleTolerance),
   m_PosYTolerance(PosYTolerance),
   m_ExpectedPosY(ExpectedPosY)
   {
   }

//*****************************************************************************
// Destructor.
//*****************************************************************************
CLabelAlignmentInspection::~CLabelAlignmentInspection()
   {
   }  

//*****************************************************************************
// Inspect.
//*****************************************************************************
bool CLabelAlignmentInspection::Calculate(MIL_ID MilImage)
   {   
   // Do the measurement task.
   if(CMeasInspectionTask::Calculate(MilImage))
      {
      MmeasGetResult(MilResult(), M_ANGLE, &m_Angle, M_NULL);
      MmeasGetResult(MilResult(), M_POSITION, &m_PosX, &m_PosY);

      // Put the angle between -PI and PI.
      PutAngleBetweenMinus180And180(m_Angle);
      
      return true;
      }
   return false;   
   }

//*****************************************************************************
// Set fixture.
//*****************************************************************************
void CLabelAlignmentInspection::SetFixture()
   {
   MIL_DOUBLE LineB, LineC;
   MmeasGetResult(MilResult(), M_LINE_B, &LineB, M_NULL);
   MmeasGetResult(MilResult(), M_LINE_C, &LineC, M_NULL);

   // Create the output fixture intersection of the line and the x = 0(according to the source fixture) vertical axis.
   SetOutputFixture(M_POINT_AND_ANGLE, M_NULL, M_DEFAULT, 0, -LineC/LineB, m_Angle, M_DEFAULT);
   }

//*****************************************************************************
// Draw text result.
//*****************************************************************************
void CLabelAlignmentInspection::DrawTextResult(MIL_ID MilGraContext, MIL_ID MilDest)
   {
   MIL_DOUBLE PosYDiff = fabs(m_PosY - m_ExpectedPosY);
   MIL_DOUBLE AngleDiff = fabs(m_Angle);

   if(IsResultValid() &&
      (AngleDiff <= m_AngleTolerance) &&
      (PosYDiff <= m_PosYTolerance))
      {
      MgraColor(MilGraContext, M_COLOR_GREEN);
      MgraText(MilGraContext, MilDest, 0, 0, MIL_TEXT("Label alignment: PASS"));
      }
   else
      {
      MgraColor(MilGraContext, M_COLOR_RED);
      MgraText(MilGraContext, MilDest, 0, 0, MIL_TEXT("Label alignment: FAIL"));
      }
   MoveGraphicContextYOffset(MilGraContext, 1);  
   }

//*****************************************************************************
// Draw graphical result.
//*****************************************************************************
void CLabelAlignmentInspection::DrawGraphicalResult(MIL_ID MilGraContext, MIL_ID MilDest)
   {
   MgraColor(MilGraContext, M_COLOR_GREEN);
   MmeasDraw(MilGraContext, MilResult(), MilDest, M_DRAW_SEARCH_REGION + M_DRAW_SEARCH_DIRECTION, M_DEFAULT, M_RESULT);
   MgraColor(MilGraContext, M_COLOR_MAGENTA);
   MmeasDraw(MilGraContext, MilResult(), MilDest, M_DRAW_EDGES, M_DEFAULT, M_RESULT);
   }