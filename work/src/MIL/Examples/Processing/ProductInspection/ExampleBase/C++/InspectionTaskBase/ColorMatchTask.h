//***************************************************************************************
// 
// File name: ColorMatchTask.h
//
// Synopsis: This file contains the declaration of the CColorMatchTask class
//           which is the base task for any color match task.
//

#ifndef COLOR_MATCH_TASK_H
#define COLOR_MATCH_TASK_H

#include "HighLevelInspectionTask.h"
#include "RegionManager.h"

class CColorMatchTask : public CHighLevelInspectionTask, public CRegionMngr
   {
   public:

      // Constructor.
      CColorMatchTask(MIL_CONST_TEXT_PTR ColContextPath,
                      const MIL_INT* ExpectedMatches,
                      MIL_INT ColorConversion = M_NONE,
                      CInspectionTask* FixtureProvider = M_NULL,
                      CInspectionTask* ImageProvider = M_NULL,
                      CInspectionTask* RegionProvider = M_NULL);

      // Destructor.
      virtual ~CColorMatchTask();

      // Initialization function.
      virtual void Init(MIL_ID MilSystem, MIL_INT ImageSizeX = 0, MIL_INT ImageSizeY = 0); 
      
      // Free function.
      virtual void Free();      
            
      // Drawing functions
      virtual void DrawGraphicalResult(MIL_ID MilGraContext, MIL_ID MilDest);
      virtual void DrawTextResult(MIL_ID MilGraContext, MIL_ID MilDest);
      
   protected:

      // Inspection function.
      virtual bool Calculate(MIL_ID MilImage);

      // Function to get the label of the sample area.
      virtual MIL_CONST_TEXT_PTR GetProductLabel() const { return MIL_TEXT("Sample"); }
      virtual MIL_CONST_TEXT_PTR GetBestMatchLabel(MIL_INT MatchIdx) const = 0;
      
   private:
      
      // Function to restore the context.
      virtual void Restore(MIL_ID MilSystem, MIL_CONST_TEXT_PTR ContextPath, MIL_ID *pMilContext, MIL_ID *pMilResult)
         {
         McolRestore(ContextPath, MilSystem, M_DEFAULT, pMilContext);
         McolAllocResult(MilSystem, M_DEFAULT, M_DEFAULT, pMilResult);
         }
      
      // Function to preprocess the context.
      virtual void Preprocess() 
         {
         McolPreprocess(MilContext(), M_DEFAULT);
         }

      // The inspection result.
      MIL_INT m_NbAreas;
      MIL_INT *m_BestMatchProductIndexTable;
      
      // The expected result.
      const MIL_INT* m_ExpectedMatches;
   };
#endif // COLOR_MATCH_TASK_H