//*******************************************************************************
//
// File name: Mcal.cs
//
// Synopsis:  This program uses the Calibration module to calibrate a high
//            distortion camera with an image of a grid. Then, an image of
//            a board grabbed with the same camera is loaded and some
//            measurements are done in real world calibrated units. Finally
//            the distorted board image is corrected using the same camera
//            calibration.
//*******************************************************************************
using System;
using System.Collections.Generic;
using System.Text;

using Matrox.MatroxImagingLibrary;

namespace Mcal
{
    class Program
    {
        // Example selection.
        private const int RUN_LINEAR_CALIBRATION_EXAMPLE = MIL.M_YES;
        private const int RUN_TSAI_CALIBRATION_EXAMPLE = MIL.M_YES;

        static void Main(string[] args)
        {
            MIL_ID MilApplication = MIL.M_NULL; // Application identifier.
            MIL_ID MilSystem = MIL.M_NULL;      // System Identifier.
            MIL_ID MilDisplay = MIL.M_NULL;     // Display identifier.

            // Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, ref MilApplication, ref MilSystem, ref MilDisplay, MIL.M_NULL, MIL.M_NULL);

            // Print module name.
            Console.Write("CALIBRATION MODULE:\n");
            Console.Write("-------------------\n\n");

            if (RUN_LINEAR_CALIBRATION_EXAMPLE == MIL.M_YES)
            {
                LinearInterpolationCalibration(MilSystem, MilDisplay);
            }

            if (RUN_TSAI_CALIBRATION_EXAMPLE == MIL.M_YES)
            {
                TsaiCalibration(MilSystem, MilDisplay);
            }

            // Free defaults.
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL);
        }

        //****************************************************************************
        // Linear interpolation example. 
        //****************************************************************************

        // Source image files specification.
        private const string GRID_IMAGE_FILE = MIL.M_IMAGE_PATH + "CalGrid.mim";
        private const string BOARD_IMAGE_FILE = MIL.M_IMAGE_PATH + "CalBoard.mim";

        // World description of the calibration grid.
        private const int GRID_OFFSET_X = 0;
        private const int GRID_OFFSET_Y = 0;
        private const int GRID_OFFSET_Z = 0;
        private const int GRID_ROW_SPACING = 1;
        private const int GRID_COLUMN_SPACING = 1;
        private const int GRID_ROW_NUMBER = 18;
        private const int GRID_COLUMN_NUMBER = 25;

        // Measurement boxes specification.
        private const int MEAS_BOX_POS_X1 = 55;
        private const int MEAS_BOX_POS_Y1 = 24;
        private const int MEAS_BOX_WIDTH1 = 7;
        private const int MEAS_BOX_HEIGHT1 = 425;

        private const int MEAS_BOX_POS_X2 = 225;
        private const int MEAS_BOX_POS_Y2 = 11;
        private const int MEAS_BOX_WIDTH2 = 7;
        private const int MEAS_BOX_HEIGHT2 = 450;

        // Specification of the stripes' constraints.
        private const int WIDTH_APPROXIMATION = 410;
        private const int WIDTH_VARIATION = 25;
        private const int MIN_EDGE_VALUE = 5;

        static void LinearInterpolationCalibration(MIL_ID MilSystem, MIL_ID MilDisplay)
        {
            MIL_ID MilImage = MIL.M_NULL;          // Image buffer identifier.
            MIL_ID MilOverlayImage = MIL.M_NULL;   // Overlay image.
            MIL_ID MilCalibration = MIL.M_NULL;    // Calibration identifier.
            MIL_ID MeasMarker1 = MIL.M_NULL;       // Measurement marker identifier.
            MIL_ID MeasMarker2 = MIL.M_NULL;       // Measurement marker identifier.
            double WorldDistance1 = 0.0;
            double WorldDistance2 = 0.0;
            double PixelDistance1 = 0.0;
            double PixelDistance2 = 0.0;
            double PosX1 = 0.0;
            double PosY1 = 0.0;
            double PosX2 = 0.0;
            double PosY2 = 0.0;
            double PosX3 = 0.0;
            double PosY3 = 0.0;
            double PosX4 = 0.0;
            double PosY4 = 0.0;
            MIL_INT CalibrationStatus = 0;

            // Clear the display.
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY_CLEAR, MIL.M_DEFAULT);

            // Restore source image into an automatically allocated image buffer.
            MIL.MbufRestore(GRID_IMAGE_FILE, MilSystem, ref MilImage);

            // Display the image buffer.
            MIL.MdispSelect(MilDisplay, MilImage);

            // Prepare for overlay annotation.
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY, MIL.M_ENABLE);
            MIL.MdispInquire(MilDisplay, MIL.M_OVERLAY_ID, ref MilOverlayImage);

            // Pause to show the original image.
            Console.Write("\nLINEAR INTERPOLATION CALIBRATION:\n");
            Console.Write("------------\n\n");
            Console.Write("The displayed grid has been grabbed with a high distortion\n");
            Console.Write("camera and will be used to calibrate the camera.\n");
            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();

            // Allocate a camera calibration context.
            MIL.McalAlloc(MilSystem, MIL.M_DEFAULT, MIL.M_DEFAULT, ref MilCalibration);

            // Calibrate the camera with the image of the grid and its world description.
            MIL.McalGrid(MilCalibration, MilImage, GRID_OFFSET_X, GRID_OFFSET_Y, GRID_OFFSET_Z, GRID_ROW_NUMBER, GRID_COLUMN_NUMBER, GRID_ROW_SPACING, GRID_COLUMN_SPACING, MIL.M_DEFAULT, MIL.M_DEFAULT);

            MIL.McalInquire(MilCalibration, MIL.M_CALIBRATION_STATUS + MIL.M_TYPE_MIL_INT, ref CalibrationStatus);
            if (CalibrationStatus == MIL.M_CALIBRATED)
            {
                // Perform a first image transformation with the calibration grid.
                MIL.McalTransformImage(MilImage, MilImage, MilCalibration, MIL.M_BILINEAR | MIL.M_OVERSCAN_CLEAR, MIL.M_DEFAULT, MIL.M_DEFAULT);

                // Pause to show the corrected image of the grid.
                Console.Write("The camera has been calibrated and the image of the grid\n");
                Console.Write("has been transformed to remove its distortions.\n");
                Console.Write("Press <Enter> to continue.\n\n");
                Console.ReadKey();

                // Read the image of the board and associate the calibration to the image.
                MIL.MbufLoad(BOARD_IMAGE_FILE, MilImage);
                MIL.McalAssociate(MilCalibration, MilImage, MIL.M_DEFAULT);

                // Allocate the measurement markers.
                MIL.MmeasAllocMarker(MilSystem, MIL.M_STRIPE, MIL.M_DEFAULT, ref MeasMarker1);
                MIL.MmeasAllocMarker(MilSystem, MIL.M_STRIPE, MIL.M_DEFAULT, ref MeasMarker2);

                // Set the markers' measurement search region.
                MIL.MmeasSetMarker(MeasMarker1, MIL.M_BOX_ORIGIN, MEAS_BOX_POS_X1, MEAS_BOX_POS_Y1);
                MIL.MmeasSetMarker(MeasMarker1, MIL.M_BOX_SIZE, MEAS_BOX_WIDTH1, MEAS_BOX_HEIGHT1);
                MIL.MmeasSetMarker(MeasMarker2, MIL.M_BOX_ORIGIN, MEAS_BOX_POS_X2, MEAS_BOX_POS_Y2);
                MIL.MmeasSetMarker(MeasMarker2, MIL.M_BOX_SIZE, MEAS_BOX_WIDTH2, MEAS_BOX_HEIGHT2);

                // Set markers' orientation.
                MIL.MmeasSetMarker(MeasMarker1, MIL.M_ORIENTATION, MIL.M_HORIZONTAL, MIL.M_NULL);
                MIL.MmeasSetMarker(MeasMarker2, MIL.M_ORIENTATION, MIL.M_HORIZONTAL, MIL.M_NULL);

                // Set markers' settings to locate the largest stripe within the range
                // [WIDTH_APPROXIMATION - WIDTH_VARIATION,
                //  WIDTH_APPROXIMATION + WIDTH_VARIATION],
                // and with an edge strength over MIN_EDGE_VALUE.
                MIL.MmeasSetMarker(MeasMarker1, MIL.M_EDGEVALUE_MIN, MIN_EDGE_VALUE, MIL.M_NULL);

                // Remove the default strength characteristic score mapping.
                MIL.MmeasSetScore(MeasMarker1, MIL.M_STRENGTH_SCORE,
                                           0.0,
                                           0.0,
                                           MIL.M_MAX_POSSIBLE_VALUE,
                                           MIL.M_MAX_POSSIBLE_VALUE,
                                           MIL.M_DEFAULT,
                                           MIL.M_DEFAULT,
                                           MIL.M_DEFAULT);

                // Add a width characteristic score mapping (increasing ramp)
                // to find the largest stripe within a given range.
                //
                // Width score mapping to find the largest stripe within a given
                // width range ]Wmin, Wmax]:
                //
                //    Score
                //       ^
                //       |         /|
                //       |       /  |
                //       |     /    |
                //       +---------------> Width
                //           Wmin  Wmax
                //
                MIL.MmeasSetScore(MeasMarker1, MIL.M_STRIPE_WIDTH_SCORE,
                                           WIDTH_APPROXIMATION - WIDTH_VARIATION,
                                           WIDTH_APPROXIMATION + WIDTH_VARIATION,
                                           WIDTH_APPROXIMATION + WIDTH_VARIATION,
                                           WIDTH_APPROXIMATION + WIDTH_VARIATION,
                                           MIL.M_DEFAULT,
                                           MIL.M_PIXEL,
                                           MIL.M_DEFAULT);

                /* Set the same settings for the second marker. */
                MIL.MmeasSetMarker(MeasMarker2, MIL.M_EDGEVALUE_MIN, MIN_EDGE_VALUE, MIL.M_NULL);

                MIL.MmeasSetScore(MeasMarker2, MIL.M_STRENGTH_SCORE,
                                           0.0,
                                           0.0,
                                           MIL.M_MAX_POSSIBLE_VALUE,
                                           MIL.M_MAX_POSSIBLE_VALUE,
                                           MIL.M_DEFAULT,
                                           MIL.M_DEFAULT,
                                           MIL.M_DEFAULT);

                MIL.MmeasSetScore(MeasMarker2, MIL.M_STRIPE_WIDTH_SCORE,
                                           WIDTH_APPROXIMATION - WIDTH_VARIATION,
                                           WIDTH_APPROXIMATION + WIDTH_VARIATION,
                                           WIDTH_APPROXIMATION + WIDTH_VARIATION,
                                           WIDTH_APPROXIMATION + WIDTH_VARIATION,
                                           MIL.M_DEFAULT,
                                           MIL.M_PIXEL,
                                           MIL.M_DEFAULT);

                // Find and measure the position and width of the board.
                MIL.MmeasFindMarker(MIL.M_DEFAULT, MilImage, MeasMarker1, MIL.M_STRIPE_WIDTH + MIL.M_POSITION);
                MIL.MmeasFindMarker(MIL.M_DEFAULT, MilImage, MeasMarker2, MIL.M_STRIPE_WIDTH + MIL.M_POSITION);

                // Get the world width of the two markers.
                MIL.MmeasGetResult(MeasMarker1, MIL.M_STRIPE_WIDTH, ref WorldDistance1);
                MIL.MmeasGetResult(MeasMarker2, MIL.M_STRIPE_WIDTH, ref WorldDistance2);

                // Get the pixel width of the two markers.
                MIL.MmeasSetMarker(MeasMarker1, MIL.M_RESULT_OUTPUT_UNITS, MIL.M_PIXEL, MIL.M_NULL);
                MIL.MmeasSetMarker(MeasMarker2, MIL.M_RESULT_OUTPUT_UNITS, MIL.M_PIXEL, MIL.M_NULL);
                MIL.MmeasGetResult(MeasMarker1, MIL.M_STRIPE_WIDTH, ref PixelDistance1);
                MIL.MmeasGetResult(MeasMarker2, MIL.M_STRIPE_WIDTH, ref PixelDistance2);

                // Get the edges position in pixel to draw the annotations.
                MIL.MmeasGetResult(MeasMarker1, MIL.M_POSITION + MIL.M_EDGE_FIRST, ref PosX1, ref PosY1);
                MIL.MmeasGetResult(MeasMarker1, MIL.M_POSITION + MIL.M_EDGE_SECOND, ref PosX2, ref PosY2);
                MIL.MmeasGetResult(MeasMarker2, MIL.M_POSITION + MIL.M_EDGE_FIRST, ref PosX3, ref PosY3);
                MIL.MmeasGetResult(MeasMarker2, MIL.M_POSITION + MIL.M_EDGE_SECOND, ref PosX4, ref PosY4);

                // Draw the measurement indicators on the image. 
                MIL.MgraColor(MIL.M_DEFAULT, MIL.M_COLOR_YELLOW);
                MIL.MmeasDraw(MIL.M_DEFAULT, MeasMarker1, MilOverlayImage, MIL.M_DRAW_WIDTH, MIL.M_DEFAULT, MIL.M_RESULT);
                MIL.MmeasDraw(MIL.M_DEFAULT, MeasMarker2, MilOverlayImage, MIL.M_DRAW_WIDTH, MIL.M_DEFAULT, MIL.M_RESULT);

                MIL.MgraBackColor(MIL.M_DEFAULT, MIL.M_COLOR_BLACK);
                MIL.MgraText(MIL.M_DEFAULT, MilOverlayImage, (int)(PosX1 + 0.5 - 40), (int)((PosY1 + 0.5) + ((PosY2 - PosY1) / 2.0)), " Distance 1 ");
                MIL.MgraText(MIL.M_DEFAULT, MilOverlayImage, (int)(PosX3 + 0.5 - 40), (int)((PosY3 + 0.5) + ((PosY4 - PosY3) / 2.0)), " Distance 2 ");

                // Pause to show the original image and the measurement results.
                Console.Write("A distorted image grabbed with the same camera was loaded and\n");
                Console.Write("calibrated measurements were done to evaluate the board dimensions.\n");
                Console.Write("\n========================================================\n");
                Console.Write("                      Distance 1          Distance 2 \n");
                Console.Write("--------------------------------------------------------\n");
                Console.Write(" Calibrated unit:   {0,8:0.00} cm           {1,6:0.00} cm    \n", WorldDistance1, WorldDistance2);
                Console.Write(" Uncalibrated unit: {0,8:0.00} pixels       {1,6:0.00} pixels\n", PixelDistance1, PixelDistance2);
                Console.Write("========================================================\n\n");
                Console.Write("Press <Enter> to continue.\n\n");
                Console.ReadKey();

                // Clear the display overlay.
                MIL.MdispControl(MilDisplay, MIL.M_OVERLAY_CLEAR, MIL.M_DEFAULT);

                // Read the image of the PCB.
                MIL.MbufLoad(BOARD_IMAGE_FILE, MilImage);

                // Transform the image of the board.
                MIL.McalTransformImage(MilImage, MilImage, MilCalibration, MIL.M_BILINEAR + MIL.M_OVERSCAN_CLEAR, MIL.M_DEFAULT, MIL.M_DEFAULT);

                // show the transformed image of the board.
                Console.Write("The image was corrected to remove its distortions.\n");

                // Free measurement markers.
                MIL.MmeasFree(MeasMarker1);
                MIL.MmeasFree(MeasMarker2);
            }
            else
            {
                Console.Write("Calibration generated an exception.\n");
                Console.Write("See User Guide to resolve the situation.\n\n");
            }

            // Wait for a key to be pressed.
            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();

            // Free all allocations.
            MIL.McalFree(MilCalibration);
            MIL.MbufFree(MilImage);
        }

        //****************************************************************************
        // Tsai example. 
        //****************************************************************************
        // Source image files specification.
        private const string GRID_ORIGINAL_IMAGE_FILE = MIL.M_IMAGE_PATH + "CalGridOriginal.mim";
        private const string OBJECT_ORIGINAL_IMAGE_FILE = MIL.M_IMAGE_PATH + "CalObjOriginal.mim";
        private const string OBJECT_MOVED_IMAGE_FILE = MIL.M_IMAGE_PATH + "CalObjMoved.mim";

        // World description of the calibration grid.
        private const double GRID_ORG_ROW_SPACING = 1.5;
        private const double GRID_ORG_COLUMN_SPACING = 1.5;
        private const int GRID_ORG_ROW_NUMBER = 12;
        private const int GRID_ORG_COLUMN_NUMBER = 13;
        private const int GRID_ORG_OFFSET_X = 0;
        private const int GRID_ORG_OFFSET_Y = 0;
        private const int GRID_ORG_OFFSET_Z = 0;

        // Camera transformation
        private const double ENCODER_TRANSLATION_X = 0.817;
        private const double ENCODER_TRANSLATION_Y = 13.293;
        private const double ENCODER_TRANSLATION_Z = -14.060;
        private const double ENCODER_ROTATION_Y = 354.207;
        private const double ENCODER_ROTATION_X = 317.467;
        private const double ENCODER_ROTATION_Z = 281.969;

        // Region parameters for metrology
        private const int MEASURED_CIRCLE_LABEL = 1;
        private const double RING1_POS1_X = 2.3 ;
        private const double RING1_POS1_Y = 3.9;
        private const double RING2_POS1_X = 10.7;
        private const double RING2_POS1_Y = 11.1;

        private const double RING1_POS2_X = 8.6;
        private const double RING1_POS2_Y = 2.5;
        private const double RING2_POS2_X = 7.9;
        private const double RING2_POS2_Y = 13.2;

        private const double RING_START_RADIUS = 1.25;
        private const double RING_END_RADIUS = 2.3;

        // measured plane position
        private const double RING_THICKNESS = 0.175;
        private const double STEP_THICKNESS = 4.0;

        // Color definitions
        static readonly int REGION_COLOR = MIL.M_RGB888(0, 100, 255);
        static readonly int FEATURE_COLOR = MIL.M_RGB888(255, 0, 255);

        static void TsaiCalibration(MIL_ID MilSystem, MIL_ID MilDisplay)
        {
            MIL_ID MilImage = MIL.M_NULL;          // Image buffer identifier.
            MIL_ID MilCalibration = MIL.M_NULL;    // Calibration identifier.

            MIL_INT CalibrationStatus = 0;

            // Restore source image into an automatically allocated image buffer.
            MIL.MbufRestore(GRID_ORIGINAL_IMAGE_FILE, MilSystem, ref MilImage);

            // Display the image buffer.
            MIL.MdispSelect(MilDisplay, MilImage);

            // Pause to show the original image.
            Console.Write("TSAI BASED CALIBRATION:\n");
            Console.Write("------------\n\n");
            Console.Write("The displayed grid has been grabbed with a high perspective\n");
            Console.Write("camera and will be used to calibrate the camera.\n");
            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();

            // Allocate a camera calibration context.
            MIL.McalAlloc(MilSystem, MIL.M_TSAI_BASED, MIL.M_DEFAULT, ref MilCalibration);

            // Calibrate the camera with the image of the grid and its world description.
            MIL.McalGrid(MilCalibration, MilImage, GRID_ORG_OFFSET_X, GRID_ORG_OFFSET_Y, GRID_ORG_OFFSET_Z, GRID_ORG_ROW_NUMBER, GRID_ORG_COLUMN_NUMBER, GRID_ORG_ROW_SPACING, GRID_ORG_COLUMN_SPACING, MIL.M_DEFAULT, MIL.M_DEFAULT);

            // Verify if the camera calibration was successful
            MIL.McalInquire(MilCalibration, MIL.M_CALIBRATION_STATUS + MIL.M_TYPE_MIL_INT, ref CalibrationStatus);
            if (CalibrationStatus == MIL.M_CALIBRATED)
            {
                // Print camera information
                Console.Write("The camera has been calibrated.\n\n");
                ShowCameraInformation(MilCalibration);

                // Load source image into an image buffer.
                MIL.MbufLoad(OBJECT_ORIGINAL_IMAGE_FILE, MilImage);

                // Set offset to the camera calibration plane and associate calibration to image.
                // This moves the relative origin to the top of the first measured circle 
                SetCalibrationOffset(MilCalibration, -RING_THICKNESS, MilImage);

                // Measure the first circle.
                Console.Write("First measured circle at  z = {0:0.00} cm. ", -RING_THICKNESS);
                MeasureRing(MilSystem, MilDisplay, MilImage, RING1_POS1_X, RING1_POS1_Y);

                // Set offset to the camera calibration plane and associate calibration to image.
                // This moves the relative origin to the top of the second measured circle 
                SetCalibrationOffset(MilCalibration, -(STEP_THICKNESS + RING_THICKNESS), MilImage);

                // Measure the second circle.
                Console.Write("Second measured circle at z = {0:0.00} cm. ", -(STEP_THICKNESS + RING_THICKNESS));
                MeasureRing(MilSystem, MilDisplay, MilImage, RING2_POS1_X, RING2_POS1_Y);
                Console.Write("Press <Enter> to continue.\n\n");
                Console.ReadKey();

                // Clear the overlay to transparent.
                MIL.MdispControl(MilDisplay, MIL.M_OVERLAY_CLEAR, MIL.M_DEFAULT);

                // Move camera rotation with the tool
                MIL.McalSetCoordinateSystem(MilCalibration, MIL.M_TOOL_COORDINATE_SYSTEM, MIL.M_ABSOLUTE_COORDINATE_SYSTEM, MIL.M_ROTATION_YXZ | MIL.M_ASSIGN, MIL.M_NULL, ENCODER_ROTATION_Y, ENCODER_ROTATION_X, ENCODER_ROTATION_Z, MIL.M_DEFAULT);

                // Move camera position with the tool
                MIL.McalSetCoordinateSystem(MilCalibration, MIL.M_TOOL_COORDINATE_SYSTEM, MIL.M_ABSOLUTE_COORDINATE_SYSTEM, MIL.M_TRANSLATION | MIL.M_COMPOSE_WITH_CURRENT, MIL.M_NULL, ENCODER_TRANSLATION_X, ENCODER_TRANSLATION_Y, ENCODER_TRANSLATION_Z, MIL.M_DEFAULT);
                // Load source image into an image buffer.
                MIL.MbufLoad(OBJECT_MOVED_IMAGE_FILE, MilImage);

                // Print camera information
                Console.Write("Calibration module moved camera position and camera \n");
                Console.Write("orientation according to known encoder transformation.\n\n");
                ShowCameraInformation(MilCalibration);

                // Set offset to the camera calibration plane and associate calibration to image.
                // This moves the relative origin to the top of the first measured circle  
                SetCalibrationOffset(MilCalibration, -RING_THICKNESS, MilImage);

                // Measure the first circle.
                Console.Write("First measured circle at  z = {0:0.00} cm. ", -RING_THICKNESS);
                MeasureRing(MilSystem, MilDisplay, MilImage, RING1_POS2_X, RING1_POS2_Y);

                // Set offset to the camera calibration plane and associate calibration to image.
                // This moves the relative origin to the top of the second measured circle 
                SetCalibrationOffset(MilCalibration, -(STEP_THICKNESS + RING_THICKNESS), MilImage);

                // Measure the second circle.
                Console.Write("Second measured circle at z = {0:0.00} cm. ", -(STEP_THICKNESS + RING_THICKNESS));
                MeasureRing(MilSystem, MilDisplay, MilImage, RING2_POS2_X, RING2_POS2_Y);

                Console.Write("Press <Enter> to quit.\n\n");
                Console.ReadKey();
            }
            else
            {
                Console.Write("Calibration generated an exception.\n");
                Console.Write("See User Guide to resolve the situation.\n\n");
            }

            // Free all allocations.
            MIL.McalFree(MilCalibration);
            MIL.MbufFree(MilImage);
        }

        // Offset camera calibration plane.
        static void SetCalibrationOffset(MIL_ID MilCalibration, double ZOffset, MIL_ID MilImage)
        {
            MIL.McalSetCoordinateSystem(MilCalibration, MIL.M_RELATIVE_COORDINATE_SYSTEM, MIL.M_ABSOLUTE_COORDINATE_SYSTEM, MIL.M_TRANSLATION | MIL.M_ASSIGN, MIL.M_NULL, 0, 0, ZOffset, MIL.M_DEFAULT);

            // Associate calibration context to source image
            MIL.McalAssociate(MilCalibration, MilImage, MIL.M_DEFAULT);
        }

        // Measuring function with MilMetrology module
        static void MeasureRing(MIL_ID MilSystem, MIL_ID MilDisplay, MIL_ID MilImage, double MeasureRingX, double MeasureRingY)
        {
            MIL_ID MilMetrolContext = MIL.M_NULL;  // Metrology Context.
            MIL_ID MilMetrolResult = MIL.M_NULL;   // Metrology Result.
            MIL_ID MilOverlayImage = MIL.M_NULL;   // Overlay image.

            double Value = 0.0;

            // Prepare for overlay annotation.
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY, MIL.M_ENABLE);
            MIL.MdispInquire(MilDisplay, MIL.M_OVERLAY_ID, ref MilOverlayImage);

            // Allocate metrology context and result.
            MIL.MmetAlloc(MilSystem, MIL.M_DEFAULT, ref MilMetrolContext);
            MIL.MmetAllocResult(MilSystem, MIL.M_DEFAULT, ref MilMetrolResult);

            // Add a first measured segment feature to context and set its search region.
            MIL.MmetAddFeature(MilMetrolContext, MIL.M_MEASURED, MIL.M_CIRCLE, MEASURED_CIRCLE_LABEL, MIL.M_DEFAULT, MIL.M_NULL, MIL.M_NULL, MIL.M_DEFAULT, MIL.M_DEFAULT);

            MIL.MmetSetRegion(MilMetrolContext, MIL.M_FEATURE_LABEL(MEASURED_CIRCLE_LABEL), MIL.M_DEFAULT, MIL.M_RING, MeasureRingX, MeasureRingY, RING_START_RADIUS, RING_END_RADIUS, MIL.M_NULL, MIL.M_NULL);

            // Calculate.
            MIL.MmetCalculate(MilMetrolContext, MilImage, MilMetrolResult, MIL.M_DEFAULT);

            // Draw region.
            MIL.MgraColor(MIL.M_DEFAULT, REGION_COLOR);
            MIL.MmetDraw(MIL.M_DEFAULT, MilMetrolResult, MilOverlayImage, MIL.M_DRAW_REGION, MIL.M_DEFAULT, MIL.M_DEFAULT);

            // Draw features.
            MIL.MgraColor(MIL.M_DEFAULT, FEATURE_COLOR);
            MIL.MmetDraw(MIL.M_DEFAULT, MilMetrolResult, MilOverlayImage, MIL.M_DRAW_FEATURE, MIL.M_DEFAULT, MIL.M_DEFAULT);

            MIL.MmetGetResult(MilMetrolResult, MIL.M_FEATURE_LABEL(MEASURED_CIRCLE_LABEL), MIL.M_RADIUS, ref Value);
            Console.Write("Measured Radius: {0:0.000} cm\n", Value);

            // Free all allocations.
            MIL.MmetFree(MilMetrolResult);
            MIL.MmetFree(MilMetrolContext);
        }

        // Print the current camera position and orientation 
        static void ShowCameraInformation(MIL_ID MilCalibration)
        {
            double CameraPosX = 0.0;
            double CameraPosY = 0.0;
            double CameraPosZ = 0.0;
            double CameraYaw = 0.0;
            double CameraPitch = 0.0;
            double CameraRoll = 0.0;

            MIL.McalGetCoordinateSystem(MilCalibration, MIL.M_CAMERA_COORDINATE_SYSTEM, MIL.M_ABSOLUTE_COORDINATE_SYSTEM, MIL.M_TRANSLATION, MIL.M_NULL, ref CameraPosX, ref CameraPosY, ref CameraPosZ, MIL.M_NULL);
            MIL.McalGetCoordinateSystem(MilCalibration, MIL.M_CAMERA_COORDINATE_SYSTEM, MIL.M_ABSOLUTE_COORDINATE_SYSTEM, MIL.M_ROTATION_YXZ, MIL.M_NULL, ref CameraYaw, ref CameraPitch, ref CameraRoll, MIL.M_NULL);

            // Pause to show the corrected image of the grid.
            Console.Write("Camera Position in cm:          (x, y, z)           ({0:0.00}, {1:0.00}, {2:0.00})\n", CameraPosX, CameraPosY, CameraPosZ);
            Console.Write("Camera Orientation in degrees:  (yaw, pitch, roll)  ({0:0.00}, {1:0.00}, {2:0.00})\n", CameraYaw, CameraPitch, CameraRoll);
            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();
        }
    }
}
