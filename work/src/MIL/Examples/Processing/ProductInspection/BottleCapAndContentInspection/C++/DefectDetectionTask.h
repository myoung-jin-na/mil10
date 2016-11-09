//***************************************************************************************
// 
// File name: DefectDetectionTask.h
//
// Synopsis: This file contains the declaration of the CDefectDetectionTask class
//           which is the inspection task that detects defect using a template based
//           difference extraction.
//

#ifndef DEFECT_DETECTION_TASK_H
#define DEFECT_DETECTION_TASK_H

#include "InspectionTask.h"
#include "defectdetectionprocfunc_CapAndContent.h"

// Forward enum declares.
enum DifferenceExtractionMethod;
enum BinarizationMethod;

class CDefectDetectionTask : public CInspectionTask
   {
   public:

      // Constructor.
      CDefectDetectionTask(MIL_CONST_TEXT_PTR TemplatePath, MIL_CONST_TEXT_PTR TemplateMaskPath, DifferenceExtractionMethod DiffMethod, BinarizationMethod BinMethod, MIL_INT ColorConversion = M_NONE, CInspectionTask* FixtureProvider = M_NULL, CInspectionTask* ImageProvider = M_NULL);

      // Destructor.
      virtual ~CDefectDetectionTask();

      // Initialization function.
      virtual void Init(MIL_ID MilSystem, MIL_INT ImageSizeX = 0, MIL_INT ImageSizeY = 0);

      // Free function.
      virtual void Free();

      // Inspection function.
      virtual bool Inspect(MIL_ID MilImage);      
      
      // Drawing functions.
      virtual void DrawGraphicalResult(MIL_ID MilGraContext, MIL_ID MilDest);
      virtual void DrawTextResult(MIL_ID MilGraContext, MIL_ID MilDest);

   protected:
      
   private:

      // Free the mil objects.
      void FreeMilObjects();

      // The mil blob objects
      MIL_ID m_MilBlobResult;
      MIL_ID m_MilBlobFeatureList;

      // The string that holds the path to the color context.
      MIL_TEXT_PTR m_TemplatePath;
      MIL_TEXT_PTR m_TemplateMaskPath;

      // The work images.
      MIL_ID m_MilTemplateImage;
      MIL_ID m_MilTemplateGrayImage;
      MIL_ID m_MilTemplateMask;
      MIL_ID m_MilTemplateGradientMask;
      MIL_ID m_MilTemplateGradientGrayMask;
      MIL_ID m_MilDifferenceGrayImage;
      MIL_ID m_MilExtractedDefectsImage;

      // The difference extraction and binarization methods used.
      DifferenceExtractionMethod m_DiffMethod;
      BinarizationMethod m_BinMethod;     
   };

#endif // DEFECT_DETECTION_TASK_H