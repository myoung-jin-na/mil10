'***********************************************************************************
'
' File name: MimFFT.vb
'
' Synopsis:  This program uses the Fast Fourier Transform to filter an image.
'***********************************************************************************
Imports Microsoft.VisualBasic
Imports System
Imports System.Collections.Generic
Imports System.Text

Imports Matrox.MatroxImagingLibrary

Namespace MImFFT
    Friend Class Program
        ' Target image specifications.
        Private Const NOISY_IMAGE As String = MIL.M_IMAGE_PATH & "noise.mim"
        Private Const IMAGE_WIDTH As Integer = 256
        Private Const IMAGE_HEIGHT As Integer = 256
        Private Const X_NEGATIVE_FREQUENCY_POSITION As Integer = 63
        Private Const X_POSITIVE_FREQUENCY_POSITION As Integer = 191
        Private Const Y_FREQUENCY_POSITION As Integer = 127
        Private Const CIRCLE_WIDTH As Integer = 9

        Shared Sub Main(ByVal args() As String)
            Dim MilApplication As MIL_ID = MIL.M_NULL ' Application identifier.
            Dim MilSystem As MIL_ID = MIL.M_NULL ' System identifier.
            Dim MilDisplay As MIL_ID = MIL.M_NULL ' Display identifier.
            Dim MilImage As MIL_ID = MIL.M_NULL ' Image buffer identifier.
            Dim MilOverlayImage As MIL_ID = MIL.M_NULL ' Overlay image buffer identifier.
            Dim MilSubImage00 As MIL_ID = MIL.M_NULL ' Child buffer identifier.
            Dim MilSubImage01 As MIL_ID = MIL.M_NULL ' Child buffer identifier.
            Dim MilSubImage10 As MIL_ID = MIL.M_NULL ' Child buffer identifier.
            Dim MilSubImage11 As MIL_ID = MIL.M_NULL ' Child buffer identifier.
            Dim MilTransformReal As MIL_ID = MIL.M_NULL ' Real part of the transformed image.
            Dim MilTransformIm As MIL_ID = MIL.M_NULL ' Imaginary part of the transformed image.

            Dim ZeroVal(0) As Single
            ZeroVal(0) = 0.0F

            ' Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, MilApplication, MilSystem, MilDisplay, CType(MIL.M_NULL, IntPtr), CType(MIL.M_NULL, IntPtr))

            ' Allocate a display buffer and clear it.
            MIL.MbufAlloc2d(MilSystem, IMAGE_WIDTH * 2, IMAGE_HEIGHT * 2, 8 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_PROC + MIL.M_DISP, MilImage)
            MIL.MbufClear(MilImage, 0L)

            ' Display the image buffer and prepare for overlay annotations.
            MIL.MdispSelect(MilDisplay, MilImage)
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY, MIL.M_ENABLE)
            MIL.MdispInquire(MilDisplay, MIL.M_OVERLAY_ID, MilOverlayImage)

            ' Allocate child buffers in the 4 quadrants of the display image.
            MIL.MbufChild2d(MilImage, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT, MilSubImage00)
            MIL.MbufChild2d(MilImage, IMAGE_WIDTH, 0, IMAGE_WIDTH, IMAGE_HEIGHT, MilSubImage01)
            MIL.MbufChild2d(MilImage, 0, IMAGE_HEIGHT, IMAGE_WIDTH, IMAGE_HEIGHT, MilSubImage10)
            MIL.MbufChild2d(MilImage, IMAGE_WIDTH, IMAGE_HEIGHT, IMAGE_WIDTH, IMAGE_HEIGHT, MilSubImage11)

            ' Allocate processing buffers.
            MIL.MbufAlloc2d(MilSystem, IMAGE_WIDTH, IMAGE_HEIGHT, 32 + MIL.M_FLOAT, MIL.M_IMAGE + MIL.M_PROC, MilTransformReal)
            MIL.MbufAlloc2d(MilSystem, IMAGE_WIDTH, IMAGE_HEIGHT, 32 + MIL.M_FLOAT, MIL.M_IMAGE + MIL.M_PROC, MilTransformIm)

            ' Load a noisy image.
            MIL.MbufLoad(NOISY_IMAGE, MilSubImage00)

            ' Print a message on the screen.
            Console.Write(Constants.vbLf + "FFT:" + Constants.vbLf)
            Console.Write("----" + Constants.vbLf + Constants.vbLf)
            Console.Write("The frequency spectrum of a noisy image will be computed to remove the periodic noise." + Constants.vbLf)
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' The image is Fourier transformed to obtain the magnitude of the
            ' spectrum. This result will be used to design the filter.
            MIL.MimTransform(MilSubImage00, MIL.M_NULL, MilTransformReal, MilTransformIm, MIL.M_FFT, MIL.M_FORWARD + MIL.M_CENTER + MIL.M_MAGNITUDE + MIL.M_LOG_SCALE)
            MIL.MbufCopy(MilTransformReal, MilSubImage10)

            ' Draw circles in the overlay around the points of interest.
            MIL.MbufCopy(MilTransformReal, MilSubImage11)
            MIL.MgraColor(MIL.M_DEFAULT, MIL.M_COLOR_YELLOW)
            MIL.MgraArc(MIL.M_DEFAULT, MilOverlayImage, X_NEGATIVE_FREQUENCY_POSITION, Y_FREQUENCY_POSITION + IMAGE_HEIGHT, CIRCLE_WIDTH, CIRCLE_WIDTH, 0, 360)
            MIL.MgraArc(MIL.M_DEFAULT, MilOverlayImage, X_POSITIVE_FREQUENCY_POSITION, Y_FREQUENCY_POSITION + IMAGE_HEIGHT, CIRCLE_WIDTH, CIRCLE_WIDTH, 0, 360)
            MIL.MgraArc(MIL.M_DEFAULT, MilOverlayImage, X_NEGATIVE_FREQUENCY_POSITION + IMAGE_WIDTH, Y_FREQUENCY_POSITION + IMAGE_HEIGHT, CIRCLE_WIDTH, CIRCLE_WIDTH, 0, 360)
            MIL.MgraArc(MIL.M_DEFAULT, MilOverlayImage, X_POSITIVE_FREQUENCY_POSITION + IMAGE_WIDTH, Y_FREQUENCY_POSITION + IMAGE_HEIGHT, CIRCLE_WIDTH, CIRCLE_WIDTH, 0, 360)

            ' Put zero in the spectrum where the noise is located.
            MIL.MbufPut2d(MilSubImage11, X_NEGATIVE_FREQUENCY_POSITION, Y_FREQUENCY_POSITION, 1, 1, ZeroVal)
            MIL.MbufPut2d(MilSubImage11, X_POSITIVE_FREQUENCY_POSITION, Y_FREQUENCY_POSITION, 1, 1, ZeroVal)

            ' Compute the Fast Fourier Transform of the image.
            MIL.MimTransform(MilSubImage00, MIL.M_NULL, MilTransformReal, MilTransformIm, MIL.M_FFT, MIL.M_FORWARD + MIL.M_CENTER)

            ' Filter the image in the frequency domain.
            MIL.MbufPut2d(MilTransformReal, X_NEGATIVE_FREQUENCY_POSITION, Y_FREQUENCY_POSITION, 1, 1, ZeroVal)
            MIL.MbufPut2d(MilTransformReal, X_POSITIVE_FREQUENCY_POSITION, Y_FREQUENCY_POSITION, 1, 1, ZeroVal)
            MIL.MbufPut2d(MilTransformIm, X_NEGATIVE_FREQUENCY_POSITION, Y_FREQUENCY_POSITION, 1, 1, ZeroVal)
            MIL.MbufPut2d(MilTransformIm, X_POSITIVE_FREQUENCY_POSITION, Y_FREQUENCY_POSITION, 1, 1, ZeroVal)

            ' Recover the image in the spatial domain.
            MIL.MimTransform(MilTransformReal, MilTransformIm, MilSubImage01, MIL.M_NULL, MIL.M_FFT, MIL.M_REVERSE + MIL.M_CENTER + MIL.M_SATURATION)

            ' Print a message.
            Console.Write("The frequency components of the noise are located in the center of the circles." + Constants.vbLf)
            Console.Write("The noise was removed by setting these frequency components to zero." + Constants.vbLf)
            Console.Write("Press <Enter> to end." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Free buffers.
            MIL.MbufFree(MilSubImage00)
            MIL.MbufFree(MilSubImage01)
            MIL.MbufFree(MilSubImage10)
            MIL.MbufFree(MilSubImage11)
            MIL.MbufFree(MilImage)
            MIL.MbufFree(MilTransformReal)
            MIL.MbufFree(MilTransformIm)

            ' Free defaults.
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL)

        End Sub
    End Class
End Namespace
