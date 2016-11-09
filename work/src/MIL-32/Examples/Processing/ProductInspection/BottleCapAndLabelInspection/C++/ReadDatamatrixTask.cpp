//***************************************************************************************
// 
// File name: ReadDatamatrixTask.cpp
//
// Synopsis: This file contains the implementation of the CReadDatamatrixTask class
//           which is the inspection task used to read a datamatrix.
//

#include <mil.h>
#include "ReadDatamatrixTask.h"

//*****************************************************************************
// Constants.
//*****************************************************************************

//*****************************************************************************
// Constructor.
//*****************************************************************************
CReadDatamatrixTask::CReadDatamatrixTask(MIL_CONST_TEXT_PTR CodeContextPath, MIL_INT ColorConversion /* = M_NONE */, CInspectionTask* FixtureProvider /* = M_NULL */, CInspectionTask* ImageProvider /* = M_NULL */, CInspectionTask* RegionProvider /* = M_NULL */)
 : CReadCodeTask(CodeContextPath, ColorConversion, FixtureProvider, ImageProvider, RegionProvider)
   {
   }

//*****************************************************************************
// Destructor.
//*****************************************************************************
CReadDatamatrixTask::~CReadDatamatrixTask()
   {
   }

//*****************************************************************************
// Draw text result.
//*****************************************************************************
void CReadDatamatrixTask::DrawTextResult(MIL_ID MilGraContext, MIL_ID MilDest)
   {   
   MIL_TEXT_CHAR OutputString[256];
   MgraColor(MilGraContext, GetReadStatus() ? M_COLOR_GREEN : M_COLOR_RED);
   MosSprintf(OutputString, 256, MIL_TEXT("Datamatrix read: %s"), GetReadString());
   MgraText(MilGraContext, MilDest, 0, 0, OutputString);
   MoveGraphicContextYOffset(MilGraContext, 1);
   }