//***************************************************************************************
// 
// File name: ReadCodetask.h
//
// Synopsis: This file contains the declaration of the CReadCodeTask class
//           which ifs the base class for reading a code.
//

#ifndef READ_CODE_TASK_H
#define READ_CODE_TASK_H

#include "ReadingTask.h"

class CReadCodeTask : public CReadTask
   {
   public:

      // Constructor.
      CReadCodeTask(MIL_CONST_TEXT_PTR CodeContextPath, MIL_INT ColorConversion = M_NONE, CInspectionTask* FixtureProvider = M_NULL, CInspectionTask* ImageProvider = M_NULL, CInspectionTask* RegionProvider = M_NULL)
       : CReadTask(CodeContextPath, McodeFree, ColorConversion, FixtureProvider, ImageProvider, RegionProvider)
         {};

      // Destructor.
      virtual ~CReadCodeTask(){};
      
      // Read function.
      virtual void Read(MIL_ID MilImage)
         {
         McodeRead(MilContext(), MilImage, MilResult());
         }

      // Restore function.
      virtual void Restore(MIL_ID MilSystem, MIL_CONST_TEXT_PTR ContextPath, MIL_ID *pMilContext, MIL_ID *pMilResult)
         {
         McodeRestore(ContextPath, MilSystem, M_DEFAULT, pMilContext);
         McodeAllocResult(MilSystem, M_DEFAULT, pMilResult);
         }

      // Get result.
      virtual bool GetReadStringResult(MIL_TEXT_PTR &ReadString)
         {
         MIL_INT Status, StringLength;
         McodeGetResult(MilResult(), M_STATUS + M_TYPE_MIL_INT, &Status);
         if(Status == M_STATUS_READ_OK)
            {
            McodeGetResult(MilResult(), M_STRING_SIZE + M_TYPE_MIL_INT, &StringLength);
            ReadString = new MIL_TEXT_CHAR[StringLength+1];
            McodeGetResult(MilResult(), M_STRING, ReadString);
            return true;
            }         
         return false;
         }
      
      // Drawing functions.
      virtual void DrawGraphicalResult(MIL_ID MilGraContext, MIL_ID MilDest)
         {
         // Draw the code in the graphics list.
         MgraColor(MilGraContext, M_COLOR_BLUE);
         McodeDraw(MilGraContext, MilResult(), MilDest, M_DRAW_CODE, M_DEFAULT, M_DEFAULT);

         CReadTask::DrawGraphicalResult(MilGraContext, MilDest);
         }
      virtual void DrawTextResult(MIL_ID MilGraContext, MIL_ID MilDest) = 0;
      
   protected:

   private:   
   };

#endif // READ_CODE_TASK_H