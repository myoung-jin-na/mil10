/*****************************************************************************************/
/*
 * File name: DistributedMilAddConstantMain.cpp
 *
 * Synopsis:  This file contains the main program to test and time the different 
 *            versions of a custom add constant MIL function MIL created with 
 *            the MIL Function Development module.
 *
 *            The slave functions can be found in the DistributedMILAddConstantSlave project.
 */

/* Standard headers. */
#include <mil.h>

/* Target image file name */
#define IMAGE_FILE   M_IMAGE_PATH MIL_TEXT("Board.mim")

/* Timing loop iterations. */
#define NB_LOOP 100

/* Defines for the different versions of the target function to run on the remote computer. */
#define USE_C           0 /* Target C function C.                 */
#define USE_MIL         1 /* Target function MIL.                 */
#define NB_VERSIONS     2 /* Number of different versions to call. */
static const MIL_TEXT_CHAR *VersionName[] = {MIL_TEXT("C"), MIL_TEXT("MIL")}; 

#if M_MIL_USE_CE
#define SLAVE_SYSTEM_DESCRIPTOR   MIL_TEXT("dmiltcp://localhost/M_SYSTEM_DEFAULT")
#else
#define SLAVE_SYSTEM_DESCRIPTOR   M_SYSTEM_DEFAULT
#endif


/* Slave dll path and name */
#define SLAVE_DLL_NAME MIL_TEXT("dmiladdconstantslave")

#if M_MIL_USE_WINDOWS && !M_MIL_USE_CE
#define SLAVE_DLL_TARGET_NAME INSTALL_DIR MIL_TEXT("mil\\dll\\") SLAVE_DLL_NAME MIL_TEXT(".dll")
#elif M_MIL_USE_CE
#define SLAVE_DLL_TARGET_NAME INSTALL_DIR SLAVE_DLL_NAME MIL_TEXT(".dll")
#elif M_MIL_USE_LINUX
#define SLAVE_DLL_TARGET_NAME INSTALL_DIR MIL_TEXT("mil/lib/lib") SLAVE_DLL_NAME MIL_TEXT(".so")
#endif

//The display format
#if M_MIL_USE_CE
#define DISPLAY_FORMAT MIL_TEXT("M_REMOTEVIEW_RGB")
#else
#define DISPLAY_FORMAT MIL_TEXT("M_DEFAULT")
#endif

/* Master functions prototypes: */
/* Custom C function (See DTKAddConstantC.c) */
void MFTYPE AddConstantC(MIL_ID SrcImage, MIL_ID DstImage, MIL_UINT Constant);


/* Main to test the AddConstant functions. */
/* --------------------------------------- */

int MosMain(void)
   {
   MIL_ID MilApplication,           /* Application Identifier.  */
          MilSystem,                /* System Identifier.       */
          MilDisplay,               /* Display Identifier.      */
          MilImageDisp,             /* Image buffer Identifier. */ 
          MilImageSrc,              /* Image buffer Identifier. */ 
          MilImageDst,              /* Image buffer Identifier. */ 
          MilSystemOwnerApplication;/* System owner application.*/

   int    Version, n;            /* Loop variables.          */
   MIL_DOUBLE Time;                  /* Processing time.         */
   bool  CheckExistence = false;

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

   MdispAlloc(MilSystem, M_DEFAULT, DISPLAY_FORMAT, M_DEFAULT, &MilDisplay);

   /* Restore the source image into one display buffer and 2 processing buffers. */
   MbufRestore(IMAGE_FILE, MilSystem, &MilImageDisp);
   MbufRestore(IMAGE_FILE, MilSystem, &MilImageSrc);
   MbufRestore(IMAGE_FILE, MilSystem, &MilImageDst);

   /* Display the source image. */
   MdispSelect(MilDisplay, MilImageDisp);
   
   /* Pause */
   MosPrintf(MIL_TEXT("\nMIL DTK:\n"));
   MosPrintf(MIL_TEXT("--------\n\n"));
   MosPrintf(MIL_TEXT("This example tests and times different versions of a custom\n"));
   MosPrintf(MIL_TEXT("asynchronous MIL function that adds a constant to an image.\n"));
   MosPrintf(MIL_TEXT("Press a key to continue.\n\n"));
   MosGetch();

   /* Process the image using the C version of the custom MIL function. */
   AddConstantC(MilImageSrc, MilImageDisp, 0x40);
   
   /* Print comment */
   MosPrintf(MIL_TEXT("A constant was added to the image using a user-made MIL function.\n\n"));

   /* Call and time all the versions of the add constant function.
      Do it in loop for more precision.
   */
   for (Version = 0; Version < NB_VERSIONS; Version++)
      {
      for (n= 0; n < NB_LOOP+1; n++)
         {
         /* Don't time the first iteration (avoid DLL load time, ...). */
         if (n == 1)
            MappTimer(M_DEFAULT, M_TIMER_RESET+M_SYNCHRONOUS, M_NULL);

         /* Call the proper version */
         switch(Version)
            {
            case USE_C:
                 AddConstantC(MilImageSrc, MilImageDst, 0x40);
                 break;

            case USE_MIL:
                 MimArith(MilImageSrc, 0x40, MilImageDst, M_ADD_CONST);  
                 break;
            }
         }

      /* Read and print the time.*/
      MappTimer(M_DEFAULT, M_TIMER_READ+M_SYNCHRONOUS, &Time);
      MosPrintf(MIL_TEXT("Add constant time (%s version): %.3f ms.\n"), VersionName[Version], 
         Time*1000/NB_LOOP);
      }

   /* Pause */
   MosPrintf(MIL_TEXT("Press a key to terminate.\n\n"));
   MosGetch();

   /* Free all allocations. */
   MbufFree(MilImageSrc);
   MbufFree(MilImageDst);
   MbufFree(MilImageDisp);
   MdispFree(MilDisplay);
   MsysFree(MilSystem);
   MappFree(MilApplication);

   return 0;
   }



