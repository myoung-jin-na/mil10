/*****************************************************************************************/
/*
 * File name: DistributedMilSyncAsyncMain.cpp 
 *
 * Synopsis:  This example shows how to use the MIL Function Development module to 
 *            call custom synchronous and asynchronous MIL functions.
 *
 *            It contains the main to test the SynchronousFunction() and 
 *            AsynchronousFunction() master functions.
 *
 *            The slave functions can be found in the DistributedMILSyncAsyncSlave project.
 */
#include <mil.h>

/* Master MIL functions declarations */
MIL_INT MFTYPE SynchronousFunction(MIL_ID SrcImage, MIL_ID DstImage,  MIL_INT Option);
void MFTYPE AsynchronousFunction(MIL_ID SrcImage, MIL_ID DstImage, MIL_INT Option);

/* Target image file name */
#define IMAGE_FILE   M_IMAGE_PATH MIL_TEXT("Wafer.mim")
#define NB_LOOP      100

#if M_MIL_USE_CE
#define SLAVE_SYSTEM_DESCRIPTOR   MIL_TEXT("dmiltcp://localhost/M_SYSTEM_DEFAULT")
#else
#define SLAVE_SYSTEM_DESCRIPTOR   M_SYSTEM_DEFAULT
#endif

/* Slave dll path and name */
#define SLAVE_DLL_NAME MIL_TEXT("dmilsyncasyncslave")

#if M_MIL_USE_WINDOWS && !M_MIL_USE_CE
#define SLAVE_DLL_TARGET_NAME INSTALL_DIR MIL_TEXT("mil\\dll\\") SLAVE_DLL_NAME MIL_TEXT(".dll")
#elif M_MIL_USE_CE
#define SLAVE_DLL_TARGET_NAME INSTALL_DIR SLAVE_DLL_NAME MIL_TEXT(".dll")
#elif M_MIL_USE_LINUX
#define SLAVE_DLL_TARGET_NAME INSTALL_DIR MIL_TEXT("mil/lib/lib") SLAVE_DLL_NAME MIL_TEXT(".so")
#endif


/* Main to test the functions. */
/* --------------------------- */

int MosMain(void)
{
   MIL_ID       MilApplication,           /* Application Identifier.  */
                MilSystem,                /* System Identifier.       */
                MilDisplay,               /* Display Identifier.      */
                MilImage,                 /* Image buffer Identifier. */
                MilSystemOwnerApplication;/* System owner application.*/
   MIL_INT      ReturnValue;              /* Return Value holder.     */
   MIL_DOUBLE   SynchronousCallTime,      /* Timer variable.          */
                AsynchronousCallTime;     /* Timer variable.          */
   int          n;                        /* Counter.                 */
   bool CheckExistence = false;
   
   /* Allocate application, system and display. */
   MappAlloc(M_NULL, M_DEFAULT, &MilApplication);
   MsysAlloc(M_DEFAULT, SLAVE_SYSTEM_DESCRIPTOR, M_DEFAULT, M_DEFAULT, &MilSystem);

   // Now we check if the system is remote
   if(MsysInquire(MilSystem, M_LOCATION, M_NULL) != M_REMOTE)
      {
      MosPrintf(MIL_TEXT("This examples requires the default system to be a remote system.\n"));
      MosPrintf(MIL_TEXT("Please select a remote system as the default.\n"));
      MosPrintf(MIL_TEXT("If no remote systems are registered "));
      MosPrintf(MIL_TEXT("please go to the DistributedMIL->Connections page, "));
      MosPrintf(MIL_TEXT("register a remote system, "));
      MosPrintf(MIL_TEXT("and then select it as the default system.\n"));
      MsysFree(MilSystem);
      MappFree(MilApplication);
      MosGetch();
      return -1;
      }

      /* Inquire the system's owner application used to copy the slave dll with
      MappFileOperation. */
   MsysInquire(MilSystem, M_OWNER_APPLICATION, &MilSystemOwnerApplication);

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
         MsysFree(MilSystem);
         MappFree(MilApplication);
         MosPrintf(MIL_TEXT("Press a key to terminate.\n\n"));
         MosGetch();
         return -1;
         }
      }


   MdispAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &MilDisplay);


   /* Restore source image into an automatically allocated image buffer. */
   MbufRestore(IMAGE_FILE, MilSystem, &MilImage);
                       
   /* Uncomment to display the image.    */
   /* MdispSelect(MilDisplay, MilImage); */
   
   /* Pause */
   MosPrintf(MIL_TEXT("\nMIL DTK:\n"));
   MosPrintf(MIL_TEXT("--------\n\n"));
   MosPrintf(MIL_TEXT("Custom synchronous and asynchronous MIL functions:\n\n"));
   MosPrintf(MIL_TEXT("This example times a synchronous and asynchronous custom function call.\n"));
   MosPrintf(MIL_TEXT("Press a key to continue.\n\n"));
   MosGetch();

   /* Synchronous function call. */
   /* -------------------------- */

   /* Call the function a first time for more accurate timings later (dll load, ...). */
   ReturnValue = SynchronousFunction(MilImage, MilImage, M_DEFAULT);

   /* Start the timer */
   MappTimer(M_DEFAULT, M_TIMER_RESET+M_SYNCHRONOUS, M_NULL);

   /* Loop many times for more precise timing. */
   for (n= 0; n < NB_LOOP; n++)
      {
      /* Call the custom MIL synchronous function. */
      ReturnValue = SynchronousFunction(MilImage, MilImage, M_DEFAULT);
      }

   /* Read the timer. */
   MappTimer(M_DEFAULT, M_TIMER_READ+M_SYNCHRONOUS, &SynchronousCallTime);

   /* Print the synchronous call time. */
   MosPrintf(MIL_TEXT("Synchronous  function call time: %.1f us.\n"), 
      SynchronousCallTime*1000000/NB_LOOP);

   /* Asynchronous function call. */
   /* --------------------------- */

   /* Call the function a first time for more accurate timings later (dll load, ...). */
  AsynchronousFunction(MilImage, MilImage, M_DEFAULT);
  MthrWait(M_DEFAULT, M_THREAD_WAIT, M_NULL);

   /* Start the timer */
   MappTimer(M_DEFAULT, M_TIMER_RESET+M_SYNCHRONOUS, M_NULL);

   /* Loop many times for more precise timing. */
   for (n= 0; n < NB_LOOP; n++)
      {
      /* Call the custom MIL asynchronous function. */
      AsynchronousFunction(MilImage, MilImage, M_DEFAULT);
      }

   /* Read and print the time. */
   MappTimer(M_DEFAULT, M_TIMER_READ+M_SYNCHRONOUS, &AsynchronousCallTime);
      
   /* Print the asynchronous call time. */
   MosPrintf(MIL_TEXT("Asynchronous function call time: %.1f us.\n"), 
      AsynchronousCallTime*1000000/NB_LOOP);
   MosPrintf(MIL_TEXT("Press a key to terminate.\n\n"));
   MosGetch();

   /* Free all allocations. */
   MbufFree(MilImage);
   MdispFree(MilDisplay);
   MsysFree(MilSystem);
   MappFree(MilApplication);

   return 0;
}

