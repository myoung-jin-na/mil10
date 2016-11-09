'*******************************************************************************
'
' File name: Mocr.vb
'
' Synopsis:  This program uses the OCR module to read a SEMI font string:
'            the example calibrates a predefined OCR font (semi font) 
'            and uses it to read a string in the image. The string read
'            is then printed to the screen and the calibrated font is
'            saved to disk.
'*******************************************************************************
Imports Microsoft.VisualBasic
Imports System
Imports System.Collections.Generic
Imports System.Text

Imports Matrox.MatroxImagingLibrary

Namespace MOcr
    Friend Class Program
        ' *****************************************************************************
        ' OCR SEMI font read example.
        ' *****************************************************************************

        ' Target image character specifications.
        Private Const CHAR_IMAGE_FILE As String = MIL.M_IMAGE_PATH & "OcrSemi1292.mim"
        Private Const CHAR_SIZE_X_MIN As Double = 22.0
        Private Const CHAR_SIZE_X_MAX As Double = 23.0
        Private Const CHAR_SIZE_X_STEP As Double = 0.5
        Private Const CHAR_SIZE_Y_MIN As Double = 43.0
        Private Const CHAR_SIZE_Y_MAX As Double = 44.0
        Private Const CHAR_SIZE_Y_STEP As Double = 0.5

        ' Target reading specifications.
        Private Const READ_REGION_POS_X As Integer = 30
        Private Const READ_REGION_POS_Y As Integer = 40
        Private Const READ_REGION_WIDTH As Integer = 420
        Private Const READ_REGION_HEIGHT As Integer = 70
        Private Const READ_SCORE_MIN As Double = 50.0

        ' Font file names. 
        Private Const FONT_FILE_IN As String = MIL.M_CONTEXT_PATH + "SEMI_M12-92.mfo"
        Private Const FONT_FILE_OUT As String = MIL.M_TEMP_DIR + "Semi1292Calibrated.mfo"

        ' Length of the string to read (null terminated).
        Private Const STRING_LENGTH As Integer = 13
        Private Const STRING_CALIBRATION As String = "M940902-MXD5"

        Shared Sub Main(ByVal args() As String)
            Dim MilApplication As MIL_ID = MIL.M_NULL ' Application identifier.
            Dim MilSystem As MIL_ID = MIL.M_NULL ' System identifier.
            Dim MilDisplay As MIL_ID = MIL.M_NULL ' Display identifier.
            Dim MilImage As MIL_ID = MIL.M_NULL ' Image buffer identifier.
            Dim MilSubImage As MIL_ID = MIL.M_NULL ' Sub-image buffer identifier.
            Dim MilFontSubImage As MIL_ID = MIL.M_NULL ' Font display sub image.
            Dim MilOverlayImage As MIL_ID = MIL.M_NULL ' Overlay image.
            Dim OcrFont As MIL_ID = MIL.M_NULL ' OCR font identifier.
            Dim OcrResult As MIL_ID = MIL.M_NULL ' OCR result buffer identifier.
            Dim Score As Double = 0 ' Reading score.
            Dim SizeX As MIL_INT = 0 ' Source image size x.
            Dim SizeY As MIL_INT = 0 ' Source image size y.
            Dim Type As MIL_INT = 0 ' Source image type.

            Dim ReadString As New StringBuilder(STRING_LENGTH) ' Characters to read.

            Console.Write(Constants.vbLf + "OCR MODULE (SEMI font reading):" + Constants.vbLf)
            Console.Write("-------------------------------" + Constants.vbLf + Constants.vbLf)

            ' Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, MilApplication, MilSystem, MilDisplay, CType(MIL.M_NULL, IntPtr), CType(MIL.M_NULL, IntPtr))

            ' Load and display the source image into a new image buffer.
            MIL.MbufAlloc2d(MilSystem, MIL.MbufDiskInquire(CHAR_IMAGE_FILE, MIL.M_SIZE_X, SizeX), MIL.MbufDiskInquire(CHAR_IMAGE_FILE, MIL.M_SIZE_Y, SizeY) * 3 / 2, MIL.MbufDiskInquire(CHAR_IMAGE_FILE, MIL.M_TYPE, Type), MIL.M_IMAGE + MIL.M_PROC + MIL.M_DISP, MilImage)
            MIL.MbufClear(MilImage, 0)
            MIL.MbufLoad(CHAR_IMAGE_FILE, MilImage)
            MIL.MdispSelect(MilDisplay, MilImage)

            ' Restrict the region of the image where to read the string.
            MIL.MbufChild2d(MilImage, READ_REGION_POS_X, READ_REGION_POS_Y, READ_REGION_WIDTH, READ_REGION_HEIGHT, MilSubImage)

            ' Define the bottom of the image as the region where to copy the font representation.
            MIL.MbufChild2d(MilImage, 50, SizeY + 10, SizeX - 100, (SizeY / 3) - 10, MilFontSubImage)


            ' Restore the OCR character font from disk.
            MIL.MocrRestoreFont(FONT_FILE_IN, MIL.M_RESTORE, MilSystem, OcrFont)

            ' Show the font representation. 
            MIL.MocrCopyFont(MilFontSubImage, OcrFont, MIL.M_COPY_FROM_FONT + MIL.M_ALL_CHAR, "")

            ' Pause to show the original image.
            Console.Write("The SEMI string at the top will be read using the font displayed at the bottom." + Constants.vbLf + Constants.vbLf)
            Console.Write("Calibrating SEMI font..." + Constants.vbLf + Constants.vbLf)

            ' Calibrate the OCR font.
            MIL.MocrCalibrateFont(MilSubImage, OcrFont, STRING_CALIBRATION, CHAR_SIZE_X_MIN, CHAR_SIZE_X_MAX, CHAR_SIZE_X_STEP, CHAR_SIZE_Y_MIN, CHAR_SIZE_Y_MAX, CHAR_SIZE_Y_STEP, MIL.M_DEFAULT)

            ' Set the user-specific character constraints for each string position.
            MIL.MocrSetConstraint(OcrFont, 0, MIL.M_LETTER) ' A to Z only
            MIL.MocrSetConstraint(OcrFont, 1, MIL.M_DIGIT, "9") ' 9 only
            MIL.MocrSetConstraint(OcrFont, 2, MIL.M_DIGIT) ' 0 to 9 only
            MIL.MocrSetConstraint(OcrFont, 3, MIL.M_DIGIT) ' 0 to 9 only
            MIL.MocrSetConstraint(OcrFont, 4, MIL.M_DIGIT) ' 0 to 9 only
            MIL.MocrSetConstraint(OcrFont, 5, MIL.M_DIGIT) ' 0 to 9 only
            MIL.MocrSetConstraint(OcrFont, 6, MIL.M_DIGIT) ' 0 to 9 only
            MIL.MocrSetConstraint(OcrFont, 7, MIL.M_DEFAULT, "-") ' - only
            MIL.MocrSetConstraint(OcrFont, 8, MIL.M_LETTER, "M") ' M only
            MIL.MocrSetConstraint(OcrFont, 9, MIL.M_LETTER, "X") ' X only
            MIL.MocrSetConstraint(OcrFont, 10, MIL.M_LETTER, "ABCDEFGH") ' SEMI checksum
            MIL.MocrSetConstraint(OcrFont, 11, MIL.M_DIGIT, "01234567") ' SEMI checksum

            ' Pause before the read operation.
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Allocate an OCR result buffer.
            MIL.MocrAllocResult(MilSystem, MIL.M_DEFAULT, OcrResult)

            ' Read the string.
            MIL.MocrReadString(MilSubImage, OcrFont, OcrResult)

            ' Get the string and its reading score.
            MIL.MocrGetResult(OcrResult, MIL.M_STRING, ReadString)
            MIL.MocrGetResult(OcrResult, MIL.M_STRING_SCORE, Score)

            ' Print the result.
            Console.Write(Constants.vbLf + "The string read is: ""{0}"" (score: {1:P1})." + Constants.vbLf + Constants.vbLf, ReadString.ToString(), Score / 100)

            ' Draw the string in the overlay under the reading region.
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY, MIL.M_ENABLE)
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY_CLEAR, MIL.M_DEFAULT)
            MIL.MdispInquire(MilDisplay, MIL.M_OVERLAY_ID, MilOverlayImage)
            MIL.MgraFont(MIL.M_DEFAULT, MIL.M_FONT_DEFAULT_LARGE)
            MIL.MgraColor(MIL.M_DEFAULT, MIL.M_COLOR_YELLOW)
            MIL.MgraText(MIL.M_DEFAULT, MilOverlayImage, READ_REGION_POS_X + (READ_REGION_WIDTH / 4), READ_REGION_POS_Y + READ_REGION_HEIGHT + 50, ReadString.ToString())

            ' Save the calibrated font if the reading score was sufficiently high.
            If Score > READ_SCORE_MIN Then
                MIL.MocrSaveFont(FONT_FILE_OUT, MIL.M_SAVE, OcrFont)
                Console.Write("Read successful, calibrated OCR font was saved to disk." + Constants.vbLf)
            Else
                Console.Write("Error: Read score too low, calibrated OCR font not saved." + Constants.vbLf)
            End If
            Console.Write("Press <Enter> to end." + Constants.vbLf + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Clear the overlay.
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY_CLEAR, MIL.M_DEFAULT)

            ' Free all allocations.
            MIL.MocrFree(OcrFont)
            MIL.MocrFree(OcrResult)
            MIL.MbufFree(MilSubImage)
            MIL.MbufFree(MilFontSubImage)
            MIL.MbufFree(MilImage)
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL)
        End Sub
    End Class
End Namespace
