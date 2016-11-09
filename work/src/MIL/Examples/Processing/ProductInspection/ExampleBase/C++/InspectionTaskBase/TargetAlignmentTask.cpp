//***************************************************************************************
// 
// File name: TargetAlignmentTask.cpp
//
// Synopsis: This file contains the implementation of the CTargetAlignmentTask class
//           which is a utility class to align a model on a template. The class
//           outputs the fixture location in the original image and outputs
//           the warped image.
//

#include <mil.h>
#include "TargetAlignmentTask.h"

//*****************************************************************************
// Constants.
//*****************************************************************************

//*****************************************************************************
// Constructor.
//*****************************************************************************
CTargetAlignmentTask::CTargetAlignmentTask(MIL_CONST_TEXT_PTR ModContextPath, MIL_CONST_TEXT_PTR TemplatePath, MIL_INT ColorConversion /* = M_NONE */, CInspectionTask* FixtureProvider /* = M_NULL */, CInspectionTask* ImageProvider /* = M_NULL */)
 : CModAlignmentTask(ModContextPath, ColorConversion, FixtureProvider, ImageProvider),
   m_MilFixturingOffset(M_NULL),
   m_MilTemplateImage(M_NULL)
   {
   // Clone the template path string.
   CloneString(m_TemplatePath, TemplatePath);
   }

//*****************************************************************************
// Destructor.
//*****************************************************************************
CTargetAlignmentTask::~CTargetAlignmentTask()
   {
   if(m_TemplatePath)
      delete [] m_TemplatePath;

   if(m_MilTemplateImage)
      MbufFree(m_MilTemplateImage);

   if(m_MilFixturingOffset)
      McalFree(m_MilFixturingOffset);
   }

//*****************************************************************************
// Free function.
//*****************************************************************************
void CTargetAlignmentTask::Free()
   {
   if(m_MilTemplateImage)
      {
      MbufFree(m_MilTemplateImage);
      m_MilTemplateImage = M_NULL;
      }

   if(m_MilFixturingOffset)
      {
      McalFree(m_MilFixturingOffset);
      m_MilFixturingOffset = M_NULL;
      }
   CModAlignmentTask::Free();
   }
//*****************************************************************************
// Init.
//*****************************************************************************
void CTargetAlignmentTask::Init(MIL_ID MilSystem, MIL_INT ImageSizeX /* = 0 */, MIL_INT ImageSizeY /* = 0 */)
   {
   // Initialize the base classes.
   CModAlignmentTask::Init(MilSystem, ImageSizeX, ImageSizeY);

   // Restore the template image.
   MbufRestore(m_TemplatePath, MilSystem, &m_MilTemplateImage);

   // Calculate the alignment.
   CModAlignmentTask::Calculate(m_MilTemplateImage);
   
   // Allocate the fixturing offset.
   McalAlloc(MilSystem, M_FIXTURING_OFFSET, M_DEFAULT, &m_MilFixturingOffset);

   // Set the fixturing offset.
   McalFixture(M_NULL, m_MilFixturingOffset, M_LEARN_OFFSET, M_RESULT_MOD, MilResult(), 0, M_DEFAULT, M_DEFAULT, M_DEFAULT);
   
   // Allocate a target that is the size of the template.
   MIL_INT64 SrcAttributes;
   MbufInquire(m_MilTemplateImage, M_EXTENDED_ATTRIBUTE, &SrcAttributes);
   AllocateOutputImage(MilSystem,
                       MbufInquire(m_MilTemplateImage, M_SIZE_BAND, M_NULL),
                       MbufInquire(m_MilTemplateImage, M_SIZE_X, M_NULL),
                       MbufInquire(m_MilTemplateImage, M_SIZE_Y, M_NULL),
                       MbufInquire(m_MilTemplateImage, M_TYPE, M_NULL),
                       SrcAttributes);   

   // Put a uniform calibration on the target image.
   McalUniform(GetOutputImage(), 0, 0, 1, 1, 0, M_DEFAULT);
   }

//*****************************************************************************
// Set fixture.
//*****************************************************************************
void CTargetAlignmentTask::SetFixture()
   {
   // Add the fixturing offset on the fixture of the image.
   SetOutputFixture(M_RESULT_MOD, m_MilFixturingOffset, MilResult(), 0, M_DEFAULT, M_DEFAULT, M_DEFAULT); 
   }

//*****************************************************************************
// Set output image.
//*****************************************************************************
void CTargetAlignmentTask::SetOutputImage(MIL_ID MilImage)
   {
   // Transform the image to the target.
   McalTransformImage(MilImage, GetOutputImage(), M_DEFAULT, M_BILINEAR, M_DEFAULT, M_WARP_IMAGE + M_USE_DESTINATION_CALIBRATION);
   }


//*****************************************************************************
// Draw text result.
//*****************************************************************************
void CTargetAlignmentTask::DrawTextResult(MIL_ID MilGraContext, MIL_ID MilDest)
   {
   }