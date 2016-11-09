//***************************************************************************************
// 
// File name: MetrologyInspectionTask.cpp
//
// Synopsis:This file contains the implementation of the CMetInspectionTask class
//           which is the base inspection task that uses metrology.
//

#include <mil.h>
#include "MetrologyInspectionTask.h"

//*****************************************************************************
// Constants.
//*****************************************************************************

//*****************************************************************************
// Constructor.
//*****************************************************************************
CMetInspectionTask::CMetInspectionTask(MIL_CONST_TEXT_PTR MetContextPath, MIL_INT ColorConversion /* = M_NONE */, CInspectionTask* FixtureProvider /* = M_NULL */, CInspectionTask* ImageProvider /* = M_NULL */)
 : CHighLevelInspectionTask(MetContextPath, MmetFree, ColorConversion, FixtureProvider, ImageProvider)
   {
   }

//*****************************************************************************
// Destructor.
//*****************************************************************************
CMetInspectionTask::~CMetInspectionTask()
   {
   }

//*****************************************************************************
// Inspect.
//*****************************************************************************
bool CMetInspectionTask::Calculate(MIL_ID MilImage)
   {
   // Calculate the metrology template.
   MmetCalculate(MilContext(), MilImage, MilResult(), M_DEFAULT);

   return true;
   }

//*****************************************************************************
// Draw the graphical result
//*****************************************************************************
void CMetInspectionTask::DrawGraphicalResult(MIL_ID MilGraContext, MIL_ID MilDest)
   {
   // Draw the features.
   MgraColor(MilGraContext, M_COLOR_MAGENTA);
   MmetDraw(MilGraContext, MilResult(), MilDest, M_DRAW_FEATURE, M_ALL_FEATURES, M_DEFAULT);

   // Draw the passed tolerances in green.
   MgraColor(MilGraContext, M_COLOR_GREEN);
   MmetDraw(MilGraContext, MilResult(), MilDest, M_DRAW_TOLERANCE, M_ALL_PASS_TOLERANCES, M_DEFAULT);

   // Draw the failed tolerances in red.
   MgraColor(MilGraContext, M_COLOR_RED);
   MmetDraw(MilGraContext, MilResult(), MilDest, M_DRAW_TOLERANCE, M_ALL_FAIL_TOLERANCES, M_DEFAULT);
   }

//*****************************************************************************
// Draw function by name of the feature.
//*****************************************************************************
void CMetInspectionTask::DrawFeatureByName(MIL_ID MilGraContext, MIL_CONST_TEXT_PTR FeatureName, MIL_ID MilDest, MIL_DOUBLE Color)
   {
   MIL_INT Label;

   // Get the label.
   MmetName(MilContext(), M_GET_FEATURE_LABEL, M_DEFAULT, const_cast<MIL_TEXT_PTR>(FeatureName), &Label, M_DEFAULT);

   // If the label exists.
   if(Label)
      {
      // Draw the feature.
      MgraColor(MilGraContext, Color);
      MmetDraw(MilGraContext, MilResult(), MilDest, M_DRAW_FEATURE, M_FEATURE_LABEL(Label), M_DEFAULT);
      }
   }

//*****************************************************************************
// Draw function by name of the tolerance.
//*****************************************************************************
void CMetInspectionTask::DrawToleranceByName(MIL_ID MilGraContext, MIL_CONST_TEXT_PTR ToleranceName, MIL_ID MilDest, MIL_DOUBLE PassColor, MIL_DOUBLE FailColor, MIL_DOUBLE WarningColor)
   {
   MIL_INT Label, Status;
   
   // Get the label.
   MmetName(MilContext(), M_GET_TOLERANCE_LABEL, M_DEFAULT, const_cast<MIL_TEXT_PTR>(ToleranceName), &Label, M_DEFAULT);

   // If the label exists.
   if(Label)
      {
      // Get the status of the feature.
      MmetGetResult(MilResult(), M_TOLERANCE_LABEL(Label), M_STATUS + M_TYPE_MIL_INT, &Status);

      // Get the color to use
      MIL_DOUBLE Color;
      switch(Status)
         {
         case M_PASS : Color = PassColor;
            break;
         case M_WARNING: Color = WarningColor;
            break;
         default:
         case M_FAIL : Color = FailColor;
            break;
         }

      // Draw the feature.
      MgraColor(MilGraContext, Color);
      MmetDraw(MilGraContext, MilResult(), MilDest, M_DRAW_TOLERANCE, M_TOLERANCE_LABEL(Label), M_DEFAULT);
      }
   }
