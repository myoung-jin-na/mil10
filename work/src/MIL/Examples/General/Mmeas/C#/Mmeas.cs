//*****************************************************************************
//
// File name: Mmeas.cs
//
// Synopsis:  This program consists of 3 examples that use the Measurement module 
//            to calculate the position, width and angle of objects in an image. 
//            The first one measures the position, width and angle of a stripe
//            in an image, and marks its center and edges. The second one measures
//            the average position, width and angle of a row of pins on a chip.
//            Finally the third example uses the fixturing capability to measure
//            the gap width of objects relative to the object's positions.
//*****************************************************************************
using System;
using System.Collections.Generic;
using System.Text;

using Matrox.MatroxImagingLibrary;

namespace MMeas
{
    class Program
    {
        // Example selection.
        private const int RUN_SINGLE_MEASUREMENT_EXAMPLE = MIL.M_YES;
        private const int RUN_MULTIPLE_MEASUREMENT_EXAMPLE = MIL.M_YES;
        private const int RUN_FIXTURED_MEASUREMENT_EXAMPLE = MIL.M_YES;

        static int Main(string[] args)
        {
            MIL_ID MilApplication = MIL.M_NULL;     // Application identifier.
            MIL_ID MilSystem = MIL.M_NULL;          // System Identifier.
            MIL_ID MilDisplay = MIL.M_NULL;         // Display identifier.

            // Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, ref MilApplication, ref MilSystem, ref MilDisplay, MIL.M_NULL, MIL.M_NULL);

            // Print module name.
            Console.Write("\nMEASUREMENT MODULE:\n");
            Console.Write("-------------------\n\n");

            if (RUN_SINGLE_MEASUREMENT_EXAMPLE == MIL.M_YES)
            {
                SingleMeasurementExample(MilSystem, MilDisplay);
            }

            if (RUN_MULTIPLE_MEASUREMENT_EXAMPLE == MIL.M_YES)
            {
                MultipleMeasurementExample(MilSystem, MilDisplay);
            }

            if (RUN_MULTIPLE_MEASUREMENT_EXAMPLE == MIL.M_YES)
            {
                FixturedMeasurementExample(MilSystem, MilDisplay);
            }

            // Free defaults.
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL);

            return 0;
        }

        //****************************************************************************
        //Single measurement example. 
        //****************************************************************************

        // Source MIL image file specification.
        private const string MEAS_IMAGE_FILE = MIL.M_IMAGE_PATH + "lead.mim";

        // Processing region specification.
        private const int MEAS_BOX_WIDTH = 128;
        private const int MEAS_BOX_HEIGHT = 100;
        private const int MEAS_BOX_POS_X = 166;
        private const int MEAS_BOX_POS_Y = 130;

        // Target stripe typical specifications.
        private const int STRIPE_POLARITY_LEFT = MIL.M_POSITIVE;
        private const int STRIPE_POLARITY_RIGHT = MIL.M_NEGATIVE;
        private const int STRIPE_WIDTH = 45;
        private const int STRIPE_WIDTH_VARIATION = 10;

        static void SingleMeasurementExample(MIL_ID MilSystem, MIL_ID MilDisplay)
        {
            MIL_ID MilImage = MIL.M_NULL;                   // Image buffer identifier.
            MIL_ID MilGraphicList = MIL.M_NULL;             // Graphic list identifier.
            MIL_ID StripeMarker = MIL.M_NULL;               // Stripe marker identifier.
            double StripeCenterX = 0.0;                     // Stripe X center position.
            double StripeCenterY = 0.0;                     // Stripe Y center position.
            double StripeWidth = 0.0;                       // Stripe width.
            double StripeAngle = 0.0;                       // Stripe angle.
            double CrossColor = MIL.M_COLOR_YELLOW;         // Cross drawing color.
            double BoxColor = MIL.M_COLOR_RED;              // Box drawing color.

            // Restore and display the source image.
            MIL.MbufRestore(MEAS_IMAGE_FILE, MilSystem, ref MilImage);
            MIL.MdispSelect(MilDisplay, MilImage);

            // Allocate a graphic list to hold the subpixel annotations to draw.
            MIL.MgraAllocList(MilSystem, MIL.M_DEFAULT, ref MilGraphicList);

            // Associate the graphic list to the display.
            MIL.MdispControl(MilDisplay, MIL.M_ASSOCIATED_GRAPHIC_LIST_ID, MilGraphicList);

            // Allocate a stripe marker.
            MIL.MmeasAllocMarker(MilSystem, MIL.M_STRIPE, MIL.M_DEFAULT, ref StripeMarker);

            // Specify the stripe characteristics.
            MIL.MmeasSetMarker(StripeMarker, MIL.M_POLARITY, STRIPE_POLARITY_LEFT, STRIPE_POLARITY_RIGHT);

            // Set score function to find the expected width.
            MIL.MmeasSetScore(StripeMarker,
                MIL.M_STRIPE_WIDTH_SCORE,
                STRIPE_WIDTH - STRIPE_WIDTH_VARIATION,
                STRIPE_WIDTH - STRIPE_WIDTH_VARIATION,
                STRIPE_WIDTH + STRIPE_WIDTH_VARIATION,
                STRIPE_WIDTH + STRIPE_WIDTH_VARIATION,
                MIL.M_DEFAULT,
                MIL.M_DEFAULT,
                MIL.M_DEFAULT);

            MIL.MmeasSetMarker(StripeMarker, MIL.M_BOX_ANGLE_MODE, MIL.M_ENABLE, MIL.M_NULL);

            // Specify the search region size and position.
            MIL.MmeasSetMarker(StripeMarker, MIL.M_BOX_ORIGIN, MEAS_BOX_POS_X, MEAS_BOX_POS_Y);
            MIL.MmeasSetMarker(StripeMarker, MIL.M_BOX_SIZE, MEAS_BOX_WIDTH, MEAS_BOX_HEIGHT);

            // Draw the contour of the measurement region.
            MIL.MgraColor(MIL.M_DEFAULT, BoxColor);
            MIL.MmeasDraw(MIL.M_DEFAULT, StripeMarker, MilGraphicList, MIL.M_DRAW_SEARCH_REGION, MIL.M_DEFAULT, MIL.M_MARKER);

            // Pause to show the original image.
            Console.Write("Position, width and angle of the stripe in the highlighted box\n");
            Console.Write("will be calculated and the center and edges will be marked.\n");
            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();

            // Clear the annotations.
            MIL.MgraClear(MIL.M_DEFAULT, MilGraphicList);

            // Find the stripe and measure its width and angle.
            MIL.MmeasFindMarker(MIL.M_DEFAULT, MilImage, StripeMarker, MIL.M_DEFAULT);

            // Get the stripe position, width and angle.
            MIL.MmeasGetResult(StripeMarker, MIL.M_POSITION, ref StripeCenterX, ref StripeCenterY);
            MIL.MmeasGetResult(StripeMarker, MIL.M_STRIPE_WIDTH, ref StripeWidth);
            MIL.MmeasGetResult(StripeMarker, MIL.M_ANGLE, ref StripeAngle);

            // Draw the contour of the found measurement box.
            MIL.MgraColor(MIL.M_DEFAULT, BoxColor);
            MIL.MmeasDraw(MIL.M_DEFAULT, StripeMarker, MilGraphicList, MIL.M_DRAW_BOX, MIL.M_DEFAULT, MIL.M_RESULT);

            // Draw a cross on the center, left edge and right edge of the found stripe.
            MIL.MgraColor(MIL.M_DEFAULT, CrossColor);
            MIL.MmeasDraw(MIL.M_DEFAULT, StripeMarker, MilGraphicList, MIL.M_DRAW_POSITION, MIL.M_DEFAULT, MIL.M_RESULT);
            MIL.MmeasDraw(MIL.M_DEFAULT, StripeMarker, MilGraphicList, MIL.M_DRAW_POSITION + MIL.M_EDGE_FIRST + MIL.M_EDGE_SECOND, MIL.M_DEFAULT, MIL.M_RESULT);

            // Print the result.
            Console.Write("The stripe in the image is at position {0:0.00},{1:0.00} and\n", StripeCenterX, StripeCenterY);
            Console.Write("is {0:0.00} pixels wide with an angle of {1:0.00} degrees.\n", StripeWidth, StripeAngle);
            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();

            // Remove the graphic list association to the display.
            MIL.MdispControl(MilDisplay, MIL.M_ASSOCIATED_GRAPHIC_LIST_ID, MIL.M_NULL);

            // Free all allocations.
            MIL.MgraFree(MilGraphicList);
            MIL.MmeasFree(StripeMarker);
            MIL.MbufFree(MilImage);
        }

        //****************************************************************************
        // Multiple measurement example. 
        //****************************************************************************

        // Source MIL image file specification.
        private const string MULT_MEAS_IMAGE_FILE = MIL.M_IMAGE_PATH + "chip.mim";

        // Processing region specification.
        private const int MULT_MEAS_BOX_WIDTH = 230;
        private const int MULT_MEAS_BOX_HEIGHT = 7;
        private const int MULT_MEAS_BOX_POS_X = 220;
        private const int MULT_MEAS_BOX_POS_Y = 171;

        // Target stripe specifications.
        private const int MULT_STRIPES_ORIENTATION = MIL.M_VERTICAL;
        private const int MULT_STRIPES_POLARITY_LEFT = MIL.M_POSITIVE;
        private const int MULT_STRIPES_POLARITY_RIGHT = MIL.M_NEGATIVE;
        private const int MULT_STRIPES_NUMBER = 12;

        static void MultipleMeasurementExample(MIL_ID MilSystem, MIL_ID MilDisplay)
        {
            MIL_ID MilImage = MIL.M_NULL;                   // Image buffer identifier. 
            MIL_ID MilGraphicList = MIL.M_NULL;             // Graphic list identifier.
            MIL_ID StripeMarker = MIL.M_NULL;               // Stripe marker identifier.
            double MeanAngle = 0.0;                         // Stripe mean angle.
            double MeanWidth = 0.0;                         // Stripe mean width.
            double MeanSpacing = 0.0;                       // Stripe mean spacing.
            double CrossColor = MIL.M_COLOR_YELLOW;         // Cross drawing color.
            double BoxColor = MIL.M_COLOR_RED;              // Box drawing color.

            // Restore and display the source image.
            MIL.MbufRestore(MULT_MEAS_IMAGE_FILE, MilSystem, ref MilImage);
            MIL.MdispSelect(MilDisplay, MilImage);

            // Allocate a graphic list to hold the subpixel annotations to draw.
            MIL.MgraAllocList(MilSystem, MIL.M_DEFAULT, ref MilGraphicList);

            // Associate the graphic list to the display.
            MIL.MdispControl(MilDisplay, MIL.M_ASSOCIATED_GRAPHIC_LIST_ID, MilGraphicList);

            // Allocate a stripe marker.
            MIL.MmeasAllocMarker(MilSystem, MIL.M_STRIPE, MIL.M_DEFAULT, ref StripeMarker);

            // Specify the stripe characteristics.
            MIL.MmeasSetMarker(StripeMarker, MIL.M_NUMBER, MULT_STRIPES_NUMBER, MIL.M_NULL);
            MIL.MmeasSetMarker(StripeMarker, MIL.M_POLARITY, MULT_STRIPES_POLARITY_LEFT, MULT_STRIPES_POLARITY_RIGHT);
            MIL.MmeasSetMarker(StripeMarker, MIL.M_ORIENTATION, MULT_STRIPES_ORIENTATION, MIL.M_NULL);

            // Specify the measurement box size and position.
            MIL.MmeasSetMarker(StripeMarker, MIL.M_BOX_ORIGIN, MULT_MEAS_BOX_POS_X, MULT_MEAS_BOX_POS_Y);
            MIL.MmeasSetMarker(StripeMarker, MIL.M_BOX_SIZE, MULT_MEAS_BOX_WIDTH, MULT_MEAS_BOX_HEIGHT);

            // Draw the contour of the measurement region.
            MIL.MgraColor(MIL.M_DEFAULT, BoxColor);
            MIL.MmeasDraw(MIL.M_DEFAULT, StripeMarker, MilGraphicList, MIL.M_DRAW_SEARCH_REGION, MIL.M_DEFAULT, MIL.M_MARKER);

            // Pause to show the original image.
            Console.Write("The position and angle of a row of pins on a chip will be calculated.\n");
            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();

            // Find the stripe and measure its width and angle.
            MIL.MmeasFindMarker(MIL.M_DEFAULT, MilImage, StripeMarker, MIL.M_POSITION + MIL.M_ANGLE + MIL.M_STRIPE_WIDTH);

            // Draw the contour of the measurement region.
            MIL.MgraColor(MIL.M_DEFAULT, BoxColor);
            MIL.MmeasDraw(MIL.M_DEFAULT, StripeMarker, MilGraphicList, MIL.M_DRAW_SEARCH_REGION, MIL.M_DEFAULT, MIL.M_RESULT);

            // Draw a cross at the center of each stripe found.
            MIL.MgraColor(MIL.M_DEFAULT, CrossColor);
            MIL.MmeasDraw(MIL.M_DEFAULT, StripeMarker, MilGraphicList, MIL.M_DRAW_POSITION, MIL.M_ALL, MIL.M_RESULT);

            // Get the stripe's width, angle and spacing.
            MIL.MmeasGetResult(StripeMarker, MIL.M_ANGLE + MIL.M_MEAN, ref MeanAngle);
            MIL.MmeasGetResult(StripeMarker, MIL.M_STRIPE_WIDTH + MIL.M_MEAN, ref MeanWidth);
            MIL.MmeasGetResult(StripeMarker, MIL.M_SPACING + MIL.M_MEAN, ref MeanSpacing);

            // Print the results.
            Console.Write("The center and angle of each pin have been marked.\n\n");
            Console.Write("The statistics for the pins are:\n");
            Console.Write("Average angle   : {0,5:0.00}\n", MeanAngle);
            Console.Write("Average width   : {0,5:0.00}\n", MeanWidth);
            Console.Write("Average spacing : {0,5:0.00}\n", MeanSpacing);
            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();

            // Remove the graphic list association to the display.
            MIL.MdispControl(MilDisplay, MIL.M_ASSOCIATED_GRAPHIC_LIST_ID, MIL.M_NULL);

            // Free all allocations.
            MIL.MgraFree(MilGraphicList);
            MIL.MmeasFree(StripeMarker);
            MIL.MbufFree(MilImage);
        }

        //*****************************************************************************
        // Fixtured measurement example. 
        //*****************************************************************************

        // Source MIL image file specification.
        private const string FIXTURED_MEAS_IMAGE_FILE = MIL.M_IMAGE_PATH + "Fuse.mim";

        // Processing region specification.
        private const int FIXTURED_MEAS_BOX_OFFSET_X = 400;
        private const int FIXTURED_MEAS_BOX_OFFSET_Y = 290;
        private const int FIXTURED_MEAS_BOX_WIDTH = 100;
        private const int FIXTURED_MEAS_BOX_HEIGHT = 15;

        // Model region specification.
        private const int FIXTURED_MODEL_OFFSET_X = 395;
        private const int FIXTURED_MODEL_OFFSET_Y = 200;
        private const int FIXTURED_MODEL_SIZE_X = 110;
        private const int FIXTURED_MODEL_SIZE_Y = 120;

        private const int FIXTURED_IMAGE_SIZE_X = 512;
        private const int FIXTURED_IMAGE_SIZE_Y = 384;

        // Target stripe typical specifications.
        private const int FIXTURED_STRIPE_POLARITY_LEFT = MIL.M_POSITIVE;
        private const int FIXTURED_STRIPE_POLARITY_RIGHT = MIL.M_OPPOSITE;

        private static void FixturedMeasurementExample(MIL_ID MilSystem, MIL_ID MilDisplay)
        {
            MIL_ID MilSourceImage = MIL.M_NULL;             // Source image buffer identifier.
            MIL_ID MilImage = MIL.M_NULL;                   // Image buffer identifier.
            MIL_ID MilModContext = MIL.M_NULL;              // Model finder context identifier.
            MIL_ID MilModResult = MIL.M_NULL;               // Model finder result identifier.
            MIL_ID MilFixturingOffset = MIL.M_NULL;         // Fixturing object identifier.
            MIL_ID StripeMarker = MIL.M_NULL;               // Stripe marker identifier.
            MIL_ID MilGraphicList = MIL.M_NULL;             // Graphic list identifier.

            double StripeWidth = 0.0;                       // Stripe width.
            double PositionX = 0.0;                         // Occurrence position X.
            double PositionY = 0.0;                         // Occurrence position Y.

            MIL_INT SizeX = 0;                              // Source image size X.
            MIL_INT SizeY = 0;                              // Source image size Y.
            MIL_INT NbOccurrences = 0;                      // Number of found occurrences.
            MIL_INT NbStripes = 0;                          // Number of found stripes.
            MIL_INT Index = 0;                              // Occurrence index.

            // Restore the source image.
            MIL.MbufRestore(FIXTURED_MEAS_IMAGE_FILE, MilSystem, ref MilSourceImage);
            MIL.MbufInquire(MilSourceImage, MIL.M_SIZE_X, ref SizeX);
            MIL.MbufInquire(MilSourceImage, MIL.M_SIZE_Y, ref SizeY);

            // Allocate, then compute the source image luminance.
            MIL.MbufAlloc2d(MilSystem, SizeX, SizeY, 8 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_PROC + MIL.M_DISP, ref MilImage);
            MIL.MimConvert(MilSourceImage, MilImage, MIL.M_RGB_TO_L);

            // Allocate a graphic list to hold the subpixel annotations to draw.
            MIL.MgraAllocList(MilSystem, MIL.M_DEFAULT, ref MilGraphicList);

            // Select the image to the display.
            MIL.MdispSelect(MilDisplay, MilImage);

            // Associate the graphic list to the display.
            MIL.MdispControl(MilDisplay, MIL.M_ASSOCIATED_GRAPHIC_LIST_ID, MilGraphicList);

            // Allocate a stripe marker.
            MIL.MmeasAllocMarker(MilSystem, MIL.M_STRIPE, MIL.M_DEFAULT, ref StripeMarker);

            // Set inputs units to world in order to fixture the region.
            MIL.MmeasSetMarker(StripeMarker, MIL.M_SEARCH_REGION_INPUT_UNITS, MIL.M_WORLD, MIL.M_NULL);

            // Calibrate the destination image to receive the world units annotations.
            MIL.McalUniform(MilImage, 0.0, 0.0, 1.0, 1.0, 0.0, MIL.M_DEFAULT);

            // Specify the stripe characteristics.
            MIL.MmeasSetMarker(StripeMarker, MIL.M_BOX_ORIGIN, FIXTURED_MEAS_BOX_OFFSET_X, FIXTURED_MEAS_BOX_OFFSET_Y);
            MIL.MmeasSetMarker(StripeMarker, MIL.M_POLARITY, FIXTURED_STRIPE_POLARITY_LEFT, FIXTURED_STRIPE_POLARITY_RIGHT);
            MIL.MmeasSetMarker(StripeMarker, MIL.M_SEARCH_REGION_CLIPPING, MIL.M_ENABLE, MIL.M_NULL);

            // Specify the search region size and position.
            MIL.MmeasSetMarker(StripeMarker, MIL.M_BOX_SIZE, FIXTURED_MEAS_BOX_WIDTH, FIXTURED_MEAS_BOX_HEIGHT);

            // Draw the contour of the measurement region.
            MIL.MgraColor(MIL.M_DEFAULT, MIL.M_COLOR_BLUE);
            MIL.MgraControl(MIL.M_DEFAULT, MIL.M_INPUT_UNITS, MIL.M_WORLD);
            MIL.MmeasDraw(MIL.M_DEFAULT, StripeMarker, MilGraphicList, MIL.M_DRAW_SEARCH_REGION, MIL.M_DEFAULT, MIL.M_MARKER);
            MIL.MgraControl(MIL.M_DEFAULT, MIL.M_INPUT_UNITS, MIL.M_PIXEL);

            Console.Write("A measurement stripe marker (in blue) is defined.\n");

            // Define model to further fixture the measurement marker.
            MIL.MmodAlloc(MilSystem, MIL.M_GEOMETRIC, MIL.M_DEFAULT, ref MilModContext);

            MIL.MmodAllocResult(MilSystem, MIL.M_DEFAULT, ref MilModResult);

            MIL.MmodDefine(MilModContext, MIL.M_IMAGE, MilImage,
               FIXTURED_MODEL_OFFSET_X, FIXTURED_MODEL_OFFSET_Y,
               FIXTURED_MODEL_SIZE_X, FIXTURED_MODEL_SIZE_Y);

            MIL.MmodControl(MilModContext, MIL.M_DEFAULT, MIL.M_NUMBER, MIL.M_ALL);
            MIL.MmodControl(MilModContext, MIL.M_CONTEXT, MIL.M_SPEED, MIL.M_VERY_HIGH);

            // Preprocess the model.
            MIL.MmodPreprocess(MilModContext, MIL.M_DEFAULT);

            // Display the model.
            MIL.MgraColor(MIL.M_DEFAULT, MIL.M_COLOR_GREEN);
            MIL.MmodDraw(MIL.M_DEFAULT, MilModContext, MilGraphicList, MIL.M_DRAW_BOX + MIL.M_DRAW_POSITION, MIL.M_DEFAULT, MIL.M_ORIGINAL);
            Console.Write("A Model Finder model (in green) is defined to\n");
            Console.Write("further fixture the measurement operation.\n\n");

            Console.Write("The stripe marker determines the gap between\n");
            Console.Write("the fuse connectors. Model Finder tracks the\n");
            Console.Write("fuses while the attached fixturing automatically\n");
            Console.Write("relocates the marker relative to the found fuses.\n");
            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();

            // Allocate a fixture object.
            MIL.McalAlloc(MilSystem, MIL.M_FIXTURING_OFFSET, MIL.M_DEFAULT, ref MilFixturingOffset);

            // Learn the relative offset of the model.
            MIL.McalFixture(MIL.M_NULL, MilFixturingOffset, MIL.M_LEARN_OFFSET, MIL.M_MODEL_MOD,
               MilModContext, 0, MIL.M_DEFAULT, MIL.M_DEFAULT, MIL.M_DEFAULT);

            // Find the location of the fixtures using Model Finder.
            MIL.MmodFind(MilModContext, MilImage, MilModResult);

            // Display and retrieve the number of occurrences found.
            MIL.MgraClear(MIL.M_DEFAULT, MilGraphicList);
            MIL.MmodDraw(MIL.M_DEFAULT, MilModResult, MilGraphicList, MIL.M_DRAW_POSITION + MIL.M_DRAW_BOX, MIL.M_DEFAULT, MIL.M_DEFAULT);

            MIL.MmodGetResult(MilModResult, MIL.M_DEFAULT, MIL.M_NUMBER + MIL.M_TYPE_MIL_INT, ref NbOccurrences);

            Console.Write("Locating the parts: {0} occurrences are found.\n", NbOccurrences);
            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();

            Console.Write("The measurement tool is moved relative to each piece.\n");
            Console.Write("A graphic list is used to display the results with\n");
            Console.Write("subpixel annotations.\n\n");

            // Clear the annotations.
            MIL.MgraClear(MIL.M_DEFAULT, MilGraphicList);

            for (Index = 0; Index < NbOccurrences; Index++)
            {
                // Display the found model occurrence position.
                MIL.MgraColor(MIL.M_DEFAULT, MIL.M_COLOR_GREEN);
                MIL.MmodDraw(MIL.M_DEFAULT, MilModResult, MilGraphicList, MIL.M_DRAW_POSITION, Index, MIL.M_DEFAULT);

                MIL.MmodGetResult(MilModResult, Index, MIL.M_POSITION_X + MIL.M_TYPE_MIL_DOUBLE, ref PositionX);
                MIL.MmodGetResult(MilModResult, Index, MIL.M_POSITION_Y + MIL.M_TYPE_MIL_DOUBLE, ref PositionY);

                MIL.MgraText(MIL.M_DEFAULT, MilGraphicList, PositionX - 20, PositionY, Index.ToString());

                // Apply a fixture offset to the implicit 1:1 calibration of the target image.
                MIL.McalFixture(MilImage, MilFixturingOffset, MIL.M_MOVE_RELATIVE, MIL.M_RESULT_MOD,
                   MilModResult, Index, MIL.M_DEFAULT, MIL.M_DEFAULT, MIL.M_DEFAULT);

                // Find the stripe and measure its width and angle.
                MIL.MmeasFindMarker(MIL.M_DEFAULT, MilImage, StripeMarker, MIL.M_POSITION + MIL.M_STRIPE_WIDTH);

                // Get the number of found results.
                MIL.MmeasGetResult(StripeMarker, MIL.M_NUMBER + MIL.M_TYPE_MIL_INT, ref NbStripes, MIL.M_NULL);

                if (NbStripes == 1)
                {
                    // Get the stripe width.
                    MIL.MmeasGetResult(StripeMarker, MIL.M_STRIPE_WIDTH, ref StripeWidth, MIL.M_NULL);

                    // Draw the contour of the found measurement region.
                    MIL.MgraColor(MIL.M_DEFAULT, MIL.M_COLOR_BLUE);
                    MIL.MmeasDraw(MIL.M_DEFAULT, StripeMarker, MilGraphicList, MIL.M_DRAW_SEARCH_REGION, MIL.M_DEFAULT, MIL.M_RESULT);

                    // Draw a cross on the center, left edge and right edge of the found stripe.
                    MIL.MgraColor(MIL.M_DEFAULT, MIL.M_COLOR_RED);
                    MIL.MmeasDraw(MIL.M_DEFAULT, StripeMarker, MilGraphicList, MIL.M_DRAW_WIDTH, MIL.M_DEFAULT, MIL.M_RESULT);

                    // Print the result.
                    Console.Write("The gap (in red) of occurrence {0} is {1:#.##} pixels wide.\n", Index, StripeWidth);
                }
                else
                {
                    Console.Write("The gap of occurrence {0} could not be measured.\n", Index);
                }
            }

            Console.Write("Press <Enter> to end.\n");
            Console.ReadKey();

            // Free all allocations.
            MIL.MgraFree(MilGraphicList);
            MIL.MmeasFree(StripeMarker);
            MIL.MmodFree(MilModContext);
            MIL.MmodFree(MilModResult);
            MIL.McalFree(MilFixturingOffset);
            MIL.MbufFree(MilImage);
            MIL.MbufFree(MilSourceImage);
        }

    }
}
