'''****************************************************************************
'
' File name: MimWarp.vb
'
' Synopsis:  : This program performs three types of warp transformations. 
'              First the image is stretched according to four specified 
'              reference points. Then it is warped on a sinusoid, and 
'              finally, the program loops while warping the image on a 
'              sphere.
'*****************************************************************************
Imports Microsoft.VisualBasic
Imports System
Imports System.Collections.Generic
Imports System.Text

Imports Matrox.MatroxImagingLibrary

Namespace MImWarp
    Friend Class Program
        ' Target image specifications.
        Private Const IMAGE_FILE As String = MIL.M_IMAGE_PATH & "BaboonMono.mim"
        Private Const FIXED_POINT_PRECISION As Integer = MIL.M_FIXED_POINT + 6
        Private Shared Function FLOAT_TO_FIXED_POINT(ByVal x As Double) As Integer
            Dim ReturnValue As Integer = CInt(64 * (x))
            If ReturnValue > Short.MaxValue Then
                ReturnValue = ReturnValue Or &HFFFF0000
            End If
            Return ReturnValue
        End Function
        Private Const ROTATION_STEP As Integer = 2

        Shared Sub Main(ByVal args() As String)
            Dim MilApplication As MIL_ID = MIL.M_NULL ' Application identifier.
            Dim MilSystem As MIL_ID = MIL.M_NULL ' System identifier.
            Dim MilDisplay As MIL_ID = MIL.M_NULL ' Display identifier.
            Dim MilDisplayImage As MIL_ID = MIL.M_NULL ' Image buffer identifier.
            Dim MilSourceImage As MIL_ID = MIL.M_NULL ' Image buffer identifier.
            Dim Mil4CornerArray As MIL_ID = MIL.M_NULL ' Coefficients buffer identifier.
            Dim MilLutX As MIL_ID = MIL.M_NULL ' Lut buffer identifier.
            Dim MilLutY As MIL_ID = MIL.M_NULL ' Lut buffer identifier.
            Dim ChildWindow As MIL_ID = MIL.M_NULL ' Child Image identifier.
            Dim FourCornerMatrix() As Single = {0.0F, 0.0F, 456.0F, 62.0F, 333.0F, 333.0F, 100.0F, 500.0F, 0.0F, 0.0F, 511.0F, 511.0F} ' Y coordinate of rectangle's bottom-right corner
            Dim Precision As Integer = FIXED_POINT_PRECISION
            Dim Interpolation As Integer = MIL.M_NEAREST_NEIGHBOR
            Dim MilLutXPtr(), MilLutYPtr() As Short
            Dim OffsetX As Integer = 0
            Dim ImageWidth As MIL_INT = 0
            Dim ImageHeight As MIL_INT = 0
            Dim ImageType As MIL_INT = 0
            Dim i As Integer = 0
            Dim j As Integer = 0
            Dim FramesPerSecond As Double = 0.0
            Dim Time As Double = 0.0
            Dim NbLoop As Double = 0.0

            ' Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, MilApplication, MilSystem, MilDisplay, CType(MIL.M_NULL, IntPtr), CType(MIL.M_NULL, IntPtr))

            ' Restore the source image.
            MIL.MbufRestore(IMAGE_FILE, MilSystem, MilSourceImage)

            ' Allocate a display buffers and show the source image.
            MIL.MbufInquire(MilSourceImage, MIL.M_SIZE_X, ImageWidth)
            MIL.MbufInquire(MilSourceImage, MIL.M_SIZE_Y, ImageHeight)
            MIL.MbufInquire(MilSourceImage, MIL.M_TYPE, ImageType)
            MIL.MbufAlloc2d(MilSystem, ImageWidth, ImageHeight, ImageType, MIL.M_IMAGE + MIL.M_PROC + MIL.M_DISP, MilDisplayImage)
            MIL.MbufCopy(MilSourceImage, MilDisplayImage)
            MIL.MdispSelect(MilDisplay, MilDisplayImage)

            ' Print a message.
            Console.Write(Constants.vbLf + "WARPING:" + Constants.vbLf)
            Console.Write("--------" + Constants.vbLf + Constants.vbLf)
            Console.Write("This image will be warped using different methods." + Constants.vbLf)
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()


            ' Four-corner LUT warping
            '-------------------------

            ' Allocate 2 LUT buffers.
            MIL.MbufAlloc2d(MilSystem, ImageWidth, ImageHeight, 16 + MIL.M_SIGNED, MIL.M_LUT, MilLutX)
            MIL.MbufAlloc2d(MilSystem, ImageWidth, ImageHeight, 16 + MIL.M_SIGNED, MIL.M_LUT, MilLutY)

            ' Allocate the coefficient buffer.
            MIL.MbufAlloc2d(MilSystem, 12, 1, 32 + MIL.M_FLOAT, MIL.M_ARRAY, Mil4CornerArray)

            ' Put warp values into the coefficient buffer.
            MIL.MbufPut1d(Mil4CornerArray, 0, 12, FourCornerMatrix)

            ' Generate LUT buffers.
            MIL.MgenWarpParameter(Mil4CornerArray, MilLutX, MilLutY, MIL.M_WARP_4_CORNER + Precision, MIL.M_DEFAULT, 0.0, 0.0)

            ' Clear the destination.
            MIL.MbufClear(MilDisplayImage, 0)

            ' Warp the image.
            MIL.MimWarp(MilSourceImage, MilDisplayImage, MilLutX, MilLutY, MIL.M_WARP_LUT + Precision, Interpolation)

            ' Print a message.
            Console.Write("The image was warped from an arbitrary quadrilateral to a square." + Constants.vbLf)
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()


            ' Sinusoidal LUT warping
            '------------------------

            ' Allocate user-defined LUTs.
            MilLutXPtr = New Short(CType((ImageHeight * ImageWidth - 1), Integer)) {}
            MilLutYPtr = New Short(CType((ImageHeight * ImageWidth - 1), Integer)) {}

            ' Fill the LUT with a sinusoidal waveforms with a 6-bit precision.
            For j = 0 To CType((ImageHeight - 1), Integer)
                For i = 0 To CType((ImageWidth - 1), Integer)
                    MilLutYPtr(CType((i + (j * ImageWidth)), Integer)) = CShort(Fix(FLOAT_TO_FIXED_POINT(((j) + CInt(Fix((20 * Math.Sin(0.03 * i))))))))
                    MilLutXPtr(CType((i + (j * ImageWidth)), Integer)) = CShort(Fix(FLOAT_TO_FIXED_POINT(((i) + CInt(Fix((20 * Math.Sin(0.03 * j))))))))
                Next i
            Next j

            ' Put the values into the LUT buffers.
            MIL.MbufPut2d(MilLutX, 0, 0, ImageWidth, ImageHeight, MilLutXPtr)
            MIL.MbufPut2d(MilLutY, 0, 0, ImageWidth, ImageHeight, MilLutYPtr)

            ' Clear the destination.
            MIL.MbufClear(MilDisplayImage, 0)

            ' Warp the image.
            MIL.MimWarp(MilSourceImage, MilDisplayImage, MilLutX, MilLutY, MIL.M_WARP_LUT + Precision, Interpolation)

            ' wait for a key
            Console.Write("The image was warped on two sinusoidal waveforms." + Constants.vbLf)
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Continuous spherical LUT warping
            '--------------------------------

            ' Allocate temporary buffer.
            MIL.MbufFree(MilSourceImage)
            MIL.MbufAlloc2d(MilSystem, ImageWidth * 2, ImageHeight, ImageType, MIL.M_IMAGE + MIL.M_PROC, MilSourceImage)

            ' Reload the image.
            MIL.MbufLoad(IMAGE_FILE, MilSourceImage)

            ' Fill the LUTs with a sphere pattern with a 6-bit precision.
            GenerateSphericLUT(ImageWidth, ImageHeight, MilLutXPtr, MilLutYPtr)
            MIL.MbufPut2d(MilLutX, 0, 0, ImageWidth, ImageHeight, MilLutXPtr)
            MIL.MbufPut2d(MilLutY, 0, 0, ImageWidth, ImageHeight, MilLutYPtr)

            ' Duplicate the buffer to allow wrap around in the warping.
            MIL.MbufCopy(MilSourceImage, MilDisplayImage)
            MIL.MbufChild2d(MilSourceImage, ImageWidth, 0, ImageWidth, ImageHeight, ChildWindow)
            MIL.MbufCopy(MilDisplayImage, ChildWindow)
            MIL.MbufFree(ChildWindow)

            ' Clear the destination.
            MIL.MbufClear(MilDisplayImage, 0)

            ' Print a message and start the timer.
            Console.Write("The image is continuously warped on a sphere." + Constants.vbLf)
            Console.Write("Press <Enter> to stop." + Constants.vbLf + Constants.vbLf)
            MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_RESET + MIL.M_SYNCHRONOUS, CType(MIL.M_NULL, IntPtr))

            ' Warp the image continuously.
            Do While Not Console.KeyAvailable
                ' Create a child in the buffer containing the two images.
                MIL.MbufChild2d(MilSourceImage, OffsetX, 0, ImageWidth, ImageHeight, ChildWindow)

                ' Warp the child in the window.
                MIL.MimWarp(ChildWindow, MilDisplayImage, MilLutX, MilLutY, MIL.M_WARP_LUT + Precision, Interpolation)

                ' Update the offset (shift the window to the right).
                OffsetX += ROTATION_STEP

                ' Reset the offset if the child is outside the buffer.
                If OffsetX > ImageWidth - 1 Then
                    OffsetX = 0
                End If

                ' Free the child.
                MIL.MbufFree(ChildWindow)

                NbLoop += 1

                ' Calculate and print the number of frames per second processed.
                MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ + MIL.M_SYNCHRONOUS, Time)
                FramesPerSecond = NbLoop / Time
                Console.Write("Processing speed: {0:0} Images/Sec." & Constants.vbCr, FramesPerSecond)
                YieldToGUI()
            Loop
            Console.ReadKey()

            ' Free objects.
            MIL.MbufFree(MilLutX)
            MIL.MbufFree(MilLutY)
            MIL.MbufFree(Mil4CornerArray)
            MIL.MbufFree(MilSourceImage)
            MIL.MbufFree(MilDisplayImage)
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL)
        End Sub

        ' Generate two custom LUTs used to map the image on a sphere.
        ' -----------------------------------------------------------
        Private Shared Sub GenerateSphericLUT(ByVal ImageWidth As MIL_INT, ByVal ImageHeight As MIL_INT, ByVal MilLutXPtr() As Short, ByVal MilLutYPtr() As Short)
            Dim i, j, k As MIL_INT
            Dim utmp, vtmp, tmp As Double
            Dim v As Short

            ' Set the radius of the sphere
            Dim Radius As Double = 200.0

            ' Generate the X and Y buffers
            For j = 0 To ImageHeight - 1
                k = j * ImageWidth

                ' Check that still in the sphere (in the Y axis).
                vtmp = (CDbl(j - (ImageHeight / 2)) / Radius)
                If Math.Abs(vtmp) < 1.0 Then
                    ' We scan from top to bottom, so reverse the value obtained above
                    ' and obtain the angle.
                    vtmp = Math.Acos(-vtmp)
                    If vtmp = 0.0 Then
                        vtmp = 0.0000001
                    End If

                    ' Compute the position to fetch in the source.
                    v = CShort(Fix((vtmp / 3.1415926) * CDbl(ImageHeight - 1) + 0.5))

                    ' Compute the Y coordinate of the sphere.
                    tmp = Radius * Math.Sin(vtmp)

                    For i = 0 To ImageWidth - 1
                        ' Check that still in the sphere.
                        utmp = (CDbl(i - (ImageWidth / 2)) / tmp)
                        If Math.Abs(utmp) < 1.0 Then
                            utmp = Math.Acos(-utmp)

                            ' Compute the position to fetch (fold the image in four).
                            MilLutXPtr(CType((i + k), Integer)) = CShort(Fix(FLOAT_TO_FIXED_POINT(((utmp / 3.1415926) * CDbl((ImageWidth / 2) - 1) + 0.5))))
                            MilLutYPtr(CType((i + k), Integer)) = CShort(Fix(FLOAT_TO_FIXED_POINT(v)))
                        Else
                            ' Default position (fetch outside the buffer to 
                            ' activate the clear overscan).
                            MilLutXPtr(CType((i + k), Integer)) = CShort(Fix(FLOAT_TO_FIXED_POINT(ImageWidth)))
                            MilLutYPtr(CType((i + k), Integer)) = CShort(Fix(FLOAT_TO_FIXED_POINT(ImageHeight)))
                        End If
                    Next i
                Else
                    For i = 0 To ImageWidth - 1
                        ' Default position (fetch outside the buffer for clear overscan).
                        MilLutXPtr(CType((i + k), Integer)) = CShort(Fix(FLOAT_TO_FIXED_POINT(ImageWidth)))
                        MilLutYPtr(CType((i + k), Integer)) = CShort(Fix(FLOAT_TO_FIXED_POINT(ImageHeight)))
                    Next i
                End If
            Next j
        End Sub

        ' Windows CE GUI Scheduling Adjustment Handling
        ' ---------------------------------------------
        ' NOTE: Under Windows CE, YieldToGUI function improves system responsiveness 
        '       in case a normal Windows CE console application thread is processing 
        '       in a while loop.
        Private Shared Sub YieldToGUI()
#If M_MIL_USE_CE Then
            System.Threading.Thread.Sleep(0)
#End If
        End Sub
    End Class
End Namespace
