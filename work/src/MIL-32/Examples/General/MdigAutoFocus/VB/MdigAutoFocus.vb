'*****************************************************************************
'
' File name: MdigAutoFocus.vb
'
' Synopsis:  This program performs an autofocus operation using the 
'            MdigFocus() function. Since the way to move a motorized
'            camera lens is device-specific, we will not include real
'            lens movement control and image grab but will simulate 
'            the lens focus with a smooth operation. 
'
'     Note : Under MIL-Lite, the out of focus lens simulation is not supported.
'*****************************************************************************
Imports Microsoft.VisualBasic
Imports System
Imports System.Collections.Generic
Imports System.Text
Imports System.Runtime.InteropServices

Imports Matrox.MatroxImagingLibrary

Namespace MDigAutoFocus
    Class Program
        ' Source MIL image file specification.
        Private Const IMAGE_FILE As String = MIL.M_IMAGE_PATH & "BaboonMono.mim"

        ' Lens mechanical characteristics.
        Private Const FOCUS_MAX_NB_POSITIONS As Integer = 100
        Private Const FOCUS_MIN_POSITION As Integer = 0
        Private Const FOCUS_MAX_POSITION As Integer = (FOCUS_MAX_NB_POSITIONS - 1)
        Private Const FOCUS_START_POSITION As Integer = 10

        ' Autofocus search properties.
        Private Const FOCUS_MAX_POSITION_VARIATION As Integer = MIL.M_DEFAULT
        Private Const FOCUS_MODE As Integer = MIL.M_SMART_SCAN
        Private Const FOCUS_SENSITIVITY As Integer = 1

        ' User Data object definition.
        Public Class DigHookUserData
            Public SourceImage As MIL_ID
            Public FocusImage As MIL_ID
            Public Display As MIL_ID
            Public Iteration As Integer
        End Class

        Shared Sub Main(ByVal args() As String)
            Dim MilApplication As MIL_ID = MIL.M_NULL ' Application identifier.
            Dim MilSystem As MIL_ID = MIL.M_NULL ' System identifier.
            Dim MilDisplay As MIL_ID = MIL.M_NULL ' Display identifier.
            Dim MilSource As MIL_ID = MIL.M_NULL ' Source image.
            Dim MilCameraFocus As MIL_ID = MIL.M_NULL ' Focus simulated image.
            Dim FocusPos As MIL_INT = 0 ' Best focus position. 
            Dim UserData As DigHookUserData = New DigHookUserData() ' User data passed to the hook.

            ' Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, MilApplication, MilSystem, MilDisplay, CType(MIL.M_NULL, IntPtr), CType(MIL.M_NULL, IntPtr))

            ' Load the source image.
            MIL.MbufRestore(IMAGE_FILE, MilSystem, MilSource)
            MIL.MbufRestore(IMAGE_FILE, MilSystem, MilCameraFocus)
            MIL.MbufClear(MilCameraFocus, 0)

            ' Select image on the display.
            MIL.MdispSelect(MilDisplay, MilCameraFocus)

            ' Simulate the first image grab.
            SimulateGrabFromCamera(MilSource, MilCameraFocus, FOCUS_START_POSITION, MilDisplay)

            ' Initialize user data needed within the hook function.
            UserData.SourceImage = MilSource
            UserData.FocusImage = MilCameraFocus
            UserData.Iteration = 0
            UserData.Display = MilDisplay

            ' Pause to show the original image.
            Console.Write(Constants.vbLf + "AUTOFOCUS:" + Constants.vbLf)
            Console.Write("----------" + Constants.vbLf + Constants.vbLf)
            Console.Write("Automatic focusing operation will be done on this image." + Constants.vbLf)
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()
            Console.Write("Autofocusing..." + Constants.vbLf + Constants.vbLf)

            ' Perform Autofocus. 
            ' Since lens movement is hardware specific, no digitizer is used here.
            ' We simulate the lens movement with by smoothing the image data in 
            ' the hook function instead.

            ' get a handle to the DigHookUserData object in the managed heap, we will use this 
            ' handle to get the object back in the callback function
            Dim hUserData As GCHandle = GCHandle.Alloc(UserData)

            ' We give the handle to the DigHookUserData object by casting it to a IntPtr, 
            ' later in the hook handler we will receive our IntPtr back and cast it again 
            ' to a GCHandle to get a handle to our object
            MIL.MdigFocus(MIL.M_NULL, MilCameraFocus, MIL.M_DEFAULT, AddressOf MoveLensHookFunction, CType(hUserData, IntPtr), FOCUS_MIN_POSITION, FOCUS_START_POSITION, FOCUS_MAX_POSITION, FOCUS_MAX_POSITION_VARIATION, FOCUS_MODE + FOCUS_SENSITIVITY, FocusPos)

            ' Free the GCHandle when no longer used.
            hUserData.Free()

            ' Print the best focus position and number of iterations.
            Console.Write("The best focus position is {0}." + Constants.vbLf, FocusPos)
            Console.Write("The best focus position found in {0} iterations." + Constants.vbLf + Constants.vbLf, UserData.Iteration)
            Console.Write("Press <Enter> to end." + Constants.vbLf)
            Console.ReadKey()

            ' Free all allocations.
            MIL.MbufFree(MilSource)
            MIL.MbufFree(MilCameraFocus)
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL)
        End Sub

        '*******************************************************************************
        ' Autofocus hook function responsible to move the lens.
        Private Shared Function MoveLensHookFunction(ByVal HookType As MIL_INT, ByVal Position As MIL_INT, ByVal UserDataHookPtr As IntPtr) As MIL_INT
            ' this is how to check if the user data is null, the IntPtr class
            ' contains a member, Zero, which exists solely for this purpose
            If (Not IntPtr.Zero.Equals(UserDataHookPtr)) Then
                ' get the handle to the DigHookUserData object back from the IntPtr
                Dim hUserData As GCHandle = CType(UserDataHookPtr, GCHandle)

                ' get a reference to the DigHookUserData object
                Dim UserData As DigHookUserData = CType(hUserData.Target, DigHookUserData)

                ' Here, the lens position must be changed according to the Position parameter.
                ' In that case, we simulate the lens position change followed by a grab.
                If HookType = MIL.M_CHANGE OrElse HookType = MIL.M_ON_FOCUS Then
                    SimulateGrabFromCamera(UserData.SourceImage, UserData.FocusImage, CInt(Fix(Position)), UserData.Display)
                    UserData.Iteration += 1
                End If
            End If

            Return 0
        End Function

        '**********************************************************************************
        ' Utility function to simulate a grab from a camera at different lens position
        ' by smoothing the original image. It should be replaced with a true camera grab.
        '
        ' Note that this lens simulation will not work under MIL-lite because it uses
        ' MimConvolve().

        ' Lens simulation characteristics.
        Private Const FOCUS_BEST_POSITION As Double = (FOCUS_MAX_NB_POSITIONS / 2)

        Private Shared Sub SimulateGrabFromCamera(ByVal SourceImage As MIL_ID, ByVal FocusImage As MIL_ID, ByVal Iteration As Integer, ByVal AnnotationDisplay As MIL_ID)
            Dim NbSmoothNeeded As Integer = 0 ' Number of smooths needed.
            Dim BufType As MIL_INT = 0 ' Buffer type.
            Dim BufSizeX As MIL_INT = 0 ' Buffer size X.
            Dim BufSizeY As MIL_INT = 0 ' Buffer size Y.
            Dim Smooth As Integer = 0
            Dim TempBuffer As MIL_ID = MIL.M_NULL ' Temporary buffer.
            Dim SourceOwnerSystem As MIL_ID = MIL.M_NULL ' Owner system of the source buffer.

            ' Compute number of smooths needed to simulate focus.
            NbSmoothNeeded = CInt(Fix(Math.Abs(Iteration - FOCUS_BEST_POSITION)))

            ' Buffer inquires.
            BufType = MIL.MbufInquire(FocusImage, MIL.M_TYPE, MIL.M_NULL)
            BufSizeX = MIL.MbufInquire(FocusImage, MIL.M_SIZE_X, MIL.M_NULL)
            BufSizeY = MIL.MbufInquire(FocusImage, MIL.M_SIZE_Y, MIL.M_NULL)

            If NbSmoothNeeded = 0 Then
                ' Directly copy image source to destination.
                MIL.MbufCopy(SourceImage, FocusImage)
            ElseIf NbSmoothNeeded = 1 Then
                ' Directly convolve image from source to destination.
                MIL.MimConvolve(SourceImage, FocusImage, MIL.M_SMOOTH)
            Else
                SourceOwnerSystem = CType(MIL.MbufInquire(SourceImage, MIL.M_OWNER_SYSTEM, MIL.M_NULL), MIL_ID)

                ' Allocate temporary buffer.
                MIL.MbufAlloc2d(SourceOwnerSystem, BufSizeX, BufSizeY, BufType, MIL.M_IMAGE + MIL.M_PROC, TempBuffer)

                ' Perform first smooth.
                MIL.MimConvolve(SourceImage, TempBuffer, MIL.M_SMOOTH)

                ' Perform smooths.
                For Smooth = 1 To NbSmoothNeeded - 2
                    MIL.MimConvolve(TempBuffer, TempBuffer, MIL.M_SMOOTH)
                Next Smooth

                ' Perform last smooth.
                MIL.MimConvolve(TempBuffer, FocusImage, MIL.M_SMOOTH)

                ' Free temporary buffer.
                MIL.MbufFree(TempBuffer)
            End If

            ' Draw position cursor.
            DrawCursor(AnnotationDisplay, Iteration)
        End Sub

        '***************************************************************
        ' Draw position of the focus lens.

        ' Cursor specifications.
        Private Shared Function CURSOR_POSITION(ByVal BufSizeY As MIL_INT) As Double
            Return ((BufSizeY * 7) / 8)
        End Function

        Private Const CURSOR_SIZE As Integer = 14
        Private Shared ReadOnly CURSOR_COLOR As Integer = MIL.M_COLOR_GREEN

        Private Shared Sub DrawCursor(ByVal AnnotationDisplay As MIL_ID, ByVal Position As Integer)
            Dim AnnotationImage As MIL_ID = MIL.M_NULL
            Dim BufSizeX As MIL_INT = 0
            Dim BufSizeY As MIL_INT = 0
            Dim n As MIL_INT = 0

            ' Prepare for overlay annotations.
            MIL.MdispControl(AnnotationDisplay, MIL.M_OVERLAY, MIL.M_ENABLE)
            MIL.MdispControl(AnnotationDisplay, MIL.M_OVERLAY_CLEAR, MIL.M_DEFAULT)
            MIL.MdispInquire(AnnotationDisplay, MIL.M_OVERLAY_ID, AnnotationImage)
            MIL.MbufInquire(AnnotationImage, MIL.M_SIZE_X, BufSizeX)
            MIL.MbufInquire(AnnotationImage, MIL.M_SIZE_Y, BufSizeY)
            MIL.MgraColor(MIL.M_DEFAULT, CURSOR_COLOR)

            ' Write annotations.
            n = (BufSizeX / FOCUS_MAX_NB_POSITIONS)
            MIL.MgraLine(MIL.M_DEFAULT, AnnotationImage, 0, CURSOR_POSITION(BufSizeY) + CURSOR_SIZE, BufSizeX - 1, CURSOR_POSITION(BufSizeY) + CURSOR_SIZE)
            MIL.MgraLine(MIL.M_DEFAULT, AnnotationImage, Position * n, CURSOR_POSITION(BufSizeY) + CURSOR_SIZE, Position * n - CURSOR_SIZE, CURSOR_POSITION(BufSizeY))
            MIL.MgraLine(MIL.M_DEFAULT, AnnotationImage, Position * n, CURSOR_POSITION(BufSizeY) + CURSOR_SIZE, Position * n + CURSOR_SIZE, CURSOR_POSITION(BufSizeY))
            MIL.MgraLine(MIL.M_DEFAULT, AnnotationImage, Position * n - CURSOR_SIZE, CURSOR_POSITION(BufSizeY), Position * n + CURSOR_SIZE, CURSOR_POSITION(BufSizeY))
        End Sub
    End Class
End Namespace
