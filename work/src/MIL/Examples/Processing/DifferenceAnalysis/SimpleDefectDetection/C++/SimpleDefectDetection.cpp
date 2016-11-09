//***************************************************************************************/
// 
// File name: SimpleDefectDetection.cpp  
//
// Synopsis:  This program contains an example of defect detection using Model Finder,
//            Color and some image processing primitives.
//            See the PrintHeader() function below for detailed description.
//
//
#include <mil.h>
#include "defectdetectionprocfunc.h"
#include "defectdetectionexample.h"

//****************************************************************************
// Example description.
//****************************************************************************
void PrintHeader()   
   {
   MosPrintf(MIL_TEXT("[EXAMPLE NAME]\n"));
   MosPrintf(MIL_TEXT("SimpleDefectDetection\n\n"));

   MosPrintf(MIL_TEXT("[SYNOPSIS]\n"));
   MosPrintf(MIL_TEXT("This example demontrates how to perform basic defect detection \n"));
   MosPrintf(MIL_TEXT("using various methods to extract the differences and to binarize \n"));
   MosPrintf(MIL_TEXT("them. The application uses MIL's fixturing capabilities in \n"));
   MosPrintf(MIL_TEXT("conjunction with Model Finder to align target occurences with the \n"));
   MosPrintf(MIL_TEXT("template image.\n\n"));

   MosPrintf(MIL_TEXT("[MODULES USED]\n"));
   MosPrintf(MIL_TEXT("Modules used: Application, system, display, buffer, graphic, \n"));
   MosPrintf(MIL_TEXT("image processing, model finder, blob, color.\n\n"));

   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();
   }

//****************************************************************************
// Constants.
//****************************************************************************

// Filenames.
#define EXAMPLE_IMAGE_PATH    M_IMAGE_PATH MIL_TEXT("SimpleDefectDetection/")

static const MIL_INT NB_IMAGES = 5;

static MIL_CONST_TEXT_PTR TEMPLATE_IMAGE_FILENAME[NB_IMAGES] =
   {
   EXAMPLE_IMAGE_PATH MIL_TEXT("Template.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("Template.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("Template.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("TemplateBin.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("Template.mim"),   
   };

static MIL_CONST_TEXT_PTR TARGET_IMAGE_FILENAMES[NB_IMAGES] =
   {
   EXAMPLE_IMAGE_PATH MIL_TEXT("Target.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("TargetColorDefect.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("TargetTriangle.mim"), 
   EXAMPLE_IMAGE_PATH MIL_TEXT("TargetTopHat.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("TargetNoise.mim"),   
   };

static MIL_CONST_TEXT_PTR CASE_INFORMATION[NB_IMAGES] = 
   {
   MIL_TEXT("The absolute value of difference of the grayscale image combined \n")
   MIL_TEXT("with the bimodal threshold is the general method used. It works \n")
   MIL_TEXT("when the defects are of any size but much stronger than the \n")
   MIL_TEXT("normal variations of the template. The defects intensity must \n")
   MIL_TEXT("create a single mode, while the background creates another."),

   MIL_TEXT("The color distance can be used instead of the absolute value of \n")
   MIL_TEXT("difference to extract differences that cannot be seen in the \n")
   MIL_TEXT("luminance image."),

   MIL_TEXT("The triangle bisection helps to extract all defects when the \n")
   MIL_TEXT("intensity of the defects forms multiple modes. The triangle \n")
   MIL_TEXT("binarization separates the background from the other differences."),
   
   MIL_TEXT("The morphological background substraction method, that uses a \n")
   MIL_TEXT("bottom hat or top hat, should be used when there is non-uniform \n")
   MIL_TEXT("lighting. The object should not contain structures thinner than \n")
   MIL_TEXT("the structuring element and the defect size cannot be greater \n")
   MIL_TEXT("than structuring element."),

   MIL_TEXT("The triangle bisection can also be used when there are normal\n")
   MIL_TEXT("small variations in the template, moving the background threshold\n")
   MIL_TEXT("to a higher value."),
   };

// Default methods.
static const DifferenceExtractionMethod DEFAULT_DIFFERENCE_METHOD[NB_IMAGES] = 
   {
   enAbsoluteDiff,
   enColDistance,
   enColDistance,
   enTopHat,
   enColDistance,   
   };

static const BinarizationMethod DEFAULT_BINARIZATION_METHOD[NB_IMAGES] = 
   {
   enBiModal,
   enBiModal,
   enTriangleBisection,
   enTriangleBisection,
   enTriangleBisection, 
   };


//*****************************************************************************
// Main.
//*****************************************************************************
int MosMain(void)
   {
   CDefectDetectionExampleMngr ExampleMngr(M_SYSTEM_HOST);
   
   // Print the header.
   PrintHeader();

   // Ask the user if he wants to run the example interactive mode
   if(ExampleMngr.AskForInteractive())
      {
      // Load the template image in the example.
      if(ExampleMngr.LoadTemplate(M_INTERACTIVE))
         {
         // Initialize the example with the current target image.
         if(ExampleMngr.InitializeExample(M_INTERACTIVE, enAbsoluteDiff, enBiModal))
            {
            // Run the example.
            ExampleMngr.Run(MIL_TEXT("")); 
            }
         }        
      }
   else
      {
      for(MIL_INT TargetIdx = 0; TargetIdx < NB_IMAGES; TargetIdx++)
         {
         // Load the template image in the example.
         if(ExampleMngr.LoadTemplate(TEMPLATE_IMAGE_FILENAME[TargetIdx]))
            {
            // Initialize the example with the current target image.
            if(ExampleMngr.InitializeExample(TARGET_IMAGE_FILENAMES[TargetIdx],
                                             DEFAULT_DIFFERENCE_METHOD[TargetIdx],
                                             DEFAULT_BINARIZATION_METHOD[TargetIdx]))
               {
               // Run the example.
               ExampleMngr.Run(CASE_INFORMATION[TargetIdx]);
               }            
            }         
         }
      }
   }