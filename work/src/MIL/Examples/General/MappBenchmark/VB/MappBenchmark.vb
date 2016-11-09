'****************************************************************************
'
' File name: MappBenchmark.vb
'
' Synopsis:  This program uses the MappTimer function to demonstrate the 
'            benchmarking of MIL functions. It can be used as a template 
'            to benchmark any MIL or custom processing function accurately.
'
'            It takes into account different factors that can influence 
'            the timing such as dll load time and OS inaccuracy when 
'            measuring a very short time.
'****************************************************************************
Imports Microsoft.VisualBasic
Imports System

Imports Matrox.MatroxImagingLibrary

Namespace MAppBenchmark
    ' Processing function parameters structure.
    Public Structure PROC_PARAM
        Public MilSourceImage As MIL_ID ' Image buffer identifier.
        Public MilDestinationImage As MIL_ID ' Image buffer identifier.
    End Structure

    Public Class Program
        ' Target MIL image specifications.
        Private Shared ReadOnly IMAGE_FILE As String = MIL.M_IMAGE_PATH & "LargeWafer.mim"
        Private Const ROTATE_ANGLE As Integer = -15

        ' Timing loop iterations setting.
        Private Const MINIMUM_BENCHMARK_TIME As Double = 1.0 ' In seconds (1.0 and more recommended).
        Private Const ESTIMATION_NB_LOOP As Integer = 5
        Private Const DEFAULT_NB_LOOP As Integer = 100

        Shared Sub Main(ByVal args() As String)
            Dim MilApplication As MIL_ID = MIL.M_NULL               ' Application identifier.
            Dim MilSystem As MIL_ID = MIL.M_NULL                    ' System identifier.
            Dim MilDisplay As MIL_ID = MIL.M_NULL                   ' Display identifier.
            Dim MilDisplayImage As MIL_ID = MIL.M_NULL              ' Image buffer identifier.
            Dim MilSystemOwnerApplication As MIL_ID = MIL.M_NULL    ' System's owner application.
            Dim MilSystemCurrentThreadId As MIL_ID = MIL.M_NULL     ' System's current thread identifier.

            Dim ProcessingParam As New PROC_PARAM()     ' Processing parameters.
            Dim TimeAllCores As Double = 0.0            ' Timer variables.
            Dim TimeAllCoresNoCS As Double = 0.0
            Dim TimeOneCore As Double = 0.0
            Dim FPSAllCores As Double = 0.0             ' FPS variables.
            Dim FPSAllCoresNoCS As Double = 0.0
            Dim FPSOneCore As Double = 0.0
            Dim NbCoresUsed As MIL_INT = 0              ' Number of CPU Core used.
            Dim NbCoresUsedNoCS As MIL_INT = 0
            Dim UseGPUSystem As Boolean = False

            ' Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, MilApplication, MilSystem, MilDisplay, CType(MIL.M_NULL, IntPtr), CType(MIL.M_NULL, IntPtr))

            ' Get the system's owner application.
            MIL.MsysInquire(MilSystem, MIL.M_OWNER_APPLICATION, MilSystemOwnerApplication)

            ' Get the system's current thread identifier.
            MIL.MsysInquire(MilSystem, MIL.M_CURRENT_THREAD_ID, MilSystemCurrentThreadId)

            ' Determine if the processing will be done on the GPU.
            If MIL.MsysInquire(MilSystem, MIL.M_SYSTEM_TYPE, MIL.M_NULL) = MIL.M_SYSTEM_GPU_TYPE Then
                UseGPUSystem = True
            End If

            ' Restore image into an automatically allocated image buffer.
            MIL.MbufRestore(IMAGE_FILE, MilSystem, MilDisplayImage)

            ' Display the image buffer.
            MIL.MdispSelect(MilDisplay, MilDisplayImage)

            ' Allocate the processing objects.
            ProcessingInit(MilSystem, ProcessingParam)

            ' Pause to show the original image.
            Console.WriteLine()
            Console.WriteLine("PROCESSING FUNCTION BENCHMARKING:")
            Console.WriteLine("---------------------------------")
            Console.WriteLine()
            Console.WriteLine("This program times a processing function under ")
            Console.WriteLine("different conditions.")
            Console.WriteLine("Press <Enter> to start.")
            Console.WriteLine()
            Console.ReadKey()
            Console.WriteLine("PROCESSING TIME FOR {0}x{1}:", _
                MIL.MbufInquire(ProcessingParam.MilDestinationImage, MIL.M_SIZE_X, MIL.M_NULL), _
                MIL.MbufInquire(ProcessingParam.MilDestinationImage, MIL.M_SIZE_Y, MIL.M_NULL))
            Console.WriteLine("------------------------------")
            Console.WriteLine()

            If Not UseGPUSystem Then
                ' Benchmark the processing function with multi-processing.
                ' --------------------------------------------------------
                MIL.MappControlMp(MilSystemOwnerApplication, MIL.M_MP_USE, MIL.M_DEFAULT, MIL.M_ENABLE, MIL.M_NULL)
                MIL.MappControlMp(MilSystemOwnerApplication, MIL.M_CORE_SHARING, MIL.M_DEFAULT, MIL.M_ENABLE, MIL.M_NULL)
                MIL.MthrInquireMp(MilSystemCurrentThreadId, MIL.M_CORE_NUM_EFFECTIVE, MIL.M_DEFAULT, MIL.M_DEFAULT, NbCoresUsed)
                If (NbCoresUsed > 1) Then
                    Benchmark(ProcessingParam, TimeAllCores, FPSAllCores)

                    ' Show the resulting image and the timing results.
                    MIL.MbufCopy(ProcessingParam.MilDestinationImage, MilDisplayImage)


                    Console.WriteLine("Using multi-processing   ({0,3} CPU cores): {1,5:0.000} ms ({2,6:0.0} fps)", NbCoresUsed, TimeAllCores, FPSAllCores)

                End If
                MIL.MappControlMp(MilSystemOwnerApplication, MIL.M_MP_USE, MIL.M_DEFAULT, MIL.M_DEFAULT, MIL.M_NULL)
                MIL.MappControlMp(MilSystemOwnerApplication, MIL.M_CORE_SHARING, MIL.M_DEFAULT, MIL.M_DEFAULT, MIL.M_NULL)

                ' Benchmark the processing function with multi-processing but no hyper-threading.
                ' --------------------------------------------------------------------------------

                ' If Hyper-threading is available on the PC, test if the performance is better 
                ' with it disabled. Sometimes, too many cores sharing the same CPU resources that
                ' are already fully occupied can reduce the overall performance.
                '
                MIL.MappControlMp(MilSystemOwnerApplication, MIL.M_MP_USE, MIL.M_DEFAULT, MIL.M_ENABLE, MIL.M_NULL)
                MIL.MappControlMp(MilSystemOwnerApplication, MIL.M_CORE_SHARING, MIL.M_DEFAULT, MIL.M_DISABLE, MIL.M_NULL)
                MIL.MthrInquireMp(MilSystemCurrentThreadId, MIL.M_CORE_NUM_EFFECTIVE, MIL.M_DEFAULT, MIL.M_DEFAULT, NbCoresUsedNoCS)
                If (NbCoresUsedNoCS <> NbCoresUsed) Then
                    Benchmark(ProcessingParam, TimeAllCoresNoCS, FPSAllCoresNoCS)

                    ' Show the resulting image and the timing results.
                    MIL.MbufCopy(ProcessingParam.MilDestinationImage, MilDisplayImage)

                    Console.WriteLine("Using multi-processing   ({0,3} CPU cores): {1,5:0.000} ms ({2,6:0.0} fps), no Hyper-Thread.", NbCoresUsedNoCS, TimeAllCoresNoCS, FPSAllCoresNoCS)
                End If

                MIL.MappControlMp(MilSystemOwnerApplication, MIL.M_MP_USE, MIL.M_DEFAULT, MIL.M_DEFAULT, MIL.M_NULL)
                MIL.MappControlMp(MilSystemOwnerApplication, MIL.M_CORE_SHARING, MIL.M_DEFAULT, MIL.M_DEFAULT, MIL.M_NULL)


                ' Benchmark the processing function without multi-processing.
                ' -----------------------------------------------------------
                MIL.MappControlMp(MilSystemOwnerApplication, MIL.M_MP_USE, MIL.M_DEFAULT, MIL.M_DISABLE, MIL.M_NULL)
                Benchmark(ProcessingParam, TimeOneCore, FPSOneCore)

                ' Show the resulting image and the timing results.
                MIL.MbufCopy(ProcessingParam.MilDestinationImage, MilDisplayImage)

                Console.WriteLine("Without multi-processing (  1 CPU core ): {0,5:0.000} ms ({1,6:0.0} fps)", TimeOneCore, FPSOneCore)
                Console.WriteLine()

                MIL.MappControlMp(MilSystemOwnerApplication, MIL.M_MP_USE, MIL.M_DEFAULT, MIL.M_DEFAULT, MIL.M_NULL)

                ' Show a comparative summary of the timing results.
                If (NbCoresUsed > 1) Then
                    Console.WriteLine("Benchmark is {0:0.0} times faster with multi-processing.", TimeOneCore / TimeAllCores)
                    Console.WriteLine()
                End If

                If (NbCoresUsedNoCS <> NbCoresUsed) Then
                    Console.WriteLine("Benchmark is {0:0.0} times faster with multi-processing and no Hyper-Thread.", TimeOneCore / TimeAllCoresNoCS)
                    Console.WriteLine()
                End If
            Else
                ' Benchmark on GPU system.
                Benchmark(ProcessingParam, TimeAllCores, FPSAllCores)

                ' Show the resulting image and the timing results.
                MIL.MbufCopy(ProcessingParam.MilDestinationImage, MilDisplayImage)

                Console.WriteLine("Using GPU: {0,5:0.000} ms ({1,6:0.0} fps)", TimeAllCores, FPSAllCores)
            End If
            ' Wait for a key press.
            Console.WriteLine("Press <Enter> to end.")
            Console.ReadKey()

            ' Free all allocations.
            ProcessingFree(ProcessingParam)
            MIL.MdispSelect(MilDisplay, MIL.M_NULL)
            MIL.MbufFree(MilDisplayImage)
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL)
        End Sub

        '*****************************************************************************
        ' Benchmark function.
        '*****************************************************************************
        Private Shared Sub Benchmark(ByRef ProcParamPtr As PROC_PARAM, ByRef Time As Double, ByRef FramesPerSecond As Double)
            Dim EstimatedNbLoop As MIL_INT = DEFAULT_NB_LOOP
            Dim StartTime As Double = 0
            Dim EndTime As Double = 0
            Dim MinTime As Double = 0
            Dim n As MIL_INT

            ' Wait for the completion of all functions in this thread.
            MIL.MthrWait(MIL.M_DEFAULT, MIL.M_THREAD_WAIT, CType(MIL.M_NULL, IntPtr))


            ' Call the processing once before benchmarking for a more accurate time.
            ' This compensates for Dll load time, etc.

            MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ, StartTime)
            ProcessingExecute(ProcParamPtr)
            MIL.MthrWait(MIL.M_DEFAULT, MIL.M_THREAD_WAIT, CType(MIL.M_NULL, IntPtr))
            MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ, EndTime)

            MinTime = EndTime - StartTime

            ' Estimate the number of loops required to benchmark the processing for 
            ' the specified minimum time.

            For n = 0 To ESTIMATION_NB_LOOP - 1
                MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ, StartTime)
                ProcessingExecute(ProcParamPtr)
                MIL.MthrWait(MIL.M_DEFAULT, MIL.M_THREAD_WAIT, CType(MIL.M_NULL, IntPtr))
                MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ, EndTime)

                Time = EndTime - StartTime

                MinTime = If((Time < MinTime), Time, MinTime)
            Next n
            If MinTime > 0 Then
                EstimatedNbLoop = CType(MINIMUM_BENCHMARK_TIME / MinTime, MIL_INT) + 1
            End If

            ' Benchmark the processing according to the estimated number of loops.
            MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ, StartTime)

            For n = 0 To EstimatedNbLoop - 1
                ProcessingExecute(ProcParamPtr)
            Next n
            MIL.MthrWait(MIL.M_DEFAULT, MIL.M_THREAD_WAIT, CType(MIL.M_NULL, IntPtr))
            MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ, EndTime)

            Time = EndTime - StartTime


            FramesPerSecond = CType(EstimatedNbLoop, Double) / Time
            Time = Time * 1000 / CType(EstimatedNbLoop, Double)
        End Sub

        '*****************************************************************************
        ' Processing initialization function.
        '*****************************************************************************
        Private Shared Sub ProcessingInit(ByVal MilSystem As MIL_ID, ByRef ProcParamPtr As PROC_PARAM)
            Dim SizeBand As MIL_INT = MIL.MbufDiskInquire(IMAGE_FILE, MIL.M_SIZE_BAND, CType(MIL.M_NULL, IntPtr))
            Dim SizeX As MIL_INT = MIL.MbufDiskInquire(IMAGE_FILE, MIL.M_SIZE_X, CType(MIL.M_NULL, IntPtr))
            Dim SizeY As MIL_INT = MIL.MbufDiskInquire(IMAGE_FILE, MIL.M_SIZE_Y, CType(MIL.M_NULL, IntPtr))
            Dim SizeBit As MIL_INT = MIL.MbufDiskInquire(IMAGE_FILE, MIL.M_SIZE_BIT, CType(MIL.M_NULL, IntPtr))

            ' Allocate a MIL source buffer.
            MIL.MbufAllocColor(MilSystem, SizeBand, SizeX, SizeY, SizeBit + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_PROC, ProcParamPtr.MilSourceImage)

            ' Load the image into the source image.
            MIL.MbufLoad(IMAGE_FILE, ProcParamPtr.MilSourceImage)

            ' Allocate a MIL destination buffer.
            MIL.MbufAllocColor(MilSystem, SizeBand, SizeX, SizeY, SizeBit + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_PROC, ProcParamPtr.MilDestinationImage)
        End Sub

        '*****************************************************************************
        ' Processing execution function.
        '*****************************************************************************
        Private Shared Sub ProcessingExecute(ByRef ProcParamPtr As PROC_PARAM)
            MIL.MimRotate(ProcParamPtr.MilSourceImage, ProcParamPtr.MilDestinationImage, ROTATE_ANGLE, MIL.M_DEFAULT, MIL.M_DEFAULT, MIL.M_DEFAULT, MIL.M_DEFAULT, MIL.M_BILINEAR + MIL.M_OVERSCAN_CLEAR)
        End Sub

        '*****************************************************************************
        ' Processing free function.
        '*****************************************************************************
        Private Shared Sub ProcessingFree(ByRef ProcParamPtr As PROC_PARAM)
            ' Free all processing allocations.
            MIL.MbufFree(ProcParamPtr.MilSourceImage)
            MIL.MbufFree(ProcParamPtr.MilDestinationImage)
        End Sub
    End Class
End Namespace
