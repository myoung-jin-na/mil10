//***************************************************************************************
// 
// File name: ExampleManager.cpp
//
// Synopsis: This file contains the implementation of the CExampleMngr class that
//           manages the different inspection tasks defined from a specific example.
//

#include <mil.h>
#include <math.h>
#include "ExampleManager.h"
#include "../InspectionTaskBase/InspectionTask.h"

//*****************************************************************************
// Constants.
//*****************************************************************************
static const MIL_INT MAX_DISPLAY_SIZE_Y = 1080;
static const MIL_INT SAMPLES_BORDER_Y   = 10;
static const MIL_INT VIEW_BORDER_X      = 40;

static const MIL_INT    NB_ARROW_POINT     = 7;
static const MIL_DOUBLE ARROW_X[NB_ARROW_POINT] = {   5,  25,  25,  35,  25, 25, 5 };
static const MIL_DOUBLE ARROW_Y[NB_ARROW_POINT] = {  -5,  -5, -10,   0,  10,  5, 5 };

// Change this to have more samples per display.
static const MIL_INT MAX_SAMPLES_PER_GROUP = 1;

// The point size of the scale 1:1 display.
static const MIL_DOUBLE DEFAULT_TITLE_POINT_SIZE = 20;
static const MIL_DOUBLE DEFAULT_TEXT_POINT_SIZE  = 14;

// The font to used for the title.
static MIL_CONST_TEXT_PTR TITLE_FONT = M_FONT_DEFAULT_TTF;
static MIL_CONST_TEXT_PTR TEXT_FONT = M_FONT_DEFAULT_TTF;

//*****************************************************************************
// Mouse Callbacks
//*****************************************************************************
MIL_INT MFTYPE MouseMoveFunc(MIL_INT HookType, MIL_ID EventID, void* UserDataPtr)
   {
   CExampleMngr *pExampleMngr = (CExampleMngr*) UserDataPtr;

   // Get the mouse position.
   MIL_DOUBLE PosX, PosY;
   MdispGetHookInfo(EventID, M_MOUSE_POSITION_BUFFER_X,  &PosX);
   MdispGetHookInfo(EventID, M_MOUSE_POSITION_BUFFER_Y,  &PosY);

   // Select the image as the hovered.
   pExampleMngr->SetHoverImageAt(PosX, PosY);

   return 0;
   }

MIL_INT MFTYPE MouseLeftClickFunc(MIL_INT HookType, MIL_ID EventID, void* UserDataPtr)
   {
   CExampleMngr *pExampleMngr = (CExampleMngr*) UserDataPtr;

   // Get the mouse position.
   MIL_DOUBLE PosX, PosY;
   MdispGetHookInfo(EventID, M_MOUSE_POSITION_BUFFER_X,  &PosX);
   MdispGetHookInfo(EventID, M_MOUSE_POSITION_BUFFER_Y,  &PosY);

   // Select the the image as the currently selected.
   pExampleMngr->SetSelectedImageAt(PosX, PosY);

   return 0;
   }

//*****************************************************************************
// Constructor.
//*****************************************************************************
CExampleMngr::CExampleMngr(MIL_ID MilSystem)
 : m_MilSystem                 (MilSystem),
   m_MilDisplay                (M_NULL),
   m_MilGraList                (M_NULL),
   m_MilGraGraphicContext      (M_NULL),
   m_MilGraTextContext         (M_NULL),
   m_MilGraSingleContext       (M_NULL),
   m_MilGraSingleTextContext   (M_NULL),
   m_MilGraTitleContext        (M_NULL),
   m_MilViewChildArray         (NULL),
   m_MilDisplayImage           (M_NULL),
   m_MilSingleDisplay          (M_NULL),
   m_MilSingleDisplayChild     (M_NULL),
   m_NbSamplesToInspect        (0),
   m_CurProductInfo            (NULL),
   m_HoverLabel                (0),
   m_SelectedLabel             (0),
   m_CurrentGroupIdx           (0)
   {
   // Allocate the display.
   MdispAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &m_MilDisplay);
   MdispControl(m_MilDisplay, M_SCALE_DISPLAY, M_ENABLE);

   // Allocate a single display.
   MdispAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &m_MilSingleDisplay);
   
   // Allocate a graphics list to be associated on the display.
   MgraAllocList(MilSystem, M_DEFAULT, &m_MilGraList);
   MgraAllocList(MilSystem, M_DEFAULT, &m_MilSingleGraList);
   MdispControl(m_MilDisplay, M_ASSOCIATED_GRAPHIC_LIST_ID, m_MilGraList);
   MdispControl(m_MilSingleDisplay, M_ASSOCIATED_GRAPHIC_LIST_ID, m_MilSingleGraList);

   // Allocate a graphic context for the graphic results annotations.
   MgraAlloc(MilSystem, &m_MilGraGraphicContext);
   MgraAlloc(MilSystem, &m_MilGraSingleContext);

   // Allocate a graphic context for the text annotation.
   MgraAlloc(MilSystem, &m_MilGraTextContext);
   MgraAlloc(MilSystem, &m_MilGraSingleTextContext);
   MgraAlloc(MilSystem, &m_MilGraTitleContext);

   // Set the text graphic context.
   MgraFont(m_MilGraTextContext, MIL_FONT_NAME(TEXT_FONT));
   
   MgraFont(m_MilGraSingleTextContext, MIL_FONT_NAME(TEXT_FONT));
   MgraControl(m_MilGraSingleTextContext, M_FONT_SIZE, DEFAULT_TEXT_POINT_SIZE);
   
   MgraColor(m_MilGraTitleContext, M_COLOR_DARK_GREEN);
   MgraFont(m_MilGraTitleContext, MIL_FONT_NAME(TEXT_FONT));
   MgraControl(m_MilGraTitleContext, M_TEXT_ALIGN_HORIZONTAL, M_CENTER);
   MgraControl(m_MilGraTitleContext, M_DRAW_OFFSET_Y, -SAMPLES_BORDER_Y);
   }

//*****************************************************************************
// Destructor.
//*****************************************************************************
CExampleMngr::~CExampleMngr()
   {
   // Reset the tasks.
   ResetTasks();

   // Reset the images.
   ResetImages();
   
   // Free the graphic contexts.
   MgraFree(m_MilGraGraphicContext);
   MgraFree(m_MilGraSingleContext);
   MgraFree(m_MilGraTextContext);
   MgraFree(m_MilGraSingleTextContext);
   MgraFree(m_MilGraTitleContext);

   // Free the graphics lists.
   MgraFree(m_MilGraList);
   MgraFree(m_MilSingleGraList);
   
   // Free the displays.
   MdispFree(m_MilDisplay);
   MdispFree(m_MilSingleDisplay);
   }

//*****************************************************************************
// Main run function.
//*****************************************************************************
void CExampleMngr::Run(SProductInfo* ProductsInfoList, MIL_INT NbProduct)
   {
   // For each types of product.
   for(MIL_INT ProductIdx = 0; ProductIdx < NbProduct; ProductIdx++)
      {
      // Reset the inspection for the new product.
      ResetInspection(ProductsInfoList[ProductIdx]);

      for(MIL_INT GroupIdx = 0, SampleIdx =0; GroupIdx < GetNbGroups(); GroupIdx++)
         {
         // Load the groups images.
         LoadGroupImages(GroupIdx);
                  
         for(MIL_INT DispSampleIdx = 0; DispSampleIdx < GetNbSamples(GroupIdx); DispSampleIdx++, SampleIdx++)
            {
            // Disable display updates.
            MdispControl(m_MilDisplay, M_UPDATE, M_DISABLE);                  

            // Print message.
            MosPrintf(MIL_TEXT("Inspecting sample %i.\n\n"), SampleIdx);

            // Inspect the products.
            InspectProducts(DispSampleIdx);

            // Draw the results.
            DrawResults(DispSampleIdx);

            // Print message.
            MosPrintf(MIL_TEXT("The results of sample %i are displayed.\n\n"), SampleIdx);

            // Enable the display updates.
            MdispControl(m_MilDisplay, M_UPDATE, M_ENABLE);
            }         

         // Enable the display hooks.
         EnableDisplayHooks();

         // Wait for the user input.
         MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
         MosGetch();

         // Disable the display hooks.
         DisableDisplayHooks();
         }
      }
   }

//*****************************************************************************
// Function that resets the images.
//*****************************************************************************
void CExampleMngr::ResetImages()
   {
   if(m_MilDisplayImage)
      {
      for(MIL_INT SampleIdx = 0; SampleIdx < m_NbSamplesPerGroup; SampleIdx++)
         {
         // Free all the childs.
         for(MIL_INT ViewIdx = 0; ViewIdx < m_CurProductInfo->NbViews; ViewIdx++)
            {
            MbufFree(m_MilViewChildArray[SampleIdx][ViewIdx].MilChild);
            MgraFree(m_MilViewChildArray[SampleIdx][ViewIdx].MilGraList);
            MgraFree(m_MilViewChildArray[SampleIdx][ViewIdx].MilGraTextList);
            }

         // Delete the child array.
         delete [] m_MilViewChildArray[SampleIdx];            
         }      

      // Delete the pointers to the arrays.
      delete [] m_MilViewChildArray;

      // Free the displays images.
      MbufFree(m_MilDisplayImage);
      m_MilDisplayImage = M_NULL;
      MbufFree(m_MilSingleDisplayChild);
      MbufFree(m_MilSingleDisplayImage);
      }
   }

//*****************************************************************************
// Function that resets the tasks.
//*****************************************************************************
void CExampleMngr::ResetTasks()
   {
   if(m_CurProductInfo)
      {
      for(MIL_INT ViewIdx = 0; ViewIdx < m_CurProductInfo->NbViews; ViewIdx++)
         {
         // Get a reference to the current view task list.
         SImageTaskList &rViewTaskList = m_CurProductInfo->ImageTasksListArray[ViewIdx];

         // Deallocates the tasks. Do it in reverse order to free the child before the parents.
         for(MIL_INT TaskIdx = rViewTaskList.NbTasks-1; TaskIdx >=0 ; TaskIdx--)
            { rViewTaskList.TaskList[TaskIdx]->Free(); }
         }
      }
   }

//*****************************************************************************
// Function that initializes the tasks.
//*****************************************************************************
void CExampleMngr::InitializeTasks()
   {
   // Initialize the tasks.
   for(MIL_INT ViewIdx = 0; ViewIdx < m_CurProductInfo->NbViews; ViewIdx++)
      {
      // Get a reference to the current view task list.
      SImageTaskList &rViewTaskList = m_CurProductInfo->ImageTasksListArray[ViewIdx];

      // Initialize the task.
      for(MIL_INT TaskIdx = 0; TaskIdx < rViewTaskList.NbTasks; TaskIdx++)
         { rViewTaskList.TaskList[TaskIdx]->Init(m_MilSystem, m_MilViewChildArray[0][ViewIdx].SizeX, m_MilViewChildArray[0][ViewIdx].SizeY); }
      }
   }

//*****************************************************************************
// Function that initialize the display images.
//*****************************************************************************
void CExampleMngr::InitializeDisplayImages()
   {
   // Get the number of products to inspect.
   m_NbSamplesToInspect = MbufDiskInquire(m_CurProductInfo->ViewAviFilePathArray[0], M_NUMBER_OF_IMAGES, M_NULL);

   // Reset the size of the display grid.
   m_DisplaySizeX = VIEW_BORDER_X;
   m_DisplayGridSizeY = 0; 
   m_DisplayGridTextSizeY = 0;
   m_DisplayGridImageSizeY = 0;

   // Calculate the parameters of the display.
   for(MIL_INT ViewIdx = 0; ViewIdx < m_CurProductInfo->NbViews; ViewIdx++)
      {
      // Calculate the size of the text for the view.
      MIL_INT TextSizeY = m_CurProductInfo->ImageTasksListArray[ViewIdx].NbTasks * RESULT_TEXT_LINE_SPACING_Y;

      // Get the current size of the view.
      MIL_INT CurSizeX = MbufDiskInquire(m_CurProductInfo->ViewAviFilePathArray[ViewIdx], M_SIZE_X, M_NULL);
      MIL_INT CurSizeY = MbufDiskInquire(m_CurProductInfo->ViewAviFilePathArray[ViewIdx], M_SIZE_Y, M_NULL);

      // Update the display size info.
      m_DisplayGridTextSizeY  = (TextSizeY > m_DisplayGridTextSizeY) ? TextSizeY : m_DisplayGridTextSizeY ;
      m_DisplayGridImageSizeY = (CurSizeY > m_DisplayGridImageSizeY) ? CurSizeY  : m_DisplayGridImageSizeY;
      m_DisplaySizeX          += CurSizeX + VIEW_BORDER_X;
      }

   // Set the size of the grid in Y.
   m_DisplayGridSizeY = m_DisplayGridTextSizeY + m_DisplayGridImageSizeY + 2 * SAMPLES_BORDER_Y;

   // Calculate the nb of samples per group processed.
   m_NbSamplesPerGroup = (MIL_INT)((MIL_DOUBLE)(MAX_DISPLAY_SIZE_Y-SAMPLES_BORDER_Y) / (m_DisplayGridSizeY));
   m_NbSamplesPerGroup = m_NbSamplesPerGroup > m_NbSamplesToInspect ? m_NbSamplesToInspect : m_NbSamplesPerGroup;
   m_NbSamplesPerGroup = m_NbSamplesPerGroup > MAX_SAMPLES_PER_GROUP ? MAX_SAMPLES_PER_GROUP : m_NbSamplesPerGroup;
   if(m_NbSamplesPerGroup == 0)
      { m_NbSamplesPerGroup = 1; }

   // Allocate the number samples.
   m_MilViewChildArray = new SViewChildInfo*[m_NbSamplesPerGroup];

   // Set the display size Y.
   m_DisplaySizeY = (m_DisplayGridSizeY * m_NbSamplesPerGroup) + 2 * SAMPLES_BORDER_Y + TITLE_LINE_SPACING_Y;

   // Allocate the display image.
   MbufAllocColor(m_MilSystem, 3, m_DisplaySizeX, m_DisplaySizeY, 8+M_UNSIGNED, M_IMAGE + M_PROC + M_DISP, &m_MilDisplayImage);
   MbufClear(m_MilDisplayImage, M_COLOR_BLACK);
   
   MbufAllocColor(m_MilSystem, 3, m_DisplaySizeX, m_DisplaySizeY, 8+M_UNSIGNED, M_IMAGE + M_PROC + M_DISP, &m_MilSingleDisplayImage);
   MbufClear(m_MilSingleDisplayImage, M_COLOR_BLACK);
   MbufChild2d(m_MilSingleDisplayImage, 0, 0, 1, 1, &m_MilSingleDisplayChild);
   
   // For all the childs set the offset and allocate the child.
   for(MIL_INT SampleIdx = 0, CurrentOffsetY = 2 * SAMPLES_BORDER_Y + TITLE_LINE_SPACING_Y; SampleIdx < m_NbSamplesPerGroup; SampleIdx++, CurrentOffsetY += m_DisplayGridSizeY)
      {
      // Allocate the child info structures.
      m_MilViewChildArray[SampleIdx] = new SViewChildInfo[m_CurProductInfo->NbViews];

      for(MIL_INT ViewIdx = 0, CurrentOffsetX = VIEW_BORDER_X; ViewIdx < m_CurProductInfo->NbViews; ViewIdx++)
         {  
         // Get the current size of the view.
         MIL_INT CurSizeX = MbufDiskInquire(m_CurProductInfo->ViewAviFilePathArray[ViewIdx], M_SIZE_X, M_NULL);
         MIL_INT CurSizeY = MbufDiskInquire(m_CurProductInfo->ViewAviFilePathArray[ViewIdx], M_SIZE_Y, M_NULL);

         // Get a reference to the structure.
         SViewChildInfo &rChildInfo = m_MilViewChildArray[SampleIdx][ViewIdx];

         // Set the child information and allocate it.
         rChildInfo.SizeX   = CurSizeX;
         rChildInfo.SizeY   = CurSizeY;
         rChildInfo.OffsetX = CurrentOffsetX;
         rChildInfo.OffsetY = CurrentOffsetY;
         rChildInfo.RealOffsetY = CurrentOffsetY + (m_DisplayGridImageSizeY - CurSizeY)/2;
         CurrentOffsetX += CurSizeX + VIEW_BORDER_X; 
         MbufChild2d(m_MilDisplayImage, rChildInfo.OffsetX, rChildInfo.RealOffsetY, rChildInfo.SizeX, rChildInfo.SizeY, &rChildInfo.MilChild);
         MgraAllocList(m_MilSystem, M_DEFAULT, &rChildInfo.MilGraList);
         MgraAllocList(m_MilSystem, M_DEFAULT, &rChildInfo.MilGraTextList);
         }
      }
   }

//*****************************************************************************
// Function that sets the rectangle of the views.
//*****************************************************************************
void CExampleMngr::SetImageRect()
   {
   for(MIL_INT SampleIdx = 0; SampleIdx < GetNbSamples(m_CurrentGroupIdx); SampleIdx++)
      {
      for(MIL_INT ViewIdx = 0; ViewIdx < m_CurProductInfo->NbViews; ViewIdx++)
         {  
         // Get a reference to the structure.
         SViewChildInfo &rChildInfo = m_MilViewChildArray[SampleIdx][ViewIdx];

         // Draw the rectangle in the graphics list.
         MgraColor(M_DEFAULT, M_COLOR_WHITE);
         MgraControl(M_DEFAULT, M_DRAW_OFFSET_X, 0.0);
         MgraControl(M_DEFAULT, M_DRAW_OFFSET_Y, 0.0);
         MgraRect(M_DEFAULT, m_MilGraList, rChildInfo.OffsetX, rChildInfo.RealOffsetY, rChildInfo.OffsetX + rChildInfo.SizeX, rChildInfo.RealOffsetY + rChildInfo.SizeY);
         MgraInquireList(m_MilGraList, M_LIST, M_DEFAULT, M_LAST_LABEL + M_TYPE_MIL_INT, &rChildInfo.GraRectLabel);
         }
      }
   }

//*****************************************************************************
// Function that loads the group images.
//*****************************************************************************
void CExampleMngr::LoadGroupImages(MIL_INT GroupIdx)
   {
   // Enable the display updates.
   MdispControl(m_MilDisplay, M_UPDATE, M_DISABLE);

   // Clear the display.
   MbufClear(m_MilDisplayImage, 0);
   MbufClear(m_MilSingleDisplayImage, 0);
   MgraClear(M_DEFAULT, m_MilGraList);
   
   // Draw the views titles.
   DrawViewsTitles();

   // Reset the hover and selected labels.
   m_HoverLabel = -1;
   m_SelectedLabel = -1;

   // Clear the single graphics list.
   for(MIL_INT ViewIdx = 0; ViewIdx < m_CurProductInfo->NbViews; ViewIdx++)
      {
      for(MIL_INT DispSampleIdx = 0; DispSampleIdx < m_NbSamplesPerGroup ; DispSampleIdx++)
         {
         MgraClear(M_DEFAULT, m_MilViewChildArray[DispSampleIdx][ViewIdx].MilGraList);
         MgraClear(M_DEFAULT, m_MilViewChildArray[DispSampleIdx][ViewIdx].MilGraTextList);
         }
      }

   // Set the current group index.
   m_CurrentGroupIdx = GroupIdx;

   // Set the images rectangles.
   SetImageRect();

   for(MIL_INT ViewIdx = 0; ViewIdx < m_CurProductInfo->NbViews; ViewIdx++)
      {
      // Open the sequence.
      MbufImportSequence(m_CurProductInfo->ViewAviFilePathArray[ViewIdx], M_DEFAULT, M_NULL, M_NULL, M_NULL, M_NULL, M_NULL, M_OPEN);

      for(MIL_INT DispSampleIdx = 0, SampleIdx = m_NbSamplesPerGroup * GroupIdx; DispSampleIdx < m_NbSamplesPerGroup && SampleIdx < m_NbSamplesToInspect; DispSampleIdx++, SampleIdx++)
         {
         // Get the image from the sequence.
         MbufImportSequence(m_CurProductInfo->ViewAviFilePathArray[ViewIdx], M_DEFAULT, M_LOAD, M_NULL, &m_MilViewChildArray[DispSampleIdx][ViewIdx].MilChild, SampleIdx, 1, M_READ);
         } 

      // Close the sequence.
      MbufImportSequence(m_CurProductInfo->ViewAviFilePathArray[ViewIdx], M_DEFAULT, M_NULL, M_NULL, M_NULL, M_NULL, M_NULL, M_CLOSE);
      }

   // Enable the display updates.
   MdispControl(m_MilDisplay, M_UPDATE, M_DISABLE);
   }

//*****************************************************************************
// Function that resets the inspection using a new product information.
//*****************************************************************************
void CExampleMngr::ResetInspection(SProductInfo &ProductInfo)
   {
   // Print message.
   MosPrintf(MIL_TEXT("Setting up the inspection of the product...\n\n"));

   // Reset the previously used tasks.
   ResetTasks();

   // Reset the images.
   ResetImages();

   // Get a pointer to the new product info.
   m_CurProductInfo = &ProductInfo;

   // Initialize the display.
   InitializeDisplayImages();

   // Select the display image on the display.
   MdispSelect(m_MilDisplay, m_MilDisplayImage);

   // Calculate the display scale factor.
   CalculateDisplayScaleFactor();   

   // Initialize the tasks.
   InitializeTasks();
   }

//*****************************************************************************
// Function that resets the inspection using a new product information.
//*****************************************************************************
void CExampleMngr::EnableDisplayHooks()
   {
   // Put the callbacks on the display.
   MdispHookFunction(m_MilDisplay, M_MOUSE_MOVE, MouseMoveFunc, (void*)this);
   MdispHookFunction(m_MilDisplay, M_MOUSE_LEFT_BUTTON_UP, MouseLeftClickFunc, (void*)this);
   }

//*****************************************************************************
// Function that resets the inspection using a new product information.
//*****************************************************************************
void CExampleMngr::DisableDisplayHooks()
   {
   // Remove the callbacks on the display.
   MdispHookFunction(m_MilDisplay, M_MOUSE_MOVE + M_UNHOOK, MouseMoveFunc, (void*)this);
   MdispHookFunction(m_MilDisplay, M_MOUSE_LEFT_BUTTON_UP + M_UNHOOK, MouseLeftClickFunc, (void*)this);

   // Deselect the single display.
   if(MdispInquire(m_MilSingleDisplay, M_SELECTED, M_NULL))
      { MdispSelect(m_MilSingleDisplay, M_NULL); }
   }

//*****************************************************************************
// Function that inspects the products.
//*****************************************************************************
void CExampleMngr::InspectProducts(MIL_INT SampleIdx)
   {
   for(MIL_INT ViewIdx = 0; ViewIdx < m_CurProductInfo->NbViews; ViewIdx++)
      {
      // Get a reference to the task list.
      SImageTaskList &rTaskList = m_CurProductInfo->ImageTasksListArray[ViewIdx];

      // Perform all the task for the given product.
      for(MIL_INT TaskIdx = 0; TaskIdx < rTaskList.NbTasks; TaskIdx++)
         rTaskList.TaskList[TaskIdx]->InspectImage(m_MilViewChildArray[SampleIdx][ViewIdx].MilChild);
      }
   }

//*****************************************************************************
// Function that draws the results.
//*****************************************************************************
void CExampleMngr::DrawResults(MIL_INT SampleIdx)
   {
   for(MIL_INT ViewIdx = 0; ViewIdx < m_CurProductInfo->NbViews; ViewIdx++)
      {
      // Get a reference to the task list.
      SImageTaskList &rTaskList = m_CurProductInfo->ImageTasksListArray[ViewIdx];

      // Get a reference to the child info structure.
      SViewChildInfo &rChildInfo = m_MilViewChildArray[SampleIdx][ViewIdx];

      // Set the text offset and the point size according to the size of the display.
      MgraControl(m_MilGraTextContext, M_FONT_SIZE, DEFAULT_TEXT_POINT_SIZE * m_DisplayScaleFactor);
      MgraControl(m_MilGraTextContext, M_DRAW_OFFSET_X, (MIL_DOUBLE)-(rChildInfo.OffsetX + 5));
      MgraControl(m_MilGraTextContext, M_DRAW_OFFSET_Y, (MIL_DOUBLE)-(rChildInfo.RealOffsetY + rChildInfo.SizeY + SAMPLES_BORDER_Y));

      // Set the text offset.
      MgraControl(m_MilGraSingleTextContext, M_DRAW_OFFSET_X, -5.0);
      MgraControl(m_MilGraSingleTextContext, M_DRAW_OFFSET_Y, (MIL_DOUBLE)-(rChildInfo.SizeY + SAMPLES_BORDER_Y));

      // Set the drawing offset of the graphic context.
      MgraControl(m_MilGraGraphicContext, M_DRAW_OFFSET_X, (MIL_DOUBLE)(-rChildInfo.OffsetX));
      MgraControl(m_MilGraGraphicContext, M_DRAW_OFFSET_Y, (MIL_DOUBLE)(-rChildInfo.RealOffsetY));

      // Set the drawing offset of the graphic context.
      MgraControl(m_MilGraSingleContext, M_DRAW_OFFSET_X, 0.0);
      MgraControl(m_MilGraSingleContext, M_DRAW_OFFSET_Y, 0.0);

      // Draw the graphical results of all the tasks.
      for(MIL_INT TaskIdx = 0; TaskIdx < rTaskList.NbTasks; TaskIdx++)
         {
         // Draw the graphical result.
         rTaskList.TaskList[TaskIdx]->DrawInspectionGraphicalResult(m_MilGraGraphicContext, m_MilGraList);
         rTaskList.TaskList[TaskIdx]->DrawInspectionGraphicalResult(m_MilGraSingleContext, rChildInfo.MilGraList);

         // Draw the text result in the graphics list.
         rTaskList.TaskList[TaskIdx]->DrawTextResult(m_MilGraTextContext, m_MilGraList);
         rTaskList.TaskList[TaskIdx]->DrawTextResult(m_MilGraSingleTextContext, rChildInfo.MilGraTextList);
         }

      // Draw the arrow if necessary.
      if(m_CurProductInfo->ViewsType == enSequential && ViewIdx < m_CurProductInfo->NbViews-1)
         {
         MgraControl(M_DEFAULT, M_DRAW_OFFSET_X, (MIL_DOUBLE)-(rChildInfo.OffsetX + rChildInfo.SizeX));
         MgraControl(M_DEFAULT, M_DRAW_OFFSET_Y, (MIL_DOUBLE)-(rChildInfo.RealOffsetY + rChildInfo.SizeY / 2));
         DrawArrow();
         }
      }
   }

//*****************************************************************************
// Function that draws the arrow between the views.
//*****************************************************************************
void CExampleMngr::DrawArrow()
   {
   MgraColor(M_DEFAULT, M_COLOR_DARK_GREEN);
   MgraLines(M_DEFAULT, m_MilGraList, NB_ARROW_POINT, ARROW_X, ARROW_Y, M_NULL, M_NULL, M_POLYGON + M_FILLED);
   }

//*****************************************************************************
// Function that draws the title of each view.
//*****************************************************************************
void CExampleMngr::DrawViewsTitles()
   {
   // Set the graphic context to draw the title text.
   MgraControl(m_MilGraTitleContext, M_FONT_SIZE, (MIL_DOUBLE)DEFAULT_TITLE_POINT_SIZE * m_DisplayScaleFactor);
   for(MIL_INT ViewIdx = 0; ViewIdx < m_CurProductInfo->NbViews; ViewIdx++)
      {  
      // Get a reference to the structure.
      SViewChildInfo &rChildInfo = m_MilViewChildArray[0][ViewIdx];

      // Draw the title of the inspection.
      MgraText(m_MilGraTitleContext, m_MilGraList, rChildInfo.OffsetX + rChildInfo.SizeX/2, 0, m_CurProductInfo->ImageTasksListArray[ViewIdx].TaskTitle);
      }
   }

//*****************************************************************************
// Function that calculates the display scale factor.
//*****************************************************************************
void CExampleMngr::CalculateDisplayScaleFactor()
   {
   MIL_INT DisplayAreaSizeX = MdispInquire(m_MilDisplay, M_SIZE_X, M_NULL);
   MIL_INT DisplayAreaSizeY = MdispInquire(m_MilDisplay, M_SIZE_Y, M_NULL);
   
   if(DisplayAreaSizeX == m_DisplaySizeX)
      m_DisplayScaleFactor = (MIL_DOUBLE)DisplayAreaSizeY / m_DisplaySizeY;
   else
      m_DisplayScaleFactor = (MIL_DOUBLE)DisplayAreaSizeX / m_DisplaySizeX;
   }

//*****************************************************************************
// Function that gets the number of groups of a given product.
//*****************************************************************************
MIL_INT CExampleMngr::GetNbGroups() const
   {
   // Gets the number of groups of a given product.
   return (MIL_INT)ceil((MIL_DOUBLE)m_NbSamplesToInspect / m_NbSamplesPerGroup);
   }

//*****************************************************************************
// Function that gets the number of samples of a given group.
//*****************************************************************************
MIL_INT CExampleMngr::GetNbSamples(MIL_INT GroupIdx) const
   {
   MIL_INT NbSamplesLeft = m_NbSamplesToInspect - GroupIdx * m_NbSamplesPerGroup;

   // Gets the number of samples for a given group.
   return NbSamplesLeft < m_NbSamplesPerGroup ? NbSamplesLeft : m_NbSamplesPerGroup;
   }

//*****************************************************************************
// Mouse interaction functions.
//*****************************************************************************
void CExampleMngr::SetHoverImageAt(MIL_DOUBLE PosX, MIL_DOUBLE PosY)
   {
   const SViewChildInfo *pViewChild = GetViewOfPos(PosX, PosY);
   if(pViewChild)
      {
      // Uncomment to display selected in green.
      /*if(pViewChild->GraRectLabel == m_SelectedLabel)
         ResetSpecialLabel(&m_HoverLabel, -1, M_COLOR_YELLOW);
      else */
         ResetSpecialLabel(&m_HoverLabel, pViewChild->GraRectLabel, M_COLOR_YELLOW);
      }
   else
      ResetSpecialLabel(&m_HoverLabel, -1, M_COLOR_YELLOW);
   }

void CExampleMngr::SetSelectedImageAt(MIL_DOUBLE PosX, MIL_DOUBLE PosY)
   {
   MIL_INT ViewIdx;
   const SViewChildInfo *pViewChild = GetViewOfPos(PosX, PosY, &ViewIdx);
   if(pViewChild)
      {
      // Update the display.
      UpdateSingleDisplay(pViewChild, ViewIdx);
      ResetSpecialLabel(&m_SelectedLabel, pViewChild->GraRectLabel, M_COLOR_WHITE);
      if(m_SelectedLabel == m_HoverLabel)
         m_HoverLabel = -1;
      }
   }

void CExampleMngr::ResetSpecialLabel(MIL_INT* pCurrentLabel, MIL_INT NewLabel, MIL_INT Color)
   {
   // If the label status has changed.
   if(*pCurrentLabel != NewLabel)
      {
      // If the current label exist, reset it to white.
      if(*pCurrentLabel != -1)
         MgraControlList(m_MilGraList, M_GRAPHIC_LABEL(*pCurrentLabel), M_DEFAULT, M_COLOR, M_COLOR_WHITE);

      // If the new label exists, put it to the new color.
      if(NewLabel != -1)
         MgraControlList(m_MilGraList, M_GRAPHIC_LABEL(NewLabel), M_DEFAULT, M_COLOR, Color);

      *pCurrentLabel = NewLabel;
      }
   }

const SViewChildInfo* CExampleMngr::GetViewOfPos(MIL_DOUBLE PosX, MIL_DOUBLE PosY, MIL_INT *pViewIdx /* = NULL */)
   {
   for(MIL_INT SampleIdx = 0; SampleIdx < GetNbSamples(m_CurrentGroupIdx); SampleIdx ++)
      {
      for(MIL_INT ViewIdx = 0; ViewIdx < m_CurProductInfo->NbViews; ViewIdx++)
         {
         SViewChildInfo &rChildInfo = m_MilViewChildArray[SampleIdx][ViewIdx];
         if((PosX >= rChildInfo.OffsetX)                             &&
            (PosX <= (rChildInfo.OffsetX + rChildInfo.SizeX - 1))    &&
            (PosY >= rChildInfo.RealOffsetY)                         &&
            (PosY <= (rChildInfo.RealOffsetY + rChildInfo.SizeY - 1)))
            {
            if(pViewIdx)
               { *pViewIdx = ViewIdx; }
            return &rChildInfo;
            }
         }
      }
   return NULL;
   }

//*****************************************************************************
// Function that updates the single display.
//*****************************************************************************
void CExampleMngr::UpdateSingleDisplay(const SViewChildInfo *pViewChild, MIL_INT ViewIdx)
   {
   // Disable the display updates.
   MdispControl(m_MilSingleDisplay, M_UPDATE, M_DISABLE);

   // Deselect the image.
   if(MdispInquire(m_MilSingleDisplay, M_SELECTED, M_NULL))
      { MdispSelect(m_MilSingleDisplay, M_NULL); }

   // Move the child.
   MbufChildMove(m_MilSingleDisplayChild, pViewChild->OffsetX, pViewChild->RealOffsetY, pViewChild->SizeX,  pViewChild->SizeY + m_DisplayGridTextSizeY + SAMPLES_BORDER_Y, M_DEFAULT);

   // Copy the child.
   MbufCopy(pViewChild->MilChild, m_MilSingleDisplayChild);

   // Set the graphics list.
   MdispControl(m_MilSingleDisplay, M_ASSOCIATED_GRAPHIC_LIST_ID, pViewChild->MilGraList);

   // Draw the text in the child.
   MgraDraw(pViewChild->MilGraTextList, m_MilSingleDisplayChild, M_DEFAULT);

   // Select the child.
   MdispSelect(m_MilSingleDisplay, m_MilSingleDisplayChild);
  
   // Enable the display updates.
   MdispControl(m_MilSingleDisplay, M_UPDATE, M_ENABLE);
   }
