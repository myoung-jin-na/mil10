/***************************************************************************************/
/*
 * File name: MdigProcess.cpp
 *
 * Synopsis:  This program shows the use of the MdigProcess() function and its multiple
 *            buffering acquisition to do robust real-time processing.
 *
 *            The user's processing code to execute is located in a callback function 
 *            that will be called for each frame acquired (see ProcessingFunction()).
 *
 *      Note: The average processing time must be shorter than the grab time or some
 *            frames will be missed. Also, if the processing results are not displayed
 *            and the frame count is not drawn or printed, the CPU usage is reduced 
 *            significantly.
 */
#include <mil.h>

/* Number of images in the buffering grab queue.
   Generally, increasing this number gives a better real-time grab.
 */
#define BUFFERING_SIZE_MAX 22

/* User's processing function prototype. */
MIL_INT MFTYPE ProcessingFunction(MIL_INT HookType, MIL_ID HookId, void* HookDataPtr);

/* User's processing function hook data structure. */
typedef struct
   {
   MIL_ID  MilDigitizer;
   MIL_ID  MilImageDisp;
   MIL_INT ProcessedImageCount;
   } HookDataStruct;


/* Main function. */
/* ---------------*/

int MosMain(void)
{
   MIL_ID MilApplication;
   MIL_ID MilSystem     ;
   MIL_ID MilDigitizer  ;
   MIL_ID MilDisplay    ;
   MIL_ID MilImageDisp  ;
   MIL_ID MilGrabBufferList[BUFFERING_SIZE_MAX] = { 0 };
   MIL_INT MilGrabBufferListSize;
   MIL_INT ProcessFrameCount   = 0;
   MIL_DOUBLE ProcessFrameRate = 0;
   MIL_INT NbFrames = 0, n = 0;
   HookDataStruct UserHookData;

   /* Allocate defaults. */
   MappAllocDefault(M_DEFAULT, &MilApplication, &MilSystem, &MilDisplay,
                                                &MilDigitizer, &MilImageDisp);

   /* Allocate the grab buffers and clear them. */
   MappControl(M_DEFAULT, M_ERROR, M_PRINT_DISABLE);
   for(MilGrabBufferListSize = 0; MilGrabBufferListSize<BUFFERING_SIZE_MAX;
                                                        MilGrabBufferListSize++)
      {
      MbufAlloc2d(MilSystem,
                  MdigInquire(MilDigitizer, M_SIZE_X, M_NULL),
                  MdigInquire(MilDigitizer, M_SIZE_Y, M_NULL),
                  8+M_UNSIGNED,
                  M_IMAGE+M_GRAB+M_PROC,
                  &MilGrabBufferList[MilGrabBufferListSize]);
      if (MilGrabBufferList[MilGrabBufferListSize])
         MbufClear(MilGrabBufferList[MilGrabBufferListSize], 0xFF);
      else
         break;
      }
   MappControl(M_DEFAULT, M_ERROR, M_PRINT_ENABLE);

   /* Free buffers to leave space for possible temporary buffers. */
   for (n=0; n<2 && MilGrabBufferListSize; n++)
      {
      MilGrabBufferListSize--;
      MbufFree(MilGrabBufferList[MilGrabBufferListSize]);
      }

   /* Print a message. */
   MosPrintf(MIL_TEXT("\nMULTIPLE BUFFERED PROCESSING.\n"));
   MosPrintf(MIL_TEXT("-----------------------------\n\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to start processing.\r"));

   /* Grab continuously on the display and wait for a key press. */
   MdigGrabContinuous(MilDigitizer, MilImageDisp);
   MosGetch();

   /* Halt continuous grab. */
   MdigHalt(MilDigitizer);

   /* Initialize the user's processing function data structure. */
   UserHookData.MilDigitizer        = MilDigitizer;
   UserHookData.MilImageDisp        = MilImageDisp;
   UserHookData.ProcessedImageCount = 0;

   /* Start the processing. The processing function is called with every frame grabbed. */
   MdigProcess(MilDigitizer, MilGrabBufferList, MilGrabBufferListSize,
               M_START, M_DEFAULT, ProcessingFunction, &UserHookData);


   /* Here the main() is free to perform other tasks while the processing is executing. */
   /* --------------------------------------------------------------------------------- */


   /* Print a message and wait for a key press after a minimum number of frames. */
   MosPrintf(MIL_TEXT("Press <Enter> to stop.                    \n\n"));
   MosGetch();

   /* Stop the processing. */
   MdigProcess(MilDigitizer, MilGrabBufferList, MilGrabBufferListSize,
               M_STOP, M_DEFAULT, ProcessingFunction, &UserHookData);

   /* Print statistics. */
   MdigInquire(MilDigitizer, M_PROCESS_FRAME_COUNT,  &ProcessFrameCount);
   MdigInquire(MilDigitizer, M_PROCESS_FRAME_RATE,   &ProcessFrameRate);
   MosPrintf(MIL_TEXT("\n\n%ld frames grabbed at %.1f frames/sec (%.1f ms/frame).\n"),
                        ProcessFrameCount, ProcessFrameRate, 1000.0/ProcessFrameRate);
   MosPrintf(MIL_TEXT("Press <Enter> to end.\n\n"));
   MosGetch();

   /* Free the grab buffers. */
   while(MilGrabBufferListSize > 0)
         MbufFree(MilGrabBufferList[--MilGrabBufferListSize]);

   /* Release defaults. */
   MappFreeDefault(MilApplication, MilSystem, MilDisplay, MilDigitizer, MilImageDisp);

   return 0;
}


/* User's processing function called every time a grab buffer is ready. */
/* -------------------------------------------------------------------- */

/* Local defines. */
#define STRING_LENGTH_MAX  20
#define STRING_POS_X       20
#define STRING_POS_Y       20

MIL_INT MFTYPE ProcessingFunction(MIL_INT HookType, MIL_ID HookId, void* HookDataPtr)
   {
   HookDataStruct *UserHookDataPtr = (HookDataStruct *)HookDataPtr;
   MIL_ID ModifiedBufferId;
   MIL_TEXT_CHAR Text[STRING_LENGTH_MAX]= {MIL_TEXT('\0'),};

   /* Retrieve the MIL_ID of the grabbed buffer. */
   MdigGetHookInfo(HookId, M_MODIFIED_BUFFER+M_BUFFER_ID, &ModifiedBufferId);

   /* Increment the frame counter. */
   UserHookDataPtr->ProcessedImageCount++;

   /* Print and draw the frame count (remove to reduce CPU usage). */
   MosPrintf(MIL_TEXT("Processing frame #%d.\r"), UserHookDataPtr->ProcessedImageCount);
   MosSprintf(Text, STRING_LENGTH_MAX, MIL_TEXT("%ld"), 
                                       UserHookDataPtr->ProcessedImageCount);
   MgraText(M_DEFAULT, ModifiedBufferId, STRING_POS_X, STRING_POS_Y, Text);

   /* Execute the processing and update the display. */
   MimArith(ModifiedBufferId, M_NULL, UserHookDataPtr->MilImageDisp, M_NOT);
   
   #if M_MIL_USE_CE
   /* Give execution time to the user interface when the digitizer processing 
      queue is full. If necessary, the Sleep value can be increased to give more 
      execution time to the user interface.
   */
   if(MdigInquire(UserHookDataPtr->MilDigitizer, M_PROCESS_PENDING_GRAB_NUM, M_NULL) <= 1)
      {
      if ((UserHookDataPtr->ProcessedImageCount%10) == 0)
         Sleep(2);
      }
   #endif

   return 0;
   }
