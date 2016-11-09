//******************************************************************************
// 
// File name: Mbead.cs
//
// Synopsis:  This program uses the Bead module to train a bead template
//            and then to inspect a defective bead in a target image.
//
//******************************************************************************
using System;
using System.Collections.Generic;
using System.Text;

using Matrox.MatroxImagingLibrary;

namespace MBead
{
    class Program
    {
        // Utility definitions.
        private static readonly MIL_INT USER_POSITION_COLOR = MIL.M_COLOR_RED;
        private static readonly MIL_INT USER_TEMPLATE_COLOR = MIL.M_COLOR_CYAN;
        private static readonly MIL_INT TRAINED_BEAD_WIDTH_COLOR = MIL.M_RGB888(255, 128, 0);
        private static readonly MIL_INT MODEL_FINDER_COLOR = MIL.M_COLOR_GREEN;
        private static readonly MIL_INT COORDINATE_SYSTEM_COLOR = MIL.M_RGB888(164, 164, 0);
        private static readonly MIL_INT RESULT_SEARCH_BOX_COLOR = MIL.M_COLOR_CYAN;
        private static readonly MIL_INT PASS_BEAD_WIDTH_COLOR = MIL.M_COLOR_GREEN;
        private static readonly MIL_INT PASS_BEAD_POSITION_COLOR = MIL.M_COLOR_GREEN;
        private static readonly MIL_INT FAIL_NOT_FOUND_COLOR = MIL.M_COLOR_RED;
        private static readonly MIL_INT FAIL_SMALL_WIDTH_COLOR = MIL.M_RGB888(255, 128, 0);
        private static readonly MIL_INT FAIL_EDGE_OFFSET_COLOR = MIL.M_COLOR_RED;

        //*****************************************************************************
        // Main. 
        //*****************************************************************************
        static void Main(string[] args)
        {
            MIL_ID MilApplication = MIL.M_NULL;         // Application identifier.
            MIL_ID MilSystem = MIL.M_NULL;              // System identifier.
            MIL_ID MilDisplay = MIL.M_NULL;             // Display identifier.

            // Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, ref MilApplication, ref MilSystem, ref MilDisplay, MIL.M_NULL, MIL.M_NULL);

            // Run fixtured bead example.
            FixturedBeadExample(MilSystem, MilDisplay);

            // Run predefined bead example.
            PredefinedBeadExample(MilSystem, MilDisplay);

            // Free defaults.
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL);
        }

        //****************************************************************************
        // Fixtured 'stripe' bead example.

        // Target MIL image specifications.
        private static readonly string IMAGE_FILE_TRAINING = MIL.M_IMAGE_PATH + "BeadTraining.mim";
        private static readonly string IMAGE_FILE_TARGET = MIL.M_IMAGE_PATH + "BeadTarget.mim";

        // Bead stripe training data definition.
        private const int NUMBER_OF_TRAINING_POINTS = 13;

        private static readonly double[] TrainingPointsX = new double[NUMBER_OF_TRAINING_POINTS] { 180, 280, 400, 430, 455, 415, 370, 275, 185, 125, 105, 130, 180 };
        private static readonly double[] TrainingPointsY = new double[NUMBER_OF_TRAINING_POINTS] { 190, 215, 190, 200, 260, 330, 345, 310, 340, 305, 265, 200, 190 };

        // Max angle correction.
        private const double MAX_ANGLE_CORRECTION_VALUE = 20.0;

        // Max offset deviation.
        private const double MAX_DEVIATION_OFFSET_VALUE = 25.0;

        // Maximum negative width variation.
        private const double WIDTH_DELTA_NEG_VALUE = 2.0;

        // Model region  definition.
        private const int MODEL_ORIGIN_X = 145;
        private const int MODEL_ORIGIN_Y = 115;
        private const int MODEL_SIZE_X = 275;
        private const int MODEL_SIZE_Y = 60;

        private static void FixturedBeadExample(MIL_ID MilSystem, MIL_ID MilDisplay)
        {
            MIL_ID MilGraList = MIL.M_NULL;             // Graphic list identifier.
            MIL_ID MilImageTraining = MIL.M_NULL;       // Image buffer identifier.
            MIL_ID MilImageTarget = MIL.M_NULL;         // Image buffer identifier.
            MIL_ID MilBeadContext = MIL.M_NULL;         // Bead context identifier.
            MIL_ID MilBeadResult = MIL.M_NULL;          // Bead result identifier.
            MIL_ID MilModelFinderContext = MIL.M_NULL;  // Model finder context identifier.
            MIL_ID MilModelFinderResult = MIL.M_NULL;   // Model finder result identifier.
            MIL_ID MilFixturingOffset = MIL.M_NULL;     // Fixturing offset identifier.

            double NominalWidth = 0.0;                  // Nominal width result value.
            double AvWidth = 0.0;                       // Average width result value.
            double GapCov = 0.0;                        // Gap coverage result value.
            double MaxGap = 0.0;                        // Maximum gap result value.
            double Score = 0.0;                         // Bead score result value.

            // Restore source images into an automatically allocated image buffers.
            MIL.MbufRestore(IMAGE_FILE_TRAINING, MilSystem, ref MilImageTraining);
            MIL.MbufRestore(IMAGE_FILE_TARGET, MilSystem, ref MilImageTarget);

            // Display the training image buffer.
            MIL.MdispSelect(MilDisplay, MilImageTraining);

            // Allocate a graphic list to hold the subpixel annotations to draw.
            MIL.MgraAllocList(MilSystem, MIL.M_DEFAULT, ref MilGraList);

            // Associate the graphic list to the display for annotations.
            MIL.MdispControl(MilDisplay, MIL.M_ASSOCIATED_GRAPHIC_LIST_ID, MilGraList);

            // Original template image.
            Console.Write("\nFIXTURED BEAD INSPECTION:\n");
            Console.Write("-------------------------\n\n");
            Console.Write("This program performs a bead inspection on a mechanical part.\n");
            Console.Write("In the first step, a bead template context is trained using an ");
            Console.Write("image.\nIn the second step, a mechanical part, at an arbitrary ");
            Console.Write("angle and with\na defective bead, is inspected.\n");
            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();

            // Allocate a MIL bead context.
            MIL.MbeadAlloc(MilSystem, MIL.M_DEFAULT, MIL.M_DEFAULT, ref MilBeadContext);

            // Allocate a MIL bead result.
            MIL.MbeadAllocResult(MilSystem, MIL.M_DEFAULT, ref MilBeadResult);

            // Add bead templates
            MIL.MbeadTemplate(MilBeadContext, MIL.M_ADD, MIL.M_DEFAULT, MIL.M_TEMPLATE_LABEL(1),
               NUMBER_OF_TRAINING_POINTS, TrainingPointsX, TrainingPointsY, MIL.M_NULL, MIL.M_DEFAULT);

            // Set template input units to world units
            MIL.MbeadControl(MilBeadContext, MIL.M_TEMPLATE_LABEL(1), MIL.M_TEMPLATE_INPUT_UNITS, MIL.M_WORLD);

            // Set the bead 'edge type' search properties
            MIL.MbeadControl(MilBeadContext, MIL.M_TEMPLATE_LABEL(1), MIL.M_ANGLE_ACCURACY_MAX_DEVIATION, MAX_ANGLE_CORRECTION_VALUE);

            // Set the maximum valid bead deformation.
            MIL.MbeadControl(MilBeadContext, MIL.M_TEMPLATE_LABEL(1), MIL.M_OFFSET_MAX, MAX_DEVIATION_OFFSET_VALUE);

            // Set the valid bead stripe minimum width criterion.
            MIL.MbeadControl(MilBeadContext, MIL.M_TEMPLATE_LABEL(1), MIL.M_WIDTH_DELTA_NEG, WIDTH_DELTA_NEG_VALUE);

            // Display the bead polyline.
            MIL.MgraColor(MIL.M_DEFAULT, USER_TEMPLATE_COLOR);
            MIL.MbeadDraw(MIL.M_DEFAULT, MilBeadContext, MilGraList, MIL.M_DRAW_POSITION_POLYLINE,
               MIL.M_USER, MIL.M_ALL, MIL.M_ALL, MIL.M_DEFAULT);

            // Display the bead training points.
            MIL.MgraColor(MIL.M_DEFAULT, USER_POSITION_COLOR);
            MIL.MbeadDraw(MIL.M_DEFAULT, MilBeadContext, MilGraList, MIL.M_DRAW_POSITION,
               MIL.M_USER, MIL.M_ALL, MIL.M_ALL, MIL.M_DEFAULT);

            // Pause to show the template image and user points.
            Console.Write("The initial points specified by the user (in red) are\n");
            Console.Write("used to train the bead information from an image.\n");
            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();

            // Set a 1:1 calibration to the training image.
            MIL.McalUniform(MilImageTraining, 0, 0, 1, 1, 0, MIL.M_DEFAULT);

            // Train the bead context.
            MIL.MbeadTrain(MilBeadContext, MilImageTraining, MIL.M_DEFAULT);

            // Display the trained bead.
            MIL.MgraColor(MIL.M_DEFAULT, TRAINED_BEAD_WIDTH_COLOR);
            MIL.MbeadDraw(MIL.M_DEFAULT, MilBeadContext, MilGraList, MIL.M_DRAW_WIDTH,
               MIL.M_TRAINED, MIL.M_TEMPLATE_LABEL(1), MIL.M_ALL, MIL.M_DEFAULT);

            // Retrieve the trained nominal width.
            MIL.MbeadInquire(MilBeadContext, MIL.M_TEMPLATE_LABEL(1), MIL.M_TRAINED_WIDTH_NOMINAL, ref NominalWidth);

            Console.Write("The template has been trained and is displayed in orange.\n");
            Console.Write("Its nominal trained width is {0:#.##} pixels.\n\n", NominalWidth);

            // Define model to further fixture the bead template.
            MIL.MmodAlloc(MilSystem, MIL.M_GEOMETRIC, MIL.M_DEFAULT, ref MilModelFinderContext);

            MIL.MmodAllocResult(MilSystem, MIL.M_DEFAULT, ref MilModelFinderResult);

            MIL.MmodDefine(MilModelFinderContext, MIL.M_IMAGE, MilImageTraining,
                MODEL_ORIGIN_X, MODEL_ORIGIN_Y, MODEL_SIZE_X, MODEL_SIZE_Y);

            // Preprocess the model.
            MIL.MmodPreprocess(MilModelFinderContext, MIL.M_DEFAULT);

            // Allocate a fixture object.
            MIL.McalAlloc(MilSystem, MIL.M_FIXTURING_OFFSET, MIL.M_DEFAULT, ref MilFixturingOffset);

            // Learn the relative offset of the model.
            MIL.McalFixture(MIL.M_NULL, MilFixturingOffset, MIL.M_LEARN_OFFSET, MIL.M_MODEL_MOD,
               MilModelFinderContext, 0, MIL.M_DEFAULT, MIL.M_DEFAULT, MIL.M_DEFAULT);

            // Display the model.
            MIL.MgraColor(MIL.M_DEFAULT, MODEL_FINDER_COLOR);
            MIL.MmodDraw(MIL.M_DEFAULT, MilModelFinderContext, MilGraList, MIL.M_DRAW_BOX + MIL.M_DRAW_POSITION,
               MIL.M_DEFAULT, MIL.M_ORIGINAL);

            Console.Write("A Model Finder model (in green) is also defined to\n");
            Console.Write("further fixture the bead verification operation.\n");
            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();

            // Clear the overlay annotation.
            MIL.MgraClear(MIL.M_DEFAULT, MilGraList);

            // Display the target image buffer.
            MIL.MdispSelect(MilDisplay, MilImageTarget);

            // Find the location of the fixture using Model Finder.
            MIL.MmodFind(MilModelFinderContext, MilImageTarget, MilModelFinderResult);

            // Display the found model occurrence.
            MIL.MgraColor(MIL.M_DEFAULT, MODEL_FINDER_COLOR);
            MIL.MmodDraw(MIL.M_DEFAULT, MilModelFinderResult, MilGraList, MIL.M_DRAW_BOX + MIL.M_DRAW_POSITION,
               MIL.M_DEFAULT, MIL.M_DEFAULT);

            // Apply fixture offset to the target image.
            MIL.McalFixture(MilImageTarget, MilFixturingOffset, MIL.M_MOVE_RELATIVE, MIL.M_RESULT_MOD,
               MilModelFinderResult, 0, MIL.M_DEFAULT, MIL.M_DEFAULT, MIL.M_DEFAULT);

            // Display the relative coordinate system.
            MIL.MgraColor(MIL.M_DEFAULT, COORDINATE_SYSTEM_COLOR);
            MIL.McalDraw(MIL.M_DEFAULT, MIL.M_NULL, MilGraList, MIL.M_DRAW_RELATIVE_COORDINATE_SYSTEM,
               MIL.M_DEFAULT, MIL.M_DEFAULT);

            // Perform the inspection of the bead in the fixtured target image.
            MIL.MbeadVerify(MilBeadContext, MilImageTarget, MilBeadResult, MIL.M_DEFAULT);

            // Display the result search boxes.
            MIL.MgraColor(MIL.M_DEFAULT, RESULT_SEARCH_BOX_COLOR);
            MIL.MbeadDraw(MIL.M_DEFAULT, MilBeadResult, MilGraList, MIL.M_DRAW_SEARCH_BOX, MIL.M_ALL,
               MIL.M_ALL, MIL.M_ALL, MIL.M_DEFAULT);

            Console.Write("The mechanical part's position and angle (in green) were ");
            Console.Write("located\nusing Model Finder, and the bead's search boxes ");
            Console.Write("(in cyan) were\npositioned accordingly.\n");
            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();

            // Clear the overlay annotation.
            MIL.MgraClear(MIL.M_DEFAULT, MilGraList);

            // Display the moved relative coordinate system.
            MIL.MgraColor(MIL.M_DEFAULT, COORDINATE_SYSTEM_COLOR);
            MIL.McalDraw(MIL.M_DEFAULT, MIL.M_NULL, MilGraList, MIL.M_DRAW_RELATIVE_COORDINATE_SYSTEM,
               MIL.M_DEFAULT, MIL.M_DEFAULT);

            // Display the pass bead sections.
            MIL.MgraColor(MIL.M_DEFAULT, PASS_BEAD_WIDTH_COLOR);
            MIL.MbeadDraw(MIL.M_DEFAULT, MilBeadResult, MilGraList, MIL.M_DRAW_WIDTH, MIL.M_PASS,
               MIL.M_TEMPLATE_LABEL(1), MIL.M_ALL, MIL.M_DEFAULT);

            // Display the missing bead sections.
            MIL.MgraColor(MIL.M_DEFAULT, FAIL_NOT_FOUND_COLOR);
            MIL.MbeadDraw(MIL.M_DEFAULT, MilBeadResult, MilGraList, MIL.M_DRAW_SEARCH_BOX,
               MIL.M_FAIL_NOT_FOUND, MIL.M_ALL, MIL.M_ALL, MIL.M_DEFAULT);

            // Display bead sections which do not meet the minimum width criteria.
            MIL.MgraColor(MIL.M_DEFAULT, FAIL_SMALL_WIDTH_COLOR);
            MIL.MbeadDraw(MIL.M_DEFAULT, MilBeadResult, MilGraList, MIL.M_DRAW_SEARCH_BOX,
               MIL.M_FAIL_WIDTH_MIN, MIL.M_TEMPLATE_LABEL(1), MIL.M_ALL, MIL.M_DEFAULT);

            // Retrieve and display general bead results.
            MIL.MbeadGetResult(MilBeadResult, MIL.M_TEMPLATE_LABEL(1), MIL.M_GENERAL, MIL.M_SCORE, ref Score);
            MIL.MbeadGetResult(MilBeadResult, MIL.M_TEMPLATE_LABEL(1), MIL.M_GENERAL, MIL.M_GAP_COVERAGE, ref GapCov);
            MIL.MbeadGetResult(MilBeadResult, MIL.M_TEMPLATE_LABEL(1), MIL.M_GENERAL, MIL.M_WIDTH_AVERAGE, ref AvWidth);
            MIL.MbeadGetResult(MilBeadResult, MIL.M_TEMPLATE_LABEL(1), MIL.M_GENERAL, MIL.M_GAP_MAX_LENGTH, ref MaxGap);

            Console.Write("The bead 'has been inspected:\n");
            Console.Write(" -Passing bead sections (green) cover {0:0.00}% of the bead\n", Score);
            Console.Write(" -Missing bead sections (red) cover {0:0.00}% of the bead\n", GapCov);
            Console.Write(" -Sections outside the specified width tolerances are drawn in orange\n");
            Console.Write(" -The bead's average width is {0:0.00} pixels\n", AvWidth);
            Console.Write(" -The bead's longest gap section is {0:0.00} pixels\n\n", MaxGap);

            // Pause to show the result.
            Console.Write("Press <Enter> to continue.\n");
            Console.ReadKey();

            // Free all allocations.
            MIL.MmodFree(MilModelFinderContext);
            MIL.MmodFree(MilModelFinderResult);
            MIL.MbeadFree(MilBeadContext);
            MIL.MbeadFree(MilBeadResult);
            MIL.McalFree(MilFixturingOffset);
            MIL.MbufFree(MilImageTraining);
            MIL.MbufFree(MilImageTarget);
            MIL.MgraFree(MilGraList);
        }

        //*****************************************************************************
        // Predefined 'edge' bead example.

        // Target MIL image specifications.
        private static readonly string CAP_FILE_TARGET = MIL.M_IMAGE_PATH + "Cap.mim";

        // Template attributes definition.
        private const double CIRCLE_CENTER_X = 330.0;
        private const double CIRCLE_CENTER_Y = 230.0;
        private const double CIRCLE_RADIUS = 120.0;

        // Edge threshold value.
        private const double EDGE_THRESHOLD_VALUE = 25.0;

        // Max offset found and deviation tolerance.
        private const double MAX_CONTOUR_DEVIATION_OFFSET = 5.0;
        private const double MAX_CONTOUR_FOUND_OFFSET = 25.0;

        private static void PredefinedBeadExample(MIL_ID MilSystem, MIL_ID MilDisplay)
        {
            MIL_ID MilOverlayImage = MIL.M_NULL;       // Overlay buffer identifier.
            MIL_ID MilImageTarget = MIL.M_NULL;        // Image buffer identifier.
            MIL_ID MilBeadContext = MIL.M_NULL;        // Bead context identifier.
            MIL_ID MilBeadResult = MIL.M_NULL;         // Bead result identifier.

            double MaximumOffset = 0.0;     // Maximum offset result value.

            // Restore target image into an automatically allocated image buffers.
            MIL.MbufRestore(CAP_FILE_TARGET, MilSystem, ref MilImageTarget);

            // Display the training image buffer.
            MIL.MdispSelect(MilDisplay, MilImageTarget);

            // Prepare the overlay for annotations.
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY, MIL.M_ENABLE);
            MIL.MdispInquire(MilDisplay, MIL.M_OVERLAY_ID, ref MilOverlayImage);
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY_CLEAR, MIL.M_TRANSPARENT_COLOR);

            // Original template image.
            Console.Write("\nPREDEFINED BEAD INSPECTION:\n");
            Console.Write("---------------------------\n\n");
            Console.Write("This program performs a bead inspection of a bottle\n");
            Console.Write("cap's contour using a predefined circular bead.\n");
            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();

            // Allocate a MIL bead context.
            MIL.MbeadAlloc(MilSystem, MIL.M_DEFAULT, MIL.M_DEFAULT, ref MilBeadContext);

            // Allocate a MIL bead result.
            MIL.MbeadAllocResult(MilSystem, MIL.M_DEFAULT, ref MilBeadResult);

            // Add the bead templates.
            MIL.MbeadTemplate(MilBeadContext, MIL.M_ADD, MIL.M_BEAD_EDGE, MIL.M_TEMPLATE_LABEL(1),
               0, MIL.M_NULL, MIL.M_NULL, MIL.M_NULL, MIL.M_DEFAULT);

            // Set the bead shape properties.
            MIL.MbeadControl(MilBeadContext, MIL.M_TEMPLATE_LABEL(1), MIL.M_TRAINING_PATH, MIL.M_CIRCLE);

            MIL.MbeadControl(MilBeadContext, MIL.M_TEMPLATE_LABEL(1), MIL.M_TEMPLATE_CIRCLE_CENTER_X, CIRCLE_CENTER_X);
            MIL.MbeadControl(MilBeadContext, MIL.M_TEMPLATE_LABEL(1), MIL.M_TEMPLATE_CIRCLE_CENTER_Y, CIRCLE_CENTER_Y);
            MIL.MbeadControl(MilBeadContext, MIL.M_TEMPLATE_LABEL(1), MIL.M_TEMPLATE_CIRCLE_RADIUS, CIRCLE_RADIUS);

            // Set the edge threshold value to extract the object shape.
            MIL.MbeadControl(MilBeadContext, MIL.M_TEMPLATE_LABEL(1), MIL.M_THRESHOLD_VALUE, EDGE_THRESHOLD_VALUE);

            // Using the default fixed user defined nominal edge width.
            MIL.MbeadControl(MilBeadContext, MIL.M_ALL, MIL.M_WIDTH_NOMINAL_MODE, MIL.M_USER_DEFINED);

            // Set the maximal expected contour deformation.
            MIL.MbeadControl(MilBeadContext, MIL.M_ALL, MIL.M_FOUND_OFFSET_MAX, MAX_CONTOUR_FOUND_OFFSET);

            // Set the maximum valid bead deformation.
            MIL.MbeadControl(MilBeadContext, MIL.M_ALL, MIL.M_OFFSET_MAX, MAX_CONTOUR_DEVIATION_OFFSET);

            // Display the bead in the overlay image.
            MIL.MgraColor(MIL.M_DEFAULT, USER_TEMPLATE_COLOR);
            MIL.MbeadDraw(MIL.M_DEFAULT, MilBeadContext, MilOverlayImage, MIL.M_DRAW_POSITION,
               MIL.M_USER, MIL.M_ALL, MIL.M_ALL, MIL.M_DEFAULT);

            // The bead template is entirely defined and is trained without sample image.
            MIL.MbeadTrain(MilBeadContext, MIL.M_NULL, MIL.M_DEFAULT);

            // Display the trained bead.
            MIL.MgraColor(MIL.M_DEFAULT, TRAINED_BEAD_WIDTH_COLOR);
            MIL.MbeadDraw(MIL.M_DEFAULT, MilBeadContext, MilOverlayImage, MIL.M_DRAW_SEARCH_BOX,
               MIL.M_TRAINED, MIL.M_ALL, MIL.M_ALL, MIL.M_DEFAULT);

            // Pause to show the template image and user points.
            Console.Write("A circular template that was parametrically defined by the user\n");
            Console.Write("is displayed (in cyan). The template has been trained and the resulting\n");
            Console.Write("search is displayed (in orange).\n");
            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();

            // Perform the inspection of the bead in the fixtured target image.
            MIL.MbeadVerify(MilBeadContext, MilImageTarget, MilBeadResult, MIL.M_DEFAULT);

            // Clear the overlay annotation.
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY_CLEAR, MIL.M_TRANSPARENT_COLOR);

            // Display the pass bead sections.
            MIL.MgraColor(MIL.M_DEFAULT, PASS_BEAD_POSITION_COLOR);
            MIL.MbeadDraw(MIL.M_DEFAULT, MilBeadResult, MilOverlayImage, MIL.M_DRAW_POSITION, MIL.M_PASS,
               MIL.M_ALL, MIL.M_ALL, MIL.M_DEFAULT);

            // Display the offset bead sections.
            MIL.MgraColor(MIL.M_DEFAULT, FAIL_EDGE_OFFSET_COLOR);
            MIL.MbeadDraw(MIL.M_DEFAULT, MilBeadResult, MilOverlayImage, MIL.M_DRAW_POSITION, MIL.M_FAIL_OFFSET,
               MIL.M_ALL, MIL.M_ALL, MIL.M_DEFAULT);

            // Retrieve and display general bead results.
            MIL.MbeadGetResult(MilBeadResult, MIL.M_TEMPLATE_LABEL(1), MIL.M_GENERAL, MIL.M_OFFSET_MAX, ref MaximumOffset);

            Console.Write("The bottle cap shape has been inspected:\n");
            Console.Write(" -Sections outside the specified offset tolerance are drawn in red\n");
            Console.Write(" -The maximum offset value is {0:0.00} pixels.\n\n", MaximumOffset);

            // Pause to show the result.
            Console.Write("Press <Enter> to terminate.\n");
            Console.ReadKey();

            // Free all allocations.
            MIL.MbeadFree(MilBeadContext);
            MIL.MbeadFree(MilBeadResult);
            MIL.MbufFree(MilImageTarget);
        }
    }
}
