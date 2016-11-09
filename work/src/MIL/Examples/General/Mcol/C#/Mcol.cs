//*****************************************************************************
//
// File name: Mcol.cs
//
// Synopsis:  This program contains 3 examples of the color module usage:
//
//            The first example performs color segmentation of an image
//            by classifying each pixel with one out of 7 color samples.
//            The ratio of each color in the image is then calculated.
//
//            The second example performs color matching of circular regions
//            in objects located with model finder.
//
//            The third example performs color separation in order to
//            separate 2 types of ink on a piece of paper.
//*****************************************************************************
using System;
using System.Collections.Generic;
using System.Text;
using Matrox.MatroxImagingLibrary;

namespace MCol
{
    class Program
    {
        // Display image margin
        const int DISPLAY_CENTER_MARGIN_X = 5;

        // Color patch sizes
        const int COLOR_PATCH_SIZEX = 30;
        const int COLOR_PATCH_SIZEY = 40;

        static void Main(string[] args)
        {
            //****************************************************************************
            // Main.
            //****************************************************************************

            MIL_ID MilApplication = MIL.M_NULL;     // Application identifier.
            MIL_ID MilSystem = MIL.M_NULL;          // System identifier.
            MIL_ID MilDisplay = MIL.M_NULL;         // Display identifier.

            // Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, ref MilApplication, ref MilSystem, ref MilDisplay, MIL.M_NULL, MIL.M_NULL);

            // Run the color segmentation example.
            ColorSegmentationExample(MilSystem, MilDisplay);

            // Run the color matching example.
            ColorMatchingExample(MilSystem, MilDisplay);

            // Run the color projection example.
            ColorSeparationExample(MilSystem, MilDisplay);

            // Free defaults.
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL);
        }

        //***************************************************************************
        // Color Segmentation using color samples. 
        //***************************************************************************

        // Image filenames
        const string CANDY_SAMPLE_IMAGE_FILE = MIL.M_IMAGE_PATH + "CandySamples.mim";
        const string CANDY_TARGET_IMAGE_FILE = MIL.M_IMAGE_PATH + "Candy.mim";

        // Number of samples
        const int NUM_SAMPLES = 6;

        // Draw spacing and offset
        const int CANDY_SAMPLES_XSPACING = 35;
        const int CANDY_SAMPLES_YOFFSET = 145;

        // Match parameters
        const int MATCH_MODE = MIL.M_MIN_DIST_VOTE;     // Minimal distance vote mode.
        const int DISTANCE_TYPE = MIL.M_MAHALANOBIS;    // Mahalanobis distance.
        const int TOLERANCE_MODE = MIL.M_SAMPLE_STDDEV; // Standard deviation tolerance mode.
        const double TOLERANCE_VALUE = 6.0;             // Mahalanobis tolerance value.
        const double RED_TOLERANCE_VALUE = 6.0;
        const double YELLOW_TOLERANCE_VALUE = 12.0;
        const double PINK_TOLERANCE_VALUE = 5.0;

        static void ColorSegmentationExample(MIL_ID MilSystem, MIL_ID MilDisplay)
        {
            MIL_ID SourceChild = MIL.M_NULL;            // Source image buffer identifier.
            MIL_ID DestChild = MIL.M_NULL;              // Dest image buffer identifier.
            MIL_ID MatchContext = MIL.M_NULL;           // Color matching context identifier.
            MIL_ID MatchResult = MIL.M_NULL;            // Color matching result identifier.
            MIL_ID DisplayImage = MIL.M_NULL;           // Display image buffer identifier.

            MIL_INT SourceSizeX = 0;
            MIL_INT SourceSizeY = 0;                    // Source image sizes
            MIL_INT SampleIndex = 0;
            MIL_INT SpacesIndex = 0;                    // Indices

            double MatchScore = 0.0;                    // Color matching score.

            // Blank spaces to align the samples names evenly.
            string[] Spaces = { "", " ", "  ", "   " };

            // Color samples names.
            string[] SampleNames = { "Green", "Red", "Yellow", "Purple", "Blue", "Pink" };

            // Color samples position: {OffsetX, OffsetY} 
            double[,] SamplesROI = new double[,] { { 58, 143 }, { 136, 148 }, { 217, 144 }, { 295, 142 }, { 367, 143 }, { 442, 147 } };

            // Color samples size.
            const double SampleSizeX = 36, SampleSizeY = 32;

            // Array for match sample colors.
            MIL_INT[,] SampleMatchColor = new MIL_INT[NUM_SAMPLES, 3];

            Console.Write("\nCOLOR SEGMENTATION:\n");
            Console.Write("-------------------\n");

            // Allocate the parent display image.
            MIL.MbufDiskInquire(CANDY_SAMPLE_IMAGE_FILE, MIL.M_SIZE_X, ref SourceSizeX);
            MIL.MbufDiskInquire(CANDY_SAMPLE_IMAGE_FILE, MIL.M_SIZE_Y, ref SourceSizeY);
            MIL.MbufAllocColor(MilSystem, 3, 2 * SourceSizeX + DISPLAY_CENTER_MARGIN_X, SourceSizeY, 8 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_DISP + MIL.M_PROC, ref DisplayImage);
            MIL.MbufClear(DisplayImage, MIL.M_COLOR_BLACK);

            // Create a source and dest child in the display image.
            MIL.MbufChild2d(DisplayImage, 0, 0, SourceSizeX, SourceSizeY, ref SourceChild);
            MIL.MbufChild2d(DisplayImage, SourceSizeX + DISPLAY_CENTER_MARGIN_X, 0, SourceSizeX, SourceSizeY, ref DestChild);

            // Load the source image into the source child.
            MIL.MbufLoad(CANDY_SAMPLE_IMAGE_FILE, SourceChild);

            // Allocate a color matching context.
            MIL.McolAlloc(MilSystem, MIL.M_COLOR_MATCHING, MIL.M_RGB, MIL.M_DEFAULT, MIL.M_DEFAULT, ref MatchContext);

            // Define each color sample in the context.
            for (int i = 0; i < NUM_SAMPLES; i++)
            {
                MIL.McolDefine(MatchContext, SourceChild, MIL.M_SAMPLE_LABEL(i + 1), MIL.M_IMAGE, SamplesROI[i, 0], SamplesROI[i, 1], SampleSizeX, SampleSizeY);
            }

            // Set the color matching parameters.
            MIL.McolSetMethod(MatchContext, MATCH_MODE, DISTANCE_TYPE, MIL.M_DEFAULT, MIL.M_DEFAULT);
            MIL.McolControl(MatchContext, MIL.M_CONTEXT, MIL.M_DISTANCE_TOLERANCE_MODE, TOLERANCE_MODE);
            MIL.McolControl(MatchContext, MIL.M_ALL, MIL.M_DISTANCE_TOLERANCE, TOLERANCE_VALUE);

            // Adjust tolerances for the red, yellow and pink samples.
            MIL.McolControl(MatchContext, MIL.M_SAMPLE_INDEX(1), MIL.M_DISTANCE_TOLERANCE, RED_TOLERANCE_VALUE);
            MIL.McolControl(MatchContext, MIL.M_SAMPLE_INDEX(2), MIL.M_DISTANCE_TOLERANCE, YELLOW_TOLERANCE_VALUE);
            MIL.McolControl(MatchContext, MIL.M_SAMPLE_INDEX(5), MIL.M_DISTANCE_TOLERANCE, PINK_TOLERANCE_VALUE);

            // Preprocess the context.
            MIL.McolPreprocess(MatchContext, MIL.M_DEFAULT);

            // Fill the samples colors array.
            for (int i = 0; i < NUM_SAMPLES; i++)
            {
                MIL.McolInquire(MatchContext, MIL.M_SAMPLE_LABEL(i + 1), MIL.M_MATCH_SAMPLE_COLOR_BAND_0 + MIL.M_TYPE_MIL_INT, ref SampleMatchColor[i, 0]);
                MIL.McolInquire(MatchContext, MIL.M_SAMPLE_LABEL(i + 1), MIL.M_MATCH_SAMPLE_COLOR_BAND_1 + MIL.M_TYPE_MIL_INT, ref SampleMatchColor[i, 1]);
                MIL.McolInquire(MatchContext, MIL.M_SAMPLE_LABEL(i + 1), MIL.M_MATCH_SAMPLE_COLOR_BAND_2 + MIL.M_TYPE_MIL_INT, ref SampleMatchColor[i, 2]);
            }

            // Draw the samples.
            DrawSampleColors(DestChild, SampleMatchColor, SampleNames, NUM_SAMPLES, CANDY_SAMPLES_XSPACING, CANDY_SAMPLES_YOFFSET);

            // Select the image buffer for display.
            MIL.MdispSelect(MilDisplay, DisplayImage);

            // Pause to show the original image.
            Console.Write("Color samples are defined for each possible candy color.\n");
            Console.Write("Press <Enter> to do color matching.\n\n");
            Console.ReadKey();

            // Load the target image.*/
            MIL.MbufClear(DisplayImage, MIL.M_COLOR_BLACK);
            MIL.MbufLoad(CANDY_TARGET_IMAGE_FILE, SourceChild);

            // Allocate a color matching result buffer.
            MIL.McolAllocResult(MilSystem, MIL.M_COLOR_MATCHING_RESULT, MIL.M_DEFAULT, ref MatchResult);

            // Enable controls to draw the labeled color image.
            MIL.McolControl(MatchContext, MIL.M_CONTEXT, MIL.M_GENERATE_PIXEL_MATCH, MIL.M_ENABLE);
            MIL.McolControl(MatchContext, MIL.M_CONTEXT, MIL.M_GENERATE_SAMPLE_COLOR_LUT, MIL.M_ENABLE);

            // Match with target image.
            MIL.McolMatch(MatchContext, SourceChild, MIL.M_DEFAULT, MIL.M_NULL, MatchResult, MIL.M_DEFAULT);

            // Retrieve and display results.
            Console.Write("Each pixel of the mixture is matched " +
                      "with one of the color samples.\n");
            Console.Write("\nColor segmentation results:\n");
            Console.Write("---------------------------\n");

            for (SampleIndex = 0; SampleIndex < NUM_SAMPLES; SampleIndex++)
            {
                MIL.McolGetResult(MatchResult, MIL.M_DEFAULT, MIL.M_SAMPLE_INDEX((int)SampleIndex), MIL.M_SCORE, ref MatchScore);
                SpacesIndex = 6 - SampleNames[SampleIndex].Length;
                Console.Write("Ratio of {0}{1} sample = {2,5:0.00}%\n", SampleNames[SampleIndex], Spaces[SpacesIndex], MatchScore);
            }
            Console.Write("\nResults reveal the low proportion of Blue candy.\n");

            // Draw the colored label image in the destination child.
            MIL.McolDraw(MIL.M_DEFAULT, MatchResult, DestChild, MIL.M_DRAW_PIXEL_MATCH_USING_COLOR, MIL.M_ALL, MIL.M_ALL, MIL.M_DEFAULT);

            // Pause to show the result image.
            Console.Write("\nPress <Enter> to end.\n\n");
            Console.ReadKey();

            // Free all allocations.
            MIL.MbufFree(DestChild);
            MIL.MbufFree(SourceChild);
            MIL.MbufFree(DisplayImage);
            MIL.McolFree(MatchResult);
            MIL.McolFree(MatchContext);
        }

        //****************************************************************************
        // Color matching in labeled regions.
        //****************************************************************************
        // Image filenames
        const string FUSE_SAMPLES_IMAGE = MIL.M_IMAGE_PATH + "FuseSamples.mim";
        const string FUSE_TARGET_IMAGE = MIL.M_IMAGE_PATH + "Fuse.mim";

        // Model Finder context filename
        const string FINDER_CONTEXT = MIL.M_IMAGE_PATH + "FuseModel.mmf";

        // Number of fuse sample objects
        const int NUM_FUSES = 4;

        // Draw spacing and offset
        const int FUSE_SAMPLES_XSPACING = 40;
        const int FUSE_SAMPLES_YOFFSET = 145;

        static void ColorMatchingExample(MIL_ID MilSystem, MIL_ID MilDisplay)
        {
            MIL_ID DisplayImage = MIL.M_NULL;           // Display image buffer identifier.
            MIL_ID SourceChild = MIL.M_NULL;            // Source image buffer identifier.
            MIL_ID DestChild = MIL.M_NULL;              // Dest image buffer identifier.
            MIL_ID ColMatchContext = MIL.M_NULL;        // Color matching context identifier.
            MIL_ID ColMatchResult = MIL.M_NULL;         // Color matching result identifier.
            MIL_ID ModelImage = MIL.M_NULL;             // Model image buffer identifier.
            MIL_ID AreaImage = MIL.M_NULL;              // Area  image buffer identifier.
            MIL_ID OverlayID = MIL.M_NULL;              // Overlay image buffer identifier.
            MIL_ID OverlaySourceChild = MIL.M_NULL;     // Overlay source child identifier.
            MIL_ID OverlayDestChild = MIL.M_NULL;       // Overlay dest child identifier.
            MIL_ID FuseFinderCtx = MIL.M_NULL;          // Model finder context identifier.
            MIL_ID FuseFinderRes = MIL.M_NULL;          // Model finder result identifier.

            // Image sizes
            MIL_INT SizeX = 0;
            MIL_INT SizeY = 0;

            // Color sample names
            string[] SampleNames =                                    { "Green", " Blue", " Red", "Yellow" };

            // Sample ROIs coordinates: OffsetX, OffsetY, SizeX, SizeY
            MIL_INT[,] SampleROIs = new MIL_INT[,]{{ 54, 139, 28, 14},
                                               {172, 137, 30, 23},
                                               {296, 135, 31, 23},
                                               {417, 134, 27, 22}};

            // Array of match sample colors.
            MIL_INT[,] SampleMatchColor = new MIL_INT[NUM_FUSES, 3];

            Console.Write("\nCOLOR IDENTIFICATION:\n");
            Console.Write("---------------------\n");

            // Allocate the parent display image.
            MIL.MbufDiskInquire(FUSE_TARGET_IMAGE, MIL.M_SIZE_X, ref SizeX);
            MIL.MbufDiskInquire(FUSE_TARGET_IMAGE, MIL.M_SIZE_Y, ref SizeY);
            MIL.MbufAllocColor(MilSystem, 3, 2 * SizeX + DISPLAY_CENTER_MARGIN_X, SizeY, 8 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_DISP + MIL.M_PROC, ref DisplayImage);
            MIL.MbufClear(DisplayImage, MIL.M_COLOR_BLACK);

            // Allocate the model, area and label images.
            MIL.MbufAlloc2d(MilSystem, SizeX, SizeY, 8 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_PROC + MIL.M_DISP, ref ModelImage);
            MIL.MbufAlloc2d(MilSystem, SizeX, SizeY, 8 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_PROC + MIL.M_DISP, ref AreaImage);

            // Create a source and destination child in the display image.
            MIL.MbufChild2d(DisplayImage, 0, 0, SizeX, SizeY, ref SourceChild);
            MIL.MbufChild2d(DisplayImage, SizeX + DISPLAY_CENTER_MARGIN_X, 0, SizeX, SizeY, ref DestChild);

            // Load the sample source image.
            MIL.MbufLoad(FUSE_SAMPLES_IMAGE, SourceChild);

            // Display the image buffer.
            MIL.MdispSelect(MilDisplay, DisplayImage);

            // Prepare the overlay.
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY, MIL.M_ENABLE);
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY_CLEAR, MIL.M_DEFAULT);
            MIL.MdispInquire(MilDisplay, MIL.M_OVERLAY_ID, ref OverlayID);
            MIL.MbufChild2d(OverlayID, 0, 0, SizeX, SizeY, ref OverlaySourceChild);
            MIL.MbufChild2d(OverlayID, SizeX + DISPLAY_CENTER_MARGIN_X, 0, SizeX, SizeY, ref OverlayDestChild);

            // Prepare the model finder context and result.
            MIL.MmodRestore(FINDER_CONTEXT, MilSystem, MIL.M_DEFAULT, ref FuseFinderCtx);
            MIL.MmodPreprocess(FuseFinderCtx, MIL.M_DEFAULT);
            MIL.MmodAllocResult(MilSystem, MIL.M_DEFAULT, ref FuseFinderRes);

            // Allocate a color match context and result.
            MIL.McolAlloc(MilSystem, MIL.M_COLOR_MATCHING, MIL.M_RGB, MIL.M_DEFAULT, MIL.M_DEFAULT, ref ColMatchContext);
            MIL.McolAllocResult(MilSystem, MIL.M_COLOR_MATCHING_RESULT, MIL.M_DEFAULT, ref ColMatchResult);

            // Define the color samples in the context.
            for (int i = 0; i < NUM_FUSES; i++)
            {
                MIL.McolDefine(ColMatchContext, SourceChild, MIL.M_SAMPLE_LABEL(i + 1), MIL.M_IMAGE, (double)SampleROIs[i, 0], (double)SampleROIs[i, 1], (double)SampleROIs[i, 2], (double)SampleROIs[i, 3]);
            }

            // Preprocess the context.
            MIL.McolPreprocess(ColMatchContext, MIL.M_DEFAULT);

            // Fill the samples colors array.
            for (int i = 0; i < NUM_FUSES; i++)
            {
                MIL.McolInquire(ColMatchContext, MIL.M_SAMPLE_LABEL(i + 1), MIL.M_MATCH_SAMPLE_COLOR_BAND_0 + MIL.M_TYPE_MIL_INT, ref SampleMatchColor[i, 0]);
                MIL.McolInquire(ColMatchContext, MIL.M_SAMPLE_LABEL(i + 1), MIL.M_MATCH_SAMPLE_COLOR_BAND_1 + MIL.M_TYPE_MIL_INT, ref SampleMatchColor[i, 1]);
                MIL.McolInquire(ColMatchContext, MIL.M_SAMPLE_LABEL(i + 1), MIL.M_MATCH_SAMPLE_COLOR_BAND_2 + MIL.M_TYPE_MIL_INT, ref SampleMatchColor[i, 2]);
            }

            // Draw the color samples.
            DrawSampleColors(DestChild, SampleMatchColor, SampleNames, NUM_FUSES, FUSE_SAMPLES_XSPACING, FUSE_SAMPLES_YOFFSET);

            // Draw the sample ROIs in the source image overlay.
            MIL.MgraColor(MIL.M_DEFAULT, MIL.M_COLOR_RED);
            for (MIL_INT SampleIndex = 0; SampleIndex < NUM_FUSES; SampleIndex++)
            {
                MIL_INT XEnd = SampleROIs[SampleIndex, 0] + SampleROIs[SampleIndex, 2] - 1;
                MIL_INT YEnd = SampleROIs[SampleIndex, 1] + SampleROIs[SampleIndex, 3] - 1;
                MIL.MgraRect(MIL.M_DEFAULT, OverlaySourceChild, SampleROIs[SampleIndex, 0], SampleROIs[SampleIndex, 1], XEnd, YEnd);
            }

            // Pause to show the source image.
            Console.Write("Colors are defined using one color sample region per fuse.\n");
            Console.Write("Press <Enter> to process the target image.\n");
            Console.ReadKey();

            // Clear the overlay.
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY_CLEAR, MIL.M_DEFAULT);

            // Load the target image into the source child.
            MIL.MbufLoad(FUSE_TARGET_IMAGE, SourceChild);

            // Get the grayscale model image and copy it into the display dest child.
            MIL.MimConvert(SourceChild, ModelImage, MIL.M_RGB_TO_L);
            MIL.MbufCopy(ModelImage, DestChild);

            // Find the Model.
            MIL.MmodFind(FuseFinderCtx, ModelImage, FuseFinderRes);

            // Draw the blob image: labeled circular areas centered at each found fuse occurrence.
            MIL_INT Number = 0;
            MIL.MmodGetResult(FuseFinderRes, MIL.M_DEFAULT, MIL.M_NUMBER + MIL.M_TYPE_MIL_INT, ref Number);
            MIL.MbufClear(AreaImage, 0);
            for (MIL_INT ii = 0; ii < Number; ii++)
            {
                double X = 0.0;
                double Y = 0.0;

                // Get the position
                MIL.MmodGetResult(FuseFinderRes, ii, MIL.M_POSITION_X, ref X);
                MIL.MmodGetResult(FuseFinderRes, ii, MIL.M_POSITION_Y, ref Y);

                // Set the label color
                MIL.MgraColor(MIL.M_DEFAULT, (double)ii + 1);

                // Draw the filled circle
                MIL.MgraArcFill(MIL.M_DEFAULT, AreaImage, X, Y, 20, 20, 0, 360);
            }

            // Enable controls to draw the labeled color image.
            MIL.McolControl(ColMatchContext, MIL.M_CONTEXT, MIL.M_SAVE_AREA_IMAGE, MIL.M_ENABLE);
            MIL.McolControl(ColMatchContext, MIL.M_CONTEXT, MIL.M_GENERATE_SAMPLE_COLOR_LUT, MIL.M_ENABLE);

            // Perform the color matching.
            MIL.McolMatch(ColMatchContext, SourceChild, MIL.M_DEFAULT, AreaImage, ColMatchResult, MIL.M_DEFAULT);

            // Draw the label image into the overlay child.
            MIL.McolDraw(MIL.M_DEFAULT, ColMatchResult, OverlayDestChild, MIL.M_DRAW_AREA_MATCH_USING_COLOR, MIL.M_ALL, MIL.M_ALL, MIL.M_DEFAULT);

            // Draw the model position over the colored areas.
            MIL.MgraColor(MIL.M_DEFAULT, MIL.M_COLOR_BLUE);
            MIL.MmodDraw(MIL.M_DEFAULT, FuseFinderRes, OverlayDestChild, MIL.M_DRAW_BOX + MIL.M_DRAW_POSITION, MIL.M_ALL, MIL.M_DEFAULT);

            // Enable the display update.
            MIL.MdispControl(MilDisplay, MIL.M_UPDATE, MIL.M_ENABLE);

            // Pause to show the resulting image.
            Console.Write("\nFuses are located using the Model Finder tool.\n");
            Console.Write("The color of each target area is identified.\n");
            Console.Write("Press <Enter> to end.\n");
            Console.ReadKey();

            // Free all allocations.
            MIL.MmodFree(FuseFinderRes);
            MIL.MmodFree(FuseFinderCtx);
            MIL.MbufFree(AreaImage);
            MIL.MbufFree(ModelImage);
            MIL.MbufFree(SourceChild);
            MIL.MbufFree(DestChild);
            MIL.MbufFree(OverlaySourceChild);
            MIL.MbufFree(OverlayDestChild);
            MIL.MbufFree(DisplayImage);
            MIL.McolFree(ColMatchContext);
            MIL.McolFree(ColMatchResult);
        }

        //****************************************************************************
        // Perform color separation of colored inks on a piece of paper.
        //****************************************************************************
        // Source image
        const string WRITING_IMAGE_FILE = MIL.M_IMAGE_PATH + "stamp.mim";

        // Color triplets
        static readonly MIL_INT[] BACKGROUND_COLOR ={ 245, 234, 206 };
        static readonly MIL_INT[] WRITING_COLOR ={ 141, 174, 174 };
        static readonly MIL_INT[] STAMP_COLOR = { 226, 150, 118 };

        // Drawing spacing
        const int PATCHES_XSPACING = 70;

        static void ColorSeparationExample(MIL_ID MilSystem, MIL_ID MilDisplay)
        {
            MIL_ID DisplayImage = MIL.M_NULL;           // Display image buffer identifier.
            MIL_ID SourceChild = MIL.M_NULL;            // Source image buffer identifier.
            MIL_ID DestChild = MIL.M_NULL;              // Destination image buffer identifier.
            MIL_ID Child = MIL.M_NULL;                  // Child buffer identifier.
            MIL_ID ColorsArray = MIL.M_NULL;            // Array buffer identifier.

            // Source image sizes.
            MIL_INT SourceSizeX = 0;
            MIL_INT SourceSizeY = 0;

            // Color samples' names
            string[] ColorNames = { "BACKGROUND", "WRITING", "STAMP" };

            // Array with color patches to draw.
            MIL_INT[,] Colors = new MIL_INT[,] { { 245, 234, 206 }, { 141, 174, 174 }, { 226, 150, 118 } };

            // Samples' color coordinates
            byte[] BackgroundColor = new byte[3] { 245, 234, 206 };
            byte[] SelectedColor = new byte[3] { 141, 174, 174 };
            byte[] RejectedColor = new byte[3] { 226, 150, 118 };

            Console.Write("\nCOLOR SEPARATION:\n");
            Console.Write("-----------------\n");

            // Allocate an array buffer and fill it with the color coordinates.
            MIL.MbufAlloc2d(MilSystem, 3, 3, 8 + MIL.M_UNSIGNED, MIL.M_ARRAY, ref ColorsArray);
            MIL.MbufPut2d(ColorsArray, 0, 0, 3, 1, BackgroundColor);
            MIL.MbufPut2d(ColorsArray, 0, 1, 3, 1, SelectedColor);
            MIL.MbufPut2d(ColorsArray, 0, 2, 3, 1, RejectedColor);

            // Allocate the parent display image.
            MIL.MbufDiskInquire(WRITING_IMAGE_FILE, MIL.M_SIZE_X, ref SourceSizeX);
            MIL.MbufDiskInquire(WRITING_IMAGE_FILE, MIL.M_SIZE_Y, ref SourceSizeY);
            MIL.MbufAllocColor(MilSystem, 3, 2 * SourceSizeX + DISPLAY_CENTER_MARGIN_X, SourceSizeY, 8 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_DISP + MIL.M_PROC, ref DisplayImage);
            MIL.MbufClear(DisplayImage, MIL.M_COLOR_BLACK);

            // Clear the overlay.
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY_CLEAR, MIL.M_DEFAULT);

            // Create a source and dest child in the display image
            MIL.MbufChild2d(DisplayImage, 0, 0, SourceSizeX, SourceSizeY, ref SourceChild);
            MIL.MbufChild2d(DisplayImage, SourceSizeX + DISPLAY_CENTER_MARGIN_X, 0, SourceSizeX, SourceSizeY, ref DestChild);

            // Load the source image into the display image source child.
            MIL.MbufLoad(WRITING_IMAGE_FILE, SourceChild);

            // Draw the color patches.
            DrawSampleColors(DestChild, Colors, ColorNames, 3, PATCHES_XSPACING, -1);

            // Display the image.
            MIL.MdispSelect(MilDisplay, DisplayImage);

            // Pause to show the source image and color patches.
            Console.Write("The writing will be separated from the stamp using the following triplets:\n");
            Console.Write("the background color: beige [{0}, {1}, {2}],\n", BackgroundColor[0], BackgroundColor[1], BackgroundColor[2]);
            Console.Write("the writing color   : green [{0}, {1}, {2}],\n", SelectedColor[0], SelectedColor[1], SelectedColor[2]);
            Console.Write("the stamp color     : red   [{0}, {1}, {2}].\n\n", RejectedColor[0], RejectedColor[1], RejectedColor[2]);
            Console.Write("Press <Enter> to extract the writing.\n\n");
            Console.ReadKey();

            // Perform the color projection.
            MIL.McolProject(SourceChild, ColorsArray, DestChild, MIL.M_NULL, MIL.M_COLOR_SEPARATION, MIL.M_DEFAULT, MIL.M_NULL);

            // Wait for a key.
            Console.Write("Press <Enter> to extract the stamp.\n\n");
            Console.ReadKey();

            // Switch the order of the selected vs rejected colors in the color array.
            MIL.MbufPut2d(ColorsArray, 0, 2, 3, 1, SelectedColor);
            MIL.MbufPut2d(ColorsArray, 0, 1, 3, 1, RejectedColor);

            // Perform the color projection.
            MIL.McolProject(SourceChild, ColorsArray, DestChild, MIL.M_NULL, MIL.M_COLOR_SEPARATION, MIL.M_DEFAULT, MIL.M_NULL);

            // Wait for a key.
            Console.Write("Press <Enter> to end.\n\n");
            Console.ReadKey();

            // Free all allocations.
            MIL.MbufFree(ColorsArray);
            MIL.MbufFree(SourceChild);
            MIL.MbufFree(DestChild);
            MIL.MbufFree(DisplayImage);
        }

        //****************************************************************************
        // Draw the samples as color patches.
        static void DrawSampleColors(MIL_ID DestImage, MIL_INT[,] pSamplesColors, string[] pSampleNames, MIL_INT NumSamples, MIL_INT XSpacing, MIL_INT YOffset)
        {
            MIL_INT DestSizeX = MIL.MbufInquire(DestImage, MIL.M_SIZE_X, MIL.M_NULL);
            MIL_INT DestSizeY = MIL.MbufInquire(DestImage, MIL.M_SIZE_Y, MIL.M_NULL);
            double OffsetX = (DestSizeX - (NumSamples * COLOR_PATCH_SIZEX) - ((NumSamples - 1) * XSpacing)) / 2.0;
            double OffsetY = YOffset > 0 ? YOffset : (DestSizeY - COLOR_PATCH_SIZEY) / 2.0;
            double TextOffsetX;
            MIL.MgraFont(MIL.M_DEFAULT, MIL.M_FONT_DEFAULT_SMALL);

            for (MIL_INT SampleIndex = 0; SampleIndex < NumSamples; SampleIndex++)
            {
                MIL.MgraColor(MIL.M_DEFAULT, MIL.M_RGB888((int)pSamplesColors[SampleIndex, 0], (int)pSamplesColors[SampleIndex, 1], (int)pSamplesColors[SampleIndex, 2]));
                MIL.MgraRectFill(MIL.M_DEFAULT, DestImage, OffsetX, OffsetY, OffsetX + COLOR_PATCH_SIZEX, OffsetY + COLOR_PATCH_SIZEY);
                MIL.MgraColor(MIL.M_DEFAULT, MIL.M_COLOR_YELLOW);
                TextOffsetX = OffsetX + COLOR_PATCH_SIZEX / 2.0 - 4.0 * pSampleNames[SampleIndex].Length + 0.5;
                MIL.MgraText(MIL.M_DEFAULT, DestImage, TextOffsetX, OffsetY - 20, pSampleNames[SampleIndex]);
                OffsetX += (COLOR_PATCH_SIZEX + XSpacing);
            }
        }

    }
}
