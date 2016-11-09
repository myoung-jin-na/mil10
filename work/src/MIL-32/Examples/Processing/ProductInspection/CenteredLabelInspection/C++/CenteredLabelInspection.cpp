//***************************************************************************************/
// 
// File name: CenteredLabelInspection.cpp  
//
// Synopsis:  This program contains an example of lid label inspection.
//            See the PrintHeader() function below for detailed description.
//
//
#include <mil.h>

#include "ExampleManager.h"

// Include the different tasks that can be performed.
#include "MetrologyAlignment.h"
#include "LidColorsVerification.h"
#include "LidIdentification.h"
#include "SimpleFixture.h"
#include "Region/ArcRegion.h"
#include "Region/PolygonRegion.h"

//****************************************************************************
// Example description.
//****************************************************************************
void PrintHeader()   
   {
   MosPrintf(MIL_TEXT("[EXAMPLE NAME]\n")
             MIL_TEXT("CenteredLabelInspection\n\n")

             MIL_TEXT("[SYNOPSIS]\n")
             MIL_TEXT("This example shows the inspection of lid labels on three\n")
             MIL_TEXT("product lanes. The following features will be inspected:\n")
             MIL_TEXT("    - Concentricity of the label and the lid\n")
             MIL_TEXT("    - Correct label type for a given lane\n")
             MIL_TEXT("    - Color of some label regions\n\n")
                          
             MIL_TEXT("[MODULES USED]\n")
             MIL_TEXT("Modules used: Application, system, graphic, metrology\n")
             MIL_TEXT("              color, pattern matching\n\n")

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
#define EXAMPLE_IMAGE_PATH M_IMAGE_PATH MIL_TEXT("CenteredLabelInspection/")

//****************************************************************************
// Product 0 : Lid.
//****************************************************************************
// Image path 
#define  PRODUCT0_PATH EXAMPLE_IMAGE_PATH MIL_TEXT("Product0/")

//*****************************************************************************
// Main.
//*****************************************************************************
int MosMain(void)
   {
   // Declare and the tasks associated to each image associated with the product.
   // Image 0: Banana lid of the product
   CMetrologyAlignment     Prod0BananaMetAndAlignment   (PRODUCT0_PATH MIL_TEXT("MetrologyAlignment.met"), M_RGB_TO_L);
   CLidIdentification      Prod0BananaPattern           (PRODUCT0_PATH MIL_TEXT("BananaPattern.mod"), M_RGB_TO_L, &Prod0BananaMetAndAlignment);
   CArcRegion              Prod0BananaBackgroundRegion  (M_WORLD,  0, 30, 179, 127, 0, 180, 0);

   static const MIL_DOUBLE BANANA_POSX[] = { -70, -76, -99, -106, -82,  -8,  51,  76,  67,  76,  69,  61,  57,  47,  22, -15, -41}; 
   static const MIL_DOUBLE BANANA_POSY[] = { 165, 183, 177,  189, 223, 251, 226, 192, 159, 135, 134, 141, 170, 167, 183, 188, 175}; 
   CPolygonRegion          Prod0BananaRegion            (M_WORLD, ARRAY_COUNT(BANANA_POSX), BANANA_POSX, BANANA_POSY, &Prod0BananaBackgroundRegion);

   static const MIL_INT BananaExpectedMatches[] = { 1, 0 };
   CLidColorsVerification  Prod0BananaColorMatch         (PRODUCT0_PATH MIL_TEXT("BananaLid.mcol"), BananaExpectedMatches, M_NONE, &Prod0BananaPattern, M_NULL, &Prod0BananaRegion);
   CInspectionTask* Prod0Image0Tasks[] = { &Prod0BananaMetAndAlignment,  
                                           &Prod0BananaPattern,
                                           &Prod0BananaBackgroundRegion, 
                                           &Prod0BananaRegion,
                                           &Prod0BananaColorMatch };

   // Image 1: Strawberry lid of the product
   CMetrologyAlignment     Prod0StrawberryMetAndAlignment   (PRODUCT0_PATH MIL_TEXT("MetrologyAlignment.met"), M_RGB_TO_L);
   CLidIdentification      Prod0StrawberryPattern           (PRODUCT0_PATH MIL_TEXT("StrawberryPattern.mod"), M_RGB_TO_L, &Prod0StrawberryMetAndAlignment);
   CArcRegion              Prod0StrawberryBackgroundRegion  (M_WORLD,  0, 30, 179, 127, 0, 180, 0);

   static const MIL_DOUBLE STRAWBERRY_POSX[] = { -39, -55, -53, -17, -11,  37,  55,  42,  40,  19,  13,   3,  -9, -21}; 
   static const MIL_DOUBLE STRAWBERRY_POSY[] = { 152, 168, 192, 247, 249, 215, 187, 150, 156, 165, 161, 175, 158, 159}; 
   CPolygonRegion          Prod0StrawberryRegion  (M_WORLD, ARRAY_COUNT(STRAWBERRY_POSX), STRAWBERRY_POSX, STRAWBERRY_POSY, &Prod0StrawberryBackgroundRegion);

   static const MIL_INT StrawberryExpectedMatches[] = { 1, 0 };
   CLidColorsVerification  Prod0StrawberryColorMatch        (PRODUCT0_PATH MIL_TEXT("StrawberryLid.mcol"), StrawberryExpectedMatches, M_NONE, &Prod0StrawberryPattern, M_NULL, &Prod0StrawberryRegion);
   CInspectionTask* Prod0Image1Tasks[] = { &Prod0StrawberryMetAndAlignment,
                                           &Prod0StrawberryPattern,
                                           &Prod0StrawberryBackgroundRegion,
                                           &Prod0StrawberryRegion,
                                           &Prod0StrawberryColorMatch };

   // Image 2: Blueberry lid of the product
   CMetrologyAlignment     Prod0BlueberryMetAndAlignment (PRODUCT0_PATH MIL_TEXT("MetrologyAlignment.met"), M_RGB_TO_L);
   CLidIdentification      Prod0BlueberryPattern         (PRODUCT0_PATH MIL_TEXT("BlueberryPattern.mod"), M_RGB_TO_L, &Prod0BlueberryMetAndAlignment);
   CArcRegion              Prod0BlueberryBackgroundRegion(M_WORLD,  0, 30, 179, 127, 0, 180, 0);
   CArcRegion              Prod0BlueberryRegion          (M_WORLD,  6, 175,  55,  55, 0, 360, 0, &Prod0BlueberryBackgroundRegion);

   static const MIL_INT BlueberryExpectedMatches[] = { 1, 0 };
   CLidColorsVerification  Prod0BlueberryColorMatch (PRODUCT0_PATH MIL_TEXT("BlueberryLid.mcol"), BlueberryExpectedMatches, M_NONE, &Prod0BlueberryPattern, M_NULL, &Prod0BlueberryRegion);
   CInspectionTask* Prod0Image2Tasks[] = { &Prod0BlueberryMetAndAlignment,  
                                           &Prod0BlueberryPattern, 
                                           &Prod0BlueberryBackgroundRegion, 
                                           &Prod0BlueberryRegion,  
                                           &Prod0BlueberryColorMatch };

   // Table of the task associated to each image.
   SImageTaskList Prod0ImagesTaskList[] = 
      {
      CREATE_LIST(Prod0Image0Tasks, MIL_TEXT("BananaLid inspection")),
      CREATE_LIST(Prod0Image1Tasks, MIL_TEXT("StrawberryLid inspection")),
      CREATE_LIST(Prod0Image2Tasks, MIL_TEXT("BlueberryLid inspection"))
      };

   // The filenames of the avi files.
   static MIL_CONST_TEXT_PTR Prod0ImagesAvi[ARRAY_COUNT(Prod0ImagesTaskList)] =
      {
      PRODUCT0_PATH MIL_TEXT("View0.avi"),
      PRODUCT0_PATH MIL_TEXT("View1.avi"),
      PRODUCT0_PATH MIL_TEXT("View2.avi")
      };

   //*****************************************************************************

   // The product info structure that indicates the task and avi to load.
   SProductInfo ProductsInfoList[] =
      {
         { ARRAY_COUNT(Prod0ImagesTaskList), enParallel, Prod0ImagesAvi, Prod0ImagesTaskList }
      };

   //*****************************************************************************
   // Allocate application.
   MIL_ID MilApplication = MappAlloc(M_NULL, M_DEFAULT, M_NULL);
   
   // Allocate system.
   MIL_ID MilSystem = MsysAlloc(M_DEFAULT, M_SYSTEM_HOST, M_DEFAULT, M_DEFAULT, M_NULL);
   
   MosPrintf(MIL_TEXT("CENTERED LABEL INSPECTION:\n"));
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