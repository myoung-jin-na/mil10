//*****************************************************************************
//
// File name: Form1.cs
//
// Synopsis:  This program displays a welcoming message in a user-defined
//            window and grabs (if supported) in it. It uses 
//            the MIL system and the MdispSelectWindow() function 
//            to display the MIL buffer in the client area of a windows form.
//*****************************************************************************
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using Matrox.MatroxImagingLibrary;

namespace MDispWindow
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

            // Set the window title.
            this.Text = MIL_APPLICATION_NAME + " - " + Process.GetCurrentProcess().MainModule.FileName;
        }

        // Window title.
        private const string MIL_APPLICATION_NAME = "MIL Application";

        // Default image dimensions.
        private const int DEFAULT_IMAGE_SIZE_X = 640;
        private const int DEFAULT_IMAGE_SIZE_Y = 480;
        private const int DEFAULT_IMAGE_SIZE_BAND = 1;

        // Background color of the window client area.
        private const int BACKCOLORRED = 160;
        private const int BACKCOLORGREEN = 160;
        private const int BACKCOLORBLUE = 164;

        //**************************************************************************
        //
        // Name:         MilApplication()
        //
        // Synopsis:     This function is the core of the MIL application that
        //               is executed when the "Start" menu item of this
        //               Windows forms program is selected. See main() in the 
        //               program.cs file for the program's entry point.
        //
        //               It uses MIL to display a welcoming message in the 
        //               specified user window and to grab in it (if it is supported) 
        //               using the target system.
        //
        //**************************************************************************
        private void MilApplication(IntPtr UserWindowHandle)
        {
            // MIL variables 
            MIL_ID MilApplication = MIL.M_NULL;  // MIL Application identifier.
            MIL_ID MilSystem = MIL.M_NULL;       // MIL System identifier.
            MIL_ID MilDisplay = MIL.M_NULL;      // MIL Display identifier.
            MIL_ID MilDigitizer = MIL.M_NULL;    // MIL Digitizer identifier.
            MIL_ID MilImage = MIL.M_NULL;        // MIL Image buffer identifier.
            MIL_INT BufSizeX = DEFAULT_IMAGE_SIZE_X;
            MIL_INT BufSizeY = DEFAULT_IMAGE_SIZE_Y;
            MIL_INT BufSizeBand = DEFAULT_IMAGE_SIZE_BAND;

            // Allocate a MIL application.
            MIL.MappAlloc(MIL.M_NULL, MIL.M_DEFAULT, ref MilApplication);

            // Allocate a MIL system.
            MIL.MsysAlloc(MIL.M_DEFAULT, "M_DEFAULT", MIL.M_DEFAULT, MIL.M_DEFAULT, ref MilSystem);

            // Allocate a MIL display.
            MIL.MdispAlloc(MilSystem, MIL.M_DEFAULT, "M_DEFAULT", MIL.M_WINDOWED, ref MilDisplay);

            // Allocate a MIL digitizer, if supported, and set the target image size.
            if (MIL.MsysInquire(MilSystem, MIL.M_DIGITIZER_NUM, MIL.M_NULL) > 0)
            {
                MIL.MdigAlloc(MilSystem, MIL.M_DEFAULT, "M_DEFAULT", MIL.M_DEFAULT, ref MilDigitizer);
                MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_X, ref BufSizeX);
                MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_Y, ref BufSizeY);
                MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_BAND, ref BufSizeBand);

                // Resize the display window
                if ((BufSizeX > DEFAULT_IMAGE_SIZE_X) || (BufSizeY > DEFAULT_IMAGE_SIZE_Y))
                {
                    FromHandle(UserWindowHandle).Size = new Size((int)BufSizeX, (int)BufSizeY);
                }
            }

            // Allocate a MIL buffer.
            long Attributes = MIL.M_IMAGE + MIL.M_DISP;
            if (MilDigitizer != MIL.M_NULL)
            {
                // Add M_GRAB attribute if a digitizer is allocated.
                Attributes |= MIL.M_GRAB;
            }
            MIL.MbufAllocColor(MilSystem, BufSizeBand, BufSizeX, BufSizeY, 8 + MIL.M_UNSIGNED, Attributes, ref MilImage);

            // Clear the buffer.
            MIL.MbufClear(MilImage, 0);

            // Select the MIL buffer to be displayed in the user-specified window.
            MIL.MdispSelectWindow(MilDisplay, MilImage, UserWindowHandle);

            // Print a string in the image buffer using MIL.
            // Note: When a MIL buffer is modified using a MIL command, the display 
            // automatically updates the window passed to MIL.MdispSelectWindow().

            MIL.MgraFont(MIL.M_DEFAULT, MIL.M_FONT_DEFAULT_LARGE);
            MIL.MgraText(MIL.M_DEFAULT, MilImage, (BufSizeX / 8) * 2, BufSizeY / 2, " Welcome to MIL !!! ");
            MIL.MgraRect(MIL.M_DEFAULT, MilImage, ((BufSizeX / 8) * 2) - 60, (BufSizeY / 2) - 80, ((BufSizeX / 8) * 2) + 370, (BufSizeY / 2) + 100);
            MIL.MgraRect(MIL.M_DEFAULT, MilImage, ((BufSizeX / 8) * 2) - 40, (BufSizeY / 2) - 60, ((BufSizeX / 8) * 2) + 350, (BufSizeY / 2) + 80);
            MIL.MgraRect(MIL.M_DEFAULT, MilImage, ((BufSizeX / 8) * 2) - 20, (BufSizeY / 2) - 40, ((BufSizeX / 8) * 2) + 330, (BufSizeY / 2) + 60);

            // Open a message box to wait for a key press.
            MessageBox.Show("\"Welcome to MIL !!!\" was printed", "MIL application example", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);

            // Grab in the user window if supported.
            if (MilDigitizer != MIL.M_NULL)
            {
                // Grab continuously.
                MIL.MdigGrabContinuous(MilDigitizer, MilImage);

                // Open a message box to wait for a key press.
                MessageBox.Show("Continuous grab in progress", "MIL application example", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);

                // Stop continuous grab.
                MIL.MdigHalt(MilDigitizer);
            }

            // Remove the MIL buffer from the display.
            MIL.MdispSelect(MilDisplay, MIL.M_NULL);

            // Free allocated objects.
            MIL.MbufFree(MilImage);

            if (MilDigitizer != MIL.M_NULL)
            {
                MIL.MdigFree(MilDigitizer);
            }

            MIL.MdispFree(MilDisplay);
            MIL.MsysFree(MilSystem);
            MIL.MappFree(MilApplication);
        }

        //**************************************************************************
        //
        // Name:         startMenuItem_Click()
        //
        // Synopsis:    This function hadles the main menu commands and start the 
        //              MIL application.
        //
        //**************************************************************************
        private void startMenuItem_Click(object sender, EventArgs e)
        {
            MilApplication(this.Handle);
        }
    }
}