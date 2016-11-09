//***************************************************************************************/
// 
// File name: MPProcessing.cpp  
//
// Synopsis:  Implements the CMPProcessing class.
//

#include "common.h"

static const MIL_INT DISPLAY_OFFSET_X = 40;
static const MIL_INT MAX_DISPLAY_OFFSET_X = 600;


//*****************************************************************************
// Constructor. Allocates and initializes the processing objects.
//*****************************************************************************
CMPProcessing::CMPProcessing(const MIL_TEXT_CHAR* Title,
                             MIL_INT DisplayBufferSizeX, 
                             MIL_INT DisplayBufferSizeY,
                             MIL_INT DisplayBufferType, 
                             MIL_INT DisplayBufferSizeBand,
                             MIL_INT ProcessingIndex)
: m_DisplayBufferSizeX(DisplayBufferSizeX),
  m_DisplayBufferSizeY(DisplayBufferSizeY), 
  m_DisplayBufferType(DisplayBufferType),
  m_DisplayBufferSizeBand(DisplayBufferSizeBand), 
  m_ProcessingIndex(ProcessingIndex)
   {
   //Allocate processing objects
   Alloc(Title);
   }

//*****************************************************************************
// Destructor. Frees the processing objects.
//*****************************************************************************
CMPProcessing::~CMPProcessing()
   {
   //Free processing objects
   Free();
   }

//*****************************************************************************
// StartThread. Starts the processing thread.
//*****************************************************************************
void CMPProcessing::StartThread()
   {
   //Start the dispatching thread
   m_Dispatcher->StartThread();
   }

//*****************************************************************************
// StopThread. Stops the processing thread.
//*****************************************************************************
void CMPProcessing::StopThread()
   {
   //Stop the dispatching thread
   m_Dispatcher->StopThread();
   UpdateDisplayTitle();
   }

//*****************************************************************************
// Run. Runs the processing.
//*****************************************************************************
void CMPProcessing::Run()
   {
   //Run the dispatcher
   m_Dispatcher->Run();
   }

//*****************************************************************************
// Pause. Pauses the processing.
//*****************************************************************************
void CMPProcessing::Pause()
   {
   //Pause the dispatcher
   if (m_Dispatcher->IsRunning())
      {
      m_Dispatcher->Pause();
      UpdateDisplayTitle();
      }
   }

//*****************************************************************************
// SetMP. Sets the state of MP (enable or disable).
//*****************************************************************************
void CMPProcessing::SetMP(bool Enable)
   {
   //Set whether or not MP is enabled 
   m_MPEnable = Enable;
   m_Dispatcher->ControlMp(M_MP_USE, M_DEFAULT, (m_MPEnable)?M_ENABLE:M_DISABLE, M_NULL);
   }

//*****************************************************************************
// RunDisplay. Sets the state of the display (running or paused).
//*****************************************************************************
void CMPProcessing::RunDisplay(bool Run)
   {
   //Set whether or not the display is running.
   m_DisplayRunning = Run;
   }


//*****************************************************************************
// SetCoreMax. Sets the maxixum number of cores to be used by MP.
//*****************************************************************************
void CMPProcessing::SetCoreMax(MIL_INT Max)
   {
   //Set the maximum cores to use for this processing thread
   m_CoreMax=Max;
   m_Dispatcher->ControlMp(M_CORE_MAX, M_DEFAULT, m_CoreMax, M_NULL);
   }

//*****************************************************************************
// SetCoreSharing. Sets the state of core sharing in MP.
//*****************************************************************************
void CMPProcessing::SetCoreSharing(bool Enable)
   {
   //Set whether core sharing is enabled or not
   m_CoreSharing=Enable;
   m_Dispatcher->ControlMp(M_CORE_SHARING, M_DEFAULT, (m_CoreSharing)?M_ENABLE:M_DISABLE, M_NULL);
   }

//*****************************************************************************
// SetMPPriority. Sets the processing priority in MP.
//*****************************************************************************
void CMPProcessing::SetMPPriority(MIL_INT Priority)
   {
   //Set the thread priority
   m_MPPriority = Priority;
   m_Dispatcher->ControlMp(M_MP_PRIORITY, M_DEFAULT, m_MPPriority, M_NULL);
   }

//*****************************************************************************
// SetCoreAffinity. Sets the affinity to the given core.
//*****************************************************************************
void CMPProcessing::SetCoreAffinity(MIL_UINT64 AffinityMask)
   {
   //Set the core affinity of this thread
   m_CoreAffinityMask[0]=AffinityMask;
   
   m_Dispatcher->ControlMp(M_CORE_AFFINITY_MASK, M_DEFAULT, M_USER_DEFINED, m_CoreAffinityMask);
   }

//*****************************************************************************
// SetCurrentMemoryBank. Sets the current memory bank.
//*****************************************************************************
void CMPProcessing::SetCurrentMemoryBank(MIL_INT64 MemoryBank, bool UseBank)
   { 
   //Set the memory bank to use for buffers used in this thread
   m_CurrentMemoryBank = MemoryBank; 
   m_UseMemoryBank = UseBank;
   }

//*****************************************************************************
// GetMemoryBank. Gets the next memory bank available.
//*****************************************************************************
MIL_INT64 CMPProcessing::GetMemoryBank(bool Next, bool& ValidBank) const
   { 
   MIL_INT64 NewMemoryBank=0;
   ValidBank = false;
   if (Next)
      {
      //Get the next memory bank
      MIL_INT NextBankIndex = GetMemoryBankIndex(m_CurrentMemoryBank);
      if ( (NextBankIndex>=0) && (NextBankIndex<m_NumMemoryBank-1))
         NextBankIndex++;
      NewMemoryBank = GetMemoryBank(NextBankIndex, ValidBank);
      }
   else
      {
      //Get the previous memory bank
      MIL_INT PreviousBankIndex = GetMemoryBankIndex(m_CurrentMemoryBank);
      if (PreviousBankIndex>=1)
         PreviousBankIndex = PreviousBankIndex--;;
      NewMemoryBank = GetMemoryBank(PreviousBankIndex, ValidBank);
      }

   return NewMemoryBank;
   }


//*****************************************************************************
// Alloc. Allocate processing objects.
//*****************************************************************************
void CMPProcessing::Alloc(const MIL_TEXT_CHAR* Title)
   {
   // Allocate MIL objects. 
   MsysAlloc(M_DEFAULT, M_SYSTEM_HOST, M_DEFAULT, M_DEFAULT, &m_MilSystem);
   MdispAlloc(m_MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &m_MilDisplay);

#if M_MIL_USE_WINDOWS
   MdispControl(m_MilDisplay, M_WINDOW_SYSBUTTON, M_DISABLE);
#endif

   //Change the display window title
   MosSprintf(m_DisplayTitle, STRING_SIZE, MIL_TEXT("%s"), Title);
   MdispControl(m_MilDisplay, M_TITLE, M_PTR_TO_DOUBLE(m_DisplayTitle));   

   //Allocate the dispatcher
   m_Dispatcher = new CDispatcher(m_MilSystem, (PROC_FUNCTION_PTR)&ProcessingFunction, (void*)this);

   m_DisplayRunning = false;
   m_DisplaySelected = false;

   m_MPEnable = false;
   m_CoreMax = -1;
   m_CoreSharing = false;
   m_MPPriority = -1;

   //Initialize core affinity information
   MIL_INT CoreAffinityMaskArraySize = 0;
   MappInquireMp(M_DEFAULT, M_CORE_AFFINITY_MASK_ARRAY_SIZE, M_DEFAULT, M_NULL, &CoreAffinityMaskArraySize);

   m_CoreAffinityMask = new MIL_UINT64 [CoreAffinityMaskArraySize];

   for (MIL_INT i=0; i<CoreAffinityMaskArraySize; i++)
      {
      m_CoreAffinityMask[i]=0;
      }
   
   //Initialize memory bank affinity information
   m_CurrentMemoryBank = 0;  
   m_UseMemoryBank = false;

   //Create array of available memory banks
   MIL_INT MemoryBankAffinityArraySize = 0;
   MappInquireMp(M_DEFAULT, M_MEMORY_BANK_AFFINITY_MASK_ARRAY_SIZE, M_DEFAULT, M_NULL, &MemoryBankAffinityArraySize);

   MIL_UINT64* MemoryBankAffinityMask = new MIL_UINT64 [MemoryBankAffinityArraySize];

   for (MIL_INT i=0; i<MemoryBankAffinityArraySize; i++)
      {
      MemoryBankAffinityMask[i]=0;
      }   

   MappInquireMp(M_DEFAULT, M_MEMORY_BANK_AFFINITY_MASK, M_LOCAL, M_NULL, MemoryBankAffinityMask);   

   //All possible memory banks
   static const MIL_INT   NUM_MAX_MEMORY_BANK = 7;
   static const MIL_INT64 ALL_MEMORY_BANKS[NUM_MAX_MEMORY_BANK] =
      {
      M_MEMORY_BANK_0, M_MEMORY_BANK_1, M_MEMORY_BANK_2, M_MEMORY_BANK_3,
      M_MEMORY_BANK_4, M_MEMORY_BANK_5, M_MEMORY_BANK_6
      };

   //Initialize the number of memory banks (maximum is 7 for this example)
   m_NumMemoryBank = 0;
   m_AvailableMemoryBanks = M_NULL;
   MIL_INT MaxNumMemoryBanks = MappInquireMp(M_DEFAULT, M_MEMORY_BANK_NUM, M_DEFAULT, M_NULL, M_NULL);

   if (MaxNumMemoryBanks > 0)
      {
      m_AvailableMemoryBanks = new MIL_INT64[MaxNumMemoryBanks];

      //Put the M_MEMORY_BANK_n constants (corresponding to the affinity mask) inside the array.
      for (MIL_INT i=0; i<NUM_MAX_MEMORY_BANK; i++)
         {
         if ((MemoryBankAffinityMask[0] & ((MIL_UINT64)1 << i)) != 0)
            {
            m_AvailableMemoryBanks[m_NumMemoryBank++] = ALL_MEMORY_BANKS[i];
            }
         }
      }

   delete [] MemoryBankAffinityMask;

   //Allocate and clear the display buffer
   MbufAllocColor(m_MilSystem, m_DisplayBufferSizeBand, m_DisplayBufferSizeX, m_DisplayBufferSizeY, 
      m_DisplayBufferType, M_IMAGE+M_PROC+M_DISP, &m_MilDisplayBuffer);  

   MbufClear(m_MilDisplayBuffer, 0.0);
   }

//*****************************************************************************
// Free. Free processing objects.
//*****************************************************************************
void CMPProcessing::Free()
   {
   //Deallocate memory
   delete m_Dispatcher;
   delete [] m_CoreAffinityMask;

   m_Dispatcher = M_NULL;
   m_CoreAffinityMask = M_NULL;
   
   //Free MIL objects
   MbufFree(m_MilDisplayBuffer);
   MdispFree(m_MilDisplay);
   MsysFree(m_MilSystem);
   
   delete [] m_AvailableMemoryBanks;
   }

//*****************************************************************************
// UpdateDisplayTitle. Updates the processing display title
//*****************************************************************************
void CMPProcessing::UpdateDisplayTitle()
   {
   //Update FPS in the display title
   MIL_TEXT_CHAR TitleText[STRING_SIZE] = MIL_TEXT("");
   MosSprintf(TitleText, STRING_SIZE, MIL_TEXT("%s   %.1f frames per second"), m_DisplayTitle, 
      m_Dispatcher->GetFrameRate());
   MdispControl(m_MilDisplay, M_TITLE, M_PTR_TO_DOUBLE(TitleText));   
   }

//*****************************************************************************
// DisplaySelect. Select the buffer on the display
//*****************************************************************************
void CMPProcessing::DisplaySelect()
   {
   //Select the buffer on the display
   if (!m_DisplaySelected)
      {
      //Set an x offset so we can see a part of all displays
      MdispControl(m_MilDisplay, M_WINDOW_INITIAL_POSITION_X, 
         (m_ProcessingIndex*DISPLAY_OFFSET_X) % MAX_DISPLAY_OFFSET_X);

      MdispControl(m_MilDisplay, M_WINDOW_INITIAL_POSITION_Y, 0);

      MdispSelect(m_MilDisplay, m_MilDisplayBuffer);
      m_DisplaySelected = true;
      }
   }

//*****************************************************************************
// DisplayDeselect. Select the buffer on the display
//*****************************************************************************
void CMPProcessing::DisplayDeselect()
   {
   //Deslect the buffer on the display
   if (m_DisplaySelected)
      {
      MdispSelect(m_MilDisplay, M_NULL);
      m_DisplaySelected = false;
      }
   }

//*****************************************************************************
// GetMemoryBank. Returns the memory bank define associated to the index.
// It returns 0 if the memory bank is not valid.
//*****************************************************************************
MIL_INT64 CMPProcessing::GetMemoryBank(MIL_INT Index, bool& ValidBank) const
   {
   //Get the memory bank associated to the given index
   MIL_INT64 ReturnValue = 0;
   ValidBank = false;

   if ( (Index>=0) && (Index<m_NumMemoryBank) )
      {
      ReturnValue = m_AvailableMemoryBanks[Index]; 
      ValidBank = true;
      }

   return ReturnValue;
   }

//*****************************************************************************
// UpdateDisplay. Copies the given image to the display if it is not disabled 
// and updates the information in the display title.
//*****************************************************************************
void CMPProcessing::UpdateDisplay(MIL_ID ImageToDisplay)
   {
   //Copy the result to the display if it is not disabled
   if (m_DisplaySelected && m_DisplayRunning)
      MbufCopy(ImageToDisplay, m_MilDisplayBuffer);

   //Update the information in the display title
   UpdateDisplayTitle();
   }

//*****************************************************************************
// GetMemoryBankIndex. Returns the index of the memory bank (place in array)
//**************************************************************************f ***
MIL_INT CMPProcessing::GetMemoryBankIndex(MIL_INT64 MemoryBank) const
   {
   MIL_INT BankIndex=-1;

   if (m_NumMemoryBank > 0)
      {
      //Get the index of the given memory bank
      for (BankIndex=0; BankIndex<m_NumMemoryBank; BankIndex++)
         {
         if (m_AvailableMemoryBanks[BankIndex]==MemoryBank)
           break;
         }
      }

   return (BankIndex);
   }
   
//*****************************************************************************
// ProcessingFunction. Function that calls the processing to do.
//*****************************************************************************
long CMPProcessing::ProcessingFunction(void* DataPtr)
   {
   CMPProcessing* Processing = (CMPProcessing*)DataPtr;

   //Get the index of the CPU memory bank that has been selected. 0 is the index
   //where no memory bank was specified during allocation.
   MIL_INT ProcessingObjectIndex = 0;
   if (Processing->UseMemoryBank())
      ProcessingObjectIndex = Processing->GetMemoryBankIndex(Processing->GetCurrentMemoryBank())+1;

   //Run the processing
   Processing->Process(ProcessingObjectIndex);
   return 0;
   }
