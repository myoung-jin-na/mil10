'*****************************************************************************
'
' File name: MimDeinterlace.vb
'
' Synopsis:  : This program performs deinterlacing on an image. 
'
'              First, a static image is deinterlaced using the Discard
'              algorithm. Then, if a digitizer is present, the grabbed 
'              images are deinterlaced live using the same algorithms.
'*****************************************************************************
Imports Microsoft.VisualBasic
Imports System
Imports System.Collections.Generic
Imports System.Text

Imports Matrox.MatroxImagingLibrary

Namespace MImDeinterlace
    Friend Class Program
        Private Const IMAGE_FILE As String = MIL.M_IMAGE_PATH & "/Preprocessing/InterlacedBird.mim"

        Private Const MOTION_DETECT_NUM_FRAMES As Integer = 3
        Private Const MOTION_DETECT_THRESHOLD As Integer = 20

        Private Const NUM_GRAB_IMAGES As Integer = (MOTION_DETECT_NUM_FRAMES + 1)

        Private Shared GRAB_DEINTERLACE_METHOD As Integer = MIL.M_DISCARD


        ' Main function.
        ' --------------
        Shared Sub Main(ByVal args() As String)
            Dim MilApplication As MIL_ID = MIL.M_NULL ' Application identifier. 
            Dim MilSystem As MIL_ID = MIL.M_NULL ' System identifier. 
            Dim MilDisplay As MIL_ID = MIL.M_NULL ' Display identifier. 
            Dim MilDisplayImage As MIL_ID = MIL.M_NULL ' Image buffer identifier. 
            Dim MilSourceImage As MIL_ID = MIL.M_NULL ' Image buffer identifier. 
            Dim MilDeinterlaceContext As MIL_ID = MIL.M_NULL ' Deinterlace context identifier. 
            Dim ImageSizeBand As MIL_INT = 0
            Dim ImageWidth As MIL_INT = 0
            Dim ImageHeight As MIL_INT = 0
            Dim ImageType As MIL_INT = 0

            ' Allocate defaults. 
            MIL.MappAllocDefault(MIL.M_DEFAULT, MilApplication, MilSystem, MilDisplay, CType(MIL.M_NULL, IntPtr), CType(MIL.M_NULL, IntPtr))

            ' Restore the source image. 
            MIL.MbufRestore(IMAGE_FILE, MilSystem, MilSourceImage)

            ' Allocate a display buffers and show the source image. 
            MIL.MbufInquire(MilSourceImage, MIL.M_SIZE_BAND, ImageSizeBand)
            MIL.MbufInquire(MilSourceImage, MIL.M_SIZE_X, ImageWidth)
            MIL.MbufInquire(MilSourceImage, MIL.M_SIZE_Y, ImageHeight)
            MIL.MbufInquire(MilSourceImage, MIL.M_TYPE, ImageType)
            MIL.MbufAllocColor(MilSystem, ImageSizeBand, ImageWidth, ImageHeight, ImageType, MIL.M_IMAGE + MIL.M_PROC + MIL.M_DISP, MilDisplayImage)
            MIL.MbufCopy(MilSourceImage, MilDisplayImage)
            MIL.MdispSelect(MilDisplay, MilDisplayImage)

            ' Allocate a deinterlacing context. 
            MIL.MimAlloc(MilSystem, MIL.M_DEINTERLACE_CONTEXT, MIL.M_DEFAULT, MilDeinterlaceContext)

            ' Print a message. 
            Console.Write(Constants.vbLf + "DEINTERLACING:" + Constants.vbLf)
            Console.Write("----------------" + Constants.vbLf + Constants.vbLf)
            Console.Write("This image has been grabbed using an interlaced camera." + Constants.vbLf)
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Deinterlace using MIL.M_DISCARD. 
            MIL.MimControl(MilDeinterlaceContext, MIL.M_DEINTERLACE_TYPE, MIL.M_DISCARD)
            MIL.MimDeinterlace(MilDeinterlaceContext, MilSourceImage, MilDisplayImage, 1, 1, MIL.M_DEFAULT)

            ' Display message. 
            Console.Write("The image has been deinterlaced using the DISCARD algorithm." + Constants.vbLf)
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Free objects. 
            MIL.MbufFree(MilSourceImage)
            MIL.MbufFree(MilDisplayImage)
            MIL.MdispFree(MilDisplay)
            MIL.MimFree(MilDeinterlaceContext)

            ' If the system has a digitizer, use it to do live deinterlacing. 
            If MIL.MsysInquire(MilSystem, MIL.M_DIGITIZER_NUM, MIL.M_NULL) > 0 Then
                GrabDeinterlace(MilSystem)
            End If

            ' Free objects. 
            MIL.MappFreeDefault(MilApplication, MilSystem, MIL.M_NULL, MIL.M_NULL, MIL.M_NULL)

        End Sub

        Private Shared Sub GrabDeinterlace(ByVal MilSystem As MIL_ID)
            Dim MilDigitizer As MIL_ID = MIL.M_NULL ' Digitizer identifier. 
            Dim MilDisplay As MIL_ID = MIL.M_NULL ' Display identifier. 
            Dim MilDisplayImage As MIL_ID = MIL.M_NULL ' Image buffer identifier. 
            Dim MilDeinterlaceContext As MIL_ID = MIL.M_NULL ' Deinterlace context identifier. 
            Dim MilGrabImages(NUM_GRAB_IMAGES - 1) As MIL_ID
            Dim MilPreviousImages(MOTION_DETECT_NUM_FRAMES - 1) As MIL_ID
            Dim ImageSizeBand As MIL_INT = 0
            Dim ImageWidth As MIL_INT = 0
            Dim ImageHeight As MIL_INT = 0
            Dim i As Integer = 0
            Dim CurrentGrabIndex As Integer = 0
            Dim NextGrabIndex As Integer = 0

            ' Try to allocate a digitizer. 
            MIL.MdigAlloc(MilSystem, MIL.M_DEFAULT, "M_DEFAULT", MIL.M_DEFAULT, MilDigitizer)
            If MilDigitizer = MIL.M_NULL Then
                Return
            End If

            ' Inquire characteristics of the digitizer. 
            MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_BAND, ImageSizeBand)
            MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_X, ImageWidth)
            MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_Y, ImageHeight)

            ' Allocate grab images. 
            For i = 0 To NUM_GRAB_IMAGES - 1
                MIL.MbufAllocColor(MilSystem, ImageSizeBand, ImageWidth, ImageHeight, 8 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_GRAB + MIL.M_PROC, MilGrabImages(i))
            Next i

            ' Allocate a deinterlacing context. 
            MIL.MimAlloc(MilSystem, MIL.M_DEINTERLACE_CONTEXT, MIL.M_DEFAULT, MilDeinterlaceContext)

            ' Allocate new display and display image. 
            MIL.MbufAllocColor(MilSystem, ImageSizeBand, ImageWidth, ImageHeight, 8 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_GRAB + MIL.M_DISP + MIL.M_PROC, MilDisplayImage)
            MIL.MdispAlloc(MilSystem, MIL.M_DEFAULT, "M_DEFAULT", MIL.M_DEFAULT, MilDisplay)
            MIL.MbufClear(MilDisplayImage, 0)
            MIL.MdispSelect(MilDisplay, MilDisplayImage)

            ' Print a message. 
            Console.Write("Normal live grab is displayed." + Constants.vbLf)
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)

            ' Grab continuously on display and wait for a key press. 
            MIL.MdigGrabContinuous(MilDigitizer, MilDisplayImage)
            Console.ReadKey()

            ' Halt continuous grab. 
            MIL.MdigHalt(MilDigitizer)

            ' Configure the Deinterlace context for the appropriate live deinterlacing method. 
            MIL.MimControl(MilDeinterlaceContext, MIL.M_DEINTERLACE_TYPE, GRAB_DEINTERLACE_METHOD)
            If GRAB_DEINTERLACE_METHOD = MIL.M_ADAPTIVE_DISCARD OrElse GRAB_DEINTERLACE_METHOD = MIL.M_ADAPTIVE_AVERAGE Then
                MIL.MimControl(MilDeinterlaceContext, MIL.M_MOTION_DETECT_NUM_FRAMES, MOTION_DETECT_NUM_FRAMES)
                MIL.MimControl(MilDeinterlaceContext, MIL.M_MOTION_DETECT_THRESHOLD, MOTION_DETECT_THRESHOLD)
            End If

            ' Set the digitizer to asynchronous grab. 
            MIL.MdigControl(MilDigitizer, MIL.M_GRAB_MODE, MIL.M_ASYNCHRONOUS)

            ' Send the first grab to the digitizer. 
            MIL.MdigGrab(MilDigitizer, MilGrabImages(0))
            CurrentGrabIndex = 0
            NextGrabIndex = 1

            ' Print a message. 
            Console.Write("Deinterlaced live grab is displayed." + Constants.vbLf)
            Console.Write("Press <Enter> to stop." + Constants.vbLf + Constants.vbLf)

            ' Grab and process. 
            Do While Not Console.KeyAvailable
                ' Send the next grab command to the digitizer. This will block until the current grab is finished. 
                MIL.MdigGrab(MilDigitizer, MilGrabImages(NextGrabIndex))

                If (GRAB_DEINTERLACE_METHOD <> MIL.M_ADAPTIVE_DISCARD) AndAlso (GRAB_DEINTERLACE_METHOD <> MIL.M_ADAPTIVE_AVERAGE) Then
                    ' Deinterlace the current grabbed image. 
                    MIL.MimDeinterlace(MilDeinterlaceContext, MilGrabImages(CurrentGrabIndex), MilDisplayImage, 1, 1, MIL.M_DEFAULT)
                Else
                    ' Build the table of the last MOTION_DETECT_NUM_FRAMES grabbed frames. 
                    For i = 0 To MOTION_DETECT_NUM_FRAMES - 1
                        Dim PreviousGrabIndex As Integer = NextGrabIndex - MOTION_DETECT_NUM_FRAMES + i
                        If PreviousGrabIndex < 0 Then
                            PreviousGrabIndex += NUM_GRAB_IMAGES
                        End If
                        MilPreviousImages(i) = MilGrabImages(PreviousGrabIndex)
                    Next i

                    ' Deinterlace the previous grabbed images. 
                    MIL.MimDeinterlace(MilDeinterlaceContext, MilPreviousImages, MilDisplayImage, MOTION_DETECT_NUM_FRAMES, 1, MIL.M_DEFAULT)
                End If

                ' Increment grab index. 
                CurrentGrabIndex = NextGrabIndex
                NextGrabIndex = (NextGrabIndex + 1) Mod NUM_GRAB_IMAGES
            Loop
            Console.ReadKey()

            ' Display the last grabbed image. 
            Console.Write("Last deinterlaced image displayed." + Constants.vbLf)
            Console.Write("Press <Enter> to end." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Free objects. 
            MIL.MimFree(MilDeinterlaceContext)
            MIL.MbufFree(MilDisplayImage)
            MIL.MdispFree(MilDisplay)
            For i = 0 To NUM_GRAB_IMAGES - 1
                MIL.MbufFree(MilGrabImages(i))
            Next i
            MIL.MdigFree(MilDigitizer)
        End Sub
    End Class
End Namespace
