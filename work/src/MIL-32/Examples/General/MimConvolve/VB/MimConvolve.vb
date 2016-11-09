'*****************************************************************************
' 
' File name: MimConvolve.vb
'
' Synopsis:  This program performs a 3x3 convolution using a custom kernel
'            and calculates the convolution time.
'*****************************************************************************
Imports Microsoft.VisualBasic
Imports System
Imports System.Collections.Generic
Imports System.Text

Imports Matrox.MatroxImagingLibrary

Namespace MImConvolve
    Friend Class Program
        ' Target MIL image specifications.
        Private Const IMAGE_FILE As String = MIL.M_IMAGE_PATH & "BaboonMono.mim"
        Private Const ZOOM_VALUE As Integer = 2

        ' Kernel data definition.
        Private Const KERNEL_WIDTH As Integer = 3
        Private Const KERNEL_HEIGHT As Integer = 3
        Private Const KERNEL_DEPTH As Integer = 8
        Private Shared ReadOnly KernelData(,) As Byte = {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}}

        ' Timing loop iterations.
        Private Const NB_LOOP As Integer = 100

        Shared Sub Main(ByVal args() As String)
            Dim MilApplication As MIL_ID = MIL.M_NULL ' Application identifier.
            Dim MilSystem As MIL_ID = MIL.M_NULL ' System identifier.
            Dim MilDisplay As MIL_ID = MIL.M_NULL ' Display identifier.
            Dim MilDisplayImage As MIL_ID = MIL.M_NULL ' Image buffer identifier.
            Dim MilImage As MIL_ID = MIL.M_NULL ' Image buffer identifier.
            Dim MilKernel As MIL_ID = MIL.M_NULL ' Custom kernel identifier.
            Dim n As Integer = 0
            Dim Time As Double = 0.0

            ' Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, MilApplication, MilSystem, MilDisplay, CType(MIL.M_NULL, IntPtr), CType(MIL.M_NULL, IntPtr))

            ' Restore source image into an automatically allocated image buffers.
            MIL.MbufRestore(IMAGE_FILE, MilSystem, MilImage)
            MIL.MbufRestore(IMAGE_FILE, MilSystem, MilDisplayImage)

            ' Zoom display to see the result of image processing better.
            MIL.MdispZoom(MilDisplay, ZOOM_VALUE, ZOOM_VALUE)

            ' Display the image buffer.
            MIL.MdispSelect(MilDisplay, MilDisplayImage)

            ' Pause to show the original image.
            Console.Write(Constants.vbLf + "IMAGE PROCESSING:" + Constants.vbLf)
            Console.Write("-----------------" + Constants.vbLf + Constants.vbLf)
            Console.Write("This program performs a convolution on the displayed image." + Constants.vbLf)
            Console.Write("It uses a custom smoothing kernel." + Constants.vbLf)
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Allocate a MIL kernel.
            MIL.MbufAlloc2d(MilSystem, KERNEL_WIDTH, KERNEL_HEIGHT, KERNEL_DEPTH + MIL.M_UNSIGNED, MIL.M_KERNEL, MilKernel)

            ' Put the custom data in it.
            MIL.MbufPut(MilKernel, KernelData)

            ' Set a normalization (divide) factor to have a kernel with
            ' a sum equal to one.
            MIL.MbufControlNeighborhood(MilKernel, MIL.M_NORMALIZATION_FACTOR, 16)

            ' Convolve the image using the kernel.
            MIL.MimConvolve(MilImage, MilDisplayImage, MilKernel)

            ' Now time the convolution (MimConvolve()):
            ' Overscan calculation is disabled and a destination image that
            ' is not displayed is used to have the real convolution time. Also the 
            ' function must be called once before the timing loop for more accurate 
            ' time (dll load, ...).
            MIL.MbufControlNeighborhood(MilKernel, MIL.M_OVERSCAN, MIL.M_DISABLE)
            MIL.MimConvolve(MilDisplayImage, MilImage, MilKernel)
            MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_RESET + MIL.M_SYNCHRONOUS, CType(MIL.M_NULL, IntPtr))
            For n = 0 To NB_LOOP - 1
                MIL.MimConvolve(MilDisplayImage, MilImage, MilKernel)
            Next n
            MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ + MIL.M_SYNCHRONOUS, Time)

            ' Pause to show the result.
            Console.Write("Convolve time: {0:0.000} ms." + Constants.vbLf, Time * 1000.0 / NB_LOOP)
            Console.Write("Press <Enter> to terminate." + Constants.vbLf)
            Console.ReadKey()

            ' Free all allocations.
            MIL.MbufFree(MilKernel)
            MIL.MbufFree(MilImage)
            MIL.MbufFree(MilDisplayImage)
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL)

        End Sub
    End Class
End Namespace
