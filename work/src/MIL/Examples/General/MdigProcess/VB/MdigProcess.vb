'***************************************************************************************
'
' File name: MdigProcess.vb
'
' Synopsis:  This program shows the use of the MdigProcess() function and its multiple
'            buffering acquisition to do robust real-time processing.
'
'            The user's processing code to execute is located in a callback function 
'            that will be called for each frame acquired (see ProcessingFunction()).
'
'      Note: The average processing time must be shorter than the grab time or some
'            frames will be missed. Also, if the processing results are not displayed
'            and the frame count is not drawn or printed, the CPU usage is reduced 
'            significantly.
'***************************************************************************************
Imports Microsoft.VisualBasic
Imports System
Imports System.Collections.Generic
Imports System.Text
Imports System.Runtime.InteropServices

Imports Matrox.MatroxImagingLibrary

Namespace MDigProcess
    Friend Class Program

        ' Number of images in the buffering grab queue.
        ' Generally, increasing this number gives a better real-time grab.
        Private Const BUFFERING_SIZE_MAX As Integer = 20

        ' User's processing function hook data object.
        Public Class HookDataStruct
            Public MilDigitizer As MIL_ID
            Public MilImageDisp As MIL_ID
            Public ProcessedImageCount As Integer
        End Class

        ' Main function.
        Shared Sub Main(ByVal args() As String)
            Dim MilApplication As MIL_ID = MIL.M_NULL
            Dim MilSystem As MIL_ID = MIL.M_NULL
            Dim MilDigitizer As MIL_ID = MIL.M_NULL
            Dim MilDisplay As MIL_ID = MIL.M_NULL
            Dim MilImageDisp As MIL_ID = MIL.M_NULL
            Dim MilGrabBufferList(BUFFERING_SIZE_MAX - 1) As MIL_ID
            Dim MilGrabBufferListSize As Integer = 0
            Dim ProcessFrameCount As MIL_INT = 0
            Dim ProcessFrameRate As Double = 0

            Dim UserHookData As New HookDataStruct()

            ' Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, MilApplication, MilSystem, MilDisplay, MilDigitizer, MilImageDisp)

            ' Allocate the grab buffers and clear them.
            MIL.MappControl(MIL.M_ERROR, MIL.M_PRINT_DISABLE)
            For MilGrabBufferListSize = 0 To BUFFERING_SIZE_MAX - 1
            MIL.MbufAlloc2d(MilSystem, MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_X, MIL.M_NULL), MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_Y, MIL.M_NULL), 8 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_GRAB + MIL.M_PROC, MilGrabBufferList(MilGrabBufferListSize))

                If MilGrabBufferList(MilGrabBufferListSize) <> MIL.M_NULL Then
                    MIL.MbufClear(MilGrabBufferList(MilGrabBufferListSize), &HFF)
                Else
                    Exit For
                End If
            Next MilGrabBufferListSize
            MIL.MappControl(MIL.M_ERROR, MIL.M_PRINT_ENABLE)

            ' Free buffers to leave space for possible temporary buffers.
            Dim n As Integer = 0
            Do While n < 2 AndAlso MilGrabBufferListSize > 0
                MilGrabBufferListSize -= 1
                MIL.MbufFree(MilGrabBufferList(MilGrabBufferListSize))
                n += 1
            Loop

            ' Print a message.
            Console.WriteLine()
            Console.WriteLine("MULTIPLE BUFFERED PROCESSING.")
            Console.WriteLine("-----------------------------")
            Console.WriteLine()
            Console.Write("Press <Enter> to start processing." + Constants.vbCr)

            ' Grab continuously on the display and wait for a key press.
            MIL.MdigGrabContinuous(MilDigitizer, MilImageDisp)
            Console.ReadKey()

            ' Halt continuous grab.
            MIL.MdigHalt(MilDigitizer)

            ' Initialize the user's processing function data structure.
            UserHookData.MilDigitizer = MilDigitizer
            UserHookData.MilImageDisp = MilImageDisp
            UserHookData.ProcessedImageCount = 0

            ' get a handle to the HookDataStruct object in the managed heap, we will use this 
            ' handle to get the object back in the callback function
            Dim hUserData As GCHandle = GCHandle.Alloc(UserHookData)
            Dim ProcessingFunctionPtr As New MIL_DIG_HOOK_FUNCTION_PTR(AddressOf ProcessingFunction)

            ' Start the processing. The processing function is called with every frame grabbed.
            MIL.MdigProcess(MilDigitizer, MilGrabBufferList, MilGrabBufferListSize, MIL.M_START, MIL.M_DEFAULT, ProcessingFunctionPtr, CType(hUserData, IntPtr))


            ' Here the main() is free to perform other tasks while the processing is executing.
            ' ---------------------------------------------------------------------------------

            ' Print a message and wait for a key press after a minimum number of frames.
            Console.WriteLine("Press <Enter> to stop.                    ")
            Console.WriteLine()
            Console.ReadKey()

            ' Stop the processing.
            MIL.MdigProcess(MilDigitizer, MilGrabBufferList, MilGrabBufferListSize, MIL.M_STOP, MIL.M_DEFAULT, ProcessingFunctionPtr, CType(hUserData, IntPtr))

            ' Free the GCHandle when no longer used.
            hUserData.Free()

            ' Print statistics.
            MIL.MdigInquire(MilDigitizer, MIL.M_PROCESS_FRAME_COUNT, ProcessFrameCount)
            MIL.MdigInquire(MilDigitizer, MIL.M_PROCESS_FRAME_RATE, ProcessFrameRate)
            Console.WriteLine()
            Console.WriteLine()
            Console.WriteLine("{0} frames grabbed at {1:0.0} frames/sec ({2:0.0} ms/frame).", ProcessFrameCount, ProcessFrameRate, 1000.0 / ProcessFrameRate)
            Console.WriteLine("Press <Enter> to end.")
            Console.WriteLine()
            Console.ReadKey()

            ' Free the grab buffers.
            Do While MilGrabBufferListSize > 0
                MilGrabBufferListSize -= 1
                MIL.MbufFree(MilGrabBufferList(MilGrabBufferListSize))
            Loop

            ' Release defaults.
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MilDigitizer, MilImageDisp)
        End Sub

        ' User's processing function called every time a grab buffer is ready.
        ' -----------------------------------------------------------------------

        ' Local defines.
        Private Const STRING_LENGTH_MAX As Integer = 20
        Private Const STRING_POS_X As Integer = 20
        Private Const STRING_POS_Y As Integer = 20

        Private Shared Function ProcessingFunction(ByVal HookType As MIL_INT, ByVal HookId As MIL_ID, ByVal HookDataPtr As IntPtr) As MIL_INT
            Dim ModifiedBufferId As MIL_ID = MIL.M_NULL

            ' this is how to check if the user data is null, the IntPtr class
            ' contains a member, Zero, which exists solely for this purpose
            If (Not IntPtr.Zero.Equals(HookDataPtr)) Then
                ' get the handle to the DigHookUserData object back from the IntPtr
                Dim hUserData As GCHandle = CType(HookDataPtr, GCHandle)

                ' get a reference to the DigHookUserData object
                Dim UserData As HookDataStruct = CType(hUserData.Target, HookDataStruct)

                ' Retrieve the MIL_ID of the grabbed buffer.
                MIL.MdigGetHookInfo(HookId, MIL.M_MODIFIED_BUFFER + MIL.M_BUFFER_ID, ModifiedBufferId)

                ' Increment the frame counter.
                UserData.ProcessedImageCount += 1

                ' Print and draw the frame count (remove to reduce CPU usage).
                Console.Write("Processing frame #{0}." & Constants.vbCr, UserData.ProcessedImageCount)
                MIL.MgraText(MIL.M_DEFAULT, ModifiedBufferId, STRING_POS_X, STRING_POS_Y, String.Format("{0}", UserData.ProcessedImageCount))

                ' Execute the processing and update the display.
                MIL.MimArith(ModifiedBufferId, MIL.M_NULL, UserData.MilImageDisp, MIL.M_NOT)
            End If

            Return 0
        End Function
    End Class
End Namespace
