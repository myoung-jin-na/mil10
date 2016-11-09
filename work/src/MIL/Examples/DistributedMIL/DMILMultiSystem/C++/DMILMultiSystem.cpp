/**********************************************************************************************/
/*
 * File name: DistributedMilMultiSystem.cpp
 *
 * Synopsys: Multiple systems processing demo with equal processing load using Distributed MIL.
 *          
 *           Each buffer grabbed is processed with all the allocated systems.
 *           This means that when each system is fast enough, the frame 
 *           rate should be the camera frame rate times the number of systems.
 *
 *           The grab system is the one specified by MilConfig default values.
 *           The type of the processing systems is specified below.
 *
 *           Warning: Display is very often the limiting factor for speed. 
 *                    Disable display with DISPLAY_EACH_IMAGE_PROCESSED = M_NO if required.
 *
 *                    The default processing systems of this example are Host systems 
 *                    located on the same PC (see: Local DMIL systems = localhost).
 */

/* Headers. */
#include <mil.h> 

/* Processing systems specification. */
#define PROCESSING_SYSTEM_TYPE               M_SYSTEM_HOST /* Target system type for the processing. */
#define PROCESSING_SYSTEM_NUMBER             2     /* Number of processing system (Maximum 16, see SYSTEM_ADDRESSES). */
#define USE_GRAB_SYSTEM_AS_ONE_PROCESSOR     M_YES /* Use the grabbing system as one of the processing system. */
#define PROCESS_EACH_IMAGE_ON_ALL_SYSTEMS    M_YES /* Force each grabbed image to be processed by all the systems. */
#define DISPLAY_EACH_IMAGE_PROCESSED         M_YES  /* Force each processed image to be displayed (might affect frame rate). */

/* Protocol used by Distributed MIL for inter-systems communication. */
#define DISTRIBUTED_MIL_PROTOCOL             MIL_TEXT("dmiltcp")

/* IP addresses of the slave processing systems (Local DMIL systems = localhost) */
const MIL_TEXT_CHAR* SYSTEM_ADDRESSES[] = {
                                           MIL_TEXT("localhost"), MIL_TEXT("localhost"),
                                           MIL_TEXT("localhost"), MIL_TEXT("localhost"),
                                           MIL_TEXT("localhost"), MIL_TEXT("localhost"),
                                           MIL_TEXT("localhost"), MIL_TEXT("localhost"),
                                           MIL_TEXT("localhost"), MIL_TEXT("localhost"),
                                           MIL_TEXT("localhost"), MIL_TEXT("localhost"),
                                           MIL_TEXT("localhost"), MIL_TEXT("localhost"),
                                           MIL_TEXT("localhost"), MIL_TEXT("localhost")
                                          };
#define SYSTEM_DESCRIPTOR_SIZE            512

/* Processing buffers specification. */
#define BUFFER_PER_PROCESSOR           2
#define BUFFER_NUMBER                 (PROCESSING_SYSTEM_NUMBER * BUFFER_PER_PROCESSOR)
#define BUFFER_SCALE                   1.0
#define BUFFER_MAX_STRING_LENGTH       80
#define BUFFER_DRAW_RECT_NUMBER        20
#define BUFFER_DRAW_INWARD_STEP_NUMBER 10
#define BUFFER_DRAW_RECT_STEP          10

/* Number of grab buffers for MdigProcess(). */
#define GRAB_BUFFER_NUMBER             (2 * PROCESSING_SYSTEM_NUMBER)

/* User's Processing call back function and data structure. */
MIL_INT MFTYPE ProcessingFunction(MIL_INT HookType, MIL_ID EventId, void* CallBackDataPtr);
typedef struct
   {
   MIL_ID  MilDigitizer;
   MIL_ID  DispBuffer;
   MIL_ID *SrcProcBufferListPtr;
   MIL_ID *DstProcBufferListPtr;
   MIL_INT SizeX, SizeY, SizeBand;
   long    NbSystem;
   long    NbProc;
   double  Time;
   long    ProcessEachImageOnAllSystems;
   } ProcessingDataStruct;


/* ************************************************************************ */
/* This example performs multiple buffers processing using MdigProcess() on 
   multiple distributed processing systems.  
*/

/* Main function. */
/* -------------- */
int MosMain(void)
   { 
   MIL_ID MilApplication;
   MIL_ID MilSystem     ;
   MIL_ID MilDigitizer  ;
   MIL_ID MilDisplay    ;
   MIL_ID MilImageDisp  ;
   MIL_ID GrabBufferList[GRAB_BUFFER_NUMBER];
   MIL_ID ProcSystemList[PROCESSING_SYSTEM_NUMBER];  
   MIL_ID SrcProcBufferList[BUFFER_NUMBER];
   MIL_ID DstProcBufferList[BUFFER_NUMBER];
   MIL_INT SizeX, SizeY, SizeBand;
   MIL_TEXT_CHAR SystemDescriptor[SYSTEM_DESCRIPTOR_SIZE];
   long    NbSystem = 0, NbSystemToAllocate = PROCESSING_SYSTEM_NUMBER;
   MIL_INT    GrabFrameCount, n; 
   double SingleSystemProcessingRate, MultipleSystemProcessingRate;
   ProcessingDataStruct ProcessingData;

   /* Allocations and setup. */
   /* ---------------------- */

   /* MIL application allocation. */
   MappAlloc(M_NULL, M_DEFAULT, &MilApplication);

   /* Allocations on the default grab system. */
   MsysAlloc(M_DEFAULT, M_SYSTEM_DEFAULT, M_DEFAULT, M_DEFAULT, &MilSystem);
   
   // Now we check if the system is local
   if(MsysInquire(MilSystem, M_LOCATION, M_NULL) != M_LOCAL)
      {
      MosPrintf(MIL_TEXT("This example requires the default system to be a local system.\n"));
      MosPrintf(MIL_TEXT("Please select a local system as the default.\n"));

      MsysFree(MilSystem);
      MappFree(MilApplication);
      MosGetch();
      return -1;
      }

   MdispAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &MilDisplay);
   MdigAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &MilDigitizer);
   
   /* Inquire the digitizer's size. */
   SizeX    = ProcessingData.SizeX = (MIL_INT)(MdigInquire(MilDigitizer, M_SIZE_X, M_NULL)*BUFFER_SCALE);
   SizeY    = ProcessingData.SizeY = (MIL_INT)(MdigInquire(MilDigitizer, M_SIZE_Y, M_NULL)*BUFFER_SCALE);
   SizeBand = ProcessingData.SizeBand = (MIL_INT)(MdigInquire(MilDigitizer, M_SIZE_BAND, M_NULL));
   
   /* Allocate a display buffer and clear it. */
   MbufAllocColor(MilSystem, SizeBand, SizeX, SizeY, 8L+M_UNSIGNED, M_IMAGE+M_GRAB+M_DISP, &MilImageDisp);
   MbufClear(MilImageDisp, 0x0);            

   /* Display the processing result if activated (might be the limiting factor for speed). */
   if (DISPLAY_EACH_IMAGE_PROCESSED)
      MdispSelect(MilDisplay, MilImageDisp); 
      
   /* Allocate the grab buffers. */
   for (n=0; n< GRAB_BUFFER_NUMBER; n++)
      MbufAllocColor(MilSystem, SizeBand, SizeX, SizeY, 8L+M_UNSIGNED, M_IMAGE+M_GRAB, &GrabBufferList[n]);
      
   /* Allocate and order the required processing systems. */
   if (USE_GRAB_SYSTEM_AS_ONE_PROCESSOR)
      NbSystemToAllocate--;
   for (n=0; n<NbSystemToAllocate; n++)
      {
      /* Create a system descriptor: (Protocol://Address/System (Ex: dmiltcp://localhost/M_SYSTEM_HOST)) */
      MosSprintf(SystemDescriptor, SYSTEM_DESCRIPTOR_SIZE, MIL_TEXT("%s://%s/%s"), 
                 DISTRIBUTED_MIL_PROTOCOL, SYSTEM_ADDRESSES[n], PROCESSING_SYSTEM_TYPE);

      /* Allocate the system. */
      MsysAlloc(M_DEFAULT, SystemDescriptor, M_DEFAULT, M_DEFAULT, &ProcSystemList[n]);

      /* Count the sucessfully allocated processing systems. */
      if (ProcSystemList[n])
         NbSystem++;
      }

   /* If the grab system is used to process, we add it at the end of the processing system list .
      This permits to dispatch the job to the other remote systems before to use the grab system 
      itself to process synchronously.
    */
   if (USE_GRAB_SYSTEM_AS_ONE_PROCESSOR)
      {
      ProcSystemList[NbSystem] = MilSystem;
      NbSystem++;
      }

   /* Allocate and order the source and destination processing buffers alternating the target system. */
   for (n=0; n<(NbSystem*BUFFER_PER_PROCESSOR); n++)
      {
      MbufAllocColor(ProcSystemList[n%NbSystem], SizeBand, SizeX, SizeY, 8L+M_UNSIGNED, 
                                                 M_IMAGE+M_PROC, &SrcProcBufferList[n]);
      MbufAllocColor(ProcSystemList[n%NbSystem], SizeBand, SizeX, SizeY, 8L+M_UNSIGNED, 
                                                 M_IMAGE+M_PROC, &DstProcBufferList[n]);
      }

   /* Set the specified grab scale. */
   MdigControl(MilDigitizer, M_GRAB_SCALE, BUFFER_SCALE);
      
   /* Single system processing. */
   /* ------------------------- */

   /* Print a message. */
   /* Print a message. */
   MosPrintf(MIL_TEXT("\nDISTRIBUTED MIL PROCESSING:\n"));
   MosPrintf(MIL_TEXT("---------------------------\n\n"));
   MosPrintf(MIL_TEXT("1 System processing:\n"));
   
   /* Initialize processing variables. */
   ProcessingData.NbSystem = 1;
   ProcessingData.ProcessEachImageOnAllSystems = M_NO;
   ProcessingData.NbProc = 0;
   ProcessingData.MilDigitizer = MilDigitizer;
   ProcessingData.DispBuffer = MilImageDisp; 
   ProcessingData.SrcProcBufferListPtr = SrcProcBufferList;
   ProcessingData.DstProcBufferListPtr = DstProcBufferList;
      
   /* Start processing the buffers. */
   MdigProcess(MilDigitizer, GrabBufferList, GRAB_BUFFER_NUMBER, M_START, M_DEFAULT, 
                                                 ProcessingFunction, &ProcessingData);
      
   /* Wait for a key and stop the processing. */
   MosPrintf(MIL_TEXT("Press <Enter> to stop.\n\n"));  
   MosGetch();   
   MdigProcess(MilDigitizer, GrabBufferList, GRAB_BUFFER_NUMBER, M_STOP+M_WAIT, M_DEFAULT, 
                                                       ProcessingFunction, &ProcessingData);
 
   /* Print statistics. */
   if (ProcessingData.NbProc != 0)
      {
      SingleSystemProcessingRate = ProcessingData.NbProc/ProcessingData.Time;
      MdigInquire(MilDigitizer, M_PROCESS_FRAME_COUNT, &GrabFrameCount);
      MosPrintf(MIL_TEXT("%ld Frames grabbed, %ld Frames processed at %.1f frames/sec (%.1f ms/frame).\n"),
                GrabFrameCount, ProcessingData.NbProc, SingleSystemProcessingRate, 1000.0/SingleSystemProcessingRate); 
      }
   else
      MosPrintf(MIL_TEXT("No frame has been grabbed.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   /* Multiple systems processing. */
   /* ---------------------------- */

   /* Print a message. */
   MosPrintf(MIL_TEXT("%ld Systems processing:\n"), NbSystem);
   
   /* Halt continuous grab. */
   MdigHalt(MilDigitizer);

   /* Initialize processing variables. */
   ProcessingData.NbSystem = NbSystem;
   ProcessingData.ProcessEachImageOnAllSystems = PROCESS_EACH_IMAGE_ON_ALL_SYSTEMS;
   ProcessingData.NbProc = 0;
   ProcessingData.DispBuffer = MilImageDisp; 
   ProcessingData.SrcProcBufferListPtr = SrcProcBufferList;
   ProcessingData.DstProcBufferListPtr = DstProcBufferList;
   
   
   /* Start processing the buffers. */
   MdigProcess(MilDigitizer, GrabBufferList, GRAB_BUFFER_NUMBER, M_START, M_DEFAULT, 
                                                 ProcessingFunction, &ProcessingData);
      
   /* Wait for a key and stop the processing. */
   MosPrintf(MIL_TEXT("Press <Enter> to stop.\n\n"));  
   MosGetch();   
   MdigProcess(MilDigitizer, GrabBufferList, GRAB_BUFFER_NUMBER, M_STOP+M_WAIT, M_DEFAULT, 
                                                       ProcessingFunction, &ProcessingData);
 
   /* Print statistics. */
   if (ProcessingData.NbProc != 0)
      {
      MultipleSystemProcessingRate = ProcessingData.NbProc/ProcessingData.Time;
      MdigInquire(MilDigitizer, M_PROCESS_FRAME_COUNT, &GrabFrameCount);
      MosPrintf(MIL_TEXT("%ld Frames grabbed, %ld Frames processed at %.1f frames/sec (%.1f ms/frame).\n\n"),
                GrabFrameCount, ProcessingData.NbProc, MultipleSystemProcessingRate, 1000.0/MultipleSystemProcessingRate); 
      MosPrintf(MIL_TEXT("Speedup factor: %.1f.\n\n"),MultipleSystemProcessingRate/SingleSystemProcessingRate);
      if (DISPLAY_EACH_IMAGE_PROCESSED && ((long)((MultipleSystemProcessingRate/SingleSystemProcessingRate)+0.1) < NbSystem))
          MosPrintf(MIL_TEXT("Warning: Display might limit the processing speed. Disable it and retry.\n\n"));
      }
   else
      MosPrintf(MIL_TEXT("No frame has been grabbed.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to end.\n\n"));
   MosGetch();
   
   /* Free allocations. */
   /* ----------------- */

   for (n=0; n<GRAB_BUFFER_NUMBER; n++)
      MbufFree(GrabBufferList[n]);
   for (n=0; n<BUFFER_NUMBER; n++)
      {
      MbufFree(SrcProcBufferList[n]);
      MbufFree(DstProcBufferList[n]);
      }
   if (USE_GRAB_SYSTEM_AS_ONE_PROCESSOR)
      NbSystem--;
   for (n=0; n<NbSystem; n++)
      MsysFree(ProcSystemList[n]);
   MbufFree(MilImageDisp);
   MdispFree(MilDisplay);
   MdigFree(MilDigitizer);
   MsysFree(MilSystem);
   MappFree(MilApplication);

   return 0;
}  

/* Processing function called by MdigProcess(). */
/* -------------------------------------------- */

MIL_INT MFTYPE ProcessingFunction(MIL_INT HookType, MIL_ID EventId, void* CallBackDataPtr)
   {
   /* Get the grabbed buffer. */
   ProcessingDataStruct *ProcessingDataPtr = (ProcessingDataStruct *)CallBackDataPtr;
   MIL_ID CurrentProcSrcBufId, CurrentProcDstBufId; 
   MIL_ID GrabbedBufferId;
   MIL_INT GrabbedBufferIndex;
   MIL_TEXT_CHAR Text[BUFFER_MAX_STRING_LENGTH];
   MIL_DOUBLE RectHalfSizeStep = 0.0;
   long RectStep = 0;
   long NbSystem = ProcessingDataPtr->NbSystem;
   long NbProcInitial = ProcessingDataPtr->NbProc;
   long NbBufferToProcess, n;

   /* Retrieve the buffer to process and it's index */
   MdigGetHookInfo(EventId, M_MODIFIED_BUFFER+M_BUFFER_ID,    &GrabbedBufferId);
   MdigGetHookInfo(EventId, M_MODIFIED_BUFFER+M_BUFFER_INDEX, &GrabbedBufferIndex);

   /* Reset the timer. */
   if (ProcessingDataPtr->NbProc == 0)
      MappTimer(M_DEFAULT, M_TIMER_RESET+M_SYNCHRONOUS,&ProcessingDataPtr->Time);

   /* If PROCESS_EACH_IMAGE_ON_ALL_SYSTEMS is set, each grabbed image is processed on all the systems. 
      To have each frame processed only once by one system in round robin fashion, set the define to M_NO.  
   */
   if (ProcessingDataPtr->ProcessEachImageOnAllSystems)
      NbBufferToProcess = NbSystem;
   else
      NbBufferToProcess = 1;

   /* Dispatch the job to the target processing system(s) */ 
   for(n=0; n<NbBufferToProcess; n++)
      {
      /* Calculate target processing buffer. */
      CurrentProcSrcBufId = ProcessingDataPtr->SrcProcBufferListPtr[(ProcessingDataPtr->NbProc)%(NbSystem*BUFFER_PER_PROCESSOR)];
      CurrentProcDstBufId = ProcessingDataPtr->DstProcBufferListPtr[(ProcessingDataPtr->NbProc)%(NbSystem*BUFFER_PER_PROCESSOR)];

      /* Copy the grabbed buffer to a processing platform. */
      MbufCopy(GrabbedBufferId, CurrentProcSrcBufId);

      /* Draw the buffer index in the source. */
      MosSprintf(Text, BUFFER_MAX_STRING_LENGTH, MIL_TEXT("#%ld."), ProcessingDataPtr->NbProc);
      MgraText(M_DEFAULT, CurrentProcSrcBufId, 50, 50, Text);

      /* Process the buffer. */
      #if (!M_MIL_LITE)
          {
          MimArith(CurrentProcSrcBufId, 0x10, CurrentProcDstBufId, M_SUB_CONST+M_SATURATION);
          MimArith(CurrentProcDstBufId, M_NULL, CurrentProcSrcBufId, M_NOT);
          MimRotate(CurrentProcSrcBufId, CurrentProcDstBufId,
                    (ProcessingDataPtr->NbProc*10)%360, 
                    (MIL_DOUBLE) ProcessingDataPtr->SizeX/2,
                    (MIL_DOUBLE) ProcessingDataPtr->SizeY/2,
                    (MIL_DOUBLE) ProcessingDataPtr->SizeX/2,
                    (MIL_DOUBLE) ProcessingDataPtr->SizeY/2,
                    M_NEAREST_NEIGHBOR);
          }
      #else
         {
         RectStep = (ProcessingDataPtr->NbProc % BUFFER_DRAW_RECT_NUMBER);
          if(RectStep < BUFFER_DRAW_INWARD_STEP_NUMBER)
             RectHalfSizeStep = RectStep * BUFFER_DRAW_RECT_STEP;
          else
             RectHalfSizeStep = (BUFFER_DRAW_RECT_NUMBER - RectStep) * BUFFER_DRAW_RECT_STEP;

          MgraColor(M_DEFAULT, 0xff);
          MgraRectFill(M_DEFAULT,
                       CurrentProcDstBufId,
                       ProcessingDataPtr->SizeX/2 - RectHalfSizeStep,
                       ProcessingDataPtr->SizeY/2 - RectHalfSizeStep,
                       ProcessingDataPtr->SizeX/2 + RectHalfSizeStep,
                       ProcessingDataPtr->SizeY/2 + RectHalfSizeStep);
         }
      #endif

      /* Count processed buffers. */
      ProcessingDataPtr->NbProc++;
      MosPrintf(MIL_TEXT("Processing #%ld.\r"), ProcessingDataPtr->NbProc);
      }
      
      /* If required, copy back the result buffers from the processing system(s) for display. */
      #if (DISPLAY_EACH_IMAGE_PROCESSED)
         {
         for(n=0; n<NbBufferToProcess; n++)
           {
           /* Calculate target buffer for display. */
           CurrentProcDstBufId = ProcessingDataPtr->DstProcBufferListPtr[(NbProcInitial+n)%(NbSystem*BUFFER_PER_PROCESSOR)];

           /* Copy result to display. */
           MbufCopy(CurrentProcDstBufId, ProcessingDataPtr->DispBuffer);
           } 
         }
      #endif

   /* Read the timer. */
   MappTimer(M_DEFAULT, M_TIMER_READ+M_SYNCHRONOUS, &ProcessingDataPtr->Time);

   #if M_MIL_USE_CE
   /* Give execution time to user interface when the digitizer processing queue is full.
      If necessary, the Sleep value can be increased to give more execution time to user interface.
   */
   if(MdigInquire(ProcessingDataPtr->MilDigitizer, M_PROCESS_PENDING_GRAB_NUM, M_NULL) <= 1)
      {
      if ((ProcessingDataPtr->NbProc%10) == 0)
         Sleep(2);
      }
   #endif

   return(0);
   }
