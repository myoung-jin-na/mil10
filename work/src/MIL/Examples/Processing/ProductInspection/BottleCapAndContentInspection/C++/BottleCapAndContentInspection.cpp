//***************************************************************************************/
// 
// File name: BottleCapAndContentInspection.cpp  
//
// Synopsis:  This program contains an example of bottle inspection using various modules.
//            See the PrintHeader() function below for detailed description.
//
//
#include <mil.h>

#include "ExampleManager.h"

// Include the different tasks that can be performed.
#include "CapScrewInspection_CapAndContent.h"
#include "ModelFinderAlignmentTask.h"
#include "TargetAlignmentTask.h"
#include "PositionInspection.h"
#include "DefectDetectionTask.h"
#include "SimpleFixture.h"

//****************************************************************************
// Example description.
//****************************************************************************
void PrintHeader()   
   {
   MosPrintf(MIL_TEXT("[EXAMPLE NAME]\n")
             MIL_TEXT("BottleCapAndContentInspection\n\n")

             MIL_TEXT("[SYNOPSIS]\n")
             MIL_TEXT("This example shows some basic inspection steps for bottle caps\n")
             MIL_TEXT("and content. The following features will be inspected:\n")
             MIL_TEXT("    1. Surface defect detection\n")
             MIL_TEXT("    2. Fill level measurement \n")
             MIL_TEXT("    3. Cap screw verification\n\n")
             
             MIL_TEXT("[MODULES USED]\n")
             MIL_TEXT("Modules used: Application, system, graphic, calibration, measurement, \n")
             MIL_TEXT("              blob, model finder\n\n")

             MIL_TEXT("[ADDITIONNAL INSTRUCTIONS]\n")
             MIL_TEXT("Left click on an image to view results in a zoomable display.\n\n")
               
             MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();
   }

//****************************************************************************
// General Constants.
//****************************************************************************
#define ARRAY_COUNT(x) (sizeof((x))/sizeof((x)[0])) // for array on the stack only

// Root filename.
#define EXAMPLE_IMAGE_PATH M_IMAGE_PATH MIL_TEXT("BottleCapAndContentInspection/")

//****************************************************************************
// Product 0 : Mouth Wash
//****************************************************************************

// Image path 
#define  PRODUCT0_PATH EXAMPLE_IMAGE_PATH MIL_TEXT("Product0/")

//*****************************************************************************
// Main.
//*****************************************************************************
int MosMain(void)
   {
   // Declare and the tasks associated to each image associated with the product.
   // Image 0: Empty bottle analysis.
   CTargetAlignmentTask Prod0TargetAlignment(PRODUCT0_PATH MIL_TEXT("BottleContour.mmf"), PRODUCT0_PATH MIL_TEXT("EmptyBottleTemplate.mim"), M_RED);
   CDefectDetectionTask Prod0DefectDetection(PRODUCT0_PATH MIL_TEXT("EmptyBottleTemplate.mim"),
                                             PRODUCT0_PATH MIL_TEXT("EmptyBottleTemplateMask.mim"),
                                             enAbsoluteDiff,
                                             enTriangleBisection,
                                             M_RED,
                                             &Prod0TargetAlignment,
                                             &Prod0TargetAlignment);   

   // Image 1: Fill level analysis.
   CModAlignmentTask    Prod0View1Alignment(PRODUCT0_PATH MIL_TEXT("BottleContour.mmf"), M_RED);
   CSimpleFixture       Prod0FillLevelFixture(0, 0, -90, &Prod0View1Alignment);
   CPositionInspection  Prod0FillLevel(PRODUCT0_PATH MIL_TEXT("FillLevel.mrk"), MIL_TEXT("fill level"), -170, 5, M_RED, &Prod0FillLevelFixture);

   // Image 2: Cap analysis.
   CModAlignmentTask    Prod0View2Alignment(PRODUCT0_PATH MIL_TEXT("BottleContour.mmf"), M_RED);
   CSimpleFixture       Prod0CapFixture(0, -330, 0, &Prod0View2Alignment);      
   CCapScrewInspection  Prod0CapScrew(PRODUCT0_PATH MIL_TEXT("CapScrew.mrk"), 5, 2, M_RED, &Prod0CapFixture);

   CInspectionTask* Prod0Image0Tasks[] = { &Prod0TargetAlignment, &Prod0DefectDetection                   };
   CInspectionTask* Prod0Image1Tasks[] = { &Prod0View1Alignment , &Prod0FillLevelFixture, &Prod0FillLevel };
   CInspectionTask* Prod0Image2Tasks[] = { &Prod0View2Alignment , &Prod0CapFixture      , &Prod0CapScrew  };

   // Table of the task associated to each image.
   SImageTaskList Prod0ImagesTaskList[] = 
      {
      CREATE_LIST(Prod0Image0Tasks, MIL_TEXT("Surface defect detection")),
      CREATE_LIST(Prod0Image1Tasks, MIL_TEXT("Fill level measurement")),
      CREATE_LIST(Prod0Image2Tasks, MIL_TEXT("Cap screw verification")),
      };

   // The filenames of the avi files.
   static MIL_CONST_TEXT_PTR Prod0ImagesAvi[ARRAY_COUNT(Prod0ImagesTaskList)] =
      {
      PRODUCT0_PATH MIL_TEXT("View0.avi"),
      PRODUCT0_PATH MIL_TEXT("View1.avi"),
      PRODUCT0_PATH MIL_TEXT("View2.avi"),
      };

   //*****************************************************************************

   // The product info structure that indicates the task and avi to load.
   SProductInfo ProductsInfoList[] =
      {
         { ARRAY_COUNT(Prod0ImagesTaskList), enSequential, Prod0ImagesAvi, Prod0ImagesTaskList },
      };

   //*****************************************************************************
   // Allocate application.
   MIL_ID MilApplication = MappAlloc(M_NULL, M_DEFAULT, M_NULL);
   
   // Allocate system.
   MIL_ID MilSystem = MsysAlloc(M_DEFAULT, M_SYSTEM_HOST, M_DEFAULT, M_DEFAULT, M_NULL);
   
   MosPrintf(MIL_TEXT("BOTTLE CAP AND CONTENT INSPECTION:\n"));
   MosPrintf(MIL_TEXT("---------------------------------------\n\n"));

   // Print the header.
   PrintHeader(); 

   // Allocate the example manager.
   CExampleMngr *pExampleManager = new CExampleMngr(MilSystem);   

   // Run the example.
   pExampleManager->Run(ProductsInfoList, ARRAY_COUNT(ProductsInfoList));

   // Free the example manager.
   delete pExampleManager;

   // Free the system.
   MsysFree(MilSystem);

   // Free the application.
   MappFree(MilApplication);

   return 0;
   }