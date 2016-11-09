//*****************************************************************************
// 
// File name: MimConvolve.cs
//
// Synopsis:  This program performs a 3x3 convolution using a custom kernel
//            and calculates the convolution time.
//*****************************************************************************
using System;
using System.Collections.Generic;
using System.Text;

using Matrox.MatroxImagingLibrary;

namespace MImConvolve
{
    class Program
    {
        // Target MIL image specifications.
        private const string IMAGE_FILE = MIL.M_IMAGE_PATH + "BaboonMono.mim";
        private const int ZOOM_VALUE = 2;

        // Kernel data definition.
        private const int KERNEL_WIDTH = 3;
        private const int KERNEL_HEIGHT = 3;
        private const int KERNEL_DEPTH = 8;
        private static readonly byte[,] KernelData = new byte[KERNEL_HEIGHT, KERNEL_WIDTH] { { 1, 2, 1 }, { 2, 4, 2 }, { 1, 2, 1 } };

        // Timing loop iterations.
        private const int NB_LOOP = 100;

        static void Main(string[] args)
        {
            MIL_ID MilApplication = MIL.M_NULL;     // Application identifier.
            MIL_ID MilSystem = MIL.M_NULL;          // System identifier.
            MIL_ID MilDisplay = MIL.M_NULL;         // Display identifier.
            MIL_ID MilDisplayImage = MIL.M_NULL;    // Image buffer identifier.
            MIL_ID MilImage = MIL.M_NULL;           // Image buffer identifier.
            MIL_ID MilKernel = MIL.M_NULL;          // Custom kernel identifier.
            int n = 0;
            double Time = 0.0;

            // Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, ref MilApplication, ref MilSystem, ref MilDisplay, MIL.M_NULL, MIL.M_NULL);

            // Restore source image into an automatically allocated image buffers.
            MIL.MbufRestore(IMAGE_FILE, MilSystem, ref MilImage);
            MIL.MbufRestore(IMAGE_FILE, MilSystem, ref MilDisplayImage);

            // Zoom display to see the result of image processing better.
            MIL.MdispZoom(MilDisplay, ZOOM_VALUE, ZOOM_VALUE);

            // Display the image buffer.
            MIL.MdispSelect(MilDisplay, MilDisplayImage);

            // Pause to show the original image.
            Console.Write("\nIMAGE PROCESSING:\n");
            Console.Write("-----------------\n\n");
            Console.Write("This program performs a convolution on the displayed image.\n");
            Console.Write("It uses a custom smoothing kernel.\n");
            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();

            // Allocate a MIL kernel.
            MIL.MbufAlloc2d(MilSystem, KERNEL_WIDTH, KERNEL_HEIGHT, KERNEL_DEPTH + MIL.M_UNSIGNED, MIL.M_KERNEL, ref MilKernel);

            // Put the custom data in it.
            MIL.MbufPut(MilKernel, KernelData);

            // Set a normalization (divide) factor to have a kernel with
            // a sum equal to one.
            MIL.MbufControlNeighborhood(MilKernel, MIL.M_NORMALIZATION_FACTOR, 16);

            // Convolve the image using the kernel.
            MIL.MimConvolve(MilImage, MilDisplayImage, MilKernel);

            // Now time the convolution (MimConvolve()):
            // Overscan calculation is disabled and a destination image that
            // is not displayed is used to have the real convolution time. Also the 
            // function must be called once before the timing loop for more accurate 
            // time (dll load, ...).
            MIL.MbufControlNeighborhood(MilKernel, MIL.M_OVERSCAN, MIL.M_DISABLE);
            MIL.MimConvolve(MilDisplayImage, MilImage, MilKernel);
            MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_RESET + MIL.M_SYNCHRONOUS, MIL.M_NULL);
            for (n = 0; n < NB_LOOP; n++)
            {
                MIL.MimConvolve(MilDisplayImage, MilImage, MilKernel);
            }
            MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ + MIL.M_SYNCHRONOUS, ref Time);

            // Pause to show the result.
            Console.Write("Convolve time: {0:0.000} ms.\n", Time * 1000 / NB_LOOP);
            Console.Write("Press <Enter> to terminate.\n");
            Console.ReadKey();

            // Free all allocations.
            MIL.MbufFree(MilKernel);
            MIL.MbufFree(MilImage);
            MIL.MbufFree(MilDisplayImage);
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL);

        }
    }
}
