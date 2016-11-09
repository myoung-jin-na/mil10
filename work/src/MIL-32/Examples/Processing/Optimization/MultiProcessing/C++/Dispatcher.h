//***************************************************************************************
//
// File name: Dispatcher.h
//
// Synopsis:  Class that defines the dispatcher of the processing for the MP example
//

#ifndef DISPATCHER_H
#define DISPATCHER_H

typedef long (*PROC_FUNCTION_PTR)(void* UserData);

//*****************************************************************************
// Class used to define the dispatcher of the MP processing example.
// It manages the processing thread and its associated events.
//*****************************************************************************
class CDispatcher
   {
   public:
      CDispatcher(MIL_ID MilSystem, PROC_FUNCTION_PTR ProcessingFunctionPtr, void* DataPtr);
      ~CDispatcher();

      //Dispatcher thread control operations
      void StartThread();
      void StopThread();
      void Run();
      void Pause();

      //Dispatcher inquire information functions
      inline bool ThreadStarted() const;
      inline bool IsRunning() const;

      inline MIL_DOUBLE GetFrameRate() const;

      //Functions to control multiprocessing in the dispatcher thread
      inline MIL_INT MFTYPE InquireMp( MIL_INT InquireType,
                                       MIL_INT TypeFlag,
                                       MIL_INT TypeValue,
                                       void *ResultPtr ) const;

      inline void MFTYPE ControlMp( MIL_INT ControlType,
                                    MIL_INT TypeFlag,
                                    MIL_INT TypeValue,
                                    void *ValuePtr ) const;

   private:
      //Disallow copy
      CDispatcher(const CDispatcher&);
      CDispatcher& operator=(const CDispatcher&);

      MIL_ID              m_MilSystem;
      MIL_ID              m_MilEvents[NUM_EVENTS];
      MIL_ID              m_MilDispatchThread;
      volatile MIL_DOUBLE m_FrameRate;
      bool                m_ThreadStarted;
      volatile bool       m_DispatchRunning;
      MIL_ID              m_MilDispatchStoppedEvent;

      //Variable that holds a pointer to the processing function to call in the dispatcher thread
      PROC_FUNCTION_PTR m_ProcessingFunctionPtr;
      void*             m_DataPtr;

      //Functions called from the dispatcher thread
      static MIL_UINT32 MFTYPE DispatchFunction(void *UserDataPtr);
      void RunDispatcher();
   };

//*******************************************************************************
// ThreadStarted.  Returns whether the dispatch thread has been started.
//*******************************************************************************
inline bool CDispatcher::ThreadStarted() const 
   { 
   return m_ThreadStarted; 
   }

//*******************************************************************************
// IsRunning.  Returns whether the dispatching is running.
//*******************************************************************************
inline bool CDispatcher::IsRunning() const 
   { 
   return (m_ThreadStarted&&m_DispatchRunning); 
   }

//*******************************************************************************
// GetFrameRate.  Returns the frame rate of the dispatcher
//*******************************************************************************
inline MIL_DOUBLE CDispatcher::GetFrameRate() const
   {
   return m_FrameRate;
   }

//*******************************************************************************
// InquireMp.  Inquire the state of multiprocessing relative to the threads
// created by the dispatcher.
//*******************************************************************************
inline MIL_INT MFTYPE CDispatcher::InquireMp(MIL_INT InquireType, MIL_INT TypeFlag,
                                             MIL_INT TypeValue, void *ResultPtr) const
   {
   return(MthrInquireMp(m_MilDispatchThread, InquireType, TypeFlag, TypeValue, ResultPtr));
   }

//*******************************************************************************
// ControlMp.  Control the state of multiprocessing relative to the threads
// created by the dispatcher.
//*******************************************************************************
inline void    MFTYPE CDispatcher::ControlMp(MIL_INT ControlType, MIL_INT TypeFlag,
                                             MIL_INT TypeValue, void *ValuePtr) const
   {
   MthrControlMp(m_MilDispatchThread, ControlType, TypeFlag, TypeValue, ValuePtr);
   }

#endif
