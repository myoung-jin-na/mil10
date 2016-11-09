//**********************************************************************************
// 
// File name: MimHistogram.cs
//
// Synopsis:  This program loads an image of a tissue sample, calculates its intensity
//            histogram and draws it.
//**********************************************************************************
using System;
using System.Collections.Generic;
using System.Text;

using Matrox.MatroxImagingLibrary;

namespace MImHistogram
{
    class Program
    {
        // Target MIL image file specifications.
        private const string IMAGE_FILE = MIL.M_IMAGE_PATH + "Cell.mim";

        // Number of possible pixel intensities.
        private const int HIST_NUM_INTENSITIES = 256;
        private const int HIST_SCALE_FACTOR = 8;
        private const int HIST_X_POSITION = 250;
        private const int HIST_Y_POSITION = 450;

        // Main function.
        static void Main(string[] args)
        {
            MIL_ID MilApplication = MIL.M_NULL;                   // Application identifier.
            MIL_ID MilSystem = MIL.M_NULL;                        // System identifier.
            MIL_ID MilDisplay = MIL.M_NULL;                       // Display identifier.
            MIL_ID MilImage = MIL.M_NULL;                         // Image buffer identifier.
            MIL_ID MilOverlayImage = MIL.M_NULL;                  // Overlay buffer identifier.
            MIL_ID HistResult = MIL.M_NULL;                       // Histogram buffer identifier.
            MIL_INT[] HistValues = new MIL_INT[HIST_NUM_INTENSITIES];     // Histogram values.
            double[] XStart = new double[HIST_NUM_INTENSITIES];
            double[] YStart = new double[HIST_NUM_INTENSITIES];
            double[] XEnd = new double[HIST_NUM_INTENSITIES];
            double[] YEnd = new double[HIST_NUM_INTENSITIES];
            double AnnotationColor = MIL.M_COLOR_RED;

            // Allocate the default system and image buffer.
            MIL.MappAllocDefault(MIL.M_DEFAULT, ref MilApplication, ref MilSystem, ref MilDisplay, MIL.M_NULL, MIL.M_NULL);

            // Restore source image into an automatically allocated image buffer.
            MIL.MbufRestore(IMAGE_FILE, MilSystem, ref MilImage);

            // Display the image buffer and prepare for overlay annotations.
            MIL.MdispSelect(MilDisplay, MilImage);
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY, MIL.M_ENABLE);
            MIL.MdispInquire(MilDisplay, MIL.M_OVERLAY_ID, ref MilOverlayImage);

            // Allocate a histogram result buffer.
            MIL.MimAllocResult(MilSystem, HIST_NUM_INTENSITIES, MIL.M_HIST_LIST, ref HistResult);

            // Calculate the histogram.
            MIL.MimHistogram(MilImage, HistResult);

            // Get the results.
            MIL.MimGetResult(HistResult, MIL.M_VALUE, HistValues);

            // Draw the histogram in the overlay.
            MIL.MgraColor(MIL.M_DEFAULT, AnnotationColor);
            for (int i = 0; i < HIST_NUM_INTENSITIES; i++)
            {
                XStart[i] = i + HIST_X_POSITION + 1;
                YStart[i] = HIST_Y_POSITION;
                XEnd[i] = i + HIST_X_POSITION + 1;
                YEnd[i] = HIST_Y_POSITION - (HistValues[i] / HIST_SCALE_FACTOR);
            }
            MIL.MgraLines(MIL.M_DEFAULT, MilOverlayImage, HIST_NUM_INTENSITIES, XStart, YStart, XEnd, YEnd, MIL.M_DEFAULT);

            // Print a message.
            Console.Write("\nHISTOGRAM:\n");
            Console.Write("----------\n\n");
            Console.Write("The histogram of the image was calculated and drawn.\n");
            Console.Write("Press <Enter> to end.\n\n");
            Console.ReadKey();

            // Free all allocations.
            MIL.MimFree(HistResult);
            MIL.MbufFree(MilImage);
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL);
        }
    }
}
