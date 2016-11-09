//***************************************************************************************/
// 
// File name: Menu.cpp  
//
// Synopsis:  Implements the CMPMenu class.
//
#include "common.h"

//*****************************************************************************
// Constants.
//*****************************************************************************

//Button size
static const MIL_INT BUTTON_SIZE_X = 125;
static const MIL_INT BUTTON_SIZE_Y = 25;
static const MIL_INT SMALL_BUTTON_SIZE_X = 60;
static const MIL_INT LARGE_BUTTON_SIZE_X = 180;

//Button position
static const MIL_INT LINKED_BUTTON_GAP = 20;
static const MIL_INT BUTTON_GAP = 6;
static const MIL_INT BUTTON_START_X_POS = 240;
static const MIL_INT BUTTON_START_Y_POS = BUTTON_GAP;
static const MIL_INT SECTION_GAP = 16;

//Text position
static const MIL_INT TEXT_START_X = 15;
static const MIL_INT TEXT_OFFSET_Y = 8;

//Update information
static const MIL_INT UPDATE_INFO_TIME_DELAY = 1000;

//Menu 
static const MIL_INT MENU_SIZE_X = 650;
static const MIL_INT INITIAL_MENU_SIZE_Y = 520;
static const MIL_INT CORE_ROW_SIZE = BUTTON_SIZE_Y + BUTTON_GAP;
static const MIL_INT MAX_CORES_PER_ROW = 8;
static const MIL_DOUBLE BACKGROUND_COLOR = M_RGB888(255, 255, 255); 

//FPS information text 
static const MIL_DOUBLE INFO_COLOR = M_COLOR_DARK_BLUE;
static const MIL_INT INFO_TEXT_SIZE = 350;
static const MIL_INT TOTAL_FPS_GAP = 5;
static const MIL_INT PROC_FPS_SIZE = BUTTON_GAP*3;

//Other
static const MIL_INT APPLY_TO_ALL_THREADS_ADJUSTMENT = -2;
static const MIL_INT CORE_AFFINITY_GAP = BUTTON_GAP*5;


//*****************************************************************************
// Constructor. Allocates and intializes the menu.
//*****************************************************************************
CMPMenu::CMPMenu(MIL_INT ProcessingArraySize, CMPProcessing** Processing)
: m_ProcessingArraySize(ProcessingArraySize)
   {
   //Allocate memory to contain the processing objects
   m_Processing = new CMPProcessing*[m_ProcessingArraySize]; 

   //Get the number of cores available for the whole process
   MappInquireMp(M_DEFAULT, M_CORE_NUM_PROCESS, M_DEFAULT, M_NULL, &m_NumberOfProcessorCores);

   m_NumberOfProcessorCores = (m_NumberOfProcessorCores<=MAX_CORES)?m_NumberOfProcessorCores:MAX_CORES;

   //Get the cores that are available for the process
   MIL_INT CoreAffinityMaskArraySize;
   MappInquireMp(M_DEFAULT, M_CORE_AFFINITY_MASK_ARRAY_SIZE, M_DEFAULT, M_NULL, &CoreAffinityMaskArraySize);

   m_CoreAffinityMaskProcess = new MIL_UINT64[CoreAffinityMaskArraySize];

   MappInquireMp(M_DEFAULT, M_CORE_AFFINITY_MASK_PROCESS, M_DEFAULT, M_NULL, m_CoreAffinityMaskProcess);

   //Set the number of cores to 1 if MP is disabled in MILConfig.
   if (MappInquireMp(M_DEFAULT, M_MP_FORCED_DISABLE, M_DEFAULT, M_NULL, M_NULL)==M_YES) 
      m_NumberOfProcessorCores = 1;

   m_MPIsOff = (m_NumberOfProcessorCores<MIN_MP_CORES);

   MIL_INT NumCoreRows = (MIL_INT)ceil((MIL_DOUBLE)m_NumberOfProcessorCores/MAX_CORES_PER_ROW);
   m_MenuSizeY = INITIAL_MENU_SIZE_Y + ((NumCoreRows)*CORE_ROW_SIZE);

   //Initialize the number of memory banks
   m_NumMemoryBank = MappInquireMp(M_DEFAULT, M_MEMORY_BANK_NUM, M_DEFAULT, M_NULL, M_NULL);

   //Keep a copy of the processing object pointers
   for (MIL_INT i=0; i<m_ProcessingArraySize; i++)
      {
      m_Processing[i] = Processing[i];
      }

   m_CurrentProcessing = 0;
   m_CurrentProcessingNum = 0;
   m_UpdateInfoStarted = false;

   //Set the initial state of the processing objects and run them
   if (m_MPIsOff)
      {
      //If there is no MP, disable it in the thread and run the processing.
      m_Processing[m_CurrentProcessingNum]->StartThread();
      m_Processing[m_CurrentProcessingNum]->SetMP(false);

      //Select and run the display
      m_Processing[m_CurrentProcessingNum]->DisplaySelect();
      m_Processing[m_CurrentProcessingNum]->RunDisplay(true);

      m_Processing[m_CurrentProcessingNum]->Run();

      //There is one processing thead
      m_CurrentProcessingNum = 1;
      }
   else
      {
      MIL_INT RunningThreadsNum = 0;
      MIL_INT NumCoresAssigned = 0;

      //Initially we have a maximum of two threads if there are at least 4 cores.  Otherwise,
      //run one thread and assign the cores to the thread because MP requires a minumum of two.
      if (m_NumberOfProcessorCores < 4)
         {
         RunningThreadsNum = 1;
         NumCoresAssigned = m_NumberOfProcessorCores;
         }
      else
         {
         RunningThreadsNum = 2;
         NumCoresAssigned = m_NumberOfProcessorCores>>1;
         }

      for (m_CurrentProcessingNum=0; m_CurrentProcessingNum<RunningThreadsNum; m_CurrentProcessingNum++)
         {
         //Start the threads
         m_Processing[m_CurrentProcessingNum]->StartThread();

         //Initialize the state of MP and assign half of available cores to each processing thread
         InitProcessingMP(NumCoresAssigned, m_Processing[m_CurrentProcessingNum]);

         //Select and run the display
         m_Processing[m_CurrentProcessingNum]->DisplaySelect();
         m_Processing[m_CurrentProcessingNum]->RunDisplay(true);

         //Run the procesing
         m_Processing[m_CurrentProcessingNum]->Run();
         }
      }

   //Create the menu
   Create();

   //Start updating FPS info
   UpdateTotalFPS();
   UpdateCurProcFPS();

   for (MIL_INT i=0; i<m_ProcessingArraySize; i++)
      {
      //Start updating information on processing if any of the processing are running
      if (m_Processing[i]->IsRunning() && !m_UpdateInfoStarted)
         {
         StartUpdateInfo();
         }
      }

   //Update menu information so that it corresponds to the current processing state
   UpdateMenu();
   }

//*****************************************************************************
// Destructor. Frees the menu objects.
//*****************************************************************************
CMPMenu::~CMPMenu()
   {
   //Destroy the menu and free allocated memory
   Destroy();
   delete [] m_Processing;
   delete [] m_CoreAffinityMaskProcess;
   }

//*****************************************************************************
// Create.  Create the menu and show it.
//*****************************************************************************
void CMPMenu::Create()
   {
   //Allocate host system for the menu
   MsysAlloc(M_DEFAULT, M_SYSTEM_HOST, M_DEFAULT, M_DEFAULT, &m_MilMenuSystem);

   //Allocate MIL display for menu options
   MdispAlloc(m_MilMenuSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &m_MilMenuDisplay);

   //Allocate the buffer
   MbufAllocColor(m_MilMenuSystem, 3, MENU_SIZE_X, m_MenuSizeY, 8+M_UNSIGNED, M_IMAGE+M_PROC+M_DISP, &m_MilMenuBuffer);
   MbufClear(m_MilMenuBuffer, BACKGROUND_COLOR);

   //Setup graphics context for menu text
   MgraAlloc(m_MilMenuSystem, &m_MilMenuGraphicsContext);
   MgraControl(m_MilMenuGraphicsContext, M_BACKGROUND_MODE, M_TRANSPARENT);
   MgraColor(m_MilMenuGraphicsContext, INFO_COLOR);

   //Setup graphics context for showing frames per second on the menu
   MgraAlloc(m_MilMenuSystem, &m_MilInfoGraphicsContext);
   MgraControl(m_MilInfoGraphicsContext, M_BACKGROUND_MODE, M_TRANSPARENT);
   MgraControl(m_MilInfoGraphicsContext, M_FONT_SIZE, SMALL_FONT);
   MgraFont(m_MilInfoGraphicsContext, NORMAL_FONT_TYPE);
   MgraColor(m_MilInfoGraphicsContext, INFO_COLOR);

   //Setup the menu options
   CreateMenu(); 

   //Setup the display of the menu
   MdispControl(m_MilMenuDisplay, M_KEYBOARD_USE, M_DISABLE);
   MdispControl(m_MilMenuDisplay, M_MOUSE_USE, M_DISABLE);
   MdispControl(m_MilMenuDisplay, M_WINDOW_RESIZE, M_DISABLE);
#if M_MIL_USE_WINDOWS
   MdispControl(m_MilMenuDisplay, M_WINDOW_SYSBUTTON, M_DISABLE);
#endif
   MdispControl(m_MilMenuDisplay, M_TITLE, M_PTR_TO_DOUBLE(MIL_TEXT("Multiprocessing")));
   MdispControl(m_MilMenuDisplay, M_WINDOW_TITLE_BAR_CHANGE, M_DISABLE); 

   MdispControl(m_MilMenuDisplay, M_WINDOW_OVERLAP, M_DISABLE);

   //Allocate the thread that updates FPS
   MthrAlloc (m_MilMenuSystem, M_EVENT, M_NOT_SIGNALED+M_AUTO_RESET, M_NULL, M_NULL, &m_MilInfoEvents[enRun]);
   MthrAlloc (m_MilMenuSystem, M_EVENT, M_NOT_SIGNALED+M_AUTO_RESET, M_NULL, M_NULL, &m_MilInfoEvents[enKill]);

   MthrAlloc (m_MilMenuSystem, M_THREAD, M_DEFAULT, &UpdateInfoFunction, (void*)this, &m_MilInfoThread);

   MthrAlloc (m_MilMenuSystem, M_EVENT, M_NOT_SIGNALED+M_AUTO_RESET, M_NULL, M_NULL, &m_MilUpdateInfoStoppedEvent);
   }

//*****************************************************************************
// Destroy.  Destroy the allocated menu objects.
//*****************************************************************************
void CMPMenu::Destroy()
   {
   //Pause the processing
   for (MIL_INT i=0; i<m_ProcessingArraySize; i++)
      m_Processing[i]->Pause();

   //Stop and kill the update info thread
   StopUpdateInfo();
   MthrControl(m_MilInfoEvents[enKill], M_EVENT_SET, M_SIGNALED);
   MthrWait(m_MilInfoThread, M_THREAD_END_WAIT, M_NULL);

   //Free all menu objects
   MthrFree(m_MilInfoEvents[enRun]);
   MthrFree(m_MilInfoEvents[enKill]);
   MthrFree(m_MilInfoThread);
   MthrFree(m_MilUpdateInfoStoppedEvent);

   //Unhook the function from the menu display
   MdispHookFunction(m_MilMenuDisplay, M_MOUSE_LEFT_BUTTON_UP+M_UNHOOK, MenuHookFct, (void*)this);

   //Deselect the display
   MdispSelect(m_MilMenuDisplay, M_NULL);

   //Free the rest of the menu objects
   for (MIL_INT i=FIRST_INFO; i<=LAST_INFO; i++)
      {
      MbufFree(m_InfoRegions[i].MilRectBuffer);
      }

   for (MIL_INT i=FIRST_BUTTON; i<=LAST_BUTTON; i++)
      {
      m_Buttons[i].Destroy();
      }

   MbufFree(m_MilMenuBuffer);
   MdispFree(m_MilMenuDisplay);  
   MgraFree(m_MilInfoGraphicsContext);
   MgraFree(m_MilMenuGraphicsContext);
   MsysFree(m_MilMenuSystem);
   }

//******************************************************************************************
// SetupMenu.  Write the menu options in the buffer and associate the options to functions.
//******************************************************************************************
void CMPMenu::CreateMenu()
   {
   MIL_INT ButtonStartX = BUTTON_START_X_POS;
   MIL_INT ButtonStartY = BUTTON_START_Y_POS;
   
   //Create the section with general application settings such as number of threads
   //and whether or not to show the displays.
   CreateApplicationControlSection(ButtonStartX, ButtonStartY);

   //Create the section with thread selection and configuration
   CreateUserThreadsSection(ButtonStartX, ButtonStartY);

   //Create the section with MP configuration
   CreateMPSection(ButtonStartX, ButtonStartY);

   //Hook functions to mouse events
   MdispHookFunction(m_MilMenuDisplay, M_MOUSE_LEFT_BUTTON_UP, MenuHookFct, (void*)this);
   }

//******************************************************************************************
// CreateApplicationControlSection.  Setup the user thread control menu options.
//******************************************************************************************
void CMPMenu::CreateApplicationControlSection(MIL_INT& ButtonStartX, MIL_INT& ButtonStartY)
   {
   //Set the section heading
   MgraControl(m_MilMenuGraphicsContext, M_FONT_SIZE, LARGE_FONT);

   MgraFont(m_MilMenuGraphicsContext, M_FONT_DEFAULT);

   MappControl(M_DEFAULT, M_ERROR, M_PRINT_DISABLE);
   MgraFont(m_MilMenuGraphicsContext, BOLD_FONT_TYPE);
   MappControl(M_DEFAULT, M_ERROR, M_PRINT_ENABLE);

   MgraText(m_MilMenuGraphicsContext, m_MilMenuBuffer, TEXT_START_X, ButtonStartY, MIL_TEXT("Application Control"));

   MoveXY(ButtonStartX, ButtonStartY, 0, SECTION_GAP);

   //Create menu buttons
   MgraControl(m_MilMenuGraphicsContext, M_FONT_SIZE, SMALL_FONT);
   MgraFont(m_MilMenuGraphicsContext, M_FONT_DEFAULT);
   MgraFont(m_MilMenuGraphicsContext, NORMAL_FONT_TYPE);
   MgraText(m_MilMenuGraphicsContext, m_MilMenuBuffer, TEXT_START_X, ButtonStartY+TEXT_OFFSET_Y, 
      MIL_TEXT("Number of User Threads"));

   MIL_INT LinkedStartX = ButtonStartX;
   MIL_INT LinkedStartY = ButtonStartY;
   CreateButton(enTotalThreadsLeft, LinkedStartX, LinkedStartY, SMALL_BUTTON_SIZE_X, BUTTON_SIZE_Y, MIL_TEXT(" <<"), false);

   MoveXY(LinkedStartX, LinkedStartY, SMALL_BUTTON_SIZE_X+LINKED_BUTTON_GAP, 0);

   MIL_TEXT_CHAR ButtonText[STRING_SIZE]=MIL_TEXT("");
   CreateButton(enTotalThreads, LinkedStartX, LinkedStartY, SMALL_BUTTON_SIZE_X, BUTTON_SIZE_Y, 
      GetButtonText(m_CurrentProcessing, enTotalThreads, ButtonText), false);

   MoveXY(LinkedStartX, LinkedStartY, SMALL_BUTTON_SIZE_X+LINKED_BUTTON_GAP, 0);

   CreateButton(enTotalThreadsRight, LinkedStartX, ButtonStartY, SMALL_BUTTON_SIZE_X, BUTTON_SIZE_Y, MIL_TEXT(" >>"), false);

   MoveXY(ButtonStartX, ButtonStartY, 0, BUTTON_SIZE_Y+BUTTON_GAP);

   MgraText(m_MilMenuGraphicsContext, m_MilMenuBuffer, TEXT_START_X, ButtonStartY+TEXT_OFFSET_Y, MIL_TEXT("Show Displays"));

   CreateButton(enShowDisplay, ButtonStartX, ButtonStartY, BUTTON_SIZE_X, BUTTON_SIZE_Y, 
      GetButtonText(m_CurrentProcessing, enShowDisplay, ButtonText), true);

   MoveXY(ButtonStartX, ButtonStartY, 0, BUTTON_SIZE_Y+BUTTON_GAP);

   //Create region for total FPS
   m_InfoRegions[enTotalFPS].StartX = TEXT_START_X;
   m_InfoRegions[enTotalFPS].StartY = ButtonStartY + TOTAL_FPS_GAP-1;
   m_InfoRegions[enTotalFPS].EndX = m_InfoRegions[enTotalFPS].StartX + INFO_TEXT_SIZE-1;
   m_InfoRegions[enTotalFPS].EndY = m_InfoRegions[enTotalFPS].StartY + PROC_FPS_SIZE-1;

   MbufChild2d(m_MilMenuBuffer, 
               m_InfoRegions[enTotalFPS].StartX, m_InfoRegions[enTotalFPS].StartY, 
               m_InfoRegions[enTotalFPS].EndX-m_InfoRegions[enTotalFPS].StartX+1, 
               m_InfoRegions[enTotalFPS].EndY-m_InfoRegions[enTotalFPS].StartY+1, 
               &m_InfoRegions[enTotalFPS].MilRectBuffer);

   ButtonStartY = m_InfoRegions[enTotalFPS].EndY + TOTAL_FPS_GAP-1;

   MgraRectFill(m_MilMenuGraphicsContext, m_MilMenuBuffer, TEXT_START_X, ButtonStartY, MENU_SIZE_X-TEXT_START_X, ButtonStartY+2);

   MoveXY(ButtonStartX, ButtonStartY, 0, SECTION_GAP);
   }


//******************************************************************************************
// CreateUserThreadsSection.  Setup the menu options related to the main processing thread.
//******************************************************************************************
void CMPMenu::CreateUserThreadsSection(MIL_INT& ButtonStartX, MIL_INT& ButtonStartY)
   {
   //Set the section heading
   MgraControl(m_MilMenuGraphicsContext, M_FONT_SIZE, LARGE_FONT);

   MgraFont(m_MilMenuGraphicsContext, M_FONT_DEFAULT);

   MappControl(M_DEFAULT, M_ERROR, M_PRINT_DISABLE);
   MgraFont(m_MilMenuGraphicsContext, BOLD_FONT_TYPE);
   MappControl(M_DEFAULT, M_ERROR, M_PRINT_ENABLE);

   MgraText(m_MilMenuGraphicsContext, m_MilMenuBuffer, TEXT_START_X, ButtonStartY, MIL_TEXT("User Thread Selection"));

   MoveXY(ButtonStartX, ButtonStartY, 0, SECTION_GAP);

   //Create menu buttons
   MgraControl(m_MilMenuGraphicsContext, M_FONT_SIZE, SMALL_FONT);
   MgraFont(m_MilMenuGraphicsContext, M_FONT_DEFAULT);
   MgraFont(m_MilMenuGraphicsContext, NORMAL_FONT_TYPE);
   MgraText(m_MilMenuGraphicsContext, m_MilMenuBuffer, TEXT_START_X, ButtonStartY+TEXT_OFFSET_Y, MIL_TEXT("Thread Index"));

   MIL_INT LinkedStartX = ButtonStartX;
   MIL_INT LinkedStartY = ButtonStartY;
   CreateButton(enProcThreadLeft, LinkedStartX, LinkedStartY, SMALL_BUTTON_SIZE_X, BUTTON_SIZE_Y, MIL_TEXT(" <<"), false);

   MoveXY(LinkedStartX, LinkedStartY, SMALL_BUTTON_SIZE_X+LINKED_BUTTON_GAP, 0);

   MIL_TEXT_CHAR ButtonText[STRING_SIZE]=MIL_TEXT("");
   CreateButton(enProcThread, LinkedStartX, LinkedStartY, SMALL_BUTTON_SIZE_X, BUTTON_SIZE_Y, 
      GetButtonText(m_CurrentProcessing, enProcThread, ButtonText), false);

   MoveXY(LinkedStartX, LinkedStartY, SMALL_BUTTON_SIZE_X+LINKED_BUTTON_GAP, 0);

   CreateButton(enProcThreadRight, LinkedStartX, ButtonStartY, SMALL_BUTTON_SIZE_X, BUTTON_SIZE_Y, MIL_TEXT(" >>"), false);

   MoveXY(LinkedStartX, ButtonStartY, 0, BUTTON_SIZE_Y+BUTTON_GAP);

   MgraRectFill(m_MilMenuGraphicsContext, m_MilMenuBuffer, TEXT_START_X, ButtonStartY, MENU_SIZE_X-TEXT_START_X, ButtonStartY+1);

   MoveXY(LinkedStartX, ButtonStartY, 0, BUTTON_GAP);

   MgraControl(m_MilMenuGraphicsContext, M_FONT_SIZE, LARGE_FONT);

   MgraFont(m_MilMenuGraphicsContext, M_FONT_DEFAULT);

   MappControl(M_DEFAULT, M_ERROR, M_PRINT_DISABLE);
   MgraFont(m_MilMenuGraphicsContext, BOLD_FONT_TYPE);
   MappControl(M_DEFAULT, M_ERROR, M_PRINT_ENABLE);

   MgraText(m_MilMenuGraphicsContext, m_MilMenuBuffer, TEXT_START_X, ButtonStartY, MIL_TEXT("Selected Thread Control"));

   MoveXY(ButtonStartX, ButtonStartY, 0, APPLY_TO_ALL_THREADS_ADJUSTMENT);

   CreateButton(enApplyToAllThreads, LinkedStartX, ButtonStartY, LARGE_BUTTON_SIZE_X, BUTTON_SIZE_Y, 
      GetButtonText(m_CurrentProcessing, enApplyToAllThreads, ButtonText), true);

   MoveXY(ButtonStartX, ButtonStartY, 0, BUTTON_SIZE_Y+BUTTON_GAP);

   MgraControl(m_MilMenuGraphicsContext, M_FONT_SIZE, SMALL_FONT);
   MgraFont(m_MilMenuGraphicsContext, NORMAL_FONT_TYPE);
   MgraText(m_MilMenuGraphicsContext, m_MilMenuBuffer, TEXT_START_X, ButtonStartY+TEXT_OFFSET_Y, MIL_TEXT("Run Processing"));

   CreateButton(enRunProcessing, ButtonStartX, ButtonStartY, BUTTON_SIZE_X, BUTTON_SIZE_Y, 
      GetButtonText(m_CurrentProcessing, enRunProcessing, ButtonText), true);

   MoveXY(ButtonStartX, ButtonStartY, 0, BUTTON_SIZE_Y+BUTTON_GAP);

   MgraText(m_MilMenuGraphicsContext, m_MilMenuBuffer, TEXT_START_X, ButtonStartY+TEXT_OFFSET_Y, MIL_TEXT("Refresh Display"));

   CreateButton(enRunDisplay, ButtonStartX, ButtonStartY, BUTTON_SIZE_X, BUTTON_SIZE_Y, 
      GetButtonText(m_CurrentProcessing, enRunDisplay, ButtonText), true);

   MoveXY(ButtonStartX, ButtonStartY, 0, BUTTON_SIZE_Y+BUTTON_GAP);
   }

//******************************************************************************************
// CreateBufferSection.  Setup the menu options related to buffer allocation.
//******************************************************************************************
void CMPMenu::CreateBufferSection(MIL_INT& ButtonStartX, MIL_INT& ButtonStartY)
   {
   MIL_TEXT_CHAR ButtonText[STRING_SIZE]=MIL_TEXT("");

   //Set information on memory bank
   MgraControl(m_MilMenuGraphicsContext, M_FONT_SIZE, SMALL_FONT);
   MgraFont(m_MilMenuGraphicsContext, NORMAL_FONT_TYPE);
   MgraText(m_MilMenuGraphicsContext, m_MilMenuBuffer, TEXT_START_X, ButtonStartY+TEXT_OFFSET_Y, 
      MIL_TEXT("Memory Bank Affinity"));

   //Create memory bank buttons
   MIL_INT LinkedStartX = ButtonStartX;
   MIL_INT LinkedStartY = ButtonStartY;
   CreateButton(enMemoryBankLeft, LinkedStartX, LinkedStartY, SMALL_BUTTON_SIZE_X, BUTTON_SIZE_Y, MIL_TEXT(" <<"), false);

   MoveXY(LinkedStartX, LinkedStartY, SMALL_BUTTON_SIZE_X+LINKED_BUTTON_GAP, 0);

   CreateButton(enMemoryBank, LinkedStartX, LinkedStartY, BUTTON_SIZE_X, BUTTON_SIZE_Y, 
      GetButtonText(m_CurrentProcessing, enMemoryBank, ButtonText), false);

   if (m_MPIsOff)
      m_Buttons[enMemoryBank].Disable(MIL_TEXT("n/a"));

   MoveXY(LinkedStartX, LinkedStartY, BUTTON_SIZE_X + LINKED_BUTTON_GAP, 0);

   CreateButton(enMemoryBankRight, LinkedStartX, ButtonStartY, SMALL_BUTTON_SIZE_X, BUTTON_SIZE_Y, MIL_TEXT(" >>"), false);

   MoveXY(LinkedStartX, ButtonStartY, 0, BUTTON_SIZE_Y+BUTTON_GAP);
   }

//******************************************************************************************
// CreateMPSection.  Setup the options related to MP control.
//******************************************************************************************
void CMPMenu::CreateMPSection(MIL_INT& ButtonStartX, MIL_INT& ButtonStartY)
   {
   MIL_INT LinkedStartX = 0; 
   MIL_INT LinkedStartY = 0;
   MIL_TEXT_CHAR ButtonText[STRING_SIZE]=MIL_TEXT("");   

   //Set multiprocessing information and buttons
   MgraControl(m_MilMenuGraphicsContext, M_FONT_SIZE, SMALL_FONT);
   MgraFont(m_MilMenuGraphicsContext, NORMAL_FONT_TYPE);
   MgraText(m_MilMenuGraphicsContext, m_MilMenuBuffer, TEXT_START_X, ButtonStartY+TEXT_OFFSET_Y, 
      MIL_TEXT("Use Multiprocessing"));

   CreateButton(enMP, ButtonStartX, ButtonStartY, BUTTON_SIZE_X, BUTTON_SIZE_Y, 
      GetButtonText(m_CurrentProcessing, enMP, ButtonText), true);

   MoveXY(ButtonStartX, ButtonStartY, 0, BUTTON_SIZE_Y+BUTTON_GAP+SECTION_GAP);

   if (m_MPIsOff)
      m_Buttons[enMP].Disable(MIL_TEXT("n/a"));

   MgraControl(m_MilMenuGraphicsContext, M_FONT_SIZE, MEDIUM_FONT);
   MgraFont(m_MilMenuGraphicsContext, M_FONT_DEFAULT);

   MappControl(M_DEFAULT, M_ERROR, M_PRINT_DISABLE);
   MgraFont(m_MilMenuGraphicsContext, BOLD_FONT_TYPE);
   MappControl(M_DEFAULT, M_ERROR, M_PRINT_ENABLE);

   MgraText(m_MilMenuGraphicsContext, m_MilMenuBuffer, TEXT_START_X, ButtonStartY, MIL_TEXT("Multiprocessing Control"));

   MoveXY(ButtonStartX, ButtonStartY, 0, SECTION_GAP);

   MgraControl(m_MilMenuGraphicsContext, M_FONT_SIZE, SMALL_FONT);
   MgraFont(m_MilMenuGraphicsContext, NORMAL_FONT_TYPE);
   MgraText(m_MilMenuGraphicsContext, m_MilMenuBuffer, TEXT_START_X, ButtonStartY+TEXT_OFFSET_Y, MIL_TEXT("Maximum Cores"));

   LinkedStartX = ButtonStartX;
   LinkedStartY = ButtonStartY;
   CreateButton(enCoreMaxLeft, LinkedStartX, LinkedStartY, SMALL_BUTTON_SIZE_X, BUTTON_SIZE_Y, MIL_TEXT(" <<"), false);

   MoveXY(LinkedStartX, LinkedStartY, SMALL_BUTTON_SIZE_X+LINKED_BUTTON_GAP, 0);

   CreateButton(enCoreMax, LinkedStartX, LinkedStartY, SMALL_BUTTON_SIZE_X, BUTTON_SIZE_Y, 
      GetButtonText(m_CurrentProcessing, enCoreMax, ButtonText), false);

   if (m_MPIsOff)
      m_Buttons[enCoreMax].Disable(MIL_TEXT("n/a"));

   MoveXY(LinkedStartX, LinkedStartY, SMALL_BUTTON_SIZE_X+LINKED_BUTTON_GAP, 0);

   CreateButton(enCoreMaxRight, LinkedStartX, ButtonStartY, SMALL_BUTTON_SIZE_X, BUTTON_SIZE_Y, MIL_TEXT(" >>"), false);

   MoveXY(ButtonStartX, ButtonStartY, 0, BUTTON_SIZE_Y+BUTTON_GAP);

   MgraText(m_MilMenuGraphicsContext, m_MilMenuBuffer, TEXT_START_X, ButtonStartY+TEXT_OFFSET_Y, MIL_TEXT("Core Sharing"));

   CreateButton(enCoreSharing, ButtonStartX, ButtonStartY, BUTTON_SIZE_X, BUTTON_SIZE_Y, 
      GetButtonText(m_CurrentProcessing, enCoreSharing, ButtonText), true);

   MoveXY(ButtonStartX, ButtonStartY, 0, BUTTON_SIZE_Y+BUTTON_GAP);

   if (m_MPIsOff)
      m_Buttons[enCoreSharing].Disable(MIL_TEXT("n/a"));

   MgraText(m_MilMenuGraphicsContext, m_MilMenuBuffer, TEXT_START_X, ButtonStartY+TEXT_OFFSET_Y, MIL_TEXT("Priority"));

   LinkedStartX = ButtonStartX;
   LinkedStartY = ButtonStartY;
   CreateButton(enMPPriorityLeft, LinkedStartX, LinkedStartY, SMALL_BUTTON_SIZE_X, BUTTON_SIZE_Y, MIL_TEXT(" <<"), false);

   MoveXY(LinkedStartX, LinkedStartY, SMALL_BUTTON_SIZE_X+LINKED_BUTTON_GAP, 0);

   CreateButton(enMPPriority, LinkedStartX, LinkedStartY, BUTTON_SIZE_X, BUTTON_SIZE_Y, 
      GetButtonText(m_CurrentProcessing, enMPPriority, ButtonText), false);

   if (m_MPIsOff)
      m_Buttons[enMPPriority].Disable(MIL_TEXT("n/a"));

   MoveXY(LinkedStartX, LinkedStartY, BUTTON_SIZE_X+LINKED_BUTTON_GAP, 0);

   CreateButton(enMPPriorityRight, LinkedStartX, ButtonStartY, SMALL_BUTTON_SIZE_X, BUTTON_SIZE_Y, MIL_TEXT(">>"), false);

   MoveXY(ButtonStartX, ButtonStartY, 0, BUTTON_SIZE_Y+BUTTON_GAP);

   //Create section for buffer memory banks
   CreateBufferSection(ButtonStartX, ButtonStartY);

   //Create the core affinity buttons
   MgraText(m_MilMenuGraphicsContext, m_MilMenuBuffer, TEXT_START_X, ButtonStartY+TEXT_OFFSET_Y, 
      MIL_TEXT("Core Affinity: (Core Index, Local Memory Bank Index)"));

   MoveXY(ButtonStartX, ButtonStartY, 0, CORE_AFFINITY_GAP);

   LinkedStartX = TEXT_START_X; 
   ButtonStartX = LinkedStartX;
   LinkedStartY = ButtonStartY;

   //Create one button for each core
   for (MIL_INT i=FIRST_CORE, Count=0; (i-FIRST_CORE)<m_NumberOfProcessorCores; i++, Count++)
      {      
      CreateButton((MPButtons)i, LinkedStartX, LinkedStartY, SMALL_BUTTON_SIZE_X, BUTTON_SIZE_Y, 
         GetButtonText(m_CurrentProcessing, (MPButtons)i, ButtonText), true);      

      if (m_MPIsOff)
         m_Buttons[(MPButtons)i].Disable(MIL_TEXT("n/a"));

      LinkedStartX = ButtonStartX + ( ((Count+1)%MAX_CORES_PER_ROW) * (SMALL_BUTTON_SIZE_X + LINKED_BUTTON_GAP) );
      LinkedStartY = ButtonStartY + ( ((Count+1)/MAX_CORES_PER_ROW) * (BUTTON_GAP + BUTTON_SIZE_Y));
      }

   //Create region for FPS update information
   ButtonStartX = BUTTON_START_X_POS;

   MoveXY(ButtonStartX, ButtonStartY, 0, BUTTON_GAP + SECTION_GAP);

   m_InfoRegions[enCurProcFPS].StartX = TEXT_START_X;
   m_InfoRegions[enCurProcFPS].StartY = m_MenuSizeY - PROC_FPS_SIZE;
   m_InfoRegions[enCurProcFPS].EndX = m_InfoRegions[enCurProcFPS].StartX + INFO_TEXT_SIZE-1;
   m_InfoRegions[enCurProcFPS].EndY = m_InfoRegions[enCurProcFPS].StartY + PROC_FPS_SIZE-1; 

   MbufChild2d(m_MilMenuBuffer, 
               m_InfoRegions[enCurProcFPS].StartX, m_InfoRegions[enCurProcFPS].StartY, 
               m_InfoRegions[enCurProcFPS].EndX-m_InfoRegions[enCurProcFPS].StartX+1, 
               m_InfoRegions[enCurProcFPS].EndY-m_InfoRegions[enCurProcFPS].StartY+1,
               &m_InfoRegions[enCurProcFPS].MilRectBuffer);
   }

//*******************************************************************************
// CreateButton.  Setup menu button with given information.
//*******************************************************************************
void CMPMenu::CreateButton(MPButtons ButtonType, MIL_INT StartX, MIL_INT StartY, 
                           MIL_INT ButtonSizeX, MIL_INT ButtonSizeY, 
                           const MIL_TEXT_CHAR* ButtonText, bool IsToggle)
   {
   RectStruct ButtonRect;

   //Define the button rectangle
   ButtonRect.StartX = StartX;
   ButtonRect.EndX = ButtonRect.StartX + ButtonSizeX-1;
   ButtonRect.StartY = StartY;
   ButtonRect.EndY = ButtonRect.StartY + ButtonSizeY-1;

   //Create the button on the menu buffer
   m_Buttons[ButtonType].Create(ButtonText, m_MilMenuBuffer, ButtonRect, IsToggle);
   }

//*******************************************************************************
// MoveXY.  Move the provided x,y positions by the given offsets.
//*******************************************************************************
void CMPMenu::MoveXY(MIL_INT& StartX, MIL_INT& StartY, MIL_INT OffsetX, MIL_INT OffsetY)
   {
   StartX += OffsetX;
   StartY += OffsetY;
   }


//*******************************************************************************
// Run.  Run the menu
//*******************************************************************************
void CMPMenu::Run()
   {
   //Show the menu
   MdispSelect(m_MilMenuDisplay, m_MilMenuBuffer);
   }

//*******************************************************************************
// GetButton.  Return the button that corresponds to the given x,y position
//*******************************************************************************
CMPMenu::MPButtons CMPMenu::GetButton(MIL_INT PositionX, MIL_INT PositionY)
   {
   MPButtons Button= (MPButtons)-1;

   for (MIL_INT i=FIRST_BUTTON; i<=LAST_BUTTON; i++)
      {
      //Get the button index that falls within the region the user just clicked on
      if (  (m_Buttons[i].GetRect().StartX<=PositionX) &&
            (m_Buttons[i].GetRect().EndX>=PositionX)   &&
            (m_Buttons[i].GetRect().StartY<=PositionY) &&
            (m_Buttons[i].GetRect().EndY>=PositionY) )
         {
         Button = (MPButtons)i;
         break;
         }
      }

   return Button;
   }

//*******************************************************************************
// ProcessingButtonClick.  Defines start/stop button operation.
//*******************************************************************************
void CMPMenu::ProcessingButtonClick()
   {
   MIL_TEXT_CHAR ButtonText[STRING_SIZE]=MIL_TEXT("");

   //Pause or run the button depending on the current state
   if (m_Processing[m_CurrentProcessing]->IsRunning())
      {
      m_Processing[m_CurrentProcessing]->Pause();
      StopUpdateInfo();
      }
   else
      {
      m_Processing[m_CurrentProcessing]->Run();
      StartUpdateInfo();
      }

   //Update the button and the menu
   m_Buttons[enRunProcessing].Push(GetButtonText(m_CurrentProcessing, enRunProcessing, ButtonText));

   UpdateMenu();
   }

//*******************************************************************************
// MPButtonClick.  Defines MP button operation.
//*******************************************************************************
void CMPMenu::MPButtonClick()
   {
   MIL_TEXT_CHAR ButtonText[STRING_SIZE]=MIL_TEXT("");

   //Set current MP status and update the button
   if (m_Processing[m_CurrentProcessing]->MPEnabled())
      {
      m_Processing[m_CurrentProcessing]->SetMP(false);
      }
   else
      {
      m_Processing[m_CurrentProcessing]->SetMP(true);
      }
   m_Buttons[enMP].Push(GetButtonText(m_CurrentProcessing, enMP, ButtonText));
   }

//*******************************************************************************
// CoreMaxButtonClick.  Defines core maximum add button operation.
//*******************************************************************************
void CMPMenu::CoreMaxButtonClick(bool Add)
   {
   MIL_TEXT_CHAR ButtonText[STRING_SIZE]=MIL_TEXT("");

   MIL_INT CurrentCoreMax = m_Processing[m_CurrentProcessing]->GetCoreMax();

   //Increase or reduce the maximum cores for currently selected processing
   if (Add && (CurrentCoreMax < m_NumberOfProcessorCores))
      {
      CurrentCoreMax++;
      }
   else if (!Add && (CurrentCoreMax>2))
      {
      CurrentCoreMax--;
      }
   m_Processing[m_CurrentProcessing]->SetCoreMax(CurrentCoreMax);
   m_Buttons[enCoreMax].Push(GetButtonText(m_CurrentProcessing, enCoreMax, ButtonText));
   }


//*******************************************************************************
// CoreSharingButtonClick.  Defines core sharing button operation.
//*******************************************************************************
void CMPMenu::CoreSharingButtonClick()
   {
   MIL_TEXT_CHAR ButtonText[STRING_SIZE]=MIL_TEXT("");

   //Enable or disable core sharing
   if (m_Processing[m_CurrentProcessing]->CoreSharingEnabled())
      {
      m_Processing[m_CurrentProcessing]->SetCoreSharing(false);
      }
   else
      {
      m_Processing[m_CurrentProcessing]->SetCoreSharing(true);
      }
   m_Buttons[enCoreSharing].Push(GetButtonText(m_CurrentProcessing, enCoreSharing, ButtonText));
   }

//*******************************************************************************
// MPPriorityButtonClick.  Defines MP priority button operation.
//*******************************************************************************
void CMPMenu::MPPriorityButtonClick(bool Next)
   {
   MIL_TEXT_CHAR ButtonText[STRING_SIZE]=MIL_TEXT("");
   MIL_INT MPPriority = m_Processing[m_CurrentProcessing]->GetMPPriority();

   //Set the priority for the currently selected processing thread
   switch (MPPriority)
      {
      case M_ABOVE_NORMAL:
         if (Next)
            MPPriority=M_BELOW_NORMAL;
         break;
      case M_BELOW_NORMAL:
         if (Next)
            MPPriority=M_HIGHEST;
         else
            MPPriority=M_ABOVE_NORMAL;
         break;
      case M_HIGHEST:
         if (Next)
            MPPriority=M_IDLE;
         else
            MPPriority=M_BELOW_NORMAL;
         break;
      case M_IDLE:
         if (Next)
            MPPriority=M_LOWEST;
         else
            MPPriority=M_HIGHEST;
         break;
      case M_LOWEST:
         if (Next)
            MPPriority=M_NORMAL;
         else
            MPPriority=M_IDLE;
         break;
      case M_NORMAL:
         if (Next)
            MPPriority=M_TIME_CRITICAL;
         else
            MPPriority=M_LOWEST;
         break;
      case M_TIME_CRITICAL:
         if (!Next)
            MPPriority=M_NORMAL;
         break;
      default:
         break;
      }

   m_Processing[m_CurrentProcessing]->SetMPPriority(MPPriority);
   m_Buttons[enMPPriority].Push(GetButtonText(m_CurrentProcessing, enMPPriority, ButtonText));
   }

//*******************************************************************************
// ProcessingThreadButtonClick.  Select the processing thread to control.
//*******************************************************************************
void CMPMenu::ProcessingThreadButtonClick(bool Next)
   {
   MIL_TEXT_CHAR ButtonText[STRING_SIZE]=MIL_TEXT("");

   //Select the requested processing thread and update information on the menu
   //accordingly
   if (Next && (m_CurrentProcessing < m_CurrentProcessingNum-1))
      {
      m_CurrentProcessing++;
      }
   else if (!Next && (m_CurrentProcessing > 0))
      {
      m_CurrentProcessing--;      
      }

   m_Buttons[enProcThread].Push(GetButtonText(m_CurrentProcessing, enProcThread, ButtonText));

   UpdateMenu();
   }

//*******************************************************************************
// ApplyToAllThreadsButtonClick.  Apply current thread settings to all threads
//*******************************************************************************
void CMPMenu::ApplyToAllThreadsButtonClick()
   {
   //Apply current processing configuration to all threads
   MIL_INT ThreadIndex;  
   bool UseMemoryBank = m_Processing[m_CurrentProcessing]->UseMemoryBank();
   MIL_INT64 MemoryBank = m_Processing[m_CurrentProcessing]->GetCurrentMemoryBank();
   for (MIL_INT i=1; i<m_CurrentProcessingNum; i++)
      {
      ThreadIndex = (m_CurrentProcessing+i)%(m_CurrentProcessingNum);
      if (m_Processing[m_CurrentProcessing]->IsRunning())
         m_Processing[ThreadIndex]->Run();
      else
         m_Processing[ThreadIndex]->Pause();

      m_Processing[ThreadIndex]->RunDisplay(m_Processing[m_CurrentProcessing]->DisplayRunning());
      m_Processing[ThreadIndex]->SetMP(m_Processing[m_CurrentProcessing]->MPEnabled());
      m_Processing[ThreadIndex]->SetCoreMax(m_Processing[m_CurrentProcessing]->GetCoreMax());
      m_Processing[ThreadIndex]->SetCoreSharing(m_Processing[m_CurrentProcessing]->CoreSharingEnabled());
      m_Processing[ThreadIndex]->SetMPPriority(m_Processing[m_CurrentProcessing]->GetMPPriority());
      m_Processing[ThreadIndex]->SetCoreAffinity(m_Processing[m_CurrentProcessing]->GetCoreAffinity());

      m_Processing[ThreadIndex]->SetCurrentMemoryBank(MemoryBank, UseMemoryBank);
      }

   //For visual effect, push the button twice.
   MIL_TEXT_CHAR ButtonText[STRING_SIZE]=MIL_TEXT("");
   m_Buttons[enApplyToAllThreads].Push(ButtonText);
   MosSleep(50);
   GetButtonText(m_CurrentProcessing, enApplyToAllThreads, ButtonText);
   m_Buttons[enApplyToAllThreads].Push(GetButtonText(m_CurrentProcessing, enApplyToAllThreads, ButtonText));
   }

//*******************************************************************************
// NumberOfThreadsButtonClick.  Select the number of processing threads.
//*******************************************************************************
void CMPMenu::NumberOfThreadsButtonClick(bool Next)
   {
   MIL_TEXT_CHAR ButtonText[STRING_SIZE]=MIL_TEXT("");

   //Increase or reduce the number of processing threads
   if (Next && (m_CurrentProcessingNum < m_ProcessingArraySize))
      {
      m_CurrentProcessingNum++;

      //Start and configure the thread
      m_Processing[m_CurrentProcessingNum-1]->StartThread();  

      if (m_Processing[m_CurrentProcessing]->DisplaySelected())
         {
         m_Processing[m_CurrentProcessingNum-1]->DisplaySelect();
         m_Processing[m_CurrentProcessingNum-1]->RunDisplay(true);
         }
      else
         {
         m_Processing[m_CurrentProcessingNum-1]->RunDisplay(false);
         }

      InitProcessingMP(m_NumberOfProcessorCores, m_Processing[m_CurrentProcessingNum-1]);

      m_Processing[m_CurrentProcessingNum-1]->Run();

      m_CurrentProcessing = m_CurrentProcessingNum-1;

      StartUpdateInfo();
      }
   else if (!Next && (m_CurrentProcessingNum > 1))
      {
      //Stop the thread
      m_Processing[m_CurrentProcessingNum-1]->DisplayDeselect();      
      m_Processing[m_CurrentProcessingNum-1]->StopThread();
      m_CurrentProcessingNum--;

      if (m_CurrentProcessing == m_CurrentProcessingNum)
         m_CurrentProcessing = m_CurrentProcessingNum-1;

      StopUpdateInfo();
      }

   //Update button and menu accordingly
   m_Buttons[enTotalThreads].Push(GetButtonText(m_CurrentProcessing, enTotalThreads, ButtonText));
   UpdateMenu();
   }

//*******************************************************************************
// CurrentThreadDisplayButtonClick.  Defines selected thread display button operation.
//*******************************************************************************
void CMPMenu::CurrentThreadDisplayButtonClick()
   {
   MIL_TEXT_CHAR ButtonText[STRING_SIZE]=MIL_TEXT("");

   //Set whether to run or stop the display update of the currently selected
   //processing thread.
   if (m_Processing[m_CurrentProcessing]->DisplayRunning())
      {
      m_Processing[m_CurrentProcessing]->RunDisplay(false);
      }
   else
      {
      m_Processing[m_CurrentProcessing]->RunDisplay(true);
      }

   m_Buttons[enRunDisplay].Push(GetButtonText(m_CurrentProcessing, enRunDisplay, ButtonText));
   }

//*******************************************************************************
// AllThreadsDisplayButtonClick.  Defines display button operation for all 
// threads.
//*******************************************************************************
void CMPMenu::AllThreadsDisplayButtonClick()
   {
   //Either show or remove all currently allocated threads
   if (m_Processing[m_CurrentProcessing]->DisplaySelected())
      {
      for (MIL_INT i=0; i<m_CurrentProcessingNum; i++)
         {
         //Pause the display if it is running
         if (m_Processing[i]->DisplayRunning())
            m_Processing[i]->RunDisplay(false);

         //Deselect the display
         m_Processing[i]->DisplayDeselect();
         }
      }
   else
      {
      for (MIL_INT i=0; i<m_CurrentProcessingNum; i++)
         {
         //Run the display
         m_Processing[i]->RunDisplay(true);

         //Select the display
         m_Processing[i]->DisplaySelect();
         }
      }

   UpdateMenu();
   }

//*******************************************************************************
// MemoryBankButtonClick.  Select the memory bank to allocate processing buffers 
// on.
//*******************************************************************************
void CMPMenu::MemoryBankButtonClick(bool Next)
   {      
   //Set the current memory bank to use.  Initially, no memory bank is specified.
   if ( (!m_Processing[m_CurrentProcessing]->UseMemoryBank() && Next) && (m_NumMemoryBank>0) )
      {
      m_Processing[m_CurrentProcessing]->SetCurrentMemoryBank(M_MEMORY_BANK_0, true);
      }
   else if (m_Processing[m_CurrentProcessing]->UseMemoryBank())
      {
      bool ValidBank = false;
      MIL_INT64 NewMemoryBank = m_Processing[m_CurrentProcessing]->GetMemoryBank(Next, ValidBank);

      if (ValidBank)
         {
         switch(NewMemoryBank)
            {
            case M_MEMORY_BANK_0:
               if (!Next && (m_Processing[m_CurrentProcessing]->GetCurrentMemoryBank()==M_MEMORY_BANK_0))
                  {
                  m_Processing[m_CurrentProcessing]->SetCurrentMemoryBank(0, false);
                  }
               else if (!Next)
                  {
                  m_Processing[m_CurrentProcessing]->SetCurrentMemoryBank(M_MEMORY_BANK_0, true);
                  }
               break;
            case M_MEMORY_BANK_1:
               m_Processing[m_CurrentProcessing]->SetCurrentMemoryBank(M_MEMORY_BANK_1, true);
               break;
            case M_MEMORY_BANK_2:
               m_Processing[m_CurrentProcessing]->SetCurrentMemoryBank(M_MEMORY_BANK_2, true);
               break;
            case M_MEMORY_BANK_3:
               m_Processing[m_CurrentProcessing]->SetCurrentMemoryBank(M_MEMORY_BANK_3, true);
               break;
            case M_MEMORY_BANK_4:
               m_Processing[m_CurrentProcessing]->SetCurrentMemoryBank(M_MEMORY_BANK_4, true);
               break;
            case M_MEMORY_BANK_5:
               m_Processing[m_CurrentProcessing]->SetCurrentMemoryBank(M_MEMORY_BANK_5, true);
               break;
            case M_MEMORY_BANK_6:
               m_Processing[m_CurrentProcessing]->SetCurrentMemoryBank(M_MEMORY_BANK_6, true);
               break;
            default:
               break;
            }
         }
      }

   //Update the button text
   MIL_TEXT_CHAR ButtonText[STRING_SIZE]=MIL_TEXT("");
   m_Buttons[enMemoryBank].Push(GetButtonText(m_CurrentProcessing, enMemoryBank, ButtonText));
   }

//*******************************************************************************
// CoreAffinityButtonClick.  Set or remove an affinity to the given core index.
//*******************************************************************************
void CMPMenu::CoreAffinityButtonClick(MIL_INT CoreIndex)
   {
   MIL_UINT64 CurrentCoreAffinity = m_Processing[m_CurrentProcessing]->GetCoreAffinity();
   MIL_UINT64 CoreAffinityMask = 0x00000001;

   //Set the mask on the position that represents the index of the real core
   CoreAffinityMask = CoreAffinityMask << GetRealCoreIndex(CoreIndex);

   MIL_TEXT_CHAR ButtonText[STRING_SIZE]=MIL_TEXT("");

   if (m_Buttons[CoreIndex+FIRST_CORE].IsPressed())
      {
      //remove affinity
      m_Processing[m_CurrentProcessing]->SetCoreAffinity(CurrentCoreAffinity&~CoreAffinityMask);
      }
   else
      {
      //set affinity
      m_Processing[m_CurrentProcessing]->SetCoreAffinity(CurrentCoreAffinity|CoreAffinityMask);
      }

   m_Buttons[CoreIndex+FIRST_CORE].Push(GetButtonText(m_CurrentProcessing, (MPButtons)(CoreIndex+FIRST_CORE), 
      ButtonText));   
   }

//*******************************************************************************
// UpdateMenu.  Updated the status of all the buttons and info if processing
// thread has changed.
//*******************************************************************************
void CMPMenu::UpdateMenu()
   {
   MIL_TEXT_CHAR ButtonText[STRING_SIZE]=MIL_TEXT("");

   //Make sure the display selection is consistent with the show display button
   if ( (m_Processing[m_CurrentProcessing]->DisplaySelected() && !m_Buttons[enShowDisplay].IsPressed()) ||
        (!m_Processing[m_CurrentProcessing]->DisplaySelected() && m_Buttons[enShowDisplay].IsPressed()) )
      {
      m_Buttons[enShowDisplay].Push(GetButtonText(m_CurrentProcessing, enShowDisplay, ButtonText));
      }

   //Update the currently selected processing index
   m_Buttons[enProcThread].Push(GetButtonText(m_CurrentProcessing, enProcThread, ButtonText));

   //Update whether it is running or not and it's associated FPS
   if (m_Processing[m_CurrentProcessing]->IsRunning() && !m_Buttons[enRunProcessing].IsPressed())
      {
      m_Buttons[enRunProcessing].Push(GetButtonText(m_CurrentProcessing, enRunProcessing, ButtonText));
      }
   else if (!m_Processing[m_CurrentProcessing]->IsRunning())
      {
      UpdateCurProcFPS();
      if (m_Buttons[enRunProcessing].IsPressed())
         m_Buttons[enRunProcessing].Push(GetButtonText(m_CurrentProcessing, enRunProcessing, ButtonText));
      }

   //Clear total FPS if none of the threads are processing
   MIL_INT i=0;
   for (i=0; i<m_CurrentProcessingNum; i++)
      {
      if (m_Processing[i]->IsRunning())
         break;
      }

   if (i==m_CurrentProcessingNum)
      UpdateTotalFPS();

   //Update the update display button
   if ( (m_Processing[m_CurrentProcessing]->DisplayRunning() && !m_Buttons[enRunDisplay].IsPressed()) ||
        (!m_Processing[m_CurrentProcessing]->DisplayRunning() && m_Buttons[enRunDisplay].IsPressed()) )
      {
      m_Buttons[enRunDisplay].Push(GetButtonText(m_CurrentProcessing, enRunDisplay, ButtonText));
      }

   if (!m_MPIsOff)
      {
      //Update the MP button
      if ( (!m_Processing[m_CurrentProcessing]->MPEnabled() && m_Buttons[enMP].IsPressed()) || 
           (m_Processing[m_CurrentProcessing]->MPEnabled() && !m_Buttons[enMP].IsPressed()) )
         {
         m_Buttons[enMP].Push(GetButtonText(m_CurrentProcessing, enMP, ButtonText));
         }

      //Update maximum cores button
      m_Buttons[enCoreMax].Push(GetButtonText(m_CurrentProcessing, enCoreMax, ButtonText));

      //Update MP thread Priority
      m_Buttons[enMPPriority].Push(GetButtonText(m_CurrentProcessing, enMPPriority, ButtonText));
      
      //Update Memory bank button
      m_Buttons[enMemoryBank].Push(GetButtonText(m_CurrentProcessing, enMemoryBank, ButtonText));

      //Update Core sharing button
      if ( (!m_Processing[m_CurrentProcessing]->CoreSharingEnabled() && m_Buttons[enCoreSharing].IsPressed()) || 
           (m_Processing[m_CurrentProcessing]->CoreSharingEnabled() && !m_Buttons[enCoreSharing].IsPressed()) )
         {
         m_Buttons[enCoreSharing].Push(GetButtonText(m_CurrentProcessing, enCoreSharing, ButtonText));
         }

      //Update Core affinity buttons
      MIL_UINT64 CurrentCoreAffinity = m_Processing[m_CurrentProcessing]->GetCoreAffinity();

      for (MIL_INT i=FIRST_CORE; (i-FIRST_CORE)<m_NumberOfProcessorCores; i++)
         {  
         bool AffinityIsSet = (CurrentCoreAffinity&((MIL_UINT64)1<<GetRealCoreIndex(i-FIRST_CORE))) != 0x00000000;

         if (AffinityIsSet != m_Buttons[i].IsPressed())
            {
            m_Buttons[i].Push(GetButtonText(m_CurrentProcessing, (MPButtons)i, ButtonText));
            }
         }
      }
   }

//*******************************************************************************
// UpdateInfo.  Update information such as FPS the menu display.
//*******************************************************************************
void CMPMenu::UpdateInfo()
   {
   //Wait for an event
   while ( MthrWaitMultiple(m_MilInfoEvents, NUM_EVENTS, M_EVENT_WAIT, M_NULL) != (MIL_INT)enKill )
      {
      while (m_UpdateInfoStarted)
         {
         //Update total frames per second
         UpdateTotalFPS();

         //Update frames per second for the currently selected thread
         if (m_Processing[m_CurrentProcessing]->GetFrameRate()>0)
            UpdateCurProcFPS();

         MosSleep(UPDATE_INFO_TIME_DELAY);
         }

      //Signal that the information is no longer being updated
      MthrControl(m_MilUpdateInfoStoppedEvent, M_EVENT_SET, M_SIGNALED);
      }
   }

//*******************************************************************************
// UpdateTotalFPS.  Update Total FPS on the menu
//*******************************************************************************
void CMPMenu::UpdateTotalFPS()
   {
   MIL_DOUBLE FrameRate = 0;
   
   //Get the sum of frames per second of all threads
   for (MIL_INT i=0; i<m_CurrentProcessingNum; i++)
      {
      if (m_Processing[i]->IsRunning())
         FrameRate += m_Processing[i]->GetFrameRate();
      }

   //Update total FPS
   MosSprintf(m_InfoString, STRING_SIZE, MIL_TEXT("%.1f Total Frames per Second"), FrameRate); 
   MdispControl(m_MilMenuDisplay, M_UPDATE, M_DISABLE);
   MbufClear(m_InfoRegions[enTotalFPS].MilRectBuffer, BACKGROUND_COLOR);
   MgraText(m_MilInfoGraphicsContext, m_InfoRegions[enTotalFPS].MilRectBuffer, 0, 0, m_InfoString);
   MdispControl(m_MilMenuDisplay, M_UPDATE, M_ENABLE);
   }

//*******************************************************************************
// UpdateFPS.  Update current processing FPS on the menu
//*******************************************************************************
void CMPMenu::UpdateCurProcFPS()
   {
   //Update current processing FPS
   MosSprintf(m_InfoString, STRING_SIZE, MIL_TEXT("%.1f Frames per Second"), 
      m_Processing[m_CurrentProcessing]->GetFrameRate()); 
   MdispControl(m_MilMenuDisplay, M_UPDATE, M_DISABLE);
   MbufClear(m_InfoRegions[enCurProcFPS].MilRectBuffer, BACKGROUND_COLOR);
   MgraText(m_MilInfoGraphicsContext, m_InfoRegions[enCurProcFPS].MilRectBuffer, 0, 0, m_InfoString);
   MdispControl(m_MilMenuDisplay, M_UPDATE, M_ENABLE);
   }

//*******************************************************************************
// StartUpdateInfo.  Indicate to start updating FPS
//*******************************************************************************
void CMPMenu::StartUpdateInfo()
   {
   //Set the even to start updating frames per second
   m_UpdateInfoStarted = true;
   MthrControl(m_MilInfoEvents[enRun], M_EVENT_SET, M_SIGNALED);
   }

//*******************************************************************************
// StopUpdateInfo.  Indicate to stop updating FPS
//*******************************************************************************
void CMPMenu::StopUpdateInfo()
   {
   MIL_INT i=0;
   
   //Look for a running thread
   for (i=0; i<m_ProcessingArraySize; i++)
      {
      if (m_Processing[i]->IsRunning())
         break;
      }

   //Only stop updating the info if none of the processing are started
   if ((i==m_ProcessingArraySize) && m_UpdateInfoStarted)
      {
      m_UpdateInfoStarted = false;

      //Wait until the information on the menu has stopped updating
      MthrWait(m_MilUpdateInfoStoppedEvent, M_EVENT_WAIT, M_NULL);
      }
   }

//*****************************************************************************
// GetButtonText. Centralizes the management of the text written on buttons.
//*****************************************************************************
MIL_TEXT_CHAR* CMPMenu::GetButtonText(MIL_INT ProcessingIndex, MPButtons ButtonType,  
   MIL_TEXT_CHAR* ButtonText)
   {
   bool UseMemoryBank = false;
   MIL_INT64 CurrentMemoryBank = 0;
   MIL_UINT64 CoreMask = 0x00000001;

   //Get the text for the given button type and the currently selected user thread
   switch (ButtonType)
      {
      case enTotalThreads:
         MosSprintf(ButtonText, STRING_SIZE, MIL_TEXT("%d"), m_CurrentProcessingNum);
         break;
      case enShowDisplay:
         if (m_Processing[m_CurrentProcessing]->DisplaySelected())
            MosSprintf(ButtonText, STRING_SIZE, MIL_TEXT("%s"), MIL_TEXT("Yes"));
         else
            MosSprintf(ButtonText, STRING_SIZE, MIL_TEXT("%s"), MIL_TEXT("No"));
         break;
      case enProcThread:
         MosSprintf(ButtonText, STRING_SIZE, MIL_TEXT("%d"), m_CurrentProcessing);
         break;
      case enApplyToAllThreads:
         MosSprintf(ButtonText, STRING_SIZE, MIL_TEXT("%s"), MIL_TEXT("Apply to All Threads"));
         break;
      case enRunProcessing:
         if (m_Processing[ProcessingIndex]->IsRunning())
            MosSprintf(ButtonText, STRING_SIZE, MIL_TEXT("%s"), MIL_TEXT("Yes"));
         else
            MosSprintf(ButtonText, STRING_SIZE, MIL_TEXT("%s"), MIL_TEXT("No"));
         break;
      case enRunDisplay: 
         if (m_Processing[ProcessingIndex]->DisplayRunning())
            MosSprintf(ButtonText, STRING_SIZE, MIL_TEXT("%s"), MIL_TEXT("Yes"));
         else
            MosSprintf(ButtonText, STRING_SIZE, MIL_TEXT("%s"), MIL_TEXT("No"));
         break;
      case enMemoryBank:
         CurrentMemoryBank = m_Processing[m_CurrentProcessing]->GetCurrentMemoryBank();

         if (m_Processing[m_CurrentProcessing]->UseMemoryBank())
            {
            switch(CurrentMemoryBank)
               {
               case M_MEMORY_BANK_0:
                  MosSprintf(ButtonText, STRING_SIZE, MIL_TEXT("%s"), MIL_TEXT("Bank 0"));
                  break;
               case M_MEMORY_BANK_1:
                  MosSprintf(ButtonText, STRING_SIZE, MIL_TEXT("%s"), MIL_TEXT("Bank 1"));
                  break;
               case M_MEMORY_BANK_2:
                  MosSprintf(ButtonText, STRING_SIZE, MIL_TEXT("%s"), MIL_TEXT("Bank 2"));
                  break;
               case M_MEMORY_BANK_3:
                  MosSprintf(ButtonText, STRING_SIZE, MIL_TEXT("%s"), MIL_TEXT("Bank 3"));
                  break;
               case M_MEMORY_BANK_4:
                  MosSprintf(ButtonText, STRING_SIZE, MIL_TEXT("%s"), MIL_TEXT("Bank 4"));
                  break;
               case M_MEMORY_BANK_5:
                  MosSprintf(ButtonText, STRING_SIZE, MIL_TEXT("%s"), MIL_TEXT("Bank 5"));
                  break;
               case M_MEMORY_BANK_6:
                  MosSprintf(ButtonText, STRING_SIZE, MIL_TEXT("%s"), MIL_TEXT("Bank 6"));
                  break;
               default:
                  break;
               }
            }
         else
            {
            MosSprintf(ButtonText, STRING_SIZE, MIL_TEXT("%s"), MIL_TEXT("No Affinity"));
            }
         break;
      case enMP:  
         if (m_Processing[ProcessingIndex]->MPEnabled())
            MosSprintf(ButtonText, STRING_SIZE, MIL_TEXT("%s"), MIL_TEXT("Yes"));
         else
            MosSprintf(ButtonText, STRING_SIZE, MIL_TEXT("%s"), MIL_TEXT("No"));
         break;
      case enCoreMax: 
         MosSprintf(ButtonText, STRING_SIZE, MIL_TEXT("%d"), m_Processing[m_CurrentProcessing]->GetCoreMax());
         break;
      case enCoreSharing: 
         if (m_Processing[ProcessingIndex]->CoreSharingEnabled())
            MosSprintf(ButtonText, STRING_SIZE, MIL_TEXT("%s"), MIL_TEXT("Enabled"));
         else
            MosSprintf(ButtonText, STRING_SIZE, MIL_TEXT("%s"), MIL_TEXT("Disabled"));
         break;
      case enMPPriority:
         switch (m_Processing[ProcessingIndex]->GetMPPriority())
            {
            case M_ABOVE_NORMAL:
               MosSprintf(ButtonText, STRING_SIZE, MIL_TEXT("Above Normal"));
               break;
            case M_BELOW_NORMAL:
               MosSprintf(ButtonText, STRING_SIZE, MIL_TEXT("Below Normal"));
               break;
            case M_HIGHEST:
               MosSprintf(ButtonText, STRING_SIZE, MIL_TEXT("Highest"));
               break;
            case M_IDLE:
               MosSprintf(ButtonText, STRING_SIZE, MIL_TEXT("Idle"));
               break;
            case M_LOWEST:
               MosSprintf(ButtonText, STRING_SIZE, MIL_TEXT("Lowest"));
               break;
            case M_NORMAL:
               MosSprintf(ButtonText, STRING_SIZE, MIL_TEXT("Normal"));
               break;
            case M_TIME_CRITICAL:
               MosSprintf(ButtonText, STRING_SIZE, MIL_TEXT("Time Critical"));
               break;
            default:
               break;
            }
         break;
      case enCore0:case enCore1:case enCore2:case enCore3:
      case enCore4:case enCore5:case enCore6:case enCore7:
      case enCore8:case enCore9:case enCore10:case enCore11:
      case enCore12:case enCore13:case enCore14:case enCore15:
      case enCore16:case enCore17:case enCore18:case enCore19:
      case enCore20:case enCore21:case enCore22:case enCore23:
      case enCore24:case enCore25:case enCore26:case enCore27:
      case enCore28:case enCore29:case enCore30:case enCore31:
         MosSprintf(ButtonText, STRING_SIZE, MIL_TEXT("%d, %d"), GetRealCoreIndex(ButtonType-FIRST_CORE), 
            MappInquireMp(M_DEFAULT, M_CORE_MEMORY_BANK, M_DEFAULT, GetRealCoreIndex(ButtonType-FIRST_CORE), M_NULL));
         break;
      default:
         break;
      }
   return ButtonText;
   }

//*****************************************************************************
// InitProcessingMP. Initializes the state of MP of the given processing
// object.
//*****************************************************************************
void CMPMenu::InitProcessingMP(MIL_INT NumCores, CMPProcessing* Processing)
   {
   //Set the initial MP state of the given processing object
   Processing->SetMP(true);
   Processing->SetCoreMax(NumCores);
   Processing->SetCoreSharing(false);
   Processing->SetMPPriority(M_NORMAL);
   Processing->SetCoreAffinity(0x00000000);
   Processing->SetCurrentMemoryBank(0, false);
   }


//*****************************************************************************
// GetRealCoreIndex. Get the real (physical) index of the core specified by
// the rank.  If, for example, a core affinity has been set on the process the 
// physical core indices can be different.
//*****************************************************************************
MIL_INT CMPMenu::GetRealCoreIndex(MIL_INT Rank)
   {
   MIL_INT i=0;
   MIL_INT AvailableCoresCount = 0;

   for (i=0; i<MAX_MP_CORES; i++)
      {
      if (m_CoreAffinityMaskProcess[0]&((MIL_UINT64)1<<i))
         AvailableCoresCount++;

      if (AvailableCoresCount==Rank+1)
         break;
      }

   return i;
   }

//*****************************************************************************
// MenuHookFct. Manages the interaction with the menu.
//*****************************************************************************
MIL_INT MFTYPE CMPMenu::MenuHookFct(MIL_INT HookType, MIL_ID MilEvent, void *UserDataPtr)
   {
   CMPMenu* MenuPtr = (CMPMenu*)UserDataPtr;
   MIL_INT PositionX = 0;
   MIL_INT PositionY = 0;
   MIL_UINT64 CoreMask = 0x00000001;

   //Get the position of the button click
   MdispGetHookInfo(MilEvent, M_MOUSE_POSITION_X, &PositionX);
   MdispGetHookInfo(MilEvent, M_MOUSE_POSITION_Y, &PositionY);

   //Get the button type
   MPButtons ButtonType = MenuPtr->GetButton(PositionX, PositionY);

   //Call the click handler function according to the button type
   switch (ButtonType)
      {
      case enTotalThreadsLeft:
         MenuPtr->NumberOfThreadsButtonClick(false);
         break;
      case enTotalThreadsRight:
         MenuPtr->NumberOfThreadsButtonClick(true);
         break;
      case enShowDisplay:
         MenuPtr->AllThreadsDisplayButtonClick();
         break;
      case enProcThreadLeft:
         MenuPtr->ProcessingThreadButtonClick(false);
         break;
      case enProcThreadRight:
         MenuPtr->ProcessingThreadButtonClick(true);
         break;
      case enApplyToAllThreads:
         MenuPtr->ApplyToAllThreadsButtonClick();
         break;
      case enRunProcessing:
         MenuPtr->ProcessingButtonClick();
         break;
      case enRunDisplay:
         MenuPtr->CurrentThreadDisplayButtonClick();
         break;
      case enMemoryBankLeft:
         if (MenuPtr->m_NumberOfProcessorCores>=MIN_MP_CORES)
            MenuPtr->MemoryBankButtonClick(false);
         break;
      case enMemoryBankRight:
         if (MenuPtr->m_NumberOfProcessorCores>=MIN_MP_CORES)
            MenuPtr->MemoryBankButtonClick(true);
         break;
      case enMP:
         if (MenuPtr->m_NumberOfProcessorCores>=MIN_MP_CORES)
            MenuPtr->MPButtonClick();
         break;
      case enCoreMaxLeft:
         if (MenuPtr->m_NumberOfProcessorCores>=MIN_MP_CORES)
            MenuPtr->CoreMaxButtonClick(false);
         break;
      case enCoreMaxRight:
         if (MenuPtr->m_NumberOfProcessorCores>=MIN_MP_CORES)
            MenuPtr->CoreMaxButtonClick(true);
         break;
      case enCoreSharing:
         if (MenuPtr->m_NumberOfProcessorCores>=MIN_MP_CORES)
            MenuPtr->CoreSharingButtonClick();
         break;
      case enMPPriorityLeft:
         if (MenuPtr->m_NumberOfProcessorCores>=MIN_MP_CORES)
            MenuPtr->MPPriorityButtonClick(false);
         break;
      case enMPPriorityRight:
         if (MenuPtr->m_NumberOfProcessorCores>=MIN_MP_CORES)
            MenuPtr->MPPriorityButtonClick(true);
         break;
      //Core affinity handling
      case enCore0:case enCore1:case enCore2:case enCore3:
      case enCore4:case enCore5:case enCore6:case enCore7:
      case enCore8:case enCore9:case enCore10:case enCore11:
      case enCore12:case enCore13:case enCore14:case enCore15:
      case enCore16:case enCore17:case enCore18:case enCore19:
      case enCore20:case enCore21:case enCore22:case enCore23:
      case enCore24:case enCore25:case enCore26:case enCore27:
      case enCore28:case enCore29:case enCore30:case enCore31:
         if (MenuPtr->m_NumberOfProcessorCores>=MIN_MP_CORES)
            MenuPtr->CoreAffinityButtonClick(ButtonType-FIRST_CORE);
         break;
      default:
         break;
      };

   return 0;
   }

//*****************************************************************************
// UpdateInfoFunction. Function that updates FPS
//*****************************************************************************
MIL_UINT32 MFTYPE CMPMenu::UpdateInfoFunction(void* DataPtr)
   {
   CMPMenu* Menu = (CMPMenu*)DataPtr;

   Menu->UpdateInfo();
   return 0;
   }
