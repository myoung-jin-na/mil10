//***************************************************************************************
// 
// File name: ExampleManager.h
//
// Synopsis: This file contains the declaration of the CExampleMngr class that
//           manages the different inspection tasks defined from a specific example.
//

#ifndef EXAMPLE_MANAGER_H
#define EXAMPLE_MANAGER_H

// Forward declares.
class CInspectionTask;

// Structure containing a list of all the task to perform.
struct SImageTaskList
   {
   MIL_INT              NbTasks;
   MIL_CONST_TEXT_PTR   TaskTitle;
   CInspectionTask**    TaskList;
   };

// Macro that creates a SImageTaskList.
#define CREATE_LIST(TaskPtrList, Title) { ARRAY_COUNT(TaskPtrList), Title, TaskPtrList }

// Enum describing the views of a given product.
enum ViewsTypeEnum
   {
   enParallel = 0,
   enSequential
   };

// Structure containing all the information concerning the inspection of a 
// product.
struct SProductInfo
   {
   MIL_INT             NbViews;
   ViewsTypeEnum       ViewsType;
   MIL_CONST_TEXT_PTR* ViewAviFilePathArray;
   SImageTaskList*     ImageTasksListArray;
   };

// Structure containing all the information concerning a view child.
struct SViewChildInfo
   {
   MIL_ID  MilChild;
   MIL_ID  MilGraList;
   MIL_ID  MilGraTextList;
   MIL_INT GraRectLabel;
   MIL_INT OffsetX;
   MIL_INT OffsetY;
   MIL_INT RealOffsetY;
   MIL_INT SizeX;
   MIL_INT SizeY;
   };

class CExampleMngr
   {
   public:

      // Constructor.
      CExampleMngr(MIL_ID MilSystem);

      // Destructor.
      virtual ~CExampleMngr();

      // Function to run the example.
      void Run(SProductInfo* ProductsInfoList, MIL_INT NbProduct);

      // Function to reset to a new product inspection.
      void ResetInspection(SProductInfo &ProductInfo);
      
      // Function to load the images of one group.
      void LoadGroupImages(MIL_INT GroupIdx);
      
      // Function to inspect the images.
      void InspectProducts(MIL_INT SampleIdx);

      // Draw the graphical results .
      void DrawResults(MIL_INT SampleIdx);

      // Gets the number of groups of a given product.
      MIL_INT GetNbGroups() const;

      // Gets the number of samples of a given product.
      MIL_INT GetNbSamples(MIL_INT GroupIdx) const;

      // Function to enable and disable the single display hooks.
      void EnableDisplayHooks();
      void DisableDisplayHooks();

      // Function to handle the mouse interactivity.
      void SetHoverImageAt(MIL_DOUBLE PosX, MIL_DOUBLE PosY);
      void SetSelectedImageAt(MIL_DOUBLE PosX, MIL_DOUBLE PosY);

   private:

      // Function that initializes the display.
      void InitializeDisplayImages();

      // Function that initializes the tasks.
      void InitializeTasks();

      // Functions that resets the images.
      void ResetImages();

      // Function that resets the tasks.
      void ResetTasks();

      // Function to draw the arrow in the display.
      void DrawArrow();

      // Function to draw the title of the views.
      void DrawViewsTitles();

      // Function that calculates the display scale factor.
      void CalculateDisplayScaleFactor();

      // Mouse interactivity related functions.
      const SViewChildInfo* GetViewOfPos(MIL_DOUBLE PosX, MIL_DOUBLE PosY, MIL_INT *pViewIdx = NULL);
      void ResetSpecialLabel(MIL_INT* pCurrentLabel, MIL_INT NewLabel, MIL_INT Color);
      void UpdateSingleDisplay(const SViewChildInfo *pViewChild, MIL_INT ViewIdx);

      // Function that sets the image rectangles.
      void SetImageRect();

      // The system identifier.
      MIL_ID m_MilSystem;

      // The display and its graphics list.
      MIL_ID m_MilDisplay; 
      MIL_ID m_MilGraList;
      MIL_ID m_MilGraGraphicContext;
      MIL_ID m_MilGraTextContext;
      MIL_ID m_MilGraTitleContext;
      
      // The display image.
      MIL_ID m_MilDisplayImage;
      
      // The single display.
      MIL_ID m_MilSingleDisplay;
      MIL_ID m_MilSingleGraList;
      MIL_ID m_MilGraSingleContext;
      MIL_ID m_MilGraSingleTextContext;
      
      // The single display image.
      MIL_ID m_MilSingleDisplayImage;
      MIL_ID m_MilSingleDisplayChild;      
            
      // The child images corresponding to all the inspection steps.
      SViewChildInfo** m_MilViewChildArray;

      // The current number of frames in the sequence.
      MIL_INT m_NbSamplesToInspect;  

      // The number of samples to process for a given display frame.
      MIL_INT m_NbSamplesPerGroup;

      // The index of the current group loaded.
      MIL_INT m_CurrentGroupIdx;

      // The size of a sample inspection line.
      MIL_INT m_DisplayGridSizeY;
      MIL_INT m_DisplayGridImageSizeY;
      MIL_INT m_DisplayGridTextSizeY;
      
      // The size of the display.
      MIL_INT m_DisplaySizeX;
      MIL_INT m_DisplaySizeY;

      // The scale factor of the display.
      MIL_DOUBLE m_DisplayScaleFactor;

      // The current product information used by the manager.
      SProductInfo *m_CurProductInfo;
      
      // The interactive display label.
      MIL_INT m_SelectedLabel;
      MIL_INT m_HoverLabel;
   };

#endif // EXAMPLE_MANAGER_H