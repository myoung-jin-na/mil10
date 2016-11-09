//******************************************************************************
//
// File name: Mblob.cs
//
// Synopsis:  This program loads an image of some nuts, bolts and washers, 
//            determines the number of each of these, finds and marks
//            their center of gravity using the Blob analysis module.
//******************************************************************************
using System;
using System.Collections.Generic;
using System.Text;

using Matrox.MatroxImagingLibrary;

namespace MBlob
{
    class Program
    {
        // Target MIL image file specifications.
        private const string IMAGE_FILE = MIL.M_IMAGE_PATH + "BoltsNutsWashers.mim";
        private const int IMAGE_THRESHOLD_VALUE = 26;

        // Minimum and maximum area of blobs.
        private const int MIN_BLOB_AREA = 50;
        private const int MAX_BLOB_AREA = 50000;

        // Radius of the smallest particles to keep.
        private const int MIN_BLOB_RADIUS = 3;

        // Minimum hole compactness corresponding to a washer.
        private const double MIN_COMPACTNESS = 1.5;
        static void Main(string[] args)
        {

            MIL_ID MilApplication = MIL.M_NULL;                 // Application identifier.
            MIL_ID MilSystem = MIL.M_NULL;                      // System identifier.
            MIL_ID MilDisplay = MIL.M_NULL;                     // Display identifier.
            MIL_ID MilImage = MIL.M_NULL;                       // Image buffer identifier.
            MIL_ID MilGraphicList = MIL.M_NULL;                 // Graphic list identifier.
            MIL_ID MilBinImage = MIL.M_NULL;                    // Binary image buffer identifier.
            MIL_ID MilBlobResult = MIL.M_NULL;                  // Blob result buffer identifier.
            MIL_ID MilBlobFeatureList = MIL.M_NULL;             // Feature list identifier.
            MIL_INT TotalBlobs = 0;                             // Total number of blobs.
            MIL_INT BlobsWithHoles = 0;                         // Number of blobs with holes.
            MIL_INT BlobsWithRoughHoles = 0;                    // Number of blobs with rough holes.
            MIL_INT n = 0;                                      // Counter.
            MIL_INT SizeX = 0;                                  // Size X of the source buffer
            MIL_INT SizeY = 0;                                  // Size Y of the source buffer
            double[] CogX = null;                               // X coordinate of center of gravity.
            double[] CogY = null;                               // Y coordinate of center of gravity.

            // Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, ref MilApplication, ref MilSystem, ref MilDisplay, MIL.M_NULL, MIL.M_NULL);

            // Restore source image into image buffer.
            MIL.MbufRestore(IMAGE_FILE, MilSystem, ref MilImage);

            // Allocate a graphic list to hold the subpixel annotations to draw.
            MIL.MgraAllocList(MilSystem, MIL.M_DEFAULT, ref MilGraphicList);

            // Associate the graphic list to the display.
            MIL.MdispControl(MilDisplay, MIL.M_ASSOCIATED_GRAPHIC_LIST_ID, MilGraphicList);

            // Display the buffer.
            MIL.MdispSelect(MilDisplay, MilImage);

            // Allocate a binary image buffer for fast processing.
            MIL.MbufInquire(MilImage, MIL.M_SIZE_X, ref SizeX);
            MIL.MbufInquire(MilImage, MIL.M_SIZE_Y, ref SizeY);
            MIL.MbufAlloc2d(MilSystem, SizeX, SizeY, 1 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_PROC, ref MilBinImage);

            // Pause to show the original image.
            Console.Write("\nBLOB ANALYSIS:\n");
            Console.Write("--------------\n\n");
            Console.Write("This program determines the number of bolts, nuts and washers\n");
            Console.Write("in the image and finds their center of gravity.\n");
            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();

            // Binarize image.
            MIL.MimBinarize(MilImage, MilBinImage, MIL.M_FIXED + MIL.M_GREATER_OR_EQUAL, IMAGE_THRESHOLD_VALUE, MIL.M_NULL);

            // Remove small particles and then remove small holes.
            MIL.MimOpen(MilBinImage, MilBinImage, MIN_BLOB_RADIUS, MIL.M_BINARY);
            MIL.MimClose(MilBinImage, MilBinImage, MIN_BLOB_RADIUS, MIL.M_BINARY);

            // Allocate a feature list.
            MIL.MblobAllocFeatureList(MilSystem, ref MilBlobFeatureList);

            // Enable the Area and Center Of Gravity feature calculation.
            MIL.MblobSelectFeature(MilBlobFeatureList, MIL.M_AREA);
            MIL.MblobSelectFeature(MilBlobFeatureList, MIL.M_CENTER_OF_GRAVITY);

            // Allocate a blob result buffer.
            MIL.MblobAllocResult(MilSystem, ref MilBlobResult);

            // Calculate selected features for each blob.
            MIL.MblobCalculate(MilBinImage, MIL.M_NULL, MilBlobFeatureList, MilBlobResult);

            // Exclude blobs whose area is too small.
            MIL.MblobSelect(MilBlobResult, MIL.M_EXCLUDE, MIL.M_AREA, MIL.M_LESS_OR_EQUAL, MIN_BLOB_AREA, MIL.M_NULL);

            // Get the total number of selected blobs.
            MIL.MblobGetNumber(MilBlobResult, ref TotalBlobs);
            Console.Write("There are {0} objects ", TotalBlobs);

            // Read and print the blob's center of gravity.
            CogX = new double[TotalBlobs];
            CogY = new double[TotalBlobs];
            if (CogX != null && CogY != null)
            {
                // Get the results.
                MIL.MblobGetResult(MilBlobResult, MIL.M_CENTER_OF_GRAVITY_X, CogX);
                MIL.MblobGetResult(MilBlobResult, MIL.M_CENTER_OF_GRAVITY_Y, CogY);

                // Print the center of gravity of each blob.
                Console.Write("and their centers of gravity are:\n");
                for (n = 0; n < TotalBlobs; n++)
                {
                    Console.Write("Blob #{0}: X={1,5:0.0}, Y={2,5:0.0}\n", n, CogX[n], CogY[n]);
                }

            }
            else
            {
                Console.Write("\nError: Not enough memory.\n");
            }

            // Draw a cross at the center of gravity of each blob.
            MIL.MgraColor(MIL.M_DEFAULT, MIL.M_COLOR_RED);
            MIL.MblobDraw(MIL.M_DEFAULT, MilBlobResult, MilGraphicList, MIL.M_DRAW_CENTER_OF_GRAVITY, MIL.M_INCLUDED_BLOBS, MIL.M_DEFAULT);

            // Reverse what is considered to be the background so that
            // holes are seen as being blobs.

            MIL.MblobControl(MilBlobResult, MIL.M_FOREGROUND_VALUE, MIL.M_ZERO);

            // Add a feature to distinguish between types of holes.Since area
            // has already been added to the feature list, and the processing 
            // mode has been changed, all blobs will be re-included and the area 
            // of holes will be calculated automatically.

            MIL.MblobSelectFeature(MilBlobFeatureList, MIL.M_COMPACTNESS);

            // Calculate selected features for each blob.
            MIL.MblobCalculate(MilBinImage, MIL.M_NULL, MilBlobFeatureList, MilBlobResult);

            // Exclude small holes and large (the area around objects) holes.
            MIL.MblobSelect(MilBlobResult, MIL.M_EXCLUDE, MIL.M_AREA, MIL.M_OUT_RANGE, MIN_BLOB_AREA, MAX_BLOB_AREA);

            // Get the number of blobs with holes.
            MIL.MblobGetNumber(MilBlobResult, ref BlobsWithHoles);

            // Exclude blobs whose holes are compact (i.e.nuts).
            MIL.MblobSelect(MilBlobResult, MIL.M_EXCLUDE, MIL.M_COMPACTNESS, MIL.M_LESS_OR_EQUAL, MIN_COMPACTNESS, MIL.M_NULL);

            // Get the number of blobs with holes that are NOT compact.
            MIL.MblobGetNumber(MilBlobResult, ref BlobsWithRoughHoles);

            // Print results.
            Console.Write("\nIdentified objects:\n");
            Console.Write("{0} bolts\n", TotalBlobs - BlobsWithHoles);
            Console.Write("{0} nuts\n", BlobsWithHoles - BlobsWithRoughHoles);
            Console.Write("{0} washers\n\n", BlobsWithRoughHoles);
            Console.Write("Press <Enter> to end.\n\n");
            Console.ReadKey();

            // Free all allocations.
            MIL.MgraFree(MilGraphicList);
            MIL.MblobFree(MilBlobResult);
            MIL.MblobFree(MilBlobFeatureList);
            MIL.MbufFree(MilBinImage);
            MIL.MbufFree(MilImage);
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL);
        }
    }
}
