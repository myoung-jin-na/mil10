//***************************************************************************************/
// 
// File name: BottleCapAndLabelInspection.cpp  
//
// Synopsis:  This program contains an example of bottle inspection using various modules.
//            See the PrintHeader() function below for detailed description.
//
//
#include <mil.h>

#include "ExampleManager.h"

// Include the different tasks that can be performed.
#include "CapColorIdentification.h"
#include "CapScrewInspection_CapAndLabel.h"
#include "ThreadInspection.h"
#include "LabelAlignmentInspection.h"
#include "ReadDatamatrixTask.h"
#include "ReadStringTask.h"
#include "ProductTypeIdentification.h"
#include "HorizontalAlignment.h"
#include "MeasurementAlignment.h"
#include "ModelFinderAlignmentTask.h"
#include "PositionInspection.h"
#include "SimpleFixture.h"
#include "Region/RectangleRegion.h"

//****************************************************************************
// Example description.
//****************************************************************************
void PrintHeader()   
   {
   MosPrintf(MIL_TEXT("[EXAMPLE NAME]\n")
             MIL_TEXT("BottleCapAndLabelInspection\n\n")

             MIL_TEXT("[SYNOPSIS]\n")
             MIL_TEXT("This example shows some basic inspection steps for bottle\n")
             MIL_TEXT("caps and labels. The following features will be inspected:\n")
             MIL_TEXT("    1. Neck thread metrology\n")
             MIL_TEXT("    2. Label and cap, color and alignment\n")
             MIL_TEXT("       - Verification of the cap screw\n")
             MIL_TEXT("       - Verification of the cap color\n")
             MIL_TEXT("       - Verification of label alignment\n")
             MIL_TEXT("       - Product type identification using label\n")
             MIL_TEXT("    3. Lot, expiry date and datamatrix reading\n\n")
             
             MIL_TEXT("[MODULES USED]\n")
             MIL_TEXT("Modules used: Application, system, graphic, measurement, metrology,\n")
             MIL_TEXT("              color, model finder\n\n")

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
#define EXAMPLE_IMAGE_PATH M_IMAGE_PATH MIL_TEXT("BottleCapAndLabelInspection/")

//****************************************************************************
// Product 0 : Pain pills.
//****************************************************************************

// Image path 
#define  PRODUCT0_PATH EXAMPLE_IMAGE_PATH MIL_TEXT("Product0/")

//*****************************************************************************
// Main.
//*****************************************************************************
int MosMain(void)
   {
   // Declare and the tasks associated to each image associated with the product.

   // Image 0: Neck of the bottle.
   CModAlignmentTask          Prod0View0Alignment     (PRODUCT0_PATH MIL_TEXT("Thread.mmf"), M_RED);
   CThreadInspection          Prod0Thread             (PRODUCT0_PATH MIL_TEXT("CapThreadContext.met"), M_RED, &Prod0View0Alignment);
   CInspectionTask*           Prod0Image0Tasks[] = { &Prod0View0Alignment, &Prod0Thread };

   // Image 1: Front label of the bottle with the cap.
   CHorizontalAlignment       Prod0View1HorzAlignment (PRODUCT0_PATH MIL_TEXT("HorizontalAlignmentBottle.mrk"), M_RED);
   CSimpleFixture             Prod0CapScrewFixture    (121, 30, 0, &Prod0View1HorzAlignment);
   CCapScrewInspection        Prod0CapScrew           (PRODUCT0_PATH MIL_TEXT("CapScrew.mrk"), 5, 2, M_RED, &Prod0CapScrewFixture);
   CRectangleRegion           Prod0CapColorRegion     (M_WORLD, 0, 15, 100, 30, 0, M_CENTER_AND_DIMENSION);
   static const MIL_INT ExpectedCapMatch[] =  { 0 };
   CCapColorInspection        Prod0CapColor           (PRODUCT0_PATH MIL_TEXT("CapColorContext.mcol"), ExpectedCapMatch, M_NONE, &Prod0CapScrew, M_NULL, &Prod0CapColorRegion);
   CLabelAlignmentInspection  Prod0LabelAlignment     (PRODUCT0_PATH MIL_TEXT("LabelAlignment.mrk"), 1, 165, 10, M_BLUE, &Prod0View1HorzAlignment);
   CRectangleRegion           Prod0LabelColorRegion   (M_WORLD, 120, 156, 226, 312, 0, M_CENTER_AND_DIMENSION);
   static const MIL_INT ExpectedLabelMatch[] =  { 0 };
   CProductTypeIdentification Prod0Identification     (PRODUCT0_PATH MIL_TEXT("ProductIdentification.mcol"), ExpectedLabelMatch, M_NONE, &Prod0LabelAlignment, M_NULL, &Prod0LabelColorRegion);
   CInspectionTask* Prod0Image1Tasks[] = { &Prod0View1HorzAlignment,
                                           &Prod0CapScrewFixture,
                                           &Prod0CapScrew,
                                           &Prod0CapColorRegion,
                                           &Prod0CapColor,
                                           &Prod0LabelAlignment,
                                           &Prod0LabelColorRegion,
                                           &Prod0Identification };

   // Image 2: Part of the label of the object. 
   CMeasAlignment             Prod0View2Alignment     (PRODUCT0_PATH MIL_TEXT("HorizontalAlignmentBottleRead.mrk"), M_RED);
   CRectangleRegion           Prod0ReadLotRegion      (M_WORLD, 62, 180, 220, 40, 90, M_CENTER_AND_DIMENSION);
   CReadStringTask            Prod0ReadLot            (PRODUCT0_PATH MIL_TEXT("PainPillStrContext.msr"), M_RED, &Prod0View2Alignment, M_NULL, &Prod0ReadLotRegion);
   CRectangleRegion           Prod0ReadExpiryRegion   (M_WORLD, 87, 180, 220, 40, 90, M_CENTER_AND_DIMENSION);
   CReadStringTask            Prod0ReadExpiry         (PRODUCT0_PATH MIL_TEXT("PainPillStrContext.msr"), M_RED, &Prod0View2Alignment, M_NULL, &Prod0ReadExpiryRegion);
   CRectangleRegion           Prod0DatamatrixRegion   (M_WORLD, 75, 310, 110, 130, 0, M_CENTER_AND_DIMENSION);
   CReadDatamatrixTask        Prod0Datamatrix         (PRODUCT0_PATH MIL_TEXT("Datamatrix.mco"), M_RED, &Prod0View2Alignment, M_NULL, &Prod0DatamatrixRegion);
   CInspectionTask* Prod0Image2Tasks[] = { &Prod0View2Alignment,
                                           &Prod0ReadLotRegion,
                                           &Prod0ReadLot,
                                           &Prod0ReadExpiryRegion,
                                           &Prod0ReadExpiry,
                                           &Prod0DatamatrixRegion,
                                           &Prod0Datamatrix };

   // Table of the task associated to each image.
   SImageTaskList Prod0ImagesTaskList[] = 
      {
      CREATE_LIST(Prod0Image0Tasks, MIL_TEXT("Neck thread metrology")),      
      CREATE_LIST(Prod0Image1Tasks, MIL_TEXT("Label and cap, color and alignment")),
      CREATE_LIST(Prod0Image2Tasks, MIL_TEXT("Lot and expiry date reading"))
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
         { ARRAY_COUNT(Prod0ImagesTaskList), enSequential, Prod0ImagesAvi, Prod0ImagesTaskList }
      };

   //*****************************************************************************
   // Allocate application.
   MIL_ID MilApplication = MappAlloc(M_NULL, M_DEFAULT, M_NULL);
   
   // Allocate system.
   MIL_ID MilSystem = MsysAlloc(M_DEFAULT, M_SYSTEM_HOST, M_DEFAULT, M_DEFAULT, M_NULL);
   
   MosPrintf(MIL_TEXT("BOTTLE CAP AND LABEL INSPECTION:\n"));
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