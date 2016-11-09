// MbufSharedMemory.cpp : Defines the exported functions for the DLL application.
//
//            The functions defined here can be used to create MIL buffer mapping on
//            interprocess shared memory. This can be used to share data quickly between 
//            a 32 and a 64 bit process.

#include <windows.h>
#include "mil.h"
#include "../MbufSharedMemory.h"

#ifdef __cplusplus
extern "C"
{
#endif

void MFTYPE SlaveAllocSharedMemory(MIL_ID Func);
void * MFTYPE SharedMemoryOperation(MIL_INT ControlFlag, MIL_CONST_TEXT_PTR SharedBufferName, MIL_INT64 SharedBufferSize, 
                                    MIL_UINT64 *PtrToSharedBufferHandle, char **PtrToSharedBufferPtr);
void MFTYPE SlaveFreeSharedMemory(MIL_ID Func);
#ifdef __cplusplus
}
#endif

/* --------------------------------------------------------------------------------------- */
/* Functions to handle MIL buffers residing in a chunk of sharable inter-process memory.   */
/* --------------------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------------------- */
/* Allocate or Create a MIL buffer residing in a chunk of sharable inter-process memory.   */

// Slave function.
void MFTYPE SlaveAllocSharedMemory(MIL_ID Func)
   {
   MIL_ID MilSystem;
   MIL_INT SizeBand, SizeX, SizeY, Type, DataFormat, ControlFlag, PitchByte;
   MIL_INT SharedBufferNameLength;
   MIL_CONST_TEXT_PTR SharedBufferName;
   MIL_UINT64 *SharedBufferHandlePtr;
   MIL_ID  *SharedBufferMilIdPtr;
   MIL_INT SharedBufferBandSize;
   char *SharedBufferPtr, *SharedBufferBandPtr[3];
   
   /* Read the parameters including pointer to the Return Values data. */
   MfuncParamValueMilId(Func, 1, &MilSystem);
   MfuncParamValueMilInt(Func, 2, &SizeBand);
   MfuncParamValueMilInt(Func, 3, &SizeX);
   MfuncParamValueMilInt(Func, 4, &SizeY);
   MfuncParamValueMilInt(Func, 5, &Type);
   MfuncParamValueMilInt(Func, 6, &DataFormat); 
   MfuncParamValueMilInt(Func, 7, &ControlFlag);
   MfuncParamValueMilInt(Func, 8, &PitchByte);
   MfuncParamValueMilInt(Func, 9, &SharedBufferNameLength); 
   MfuncParamValueConstMilText(Func, 10, &SharedBufferName); 
   MfuncParamValueArrayMilUint64(Func, 11, &SharedBufferHandlePtr); 
   MfuncParamValueArrayMilId(Func, 12, &SharedBufferMilIdPtr); 

   /* Do the Operation */
   if ((ControlFlag == SHARED_MEMORY_ALLOCATE) || (ControlFlag == SHARED_MEMORY_CREATE))
      {
      // Allocate or Map the shared memory. 
      SharedMemoryOperation(ControlFlag, SharedBufferName, SizeBand*PitchByte*SizeY, 
                                                           SharedBufferHandlePtr, &SharedBufferPtr);

      // Create a MIL buffer on it if memory map file allocation or mapping was successful.
      if (SharedBufferPtr != M_NULL)
         {
         if (DataFormat & M_PLANAR) // If Planar = multiples pointers.
            {
            SharedBufferBandSize = PitchByte*SizeY;
            for (int n=0; n<SizeBand; n++)
                SharedBufferBandPtr[n] =  SharedBufferPtr+(n*SharedBufferBandSize);
            }
         else // Monochrome or Packed = One pointer.
            {
            SharedBufferBandPtr[0] =  SharedBufferPtr;
            }
         MbufCreateColor(MilSystem, SizeBand, SizeX, SizeY, Type, DataFormat, 
                         M_HOST_ADDRESS + M_PITCH_BYTE, PitchByte, (void **)&SharedBufferBandPtr, SharedBufferMilIdPtr);
         }
      else
         {  
         /* Report a MIL error. */
         MfuncErrorReport(Func,M_FUNC_ERROR+ALLOC_SHARED_MEMORY_MAPPING_FILE_ERROR_CODE,
                          MIL_TEXT("Shared Memory Operation: Could not create file mapping."), M_NULL, M_NULL, M_NULL);
         }
      }
   }


/* ----------------------------------------------------------------------- */
/* Free a MIL buffer residing in a chunk of sharable inter-process memory. */


// Slave function.
void MFTYPE SlaveFreeSharedMemory(MIL_ID Func)
   {
   MIL_UINT64 SharedBufferHandle;
   MIL_ID  SharedBufferMilId;
   char   *SharedBufferPtr;
   
  /* Read the parameters including pointer to the Return Values data. */
  MfuncParamValueMilUint64(Func, 1, &SharedBufferHandle); 
  MfuncParamValueMilId(Func, 2, &SharedBufferMilId); 

  // Read the memory pointer.
  MbufInquire(SharedBufferMilId, M_HOST_ADDRESS, &SharedBufferPtr);

  // Free the MIL buffer.
  MbufFree(SharedBufferMilId);
  
  // Free the existing shared memory. 
  SharedMemoryOperation(SHARED_MEMORY_FREE, NULL, NULL, &SharedBufferHandle, &SharedBufferPtr);
  }


/* ----------------------------------------------------------------------------------- */
/* Function to handle inter-process memory located in a maps to MemoryMapped file.     */
/* This can be used to exchange data between 2 MIL processes or between a DMIL Master  */
/* it's Slave if they resides in the same physical PC and use a common memory.         */
/* ------------------------------------------------------------------------------------*/

/* ---------------------------------------------------------------------------------------------- */
/* Allocate, Map or Free an array of inter-process shared memory located in a Memory Mapped file. */

void * MFTYPE SharedMemoryOperation(MIL_INT  ControlFlag, // ControlFlag: SHARED_MEMORY_ALLOCATE, SHARED_MEMORY_CREATE or SHARED_MEMORY_FREE.
                             MIL_CONST_TEXT_PTR SharedBufferName, // Name of the Memory Map File object.
                             MIL_INT64  SharedBufferSize,         // Size of Memory Map File object.
                             MIL_UINT64 *PtrToSharedBufferHandle,    // Pointer to store the Memory Map File object Handle.
                             char   **PtrToSharedBufferPtr)       // Pointer to store the Memory Map File memory pointer.
   {
   // Clear Handle and Pointer.
   *PtrToSharedBufferHandle = NULL;
   *PtrToSharedBufferPtr    = NULL;

   // Do the ControlFlag.

   if ((ControlFlag == SHARED_MEMORY_ALLOCATE) || (ControlFlag == SHARED_MEMORY_CREATE))
      {
      if (ControlFlag == SHARED_MEMORY_ALLOCATE)
         {
         /* Create the memory Mapped file */
         *PtrToSharedBufferHandle = (MIL_UINT)CreateFileMapping(
                       INVALID_HANDLE_VALUE,          // use paging file
                       NULL,                          // default security
                       PAGE_READWRITE,                // read/write access
                       (long) (SharedBufferSize>>32), // maximum object size (high-order DWORD)
                       (long) SharedBufferSize,       // maximum object size (low-order DWORD)
                       SharedBufferName);             // name of mapping object
         }
      else if (ControlFlag == SHARED_MEMORY_CREATE)
         {
         /* Open the memory Mapped file */
         *PtrToSharedBufferHandle = (MIL_UINT)OpenFileMapping(FILE_MAP_ALL_ACCESS, // read+write
                                                             FALSE,               // do not inherit the name
                                                             SharedBufferName);   // name of mapping object
         }
      if (*PtrToSharedBufferHandle == NULL)
         {
         /* Report a MIL error. */
         MfuncErrorReport(M_DEFAULT,GetLastError(),
                          MIL_TEXT("Shared Memory Operation: Could not create file mapping object."), M_NULL, M_NULL, M_NULL);
         // MosPrintf(TEXT("Shared Memory Operation: Could not create file mapping object (%d).\n"), GetLastError());
         return NULL;
         }

      /* Create the shared memory pointer. */
      *PtrToSharedBufferPtr = (char *) MapViewOfFile((HANDLE)*PtrToSharedBufferHandle,  
                                                      FILE_MAP_ALL_ACCESS, // read+write
                                                      0, 0, (size_t)SharedBufferSize);
      if (*PtrToSharedBufferPtr == NULL)
         {
         /* Report a MIL error. */
         MfuncErrorReport(M_DEFAULT,GetLastError(),
                          MIL_TEXT("Shared Memory Operation: Could not map view of file."), M_NULL, M_NULL, M_NULL);
         //MosPrintf(TEXT("Shared Memory Operation: Could not map view of file (%d).\n"), GetLastError());
         CloseHandle((HANDLE)*PtrToSharedBufferHandle);
         return NULL;
         }
      }
   else if(ControlFlag == SHARED_MEMORY_FREE)
      {
      UnmapViewOfFile(*PtrToSharedBufferPtr);
      *PtrToSharedBufferPtr = NULL;
      CloseHandle((HANDLE)*PtrToSharedBufferHandle);
      *PtrToSharedBufferHandle = NULL;
      }

   return *PtrToSharedBufferPtr;
   }

