//***************************************************************************************
// 
// File name: ReadingTask.cpp
//
// Synopsis: This file contains the implementation of the CReadTask class
//           which is the inspection task used to read something.
//

#include <mil.h>
#include "ReadingTask.h"

//*****************************************************************************
// Constants.
//*****************************************************************************

//*****************************************************************************
// Constructor.
//*****************************************************************************
CReadTask::CReadTask(MIL_CONST_TEXT_PTR ContextPath, FreeFuncPtr FreeFct, MIL_INT ColorConversion /* = M_NONE */, CInspectionTask* FixtureProvider /* = M_NULL */, CInspectionTask* ImageProvider /* = M_NULL */, CInspectionTask* RegionProvider /* = M_NULL */)
 : CHighLevelInspectionTask(ContextPath, FreeFct, ColorConversion, FixtureProvider, ImageProvider, RegionProvider),
   CRegionMngr(),
   m_ReadString(NULL)
   {
   }

//*****************************************************************************
// Destructor.
//*****************************************************************************
CReadTask::~CReadTask()
   {
   if(m_ReadString)
      {
      delete m_ReadString;
      m_ReadString = NULL;
      }
   }

//*****************************************************************************
// Free function
//*****************************************************************************
void CReadTask::Free()
   {
   CHighLevelInspectionTask::Free();
   CRegionMngr::Free();   
   }
//*****************************************************************************
// Init.
//*****************************************************************************
void CReadTask::Init(MIL_ID MilSystem, MIL_INT ImageSizeX /* = 0 */, MIL_INT ImageSizeY /* = 0 */)
   {
   // Initialize the base classes.   
   CHighLevelInspectionTask::Init(MilSystem, ImageSizeX, ImageSizeY);
   CRegionMngr::Init(MilSystem, ImageSizeX, ImageSizeY);
   }

//*****************************************************************************
// Inspect.
//*****************************************************************************
bool CReadTask::Calculate(MIL_ID MilImage)
   {
   // Read.
   Read(MilImage);

   // Get the read string.
   if(m_ReadString)
      {
      delete m_ReadString;
      m_ReadString = NULL;
      }

   if(GetReadStringResult(m_ReadString))
      return true;
   else
      return false;
   }

//*****************************************************************************
// Function that sets the region.
//*****************************************************************************
void CReadTask::DefineRegion(MIL_ID MilImage)
   {
   CRegionMngr::SetRegion(MilImage, GetInputRegionList());
   }

//*****************************************************************************
// Draw graphical result.
//*****************************************************************************
void CReadTask::DrawGraphicalResult(MIL_ID MilGraContext, MIL_ID MilDest)
   {
   // Draw the region. 
   MgraColor(MilGraContext, M_COLOR_BLUE);
   CInspectionTask::DrawInputRegion(MilGraContext, MilDest);
   }
