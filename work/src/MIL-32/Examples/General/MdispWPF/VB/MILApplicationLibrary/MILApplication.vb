Imports System
Imports System.ComponentModel
Imports System.Diagnostics
Imports System.Drawing
Imports System.Windows.Forms.Integration

Imports Matrox.MatroxImagingLibrary

Public Class MILApplication
    Implements INotifyPropertyChanged

#Region "Constants"

    ' Default image dimensions.
    Private Const DEFAULT_IMAGE_SIZE_X As Integer = 640
    Private Const DEFAULT_IMAGE_SIZE_Y As Integer = 480
    Private Const DEFAULT_IMAGE_SIZE_BAND As Integer = 1

#End Region

#Region "Constructor"

    ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
    ''' <summary>
    ''' Creates a new unallocated MILApplication object.
    ''' </summary>
    Public Sub New()
        _appId = MIL.M_NULL
        _sysId = MIL.M_NULL
        _digId = MIL.M_NULL
        _dispId = MIL.M_NULL
        _bufId = MIL.M_NULL
    End Sub

#End Region

#Region "Public methods"
    ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
    ''' <summary>
    ''' Allocates a MIL application, system, display, image buffer and digitizer (if available).
    ''' </summary>
    Public Sub Allocate()
        ' Allocate a MIL application.
        MIL.MappAlloc(MIL.M_NULL, MIL.M_DEFAULT, _appId)

        ' Allocate a MIL system.
        MIL.MsysAlloc(MIL.M_DEFAULT, "M_DEFAULT", MIL.M_DEFAULT, MIL.M_DEFAULT, _sysId)

        ' Allocate a MIL display.
        MIL.MdispAlloc(_sysId, MIL.M_DEFAULT, "M_DEFAULT", MIL.M_WINDOWED, _dispId)

        ' Set default values for the image buffer in case no digitizer can be allocated.
        Dim bufferSizeX As MIL_INT = DEFAULT_IMAGE_SIZE_X
        Dim bufferSizeY As MIL_INT = DEFAULT_IMAGE_SIZE_Y
        Dim bufferSizeBand As MIL_INT = DEFAULT_IMAGE_SIZE_BAND
        Dim imageAttributes As Long = MIL.M_IMAGE Or MIL.M_DISP Or MIL.M_PROC

        ' Inquire the number of digitizers for the system.
        Dim numberOfDigitizers As MIL_INT = MIL.MsysInquire(_sysId, MIL.M_DIGITIZER_NUM, MIL.M_NULL)
        If numberOfDigitizers > 0 Then
            ' Allocate a digitizer.
            MIL.MdigAlloc(_sysId, MIL.M_DEFAULT, "M_DEFAULT", MIL.M_DEFAULT, _digId)

            ' Inquire the digitizer to determine the image buffer size.
            bufferSizeBand = MIL.MdigInquire(_digId, MIL.M_SIZE_BAND, MIL.M_NULL)
            bufferSizeX = MIL.MdigInquire(_digId, MIL.M_SIZE_X, MIL.M_NULL)
            bufferSizeY = MIL.MdigInquire(_digId, MIL.M_SIZE_Y, MIL.M_NULL)

            ' Add the M_GRAB attibute to the image buffer.
            imageAttributes = imageAttributes Or MIL.M_GRAB
        End If

        ' Notify the UI that grabbing options have changed.
        RaisePropertyChangedEvent("CanStartGrab")
        RaisePropertyChangedEvent("CanStopGrab")

        ' Allocate the image buffer.
        MIL.MbufAllocColor(_sysId, bufferSizeBand, bufferSizeX, bufferSizeY, 8 + MIL.M_UNSIGNED, imageAttributes, _bufId)

        ' Notify the UI that the buffer size changed.
        RaisePropertyChangedEvent("BufferSizeX")
        RaisePropertyChangedEvent("BufferSizeY")

        ' Fill the buffer with a default message.
        FillImageBuffer(bufferSizeX, bufferSizeY)
    End Sub

    ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
    ''' <summary>
    ''' Sets the <see cref="WindowsFormsHost"/> control that will be used to host the MIL display.
    ''' </summary>
    ''' <param name="displayHost">A <see cref="WindowsFormsHost"/> control on which to display an image buffer.</param>
    ''' <remarks>This method will select the image buffer to display in the <see cref="WindowsFormsHost"/> specified by <paramref name="displayHost"/>.</remarks>
    ''' <exception cref="ArgumentNullException">Thrown when <paramref name="displayHost"/> is null.</exception>
    Public Sub SetDisplayHost(ByVal displayHost As WindowsFormsHost)
        If displayHost Is Nothing Then
            Throw New ArgumentNullException("displayHost", "Must be a valid object.")
        End If

        ' Select the image buffer on the display.
        MIL.MdispSelectWPF(_dispId, _bufId, displayHost)

        ' Initialize the overlay.  It won't be visible by default
        InitializeOverlay()
    End Sub


    ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
    ''' <summary>
    ''' Starts the grab on the digitizer.
    ''' </summary>
    ''' <remarks>
    ''' This method is called from the StartGrab_Click method of the main window when the 
    ''' user clicks the Stop Grab button in the UI.
    ''' </remarks>
    Public Sub StartGrab()
        MIL.MdigGrabContinuous(_digId, _bufId)

        _isGrabbing = True

        ' Notify the UI that grabbing options have changed.
        RaisePropertyChangedEvent("CanStartGrab")
        RaisePropertyChangedEvent("CanStopGrab")
    End Sub

    ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
    ''' <summary>
    ''' Stops the grab on the digitizer.
    ''' </summary>
    ''' <remarks>
    ''' This method is called from the StopGrab_Click method of the main window when the 
    ''' user clicks the Stop Grab button in the UI.
    ''' </remarks>
    Public Sub StopGrab()
        MIL.MdigHalt(_digId)

        _isGrabbing = False

        ' Notify the UI that grabbing options have changed.
        RaisePropertyChangedEvent("CanStartGrab")
        RaisePropertyChangedEvent("CanStopGrab")
    End Sub

    ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
    ''' <summary>
    ''' Frees the MIL Application and its associated resources.
    ''' </summary>
    Public Sub Free()
        ' Stop the grab if necessary.
        If CanStopGrab Then
            StopGrab()
        End If

        ' Free the display.
        If _dispId <> MIL.M_NULL Then
            ' If an image buffer is selected on the display, deselect it before freeing the display.
            Dim selectedBufferId As MIL_ID = CType(MIL.MdispInquire(_dispId, MIL.M_SELECTED, MIL.M_NULL), MIL_ID)
            If selectedBufferId <> MIL.M_NULL Then
                MIL.MdispSelectWPF(_dispId, MIL.M_NULL, Nothing)
            End If

            MIL.MdispFree(_dispId)
            _dispId = MIL.M_NULL
        End If

        ' Free the image buffer.
        If _bufId <> MIL.M_NULL Then
            MIL.MbufFree(_bufId)
            _bufId = MIL.M_NULL
        End If

        ' Free the digitizer.
        If _digId <> MIL.M_NULL Then
            MIL.MdigFree(_digId)
            _digId = MIL.M_NULL
        End If

        ' Free the system.
        If _sysId <> MIL.M_NULL Then
            MIL.MsysFree(_sysId)
            _sysId = MIL.M_NULL
        End If

        ' Free the application.
        If _appId <> MIL.M_NULL Then
            MIL.MappFree(_appId)
            _appId = MIL.M_NULL
        End If

        ' The object has been cleaned up.
        ' This call removes the object from the finalization queue and 
        ' prevent finalization code object from executing a second time.
        GC.SuppressFinalize(Me)
    End Sub
#End Region

#Region "Properties used in data bindings"

    ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
    ''' <summary>
    ''' Returns whether or not the application can start grabbing live images.
    ''' </summary>
    ''' <remarks>This property is bound to the IsEnabled property of the Start Grab button.</remarks>
    Public ReadOnly Property CanStartGrab() As Boolean
        Get
            Return ((_digId <> MIL.M_NULL) And (Not _isGrabbing))
        End Get
    End Property

    ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
    ''' <summary>
    ''' Returns whether or not the application can stop grabbing live images.
    ''' </summary>
    ''' <remarks>This property is bound to the IsEnabled property of the Stop Grab button.</remarks>
    Public ReadOnly Property CanStopGrab() As Boolean
        Get
            Return ((_digId <> MIL.M_NULL) And _isGrabbing)
        End Get
    End Property

    ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
    ''' <summary>
    ''' Returns the width of the displayed image buffer.
    ''' </summary>
    ''' <remarks>This property is bound to the Width property of the WindowsFormsHost control.</remarks>
    Public ReadOnly Property BufferSizeX() As Integer
        Get
            Return GetBufferSize(MIL.M_SIZE_X)
        End Get
    End Property

    ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
    ''' <summary>
    ''' Returns the height of the displayed image buffer.
    ''' </summary>
    ''' <remarks>This property is bound to the Height property of the WindowsFormsHost control.</remarks>
    Public ReadOnly Property BufferSizeY() As Integer
        Get
            Return GetBufferSize(MIL.M_SIZE_Y)
        End Get
    End Property

    ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
    ''' <summary>
    ''' Gets or set whether or not the display's overlay is visible.
    ''' </summary>
    ''' <remarks>This property is bound to the IsChecked property of the Checkbox control.</remarks>
    Public Property OverlayVisible() As Boolean
        Get
            Dim overlayVisibleValue As Boolean = False

            If _dispId <> MIL.M_NULL Then
                Dim overlayShow As MIL_INT = 0
                MIL.MdispInquire(_dispId, MIL.M_OVERLAY_SHOW, overlayShow)
                If overlayShow = MIL.M_ENABLE Then
                    overlayVisibleValue = True
                End If
            End If

            Return overlayVisibleValue
        End Get

        Set(ByVal value As Boolean)
            Dim overlayVisibleValue As Boolean = value

            If _dispId <> MIL.M_NULL Then
                Dim controlValue As MIL_INT = MIL.M_DISABLE
                If overlayVisibleValue Then
                    If (Not _overlayInitialized) Then
                        InitializeOverlay()
                    End If
                    controlValue = MIL.M_ENABLE
                End If

                MIL.MdispControl(_dispId, MIL.M_OVERLAY_SHOW, controlValue)
            End If
        End Set
    End Property

#End Region

#Region "INotifyPropertyChanged Members"

    Public Event PropertyChanged(ByVal sender As Object, ByVal e As PropertyChangedEventArgs) Implements INotifyPropertyChanged.PropertyChanged

#End Region

#Region "Helper functions"
    ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
    ''' <summary>
    ''' Returns the specified size of the displayed image buffer.
    ''' </summary>
    ''' <param name="inquireType">The size can be M_SIZE_X or M_SIZE_Y</param>
    ''' <returns>Returns the inquired size, or 0 if the buffer is not allocated.</returns>
    Private Function GetBufferSize(ByVal inquireType As Long) As Integer
        Dim bufferSize As Integer = 0

        If _bufId <> MIL.M_NULL Then
            bufferSize = CInt(Fix(MIL.MbufInquire(_bufId, inquireType, MIL.M_NULL)))
        End If

        Return bufferSize
    End Function

    ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
    ''' <summary>
    ''' Fills the image buffer using MIL drawing.
    ''' </summary>
    ''' <param name="bufferSizeX">The width of the image buffer.</param>
    ''' <param name="bufferSizeY">The height of the image buffer.</param>
    Private Sub FillImageBuffer(ByVal bufferSizeX As MIL_INT, ByVal bufferSizeY As MIL_INT)
        ' Start by clearing the buffer
        MIL.MbufClear(_bufId, MIL.M_RGB888(0, 0, 0))

        ' Fill the buffer with default content.
        Dim defaultGraFont As MIL_INT = MIL.MgraInquire(MIL.M_DEFAULT, MIL.M_FONT, MIL.M_NULL)
        MIL.MgraFont(MIL.M_DEFAULT, MIL.M_FONT_DEFAULT_LARGE)
        MIL.MgraText(MIL.M_DEFAULT, _bufId, ((bufferSizeX / 8) * 2), bufferSizeY / 2, " Welcome to MIL !!! ")
        MIL.MgraRect(MIL.M_DEFAULT, _bufId, ((bufferSizeX / 8) * 2) - 60, (bufferSizeY / 2) - 80, ((bufferSizeX / 8) * 2) + 370, (bufferSizeY / 2) + 100)
        MIL.MgraRect(MIL.M_DEFAULT, _bufId, ((bufferSizeX / 8) * 2) - 40, (bufferSizeY / 2) - 60, ((bufferSizeX / 8) * 2) + 350, (bufferSizeY / 2) + 80)
        MIL.MgraRect(MIL.M_DEFAULT, _bufId, ((bufferSizeX / 8) * 2) - 20, (bufferSizeY / 2) - 40, ((bufferSizeX / 8) * 2) + 330, (bufferSizeY / 2) + 60)
        MIL.MgraFont(MIL.M_DEFAULT, defaultGraFont)
    End Sub

    ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
    ''' <summary>
    ''' Prepares annotations to be shown in the display's overlay.
    ''' </summary>
    Private Sub InitializeOverlay()
        Dim DefaultGraphicContext As MIL_ID = MIL.M_DEFAULT
        Dim MilOverlayImage As MIL_ID = MIL.M_NULL
        Dim ImageWidth, ImageHeight As MIL_INT
        Dim hCustomDC As IntPtr = IntPtr.Zero

        ' Prepare overlay buffer.
        '***************************

        ' Enable the display for overlay annotations.
        MIL.MdispControl(_dispId, MIL.M_OVERLAY, MIL.M_ENABLE)

        ' Inquire the overlay buffer associated with the display.
        MIL.MdispInquire(_dispId, MIL.M_OVERLAY_ID, MilOverlayImage)

        ' Clear the overlay to transparent.
        MIL.MdispControl(_dispId, MIL.M_OVERLAY_CLEAR, MIL.M_DEFAULT)

        ' Disable the overlay display update to accelerate annotations.
        MIL.MdispControl(_dispId, MIL.M_OVERLAY_SHOW, MIL.M_DISABLE)

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

        ' Draw GDI color overlay annotation.
        '***********************************

        ' The next control might not be supported
        MIL.MappControl(MIL.M_DEFAULT, MIL.M_ERROR, MIL.M_PRINT_DISABLE)

        ' Create a device context to draw in the overlay buffer with GDI.
        MIL.MbufControl(MilOverlayImage, MIL.M_DC_ALLOC, MIL.M_DEFAULT)

        ' Inquire the device context.
        hCustomDC = CType(MIL.MbufInquire(MilOverlayImage, MIL.M_DC_HANDLE, MIL.M_NULL), IntPtr)

        MIL.MappControl(MIL.M_DEFAULT, MIL.M_ERROR, MIL.M_PRINT_ENABLE)

        ' Perform operation if GDI drawing is supported.
        If (Not hCustomDC.Equals(IntPtr.Zero)) Then
            ' NOTE : The using blocks will automatically call the Dispose method on the GDI objects.
            '        This ensures that resources are freed even if an exception occurs.

            ' Create a System.Drawing.Graphics object from the Device context
            Using DrawingGraphics As Graphics = Graphics.FromHdc(hCustomDC)
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
                                DrawingGraphics.DrawString("GDI Overlay Text", OverlayFont, LeftBrush, System.Convert.ToInt32(ImageWidth / 4 - GDITextSize.Width / 2), System.Convert.ToInt32(ImageHeight * 3 / 4 - GDITextSize.Height / 2))
                                DrawingGraphics.DrawString("GDI Overlay Text", OverlayFont, RightBrush, System.Convert.ToInt32(ImageWidth * 3 / 4 - GDITextSize.Width / 2), System.Convert.ToInt32(ImageHeight * 3 / 4 - GDITextSize.Height / 2))
                            End Using
                        End Using
                    End Using
                End Using
            End Using

            ' Delete device context.
            MIL.MbufControl(MilOverlayImage, MIL.M_DC_FREE, MIL.M_DEFAULT)

            ' Signal MIL that the overlay buffer was modified.
            MIL.MbufControl(MilOverlayImage, MIL.M_MODIFIED, MIL.M_DEFAULT)
        End If
    End Sub

    ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
    ''' <summary>
    ''' Raises the <see cref="PropertyChanged"/> event for the specified property.
    ''' </summary>
    ''' <param name="propertyName">A <see cref="string"/> object representing the name of the property that changed.</param>
    ''' <remarks>Call this method to notify the UI that a property changed.</remarks>
    Private Sub RaisePropertyChangedEvent(ByVal propertyName As String)
        ' In debug builds, make sure the property exists in this object.
        VerifyPropertyName(propertyName)

        ' Raise the PropertyChanged event.
        RaiseEvent PropertyChanged(Me, New PropertyChangedEventArgs(propertyName))
    End Sub

    ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
    ''' <summary>
    ''' Warns the developer if this object does not have a public property with the specified name.
    ''' </summary>
    ''' <param name="propertyName">The name of the property to verify.</param>
    ''' <remarks>This method does not exist in a Release build.</remarks>
    <Conditional("DEBUG"), DebuggerStepThrough()> _
    Public Sub VerifyPropertyName(ByVal propertyName As String)
        ' Verify that the property name matches a real,
        ' public, instance property on this object.
        If TypeDescriptor.GetProperties(Me)(propertyName) Is Nothing Then
            Debug.Fail("Invalid property name: " & propertyName)
        End If
    End Sub
#End Region

#Region "Private members"

    Private _appId As MIL_ID
    Private _sysId As MIL_ID
    Private _digId As MIL_ID
    Private _dispId As MIL_ID
    Private _bufId As MIL_ID
    Private _isGrabbing As Boolean
    Private _overlayInitialized As Boolean

#End Region

End Class
