//***************************************************************************************
// 
// File name: defectdetectionexample.cpp
//
// Synopsis: This file contains the implementation of the CDefectDetectionExamplMngr class
//           that handles all the aspects of the defect detection example except the 
//           processing part. 
//

#include <mil.h>
#include "defectdetectionprocfunc.h"
#include "defectdetectionexample.h"

//*****************************************************************************
// Constants.
//*****************************************************************************
static const MIL_INT WINDOW_TITLE_HEIGHT = 60;

static const MIL_INT IMAGE_SEPARATION_X = 30;

static const MIL_INT TARGET_IMAGE_SEPARATION_Y = 30;

static const MIL_DOUBLE MODEL_OFFSET_X = M_DEFAULT;
static const MIL_DOUBLE MODEL_OFFSET_Y = M_DEFAULT;
static const MIL_DOUBLE MODEL_SIZE_X   = M_DEFAULT;
static const MIL_DOUBLE MODEL_SIZE_Y   = M_DEFAULT;

static const MIL_INT HIST_BORDER = WINDOW_TITLE_HEIGHT;

static const MIL_INT DEFECT_SIZE = 5;

static const MIL_INT CLEAN_MORPH_SIZE = 1;

static const MIL_DOUBLE GRADIENT_MASK_SMOOTHNESS = 50;

static const MIL_DOUBLE TRIANGLE_LOWER_CUTOFF = 2;

static const MIL_DOUBLE TRIANGLE_UPPER_CUTOFF = 255;

static const MIL_DOUBLE BIN_CUMULATIVE_VALUE = 95.0;

static const MIL_DOUBLE FIXED_DIFF_THRESHOLD = 10;

static const MIL_DOUBLE NORMAL_VARIATIONS = 20;

//*****************************************************************************
// Constructor. Allocates MIL application, system and display.
//*****************************************************************************
CDefectDetectionExampleMngr::CDefectDetectionExampleMngr(MIL_CONST_TEXT_PTR SystemDescriptor)
: m_MilApplication               (M_NULL),
  m_MilSystem                    (M_NULL),
  m_MilDisplay                   (M_NULL),
  m_MilDisplayImage              (M_NULL),
  m_MilGraList                   (M_NULL),
  m_MilGraContextTemplate        (M_NULL),
  m_MilGraContextTarget          (M_NULL),
  m_MilGraContextWarpedTarget    (M_NULL),
  m_MilGraContextDifference      (M_NULL),
  m_MilGraContextInspection      (M_NULL),
  m_MilHistDisplay               (M_NULL),
  m_MilHistDisplayImage          (M_NULL),
  m_MilHistDispGraphicList       (M_NULL),
  m_MilHistResult                (M_NULL),
  m_MilDispTemplateImage         (M_NULL),
  m_MilTemplateImage             (M_NULL),         
  m_MilTargetImage               (M_NULL),
  m_MilWarpedTargetImage         (M_NULL),
  m_MilDifferenceImage           (M_NULL),
  m_MilInspectionImage           (M_NULL),
  m_MilTemplateGradientMask      (M_NULL),
  m_MilTemplateLumImage          (M_NULL),
  m_MilTargetLumImage            (M_NULL),
  m_MilDifferenceGrayImage       (M_NULL),
  m_MilStructElement             (M_NULL),
  m_MilModContext                (M_NULL),
  m_MilModResult                 (M_NULL),
  m_MilBlobResult                (M_NULL),
  m_BlobDrawLabel                (M_NULL),
  m_MilBlobFeatureList           (M_NULL),
  m_MilFixturingOffset           (M_NULL),
  m_TemplateSizeX                (0),
  m_TemplateSizeY                (0),
  m_DiffMethod                   (enAbsoluteDiff),
  m_BinMethod                    (enBiModal),
  m_OverlayTransparentColor      (0),
  m_IsInteractive                (false),
  m_ImageSpacing                 (IMAGE_SEPARATION_X),
  m_ModOccDrawStartIndex         (0)
   {
   // Allocate application.
   MappAlloc(M_NULL, M_DEFAULT, &m_MilApplication);

   // Allocate system (SystemDescriptor depends on subclass).
   MsysAlloc(M_DEFAULT, SystemDescriptor, M_DEFAULT, M_DEFAULT, &m_MilSystem);

   // Allocate display. Set it to auto scale.
   MdispAlloc(m_MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &m_MilDisplay);
   MdispControl(m_MilDisplay, M_SCALE_DISPLAY, M_ENABLE);

   // Allocate a graphic list and associate it with the display.
   MgraAllocList(m_MilSystem, M_DEFAULT, &m_MilGraList);
   MdispControl(m_MilDisplay, M_ASSOCIATED_GRAPHIC_LIST_ID, m_MilGraList);

   // Allocate the histogram displays.
   MdispAlloc(m_MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &m_MilHistDisplay);

   // Allocate the histogram display graphic list and image and initialize the histogram.
   MbufAlloc2d(m_MilSystem, 256 + 2*HIST_BORDER, 256 + 2*HIST_BORDER, 8+M_UNSIGNED, M_IMAGE+M_DISP, &m_MilHistDisplayImage);
   MbufClear(m_MilHistDisplayImage, 0);
   MgraAllocList(m_MilSystem, M_DEFAULT, &m_MilHistDispGraphicList);
   InitHistogram();

   // Associate the graphic list to the display.
   MdispControl(m_MilHistDisplay, M_ASSOCIATED_GRAPHIC_LIST_ID, m_MilHistDispGraphicList);
   
   // Allocate an histogram result buffer.
   MimAllocResult(m_MilSystem, 256, M_HIST_LIST, &m_MilHistResult);

   // Allocate structuring element.
   MbufAlloc2d(m_MilSystem, DEFECT_SIZE + 1, DEFECT_SIZE + 1, 32+M_UNSIGNED, M_STRUCT_ELEMENT, &m_MilStructElement);
   MbufClear(m_MilStructElement, 0);

   // Allocate Model Finder.
   MmodAlloc(m_MilSystem, M_GEOMETRIC, M_DEFAULT, &m_MilModContext);
   MmodAllocResult(m_MilSystem, M_DEFAULT, &m_MilModResult);

   // Allocate Blob.
   MblobAllocResult(m_MilSystem, &m_MilBlobResult);
   MblobAllocFeatureList(m_MilSystem, &m_MilBlobFeatureList);

   // Allocate a calibration fixture.
   McalAlloc(m_MilSystem, M_FIXTURING_OFFSET, M_DEFAULT, &m_MilFixturingOffset);
   }

//*****************************************************************************
// Destructor, free all MIL objects.
//*****************************************************************************
CDefectDetectionExampleMngr::~CDefectDetectionExampleMngr()
   {
   // Free the display images.
   FreeDisplayImages();

   // Free the work images. 
   FreeWorkImages();
  
   // Free the structuring element.
   if(m_MilStructElement != M_NULL)
      MbufFree(m_MilStructElement);
   
   // Free Model Finder.
   if(m_MilModContext != M_NULL)
      {
      MmodFree(m_MilModContext);
      MmodFree(m_MilModResult);
      }

   // Free Blob.
   if(m_MilBlobResult != M_NULL)
      {
      MblobFree(m_MilBlobResult);
      MblobFree(m_MilBlobFeatureList);
      }

   // Free the calibration fixture.
   if(m_MilFixturingOffset != M_NULL)
      McalFree(m_MilFixturingOffset);

   // Free the histogram.
   if(m_MilHistResult)
      MimFree(m_MilHistResult);

   // Free the histogram graphic list
   if(m_MilHistDispGraphicList != M_NULL)
      MgraFree(m_MilHistDispGraphicList);

   // Free the graphic list.
   if(m_MilGraList != M_NULL)
      MgraFree(m_MilGraList);

   //Free the histogram display selected image
   if(m_MilHistDisplayImage != M_NULL)
      MbufFree(m_MilHistDisplayImage);

   // Free the histogram display.
   if(m_MilHistDisplay != M_NULL)
      MdispFree(m_MilHistDisplay);

   // Free the display.
   if (m_MilDisplay != M_NULL)
      MdispFree(m_MilDisplay);

   // Free the system.
   if (m_MilSystem != M_NULL)
      MsysFree(m_MilSystem);

   // Free the application.
   if (m_MilApplication != M_NULL)
      MappFree(m_MilApplication);
   }

//*****************************************************************************
// Free the display associated images.
//*****************************************************************************
void CDefectDetectionExampleMngr::FreeDisplayImages()
   {
   if (m_MilDisplayImage != M_NULL)
      {
      MbufFree(m_MilDispTemplateImage);
      MbufFree(m_MilTargetImage);
      MbufFree(m_MilWarpedTargetImage);
      MbufFree(m_MilDifferenceImage);
      MbufFree(m_MilInspectionImage);
      MbufFree(m_MilTargetLumImage);

      MgraFree(m_MilGraContextTemplate);
      MgraFree(m_MilGraContextTarget);
      MgraFree(m_MilGraContextWarpedTarget);
      MgraFree(m_MilGraContextDifference);
      MgraFree(m_MilGraContextInspection);
      MbufFree(m_MilDisplayImage);
      }
   }

//*****************************************************************************
// Free the work images.
//*****************************************************************************
void CDefectDetectionExampleMngr::FreeWorkImages()
   {
   if (m_MilTemplateGradientMask != M_NULL)
      {
      MbufFree(m_MilTemplateGradientMask);
      MbufFree(m_MilTemplateLumGradientMask);
      MbufFree(m_MilTemplateLumImage);    
      MbufFree(m_MilDifferenceGrayImage);
      MbufFree(m_MilTemplateImage);
      }
   }

//*****************************************************************************
// Load the template. Load the image, create a luminance version and defines
// a model.
//*****************************************************************************
bool CDefectDetectionExampleMngr::LoadTemplate(MIL_CONST_TEXT_PTR TemplateImagePath)
   {
   // Free the work images.
   FreeWorkImages();
   
   if(TemplateImagePath == M_INTERACTIVE)
      MosPrintf(MIL_TEXT("Please choose a template image.\n\n"));

   // Restore the template user image.
   MIL_ID MilUserTemplate = MbufRestore(TemplateImagePath, m_MilSystem, M_NULL);

   if(MilUserTemplate != M_NULL)
      {
      // Get the size of the user template.
      MbufInquire(MilUserTemplate, M_SIZE_X, &m_TemplateSizeX);
      MbufInquire(MilUserTemplate, M_SIZE_Y, &m_TemplateSizeY);

      // Allocate the color template image and copy the user image.
      MbufAllocColor(m_MilSystem, 3, m_TemplateSizeX, m_TemplateSizeY, 8+M_UNSIGNED, M_IMAGE+M_PROC, &m_MilTemplateImage);
      MbufCopy(MilUserTemplate, m_MilTemplateImage);

      // Free the user template.
      MbufFree(MilUserTemplate);

      //Allocate the work images.
      MbufAlloc2d(m_MilSystem, m_TemplateSizeX, m_TemplateSizeY, 8+M_UNSIGNED, M_IMAGE+M_PROC, &m_MilDifferenceGrayImage);
      
      //Create the luminance version of the template.
      CreateLuminanceImage(m_MilTemplateImage, m_TemplateSizeX, m_TemplateSizeY, &m_MilTemplateLumImage);

      //Create the gradient mask.
      CreateGradientMaskImage(m_MilTemplateImage, m_MilTemplateLumImage, GRADIENT_MASK_SMOOTHNESS, &m_MilTemplateGradientMask, &m_MilTemplateLumGradientMask);

      // Define the model from the template image.
      DefineModelAndFixture(m_MilTemplateLumImage, m_MilModContext, m_MilFixturingOffset, MODEL_OFFSET_X, MODEL_OFFSET_Y, MODEL_SIZE_X, MODEL_SIZE_Y);
      
      return true;
      }
   else
      {
      // Print error message.
      MosPrintf(MIL_TEXT("Unable to load template image.\n\n"));
      return false;   
      }
   }

//*****************************************************************************
// Initialize the example with the target image. Sets the display, creates a
// luminance image of the target and calibrates the warped target with a uniform 
// calibration.
//*****************************************************************************
bool CDefectDetectionExampleMngr::InitializeExample(MIL_CONST_TEXT_PTR TargetImagePath, DifferenceExtractionMethod DiffMethod, BinarizationMethod BinMethod)
   {
   // Initialize the methods use.
   m_DiffMethod = DiffMethod;
   m_BinMethod = BinMethod;

   if(TargetImagePath == M_INTERACTIVE)
      MosPrintf(MIL_TEXT("Please choose a target image.\n\n"));

   // Restore the target user image.
   MIL_ID MilUserTarget = MbufRestore(TargetImagePath, m_MilSystem, M_NULL);

   if(MilUserTarget != M_NULL)
      {
      // Get the size of the user target.
      MIL_INT TargetSizeX = MbufInquire(MilUserTarget, M_SIZE_X, M_NULL);
      MIL_INT TargetSizeY = MbufInquire(MilUserTarget, M_SIZE_Y, M_NULL);
 
      // Reset the display.
      ResetDisplay(TargetSizeX, TargetSizeY);   
     
      // Copy the template image in the display.
      MbufCopy(m_MilTemplateImage, m_MilDispTemplateImage);
   
      // Copy the target image.
      MbufCopy(MilUserTarget, m_MilTargetImage);

      // Free the user target.
      MbufFree(MilUserTarget);
   
      // Create the luminance version of the target. Free it if it was allocated.
      CreateLuminanceImage(m_MilTargetImage, TargetSizeX, TargetSizeY, &m_MilTargetLumImage);      
   
      // Associate a uniform calibration to the warped target image.
      McalUniform(m_MilWarpedTargetImage, 0, 0, 1, 1, 0, M_DEFAULT);  
   
      // Show the template coordinate and model information.
      ShowTargetAndTemplate(TargetImagePath);

      return true;
      }
   else
      {
      // Print error message.
      MosPrintf(MIL_TEXT("Unable to load target image.\n\n"));
      return false;
      }
   }

//*****************************************************************************
// Main running function of the example.
//*****************************************************************************
void CDefectDetectionExampleMngr::Run(MIL_CONST_TEXT_PTR CaseInformationText)
   {
   // 1. Get the number of occurrences.
   m_NbOccurences = FindModel(m_MilModContext, m_MilTargetLumImage, m_MilModResult);

   if(m_NbOccurences > 0)
      {
      // Show the occurrences found in the display.
      ShowAllOccurrences();      

      for(MIL_INT OccurrenceIdx = 0; OccurrenceIdx < m_NbOccurences; OccurrenceIdx++)
         {
         // Clear the inspection image and of the difference image.
         ClearDefectDetection();

         // 2. Align the target on the template based on the Model Finder result
         AlignImageBasedOnFixture(m_MilTargetImage, m_MilWarpedTargetImage, m_MilFixturingOffset, m_MilModResult, M_RESULT_MOD, OccurrenceIdx);

         // Show the warped occurrence in the target display.
         ShowWarpedOccurrence(OccurrenceIdx);         
         
         // Set the initial methods.
         DifferenceExtractionMethod DiffMethod = m_DiffMethod;
         BinarizationMethod BinMethod = m_BinMethod;
         
         // Print the case information
         MosPrintf(MIL_TEXT("%s\n\n"), CaseInformationText);
         
         m_ChangeDiff = true;
         m_ChangeBin = true;
         
         do
            {
            // Update the extracted differences if necessary.
            if(m_ChangeDiff)
               {
               // 3. Extract the differences.
               ExtractDifferences(m_MilTemplateImage,
                                  m_MilTemplateLumImage,
                                  m_MilTemplateGradientMask,
                                  m_MilTemplateLumGradientMask,
                                  m_MilWarpedTargetImage,
                                  m_MilDifferenceGrayImage,
                                  m_MilStructElement,
                                  DiffMethod);

               // Update the binarization if we have changed the difference image.
               m_ChangeBin = true;
               }

            // Update the binarization if necessary.
            if(m_ChangeBin)
               {
               // 4. Extract the defects.
               m_BinValue = ExtractDefects(m_MilDifferenceGrayImage,
                                           m_MilBlobResult,
                                           m_MilBlobFeatureList,
                                           TRIANGLE_LOWER_CUTOFF,
                                           TRIANGLE_UPPER_CUTOFF,
                                           BIN_CUMULATIVE_VALUE,
                                           NORMAL_VARIATIONS,
                                           FIXED_DIFF_THRESHOLD,
                                           CLEAN_MORPH_SIZE,
                                           BinMethod);              
               }  

            // Show the defect detection.
            ShowDefectDetection();

            // Show the histogram.
            ShowHistogram();

            m_ChangeBin = false;
            m_ChangeDiff = false;

            }while(AskForMethod(DiffMethod, BinMethod));
            
            //Remove the histogram
            MdispSelect(m_MilHistDisplay, M_NULL);
         }
      }
   else
      MosPrintf(MIL_TEXT("No occurrence of the model were found in the target image\n\n"));
   }

//*****************************************************************************
// Resets the display.
//*****************************************************************************
void CDefectDetectionExampleMngr::ResetDisplay(MIL_INT TargetSizeX, MIL_INT TargetSizeY)
   {
   // Compute the offsets and sizes of the different childs.
   MIL_INT InspectionChildOffsetX = 4 * IMAGE_SEPARATION_X + 3* m_TemplateSizeX;
   MIL_INT DispSizeX, TargetChildOffsetX;
   if(InspectionChildOffsetX + m_TemplateSizeX > TargetSizeX + IMAGE_SEPARATION_X)
      {
      DispSizeX = InspectionChildOffsetX + m_TemplateSizeX + IMAGE_SEPARATION_X;
      TargetChildOffsetX = (DispSizeX - TargetSizeX)/2;  
      m_ImageSpacing = IMAGE_SEPARATION_X;
      }
   else
      {      
      DispSizeX = TargetSizeX + 2 * IMAGE_SEPARATION_X;
      TargetChildOffsetX = IMAGE_SEPARATION_X;
      m_ImageSpacing = (DispSizeX - 4 * m_TemplateSizeX)/5;
      }
   MIL_INT WarpedChildOffsetX = 2 * m_ImageSpacing + m_TemplateSizeX;
   MIL_INT DifferenceChildOffsetX = WarpedChildOffsetX + m_ImageSpacing +  m_TemplateSizeX;
   InspectionChildOffsetX = DifferenceChildOffsetX + m_ImageSpacing + m_TemplateSizeX;
   MIL_INT TargetChildOffsetY = 3 * WINDOW_TITLE_HEIGHT + m_TemplateSizeY + TARGET_IMAGE_SEPARATION_Y; 
   MIL_INT DispSizeY = TargetChildOffsetY + TargetSizeY + IMAGE_SEPARATION_X; 

   // Free the displayed images.
   FreeDisplayImages();

   // Clear the graphic list.
   MgraClear(M_DEFAULT, m_MilGraList);
   
   // Allocate the display image.
   MbufAllocColor(m_MilSystem, 3, DispSizeX, DispSizeY, 8+M_UNSIGNED, M_IMAGE+M_PROC+M_DISP, &m_MilDisplayImage);

   // Allocate the works images that are childs of the display.
   MbufChild2d(m_MilDisplayImage, m_ImageSpacing, WINDOW_TITLE_HEIGHT, m_TemplateSizeX, m_TemplateSizeY, &m_MilDispTemplateImage);
   MbufChild2d(m_MilDisplayImage, TargetChildOffsetX, TargetChildOffsetY, TargetSizeX, TargetSizeY, &m_MilTargetImage);
   MbufChild2d(m_MilDisplayImage, WarpedChildOffsetX, WINDOW_TITLE_HEIGHT, m_TemplateSizeX, m_TemplateSizeY, &m_MilWarpedTargetImage);
   MbufChild2d(m_MilDisplayImage, DifferenceChildOffsetX, WINDOW_TITLE_HEIGHT, m_TemplateSizeX, m_TemplateSizeY, &m_MilDifferenceImage);
   MbufChild2d(m_MilDisplayImage, InspectionChildOffsetX, WINDOW_TITLE_HEIGHT, m_TemplateSizeX, m_TemplateSizeY, &m_MilInspectionImage);

   // Clear the displayed image.
   MbufClear(m_MilDisplayImage, 0);

   // Select the image on the display.
   MdispSelect(m_MilDisplay, m_MilDisplayImage);
   
   // Allocate the graphic contexts of the childs of the overlay.
   MgraAlloc(m_MilSystem, &m_MilGraContextTemplate);
   MgraControl(m_MilGraContextTemplate, M_DRAW_OFFSET_X, - (MIL_DOUBLE)m_ImageSpacing);
   MgraControl(m_MilGraContextTemplate, M_DRAW_OFFSET_Y, - WINDOW_TITLE_HEIGHT);

   MgraAlloc(m_MilSystem, &m_MilGraContextTarget);
   MgraControl(m_MilGraContextTarget, M_DRAW_OFFSET_X, - (MIL_DOUBLE)TargetChildOffsetX);
   MgraControl(m_MilGraContextTarget, M_DRAW_OFFSET_Y, - (MIL_DOUBLE)TargetChildOffsetY);

   MgraAlloc(m_MilSystem, &m_MilGraContextWarpedTarget);
   MgraControl(m_MilGraContextWarpedTarget, M_DRAW_OFFSET_X, - (MIL_DOUBLE)WarpedChildOffsetX);
   MgraControl(m_MilGraContextWarpedTarget, M_DRAW_OFFSET_Y, - WINDOW_TITLE_HEIGHT);

   MgraAlloc(m_MilSystem, &m_MilGraContextDifference);
   MgraControl(m_MilGraContextDifference, M_DRAW_OFFSET_X, - (MIL_DOUBLE)DifferenceChildOffsetX);
   MgraControl(m_MilGraContextDifference, M_DRAW_OFFSET_Y, - WINDOW_TITLE_HEIGHT);

   MgraAlloc(m_MilSystem, &m_MilGraContextInspection);
   MgraControl(m_MilGraContextInspection, M_DRAW_OFFSET_X, - (MIL_DOUBLE)InspectionChildOffsetX);
   MgraControl(m_MilGraContextInspection, M_DRAW_OFFSET_Y, - WINDOW_TITLE_HEIGHT);

   // Draw the display childs info.
   DrawDisplayChildInfo(m_MilGraContextTemplate    , m_TemplateSizeX, m_TemplateSizeY, MIL_TEXT("Template Image"));
   DrawDisplayChildInfo(m_MilGraContextTarget      , TargetSizeX    , TargetSizeY    , MIL_TEXT("Target Image"));
   DrawDisplayChildInfo(m_MilGraContextWarpedTarget, m_TemplateSizeX, m_TemplateSizeY, MIL_TEXT("Warped Target"));
   DrawDisplayChildInfo(m_MilGraContextDifference  , m_TemplateSizeX, m_TemplateSizeY, MIL_TEXT("Difference Image"));
   DrawDisplayChildInfo(m_MilGraContextInspection  , m_TemplateSizeX, m_TemplateSizeY, MIL_TEXT("Inspection Image"));
   }

//*****************************************************************************
// Draw display child info.
//*****************************************************************************
void CDefectDetectionExampleMngr::DrawDisplayChildInfo(MIL_ID MilGraContext, MIL_INT ImageSizeX, MIL_INT ImageSizeY, MIL_CONST_TEXT_PTR ImageName)
   {
   // Set the text alignment and color of the graphic contexts.
   MgraControl(MilGraContext, M_TEXT_ALIGN_HORIZONTAL, M_CENTER);
   MgraControl(MilGraContext, M_TEXT_ALIGN_VERTICAL, M_BOTTOM);
      
   // Draw the names of the images over them.
   MgraColor(MilGraContext, M_COLOR_CYAN);
   MgraText(MilGraContext, m_MilGraList, ImageSizeX/2, -WINDOW_TITLE_HEIGHT/2, ImageName);
   
   // Draw borders around the childs.
   MgraColor(MilGraContext, M_COLOR_WHITE);
   MgraRectAngle(MilGraContext, m_MilGraList, ImageSizeX/2, ImageSizeY/2, ImageSizeX + m_ImageSpacing, ImageSizeY + WINDOW_TITLE_HEIGHT, 0, M_CENTER_AND_DIMENSION);
   }

//*****************************************************************************
// Create the luminance version of the source image.
//*****************************************************************************
void CDefectDetectionExampleMngr::CreateLuminanceImage(MIL_ID MilRGBSrc, MIL_INT SizeX, MIL_INT SizeY, MIL_ID *MilLumImagePtr)
   {
   MbufAlloc2d(m_MilSystem, SizeX, SizeY, 8+M_UNSIGNED, M_IMAGE+M_PROC, MilLumImagePtr);
   MimConvert(MilRGBSrc, *MilLumImagePtr, M_RGB_TO_L);   
   }

//*****************************************************************************
// Displays the information of the template (model edges, coordinate system 
// and fixturing offset).
//*****************************************************************************
void CDefectDetectionExampleMngr::ShowTargetAndTemplate(MIL_CONST_TEXT_PTR TargetImagePath)
   {
   // Print intermediate info in the console.
   MIL_CONST_TEXT_PTR TargetImageName = GetFileName(TargetImagePath); 
   MosPrintf(MIL_TEXT("The target image %s has been loaded \n"), TargetImageName);
   MosPrintf(MIL_TEXT("and is shown in the display.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();
   
   // Show the fixture offset and the coordinate system on the template.
   MgraColor(m_MilGraContextTemplate, M_COLOR_YELLOW);
   McalDraw(m_MilGraContextTemplate, m_MilFixturingOffset, m_MilGraList, M_DRAW_FIXTURING_OFFSET, M_DEFAULT, M_DEFAULT);
   MgraColor(m_MilGraContextTemplate, M_COLOR_GREEN);
   McalDraw(m_MilGraContextTemplate, M_NULL, m_MilGraList, M_DRAW_PIXEL_COORDINATE_SYSTEM+M_DRAW_FRAME, M_DEFAULT, M_DEFAULT);

   // Show the model edges.
   MgraColor(m_MilGraContextTemplate, M_COLOR_RED);
   MmodDraw(m_MilGraContextTemplate, m_MilModContext, m_MilGraList, M_DRAW_EDGES, M_DEFAULT, M_ORIGINAL);

   // Print intermediate info in the console.
   MosPrintf(MIL_TEXT("The model's edges and the fixturing offset are shown in the \n"));
   MosPrintf(MIL_TEXT("template display.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();
   }

//*****************************************************************************
// Displays all the occurrences of the model that were found.
//*****************************************************************************
void CDefectDetectionExampleMngr::ShowAllOccurrences()
   {
   MgraColor(m_MilGraContextTarget, M_COLOR_GREEN);
   for(MIL_INT i = 0; i < m_NbOccurences; i++)
      {
      MmodDraw(m_MilGraContextTarget, m_MilModResult, m_MilGraList, M_DRAW_BOX, i, M_DEFAULT);
      MmodDraw(m_MilGraContextTarget, m_MilModResult, m_MilGraList, M_DRAW_POSITION, i, M_DEFAULT);
      if(i == 0)
         m_ModOccDrawStartIndex = MgraInquireList(m_MilGraList, M_LIST, M_DEFAULT, M_NUMBER_OF_GRAPHICS, M_NULL) - 2;
      }   

   // Print intermediate info in the console.
   MosPrintf(MIL_TEXT("The occurrences found in the target image are shown in the target\n"));
   MosPrintf(MIL_TEXT("display.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();
   }

//*****************************************************************************
// Displays the current warped occurrence.
//*****************************************************************************
void CDefectDetectionExampleMngr::ShowWarpedOccurrence(MIL_INT OccurrenceIdx)
   {
   // Set the color of the occurences. All are green except the one to process which is red.
   for(MIL_INT i = 0, GraphicIndex = m_ModOccDrawStartIndex; i < m_NbOccurences; i++)
      {
      MgraControlList(m_MilGraList, M_GRAPHIC_INDEX(GraphicIndex++), M_DEFAULT, M_COLOR, i == OccurrenceIdx? M_COLOR_RED : M_COLOR_GREEN);
      MgraControlList(m_MilGraList, M_GRAPHIC_INDEX(GraphicIndex++), M_DEFAULT, M_COLOR, i == OccurrenceIdx? M_COLOR_RED : M_COLOR_GREEN);
      }
  
   // Show the coordinate system of the warped target and of the fixtured target.
   MgraColor(m_MilGraContextTarget, M_COLOR_RED);
   McalDraw(m_MilGraContextTarget, m_MilTargetImage, m_MilGraList, M_DRAW_RELATIVE_COORDINATE_SYSTEM+M_DRAW_FRAME, M_DEFAULT, M_DEFAULT);
   MgraColor(m_MilGraContextWarpedTarget, M_COLOR_RED);
   McalDraw(m_MilGraContextWarpedTarget, m_MilWarpedTargetImage, m_MilGraList, M_DRAW_RELATIVE_COORDINATE_SYSTEM+M_DRAW_FRAME, M_DEFAULT, M_DEFAULT);

   // Print intermediate info in the console.
   MosPrintf(MIL_TEXT("The occurrence %i has been warped.\n"), OccurrenceIdx);
   MosPrintf(MIL_TEXT("Will now proceed with the defect detection.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();
   }

//*****************************************************************************
// Displays the defect detection.
//*****************************************************************************
void CDefectDetectionExampleMngr::ShowDefectDetection()
   {
   if(m_ChangeDiff)
      {
      // Update the displayed difference image and inspection image(the displayed images are the same).
      MbufCopy(m_MilDifferenceGrayImage, m_MilDifferenceImage);
      MbufCopy(m_MilDifferenceGrayImage, m_MilInspectionImage);
      }

   if(m_ChangeBin)
      {
      if(m_BlobDrawLabel)
         MgraControlList(m_MilGraList, M_GRAPHIC_LABEL(m_BlobDrawLabel), M_DEFAULT, M_DELETE, M_DEFAULT);
      MgraColor(m_MilGraContextInspection, M_COLOR_RED);
      MblobDraw(m_MilGraContextInspection, m_MilBlobResult, m_MilGraList, M_DRAW_BLOBS, M_DEFAULT, M_DEFAULT);
      MgraInquireList(m_MilGraList, M_LIST, M_DEFAULT, M_LAST_LABEL, &m_BlobDrawLabel);
      }
   }

//*****************************************************************************
// Clears the defect detection.
//*****************************************************************************
void CDefectDetectionExampleMngr::ClearDefectDetection()
   {
   // Get the number of graphics in the graphic list.
   MIL_INT NbGraphics = MgraInquireList(m_MilGraList, M_LIST, M_DEFAULT, M_NUMBER_OF_GRAPHICS, M_NULL);

   // Clear all the primitives related to the defect detection.
   for(MIL_INT GraphicIndex = m_ModOccDrawStartIndex + m_NbOccurences*2; GraphicIndex < NbGraphics; GraphicIndex++)
      MgraControlList(m_MilGraList, M_GRAPHIC_INDEX(GraphicIndex), M_DEFAULT, M_DELETE, M_DEFAULT);

   // Reset the blob draw label.
   m_BlobDrawLabel = M_NULL;
   }

//*****************************************************************************
// Prints a given method choices.
//*****************************************************************************
void CDefectDetectionExampleMngr::PrintMethodChoices(MIL_CONST_TEXT_PTR MethodTypeTag,
                        MIL_CONST_TEXT_PTR *MethodsTags,
                        MIL_INT NbMethod,
                        MIL_TEXT_CHAR StartChar)
   {
   MosPrintf(MIL_TEXT("%s method:\n"), MethodTypeTag);
   for(MIL_INT i=0; i<NbMethod; i++)
      MosPrintf(MIL_TEXT("   %c. %s\n"), StartChar+i, MethodsTags[i]);
   MosPrintf(MIL_TEXT("\n"));
   }

//*****************************************************************************
// Prints the current methods.
//*****************************************************************************
void CDefectDetectionExampleMngr::PrintCurrentMethods(MIL_TEXT_CHAR DiffStartChar, MIL_TEXT_CHAR BinStartChar, DifferenceExtractionMethod DiffMethod, BinarizationMethod BinMethod)
   {
   MosPrintf(MIL_TEXT("Current methods for difference extraction and binarization:\n"));
   MosPrintf(MIL_TEXT("%c. %s\n"), DiffStartChar+DiffMethod, DifferenceExtractionMethodsTags[DiffMethod]);
   MosPrintf(MIL_TEXT("%c. %s\n\n"), BinStartChar+BinMethod, BinarizationMethodsTags[BinMethod]);      
   }

//*****************************************************************************
// Ask the user for interactive method choice.
//*****************************************************************************
bool CDefectDetectionExampleMngr::AskForInteractive()
   {
   // Ask the user if he wants to run the example in interactive mode.
   MosPrintf(MIL_TEXT("Do you want to run the example in interactive mode? (Y or N)\n\n"));
   do 
      {
      switch(MosGetch())
         {
         case 'y':
         case 'Y':
            m_IsInteractive = true;
            return true;
            
         case 'n':
         case 'N':               
            return false;
         }
      } while(1);
   
   return true;
   }
    
//*****************************************************************************
// Ask the user for a method modification.
//*****************************************************************************
bool CDefectDetectionExampleMngr::AskForMethod(DifferenceExtractionMethod &DiffMethod, BinarizationMethod &BinMethod)
   {
   if(m_IsInteractive)
      {
      // Print the methods currently in use.
      PrintCurrentMethods(MIL_TEXT('1'), MIL_TEXT('A'), DiffMethod, BinMethod);

      // Print the methods choices. 
      PrintMethodChoices(MIL_TEXT("Difference extraction"), DifferenceExtractionMethodsTags, enNbDiffMethod, MIL_TEXT('1'));
      PrintMethodChoices(MIL_TEXT("Binarization"), BinarizationMethodsTags, enNbBinMethod, MIL_TEXT('A'));
      MosPrintf(MIL_TEXT("Other:\n"));
      MosPrintf(MIL_TEXT("   Q. Continue\n\n"));
      MosPrintf(MIL_TEXT("Your choice : "));

      MIL_TEXT_CHAR Choice;
      while(!m_ChangeDiff && !m_ChangeBin)
         {
         // Get the user's choice. 
         Choice = (MIL_TEXT_CHAR)MosGetch();
         
         switch(Choice)
            {
            case '1':
               m_ChangeDiff = true;
               DiffMethod = enAbsoluteDiff;
               break;

            case '2':
               m_ChangeDiff = true;
               DiffMethod = enColDistance;
               break;

            case '3':
               m_ChangeDiff = true;
               DiffMethod = enBottomHat;
               break;

            case '4':
               m_ChangeDiff = true;
               DiffMethod = enTopHat;
               break;

            case 'a':
            case 'A':
               m_ChangeBin = true;
               BinMethod = enBiModal;
               break;

            case 'b':
            case 'B':
               m_ChangeBin = true;
               BinMethod = enTriangleBisection;
               break;

            case 'c':
            case 'C':
               m_ChangeBin = true;
               BinMethod = enCumulHistPercentage;
               break;

            case 'd':
            case 'D':
               m_ChangeBin = true;
               BinMethod = enFixed;
               break;

            case 'q':
            case 'Q':
               MosPrintf(MIL_TEXT("Q. Continue\n\n"));
               return false;
            
            default:
               break;
            }
         } 

      if(m_ChangeDiff)
         MosPrintf(MIL_TEXT("%c. %s\n\n"), Choice, DifferenceExtractionMethodsTags[DiffMethod]);
      else if(m_ChangeBin)
         MosPrintf(MIL_TEXT("%c. %s\n\n"), Choice, BinarizationMethodsTags[BinMethod]);
      return true;
      }
   else
      {
      MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
      MosGetchar();
      return false;
      }
   }

//*****************************************************************************
// Draws the histogram of the image
//*****************************************************************************
void CDefectDetectionExampleMngr::ShowHistogram()
   {
   // Calculate the histogram.
   MimHistogram(m_MilDifferenceGrayImage, m_MilHistResult);

   // Get the results of the histogram.
   MIL_INT Histogram[256];
   MimGetResult(m_MilHistResult, M_VALUE+M_TYPE_MIL_INT, Histogram);

   // Get the maximum value of the histogram.
   MIL_INT MaxHistValue = -MIL_INT_MAX;
   for(MIL_INT i = ((MIL_INT) TRIANGLE_LOWER_CUTOFF); i < 256; i++)
      MaxHistValue = Histogram[i] > MaxHistValue ? Histogram[i] : MaxHistValue;

   if(MaxHistValue < 1) { MaxHistValue = 1; }

   // Calculate the pixel scale.
   MIL_DOUBLE YPixelScale = 256.0 / MaxHistValue;

   // Disable the graphic updates.
   MdispControl(m_MilHistDisplay, M_UPDATE_GRAPHIC_LIST, M_DISABLE);

   // Update the histogram bands.
   for(MIL_INT i = 0; i < 256; i++)
      {
      MIL_DOUBLE Width = Histogram[i]*YPixelScale; 
      MgraControlList(m_MilHistDispGraphicList,
                      M_GRAPHIC_INDEX(i),
                      M_DEFAULT,
                      M_RECTANGLE_WIDTH,
                      Width > 256.0 ? 256.0 : Width);
      }

   // Update the threshold
   MgraControlList(m_MilHistDispGraphicList, M_GRAPHIC_INDEX(256), M_DEFAULT, M_POSITION_X, (MIL_DOUBLE) (HIST_BORDER + m_BinValue + 0.5));
   
   // Update the threshold text
   MIL_TEXT_CHAR BinValueString[4];
   MosSprintf(BinValueString, 4, MIL_TEXT("%i"), m_BinValue);
   MgraControlList(m_MilHistDispGraphicList, M_GRAPHIC_INDEX(260), M_DEFAULT, M_DELETE, M_DEFAULT);
   MgraControl(M_DEFAULT, M_TEXT_ALIGN_HORIZONTAL, M_CENTER);
   MgraControl(M_DEFAULT, M_TEXT_ALIGN_VERTICAL, M_TOP);
   MgraColor(M_DEFAULT, M_COLOR_BLUE);
   MgraText(M_DEFAULT, m_MilHistDispGraphicList, HIST_BORDER + m_BinValue + 0.5, 256 + HIST_BORDER + 5, BinValueString);

   // Enable The graphic updates.
   MdispControl(m_MilHistDisplay, M_UPDATE_GRAPHIC_LIST, M_ENABLE);

   // Select the image on the display to show it
   MdispSelect(m_MilHistDisplay, m_MilHistDisplayImage);
   }

//*****************************************************************************
// Initialize the Histogram 
//*****************************************************************************
void CDefectDetectionExampleMngr::InitHistogram()
   {
   // Create the histogram bands.
   MgraColor(M_DEFAULT, M_COLOR_GREEN);
   for(MIL_INT i = 0; i < 256; i++)
      MgraRectAngle(M_DEFAULT, m_MilHistDispGraphicList, i+HIST_BORDER, 256.0 + HIST_BORDER, 1, 1, 90, M_CORNER_AND_DIMENSION + M_FILLED);
      
   // Create the threshold
   MgraColor(M_DEFAULT, M_COLOR_BLUE);
   MgraLine(M_DEFAULT, m_MilHistDispGraphicList, HIST_BORDER, HIST_BORDER/2, HIST_BORDER, 261.0+HIST_BORDER);
  
   // Create the axis on the histogram.
   MgraColor(M_DEFAULT, M_COLOR_RED);
   MgraLine(M_DEFAULT, m_MilHistDispGraphicList, HIST_BORDER, 256.0 + HIST_BORDER, HIST_BORDER, HIST_BORDER/2);
   MgraLine(M_DEFAULT, m_MilHistDispGraphicList, HIST_BORDER, 256.0 + HIST_BORDER, 256.0 + 3*HIST_BORDER/2, 256.0 + HIST_BORDER);

   // Create the histogram title.
   MgraColor(M_DEFAULT, M_COLOR_CYAN);
   MgraControl(M_DEFAULT, M_TEXT_ALIGN_HORIZONTAL, M_CENTER);
   MgraControl(M_DEFAULT, M_TEXT_ALIGN_VERTICAL, M_TOP);
   MgraText(M_DEFAULT, m_MilHistDispGraphicList, 128.0 + HIST_BORDER, 0, MIL_TEXT("Histogram"));

   //Draw the threshold text
   MgraControl(M_DEFAULT, M_TEXT_ALIGN_HORIZONTAL, M_CENTER);
   MgraControl(M_DEFAULT, M_TEXT_ALIGN_VERTICAL, M_TOP);
   MgraText(M_DEFAULT, m_MilHistDispGraphicList, HIST_BORDER, 256.0 + HIST_BORDER + 5, MIL_TEXT(""));  
   }