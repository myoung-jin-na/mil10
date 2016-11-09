//***************************************************************************************
// 
// File name: ThreadInspection.cpp
//
// Synopsis: This file contains the implementation of the CThreadInspection class
//           which is the inspection task used to check if the thread of the bottle is ok.
//

#include <mil.h>
#include "ThreadInspection.h"

//*****************************************************************************
// Constants.
//*****************************************************************************
static const MIL_INT NB_FEATURE_TO_DRAW = 6; 
static MIL_CONST_TEXT_PTR FEATURES_TO_DRAW[NB_FEATURE_TO_DRAW] = 
   {
   MIL_TEXT("BottomTopSegment"),
   MIL_TEXT("ThreadInsideSegment"),
   MIL_TEXT("ThreadDiagonalSegment"),
   MIL_TEXT("BottomSideSegment"),
   MIL_TEXT("ThreadTipEdgels"),
   MIL_TEXT("ThreadTipPoint"),
   };

static const MIL_INT NB_TOLERANCE_TO_DRAW = 4; 
static MIL_CONST_TEXT_PTR TOLERANCES_TO_DRAW[NB_TOLERANCE_TO_DRAW] = 
   {
   MIL_TEXT("BottomWidth"),
   MIL_TEXT("ThreadTipWidth"),
   MIL_TEXT("BottomToThreadSpacing"),
   MIL_TEXT("ThreadAngularity"),
   };

//*****************************************************************************
// Constructor.
//*****************************************************************************
CThreadInspection::CThreadInspection(MIL_CONST_TEXT_PTR MetContextPath, MIL_INT ColorConversion /* = M_NONE */, CInspectionTask* FixtureProvider /* = M_NULL */, CInspectionTask* ImageProvider /* = M_NULL */)
 : CMetInspectionTask(MetContextPath, ColorConversion, FixtureProvider, ImageProvider)
   {
   }

//*****************************************************************************
// Destructor.
//*****************************************************************************
CThreadInspection::~CThreadInspection()
   {
   }  

//*****************************************************************************
// Draw the graphical result.
//*****************************************************************************
void CThreadInspection::DrawGraphicalResult(MIL_ID MilGraContext, MIL_ID MilDest)
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
void CThreadInspection::DrawTextResult(MIL_ID MilGraContext, MIL_ID MilDest)
   {
   if(IsResultValid())
      {
      MIL_INT Status;
      MmetGetResult(MilResult(), M_ALL, M_STATUS + M_TYPE_MIL_INT, &Status);

      switch(Status)
         {
         case M_PASS:
            MgraColor(MilGraContext, M_COLOR_GREEN);
            MgraText(MilGraContext, MilDest, 0, 0, MIL_TEXT("Thread metrology: PASS"));
            break;
         case M_WARNING:
            MgraColor(MilGraContext, M_COLOR_YELLOW);
            MgraText(MilGraContext, MilDest, 0, 0, MIL_TEXT("Thread metrology: WARNING"));
            break;
         default:
         case M_FAIL:
            MgraColor(MilGraContext, M_COLOR_RED);
            MgraText(MilGraContext, MilDest, 0, 0, MIL_TEXT("Thread metrology: FAIL"));
            break;      
         }
      }
   else
      {
      MgraColor(MilGraContext, M_COLOR_RED);
      MgraText(MilGraContext, MilDest, 0, 0, MIL_TEXT("Thread metrology: FAIL"));
      }
   MoveGraphicContextYOffset(MilGraContext, 1);  
   }