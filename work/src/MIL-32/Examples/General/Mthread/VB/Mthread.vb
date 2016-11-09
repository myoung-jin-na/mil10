'*******************************************************************************
'
' File name: MThread.vb
'
' Synopsis:  This program shows how to use threads in a MIL application and
'            synchronize them with event. It creates 4 processing threads that 
'            are used to work in 4 different regions of a display buffer.
'
'     Thread usage:
'      - The main thread starts a processing thread in each of the 4 different
'        quarters of a display buffer. The main thread then waits for a key to
'        be pressed to stop them.
'      - The top-left and bottom-left threads work in a loop, as follows: the
'        top-left thread adds a constant to its buffer, then sends an event to
'        the bottom-left thread. The bottom-left thread waits for the event
'        from the top-left thread, rotates the top-left buffer image, then sends an
'        event to the top-left thread to start a new loop.
'      - The top-right and bottom-right threads work the same way as the
'        top-left and bottom-left threads, except that the bottom-right thread
'        performs an edge detection operation, rather than a rotation.
'
'      Note : - Under MIL-Lite, the threads will do graphic annotations instead.
'             - Comment out the MIL.MdispSelect() if you wish to avoid benchmarking 
'               the display update overhead on CPU usage and processing rate.
'*******************************************************************************
Imports Microsoft.VisualBasic
Imports System
Imports System.Collections.Generic
Imports System.Text
Imports System.Threading
Imports System.Runtime.InteropServices

Imports Matrox.MatroxImagingLibrary

Namespace MThread
    Friend Class Program
        ' Local constants.
        Private Const IMAGE_FILE As String = MIL.M_IMAGE_PATH & "Bird.mim"
        Private Const IMAGE_WIDTH As Integer = 256
        Private Const IMAGE_HEIGHT As Integer = 240
        Private Const STRING_LENGTH_MAX As Integer = 40
        Private Const STRING_POS_X As Integer = 10
        Private Const STRING_POS_Y As Integer = 220
        Private Const DRAW_RADIUS_NUMBER As Integer = 5
        Private Const DRAW_RADIUS_STEP As Integer = 10
        Private Const DRAW_CENTER_POSX As Integer = 196
        Private Const DRAW_CENTER_POSY As Integer = 180

        ' Thread parameters object.
        Public Class THREAD_PARAM
            Public Id As MIL_ID
            Public System As MIL_ID
            Public OrgImage As MIL_ID
            Public SrcImage As MIL_ID
            Public DstImage As MIL_ID
            Public ReadyEvent As MIL_ID
            Public DoneEvent As MIL_ID
            Public NumberOfIteration As Integer
            Public Radius As Integer
            Public [Exit] As Integer
            Public LicenseModules As MIL_INT
            Public SlaveThreadParam As THREAD_PARAM
        End Class

        ' Main function:
        ' --------------
        Shared Sub Main(ByVal args() As String)
            Dim MilApplication As MIL_ID = MIL.M_NULL ' Application identifier.
            Dim MilRemoteApplication As MIL_ID = MIL.M_NULL ' Remote Application identifier.
            Dim MilSystem As MIL_ID = MIL.M_NULL ' System identifier.
            Dim MilDisplay As MIL_ID = MIL.M_NULL ' Display identifier.
            Dim MilImage As MIL_ID = MIL.M_NULL ' Image buffer identifiers.
            Dim MilOrgImage As MIL_ID = MIL.M_NULL
            Dim TParTopLeft As New THREAD_PARAM() ' Parameters passed to top left thread.
            Dim TParBotLeft As New THREAD_PARAM() ' Parameters passed to bottom left thread.
            Dim TParTopRight As New THREAD_PARAM() ' Parameters passed to top right thread.
            Dim TParBotRight As New THREAD_PARAM() ' Parameters passed to bottom right thread.
            Dim Time As Double = 0.0 ' Timer variables.
            Dim FramesPerSecond As Double = 0.0
            Dim LicenseModules As MIL_INT = 0 ' List of available MIL modules. 

            ' Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, MilApplication, MilSystem, MilDisplay, CType(MIL.M_NULL, IntPtr), CType(MIL.M_NULL, IntPtr))

            ' Allocate and display the main image buffer.
            MIL.MbufAlloc2d(MilSystem, IMAGE_WIDTH * 2, IMAGE_HEIGHT * 2, 8 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_PROC + MIL.M_DISP, MilImage)
            MIL.MbufClear(MilImage, 0)
            MIL.MdispSelect(MilDisplay, MilImage)

            ' Allocate an image buffer to keep the original.
            MIL.MbufAlloc2d(MilSystem, IMAGE_WIDTH, IMAGE_HEIGHT, 8 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_PROC, MilOrgImage)

            ' Allocate a destination child buffer for each thread.
            MIL.MbufChild2d(MilImage, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT, TParTopLeft.SrcImage)
            MIL.MbufChild2d(MilImage, 0, IMAGE_HEIGHT, IMAGE_WIDTH, IMAGE_HEIGHT, TParBotLeft.DstImage)
            MIL.MbufChild2d(MilImage, IMAGE_WIDTH, 0, IMAGE_WIDTH, IMAGE_HEIGHT, TParTopRight.SrcImage)
            MIL.MbufChild2d(MilImage, IMAGE_WIDTH, IMAGE_HEIGHT, IMAGE_WIDTH, IMAGE_HEIGHT, TParBotRight.DstImage)

            ' Allocate synchronization events.
            MIL.MthrAlloc(MilSystem, MIL.M_EVENT, MIL.M_DEFAULT, MIL.M_NULL, MIL.M_NULL, TParTopLeft.DoneEvent)
            MIL.MthrAlloc(MilSystem, MIL.M_EVENT, MIL.M_DEFAULT, MIL.M_NULL, MIL.M_NULL, TParBotLeft.DoneEvent)
            MIL.MthrAlloc(MilSystem, MIL.M_EVENT, MIL.M_DEFAULT, MIL.M_NULL, MIL.M_NULL, TParTopRight.DoneEvent)
            MIL.MthrAlloc(MilSystem, MIL.M_EVENT, MIL.M_DEFAULT, MIL.M_NULL, MIL.M_NULL, TParBotRight.DoneEvent)

            ' Inquire MIL licenses.
            MIL.MsysInquire(MilSystem, MIL.M_OWNER_APPLICATION, MilRemoteApplication)
            MIL.MappInquire(MilRemoteApplication, MIL.M_LICENSE_MODULES, LicenseModules)

            ' Initialize remaining thread parameters.
            TParTopLeft.System = MilSystem
            TParTopLeft.OrgImage = MilOrgImage
            TParTopLeft.DstImage = TParTopLeft.SrcImage
            TParTopLeft.ReadyEvent = TParBotLeft.DoneEvent
            TParTopLeft.NumberOfIteration = 0
            TParTopLeft.Radius = 0
            TParTopLeft.Exit = 0
            TParTopLeft.LicenseModules = LicenseModules
            TParTopLeft.SlaveThreadParam = TParBotLeft

            TParBotLeft.System = MilSystem
            TParBotLeft.OrgImage = 0
            TParBotLeft.SrcImage = TParTopLeft.DstImage
            TParBotLeft.ReadyEvent = TParTopLeft.DoneEvent
            TParBotLeft.NumberOfIteration = 0
            TParBotLeft.Radius = 0
            TParBotLeft.Exit = 0
            TParBotLeft.LicenseModules = LicenseModules
            TParBotLeft.SlaveThreadParam = Nothing

            TParTopRight.System = MilSystem
            TParTopRight.OrgImage = MilOrgImage
            TParTopRight.DstImage = TParTopRight.SrcImage
            TParTopRight.ReadyEvent = TParBotRight.DoneEvent
            TParTopRight.NumberOfIteration = 0
            TParTopRight.Radius = 0
            TParTopRight.Exit = 0
            TParTopRight.LicenseModules = LicenseModules
            TParTopRight.SlaveThreadParam = TParBotRight

            TParBotRight.System = MilSystem
            TParBotRight.OrgImage = 0
            TParBotRight.SrcImage = TParTopRight.DstImage
            TParBotRight.ReadyEvent = TParTopRight.DoneEvent
            TParBotRight.NumberOfIteration = 0
            TParBotRight.Radius = 0
            TParBotRight.Exit = 0
            TParBotRight.LicenseModules = LicenseModules
            TParBotRight.SlaveThreadParam = Nothing

            ' Initialize the original image to process.
            MIL.MbufLoad(IMAGE_FILE, MilOrgImage)

            ' Start the 4 threads.
            Dim TopThreadDelegate As New MIL_THREAD_FUNCTION_PTR(AddressOf TopThread)
            Dim BotLeftThreadDelegate As New MIL_THREAD_FUNCTION_PTR(AddressOf BotLeftThread)
            Dim BotRightThreadDelegate As New MIL_THREAD_FUNCTION_PTR(AddressOf BotRightThread)

            Dim TParTopLeftHandle As GCHandle = GCHandle.Alloc(TParTopLeft)
            Dim TParBotLeftHandle As GCHandle = GCHandle.Alloc(TParBotLeft)
            Dim TParTopRightHandle As GCHandle = GCHandle.Alloc(TParTopRight)
            Dim TParBotRightHandle As GCHandle = GCHandle.Alloc(TParBotRight)

            MIL.MthrAlloc(MilSystem, MIL.M_THREAD, MIL.M_DEFAULT, TopThreadDelegate, CType(TParTopLeftHandle, IntPtr), TParTopLeft.Id)
            MIL.MthrAlloc(MilSystem, MIL.M_THREAD, MIL.M_DEFAULT, BotLeftThreadDelegate, CType(TParBotLeftHandle, IntPtr), TParBotLeft.Id)
            MIL.MthrAlloc(MilSystem, MIL.M_THREAD, MIL.M_DEFAULT, TopThreadDelegate, CType(TParTopRightHandle, IntPtr), TParTopRight.Id)
            MIL.MthrAlloc(MilSystem, MIL.M_THREAD, MIL.M_DEFAULT, BotRightThreadDelegate, CType(TParBotRightHandle, IntPtr), TParBotRight.Id)

            ' Start the timer.
            MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_RESET + MIL.M_SYNCHRONOUS, CType(MIL.M_NULL, IntPtr))

            ' Set events to start operation of top left and top right threads.
            MIL.MthrControl(TParTopLeft.ReadyEvent, MIL.M_EVENT_SET, MIL.M_SIGNALED)
            MIL.MthrControl(TParTopRight.ReadyEvent, MIL.M_EVENT_SET, MIL.M_SIGNALED)

            ' Report that the threads are started and wait for a key press to stop them.
            Console.Write(Constants.vbLf + "MULTI-THREADING:" + Constants.vbLf)
            Console.Write("----------------" + Constants.vbLf + Constants.vbLf)
            Console.Write("4 threads running..." + Constants.vbLf)
            Console.Write("Press <Enter> to stop." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Signal the threads to exit.
            TParTopLeft.Exit = 1
            TParTopRight.Exit = 1

            ' Wait for all threads to terminate.
            MIL.MthrWait(TParTopLeft.Id, MIL.M_THREAD_END_WAIT)
            MIL.MthrWait(TParBotLeft.Id, MIL.M_THREAD_END_WAIT)
            MIL.MthrWait(TParTopRight.Id, MIL.M_THREAD_END_WAIT)
            MIL.MthrWait(TParBotRight.Id, MIL.M_THREAD_END_WAIT)

            ' Stop the timer and calculate the number of frames per second processed.
            MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ + MIL.M_SYNCHRONOUS, Time)
            FramesPerSecond = (TParTopLeft.NumberOfIteration + TParBotLeft.NumberOfIteration + TParTopRight.NumberOfIteration + TParBotRight.NumberOfIteration) / Time

            ' Print statistics.
            Console.Write("Top left iterations done:     {0,4}." + Constants.vbLf, TParTopLeft.NumberOfIteration)
            Console.Write("Bottom left iterations done:  {0,4}." + Constants.vbLf, TParBotLeft.NumberOfIteration)
            Console.Write("Top right iterations done:    {0,4}." + Constants.vbLf, TParTopRight.NumberOfIteration)
            Console.Write("Bottom right iterations done: {0,4}." + Constants.vbLf + Constants.vbLf, TParBotRight.NumberOfIteration)
            Console.Write("Processing speed for the 4 threads: {0:0.0} Images/Sec." + Constants.vbLf + Constants.vbLf, FramesPerSecond)
            Console.Write("Press <Enter> to end." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Free threads.
            MIL.MthrFree(TParTopLeft.Id)
            MIL.MthrFree(TParBotLeft.Id)
            MIL.MthrFree(TParTopRight.Id)
            MIL.MthrFree(TParBotRight.Id)

            ' Free events.
            MIL.MthrFree(TParTopLeft.DoneEvent)
            MIL.MthrFree(TParBotLeft.DoneEvent)
            MIL.MthrFree(TParTopRight.DoneEvent)
            MIL.MthrFree(TParBotRight.DoneEvent)

            ' Free buffers.
            MIL.MbufFree(TParTopLeft.SrcImage)
            MIL.MbufFree(TParTopRight.SrcImage)
            MIL.MbufFree(TParBotLeft.DstImage)
            MIL.MbufFree(TParBotRight.DstImage)
            MIL.MbufFree(MilOrgImage)
            MIL.MbufFree(MilImage)

            ' Free the GCHandles
            TParTopLeftHandle.Free()
            TParBotLeftHandle.Free()
            TParTopRightHandle.Free()
            TParBotRightHandle.Free()

            ' Free defaults.
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL)
        End Sub

        ' Top left and top right threads' function (Add an offset):
        ' ---------------------------------------------------------
        Private Shared Function TopThread(ByVal ThreadParameters As IntPtr) As UInteger
            Dim TPar As THREAD_PARAM = CType((CType(ThreadParameters, GCHandle)).Target, THREAD_PARAM)

            Do While TPar.Exit = 0
                ' Wait for bottom ready event before proceeding.
                MIL.MthrWait(TPar.ReadyEvent, MIL.M_EVENT_WAIT)

                ' For better visual effect, reset SrcImage to the original image regularly.
                If (TPar.NumberOfIteration Mod 192) = 0 Then
                    MIL.MbufCopy(TPar.OrgImage, TPar.SrcImage)
                End If

                If (TPar.LicenseModules And MIL.M_LICENSE_IM) <> 0 Then
                    ' Add a constant to the image.
                    MIL.MimArith(TPar.SrcImage, 1L, TPar.DstImage, MIL.M_ADD_CONST + MIL.M_SATURATION)
                Else
                    ' Under Mil lite draw a variable size rectangle in the image.
                    TPar.SlaveThreadParam.Radius = (TPar.NumberOfIteration Mod DRAW_RADIUS_NUMBER) * DRAW_RADIUS_STEP
                    TPar.Radius = TPar.SlaveThreadParam.Radius
                    MIL.MgraColor(MIL.M_DEFAULT, &HFF)
                    MIL.MgraRectFill(MIL.M_DEFAULT, TPar.DstImage, DRAW_CENTER_POSX - TPar.Radius, DRAW_CENTER_POSY - TPar.Radius, DRAW_CENTER_POSX + TPar.Radius, DRAW_CENTER_POSY + TPar.Radius)
                End If

                ' Increment iteration count and draw text.
                TPar.NumberOfIteration += 1
                MIL.MgraColor(MIL.M_DEFAULT, &HFF)
                MIL.MgraText(MIL.M_DEFAULT, TPar.DstImage, STRING_POS_X, STRING_POS_Y, String.Format("{0}", TPar.NumberOfIteration))

                ' Signal to the bottom thread that the first part of processing is completed.
                MIL.MthrControl(TPar.DoneEvent, MIL.M_EVENT_SET, MIL.M_SIGNALED)

                ' For a smoother display effect, optional yield to the other threads.
                BalanceThreadScheduling()
            Loop

            ' Require the bottom thread to exit.
            TPar.SlaveThreadParam.Exit = 1

            ' Signal the bottom thread end to wake up.
            MIL.MthrControl(TPar.DoneEvent, MIL.M_EVENT_SET, MIL.M_SIGNALED)

            ' before exiting the thread, make sure all the commands are executed
         MIL.MthrWait(TPar.System, MIL.M_THREAD_WAIT, CType(MIL.M_NULL, IntPtr))

            Return 1
        End Function


        ' Bottom-left thread function (Rotate):
        ' -------------------------------------
        Private Shared Function BotLeftThread(ByVal ThreadParameters As IntPtr) As UInteger
            Dim TPar As THREAD_PARAM = CType((CType(ThreadParameters, GCHandle)).Target, THREAD_PARAM)

            Do While TPar.Exit = 0
                ' Wait for the event in top-left function to be ready before proceeding.
                MIL.MthrWait(TPar.ReadyEvent, MIL.M_EVENT_WAIT)

                ' If processing available, add a constant to the image.
                If (TPar.LicenseModules And MIL.M_LICENSE_IM) <> 0 Then
                    MIL.MimRotate(TPar.SrcImage, TPar.DstImage, (TPar.NumberOfIteration * 5) Mod 360, MIL.M_DEFAULT, MIL.M_DEFAULT, MIL.M_DEFAULT, MIL.M_DEFAULT, MIL.M_NEAREST_NEIGHBOR + MIL.M_OVERSCAN_CLEAR)
                Else ' else copy the top image and draw a filled circle.
                    MIL.MbufCopy(TPar.SrcImage, TPar.DstImage)
                    MIL.MgraColor(MIL.M_DEFAULT, &H80)
                    MIL.MgraArcFill(MIL.M_DEFAULT, TPar.DstImage, DRAW_CENTER_POSX, DRAW_CENTER_POSY, TPar.Radius, TPar.Radius, 0, 360)
                End If

                ' Print iteration count and draw a circle.
                TPar.NumberOfIteration += 1
                MIL.MgraColor(MIL.M_DEFAULT, &HFF)
                MIL.MgraText(MIL.M_DEFAULT, TPar.DstImage, STRING_POS_X, STRING_POS_Y, String.Format("{0}", TPar.NumberOfIteration))

                ' Signal to the top thread that the last part of the processing is completed.
                MIL.MthrControl(TPar.DoneEvent, MIL.M_EVENT_SET, MIL.M_SIGNALED)

                ' For a smoother display effect, optional yield to the other threads.
                BalanceThreadScheduling()
            Loop

            ' before exiting the thread, make sure all the commands are executed
         MIL.MthrWait(TPar.System, MIL.M_THREAD_WAIT, CType(MIL.M_NULL, IntPtr))

            Return 1
        End Function

        ' Bottom-right thread function (Edge Detect):
        ' -------------------------------------------
        Private Shared Function BotRightThread(ByVal ThreadParameters As IntPtr) As UInteger
            Dim TPar As THREAD_PARAM = CType((CType(ThreadParameters, GCHandle)).Target, THREAD_PARAM)

            Do While TPar.Exit = 0
                ' Wait for the event in top-right function to be ready before proceeding.
                MIL.MthrWait(TPar.ReadyEvent, MIL.M_EVENT_WAIT)

                ' If processing available, add a constant to the image.
                If (TPar.LicenseModules And MIL.M_LICENSE_IM) <> 0 Then
                    MIL.MimConvolve(TPar.SrcImage, TPar.DstImage, MIL.M_EDGE_DETECT)
                Else ' else copy the top image and draw a filled circle.
                    MIL.MbufCopy(TPar.SrcImage, TPar.DstImage)
                    MIL.MgraColor(MIL.M_DEFAULT, &H40)
                    MIL.MgraArcFill(MIL.M_DEFAULT, TPar.DstImage, DRAW_CENTER_POSX, DRAW_CENTER_POSY, TPar.Radius / 2, TPar.Radius / 2, 0, 360)
                End If

                ' Increment iteration count and draw text.
                TPar.NumberOfIteration += 1
                MIL.MgraColor(MIL.M_DEFAULT, &HFF)
                MIL.MgraText(MIL.M_DEFAULT, TPar.DstImage, STRING_POS_X, STRING_POS_Y, String.Format("{0}", TPar.NumberOfIteration))

                ' Signal to the top thread that the last part of the processing is completed.
                MIL.MthrControl(TPar.DoneEvent, MIL.M_EVENT_SET, MIL.M_SIGNALED)

                ' For a smoother display effect, optional yield to the other threads.
                BalanceThreadScheduling()
            Loop

            ' before exiting the thread, make sure all the commands are executed
         MIL.MthrWait(TPar.System, MIL.M_THREAD_WAIT, CType(MIL.M_NULL, IntPtr))

            Return 1
        End Function

        ' Thread Scheduling Adjustment Handling
        ' -------------------------------------
        '
        ' This function is used to give a smoother display effect. It balances the number of
        ' iterations of each thread by yielding its time slice frequently.
        '
        ' NOTE: In some situations, Windows adjusts the priority or quantum value of threads.
        '       The intent of these adjustments is to improve system throughput and
        '       responsiveness. However, like any scheduling algorithms these adjustments are
        '       not perfect, and they might not be beneficial to all applications. The
        '       following function is introduced to minimize the impact of thread scheduling
        '       adjustment as performed by Windows. Refer to the "Inside Windows NT
        '       (Second Edition, Microsoft Press, 1998, ISBN 1-57231-677-2)" manual for more
        '       details on thread scheduling under Windows.
        Private Shared Sub BalanceThreadScheduling()
            Thread.Sleep(0)
        End Sub

    End Class
End Namespace
