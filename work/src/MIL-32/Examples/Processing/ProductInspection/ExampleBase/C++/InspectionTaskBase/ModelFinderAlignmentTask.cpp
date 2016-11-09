//***************************************************************************************
// 
// File name: ModelFinderAlignmentTask.cpp
//
// Synopsis: This file contains the implementation of the CModAlignmentTask class
//           which is the base class for any alignment task.
//

#include <mil.h>
#include "ModelFinderAlignmentTask.h"

//*****************************************************************************
// Constants.
//*****************************************************************************

//*****************************************************************************
// Constructor.
//*****************************************************************************
CModAlignmentTask::CModAlignmentTask(MIL_CONST_TEXT_PTR ModContextPath, MIL_INT ColorConversion /* = M_NONE */, CInspectionTask* FixtureProvider /* = M_NULL */, CInspectionTask* ImageProvider /* = M_NULL */)
 : CHighLevelInspectionTask(ModContextPath, MmodFree, ColorConversion, FixtureProvider, ImageProvider)
   {
   }

//*****************************************************************************
// Destructor.
//*****************************************************************************
CModAlignmentTask::~CModAlignmentTask()
   {
   }

//*****************************************************************************
// Inspect.
//*****************************************************************************
bool CModAlignmentTask::Calculate(MIL_ID MilImage)
   {
   // Match the model
   MmodFind(MilContext(), MilImage, MilResult());

   // Get the number of models found
   MmodGetResult(MilResult(), M_GENERAL, M_NUMBER + M_TYPE_MIL_INT, &m_NumberFound);

   return m_NumberFound > 0;      
   }

//*****************************************************************************
// Set fixture.
//*****************************************************************************
void CModAlignmentTask::SetFixture()
   {
   SetOutputFixture(M_RESULT_MOD, M_NULL, MilResult(), 0, M_DEFAULT, M_DEFAULT, M_DEFAULT);
   }

//*****************************************************************************
// Draw the graphical result.
//*****************************************************************************
void CModAlignmentTask::DrawGraphicalResult(MIL_ID MilGraContext, MIL_ID MilDest)
   {
   // Draw the output fixture.
   MgraColor(MilGraContext, M_COLOR_RED);
   DrawOutputFixture(MilGraContext, MilDest);   
   }
