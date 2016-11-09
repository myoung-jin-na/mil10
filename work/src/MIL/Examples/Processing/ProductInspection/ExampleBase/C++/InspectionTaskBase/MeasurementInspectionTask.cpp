//***************************************************************************************
// 
// File name: MeasurementInspectionTask.cpp
//
// Synopsis: This file contains the implementation of the CMeasInspectionTask class.
//

#include <mil.h>
#include "MeasurementInspectionTask.h"

//*****************************************************************************
// Constants.
//*****************************************************************************

//*****************************************************************************
// Constructor.
//*****************************************************************************
CMeasInspectionTask::CMeasInspectionTask(MIL_CONST_TEXT_PTR MeasContextPath, MIL_INT ColorConversion /* = M_NONE */, CInspectionTask* FixtureProvider /* = M_NULL */, CInspectionTask* ImageProvider /* = M_NULL */)
 : CHighLevelInspectionTask(MeasContextPath, MmeasFree, ColorConversion, FixtureProvider, ImageProvider)
   {
   }

//*****************************************************************************
// Destructor.
//*****************************************************************************
CMeasInspectionTask::~CMeasInspectionTask()
   {
   }

//*****************************************************************************
// Inspect.
//*****************************************************************************
bool CMeasInspectionTask::Calculate(MIL_ID MilImage)
   {   
   // Measure the image.
   MmeasFindMarker(M_DEFAULT, MilImage, MilResult(), M_DEFAULT);

   // Get the number found.
   MmeasGetResult(MilResult(), M_NUMBER + M_TYPE_MIL_INT, &m_NumberFound, M_NULL);

   return (m_NumberFound > 0);
   }
