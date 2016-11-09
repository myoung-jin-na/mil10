//***************************************************************************************
// 
// File name: MetrologyAlignment.cpp
//
// Synopsis: This file contains the implementation of the CMetrologyAlignment class
//           which is the inspection task used check if the lid is present and ok
//

#include <mil.h>
#include "MetrologyAlignment.h"

//*****************************************************************************
// Constants.
//*****************************************************************************
static const MIL_INT NB_FEATURE_TO_DRAW = 2; 
static MIL_CONST_TEXT_PTR FEATURES_TO_DRAW[NB_FEATURE_TO_DRAW] = 
   {
   MIL_TEXT("InnerCircle"),
   MIL_TEXT("OuterCircle")
   };

static const MIL_INT NB_TOLERANCE_TO_DRAW = 1; 
static MIL_CONST_TEXT_PTR TOLERANCES_TO_DRAW[NB_TOLERANCE_TO_DRAW] = 
   {
   MIL_TEXT("Concentricity"),
   };

//*****************************************************************************
// Constructor.
//*****************************************************************************
CMetrologyAlignment::CMetrologyAlignment(MIL_CONST_TEXT_PTR MetContextPath, MIL_INT ColorConversion /* = M_NONE */, CInspectionTask* FixtureProvider /* = M_NULL */, CInspectionTask* ImageProvider /* = M_NULL */)
 : CMetInspectionTask(MetContextPath, ColorConversion, FixtureProvider, ImageProvider)
   {
   }

//*****************************************************************************
// Destructor.
//*****************************************************************************
CMetrologyAlignment::~CMetrologyAlignment()
   {
   }  

//*****************************************************************************
// Inspect.
//*****************************************************************************
bool CMetrologyAlignment::Calculate(MIL_ID MilImage)
   {
   if(CMetInspectionTask::Calculate(MilImage))
      {
      // Get the status of the inner circle used as the fixture for the subsequent tests.
      MIL_INT Label;
      MmetName(MilContext(), M_GET_FEATURE_LABEL, M_DEFAULT, const_cast<MIL_TEXT_PTR>(MIL_TEXT("InnerCircle")), &Label, M_DEFAULT);

      MIL_INT Status;
      MmetGetResult(MilResult(), M_FEATURE_LABEL(Label), M_STATUS + M_TYPE_MIL_INT, &Status);

      if(Status == M_PASS)
         return true;
      else
         return false;
      }
   return false;
   }

//*****************************************************************************
// Set fixture.
//*****************************************************************************
void CMetrologyAlignment::SetFixture()
   {
   // Get the label of the inner circle.
   MIL_INT Label;
   MmetName(MilContext(), M_GET_FEATURE_LABEL, M_DEFAULT, const_cast<MIL_TEXT_PTR>(MIL_TEXT("InnerCircle")), &Label, M_DEFAULT);
         
   // Get the center of the inner circle.
   MIL_DOUBLE CenterX, CenterY;
   MmetGetResult(MilResult(), M_FEATURE_LABEL(Label), M_POSITION_X, &CenterX);
   MmetGetResult(MilResult(), M_FEATURE_LABEL(Label), M_POSITION_Y, &CenterY);

   // Set the output fixture.
   SetOutputFixture(M_POINT_AND_ANGLE, M_NULL, M_DEFAULT, CenterX, CenterY, 0.0, M_DEFAULT);
   }

//*****************************************************************************
// Draw the graphical result.
//*****************************************************************************
void CMetrologyAlignment::DrawGraphicalResult(MIL_ID MilGraContext, MIL_ID MilDest)
   {
   // Draw the features
   for(MIL_INT FeatureIdx = 0; FeatureIdx < NB_FEATURE_TO_DRAW; FeatureIdx++)
      CMetInspectionTask::DrawFeatureByName(MilGraContext, FEATURES_TO_DRAW[FeatureIdx], MilDest, M_RGB888(64, 0, 128));
   
   // Draw the tolerances
   for(MIL_INT ToleranceIdx = 0; ToleranceIdx < NB_TOLERANCE_TO_DRAW; ToleranceIdx++)
      CMetInspectionTask::DrawToleranceByName(MilGraContext, TOLERANCES_TO_DRAW[ToleranceIdx], MilDest, M_COLOR_DARK_GREEN, M_COLOR_DARK_RED, M_COLOR_DARK_YELLOW);   
   }

//*****************************************************************************
// Draw text result.
//*****************************************************************************
void CMetrologyAlignment::DrawTextResult(MIL_ID MilGraContext, MIL_ID MilDest)
   {
   if(IsResultValid())
      {
      MIL_INT Status;
      MmetGetResult(MilResult(), M_ALL, M_STATUS + M_TYPE_MIL_INT, &Status);

      switch(Status)
         {
         case M_PASS:
            MgraColor(MilGraContext, M_COLOR_GREEN);
            MgraText(MilGraContext, MilDest, 0, 0, MIL_TEXT("Lid metrology: PASS"));
            break;
         case M_WARNING:
            MgraColor(MilGraContext, M_COLOR_YELLOW);
            MgraText(MilGraContext, MilDest, 0, 0, MIL_TEXT("Lid metrology: WARNING"));
            break;
         default:
         case M_FAIL:
            MgraColor(MilGraContext, M_COLOR_RED);
            MgraText(MilGraContext, MilDest, 0, 0, MIL_TEXT("Lid metrology: FAIL"));
            break;      
         }
      }
   else
      {
      MgraColor(MilGraContext, M_COLOR_RED);
      MgraText(MilGraContext, MilDest, 0, 0, MIL_TEXT("Lid metrology: FAIL"));
      }
   MoveGraphicContextYOffset(MilGraContext, 1);  
   }