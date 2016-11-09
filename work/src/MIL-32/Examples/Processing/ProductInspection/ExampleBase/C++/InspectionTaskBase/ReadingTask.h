//***************************************************************************************
// 
// File name: ReadingTask.h
//
// Synopsis: This file contains the declaration of the CReadTask class
//           which is the inspection task used to read something.
//

#ifndef READING_TASK_H
#define READING_TASK_H

#include "HighLevelInspectionTask.h"
#include "RegionManager.h"

class CReadTask : public CHighLevelInspectionTask, public CRegionMngr
   {
   public:

      // Constructor.
      CReadTask(MIL_CONST_TEXT_PTR ContextPath, FreeFuncPtr FreeFct, MIL_INT ColorConversion = M_NONE, CInspectionTask* FixtureProvider = M_NULL, CInspectionTask* ImageProvider = M_NULL, CInspectionTask* RegionProvider = M_NULL);

      // Destructor.
      virtual ~CReadTask();

      // Initialization function.
      virtual void Init(MIL_ID MilSystem, MIL_INT ImageSizeX = 0, MIL_INT ImageSizeY = 0);
      
      // Free function.
      virtual void Free();      
                 
      // Drawing functions.
      virtual void DrawGraphicalResult(MIL_ID MilGraContext, MIL_ID MilDest);
      virtual void DrawTextResult(MIL_ID MilGraContext, MIL_ID MilDest) = 0;

      // Accessor.
      MIL_CONST_TEXT_PTR GetReadString()const{return m_ReadString ? m_ReadString : MIL_TEXT("No Read");}
      bool GetReadStatus() const{return m_ReadString != NULL;}
      
   protected:
      
      // Inspection function.
      virtual bool Calculate(MIL_ID MilImage);

      // Read function performed by the subclass.
      virtual void Read(MIL_ID MilImage) = 0;

      // Get result performed by the sub class.
      virtual bool GetReadStringResult(MIL_TEXT_PTR &ReadString) = 0;

      // Restore function performed by the subclass.
      virtual void Restore(MIL_ID MilSystem, MIL_CONST_TEXT_PTR ContextPath,  MIL_ID *pMilContext, MIL_ID *pMilResult) = 0;
    
   private:

      // The read string.
      MIL_TEXT_PTR m_ReadString;
      
      // Access of the set region function from the base highlevel function class.
      virtual void DefineRegion(MIL_ID MilImage);
   };

#endif // READING_TASK_H