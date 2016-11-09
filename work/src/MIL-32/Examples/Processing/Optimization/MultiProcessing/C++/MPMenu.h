//***************************************************************************************
//
// File name: Menu.h
//
// Synopsis:  Class that defines the menu of the MP processing example
//

#ifndef MPMENU_H
#define MPMENU_H

//*****************************************************************************
// Class used to define the menu of the MP processing example.
// It executes the commands given by the user on the provided processing
// object.
//*****************************************************************************
class CMPMenu
   {
   public:
      CMPMenu(MIL_INT ProcessingArraySize, CMPProcessing** Processing);
      ~CMPMenu();

      void Run();

   private:
      //Disallow copy
      CMPMenu(const CMPMenu&);
      CMPMenu& operator=(const CMPMenu&);

      #include "ButtonsEnum.h"

      MIL_ID m_MilMenuSystem;
      MIL_ID m_MilMenuDisplay;
      MIL_ID m_MilMenuBuffer;
      MIL_ID m_MilMenuGraphicsContext;
      MIL_ID m_MilInfoGraphicsContext;
      MIL_ID m_MilInfoThread;
      MIL_ID m_MilInfoEvents[NUM_EVENTS];

      MIL_INT        m_NumberOfProcessorCores;
      MIL_UINT64*    m_CoreAffinityMaskProcess;
      MIL_INT m_MenuSizeY;
         
      CMPMenuButton  m_Buttons[NUM_BUTTONS];
      RectStruct     m_InfoRegions[NUM_INFO];
      MIL_TEXT_CHAR  m_InfoString[STRING_SIZE];

      MIL_INT           m_ProcessingArraySize;
      CMPProcessing**   m_Processing;

      MIL_INT           m_CurrentProcessing;
      MIL_INT           m_CurrentProcessingNum;
     
      MIL_INT           m_NumMemoryBank;

      bool m_UpdateInfoStarted;
      MIL_ID m_MilUpdateInfoStoppedEvent;

      bool m_MPIsOff;

      //Menu control functions
      void ProcessingButtonClick();
      void MPButtonClick();
      void CoreMaxButtonClick(bool Add);
      void CoreSharingButtonClick();
      void MPPriorityButtonClick(bool Next);
      void MemoryBankButtonClick(bool Next);
      void CoreAffinityButtonClick(MIL_INT CoreIndex);

      void ProcessingThreadButtonClick(bool Next);
      void NumberOfThreadsButtonClick(bool Next);

      void ApplyToAllThreadsButtonClick();

      void CurrentThreadDisplayButtonClick();
      void AllThreadsDisplayButtonClick();

      //Menu setup and display functions
      void CreateMenu();
      void CreateApplicationControlSection(MIL_INT& ButtonStartX, MIL_INT& ButtonStartY);
      void CreateUserThreadsSection(MIL_INT& ButtonStartX, MIL_INT& ButtonStartY);

      void CreateBufferSection(MIL_INT& ButtonStartX, MIL_INT& ButtonStartY);
      void CreateMPSection(MIL_INT& ButtonStartX, MIL_INT& ButtonStartY);
      void CreateButton( MPButtons ButtonType, 
                         MIL_INT StartX, MIL_INT StartY,
                         MIL_INT ButtonSizeX, MIL_INT ButtonSizeY, 
                         const MIL_TEXT_CHAR* ButtonText, bool IsToggle);
      void MoveXY(MIL_INT& StartX, MIL_INT& StartY, MIL_INT OffsetX, MIL_INT OffsetY);
      void Create();
      void Destroy();


      void UpdateInfo();
      void StartUpdateInfo();
      void StopUpdateInfo();
      void UpdateTotalFPS();
      void UpdateCurProcFPS();
      void UpdateMenu();

      MPButtons GetButton(MIL_INT PositionX, MIL_INT PositionY);
      MIL_TEXT_CHAR* GetButtonText(MIL_INT ProcessingIndex, MPButtons ButtonType,  MIL_TEXT_CHAR* ButtonText);
      void InitProcessingMP(MIL_INT NumCores, CMPProcessing* Processing);

      MIL_INT GetRealCoreIndex(MIL_INT Rank);

      //Hook function for handling clicks on the menu
      static MIL_INT MFTYPE MenuHookFct(MIL_INT HookType, MIL_ID MilEvent, void *UserDataPtr);

      //Function that updates processing information such as FPS in a separate thread
      static MIL_UINT32 MFTYPE  UpdateInfoFunction(void* DataPtr);
   };


#endif



