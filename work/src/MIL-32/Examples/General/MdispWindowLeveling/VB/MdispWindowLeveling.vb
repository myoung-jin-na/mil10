'************************************************************************************
'
' File name: MdispWindowLeveling.vb
'
' Synopsis:  This MIL program shows how to display a 10-bit monochrome Medical image
'            and applies a LUT to do interactive Window Leveling.
'************************************************************************************
Imports Microsoft.VisualBasic
Imports System
Imports System.Collections.Generic
Imports System.Text

Imports Matrox.MatroxImagingLibrary

Namespace MDispWindowLeveling
    Friend Class Program
        ' Image file to load.
        Private Const IMAGE_NAME As String = "ArmsMono10bit.mim"
        Private Const IMAGE_FILE As String = MIL.M_IMAGE_PATH & IMAGE_NAME

        ' Draw the LUT shape (if disabled reduces CPU usage).
        Private Const DRAW_LUT_SHAPE As Integer = MIL.M_YES
        Shared Sub Main(ByVal args() As String)
            Dim MilApplication As MIL_ID = MIL.M_NULL ' Application identifier.
            Dim MilSystem As MIL_ID = MIL.M_NULL ' System identifier.
            Dim MilDisplay As MIL_ID = MIL.M_NULL ' Display identifier.
            Dim MilImage As MIL_ID = MIL.M_NULL ' Image buffer identifier.
            Dim MilOriginalImage As MIL_ID = MIL.M_NULL ' Image buffer identifier.
            Dim MilLut As MIL_ID = MIL.M_NULL ' Lut buffer identifier.
            Dim ImageSizeX As MIL_INT = 0
            Dim ImageSizeY As MIL_INT = 0
            Dim ImageMaxValue As MIL_INT = 0
            Dim DisplaySizeBit As MIL_INT = 0
            Dim DisplayMaxValue As MIL_INT = 0
            Dim Start As MIL_INT = 0
            Dim LutEnd As MIL_INT = 0
            Dim Increment As MIL_INT = 0
            Dim InflectionLevel As MIL_INT = 0

            Dim Ch As New ConsoleKeyInfo()

            ' Allocate the MIL Application, System and Display.
            MIL.MappAllocDefault(MIL.M_DEFAULT, MilApplication, MilSystem, MilDisplay, CType(MIL.M_NULL, IntPtr), CType(MIL.M_NULL, IntPtr))

            ' Restore target image from disk.
            MIL.MbufRestore(IMAGE_FILE, MilSystem, MilImage)

            ' Dynamically calculates the maximum value of the image using processing.
            Dim MilExtremeResult As MIL_ID = MIL.M_NULL
            MIL.MimAllocResult(CType(MIL.MbufInquire(MilImage, MIL.M_OWNER_SYSTEM, MIL.M_NULL), MIL_ID), 1, MIL.M_EXTREME_LIST, MilExtremeResult)
            MIL.MimFindExtreme(MilImage, MilExtremeResult, MIL.M_MAX_VALUE)
            MIL.MimGetResult(MilExtremeResult, MIL.M_VALUE, ImageMaxValue)
            MIL.MimFree(MilExtremeResult)

            ' Set the maximum value of the image to indicate to MIL how to initialize 
            ' the default display LUT.
            '
            MIL.MbufControl(MilImage, MIL.M_MAX, CDbl(ImageMaxValue))

            ' Display the image (to specify a user-defined window, use MIL.MdispSelectWindow()).
            MIL.MdispSelect(MilDisplay, MilImage)

            ' Determine the maximum displayable value of the current display.
            MIL.MdispInquire(MilDisplay, MIL.M_SIZE_BIT, DisplaySizeBit)
            DisplayMaxValue = (1 << CInt(Fix(DisplaySizeBit))) - 1

            ' Print key information.
            Console.Write(Constants.vbLf + "INTERACTIVE WINDOW LEVELING:" + Constants.vbLf)
            Console.Write("----------------------------" + Constants.vbLf + Constants.vbLf)

            Console.Write("Image name : {0}" + Constants.vbLf, IMAGE_NAME)

            Console.Write("Image size : {0} x {1}" + Constants.vbLf, MIL.MbufInquire(MilImage, MIL.M_SIZE_X, ImageSizeX), MIL.MbufInquire(MilImage, MIL.M_SIZE_Y, ImageSizeY))
            Console.Write("Image max  : {0,4}" + Constants.vbLf, ImageMaxValue)
            Console.Write("Display max: {0,4}" + Constants.vbLf + Constants.vbLf, DisplayMaxValue)

            ' Allocate a LUT buffer according to the image maximum value and display pixel depth.
            If (DisplaySizeBit > 8) Then
                MIL.MbufAlloc1d(MilSystem, ImageMaxValue + 1, (16) + MIL.M_UNSIGNED, MIL.M_LUT, MilLut)
            Else
                MIL.MbufAlloc1d(MilSystem, ImageMaxValue + 1, (8) + MIL.M_UNSIGNED, MIL.M_LUT, MilLut)
            End If

            ' Generate a LUT with a full range ramp and set its maximum value.
            MIL.MgenLutRamp(MilLut, 0, 0, ImageMaxValue, CDbl(DisplayMaxValue))
            MIL.MbufControl(MilLut, MIL.M_MAX, CDbl(DisplayMaxValue))

            ' Set the display LUT.
            MIL.MdispLut(MilDisplay, MilLut)

            ' Interactive Window Leveling using keyboard.
            Console.Write("Keys assignement:" + Constants.vbLf + Constants.vbLf)
            Console.Write("Arrow keys :    Left=move Left, Right=move Right, Down=Narrower, Up=Wider." + Constants.vbLf)
            Console.Write("Intensity keys: L=Lower,  U=Upper,  R=Reset." + Constants.vbLf)
            Console.Write("Press <Enter> to end." + Constants.vbLf + Constants.vbLf)

            ' Modify LUT shape according to the arrow keys and update it.
            Start = 0
            LutEnd = ImageMaxValue
            InflectionLevel = DisplayMaxValue
            Increment = CInt((ImageMaxValue + 1) / 128)
            Increment = Math.Max(Increment, 4)
            Do While Ch.Key <> ConsoleKey.Enter
                Select Case Ch.Key
                    ' Left arrow: Move region left.
                    Case ConsoleKey.LeftArrow
                        Start -= Increment
                        LutEnd -= Increment
                        Exit Select

                        ' Right arrow: Move region right.
                    Case ConsoleKey.RightArrow
                        Start += Increment
                        LutEnd += Increment
                        Exit Select

                        ' Down arrow: Narrow region.
                    Case ConsoleKey.DownArrow
                        Start += Increment
                        LutEnd -= Increment
                        Exit Select

                        ' Up arrow: Widen region.
                    Case ConsoleKey.UpArrow
                        Start -= Increment
                        LutEnd += Increment
                        Exit Select

                        ' L key: Lower inflexion point.
                    Case ConsoleKey.L
                        InflectionLevel -= 1
                        Exit Select

                        ' U key: Upper inflexion point.
                    Case ConsoleKey.U
                        InflectionLevel += 1
                        Exit Select

                        ' R key: Reset the LUT to full image range.
                    Case ConsoleKey.R
                        Start = 0
                        LutEnd = ImageMaxValue
                        InflectionLevel = DisplayMaxValue
                        Exit Select
                End Select

                ' Saturate.
                LutEnd = Math.Min(LutEnd, ImageMaxValue)
                Start = Math.Min(Start, LutEnd)
                LutEnd = Math.Max(LutEnd, Start)
                Start = Math.Max(Start, 0)
                LutEnd = Math.Max(LutEnd, 0)
                InflectionLevel = Math.Max(InflectionLevel, 0)
                InflectionLevel = Math.Min(InflectionLevel, DisplayMaxValue)
                Console.Write("Inflection points: Low=({0},0), High=({1},{2}).   " & Constants.vbCr, Start, LutEnd, InflectionLevel)

                ' Generate a LUT with 3 slopes and saturated at both ends.
                MIL.MgenLutRamp(MilLut, 0, 0, Start, 0)
                MIL.MgenLutRamp(MilLut, Start, 0, LutEnd, CDbl(InflectionLevel))
                MIL.MgenLutRamp(MilLut, LutEnd, CDbl(InflectionLevel), ImageMaxValue, CDbl(DisplayMaxValue))

                ' Update the display LUT.
                MIL.MdispLut(MilDisplay, MilLut)

                ' Draw the current LUT's shape in the image.
                ' Note: This simple annotation method requires
                '       significant update and CPU time.
                '
                If DRAW_LUT_SHAPE = MIL.M_YES Then
                    If MilOriginalImage = MIL.M_NULL Then
                        MIL.MbufRestore(IMAGE_FILE, MilSystem, MilOriginalImage)
                    End If
                    DrawLutShape(MilDisplay, MilOriginalImage, MilImage, Start, LutEnd, InflectionLevel, ImageMaxValue, DisplayMaxValue)
                End If

                ' If its an arrow key, get the second code.
                Ch = Console.ReadKey(True)
            Loop
            Console.Write(Constants.vbLf + Constants.vbLf)

            ' Free all allocations.
            MIL.MbufFree(MilLut)
            MIL.MbufFree(MilImage)
            If MilOriginalImage <> MIL.M_NULL Then
                MIL.MbufFree(MilOriginalImage)
            End If
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL)
        End Sub

        ' Function to draw the current LUT's shape in the image.
        '
        '   Note: This simple annotation method requires significant update
        '  and CPU time since it repaints the entire image every time.
        '
        Private Shared Sub DrawLutShape(ByVal MilDisplay As MIL_ID, ByVal MilOriginalImage As MIL_ID, ByVal MilImage As MIL_ID, ByVal Start As MIL_INT, ByVal LutEnd As MIL_INT, ByVal InflexionIntensity As MIL_INT, ByVal ImageMaxValue As MIL_INT, ByVal DisplayMaxValue As MIL_INT)
            Dim Xstart As Double = 0.0
            Dim Xend As Double = 0.0
            Dim Xstep As Double = 0.0
            Dim Ymin As Double = 0.0
            Dim Yinf As Double = 0.0
            Dim Ymax As Double = 0.0
            Dim Ystep As Double = 0.0
            Dim ImageSizeX As MIL_INT = 0
            Dim ImageSizeY As MIL_INT = 0

            ' Inquire image dimensions.
            MIL.MbufInquire(MilImage, MIL.M_SIZE_X, ImageSizeX)
            MIL.MbufInquire(MilImage, MIL.M_SIZE_Y, ImageSizeY)

            ' Calculate the drawing parameters.
            Xstep = CDbl(ImageSizeX) / CDbl(ImageMaxValue)
            Xstart = CDbl(Start) * Xstep
            Xend = CDbl(LutEnd) * Xstep
            Ystep = (CDbl(ImageSizeY) / 4.0) / CDbl(DisplayMaxValue)
            Ymin = (CDbl(ImageSizeY) - 2)
            Yinf = Ymin - (CDbl(InflexionIntensity) * Ystep)
            Ymax = Ymin - (CDbl(DisplayMaxValue) * Ystep)

            ' To increase speed, disable display update until all annotations are done.
            MIL.MdispControl(MilDisplay, MIL.M_UPDATE, MIL.M_DISABLE)

            ' Restore the original image.
            MIL.MbufCopy(MilOriginalImage, MilImage)

            ' Draw axis max and min values.
            MIL.MgraColor(MIL.M_DEFAULT, CDbl(ImageMaxValue))
            MIL.MgraText(MIL.M_DEFAULT, MilImage, 4, CInt(Fix(Ymin)) - 22, "0")
            MIL.MgraText(MIL.M_DEFAULT, MilImage, 4, CInt(Fix(Ymax)) - 16, String.Format("{0}", DisplayMaxValue))
            MIL.MgraText(MIL.M_DEFAULT, MilImage, ImageSizeX - 38, CInt(Fix(Ymin)) - 22, String.Format("{0}", ImageMaxValue))

            ' Draw LUT Shape (X axis is display values and Y is image values).
            MIL.MgraLine(MIL.M_DEFAULT, MilImage, 0, CInt(Fix(Ymin)), CInt(Fix(Xstart)), CInt(Fix(Ymin)))
            MIL.MgraLine(MIL.M_DEFAULT, MilImage, CInt(Fix(Xstart)), CInt(Fix(Ymin)), CInt(Fix(Xend)), CInt(Fix(Yinf)))
            MIL.MgraLine(MIL.M_DEFAULT, MilImage, CInt(Fix(Xend)), CInt(Fix(Yinf)), ImageSizeX - 1, CInt(Fix(Ymax)))

            ' Enable display update to show the result.
            MIL.MdispControl(MilDisplay, MIL.M_UPDATE, MIL.M_ENABLE)
        End Sub
    End Class
End Namespace
