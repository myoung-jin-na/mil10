'*****************************************************************************
'
' File name: MimLocatePeak1d.vb
'
' Synopsis:  This program finds the peak in each column of an input sequence
'            and reconstruct the height of a 3d object using it.
'*****************************************************************************
Imports Microsoft.VisualBasic
Imports System
Imports System.Collections.Generic
Imports System.Text
Imports System.Runtime.InteropServices

Imports Matrox.MatroxImagingLibrary

Namespace MImLocatePeak1d
    Friend Class Program
        ' Input sequence specifications.
        Private Const SEQUENCE_FILE As String = MIL.M_IMAGE_PATH & "HandWithLaser.avi"

        ' Peak detection parameters.
        Private Const MAX_LINE_WIDTH As Integer = 10
        Private Const MIN_INTENSITY As Integer = 100
        Private Const NB_FIXED_POINT As Integer = 4

        ' D3D display parameters
        Private Const D3D_DISPLAY_SIZE_X As Integer = 640
        Private Const D3D_DISPLAY_SIZE_Y As Integer = 480
        Private Const D3D_MESH_SCALING_X As Double = -1.0
        Private Const D3D_MESH_SCALING_Y As Double = 4.0
        Private Const D3D_MESH_SCALING_Z As Double = -0.13

        ' Function declarations for DirectX display
        <DllImport("mdispd3d.dll", CallingConvention:=CallingConvention.Cdecl)> _
        Public Shared Function MdepthD3DAlloc(ByVal DepthBuffer As MIL_ID, ByVal IntensityBuffer As MIL_ID, ByVal DisplaySizeX As MIL_INT, ByVal DisplaySizeY As MIL_INT, ByVal ScaleX As Double, ByVal ScaleY As Double, ByVal ScaleZ As Double, ByVal MinZ As Double, ByVal MaxZ As Double, ByVal MaxDistanceZ As Double, ByVal WindowHandle As IntPtr) As IntPtr
        End Function
        <DllImport("mdispd3d.dll", CallingConvention:=CallingConvention.Cdecl)> _
        Public Shared Sub MdispD3DFree(ByVal DispHandle As IntPtr)
        End Sub
        <DllImport("mdispd3d.dll", CallingConvention:=CallingConvention.Cdecl)> _
        Public Shared Sub MdispD3DShow(ByVal DispHandle As IntPtr)
        End Sub
        <DllImport("mdispd3d.dll", CallingConvention:=CallingConvention.Cdecl)> _
        Public Shared Sub MdispD3DHide(ByVal DispHandle As IntPtr)
        End Sub
        <DllImport("mdispd3d.dll", CallingConvention:=CallingConvention.Cdecl)> _
        Public Shared Sub MdispD3DPrintHelp(ByVal DispHandle As IntPtr)
        End Sub

        Shared Sub Main(ByVal args() As String)
            Dim MilApplication As MIL_ID = MIL.M_NULL ' Application identifier.
            Dim MilSystem As MIL_ID = MIL.M_NULL ' System identifier.
            Dim MilDisplay As MIL_ID = MIL.M_NULL ' Display identifier.
            Dim MilDisplayImage As MIL_ID = MIL.M_NULL ' Image buffer identifier.
            Dim MilImage As MIL_ID = MIL.M_NULL ' Image buffer identifier.
            Dim MilPosYImage As MIL_ID = MIL.M_NULL ' Image buffer identifier.
            Dim MilValImage As MIL_ID = MIL.M_NULL ' Image buffer identifier.
            Dim MilExtreme As MIL_ID = MIL.M_NULL ' Result buffer identifier.

            Dim FrameRate As Double = 0.0
            Dim n As Integer = 0
            Dim PreviousTime As Double = 0.0
            Dim StartTime As Double = 0.0
            Dim EndTime As Double = 0.0
            Dim TotalProcessTime As Double = 0.0
            Dim WaitTime As Double = 0.0
            Dim SizeX As MIL_INT = 0
            Dim SizeY As MIL_INT = 0
            Dim NumberOfImages As MIL_INT = 0
            Dim ExtremePosY(1) As MIL_INT

            ExtremePosY(0) = 0
            ExtremePosY(1) = 0

            ' Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, MilApplication, MilSystem, MilDisplay, CType(MIL.M_NULL, IntPtr), CType(MIL.M_NULL, IntPtr))

            ' Inquire characteristics of the input sequence.

            MIL.MbufDiskInquire(SEQUENCE_FILE, MIL.M_SIZE_X, SizeX)
            MIL.MbufDiskInquire(SEQUENCE_FILE, MIL.M_SIZE_Y, SizeY)
            MIL.MbufDiskInquire(SEQUENCE_FILE, MIL.M_NUMBER_OF_IMAGES, NumberOfImages)
            MIL.MbufDiskInquire(SEQUENCE_FILE, MIL.M_FRAME_RATE, FrameRate)

            ' Allocate buffers to hold images.
            MIL.MbufAlloc2d(MilSystem, SizeX, SizeY, 8 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_PROC, MilImage)
            MIL.MbufAlloc2d(MilSystem, SizeX, SizeY, 8 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_DISP, MilDisplayImage)
            MIL.MbufAlloc2d(MilSystem, SizeX, NumberOfImages, 16 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_PROC, MilPosYImage)
            MIL.MbufAlloc2d(MilSystem, SizeX, NumberOfImages, 8 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_PROC, MilValImage)

            ' Select display.
            MIL.MdispSelect(MilDisplay, MilDisplayImage)

            ' Print a message.
            Console.Write(Constants.vbLf + "EXTRACTING 3D IMAGE FROM A LASER LINE:" + Constants.vbLf)
            Console.Write("--------------------------------------" + Constants.vbLf + Constants.vbLf)
            Console.Write("The position of a laser line is being extracted from an image" + Constants.vbLf)
            Console.Write("to generate a depth image." + Constants.vbLf + Constants.vbLf)


            ' Open the sequence file for reading.
            MIL.MbufImportSequence(SEQUENCE_FILE, MIL.M_DEFAULT, MIL.M_NULL, MIL.M_NULL, CType(MIL.M_NULL, IntPtr), MIL.M_NULL, MIL.M_NULL, MIL.M_OPEN)

            ' The function must be called once before the timing loop for more accurate 
            ' time (dll load, ...).
            MIL.MimLocatePeak1d(MilImage, MilPosYImage, MilValImage, 0, MAX_LINE_WIDTH, MIN_INTENSITY, MIL.M_1D_COLUMNS + MIL.M_FIXED_POINT + NB_FIXED_POINT, MIL.M_DEFAULT)

            ' Read and process all images in the input sequence.
            MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ + MIL.M_SYNCHRONOUS, PreviousTime)
            TotalProcessTime = 0.0

            For n = 0 To CType((NumberOfImages - 1), Integer)
                ' Read image from sequence.
                MIL.MbufImportSequence(SEQUENCE_FILE, MIL.M_DEFAULT, MIL.M_LOAD, MIL.M_NULL, MilImage, MIL.M_DEFAULT, 1, MIL.M_READ)

                ' Display the image.
                MIL.MbufCopy(MilImage, MilDisplayImage)

                ' Locate the peak in each column of the image.
                MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ + MIL.M_SYNCHRONOUS, StartTime)
                MIL.MimLocatePeak1d(MilImage, MilPosYImage, MilValImage, n, MAX_LINE_WIDTH, MIN_INTENSITY, MIL.M_1D_COLUMNS + MIL.M_FIXED_POINT + NB_FIXED_POINT, MIL.M_DEFAULT)
                MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ + MIL.M_SYNCHRONOUS, EndTime)
                TotalProcessTime += EndTime - StartTime

                ' Wait to have a proper frame rate.
                WaitTime = (1.0 / FrameRate) - (EndTime - PreviousTime)
                If (WaitTime > 0) Then
                    MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_WAIT, WaitTime)
                End If
                MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ + MIL.M_SYNCHRONOUS, PreviousTime)
            Next n

            ' Close the sequence file.
            MIL.MbufImportSequence(SEQUENCE_FILE, MIL.M_DEFAULT, MIL.M_NULL, MIL.M_NULL, CType(MIL.M_NULL, IntPtr), MIL.M_NULL, MIL.M_NULL, MIL.M_CLOSE)

            Console.Write("{0} images processed in {1,7:0.00} s ({2,7:0.00} ms/image.)" + Constants.vbLf, NumberOfImages, TotalProcessTime, TotalProcessTime / CDbl(NumberOfImages) * 1000.0)


            ' Pause to show the result.
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            Console.Write("The reconstructed images are being displayed." + Constants.vbLf)

            ' Draw extracted peak position in each column of each image.
            For n = 0 To CType((NumberOfImages - 1), Integer)
                MIL.MbufClear(MilImage, 0)
                MIL.MimDraw(MIL.M_DEFAULT, MilPosYImage, MilValImage, MilImage, MIL.M_DRAW_PEAKS + MIL.M_1D_COLUMNS + MIL.M_LINES, CDbl(n), 1, MIL.M_FIXED_POINT + NB_FIXED_POINT)

                ' Display the result image.
                MIL.MbufCopy(MilImage, MilDisplayImage)
            Next n

            ' Pause to show the result.
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()


            ' Try to allocate D3D display
            Dim DispHandle As IntPtr
            DispHandle = MdepthD3DAlloc(MilPosYImage, MilValImage, D3D_DISPLAY_SIZE_X, D3D_DISPLAY_SIZE_Y, D3D_MESH_SCALING_X, D3D_MESH_SCALING_Y, D3D_MESH_SCALING_Z, MIL.M_DEFAULT, MIL.M_DEFAULT, MIL.M_DEFAULT, IntPtr.Zero)

            If DispHandle <> IntPtr.Zero Then
                Console.Write("The depth buffer is displayed using D3D." + Constants.vbLf)

                ' Hide Mil Display
                MIL.MdispControl(MilDisplay, MIL.M_WINDOW_SHOW, MIL.M_DISABLE)

                MdispD3DShow(DispHandle)
                MdispD3DPrintHelp(DispHandle)

                ' Pause to show the result.
                Console.Write("Press <Enter> to end." + Constants.vbLf)
                Console.ReadKey()

                MdispD3DHide(DispHandle)
                MdispD3DFree(DispHandle)
            Else
                Console.Write("The depth buffer is displayed using MIL." + Constants.vbLf)

                ' Find the reMIL.Mapping for result buffers.
                MIL.MimAllocResult(MilSystem, MIL.M_DEFAULT, MIL.M_STAT_LIST, MilExtreme)

                MIL.MimStat(MilPosYImage, MilExtreme, MIL.M_MIN + MIL.M_MAX, MIL.M_NOT_EQUAL, &HFFFF, MIL.M_NULL)
                MIL.MimGetResult(MilExtreme, MIL.M_MIN + MIL.M_TYPE_MIL_INT, ExtremePosY(0))
                MIL.MimGetResult(MilExtreme, MIL.M_MAX + MIL.M_TYPE_MIL_INT, ExtremePosY(1))

                MIL.MimFree(MilExtreme)

                ' Free the display and reallocate a new one of the proper dimension for results.
                MIL.MbufFree(MilDisplayImage)
                Dim RealSizeX As MIL_INT = 0
                If D3D_MESH_SCALING_X > 0 Then
                    RealSizeX = CType((CDbl(SizeX) * D3D_MESH_SCALING_X), MIL_INT)
                Else
                    RealSizeX = CType((CDbl(SizeX) * -D3D_MESH_SCALING_X), MIL_INT)
                End If
                MIL.MbufAlloc2d(MilSystem, RealSizeX, CType((CDbl(NumberOfImages) * D3D_MESH_SCALING_Y), MIL_INT), 8 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_PROC + MIL.M_DISP, MilDisplayImage)
                MIL.MdispSelect(MilDisplay, MilDisplayImage)

                ' Display the height buffer.
                MIL.MimClip(MilPosYImage, MilPosYImage, MIL.M_GREATER, CDbl(ExtremePosY(1)), MIL.M_NULL, CDbl(ExtremePosY(1)), MIL.M_NULL)
                MIL.MimArith(MilPosYImage, CDbl(ExtremePosY(0)), MilPosYImage, MIL.M_SUB_CONST)
                MIL.MimArith(MilPosYImage, ((ExtremePosY(1) - ExtremePosY(0)) / 255) + 1, MilPosYImage, MIL.M_DIV_CONST)
                MIL.MimResize(MilPosYImage, MilDisplayImage, MIL.M_FILL_DESTINATION, MIL.M_FILL_DESTINATION, MIL.M_BILINEAR)

                ' Pause to show the result.
                Console.Write("Press <Enter> to end." + Constants.vbLf)
                Console.ReadKey()
            End If

            ' Free all allocations.
            MIL.MbufFree(MilImage)
            MIL.MbufFree(MilDisplayImage)
            MIL.MbufFree(MilPosYImage)
            MIL.MbufFree(MilValImage)
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL)
        End Sub
    End Class
End Namespace
