'*************************************************************************************
'
' File name: MdispOverlay.vb
'
' Synopsis:  This program shows how to display an image while creating
'            text and graphics annotations on top of it using MIL
'            graphic functions and windows GDI drawing under Windows.
'            If the target system supports grabbing, the annotations
'            are done on top of a continuous grab.
'*************************************************************************************
Imports Microsoft.VisualBasic
Imports System
Imports System.Collections.Generic
Imports System.Text
Imports System.Drawing
Imports System.Runtime.InteropServices

Imports Matrox.MatroxImagingLibrary

Namespace MDispOverlay
    Friend Class Program
        ' Target image.
        Private Const IMAGE_FILE As String = MIL.M_IMAGE_PATH & "Board.mim"

        ' Title for the display window.
        Private Const WINDOW_TITLE As String = "Custom Title"

        Shared Sub Main(ByVal args() As String)
            Dim MilApplication As MIL_ID = MIL.M_NULL ' Application identifier.
            Dim MilSystem As MIL_ID = MIL.M_NULL ' System identifier.
            Dim MilDisplay As MIL_ID = MIL.M_NULL ' Display identifier.
            Dim MilDigitizer As MIL_ID = MIL.M_NULL ' Digitizer identifier.
            Dim MilImage As MIL_ID = MIL.M_NULL ' Image identifier.

            ' Allocate defaults
            MIL.MappAllocDefault(MIL.M_DEFAULT, MilApplication, MilSystem, MilDisplay, CType(MIL.M_NULL, IntPtr), CType(MIL.M_NULL, IntPtr))

            ' If the system have a digitizer, use it.
            If MIL.MsysInquire(MilSystem, MIL.M_DIGITIZER_NUM, MIL.M_NULL) > 0 Then
                MIL.MdigAlloc(MilSystem, MIL.M_DEFAULT, "M_DEFAULT", MIL.M_DEFAULT, MilDigitizer)
                MIL.MbufAllocColor(MilSystem, MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_BAND, MIL.M_NULL), MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_X, MIL.M_NULL), MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_Y, MIL.M_NULL), 8 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_DISP + MIL.M_PROC + MIL.M_GRAB, MilImage)
                MIL.MbufClear(MilImage, 0)
            Else
                ' Restore a static image.
                MIL.MbufRestore(IMAGE_FILE, MilSystem, MilImage)
            End If

            ' Change display window title.
            MIL.MdispControl(MilDisplay, MIL.M_TITLE, WINDOW_TITLE)

            ' Display the image buffer.
            MIL.MdispSelect(MilDisplay, MilImage)

            ' Draw text and graphics annotations in the display overlay.
            OverlayDraw(MilDisplay)

            ' If the system supports it, grab continuously in the displayed image.
            If MilDigitizer <> MIL.M_NULL Then
                MIL.MdigGrabContinuous(MilDigitizer, MilImage)
            End If

            ' Pause to show the image.
            Console.Write(Constants.vbLf + "OVERLAY ANNOTATIONS:" + Constants.vbLf)
            Console.Write("--------------------" + Constants.vbLf + Constants.vbLf)
            Console.Write("Displaying an image with overlay annotations." + Constants.vbLf)
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Stop the continuous grab and free digitizer if needed.
            If MilDigitizer <> MIL.M_NULL Then
                MIL.MdigHalt(MilDigitizer)
                MIL.MdigFree(MilDigitizer)

                ' Pause to show the result.
                Console.Write("Displaying the last grabbed image." + Constants.vbLf)
                Console.Write("Press <Enter> to end." + Constants.vbLf + Constants.vbLf)
                Console.ReadKey()
            End If

            ' Free image.
            MIL.MbufFree(MilImage)

            ' Free default allocations.
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL)
        End Sub

        ' ---------------------------------------------------------------
        ' Name:      OverlayDraw
        ' Synopsis:  This function draws annotations in the display overlay.

        Private Shared Sub OverlayDraw(ByVal MilDisplay As MIL_ID)
            Dim DefaultGraphicContext As MIL_ID = MIL.M_DEFAULT
            Dim MilOverlayImage As MIL_ID = MIL.M_NULL
            Dim ImageWidth, ImageHeight As MIL_INT
            Dim hCustomDC As IntPtr = IntPtr.Zero

            ' Prepare overlay buffer.
            '***************************

            ' Enable the display of overlay annotations.
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY, MIL.M_ENABLE)

            ' Inquire the overlay buffer associated with the display.
            MIL.MdispInquire(MilDisplay, MIL.M_OVERLAY_ID, MilOverlayImage)

            ' Clear the overlay to transparent.
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY_CLEAR, MIL.M_DEFAULT)

            ' Disable the overlay display update to accelerate annotations.
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY_SHOW, MIL.M_DISABLE)

            ' Inquire overlay size.
            ImageWidth = MIL.MbufInquire(MilOverlayImage, MIL.M_SIZE_X, MIL.M_NULL)
            ImageHeight = MIL.MbufInquire(MilOverlayImage, MIL.M_SIZE_Y, MIL.M_NULL)

            ' Draw MIL overlay annotations.
            '*********************************

            ' Set the graphic text background to transparent.
            MIL.MgraControl(DefaultGraphicContext, MIL.M_BACKGROUND_MODE, MIL.M_TRANSPARENT)

            ' Print a white string in the overlay image buffer.
            MIL.MgraColor(DefaultGraphicContext, MIL.M_COLOR_WHITE)
            MIL.MgraText(DefaultGraphicContext, MilOverlayImage, ImageWidth / 9, ImageHeight / 5, " -------------------- ")
            MIL.MgraText(DefaultGraphicContext, MilOverlayImage, ImageWidth / 9, ImageHeight / 5 + 25, " - MIL Overlay Text - ")
            MIL.MgraText(DefaultGraphicContext, MilOverlayImage, ImageWidth / 9, ImageHeight / 5 + 50, " -------------------- ")

            ' Print a green string in the overlay image buffer.
            MIL.MgraColor(DefaultGraphicContext, MIL.M_COLOR_GREEN)
            MIL.MgraText(DefaultGraphicContext, MilOverlayImage, ImageWidth * 11 / 18, ImageHeight / 5, " ---------------------")
            MIL.MgraText(DefaultGraphicContext, MilOverlayImage, ImageWidth * 11 / 18, ImageHeight / 5 + 25, " - MIL Overlay Text - ")
            MIL.MgraText(DefaultGraphicContext, MilOverlayImage, ImageWidth * 11 / 18, ImageHeight / 5 + 50, " ---------------------")

            ' Re-enable the overlay display after all annotations are done.
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY_SHOW, MIL.M_ENABLE)

            ' Draw GDI color overlay annotation.
            '***********************************

            ' The inquire might not be supported
            MIL.MappControl(MIL.M_ERROR, MIL.M_PRINT_DISABLE)

            ' Create a device context to draw in the overlay buffer with GDI.
            MIL.MbufControl(MilOverlayImage, MIL.M_DC_ALLOC, MIL.M_DEFAULT)

            ' Inquire the device context.
            hCustomDC = CType(MIL.MbufInquire(MilOverlayImage, MIL.M_DC_HANDLE, MIL.M_NULL), IntPtr)
            MIL.MappControl(MIL.M_ERROR, MIL.M_PRINT_ENABLE)

            ' Perform operation if GDI drawing is supported.
            If (Not hCustomDC.Equals(IntPtr.Zero)) Then

                ' NOTE : The Using/ End Using blocks will automatically call the Dipose method on the GDI objects.
                '        This ensures that resources are freed even if an exception occurs.

                ' Create a System.Drawing.Graphics object from the Device context
                Using DrawingGraphics As Graphics = Graphics.FromHdc(hCustomDC)

                    ' Draw a blue cross.
                    Using DrawingPen As New Pen(Color.Blue)

                        ' Draw a blue cross in the overlay image
                        DrawingGraphics.DrawLine(DrawingPen, 0, CInt(Fix(ImageHeight / 2)), ImageWidth, CInt(Fix(ImageHeight / 2)))
                        DrawingGraphics.DrawLine(DrawingPen, CInt(Fix(ImageWidth / 2)), 0, CInt(Fix(ImageWidth / 2)), ImageHeight)

                        ' Prepare transparent text annotations.
                        ' Define the Brushes and fonts used to draw text
                        Using LeftBrush As New SolidBrush(Color.Red)

                            Using RightBrush As New SolidBrush(Color.Yellow)

                                Using OverlayFont As New Font(FontFamily.GenericSansSerif, 10, FontStyle.Bold)

                                    ' Write text in the overlay image
                                    Dim GDITextSize As SizeF = DrawingGraphics.MeasureString("GDI Overlay Text", OverlayFont)
                                    DrawingGraphics.DrawString("GDI Overlay Text", OverlayFont, LeftBrush, System.Convert.ToInt32(CSng(ImageWidth) / 4 - GDITextSize.Width / 2), System.Convert.ToInt32(CSng(ImageHeight) * 3 / 4 - GDITextSize.Height / 2))
                                    DrawingGraphics.DrawString("GDI Overlay Text", OverlayFont, RightBrush, System.Convert.ToInt32(CSng(ImageWidth) * 3 / 4 - GDITextSize.Width / 2), System.Convert.ToInt32(CSng(ImageHeight) * 3 / 4 - GDITextSize.Height / 2))

                                End Using

                            End Using

                        End Using

                    End Using

                End Using

                '   // Delete device context.
                MIL.MbufControl(MilOverlayImage, MIL.M_DC_FREE, MIL.M_DEFAULT)

                '   // Signal MIL that the overlay buffer was modified.
                MIL.MbufControl(MilOverlayImage, MIL.M_MODIFIED, MIL.M_DEFAULT)
            End If
        End Sub
    End Class
End Namespace
