//*****************************************************************************
//
// File name: MimSegment.cs
//
// Synopsis:  This program uses the watershed and the edge detection functions 
//            to remove the background of an image with a non-linear lighting.
//            Then, the watershed and the distance functions are used to separate
//            the touching objects.
//*****************************************************************************
using System;
using System.Collections.Generic;
using System.Text;

using Matrox.MatroxImagingLibrary;

namespace MImSegment
{
    class Program
    {
        // Source image specifications.
        private const string IMAGE_FILE = MIL.M_IMAGE_PATH + "pills.mim";

        // Minimal distance and gradient variations for the watershed calculation.
        private const int WATERSHED_MINIMAL_GRADIENT_VARIATION = 45;
        private const int WATERSHED_MINIMAL_DISTANCE_VARIATION = 2;

        // Position used to fetch the label of the background.
        private const int PIXEL_FETCH_POSITION_X = 2;
        private const int PIXEL_FETCH_POSITION_Y = 2;

        static void Main(string[] args)
        {
            MIL_ID MilApplication = MIL.M_NULL;         // Application identifier.
            MIL_ID MilSystem = MIL.M_NULL;              // System identifier.
            MIL_ID MilDisplay = MIL.M_NULL;             // Display identifier.
            MIL_ID MilImage = MIL.M_NULL;               // Image buffer identifier.
            MIL_ID MilImageWatershed = MIL.M_NULL;      // Image buffer identifier.
            int[] lFetchedValue = new int[1] { 0 };     // Label of the background.

            // Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, ref MilApplication, ref MilSystem, ref MilDisplay, MIL.M_NULL, MIL.M_NULL);

            // Restore the source image into two automatically allocated
            // image buffers and select one of them to the display.
            MIL.MbufRestore(IMAGE_FILE, MilSystem, ref MilImageWatershed);
            MIL.MbufRestore(IMAGE_FILE, MilSystem, ref MilImage);
            MIL.MdispSelect(MilDisplay, MilImage);

            // Pause to show the original image.
            Console.Write("\nSEGMENTATION:\n");
            Console.Write("-------------\n\n");
            Console.Write("An edge detection followed by a watershed will be used to remove\n");
            Console.Write("the background.\nPress <Enter> to continue.\n\n");
            Console.ReadKey();

            // Perform a edge detection on the original image.
            MIL.MimEdgeDetect(MilImageWatershed, MilImageWatershed, MIL.M_NULL, MIL.M_SOBEL, MIL.M_REGULAR_EDGE_DETECT, MIL.M_NULL);

            // Find the basins of the edge detected image that have a minimal gray scale
            // variation of WATERSHED_MINIMAL_GRADIENT_VARIATION.
            MIL.MimWatershed(MilImageWatershed, MIL.M_NULL, MilImageWatershed, WATERSHED_MINIMAL_GRADIENT_VARIATION, MIL.M_MINIMA_FILL + MIL.M_BASIN);

            // Fetch the label of the background, clip it to zero and clip the other labels to 
            // the maximum value of the buffer.
            MIL.MbufGet2d(MilImageWatershed, PIXEL_FETCH_POSITION_X, PIXEL_FETCH_POSITION_Y, 1, 1, lFetchedValue);
            MIL.MimClip(MilImageWatershed, MilImageWatershed, MIL.M_EQUAL, lFetchedValue[0], 0, 0, 0);
            MIL.MimClip(MilImageWatershed, MilImage, MIL.M_NOT_EQUAL, 0, 0, 0xFF, 0);

            // Pause to show the binarized image.
            Console.Write("A distance transformation followed by a watershed will be used \n");
            Console.Write("to separate the touching pills.\n");
            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();

            // Perform a distance transformation on the binarized image.
            MIL.MimDistance(MilImage, MilImageWatershed, MIL.M_CHAMFER_3_4);

            // Find the watersheds of the image.
            MIL.MimWatershed(MilImageWatershed, MIL.M_NULL, MilImageWatershed, WATERSHED_MINIMAL_DISTANCE_VARIATION, MIL.M_STRAIGHT_WATERSHED + MIL.M_MAXIMA_FILL + MIL.M_SKIP_LAST_LEVEL);

            // AND the watershed image with the binarized image to separate the touching pills.
            MIL.MimArith(MilImageWatershed, MilImage, MilImage, MIL.M_AND);

            // Pause to show the segmented image.
            Console.Write("Here are the segmented pills.\n");
            Console.Write("Press <Enter> to end.\n\n");
            Console.ReadKey();

            // Free all allocations.
            MIL.MbufFree(MilImageWatershed);
            MIL.MbufFree(MilImage);
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL);
        }
    }
}
