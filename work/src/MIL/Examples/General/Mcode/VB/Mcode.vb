'**************************************************************************************
' 
' File name: Mcode.vb
'
' Synopsis:  This program decodes a 1D Code 39 linear Barcode and a 2D DataMatrix code.
'
'**************************************************************************************
Imports Microsoft.VisualBasic
Imports System
Imports System.Collections.Generic
Imports System.Text
Imports Matrox.MatroxImagingLibrary

Namespace MCode
    Friend Class Program
        Shared Sub Main(ByVal args() As String)
            Const IMAGE_FILE As String = MIL.M_IMAGE_PATH & "Code.mim"
            Const STRING_LENGTH_MAX As Integer = 64

            ' Regions around 1D code.
            Const BARCODE_REGION_TOP_LEFT_X As Integer = 256
            Const BARCODE_REGION_TOP_LEFT_Y As Integer = 80
            Const BARCODE_REGION_SIZE_X As Integer = 290
            Const BARCODE_REGION_SIZE_Y As Integer = 60

            ' Regions around 2D code.
            Const DATAMATRIX_REGION_TOP_LEFT_X As Integer = 8
            Const DATAMATRIX_REGION_TOP_LEFT_Y As Integer = 312
            Const DATAMATRIX_REGION_SIZE_X As Integer = 118
            Const DATAMATRIX_REGION_SIZE_Y As Integer = 105

            Dim MilApplication As MIL_ID = MIL.M_NULL ' Application identifier.
            Dim MilSystem As MIL_ID = MIL.M_NULL ' System identifier.
            Dim MilDisplay As MIL_ID = MIL.M_NULL ' Display identifier.
            Dim MilImage As MIL_ID = MIL.M_NULL ' Image buffer identifier.
            Dim MilOverlayImage As MIL_ID = MIL.M_NULL ' Image buffer identifier.
            Dim DataMatrixRegion As MIL_ID = MIL.M_NULL ' Child containing DataMatrix.
            Dim DataMatrixCode As MIL_ID = MIL.M_NULL ' DataMatrix 2D code identifier.
            Dim BarCodeRegion As MIL_ID = MIL.M_NULL ' Child containing Code39.
            Dim Barcode As MIL_ID = MIL.M_NULL ' Code39 barcode identifier.
            Dim CodeResults As MIL_ID = MIL.M_NULL ' Barcode results identifier.

            Dim BarcodeStatus As MIL_INT = MIL.M_NULL ' Decoding status.
            Dim DataMatrixStatus As MIL_INT = MIL.M_NULL ' Decoding status. 
            Dim AnnotationColor As Double = MIL.M_COLOR_GREEN
            Dim AnnotationBackColor As Double = MIL.M_COLOR_GRAY
            Dim n As Integer = 0
            Dim BarcodeString As New StringBuilder(STRING_LENGTH_MAX) ' Array of characters read.
            Dim DataMatrixString As New StringBuilder(STRING_LENGTH_MAX) ' Array of characters read.


            ' Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, MilApplication, MilSystem, MilDisplay, CType(MIL.M_NULL, IntPtr), CType(MIL.M_NULL, IntPtr))

            ' Restore source image into an automatically allocated image buffer.
            MIL.MbufRestore(IMAGE_FILE, MilSystem, MilImage)

            ' Display the image buffer.
            MIL.MdispSelect(MilDisplay, MilImage)

            ' Prepare for overlay annotations.
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY, MIL.M_ENABLE)
            MIL.MdispInquire(MilDisplay, MIL.M_OVERLAY_ID, MilOverlayImage)

            ' Prepare bar code results buffer
            MIL.McodeAllocResult(MilSystem, MIL.M_DEFAULT, CodeResults)

            ' Pause to show the original image.
            Console.Write(Constants.vbLf + "1D and 2D CODE READING:" + Constants.vbLf)
            Console.Write("-----------------------" + Constants.vbLf + Constants.vbLf)
            Console.Write("This program will decode a linear Code 39 and a DataMatrix code." + Constants.vbLf)
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()


            ' 1D BARCODE READING:

            ' Create a read region around the code to speedup reading.
            MIL.MbufChild2d(MilImage, BARCODE_REGION_TOP_LEFT_X, BARCODE_REGION_TOP_LEFT_Y, BARCODE_REGION_SIZE_X, BARCODE_REGION_SIZE_Y, BarCodeRegion)

            ' Allocate CODE objects.
            MIL.McodeAlloc(MilSystem, MIL.M_DEFAULT, MIL.M_DEFAULT, Barcode)
            MIL.McodeModel(Barcode, MIL.M_ADD, MIL.M_CODE39, MIL.M_NULL, MIL.M_DEFAULT, CType(MIL.M_NULL, IntPtr))

            ' Read codes from image.
            MIL.McodeRead(Barcode, BarCodeRegion, CodeResults)

            ' Get decoding status.
            MIL.McodeGetResult(CodeResults, MIL.M_STATUS + MIL.M_TYPE_MIL_INT, BarcodeStatus)

            ' Check if decoding was successful.
            If BarcodeStatus = MIL.M_STATUS_READ_OK Then
                ' Get decoded string.
                MIL.McodeGetResult(CodeResults, MIL.M_STRING, BarcodeString)

                ' Draw the decoded strings and read region in the overlay image.
                MIL.MgraColor(MIL.M_DEFAULT, AnnotationColor)
                MIL.MgraBackColor(MIL.M_DEFAULT, AnnotationBackColor)
                MIL.MgraText(MIL.M_DEFAULT, MilOverlayImage, BARCODE_REGION_TOP_LEFT_X + 10, BARCODE_REGION_TOP_LEFT_Y + 80, " 1D linear 39 bar code:           ")
                MIL.MgraText(MIL.M_DEFAULT, MilOverlayImage, BARCODE_REGION_TOP_LEFT_X + 200, BARCODE_REGION_TOP_LEFT_Y + 80, BarcodeString.ToString())
                MIL.MgraRect(MIL.M_DEFAULT, MilOverlayImage, BARCODE_REGION_TOP_LEFT_X, BARCODE_REGION_TOP_LEFT_Y, BARCODE_REGION_TOP_LEFT_X + BARCODE_REGION_SIZE_X, BARCODE_REGION_TOP_LEFT_Y + BARCODE_REGION_SIZE_Y)
            End If

            ' Free objects.
            MIL.McodeFree(Barcode)
            MIL.MbufFree(BarCodeRegion)

            ' 2D CODE READING:

            ' Create a read region around the code to speedup reading.
            MIL.MbufChild2d(MilImage, DATAMATRIX_REGION_TOP_LEFT_X, DATAMATRIX_REGION_TOP_LEFT_Y, DATAMATRIX_REGION_SIZE_X, DATAMATRIX_REGION_SIZE_Y, DataMatrixRegion)

            ' Allocate CODE objects.
            MIL.McodeAlloc(MilSystem, MIL.M_DEFAULT, MIL.M_DEFAULT, DataMatrixCode)
            MIL.McodeModel(DataMatrixCode, MIL.M_ADD, MIL.M_DATAMATRIX, MIL.M_NULL, MIL.M_DEFAULT, CType(MIL.M_NULL, IntPtr))


            ' Set the foreground value for the DataMatrix since it is different from the default value.
            MIL.McodeControl(DataMatrixCode, MIL.M_FOREGROUND_VALUE, MIL.M_FOREGROUND_WHITE)

            ' Read codes from image.
            MIL.McodeRead(DataMatrixCode, DataMatrixRegion, CodeResults)

            ' Get decoding status.
            MIL.McodeGetResult(CodeResults, MIL.M_STATUS + MIL.M_TYPE_MIL_INT, DataMatrixStatus)

            ' Check if decoding was successful.
            If DataMatrixStatus = MIL.M_STATUS_READ_OK Then
                ' Get decoded string.
                MIL.McodeGetResult(CodeResults, MIL.M_STRING, DataMatrixString)

                ' Draw the decoded strings and read region in the overlay image.
                MIL.MgraColor(MIL.M_DEFAULT, AnnotationColor)
                MIL.MgraBackColor(MIL.M_DEFAULT, AnnotationBackColor)
                For n = 0 To DataMatrixString.Length - 1 ' Replace non printable characters with space.*/
                    If (DataMatrixString(n) < "0"c) OrElse (DataMatrixString(n) > "Z"c) Then
                        DataMatrixString(n) = " "c
                    End If
                Next n
                MIL.MgraText(MIL.M_DEFAULT, MilOverlayImage, DATAMATRIX_REGION_TOP_LEFT_X, DATAMATRIX_REGION_TOP_LEFT_Y + 120, " 2D Data Matrix code:                  ")
                MIL.MgraText(MIL.M_DEFAULT, MilOverlayImage, DATAMATRIX_REGION_TOP_LEFT_X + 200, DATAMATRIX_REGION_TOP_LEFT_Y + 120, DataMatrixString.ToString())
                MIL.MgraRect(MIL.M_DEFAULT, MilOverlayImage, DATAMATRIX_REGION_TOP_LEFT_X, DATAMATRIX_REGION_TOP_LEFT_Y, DATAMATRIX_REGION_TOP_LEFT_X + DATAMATRIX_REGION_SIZE_X, DATAMATRIX_REGION_TOP_LEFT_Y + DATAMATRIX_REGION_SIZE_Y)
            End If

            ' Free objects.
            MIL.McodeFree(DataMatrixCode)
            MIL.MbufFree(DataMatrixRegion)

            ' Free results buffer.
            MIL.McodeFree(CodeResults)

            ' Pause to show the results.
            If (DataMatrixStatus = MIL.M_STATUS_READ_OK) AndAlso (BarcodeStatus = MIL.M_STATUS_READ_OK) Then
                Console.Write("Decoding was successful and the strings were written under each code." + Constants.vbLf)
            Else
                Console.Write("Decoding error found." + Constants.vbLf)
            End If
            Console.Write("Press <Enter> to end." + Constants.vbLf)
            Console.ReadKey()

            ' Free other allocations.
            MIL.MbufFree(MilImage)
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL)
        End Sub
    End Class
End Namespace
