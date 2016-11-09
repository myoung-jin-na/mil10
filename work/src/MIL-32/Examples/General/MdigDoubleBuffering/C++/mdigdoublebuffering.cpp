/***************************************************************************************/
/*
 * File name: MdigDoubleBuffering.cpp
 *
 * Synopsis:  This program performs a double-buffering image acquisition that alternates 
 *            between 2 target buffers. This permits the processing of one buffer while 
 *            acquiring the next. 
 *
 *            The example also uses a hook callback function to the start of frames in 
 *            order to print the index of the current frame being acquired.
 *
 *     Note:  The double-buffering method is not recommended for real-time processing, 
 *            especially when the CPU usage is high. For more robust real-time behavior,
 *            use the MdigProcess() function. See MdigProcess.cpp for a complete example.
 */
#include <mil.h>
#include <stdlib.h>

/* Start of grab callback functions and data structure prototypes. */
MIL_INT MFTYPE GrabStart(MIL_INT, MIL_ID, void*);
typedef struct
   {
   MIL_INT     NbGrabStart;
   } UserDataStruct;

#define STRING_LENGTH_MAX  20


/* Main function. */
int MosMain(void)
{
   MIL_ID      MilApplication;
   MIL_ID      MilSystem     ;
   MIL_ID      MilDigitizer  ;
   MIL_ID      MilDisplay    ;
   MIL_ID      MilImage[2]   ;
   MIL_ID      MilImageDisp  ;
   long        NbProc = 0, n = 0;
   MIL_DOUBLE  Time = 0.0;
   MIL_TEXT_CHAR Text[STRING_LENGTH_MAX] = MIL_TEXT("0");
   UserDataStruct UserStruct;

   /* Allocate defaults. */
   MappAllocDefault(M_DEFAULT, &MilApplication, &MilSystem, &MilDisplay,
                                                &MilDigitizer, &MilImageDisp);

   /* Allocate 2 grab buffers. */
   for (n=0; n<2; n++)
       MbufAlloc2d(MilSystem,
                   MdigInquire(MilDigitizer, M_SIZE_X, M_NULL),
                   MdigInquire(MilDigitizer, M_SIZE_Y, M_NULL),
                   8L+M_UNSIGNED,
                   M_IMAGE+M_GRAB+M_PROC, &MilImage[n]);

   /* Hook a function to the start of each frame to print the current frame index. */
   UserStruct.NbGrabStart = 0;
   MdigHookFunction(MilDigitizer, M_GRAB_START, GrabStart, (void *)(&UserStruct));

   /* Print a message. */
   MosPrintf(MIL_TEXT("\nDOUBLE BUFFERING ACQUISITION AND PROCESSING:\n"));
   MosPrintf(MIL_TEXT("--------------------------------------------\n\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to stop.\n\n"));

   /* Put the digitizer in asynchronous mode to be able to process while grabbing. */
   MdigControl(MilDigitizer, M_GRAB_MODE, M_ASYNCHRONOUS);

   /* Grab the first buffer. */
   MdigGrab(MilDigitizer, MilImage[0]);

   /* Process one buffer while grabbing the other. */
   n=0;
   do {
      /* Grab the other buffer while processing the previous one. */
      MdigGrab(MilDigitizer, MilImage[1-n]);

      /* Synchronize and start the timer. */
      if (NbProc == 0)
         MappTimer(M_DEFAULT, M_TIMER_RESET+M_SYNCHRONOUS, M_NULL);

      /* Write the frame counter. */
      MosSprintf(Text, STRING_LENGTH_MAX, MIL_TEXT("%ld"), NbProc+1);
      MgraText(M_DEFAULT, MilImage[n], 32, 32, Text);

      /* Process the first buffer already grabbed.  */
      MimArith(MilImage[n], M_NULL, MilImageDisp, M_NOT);

      /* Count processed buffers. */
      NbProc++;

      /* Toggle grab buffers. */
      n = 1-n;
      }
   while(!MosKbhit());

   /* Wait until the end of the last grab and stop the timer. */
   MdigGrabWait(MilDigitizer, M_GRAB_END);
   MappTimer(M_DEFAULT, M_TIMER_READ+M_SYNCHRONOUS, &Time);
   MosGetch();

   /* Print statistics. */
   MosPrintf(MIL_TEXT("%ld frames processed, at a frame rate of %.2f frames/sec ")
             MIL_TEXT("(%.2f ms/frame).\n"), NbProc, NbProc/Time, 1000.0*Time/NbProc);
   MosPrintf(MIL_TEXT("Press <Enter> to end.\n\n"));
   MosGetch();

   /* Unhook the function at the start of each frame. */
   MdigHookFunction(MilDigitizer, M_GRAB_START+M_UNHOOK, GrabStart, (void *)(&UserStruct));

   /* Free allocations. */
   for (n=0; n<2; n++)
       MbufFree(MilImage[n]);
   MappFreeDefault(MilApplication, MilSystem, MilDisplay, MilDigitizer, MilImageDisp);

   return 0;
}

/* Grab Start hook function:
 * This function is called at the start of each frame captured.
 */
MIL_INT MFTYPE GrabStart(MIL_INT HookType, MIL_ID EventId, void* UserStructPtr)
{
  UserDataStruct *UserPtr=(UserDataStruct*)UserStructPtr;

  /* Increment grab start count and print it. */
  UserPtr->NbGrabStart++;
  MosPrintf(MIL_TEXT("#%ld\r"), UserPtr->NbGrabStart);

  return(0);
}
