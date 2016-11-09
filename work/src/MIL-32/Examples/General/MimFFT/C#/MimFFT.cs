//***********************************************************************************
//
// File name: MimFFT.cs
//
// Synopsis:  This program uses the Fast Fourier Transform to filter an image.
//***********************************************************************************
using System;
using System.Collections.Generic;
using System.Text;

using Matrox.MatroxImagingLibrary;

namespace MImFFT
{
    class Program
    {
        // Target image specifications.
        private const string NOISY_IMAGE = MIL.M_IMAGE_PATH + "noise.mim";
        private const int IMAGE_WIDTH = 256;
        private const int IMAGE_HEIGHT = 256;
        private const int X_NEGATIVE_FREQUENCY_POSITION = 63;
        private const int X_POSITIVE_FREQUENCY_POSITION = 191;
        private const int Y_FREQUENCY_POSITION = 127;
        private const int CIRCLE_WIDTH = 9;

        static void Main(string[] args)
        {
            MIL_ID MilApplication = MIL.M_NULL;     // Application identifier.
            MIL_ID MilSystem = MIL.M_NULL;          // System identifier.
            MIL_ID MilDisplay = MIL.M_NULL;         // Display identifier.
            MIL_ID MilImage = MIL.M_NULL;           // Image buffer identifier.
            MIL_ID MilOverlayImage = MIL.M_NULL;    // Overlay image buffer identifier.
            MIL_ID MilSubImage00 = MIL.M_NULL;      // Child buffer identifier.
            MIL_ID MilSubImage01 = MIL.M_NULL;      // Child buffer identifier.
            MIL_ID MilSubImage10 = MIL.M_NULL;      // Child buffer identifier.
            MIL_ID MilSubImage11 = MIL.M_NULL;      // Child buffer identifier.
            MIL_ID MilTransformReal = MIL.M_NULL;   // Real part of the transformed image.
            MIL_ID MilTransformIm = MIL.M_NULL;     // Imaginary part of the transformed image.

            float[] ZeroVal = new float[1];
            ZeroVal[0] = 0.0F;

            // Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, ref MilApplication, ref MilSystem, ref MilDisplay, MIL.M_NULL, MIL.M_NULL);

            // Allocate a display buffer and clear it.
            MIL.MbufAlloc2d(MilSystem, IMAGE_WIDTH * 2, IMAGE_HEIGHT * 2, 8 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_PROC + MIL.M_DISP, ref MilImage);
            MIL.MbufClear(MilImage, 0L);

            // Display the image buffer and prepare for overlay annotations.
            MIL.MdispSelect(MilDisplay, MilImage);
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY, MIL.M_ENABLE);
            MIL.MdispInquire(MilDisplay, MIL.M_OVERLAY_ID, ref MilOverlayImage);

            // Allocate child buffers in the 4 quadrants of the display image.
            MIL.MbufChild2d(MilImage, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT, ref MilSubImage00);
            MIL.MbufChild2d(MilImage, IMAGE_WIDTH, 0, IMAGE_WIDTH, IMAGE_HEIGHT, ref MilSubImage01);
            MIL.MbufChild2d(MilImage, 0, IMAGE_HEIGHT, IMAGE_WIDTH, IMAGE_HEIGHT, ref MilSubImage10);
            MIL.MbufChild2d(MilImage, IMAGE_WIDTH, IMAGE_HEIGHT, IMAGE_WIDTH, IMAGE_HEIGHT, ref MilSubImage11);

            // Allocate processing buffers.
            MIL.MbufAlloc2d(MilSystem, IMAGE_WIDTH, IMAGE_HEIGHT, 32 + MIL.M_FLOAT, MIL.M_IMAGE + MIL.M_PROC, ref MilTransformReal);
            MIL.MbufAlloc2d(MilSystem, IMAGE_WIDTH, IMAGE_HEIGHT, 32 + MIL.M_FLOAT, MIL.M_IMAGE + MIL.M_PROC, ref MilTransformIm);

            // Load a noisy image.
            MIL.MbufLoad(NOISY_IMAGE, MilSubImage00);

            // Print a message on the screen.
            Console.Write("\nFFT:\n");
            Console.Write("----\n\n");
            Console.Write("The frequency spectrum of a noisy image will be computed to remove the periodic noise.\n");
            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();

            // The image is Fourier transformed to obtain the magnitude of the
            // spectrum. This result will be used to design the filter.
            MIL.MimTransform(MilSubImage00, MIL.M_NULL, MilTransformReal, MilTransformIm, MIL.M_FFT, MIL.M_FORWARD + MIL.M_CENTER + MIL.M_MAGNITUDE + MIL.M_LOG_SCALE);
            MIL.MbufCopy(MilTransformReal, MilSubImage10);

            // Draw circles in the overlay around the points of interest.
            MIL.MbufCopy(MilTransformReal, MilSubImage11);
            MIL.MgraColor(MIL.M_DEFAULT, MIL.M_COLOR_YELLOW);
            MIL.MgraArc(MIL.M_DEFAULT, MilOverlayImage, X_NEGATIVE_FREQUENCY_POSITION, Y_FREQUENCY_POSITION + IMAGE_HEIGHT, CIRCLE_WIDTH, CIRCLE_WIDTH, 0, 360);
            MIL.MgraArc(MIL.M_DEFAULT, MilOverlayImage, X_POSITIVE_FREQUENCY_POSITION, Y_FREQUENCY_POSITION + IMAGE_HEIGHT, CIRCLE_WIDTH, CIRCLE_WIDTH, 0, 360);
            MIL.MgraArc(MIL.M_DEFAULT, MilOverlayImage, X_NEGATIVE_FREQUENCY_POSITION + IMAGE_WIDTH, Y_FREQUENCY_POSITION + IMAGE_HEIGHT, CIRCLE_WIDTH, CIRCLE_WIDTH, 0, 360);
            MIL.MgraArc(MIL.M_DEFAULT, MilOverlayImage, X_POSITIVE_FREQUENCY_POSITION + IMAGE_WIDTH, Y_FREQUENCY_POSITION + IMAGE_HEIGHT, CIRCLE_WIDTH, CIRCLE_WIDTH, 0, 360);

            // Put zero in the spectrum where the noise is located.
            MIL.MbufPut2d(MilSubImage11, X_NEGATIVE_FREQUENCY_POSITION, Y_FREQUENCY_POSITION, 1, 1, ZeroVal);
            MIL.MbufPut2d(MilSubImage11, X_POSITIVE_FREQUENCY_POSITION, Y_FREQUENCY_POSITION, 1, 1, ZeroVal);

            // Compute the Fast Fourier Transform of the image.
            MIL.MimTransform(MilSubImage00, MIL.M_NULL, MilTransformReal, MilTransformIm, MIL.M_FFT, MIL.M_FORWARD + MIL.M_CENTER);

            // Filter the image in the frequency domain.
            MIL.MbufPut2d(MilTransformReal, X_NEGATIVE_FREQUENCY_POSITION, Y_FREQUENCY_POSITION, 1, 1, ZeroVal);
            MIL.MbufPut2d(MilTransformReal, X_POSITIVE_FREQUENCY_POSITION, Y_FREQUENCY_POSITION, 1, 1, ZeroVal);
            MIL.MbufPut2d(MilTransformIm, X_NEGATIVE_FREQUENCY_POSITION, Y_FREQUENCY_POSITION, 1, 1, ZeroVal);
            MIL.MbufPut2d(MilTransformIm, X_POSITIVE_FREQUENCY_POSITION, Y_FREQUENCY_POSITION, 1, 1, ZeroVal);

            // Recover the image in the spatial domain.
            MIL.MimTransform(MilTransformReal, MilTransformIm, MilSubImage01, MIL.M_NULL, MIL.M_FFT, MIL.M_REVERSE + MIL.M_CENTER + MIL.M_SATURATION);

            // Print a message.
            Console.Write("The frequency components of the noise are located in the center of the circles.\n");
            Console.Write("The noise was removed by setting these frequency components to zero.\n");
            Console.Write("Press <Enter> to end.\n\n");
            Console.ReadKey();

            // Free buffers.
            MIL.MbufFree(MilSubImage00);
            MIL.MbufFree(MilSubImage01);
            MIL.MbufFree(MilSubImage10);
            MIL.MbufFree(MilSubImage11);
            MIL.MbufFree(MilImage);
            MIL.MbufFree(MilTransformReal);
            MIL.MbufFree(MilTransformIm);

            // Free defaults.
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL);

        }
    }
}
