//*****************************************************************************
// 
// File name: MappBenchmark.cs
//
// Synopsis:  This program uses the MappTimer function to demonstrate the 
//            benchmarking of MIL functions. It can be used as a template 
//            to benchmark any MIL or custom processing function accurately.
//
//            It takes into account different factors that can influence 
//            the timing such as dll load time and OS inaccuracy when 
//            measuring a very short time.
//*****************************************************************************
using System;

using Matrox.MatroxImagingLibrary;

namespace MAppBenchmark
{
    // Processing function parameters structure.
    public struct PROC_PARAM
    {
        public MIL_ID MilSourceImage;        // Image buffer identifier.
        public MIL_ID MilDestinationImage;   // Image buffer identifier.
    }

    public class Program
    {
        // Target MIL image specifications.
        private static readonly string IMAGE_FILE = MIL.M_IMAGE_PATH + "LargeWafer.mim";
        private const int ROTATE_ANGLE = -15;

        // Timing loop iterations setting.
        private const double MINIMUM_BENCHMARK_TIME = 1.0; // In seconds (1.0 and more recommended).
        private const int ESTIMATION_NB_LOOP = 5;
        private const int DEFAULT_NB_LOOP = 100;

        static void Main(string[] args)
        {
            MIL_ID MilApplication = MIL.M_NULL;             // Application identifier.
            MIL_ID MilSystem = MIL.M_NULL;                  // System identifier.
            MIL_ID MilDisplay = MIL.M_NULL;                 // Display identifier.
            MIL_ID MilDisplayImage = MIL.M_NULL;            // Image buffer identifier.
            MIL_ID MilSystemOwnerApplication = MIL.M_NULL;  // System's owner application.
            MIL_ID MilSystemCurrentThreadId = MIL.M_NULL;   // System's current thread identifier.
            PROC_PARAM ProcessingParam = new PROC_PARAM();  // Processing parameters.
            double TimeAllCores = 0.0;                      // Timer variables.
            double TimeAllCoresNoCS = 0.0;
            double TimeOneCore = 0.0;
            double FPSAllCores = 0.0;                       // FPS variables.
            double FPSAllCoresNoCS = 0.0;
            double FPSOneCore = 0.0;
            MIL_INT NbCoresUsed = 0;                        // Number of CPU Core used.
            MIL_INT NbCoresUsedNoCS = 0;
            bool UseGPUSystem = false;

            // Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, ref MilApplication, ref MilSystem, ref MilDisplay, MIL.M_NULL, MIL.M_NULL);

            // Get the system's owner application.
            MIL.MsysInquire(MilSystem, MIL.M_OWNER_APPLICATION, ref MilSystemOwnerApplication);

            // Get the system's current thread identifier.
            MIL.MsysInquire(MilSystem, MIL.M_CURRENT_THREAD_ID, ref MilSystemCurrentThreadId);

            // Determine if the processing will be done on the GPU.
            UseGPUSystem = MIL.MsysInquire(MilSystem, MIL.M_SYSTEM_TYPE, MIL.M_NULL) == MIL.M_SYSTEM_GPU_TYPE;

            // Restore image into an automatically allocated image buffer.
            MIL.MbufRestore(IMAGE_FILE, MilSystem, ref MilDisplayImage);

            // Display the image buffer.
            MIL.MdispSelect(MilDisplay, MilDisplayImage);

            // Allocate the processing objects.
            ProcessingInit(MilSystem, ref ProcessingParam);

            // Pause to show the original image.
            Console.WriteLine();
            Console.WriteLine("PROCESSING FUNCTION BENCHMARKING:");
            Console.WriteLine("---------------------------------");
            Console.WriteLine();
            Console.WriteLine("This program times a processing function under ");
            Console.WriteLine("different conditions.");
            Console.WriteLine("Press <Enter> to start.");
            Console.WriteLine();
            Console.ReadKey();
            Console.WriteLine("PROCESSING TIME FOR {0}x{1}:",
                MIL.MbufInquire(ProcessingParam.MilDestinationImage, MIL.M_SIZE_X, MIL.M_NULL),
                MIL.MbufInquire(ProcessingParam.MilDestinationImage, MIL.M_SIZE_Y, MIL.M_NULL));
            Console.WriteLine("------------------------------");
            Console.WriteLine();

            if (!UseGPUSystem)
            {
                // Benchmark the processing function with multi-processing.
                // --------------------------------------------------------
                MIL.MappControlMp(MilSystemOwnerApplication, MIL.M_MP_USE, MIL.M_DEFAULT, MIL.M_ENABLE, MIL.M_NULL);
                MIL.MappControlMp(MilSystemOwnerApplication, MIL.M_CORE_SHARING, MIL.M_DEFAULT, MIL.M_ENABLE, MIL.M_NULL);
                MIL.MthrInquireMp(MilSystemCurrentThreadId, MIL.M_CORE_NUM_EFFECTIVE, MIL.M_DEFAULT, MIL.M_DEFAULT, ref NbCoresUsed);
                if (NbCoresUsed > 1)
                {
                    Benchmark(ref ProcessingParam, ref TimeAllCores, ref FPSAllCores);

                    // Show the resulting image and the timing results.
                    MIL.MbufCopy(ProcessingParam.MilDestinationImage, MilDisplayImage);

                    Console.WriteLine("Using multi-processing   ({0,3} CPU cores): {1,5:0.000} ms ({2,6:0.0} fps)", NbCoresUsed, TimeAllCores, FPSAllCores);
                }
                MIL.MappControlMp(MilSystemOwnerApplication, MIL.M_MP_USE, MIL.M_DEFAULT, MIL.M_DEFAULT, MIL.M_NULL);
                MIL.MappControlMp(MilSystemOwnerApplication, MIL.M_CORE_SHARING, MIL.M_DEFAULT, MIL.M_DEFAULT, MIL.M_NULL);

                // Benchmark the processing function with multi-processing but no hyper-threading.
                // --------------------------------------------------------------------------------

                // If Hyper-threading is available on the PC, test if the performance is better 
                // with it disabled. Sometimes, too many cores sharing the same CPU resources that
                // are already fully occupied can reduce the overall performance.
                //
                MIL.MappControlMp(MilSystemOwnerApplication, MIL.M_MP_USE, MIL.M_DEFAULT, MIL.M_ENABLE, MIL.M_NULL);
                MIL.MappControlMp(MilSystemOwnerApplication, MIL.M_CORE_SHARING, MIL.M_DEFAULT, MIL.M_DISABLE, MIL.M_NULL);
                MIL.MthrInquireMp(MilSystemCurrentThreadId, MIL.M_CORE_NUM_EFFECTIVE, MIL.M_DEFAULT, MIL.M_DEFAULT, ref NbCoresUsedNoCS);
                if (NbCoresUsedNoCS != NbCoresUsed)
                {
                    Benchmark(ref ProcessingParam, ref TimeAllCoresNoCS, ref FPSAllCoresNoCS);

                    // Show the resulting image and the timing results.
                    MIL.MbufCopy(ProcessingParam.MilDestinationImage, MilDisplayImage);

                    Console.WriteLine("Using multi-processing   ({0,3} CPU cores): {1,5:0.000} ms ({2,6:0.0} fps), no Hyper-Thread.", NbCoresUsedNoCS, TimeAllCoresNoCS, FPSAllCoresNoCS);
                }
                MIL.MappControlMp(MilSystemOwnerApplication, MIL.M_MP_USE, MIL.M_DEFAULT, MIL.M_DEFAULT, MIL.M_NULL);
                MIL.MappControlMp(MilSystemOwnerApplication, MIL.M_CORE_SHARING, MIL.M_DEFAULT, MIL.M_DEFAULT, MIL.M_NULL);


                // Benchmark the processing function without multi-processing.
                // -----------------------------------------------------------
                MIL.MappControlMp(MilSystemOwnerApplication, MIL.M_MP_USE, MIL.M_DEFAULT, MIL.M_DISABLE, MIL.M_NULL);
                Benchmark(ref ProcessingParam, ref TimeOneCore, ref FPSOneCore);

                // Show the resulting image and the timing results.
                MIL.MbufCopy(ProcessingParam.MilDestinationImage, MilDisplayImage);

                Console.WriteLine("Without multi-processing (  1 CPU core ): {0,5:0.000} ms ({1,6:0.0} fps)", TimeOneCore, FPSOneCore);
                Console.WriteLine();

                MIL.MappControlMp(MilSystemOwnerApplication, MIL.M_MP_USE, MIL.M_DEFAULT, MIL.M_DEFAULT, MIL.M_NULL);

                // Show a comparative summary of the timing results.
                if (NbCoresUsed > 1)
                {
                    Console.WriteLine("Benchmark is {0:0.0} times faster with multi-processing.", TimeOneCore / TimeAllCores);
                    Console.WriteLine();
                }

                if (NbCoresUsedNoCS != NbCoresUsed)
                {
                    Console.WriteLine("Benchmark is {0:0.0} times faster with multi-processing and no Hyper-Thread.", TimeOneCore / TimeAllCoresNoCS);
                    Console.WriteLine();
                }
            }
            else
            {
                // Benchmark on GPU system.
                Benchmark(ref ProcessingParam, ref TimeAllCores, ref FPSAllCores);

                // Show the resulting image and the timing results.
                MIL.MbufCopy(ProcessingParam.MilDestinationImage, MilDisplayImage);

                Console.WriteLine("Using GPU: {0,5:0.000} ms ({1,6:0.0} fps)", TimeAllCores, FPSAllCores);
            }

            // Wait for a key press.
            Console.WriteLine("Press <Enter> to end.");
            Console.ReadKey();

            // Free all allocations.
            ProcessingFree(ref ProcessingParam);
            MIL.MdispSelect(MilDisplay, MIL.M_NULL);
            MIL.MbufFree(MilDisplayImage);
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL);
        }

        //*****************************************************************************
        // Benchmark function.
        //*****************************************************************************
        private static void Benchmark(ref PROC_PARAM ProcParamPtr, ref double Time, ref double FramesPerSecond)
        {
            MIL_INT EstimatedNbLoop = DEFAULT_NB_LOOP;
            double StartTime = 0.0;
            double EndTime = 0.0;
            double MinTime = 0;
            MIL_INT n;

            // Wait for the completion of all functions in this thread.
            MIL.MthrWait(MIL.M_DEFAULT, MIL.M_THREAD_WAIT, MIL.M_NULL);

            // Call the processing once before benchmarking for a more accurate time.
            // This compensates for Dll load time, etc.
            MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ, ref StartTime);
            ProcessingExecute(ref ProcParamPtr);
            MIL.MthrWait(MIL.M_DEFAULT, MIL.M_THREAD_WAIT, MIL.M_NULL);
            MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ, ref EndTime);

            MinTime = EndTime - StartTime;

            // Estimate the number of loops required to benchmark the processing for 
            // the specified minimum time.
            for (n = 0; n < ESTIMATION_NB_LOOP; n++)
            {
                MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ, ref StartTime);
                ProcessingExecute(ref ProcParamPtr);
                MIL.MthrWait(MIL.M_DEFAULT, MIL.M_THREAD_WAIT, MIL.M_NULL);
                MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ, ref EndTime);

                Time = EndTime - StartTime;
                MinTime = (Time < MinTime) ? Time : MinTime;
            }

            if (MinTime > 0)
            {
                EstimatedNbLoop = (MIL_INT)(MINIMUM_BENCHMARK_TIME / MinTime) + 1;
            }

            // Benchmark the processing according to the estimated number of loops.
            MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ, ref StartTime);
            for (n = 0; n < EstimatedNbLoop; n++)
            {
                ProcessingExecute(ref ProcParamPtr);
            }
            MIL.MthrWait(MIL.M_DEFAULT, MIL.M_THREAD_WAIT, MIL.M_NULL);
            MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ, ref EndTime);

            Time = EndTime - StartTime;

            FramesPerSecond = EstimatedNbLoop / Time;
            Time = Time * 1000 / EstimatedNbLoop;
        }

        //*****************************************************************************
        // Processing initialization function.
        //*****************************************************************************
        private static void ProcessingInit(MIL_ID MilSystem, ref PROC_PARAM ProcParamPtr)
        {
            // Allocate a MIL source buffer.
            MIL.MbufAllocColor(MilSystem,
                     MIL.MbufDiskInquire(IMAGE_FILE, MIL.M_SIZE_BAND, MIL.M_NULL),
                     MIL.MbufDiskInquire(IMAGE_FILE, MIL.M_SIZE_X, MIL.M_NULL),
                     MIL.MbufDiskInquire(IMAGE_FILE, MIL.M_SIZE_Y, MIL.M_NULL),
                     MIL.MbufDiskInquire(IMAGE_FILE, MIL.M_SIZE_BIT, MIL.M_NULL) + MIL.M_UNSIGNED,
                     MIL.M_IMAGE + MIL.M_PROC, ref ProcParamPtr.MilSourceImage );

            // Load the image into the source image.
            MIL.MbufLoad(IMAGE_FILE, ProcParamPtr.MilSourceImage);

            // Allocate a MIL destination buffer.
            MIL.MbufAllocColor(MilSystem,
                     MIL.MbufDiskInquire(IMAGE_FILE, MIL.M_SIZE_BAND, MIL.M_NULL),
                     MIL.MbufDiskInquire(IMAGE_FILE, MIL.M_SIZE_X, MIL.M_NULL),
                     MIL.MbufDiskInquire(IMAGE_FILE, MIL.M_SIZE_Y, MIL.M_NULL),
                     MIL.MbufDiskInquire(IMAGE_FILE, MIL.M_SIZE_BIT, MIL.M_NULL) + MIL.M_UNSIGNED,
                     MIL.M_IMAGE + MIL.M_PROC, ref ProcParamPtr.MilDestinationImage);
        }

        //*****************************************************************************
        // Processing execution function.
        //*****************************************************************************
        private static void ProcessingExecute(ref PROC_PARAM ProcParamPtr)
        {
            MIL.MimRotate(ProcParamPtr.MilSourceImage, ProcParamPtr.MilDestinationImage, ROTATE_ANGLE,
                      MIL.M_DEFAULT, MIL.M_DEFAULT, MIL.M_DEFAULT, MIL.M_DEFAULT, MIL.M_BILINEAR + MIL.M_OVERSCAN_CLEAR);
        }

        //*****************************************************************************
        // Processing free function.
        //*****************************************************************************
        private static void ProcessingFree(ref PROC_PARAM ProcParamPtr)
        {
            // Free all processing allocations.
            MIL.MbufFree(ProcParamPtr.MilSourceImage);
            MIL.MbufFree(ProcParamPtr.MilDestinationImage);
        }
    }
}
