using System;
using System.ComponentModel;
using System.Diagnostics;
using System.Drawing;
using System.Windows.Forms.Integration;

using Matrox.MatroxImagingLibrary;

namespace MILApplicationLibrary
{
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>
    /// An object that encapsulates the functionality of a simple MIL application.
    /// </summary>
    /// <remarks>
    /// <list type="bullet">
    /// <item>This object implements the <see cref="INotifyPropertyChanged"/> interface to allow data binding.</item>
    /// </list>
    /// </remarks>
    public class MILApplication : INotifyPropertyChanged
    {
        #region Constants

        // Default image dimensions.
        private const int DEFAULT_IMAGE_SIZE_X = 640;
        private const int DEFAULT_IMAGE_SIZE_Y = 480;
        private const int DEFAULT_IMAGE_SIZE_BAND = 1;

        #endregion

        #region Constructor

        ///////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>
        /// Creates a new unallocated MILApplication object.
        /// </summary>
        public MILApplication()
        {
            _appId = MIL.M_NULL;
            _sysId = MIL.M_NULL;
            _digId = MIL.M_NULL;
            _dispId = MIL.M_NULL;
            _bufId = MIL.M_NULL;
        }

        #endregion

        #region Public methods

        ///////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>
        /// Allocates a MIL application, system, display, image buffer and digitizer (if available).
        /// </summary>
        public void Allocate()
        {
            // Allocate a MIL application.
            MIL.MappAlloc(MIL.M_NULL, MIL.M_DEFAULT, ref _appId);

            // Allocate a MIL system.
            MIL.MsysAlloc(MIL.M_DEFAULT, "M_DEFAULT", MIL.M_DEFAULT, MIL.M_DEFAULT, ref _sysId);

            // Allocate a MIL display.
            MIL.MdispAlloc(_sysId, MIL.M_DEFAULT, "M_DEFAULT", MIL.M_WINDOWED, ref _dispId);

            // Set default values for the image buffer in case no digitizer can be allocated.
            MIL_INT bufferSizeX = DEFAULT_IMAGE_SIZE_X;
            MIL_INT bufferSizeY = DEFAULT_IMAGE_SIZE_Y;
            MIL_INT bufferSizeBand = DEFAULT_IMAGE_SIZE_BAND;
            long imageAttributes = MIL.M_IMAGE | MIL.M_DISP | MIL.M_PROC;

            // Inquire the number of digitizers for the system.
            MIL_INT numberOfDigitizers = MIL.MsysInquire(_sysId, MIL.M_DIGITIZER_NUM, MIL.M_NULL);
            if (numberOfDigitizers > 0)
            {
                // Allocate a digitizer.
                MIL.MdigAlloc(_sysId, MIL.M_DEFAULT, "M_DEFAULT", MIL.M_DEFAULT, ref _digId);

                // Inquire the digitizer to determine the image buffer size.
                bufferSizeBand = MIL.MdigInquire(_digId, MIL.M_SIZE_BAND, MIL.M_NULL);
                bufferSizeX = MIL.MdigInquire(_digId, MIL.M_SIZE_X, MIL.M_NULL);
                bufferSizeY = MIL.MdigInquire(_digId, MIL.M_SIZE_Y, MIL.M_NULL);

                // Add the M_GRAB attibute to the image buffer.
                imageAttributes |= MIL.M_GRAB;
            }

            // Notify the UI that grabbing options have changed.
            RaisePropertyChangedEvent("CanStartGrab");
            RaisePropertyChangedEvent("CanStopGrab");

            // Allocate the image buffer.
            MIL.MbufAllocColor(_sysId, bufferSizeBand, bufferSizeX, bufferSizeY, 8 + MIL.M_UNSIGNED, imageAttributes, ref _bufId);

            // Notify the UI that the buffer size changed.
            RaisePropertyChangedEvent("BufferSizeX");
            RaisePropertyChangedEvent("BufferSizeY");

            // Fill the buffer with a default message.
            FillImageBuffer(bufferSizeX, bufferSizeY);
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>
        /// Sets the <see cref="WindowsFormsHost"/> control that will be used to host the MIL display.
        /// </summary>
        /// <param name="displayHost">A <see cref="WindowsFormsHost"/> control on which to display an image buffer.</param>
        /// <remarks>This method will select the image buffer to display in the <see cref="WindowsFormsHost"/> specified by <paramref name="displayHost"/>.</remarks>
        /// <exception cref="ArgumentNullException">Thrown when <paramref name="displayHost"/> is null.</exception>
        public void SetDisplayHost(WindowsFormsHost displayHost)
        {
            if (displayHost == null)
                throw new ArgumentNullException("displayHost", "Must be a valid object.");

            // Select the image buffer on the display.
            MIL.MdispSelectWPF(_dispId, _bufId, displayHost);

            // Initialize the overlay.  It won't be visible by default
            InitializeOverlay();
        }


        ///////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>
        /// Starts the grab on the digitizer.
        /// </summary>
        /// <remarks>
        /// This method is called from the StartGrab_Click method of the main window when the 
        /// user clicks the Stop Grab button in the UI.
        /// </remarks>
        public void StartGrab()
        {
            MIL.MdigGrabContinuous(_digId, _bufId);

            _isGrabbing = true;

            // Notify the UI that grabbing options have changed.
            RaisePropertyChangedEvent("CanStartGrab");
            RaisePropertyChangedEvent("CanStopGrab");
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>
        /// Stops the grab on the digitizer.
        /// </summary>
        /// <remarks>
        /// This method is called from the StopGrab_Click method of the main window when the 
        /// user clicks the Stop Grab button in the UI.
        /// </remarks>
        public void StopGrab()
        {
            MIL.MdigHalt(_digId);

            _isGrabbing = false;

            // Notify the UI that grabbing options have changed.
            RaisePropertyChangedEvent("CanStartGrab");
            RaisePropertyChangedEvent("CanStopGrab");
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>
        /// Frees the MIL Application and its associated resources.
        /// </summary>
        public void Free()
        {
            // Stop the grab if necessary.
            if (CanStopGrab)
            {
                StopGrab();
            }

            // Free the display.
            if (_dispId != MIL.M_NULL)
            {
                // If an image buffer is selected on the display, deselect it before freeing the display.
                MIL_ID selectedBufferId = (MIL_ID)MIL.MdispInquire(_dispId, MIL.M_SELECTED, MIL.M_NULL);
                if (selectedBufferId != MIL.M_NULL)
                {
                    MIL.MdispSelectWPF(_dispId, MIL.M_NULL, null);
                }

                MIL.MdispFree(_dispId);
                _dispId = MIL.M_NULL;
            }

            // Free the image buffer.
            if (_bufId != MIL.M_NULL)
            {
                MIL.MbufFree(_bufId);
                _bufId = MIL.M_NULL;
            }

            // Free the digitizer.
            if (_digId != MIL.M_NULL)
            {
                MIL.MdigFree(_digId);
                _digId = MIL.M_NULL;
            }

            // Free the system.
            if (_sysId != MIL.M_NULL)
            {
                MIL.MsysFree(_sysId);
                _sysId = MIL.M_NULL;
            }

            // Free the application.
            if (_appId != MIL.M_NULL)
            {
                MIL.MappFree(_appId);
                _appId = MIL.M_NULL;
            }

            // The object has been cleaned up.
            // This call removes the object from the finalization queue and 
            // prevent finalization code object from executing a second time.
            GC.SuppressFinalize(this);
        }

        #endregion

        #region Properties used in data bindings

        ///////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>
        /// Returns whether or not the application can start grabbing live images.
        /// </summary>
        /// <remarks>This property is bound to the IsEnabled property of the Start Grab button.</remarks>
        public bool CanStartGrab
        {
            get { return ((_digId != MIL.M_NULL) && (!_isGrabbing)); }
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>
        /// Returns whether or not the application can stop grabbing live images.
        /// </summary>
        /// <remarks>This property is bound to the IsEnabled property of the Stop Grab button.</remarks>
        public bool CanStopGrab
        {
            get { return ((_digId != MIL.M_NULL) && (_isGrabbing)); }
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>
        /// Returns the width of the displayed image buffer.
        /// </summary>
        /// <remarks>This property is bound to the Width property of the WindowsFormsHost control.</remarks>
        public int BufferSizeX
        {
            get { return GetBufferSize(MIL.M_SIZE_X); }
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>
        /// Returns the height of the displayed image buffer.
        /// </summary>
        /// <remarks>This property is bound to the Height property of the WindowsFormsHost control.</remarks>
        public int BufferSizeY
        {
            get { return GetBufferSize(MIL.M_SIZE_Y); }
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>
        /// Gets or set whether or not the display's overlay is visible.
        /// </summary>
        /// <remarks>This property is bound to the IsChecked property of the Checkbox control.</remarks>
        public bool OverlayVisible
        {
            get
            {
                bool overlayVisible = false;

                if (_dispId != MIL.M_NULL)
                {
                    MIL_INT overlayShow = 0;
                    MIL.MdispInquire(_dispId, MIL.M_OVERLAY_SHOW, ref overlayShow);
                    if (overlayShow == MIL.M_ENABLE)
                    {
                        overlayVisible = true;
                    }
                }

                return overlayVisible;
            }

            set
            {
                bool overlayVisible = value;

                if (_dispId != MIL.M_NULL)
                {
                    MIL_INT controlValue = MIL.M_DISABLE;
                    if (overlayVisible)
                    {
                        controlValue = MIL.M_ENABLE;
                        if (!_overlayInitialized)
                        {
                            InitializeOverlay();
                        }
                    }

                    MIL.MdispControl(_dispId, MIL.M_OVERLAY_SHOW, controlValue);
                }
            }
        }

        #endregion

        #region INotifyPropertyChanged Members

        public event PropertyChangedEventHandler PropertyChanged;

        #endregion

        #region Helper functions

        ///////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>
        /// Returns the specified size of the displayed image buffer.
        /// </summary>
        /// <param name="inquireType">The size can be M_SIZE_X or M_SIZE_Y</param>
        /// <returns>Returns the inquired size, or 0 if the buffer is not allocated.</returns>
        private int GetBufferSize(long inquireType)
        {
            int bufferSize = 0;

            if (_bufId != MIL.M_NULL)
            {
                bufferSize = (int)MIL.MbufInquire(_bufId, inquireType, MIL.M_NULL);
            }

            return bufferSize;
        }

        ///////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>
        /// Fills the image buffer using MIL drawing.
        /// </summary>
        /// <param name="bufferSizeX">The width of the image buffer.</param>
        /// <param name="bufferSizeY">The height of the image buffer.</param>
        private void FillImageBuffer(MIL_INT bufferSizeX, MIL_INT bufferSizeY)
        {
            // Start by clearing the buffer
            MIL.MbufClear(_bufId, MIL.M_RGB888(0, 0, 0));

            // Fill the buffer with default content.
            MIL_INT defaultGraFont = MIL.MgraInquire(MIL.M_DEFAULT, MIL.M_FONT, MIL.M_NULL);
            MIL.MgraFont(MIL.M_DEFAULT, MIL.M_FONT_DEFAULT_LARGE);
            MIL.MgraText(MIL.M_DEFAULT, _bufId, ((bufferSizeX / 8) * 2), bufferSizeY / 2, " Welcome to MIL !!! ");
            MIL.MgraRect(MIL.M_DEFAULT, _bufId, ((bufferSizeX / 8) * 2) - 60, (bufferSizeY / 2) - 80, ((bufferSizeX / 8) * 2) + 370, (bufferSizeY / 2) + 100);
            MIL.MgraRect(MIL.M_DEFAULT, _bufId, ((bufferSizeX / 8) * 2) - 40, (bufferSizeY / 2) - 60, ((bufferSizeX / 8) * 2) + 350, (bufferSizeY / 2) + 80);
            MIL.MgraRect(MIL.M_DEFAULT, _bufId, ((bufferSizeX / 8) * 2) - 20, (bufferSizeY / 2) - 40, ((bufferSizeX / 8) * 2) + 330, (bufferSizeY / 2) + 60);
            MIL.MgraFont(MIL.M_DEFAULT, defaultGraFont);
        }

        ///////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>
        /// Prepares annotations to be shown in the display's overlay.
        /// </summary>
        private void InitializeOverlay()
        {
            MIL_ID defaultGraphicContext = MIL.M_DEFAULT;
            MIL_ID milOverlayImage = MIL.M_NULL;
            MIL_INT imageWidth, imageHeight;
            IntPtr hCustomDC = IntPtr.Zero;

            // Prepare overlay buffer.
            //***************************

            // Enable the display for overlay annotations.
            MIL.MdispControl(_dispId, MIL.M_OVERLAY, MIL.M_ENABLE);

            // Inquire the overlay buffer associated with the display.
            MIL.MdispInquire(_dispId, MIL.M_OVERLAY_ID, ref milOverlayImage);

            // Clear the overlay to transparent.
            MIL.MdispControl(_dispId, MIL.M_OVERLAY_CLEAR, MIL.M_DEFAULT);

            // Disable the overlay display update to accelerate annotations.
            MIL.MdispControl(_dispId, MIL.M_OVERLAY_SHOW, MIL.M_DISABLE);

            // Inquire overlay size.
            imageWidth = MIL.MbufInquire(milOverlayImage, MIL.M_SIZE_X, MIL.M_NULL);
            imageHeight = MIL.MbufInquire(milOverlayImage, MIL.M_SIZE_Y, MIL.M_NULL);

            // Draw MIL overlay annotations.
            //*********************************

            // Set the graphic text background to transparent.
            MIL.MgraControl(defaultGraphicContext, MIL.M_BACKGROUND_MODE, MIL.M_TRANSPARENT);

            // Print a white string in the overlay image buffer.
            MIL.MgraColor(defaultGraphicContext, MIL.M_COLOR_WHITE);
            MIL.MgraText(defaultGraphicContext, milOverlayImage, imageWidth / 9, imageHeight / 5, " -------------------- ");
            MIL.MgraText(defaultGraphicContext, milOverlayImage, imageWidth / 9, imageHeight / 5 + 25, " - MIL Overlay Text - ");
            MIL.MgraText(defaultGraphicContext, milOverlayImage, imageWidth / 9, imageHeight / 5 + 50, " -------------------- ");

            // Print a green string in the overlay image buffer.
            MIL.MgraColor(defaultGraphicContext, MIL.M_COLOR_GREEN);
            MIL.MgraText(defaultGraphicContext, milOverlayImage, imageWidth * 11 / 18, imageHeight / 5, " ---------------------");
            MIL.MgraText(defaultGraphicContext, milOverlayImage, imageWidth * 11 / 18, imageHeight / 5 + 25, " - MIL Overlay Text - ");
            MIL.MgraText(defaultGraphicContext, milOverlayImage, imageWidth * 11 / 18, imageHeight / 5 + 50, " ---------------------");

            // Draw GDI color overlay annotation.
            //***********************************

            // The next control might not be supported
            MIL.MappControl(MIL.M_DEFAULT, MIL.M_ERROR, MIL.M_PRINT_DISABLE);

            // Create a device context to draw in the overlay buffer with GDI.
            MIL.MbufControl(milOverlayImage, MIL.M_DC_ALLOC, MIL.M_DEFAULT);

            // Inquire the device context.
            hCustomDC = (IntPtr)MIL.MbufInquire(milOverlayImage, MIL.M_DC_HANDLE, MIL.M_NULL);

            MIL.MappControl(MIL.M_DEFAULT, MIL.M_ERROR, MIL.M_PRINT_ENABLE);

            // Perform operation if GDI drawing is supported.
            if (!hCustomDC.Equals(IntPtr.Zero))
            {
                // NOTE : The using blocks will automatically call the Dispose method on the GDI objects.
                //        This ensures that resources are freed even if an exception occurs.

                // Create a System.Drawing.Graphics object from the Device context
                using (Graphics DrawingGraphics = Graphics.FromHdc(hCustomDC))
                {
                    using (Pen DrawingPen = new Pen(Color.Blue))
                    {
                        // Draw a blue cross in the overlay image
                        DrawingGraphics.DrawLine(DrawingPen, 0, (int)(imageHeight / 2), imageWidth, (int)(imageHeight / 2));
                        DrawingGraphics.DrawLine(DrawingPen, (int)(imageWidth / 2), 0, (int)(imageWidth / 2), imageHeight);

                        // Prepare transparent text annotations.
                        // Define the Brushes and fonts used to draw text
                        using (SolidBrush LeftBrush = new SolidBrush(Color.Red))
                        {
                            using (SolidBrush RightBrush = new SolidBrush(Color.Yellow))
                            {
                                using (Font OverlayFont = new Font(FontFamily.GenericSansSerif, 10, FontStyle.Bold))
                                {
                                    // Write text in the overlay image
                                    SizeF GDITextSize = DrawingGraphics.MeasureString("GDI Overlay Text", OverlayFont);
                                    DrawingGraphics.DrawString("GDI Overlay Text", OverlayFont, LeftBrush, System.Convert.ToInt32(imageWidth / 4 - GDITextSize.Width / 2), System.Convert.ToInt32(imageHeight * 3 / 4 - GDITextSize.Height / 2));
                                    DrawingGraphics.DrawString("GDI Overlay Text", OverlayFont, RightBrush, System.Convert.ToInt32(imageWidth * 3 / 4 - GDITextSize.Width / 2), System.Convert.ToInt32(imageHeight * 3 / 4 - GDITextSize.Height / 2));
                                }
                            }
                        }
                    }
                }

                // Delete device context.
                MIL.MbufControl(milOverlayImage, MIL.M_DC_FREE, MIL.M_DEFAULT);

                // Signal MIL that the overlay buffer was modified.
                MIL.MbufControl(milOverlayImage, MIL.M_MODIFIED, MIL.M_DEFAULT);
            }

            _overlayInitialized = true;
        }

        ///////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>
        /// Raises the <see cref="PropertyChanged"/> event for the specified property.
        /// </summary>
        /// <param name="propertyName">A <see cref="string"/> object representing the name of the property that changed.</param>
        /// <remarks>Call this method to notify the UI that a property changed.</remarks>
        private void RaisePropertyChangedEvent(string propertyName)
        {
            // In debug builds, make sure the property exists in this object.
            VerifyPropertyName(propertyName);

            // Raise the PropertyChanged event.
            PropertyChangedEventHandler handler = PropertyChanged;
            if (handler != null)
            {
                handler(this, new PropertyChangedEventArgs(propertyName));
            }
        }

        ///////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>
        /// Warns the developer if this object does not have a public property with the specified name.
        /// </summary>
        /// <param name="propertyName">The name of the property to verify.</param>
        /// <remarks>This method does not exist in a Release build.</remarks>
        [Conditional("DEBUG")]
        [DebuggerStepThrough]
        public void VerifyPropertyName(string propertyName)
        {
            // Verify that the property name matches a real, 
            // public, instance property on this object.
            if (TypeDescriptor.GetProperties(this)[propertyName] == null)
            {
                Debug.Fail("Invalid property name: " + propertyName);
            }
        }

        #endregion

        #region Private members

        private MIL_ID _appId;
        private MIL_ID _sysId;
        private MIL_ID _digId;
        private MIL_ID _dispId;
        private MIL_ID _bufId;
        private bool _isGrabbing;
        private bool _overlayInitialized;

        #endregion
    }
}
