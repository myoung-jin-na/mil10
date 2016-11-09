'**********************************************************************************
' 
' File name: MimHistogram.vb
'
' Synopsis:  This program loads an image of a tissue sample, calculates its intensity
'            histogram and draws it.
'**********************************************************************************
Imports Microsoft.VisualBasic
Imports System
Imports System.Collections.Generic
Imports System.Text

Imports Matrox.MatroxImagingLibrary

Namespace MImHistogram
    Friend Class Program
        ' Target MIL image file specifications.
        Private Const IMAGE_FILE As String = MIL.M_IMAGE_PATH & "Cell.mim"

        ' Number of possible pixel intensities.
        Private Const HIST_NUM_INTENSITIES As Integer = 256
        Private Const HIST_SCALE_FACTOR As Integer = 8
        Private Const HIST_X_POSITION As Integer = 250
        Private Const HIST_Y_POSITION As Integer = 450

        ' Main function.
        Shared Sub Main(ByVal args() As String)
            Dim MilApplication As MIL_ID = MIL.M_NULL ' Application identifier.
            Dim MilSystem As MIL_ID = MIL.M_NULL ' System identifier.
            Dim MilDisplay As MIL_ID = MIL.M_NULL ' Display identifier.
            Dim MilImage As MIL_ID = MIL.M_NULL ' Image buffer identifier.
            Dim MilOverlayImage As MIL_ID = MIL.M_NULL ' Overlay buffer identifier.
            Dim HistResult As MIL_ID = MIL.M_NULL ' Histogram buffer identifier.
            Dim HistValues(HIST_NUM_INTENSITIES - 1) As MIL_INT ' Histogram values.
            Dim XStart(HIST_NUM_INTENSITIES - 1) As Double
            Dim YStart(HIST_NUM_INTENSITIES - 1) As Double
            Dim XEnd(HIST_NUM_INTENSITIES - 1) As Double
            Dim YEnd(HIST_NUM_INTENSITIES - 1) As Double
            Dim AnnotationColor As Double = MIL.M_COLOR_RED

            ' Allocate the default system and image buffer.
            MIL.MappAllocDefault(MIL.M_DEFAULT, MilApplication, MilSystem, MilDisplay, CType(MIL.M_NULL, IntPtr), CType(MIL.M_NULL, IntPtr))

            ' Restore source image into an automatically allocated image buffer.
            MIL.MbufRestore(IMAGE_FILE, MilSystem, MilImage)

            ' Display the image buffer and prepare for overlay annotations.
            MIL.MdispSelect(MilDisplay, MilImage)
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY, MIL.M_ENABLE)
            MIL.MdispInquire(MilDisplay, MIL.M_OVERLAY_ID, MilOverlayImage)

            ' Allocate a histogram result buffer.
            MIL.MimAllocResult(MilSystem, HIST_NUM_INTENSITIES, MIL.M_HIST_LIST, HistResult)

            ' Calculate the histogram.
            MIL.MimHistogram(MilImage, HistResult)

            ' Get the results.
            MIL.MimGetResult(HistResult, MIL.M_VALUE, HistValues)

            ' Draw the histogram in the overlay.
            MIL.MgraColor(MIL.M_DEFAULT, AnnotationColor)
            For i As Integer = 0 To HIST_NUM_INTENSITIES - 1
                XStart(i) = i + HIST_X_POSITION + 1
                YStart(i) = HIST_Y_POSITION
                XEnd(i) = i + HIST_X_POSITION + 1
                YEnd(i) = HIST_Y_POSITION - (HistValues(i) / HIST_SCALE_FACTOR)
            Next i
            MIL.MgraLines(MIL.M_DEFAULT, MilOverlayImage, HIST_NUM_INTENSITIES, XStart, YStart, XEnd, YEnd, MIL.M_DEFAULT)

            ' Print a message.
            Console.Write(Constants.vbLf + "HISTOGRAM:" + Constants.vbLf)
            Console.Write("----------" + Constants.vbLf + Constants.vbLf)
            Console.Write("The histogram of the image was calculated and drawn." + Constants.vbLf)
            Console.Write("Press <Enter> to end." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Free all allocations.
            MIL.MimFree(HistResult)
            MIL.MbufFree(MilImage)
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL)
        End Sub
    End Class
End Namespace
