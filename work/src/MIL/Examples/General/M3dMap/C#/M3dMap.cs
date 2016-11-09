//*****************************************************************************
//
// File name: M3dmap.cs
//
// Synopsis:  This program inspects a wood surface using laser profiling to find
//            any depth defects.
//*****************************************************************************
using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using Matrox.MatroxImagingLibrary;

namespace M3dmap
{
    class Program
    {
        // Function declarations for DirectX display
        [DllImport("mdispd3d.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr MdepthD3DAlloc(MIL_ID DepthBuffer, MIL_ID IntensityBuffer, MIL_INT DisplaySizeX, MIL_INT DisplaySizeY, double ScaleX, double ScaleY, double ScaleZ, double MinZ, double MaxZ, double MaxDistanceZ, IntPtr WindowHandle);
        [DllImport("mdispd3d.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void MdispD3DFree(IntPtr DispHandle);
        [DllImport("mdispd3d.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void MdispD3DShow(IntPtr DispHandle);
        [DllImport("mdispd3d.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void MdispD3DHide(IntPtr DispHandle);
        [DllImport("mdispd3d.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void MdispD3DPrintHelp(IntPtr DispHandle);

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

            // Run the depth correction example.
            DepthCorrectionExample(MilSystem, MilDisplay);

            // Run the calibrated camera example.
            CalibratedCameraExample(MilSystem, MilDisplay);

            // Free defaults.
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL);
        }

        //***************************************************************************
        // Depth correction example.
        //***************************************************************************

        // Input sequence specifications.
        const string REFERENCE_PLANES_SEQUENCE_FILE = MIL.M_IMAGE_PATH + "ReferencePlanes.avi";
        const string OBJECT_SEQUENCE_FILE = MIL.M_IMAGE_PATH + "ScannedObject.avi";

        // Peak detection parameters.
        const int MAX_LINE_WIDTH = 8;
        const int MIN_INTENSITY = 120;

        // Calibration heights in mm.
        static readonly double[] CORRECTED_DEPTHS = { 1.25, 2.50, 3.75, 5.00 };

        const double SCALE_FACTOR = 10000.0; // (depth in world units) * SCALE_FACTOR gives gray levels 

        // Annotation position.
        const double CALIB_TEXT_POS_X = 400;
        const double CALIB_TEXT_POS_Y = 15;

        static void DepthCorrectionExample(MIL_ID MilSystem, MIL_ID MilDisplay)
        {
            MIL_ID MilOverlayImage = MIL.M_NULL;    // Overlay image buffer identifier.
            MIL_ID MilImage = MIL.M_NULL;           // Image buffer identifier (for processing).
            MIL_ID MilDepthMap = MIL.M_NULL;        // Image buffer identifier (for results).
            MIL_ID MilLaser = MIL.M_NULL;           // 3dmap laser profiling context identifier.
            MIL_ID MilScan = MIL.M_NULL;            // 3dmap result buffer identifier.
            MIL_INT SizeX = 0;                      // Width of grabbed images.
            MIL_INT SizeY = 0;                      // Height of grabbed images.
            MIL_INT NbReferencePlanes = 0;          // Number of reference planes of known heights.
            MIL_INT NbObjectImages = 0;             // Number of frames for scanned objects.
            MIL_INT n = 0;                          // Counter.
            double FrameRate = 0.0;                 // Number of grabbed frames per second (in AVI).
            double StartTime = 0.0;                 // Time at the beginning of each iteration.
            double EndTime = 0.0;                   // Time after processing for each iteration.
            double WaitTime = 0.0;                  // Time to wait for next frame.

            // Inquire characteristics of the input sequences.
            MIL.MbufDiskInquire(REFERENCE_PLANES_SEQUENCE_FILE, MIL.M_SIZE_X, ref SizeX);
            MIL.MbufDiskInquire(REFERENCE_PLANES_SEQUENCE_FILE, MIL.M_SIZE_Y, ref SizeY);
            MIL.MbufDiskInquire(REFERENCE_PLANES_SEQUENCE_FILE, MIL.M_NUMBER_OF_IMAGES, ref NbReferencePlanes);
            MIL.MbufDiskInquire(REFERENCE_PLANES_SEQUENCE_FILE, MIL.M_FRAME_RATE, ref FrameRate);
            MIL.MbufDiskInquire(OBJECT_SEQUENCE_FILE, MIL.M_NUMBER_OF_IMAGES, ref NbObjectImages);

            // Allocate buffer to hold images.
            MIL.MbufAlloc2d(MilSystem, SizeX, SizeY, 8 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_DISP + MIL.M_PROC, ref MilImage);
            MIL.MbufClear(MilImage, 0.0);

            Console.WriteLine();
            Console.WriteLine("DEPTH ANALYSIS:");
            Console.WriteLine("---------------");
            Console.WriteLine();
            Console.WriteLine("This program performs a surface inspection to detect depth");
            Console.WriteLine("defects on a wood surface using a laser profiling system.");
            Console.WriteLine();
            Console.WriteLine("Press <Enter> to continue.");
            Console.WriteLine();
            Console.ReadKey();

            // Select display.
            MIL.MdispSelect(MilDisplay, MilImage);

            // Prepare for overlay annotations.
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY, MIL.M_ENABLE);
            MIL.MdispInquire(MilDisplay, MIL.M_OVERLAY_ID, ref MilOverlayImage);
            MIL.MgraControl(MIL.M_DEFAULT, MIL.M_BACKGROUND_MODE, MIL.M_TRANSPARENT);
            MIL.MgraColor(MIL.M_DEFAULT, MIL.M_COLOR_WHITE);

            // Allocate 3dmap objects.
            MIL.M3dmapAlloc(MilSystem, MIL.M_LASER, MIL.M_DEPTH_CORRECTION, ref MilLaser);
            MIL.M3dmapAllocResult(MilSystem, MIL.M_LASER_DATA, MIL.M_DEFAULT, ref MilScan);

            // Set laser line extraction options.
            MIL.M3dmapControl(MilLaser, MIL.M_DEFAULT, MIL.M_PEAK_WIDTH, MAX_LINE_WIDTH);
            MIL.M3dmapControl(MilLaser, MIL.M_DEFAULT, MIL.M_MIN_INTENSITY, MIN_INTENSITY);

            // Open the calibration sequence file for reading.
            MIL.MbufImportSequence(REFERENCE_PLANES_SEQUENCE_FILE, MIL.M_DEFAULT, MIL.M_NULL, MIL.M_NULL, MIL.M_NULL, MIL.M_NULL, MIL.M_NULL, MIL.M_OPEN);

            // Read and process all images in the input sequence.
            MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ + MIL.M_SYNCHRONOUS, ref StartTime);

            for (n = 0; n < NbReferencePlanes; n++)
            {
                string CalibString;

                // Read image from sequence.
                MIL.MbufImportSequence(REFERENCE_PLANES_SEQUENCE_FILE, MIL.M_DEFAULT, MIL.M_LOAD, MIL.M_NULL, ref MilImage, MIL.M_DEFAULT, 1, MIL.M_READ);

                // Annotate the image with the calibration height.
                MIL.MdispControl(MilDisplay, MIL.M_OVERLAY_CLEAR, MIL.M_DEFAULT);
                CalibString = string.Format("Reference plane {0}: {1:0.00} mm", n + 1, CORRECTED_DEPTHS[n]);
                MIL.MgraText(MIL.M_DEFAULT, MilOverlayImage, CALIB_TEXT_POS_X, CALIB_TEXT_POS_Y, CalibString);

                // Set desired corrected depth of next reference plane.
                MIL.M3dmapControl(MilLaser, MIL.M_DEFAULT, MIL.M_CORRECTED_DEPTH, CORRECTED_DEPTHS[n] * SCALE_FACTOR);

                // Analyze the image to extract laser line.
                MIL.M3dmapAddScan(MilLaser, MilScan, MilImage, MIL.M_NULL, MIL.M_NULL, MIL.M_DEFAULT, MIL.M_DEFAULT);

                // Wait to have a proper frame rate, if necessary.
                MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ + MIL.M_SYNCHRONOUS, ref EndTime);
                WaitTime = (1.0 / FrameRate) - (EndTime - StartTime);
                if (WaitTime > 0)
                {
                    MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_WAIT, ref WaitTime);
                }
                MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ + MIL.M_SYNCHRONOUS, ref StartTime);
            }

            // Close the calibration sequence file.
            MIL.MbufImportSequence(REFERENCE_PLANES_SEQUENCE_FILE, MIL.M_DEFAULT, MIL.M_NULL, MIL.M_NULL, MIL.M_NULL, MIL.M_NULL, MIL.M_NULL, MIL.M_CLOSE);

            // Calibrate the laser profiling context using reference planes of known heights.
            MIL.M3dmapCalibrate(MilLaser, MilScan, MIL.M_NULL, MIL.M_DEFAULT);

            Console.WriteLine("The laser profiling system has been calibrated using 4 reference");
            Console.WriteLine("planes of known heights.");
            Console.WriteLine();
            Console.WriteLine("Press <Enter> to continue.");
            Console.WriteLine();
            Console.ReadKey();

            Console.WriteLine("The wood surface is being scanned."); 
            Console.WriteLine();

            // Empty all result buffer contents.
            // It will now be reused for extracting corrected depths.
            MIL.M3dmapAddScan(MIL.M_NULL, MilScan, MIL.M_NULL, MIL.M_NULL, MIL.M_NULL, MIL.M_DEFAULT, MIL.M_RESET);

            // Open the object sequence file for reading.
            MIL.MbufDiskInquire(OBJECT_SEQUENCE_FILE, MIL.M_FRAME_RATE, ref FrameRate);
            MIL.MbufImportSequence(OBJECT_SEQUENCE_FILE, MIL.M_DEFAULT, MIL.M_NULL, MIL.M_NULL, MIL.M_NULL, MIL.M_NULL, MIL.M_NULL, MIL.M_OPEN);

            // Read and process all images in the input sequence.
            MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ + MIL.M_SYNCHRONOUS, ref StartTime);
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY_CLEAR, MIL.M_DEFAULT);

            for (n = 0; n < NbObjectImages; n++)
            {
                // Read image from sequence.
                MIL.MbufImportSequence(OBJECT_SEQUENCE_FILE, MIL.M_DEFAULT, MIL.M_LOAD, MIL.M_NULL, ref MilImage, MIL.M_DEFAULT, 1, MIL.M_READ);

                // Analyze the image to extract laser line and correct its depth.
                MIL.M3dmapAddScan(MilLaser, MilScan, MilImage, MIL.M_NULL, MIL.M_NULL, MIL.M_DEFAULT, MIL.M_DEFAULT);

                // Wait to have a proper frame rate, if necessary.
                MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ + MIL.M_SYNCHRONOUS, ref EndTime);
                WaitTime = (1.0 / FrameRate) - (EndTime - StartTime);
                if (WaitTime > 0)
                {
                    MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_WAIT, ref WaitTime);
                }
                MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ + MIL.M_SYNCHRONOUS, ref StartTime);
            }

            // Close the object sequence file.
            MIL.MbufImportSequence(OBJECT_SEQUENCE_FILE, MIL.M_DEFAULT, MIL.M_NULL, MIL.M_NULL, MIL.M_NULL, MIL.M_NULL, MIL.M_NULL, MIL.M_CLOSE);

            // Allocate the image for a partially corrected depth map.
            MIL.MbufAlloc2d(MilSystem, SizeX, NbObjectImages, 16 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_PROC + MIL.M_DISP, ref MilDepthMap);

            // Get partially corrected depth map from accumulated information in the result buffer.
            MIL.M3dmapExtract(MilScan, MilDepthMap, MIL.M_NULL, MIL.M_CORRECTED_DEPTH_MAP, MIL.M_DEFAULT, MIL.M_DEFAULT);

            // Show partially corrected depth map and find defects.
            SetupColorDisplay(MilSystem, MilDisplay, MIL.MbufInquire(MilDepthMap, MIL.M_SIZE_BIT, MIL.M_NULL));

            // Display partially corrected depth map.
            MIL.MdispSelect(MilDisplay, MilDepthMap);
            MIL.MdispInquire(MilDisplay, MIL.M_OVERLAY_ID, ref MilOverlayImage);

            Console.WriteLine("The pseudo-color depth map of the surface is displayed.");
            Console.WriteLine();
            Console.WriteLine("Press <Enter> to continue.");
            Console.WriteLine();
            Console.ReadKey();

            PerformBlobAnalysis(MilSystem, MilDisplay, MilOverlayImage, MilDepthMap);

            Console.WriteLine("Press <Enter> to continue.");
            Console.WriteLine();
            Console.ReadKey();

            // Disassociates display LUT and clear overlay.
            MIL.MdispLut(MilDisplay, MIL.M_DEFAULT);
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY_CLEAR, MIL.M_DEFAULT);

            // Free all allocations.
            MIL.M3dmapFree(MilScan);
            MIL.M3dmapFree(MilLaser);
            MIL.MbufFree(MilDepthMap);
            MIL.MbufFree(MilImage);
        }

        // Values used for binarization.
        const double EXPECTED_HEIGHT = 3.4;     // Inspected surface should be at this height (in mm)
        const double DEFECT_THRESHOLD = 0.2;    // Max acceptable deviation from expected height (mm)
        const double SATURATED_DEFECT = 1.0;    // Deviation at which defect will appear red (in mm)

        // Radius of the smallest particles to keep.
        const int MIN_BLOB_RADIUS = 3;

        // Pixel offset for drawing text.
        const double TEXT_H_OFFSET_1 = -50;
        const double TEXT_V_OFFSET_1 = -6;
        const double TEXT_H_OFFSET_2 = -30;
        const double TEXT_V_OFFSET_2 = 6;

        // Find defects in corrected depth map, compute max deviation and draw contours.
        static void PerformBlobAnalysis(MIL_ID MilSystem, MIL_ID MilDisplay, MIL_ID MilOverlayImage, MIL_ID MilDepthMap)
        {
            MIL_ID MilBinImage = MIL.M_NULL;            // Binary image buffer identifier.
            MIL_ID MilBlobFeatureList = MIL.M_NULL;     // Feature list identifier.
            MIL_ID MilBlobResult = MIL.M_NULL;          // Blob result buffer identifier.
            MIL_INT SizeX = 0;                          // Width of depth map.
            MIL_INT SizeY = 0;                          // Height of depth map.
            MIL_INT TotalBlobs = 0;                     // Total number of blobs.
            MIL_INT n = 0;                              // Counter.
            MIL_INT[] MinPixels = null;                 // Maximum height of defects.
            double DefectThreshold = 0.0;               // A gray level below it is a defect.
            double[] CogX = null;                       // X coordinate of center of gravity.
            double[] CogY = null;                       // Y coordinate of center of gravity.

            // Get size of depth map.
            MIL.MbufInquire(MilDepthMap, MIL.M_SIZE_X, ref SizeX);
            MIL.MbufInquire(MilDepthMap, MIL.M_SIZE_Y, ref SizeY);

            // Allocate a binary image buffer for fast processing.
            MIL.MbufAlloc2d(MilSystem, SizeX, SizeY, 1 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_PROC, ref MilBinImage);

            // Binarize image.
            DefectThreshold = (EXPECTED_HEIGHT - DEFECT_THRESHOLD) * SCALE_FACTOR;
            MIL.MimBinarize(MilDepthMap, MilBinImage, MIL.M_FIXED + MIL.M_LESS_OR_EQUAL, DefectThreshold, MIL.M_NULL);

            // Remove small particles.
            MIL.MimOpen(MilBinImage, MilBinImage, MIN_BLOB_RADIUS, MIL.M_BINARY);

            // Allocate a feature list.
            MIL.MblobAllocFeatureList(MilSystem, ref MilBlobFeatureList);

            // Enable the Center Of Gravity and Min Pixel features calculation.
            MIL.MblobSelectFeature(MilBlobFeatureList, MIL.M_CENTER_OF_GRAVITY);
            MIL.MblobSelectFeature(MilBlobFeatureList, MIL.M_MIN_PIXEL);

            // Allocate a blob result buffer.
            MIL.MblobAllocResult(MilSystem, ref MilBlobResult);

            // Calculate selected features for each blob.
            MIL.MblobCalculate(MilBinImage, MilDepthMap, MilBlobFeatureList, MilBlobResult);

            // Get the total number of selected blobs.
            MIL.MblobGetNumber(MilBlobResult, ref TotalBlobs);
            Console.WriteLine("Number of defects: {0}", TotalBlobs);

            // Read and print the blob characteristics.
            CogX = new double[TotalBlobs];
            CogY = new double[TotalBlobs];
            MinPixels = new MIL_INT[TotalBlobs];
            if (CogX != null && CogY != null && MinPixels != null)
            {
                // Get the results.
                MIL.MblobGetResult(MilBlobResult, MIL.M_CENTER_OF_GRAVITY_X, CogX);
                MIL.MblobGetResult(MilBlobResult, MIL.M_CENTER_OF_GRAVITY_Y, CogY);
                MIL.MblobGetResult(MilBlobResult, MIL.M_MIN_PIXEL + MIL.M_TYPE_MIL_INT, MinPixels);

                // Draw the defects.
                MIL.MgraColor(MIL.M_DEFAULT, MIL.M_COLOR_RED);
                MIL.MblobDraw(MIL.M_DEFAULT, MilBlobResult, MilOverlayImage, MIL.M_DRAW_BLOBS, MIL.M_INCLUDED_BLOBS, MIL.M_DEFAULT);
                MIL.MgraColor(MIL.M_DEFAULT, MIL.M_COLOR_WHITE);

                // Print the depth of each blob.
                for (n = 0; n < TotalBlobs; n++)
                {
                    double DepthOfDefect;
                    string DepthString;

                    // Write the depth of the defect in the overlay.
                    DepthOfDefect = EXPECTED_HEIGHT - (MinPixels[n] / SCALE_FACTOR);
                    DepthString = string.Format("{0:0.00} mm", DepthOfDefect);

                    Console.WriteLine("Defect #{0}: depth ={1,5:0.00} mm", n, DepthOfDefect);
                    Console.WriteLine();
                    MIL.MgraText(MIL.M_DEFAULT, MilOverlayImage, CogX[n] + TEXT_H_OFFSET_1, CogY[n] + TEXT_V_OFFSET_1, "Defect depth");
                    MIL.MgraText(MIL.M_DEFAULT, MilOverlayImage, CogX[n] + TEXT_H_OFFSET_2, CogY[n] + TEXT_V_OFFSET_2, DepthString);
                }
            }
            else
            {
                Console.WriteLine("Error: Not enough memory.");
                Console.WriteLine();
            }

            // Free all allocations.
            MIL.MblobFree(MilBlobResult);
            MIL.MblobFree(MilBlobFeatureList);
            MIL.MbufFree(MilBinImage);
        }

        // Color constants for display LUT.
        const double BLUE_HUE = 171.0;              // Expected depths will be blue.
        const double RED_HUE = 0.0;                 // Worst defects will be red.
        const int FULL_SATURATION = 255;            // All colors are fully saturated.
        const int HALF_LUMINANCE = 128;             // All colors have half luminance.

        // Creates a color display LUT to show defects in red.
        static void SetupColorDisplay(MIL_ID MilSystem, MIL_ID MilDisplay, MIL_INT SizeBit)
        {
            MIL_ID MilRampLut1Band = MIL.M_NULL;     // LUT containing hue values.
            MIL_ID MilRampLut3Band = MIL.M_NULL;     // RGB LUT used by display.
            MIL_ID MilColorImage = MIL.M_NULL;       // Image used for HSL to RGB conversion.
            MIL_INT DefectGrayLevel = 0;            // Gray level under which all is red.
            MIL_INT ExpectedGrayLevel = 0;          // Gray level over which all is blue.
            MIL_INT NbGrayLevels;

            // Number of possible gray levels in corrected depth map.
            NbGrayLevels = (MIL_INT)(1 << (int)SizeBit);

            // Allocate 1-band LUT that will contain hue values.
            MIL.MbufAlloc1d(MilSystem, NbGrayLevels, 8 + MIL.M_UNSIGNED, MIL.M_LUT, ref MilRampLut1Band);

            // Compute limit gray values.
            DefectGrayLevel = (MIL_INT)((EXPECTED_HEIGHT - SATURATED_DEFECT) * SCALE_FACTOR);
            ExpectedGrayLevel = (MIL_INT)(EXPECTED_HEIGHT * SCALE_FACTOR);

            // Create hue values for each possible gray level.
            MIL.MgenLutRamp(MilRampLut1Band, 0, RED_HUE, DefectGrayLevel, RED_HUE);
            MIL.MgenLutRamp(MilRampLut1Band, DefectGrayLevel, RED_HUE, ExpectedGrayLevel, BLUE_HUE);
            MIL.MgenLutRamp(MilRampLut1Band, ExpectedGrayLevel, BLUE_HUE, NbGrayLevels - 1, BLUE_HUE);

            // Create a HSL image buffer.
            MIL.MbufAllocColor(MilSystem, 3, NbGrayLevels, 1, 8 + MIL.M_UNSIGNED, MIL.M_IMAGE, ref MilColorImage);
            MIL.MbufClear(MilColorImage, MIL.M_RGB888(0, FULL_SATURATION, HALF_LUMINANCE));

            // Set its H band (hue) to the LUT contents and convert the image to RGB.
            MIL.MbufCopyColor2d(MilRampLut1Band, MilColorImage, 0, 0, 0, 0, 0, 0, NbGrayLevels, 1);
            MIL.MimConvert(MilColorImage, MilColorImage, MIL.M_HSL_TO_RGB);

            // Create RGB LUT to give to display and copy image contents.
            MIL.MbufAllocColor(MilSystem, 3, NbGrayLevels, 1, 8 + MIL.M_UNSIGNED, MIL.M_LUT, ref MilRampLut3Band);
            MIL.MbufCopy(MilColorImage, MilRampLut3Band);

            // Associates LUT to display.
            MIL.MdispLut(MilDisplay, MilRampLut3Band);

            // Free all allocations.
            MIL.MbufFree(MilRampLut1Band);
            MIL.MbufFree(MilRampLut3Band);
            MIL.MbufFree(MilColorImage);
        }

        //***************************************************************************
        // Calibrated camera example.
        //***************************************************************************

        // Input sequence specifications.
        const string GRID_FILENAME = MIL.M_IMAGE_PATH + "GridForLaser.mim";
        const string LASERLINE_FILENAME = MIL.M_IMAGE_PATH + "LaserLine.mim";
        const string OBJECT2_SEQUENCE_FILE = MIL.M_IMAGE_PATH + "Cookie.avi";

        // Camera calibration grid parameters.
        static readonly MIL_INT GRID_NB_ROWS = 13;
        static readonly MIL_INT GRID_NB_COLS = 12;
        const double GRID_ROW_SPACING = 5.0;            // in mm
        const double GRID_COL_SPACING = 5.0;            // in mm

        // Laser device setup parameters.
        const double CONVEYOR_SPEED = -0.2;             // in mm/frame

        // Fully corrected depth map generation parameters.
        static readonly MIL_INT DEPTH_MAP_SIZE_X = 480; // in pixels
        static readonly MIL_INT DEPTH_MAP_SIZE_Y = 480; // in pixels
        const double GAP_DEPTH = 1.5;                   // in mm
        const double SCALE_X = 0.15;                    // in mm/pixel
        const double SCALE_Y = 0.15;                    // in mm/pixel
        const double SCALE_Z = -0.0003;                 // in mm/gray level
        const double WORLD_OFFSET_X = -12.0;            // in mm
        const double WORLD_OFFSET_Y = 4.0;              // in mm
        const double WORLD_OFFSET_Z = 0.0;              // in mm

        // D3D display parameters 
        static readonly MIL_INT D3D_DISPLAY_SIZE_X = 640;
        static readonly MIL_INT D3D_DISPLAY_SIZE_Y = 480;

        // Peak detection parameters.
        static readonly MIL_INT MAX_LINE_WIDTH_2 = 4;
        static readonly MIL_INT MIN_INTENSITY_2 = 100;

        // Everything below (less negative than) this is considered as noise.
        const double MIN_HEIGHT_THRESHOLD = -1.0; // in mm

        static void CalibratedCameraExample(MIL_ID MilSystem, MIL_ID MilDisplay)
        {
            MIL_ID MilOverlayImage = MIL.M_NULL; // Overlay image buffer identifier.
            MIL_ID MilImage = MIL.M_NULL;        // Image buffer identifier (for processing).
            MIL_ID MilCalibration = MIL.M_NULL;  // Calibration context.
            MIL_ID DepthMapId = MIL.M_NULL;      // Image buffer identifier (for results).
            MIL_ID LaserId = MIL.M_NULL;         // 3dmap laser profiling context identifier.
            MIL_ID ScanId = MIL.M_NULL;          // 3dmap result buffer identifier.
            MIL_INT CalibrationStatus = 0;       // Used to ensure if McalGrid() worked.
            MIL_INT SizeX = 0;                   // Width of grabbed images.
            MIL_INT SizeY = 0;                   // Height of grabbed images.
            MIL_INT NumberOfImages = 0;          // Number of frames for scanned objects.
            MIL_INT n = 0;                       // Counter.
            double FrameRate = 0.0;              // Number of grabbed frames per second (in AVI).
            double StartTime = 0.0;              // Time at the beginning of each iteration.
            double EndTime = 0.0;                // Time after processing for each iteration.
            double WaitTime = 0.0;               // Time to wait for next frame.
            double Volume = 0.0;                  // Volume of scanned object.

            Console.WriteLine();
            Console.WriteLine("3D PROFILING AND VOLUME ANALYSIS:");
            Console.WriteLine("---------------------------------");
            Console.WriteLine();
            Console.WriteLine("This program generates fully corrected 3d data of a scanned");
            Console.WriteLine("cookie and computes its volume.The laser profiling system uses");
            Console.WriteLine("a 3d-calibrated camera.");
            Console.WriteLine();

            // Load grid image for camera calibration.
            MIL.MbufRestore(GRID_FILENAME, MilSystem, ref MilImage);

            // Select display.
            MIL.MdispSelect(MilDisplay, MilImage);

            Console.WriteLine("Calibrating the camera...");
            Console.WriteLine();

            MIL.MbufInquire(MilImage, MIL.M_SIZE_X, ref SizeX);
            MIL.MbufInquire(MilImage, MIL.M_SIZE_Y, ref SizeY);

            // Allocate calibration context in 3D mode.
            MIL.McalAlloc(MilSystem, MIL.M_TSAI_BASED, MIL.M_DEFAULT, ref MilCalibration);

            // Calibrate the camera.
            MIL.McalGrid(MilCalibration, MilImage, 0.0, 0.0, 0.0, GRID_NB_ROWS, GRID_NB_COLS, GRID_ROW_SPACING, GRID_COL_SPACING, MIL.M_DEFAULT, MIL.M_CHESSBOARD_GRID);

            MIL.McalInquire(MilCalibration, MIL.M_CALIBRATION_STATUS + MIL.M_TYPE_MIL_INT, ref CalibrationStatus);
            if (CalibrationStatus != MIL.M_CALIBRATED)
            {
                MIL.McalFree(MilCalibration);
                MIL.MbufFree(MilImage);
                Console.WriteLine("Camera calibration failed.");
                Console.WriteLine("Press <Enter> to end.");
                Console.WriteLine();
                Console.ReadKey();
                return;
            }

            // Prepare for overlay annotations.
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY, MIL.M_ENABLE);
            MIL.MdispInquire(MilDisplay, MIL.M_OVERLAY_ID, ref MilOverlayImage);
            MIL.MgraColor(MIL.M_DEFAULT, MIL.M_COLOR_GREEN);

            // Draw camera calibration points.
            MIL.McalDraw(MIL.M_DEFAULT, MilCalibration, MilOverlayImage, MIL.M_DRAW_IMAGE_POINTS, MIL.M_DEFAULT, MIL.M_DEFAULT);

            Console.WriteLine("The camera was calibrated using a chessboard grid.");
            Console.WriteLine();
            Console.WriteLine("Press <Enter> to continue.");
            Console.WriteLine();
            Console.ReadKey();

            // Disable overlay.
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY, MIL.M_DISABLE);

            // Load laser line image.
            MIL.MbufLoad(LASERLINE_FILENAME, MilImage);

            // Allocate 3dmap objects.
            MIL.M3dmapAlloc(MilSystem, MIL.M_LASER, MIL.M_CALIBRATED_CAMERA_LINEAR_MOTION, ref LaserId);
            MIL.M3dmapAllocResult(MilSystem, MIL.M_LASER_DATA, MIL.M_DEFAULT, ref ScanId);

            // Set laser line extraction options.
            MIL.M3dmapControl(LaserId, MIL.M_DEFAULT, MIL.M_PEAK_WIDTH, MAX_LINE_WIDTH_2);
            MIL.M3dmapControl(LaserId, MIL.M_DEFAULT, MIL.M_MIN_INTENSITY, MIN_INTENSITY_2);

            // Calibrate laser profiling context.
            MIL.M3dmapAddScan(LaserId, ScanId, MilImage, MIL.M_NULL, MIL.M_NULL, MIL.M_DEFAULT, MIL.M_DEFAULT);
            MIL.M3dmapCalibrate(LaserId, ScanId, MilCalibration, MIL.M_DEFAULT);

            Console.WriteLine("The laser profiling system has been calibrated using the image");
            Console.WriteLine("of one laser line.");
            Console.WriteLine();
            Console.WriteLine("Press <Enter> to continue.");
            Console.WriteLine();
            Console.ReadKey();

            // Empty all result buffer contents.
            // It will now be reused for extracting 3d points.
            MIL.M3dmapAddScan(MIL.M_NULL, ScanId, MIL.M_NULL, MIL.M_NULL, MIL.M_NULL, MIL.M_DEFAULT, MIL.M_RESET);

            // Set speed of scanned object (speed in mm/frame is constant).
            MIL.M3dmapControl(LaserId, MIL.M_DEFAULT, MIL.M_SCAN_SPEED, CONVEYOR_SPEED);

            // Inquire characteristics of the input sequence.
            MIL.MbufDiskInquire(OBJECT2_SEQUENCE_FILE, MIL.M_NUMBER_OF_IMAGES, ref NumberOfImages);
            MIL.MbufDiskInquire(OBJECT2_SEQUENCE_FILE, MIL.M_FRAME_RATE, ref FrameRate);

            // Open the object sequence file for reading.
            MIL.MbufImportSequence(OBJECT2_SEQUENCE_FILE, MIL.M_DEFAULT, MIL.M_NULL, MIL.M_NULL, MIL.M_NULL, MIL.M_NULL, MIL.M_NULL, MIL.M_OPEN);

            Console.WriteLine("The cookie is being scanned to generate 3d data.");
            Console.WriteLine();

            // Read and process all images in the input sequence.
            MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ + MIL.M_SYNCHRONOUS, ref StartTime);

            for (n = 0; n < NumberOfImages; n++)
            {
                // Read image from sequence.
                MIL.MbufImportSequence(OBJECT2_SEQUENCE_FILE, MIL.M_DEFAULT, MIL.M_LOAD, MIL.M_NULL, ref MilImage, MIL.M_DEFAULT, 1, MIL.M_READ);

                // Analyze the image to extract laser line and correct its depth.
                MIL.M3dmapAddScan(LaserId, ScanId, MilImage, MIL.M_NULL, MIL.M_NULL, MIL.M_DEFAULT, MIL.M_DEFAULT);

                // Wait to have a proper frame rate, if necessary.
                MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ + MIL.M_SYNCHRONOUS, ref EndTime);
                WaitTime = (1.0 / FrameRate) - (EndTime - StartTime);
                if (WaitTime > 0)
                {
                    MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_WAIT, ref WaitTime);
                }
                MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ + MIL.M_SYNCHRONOUS, ref StartTime);
            }

            // Close the object sequence file.
            MIL.MbufImportSequence(OBJECT2_SEQUENCE_FILE, MIL.M_DEFAULT, MIL.M_NULL, MIL.M_NULL, MIL.M_NULL, MIL.M_NULL, MIL.M_NULL, MIL.M_CLOSE);

            // Allocate image for the fully corrected depth map.
            MIL.MbufAlloc2d(MilSystem, DEPTH_MAP_SIZE_X, DEPTH_MAP_SIZE_Y, 16 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_PROC + MIL.M_DISP, ref DepthMapId);

            // Set fully corrected depth map generation parameters.
            MIL.M3dmapControl(ScanId, MIL.M_DEFAULT, MIL.M_FILL_MODE, MIL.M_X_THEN_Y);
            MIL.M3dmapControl(ScanId, MIL.M_DEFAULT, MIL.M_FILL_SHARP_ELEVATION, MIL.M_MIN);
            MIL.M3dmapControl(ScanId, MIL.M_DEFAULT, MIL.M_FILL_SHARP_ELEVATION_DEPTH, GAP_DEPTH);
            MIL.M3dmapControl(ScanId, MIL.M_DEFAULT, MIL.M_PIXEL_SIZE_X, SCALE_X);
            MIL.M3dmapControl(ScanId, MIL.M_DEFAULT, MIL.M_PIXEL_SIZE_Y, SCALE_Y);
            MIL.M3dmapControl(ScanId, MIL.M_DEFAULT, MIL.M_GRAY_LEVEL_SIZE_Z, SCALE_Z);
            MIL.M3dmapControl(ScanId, MIL.M_DEFAULT, MIL.M_WORLD_POS_X, WORLD_OFFSET_X);
            MIL.M3dmapControl(ScanId, MIL.M_DEFAULT, MIL.M_WORLD_POS_Y, WORLD_OFFSET_Y);
            MIL.M3dmapControl(ScanId, MIL.M_DEFAULT, MIL.M_WORLD_POS_Z, WORLD_OFFSET_Z);

            // Get fully corrected depth map from accumulated information in the result buffer.
            MIL.M3dmapExtract(ScanId, DepthMapId, MIL.M_NULL, MIL.M_CORRECTED_DEPTH_MAP, MIL.M_DEFAULT, MIL.M_DEFAULT);

            // Remove noise.
            // Set all small values to 0, so that all remaining non-zeros are part of the cookie.
            MIL.MimClip(DepthMapId, DepthMapId, MIL.M_LESS, (MIN_HEIGHT_THRESHOLD - WORLD_OFFSET_Z) / SCALE_Z, MIL.M_NULL, 0.0, MIL.M_NULL);

            // Compute the volume of the cookie.
            MIL.M3dmapStat(DepthMapId, MIL.M_NULL, MIL.M_NULL, MIL.M_NULL, MIL.M_VOLUME, MIL.M_DEFAULT, MIL.M_DEFAULT, ref Volume);

            Console.WriteLine("Fully corrected 3d data of the cookie is displayed.");
            Console.WriteLine();

            // Try to allocate D3D display.
            IntPtr DispHandle;
            DispHandle = MdepthD3DAlloc(DepthMapId, MIL.M_NULL, D3D_DISPLAY_SIZE_X, D3D_DISPLAY_SIZE_Y, SCALE_X, SCALE_Y, -SCALE_Z, MIL.M_DEFAULT, MIL.M_DEFAULT, MIL.M_DEFAULT, IntPtr.Zero);

            if (DispHandle != IntPtr.Zero)
            {
                // Hide Mil Display.
                MIL.MdispControl(MilDisplay, MIL.M_WINDOW_SHOW, MIL.M_DISABLE);
                MdispD3DShow(DispHandle);
                Console.WriteLine("D3D display controls:");
                Console.WriteLine(" .Left click\tmove object");
                Console.WriteLine(" .Right click\trotate object");
                Console.WriteLine(" .Scroll wheel\tzoom");
                Console.WriteLine(" .R\t\tstart/stop animation");
                Console.WriteLine(" .P\t\tenable/disable point cloud");
                Console.WriteLine();
            }
            else
            {
                MIL.MdispControl(MilDisplay, MIL.M_VIEW_MODE, MIL.M_AUTO_SCALE);
                MIL.MdispSelect(MilDisplay, DepthMapId);
            }

            Console.WriteLine("Volume of the cookie is {0,4:0.0} cm^3.", -Volume / 1000.0);
            Console.WriteLine();
            Console.WriteLine("Press <Enter> to end.");
            Console.WriteLine();
            Console.ReadKey();


            if (DispHandle != IntPtr.Zero)
            {
                MdispD3DHide(DispHandle);
                MdispD3DFree(DispHandle);
            }

            // Free all allocations.
            MIL.M3dmapFree(ScanId);
            MIL.M3dmapFree(LaserId);
            MIL.McalFree(MilCalibration);
            MIL.MbufFree(DepthMapId);
            MIL.MbufFree(MilImage);
        }
    }
}

