//***************************************************************************************
//
// File name: MPRotateProcessing.h
//
// Synopsis:  Class that defines the processing specific to rotation.
//            This is to demonstrate how to insert user processing into
//            this example.
//

#ifndef MPROTATEPROCESSING_H
#define MPROTATEPROCESSING_H

//structure for processing objects
struct RotateProcessingStruct
   {
   MIL_ID   MilSourceBuffer;
   MIL_ID   MilDest1Buffer;
   MIL_ID   MilDest2Buffer;
   MIL_ID   MilDest3Buffer;
   MIL_ID   MilStructElement;

   MIL_DOUBLE RotationAngle;
   MIL_DOUBLE BufferCenterX;
   MIL_DOUBLE BufferCenterY;
   };
   
//*****************************************************************************
// Class used to define the processing done by the example.
// MP functionality is applied to the processing in this class
//*****************************************************************************
class CMPRotateProcessing : public CMPProcessing
   {
   public:
      CMPRotateProcessing(const MIL_TEXT_CHAR* Title, MIL_INT ProcessingIndex);
      virtual ~CMPRotateProcessing();

   protected:
      virtual void Process(MIL_INT ProcessingObjectIndex);

   private:
      //Disallow copy
      CMPRotateProcessing(const CMPProcessing&);
      CMPRotateProcessing& operator=(const CMPProcessing&);

      RotateProcessingStruct* m_ProcessingElements;
   };

#endif
