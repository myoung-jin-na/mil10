/*****************************************************************************/
/* 
 * File name: MappBenchmark.cpp
 *
 * Synopsis:  This program uses the MappTimer function to demonstrate the 
 *            benchmarking of MIL functions. It can be used as a template 
 *            to benchmark any MIL or custom processing function accurately.
 *
 *            It takes into account different factors that can influence 
 *            the timing such as dll load time and OS inaccuracy when 
 *            measuring a very short time.
 */
#include <mil.h>

/* Target MIL image specifications. */
#define IMAGE_FILE   M_IMAGE_PATH MIL_TEXT("LargeWafer.mim")
#define ROTATE_ANGLE -15

/* Timing loop iterations setting. */
#define MINIMUM_BENCHMARK_TIME 2.0 /* In seconds (1.0 and more recommended). */
#define ESTIMATION_NB_LOOP      10
#define DEFAULT_NB_LOOP        100

/* Processing function parameters structure. */
typedef struct 
   {
   MIL_ID MilSourceImage;        /* Image buffer identifier. */
   MIL_ID MilDestinationImage;   /* Image buffer identifier. */
   } PROC_PARAM;

/* Declaration of the benchmarking function. */
void Benchmark(PROC_PARAM& ProcParamPtr, MIL_DOUBLE& Time, MIL_DOUBLE& FramesPerSecond);

/* Declarations of the target processing functions. You can insert your 
   own intialization, execution and free operations in these functions. 
*/
void ProcessingInit(MIL_ID MilSystem, PROC_PARAM& ProcParamPtr);
void ProcessingExecute(PROC_PARAM& ProcParamPtr);
void ProcessingFree(PROC_PARAM& ProcParamPtr);

int MosMain(void)
   {
   MIL_ID MilApplication,              /* Application identifier.  */
          MilSystem,                   /* System identifier.       */
          MilDisplay,                  /* Display identifier.      */
          MilDisplayImage,             /* Image buffer identifier. */   
          MilSystemOwnerApplication,   /* System's owner application.          */
          MilSystemCurrentThreadId;    /* System's current thread identifier.  */
   PROC_PARAM ProcessingParam;         /* Processing parameters.               */
   MIL_DOUBLE TimeAllCores, TimeAllCoresNoCS, TimeOneCore; /* Timer variables. */
   MIL_DOUBLE FPSAllCores, FPSAllCoresNoCS, FPSOneCore;    /* FPS variables.   */
   MIL_INT    NbCoresUsed, NbCoresUsedNoCS;                /* Number of CPU Core used.   */
   bool       UseGPUSystem;                                /* Is default system used GPU.*/ 
   
   /* Allocate defaults. */
   MappAllocDefault(M_DEFAULT, &MilApplication, &MilSystem,
                               &MilDisplay, M_NULL, M_NULL);

   /* Get the system's owner application.*/
   MsysInquire(MilSystem, M_OWNER_APPLICATION, &MilSystemOwnerApplication);

   /* Get the system's current thread identifier. */
   MsysInquire(MilSystem, M_CURRENT_THREAD_ID, &MilSystemCurrentThreadId);

   /* Determine if the processing will be done on the GPU. */
   UseGPUSystem = MsysInquire(MilSystem, M_SYSTEM_TYPE, M_NULL) == M_SYSTEM_GPU_TYPE;

   /* Restore image into an automatically allocated image buffer. */
   MbufRestore(IMAGE_FILE, MilSystem, &MilDisplayImage);

   /* Display the image buffer. */
   MdispSelect(MilDisplay, MilDisplayImage);

   /* Allocate the processing objects. */
   ProcessingInit(MilSystem, ProcessingParam);

   /* Pause to show the original image. */
   MosPrintf(MIL_TEXT("\nPROCESSING FUNCTION BENCHMARKING:\n"));
   MosPrintf(MIL_TEXT("---------------------------------\n\n"));
   MosPrintf(MIL_TEXT("This program times a processing function under "));
   MosPrintf(MIL_TEXT("different conditions.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to start.\n\n"));
   MosGetch();
   MosPrintf(MIL_TEXT("PROCESSING TIME FOR %ldx%ld:\n" ),
             MbufInquire(ProcessingParam.MilDestinationImage, M_SIZE_X, NULL),
             MbufInquire(ProcessingParam.MilDestinationImage, M_SIZE_Y, NULL));
   MosPrintf(MIL_TEXT("------------------------------\n\n"));

   if (!UseGPUSystem)
      {
      /* Benchmark the processing function with multi-processing. */
      /* ---------------------------------------------------------*/
      MappControlMp(MilSystemOwnerApplication, M_MP_USE, M_DEFAULT, M_ENABLE, M_NULL);   
      MappControlMp(MilSystemOwnerApplication, M_CORE_SHARING, M_DEFAULT, M_ENABLE, M_NULL); 
      MthrInquireMp(MilSystemCurrentThreadId, M_CORE_NUM_EFFECTIVE, M_DEFAULT, M_DEFAULT, &NbCoresUsed);
      if (NbCoresUsed > 1)
         {
         Benchmark(ProcessingParam, TimeAllCores, FPSAllCores);

         /* Show the resulting image and the timing results. */
         MbufCopy(ProcessingParam.MilDestinationImage, MilDisplayImage);
   
         MosPrintf(MIL_TEXT("Using multi-processing   (%3ld CPU cores): %5.3f ms (%6.1f fps)\n"), 
                   NbCoresUsed, TimeAllCores, FPSAllCores);
         }
      MappControlMp(MilSystemOwnerApplication, M_MP_USE, M_DEFAULT, M_DEFAULT, M_NULL);   
      MappControlMp(MilSystemOwnerApplication, M_CORE_SHARING, M_DEFAULT, M_DEFAULT, M_NULL); 

     /* Benchmark the processing function with multi-processing but no hyper-threading. */
     /* --------------------------------------------------------------------------------*/

      /* If Hyper-threading is available on the PC, test if the performance is better 
         with it disabled. Sometimes, too many cores sharing the same CPU resources that
         are already fully occupied can reduce the overall performance.
      */
      MappControlMp(MilSystemOwnerApplication, M_MP_USE, M_DEFAULT, M_ENABLE, M_NULL);   
      MappControlMp(MilSystemOwnerApplication, M_CORE_SHARING, M_DEFAULT, M_DISABLE, M_NULL); 
      MthrInquireMp(MilSystemCurrentThreadId, M_CORE_NUM_EFFECTIVE, M_DEFAULT, M_DEFAULT, &NbCoresUsedNoCS);
      if (NbCoresUsedNoCS != NbCoresUsed)
         {
         Benchmark(ProcessingParam, TimeAllCoresNoCS, FPSAllCoresNoCS);

         /* Show the resulting image and the timing results. */
         MbufCopy(ProcessingParam.MilDestinationImage, MilDisplayImage);
         MosPrintf(MIL_TEXT("Using multi-processing   (%3ld CPU cores): %5.3f ms (%6.1f fps), no Hyper-Thread.\n"), 
                   NbCoresUsedNoCS, TimeAllCoresNoCS, FPSAllCoresNoCS);
         }
      MappControlMp(MilSystemOwnerApplication, M_MP_USE, M_DEFAULT, M_DEFAULT, M_NULL);   
      MappControlMp(MilSystemOwnerApplication, M_CORE_SHARING, M_DEFAULT, M_DEFAULT, M_NULL);  
 
      /* Benchmark the processing function without multi-processing. */
      /* ------------------------------------------------------------*/
      MappControlMp(MilSystemOwnerApplication, M_MP_USE, M_DEFAULT, M_DISABLE, M_NULL);   
      Benchmark(ProcessingParam, TimeOneCore, FPSOneCore);

      /* Show the resulting image and the timing results. */
      MbufCopy(ProcessingParam.MilDestinationImage, MilDisplayImage);
      MosPrintf(MIL_TEXT("Without multi-processing (  1 CPU core ): %5.3f ms (%6.1f fps)\n\n"),
                TimeOneCore, FPSOneCore);               
      MappControlMp(MilSystemOwnerApplication, M_MP_USE, M_DEFAULT, M_DEFAULT, M_NULL);   
    
      /* Show a comparative summary of the timing results. */
      if (NbCoresUsed > 1)
         {
         MosPrintf(MIL_TEXT("Benchmark is %.1f times faster with multi-processing.\n"), 
                   TimeOneCore/TimeAllCores);
         }
      if (NbCoresUsedNoCS != NbCoresUsed)
         {
         MosPrintf(MIL_TEXT("Benchmark is %.1f times faster with multi-processing and no Hyper-Thread.\n\n"), 
                   TimeOneCore/TimeAllCoresNoCS);
         }
      }
   else
      {
      /* Benchmark on GPU system. */
      Benchmark(ProcessingParam, TimeAllCores, FPSAllCores);

      /* Show the resulting image and the timing results. */
      MbufCopy(ProcessingParam.MilDestinationImage, MilDisplayImage);
   
      MosPrintf(MIL_TEXT("Using GPU: %5.3f ms (%6.1f fps)\n"), TimeAllCores, FPSAllCores);
      }

   /* Wait for a key press. */
   MosPrintf(MIL_TEXT("Press <Enter> to end.\n"));
   MosGetch();

   /* Free all allocations. */
   ProcessingFree(ProcessingParam);
   MdispSelect(MilDisplay, M_NULL);
   MbufFree(MilDisplayImage);
   MappFreeDefault(MilApplication, MilSystem, MilDisplay, M_NULL, M_NULL);   
   return 0;
   }

/*****************************************************************************
Benchmark function. 
*****************************************************************************/
void Benchmark(PROC_PARAM& ProcParamPtr, MIL_DOUBLE& Time, MIL_DOUBLE& FramesPerSecond)
   {
   MIL_INT EstimatedNbLoop = DEFAULT_NB_LOOP;
   MIL_DOUBLE StartTime, EndTime;
   MIL_DOUBLE MinTime = 0;
   MIL_INT n;

   /* Wait for the completion of all functions in this thread. */
   MthrWait(M_DEFAULT, M_THREAD_WAIT, M_NULL);

   /* Call the processing once before benchmarking for a more accurate time.
      This compensates for Dll load time, etc.
   */
   MappTimer(M_DEFAULT, M_TIMER_READ, &StartTime);
   ProcessingExecute(ProcParamPtr);  
   MthrWait(M_DEFAULT, M_THREAD_WAIT, M_NULL);
   MappTimer(M_DEFAULT, M_TIMER_READ, &EndTime);

   MinTime = EndTime-StartTime;

   /* Estimate the number of loops required to benchmark the processing for 
      the specified minimum time.
   */
   for (n = 0; n < ESTIMATION_NB_LOOP; n++)
      {
      MappTimer(M_DEFAULT, M_TIMER_READ, &StartTime);
      ProcessingExecute(ProcParamPtr);
      MthrWait(M_DEFAULT, M_THREAD_WAIT, M_NULL);
      MappTimer(M_DEFAULT, M_TIMER_READ, &EndTime);

      Time = EndTime-StartTime;
      MinTime = (Time<MinTime)?Time:MinTime;
      }
   if (MinTime > 0) 
      EstimatedNbLoop = (MIL_INT)(MINIMUM_BENCHMARK_TIME/MinTime)+1;

   /* Benchmark the processing according to the estimated number of loops. */
   MappTimer(M_DEFAULT, M_TIMER_READ, &StartTime);
   for (n = 0; n < EstimatedNbLoop; n++)
      ProcessingExecute(ProcParamPtr);
   MthrWait(M_DEFAULT, M_THREAD_WAIT, M_NULL);
   MappTimer(M_DEFAULT, M_TIMER_READ, &EndTime);

   Time = EndTime-StartTime;

   FramesPerSecond = EstimatedNbLoop/Time;
   Time = Time*1000/EstimatedNbLoop;
   }

/*****************************************************************************
Processing initialization function.
*****************************************************************************/
void ProcessingInit(MIL_ID MilSystem, PROC_PARAM& ProcParamPtr)
   {
   /* Allocate a MIL source buffer. */
   MbufAllocColor(MilSystem, 
            MbufDiskInquire(IMAGE_FILE, M_SIZE_BAND, M_NULL),
            MbufDiskInquire(IMAGE_FILE, M_SIZE_X, M_NULL),
            MbufDiskInquire(IMAGE_FILE, M_SIZE_Y, M_NULL),
            MbufDiskInquire(IMAGE_FILE, M_SIZE_BIT, M_NULL)+M_UNSIGNED,
            M_IMAGE+M_PROC, &ProcParamPtr.MilSourceImage);

   /* Load the image into the source image. */ 
   MbufLoad(IMAGE_FILE, ProcParamPtr.MilSourceImage);

   /* Allocate a MIL destination buffer. */
   MbufAllocColor(MilSystem, 
            MbufDiskInquire(IMAGE_FILE, M_SIZE_BAND, M_NULL),
            MbufDiskInquire(IMAGE_FILE, M_SIZE_X, M_NULL),
            MbufDiskInquire(IMAGE_FILE, M_SIZE_Y, M_NULL),
            MbufDiskInquire(IMAGE_FILE, M_SIZE_BIT, M_NULL)+M_UNSIGNED,
            M_IMAGE+M_PROC, &ProcParamPtr.MilDestinationImage);
   }

/*****************************************************************************
Processing execution function.
*****************************************************************************/
void ProcessingExecute(PROC_PARAM& ProcParamPtr)
   {
   MimRotate(ProcParamPtr.MilSourceImage, ProcParamPtr.MilDestinationImage, ROTATE_ANGLE,
             M_DEFAULT, M_DEFAULT, M_DEFAULT, M_DEFAULT, M_BILINEAR+M_OVERSCAN_CLEAR);
   }

/*****************************************************************************
Processing free function.
*****************************************************************************/
void ProcessingFree(PROC_PARAM& ProcParamPtr)
   {
   /* Free all processing allocations. */ 
   MbufFree(ProcParamPtr.MilSourceImage);
   MbufFree(ProcParamPtr.MilDestinationImage);
   }
