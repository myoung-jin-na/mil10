'***************************************************************************
' 
' File name: M3dmap.vb
'
' Synopsis: This program inspects a wood surface using laser profiling to find
'           any depth defects.
'            
'***************************************************************************
Imports Microsoft.VisualBasic
Imports System
Imports System.Collections.Generic
Imports System.Text
Imports System.Runtime.InteropServices
Imports Matrox.MatroxImagingLibrary

Namespace M3dmap
    Friend Class Program
        ' Function declarations for DirectX display
        <DllImport("mdispd3d.dll", CallingConvention:=CallingConvention.Cdecl)> _
        Public Shared Function MdepthD3DAlloc(ByVal DepthBuffer As MIL_ID, ByVal IntensityBuffer As MIL_ID, ByVal DisplaySizeX As MIL_INT, ByVal DisplaySizeY As MIL_INT, ByVal ScaleX As Double, ByVal ScaleY As Double, ByVal ScaleZ As Double, ByVal MinZ As Double, ByVal MaxZ As Double, ByVal MaxDistanceZ As Double, ByVal WindowHandle As IntPtr) As IntPtr
        End Function
        <DllImport("mdispd3d.dll", CallingConvention:=CallingConvention.Cdecl)> _
        Public Shared Sub MdispD3DFree(ByVal DispHandle As IntPtr)
        End Sub
        <DllImport("mdispd3d.dll", CallingConvention:=CallingConvention.Cdecl)> _
        Public Shared Sub MdispD3DShow(ByVal DispHandle As IntPtr)
        End Sub
        <DllImport("mdispd3d.dll", CallingConvention:=CallingConvention.Cdecl)> _
        Public Shared Sub MdispD3DHide(ByVal DispHandle As IntPtr)
        End Sub
        <DllImport("mdispd3d.dll", CallingConvention:=CallingConvention.Cdecl)> _
        Public Shared Sub MdispD3DPrintHelp(ByVal DispHandle As IntPtr)
        End Sub

        Shared Sub Main(ByVal args() As String)
            '****************************************************************************
            ' Main.
            '****************************************************************************
            Dim MilApplication As MIL_ID = MIL.M_NULL ' Application identifier.
            Dim MilSystem As MIL_ID = MIL.M_NULL ' System identifier.
            Dim MilDisplay As MIL_ID = MIL.M_NULL ' Display identifier.

            ' Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, MilApplication, MilSystem, MilDisplay, CType(MIL.M_NULL, IntPtr), CType(MIL.M_NULL, IntPtr))

            ' Run the depth correction example.
            DepthCorrectionExample(MilSystem, MilDisplay)

            ' Run the calibrated camera example.
            CalibratedCameraExample(MilSystem, MilDisplay)

            ' Free defaults.
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL)
        End Sub

        '***************************************************************************
        ' Depth correction example.
        '***************************************************************************

        ' Input sequence specifications.
        Private Const REFERENCE_PLANES_SEQUENCE_FILE As String = MIL.M_IMAGE_PATH & "ReferencePlanes.avi"
        Private Const OBJECT_SEQUENCE_FILE As String = MIL.M_IMAGE_PATH & "ScannedObject.avi"

        ' Peak detection parameters.
        Private Const MAX_LINE_WIDTH As Integer = 8
        Private Const MIN_INTENSITY As Integer = 120

        ' Calibration heights in mm.
        Private Shared ReadOnly CORRECTED_DEPTHS() As Double = {1.25, 2.5, 3.75, 5.0}

        Private Const SCALE_FACTOR As Double = 10000.0 ' (depth in world units) * SCALE_FACTOR gives gray levels

        ' Annotation position.
        Private Const CALIB_TEXT_POS_X As Double = 400
        Private Const CALIB_TEXT_POS_Y As Double = 15

        Private Shared Sub DepthCorrectionExample(ByVal MilSystem As MIL_ID, ByVal MilDisplay As MIL_ID)
            Dim MilOverlayImage As MIL_ID = MIL.M_NULL ' Overlay image buffer identifier.
            Dim MilImage As MIL_ID = MIL.M_NULL ' Image buffer identifier (for processing).
            Dim MilDepthMap As MIL_ID = MIL.M_NULL ' Image buffer identifier (for results).
            Dim MilLaser As MIL_ID = MIL.M_NULL ' 3dmap laser profiling context identifier.
            Dim MilScan As MIL_ID = MIL.M_NULL ' 3dmap result buffer identifier.
            Dim SizeX As MIL_INT = 0 ' Width of grabbed images.
            Dim SizeY As MIL_INT = 0 ' Height of grabbed images.
            Dim NbReferencePlanes As MIL_INT = 0 ' Number of reference planes of known heights.
            Dim NbObjectImages As MIL_INT = 0 ' Number of frames for scanned objects.
            Dim n As MIL_INT = 0 ' Counter.
            Dim FrameRate As Double = 0.0 ' Number of grabbed frames per second (in AVI).
            Dim StartTime As Double = 0.0 ' Time at the beginning of each iteration.
            Dim EndTime As Double = 0.0 ' Time after processing for each iteration.
            Dim WaitTime As Double = 0.0 ' Time to wait for next frame.

            ' Inquire characteristics of the input sequences.
            MIL.MbufDiskInquire(REFERENCE_PLANES_SEQUENCE_FILE, MIL.M_SIZE_X, SizeX)
            MIL.MbufDiskInquire(REFERENCE_PLANES_SEQUENCE_FILE, MIL.M_SIZE_Y, SizeY)
            MIL.MbufDiskInquire(REFERENCE_PLANES_SEQUENCE_FILE, MIL.M_NUMBER_OF_IMAGES, NbReferencePlanes)
            MIL.MbufDiskInquire(REFERENCE_PLANES_SEQUENCE_FILE, MIL.M_FRAME_RATE, FrameRate)
            MIL.MbufDiskInquire(OBJECT_SEQUENCE_FILE, MIL.M_NUMBER_OF_IMAGES, NbObjectImages)

            ' Allocate buffer to hold images.
            MIL.MbufAlloc2d(MilSystem, SizeX, SizeY, 8 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_DISP + MIL.M_PROC, MilImage)
            MIL.MbufClear(MilImage, 0.0)

            Console.WriteLine()
            Console.WriteLine("DEPTH ANALYSIS:")
            Console.WriteLine("---------------")
            Console.WriteLine()
            Console.WriteLine("This program performs a surface inspection to detect depth")
            Console.WriteLine("defects on a wood surface using a laser profiling system.")
            Console.WriteLine()
            Console.WriteLine("Press <Enter> to continue.")
            Console.WriteLine()
            Console.ReadKey()

            ' Select display.
            MIL.MdispSelect(MilDisplay, MilImage)

            ' Prepare for overlay annotations.
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY, MIL.M_ENABLE)
            MIL.MdispInquire(MilDisplay, MIL.M_OVERLAY_ID, MilOverlayImage)
            MIL.MgraControl(MIL.M_DEFAULT, MIL.M_BACKGROUND_MODE, MIL.M_TRANSPARENT)
            MIL.MgraColor(MIL.M_DEFAULT, MIL.M_COLOR_WHITE)

            ' Allocate 3dmap objects.
            MIL.M3dmapAlloc(MilSystem, MIL.M_LASER, MIL.M_DEPTH_CORRECTION, MilLaser)
            MIL.M3dmapAllocResult(MilSystem, MIL.M_LASER_DATA, MIL.M_DEFAULT, MilScan)

            ' Set laser line extraction options.
            MIL.M3dmapControl(MilLaser, MIL.M_DEFAULT, MIL.M_PEAK_WIDTH, MAX_LINE_WIDTH)
            MIL.M3dmapControl(MilLaser, MIL.M_DEFAULT, MIL.M_MIN_INTENSITY, MIN_INTENSITY)

            ' Open the calibration sequence file for reading.
            MIL.MbufImportSequence(REFERENCE_PLANES_SEQUENCE_FILE, MIL.M_DEFAULT, MIL.M_NULL, MIL.M_NULL, CType(MIL.M_NULL, IntPtr), MIL.M_NULL, MIL.M_NULL, MIL.M_OPEN)

            ' Read and process all images in the input sequence.
            MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ + MIL.M_SYNCHRONOUS, StartTime)

            For n = 0 To NbReferencePlanes - 1
                Dim CalibString As String

                ' Read image from sequence.
                MIL.MbufImportSequence(REFERENCE_PLANES_SEQUENCE_FILE, MIL.M_DEFAULT, MIL.M_LOAD, MIL.M_NULL, MilImage, MIL.M_DEFAULT, 1, MIL.M_READ)

                ' Annotate the image with the calibration height.
                MIL.MdispControl(MilDisplay, MIL.M_OVERLAY_CLEAR, MIL.M_DEFAULT)
                CalibString = String.Format("Reference plane {0}: {1:0.00} mm", n + 1, CORRECTED_DEPTHS(CType(n, Integer)))
                MIL.MgraText(MIL.M_DEFAULT, MilOverlayImage, CALIB_TEXT_POS_X, CALIB_TEXT_POS_Y, CalibString)

                ' Set desired corrected depth of next calibration plane.
                MIL.M3dmapControl(MilLaser, MIL.M_DEFAULT, MIL.M_CORRECTED_DEPTH, CORRECTED_DEPTHS(CType(n, Integer)) * SCALE_FACTOR)

                ' Analyze the image to extract laser line.
                MIL.M3dmapAddScan(MilLaser, MilScan, MilImage, MIL.M_NULL, MIL.M_NULL, MIL.M_DEFAULT, MIL.M_DEFAULT)

                ' Wait to have a proper frame rate, if necessary.
                MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ + MIL.M_SYNCHRONOUS, EndTime)
                WaitTime = (1.0 / FrameRate) - (EndTime - StartTime)
                If WaitTime > 0 Then
                    MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_WAIT, WaitTime)
                End If
                MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ + MIL.M_SYNCHRONOUS, StartTime)
            Next n

            ' Close the calibration sequence file.
            MIL.MbufImportSequence(REFERENCE_PLANES_SEQUENCE_FILE, MIL.M_DEFAULT, MIL.M_NULL, MIL.M_NULL, CType(MIL.M_NULL, IntPtr), MIL.M_NULL, MIL.M_NULL, MIL.M_CLOSE)

            ' Calibrate the laser profiling context using reference planes of known heights.
            MIL.M3dmapCalibrate(MilLaser, MilScan, MIL.M_NULL, MIL.M_DEFAULT)

            Console.WriteLine("The laser profiling system has been calibrated using 4 reference")
            Console.WriteLine("planes of known heights.")
            Console.WriteLine()
            Console.WriteLine("Press <Enter> to continue.")
            Console.WriteLine()
            Console.ReadKey()

            Console.WriteLine("The wood surface is being scanned.")
            Console.WriteLine()

            ' Empty all result buffer contents.
            ' It will now be reused for extracting corrected depths.
            MIL.M3dmapAddScan(MIL.M_NULL, MilScan, MIL.M_NULL, MIL.M_NULL, MIL.M_NULL, MIL.M_DEFAULT, MIL.M_RESET)

            ' Open the object sequence file for reading.
            MIL.MbufDiskInquire(OBJECT_SEQUENCE_FILE, MIL.M_FRAME_RATE, FrameRate)
            MIL.MbufImportSequence(OBJECT_SEQUENCE_FILE, MIL.M_DEFAULT, MIL.M_NULL, MIL.M_NULL, CType(MIL.M_NULL, IntPtr), MIL.M_NULL, MIL.M_NULL, MIL.M_OPEN)

            ' Read and process all images in the input sequence.
            MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ + MIL.M_SYNCHRONOUS, StartTime)
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY_CLEAR, MIL.M_DEFAULT)

            For n = 0 To NbObjectImages - 1
                ' Read image from sequence.
                MIL.MbufImportSequence(OBJECT_SEQUENCE_FILE, MIL.M_DEFAULT, MIL.M_LOAD, MIL.M_NULL, MilImage, MIL.M_DEFAULT, 1, MIL.M_READ)

                ' Analyze the image to extract laser line and correct its depth.
                MIL.M3dmapAddScan(MilLaser, MilScan, MilImage, MIL.M_NULL, MIL.M_NULL, MIL.M_DEFAULT, MIL.M_DEFAULT)

                ' Wait to have a proper frame rate, if necessary.
                MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ + MIL.M_SYNCHRONOUS, EndTime)
                WaitTime = (1.0 / FrameRate) - (EndTime - StartTime)
                If WaitTime > 0 Then
                    MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_WAIT, WaitTime)
                End If
                MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ + MIL.M_SYNCHRONOUS, StartTime)
            Next n

            ' Close the object sequence file.
            MIL.MbufImportSequence(OBJECT_SEQUENCE_FILE, MIL.M_DEFAULT, MIL.M_NULL, MIL.M_NULL, CType(MIL.M_NULL, IntPtr), MIL.M_NULL, MIL.M_NULL, MIL.M_CLOSE)

            ' Allocate the image for a partially corrected depth map.
            MIL.MbufAlloc2d(MilSystem, SizeX, NbObjectImages, 16 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_PROC + MIL.M_DISP, MilDepthMap)

            ' Get partially corrected depth map from accumulated information in the result buffer.
            MIL.M3dmapExtract(MilScan, MilDepthMap, MIL.M_NULL, MIL.M_CORRECTED_DEPTH_MAP, MIL.M_DEFAULT, MIL.M_DEFAULT)

            ' Show partially corrected depth map and find defects.
            SetupColorDisplay(MilSystem, MilDisplay, MIL.MbufInquire(MilDepthMap, MIL.M_SIZE_BIT, MIL.M_NULL))

            ' Display partially corrected depth map.
            MIL.MdispSelect(MilDisplay, MilDepthMap)
            MIL.MdispInquire(MilDisplay, MIL.M_OVERLAY_ID, MilOverlayImage)

            Console.WriteLine("The pseudo-color depth map of the surface is displayed.")
            Console.WriteLine()
            Console.WriteLine("Press <Enter> to continue.")
            Console.WriteLine()
            Console.ReadKey()

            PerformBlobAnalysis(MilSystem, MilDisplay, MilOverlayImage, MilDepthMap)

            Console.WriteLine("Press <Enter> to continue.")
            Console.WriteLine()
            Console.ReadKey()

            ' Disassociates display LUT and clear overlay.
            MIL.MdispLut(MilDisplay, MIL.M_DEFAULT)
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY_CLEAR, MIL.M_DEFAULT)

            ' Free all allocations.
            MIL.M3dmapFree(MilScan)
            MIL.M3dmapFree(MilLaser)
            MIL.MbufFree(MilDepthMap)
            MIL.MbufFree(MilImage)
        End Sub

        ' Values used for binarization.
        Private Const EXPECTED_HEIGHT As Double = 3.4  ' Inspected surface should be at this height (in mm)
        Private Const DEFECT_THRESHOLD As Double = 0.2 ' Max acceptable deviation from expected height (mm)
        Private Const SATURATED_DEFECT As Double = 1.0 ' Deviation at which defect will appear red (in mm)

        ' Radius of the smallest particles to keep.
        Private Const MIN_BLOB_RADIUS As Integer = 3

        ' Pixel offset for drawing text.
        Private Const TEXT_H_OFFSET_1 As Double = -50
        Private Const TEXT_V_OFFSET_1 As Double = -6
        Private Const TEXT_H_OFFSET_2 As Double = -30
        Private Const TEXT_V_OFFSET_2 As Double = 6

        ' Find defects in corrected depth map, compute max deviation and draw contours.
        Private Shared Sub PerformBlobAnalysis(ByVal MilSystem As MIL_ID, ByVal MilDisplay As MIL_ID, ByVal MilOverlayImage As MIL_ID, ByVal MilDepthMap As MIL_ID)
            Dim MilBinImage As MIL_ID = MIL.M_NULL          ' Binary image buffer identifier.
            Dim MilBlobFeatureList As MIL_ID = MIL.M_NULL   ' Feature list identifier.
            Dim MilBlobResult As MIL_ID = MIL.M_NULL        ' Blob result buffer identifier.
            Dim SizeX As MIL_INT = 0                        ' Width of depth map.
            Dim SizeY As MIL_INT = 0                        ' Height of depth map.
            Dim TotalBlobs As MIL_INT = 0                   ' Total number of blobs.
            Dim n As MIL_INT = 0                            ' Counter.
            Dim MinPixels() As MIL_INT = Nothing            ' Maximum height of defects.
            Dim DefectThreshold As Double = 0.0             ' A gray level below it is a defect.
            Dim CogX() As Double = Nothing                  ' X coordinate of center of gravity.
            Dim CogY() As Double = Nothing                  ' Y coordinate of center of gravity.

            ' Get size of depth map.
            MIL.MbufInquire(MilDepthMap, MIL.M_SIZE_X, SizeX)
            MIL.MbufInquire(MilDepthMap, MIL.M_SIZE_Y, SizeY)

            ' Allocate a binary image buffer for fast processing.
            MIL.MbufAlloc2d(MilSystem, SizeX, SizeY, 1 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_PROC, MilBinImage)

            ' Binarize image.
            DefectThreshold = (EXPECTED_HEIGHT - DEFECT_THRESHOLD) * SCALE_FACTOR
            MIL.MimBinarize(MilDepthMap, MilBinImage, MIL.M_FIXED + MIL.M_LESS_OR_EQUAL, DefectThreshold, MIL.M_NULL)

            ' Remove small particles.
            MIL.MimOpen(MilBinImage, MilBinImage, MIN_BLOB_RADIUS, MIL.M_BINARY)

            ' Allocate a feature list.
            MIL.MblobAllocFeatureList(MilSystem, MilBlobFeatureList)

            ' Enable the Center Of Gravity and Min Pixel features calculation.
            MIL.MblobSelectFeature(MilBlobFeatureList, MIL.M_CENTER_OF_GRAVITY)
            MIL.MblobSelectFeature(MilBlobFeatureList, MIL.M_MIN_PIXEL)

            ' Allocate a blob result buffer.
            MIL.MblobAllocResult(MilSystem, MilBlobResult)

            ' Calculate selected features for each blob.
            MIL.MblobCalculate(MilBinImage, MilDepthMap, MilBlobFeatureList, MilBlobResult)

            ' Get the total number of selected blobs.
            MIL.MblobGetNumber(MilBlobResult, TotalBlobs)
            Console.WriteLine("Number of defects: {0}", TotalBlobs)

            ' Read and print the blob characteristics.
            CogX = New Double(CType(TotalBlobs, Integer) - 1) {}
            CogY = New Double(CType(TotalBlobs, Integer) - 1) {}
            MinPixels = New MIL_INT(CType(TotalBlobs, Integer) - 1) {}
            If CogX IsNot Nothing AndAlso CogY IsNot Nothing AndAlso MinPixels IsNot Nothing Then
                ' Get the results.
                MIL.MblobGetResult(MilBlobResult, MIL.M_CENTER_OF_GRAVITY_X, CogX)
                MIL.MblobGetResult(MilBlobResult, MIL.M_CENTER_OF_GRAVITY_Y, CogY)
                MIL.MblobGetResult(MilBlobResult, MIL.M_MIN_PIXEL + MIL.M_TYPE_MIL_INT, MinPixels)

                ' Draw the defects.
                MIL.MgraColor(MIL.M_DEFAULT, MIL.M_COLOR_RED)
                MIL.MblobDraw(MIL.M_DEFAULT, MilBlobResult, MilOverlayImage, MIL.M_DRAW_BLOBS, MIL.M_INCLUDED_BLOBS, MIL.M_DEFAULT)
                MIL.MgraColor(MIL.M_DEFAULT, MIL.M_COLOR_WHITE)

                ' Print the depth of each blob.
                For n = 0 To TotalBlobs - 1
                    Dim DepthOfDefect As Double
                    Dim DepthString As String

                    ' Write the depth of the defect in the overlay.
                    DepthOfDefect = EXPECTED_HEIGHT - (CType(MinPixels(CType(n, Integer)), Double) / SCALE_FACTOR)
                    DepthString = String.Format("{0:0.00} mm", DepthOfDefect)

                    Console.WriteLine("Defect #{0}: depth ={1,5:0.00} mm", n, DepthOfDefect)
                    Console.WriteLine()
                    MIL.MgraText(MIL.M_DEFAULT, MilOverlayImage, CogX(CType(n, Integer)) + TEXT_H_OFFSET_1, CogY(CType(n, Integer)) + TEXT_V_OFFSET_1, "Defect depth")
                    MIL.MgraText(MIL.M_DEFAULT, MilOverlayImage, CogX(CType(n, Integer)) + TEXT_H_OFFSET_2, CogY(CType(n, Integer)) + TEXT_V_OFFSET_2, DepthString)
                Next n
            Else
                Console.WriteLine("Error: Not enough memory.")
            End If

            ' Free all allocations.
            MIL.MblobFree(MilBlobResult)
            MIL.MblobFree(MilBlobFeatureList)
            MIL.MbufFree(MilBinImage)
        End Sub

        ' Color constants for display LUT.
        Private Const BLUE_HUE As Double = 171.0 ' Expected depths will be blue.
        Private Const RED_HUE As Double = 0.0 ' Worst defects will be red.
        Private Const FULL_SATURATION As Integer = 255 ' All colors are fully saturated.
        Private Const HALF_LUMINANCE As Integer = 128 ' All colors have half luminance.

        ' Creates a color display LUT to show defects in red.
        Private Shared Sub SetupColorDisplay(ByVal MilSystem As MIL_ID, ByVal MilDisplay As MIL_ID, ByVal SizeBit As MIL_INT)
            Dim MilRampLut1Band As MIL_ID = MIL.M_NULL ' LUT containing hue values.
            Dim MilRampLut3Band As MIL_ID = MIL.M_NULL ' RGB LUT used by display.
            Dim MilColorImage As MIL_ID = MIL.M_NULL ' Image used for HSL to RGB conversion.
            Dim DefectGrayLevel As MIL_INT = 0 ' Gray level under which all is red.
            Dim ExpectedGrayLevel As MIL_INT = 0 ' Gray level over which all is blue.
            Dim NbGrayLevels As MIL_INT

            ' Number of possible gray levels in corrected depth map.
            NbGrayLevels = CType(1 << CType(SizeBit, Integer), MIL_INT)

            ' Allocate 1-band LUT that will contain hue values.
            MIL.MbufAlloc1d(MilSystem, NbGrayLevels, 8 + MIL.M_UNSIGNED, MIL.M_LUT, MilRampLut1Band)

            ' Compute limit gray values.
            DefectGrayLevel = CType((EXPECTED_HEIGHT - SATURATED_DEFECT) * SCALE_FACTOR, MIL_INT)
            ExpectedGrayLevel = CType(EXPECTED_HEIGHT * SCALE_FACTOR, MIL_INT)

            ' Create hue values for each possible gray level.
            MIL.MgenLutRamp(MilRampLut1Band, 0, RED_HUE, DefectGrayLevel, RED_HUE)
            MIL.MgenLutRamp(MilRampLut1Band, DefectGrayLevel, RED_HUE, ExpectedGrayLevel, BLUE_HUE)
            MIL.MgenLutRamp(MilRampLut1Band, ExpectedGrayLevel, BLUE_HUE, NbGrayLevels - 1, BLUE_HUE)

            ' Create a HSL image buffer.
            MIL.MbufAllocColor(MilSystem, 3, NbGrayLevels, 1, 8 + MIL.M_UNSIGNED, MIL.M_IMAGE, MilColorImage)
            MIL.MbufClear(MilColorImage, MIL.M_RGB888(0, FULL_SATURATION, HALF_LUMINANCE))

            ' Set its H band (hue) to the LUT contents and convert the image to RGB.
            MIL.MbufCopyColor2d(MilRampLut1Band, MilColorImage, 0, 0, 0, 0, 0, 0, NbGrayLevels, 1)
            MIL.MimConvert(MilColorImage, MilColorImage, MIL.M_HSL_TO_RGB)

            ' Create RGB LUT to give to display and copy image contents.
            MIL.MbufAllocColor(MilSystem, 3, NbGrayLevels, 1, 8 + MIL.M_UNSIGNED, MIL.M_LUT, MilRampLut3Band)
            MIL.MbufCopy(MilColorImage, MilRampLut3Band)

            ' Associates LUT to display.
            MIL.MdispLut(MilDisplay, MilRampLut3Band)

            ' Free all allocations.
            MIL.MbufFree(MilRampLut1Band)
            MIL.MbufFree(MilRampLut3Band)
            MIL.MbufFree(MilColorImage)
        End Sub

        '***************************************************************************
        ' Calibrated camera example.
        '***************************************************************************

        ' Input sequence specifications.
        Private Const GRID_FILENAME As String = MIL.M_IMAGE_PATH & "GridForLaser.mim"
        Private Const LASERLINE_FILENAME As String = MIL.M_IMAGE_PATH & "LaserLine.mim"
        Private Const OBJECT2_SEQUENCE_FILE As String = MIL.M_IMAGE_PATH & "Cookie.avi"

        ' Camera calibration grid parameters.
        Private Shared ReadOnly GRID_NB_ROWS As MIL_INT = 13
        Private Shared ReadOnly GRID_NB_COLS As MIL_INT = 12
        Private Const GRID_ROW_SPACING As Double = 5.0 ' in mm
        Private Const GRID_COL_SPACING As Double = 5.0 ' in mm

        ' Laser device setup parameters.
        Private Const CONVEYOR_SPEED As Double = -0.2 ' in mm/frame

        ' Fully corrected depth map generation parameters.
        Private Shared ReadOnly DEPTH_MAP_SIZE_X As MIL_INT = 480 ' in pixels
        Private Shared ReadOnly DEPTH_MAP_SIZE_Y As MIL_INT = 480 ' in pixels
        Private Const GAP_DEPTH As Double = 1.5 ' in mm
        Private Const SCALE_X As Double = 0.15 ' in mm/pixel
        Private Const SCALE_Y As Double = 0.15 ' in mm/pixel
        Private Const SCALE_Z As Double = -0.0003 ' in mm/gray level
        Private Const WORLD_OFFSET_X As Double = -12.0 ' in mm
        Private Const WORLD_OFFSET_Y As Double = 4.0 ' in mm
        Private Const WORLD_OFFSET_Z As Double = 0.0 ' in mm

        ' D3D display parameters 
        Private Shared ReadOnly D3D_DISPLAY_SIZE_X As MIL_INT = 640
        Private Shared ReadOnly D3D_DISPLAY_SIZE_Y As MIL_INT = 480

        ' Peak detection parameters.
        Private Shared ReadOnly MAX_LINE_WIDTH_2 As MIL_INT = 4
        Private Shared ReadOnly MIN_INTENSITY_2 As MIL_INT = 100

        ' Everything below (less negative than) this is considered as noise.
        Private Const MIN_HEIGHT_THRESHOLD As Double = -1.0 ' in mm

        Private Shared Sub CalibratedCameraExample(ByVal MilSystem As MIL_ID, ByVal MilDisplay As MIL_ID)
            Dim MilOverlayImage As MIL_ID = MIL.M_NULL ' Overlay image buffer identifier.
            Dim MilImage As MIL_ID = MIL.M_NULL ' Image buffer identifier (for processing).
            Dim MilCalibration As MIL_ID = MIL.M_NULL ' Calibration context.
            Dim DepthMapId As MIL_ID = MIL.M_NULL ' Image buffer identifier (for results).
            Dim LaserId As MIL_ID = MIL.M_NULL ' 3dmap laser profiling context identifier.
            Dim ScanId As MIL_ID = MIL.M_NULL ' 3dmap result buffer identifier.
            Dim CalibrationStatus As MIL_INT = 0 ' Used to ensure if McalGrid() worked.
            Dim SizeX As MIL_INT = 0 ' Width of grabbed images.
            Dim SizeY As MIL_INT = 0 ' Height of grabbed images.
            Dim NumberOfImages As MIL_INT = 0 ' Number of frames for scanned objects.
            Dim n As MIL_INT = 0 ' Counter.
            Dim FrameRate As Double = 0.0 ' Number of grabbed frames per second (in AVI).
            Dim StartTime As Double = 0.0 ' Time at the beginning of each iteration.
            Dim EndTime As Double = 0.0 ' Time after processing for each iteration.
            Dim WaitTime As Double = 0.0 ' Time to wait for next frame.
            Dim Volume As Double = 0.0 ' Volume of scanned object.

            Console.WriteLine()
            Console.WriteLine("3D PROFILING AND VOLUME ANALYSIS:")
            Console.WriteLine("---------------------------------")
            Console.WriteLine()
            Console.WriteLine("This program generates fully corrected 3d data of a scanned")
            Console.WriteLine("cookie and computes its volume.The laser profiling system uses")
            Console.WriteLine("a 3d-calibrated camera.")
            Console.WriteLine()

            ' Load grid image for camera calibration.
            MIL.MbufRestore(GRID_FILENAME, MilSystem, MilImage)

            ' Select display.
            MIL.MdispSelect(MilDisplay, MilImage)

            Console.WriteLine("Calibrating the camera...")
            Console.WriteLine()

            MIL.MbufInquire(MilImage, MIL.M_SIZE_X, SizeX)
            MIL.MbufInquire(MilImage, MIL.M_SIZE_Y, SizeY)

            ' Allocate calibration context in 3D mode.
            MIL.McalAlloc(MilSystem, MIL.M_TSAI_BASED, MIL.M_DEFAULT, MilCalibration)

            ' Calibrate the camera.
            MIL.McalGrid(MilCalibration, MilImage, 0.0, 0.0, 0.0, GRID_NB_ROWS, GRID_NB_COLS, GRID_ROW_SPACING, GRID_COL_SPACING, MIL.M_DEFAULT, MIL.M_CHESSBOARD_GRID)

            MIL.McalInquire(MilCalibration, MIL.M_CALIBRATION_STATUS + MIL.M_TYPE_MIL_INT, CalibrationStatus)
            If CalibrationStatus <> MIL.M_CALIBRATED Then
                MIL.McalFree(MilCalibration)
                MIL.MbufFree(MilImage)
                Console.WriteLine("Camera calibration failed.")
                Console.WriteLine("Press <Enter> to end.")
                Console.WriteLine()
                Console.ReadLine()
                Return
            End If

            ' Prepare for overlay annotations.
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY, MIL.M_ENABLE)
            MIL.MdispInquire(MilDisplay, MIL.M_OVERLAY_ID, MilOverlayImage)
            MIL.MgraColor(MIL.M_DEFAULT, MIL.M_COLOR_GREEN)

            ' Draw camera calibration points.
            MIL.McalDraw(MIL.M_DEFAULT, MilCalibration, MilOverlayImage, MIL.M_DRAW_IMAGE_POINTS, MIL.M_DEFAULT, MIL.M_DEFAULT)

            Console.WriteLine("The camera was calibrated using a chessboard grid.")
            Console.WriteLine()
            Console.WriteLine("Press <Enter> to continue.")
            Console.WriteLine()
            Console.ReadKey()

            ' Disable overlay.
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY, MIL.M_DISABLE)

            ' Load laser line image.
            MIL.MbufLoad(LASERLINE_FILENAME, MilImage)

            ' Allocate 3dmap objects.
            MIL.M3dmapAlloc(MilSystem, MIL.M_LASER, MIL.M_CALIBRATED_CAMERA_LINEAR_MOTION, LaserId)
            MIL.M3dmapAllocResult(MilSystem, MIL.M_LASER_DATA, MIL.M_DEFAULT, ScanId)

            ' Set laser line extraction options.
            MIL.M3dmapControl(LaserId, MIL.M_DEFAULT, MIL.M_PEAK_WIDTH, MAX_LINE_WIDTH_2)
            MIL.M3dmapControl(LaserId, MIL.M_DEFAULT, MIL.M_MIN_INTENSITY, MIN_INTENSITY_2)

            ' Calibrate laser profiling context.
            MIL.M3dmapAddScan(LaserId, ScanId, MilImage, MIL.M_NULL, MIL.M_NULL, MIL.M_DEFAULT, MIL.M_DEFAULT)
            MIL.M3dmapCalibrate(LaserId, ScanId, MilCalibration, MIL.M_DEFAULT)

            Console.WriteLine("The laser profiling system has been calibrated using the image")
            Console.WriteLine("of one laser line.")
            Console.WriteLine()
            Console.WriteLine("Press <Enter> to continue.")
            Console.WriteLine()
            Console.ReadKey()

            ' Empty all result buffer contents.
            ' It will now be reused for extracting 3d points.
            MIL.M3dmapAddScan(MIL.M_NULL, ScanId, MIL.M_NULL, MIL.M_NULL, MIL.M_NULL, MIL.M_DEFAULT, MIL.M_RESET)

            ' Set speed of scanned object (speed in mm/frame is constant).
            MIL.M3dmapControl(LaserId, MIL.M_DEFAULT, MIL.M_SCAN_SPEED, CONVEYOR_SPEED)

            ' Inquire characteristics of the input sequence.
            MIL.MbufDiskInquire(OBJECT2_SEQUENCE_FILE, MIL.M_NUMBER_OF_IMAGES, NumberOfImages)
            MIL.MbufDiskInquire(OBJECT2_SEQUENCE_FILE, MIL.M_FRAME_RATE, FrameRate)

            ' Open the object sequence file for reading.
            MIL.MbufImportSequence(OBJECT2_SEQUENCE_FILE, MIL.M_DEFAULT, MIL.M_NULL, MIL.M_NULL, CType(MIL.M_NULL, IntPtr), MIL.M_NULL, MIL.M_NULL, MIL.M_OPEN)

            Console.WriteLine("The cookie is being scanned to generate 3d data.")
            Console.WriteLine()

            ' Read and process all images in the input sequence.
            MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ + MIL.M_SYNCHRONOUS, StartTime)

            For n = 0 To NumberOfImages - 1
                ' Read image from sequence.
                MIL.MbufImportSequence(OBJECT2_SEQUENCE_FILE, MIL.M_DEFAULT, MIL.M_LOAD, MIL.M_NULL, MilImage, MIL.M_DEFAULT, 1, MIL.M_READ)

                ' Analyze the image to extract laser line and correct its depth.
                MIL.M3dmapAddScan(LaserId, ScanId, MilImage, MIL.M_NULL, MIL.M_NULL, MIL.M_DEFAULT, MIL.M_DEFAULT)

                ' Wait to have a proper frame rate, if necessary.
                MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ + MIL.M_SYNCHRONOUS, EndTime)
                WaitTime = (1.0 / FrameRate) - (EndTime - StartTime)
                If WaitTime > 0 Then
                    MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_WAIT, WaitTime)
                End If
                MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ + MIL.M_SYNCHRONOUS, StartTime)
            Next n

            ' Close the object sequence file.
            MIL.MbufImportSequence(OBJECT2_SEQUENCE_FILE, MIL.M_DEFAULT, MIL.M_NULL, MIL.M_NULL, CType(MIL.M_NULL, IntPtr), MIL.M_NULL, MIL.M_NULL, MIL.M_CLOSE)

            ' Allocate image for the fully corrected depth map.
            MIL.MbufAlloc2d(MilSystem, DEPTH_MAP_SIZE_X, DEPTH_MAP_SIZE_Y, 16 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_PROC + MIL.M_DISP, DepthMapId)

            ' Set fully corrected depth map generation parameters.
            MIL.M3dmapControl(ScanId, MIL.M_DEFAULT, MIL.M_FILL_MODE, MIL.M_X_THEN_Y)
            MIL.M3dmapControl(ScanId, MIL.M_DEFAULT, MIL.M_FILL_SHARP_ELEVATION, MIL.M_MIN)
            MIL.M3dmapControl(ScanId, MIL.M_DEFAULT, MIL.M_FILL_SHARP_ELEVATION_DEPTH, GAP_DEPTH)
            MIL.M3dmapControl(ScanId, MIL.M_DEFAULT, MIL.M_PIXEL_SIZE_X, SCALE_X)
            MIL.M3dmapControl(ScanId, MIL.M_DEFAULT, MIL.M_PIXEL_SIZE_Y, SCALE_Y)
            MIL.M3dmapControl(ScanId, MIL.M_DEFAULT, MIL.M_GRAY_LEVEL_SIZE_Z, SCALE_Z)
            MIL.M3dmapControl(ScanId, MIL.M_DEFAULT, MIL.M_WORLD_POS_X, WORLD_OFFSET_X)
            MIL.M3dmapControl(ScanId, MIL.M_DEFAULT, MIL.M_WORLD_POS_Y, WORLD_OFFSET_Y)
            MIL.M3dmapControl(ScanId, MIL.M_DEFAULT, MIL.M_WORLD_POS_Z, WORLD_OFFSET_Z)

            ' Get fully corrected depth map from accumulated information in the result buffer.
            MIL.M3dmapExtract(ScanId, DepthMapId, MIL.M_NULL, MIL.M_CORRECTED_DEPTH_MAP, MIL.M_DEFAULT, MIL.M_DEFAULT)

            ' Remove noise.
            ' Set all small values to 0, so that all remaining non-zeros are part of the cookie.
            MIL.MimClip(DepthMapId, DepthMapId, MIL.M_LESS, (MIN_HEIGHT_THRESHOLD - WORLD_OFFSET_Z) / SCALE_Z, MIL.M_NULL, 0.0, MIL.M_NULL)

            ' Compute the volume of the cookie.
            MIL.M3dmapStat(DepthMapId, MIL.M_NULL, MIL.M_NULL, MIL.M_NULL, MIL.M_VOLUME, MIL.M_DEFAULT, MIL.M_DEFAULT, Volume)

            Console.WriteLine("fully corrected 3d data of the cookie is displayed.")
            Console.WriteLine()

            ' Try to allocate D3D display.
            Dim DispHandle As IntPtr
            DispHandle = MdepthD3DAlloc(DepthMapId, MIL.M_NULL, D3D_DISPLAY_SIZE_X, D3D_DISPLAY_SIZE_Y, SCALE_X, SCALE_Y, -SCALE_Z, MIL.M_DEFAULT, MIL.M_DEFAULT, MIL.M_DEFAULT, IntPtr.Zero)

            If DispHandle <> IntPtr.Zero Then
                ' Hide Mil Display.
                MIL.MdispControl(MilDisplay, MIL.M_WINDOW_SHOW, MIL.M_DISABLE)
                MdispD3DShow(DispHandle)
                Console.WriteLine("D3D display controls:")
                Console.WriteLine(" .Left click" & Constants.vbTab & "move object")
                Console.WriteLine(" .Right click" & Constants.vbTab & "rotate object")
                Console.WriteLine(" .Scroll wheel" & Constants.vbTab & "zoom")
                Console.WriteLine(" .R" & Constants.vbTab + Constants.vbTab & "start/stop animation")
                Console.WriteLine(" .P" & Constants.vbTab + Constants.vbTab & "enable/disable point cloud")
                Console.WriteLine()
            Else
                MIL.MdispControl(MilDisplay, MIL.M_VIEW_MODE, MIL.M_AUTO_SCALE)
                MIL.MdispSelect(MilDisplay, DepthMapId)
            End If

            Console.WriteLine("Volume of the cookie is {0,4:0.0} cm^3.", -Volume / 1000.0)
            Console.WriteLine()
            Console.WriteLine("Press <Enter> to end.")
            Console.WriteLine()
            Console.ReadKey()


            If DispHandle <> IntPtr.Zero Then
                MdispD3DHide(DispHandle)
                MdispD3DFree(DispHandle)
            End If

            ' Free all allocations.
            MIL.M3dmapFree(ScanId)
            MIL.M3dmapFree(LaserId)
            MIL.McalFree(MilCalibration)
            MIL.MbufFree(DepthMapId)
            MIL.MbufFree(MilImage)
        End Sub
    End Class
End Namespace

