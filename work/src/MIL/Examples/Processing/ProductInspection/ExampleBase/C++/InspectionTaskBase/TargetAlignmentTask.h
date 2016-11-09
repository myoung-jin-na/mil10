//***************************************************************************************
// 
// File name: TargetAlignmentTask.h
//
// Synopsis: This file contains the declaration of the CTargetAlignmentTask class
//           which is a utility class to align a model on a template. The class
//           outputs the fixture location in the original image and outputs
//           the warped image.
//

#ifndef TARGET_ALIGMNENT_TASK_H
#define TARGET_ALIGMNENT_TASK_H

#include "ModelFinderAlignmentTask.h"

class  CTargetAlignmentTask: public CModAlignmentTask
   {
   public:

      // Constructor.
      CTargetAlignmentTask(MIL_CONST_TEXT_PTR ModContextPath, MIL_CONST_TEXT_PTR TemplatePath, MIL_INT ColorConversion = M_NONE, CInspectionTask* FixtureProvider = M_NULL, CInspectionTask* ImageProvider = M_NULL);

      // Destructor.
      virtual ~CTargetAlignmentTask();

      // Initialization function.
      virtual void Init(MIL_ID MilSystem, MIL_INT ImageSizeX = 0, MIL_INT ImageSizeY = 0);

      // Free function.
      virtual void Free();
      
      // Drawing functions.
      virtual void DrawTextResult(MIL_ID MilGraContext, MIL_ID MilDest);
      
   protected:
           
   private:

      // Function that sets the output fixture.
      virtual void SetFixture();

      // Function that sets the output image.
      virtual void SetOutputImage(MIL_ID MilImage);

      // The template image path.
      MIL_TEXT_PTR m_TemplatePath;

      // The template image.
      MIL_ID m_MilTemplateImage;

      // The fixturing offset.
      MIL_ID m_MilFixturingOffset;
   };

#endif // TARGET_ALIGMNENT_TASK_H