//****************************************************************************
//
// File name: Medge.cs
//
// Synopsis:  This program uses the MIL Edge Finder module to find and measure
//            the outer diameter of good seals in a target image. 
//****************************************************************************
using System;
using System.Collections.Generic;
using System.Text;

using Matrox.MatroxImagingLibrary;

namespace MEdge
{
    class Program
    {
        // Source MIL image file specifications.
        private const string CONTOUR_IMAGE = MIL.M_IMAGE_PATH + "Seals.mim";
        private const int CONTOUR_MAX_RESULTS = 100;
        private const double CONTOUR_MAXIMUM_ELONGATION = 0.8;
        private static readonly int CONTOUR_DRAW_COLOR = MIL.M_COLOR_GREEN;
        private static readonly int CONTOUR_LABEL_COLOR = MIL.M_COLOR_RED;

        static void Main(string[] args)
        {
            MIL_ID MilApplication = MIL.M_NULL;                             // Application identifier.
            MIL_ID MilSystem = MIL.M_NULL;                                  // System Identifier.
            MIL_ID MilDisplay = MIL.M_NULL;                                 // Display identifier.
            MIL_ID MilImage = MIL.M_NULL;                                   // Image buffer identifier.
            MIL_ID GraphicList = MIL.M_NULL;                                // Graphic list identifier.
            MIL_ID MilEdgeContext = MIL.M_NULL;                             // Edge context.
            MIL_ID MilEdgeResult = MIL.M_NULL;                              // Edge result identifier.
            double EdgeDrawColor = CONTOUR_DRAW_COLOR;                      // Edge draw color.
            double LabelDrawColor = CONTOUR_LABEL_COLOR;                    // Text draw color.
            MIL_INT NumEdgeFound = 0;                                       // Number of edges found.
            MIL_INT NumResults = 0;                                         // Number of results found.
            int i = 0;                                                      // Loop variable.
            double[] MeanFeretDiameter = new double[CONTOUR_MAX_RESULTS];   // Edge mean Feret diameter.

            // Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, ref MilApplication, ref MilSystem, ref MilDisplay, MIL.M_NULL, MIL.M_NULL);

            // Restore the image and display it.
            MIL.MbufRestore(CONTOUR_IMAGE, MilSystem, ref MilImage);
            MIL.MdispSelect(MilDisplay, MilImage);

            // Allocate a graphic list to hold the subpixel annotations to draw.
            MIL.MgraAllocList(MilSystem, MIL.M_DEFAULT, ref GraphicList);

            // Associate the graphic list to the display for annotations.
            MIL.MdispControl(MilDisplay, MIL.M_ASSOCIATED_GRAPHIC_LIST_ID, GraphicList);

            // Pause to show the original image.
            Console.Write("\nEDGE MODULE:\n");
            Console.Write("------------\n\n");
            Console.Write("This program determines the outer seal diameters in the displayed image \n");
            Console.Write("by detecting and analyzing contours with the Edge Finder module.\n");
            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();

            // Allocate a Edge Finder context.
            MIL.MedgeAlloc(MilSystem, MIL.M_CONTOUR, MIL.M_DEFAULT, ref MilEdgeContext);

            // Allocate a result buffer.
            MIL.MedgeAllocResult(MilSystem, MIL.M_DEFAULT, ref MilEdgeResult);

            // Enable features to compute.
            MIL.MedgeControl(MilEdgeContext, MIL.M_MOMENT_ELONGATION, MIL.M_ENABLE);
            MIL.MedgeControl(MilEdgeContext, MIL.M_FERET_MEAN_DIAMETER + MIL.M_SORT1_DOWN, MIL.M_ENABLE);

            // Calculate edges and features.
            MIL.MedgeCalculate(MilEdgeContext, MilImage, MIL.M_NULL, MIL.M_NULL, MIL.M_NULL, MilEdgeResult, MIL.M_DEFAULT);

            // Get the number of edges found.
            MIL.MedgeGetResult(MilEdgeResult, MIL.M_DEFAULT, MIL.M_NUMBER_OF_CHAINS + MIL.M_TYPE_MIL_INT, ref NumEdgeFound);

            // Draw edges in the source image to show the result.
            MIL.MgraColor(MIL.M_DEFAULT, EdgeDrawColor);
            MIL.MedgeDraw(MIL.M_DEFAULT, MilEdgeResult, GraphicList, MIL.M_DRAW_EDGES, MIL.M_DEFAULT, MIL.M_DEFAULT);

            // Pause to show the edges.
            Console.Write("{0} edges were found in the image.\n", NumEdgeFound);
            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();

            // Exclude elongated edges.
            MIL.MedgeSelect(MilEdgeResult, MIL.M_EXCLUDE, MIL.M_MOMENT_ELONGATION, MIL.M_LESS, CONTOUR_MAXIMUM_ELONGATION, MIL.M_NULL);

            // Exclude inner chains.
            MIL.MedgeSelect(MilEdgeResult, MIL.M_EXCLUDE, MIL.M_INCLUDED_EDGES, MIL.M_INSIDE_BOX, MIL.M_NULL, MIL.M_NULL);

            // Draw remaining edges and their index to show the result.
            MIL.MgraClear(MIL.M_DEFAULT, GraphicList);
            MIL.MgraColor(MIL.M_DEFAULT, EdgeDrawColor);
            MIL.MedgeDraw(MIL.M_DEFAULT, MilEdgeResult, GraphicList, MIL.M_DRAW_EDGES, MIL.M_DEFAULT, MIL.M_DEFAULT);

            // Pause to show the results.
            Console.Write("Elongated edges and inner edges of each seal were removed.\n");
            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();

            // Get the number of edges found.
            MIL.MedgeGetResult(MilEdgeResult, MIL.M_DEFAULT, MIL.M_NUMBER_OF_CHAINS + MIL.M_TYPE_MIL_INT, ref NumResults);

            // If the right number of edges were found.
            if ((NumResults >= 1) && (NumResults <= CONTOUR_MAX_RESULTS))
            {
                // Draw the index of each edge.
                MIL.MgraColor(MIL.M_DEFAULT, LabelDrawColor);
                MIL.MedgeDraw(MIL.M_DEFAULT, MilEdgeResult, GraphicList, MIL.M_DRAW_INDEX, MIL.M_DEFAULT, MIL.M_DEFAULT);

                // Get the mean Feret diameters.
                MIL.MedgeGetResult(MilEdgeResult, MIL.M_DEFAULT, MIL.M_FERET_MEAN_DIAMETER, MeanFeretDiameter);

                // Print the results.
                Console.Write("Mean diameter of the {0} outer edges are:\n\n", NumResults);
                Console.Write("Index   Mean diameter \n");
                for (i = 0; i < NumResults; i++)
                {
                    Console.Write("{0,-11}{1,-13:0.00}\n", i, MeanFeretDiameter[i]);
                }
            }
            else
            {
                Console.Write("Edges have not been found or the number of found edges is greater than\n");
                Console.Write("the specified maximum number of edges !\n\n");
            }

            // Wait for a key press.
            Console.Write("\nPress <Enter> to end.\n");
            Console.ReadKey();

            // Free MIL objects.
            MIL.MgraFree(GraphicList);
            MIL.MbufFree(MilImage);
            MIL.MedgeFree(MilEdgeContext);
            MIL.MedgeFree(MilEdgeResult);

            // Free defaults.
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL);
        }
    }
}
