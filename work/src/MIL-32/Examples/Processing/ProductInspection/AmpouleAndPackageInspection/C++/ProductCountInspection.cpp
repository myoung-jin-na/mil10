//***************************************************************************************
// 
// File name: ProductCountInspection.cpp
//
// Synopsis: This file contains the implementation of the CSimpleFixture class.
//

#include <mil.h>
#include "ProductCountInspection.h"

//*****************************************************************************
// Constants.
//*****************************************************************************

//*****************************************************************************
// Constructor.
//*****************************************************************************
CProductCountInspection::CProductCountInspection(MIL_CONST_TEXT_PTR PatModelPath, MIL_INT ExpectedNumber, MIL_INT ColorConversion /* = M_NONE */, CInspectionTask* FixtureProvider /* = M_NULL */, CInspectionTask* ImageProvider /* = M_NULL */)
 : CPatMatchingTask(PatModelPath, ColorConversion, FixtureProvider, ImageProvider),
   m_ExpectedNumber(ExpectedNumber)
   {
   }

//*****************************************************************************
// Destructor.
//*****************************************************************************
CProductCountInspection::~CProductCountInspection()
   {
   }

//*****************************************************************************
// Inspect.
//*****************************************************************************
bool CProductCountInspection::Calculate(MIL_ID MilImage)
   {
   // Calculate the number of products.
   CPatMatchingTask::Calculate(MilImage);

   return true;
   }

//*****************************************************************************
// Draw text result
//*****************************************************************************
void CProductCountInspection::DrawTextResult(MIL_ID MilGraContext, MIL_ID MilDest)
   {
   if(IsResultValid() && GetNumberFound() == m_ExpectedNumber)
      {
      MgraColor(MilGraContext, M_COLOR_GREEN);
      MgraText(MilGraContext, MilDest, 0, 0, MIL_TEXT("Product count: PASS"));
      }
   else
      {
      MgraColor(MilGraContext, M_COLOR_RED);
      MgraText(MilGraContext, MilDest, 0, 0, MIL_TEXT("Product count: FAIL"));
      }   
   MoveGraphicContextYOffset(MilGraContext, 1);  
   }

//*****************************************************************************
// Draw graphical result
//*****************************************************************************
void CProductCountInspection::DrawGraphicalResult(MIL_ID MilGraContext, MIL_ID MilDest)
   {
   // Draw the position
   MgraColor(MilGraContext, M_COLOR_MAGENTA);
   MpatDraw(MilGraContext, MilResult(), MilDest, M_DRAW_POSITION, M_DEFAULT, M_DEFAULT);
   }
