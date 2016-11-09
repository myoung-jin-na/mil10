'*****************************************************************************
'
' File name: Form1.vb
'
' Synopsis:  This program displays a welcoming message in a user-defined
'            window and grabs (if supported) in it. It uses 
'            the MIL system and the MdispSelectWindow() function 
'            to display the MIL buffer in the client area of a windows form.
'*****************************************************************************
Imports Microsoft.VisualBasic
Imports System
Imports System.Collections.Generic
Imports System.ComponentModel
Imports System.Data
Imports System.Diagnostics
Imports System.Drawing
Imports System.Text
Imports System.Windows.Forms

Imports Matrox.MatroxImagingLibrary

Namespace MDispWindow
    Partial Public Class Form1
        Inherits Form
        Public Sub New()
            InitializeComponent()

            ' Set the window title.
            Me.Text = MIL_APPLICATION_NAME + " - " + Process.GetCurrentProcess().MainModule.FileName
        End Sub

        ' Window title.
        Private Const MIL_APPLICATION_NAME As String = "MIL Application"

        ' Default image dimensions.
        Private Const DEFAULT_IMAGE_SIZE_X As Integer = 640
        Private Const DEFAULT_IMAGE_SIZE_Y As Integer = 480
        Private Const DEFAULT_IMAGE_SIZE_BAND As Integer = 1

        ' Background color of the window client area.
        Private Const BACKCOLORRED As Integer = 160
        Private Const BACKCOLORGREEN As Integer = 160
        Private Const BACKCOLORBLUE As Integer = 164

        '**************************************************************************
        '
        ' Name:         MilApplication()
        '
        ' Synopsis:     This function is the core of the MIL application that
        '               is executed when the "Start" menu item of this
        '               Windows forms program is selected. See main() in the 
        '               program.cs file for the program's entry point.
        '
        '               It uses MIL to display a welcoming message in the 
        '               specified user window and to grab in it (if it is supported) 
        '               using the target system.
        '
        '**************************************************************************
        Private Sub MilApplication(ByVal UserWindowHandle As IntPtr)
            ' MIL variables 
            Dim MilApplication As MIL_ID = MIL.M_NULL ' MIL Application identifier.
            Dim MilSystem As MIL_ID = MIL.M_NULL ' MIL System identifier.
            Dim MilDisplay As MIL_ID = MIL.M_NULL ' MIL Display identifier.
            Dim MilDigitizer As MIL_ID = MIL.M_NULL ' MIL Digitizer identifier.
            Dim MilImage As MIL_ID = MIL.M_NULL ' MIL Image buffer identifier.
            Dim BufSizeX As MIL_INT = DEFAULT_IMAGE_SIZE_X
            Dim BufSizeY As MIL_INT = DEFAULT_IMAGE_SIZE_Y
            Dim BufSizeBand As MIL_INT = DEFAULT_IMAGE_SIZE_BAND

            ' Allocate a MIL application.
            MIL.MappAlloc(MIL.M_NULL, MIL.M_DEFAULT, MilApplication)

            ' Allocate a MIL system.
            MIL.MsysAlloc(MIL.M_DEFAULT, "M_DEFAULT", MIL.M_DEFAULT, MIL.M_DEFAULT, MilSystem)

            ' Allocate a MIL display.
            MIL.MdispAlloc(MilSystem, MIL.M_DEFAULT, "M_DEFAULT", MIL.M_WINDOWED, MilDisplay)

            ' Allocate a MIL digitizer, if supported, and set the target image size.
            If MIL.MsysInquire(MilSystem, MIL.M_DIGITIZER_NUM, MIL.M_NULL) > 0 Then
                MIL.MdigAlloc(MilSystem, MIL.M_DEFAULT, "M_DEFAULT", MIL.M_DEFAULT, MilDigitizer)
                MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_X, BufSizeX)
                MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_Y, BufSizeY)
                MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_BAND, BufSizeBand)

                ' Resize the display window
                If ((BufSizeX > DEFAULT_IMAGE_SIZE_X) Or (BufSizeY > DEFAULT_IMAGE_SIZE_Y)) Then
                    FromHandle(UserWindowHandle).Size = New Size(CType(BufSizeX, Integer), CType(BufSizeY, Integer))
                End If
            End If

            ' Allocate a MIL buffer.
            Dim Attributes As Long = MIL.M_IMAGE + MIL.M_DISP
            If MilDigitizer <> MIL.M_NULL Then
                ' Add M_GRAB attribute if a digitizer is allocated.
                Attributes = Attributes Or MIL.M_GRAB
            End If
            MIL.MbufAllocColor(MilSystem, BufSizeBand, BufSizeX, BufSizeY, 8 + MIL.M_UNSIGNED, Attributes, MilImage)

            ' Clear the buffer.
            MIL.MbufClear(MilImage, 0)

            ' Select the MIL buffer to be displayed in the user-specified window.
            MIL.MdispSelectWindow(MilDisplay, MilImage, UserWindowHandle)

            ' Print a string in the image buffer using MIL.
            ' Note: When a MIL buffer is modified using a MIL command, the display 
            ' automatically updates the window passed to MIL.MdispSelectWindow().

            MIL.MgraFont(MIL.M_DEFAULT, MIL.M_FONT_DEFAULT_LARGE)
            MIL.MgraText(MIL.M_DEFAULT, MilImage, (BufSizeX / 8) * 2, BufSizeY / 2, " Welcome to MIL !!! ")
            MIL.MgraRect(MIL.M_DEFAULT, MilImage, ((BufSizeX / 8) * 2) - 60, (BufSizeY / 2) - 80, ((BufSizeX / 8) * 2) + 370, (BufSizeY / 2) + 100)
            MIL.MgraRect(MIL.M_DEFAULT, MilImage, ((BufSizeX / 8) * 2) - 40, (BufSizeY / 2) - 60, ((BufSizeX / 8) * 2) + 350, (BufSizeY / 2) + 80)
            MIL.MgraRect(MIL.M_DEFAULT, MilImage, ((BufSizeX / 8) * 2) - 20, (BufSizeY / 2) - 40, ((BufSizeX / 8) * 2) + 330, (BufSizeY / 2) + 60)

            ' Open a message box to wait for a key press.
            MessageBox.Show("""Welcome to MIL !!!"" was printed", "MIL application example", MessageBoxButtons.OK, MessageBoxIcon.Information)

            ' Grab in the user window if supported.
            If MilDigitizer <> MIL.M_NULL Then
                ' Grab continuously.
                MIL.MdigGrabContinuous(MilDigitizer, MilImage)

                ' Open a message box to wait for a key press.
                MessageBox.Show("Continuous grab in progress", "MIL application example", MessageBoxButtons.OK, MessageBoxIcon.Information)

                ' Stop continuous grab.
                MIL.MdigHalt(MilDigitizer)
            End If

            ' Remove the MIL buffer from the display.
            MIL.MdispSelect(MilDisplay, MIL.M_NULL)

            ' Free allocated objects.
            MIL.MbufFree(MilImage)

            If MilDigitizer <> MIL.M_NULL Then
                MIL.MdigFree(MilDigitizer)
            End If

            MIL.MdispFree(MilDisplay)
            MIL.MsysFree(MilSystem)
            MIL.MappFree(MilApplication)
        End Sub

        '**************************************************************************
        '
        ' Name:         startMenuItem_Click()
        '
        ' Synopsis:    This function hadles the main menu commands and start the 
        '              MIL application.
        '
        '**************************************************************************
        Private Sub startMenuItem_Click(ByVal sender As Object, ByVal e As EventArgs) Handles startMenuItem.Click
            MilApplication(Me.Handle)
        End Sub
    End Class
End Namespace