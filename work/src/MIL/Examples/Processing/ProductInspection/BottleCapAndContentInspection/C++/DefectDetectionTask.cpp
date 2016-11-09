//***************************************************************************************
// 
// File name: DefectDetectionTask.cpp
//
// Synopsis: This file contains the implementation of the CDefectDetectionTask class
//           which is the inspection task that detects defect using a template based
//           difference extraction.
//

#include <mil.h>
#include "DefectDetectionTask.h"

//*****************************************************************************
// Constants.
//*****************************************************************************
static const MIL_INT DEFECT_SIZE = 4;
static const MIL_DOUBLE GRADIENT_MASK_SMOOTHNESS = 70;
static const MIL_DOUBLE TRIANGLE_LOWER_CUTOFF = 2;
static const MIL_DOUBLE TRIANGLE_UPPER_CUTOFF = 255;
static const MIL_DOUBLE BIN_CUMULATIVE_VALUE = 99.0;
static const MIL_DOUBLE FIXED_DIFF_THRESHOLD = 10;
static const MIL_DOUBLE NORMAL_VARIATIONS = 20;
static const MIL_INT CLEAN_MORPH_SIZE = 0;
static const MIL_DOUBLE MIN_AREA = 10;

//*****************************************************************************
// Constructor.
//*****************************************************************************
CDefectDetectionTask::CDefectDetectionTask(MIL_CONST_TEXT_PTR TemplatePath, MIL_CONST_TEXT_PTR TemplateMaskPath, DifferenceExtractionMethod DiffMethod, BinarizationMethod BinMethod, MIL_INT ColorConversion /* = M_NONE */, CInspectionTask* FixtureProvider /* = M_NULL */, CInspectionTask* ImageProvider /* = M_NULL */)
 : CInspectionTask(ColorConversion, FixtureProvider, ImageProvider),
   m_DiffMethod(DiffMethod),
   m_BinMethod(BinMethod),
    
   m_MilBlobFeatureList          (M_NULL),
   m_MilBlobResult               (M_NULL),

   m_MilTemplateImage            (M_NULL),
   m_MilTemplateGrayImage        (M_NULL),
   m_MilTemplateMask             (M_NULL),
   m_MilTemplateGradientMask     (M_NULL),
   m_MilTemplateGradientGrayMask (M_NULL),
   m_MilDifferenceGrayImage      (M_NULL),
   m_MilExtractedDefectsImage    (M_NULL)
   {
   // Copy the path.
   CloneString(m_TemplatePath, TemplatePath);
   CloneString(m_TemplateMaskPath, TemplateMaskPath);
   }

//*****************************************************************************
// Destructor.
//*****************************************************************************
CDefectDetectionTask::~CDefectDetectionTask()
   {
   if(m_TemplateMaskPath)
      { delete [] m_TemplateMaskPath; }
   
   if(m_TemplatePath)
      { delete [] m_TemplatePath; }

   // Free the mil objects
   FreeMilObjects();
   }

//*****************************************************************************
// Free function.
//*****************************************************************************
void CDefectDetectionTask::Free()
   {
   CInspectionTask::Free();

   //Free the Mil objects
   FreeMilObjects();
   }
//*****************************************************************************
// Init.
//*****************************************************************************
void CDefectDetectionTask::Init(MIL_ID MilSystem, MIL_INT ImageSizeX /* = 0 */, MIL_INT ImageSizeY /* = 0 */)
   {
   // Initialize the base classes
   CInspectionTask::Init(MilSystem, ImageSizeX, ImageSizeY);

   // Restore the template
   MbufRestore(m_TemplatePath, MilSystem, &m_MilTemplateImage);

   // Get the size of the template
   MIL_INT TemplateSizeX = MbufInquire(m_MilTemplateImage, M_SIZE_X, M_NULL);
   MIL_INT TemplateSizeY = MbufInquire(m_MilTemplateImage, M_SIZE_Y, M_NULL);
   
   // Restore the mask
   MbufRestore(m_TemplateMaskPath, MilSystem, &m_MilTemplateMask);

   // Create the gray scale image
   m_MilTemplateGrayImage = CreateConvertedImage(m_MilTemplateImage, GetColorConversion());

   // Create the gradient mask
   CreateGradientMaskImage(m_MilTemplateGrayImage, m_MilTemplateImage, GRADIENT_MASK_SMOOTHNESS, &m_MilTemplateGradientMask, &m_MilTemplateGradientGrayMask);

   // Allocate the difference image
   MbufAlloc2d(MilSystem, TemplateSizeX, TemplateSizeY, 8+M_UNSIGNED, M_IMAGE+M_PROC, &m_MilDifferenceGrayImage);

   // Allocate the extracted defects image
   MbufAlloc2d(MilSystem, TemplateSizeX, TemplateSizeY, 1+M_UNSIGNED, M_IMAGE+M_PROC, &m_MilExtractedDefectsImage);

   // Allocate blob
   MblobAllocFeatureList(MilSystem, &m_MilBlobFeatureList);
   MblobSelectFeature(m_MilBlobFeatureList, M_BOX);
   MblobAllocResult(MilSystem, &m_MilBlobResult);
   MblobControl(m_MilBlobResult, M_RESULT_OUTPUT_UNITS, M_WORLD);
   }

//*****************************************************************************
// Inspect.
//*****************************************************************************
bool CDefectDetectionTask::Inspect(MIL_ID MilImage)
   {   
   // Extract the differences
   ExtractDifferences(m_MilTemplateImage, m_MilTemplateGrayImage, m_MilTemplateGradientMask, m_MilTemplateGradientGrayMask, MilImage, m_MilDifferenceGrayImage, M_NULL, m_DiffMethod);
   
   // Mask out differences outside the mask
   MimArith(m_MilDifferenceGrayImage, m_MilTemplateMask, m_MilDifferenceGrayImage, M_AND);

   // Extract the defect
   ExtractDefects(m_MilDifferenceGrayImage,
                  m_MilExtractedDefectsImage,
                  TRIANGLE_LOWER_CUTOFF,
                  TRIANGLE_UPPER_CUTOFF,
                  BIN_CUMULATIVE_VALUE, 
                  NORMAL_VARIATIONS,
                  FIXED_DIFF_THRESHOLD,
                  CLEAN_MORPH_SIZE,
                  m_BinMethod);

   // Set the fixture on the image
   McalAssociate(M_NULL, m_MilExtractedDefectsImage, M_DEFAULT);
   McalUniform(m_MilExtractedDefectsImage, 0.0, 0.0, 1.0, 1.0, 0.0, M_DEFAULT);

   // Calculate the blobs
   MblobCalculate(m_MilExtractedDefectsImage, M_NULL, m_MilBlobFeatureList, m_MilBlobResult);

   // Select the blobs that meet the area criteria
   MblobSelect(m_MilBlobResult, M_DELETE, M_AREA, M_LESS, MIN_AREA, M_NULL);

   return true;
   }

//*****************************************************************************
// Draw graphical result.
//*****************************************************************************
void CDefectDetectionTask::DrawGraphicalResult(MIL_ID MilGraContext, MIL_ID MilDest)
   {
   // Draw the blob result.
   MgraColor(MilGraContext, M_COLOR_RED);
   MblobDraw(MilGraContext, m_MilBlobResult, MilDest, M_DRAW_BOX, M_DEFAULT, M_DEFAULT);
   }

//*****************************************************************************
// Draw text result.
//*****************************************************************************
void CDefectDetectionTask::DrawTextResult(MIL_ID MilGraContext, MIL_ID MilDest)
   {   
   if(IsResultValid() && MblobGetNumber(m_MilBlobResult, M_NULL) == 0)
      {
      MgraColor(MilGraContext, M_COLOR_GREEN);
      MgraText(MilGraContext, MilDest, 0, 0, MIL_TEXT("Defect detection: PASS"));
      }
   else
      {
      MgraColor(MilGraContext, M_COLOR_RED);
      MgraText(MilGraContext, MilDest, 0, 0, MIL_TEXT("Defect detection: FAIL"));
      }
   MoveGraphicContextYOffset(MilGraContext, 1);
   }

//*****************************************************************************
// Function that frees the MIL objects.
//*****************************************************************************
void CDefectDetectionTask::FreeMilObjects()
   {
   if(m_MilTemplateGrayImage)
      {
      MbufFree(m_MilTemplateGrayImage);
      m_MilTemplateGrayImage = 0;
      }

   if(m_MilTemplateImage)
      {
      MbufFree(m_MilTemplateImage);
      m_MilTemplateImage = 0;
      }

   if(m_MilTemplateMask)
      {
      MbufFree(m_MilTemplateMask);
      m_MilTemplateMask = 0;
      }

   if(m_MilTemplateGradientMask)
      {
      MbufFree(m_MilTemplateGradientMask);
      m_MilTemplateGradientMask = 0;
      }

   if(m_MilTemplateGradientGrayMask)
      {
      MbufFree(m_MilTemplateGradientGrayMask);
      m_MilTemplateGradientGrayMask = 0;
      }

   if(m_MilDifferenceGrayImage)
      {
      MbufFree(m_MilDifferenceGrayImage);
      m_MilDifferenceGrayImage = 0;
      }

   if(m_MilExtractedDefectsImage)
      {
      MbufFree(m_MilExtractedDefectsImage);
      m_MilExtractedDefectsImage = 0;
      }   

   if(m_MilBlobFeatureList)
      {
      MblobFree(m_MilBlobFeatureList);
      m_MilBlobFeatureList = 0;
      }

   if(m_MilBlobResult)
      {
      MblobFree(m_MilBlobResult);
      m_MilBlobResult = 0;
      }
   }