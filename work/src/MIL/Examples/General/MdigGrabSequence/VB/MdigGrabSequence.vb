'*******************************************************************************
'
' File name: MdigGrabSequence.vb
'
' Synopsis:  This example shows how to grab a sequence, archive it, and play 
'            it back in real time from an AVI file.
'
' NOTE:      This example assumes that the hard disk is sufficiently fast 
'            to keep up with the grab. Also, removing the sequence display or 
'            the text annotation while grabbing will reduce the CPU usage and
'            might help if some frames are missed during acquisition. 
'            If the disk or system are not fast enough, set GRAB_SCALE to 0.5, 
'            FRAME_NUMBER_ANNOTATION to False or SAVE_SEQUENCE_TO_DISK to False.
'*******************************************************************************
Imports Microsoft.VisualBasic
Imports System
Imports System.Collections.Generic
Imports System.Text
Imports System.Runtime.InteropServices

Imports Matrox.MatroxImagingLibrary

Namespace MDigGrabSequence
    Friend Class Program
        ' Sequence file name
        Private Const SEQUENCE_FILE As String = MIL.M_TEMP_DIR & "MilSequence.avi"

        ' Image acquisition scale.
        Private Const GRAB_SCALE As Double = 1.0

        ' Quantization factor to use during the compression.
        ' Valid values are 1 to 99 (higher to lower quality).
        Private Const COMPRESSION_Q_FACTOR As Integer = 50

        ' Annotation flag. Set to False to draw the frame number in the saved image.
        Private Shared ReadOnly FRAME_NUMBER_ANNOTATION As MIL_INT = MIL.M_YES

        ' Archive flag. Set to False to disable AVI Import/Export to disk.
        Private Shared ReadOnly SaveSequenceToDisk As Boolean = True

        ' Maximum number of images for the multiple buffering grab.
        Private Const NB_GRAB_IMAGE_MAX As Integer = 22

        Public Class HookDataObject ' User's archive function hook data structure.
            Public MilSystem As MIL_ID
            Public MilDisplay As MIL_ID
            Public MilImageDisp As MIL_ID
            Public MilCompressedImage As MIL_ID
            Public NbGrabbedFrames As Integer
            Public NbArchivedFrames As Integer
            Public SaveSequenceToDisk As Boolean
        End Class

        ' Main function.
        ' --------------
        Shared Sub Main(ByVal args() As String)
            Dim MilApplication As MIL_ID = MIL.M_NULL
            Dim MilRemoteApplication As MIL_ID = MIL.M_NULL
            Dim MilSystem As MIL_ID = MIL.M_NULL
            Dim MilDigitizer As MIL_ID = MIL.M_NULL
            Dim MilDisplay As MIL_ID = MIL.M_NULL
            Dim MilImageDisp As MIL_ID = MIL.M_NULL
            Dim MilGrabImages(NB_GRAB_IMAGE_MAX) As MIL_ID
            Dim MilCompressedImage As MIL_ID = MIL.M_NULL
            Dim Selection As New ConsoleKeyInfo("1"c, ConsoleKey.D1, False, False, False)

            Dim NbFrames As Integer = 0
            Dim n As Integer = 0
            Dim NbFramesReplayed As Integer = 0
            Dim FrameRate As Double = 0
            Dim TimeWait As Double = 0
            Dim TotalReplay As Double = 0
            Dim GrabScale As Double = GRAB_SCALE
            Dim UserHookData As New HookDataObject()
            Dim LicenseModules As MIL_INT = 0
            Dim FrameCount As MIL_INT = 0
            Dim FrameMissed As MIL_INT = 0
            Dim CompressAttribute As MIL_INT = 0

            ' Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, MilApplication, MilSystem, MilDisplay, MilDigitizer, CType(MIL.M_NULL, IntPtr))

            ' Allocate an image and display it.
            MIL.MbufAllocColor(MilSystem, MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_BAND, MIL.M_NULL), CInt(CDbl(MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_X, CType(MIL.M_NULL, IntPtr))) * GrabScale), CInt(CDbl(MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_Y, CType(MIL.M_NULL, IntPtr))) * GrabScale), 8 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_GRAB + MIL.M_DISP, MilImageDisp)

            MIL.MbufClear(MilImageDisp, &H0)
            MIL.MdispSelect(MilDisplay, MilImageDisp)

            ' Grab continuously on display at the specified scale.
            MIL.MdigControl(MilDigitizer, MIL.M_GRAB_SCALE, GrabScale)
            MIL.MdigGrabContinuous(MilDigitizer, MilImageDisp)

            ' Print a message
            Console.WriteLine()
            Console.WriteLine("SEQUENCE ACQUISITION:")
            Console.WriteLine("--------------------")
            Console.WriteLine()

            ' Inquire MIL licenses.
            MIL.MsysInquire(MilSystem, MIL.M_OWNER_APPLICATION, MilRemoteApplication)
            MIL.MappInquire(MilRemoteApplication, MIL.M_LICENSE_MODULES, LicenseModules)

            ' If sequence is saved to disk, select between grabbing an 
            ' uncompressed, JPEG or JPEG2000 sequence. 
            If SaveSequenceToDisk AndAlso ((LicenseModules And (MIL.M_LICENSE_JPEGSTD Or MIL.M_LICENSE_JPEG2000)) <> 0) Then
                Console.WriteLine("Choose the sequence format:")
                Console.WriteLine("1) Uncompressed images.")
                If (LicenseModules And MIL.M_LICENSE_JPEGSTD) <> 0 Then
                    Console.WriteLine("2) Compressed images with a lossy JPEG algorithm.")
                End If
                If (LicenseModules And MIL.M_LICENSE_JPEG2000) <> 0 Then
                    Console.WriteLine("3) Compressed images with a lossy JPEG 2000 algorithm.")
                End If
                Selection = Console.ReadKey()
            Else
                Console.WriteLine("Press <Enter> to record images.")
                Console.ReadKey()
            End If


            ' Set the buffer attribute.
            Select Case Selection.Key
                Case ConsoleKey.NumPad1, ConsoleKey.D1, ConsoleKey.Enter
                    Console.WriteLine()
                    Console.WriteLine("Recording uncompressed images...")
                    Console.WriteLine()
                    CompressAttribute = 0

                Case ConsoleKey.NumPad2, ConsoleKey.D2
                    Console.WriteLine()
                    Console.WriteLine("Recording JPEG images...")
                    Console.WriteLine()
                    CompressAttribute = MIL.M_COMPRESS + MIL.M_JPEG_LOSSY

                Case ConsoleKey.NumPad3, ConsoleKey.D3
                    Console.WriteLine()
                    Console.WriteLine("Recording JPEG 2000 images...")
                    Console.WriteLine()
                    CompressAttribute = MIL.M_COMPRESS + MIL.M_JPEG2000_LOSSY

                Case Else
                    Console.WriteLine()
                    Console.WriteLine("Invalid selection !.")
                    Console.WriteLine()
                    Console.WriteLine("Using uncompressed images.")
                    Console.WriteLine()
                    CompressAttribute = 0
                    Do While Console.KeyAvailable
                        Console.ReadKey()
                    Loop
            End Select

            ' Allocate a compressed buffer if required.
            If CompressAttribute <> MIL.M_NULL Then
                MIL.MbufAllocColor(MilSystem, MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_BAND, MIL.M_NULL), CInt(CDbl(MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_X, MIL.M_NULL)) * GrabScale), CInt(CDbl(MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_Y, MIL.M_NULL)) * GrabScale), 8 + MIL.M_UNSIGNED, MIL.M_IMAGE + CompressAttribute, MilCompressedImage)

                MIL.MbufControl(MilCompressedImage, MIL.M_Q_FACTOR, COMPRESSION_Q_FACTOR)
            End If

            ' Allocate the grab buffers to hold the sequence buffering.
            MIL.MappControl(MIL.M_ERROR, MIL.M_PRINT_DISABLE)
            NbFrames = 0
            n = 0
            Do While n < NB_GRAB_IMAGE_MAX
                MIL.MbufAllocColor(MilSystem, MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_BAND, MIL.M_NULL), CInt(CDbl(MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_X, MIL.M_NULL)) * GrabScale), CInt(CDbl(MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_Y, MIL.M_NULL)) * GrabScale), 8 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_GRAB, MilGrabImages(n))

                If MilGrabImages(n) <> MIL.M_NULL Then
                    NbFrames += 1
                    MIL.MbufClear(MilGrabImages(n), &HFF)
                Else
                    Exit Do
                End If
                n += 1
            Loop
            MIL.MappControl(MIL.M_ERROR, MIL.M_PRINT_ENABLE)

            '  Free buffers to leave some space for possible temporary buffers.
            n = 0
            While n < 2 And NbFrames > 0
                NbFrames -= 1
                MIL.MbufFree(MilGrabImages(NbFrames))
                n += 1
            End While

            ' Halt continuous grab.
            MIL.MdigHalt(MilDigitizer)

            ' Open the AVI file if required.
            If SaveSequenceToDisk Then
                Console.WriteLine("Saving the sequence to an AVI file...")
                Console.WriteLine()
                MIL.MbufExportSequence(SEQUENCE_FILE, MIL.M_DEFAULT, CType(MIL.M_NULL, IntPtr), MIL.M_NULL, MIL.M_DEFAULT, MIL.M_OPEN)
            End If

            ' Initialize User's archiving function hook data structure.
            UserHookData.MilSystem = MilSystem
            UserHookData.MilDisplay = MilDisplay
            UserHookData.MilImageDisp = MilImageDisp
            UserHookData.MilCompressedImage = MilCompressedImage
            UserHookData.SaveSequenceToDisk = SaveSequenceToDisk
            UserHookData.NbGrabbedFrames = 0
            UserHookData.NbArchivedFrames = 0

            ' get a handle to the DigHookUserData object in the managed heap, we will use this 
            ' handle to get the object back in the callback function
            Dim UserHookDataHandle As GCHandle = GCHandle.Alloc(UserHookData)
            Dim UserHookFunctionDelegate As New MIL_DIG_HOOK_FUNCTION_PTR(AddressOf ArchiveFunction)

            ' Acquire the sequence. The processing hook function will
            ' be called for each image grabbed to archive and display it. 
            ' If sequence is not saved to disk, stop after NbFrames.
            If SaveSequenceToDisk Then
                MIL.MdigProcess(MilDigitizer, MilGrabImages, NbFrames, MIL.M_START, MIL.M_DEFAULT, UserHookFunctionDelegate, CType(UserHookDataHandle, IntPtr))
            Else
                MIL.MdigProcess(MilDigitizer, MilGrabImages, NbFrames, MIL.M_SEQUENCE, MIL.M_DEFAULT, UserHookFunctionDelegate, CType(UserHookDataHandle, IntPtr))
            End If

            ' Wait for a key press.
            Console.WriteLine("Press <Enter> to continue.")
            Console.WriteLine()
            Console.ReadKey(True)

            ' Stop the sequence acquisition.
            MIL.MdigProcess(MilDigitizer, MilGrabImages, NbFrames, MIL.M_STOP, MIL.M_DEFAULT, UserHookFunctionDelegate, CType(UserHookDataHandle, IntPtr))

            ' Free the GCHandle when no longer used.
            UserHookDataHandle.Free()

            ' Read and print final statistics.
            MIL.MdigInquire(MilDigitizer, MIL.M_PROCESS_FRAME_COUNT, FrameCount)
            MIL.MdigInquire(MilDigitizer, MIL.M_PROCESS_FRAME_RATE, FrameRate)
            MIL.MdigInquire(MilDigitizer, MIL.M_PROCESS_FRAME_MISSED, FrameMissed)

            Console.WriteLine()
            Console.WriteLine()
            Console.WriteLine("{0} frames archived ({1} missed), at {2:0.0} frames/sec ({3:0.0}ms/frame).", UserHookData.NbArchivedFrames, FrameMissed, FrameRate, 1000.0 / FrameRate)

            ' Sequence file closing if required.
            If SaveSequenceToDisk Then
                MIL.MbufExportSequence(SEQUENCE_FILE, MIL.M_DEFAULT, CType(MIL.M_NULL, IntPtr), MIL.M_NULL, FrameRate, MIL.M_CLOSE)
            End If

            ' Playback the sequence until a key is pressed.
            If UserHookData.NbArchivedFrames > 0 Then
                Do
                    ' If sequence must be loaded.
                    If SaveSequenceToDisk Then
                        ' Inquire information about the sequence.
                        Console.WriteLine()
                        Console.WriteLine("Playing back sequence from the AVI file...")
                        Console.WriteLine()
                        Console.WriteLine("Press <Enter> to end.")
                        Console.WriteLine()
                        Console.WriteLine()
                        MIL.MbufDiskInquire(SEQUENCE_FILE, MIL.M_NUMBER_OF_IMAGES, FrameCount)
                        MIL.MbufDiskInquire(SEQUENCE_FILE, MIL.M_FRAME_RATE, FrameRate)
                        MIL.MbufDiskInquire(SEQUENCE_FILE, MIL.M_COMPRESSION_TYPE, CompressAttribute)

                        ' Open the sequence file.
                        MIL.MbufImportSequence(SEQUENCE_FILE, MIL.M_DEFAULT, MIL.M_NULL, MIL.M_NULL, CType(MIL.M_NULL, IntPtr), MIL.M_NULL, MIL.M_NULL, MIL.M_OPEN)
                    End If

                    ' Copy the images to the screen respecting the sequence frame rate.
                    TotalReplay = 0.0
                    NbFramesReplayed = 0
                    n = 0
                    Do While n < FrameCount
                        ' Reset the time.
                        MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_RESET, CType(MIL.M_NULL, IntPtr))

                        ' If image was saved to disk.
                        If SaveSequenceToDisk Then
                            ' Load image directly to the display.
                            MIL.MbufImportSequence(SEQUENCE_FILE, MIL.M_DEFAULT, MIL.M_LOAD, MIL.M_NULL, MilImageDisp, n, 1, MIL.M_READ)
                            NbFramesReplayed += 1
                            Console.Write("Frame #{0}             " & Constants.vbCr, NbFramesReplayed)
                        Else
                            ' Copy the grabbed image to the display.
                            MIL.MbufCopy(MilGrabImages(n), MilImageDisp)
                            NbFramesReplayed += 1
                            Console.Write("Frame #{0}             " & Constants.vbCr, NbFramesReplayed)

                        End If

                        ' Check for a pressed key to exit.
                        If Console.KeyAvailable AndAlso (n >= (NB_GRAB_IMAGE_MAX - 1)) Then
                            Console.ReadKey(True)
                            Exit Do
                        End If

                        ' Wait to have a proper frame rate.
                        MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ, TimeWait)
                        TotalReplay += TimeWait
                        TimeWait = (1 / FrameRate) - TimeWait
                        MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_WAIT, TimeWait)
                        If (TimeWait > 0) Then
                            TotalReplay += TimeWait
                        Else
                            TotalReplay += 0.0
                        End If
                        n += 1
                    Loop

                    ' Close the sequence file.
                    If SaveSequenceToDisk Then
                        MIL.MbufImportSequence(SEQUENCE_FILE, MIL.M_DEFAULT, MIL.M_NULL, MIL.M_NULL, CType(MIL.M_NULL, IntPtr), MIL.M_NULL, MIL.M_NULL, MIL.M_CLOSE)
                    End If

                    ' Print statistics.
                    Console.WriteLine()
                    Console.WriteLine()
                    Console.WriteLine("{0} frames replayed, at a frame rate of {1:0.0} frames/sec ({2:0.0} ms/frame).", NbFramesReplayed, n / TotalReplay, 1000.0 * TotalReplay / n)
                    Console.WriteLine()
                    Console.WriteLine("Press <Enter> to end (or any other key to playback again).")
                    Console.WriteLine()

                Loop While Console.ReadKey(True).Key <> ConsoleKey.Enter
            End If

            ' Free all allocated buffers.
            MIL.MbufFree(MilImageDisp)
            For n = 0 To NbFrames - 1
                MIL.MbufFree(MilGrabImages(n))
            Next n

            If MilCompressedImage <> MIL.M_NULL Then
                MIL.MbufFree(MilCompressedImage)
            End If

            ' Free defaults.
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MilDigitizer, MIL.M_NULL)
        End Sub

        ' User's archive function called each time a new buffer is grabbed.
        ' -------------------------------------------------------------------*/

        ' Local defines for the annotations.
        Private Const STRING_LENGTH_MAX As Integer = 20
        Private Const STRING_POS_X As Integer = 20
        Private Const STRING_POS_Y As Integer = 20

        Private Shared Function ArchiveFunction(ByVal HookType As MIL_INT, ByVal HookId As MIL_ID, ByVal HookDataPtr As IntPtr) As MIL_INT

            Dim HookDataHandle As GCHandle = CType(HookDataPtr, GCHandle)
            Dim UserHookDataPtr As HookDataObject = CType(HookDataHandle.Target, HookDataObject)
            Dim ModifiedImage As MIL_ID = MIL.M_NULL

            ' Retrieve the MIL_ID of the grabbed buffer.
            MIL.MdigGetHookInfo(HookId, MIL.M_MODIFIED_BUFFER + MIL.M_BUFFER_ID, ModifiedImage)

            ' Increment the frame count.
            UserHookDataPtr.NbGrabbedFrames += 1

            ' Draw the frame count in the image if enabled.
            If FRAME_NUMBER_ANNOTATION = MIL.M_YES Then
                MIL.MgraText(MIL.M_DEFAULT, ModifiedImage, STRING_POS_X, STRING_POS_Y, UserHookDataPtr.NbGrabbedFrames.ToString())
            End If

            ' Compress the new image if required while archiving the previous one.
            If UserHookDataPtr.MilCompressedImage <> MIL.M_NULL Then
                MIL.MbufCopy(ModifiedImage, UserHookDataPtr.MilCompressedImage)
            End If

            ' Archive the new image.
            If UserHookDataPtr.SaveSequenceToDisk Then
                Dim ImageToWrite As MIL_ID
                If UserHookDataPtr.MilCompressedImage <> MIL.M_NULL Then
                    ImageToWrite = UserHookDataPtr.MilCompressedImage
                Else
                    ImageToWrite = ModifiedImage
                End If

                MIL.MbufExportSequence(SEQUENCE_FILE, MIL.M_DEFAULT, ImageToWrite, 1, MIL.M_DEFAULT, MIL.M_WRITE)

                UserHookDataPtr.NbArchivedFrames += 1
                Console.Write("Frame #{0}               " & Constants.vbCr, UserHookDataPtr.NbArchivedFrames)
            End If

            ' Copy the new grabbed image to the display.
            MIL.MbufCopy(ModifiedImage, UserHookDataPtr.MilImageDisp)

            Return MIL.M_NULL
        End Function
    End Class
End Namespace
