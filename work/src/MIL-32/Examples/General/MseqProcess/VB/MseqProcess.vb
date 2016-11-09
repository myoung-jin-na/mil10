'************************************************************************************
'
' File name: MseqProcess.vb
'
' Synopsis:  This program shows the use of the MseqProcess() and MseqFeed() functions 
'            to perform real-time encoding of a sequence of captured images.
'
'            The user's preprocessing and compression code is written in a hook 
'            function that will be called by MdigProcess() for each frame grabbed 
'            (see ProcessingFunction()). The queueing for encoding of the next 
'            frame is also made in that hook function to allow fully parallel 
'            execution of the capture and the encoding.
'
'      Note: The average encoding time must be shorter than the grab time or
'            some frames will be missed. To miss frames is very frequent when 
'            the encoding is done by software. Also, if the captured images 
'            are not displayed and the frame count is not drawn or printed 
'            in the hook function, the CPU usage is reduced significantly.
'            
'            When encoding a 1080p source it is recommended to have your 
'            MIL Non-Paged Memory set to at least 64MB.
'************************************************************************************
Imports Microsoft.VisualBasic
Imports System
Imports System.Collections.Generic
Imports System.Text

Imports Matrox.MatroxImagingLibrary
Imports System.Runtime.InteropServices

Namespace MseqProcess
    ' User's processing function hook data object.
    Friend Class ProcessingHookDataStruct
        Public MilDigitizer As MIL_ID
        Public MilImageDisp As MIL_ID
        Public MilSeqContext As MIL_ID
        Public ProcessedImageCount As MIL_INT
        Public ProcessingOperation As ProcessingHookOperation
    End Class

    ' Optional encoding end function hook data object.
    Friend Class EncodingFrameEndHookDataStruct
        Public EncodedImageCount As MIL_INT
    End Class

    ' Optional decoding end function hook data object.
    Friend Class DecodingFrameEndHookDataStruct
        Public DecodedImageCount As MIL_INT
        Public MilImageDisp As MIL_ID
    End Class

    Friend Enum ProcessingHookOperation
        DISPLAY
        ENCODE
    End Enum

    Friend Class Program
        ' Number of images in the buffering grab queue.
        ' Generally, increasing this number gives better real-time grab.
        Private Const BUFFERING_SIZE_MAX As Integer = 20

        ' Target sequence file name and location.
        ' The temporary directory location can be reached with %temp% under Windows. 
        Private Shared ReadOnly SEQUENCE_FILE As String = MIL.M_TEMP_DIR & "SeqProcess.mp4"

        ' Remote target sequence file name and location if Distributed MIL is used.
        Private Shared ReadOnly REMOTE_SEQUENCE_FILE As String = "remote:///" + SEQUENCE_FILE


        Shared Function Main(ByVal args() As String) As Integer
            Dim MilApplication As MIL_ID = MIL.M_NULL
            Dim MilRemoteApplication As MIL_ID = MIL.M_NULL
            Dim MilSystem As MIL_ID = MIL.M_NULL
            Dim MilDigitizer As MIL_ID = MIL.M_NULL
            Dim MilDisplay As MIL_ID = MIL.M_NULL
            Dim MilImageDisp As MIL_ID = MIL.M_NULL
            Dim MilGrabBufferList(BUFFERING_SIZE_MAX - 1) As MIL_ID
            Dim MilCompressContext As MIL_ID = MIL.M_NULL
            Dim MilDecompressContext As MIL_ID = MIL.M_NULL
            Dim LicenseModules As MIL_INT = 0
            Dim MilSystemLocation As MIL_INT = MIL.M_NULL
            Dim MilGrabBufferListSize As MIL_INT
            Dim ProcessFrameCount As MIL_INT = 0
            Dim NbFrames As MIL_INT = 0
            Dim n As MIL_INT = 0
            Dim EncodingDesiredFrameRate As Double = 0.0
            Dim ProcessFrameRate As Double = 0.0
            Dim SeqProcessFilePathSize As MIL_INT = 0
            Dim SeqProcessFilePath As StringBuilder = Nothing
            Dim ProcessingUserHookData As New ProcessingHookDataStruct()
            Dim EncodingFrameEndUserHookData As New EncodingFrameEndHookDataStruct()
            Dim DecodingFrameEndUserHookData As New DecodingFrameEndHookDataStruct()
            Dim SeqSystemType As MIL_INT = MIL.M_NULL

            ' Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, MilApplication, MilSystem, MilDisplay, MilDigitizer, MilImageDisp)

            MIL.MsysInquire(MilSystem, MIL.M_OWNER_APPLICATION, MilRemoteApplication)
            MilSystemLocation = MIL.MsysInquire(MilSystem, MIL.M_LOCATION, MIL.M_NULL)

         If (MIL.MappInquire(MilRemoteApplication, MIL.M_PLATFORM_OS_TYPE, MIL.M_NULL) <> MIL.M_OS_WINDOWS) Then
            If (MilSystemLocation = MIL.M_REMOTE) Then
               Console.WriteLine("The Distributed MIL server must run on a Windows system.")
            Else
               Console.WriteLine("This example only works with a Windows system.")
            End If
            Console.WriteLine("Press <Enter> to end.")
            Console.ReadKey()

            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MilDigitizer, MilImageDisp)
            Return 0
         End If
            ' Inquire MIL licenses.
            MIL.MappInquire(MilRemoteApplication, MIL.M_LICENSE_MODULES, LicenseModules)

            If ((LicenseModules And MIL.M_LICENSE_JPEGSTD) <> MIL.M_LICENSE_JPEGSTD) Then
               Console.WriteLine("Need a Compression/Decompression license to run this example.")
               Console.WriteLine("Press <Enter> to end.")
               Console.ReadKey()

               MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MilDigitizer, MilImageDisp)
               Return 0
            End If

            ' Allocate the grab buffers and clear them.
            MIL.MappControl(MIL.M_DEFAULT, MIL.M_ERROR, MIL.M_PRINT_DISABLE)
            For MilGrabBufferListSize = 0 To BUFFERING_SIZE_MAX - 1
               MIL.MbufAllocColor(MilSystem, MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_BAND, MIL.M_NULL), MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_X, MIL.M_NULL), MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_Y, MIL.M_NULL), 8 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_GRAB + MIL.M_PROC, MilGrabBufferList(CType(MilGrabBufferListSize, Integer)))

               If MilGrabBufferList(CType(MilGrabBufferListSize, Integer)) <> MIL.M_NULL Then
                  MIL.MbufClear(MilGrabBufferList(CType(MilGrabBufferListSize, Integer)), &HFF)
               Else
                  Exit For
               End If
            Next MilGrabBufferListSize
            MIL.MappControl(MIL.M_DEFAULT, MIL.M_ERROR, MIL.M_PRINT_ENABLE)

            ' Free buffers to leave space for possible temporary buffers.
            n = 0
            Do While n < 2 AndAlso MilGrabBufferListSize > 0
               MilGrabBufferListSize -= 1
               MIL.MbufFree(MilGrabBufferList(CType(MilGrabBufferListSize, Integer)))
               n += 1
            Loop

            If MilGrabBufferListSize = 0 Then
               Console.WriteLine("!!! No grab buffers have been allocated. Need to set more Non-Paged Memory. !!!")

               MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MilDigitizer, MilImageDisp)
               Console.WriteLine("Press <Enter> to end.")
               Console.ReadKey()
               Return 1
            End If

            ' Initialize the User's processing function data structure only for Display.
            ProcessingUserHookData.MilDigitizer = MilDigitizer
            ProcessingUserHookData.MilSeqContext = MIL.M_NULL
            ProcessingUserHookData.MilImageDisp = MilImageDisp
            ProcessingUserHookData.ProcessedImageCount = 0
            ProcessingUserHookData.ProcessingOperation = ProcessingHookOperation.DISPLAY

            ' get a handle to the HookDataStruct object in the managed heap, we will use this 
            ' handle to get the object back in the callback function
            Dim hUserData As GCHandle = GCHandle.Alloc(ProcessingUserHookData)

            ' Start the sequence acquisition. The preprocessing and encoding hook function 
            ' is called for every frame grabbed.
            Dim ProcessingFunctionDelegate As MIL_DIG_HOOK_FUNCTION_PTR = AddressOf ProcessingFunction

            ' Start a digProcess to show the live camera output.
            MIL.MdigProcess(MilDigitizer, MilGrabBufferList, MilGrabBufferListSize, MIL.M_START, MIL.M_DEFAULT, ProcessingFunctionDelegate, GCHandle.ToIntPtr(hUserData))

            ' Print a message.
            Console.Write(vbNewLine + "H.264 IMAGE SEQUENCE COMPRESSION." + vbNewLine)
            Console.Write("---------------------------------" + vbNewLine + vbNewLine)
            Console.Write("Press <Enter> to start compression." + vbCr)
            Console.ReadKey()

            ' Stop digProcess.
            MIL.MdigProcess(MilDigitizer, MilGrabBufferList, MilGrabBufferListSize, MIL.M_STOP, MIL.M_DEFAULT, ProcessingFunctionDelegate, GCHandle.ToIntPtr(hUserData))

            ' Inquire the dig process frame rate
            MIL.MdigInquire(MilDigitizer, MIL.M_PROCESS_FRAME_RATE, EncodingDesiredFrameRate)
            Console.Write("Grabbing frames at  {0:0.00} frames/sec." + vbNewLine, EncodingDesiredFrameRate)

            ' Creates a context for the H.264 compression engine. Compression will be done
            ' using hardware or software depending on the system hardware configuration. 
            MIL.MseqAlloc(MilSystem, MIL.M_DEFAULT, MIL.M_SEQ_COMPRESS, MIL.M_DEFAULT, MIL.M_DEFAULT, MilCompressContext)

            ' Specify the destination of the compressed file and the target container type.
            ' The last argument specifies to generate an MP4 file.
            MIL.MseqDefine(MilCompressContext, MIL.M_SEQ_OUTPUT(0) + MIL.M_SEQ_DEST(0), MIL.M_FILE, (If(MilSystemLocation <> MIL.M_REMOTE, SEQUENCE_FILE, REMOTE_SEQUENCE_FILE)), MIL.M_FILE_FORMAT_MP4)

            ' Set the compression parameters.
            ' Sets the compression parameters valid for any resolution under 1920X1080.
            ' Any resolution higher than that will generate an warning that can be disabled using
            ' MseqControl with M_SETTING_AUTO_ADJUSTMENT. See documentation for more details.

            MIL.MseqControl(MilCompressContext, MIL.M_CONTEXT, MIL.M_STREAM_BIT_RATE_MODE, MIL.M_VARIABLE)      ' MIL.M_VARIABLE or MIL.M_CONSTANT
            MIL.MseqControl(MilCompressContext, MIL.M_CONTEXT, MIL.M_STREAM_BIT_RATE, 5000)                     ' 5 Mbps bit rate
            MIL.MseqControl(MilCompressContext, MIL.M_CONTEXT, MIL.M_STREAM_BIT_RATE_MAX, 5000)                 ' 5 Mbps bit rate
            MIL.MseqControl(MilCompressContext, MIL.M_CONTEXT, MIL.M_STREAM_FRAME_RATE, EncodingDesiredFrameRate)      ' 60Hz frame rate.
            MIL.MseqControl(MilCompressContext, MIL.M_CONTEXT, MIL.M_STREAM_FRAME_RATE_MODE, MIL.M_VARIABLE)    ' Attempts to update the file header with the encoding frame rate
            ' if lower than the specified frame rate.

            MIL.MseqControl(MilCompressContext, MIL.M_CONTEXT, MIL.M_STREAM_QUALITY, 100)                       ' 1=best speed, 100=best quality
            MIL.MseqControl(MilCompressContext, MIL.M_CONTEXT, MIL.M_STREAM_PROFILE, MIL.M_PROFILE_HIGH)        ' MIL.M_PROFILE_BASELINE, MIL.M_PROFILE_MAIN, MIL.M_PROFILE_HIGH
            MIL.MseqControl(MilCompressContext, MIL.M_CONTEXT, MIL.M_STREAM_LEVEL, MIL.M_LEVEL_4_2)             ' MIL.M_LEVEL_1, MIL.M_LEVEL_1B, MIL.M_LEVEL_1_1, MIL.M_LEVEL_1_2, MIL.M_LEVEL_1_3,
            ' MIL.M_LEVEL_2, MIL.M_LEVEL_2_1, MIL.M_LEVEL_2_2,
            ' MIL.M_LEVEL_3, MIL.M_LEVEL_3_1, MIL.M_LEVEL_3_2,
            ' MIL.M_LEVEL_4, MIL.M_LEVEL_4_1, MIL.M_LEVEL_4_2,
            ' MIL.M_LEVEL_5, MIL.M_LEVEL_5_1
            MIL.MseqControl(MilCompressContext, MIL.M_CONTEXT, MIL.M_STREAM_GROUP_OF_PICTURE_SIZE, 30) ' Interval between I-Frame

            ' Initialize the optional encoding end function data structure.
            EncodingFrameEndUserHookData.EncodedImageCount = 0

            ' get a handle to the HookDataStruct object in the managed heap, we will use this 
            ' handle to get the object back in the callback function
            Dim EncodingFrameEndUserHookDataHandle As GCHandle = GCHandle.Alloc(EncodingFrameEndUserHookData)

            ' Register the encoding end function to the sequence context.
            Dim FrameEncodingEndFunctionDelegate As MIL_SEQ_HOOK_FUNCTION_PTR = AddressOf FrameEncodingEndFunction
            MIL.MseqHookFunction(MilCompressContext, MIL.M_FRAME_END, FrameEncodingEndFunctionDelegate, GCHandle.ToIntPtr(EncodingFrameEndUserHookDataHandle))

            ' Provide a sample image to initialize the encoding engine accordingly.
            MIL.MseqControl(MilCompressContext, MIL.M_CONTEXT, MIL.M_BUFFER_SAMPLE, MilGrabBufferList(0))

            ' Start the encoding process, waits for buffer to be fed for encoding.
            MIL.MseqProcess(MilCompressContext, MIL.M_START, MIL.M_ASYNCHRONOUS)

            ' Display the type of compression used.
            Console.Write("Live image capture and compression to file using ")
            MIL.MseqInquire(MilCompressContext, MIL.M_CONTEXT, MIL.M_CODEC_TYPE, SeqSystemType)
            If SeqSystemType = MIL.M_HARDWARE + MIL.M_QSV Then
               Console.WriteLine("Hardware acceleration.")
            Else ' MIL.M_SOFTWARE + MIL.M_QSV
               Console.WriteLine("Software implementation.")
            End If

            ' Set the sequence context id in the user hook data structure to start
            ' feeding buffers for encoding in ProcessingFunction.
            '
            ProcessingUserHookData.MilSeqContext = MilCompressContext
            ProcessingUserHookData.ProcessedImageCount = 0
            ProcessingUserHookData.ProcessingOperation = ProcessingHookOperation.ENCODE

            MIL.MdigProcess(MilDigitizer, MilGrabBufferList, MilGrabBufferListSize, MIL.M_START, MIL.M_DEFAULT, ProcessingFunctionDelegate, GCHandle.ToIntPtr(hUserData))

            ' NOTE: Now the main() is free to perform other tasks while the compression is executing.
            ' ---------------------------------------------------------------------------------------

            ' Print a message and wait for a key press after a minimum number of frames.
            Console.WriteLine("Press <Enter> to stop." + vbNewLine)
            Console.ReadKey()

            ' Stop the processing.
            MIL.MdigProcess(MilDigitizer, MilGrabBufferList, MilGrabBufferListSize, MIL.M_STOP + MIL.M_WAIT, MIL.M_DEFAULT, ProcessingFunctionDelegate, GCHandle.ToIntPtr(hUserData))

            ' Free the processing user data handle.
            hUserData.Free()

            ' Stop the encoding process
            MIL.MseqProcess(MilCompressContext, MIL.M_STOP, MIL.M_WAIT)

            ' Make sure the hook handler function delegate is not prematurely garbage collected since
            ' only MIL has a reference to it.
            GC.KeepAlive(FrameEncodingEndFunctionDelegate)

            ' Free the encoding user data handle.
            EncodingFrameEndUserHookDataHandle.Free()

            ' Print statistics.
            MIL.MdigInquire(MilDigitizer, MIL.M_PROCESS_FRAME_COUNT, ProcessFrameCount)
            MIL.MdigInquire(MilDigitizer, MIL.M_PROCESS_FRAME_RATE, ProcessFrameRate)
            Console.WriteLine("{0} frames encoded at {1:0.00} frames/sec ({2:0.0} ms/frame).", ProcessFrameCount, ProcessFrameRate, 1000.0 / ProcessFrameRate)
            Console.WriteLine()

            MIL.MseqInquire(MilCompressContext, MIL.M_SEQ_OUTPUT(0) + MIL.M_SEQ_DEST(0), MIL.M_STREAM_FILE_NAME_SIZE, SeqProcessFilePathSize)
            SeqProcessFilePath = New StringBuilder(CInt(Fix(SeqProcessFilePathSize)))
            MIL.MseqInquire(MilCompressContext, MIL.M_SEQ_OUTPUT(0) + MIL.M_SEQ_DEST(0), MIL.M_STREAM_FILE_NAME, SeqProcessFilePath)
            Console.WriteLine("The video sequence file was written to:")
            Console.WriteLine("{0}.", SeqProcessFilePath.ToString())
            Console.WriteLine()
            Console.WriteLine("It can be played back using any compatible video player.")

            ' Free the grab buffers and sequence context.
            Do While MilGrabBufferListSize > 0
               MilGrabBufferListSize -= 1
               MIL.MbufFree(MilGrabBufferList(CType(MilGrabBufferListSize, Integer)))
               MilGrabBufferList(CType(MilGrabBufferListSize, Integer)) = MIL.M_NULL
            Loop

            MIL.MseqFree(MilCompressContext)

            ' Wait for a key to start the replay.
            Console.WriteLine("Press <Enter> to replay encoded sequence.")
            Console.ReadKey()

            MIL.MseqAlloc(MilSystem, MIL.M_DEFAULT, MIL.M_SEQ_DECOMPRESS, MIL.M_DEFAULT, MIL.M_DEFAULT, MilDecompressContext)

            ' Specify the destination of the compressed file and the target container type.
            ' The last argument specifies to generate an MP4 file.
            MIL.MseqDefine(MilDecompressContext, MIL.M_SEQ_INPUT(0), MIL.M_FILE, (If(MilSystemLocation <> MIL.M_REMOTE, SEQUENCE_FILE, REMOTE_SEQUENCE_FILE)), MIL.M_FILE_FORMAT_MP4)

            Dim outputFrameRate As Double = 0.0
            MIL.MseqInquire(MilDecompressContext, MIL.M_SEQ_INPUT(0), MIL.M_STREAM_FRAME_RATE, outputFrameRate)

            Console.WriteLine()
            Console.WriteLine("Replaying file at {0:0.00} frames/second.", outputFrameRate)

            ' Initialize the optional decoding end function data structure.
            DecodingFrameEndUserHookData.DecodedImageCount = 0
            DecodingFrameEndUserHookData.MilImageDisp = MilImageDisp

            ' get a handle to the HookDataStruct object in the managed heap, we will use this 
            ' handle to get the object back in the callback function
            Dim DecodingFrameEndUserHookDataHandle As GCHandle = GCHandle.Alloc(DecodingFrameEndUserHookData)

            ' Register the decoding end function to the sequence context.
            Dim FrameDecodingEndFunctionDelegate As MIL_SEQ_HOOK_FUNCTION_PTR = AddressOf FrameDecodingEndFunction
            MIL.MseqHookFunction(MilDecompressContext, MIL.M_FRAME_END, FrameDecodingEndFunctionDelegate, GCHandle.ToIntPtr(DecodingFrameEndUserHookDataHandle))

            ' Start the decoding process, waits for buffer to be fed for encoding.
            MIL.MseqProcess(MilDecompressContext, MIL.M_START, MIL.M_ASYNCHRONOUS)

            ' Print a message and wait for a key press after a minimum number of frames.
            Console.WriteLine("Press <Enter> to stop." + vbNewLine)
            Console.ReadKey()

            ' Stop the play back.
            MIL.MseqProcess(MilDecompressContext, MIL.M_STOP, MIL.M_NULL)

            ' Make sure the hook handler function delegate is not prematurely garbage collected since
            ' only MIL has a reference to it.
            GC.KeepAlive(FrameDecodingEndFunctionDelegate)

            ' Free the decoding user data handle.
            DecodingFrameEndUserHookDataHandle.Free()

            MIL.MseqFree(MilDecompressContext)

            ' Wait for a key to end.
            Console.WriteLine("Press <Enter> to end.")
            Console.ReadKey()

            ' Release defaults.
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MilDigitizer, MilImageDisp)

            Return 0
      End Function

        ' User's processing function called every time a grab buffer is modified. 
        ' ------------------------------------------------------------------------
        ' 
        ' Local defines.
        Private Const STRING_LENGTH_MAX As Integer = 20
        Private Const STRING_POS_X As Integer = 20
        Private Const STRING_POS_Y As Integer = 20

        Private Shared Function ProcessingFunction(ByVal HookType As MIL_INT, ByVal HookId As MIL_ID, ByVal HookDataPtr As IntPtr) As MIL_INT
            ' this is how to check if the user data is null, the IntPtr class
            ' contains a member, Zero, which exists solely for this purpose
            If (Not IntPtr.Zero.Equals(HookDataPtr)) Then
                ' get the handle to the ProcessingHookDataStruct object back from the IntPtr
                Dim hUserData As GCHandle = GCHandle.FromIntPtr(HookDataPtr)

                ' get a reference to the ProcessingHookDataStruct object
                Dim UserHookDataPtr As ProcessingHookDataStruct = TryCast(hUserData.Target, ProcessingHookDataStruct)
                Dim ModifiedBufferId As MIL_ID = MIL.M_NULL
                Dim Text As String

                ' Retrieve the MIL_ID of the grabbed buffer.
                MIL.MdigGetHookInfo(HookId, MIL.M_MODIFIED_BUFFER + MIL.M_BUFFER_ID, ModifiedBufferId)

                Select Case UserHookDataPtr.ProcessingOperation
                    Case ProcessingHookOperation.DISPLAY
                        ' Update the display with the last captured image.
                        MIL.MbufCopy(ModifiedBufferId, UserHookDataPtr.MilImageDisp)
                    Case ProcessingHookOperation.ENCODE
                        ' Increase the compressed images count.
                        UserHookDataPtr.ProcessedImageCount += 1

                        ' Print and draw the frame count (comment to reduce CPU usage).
                        Console.Write("Processing frame #{0}." & Constants.vbCr, UserHookDataPtr.ProcessedImageCount)
                        Text = String.Format("{0}", UserHookDataPtr.ProcessedImageCount)
                        MIL.MgraText(MIL.M_DEFAULT, ModifiedBufferId, STRING_POS_X, STRING_POS_Y, Text)

                        ' Enqueue the grabbed buffer for parallel encoding.
                        MIL.MseqFeed(UserHookDataPtr.MilSeqContext, ModifiedBufferId, MIL.M_DEFAULT)

                        ' Update the display with the last captured image.
                        MIL.MbufCopy(ModifiedBufferId, UserHookDataPtr.MilImageDisp)
                End Select
            End If

            Return 0
        End Function

        ' Optional encoding end function called every time a buffer is finished being compressed. 
        ' ----------------------------------------------------------------------------------------
        Private Shared Function FrameEncodingEndFunction(ByVal HookType As MIL_INT, ByVal HookId As MIL_ID, ByVal HookDataPtr As IntPtr) As MIL_INT
            ' this is how to check if the user data is null, the IntPtr class
            ' contains a member, Zero, which exists solely for this purpose
            If (Not IntPtr.Zero.Equals(HookDataPtr)) Then
                ' get the handle to the FrameEndHookDataStruct object back from the IntPtr
                Dim hUserData As GCHandle = GCHandle.FromIntPtr(HookDataPtr)

                ' get a reference to the EncodingFrameEndHookDataStruct object
                Dim UserHookDataPtr As EncodingFrameEndHookDataStruct = TryCast(hUserData.Target, EncodingFrameEndHookDataStruct)

                ' Frame end hook post processing.
                If HookType = MIL.M_FRAME_END Then
                    Dim CompressedBufferId As MIL_ID = MIL.M_NULL
                    Dim CompressedDataPtr As MIL_INT = MIL.M_NULL
                    Dim CompressedDataSize As MIL_INT = 0

                    ' Increment a encoded frame counter
                    UserHookDataPtr.EncodedImageCount += 1

                    ' Retrieve the MIL_ID of the encoded buffer.
                    MIL.MseqGetHookInfo(HookId, MIL.M_MODIFIED_BUFFER + MIL.M_BUFFER_ID, CompressedBufferId)

                    ' Retrieves the address of the encoded data.
                    MIL.MbufInquire(CompressedBufferId, MIL.M_HOST_ADDRESS, CompressedDataPtr)

                    ' Retrieves the size in bytes of the encoded data.
                    MIL.MbufInquire(CompressedBufferId, MIL.M_SIZE_BYTE, CompressedDataSize)

                    '  -----------------------------------------------------------------------------------------------
                    '  Here you can do any action with the encoded data such as send buffer through a network stream. 
                    '  If the processing done on the compressed data is long, it is recommended to copy the           
                    '  buffer and to process it in a separate thread to avoid blocking the compressions flow.         
                    '  -----------------------------------------------------------------------------------------------
                End If
            End If

            Return 0
        End Function

        ' Optional encoding end function called every time a buffer is finished being compressed. 
        ' ----------------------------------------------------------------------------------------
        Private Shared Function FrameDecodingEndFunction(ByVal HookType As MIL_INT, ByVal HookId As MIL_ID, ByVal HookDataPtr As IntPtr) As MIL_INT
            ' this is how to check if the user data is null, the IntPtr class
            ' contains a member, Zero, which exists solely for this purpose
            If (Not IntPtr.Zero.Equals(HookDataPtr)) Then
                ' get the handle to the FrameEndHookDataStruct object back from the IntPtr
                Dim hUserData As GCHandle = GCHandle.FromIntPtr(HookDataPtr)

                ' get a reference to the DecodingFrameEndHookDataStruct object
                Dim UserHookDataPtr As DecodingFrameEndHookDataStruct = TryCast(hUserData.Target, DecodingFrameEndHookDataStruct)

                ' Frame end hook post processing.
                If HookType = MIL.M_FRAME_END Then
                    Dim DecompressedBufferId As MIL_ID = MIL.M_NULL

                    ' Increment a encoded frame counter
                    UserHookDataPtr.DecodedImageCount += 1

                    ' Retrieve the MIL_ID of the encoded buffer.
                    MIL.MseqGetHookInfo(HookId, MIL.M_MODIFIED_BUFFER + MIL.M_BUFFER_ID, DecompressedBufferId)

                    ' -----------------------------------------------------------------------------------------------
                    ' Here you can do any action with the decoded buffer.
                    ' -----------------------------------------------------------------------------------------------

                    MIL.MbufCopy(DecompressedBufferId, UserHookDataPtr.MilImageDisp)

                End If
            End If

            Return 0
        End Function

    End Class
End Namespace
