///****************************************************************************
//
// File name: MimWarp.cs
//
// Synopsis:  : This program performs three types of warp transformations. 
//              First the image is stretched according to four specified 
//              reference points. Then it is warped on a sinusoid, and 
//              finally, the program loops while warping the image on a 
//              sphere.
//*****************************************************************************
using System;
using System.Collections.Generic;
using System.Text;

using Matrox.MatroxImagingLibrary;

namespace MImWarp
{
    class Program
    {
        // Target image specifications.
        private const string IMAGE_FILE = MIL.M_IMAGE_PATH + "BaboonMono.mim";
        private const int FIXED_POINT_PRECISION = MIL.M_FIXED_POINT + 6;
        private static int FLOAT_TO_FIXED_POINT(double x) { return (int)(64 * (x)); }
        private const int ROTATION_STEP = 2;

        static void Main(string[] args)
        {
            MIL_ID MilApplication = MIL.M_NULL;         // Application identifier.
            MIL_ID MilSystem = MIL.M_NULL;              // System identifier.
            MIL_ID MilDisplay = MIL.M_NULL;             // Display identifier.
            MIL_ID MilDisplayImage = MIL.M_NULL;        // Image buffer identifier.
            MIL_ID MilSourceImage = MIL.M_NULL;         // Image buffer identifier.
            MIL_ID Mil4CornerArray = MIL.M_NULL;        // Coefficients buffer identifier.
            MIL_ID MilLutX = MIL.M_NULL;                // Lut buffer identifier.
            MIL_ID MilLutY = MIL.M_NULL;                // Lut buffer identifier.
            MIL_ID ChildWindow = MIL.M_NULL;            // Child Image identifier.
            float[] FourCornerMatrix = new float[12] 
            {
                0.0F,                                       // X coordinate of quadrilateral's 1st corner
                0.0F,                                       // Y coordinate of quadrilateral's 1st corner
                456.0F,                                     // X coordinate of quadrilateral's 2nd corner
                62.0F,                                      // Y coordinate of quadrilateral's 2nd corner
                333.0F,                                     // X coordinate of quadrilateral's 3rd corner
                333.0F,                                     // Y coordinate of quadrilateral's 3rd corner
                100.0F,                                     // X coordinate of quadrilateral's 4th corner
                500.0F,                                     // Y coordinate of quadrilateral's 4th corner
                0.0F,                                       // X coordinate of rectangle's top-left corner
                0.0F,                                       // Y coordinate of rectangle's top-left corner
                511.0F,                                     // X coordinate of rectangle's bottom-right corner
                511.0F 
            };                                   // Y coordinate of rectangle's bottom-right corner
            int Precision = FIXED_POINT_PRECISION;
            int Interpolation = MIL.M_NEAREST_NEIGHBOR;
            short[] MilLutXPtr, MilLutYPtr;
            int OffsetX = 0;
            MIL_INT ImageWidth = 0;
            MIL_INT ImageHeight = 0;
            MIL_INT ImageType = 0;
            int i = 0;
            int j = 0;
            double FramesPerSecond = 0.0;
            double Time = 0.0;
            double NbLoop = 0.0;

            // Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, ref MilApplication, ref MilSystem, ref MilDisplay, MIL.M_NULL, MIL.M_NULL);

            // Restore the source image.
            MIL.MbufRestore(IMAGE_FILE, MilSystem, ref MilSourceImage);

            // Allocate a display buffers and show the source image.
            MIL.MbufInquire(MilSourceImage, MIL.M_SIZE_X, ref ImageWidth);
            MIL.MbufInquire(MilSourceImage, MIL.M_SIZE_Y, ref ImageHeight);
            MIL.MbufInquire(MilSourceImage, MIL.M_TYPE, ref ImageType);
            MIL.MbufAlloc2d(MilSystem, ImageWidth, ImageHeight, ImageType, MIL.M_IMAGE + MIL.M_PROC + MIL.M_DISP, ref MilDisplayImage);
            MIL.MbufCopy(MilSourceImage, MilDisplayImage);
            MIL.MdispSelect(MilDisplay, MilDisplayImage);

            // Print a message.
            Console.Write("\nWARPING:\n");
            Console.Write("--------\n\n");
            Console.Write("This image will be warped using different methods.\n");
            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();


            // Four-corner LUT warping
            //-------------------------

            // Allocate 2 LUT buffers.
            MIL.MbufAlloc2d(MilSystem, ImageWidth, ImageHeight, 16 + MIL.M_SIGNED, MIL.M_LUT, ref MilLutX);
            MIL.MbufAlloc2d(MilSystem, ImageWidth, ImageHeight, 16 + MIL.M_SIGNED, MIL.M_LUT, ref MilLutY);

            // Allocate the coefficient buffer.
            MIL.MbufAlloc2d(MilSystem, 12, 1, 32 + MIL.M_FLOAT, MIL.M_ARRAY, ref Mil4CornerArray);

            // Put warp values into the coefficient buffer.
            MIL.MbufPut1d(Mil4CornerArray, 0, 12, FourCornerMatrix);

            // Generate LUT buffers.
            MIL.MgenWarpParameter(Mil4CornerArray, MilLutX, MilLutY, MIL.M_WARP_4_CORNER + Precision, MIL.M_DEFAULT, 0.0, 0.0);

            // Clear the destination.
            MIL.MbufClear(MilDisplayImage, 0);

            // Warp the image.
            MIL.MimWarp(MilSourceImage, MilDisplayImage, MilLutX, MilLutY, MIL.M_WARP_LUT + Precision, Interpolation);

            // Print a message.
            Console.Write("The image was warped from an arbitrary quadrilateral to a square.\n");
            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();


            // Sinusoidal LUT warping
            //------------------------

            // Allocate user-defined LUTs.
            MilLutXPtr = new short[ImageHeight * ImageWidth];
            MilLutYPtr = new short[ImageHeight * ImageWidth];

            // Fill the LUT with a sinusoidal waveforms with a 6-bit precision.
            for (j = 0; j < ImageHeight; j++)
            {
                for (i = 0; i < ImageWidth; i++)
                {
                    MilLutYPtr[i + (j * ImageWidth)] = (short)FLOAT_TO_FIXED_POINT(((j) + (int)((20 * Math.Sin(0.03 * i)))));
                    MilLutXPtr[i + (j * ImageWidth)] = (short)FLOAT_TO_FIXED_POINT(((i) + (int)((20 * Math.Sin(0.03 * j)))));
                }
            }

            // Put the values into the LUT buffers.
            MIL.MbufPut2d(MilLutX, 0, 0, ImageWidth, ImageHeight, MilLutXPtr);
            MIL.MbufPut2d(MilLutY, 0, 0, ImageWidth, ImageHeight, MilLutYPtr);

            // Clear the destination.
            MIL.MbufClear(MilDisplayImage, 0);

            // Warp the image.
            MIL.MimWarp(MilSourceImage, MilDisplayImage, MilLutX, MilLutY, MIL.M_WARP_LUT + Precision, Interpolation);

            // wait for a key
            Console.Write("The image was warped on two sinusoidal waveforms.\n");
            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();

            // Continuous spherical LUT warping
            //--------------------------------

            // Allocate temporary buffer.
            MIL.MbufFree(MilSourceImage);
            MIL.MbufAlloc2d(MilSystem, ImageWidth * 2, ImageHeight, ImageType, MIL.M_IMAGE + MIL.M_PROC, ref MilSourceImage);

            // Reload the image.
            MIL.MbufLoad(IMAGE_FILE, MilSourceImage);

            // Fill the LUTs with a sphere pattern with a 6-bit precision.
            GenerateSphericLUT(ImageWidth, ImageHeight, MilLutXPtr, MilLutYPtr);
            MIL.MbufPut2d(MilLutX, 0, 0, ImageWidth, ImageHeight, MilLutXPtr);
            MIL.MbufPut2d(MilLutY, 0, 0, ImageWidth, ImageHeight, MilLutYPtr);

            // Duplicate the buffer to allow wrap around in the warping.
            MIL.MbufCopy(MilSourceImage, MilDisplayImage);
            MIL.MbufChild2d(MilSourceImage, ImageWidth, 0, ImageWidth, ImageHeight, ref ChildWindow);
            MIL.MbufCopy(MilDisplayImage, ChildWindow);
            MIL.MbufFree(ChildWindow);

            // Clear the destination.
            MIL.MbufClear(MilDisplayImage, 0);

            // Print a message and start the timer.
            Console.Write("The image is continuously warped on a sphere.\n");
            Console.Write("Press <Enter> to stop.\n\n");
            MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_RESET + MIL.M_SYNCHRONOUS, MIL.M_NULL);

            // Warp the image continuously.
            while (!Console.KeyAvailable)
            {
                // Create a child in the buffer containing the two images.
                MIL.MbufChild2d(MilSourceImage, OffsetX, 0, ImageWidth, ImageHeight, ref ChildWindow);

                // Warp the child in the window.
                MIL.MimWarp(ChildWindow, MilDisplayImage, MilLutX, MilLutY, MIL.M_WARP_LUT + Precision, Interpolation);

                // Update the offset (shift the window to the right).
                OffsetX += ROTATION_STEP;

                // Reset the offset if the child is outside the buffer.
                if (OffsetX > ImageWidth - 1)
                {
                    OffsetX = 0;
                }

                // Free the child.
                MIL.MbufFree(ChildWindow);

                NbLoop++;

                // Calculate and print the number of frames per second processed.
                MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ + MIL.M_SYNCHRONOUS, ref Time);
                FramesPerSecond = NbLoop / Time;
                Console.Write("Processing speed: {0:0} Images/Sec.\r", FramesPerSecond);
                YieldToGUI();
            }
            Console.ReadKey();

            // Free objects.
            MIL.MbufFree(MilLutX);
            MIL.MbufFree(MilLutY);
            MIL.MbufFree(Mil4CornerArray);
            MIL.MbufFree(MilSourceImage);
            MIL.MbufFree(MilDisplayImage);
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL);
        }

        // Generate two custom LUTs used to map the image on a sphere.
        // -----------------------------------------------------------
        static void GenerateSphericLUT(MIL_INT ImageWidth, MIL_INT ImageHeight, short[] MilLutXPtr, short[] MilLutYPtr)
        {
            MIL_INT i, j, k;
            double utmp, vtmp, tmp;
            short v;

            // Set the radius of the sphere
            double Radius = 200.0;

            // Generate the X and Y buffers
            for (j = 0; j < ImageHeight; j++)
            {
                k = j * ImageWidth;

                // Check that still in the sphere (in the Y axis).
                if (Math.Abs(vtmp = ((double)(j - (ImageHeight / 2)) / Radius)) < 1.0)
                {
                    // We scan from top to bottom, so reverse the value obtained above
                    // and obtain the angle.
                    vtmp = Math.Acos(-vtmp);
                    if (vtmp == 0.0)
                    {
                        vtmp = 0.0000001;
                    }

                    // Compute the position to fetch in the source.
                    v = (short)((vtmp / 3.1415926) * (double)(ImageHeight - 1) + 0.5);

                    // Compute the Y coordinate of the sphere.
                    tmp = Radius * Math.Sin(vtmp);

                    for (i = 0; i < ImageWidth; i++)
                    {
                        // Check that still in the sphere.
                        if (Math.Abs(utmp = ((double)(i - (ImageWidth / 2)) / tmp)) < 1.0)
                        {
                            utmp = Math.Acos(-utmp);

                            // Compute the position to fetch (fold the image in four).
                            MilLutXPtr[i + k] = (short)FLOAT_TO_FIXED_POINT(((utmp / 3.1415926) *
                                     (double)((ImageWidth / 2) - 1) + 0.5));
                            MilLutYPtr[i + k] = (short)FLOAT_TO_FIXED_POINT(v);
                        }
                        else
                        {
                            // Default position (fetch outside the buffer to 
                            // activate the clear overscan).
                            MilLutXPtr[i + k] = (short)FLOAT_TO_FIXED_POINT(ImageWidth);
                            MilLutYPtr[i + k] = (short)FLOAT_TO_FIXED_POINT(ImageHeight);
                        }
                    }
                }
                else
                {
                    for (i = 0; i < ImageWidth; i++)
                    {
                        // Default position (fetch outside the buffer for clear overscan).
                        MilLutXPtr[i + k] = (short)FLOAT_TO_FIXED_POINT(ImageWidth);
                        MilLutYPtr[i + k] = (short)FLOAT_TO_FIXED_POINT(ImageHeight);
                    }
                }
            }
        }

        // Windows CE GUI Scheduling Adjustment Handling
        // ---------------------------------------------
        // NOTE: Under Windows CE, YieldToGUI function improves system responsiveness 
        //       in case a normal Windows CE console application thread is processing 
        //       in a while loop.
        static void YieldToGUI()
        {
#if M_MIL_USE_CE
            System.Threading.Thread.Sleep(0);
#endif
        }
    }
}
