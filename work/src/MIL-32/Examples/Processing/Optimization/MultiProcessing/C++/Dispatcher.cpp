//***************************************************************************************/
// 
// File name: Dispatcher.cpp  
//
// Synopsis:  Implements the CDispatcher class.
//
#include "common.h"

//*******************************************************************************
// Constructor.  Allocates and initializes the dispatcher
//*******************************************************************************
CDispatcher::CDispatcher(MIL_ID MilSystem, PROC_FUNCTION_PTR ProcessingFunctionPtr, void* DataPtr)
: m_MilSystem(MilSystem), m_ProcessingFunctionPtr(ProcessingFunctionPtr), m_DataPtr(DataPtr), 
  m_ThreadStarted(false), m_DispatchRunning(false)
   {   
   }

//*******************************************************************************
// Destructor.  Frees the dispatcher objects.
//*******************************************************************************
CDispatcher::~CDispatcher()
   {
   StopThread();
   }

//*******************************************************************************
// StartThread.  Start the dispatcher thread.  Initially the thread is paused.
//*******************************************************************************
void CDispatcher::StartThread()
   {
   if (!m_ThreadStarted)
      {
      //Allocate the dispatcher thread and events
      MthrAlloc (m_MilSystem, M_EVENT, M_NOT_SIGNALED+M_AUTO_RESET, M_NULL, M_NULL, &m_MilEvents[enRun]);
      MthrAlloc (m_MilSystem, M_EVENT, M_NOT_SIGNALED+M_AUTO_RESET, M_NULL, M_NULL, &m_MilEvents[enKill]);
      MthrAlloc (m_MilSystem, M_EVENT, M_NOT_SIGNALED+M_AUTO_RESET, M_NULL, M_NULL, &m_MilDispatchStoppedEvent);

      //Allocate a thread, it is initially paused.
      MthrAlloc (m_MilSystem, M_THREAD, M_DEFAULT, &DispatchFunction, (void*)this, &m_MilDispatchThread);
      m_ThreadStarted = true;
      }
   }

//*******************************************************************************
// StopThread.  Stop the thread and free related events.
//*******************************************************************************
void CDispatcher::StopThread()
   {
   if (m_ThreadStarted)
      {
      //Stop the thread
      Pause();
      MthrControl(m_MilEvents[enKill], M_EVENT_SET, M_SIGNALED);

      //Free related objects
      MthrWait(m_MilDispatchThread, M_THREAD_END_WAIT, M_NULL);
      MthrFree(m_MilEvents[enRun]);
      MthrFree(m_MilEvents[enKill]);
      MthrFree(m_MilDispatchStoppedEvent);
      MthrFree(m_MilDispatchThread);
      m_ThreadStarted = false;
      }
   }

//*******************************************************************************
// Run.  Runs the processing in the dispatch thread.
//*******************************************************************************
void CDispatcher::Run()
   {
   if (!m_DispatchRunning && m_ThreadStarted)
      {
      m_DispatchRunning = true;

      //Signal to start processing
      MthrControl(m_MilEvents[enRun], M_EVENT_SET, M_SIGNALED);
      }
   }

//*******************************************************************************
// Pause.  Pauses the processing in the dispatch thread.
//*******************************************************************************
void CDispatcher::Pause()
   {
   if (m_DispatchRunning)
      {
      //Signal to stop the processing
      m_DispatchRunning = false;

      //Wait for the dispatching to stop
      MthrWait(m_MilDispatchStoppedEvent, M_EVENT_WAIT, M_NULL);
      m_FrameRate = 0.0;
      }
   }

//*******************************************************************************
// RunDispatcher.  Function called in DispatchFunction to copy the buffer and
// manage the call to the user function.
//*******************************************************************************
void CDispatcher::RunDispatcher()
   {
   const MIL_INT UpdateInterval = 2;
   MIL_DOUBLE EndTime;
   MIL_INT NumFrames=0;
   MIL_DOUBLE TotalTime;
   MIL_DOUBLE PreviousUpdateTime = 0.0;
   m_FrameRate = 0;

   //Wait for an event
   while ( MthrWaitMultiple(m_MilEvents, NUM_EVENTS, M_EVENT_WAIT, M_NULL) != (MIL_INT)enKill )
      {
      MappTimer(M_TIMER_READ, &PreviousUpdateTime);

      //Call and benchmark the processing function while the stop signal has not been received
      while (m_DispatchRunning)
         {
         //Call the processing function
         m_ProcessingFunctionPtr(m_DataPtr);
         
         MthrWait(M_DEFAULT, M_THREAD_WAIT, M_NULL);
         
         //Time the processing and calculate the FPS
         NumFrames++;
         MappTimer(M_TIMER_READ, &EndTime);

         if ((EndTime-PreviousUpdateTime)>UpdateInterval)
            {
            TotalTime = EndTime-PreviousUpdateTime;

            m_FrameRate = NumFrames / TotalTime;
            NumFrames = 0;
            PreviousUpdateTime=EndTime;
            }
         }

      //Signal that the dispatcher has stopped
      MthrControl(m_MilDispatchStoppedEvent, M_EVENT_SET, M_SIGNALED);
      }   
   }

//*******************************************************************************
// DispatchFunction.  Call back function for the dispatch thread.
//*******************************************************************************
MIL_UINT32 MFTYPE CDispatcher::DispatchFunction(void *UserDataPtr)
   {
   CDispatcher* Dispatcher = (CDispatcher*)UserDataPtr;

   //Run the dispatcher
   Dispatcher->RunDispatcher();

   return 0;
   }
