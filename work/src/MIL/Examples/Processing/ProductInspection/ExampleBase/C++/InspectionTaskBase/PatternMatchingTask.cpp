//***************************************************************************************
// 
// File name: PatternMatchingTask.cpp
//
// Synopsis: This file contains the implementation of the CPatMatchingTask class
//           which is the base class for any pattern matching task.
//

#include <mil.h>
#include "PatternMatchingTask.h"

//*****************************************************************************
// Constants.
//*****************************************************************************

//*****************************************************************************
// Constructor.
//*****************************************************************************
CPatMatchingTask::CPatMatchingTask(MIL_CONST_TEXT_PTR PatContextPath, MIL_INT ColorConversion /* = M_NONE */, CInspectionTask* FixtureProvider /* = M_NULL */, CInspectionTask* ImageProvider /* = M_NULL */)
 : CHighLevelInspectionTask(PatContextPath, MpatFree, ColorConversion, FixtureProvider, ImageProvider)
   {
   }

//*****************************************************************************
// Destructor.
//*****************************************************************************
CPatMatchingTask::~CPatMatchingTask()
   {
   }

//*****************************************************************************
// Inspect.
//*****************************************************************************
bool CPatMatchingTask::Calculate(MIL_ID MilImage)
   {
   // Match the model.
   MpatFindModel(MilImage, MilContext(), MilResult());

   // Get the number of models found.
   MpatGetNumber(MilResult(), &m_NumberFound);

   return m_NumberFound > 0;
   }

//*****************************************************************************
// Set fixture.
//*****************************************************************************
void CPatMatchingTask::SetFixture()
   {
   SetOutputFixture(M_RESULT_PAT, M_NULL, MilResult(), 0, M_DEFAULT, M_DEFAULT, M_DEFAULT);
   }

//*****************************************************************************
// Draw the graphical result.
//*****************************************************************************
void CPatMatchingTask::DrawGraphicalResult(MIL_ID MilGraContext, MIL_ID MilDest)
   {
   // Draw the output fixture.
   MgraColor(MilGraContext, M_COLOR_RED);
   DrawOutputFixture(MilGraContext, MilDest);   
   }
