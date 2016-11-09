//***************************************************************************************
// 
// File name: ReadStringTask.cpp
//
// Synopsis: This file contains the implementation of the CReadStringTask class
//           which is the inspection task used to read the a string.
//

#include <mil.h>
#include "ReadStringTask.h"

//*****************************************************************************
// Constants.
//*****************************************************************************

//*****************************************************************************
// Constructor.
//*****************************************************************************
CReadStringTask::CReadStringTask(MIL_CONST_TEXT_PTR StrContextPath, MIL_INT ColorConversion /* = M_NONE */, CInspectionTask* FixtureProvider /* = M_NULL */, CInspectionTask* ImageProvider /* = M_NULL */, CInspectionTask* RegionProvider /* = M_NULL */)
 : CReadStrTask(StrContextPath, ColorConversion, FixtureProvider, ImageProvider, RegionProvider)
   {
   }

//*****************************************************************************
// Destructor.
//*****************************************************************************
CReadStringTask::~CReadStringTask()
   {
   }

//*****************************************************************************
// Draw text result.
//*****************************************************************************
void CReadStringTask::DrawTextResult(MIL_ID MilGraContext, MIL_ID MilDest)
   {
   MIL_TEXT_CHAR OutputString[256];
   MgraColor(MilGraContext, GetReadStatus() ? M_COLOR_GREEN : M_COLOR_RED);
   MosSprintf(OutputString, 256, MIL_TEXT("String read: %s"), GetReadString());
   MgraText(MilGraContext, MilDest, 0, 0, OutputString);
   MoveGraphicContextYOffset(MilGraContext, 1);
   }