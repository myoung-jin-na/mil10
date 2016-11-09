//***************************************************************************************
//
// File name: MPWarpProcessing.h
//
// Synopsis:  Class that defines the processing specific to warp.
//            This is to demonstrate how to insert user processing into
//            this example.
//

#ifndef MPWARPPROCESSING_H
#define MPWARPPROCESSING_H

enum WarpDirection 
   {
   enWarpLeft,
   enWarpRight,
   enWarpFront,
   enWarpBack
   };
   
//structure for processing objects
struct WarpProcessingStruct
   {
   MIL_ID   MilSourceBuffer;
   MIL_ID   MilDest1Buffer;
   MIL_ID   MilDest2Buffer;
   MIL_ID   MilDest3Buffer;
   MIL_ID   MilStructElement;
   MIL_ID   MilFourCornerBuffer;
   MIL_ID   MilCoefficientsBuffer;
   };
  
//*****************************************************************************
// Class used to define the processing done by the example.
// MP functionality is applied to the processing in this class
//*****************************************************************************
class CMPWarpProcessing: public CMPProcessing
   {
   public:
      CMPWarpProcessing(const MIL_TEXT_CHAR* Title, MIL_INT ProcessingIndex);
      virtual ~CMPWarpProcessing();

   protected:
      virtual void Process(MIL_INT ProcessingObjectIndex);

   private:
      //Disallow copy
      CMPWarpProcessing(const CMPProcessing&);
      CMPWarpProcessing& operator=(const CMPProcessing&);

      void UpdateWarpCoefficients(MIL_INT ProcessingObjectIndex);
      void GenerateCoefficientsBuffer(MIL_INT ProcessingObjectIndex);

      WarpProcessingStruct* m_ProcessingElements;

      MIL_INT        m_IncrementCount;
      WarpDirection  m_Direction;
      MIL_FLOAT      m_FourCorners[12];
   };

#endif
