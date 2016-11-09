//*************************************************************************************
//
// File name: MdispOverlay.cs
//
// Synopsis:  This program shows how to display an image while creating
//            text and graphics annotations on top of it using MIL
//            graphic functions and windows GDI drawing under Windows.
//            If the target system supports grabbing, the annotations
//            are done on top of a continuous grab.
//*************************************************************************************
using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Runtime.InteropServices;

using Matrox.MatroxImagingLibrary;

namespace MDispOverlay
{
    class Program
    {
        // Target image.
        private const string IMAGE_FILE = MIL.M_IMAGE_PATH + "Board.mim";

        // Title for the display window.
        private const string WINDOW_TITLE = "Custom Title";

        static void Main(string[] args)
        {
            MIL_ID MilApplication = MIL.M_NULL;            // Application identifier.
            MIL_ID MilSystem = MIL.M_NULL;                 // System identifier.     
            MIL_ID MilDisplay = MIL.M_NULL;                // Display identifier.    
            MIL_ID MilDigitizer = MIL.M_NULL;            // Digitizer identifier.  
            MIL_ID MilImage = MIL.M_NULL;                  // Image identifier.      

            // Allocate defaults
            MIL.MappAllocDefault(MIL.M_DEFAULT, ref MilApplication, ref MilSystem, ref MilDisplay, MIL.M_NULL, MIL.M_NULL);

            // If the system have a digitizer, use it.
            if (MIL.MsysInquire(MilSystem, MIL.M_DIGITIZER_NUM, MIL.M_NULL) > 0)
            {
                MIL.MdigAlloc(MilSystem, MIL.M_DEFAULT, "M_DEFAULT", MIL.M_DEFAULT, ref MilDigitizer);
                MIL.MbufAllocColor(MilSystem,
                               MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_BAND, MIL.M_NULL),
                               MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_X, MIL.M_NULL),
                               MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_Y, MIL.M_NULL),
                               8 + MIL.M_UNSIGNED,
                               MIL.M_IMAGE + MIL.M_DISP + MIL.M_PROC + MIL.M_GRAB,
                               ref MilImage);
                MIL.MbufClear(MilImage, 0);
            }
            else
            {
                // Restore a static image.
                MIL.MbufRestore(IMAGE_FILE, MilSystem, ref MilImage);
            }

            // Change display window title.
            MIL.MdispControl(MilDisplay, MIL.M_TITLE, WINDOW_TITLE);

            // Display the image buffer.
            MIL.MdispSelect(MilDisplay, MilImage);

            // Draw text and graphics annotations in the display overlay.
            OverlayDraw(MilDisplay);

            // If the system supports it, grab continuously in the displayed image.
            if (MilDigitizer != MIL.M_NULL)
                MIL.MdigGrabContinuous(MilDigitizer, MilImage);

            // Pause to show the image.
            Console.Write("\nOVERLAY ANNOTATIONS:\n");
            Console.Write("--------------------\n\n");
            Console.Write("Displaying an image with overlay annotations.\n");
            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();

            // Stop the continuous grab and free digitizer if needed.
            if (MilDigitizer != MIL.M_NULL)
            {
                MIL.MdigHalt(MilDigitizer);
                MIL.MdigFree(MilDigitizer);

                // Pause to show the result.
                Console.Write("Displaying the last grabbed image.\n");
                Console.Write("Press <Enter> to end.\n\n");
                Console.ReadKey();
            }

            // Free image.
            MIL.MbufFree(MilImage);

            // Free default allocations.
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL);
        }

        // ---------------------------------------------------------------
        // Name:      OverlayDraw
        // Synopsis:  This function draws annotations in the display overlay.

        static void OverlayDraw(MIL_ID MilDisplay)
        {
            MIL_ID DefaultGraphicContext = MIL.M_DEFAULT;
            MIL_ID MilOverlayImage = MIL.M_NULL;
            MIL_INT ImageWidth, ImageHeight;
            IntPtr hCustomDC = IntPtr.Zero;

            // Prepare overlay buffer.
            //***************************

            // Enable the display of overlay annotations.
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY, MIL.M_ENABLE);

            // Inquire the overlay buffer associated with the display.
            MIL.MdispInquire(MilDisplay, MIL.M_OVERLAY_ID, ref MilOverlayImage);

            // Clear the overlay to transparent.
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY_CLEAR, MIL.M_DEFAULT);

            // Disable the overlay display update to accelerate annotations.
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY_SHOW, MIL.M_DISABLE);

            // Inquire overlay size.
            ImageWidth = MIL.MbufInquire(MilOverlayImage, MIL.M_SIZE_X, MIL.M_NULL);
            ImageHeight = MIL.MbufInquire(MilOverlayImage, MIL.M_SIZE_Y, MIL.M_NULL);

            // Draw MIL overlay annotations.
            //*********************************

            // Set the graphic text background to transparent.
            MIL.MgraControl(DefaultGraphicContext, MIL.M_BACKGROUND_MODE, MIL.M_TRANSPARENT);

            // Print a white string in the overlay image buffer.
            MIL.MgraColor(DefaultGraphicContext, MIL.M_COLOR_WHITE);
            MIL.MgraText(DefaultGraphicContext, MilOverlayImage, ImageWidth / 9, ImageHeight / 5, " -------------------- ");
            MIL.MgraText(DefaultGraphicContext, MilOverlayImage, ImageWidth / 9, ImageHeight / 5 + 25, " - MIL Overlay Text - ");
            MIL.MgraText(DefaultGraphicContext, MilOverlayImage, ImageWidth / 9, ImageHeight / 5 + 50, " -------------------- ");

            // Print a green string in the overlay image buffer.
            MIL.MgraColor(DefaultGraphicContext, MIL.M_COLOR_GREEN);
            MIL.MgraText(DefaultGraphicContext, MilOverlayImage, ImageWidth * 11 / 18, ImageHeight / 5, " ---------------------");
            MIL.MgraText(DefaultGraphicContext, MilOverlayImage, ImageWidth * 11 / 18, ImageHeight / 5 + 25, " - MIL Overlay Text - ");
            MIL.MgraText(DefaultGraphicContext, MilOverlayImage, ImageWidth * 11 / 18, ImageHeight / 5 + 50, " ---------------------");

            // Re-enable the overlay display after all annotations are done.
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY_SHOW, MIL.M_ENABLE);

            // Draw GDI color overlay annotation.
            //***********************************

            // The inquire might not be supported
            MIL.MappControl(MIL.M_DEFAULT, MIL.M_ERROR, MIL.M_PRINT_DISABLE);

            // Create a device context to draw in the overlay buffer with GDI.
            MIL.MbufControl(MilOverlayImage, MIL.M_DC_ALLOC, MIL.M_DEFAULT);

            // Inquire the device context.
            hCustomDC = (IntPtr)MIL.MbufInquire(MilOverlayImage, MIL.M_DC_HANDLE, MIL.M_NULL);

            MIL.MappControl(MIL.M_DEFAULT, MIL.M_ERROR, MIL.M_PRINT_ENABLE);

            // Perform operation if GDI drawing is supported.
            if (!hCustomDC.Equals(IntPtr.Zero))
            {
                // NOTE : The using blocks will automatically call the Dipose method on the GDI objects.
                //        This ensures that resources are freed even if an exception occurs.

                // Create a System.Drawing.Graphics object from the Device context
                using (Graphics DrawingGraphics = Graphics.FromHdc(hCustomDC))
                {
                    // Draw a blue cross.
                    using (Pen DrawingPen = new Pen(Color.Blue))
                    {
                        // Draw a blue cross in the overlay image
                        DrawingGraphics.DrawLine(DrawingPen, 0, (int)(ImageHeight / 2), ImageWidth, (int)(ImageHeight / 2));
                        DrawingGraphics.DrawLine(DrawingPen, (int)(ImageWidth / 2), 0, (int)(ImageWidth / 2), ImageHeight);

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
                                    DrawingGraphics.DrawString("GDI Overlay Text", OverlayFont, LeftBrush, System.Convert.ToInt32(ImageWidth / 4 - GDITextSize.Width / 2), System.Convert.ToInt32(ImageHeight * 3 / 4 - GDITextSize.Height / 2));
                                    DrawingGraphics.DrawString("GDI Overlay Text", OverlayFont, RightBrush, System.Convert.ToInt32(ImageWidth * 3 / 4 - GDITextSize.Width / 2), System.Convert.ToInt32(ImageHeight * 3 / 4 - GDITextSize.Height / 2));
                                }
                            }
                        }
                    }
                }

                //   // Delete device context.
                MIL.MbufControl(MilOverlayImage, MIL.M_DC_FREE, MIL.M_DEFAULT);

                //   // Signal MIL that the overlay buffer was modified.
                MIL.MbufControl(MilOverlayImage, MIL.M_MODIFIED, MIL.M_DEFAULT);
            }
        }
    }
}
