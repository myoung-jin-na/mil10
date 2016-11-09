//***************************************************************************************
//
// File name: MPProcessing.h
//
// Synopsis:  Class that defines the processing functionality
//

#ifndef MPPROCESSING_H
#define MPPROCESSING_H

//*****************************************************************************
// Class used to define the processing done by the example.
// MP functionality is applied to the processing in this class
//*****************************************************************************
class CMPProcessing
   {
   public:
      CMPProcessing(const MIL_TEXT_CHAR* Title,
                    MIL_INT DisplayBufferSizeX, 
                    MIL_INT DisplayBufferSizeY,
                    MIL_INT DisplayBufferType, 
                    MIL_INT DisplayBufferSizeBand,
                    MIL_INT ProcessingIndex);

      virtual ~CMPProcessing();

      //Thread Control functions
      void StartThread();
      void StopThread();
      void Run();
      void Pause();

      //Display control functions
      void DisplaySelect();
      void DisplayDeselect();
      void RunDisplay(bool Run);

      //MP control functions
      void SetMP(bool Enable);
      void SetCoreMax(MIL_INT Max);
      void SetCoreSharing(bool Enable);
      void SetMPPriority(MIL_INT Priority);
      void SetCoreAffinity(MIL_UINT64 AffinityMask);
      void SetCurrentMemoryBank(MIL_INT64 MemoryBank, bool UseBank);

      //Thread inquire functions
      inline bool ThreadStarted() const;
      inline bool IsRunning() const;

      //Display inquire functions
      inline bool DisplaySelected() const;
      inline bool DisplayRunning() const;

      //MP inquire functions
      inline bool MPEnabled() const;
      inline MIL_INT GetCoreMax() const;
      inline bool CoreSharingEnabled() const;
      inline MIL_INT GetMPPriority() const;

      inline bool UseMemoryBank() const;
      inline MIL_INT64 GetCurrentMemoryBank() const;
      inline MIL_UINT64 GetCoreAffinity() const;
      inline MIL_DOUBLE GetFrameRate() const;
      MIL_INT64 GetMemoryBank(bool Next, bool& ValidBank) const;

   protected:
      inline MIL_INT GetBufferSizeX() const;
      inline MIL_INT GetBufferSizeY() const;
      inline MIL_INT GetBufferType() const;
      inline MIL_INT GetBufferSizeBand() const;

      inline MIL_ID  GetSystemID() const;

      inline MIL_INT GetNumMemoryBank() const;

      MIL_INT64 GetMemoryBank(MIL_INT Index, bool& ValidBank) const;
      void UpdateDisplay(MIL_ID ImageToDisplay);

      //Virtual function that must be defined in derived classes to define processing.
      virtual void Process(MIL_INT ProcessingObjectIndex) = 0;

   private:
      //Disallow copy
      CMPProcessing(const CMPProcessing&);
      CMPProcessing& operator=(const CMPProcessing&);

      void Alloc(const MIL_TEXT_CHAR* Title);
      void Free();

      MIL_INT GetMemoryBankIndex(MIL_INT64 MemoryBank) const;
      void UpdateDisplayTitle();

      MIL_ID   m_MilSystem;

      MIL_ID   m_MilDisplay;
      MIL_ID   m_MilDisplayBuffer;

      MIL_INT  m_ProcessingIndex;

      CDispatcher* m_Dispatcher;

      bool     m_DisplayRunning;
      bool     m_DisplaySelected;

      bool     m_MPEnable;
      MIL_INT  m_CoreMax;
      bool     m_CoreSharing;
      MIL_INT  m_MPPriority;

      MIL_UINT64* m_CoreAffinityMask;
      MIL_INT64   m_CurrentMemoryBank;
      MIL_INT     m_NumMemoryBank;
      MIL_INT64*  m_AvailableMemoryBanks;  
      bool        m_UseMemoryBank;
     
      MIL_TEXT_CHAR m_DisplayTitle[STRING_SIZE];

      MIL_INT     m_DisplayBufferSizeX;
      MIL_INT     m_DisplayBufferSizeY;
      MIL_INT     m_DisplayBufferType;
      MIL_INT     m_DisplayBufferSizeBand;

      static long ProcessingFunction(void* DataPtr);
   };

//*****************************************************************************
// ThreadStarted. Returns whether the processing thread has started.
//*****************************************************************************
inline bool CMPProcessing::ThreadStarted() const 
   {
   return m_Dispatcher->ThreadStarted();
   }

//*****************************************************************************
// IsRunning. Returns whether the processing is running.
//*****************************************************************************
inline bool CMPProcessing::IsRunning() const 
   {
   return m_Dispatcher->IsRunning();
   }

//*****************************************************************************
// DisplaySelected. Returns whether the display has been selected.
//*****************************************************************************
inline bool CMPProcessing::DisplaySelected() const 
   {
   return m_DisplaySelected;  
   }

//*****************************************************************************
// ThreadStarted. Returns whether the display is being updated.
//*****************************************************************************
inline bool CMPProcessing::DisplayRunning() const 
   {
   return m_DisplayRunning;   
   } 

//*****************************************************************************
// MPEnabled. Returns whether multiprocessing is enabled.
//*****************************************************************************
inline bool CMPProcessing::MPEnabled() const          
   {
   return m_MPEnable;
   }

//*****************************************************************************
// GetCoreMax. Returns the current maximum number of cores to use for this
// processing thread.
//*****************************************************************************
inline MIL_INT CMPProcessing::GetCoreMax() const   
   {
   return m_CoreMax;
   }

//*****************************************************************************
// CoreSharingEnabled. Returns whether core sharing is enabled or not.
//*****************************************************************************
inline bool CMPProcessing::CoreSharingEnabled() const 
   {
   return m_CoreSharing;
   }

//*****************************************************************************
// GetMPPriority. Returns the current priority of the MP threads.
//*****************************************************************************
inline MIL_INT CMPProcessing::GetMPPriority() const 
   {
   return m_MPPriority;
   }

//*****************************************************************************
// GetCurrentMemoryBank. Returns the current memory bank.
//*****************************************************************************
inline MIL_INT64 CMPProcessing::GetCurrentMemoryBank() const 
   { 
   return m_CurrentMemoryBank; 
   }

//*****************************************************************************
// GetCoreAffinity. Returns the core affinity mask of the processing thread.
//*****************************************************************************
inline MIL_UINT64 CMPProcessing::GetCoreAffinity() const 
   {
   return m_CoreAffinityMask[0];
   }

//*****************************************************************************
// GetFrameRate. Returns the processing frame rate.
//*****************************************************************************
inline MIL_DOUBLE CMPProcessing::GetFrameRate() const 
   { 
   return m_Dispatcher->GetFrameRate(); 
   }

//*****************************************************************************
// GetBufferSizeX. Get the size X of the display buffer.
//*****************************************************************************
inline MIL_INT CMPProcessing::GetBufferSizeX() const
   {
   return m_DisplayBufferSizeX;
   }

//*****************************************************************************
// GetBufferSizeY. Get the size Y of the display buffer.
//*****************************************************************************
inline MIL_INT CMPProcessing::GetBufferSizeY() const
   {
   return m_DisplayBufferSizeY;
   }

//*****************************************************************************
// GetBufferType. Get the type of the display buffer.
//*****************************************************************************
inline MIL_INT CMPProcessing::GetBufferType() const
   {
   return m_DisplayBufferType;
   }

//*****************************************************************************
// GetBufferSizeBand. Get the number of bands of the display buffer.
//*****************************************************************************
inline MIL_INT CMPProcessing::GetBufferSizeBand() const
   {
   return m_DisplayBufferSizeBand;
   }

//*****************************************************************************
// GetSystemID. Get the ID of the system on which the processing will be done.
//*****************************************************************************
inline MIL_ID  CMPProcessing::GetSystemID() const
   {
   return m_MilSystem;
   }

//*****************************************************************************
// UseMemoryBank. Get whether or not to use a CPU memory bank.
//*****************************************************************************
inline bool CMPProcessing::UseMemoryBank() const
   {
   return m_UseMemoryBank;
   }

//*****************************************************************************
// GetNumMemoryBank. Get the number of memory banks.
//*****************************************************************************
inline MIL_INT CMPProcessing::GetNumMemoryBank() const
   {
   return m_NumMemoryBank;
   }

#endif 
