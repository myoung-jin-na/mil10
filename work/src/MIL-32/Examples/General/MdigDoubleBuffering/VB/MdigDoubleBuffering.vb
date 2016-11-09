'***************************************************************************************
'
' File name: MdigDoubleBuffering.vb
'
' Synopsis:  This program performs a double-buffering image acquisition that alternates 
'            between 2 target buffers. This permits the processing of one buffer while 
'            acquiring the next. 
'
'            The example also uses a hook callback function to the start of frames in 
'            order to print the index of the current frame being acquired.
'
'     Note:  The double-buffering method is not recommended for real-time processing, 
'            especially when the CPU usage is high. For more robust real-time behavior,
'            use the MdigProcess() function. See MdigProcess.cpp for a complete example.
'***************************************************************************************
Imports Microsoft.VisualBasic
Imports System
Imports System.Collections.Generic
Imports System.Runtime.InteropServices
Imports System.Text

Imports Matrox.MatroxImagingLibrary

Namespace MDigDoubleBuffering
    Public Class UserDataObject
        Public NbGrabStart As MIL_INT
    End Class

    Friend Class Program
        Const STRING_LENGTH_MAX As Integer = 20

        ' Main function.
        Shared Sub Main(ByVal args() As String)
            Dim MilApplication As MIL_ID = MIL.M_NULL
            Dim MilSystem As MIL_ID = MIL.M_NULL
            Dim MilDigitizer As MIL_ID = MIL.M_NULL
            Dim MilDisplay As MIL_ID = MIL.M_NULL
            Dim MilImage(1) As MIL_ID
            Dim MilImageDisp As MIL_ID = MIL.M_NULL
            Dim [Default] As MIL_ID = MIL.M_DEFAULT
            Dim NbProc As Integer = 0
            Dim n As Integer = 0
            Dim Time As Double = 0.0
            Dim Text As New StringBuilder("0", STRING_LENGTH_MAX)
            Dim userObject As New UserDataObject()

            ' Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, MilApplication, MilSystem, MilDisplay, MilDigitizer, MilImageDisp)

            ' Allocate 2 grab buffers.
            For n = 0 To 1
                MIL.MbufAlloc2d(MilSystem, MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_X, MIL.M_NULL), MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_Y, MIL.M_NULL), 8 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_GRAB + MIL.M_PROC, MilImage(n))
            Next n

            ' Hook a function to the start of each frame to print the current frame index.
            userObject.NbGrabStart = 0
            Dim userObjectHandle As GCHandle = GCHandle.Alloc(userObject)

            Dim grabStartDelegate As New MIL_DIG_HOOK_FUNCTION_PTR(AddressOf GrabStart)
            MIL.MdigHookFunction(MilDigitizer, MIL.M_GRAB_START, grabStartDelegate, GCHandle.ToIntPtr(userObjectHandle))

            ' Print a message.
            Console.WriteLine()
            Console.WriteLine("DOUBLE BUFFERING ACQUISITION AND PROCESSING:")
            Console.WriteLine("--------------------------------------------")
            Console.WriteLine()
            Console.WriteLine("Press <Enter> to stop.")
            Console.WriteLine()

            ' Put the digitizer in asynchronous mode to be able to process while grabbing.
            MIL.MdigControl(MilDigitizer, MIL.M_GRAB_MODE, MIL.M_ASYNCHRONOUS)

            ' Grab the first buffer.
            MIL.MdigGrab(MilDigitizer, MilImage(0))

            ' Process one buffer while grabbing the other.
            n = 0
            Do
                ' Grab the other buffer while processing the previous one.
                MIL.MdigGrab(MilDigitizer, MilImage(1 - n))

                ' Synchronize and start the timer.
                If NbProc = 0 Then
                    MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_RESET + MIL.M_SYNCHRONOUS, CType(MIL.M_NULL, IntPtr))
                End If

                ' Write the frame counter.
                MIL.MgraText([Default], MilImage(n), 32, 32, String.Format("{0}", NbProc + 1))

                ' Process the first buffer already grabbed. 
                MIL.MimArith(MilImage(n), MIL.M_NULL, MilImageDisp, MIL.M_NOT)

                ' Count processed buffers.
                NbProc += 1

                ' Toggle grab buffers.
                n = 1 - n
            Loop While Not Console.KeyAvailable

            ' Wait until the end of the last grab and stop timer.
            MIL.MdigGrabWait(MilDigitizer, MIL.M_GRAB_END)
            MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ + MIL.M_SYNCHRONOUS, Time)
            Console.ReadKey()

            ' Print statistics.
            Console.WriteLine("{0} frames grabbed, at a frame rate of {1:0.00} frames/sec ({2:0.00} ms/frame).", NbProc, NbProc / Time, 1000.0 * Time / NbProc)
            Console.WriteLine("Press <Enter> to end.")
            Console.WriteLine()
            Console.ReadKey()

            ' Unhook the function at the start of each frame.
            MIL.MdigHookFunction(MilDigitizer, MIL.M_GRAB_START + MIL.M_UNHOOK, grabStartDelegate, GCHandle.ToIntPtr(userObjectHandle))

            ' Free GCHandle to allow the garbage collector to reclaim the object.
            userObjectHandle.Free()

            ' Free allocations.
            For n = 0 To 1
                MIL.MbufFree(MilImage(n))
            Next n
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MilDigitizer, MilImageDisp)
        End Sub

        ' Grab Start hook function:
        ' This function is called at the start of each frame captured.
        '
        Private Shared Function GrabStart(ByVal HookType As MIL_INT, ByVal EventId As MIL_ID, ByVal UserObjectPtr As IntPtr) As MIL_INT

            If (UserObjectPtr <> IntPtr.Zero) Then

                Dim userObjectHandle As GCHandle = GCHandle.FromIntPtr(UserObjectPtr)
                Dim userData As UserDataObject = TryCast(userObjectHandle.Target, UserDataObject)
                If (userData IsNot Nothing) Then

                    ' Increment grab start count and print it.
                    userData.NbGrabStart += 1
                    Console.Write("#{0}" + Constants.vbCr, userData.NbGrabStart)
                End If
            End If

            Return 0

        End Function
    End Class
End Namespace
