///*************************************************************************************
//
// File name: ColorBasedProductIdentification.cpp
//
// Synopsis:  This program shows color-based product identification on a conveyor. Only color 
//            information of the images is used. First, product samples are added to a MIL 
//            color context. This context is then used to identify the product in target 
//            images under various conditions.
//

#include <mil.h>

// Function declaration.
void AddColorSamples(MIL_ID MilSystem, 
                     MIL_ID MilDisplay, 
                     MIL_ID MilColorContext, 
                     MIL_ID MilDispColorImageChild);

void DisplayColorSamplesUtil(MIL_ID   MilSystem,
                             MIL_ID   MilDisplay,
                             MIL_ID   MilColorContext, 
                             MIL_ID   MilDispColorImageChild,
                             MIL_INT  ActiveIndex);

// Macros definition.
#define EXAMPLE_IMAGE_PATH M_IMAGE_PATH MIL_TEXT("ProductIdentification_Oasis/")

// Util definitions
#define _HIGHLIGHT_ALL_SAMPLES -1
#define _HIGHLIGHT_NONE_SAMPLE -2
#define _SAMPLE_DISPLAY_DELAY 500

// Constants definition.
static const MIL_INT  NUM_SAMPLES     =   10;            // Number of product samples
static MIL_CONST_TEXT_PTR SAMPLE_IMAGE_LIST_FILENAME[NUM_SAMPLES] =
   {
   EXAMPLE_IMAGE_PATH MIL_TEXT("OasisSample01.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("OasisSample02.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("OasisSample03.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("OasisSample04.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("OasisSample05.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("OasisSample06.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("OasisSample07.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("OasisSample08.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("OasisSample09.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("OasisSample10.mim") 
   };

static const MIL_INT  NUM_PRODUCT     =   10;            // Number of target products
static MIL_CONST_TEXT_PTR PRODUCT_IMAGE_LIST_FILENAME[NUM_PRODUCT] =
   {
   EXAMPLE_IMAGE_PATH MIL_TEXT("OasisTarget01.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("OasisTarget02.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("OasisTarget03.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("OasisTarget04.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("OasisTarget05.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("OasisTarget06.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("OasisTarget07.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("OasisTarget08.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("OasisTarget09.mim"),
   EXAMPLE_IMAGE_PATH MIL_TEXT("OasisTarget10.mim")
   };

///****************************************************************************
// Main.
///****************************************************************************
int MosMain(void)
   {
   MIL_ID MilApplication,           // MIL application identifier
          MilSystem,                // MIL system identifier
          MilDisplay,               // MIL display identifier
          MilDispColorImage,        // MIL image to display identifier
          MilDispColorImageChild,   // MIL child image to display identifier
          MilColorContext,          // MIL color context identifier
          MilColorResult,           // MIL color result identifier
          MilProductImage;          // MIL image identifier

   MIL_INT ImageSizeX, 
           ImageSizeY, 
           SampleMargin,
           Index,
           BestMatchSampleIndex;
   
   // Allocate MIL objects.
   MappAlloc(M_NULL, M_DEFAULT, &MilApplication);
   MsysAlloc(M_DEFAULT, M_SYSTEM_HOST, M_DEFAULT, M_DEFAULT, &MilSystem);
   MdispAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &MilDisplay);
   
   // Print the example synopsis.
   MosPrintf(MIL_TEXT("\nThis program shows color-based product identification on a conveyor.\n\n"));
   MosPrintf(MIL_TEXT("\nOnly color information of the images is used."));
   MosPrintf(MIL_TEXT("\nFirst, product samples are added to a MIL color context."));
   MosPrintf(MIL_TEXT("\nThis context is then used to identify the product")); 
   MosPrintf(MIL_TEXT("\nin target images under various conditions.\n"));
   MosPrintf(MIL_TEXT("\nNote: the OASIS Trademark (Registered) is reproduced with")); 
   MosPrintf(MIL_TEXT("\nthe consent of its owner, Industries Lassonde Inc.\n"));
   MosPrintf(MIL_TEXT("\nPress <Enter> to define product samples"));
   MosGetch();

   // Get target buffer size.
   MbufDiskInquire(SAMPLE_IMAGE_LIST_FILENAME[0], M_SIZE_X, &ImageSizeX);
   MbufDiskInquire(SAMPLE_IMAGE_LIST_FILENAME[0], M_SIZE_Y, &ImageSizeY);

   // Allocate a color buffer and select it to display.
   SampleMargin = 2*ImageSizeX/(NUM_SAMPLES/2);
   MbufAllocColor(MilSystem, 3, ImageSizeX+SampleMargin, ImageSizeY, 
                  8+M_UNSIGNED, M_IMAGE+M_PROC+M_DISP, &MilDispColorImage);

   // Display the window 
   MbufClear(MilDispColorImage, M_COLOR_BLACK);
   MdispSelect(MilDisplay, MilDispColorImage);

   // Allocate an RGB color matching context.
   McolAlloc(MilSystem, M_COLOR_MATCHING, M_RGB, M_DEFAULT, M_DEFAULT, &MilColorContext);
   
   // Allocate a child buffer for product display.   
   MbufChild2d(MilDispColorImage, 0, 0, ImageSizeX, ImageSizeY, &MilDispColorImageChild);

   // Define color references from product samples
   AddColorSamples(MilSystem, MilDisplay, MilColorContext, MilDispColorImageChild);

   MosPrintf(MIL_TEXT("\n\nNew product samples have been added to the color context.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n"));
   MosGetch();

   // Display preparation for matching
   MbufClear(MilDispColorImageChild, M_COLOR_BLACK);
   DisplayColorSamplesUtil(MilSystem, MilDisplay, MilColorContext, MilDispColorImageChild, 
                          _HIGHLIGHT_NONE_SAMPLE);

   // Choose histogram matching strategy.
   McolSetMethod(MilColorContext, M_HISTOGRAM_MATCHING, M_DEFAULT, M_DEFAULT, M_DEFAULT);

   // Allocate a color matching result.
   McolAllocResult(MilSystem, M_COLOR_MATCHING_RESULT, M_DEFAULT, &MilColorResult);

   // Pre-process the context.
   MosPrintf(MIL_TEXT("\n\nThe color tool is preprocessing the color information for each sample...\n"));
   MgraColor(M_DEFAULT, M_RGB888(255, 255, 255));
   MgraText(M_DEFAULT, MilDispColorImageChild, 50, 50, MIL_TEXT("Preprocessing in progress..."));
   
   McolPreprocess(MilColorContext, M_DEFAULT);
   
   // Ready to product identification 
   MbufClear(MilDispColorImageChild, M_COLOR_BLACK);
   MosPrintf(MIL_TEXT("\nPress <Enter> to identify the product in target images.\n\n"));
   MosGetch();

   // Identify the product on the conveyor:
   for(Index=0; Index<NUM_PRODUCT; Index++)
      {
      // Capture the product on conveyor. 
      MbufRestore(PRODUCT_IMAGE_LIST_FILENAME[Index], MilSystem, &MilProductImage);
      MbufCopy(MilProductImage, MilDispColorImageChild);
      
      // Perform the product identification using histogram matching in the data base.
      McolMatch(MilColorContext, MilProductImage, M_DEFAULT, M_NULL, MilColorResult, M_DEFAULT); 
      
      // Get the result of matching
      McolGetResult(MilColorResult, M_ALL, M_GENERAL, 
                    M_BEST_MATCH_INDEX + M_TYPE_MIL_INT, &BestMatchSampleIndex);

      // Display the matching result. 
      DisplayColorSamplesUtil(MilSystem, MilDisplay, MilColorContext, MilDispColorImageChild, 
                              BestMatchSampleIndex);

      MosPrintf(MIL_TEXT("The sample product #%d has been found.\n"), BestMatchSampleIndex+1);
      MosPrintf(MIL_TEXT("Press <Enter> for the next target image.\n\n"));
      MosGetch();

      // Free the buffers.
      MbufFree(MilProductImage);
      }
   
   // Ready to exit
   MosPrintf(MIL_TEXT("\nAll products on the conveyer have been identified.\n\n"));
   MosPrintf(MIL_TEXT("\nPress <Enter> to exit.\n"));
   MosGetch();

   // Free allocated buffers.
   MbufFree(MilDispColorImageChild);
   MbufFree(MilDispColorImage);
   
   // Free color objects.
   McolFree(MilColorResult);
   McolFree(MilColorContext);

   // Free MIL objects.
   MdispFree(MilDisplay);
   MsysFree(MilSystem);
   MappFree(MilApplication);
   return 0;	
   }


//***************************************************************************
// Display color samples. 
//***************************************************************************
void DisplayColorSamplesUtil(MIL_ID   MilSystem,
                             MIL_ID   MilDisplay,
                             MIL_ID   MilColorContext, 
                             MIL_ID   MilDispColorImageChild,
                             MIL_INT  ActiveIndex)
   {
   MIL_ID MilBufferId,           // MIL image identifier
          MilDispColorImage,     // MIL image identifier
          MilDispColorChildIcon; // MIL child image identifier

   const MIL_INT HALF_SAMPLES = NUM_SAMPLES/2;

   MIL_INT ImageChildSizeX, 
           ImageChildSizeY, 
           IconSizeX, 
           IconSizeY,
           NbSamples,
           i,j,k;

   bool InMainDisplay = (ActiveIndex==_HIGHLIGHT_ALL_SAMPLES);
   
   // Retrieve parent information
   MbufInquire(MilDispColorImageChild, M_PARENT_ID, &MilDispColorImage);
   MbufInquire(MilDispColorImageChild, M_SIZE_X, &ImageChildSizeX);
   MbufInquire(MilDispColorImageChild, M_SIZE_Y, &ImageChildSizeY);

   if(InMainDisplay)
      MilBufferId = MilDispColorImageChild;
   else
      MbufAllocColor(MilSystem, 3, ImageChildSizeX, ImageChildSizeY, 8, M_IMAGE+M_PROC, &MilBufferId);

   IconSizeX = ImageChildSizeX/HALF_SAMPLES;
   IconSizeY = ImageChildSizeY/HALF_SAMPLES;

   // Retrieve the number of samples in the context
   McolInquire(MilColorContext, M_CONTEXT, M_NUMBER_OF_SAMPLES+M_TYPE_MIL_INT, &NbSamples);

   // Add sample images to data base 
   for(i=0; i<NbSamples ; i++)
      {
      // Disable display update
      MdispControl(MilDisplay, M_UPDATE, M_DISABLE);

      // Draw sample
      McolDraw(M_DEFAULT, MilColorContext, MilBufferId, M_DRAW_SAMPLE, M_DEFAULT, M_SAMPLE_INDEX(i), M_DEFAULT);
      
      // Copy the samples inside the display image.
      j=0;
      k=i;
      if(i>(HALF_SAMPLES-1))      // If the product to be identified arrives the second half: 
         {
         k = k-HALF_SAMPLES;        // Re-assign number of line where the icon image will be place,
         j = 1;                     // And the number of the column of its location
         }

      // Allocate a child for this icon image 
      MbufChild2d(MilDispColorImage, ImageChildSizeX+j*IconSizeX, k*IconSizeY, 
                  IconSizeX, IconSizeY, &MilDispColorChildIcon);

      // Resize sample image to its icon size
      MimResize(MilBufferId, MilDispColorChildIcon, 
                M_FILL_DESTINATION, M_FILL_DESTINATION, M_DEFAULT);

      // Darken the icon images except selected active index
      if((i!=ActiveIndex)&&(ActiveIndex!=_HIGHLIGHT_ALL_SAMPLES))
         MimArith(MilDispColorChildIcon, 4, MilDispColorChildIcon, M_DIV_CONST);

      // Sleep
      if(ActiveIndex==_HIGHLIGHT_ALL_SAMPLES)
         {
         MdispControl(MilDisplay, M_UPDATE, M_ENABLE);
         MosSleep(_SAMPLE_DISPLAY_DELAY);
         }

      // Free the buffers.
      MbufFree(MilDispColorChildIcon);
      }

   // Update display
   MdispControl(MilDisplay, M_UPDATE, M_ENABLE);

   // Release allocated resources
   if(!InMainDisplay)
      MbufFree(MilBufferId);

   }

//***************************************************************************
// Create data base using product samples. 
//***************************************************************************
void AddColorSamples(MIL_ID MilSystem,
                     MIL_ID MilDisplay,
                     MIL_ID MilColorContext, 
                     MIL_ID MilDispColorImageChild)
   {
   MIL_INT i;
   MIL_ID MilBufferId; // MIL image identifier

   // Add sample images to data base 
   for(i=0; i<NUM_SAMPLES ; i++)
      {
      // Load the sample image.
      MbufRestore(SAMPLE_IMAGE_LIST_FILENAME[i], MilSystem, &MilBufferId);
      
      // Add the sample into the context.
      McolDefine(MilColorContext, MilBufferId, M_DEFAULT, M_IMAGE, M_DEFAULT, M_DEFAULT, M_DEFAULT, M_DEFAULT);

      // Release allocate temp buffer
      MbufFree(MilBufferId);
      }

   // Display the context
   DisplayColorSamplesUtil(MilSystem, MilDisplay, MilColorContext, MilDispColorImageChild, 
                           _HIGHLIGHT_ALL_SAMPLES);
   };
