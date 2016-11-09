//*****************************************************************************
//
// File name: MImLocatePeak1d.cs
//
// Synopsis:  This program finds the peak in each column of an input sequence
//            and reconstruct the height of a 3d object using it.
//*****************************************************************************
using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

using Matrox.MatroxImagingLibrary;

namespace MImLocatePeak1d
{
    class Program
    {
        // Input sequence specifications.
        private const string SEQUENCE_FILE = MIL.M_IMAGE_PATH + "HandWithLaser.avi";

        // Peak detection parameters.
        private const int MAX_LINE_WIDTH = 10;
        private const int MIN_INTENSITY = 100;
        private const int NB_FIXED_POINT = 4;

        // D3D display parameters
        private static readonly int D3D_DISPLAY_SIZE_X = 640;
        private static readonly int D3D_DISPLAY_SIZE_Y = 480;
        private static readonly double D3D_MESH_SCALING_X = -1.0;
        private static readonly double D3D_MESH_SCALING_Y = 4.0;
        private static readonly double D3D_MESH_SCALING_Z = -0.13;

        // Function declarations for DirectX display
        [DllImport("mdispd3d.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern IntPtr MdepthD3DAlloc(MIL_ID DepthBuffer, MIL_ID IntensityBuffer, MIL_INT DisplaySizeX, MIL_INT DisplaySizeY, double ScaleX, double ScaleY, double ScaleZ, double MinZ, double MaxZ, double MaxDistanceZ, IntPtr WindowHandle);
        [DllImport("mdispd3d.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void MdispD3DFree(IntPtr DispHandle);
        [DllImport("mdispd3d.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void MdispD3DShow(IntPtr DispHandle);
        [DllImport("mdispd3d.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void MdispD3DHide(IntPtr DispHandle);
        [DllImport("mdispd3d.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void MdispD3DPrintHelp(IntPtr DispHandle);

        static void Main(string[] args)
        {
            MIL_ID MilApplication = MIL.M_NULL;  // Application identifier.
            MIL_ID MilSystem = MIL.M_NULL;       // System identifier.
            MIL_ID MilDisplay = MIL.M_NULL;      // Display identifier.
            MIL_ID MilDisplayImage = MIL.M_NULL; // Image buffer identifier.
            MIL_ID MilImage = MIL.M_NULL;        // Image buffer identifier.
            MIL_ID MilPosYImage = MIL.M_NULL;    // Image buffer identifier.
            MIL_ID MilValImage = MIL.M_NULL;     // Image buffer identifier.
            MIL_ID MilExtreme = MIL.M_NULL;      // Result buffer identifier.

            double FrameRate = 0.0;
            int n = 0;
            double PreviousTime = 0.0;
            double StartTime = 0.0;
            double EndTime = 0.0;
            double TotalProcessTime = 0.0;
            double WaitTime = 0.0;
            MIL_INT SizeX = 0;
            MIL_INT SizeY = 0;
            MIL_INT NumberOfImages = 0;
            MIL_INT[] ExtremePosY = new MIL_INT[2];
            ExtremePosY[0] = 0;
            ExtremePosY[1] = 0;

            // Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, ref MilApplication, ref MilSystem, ref MilDisplay, MIL.M_NULL, MIL.M_NULL);

            // Inquire characteristics of the input sequence.
            MIL.MbufDiskInquire(SEQUENCE_FILE, MIL.M_SIZE_X, ref SizeX);
            MIL.MbufDiskInquire(SEQUENCE_FILE, MIL.M_SIZE_Y, ref SizeY);
            MIL.MbufDiskInquire(SEQUENCE_FILE, MIL.M_NUMBER_OF_IMAGES, ref NumberOfImages);
            MIL.MbufDiskInquire(SEQUENCE_FILE, MIL.M_FRAME_RATE, ref FrameRate);

            // Allocate buffers to hold images.
            MIL.MbufAlloc2d(MilSystem, SizeX, SizeY, 8 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_PROC, ref MilImage);
            MIL.MbufAlloc2d(MilSystem, SizeX, SizeY, 8 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_DISP, ref MilDisplayImage);
            MIL.MbufAlloc2d(MilSystem, SizeX, NumberOfImages, 16 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_PROC, ref MilPosYImage);
            MIL.MbufAlloc2d(MilSystem, SizeX, NumberOfImages, 8 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_PROC, ref MilValImage);

            // Select display.
            MIL.MdispSelect(MilDisplay, MilDisplayImage);

            // Print a message.
            Console.Write("\nEXTRACTING 3D IMAGE FROM A LASER LINE:\n");
            Console.Write("--------------------------------------\n\n");
            Console.Write("The position of a laser line is being extracted from an image\n");
            Console.Write("to generate a depth image.\n\n");

            // Open the sequence file for reading.
            MIL.MbufImportSequence(SEQUENCE_FILE, MIL.M_DEFAULT, MIL.M_NULL, MIL.M_NULL, MIL.M_NULL, MIL.M_NULL, MIL.M_NULL, MIL.M_OPEN);

            // The function must be called once before the timing loop for more accurate 
            // time (dll load, ...).
            MIL.MimLocatePeak1d(MilImage, MilPosYImage, MilValImage, 0, MAX_LINE_WIDTH, MIN_INTENSITY, MIL.M_1D_COLUMNS + MIL.M_FIXED_POINT + NB_FIXED_POINT, MIL.M_DEFAULT);

            // Read and process all images in the input sequence.
            MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ + MIL.M_SYNCHRONOUS, ref PreviousTime);
            TotalProcessTime = 0.0;

            for (n = 0; n < NumberOfImages; n++)
            {
                // Read image from sequence.
                MIL.MbufImportSequence(SEQUENCE_FILE, MIL.M_DEFAULT, MIL.M_LOAD, MIL.M_NULL, ref MilImage, MIL.M_DEFAULT, 1, MIL.M_READ);

                // Display the image.
                MIL.MbufCopy(MilImage, MilDisplayImage);

                // Locate the peak in each column of the image.
                MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ + MIL.M_SYNCHRONOUS, ref StartTime);
                MIL.MimLocatePeak1d(MilImage, MilPosYImage, MilValImage, n, MAX_LINE_WIDTH, MIN_INTENSITY, MIL.M_1D_COLUMNS + MIL.M_FIXED_POINT + NB_FIXED_POINT, MIL.M_DEFAULT);
                MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ + MIL.M_SYNCHRONOUS, ref EndTime);
                TotalProcessTime += EndTime - StartTime;

                // Wait to have a proper frame rate.
                WaitTime = (1.0 / FrameRate) - (EndTime - PreviousTime);
                if (WaitTime > 0)
                {
                    MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_WAIT, ref WaitTime);
                }
                MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ + MIL.M_SYNCHRONOUS, ref PreviousTime);
            }

            // Close the sequence file.
            MIL.MbufImportSequence(SEQUENCE_FILE, MIL.M_DEFAULT, MIL.M_NULL, MIL.M_NULL, MIL.M_NULL, MIL.M_NULL, MIL.M_NULL, MIL.M_CLOSE);

            Console.Write("{0} images processed in {1,7:0.00} s ({2,7:0.00} ms/image).\n", NumberOfImages, TotalProcessTime, TotalProcessTime / (double)NumberOfImages * 1000.0);


            // Pause to show the result.
            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();

            Console.Write("The reconstructed images are being displayed.\n");

            // Draw extracted peak position in each column of each image.
            for (n = 0; n < NumberOfImages; n++)
            {
                MIL.MbufClear(MilImage, 0);
                MIL.MimDraw(MIL.M_DEFAULT, MilPosYImage, MilValImage, MilImage, MIL.M_DRAW_PEAKS + MIL.M_1D_COLUMNS + MIL.M_LINES, (double)n, 1, MIL.M_FIXED_POINT + NB_FIXED_POINT);

                // Display the result image.
                MIL.MbufCopy(MilImage, MilDisplayImage);
            }


            // Pause to show the result.
            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();


            // Try to allocate D3D display
            IntPtr DispHandle;
            DispHandle = MdepthD3DAlloc(MilPosYImage, MilValImage, D3D_DISPLAY_SIZE_X, D3D_DISPLAY_SIZE_Y, D3D_MESH_SCALING_X, D3D_MESH_SCALING_Y, D3D_MESH_SCALING_Z, MIL.M_DEFAULT, MIL.M_DEFAULT, MIL.M_DEFAULT, IntPtr.Zero);

            if (DispHandle != IntPtr.Zero)
            {
                Console.Write("The depth buffer is displayed using D3D.\n");

                // Hide Mil Display
                MIL.MdispControl(MilDisplay, MIL.M_WINDOW_SHOW, MIL.M_DISABLE);

                MdispD3DShow(DispHandle);
                MdispD3DPrintHelp(DispHandle);

                // Pause to show the result.
                Console.Write("Press <Enter> to end.\n");
                Console.ReadKey();

                MdispD3DHide(DispHandle);
                MdispD3DFree(DispHandle);
            }
            else
            {
                Console.Write("The depth buffer is displayed using MIL.\n");

                // Find the reMIL.Mapping for result buffers.
                MIL.MimAllocResult(MilSystem, MIL.M_DEFAULT, MIL.M_STAT_LIST, ref MilExtreme);

                MIL.MimStat(MilPosYImage, MilExtreme, MIL.M_MIN + MIL.M_MAX, MIL.M_NOT_EQUAL, 0xFFFF, MIL.M_NULL);
                MIL.MimGetResult(MilExtreme, MIL.M_MIN + MIL.M_TYPE_MIL_INT, ref ExtremePosY[0]);
                MIL.MimGetResult(MilExtreme, MIL.M_MAX + MIL.M_TYPE_MIL_INT, ref ExtremePosY[1]);

                MIL.MimFree(MilExtreme);

                // Free the display and reallocate a new one of the proper dimension for results.
                MIL.MbufFree(MilDisplayImage);
                MIL.MbufAlloc2d(MilSystem,
                                (MIL_INT)((double)SizeX * (D3D_MESH_SCALING_X > 0 ? D3D_MESH_SCALING_X : -D3D_MESH_SCALING_X)),
                                (MIL_INT)((double)NumberOfImages * D3D_MESH_SCALING_Y),
                                8 + MIL.M_UNSIGNED,
                                MIL.M_IMAGE + MIL.M_PROC + MIL.M_DISP,
                                ref MilDisplayImage);
                MIL.MdispSelect(MilDisplay, MilDisplayImage);

                // Display the height buffer.
                MIL.MimClip(MilPosYImage, MilPosYImage, MIL.M_GREATER, (double)ExtremePosY[1], MIL.M_NULL, (double)ExtremePosY[1], MIL.M_NULL);
                MIL.MimArith(MilPosYImage, (double)ExtremePosY[0], MilPosYImage, MIL.M_SUB_CONST);
                MIL.MimArith(MilPosYImage, ((ExtremePosY[1] - ExtremePosY[0]) / 255.0) + 1, MilPosYImage, MIL.M_DIV_CONST);
                MIL.MimResize(MilPosYImage, MilDisplayImage, MIL.M_FILL_DESTINATION, MIL.M_FILL_DESTINATION, MIL.M_BILINEAR);

                // Pause to show the result.
                Console.Write("Press <Enter> to end.\n");
                Console.ReadKey();
            }

            // Free all allocations.
            MIL.MbufFree(MilImage);
            MIL.MbufFree(MilDisplayImage);
            MIL.MbufFree(MilPosYImage);
            MIL.MbufFree(MilValImage);
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL);
        }
    }
}
