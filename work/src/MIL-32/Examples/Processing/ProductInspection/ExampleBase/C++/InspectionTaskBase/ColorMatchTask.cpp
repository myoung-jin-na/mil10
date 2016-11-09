//***************************************************************************************
// 
// File name: ColorMatchTask.cpp
//
// Synopsis: This file contains the implementation of the CColorMatchTask class
//           which is the base task for any color match task.
//

#include <mil.h>
#include "ColorMatchTask.h"

//*****************************************************************************
// Constants.
//*****************************************************************************

//*****************************************************************************
// Constructor.
//*****************************************************************************
CColorMatchTask::CColorMatchTask(MIL_CONST_TEXT_PTR ColContextPath, const MIL_INT* ExpectedMatches, MIL_INT ColorConversion /* = M_NONE */, CInspectionTask* FixtureProvider /* = M_NULL */, CInspectionTask* ImageProvider /* = M_NULL */, CInspectionTask* RegionProvider /* = M_NULL */)
 : CHighLevelInspectionTask(ColContextPath, McolFree, M_NONE, FixtureProvider, ImageProvider, RegionProvider),
   CRegionMngr(),
   m_ExpectedMatches(ExpectedMatches),
   m_BestMatchProductIndexTable(NULL)
   {
   }

//*****************************************************************************
// Destructor.
//*****************************************************************************
CColorMatchTask::~CColorMatchTask()
   {
   }

//*****************************************************************************
// Free function
//*****************************************************************************
void CColorMatchTask::Free()
   {
   CRegionMngr::Free();
   CHighLevelInspectionTask::Free();

   if(m_BestMatchProductIndexTable)
      {
      delete [] m_BestMatchProductIndexTable;
      m_BestMatchProductIndexTable = NULL;
      }
   }
//*****************************************************************************
// Init.
//*****************************************************************************
void CColorMatchTask::Init(MIL_ID MilSystem, MIL_INT ImageSizeX /* = 0 */, MIL_INT ImageSizeY /* = 0 */)
   {
   // Initialize the base classes.
   CRegionMngr::Init(MilSystem, ImageSizeX, ImageSizeY);
   CHighLevelInspectionTask::Init(MilSystem, ImageSizeX, ImageSizeY);
   }

//*****************************************************************************
// Inspect.
//*****************************************************************************
bool CColorMatchTask::Calculate(MIL_ID MilImage)
   {
   MIL_ID MilAreaImage = HasRegionProvider() ? RasterizeRegion(MilImage, GetInputRegionList()) : M_NULL;

   // Match the color. 
   McolMatch(MilContext(), MilImage, M_DEFAULT, MilAreaImage, MilResult(), M_DEFAULT);

   // Get the number of areas.
   McolGetResult(MilResult(), M_GENERAL, M_GENERAL, M_NUMBER_OF_AREAS + M_TYPE_MIL_INT, &m_NbAreas);

   // Allocate the best match product index table.
   if(m_BestMatchProductIndexTable)
      delete [] m_BestMatchProductIndexTable;
   m_BestMatchProductIndexTable = new MIL_INT[m_NbAreas];

   // Get the best match index.
   McolGetResult(MilResult(), M_ALL, M_GENERAL, M_BEST_MATCH_INDEX + M_TYPE_MIL_INT, m_BestMatchProductIndexTable);
      
   return true;
   }

//*****************************************************************************
// Inspect.
//*****************************************************************************
void CColorMatchTask::DrawGraphicalResult(MIL_ID MilGraContext, MIL_ID MilDest)
   {
   // Draw the region .
   MgraColor(MilGraContext, M_COLOR_MAGENTA);
   CInspectionTask::DrawInputRegion(MilGraContext, MilDest);   
   
   // Draw the result of the matching.
   MgraColor(MilGraContext, M_COLOR_MAGENTA);
   for(MIL_INT AreaIdx = 0; AreaIdx < m_NbAreas; AreaIdx++)
      CRegionMngr::DrawTextInRegion(MilGraContext, GetInputRegionList(), MilDest, AreaIdx, GetBestMatchLabel(m_BestMatchProductIndexTable[AreaIdx]));      
   }

//*****************************************************************************
// Draw the text result.
//*****************************************************************************
void CColorMatchTask::DrawTextResult(MIL_ID MilGraContext, MIL_ID MilDest)
   {
   MIL_TEXT_CHAR OutputString[256];

   // Check if all the matches are as expected.
   bool AllAreasValid = true;
   if(m_ExpectedMatches != NULL)
      {
      for(MIL_INT AreaIdx = 0; AreaIdx < m_NbAreas && AllAreasValid; AreaIdx++)
         AllAreasValid = m_BestMatchProductIndexTable[AreaIdx] == m_ExpectedMatches[AreaIdx];
      }

   if(IsResultValid() && AllAreasValid)
      {
      MgraColor(MilGraContext, M_COLOR_GREEN);
      MosSprintf(OutputString, 256, MIL_TEXT("%s color match: PASS"), GetProductLabel());
      }
   else
      {
      MgraColor(MilGraContext, M_COLOR_RED);
      MosSprintf(OutputString, 256, MIL_TEXT("%s color match: FAIL"), GetProductLabel());
      }
   MgraText(MilGraContext, MilDest, 0, 0, OutputString);
   MoveGraphicContextYOffset(MilGraContext, 1);
   }