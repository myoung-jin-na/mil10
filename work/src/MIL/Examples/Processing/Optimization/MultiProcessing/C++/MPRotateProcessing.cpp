//***************************************************************************************/
// 
// File name: MPRotateProcessing.cpp  
//
// Synopsis:  Implements the CMPRotateProcessing class.
//

#include "common.h"

static const MIL_TEXT_CHAR* const ROTATE_PROCESSING_IMAGE = EXAMPLE_IMAGE_PATH MIL_TEXT("LargeWafer.mim");
static const MIL_DOUBLE           ROTATION_INCREMENT      = 8.0;

//*****************************************************************************
// Constructor. 
//*****************************************************************************
CMPRotateProcessing::CMPRotateProcessing(const MIL_TEXT_CHAR* Title, MIL_INT ProcessingIndex)
: CMPProcessing(Title,
                MbufDiskInquire(ROTATE_PROCESSING_IMAGE, M_SIZE_X,    M_NULL), 
                MbufDiskInquire(ROTATE_PROCESSING_IMAGE, M_SIZE_Y,    M_NULL),
                MbufDiskInquire(ROTATE_PROCESSING_IMAGE, M_TYPE,      M_NULL), 
                MbufDiskInquire(ROTATE_PROCESSING_IMAGE, M_SIZE_BAND, M_NULL),
                ProcessingIndex)
   {
   //Allocate the structure which will contain all processing object information.
   m_ProcessingElements = new RotateProcessingStruct[GetNumMemoryBank()+1];

   bool ValidBank=false;
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
   
      //Clear the buffers to initialize them
      MbufClear(m_ProcessingElements[i].MilSourceBuffer, 0.0);
      MbufClear(m_ProcessingElements[i].MilDest1Buffer, 0.0);
      MbufClear(m_ProcessingElements[i].MilDest2Buffer, 0.0);
      MbufClear(m_ProcessingElements[i].MilDest3Buffer, 0.0);
      MbufClear(m_ProcessingElements[i].MilStructElement, 0.0);

      //Load the source image
      MbufLoad(ROTATE_PROCESSING_IMAGE, m_ProcessingElements[i].MilSourceBuffer);
     
      //Initialize rotation information
      m_ProcessingElements[i].RotationAngle = ROTATION_INCREMENT;
      m_ProcessingElements[i].BufferCenterX = (MIL_DOUBLE)(GetBufferSizeX()/2);
      m_ProcessingElements[i].BufferCenterY = (MIL_DOUBLE)(GetBufferSizeY()/2);
     }

   }

//*****************************************************************************
// Destructor. 
//*****************************************************************************
CMPRotateProcessing::~CMPRotateProcessing()
   {
   //Stop the thread
   StopThread();

   //Free the processing objects
   for (MIL_INT i=0; i<=GetNumMemoryBank(); i++)
      {   
      // Free MIL objects.  
      MbufFree(m_ProcessingElements[i].MilSourceBuffer);
      MbufFree(m_ProcessingElements[i].MilDest1Buffer);
      MbufFree(m_ProcessingElements[i].MilDest2Buffer);
      MbufFree(m_ProcessingElements[i].MilDest3Buffer);
      MbufFree(m_ProcessingElements[i].MilStructElement);
     }

   //Free the memory for the processing elements structure
   delete [] m_ProcessingElements;
   }

//*****************************************************************************
// Process. Do the processing.
//*****************************************************************************
void CMPRotateProcessing::Process(MIL_INT ProcessingObjectIndex)
   {
   //Do processing using the buffers allocated on the specified memory location.
   //Get the contours
   MimMorphic(m_ProcessingElements[ProcessingObjectIndex].MilSourceBuffer, m_ProcessingElements[ProcessingObjectIndex].MilDest1Buffer, 
      m_ProcessingElements[ProcessingObjectIndex].MilStructElement, M_DILATE, 1, M_GRAYSCALE);

   MimMorphic(m_ProcessingElements[ProcessingObjectIndex].MilSourceBuffer, m_ProcessingElements[ProcessingObjectIndex].MilDest2Buffer, 
      m_ProcessingElements[ProcessingObjectIndex].MilStructElement, M_ERODE,  1, M_GRAYSCALE);

   MimArith(m_ProcessingElements[ProcessingObjectIndex].MilDest1Buffer, m_ProcessingElements[ProcessingObjectIndex].MilDest2Buffer,
      m_ProcessingElements[ProcessingObjectIndex].MilDest3Buffer, M_SUB);

   //Do the rotation
   MimRotate( m_ProcessingElements[ProcessingObjectIndex].MilDest3Buffer, 
      m_ProcessingElements[ProcessingObjectIndex].MilDest2Buffer,
      -m_ProcessingElements[ProcessingObjectIndex].RotationAngle,
      m_ProcessingElements[ProcessingObjectIndex].BufferCenterX,
      m_ProcessingElements[ProcessingObjectIndex].BufferCenterY,
      m_ProcessingElements[ProcessingObjectIndex].BufferCenterX,
      m_ProcessingElements[ProcessingObjectIndex].BufferCenterY,
      M_BILINEAR+M_OVERSCAN_CLEAR);

   //Prepare the rotation angle for next call.
   m_ProcessingElements[ProcessingObjectIndex].RotationAngle += ROTATION_INCREMENT;

   if (m_ProcessingElements[ProcessingObjectIndex].RotationAngle > 360.0)
      m_ProcessingElements[ProcessingObjectIndex].RotationAngle = ROTATION_INCREMENT;

   //Make the image brighter to make the result more visible
   const MIL_INT PixelAdjustmentFactor = 3;
   MimArith(m_ProcessingElements[ProcessingObjectIndex].MilDest2Buffer, PixelAdjustmentFactor, 
      m_ProcessingElements[ProcessingObjectIndex].MilDest3Buffer, M_MULT_CONST);

   //Copy the result to the display if it is not disabled and Update the information in the display title
   UpdateDisplay(m_ProcessingElements[ProcessingObjectIndex].MilDest3Buffer);
   }
