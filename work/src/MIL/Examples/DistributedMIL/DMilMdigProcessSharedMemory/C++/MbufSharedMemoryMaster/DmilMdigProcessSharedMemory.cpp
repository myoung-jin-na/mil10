 
/*************************************************************************************/
/*
 * File name: DMilMdigProcessSharedMemory.cpp
 *
 * Synopsis:  This program shows the use of Distributed MIL (DMIL) and the MdigProcess() 
 *            function to do image capture using a 32 bit MIL process running on a 64 bit 
 *            OS.
 *
 *            The 32 bit process uses DMIL to send commands to a 64 bit MIL 
 *            and its device drivers. Images are acquired by the 64 bit DMIL Slave
 *            System and the grabbed images are copied very quickly to the 32 bit 
 *            MIL process via inter-process shared memory. 
 *
 *            32 bit MIL and 64 bit MIL with the DMIL option must be installed on 
 *            the same PC that will run the project. The project must be compiled
 *            in both Win32 and x64 platform so that all the necessary components 
 *            are generated and the DMIL DLL is copied to the proper directory on 
 *            the local PC.
 *
 *            If USE_INTERPROCESS_SHARED_MEMORY_COPY is set to M_YES, this example 
 *            will copy the images grabbed by the 64 bit MIL to the 32 bit process 
 *            using inter-process shared memory.
 *
 *            The user's processing code is located in a hook function that
 *            will be called for each grabbed frame (see ProcessingFunction()).
 *
 */

#include <windows.h>
#include <mil.h>

/* Specification of the target DMIL system.                      */
/* Format is: "DmilProtocolToUse://TargetPcName/TargetMilSystem" */
#if M_MIL_USE_CE
#define SLAVE_SYSTEM_DESCRIPTOR   MIL_TEXT("dmiltcp://localhost/M_SYSTEM_DEFAULT")
#else
#define SLAVE_SYSTEM_DESCRIPTOR   M_SYSTEM_DEFAULT
#endif

#define SLAVE_DLL_NAME MIL_TEXT("mbufsharedmemory")

#if M_MIL_USE_WINDOWS && !M_MIL_USE_CE
#define SLAVE_DLL_TARGET_NAME INSTALL_DIR MIL_TEXT("mil\\dll\\") SLAVE_DLL_NAME MIL_TEXT(".dll")
#elif M_MIL_USE_CE
#define SLAVE_DLL_TARGET_NAME INSTALL_DIR SLAVE_DLL_NAME MIL_TEXT(".dll")
#elif M_MIL_USE_LINUX
#define SLAVE_DLL_TARGET_NAME INSTALL_DIR MIL_TEXT("mil/lib/lib") SLAVE_DLL_NAME MIL_TEXT(".so")
#endif

/* Number of images in the buffering grab queue.  */
#define BUFFERING_SIZE_MAX 20

/* User's processing function prototype. */
MIL_INT MFTYPE ProcessingFunction(MIL_INT HookType, MIL_ID HookId, void* HookDataPtr);

/* This section activate the code to use fast inter-process shared memory to copy the buffers grabbed */
/* This works only if the target Slave system is allocated on the same PC using "\localhost".         */
#define USE_INTERPROCESS_SHARED_MEMORY_COPY M_YES
#if USE_INTERPROCESS_SHARED_MEMORY_COPY
   #define SHARED_BUFFER_NAME MIL_TEXT("Global\\SharedBuffer0")
#endif
#include "../MbufSharedMemory.h"

/* User's processing function hook data structure. */
typedef struct
   {
   MIL_ID  MilRemoteDigitizer;
   MIL_ID  MilRemoteSharedImage;
   MIL_ID  MilLocalImageDisp;
   MIL_ID  MilLocalImageProc;
   MIL_ID  MilLocalSharedImage;
   MIL_INT ProcessedImageCount;
   } HookDataStruct;

// Master functions prototype
MIL_ID MFTYPE MbufAllocSharedMemory(MIL_ID MilSystem, MIL_INT SizeBand, 
   MIL_INT SizeX, MIL_INT SizeY, MIL_INT Type, MIL_INT DataFormat,
   MIL_INT ControlFlag,  MIL_INT PitchByte, MIL_CONST_TEXT_PTR SharedBufferName, 
   MIL_UINT64 *SharedBufferHandlePtr, MIL_ID *SharedBufferMilIdPtr);
void MFTYPE MbufFreeSharedMemory(MIL_UINT64 SharedBufferHandle, MIL_ID SharedBufferMilId);


/* Main function. */
/* ---------------*/

int MosMain(void)
{
   MIL_ID  MilApplication;
   MIL_ID  MilRemoteSystem;
   MIL_ID  MilRemoteDigitizer;
   MIL_ID  MilSystemOwnerApplication;/* System owner application.*/
   MIL_ID  MilRemoteGrabBufferList[BUFFERING_SIZE_MAX] = { 0 }, MilRemoteSharedImage = M_NULL;
   MIL_INT MilRemoteGrabBufferListSize;
   MIL_ID  MilLocalSystem;
   MIL_ID  MilLocalDisplay;
   MIL_ID  MilLocalImageProc, MilLocalImageDisp, MilLocalSharedImage = M_NULL;
   MIL_INT ProcessFrameCount = 0;
   MIL_INT NbFrames = 0, n = 0;
   MIL_INT BufferSize = 0;
   MIL_DOUBLE ProcessFrameRate = 0, Time = 0;
   HookDataStruct UserHookData;
   bool CheckExistence = false;

   
   /* Allocate the MIL Application. */
   MappAlloc(M_DEFAULT, &MilApplication);

   /* Allocate MIL Remotes MIL objects. */
   MsysAlloc(M_DEFAULT, SLAVE_SYSTEM_DESCRIPTOR, M_DEFAULT, M_DEFAULT, &MilRemoteSystem);

   // Now we check if the system is remote
   if(MsysInquire(MilRemoteSystem, M_LOCATION, M_NULL) != M_REMOTE)
      {
      MosPrintf(MIL_TEXT("This examples requires the default system to be a remote system.\n"));
      MosPrintf(MIL_TEXT("Please select a remote system as the default.\n"));
      MosPrintf(MIL_TEXT("If no remote systems are registered "));
      MosPrintf(MIL_TEXT("please go to the DistributedMIL->Connections page, "));
      MosPrintf(MIL_TEXT("register a remote system, "));
      MosPrintf(MIL_TEXT("and then select it as the default system.\n"));
      MsysFree(MilRemoteSystem);
      MappFree(MilApplication);
      MosGetch();
      return -1;
      }

   /* Inquire the system's owner application used to copy the slave dll with MappFileOperation. */
   MsysInquire(MilRemoteSystem, M_OWNER_APPLICATION, &MilSystemOwnerApplication);

   /* Copy the slave dll to the destination computer if they are compatible */
   if((MappInquire(M_DEFAULT, M_PLATFORM_BITNESS, M_NULL) == 
      MappInquire(MilSystemOwnerApplication, M_PLATFORM_BITNESS, M_NULL)) &&
      (MappInquire(M_DEFAULT, M_PLATFORM_OS_TYPE, M_NULL) == 
      MappInquire(MilSystemOwnerApplication, M_PLATFORM_OS_TYPE, M_NULL)))
      {
      MappControl(M_DEFAULT, M_ERROR, M_PRINT_DISABLE);
      MappFileOperation(M_DEFAULT, SLAVE_DLL_TARGET_NAME, MilSystemOwnerApplication, M_NULL, 
         M_FILE_COPY_MIL_DLL, M_DEFAULT, M_NULL);
      if(0 != MappGetError(M_DEFAULT, M_CURRENT, M_NULL))
         {
         // we have an error during the copy, check the existence
         MosPrintf(MIL_TEXT("There was an error while copying the slave library.\n"));
         MosPrintf(MIL_TEXT("Checking if one is present on the remote system.\n"));
         CheckExistence = true;
         }
      MappControl(M_DEFAULT, M_ERROR, M_PRINT_ENABLE);
      }
   else
      CheckExistence = true;

   if(CheckExistence)
      {
      MIL_INT DllExists = M_NO;

      MappFileOperation(MilSystemOwnerApplication, SLAVE_DLL_NAME, M_NULL, M_NULL, 
         M_FILE_EXISTS_MIL_DLL, M_DEFAULT, &DllExists);

      if(DllExists != M_YES)
         {
         MosPrintf(MIL_TEXT("The slave library was NOT copied to the remote system.\n"));
         MosPrintf(MIL_TEXT("Make sure it is present for the example to work properly.\n"));
         MosPrintf(MIL_TEXT("See DistributedMILExamples.txt in the DistributedMIL examples "));
         MosPrintf(MIL_TEXT("folder\nfor more information.\n"));
         MsysFree(MilRemoteSystem);
         MappFree(MilApplication);
         MosPrintf(MIL_TEXT("Press a key to terminate.\n\n"));
         MosGetch();
         return -1;
         }
      }


   MdigAlloc(MilRemoteSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &MilRemoteDigitizer);

   /* Allocate the remote grab buffers and clear them. */
   MappControl(M_DEFAULT, M_ERROR, M_PRINT_DISABLE);
   for(MilRemoteGrabBufferListSize = 0; MilRemoteGrabBufferListSize<BUFFERING_SIZE_MAX; 
         MilRemoteGrabBufferListSize++)
      {
      MbufAllocColor(MilRemoteSystem,
                  MdigInquire(MilRemoteDigitizer, M_SIZE_BAND, M_NULL),
                  MdigInquire(MilRemoteDigitizer, M_SIZE_X, M_NULL),
                  MdigInquire(MilRemoteDigitizer, M_SIZE_Y, M_NULL),
                  MdigInquire(MilRemoteDigitizer, M_TYPE, M_NULL),
                  M_IMAGE+M_GRAB+M_PROC,
                  &MilRemoteGrabBufferList[MilRemoteGrabBufferListSize]);

      if (MilRemoteGrabBufferList[MilRemoteGrabBufferListSize])
         MbufClear(MilRemoteGrabBufferList[MilRemoteGrabBufferListSize], 0xFF);
      else
         break;
      }
   MappControl(M_DEFAULT, M_ERROR, M_PRINT_ENABLE);

   /* Free few buffers to leave space for possible temporary buffers. */
   for (n=0; n<2 && MilRemoteGrabBufferListSize; n++)
      {
      MilRemoteGrabBufferListSize--;
      MbufFree(MilRemoteGrabBufferList[MilRemoteGrabBufferListSize]);
      }

   /* Allocate MIL Local MIL objects. */
   MsysAlloc(M_SYSTEM_HOST, M_DEFAULT, M_DEFAULT, &MilLocalSystem);
   MdispAlloc(MilLocalSystem, M_DEV0, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &MilLocalDisplay);
   MbufAllocColor(MilLocalSystem, 
               MdigInquire(MilRemoteDigitizer, M_SIZE_BAND, M_NULL),
               MdigInquire(MilRemoteDigitizer, M_SIZE_X, M_NULL), 
               MdigInquire(MilRemoteDigitizer, M_SIZE_Y, M_NULL),  
               MdigInquire(MilRemoteDigitizer, M_TYPE, M_NULL),
               M_IMAGE+M_PROC, &MilLocalImageProc);
   MbufClear(MilLocalImageProc, 0); 
   MbufAllocColor(MilLocalSystem, 
               MdigInquire(MilRemoteDigitizer, M_SIZE_BAND, M_NULL),
               MdigInquire(MilRemoteDigitizer, M_SIZE_X, M_NULL),
               MdigInquire(MilRemoteDigitizer, M_SIZE_Y, M_NULL),
               MdigInquire(MilRemoteDigitizer, M_TYPE, M_NULL),
               M_IMAGE+M_PROC+M_DISP, &MilLocalImageDisp);
   MbufClear(MilLocalImageDisp, 0); 
   MdispSelect(MilLocalDisplay, MilLocalImageDisp); 

      /* Print a start message. */
   MosPrintf(MIL_TEXT("\nDMIL REMOTE CAPTURE WITH LOCAL PROCESSING.\n"));
   MosPrintf(MIL_TEXT("------------------------------------------\n\n"));


   /* This section activate the code to use fast inter-process shared memory to copy the */
   /* buffers grabbed. This works only if the target Slave system is allocated on the    */
   /* same PC using "\localhost".         */
   #if USE_INTERPROCESS_SHARED_MEMORY_COPY

   /* Variables for shared inter-process object handling. */
   MIL_TEXT_CHAR   SharedBufferName[] = SHARED_BUFFER_NAME;   
   MIL_UINT64  SharedBufferRemoteHandle, SharedBufferLocalHandle; 

   /* Allocate a Local buffer residing in sharable inter-process memory. */
   MbufAllocSharedMemory(MilLocalSystem, 
                         MbufInquire(MilRemoteGrabBufferList[0], M_SIZE_BAND, M_NULL),
                         MbufInquire(MilRemoteGrabBufferList[0], M_SIZE_X, M_NULL),
                         MbufInquire(MilRemoteGrabBufferList[0], M_SIZE_Y, M_NULL),  
                         MbufInquire(MilRemoteGrabBufferList[0], M_TYPE, M_NULL),  
                         M_IMAGE+M_PROC+
                         MbufInquire(MilRemoteGrabBufferList[0], M_DATA_FORMAT, M_NULL),
                         SHARED_MEMORY_ALLOCATE,
                         MbufInquire(MilRemoteGrabBufferList[0], M_PITCH_BYTE, M_NULL),  
                         SharedBufferName, &SharedBufferLocalHandle, &MilLocalSharedImage);
   MbufClear(MilLocalSharedImage, 0x80);

   /* Create a Remote buffer mapping to the same sharable inter-process memory. */
   MbufAllocSharedMemory(MilRemoteSystem, 
                         MbufInquire(MilLocalSharedImage, M_SIZE_BAND, M_NULL),
                         MbufInquire(MilLocalSharedImage, M_SIZE_X, M_NULL),
                         MbufInquire(MilLocalSharedImage, M_SIZE_Y, M_NULL),  
                         MbufInquire(MilLocalSharedImage, M_TYPE, M_NULL),  
                         M_IMAGE+M_PROC+MbufInquire(MilLocalSharedImage, M_DATA_FORMAT, M_NULL), 
                         SHARED_MEMORY_CREATE,
                         MbufInquire(MilLocalSharedImage, M_PITCH_BYTE, M_NULL),
                         SharedBufferName, &SharedBufferRemoteHandle, &MilRemoteSharedImage);
   MbufClear(MilRemoteSharedImage, 0x80);
    
   #endif // USE_INTERPROCESS_SHARED_MEMORY_COPY

   /* Initialize the User's processing function data structure. */
   UserHookData.MilRemoteDigitizer   = MilRemoteDigitizer;
   UserHookData.MilRemoteSharedImage = MilRemoteSharedImage;
   UserHookData.MilLocalImageProc    = MilLocalImageProc;
   UserHookData.MilLocalImageDisp    = MilLocalImageDisp;
   UserHookData.MilLocalSharedImage  = MilLocalSharedImage;
   UserHookData.ProcessedImageCount  = 0;

   /* Start the processing. The processing function is called for every frame grabbed. */
   MdigProcess(MilRemoteDigitizer, MilRemoteGrabBufferList, MilRemoteGrabBufferListSize,
                                   M_START, M_DEFAULT, ProcessingFunction, &UserHookData);


   /* NOTE: Now the main() is free to perform other tasks while the processing is executing. */
   /* -------------------------------------------------------------------------------------- */
   
   /* Print a message and wait for a key press after a minimum number of frames. */
   MosPrintf(MIL_TEXT("Press <Enter> to stop.\n\n"));
   MosGetch();

   /* Stop the processing. */
   MdigProcess(MilRemoteDigitizer, MilRemoteGrabBufferList, MilRemoteGrabBufferListSize,
                                   M_STOP, M_DEFAULT, ProcessingFunction, &UserHookData);
   
   /* Print statistics. */
   MdigInquire(MilRemoteDigitizer, M_PROCESS_FRAME_COUNT,  &ProcessFrameCount);
   MdigInquire(MilRemoteDigitizer, M_PROCESS_FRAME_RATE,   &ProcessFrameRate);
   MosPrintf(MIL_TEXT("\n\n%ld frames grabbed at %.1f frames/sec (%.1f ms/frame).\n"),
                          ProcessFrameCount, ProcessFrameRate, 1000.0/ProcessFrameRate);
   MosPrintf(MIL_TEXT("Press <Enter> to end.\n\n"));
   MosGetch();

   /* Free the grab buffers. */
   while(MilRemoteGrabBufferListSize > 0)
      MbufFree(MilRemoteGrabBufferList[--MilRemoteGrabBufferListSize]);

   /* Release MIL objects */
   #if USE_INTERPROCESS_SHARED_MEMORY_COPY
   
   MbufFreeSharedMemory(SharedBufferLocalHandle, MilLocalSharedImage);
   MbufFreeSharedMemory(SharedBufferRemoteHandle, MilRemoteSharedImage);
   #endif
   MdigFree(MilRemoteDigitizer);
   MsysFree(MilRemoteSystem);
   MbufFree(MilLocalImageDisp);
   MbufFree(MilLocalImageProc);
   MdispFree(MilLocalDisplay);
   MsysFree(MilLocalSystem);
   MappFree(MilApplication);

   return 0;
}


/* User's processing function called every time a grab buffer is modified. */
/* -----------------------------------------------------------------------*/

/* Local defines. */
#define STRING_LENGTH_MAX  20
#define STRING_POS_X       20
#define STRING_POS_Y       20

MIL_INT MFTYPE ProcessingFunction(MIL_INT HookType, MIL_ID HookId, void* HookDataPtr)
   {
   HookDataStruct *UserHookDataPtr = (HookDataStruct *)HookDataPtr;
   MIL_ID ModifiedBufferId, BufferToProcessId;
   MIL_TEXT_CHAR Text[STRING_LENGTH_MAX]= {MIL_TEXT('\0'),};

   /* Retrieve the MIL_ID of the grabbed buffer. */
   MdigGetHookInfo(HookId, M_MODIFIED_BUFFER+M_BUFFER_ID, &ModifiedBufferId);

   #if (USE_INTERPROCESS_SHARED_MEMORY_COPY == M_NO)
   
   /* Copy the remote grabbed image locally using normal DMIL communication. */
   MbufCopy(ModifiedBufferId, UserHookDataPtr->MilLocalImageProc);

   /* Use the local buffer for processing. */
   BufferToProcessId = UserHookDataPtr->MilLocalImageProc;

   #else
  
   /* Copy the remote grabbed image locally using a shared memory buffer to be faster. 
      Here, UserHookDataPtr->MilRemoteSharedImage and UserHookDataPtr->MilLocalSharedImage
      MIL buffer maps to the same shared interprocess memory buffer. So first the 
      remote Slave copies the grab buffer to the shared buffer using its "Remote" Id.
      Then the processing can be done directly on this same shared buffer using its "Local" Id.
   */

   /* Copy the image grabbed remotely to a buffer residing in shared memory between */
   /* the 2 processes. */
   MbufCopy(ModifiedBufferId, UserHookDataPtr->MilRemoteSharedImage);

   /* Make sure the copy is completed on the slave before to access the shared memory buffer. */
   MthrWait(M_DEFAULT, M_THREAD_WAIT, M_NULL);

   /* Use directly the buffer residing in shared memory between the 2 processes for processing. */
   BufferToProcessId = UserHookDataPtr->MilLocalSharedImage;
   
   #endif // USE_INTERPROCESS_SHARED_MEMORY_COPY
   
   /* Print and draw the frame count locally. */
   UserHookDataPtr->ProcessedImageCount++;
   MosPrintf(MIL_TEXT("Processing frame #%d.\r"), UserHookDataPtr->ProcessedImageCount);
   MosSprintf(Text, STRING_LENGTH_MAX, MIL_TEXT("%ld"), 
                                       UserHookDataPtr->ProcessedImageCount);
   MgraText(M_DEFAULT, BufferToProcessId, STRING_POS_X, STRING_POS_Y, Text);

   /* Perform the processing and update the display locally. */
   MbufCopy(BufferToProcessId, UserHookDataPtr->MilLocalImageDisp);
   //MimArith(BufferToProcessId, M_NULL, UserHookDataPtr->MilLocalImageDisp, M_NOT);
   
   return 0;
   }

// Slave functions prototypes
MIL_ID MFTYPE MbufAllocSharedMemory(MIL_ID MilSystem, MIL_INT SizeBand, 
                             MIL_INT SizeX, MIL_INT SizeY, MIL_INT Type, MIL_INT DataFormat,
                             MIL_INT ControlFlag, MIL_INT PitchByte, 
                             MIL_CONST_TEXT_PTR SharedBufferName, 
                             MIL_UINT64 *SharedBufferHandlePtr, MIL_ID *SharedBufferMilIdPtr)
   {
   MIL_ID   Func;
   MIL_INT SharedBufferNameLength = MosStrlen(SharedBufferName)+1;
   
   /* Allocate a MIL function context that will be used to call a target 
      Slave function locally on the Host to do the processing.
   */
   MfuncAlloc(MIL_TEXT("MbufAllocSharedMemory"), 
              ALLOC_SHARED_MEMORY_NB_PARAM,
              M_NULL,
              SLAVE_DLL_NAME, 
              MIL_TEXT("SlaveAllocSharedMemory"), 
              M_USER_MODULE_1+ALLOC_SHARED_MEMORY_OPCODE, 
              M_SYNCHRONOUS_FUNCTION, 
              &Func
             );

   /* Execute requested operation */
   if ((ControlFlag == SHARED_MEMORY_ALLOCATE) || (ControlFlag == SHARED_MEMORY_CREATE))
      {
      /* Register the parameters. */
      MfuncParamMilId (Func, 1, MilSystem, M_SYSTEM, M_IN);
      MfuncParamMilInt(Func, 2, SizeBand);
      MfuncParamMilInt(Func, 3, SizeX);
      MfuncParamMilInt(Func, 4, SizeY);
      MfuncParamMilInt(Func, 5, Type);
      MfuncParamMilInt(Func, 6, DataFormat);
      MfuncParamMilInt(Func, 7, ControlFlag);
      MfuncParamMilInt(Func, 8, PitchByte);
      MfuncParamMilInt(Func, 9, SharedBufferNameLength);
      MfuncParamConstMilText(Func, 10, SharedBufferName, SharedBufferNameLength, M_IN);
      MfuncParamArrayMilUint64(Func, 11, SharedBufferHandlePtr, 1, M_OUT);
      MfuncParamArrayMilId(Func, 12, SharedBufferMilIdPtr, 1, M_IMAGE, M_OUT+M_AS_VALUE);

      /* Call the target Slave function. */
      MfuncCall(Func);

      /* Free the MIL function context. */
      MfuncFree(Func);
      }
   else 
      {  
      /* Report a MIL error. */
      MfuncErrorReport(Func,M_FUNC_ERROR+ALLOC_SHARED_MEMORY_PARAMETER_ERROR_CODE,
                       MIL_TEXT("Invalid parameter 7 (ControlFlag)."),
                       MIL_TEXT("Operation must be: SHARED_MEMORY_ALLOCATE or SHARED_MEMORY_CREATE."),
                       M_NULL,
                       M_NULL
                      );
      }

   return (*SharedBufferMilIdPtr);
   }

// Master function.
void MFTYPE MbufFreeSharedMemory(MIL_UINT64 SharedBufferHandle, MIL_ID SharedBufferMilId)
   {
   MIL_ID   Func;
      
   /* Allocate a MIL function context that will be used to call a target 
      Slave function locally on the Host to do the processing.
   */
   MfuncAlloc(MIL_TEXT("MbufFreeSharedMemory"), 
              FREE_SHARED_MEMORY_NB_PARAM,
              M_NULL,
              SLAVE_DLL_NAME, 
              MIL_TEXT("SlaveFreeSharedMemory"), 
              M_USER_MODULE_1+FREE_SHARED_MEMORY_OPCODE, 
              M_SYNCHRONOUS_FUNCTION, 
              &Func
             );

   /* Execute requested operation */
   if (SharedBufferHandle != M_NULL)
      {
      /* Register the parameters. */
      MfuncParamMilUint64(Func, 1, SharedBufferHandle);
      MfuncParamMilId(Func, 2, SharedBufferMilId, M_IMAGE, M_IN+M_AS_VALUE);

      /* Call the target Slave function. */
      MfuncCall(Func);

      /* Free the MIL function context. */
      MfuncFree(Func);
      }
   else 
      {  
      /* Report a MIL error. */
      MfuncErrorReport(Func, M_FUNC_ERROR+FREE_SHARED_MEMORY_PARAMETER_ERROR_CODE,
                       MIL_TEXT("Invalid Parameter."), MIL_TEXT("Handle cannot be NULL.)"), 
                       M_NULL, M_NULL);
      }
   }
