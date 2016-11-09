//***************************************************************************************/
// 
// File name: AmpouleAndPackageInspection.cpp  
//
// Synopsis:  This program contains an example of bottle inspection using various modules.
//            See the PrintHeader() function below for detailed description.
//
//
#include <mil.h>

#include "ExampleManager.h"

// Include the different tasks that can be performed
#include "SimpleFixture.h"
#include "ProductCountInspection.h"
#include "HorizontalAndAngleAlignment.h"
#include "VerticalAlignment.h"
#include "PositionInspection.h"
#include "CapOrientation.h"

//****************************************************************************
// Example description.
//****************************************************************************
void PrintHeader()   
   {
   MosPrintf(MIL_TEXT("[EXAMPLE NAME]\n")
             MIL_TEXT("AmpouleAndPackageInspection\n\n")

             MIL_TEXT("[SYNOPSIS]\n")
             MIL_TEXT("This example shows some basic inspection of ampoules\n")
             MIL_TEXT("and their packaging. The following features will be inspected:\n")
             MIL_TEXT("  1-2. Fill level and cap\n")
             MIL_TEXT("       - Verification of the fill level\n")
             MIL_TEXT("       - Verification of the cap position\n")
             MIL_TEXT("       - Verification of the cap orientation\n")
             MIL_TEXT("    3. Packaging ampoule count\n\n")             
             
             MIL_TEXT("[MODULES USED]\n")
             MIL_TEXT("Modules used: Application, system, graphic, measurement,\n")
             MIL_TEXT("              pattern matching\n\n")

             MIL_TEXT("[ADDITIONNAL INSTRUCTIONS]\n")
             MIL_TEXT("Left click on an image to view results in a zoomable display.\n\n")
  
             MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();
   }

//****************************************************************************
// General Constants
//****************************************************************************
#define ARRAY_COUNT(x) (sizeof((x))/sizeof((x)[0])) // for array on the stack only

// Root filename.
#define EXAMPLE_IMAGE_PATH M_IMAGE_PATH MIL_TEXT("AmpouleAndPackageInspection/")

//****************************************************************************
// Product 0 : Ampoules
//****************************************************************************

// Image path 
#define  PRODUCT0_PATH EXAMPLE_IMAGE_PATH MIL_TEXT("Product0/")

//*****************************************************************************
// Main.
//*****************************************************************************
int MosMain(void)
   {
   // Declare the tasks to be done on the product.
   // View 0: Front label of the bottle with the cap.
   CHorizontalAndAngleAlignment  Prod0HorzAlignment           (PRODUCT0_PATH MIL_TEXT("HorizontalAlignmentAmpoule.mrk"), M_RED);
   CVerticalAlignment            Prod0VertAlignment           (PRODUCT0_PATH MIL_TEXT("VerticalAlignmentAmpoule.mrk"), M_RED, &Prod0HorzAlignment);

   CSimpleFixture                Prod0FillLevelFixture        (0, 68, 90, &Prod0VertAlignment);
   CPositionInspection           Prod0FillLevel               (PRODUCT0_PATH MIL_TEXT("FillLevel.mrk"), MIL_TEXT("fill level"), 0, 10, M_RED, &Prod0FillLevelFixture);

   CCapOrientation               Prod0CapOrientation          (PRODUCT0_PATH MIL_TEXT("CapOrientation.mrk"), 83, 5, M_RED, &Prod0VertAlignment);

   CSimpleFixture                Prod0CapPositionFixture      (0, -170, 90, &Prod0VertAlignment);
   CPositionInspection           Prod0CapPosition             (PRODUCT0_PATH MIL_TEXT("CapPosition.mrk"), MIL_TEXT("cap"), 0, 5, M_RED, &Prod0CapPositionFixture);

   // View1 : Front label of the bottle with the cap.
   CHorizontalAndAngleAlignment  Prod0HorzAlignment2          (PRODUCT0_PATH MIL_TEXT("HorizontalAlignmentAmpoule.mrk"), M_RED);
   CVerticalAlignment            Prod0VertAlignment2          (PRODUCT0_PATH MIL_TEXT("VerticalAlignmentAmpoule.mrk"), M_RED, &Prod0HorzAlignment2);

   CSimpleFixture                Prod0FillLevelFixture2       (0, 68, 90, &Prod0VertAlignment2);
   CPositionInspection           Prod0FillLevel2              (PRODUCT0_PATH MIL_TEXT("FillLevel.mrk"), MIL_TEXT("fill level"), 0, 10, M_RED, &Prod0FillLevelFixture2);

   CCapOrientation               Prod0CapOrientation2         (PRODUCT0_PATH MIL_TEXT("CapOrientation.mrk"), 83, 5, M_RED, &Prod0VertAlignment2);

   CSimpleFixture                Prod0CapPositionFixture2     (0, -170, 90, &Prod0VertAlignment2);
   CPositionInspection           Prod0CapPosition2            (PRODUCT0_PATH MIL_TEXT("CapPosition.mrk"), MIL_TEXT("cap"), 0, 5, M_RED, &Prod0CapPositionFixture2);

   // View 2: The box containing the ampoules.
   CProductCountInspection       Prod0Count                   (PRODUCT0_PATH MIL_TEXT("Pattern.mod"), 30, M_RED);

   CInspectionTask* Prod0Image0Tasks[] = { &Prod0HorzAlignment , &Prod0VertAlignment , &Prod0FillLevelFixture , &Prod0FillLevel , &Prod0CapOrientation , &Prod0CapPositionFixture , &Prod0CapPosition  };
   CInspectionTask* Prod0Image1Tasks[] = { &Prod0HorzAlignment2, &Prod0VertAlignment2, &Prod0FillLevelFixture2, &Prod0FillLevel2, &Prod0CapOrientation2, &Prod0CapPositionFixture2, &Prod0CapPosition2 }; 
   CInspectionTask* Prod0Image2Tasks[] = { &Prod0Count };

   // Table of the task associated to each image
   SImageTaskList Prod0ImagesTaskList[] = 
      {
      CREATE_LIST(Prod0Image0Tasks, MIL_TEXT("Fill level and cap")),
      CREATE_LIST(Prod0Image1Tasks, MIL_TEXT("Fill level and cap")),
      CREATE_LIST(Prod0Image2Tasks, MIL_TEXT("Packaging ampoule count"))
      };

   // The filenames of the avi files.
   static MIL_CONST_TEXT_PTR Prod0ImagesAvi[ARRAY_COUNT(Prod0ImagesTaskList)] =
      {
      PRODUCT0_PATH MIL_TEXT("View0.avi"),
      PRODUCT0_PATH MIL_TEXT("View1.avi"),
      PRODUCT0_PATH MIL_TEXT("View2.avi")
      };

   //*****************************************************************************

   SProductInfo ProductsInfoList[] =
      {
         { ARRAY_COUNT(Prod0ImagesTaskList), enParallel, Prod0ImagesAvi, Prod0ImagesTaskList },
      };

   //*****************************************************************************
   // Allocate application.
   MIL_ID MilApplication = MappAlloc(M_NULL, M_DEFAULT, M_NULL);
   
   // Allocate system.
   MIL_ID MilSystem = MsysAlloc(M_DEFAULT, M_SYSTEM_HOST, M_DEFAULT, M_DEFAULT, M_NULL);
   
   MosPrintf(MIL_TEXT("AMPOULE AND PACKAGE INSPECTION:\n"));
   MosPrintf(MIL_TEXT("---------------------------------------\n\n"));

   // Print the header.
   PrintHeader(); 

   // Allocate the example manager.
   CExampleMngr *pExampleManager = new CExampleMngr(MilSystem);   

   // Run the example.
   pExampleManager->Run(ProductsInfoList, ARRAY_COUNT(ProductsInfoList));

   // Free the example manager.
   delete pExampleManager;

   // Free the system
   MsysFree(MilSystem);

   // Free the application
   MappFree(MilApplication);

   return 0;
   }