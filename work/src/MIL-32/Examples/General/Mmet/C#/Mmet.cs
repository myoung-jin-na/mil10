//*****************************************************************************
// 
// File name: Mmet.cs
//
// Synopsis:  This program uses the MIL Metrology module to measure geometric 
//            features and to validate tolerance relationships between features.
//*****************************************************************************
using System;
using System.Collections.Generic;
using System.Text;

using Matrox.MatroxImagingLibrary;

namespace MMet
{
    class Program
    {
        // Example selection.
        private const int RUN_SIMPLE_IMAGE_EXAMPLE = MIL.M_YES;
        private const int RUN_COMPLETE_IMAGE_EXAMPLE = MIL.M_YES;

        //*****************************************************************************
        // Main.
        //*****************************************************************************
        static void Main(string[] args)
        {
            MIL_ID MilApplication = MIL.M_NULL;     // Application identifier.
            MIL_ID MilSystem = MIL.M_NULL;          // System Identifier.     
            MIL_ID MilDisplay = MIL.M_NULL;         // Display identifier.    

            // Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, ref MilApplication, ref MilSystem, ref MilDisplay, MIL.M_NULL, MIL.M_NULL);

            // Print module name.
            Console.Write("\nMETROLOGY MODULE:\n");
            Console.Write("-------------------\n\n");

            if (RUN_SIMPLE_IMAGE_EXAMPLE == MIL.M_YES)
            {
                SimpleImageExample(MilSystem, MilDisplay);
            }

            if (RUN_COMPLETE_IMAGE_EXAMPLE == MIL.M_YES)
            {
                CompleteImageExample(MilSystem, MilDisplay);
            }

            // Free defaults.
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL);
        }

        //*****************************************************************************
        // Simple example.
        //*****************************************************************************
        // Source MIL image file specification.
        private const string METROL_SIMPLE_IMAGE_FILE = MIL.M_IMAGE_PATH + "SingleModel.mim";

        // Region parameters
        private const int TOP_RING_POSITION_X = 240;
        private const int TOP_RING_POSITION_Y = 155;
        private const int TOP_RING_START_RADIUS = 2;
        private const int TOP_RING_END_RADIUS = 15;

        private const int MIDDLE_RING_POSITION_X = 240;
        private const int MIDDLE_RING_POSITION_Y = 190;
        private const int MIDDLE_RING_START_RADIUS = 2;
        private const int MIDDLE_RING_END_RADIUS = 15;

        private const int BOTTOM_RECT_POSITION_X = 320;
        private const int BOTTOM_RECT_POSITION_Y = 265;
        private const int BOTTOM_RECT_WIDTH = 170;
        private const int BOTTOM_RECT_HEIGHT = 20;
        private const int BOTTOM_RECT_ANGLE = 180;

        // Tolerance parameters
        private const double PERPENDICULARITY_MIN = 0.5;
        private const double PERPENDICULARITY_MAX = 0.5;

        // Color definitions
        static readonly double FAIL_COLOR = MIL.M_RGB888(255, 0, 0);
        static readonly double PASS_COLOR = MIL.M_RGB888(0, 255, 0);
        static readonly double REGION_COLOR = MIL.M_RGB888(0, 100, 255);
        static readonly double FEATURE_COLOR = MIL.M_RGB888(255, 0, 255);

        static void SimpleImageExample(MIL_ID MilSystem, MIL_ID MilDisplay)
        {
            MIL_ID MilImage = MIL.M_NULL;                    // Image buffer identifier.
            MIL_ID GraphicList = MIL.M_NULL;                 // Graphic list identifier.
            MIL_ID MilMetrolContext = MIL.M_NULL;            // Metrology Context
            MIL_ID MilMetrolResult = MIL.M_NULL;             // Metrology Result

            double Status = 0.0;
            double Value = 0.0;
            MIL_INT FeatureIndexForTopConstructedPoint = MIL.M_FEATURE_INDEX(1);
            MIL_INT FeatureIndexForMiddleConstructedPoint = MIL.M_FEATURE_INDEX(2);
            MIL_INT[] FeatureIndexForConstructedSegment = new MIL_INT[2];
            MIL_INT[] FeatureIndexForTolerance = new MIL_INT[2];
            FeatureIndexForConstructedSegment[0] = MIL.M_FEATURE_INDEX(3);
            FeatureIndexForConstructedSegment[1] = MIL.M_FEATURE_INDEX(4);
            FeatureIndexForTolerance[0] = MIL.M_FEATURE_INDEX(5);
            FeatureIndexForTolerance[1] = MIL.M_FEATURE_INDEX(6);

            // Restore and display the source image.
            MIL.MbufRestore(METROL_SIMPLE_IMAGE_FILE, MilSystem, ref MilImage);
            MIL.MdispSelect(MilDisplay, MilImage);

            // Allocate a graphic list to hold the subpixel annotations to draw.
            MIL.MgraAllocList(MilSystem, MIL.M_DEFAULT, ref GraphicList);

            // Associate the graphic list to the display for annotations.
            MIL.MdispControl(MilDisplay, MIL.M_ASSOCIATED_GRAPHIC_LIST_ID, GraphicList);

            // Allocate metrology context and result.
            MIL.MmetAlloc(MilSystem, MIL.M_DEFAULT, ref MilMetrolContext);
            MIL.MmetAllocResult(MilSystem, MIL.M_DEFAULT, ref MilMetrolResult);

            // Add a first measured circle feature to context and set its search region
            MIL.MmetAddFeature(MilMetrolContext, MIL.M_MEASURED, MIL.M_CIRCLE, MIL.M_DEFAULT,
                           MIL.M_DEFAULT, MIL.M_NULL, MIL.M_NULL, MIL.M_DEFAULT, MIL.M_DEFAULT);

            MIL.MmetSetRegion(MilMetrolContext, MIL.M_FEATURE_INDEX(1), MIL.M_DEFAULT, MIL.M_RING,
                          TOP_RING_POSITION_X, TOP_RING_POSITION_Y, TOP_RING_START_RADIUS,
                          TOP_RING_END_RADIUS, MIL.M_NULL, MIL.M_NULL);

            // Add a second measured circle feature to context and set its search region
            MIL.MmetAddFeature(MilMetrolContext, MIL.M_MEASURED, MIL.M_CIRCLE, MIL.M_DEFAULT,
                           MIL.M_DEFAULT, MIL.M_NULL, MIL.M_NULL, MIL.M_DEFAULT, MIL.M_DEFAULT);

            MIL.MmetSetRegion(MilMetrolContext, MIL.M_FEATURE_INDEX(2), MIL.M_DEFAULT, MIL.M_RING,
                          MIDDLE_RING_POSITION_X, MIDDLE_RING_POSITION_Y, MIDDLE_RING_START_RADIUS,
                          MIDDLE_RING_END_RADIUS, MIL.M_NULL, MIL.M_NULL);

            // Add a first constructed point feature to context
            MIL.MmetAddFeature(MilMetrolContext, MIL.M_CONSTRUCTED, MIL.M_POINT, MIL.M_DEFAULT,
                           MIL.M_CENTER, ref FeatureIndexForTopConstructedPoint, MIL.M_NULL, 1, MIL.M_DEFAULT);

            // Add a second constructed point feature to context
            MIL.MmetAddFeature(MilMetrolContext, MIL.M_CONSTRUCTED, MIL.M_POINT, MIL.M_DEFAULT,
                           MIL.M_CENTER, ref FeatureIndexForMiddleConstructedPoint, MIL.M_NULL, 1, MIL.M_DEFAULT);

            // Add a constructed segment feature to context passing through the two points
            MIL.MmetAddFeature(MilMetrolContext, MIL.M_CONSTRUCTED, MIL.M_SEGMENT, MIL.M_DEFAULT,
                           MIL.M_CONSTRUCTION, FeatureIndexForConstructedSegment, MIL.M_NULL, 2, MIL.M_DEFAULT);

            // Add a first segment feature to context and set its search region
            MIL.MmetAddFeature(MilMetrolContext, MIL.M_MEASURED, MIL.M_SEGMENT, MIL.M_DEFAULT,
                           MIL.M_DEFAULT, MIL.M_NULL, MIL.M_NULL, MIL.M_DEFAULT, MIL.M_DEFAULT);

            MIL.MmetSetRegion(MilMetrolContext, MIL.M_FEATURE_INDEX(6), MIL.M_DEFAULT, MIL.M_RECTANGLE,
                          BOTTOM_RECT_POSITION_X, BOTTOM_RECT_POSITION_Y, BOTTOM_RECT_WIDTH,
                          BOTTOM_RECT_HEIGHT, BOTTOM_RECT_ANGLE, MIL.M_NULL);

            // Add perpendicularity tolerance
            MIL.MmetAddTolerance(MilMetrolContext, MIL.M_PERPENDICULARITY, MIL.M_DEFAULT, PERPENDICULARITY_MIN,
                             PERPENDICULARITY_MAX, FeatureIndexForTolerance, MIL.M_NULL, 2, MIL.M_DEFAULT);

            // Calculate
            MIL.MmetCalculate(MilMetrolContext, MilImage, MilMetrolResult, MIL.M_DEFAULT);

            // Draw region
            MIL.MgraColor(MIL.M_DEFAULT, REGION_COLOR);
            MIL.MmetDraw(MIL.M_DEFAULT, MilMetrolResult, GraphicList, MIL.M_DRAW_REGION, MIL.M_DEFAULT, MIL.M_DEFAULT);
            Console.Write("Regions used to calculate measured features:\n");
            Console.Write("- two measured circles\n");
            Console.Write("- one measured segment\n");
            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();

            // Clear annotations.
            MIL.MgraClear(MIL.M_DEFAULT, GraphicList);

            MIL.MgraColor(MIL.M_DEFAULT, FEATURE_COLOR);
            MIL.MmetDraw(MIL.M_DEFAULT, MilMetrolResult, GraphicList, MIL.M_DRAW_FEATURE, MIL.M_DEFAULT, MIL.M_DEFAULT);
            Console.Write("Calculated features:\n");

            MIL.MmetGetResult(MilMetrolResult, MIL.M_FEATURE_INDEX(1), MIL.M_RADIUS, ref Value);
            Console.Write("- first measured circle:  radius={0:0.00}\n", Value);

            MIL.MmetGetResult(MilMetrolResult, MIL.M_FEATURE_INDEX(2), MIL.M_RADIUS, ref Value);
            Console.Write("- second measured circle: radius={0:0.00}\n", Value);

            MIL.MmetGetResult(MilMetrolResult, MIL.M_FEATURE_INDEX(5), MIL.M_LENGTH, ref Value);
            Console.Write("- constructed segment between the two circle centers: length={0:0.00}\n", Value);

            MIL.MmetGetResult(MilMetrolResult, MIL.M_FEATURE_INDEX(6), MIL.M_LENGTH, ref Value);
            Console.Write("- measured segment: length={0:0.00}\n", Value);

            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();

            // Get angularity tolerance status and value
            MIL.MmetGetResult(MilMetrolResult, MIL.M_TOLERANCE_INDEX(0), MIL.M_STATUS, ref Status);
            MIL.MmetGetResult(MilMetrolResult, MIL.M_TOLERANCE_INDEX(0), MIL.M_TOLERANCE_VALUE, ref Value);

            if (Status == MIL.M_PASS)
            {
                MIL.MgraColor(MIL.M_DEFAULT, PASS_COLOR);
                Console.Write("Perpendicularity between the two segments: {0:0.00} degrees.\n", Value);
            }
            else
            {
                MIL.MgraColor(MIL.M_DEFAULT, FAIL_COLOR);
                Console.Write("Perpendicularity between the two segments - Fail.\n");
            }
            MIL.MmetDraw(MIL.M_DEFAULT, MilMetrolResult, GraphicList, MIL.M_DRAW_TOLERANCE, MIL.M_TOLERANCE_INDEX(0), MIL.M_DEFAULT);

            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();

            // Free all allocations.
            MIL.MgraFree(GraphicList);
            MIL.MmetFree(MilMetrolResult);
            MIL.MmetFree(MilMetrolContext);
            MIL.MbufFree(MilImage);
        }

        //****************************************************************************
        // Complete example. 
        //****************************************************************************
        // Source MIL image, calibration and model finder context file specification.
        private const string METROL_CALIBRATION_FILE = MIL.M_IMAGE_PATH + "Hook.mca";
        private const string METROL_COMPLETE_IMAGE_FILE = MIL.M_IMAGE_PATH + "Hook.tif";
        private const string METROL_MODEL_FINDER_FILE = MIL.M_IMAGE_PATH + "Hook.mmf";

        // Region parameters
        private const int CIRCLE1_LABEL = 1;
        private const double RING1_POSITION_X = 1.10;
        private const double RING1_POSITION_Y = 0.80;
        private const double RING1_START_RADIUS = 0.20;
        private const double RING1_END_RADIUS = 0.50;

        private const int CIRCLE2_LABEL = 2;
        private const double RING2_POSITION_X = 1.10;
        private const double RING2_POSITION_Y = 3.00;
        private const double RING2_START_RADIUS = 0.10;
        private const double RING2_END_RADIUS = 0.40;

        private const int SEGMENT1_LABEL = 3;
        private const double RECT1_POSITION_X = 0.10;
        private const double RECT1_POSITION_Y = 2.40;
        private const double RECT1_WIDTH = 1.40;
        private const double RECT1_HEIGHT = 0.30;
        private const double RECT1_ANGLE = 90.0;

        private const int SEGMENT2_LABEL = 4;
        private const double RECT2_POSITION_X = 0.90;
        private const double RECT2_POSITION_Y = 2.80;
        private const double RECT2_WIDTH = 0.40;
        private const double RECT2_HEIGHT = 0.20;
        private const double RECT2_ANGLE = 165.0;

        private const int POINT1_LABEL = 5;
        private const double SEG1_START_X = 1.60;
        private const double SEG1_START_Y = 1.50;
        private const double SEG1_END_X = 1.60;
        private const double SEG1_END_Y = 2.40;

        // Tolerance parameters
        private const int MIN_DISTANCE_LABEL = 1;
        private const int ANGULARITY_LABEL = 2;
        private const int MAX_DISTANCE_LABEL = 3;

        private const double MIN_DISTANCE_VALUE_MIN = 1.40;
        private const double MIN_DISTANCE_VALUE_MAX = 1.60;
        private const double MAX_DISTANCE_VALUE_MIN = 0.40;
        private const double MAX_DISTANCE_VALUE_MAX = 0.60;
        private const double ANGULARITY_VALUE_MIN = 65.0;
        private const double ANGULARITY_VALUE_MAX = 75.0;

        static void CompleteImageExample(MIL_ID MilSystem, MIL_ID MilDisplay)
        {
            MIL_ID MilImage = MIL.M_NULL;                        // Image buffer identifier.
            MIL_ID GraphicList = MIL.M_NULL;                     // Graphic list identifier.
            MIL_ID MilCalibration = MIL.M_NULL;                  // Calibration context
            MIL_ID MilMetrolContext = MIL.M_NULL;                // Metrology Context
            MIL_ID MilMetrolResult = MIL.M_NULL;                 // Metrology Result
            MIL_ID MilModelFinderContext = MIL.M_NULL;           // Model Finder Context
            MIL_ID MilModelFinderResult = MIL.M_NULL;            // Model Finder Result

            double Status = 0.0;
            double Value = 0.0;

            MIL_INT[] MinDistanceFeatureLabels = new MIL_INT[2];
            MIL_INT[] AngularityFeatureLabels = new MIL_INT[2];
            MIL_INT[] MaxDistanceFeatureLabels = new MIL_INT[2];
            MIL_INT[] MaxDistanceFeatureIndices = new MIL_INT[2];

            MinDistanceFeatureLabels[0] = CIRCLE1_LABEL;
            MinDistanceFeatureLabels[1] = CIRCLE2_LABEL;

            AngularityFeatureLabels[0] = SEGMENT1_LABEL;
            AngularityFeatureLabels[1] = SEGMENT2_LABEL;

            MaxDistanceFeatureLabels[0] = POINT1_LABEL;
            MaxDistanceFeatureLabels[1] = POINT1_LABEL;

            MaxDistanceFeatureIndices[0] = 0;
            MaxDistanceFeatureIndices[1] = 1;

            // Restore and display the source image.
            MIL.MbufRestore(METROL_COMPLETE_IMAGE_FILE, MilSystem, ref MilImage);
            MIL.MdispSelect(MilDisplay, MilImage);

            // Restore and associate calibration context to source image
            MIL.McalRestore(METROL_CALIBRATION_FILE, MilSystem, MIL.M_DEFAULT, ref MilCalibration);
            MIL.McalAssociate(MilCalibration, MilImage, MIL.M_DEFAULT);

            // Allocate a graphic list to hold the subpixel annotations to draw.
            MIL.MgraAllocList(MilSystem, MIL.M_DEFAULT, ref GraphicList);

            // Associate the graphic list to the display for annotations.
            MIL.MdispControl(MilDisplay, MIL.M_ASSOCIATED_GRAPHIC_LIST_ID, GraphicList);

            // Allocate metrology context and result.
            MIL.MmetAlloc(MilSystem, MIL.M_DEFAULT, ref MilMetrolContext);
            MIL.MmetAllocResult(MilSystem, MIL.M_DEFAULT, ref MilMetrolResult);

            // Add a first measured circle feature to context and set its search region
            MIL.MmetAddFeature(MilMetrolContext, MIL.M_MEASURED, MIL.M_CIRCLE, CIRCLE1_LABEL,
                           MIL.M_DEFAULT, MIL.M_NULL, MIL.M_NULL, MIL.M_DEFAULT, MIL.M_DEFAULT);

            MIL.MmetSetRegion(MilMetrolContext, MIL.M_FEATURE_LABEL(CIRCLE1_LABEL), MIL.M_DEFAULT, MIL.M_RING,
                          RING1_POSITION_X, RING1_POSITION_Y, RING1_START_RADIUS, RING1_END_RADIUS,
                          MIL.M_NULL, MIL.M_NULL);

            // Add a second measured circle feature to context and set its search region
            MIL.MmetAddFeature(MilMetrolContext, MIL.M_MEASURED, MIL.M_CIRCLE, CIRCLE2_LABEL,
                           MIL.M_DEFAULT, MIL.M_NULL, MIL.M_NULL, MIL.M_DEFAULT, MIL.M_DEFAULT);

            MIL.MmetSetRegion(MilMetrolContext, MIL.M_FEATURE_LABEL(CIRCLE2_LABEL), MIL.M_DEFAULT, MIL.M_RING,
                          RING2_POSITION_X, RING2_POSITION_Y, RING2_START_RADIUS, RING2_END_RADIUS,
                          MIL.M_NULL, MIL.M_NULL);

            // Add a first measured segment feature to context and set its search region
            MIL.MmetAddFeature(MilMetrolContext, MIL.M_MEASURED, MIL.M_SEGMENT, SEGMENT1_LABEL,
                           MIL.M_DEFAULT, MIL.M_NULL, MIL.M_NULL, MIL.M_DEFAULT, MIL.M_DEFAULT);

            MIL.MmetSetRegion(MilMetrolContext, MIL.M_FEATURE_LABEL(SEGMENT1_LABEL), MIL.M_DEFAULT, MIL.M_RECTANGLE,
                          RECT1_POSITION_X, RECT1_POSITION_Y, RECT1_WIDTH, RECT1_HEIGHT,
                          RECT1_ANGLE, MIL.M_NULL);

            MIL.MmetControl(MilMetrolContext, MIL.M_FEATURE_LABEL(SEGMENT1_LABEL), MIL.M_EDGEL_ANGLE_RANGE, 10);

            // Add a second measured segment feature to context and set its search region
            MIL.MmetAddFeature(MilMetrolContext, MIL.M_MEASURED, MIL.M_SEGMENT, SEGMENT2_LABEL,
                           MIL.M_INNER_FIT, MIL.M_NULL, MIL.M_NULL, MIL.M_DEFAULT, MIL.M_DEFAULT);

            MIL.MmetSetRegion(MilMetrolContext, MIL.M_FEATURE_LABEL(SEGMENT2_LABEL), MIL.M_DEFAULT, MIL.M_RECTANGLE,
                          RECT2_POSITION_X, RECT2_POSITION_Y, RECT2_WIDTH, RECT2_HEIGHT,
                          RECT2_ANGLE, MIL.M_NULL);

            // Add a measured point feature to context and set its search region
            MIL.MmetAddFeature(MilMetrolContext, MIL.M_MEASURED, MIL.M_POINT, POINT1_LABEL,
                           MIL.M_DEFAULT, MIL.M_NULL, MIL.M_NULL, MIL.M_DEFAULT, MIL.M_DEFAULT);

            MIL.MmetSetRegion(MilMetrolContext, MIL.M_FEATURE_LABEL(POINT1_LABEL), MIL.M_DEFAULT, MIL.M_SEGMENT,
                          SEG1_START_X, SEG1_START_Y, SEG1_END_X, SEG1_END_Y,
                          MIL.M_NULL, MIL.M_NULL);

            MIL.MmetControl(MilMetrolContext, MIL.M_FEATURE_LABEL(POINT1_LABEL), MIL.M_FILTER_MODE, MIL.M_KERNEL);

            // Set the polarity and the maximum number of points to detect along the segment region
            MIL.MmetControl(MilMetrolContext, MIL.M_FEATURE_LABEL(POINT1_LABEL), MIL.M_EDGEL_RELATIVE_ANGLE, MIL.M_SAME_OR_REVERSE);
            MIL.MmetControl(MilMetrolContext, MIL.M_FEATURE_LABEL(POINT1_LABEL), MIL.M_NUMBER_MAX, 2);

            // Add minimum distance tolerance
            MIL.MmetAddTolerance(MilMetrolContext, MIL.M_DISTANCE_MIN, MIN_DISTANCE_LABEL,
                             MIN_DISTANCE_VALUE_MIN, MIN_DISTANCE_VALUE_MAX, MinDistanceFeatureLabels,
                             MIL.M_NULL, 2, MIL.M_DEFAULT);

            // Add angularity tolerance
            MIL.MmetAddTolerance(MilMetrolContext, MIL.M_ANGULARITY, ANGULARITY_LABEL,
                             ANGULARITY_VALUE_MIN, ANGULARITY_VALUE_MAX, AngularityFeatureLabels,
                             MIL.M_NULL, 2, MIL.M_DEFAULT);


            // Add maximum distance tolerance
            MIL.MmetAddTolerance(MilMetrolContext, MIL.M_DISTANCE_MAX, MAX_DISTANCE_LABEL,
                             MAX_DISTANCE_VALUE_MIN, MAX_DISTANCE_VALUE_MAX, MaxDistanceFeatureLabels,
                             MaxDistanceFeatureIndices, 2, MIL.M_DEFAULT);

            // Calculate
            MIL.MmetCalculate(MilMetrolContext, MilImage, MilMetrolResult, MIL.M_DEFAULT);

            // Draw features
            MIL.MgraColor(MIL.M_DEFAULT, REGION_COLOR);
            MIL.MmetDraw(MIL.M_DEFAULT, MilMetrolResult, GraphicList, MIL.M_DRAW_REGION, MIL.M_DEFAULT, MIL.M_DEFAULT);
            Console.Write("Regions used to calculate measured features:\n");
            Console.Write("- two measured circle features\n");
            Console.Write("- two measured segment features\n");
            Console.Write("- one measured points feature\n");
            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();

            // Clear annotations.
            MIL.MgraClear(MIL.M_DEFAULT, GraphicList);

            MIL.MgraColor(MIL.M_DEFAULT, FEATURE_COLOR);
            MIL.MmetDraw(MIL.M_DEFAULT, MilMetrolResult, GraphicList, MIL.M_DRAW_FEATURE, MIL.M_DEFAULT, MIL.M_DEFAULT);
            Console.Write("Calculated features:\n");

            MIL.MmetGetResult(MilMetrolResult, MIL.M_FEATURE_LABEL(CIRCLE1_LABEL), MIL.M_RADIUS, ref Value);
            Console.Write("- first measured circle:   radius={0:0.00}mm\n", Value);

            MIL.MmetGetResult(MilMetrolResult, MIL.M_FEATURE_LABEL(CIRCLE2_LABEL), MIL.M_RADIUS, ref Value);
            Console.Write("- second measured circle:  radius={0:0.00}mm\n", Value);

            MIL.MmetGetResult(MilMetrolResult, MIL.M_FEATURE_LABEL(SEGMENT1_LABEL), MIL.M_LENGTH, ref Value);
            Console.Write("- first measured segment:  length={0:0.00}mm\n", Value);

            MIL.MmetGetResult(MilMetrolResult, MIL.M_FEATURE_LABEL(SEGMENT2_LABEL), MIL.M_LENGTH, ref Value);
            Console.Write("- second measured segment: length={0:0.00}mm\n", Value);

            Console.Write("- two measured points\n");

            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();

            // Get angularity tolerance status and value
            MIL.MmetGetResult(MilMetrolResult, MIL.M_TOLERANCE_LABEL(ANGULARITY_LABEL), MIL.M_STATUS, ref Status);
            MIL.MmetGetResult(MilMetrolResult, MIL.M_TOLERANCE_LABEL(ANGULARITY_LABEL), MIL.M_TOLERANCE_VALUE, ref Value);

            if (Status == MIL.M_PASS)
            {
                MIL.MgraColor(MIL.M_DEFAULT, PASS_COLOR);
                Console.Write("Angularity value: {0:0.00} degrees.\n", Value);
            }
            else
            {
                MIL.MgraColor(MIL.M_DEFAULT, FAIL_COLOR);
                Console.Write("Angularity value - Fail.\n");
            }
            MIL.MmetDraw(MIL.M_DEFAULT, MilMetrolResult, GraphicList, MIL.M_DRAW_TOLERANCE, MIL.M_TOLERANCE_LABEL(ANGULARITY_LABEL), MIL.M_DEFAULT);

            // Get min distance tolerance status and value
            MIL.MmetGetResult(MilMetrolResult, MIL.M_TOLERANCE_LABEL(MIN_DISTANCE_LABEL), MIL.M_STATUS, ref Status);
            MIL.MmetGetResult(MilMetrolResult, MIL.M_TOLERANCE_LABEL(MIN_DISTANCE_LABEL), MIL.M_TOLERANCE_VALUE, ref Value);

            if (Status == MIL.M_PASS)
            {
                MIL.MgraColor(MIL.M_DEFAULT, PASS_COLOR);
                Console.Write("Min distance tolerance value: {0:0.00} mm.\n", Value);
            }
            else
            {
                MIL.MgraColor(MIL.M_DEFAULT, FAIL_COLOR);
                Console.Write("Min distance tolerance value - Fail.\n");
            }
            MIL.MmetDraw(MIL.M_DEFAULT, MilMetrolResult, GraphicList, MIL.M_DRAW_TOLERANCE, MIL.M_TOLERANCE_LABEL(MIN_DISTANCE_LABEL), MIL.M_DEFAULT);

            // Get max distance tolerance status and value
            MIL.MmetGetResult(MilMetrolResult, MIL.M_TOLERANCE_LABEL(MAX_DISTANCE_LABEL), MIL.M_STATUS, ref Status);
            MIL.MmetGetResult(MilMetrolResult, MIL.M_TOLERANCE_LABEL(MAX_DISTANCE_LABEL), MIL.M_TOLERANCE_VALUE, ref Value);

            if (Status == MIL.M_PASS)
            {
                MIL.MgraColor(MIL.M_DEFAULT, PASS_COLOR);
                Console.Write("Max distance tolerance value: {0:0.00} mm.\n", Value);
            }
            else
            {
                MIL.MgraColor(MIL.M_DEFAULT, FAIL_COLOR);
                Console.Write("Max distance tolerance value - Fail.\n");
            }
            MIL.MmetDraw(MIL.M_DEFAULT, MilMetrolResult, GraphicList, MIL.M_DRAW_TOLERANCE, MIL.M_TOLERANCE_LABEL(MAX_DISTANCE_LABEL), MIL.M_DEFAULT);

            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();

            // Clear annotations.
            MIL.MgraClear(MIL.M_DEFAULT, GraphicList);

            // Restore the model finder context and calibrate it
            MIL.MmodRestore(METROL_MODEL_FINDER_FILE, MilSystem, MIL.M_DEFAULT, ref MilModelFinderContext);
            MIL.MmodControl(MilModelFinderContext, 0, MIL.M_ASSOCIATED_CALIBRATION, MilCalibration);

            // Allocate a result buffer
            MIL.MmodAllocResult(MilSystem, MIL.M_DEFAULT, ref MilModelFinderResult);

            // Find object occurrence
            MIL.MmodPreprocess(MilModelFinderContext, MIL.M_DEFAULT);
            MIL.MmodFind(MilModelFinderContext, MilImage, MilModelFinderResult);

            // Get number of found occurrences
            MIL.MmodGetResult(MilModelFinderResult, MIL.M_GENERAL, MIL.M_NUMBER, ref Value);

            if (Value == 1)
            {
                MIL.MmodDraw(MIL.M_DEFAULT, MilModelFinderResult, GraphicList, MIL.M_DRAW_POSITION + MIL.M_DRAW_BOX, MIL.M_DEFAULT, MIL.M_DEFAULT);
                Console.Write("Found occurrence using MIL Model Finder.\n");
                Console.Write("Press <Enter> to continue.\n\n");
                Console.ReadKey();

                // Clear annotations.
                MIL.MgraClear(MIL.M_DEFAULT, GraphicList);

                // Set the new context position
                MIL.MmetSetPosition(MilMetrolContext, MIL.M_DEFAULT, MIL.M_RESULT, MilModelFinderResult, 0, MIL.M_NULL, MIL.M_NULL, MIL.M_DEFAULT);

                // Calculate
                MIL.MmetCalculate(MilMetrolContext, MilImage, MilMetrolResult, MIL.M_DEFAULT);

                // Draw features
                MIL.MgraColor(MIL.M_DEFAULT, REGION_COLOR);
                MIL.MmetDraw(MIL.M_DEFAULT, MilMetrolResult, GraphicList, MIL.M_DRAW_REGION, MIL.M_DEFAULT, MIL.M_DEFAULT);
                Console.Write("Regions used to calculate measured features at the new location.\n");
                Console.Write("Press <Enter> to continue.\n\n");
                Console.ReadKey();

                // Clear annotations.
                MIL.MgraClear(MIL.M_DEFAULT, GraphicList);

                MIL.MgraColor(MIL.M_DEFAULT, FEATURE_COLOR);
                MIL.MmetDraw(MIL.M_DEFAULT, MilMetrolResult, GraphicList, MIL.M_DRAW_FEATURE, MIL.M_DEFAULT, MIL.M_DEFAULT);
                Console.Write("Calculated features.\n");

                MIL.MmetGetResult(MilMetrolResult, MIL.M_FEATURE_LABEL(CIRCLE1_LABEL), MIL.M_RADIUS, ref Value);
                Console.Write("- first measured circle:   radius={0:0.00}mm\n", Value);

                MIL.MmetGetResult(MilMetrolResult, MIL.M_FEATURE_LABEL(CIRCLE2_LABEL), MIL.M_RADIUS, ref Value);
                Console.Write("- second measured circle:  radius={0:0.00}mm\n", Value);

                MIL.MmetGetResult(MilMetrolResult, MIL.M_FEATURE_LABEL(SEGMENT1_LABEL), MIL.M_LENGTH, ref Value);
                Console.Write("- first measured segment:  length={0:0.00}mm\n", Value);

                MIL.MmetGetResult(MilMetrolResult, MIL.M_FEATURE_LABEL(SEGMENT2_LABEL), MIL.M_LENGTH, ref Value);
                Console.Write("- second measured segment: length={0:0.00}mm\n", Value);

                Console.Write("- two measured points\n");

                Console.Write("Press <Enter> to continue.\n\n");
                Console.ReadKey();

                // Get angularity tolerance status and value
                MIL.MmetGetResult(MilMetrolResult, MIL.M_TOLERANCE_LABEL(ANGULARITY_LABEL), MIL.M_STATUS, ref Status);
                MIL.MmetGetResult(MilMetrolResult, MIL.M_TOLERANCE_LABEL(ANGULARITY_LABEL), MIL.M_TOLERANCE_VALUE, ref Value);

                if (Status == MIL.M_PASS)
                {
                    MIL.MgraColor(MIL.M_DEFAULT, PASS_COLOR);
                    Console.Write("Angularity value: {0:0.00} degrees.\n", Value);
                }
                else
                {
                    MIL.MgraColor(MIL.M_DEFAULT, FAIL_COLOR);
                    Console.Write("Angularity value - Fail.\n");
                }
                MIL.MmetDraw(MIL.M_DEFAULT, MilMetrolResult, GraphicList, MIL.M_DRAW_TOLERANCE, MIL.M_TOLERANCE_LABEL(ANGULARITY_LABEL), MIL.M_DEFAULT);

                // Get min distance tolerance status and value
                MIL.MmetGetResult(MilMetrolResult, MIL.M_TOLERANCE_LABEL(MIN_DISTANCE_LABEL), MIL.M_STATUS, ref Status);
                MIL.MmetGetResult(MilMetrolResult, MIL.M_TOLERANCE_LABEL(MIN_DISTANCE_LABEL), MIL.M_TOLERANCE_VALUE, ref Value);

                if (Status == MIL.M_PASS)
                {
                    MIL.MgraColor(MIL.M_DEFAULT, PASS_COLOR);
                    Console.Write("Min distance tolerance value: {0:0.00} mm.\n", Value);
                }
                else
                {
                    MIL.MgraColor(MIL.M_DEFAULT, FAIL_COLOR);
                    Console.Write("Min distance tolerance value - Fail.\n");
                }
                MIL.MmetDraw(MIL.M_DEFAULT, MilMetrolResult, GraphicList, MIL.M_DRAW_TOLERANCE, MIL.M_TOLERANCE_LABEL(MIN_DISTANCE_LABEL), MIL.M_DEFAULT);

                // Get max distance tolerance status and value
                MIL.MmetGetResult(MilMetrolResult, MIL.M_TOLERANCE_LABEL(MAX_DISTANCE_LABEL), MIL.M_STATUS, ref Status);
                MIL.MmetGetResult(MilMetrolResult, MIL.M_TOLERANCE_LABEL(MAX_DISTANCE_LABEL), MIL.M_TOLERANCE_VALUE, ref Value);

                if (Status == MIL.M_PASS)
                {
                    MIL.MgraColor(MIL.M_DEFAULT, PASS_COLOR);
                    Console.Write("Max distance tolerance value: {0:0.00} mm.\n", Value);
                }
                else
                {
                    MIL.MgraColor(MIL.M_DEFAULT, FAIL_COLOR);
                    Console.Write("Max distance tolerance value - Fail.\n");
                }
                MIL.MmetDraw(MIL.M_DEFAULT, MilMetrolResult, GraphicList, MIL.M_DRAW_TOLERANCE, MIL.M_TOLERANCE_LABEL(MAX_DISTANCE_LABEL), MIL.M_DEFAULT);

                Console.Write("Press <Enter> to quit.\n\n");
                Console.ReadKey();
            }
            else
            {
                Console.Write("Occurrence not found.\n");
                Console.Write("Press <Enter> to quit.\n\n");
                Console.ReadKey();
            }

            // Free all allocations.
            MIL.MgraFree(GraphicList);
            MIL.MmodFree(MilModelFinderContext);
            MIL.MmodFree(MilModelFinderResult);
            MIL.MmetFree(MilMetrolResult);
            MIL.MmetFree(MilMetrolContext);
            MIL.McalFree(MilCalibration);
            MIL.MbufFree(MilImage);
        }
    }
}
