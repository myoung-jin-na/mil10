//***************************************************************************************
// 
// File name: HighLevelInspectionTask.h
//
// Synopsis: This file contains the declaration of the CHighLevelInspectionTask class
//           the base class of all high level module, except blob.
//

#ifndef HIGH_LEVEL_INSPECTION_TASK_H
#define HIGH_LEVEL_INSPECTION_TASK_H

#include "InspectionTask.h"

typedef void (MFTYPE *FreeFuncPtr)(MIL_ID ObjectId);

class CHighLevelInspectionTask : public CInspectionTask
   {
   public:

      // Constructor.
      CHighLevelInspectionTask(MIL_CONST_TEXT_PTR ContextPath, FreeFuncPtr FreeFct, MIL_INT ColorConversion = M_NONE, CInspectionTask* FixtureProvider = M_NULL, CInspectionTask* ImageProvider = M_NULL, CInspectionTask* RegionProvider = M_NULL );

      // Destructor.
      virtual ~CHighLevelInspectionTask();

      // Initialization function.
      virtual void Init(MIL_ID MilSystem, MIL_INT ImageSizeX = 0, MIL_INT ImageSizeY = 0);

      // Free function.
      virtual void Free();

      // Inspection function.
      virtual bool Inspect(MIL_ID MilImage);      
      
      // Drawing functions.
      virtual void DrawGraphicalResult(MIL_ID MilGraContext, MIL_ID MilDest) = 0;
      virtual void DrawTextResult(MIL_ID MilGraContext, MIL_ID MilDest) = 0;

      // Accessors to the context and results.
      MIL_ID MilContext() const {return m_MilContext;}
      MIL_ID MilResult() const {return m_MilResult;}

   protected:
      
      // Calculation function to be implemented by the subclasses.
      virtual bool Calculate(MIL_ID MilImage) = 0;

      // Function to restore the context.
      virtual void Restore(MIL_ID MilSystem, MIL_CONST_TEXT_PTR ContextPath, MIL_ID *pMilContext, MIL_ID *pMilResult) = 0;

      // Function to set the region of the module.
      virtual void DefineRegion(MIL_ID MilImage) {};

      // Function that sets the output fixture.
      virtual void SetFixture() {};

      // Function that sets the output image.
      virtual void SetOutputImage(MIL_ID MilImage) {};

      // Function to preprocess the context.
      virtual void Preprocess() {};
      
   private:

      // The mil modules objects.
      MIL_ID m_MilContext;
      MIL_ID m_MilResult;

      // The string that holds the path to the context object.
      MIL_TEXT_PTR m_ContextPath;

      // A pointer to the freeing function.
      FreeFuncPtr m_FreeFct;
   };

#endif // HIGH_LEVEL_INSPECTION_TASK_H