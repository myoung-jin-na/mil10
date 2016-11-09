//***************************************************************************************
// 
// File name: ReadStrTask.h
//
// Synopsis: This file contains the declaration of the CReadStrTask class
//           which is the inspection task used to read a string with string reader
//

#ifndef READ_STR_TASK_H
#define READ_STR_TASK_H

#include "ReadingTask.h"

class CReadStrTask : public CReadTask
   {
   public:

      // Constructor.
      CReadStrTask(MIL_CONST_TEXT_PTR StrContextPath, MIL_INT ColorConversion = M_NONE, CInspectionTask* FixtureProvider = M_NULL, CInspectionTask* ImageProvider = M_NULL, CInspectionTask* RegionProvider = M_NULL)
       : CReadTask(StrContextPath, MstrFree, ColorConversion, FixtureProvider, ImageProvider, RegionProvider)
         {};

      // Destructor.
      virtual ~CReadStrTask(){};
      
      // Read function.
      virtual void Read(MIL_ID MilImage)
         {
         MstrRead(MilContext(), MilImage, MilResult());
         }
      
      // Get result performed by the sub class.
      virtual bool GetReadStringResult(MIL_TEXT_PTR &ReadString)
         {
         MIL_INT Status, StringLength;
         MstrGetResult(MilResult(), M_GENERAL, M_STRING_NUMBER + M_TYPE_MIL_INT, &Status);
         if(Status)
            {
            MstrGetResult(MilResult(), 0, M_FORMATTED_STRING_SIZE + M_TYPE_MIL_INT, &StringLength);
            ReadString = new MIL_TEXT_CHAR[StringLength+1];
            MstrGetResult(MilResult(), 0, M_FORMATTED_STRING + M_TYPE_TEXT_CHAR, ReadString);
            return true;
            }         
         return false;
         }

      // Drawing functions.
      virtual void DrawGraphicalResult(MIL_ID MilGraContext, MIL_ID MilDest)
         {
         // Draw the string in the graphics list.
         MgraColor(MilGraContext, M_COLOR_BLUE);
         MstrDraw(MilGraContext, MilResult(), MilDest, M_DRAW_STRING_CHAR_BOX, M_DEFAULT, M_NULL, M_DEFAULT);

         CReadTask::DrawGraphicalResult(MilGraContext, MilDest);
         }

      virtual void DrawTextResult(MIL_ID MilGraContext, MIL_ID MilDest) = 0;
      
   protected:

   private:   

      // Restore function.
      virtual void Restore(MIL_ID MilSystem, MIL_CONST_TEXT_PTR ContextPath, MIL_ID *pMilContext, MIL_ID *pMilResult)
         {
         MstrRestore(ContextPath, MilSystem, M_DEFAULT, pMilContext);
         MstrPreprocess(*pMilContext, M_DEFAULT);
         MstrAllocResult(MilSystem, M_DEFAULT, pMilResult);
         }

   };

#endif // READ_STR_TASK_H