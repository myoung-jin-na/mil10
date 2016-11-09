//***************************************************************************
// 
// File name: Multiprocessing.cpp  
//
// Synopsis:  This example demonstrates the usage of Multiprocessing in MIL.
//
//

#include "common.h"

//****************************************************************************
// Example description.
//****************************************************************************
void PrintHeader()   
   {
   MosPrintf(MIL_TEXT("[EXAMPLE NAME]\n")
             MIL_TEXT("Multiprocessing\n\n")

             MIL_TEXT("[SYNOPSIS]\n")
             MIL_TEXT("This example demonstrates the usage of multi-processing in MIL.\n")
             MIL_TEXT("The menu allows you to interactively modify the number of user processing \n")
             MIL_TEXT("threads and the multi-processing controls for each one. This allows for\n")
             MIL_TEXT("the comparison of various multi-processing modes.\n")
             MIL_TEXT("\n\n")
  
             MIL_TEXT("[MODULES USED]\n")
             MIL_TEXT("Modules used: application, system, display, buffer, graphic,\n")
             MIL_TEXT("image processing, multi-processing, multi-threading.\n\n")); 
   }

//*****************************************************************************
// Main.
//*****************************************************************************
int MosMain(void)
   {
   MIL_INT NumCoresAvailable;
   CMPMenu* Menu;
   CMPProcessing** Processing; 
   MIL_TEXT_CHAR Title[STRING_SIZE];
   MIL_ID MilApplication;

   //Allocate MIL objects
   MappAlloc(M_NULL, M_DEFAULT, &MilApplication);
   MappInquireMp(M_DEFAULT, M_CORE_NUM_PROCESS, M_DEFAULT, M_NULL, &NumCoresAvailable);

   //If Multiprocessing is disabled in MILConfig, the number of available cores is 1
   if (MappInquireMp(M_DEFAULT, M_MP_FORCED_DISABLE, M_DEFAULT, M_NULL, M_NULL)==M_YES) 
      NumCoresAvailable=1;

   MosPrintf(MIL_TEXT("Multiprocessing:\n"));
   MosPrintf(MIL_TEXT("---------------------------------------\n\n"));
   
   PrintHeader();    
   MosPrintf(MIL_TEXT("Loading menu...\n\n"));

   //Allocate the processing objects.  There are as many as there are available cores.
   Processing = new CMPProcessing* [NumCoresAvailable];

   const MIL_INT NumberOfProcessingTypes = 2;
   for (MIL_INT i=0; i<NumCoresAvailable; i++)
      {
      MosSprintf(Title, STRING_SIZE, MIL_TEXT("Thread %d"), i);
      switch (i%NumberOfProcessingTypes)
         {
         case 0:
            Processing[i] = new CMPRotateProcessing(Title, i);
            break;
         case 1:
         default:
            Processing[i] = new CMPWarpProcessing(Title, i);
            break;
         }
      }

   //Allocate and run the menu
   Menu = new CMPMenu(NumCoresAvailable, Processing);
   Menu->Run();

   //Wait for user input to end the example
   MosPrintf(MIL_TEXT("Press any key to exit.\n\n"));
   MosGetch();

   MosPrintf(MIL_TEXT("Ending example...\n"));

   //Free allocated objects
   delete Menu;
   for (MIL_INT i=0; i<NumCoresAvailable; i++)
      {
      delete Processing[i];
      }
   delete [] Processing;
   MappFree(MilApplication);
   return 0;
   }