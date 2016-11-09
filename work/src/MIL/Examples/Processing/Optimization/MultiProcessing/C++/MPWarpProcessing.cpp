//***************************************************************************************/
// 
// File name: MPWarpProcessing.cpp  
//
// Synopsis:  Implements the CMPWarpProcessing class.
//

#include "common.h"

static const MIL_TEXT_CHAR* const   WARP_PROCESSING_IMAGE = EXAMPLE_IMAGE_PATH MIL_TEXT("LargeWafer.mim");
static const MIL_INT WARP_INCREMENT = 40;  
static const MIL_INT MAX_INCREMENT = 20;

//*****************************************************************************
// Constructor. 
//*****************************************************************************
CMPWarpProcessing::CMPWarpProcessing(const MIL_TEXT_CHAR* Title, MIL_INT ProcessingIndex)
: CMPProcessing(Title,
                MbufDiskInquire(WARP_PROCESSING_IMAGE, M_SIZE_X,    M_NULL),
                MbufDiskInquire(WARP_PROCESSING_IMAGE, M_SIZE_Y,    M_NULL),
                MbufDiskInquire(WARP_PROCESSING_IMAGE, M_TYPE,      M_NULL),
                MbufDiskInquire(WARP_PROCESSING_IMAGE, M_SIZE_BAND, M_NULL),
                ProcessingIndex)
   {
   m_IncrementCount = 0;

   m_Direction = enWarpRight;
   //Setup the warp 4 corner parameters.

   //Move left
   m_FourCorners[0] = -(MAX_INCREMENT*WARP_INCREMENT)/2;
   m_FourCorners[1] = 0.0f;
   m_FourCorners[2] = (MIL_FLOAT)GetBufferSizeX()-1-(MAX_INCREMENT*WARP_INCREMENT)/2;
   m_FourCorners[3] = 0.0f;
   m_FourCorners[4] = (MIL_FLOAT)GetBufferSizeX()-1+(MAX_INCREMENT*WARP_INCREMENT)/2;
   m_FourCorners[5] = (MIL_FLOAT)GetBufferSizeY()-1;
   m_FourCorners[6] = (MAX_INCREMENT*WARP_INCREMENT)/2;
   m_FourCorners[7] = (MIL_FLOAT)GetBufferSizeY()-1;

   //Tilt back
   m_FourCorners[0] += (MAX_INCREMENT*WARP_INCREMENT)/2;
   m_FourCorners[1] += (MAX_INCREMENT*WARP_INCREMENT)/2;
   m_FourCorners[2] -= (MAX_INCREMENT*WARP_INCREMENT)/2;
   m_FourCorners[3] += (MAX_INCREMENT*WARP_INCREMENT)/2;
   m_FourCorners[4] += (MAX_INCREMENT*WARP_INCREMENT)/2;
   m_FourCorners[5] -= (MAX_INCREMENT*WARP_INCREMENT)/2;
   m_FourCorners[6] -= (MAX_INCREMENT*WARP_INCREMENT)/2;
   m_FourCorners[7] -= (MAX_INCREMENT*WARP_INCREMENT)/2;

   m_FourCorners[8]  = 0.0f;
   m_FourCorners[9]  = 0.0f;
   m_FourCorners[10] = (MIL_FLOAT)GetBufferSizeX()-1; 
   m_FourCorners[11] = (MIL_FLOAT)GetBufferSizeY()-1;

   //Allocate the structure which will contain all processing object information.
   m_ProcessingElements = new WarpProcessingStruct[GetNumMemoryBank()+1];

   bool ValidBank = false;
   MIL_INT64 MemoryBank = 0;
   for (MIL_INT i=0; i<=GetNumMemoryBank(); i++)
      {
      MemoryBank = GetMemoryBank(i-1, ValidBank);

      //Allocate required buffers for processing
      MbufAllocColor(GetSystemID(), GetBufferSizeBand(), GetBufferSizeX(), GetBufferSizeY(), GetBufferType(), 
         M_IMAGE+M_PROC+M_HOST_MEMORY+MemoryBank, &m_ProcessingElements[i].MilSourceBuffer);

      MbufAllocColor(GetSystemID(), GetBufferSizeBand(), GetBufferSizeX(), GetBufferSizeY(), GetBufferType(), 
         M_IMAGE+M_PROC+M_HOST_MEMORY+MemoryBank, &m_ProcessingElements[i].MilDest1Buffer);

      MbufAllocColor(GetSystemID(), GetBufferSizeBand(), GetBufferSizeX(), GetBufferSizeY(), GetBufferType(), 
         M_IMAGE+M_PROC+M_HOST_MEMORY+MemoryBank, &m_ProcessingElements[i].MilDest2Buffer);

      MbufAllocColor(GetSystemID(), GetBufferSizeBand(), GetBufferSizeX(), GetBufferSizeY(), GetBufferType(), 
         M_IMAGE+M_PROC+M_HOST_MEMORY+MemoryBank, &m_ProcessingElements[i].MilDest3Buffer);

      MbufAlloc2d(GetSystemID(), 3, 3, 32+M_FLOAT, M_STRUCT_ELEMENT+M_HOST_MEMORY+MemoryBank, 
         &m_ProcessingElements[i].MilStructElement);

      MbufAlloc1d(GetSystemID(), 12, 32+M_FLOAT, M_ARRAY+M_HOST_MEMORY+MemoryBank, 
         &m_ProcessingElements[i].MilFourCornerBuffer);

      MbufAlloc2d(GetSystemID(), 3, 3, 32+M_FLOAT, M_ARRAY+M_HOST_MEMORY+MemoryBank, 
         &m_ProcessingElements[i].MilCoefficientsBuffer);
   
      //Clear the buffers to initialize them
      MbufClear(m_ProcessingElements[i].MilSourceBuffer, 0.0);
      MbufClear(m_ProcessingElements[i].MilDest1Buffer, 0.0);
      MbufClear(m_ProcessingElements[i].MilDest2Buffer, 0.0);
      MbufClear(m_ProcessingElements[i].MilDest3Buffer, 0.0);
      MbufClear(m_ProcessingElements[i].MilStructElement, 0.0);
      MbufClear(m_ProcessingElements[i].MilFourCornerBuffer, 0.0);
      MbufClear(m_ProcessingElements[i].MilCoefficientsBuffer, 0.0);   

      //Load the source image
      MbufLoad(WARP_PROCESSING_IMAGE, m_ProcessingElements[i].MilSourceBuffer);

      //Generate the coefficients buffer for warping
      GenerateCoefficientsBuffer(i);
     }
   }

//*****************************************************************************
// Destructor. 
//*****************************************************************************
CMPWarpProcessing::~CMPWarpProcessing()
   {
   //Stop the thread
   StopThread();

   for (MIL_INT i=0; i<=GetNumMemoryBank(); i++)
      {
      // Free MIL objects.  
      MbufFree(m_ProcessingElements[i].MilSourceBuffer);
      MbufFree(m_ProcessingElements[i].MilDest1Buffer);
      MbufFree(m_ProcessingElements[i].MilDest2Buffer);
      MbufFree(m_ProcessingElements[i].MilDest3Buffer);
      MbufFree(m_ProcessingElements[i].MilStructElement);
      MbufFree(m_ProcessingElements[i].MilFourCornerBuffer);
      MbufFree(m_ProcessingElements[i].MilCoefficientsBuffer);
     }

   //Free processing information structure
   delete [] m_ProcessingElements;
   }

//*****************************************************************************
// Process. Do the processing.
//*****************************************************************************
void CMPWarpProcessing::Process(MIL_INT ProcessingObjectIndex)
   {
   //Do processing using the buffers allocated on the requested memory bank.
   //Get the contours
   MimMorphic(m_ProcessingElements[ProcessingObjectIndex].MilSourceBuffer, m_ProcessingElements[ProcessingObjectIndex].MilDest1Buffer, 
      m_ProcessingElements[ProcessingObjectIndex].MilStructElement, M_DILATE, 1, M_GRAYSCALE);

   MimMorphic(m_ProcessingElements[ProcessingObjectIndex].MilSourceBuffer, m_ProcessingElements[ProcessingObjectIndex].MilDest2Buffer, 
      m_ProcessingElements[ProcessingObjectIndex].MilStructElement, M_ERODE,  1, M_GRAYSCALE);

   MimArith(m_ProcessingElements[ProcessingObjectIndex].MilDest1Buffer, m_ProcessingElements[ProcessingObjectIndex].MilDest2Buffer, 
      m_ProcessingElements[ProcessingObjectIndex].MilDest3Buffer, M_SUB);

   //Warp the image
   MimWarp( m_ProcessingElements[ProcessingObjectIndex].MilDest3Buffer, m_ProcessingElements[ProcessingObjectIndex].MilDest2Buffer, 
            m_ProcessingElements[ProcessingObjectIndex].MilCoefficientsBuffer, 
            M_NULL, M_WARP_POLYNOMIAL, 
            M_BILINEAR+M_OVERSCAN_CLEAR);

   //Make the image brighter to make the result more visible
   const MIL_INT PixelAdjustmentFactor = 3;
   MimArith(m_ProcessingElements[ProcessingObjectIndex].MilDest2Buffer, PixelAdjustmentFactor, 
      m_ProcessingElements[ProcessingObjectIndex].MilDest3Buffer, M_MULT_CONST);

   //Update the warp coefficients
   UpdateWarpCoefficients(ProcessingObjectIndex);

   //Copy the result to the display if it is not disabled and Update the information in the display title
   UpdateDisplay(m_ProcessingElements[ProcessingObjectIndex].MilDest3Buffer);
   }

//*****************************************************************************
// UpdateWarpArray. Updates the warp array used for processing.
//*****************************************************************************
void CMPWarpProcessing::UpdateWarpCoefficients(MIL_INT ProcessingObjectIndex)
   {
   m_IncrementCount = (m_IncrementCount+1) % MAX_INCREMENT;

   //Setup the 4 corners according to the current direction.
   switch(m_Direction)
      {
      case enWarpRight:
         //Move the top to the right and the bottom towards the left.
         m_FourCorners[0] += WARP_INCREMENT;
         m_FourCorners[2] += WARP_INCREMENT;
         m_FourCorners[4] -= WARP_INCREMENT;
         m_FourCorners[6] -= WARP_INCREMENT;
         //Change direction if the increment has reached the maximum
         m_Direction = (m_IncrementCount==0)?enWarpFront:enWarpRight;
         break;
      case enWarpLeft:
         //Move the top to the left and the bottom to the right
         m_FourCorners[0] -= WARP_INCREMENT;
         m_FourCorners[2] -= WARP_INCREMENT;
         m_FourCorners[4] += WARP_INCREMENT;
         m_FourCorners[6] += WARP_INCREMENT;
         //Change direction if the increment has reached the maximum
         m_Direction = (m_IncrementCount==0)?enWarpBack:enWarpLeft;
         break;
      case enWarpFront:
         //Move the top to the front and the bottom to the back
         m_FourCorners[0] -= WARP_INCREMENT;

         if (m_IncrementCount<MAX_INCREMENT/2)
            m_FourCorners[1] -= WARP_INCREMENT;
         else
            m_FourCorners[1] += WARP_INCREMENT;

         m_FourCorners[2] += WARP_INCREMENT;

         if (m_IncrementCount<MAX_INCREMENT/2)
            m_FourCorners[3] -= WARP_INCREMENT;
         else
            m_FourCorners[3] += WARP_INCREMENT;

         m_FourCorners[4] -= WARP_INCREMENT;

         if (m_IncrementCount<MAX_INCREMENT/2)
            m_FourCorners[5] += WARP_INCREMENT;
         else
            m_FourCorners[5] -= WARP_INCREMENT;

         m_FourCorners[6] += WARP_INCREMENT;

         if (m_IncrementCount<MAX_INCREMENT/2)
            m_FourCorners[7] += WARP_INCREMENT;
         else
            m_FourCorners[7] -= WARP_INCREMENT;

         //Change direction if the increment has reached the maximum
         m_Direction = (m_IncrementCount==0)?enWarpLeft:enWarpFront;
         break;
      case enWarpBack:
         //Move the top to the back and the bottom to the front
         m_FourCorners[0] += WARP_INCREMENT;

         if (m_IncrementCount<MAX_INCREMENT/2)
            m_FourCorners[1] -= WARP_INCREMENT;
         else
            m_FourCorners[1] += WARP_INCREMENT;

         m_FourCorners[2] -= WARP_INCREMENT;

         if (m_IncrementCount<MAX_INCREMENT/2)
            m_FourCorners[3] -= WARP_INCREMENT;
         else
            m_FourCorners[3] += WARP_INCREMENT;

         m_FourCorners[4] += WARP_INCREMENT;

         if (m_IncrementCount<MAX_INCREMENT/2)
            m_FourCorners[5] += WARP_INCREMENT;
         else
            m_FourCorners[5] -= WARP_INCREMENT;

         m_FourCorners[6] -= WARP_INCREMENT;

         if (m_IncrementCount<MAX_INCREMENT/2)
            m_FourCorners[7] += WARP_INCREMENT;
         else
            m_FourCorners[7] -= WARP_INCREMENT;

         //Change direction if the increment has reached the maximum
         m_Direction = (m_IncrementCount==0)?enWarpRight:enWarpBack;
         break;
      default:
         break;
      }

   //Generate the coefficients buffer for warping
   GenerateCoefficientsBuffer(ProcessingObjectIndex);
   }

//*****************************************************************************
// GenerateCoefficientsBuffer. Generates the coefficients buffer for warping.
//*****************************************************************************
void CMPWarpProcessing::GenerateCoefficientsBuffer(MIL_INT ProcessingObjectIndex)
   {
   MbufPut(m_ProcessingElements[ProcessingObjectIndex].MilFourCornerBuffer, m_FourCorners);

   MgenWarpParameter(m_ProcessingElements[ProcessingObjectIndex].MilFourCornerBuffer,
      m_ProcessingElements[ProcessingObjectIndex].MilCoefficientsBuffer, M_NULL, 
      M_WARP_4_CORNER_REVERSE, M_DEFAULT, M_NULL, M_NULL);
   }
