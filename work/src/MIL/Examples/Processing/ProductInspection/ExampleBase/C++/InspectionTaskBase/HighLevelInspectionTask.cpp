//***************************************************************************************
// 
// File name: HighLevelInspectionTask.cpp
//
// Synopsis: This file contains the implementation of the CHighLevelInspectionTask class
//           the base class of all high level module, except blob.
//

#include <mil.h>
#include "HighLevelInspectionTask.h"

//*****************************************************************************
// Constants.
//*****************************************************************************

//*****************************************************************************
// Constructor.
//*****************************************************************************
CHighLevelInspectionTask::CHighLevelInspectionTask(MIL_CONST_TEXT_PTR ContextPath, FreeFuncPtr FreeFct, MIL_INT ColorConversion /* = M_NONE */, CInspectionTask* FixtureProvider /* = M_NULL */, CInspectionTask* ImageProvider /* = M_NULL */, CInspectionTask* RegionProvider /* = M_NULL */)
 : CInspectionTask(ColorConversion, FixtureProvider, ImageProvider, RegionProvider),
   m_FreeFct(FreeFct),
   m_MilContext(M_NULL),
   m_MilResult(M_NULL)
   {
   // Copy the path.
   CloneString(m_ContextPath, ContextPath);     
   }

//*****************************************************************************
// Destructor.
//*****************************************************************************
CHighLevelInspectionTask::~CHighLevelInspectionTask()
   {
   if(m_ContextPath)
      { delete [] m_ContextPath; }

   if(m_MilContext)
      {
      (*m_FreeFct)(m_MilContext);
      m_MilContext = M_NULL;
      }
   if(m_MilResult)
      {
      (*m_FreeFct)(m_MilResult);
      m_MilResult = M_NULL;
      }
   }

//*****************************************************************************
// Free function.
//*****************************************************************************
void CHighLevelInspectionTask::Free()
   {
   CInspectionTask::Free();
  
   if(m_MilContext)
      {
      (*m_FreeFct)(m_MilContext);
      m_MilContext = M_NULL;
      }
   if(m_MilResult)
      {
      (*m_FreeFct)(m_MilResult);
      m_MilResult = M_NULL;
      }
   }
//*****************************************************************************
// Init.
//*****************************************************************************
void CHighLevelInspectionTask::Init(MIL_ID MilSystem, MIL_INT ImageSizeX /* = 0 */, MIL_INT ImageSizeY /* = 0 */)
   {
   // Initialize the base classes
   CInspectionTask::Init(MilSystem, ImageSizeX, ImageSizeY);

   // Restore the context.
   Restore(MilSystem, m_ContextPath, &m_MilContext, &m_MilResult);

   // Preprocess it.
   Preprocess();
   }

//*****************************************************************************
// Inspect.
//*****************************************************************************
bool CHighLevelInspectionTask::Inspect(MIL_ID MilImage)
   {   
   // Set the region in the image.
   DefineRegion(MilImage);

   // Do the inspection.
   if(Calculate(MilImage))
      {
      // Set the fixture.
      SetFixture();

      // Set the output image.
      SetOutputImage(MilImage);

      return true;
      }

   return false;
   }
