/********************************************************************************/
/* 
 * File name: MDigGrab.cpp 
 *
 * Synopsis:  This program demonstrates how to grab from a camera in
 *            continuous and monoshot mode.
 */
#include <mil.h> 

int MosMain(void)
{ 
   MIL_ID MilApplication,  /* Application identifier.  */
          MilSystem,       /* System identifier.       */
          MilDisplay,      /* Display identifier.      */
          MilDigitizer,    /* Digitizer identifier.    */ 
          MilImage;        /* Image buffer identifier. */

   /* Allocate defaults. */
   MappAllocDefault(M_DEFAULT, &MilApplication, &MilSystem,
                             &MilDisplay, &MilDigitizer, &MilImage);

   /* Grab continuously. */
   MdigGrabContinuous(MilDigitizer, MilImage);

   /* When a key is pressed, halt. */
   MosPrintf(MIL_TEXT("\nDIGITIZER ACQUISITION:\n"));
   MosPrintf(MIL_TEXT("----------------------\n\n"));
   MosPrintf(MIL_TEXT("Continuous image grab in progress.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to stop.\n\n"));
   MosGetch();

   /* Stop continuous grab. */
   MdigHalt(MilDigitizer);

   /* Pause to show the result. */
   MosPrintf(MIL_TEXT("Continuous grab stopped.\n\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to do a single image grab.\n\n"));
   MosGetch();

   /* Monoshot grab. */
   MdigGrab(MilDigitizer, MilImage);

   /* Pause to show the result. */
   MosPrintf(MIL_TEXT("Displaying the grabbed image.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to end.\n\n"));
   MosGetch();

   /* Free defaults. */
   MappFreeDefault(MilApplication, MilSystem, MilDisplay, MilDigitizer, MilImage);

   return 0;
}

