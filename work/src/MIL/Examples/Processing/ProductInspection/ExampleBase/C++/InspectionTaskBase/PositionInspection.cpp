//***************************************************************************************
// 
// File name: PositionInspection.cpp
//
// Synopsis: This file contains the implementation of the CPositionInspection class
//           which is a measurement derived class that verifies if the position 
//           of the measurement is good.
//

#include <mil.h>
#include <math.h>
#include "PositionInspection.h"

//*****************************************************************************
// Constants.
//*****************************************************************************

//*****************************************************************************
// Constructor.
//*****************************************************************************
CPositionInspection::CPositionInspection(MIL_CONST_TEXT_PTR MeasMarkerPath, MIL_CONST_TEXT_PTR PositionLabel, MIL_DOUBLE ExpectedPosition, MIL_DOUBLE PosTolerance, MIL_INT ColorConversion /* = M_NONE */, CInspectionTask* FixtureProvider /* = M_NULL */, CInspectionTask* ImageProvider /* = M_NULL */)
 : CMeasInspectionTask(MeasMarkerPath, ColorConversion, FixtureProvider, ImageProvider),
   m_ExpectedPosition(ExpectedPosition),
   m_PosTolerance(PosTolerance)
   {
   CloneString(m_PositionLabel, PositionLabel);
   }

//*****************************************************************************
// Destructor.
//*****************************************************************************
CPositionInspection::~CPositionInspection()
   {
   if(m_PositionLabel)
      delete [] m_PositionLabel;
   }

//*****************************************************************************
// Inspect.
//*****************************************************************************
bool CPositionInspection::Calculate(MIL_ID MilImage)
   {
   // Calculate the measurement marker
   if(CMeasInspectionTask::Calculate(MilImage))
      {
      // Get the results
      MmeasGetResult(MilResult(), M_POSITION, &m_Pos, M_NULL);

      return true;
      }
   return false;
   }

//*****************************************************************************
// Draw text result.
//*****************************************************************************
void CPositionInspection::DrawTextResult(MIL_ID MilGraContext, MIL_ID MilDest)
   {
   MIL_TEXT_CHAR OutputString[1024];

   if(IsResultValid() && fabs(m_Pos - m_ExpectedPosition) <= m_PosTolerance)
      {
      MgraColor(MilGraContext, M_COLOR_GREEN);
      MosSprintf(OutputString, 1024, MIL_TEXT("Position of %s: PASS"), m_PositionLabel);
      }
   else
      {
      MgraColor(MilGraContext, M_COLOR_RED);
      MosSprintf(OutputString, 1024, MIL_TEXT("Position of %s: FAIL"), m_PositionLabel);
      }
   MgraText(MilGraContext, MilDest, 0, 0, OutputString);

   MoveGraphicContextYOffset(MilGraContext, 1);  
   }

//*****************************************************************************
// Draw graphical result.
//*****************************************************************************
void CPositionInspection::DrawGraphicalResult(MIL_ID MilGraContext, MIL_ID MilDest)
   {
   MgraColor(MilGraContext, M_COLOR_GREEN);
   MmeasDraw(MilGraContext, MilResult(), MilDest, M_DRAW_SEARCH_REGION + M_DRAW_SEARCH_DIRECTION, M_DEFAULT, M_RESULT);
   MgraColor(MilGraContext, M_COLOR_MAGENTA);
   MmeasDraw(MilGraContext, MilResult(), MilDest, M_DRAW_EDGES, M_DEFAULT, M_RESULT);   
   }
