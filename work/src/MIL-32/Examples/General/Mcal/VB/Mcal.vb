'*******************************************************************************
'
' File name: Mcal.vb
'
' Synopsis:  This program uses the Calibration module to calibrate a high
'            distortion camera with an image of a grid. Then, an image of
'            a board grabbed with the same camera is loaded and some
'            measurements are done in real world calibrated units. Finally
'            the distorted board image is corrected using the same camera
'            calibration.
'*******************************************************************************
Imports Microsoft.VisualBasic
Imports System
Imports System.Collections.Generic
Imports System.Text

Imports Matrox.MatroxImagingLibrary

Namespace Mcal
    Friend Class Program
        ' Example selection.
        Private Const RUN_LINEAR_CALIBRATION_EXAMPLE As Integer = MIL.M_YES
        Private Const RUN_TSAI_CALIBRATION_EXAMPLE As Integer = MIL.M_YES

        Shared Sub Main(ByVal args() As String)
            Dim MilApplication As MIL_ID = MIL.M_NULL ' Application identifier.
            Dim MilSystem As MIL_ID = MIL.M_NULL ' System Identifier.
            Dim MilDisplay As MIL_ID = MIL.M_NULL ' Display identifier.

            ' Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, MilApplication, MilSystem, MilDisplay, CType(MIL.M_NULL, IntPtr), CType(MIL.M_NULL, IntPtr))

            ' Print module name.
            Console.Write("CALIBRATION MODULE:" + Constants.vbLf)
            Console.Write("-------------------" + Constants.vbLf + Constants.vbLf)

            If RUN_LINEAR_CALIBRATION_EXAMPLE = MIL.M_YES Then
                LinearInterpolationCalibration(MilSystem, MilDisplay)
            End If

            If RUN_TSAI_CALIBRATION_EXAMPLE = MIL.M_YES Then
                TsaiCalibration(MilSystem, MilDisplay)
            End If

            ' Free defaults.
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL)
        End Sub

        '****************************************************************************
        ' Linear interpolation example. 
        '****************************************************************************

        ' Source image files specification.
        Private Const GRID_IMAGE_FILE As String = MIL.M_IMAGE_PATH & "CalGrid.mim"
        Private Const BOARD_IMAGE_FILE As String = MIL.M_IMAGE_PATH & "CalBoard.mim"

        ' World description of the calibration grid.
        Private Const GRID_OFFSET_X As Integer = 0
        Private Const GRID_OFFSET_Y As Integer = 0
        Private Const GRID_OFFSET_Z As Integer = 0
        Private Const GRID_ROW_SPACING As Integer = 1
        Private Const GRID_COLUMN_SPACING As Integer = 1
        Private Const GRID_ROW_NUMBER As Integer = 18
        Private Const GRID_COLUMN_NUMBER As Integer = 25

        ' Measurement boxes specification.
        Private Const MEAS_BOX_POS_X1 As Integer = 55
        Private Const MEAS_BOX_POS_Y1 As Integer = 24
        Private Const MEAS_BOX_WIDTH1 As Integer = 7
        Private Const MEAS_BOX_HEIGHT1 As Integer = 425

        Private Const MEAS_BOX_POS_X2 As Integer = 225
        Private Const MEAS_BOX_POS_Y2 As Integer = 11
        Private Const MEAS_BOX_WIDTH2 As Integer = 7
        Private Const MEAS_BOX_HEIGHT2 As Integer = 450

        ' Specification of the stripes' constraints.
        Private Const WIDTH_APPROXIMATION As Integer = 410
        Private Const WIDTH_VARIATION As Integer = 25
        Private Const MIN_EDGE_VALUE As Integer = 5

        Private Shared Sub LinearInterpolationCalibration(ByVal MilSystem As MIL_ID, ByVal MilDisplay As MIL_ID)
            Dim MilImage As MIL_ID = MIL.M_NULL ' Image buffer identifier.
            Dim MilOverlayImage As MIL_ID = MIL.M_NULL ' Overlay image.
            Dim MilCalibration As MIL_ID = MIL.M_NULL ' Calibration identifier.
            Dim MeasMarker1 As MIL_ID = MIL.M_NULL ' Measurement marker identifier.
            Dim MeasMarker2 As MIL_ID = MIL.M_NULL ' Measurement marker identifier.
            Dim WorldDistance1 As Double = 0.0
            Dim WorldDistance2 As Double = 0.0
            Dim PixelDistance1 As Double = 0.0
            Dim PixelDistance2 As Double = 0.0
            Dim PosX1 As Double = 0.0
            Dim PosY1 As Double = 0.0
            Dim PosX2 As Double = 0.0
            Dim PosY2 As Double = 0.0
            Dim PosX3 As Double = 0.0
            Dim PosY3 As Double = 0.0
            Dim PosX4 As Double = 0.0
            Dim PosY4 As Double = 0.0
            Dim CalibrationStatus As MIL_INT = 0

            ' Clear the display.
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY_CLEAR, MIL.M_DEFAULT)

            ' Restore source image into an automatically allocated image buffer.
            MIL.MbufRestore(GRID_IMAGE_FILE, MilSystem, MilImage)

            ' Display the image buffer.
            MIL.MdispSelect(MilDisplay, MilImage)

            ' Prepare for overlay annotation.
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY, MIL.M_ENABLE)
            MIL.MdispInquire(MilDisplay, MIL.M_OVERLAY_ID, MilOverlayImage)

            ' Pause to show the original image.
            Console.Write(Constants.vbLf + "LINEAR INTERPOLATION CALIBRATION:" + Constants.vbLf)
            Console.Write("------------" + Constants.vbLf + Constants.vbLf)
            Console.Write("The displayed grid has been grabbed with a high distortion" + Constants.vbLf)
            Console.Write("camera and will be used to calibrate the camera." + Constants.vbLf)
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Allocate a camera calibration context.
            MIL.McalAlloc(MilSystem, MIL.M_DEFAULT, MIL.M_DEFAULT, MilCalibration)

            ' Calibrate the camera with the image of the grid and its world description.
            MIL.McalGrid(MilCalibration, MilImage, GRID_OFFSET_X, GRID_OFFSET_Y, GRID_OFFSET_Z, GRID_ROW_NUMBER, GRID_COLUMN_NUMBER, GRID_ROW_SPACING, GRID_COLUMN_SPACING, MIL.M_DEFAULT, MIL.M_DEFAULT)

            MIL.McalInquire(MilCalibration, MIL.M_CALIBRATION_STATUS + MIL.M_TYPE_MIL_INT, CalibrationStatus)
            If CalibrationStatus = MIL.M_CALIBRATED Then
                ' Perform a first image transformation with the calibration grid.
                MIL.McalTransformImage(MilImage, MilImage, MilCalibration, MIL.M_BILINEAR Or MIL.M_OVERSCAN_CLEAR, MIL.M_DEFAULT, MIL.M_DEFAULT)

                ' Pause to show the corrected image of the grid.
                Console.Write("The camera has been calibrated and the image of the grid" + Constants.vbLf)
                Console.Write("has been transformed to remove its distortions." + Constants.vbLf)
                Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
                Console.ReadKey()

                ' Read the image of the board and associate the calibration to the image.
                MIL.MbufLoad(BOARD_IMAGE_FILE, MilImage)
                MIL.McalAssociate(MilCalibration, MilImage, MIL.M_DEFAULT)

                ' Allocate the measurement markers.
                MIL.MmeasAllocMarker(MilSystem, MIL.M_STRIPE, MIL.M_DEFAULT, MeasMarker1)
                MIL.MmeasAllocMarker(MilSystem, MIL.M_STRIPE, MIL.M_DEFAULT, MeasMarker2)

                ' Set the markers' measurement regions.
                MIL.MmeasSetMarker(MeasMarker1, MIL.M_BOX_ORIGIN, MEAS_BOX_POS_X1, MEAS_BOX_POS_Y1)
                MIL.MmeasSetMarker(MeasMarker1, MIL.M_BOX_SIZE, MEAS_BOX_WIDTH1, MEAS_BOX_HEIGHT1)
                MIL.MmeasSetMarker(MeasMarker2, MIL.M_BOX_ORIGIN, MEAS_BOX_POS_X2, MEAS_BOX_POS_Y2)
                MIL.MmeasSetMarker(MeasMarker2, MIL.M_BOX_SIZE, MEAS_BOX_WIDTH2, MEAS_BOX_HEIGHT2)

                ' Set markers' orientation.
                MIL.MmeasSetMarker(MeasMarker1, MIL.M_ORIENTATION, MIL.M_HORIZONTAL, MIL.M_NULL)
                MIL.MmeasSetMarker(MeasMarker2, MIL.M_ORIENTATION, MIL.M_HORIZONTAL, MIL.M_NULL)

                ' Set markers' settings to locate the largest stripe within the range
                ' [WIDTH_APPROXIMATION - WIDTH_VARIATION,
                '  WIDTH_APPROXIMATION + WIDTH_VARIATION],
                ' and with an edge strength over MIN_EDGE_VALUE. */
                MIL.MmeasSetMarker(MeasMarker1, MIL.M_EDGEVALUE_MIN, MIN_EDGE_VALUE, MIL.M_NULL)

                ' Remove the default strength characteristic score mapping.
                MIL.MmeasSetScore(MeasMarker1, MIL.M_STRENGTH_SCORE, _
                                               0.0, _
                                               0.0, _
                                               MIL.M_MAX_POSSIBLE_VALUE, _
                                               MIL.M_MAX_POSSIBLE_VALUE, _
                                               MIL.M_DEFAULT, _
                                               MIL.M_DEFAULT, _
                                               MIL.M_DEFAULT)

                ' Add a width characteristic score mapping (increasing ramp)
                ' to find the largest stripe within a given range.
                '
                ' Width score mapping to find the largest stripe within a given
                ' width range ]Wmin, Wmax]:
                '
                '    Score
                '       ^
                '       |         /|
                '       |       /  |
                '       |     /    |
                '       +---------------> Width
                '           Wmin  Wmax
                ' 
                MIL.MmeasSetScore(MeasMarker1, MIL.M_STRIPE_WIDTH_SCORE, _
                                               WIDTH_APPROXIMATION - WIDTH_VARIATION, _
                                               WIDTH_APPROXIMATION + WIDTH_VARIATION, _
                                               WIDTH_APPROXIMATION + WIDTH_VARIATION, _
                                               WIDTH_APPROXIMATION + WIDTH_VARIATION, _
                                               MIL.M_DEFAULT, _
                                               MIL.M_PIXEL, _
                                               MIL.M_DEFAULT)

                ' Set the same settings for the second marker.
                MIL.MmeasSetMarker(MeasMarker2, MIL.M_EDGEVALUE_MIN, MIN_EDGE_VALUE, MIL.M_NULL)

                MIL.MmeasSetScore(MeasMarker2, MIL.M_STRENGTH_SCORE, _
                                           0.0, _
                                           0.0, _
                                           MIL.M_MAX_POSSIBLE_VALUE, _
                                           MIL.M_MAX_POSSIBLE_VALUE, _
                                           MIL.M_DEFAULT, _
                                           MIL.M_DEFAULT, _
                                           MIL.M_DEFAULT)

                MIL.MmeasSetScore(MeasMarker2, MIL.M_STRIPE_WIDTH_SCORE, _
                                               WIDTH_APPROXIMATION - WIDTH_VARIATION, _
                                               WIDTH_APPROXIMATION + WIDTH_VARIATION, _
                                               WIDTH_APPROXIMATION + WIDTH_VARIATION, _
                                               WIDTH_APPROXIMATION + WIDTH_VARIATION, _
                                               MIL.M_DEFAULT, _
                                               MIL.M_PIXEL, _
                                               MIL.M_DEFAULT)

                ' Find and measure the position and width of the board.
                MIL.MmeasFindMarker(MIL.M_DEFAULT, MilImage, MeasMarker1, MIL.M_STRIPE_WIDTH + MIL.M_POSITION)
                MIL.MmeasFindMarker(MIL.M_DEFAULT, MilImage, MeasMarker2, MIL.M_STRIPE_WIDTH + MIL.M_POSITION)

                ' Get the world width of the two markers.
                MIL.MmeasGetResult(MeasMarker1, MIL.M_STRIPE_WIDTH, WorldDistance1)
                MIL.MmeasGetResult(MeasMarker2, MIL.M_STRIPE_WIDTH, WorldDistance2)

                ' Get the pixel width of the two markers.
                MIL.MmeasSetMarker(MeasMarker1, MIL.M_RESULT_OUTPUT_UNITS, MIL.M_PIXEL, MIL.M_NULL)
                MIL.MmeasSetMarker(MeasMarker2, MIL.M_RESULT_OUTPUT_UNITS, MIL.M_PIXEL, MIL.M_NULL)
                MIL.MmeasGetResult(MeasMarker1, MIL.M_STRIPE_WIDTH, PixelDistance1)
                MIL.MmeasGetResult(MeasMarker2, MIL.M_STRIPE_WIDTH, PixelDistance2)

                ' Get the edges position in pixel to draw the annotations.
                MIL.MmeasGetResult(MeasMarker1, MIL.M_POSITION + MIL.M_EDGE_FIRST, PosX1, PosY1)
                MIL.MmeasGetResult(MeasMarker1, MIL.M_POSITION + MIL.M_EDGE_SECOND, PosX2, PosY2)
                MIL.MmeasGetResult(MeasMarker2, MIL.M_POSITION + MIL.M_EDGE_FIRST, PosX3, PosY3)
                MIL.MmeasGetResult(MeasMarker2, MIL.M_POSITION + MIL.M_EDGE_SECOND, PosX4, PosY4)

                ' Draw the measurement indicators on the image. 
                MIL.MgraColor(MIL.M_DEFAULT, MIL.M_COLOR_YELLOW)
                MIL.MmeasDraw(MIL.M_DEFAULT, MeasMarker1, MilOverlayImage, MIL.M_DRAW_WIDTH, MIL.M_DEFAULT, MIL.M_RESULT)
                MIL.MmeasDraw(MIL.M_DEFAULT, MeasMarker2, MilOverlayImage, MIL.M_DRAW_WIDTH, MIL.M_DEFAULT, MIL.M_RESULT)

                MIL.MgraBackColor(MIL.M_DEFAULT, MIL.M_COLOR_BLACK)
                MIL.MgraText(MIL.M_DEFAULT, MilOverlayImage, CInt(Fix(PosX1 + 0.5 - 40)), CInt(Fix((PosY1 + 0.5) + ((PosY2 - PosY1) / 2.0))), " Distance 1 ")
                MIL.MgraText(MIL.M_DEFAULT, MilOverlayImage, CInt(Fix(PosX3 + 0.5 - 40)), CInt(Fix((PosY3 + 0.5) + ((PosY4 - PosY3) / 2.0))), " Distance 2 ")

                ' Pause to show the original image and the measurement results.
                Console.Write("A distorted image grabbed with the same camera was loaded and" + Constants.vbLf)
                Console.Write("calibrated measurements were done to evaluate the board dimensions." + Constants.vbLf)
                Console.Write(Constants.vbLf + "========================================================" + Constants.vbLf)
                Console.Write("                      Distance 1          Distance 2 " + Constants.vbLf)
                Console.Write("--------------------------------------------------------" + Constants.vbLf)
                Console.Write(" Calibrated unit:   {0,8:0.00} cm           {1,6:0.00} cm    " + Constants.vbLf, WorldDistance1, WorldDistance2)
                Console.Write(" Uncalibrated unit: {0,8:0.00} pixels       {1,6:0.00} pixels" + Constants.vbLf, PixelDistance1, PixelDistance2)
                Console.Write("========================================================" + Constants.vbLf + Constants.vbLf)
                Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
                Console.ReadKey()

                ' Clear the display overlay.
                MIL.MdispControl(MilDisplay, MIL.M_OVERLAY_CLEAR, MIL.M_DEFAULT)

                ' Read the image of the PCB.
                MIL.MbufLoad(BOARD_IMAGE_FILE, MilImage)

                ' Transform the image of the board.
                MIL.McalTransformImage(MilImage, MilImage, MilCalibration, MIL.M_BILINEAR + MIL.M_OVERSCAN_CLEAR, MIL.M_DEFAULT, MIL.M_DEFAULT)

                ' show the transformed image of the board.
                Console.Write("The image was corrected to remove its distortions." + Constants.vbLf)

                ' Free measurement markers.
                MIL.MmeasFree(MeasMarker1)
                MIL.MmeasFree(MeasMarker2)
            Else
                Console.Write("Calibration generated an exception." + Constants.vbLf)
                Console.Write("See User Guide to resolve the situation." + Constants.vbLf + Constants.vbLf)
            End If

            ' Wait for a key to be pressed.
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Free all allocations.
            MIL.McalFree(MilCalibration)
            MIL.MbufFree(MilImage)
        End Sub

        '****************************************************************************
        ' Tsai example. 
        '****************************************************************************
        ' Source image files specification.
        Private Const GRID_ORIGINAL_IMAGE_FILE As String = MIL.M_IMAGE_PATH & "CalGridOriginal.mim"
        Private Const OBJECT_ORIGINAL_IMAGE_FILE As String = MIL.M_IMAGE_PATH & "CalObjOriginal.mim"
        Private Const OBJECT_MOVED_IMAGE_FILE As String = MIL.M_IMAGE_PATH & "CalObjMoved.mim"

        ' World description of the calibration grid.
        Private Const GRID_ORG_ROW_SPACING As Double = 1.5
        Private Const GRID_ORG_COLUMN_SPACING As Double = 1.5
        Private Const GRID_ORG_ROW_NUMBER As Integer = 12
        Private Const GRID_ORG_COLUMN_NUMBER As Integer = 13
        Private Const GRID_ORG_OFFSET_X As Integer = 0
        Private Const GRID_ORG_OFFSET_Y As Integer = 0
        Private Const GRID_ORG_OFFSET_Z As Integer = 0

        ' Camera transformation
        Private Const ENCODER_TRANSLATION_X As Double = 0.817
        Private Const ENCODER_TRANSLATION_Y As Double = 13.293
        Private Const ENCODER_TRANSLATION_Z As Double = -14.06
        Private Const ENCODER_ROTATION_Y As Double = 354.207
        Private Const ENCODER_ROTATION_X As Double = 317.467
        Private Const ENCODER_ROTATION_Z As Double = 281.969

        ' Region parameters for metrology
        Private Const MEASURED_CIRCLE_LABEL As Integer = 1
        Private Const RING1_POS1_X As Double = 2.3
        Private Const RING1_POS1_Y As Double = 3.9
        Private Const RING2_POS1_X As Double = 10.7
        Private Const RING2_POS1_Y As Double = 11.1

        Private Const RING1_POS2_X As Double = 8.6
        Private Const RING1_POS2_Y As Double = 2.5
        Private Const RING2_POS2_X As Double = 7.9
        Private Const RING2_POS2_Y As Double = 13.2

        Private Const RING_START_RADIUS As Double = 1.25
        Private Const RING_END_RADIUS As Double = 2.3

        ' measured plane position
        Private Const RING_THICKNESS As Double = 0.175
        Private Const STEP_THICKNESS As Double = 4.0

        ' Color definitions
        Private Shared ReadOnly REGION_COLOR As Integer = MIL.M_RGB888(0, 100, 255)
        Private Shared ReadOnly FEATURE_COLOR As Integer = MIL.M_RGB888(255, 0, 255)

        Private Shared Sub TsaiCalibration(ByVal MilSystem As MIL_ID, ByVal MilDisplay As MIL_ID)
            Dim MilImage As MIL_ID = MIL.M_NULL ' Image buffer identifier.
            Dim MilCalibration As MIL_ID = MIL.M_NULL ' Calibration identifier.

            Dim CalibrationStatus As MIL_INT = 0

            ' Restore source image into an automatically allocated image buffer.
            MIL.MbufRestore(GRID_ORIGINAL_IMAGE_FILE, MilSystem, MilImage)

            ' Display the image buffer.
            MIL.MdispSelect(MilDisplay, MilImage)

            ' Pause to show the original image.
            Console.Write("TSAI BASED CALIBRATION:" + Constants.vbLf)
            Console.Write("------------" + Constants.vbLf + Constants.vbLf)
            Console.Write("The displayed grid has been grabbed with a high perspective" + Constants.vbLf)
            Console.Write("camera and will be used to calibrate the camera." + Constants.vbLf)
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Allocate a camera calibration context.
            MIL.McalAlloc(MilSystem, MIL.M_TSAI_BASED, MIL.M_DEFAULT, MilCalibration)

            ' Calibrate the camera with the image of the grid and its world description.
            MIL.McalGrid(MilCalibration, MilImage, GRID_ORG_OFFSET_X, GRID_ORG_OFFSET_Y, GRID_ORG_OFFSET_Z, GRID_ORG_ROW_NUMBER, GRID_ORG_COLUMN_NUMBER, GRID_ORG_ROW_SPACING, GRID_ORG_COLUMN_SPACING, MIL.M_DEFAULT, MIL.M_DEFAULT)

            ' Verify if the camera calibration was successful.
            MIL.McalInquire(MilCalibration, MIL.M_CALIBRATION_STATUS + MIL.M_TYPE_MIL_INT, CalibrationStatus)
            If CalibrationStatus = MIL.M_CALIBRATED Then
                ' Print camera information
                Console.Write("The camera has been calibrated." + Constants.vbLf + Constants.vbLf)
                ShowCameraInformation(MilCalibration)

                ' Load source image into an image buffer.
                MIL.MbufLoad(OBJECT_ORIGINAL_IMAGE_FILE, MilImage)

                ' Set offset to the camera calibration plane and associate calibration to image.
                ' This moves the relative origin to the top of the first measured circle 
                SetCalibrationOffset(MilCalibration, -RING_THICKNESS, MilImage)

                ' Measure the first circle.
                Console.Write("First measured circle at  z = {0:0.00} cm. ", -RING_THICKNESS)
                MeasureRing(MilSystem, MilDisplay, MilImage, RING1_POS1_X, RING1_POS1_Y)

                ' Set offset to the camera calibration plane and associate calibration to image.
                ' This moves the relative origin to the top of the second measured circle 
                SetCalibrationOffset(MilCalibration, -(STEP_THICKNESS + RING_THICKNESS), MilImage)

                ' Measure the second circle.
                Console.Write("Second measured circle at z = {0:0.00} cm. ", -(STEP_THICKNESS + RING_THICKNESS))
                MeasureRing(MilSystem, MilDisplay, MilImage, RING2_POS1_X, RING2_POS1_Y)
                Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
                Console.ReadKey()

                ' Clear the overlay to transparent.
                MIL.MdispControl(MilDisplay, MIL.M_OVERLAY_CLEAR, MIL.M_DEFAULT)

                ' Move camera rotation with the tool
                MIL.McalSetCoordinateSystem(MilCalibration, MIL.M_TOOL_COORDINATE_SYSTEM, MIL.M_ABSOLUTE_COORDINATE_SYSTEM, MIL.M_ROTATION_YXZ Or MIL.M_ASSIGN, MIL.M_NULL, ENCODER_ROTATION_Y, ENCODER_ROTATION_X, ENCODER_ROTATION_Z, MIL.M_DEFAULT)

                ' Move camera position with the tool
                MIL.McalSetCoordinateSystem(MilCalibration, MIL.M_TOOL_COORDINATE_SYSTEM, MIL.M_ABSOLUTE_COORDINATE_SYSTEM, MIL.M_TRANSLATION Or MIL.M_COMPOSE_WITH_CURRENT, MIL.M_NULL, ENCODER_TRANSLATION_X, ENCODER_TRANSLATION_Y, ENCODER_TRANSLATION_Z, MIL.M_DEFAULT)
                ' Load source image into an image buffer.
                MIL.MbufLoad(OBJECT_MOVED_IMAGE_FILE, MilImage)

                ' Print camera information
                Console.Write("Calibration module moved camera position and camera " + Constants.vbLf)
                Console.Write("orientation according to known encoder transformation." + Constants.vbLf + Constants.vbLf)
                ShowCameraInformation(MilCalibration)

                ' Set offset to the camera calibration plane and associate calibration to image.
                ' This moves the relative origin to the top of the first measured circle  
                SetCalibrationOffset(MilCalibration, -RING_THICKNESS, MilImage)

                ' Measure the first circle.
                Console.Write("First measured circle at  z = {0:0.00} cm. ", -RING_THICKNESS)
                MeasureRing(MilSystem, MilDisplay, MilImage, RING1_POS2_X, RING1_POS2_Y)

                ' Set offset to the camera calibration plane and associate calibration to image.
                ' This moves the relative origin to the top of the second measured circle 
                SetCalibrationOffset(MilCalibration, -(STEP_THICKNESS + RING_THICKNESS), MilImage)

                ' Measure the second circle.
                Console.Write("Second measured circle at z = {0:0.00} cm. ", -(STEP_THICKNESS + RING_THICKNESS))
                MeasureRing(MilSystem, MilDisplay, MilImage, RING2_POS2_X, RING2_POS2_Y)

                Console.Write("Press <Enter> to quit." + Constants.vbLf + Constants.vbLf)
                Console.ReadKey()
            Else
                Console.Write("Calibration generated an exception." + Constants.vbLf)
                Console.Write("See User Guide to resolve the situation." + Constants.vbLf + Constants.vbLf)
            End If

            ' Free all allocations.
            MIL.McalFree(MilCalibration)
            MIL.MbufFree(MilImage)
        End Sub

        ' Offset camera calibration plane.
        Private Shared Sub SetCalibrationOffset(ByVal MilCalibration As MIL_ID, ByVal ZOffset As Double, ByVal MilImage As MIL_ID)
            MIL.McalSetCoordinateSystem(MilCalibration, MIL.M_RELATIVE_COORDINATE_SYSTEM, MIL.M_ABSOLUTE_COORDINATE_SYSTEM, MIL.M_TRANSLATION + MIL.M_ASSIGN, MIL.M_NULL, 0, 0, ZOffset, MIL.M_DEFAULT)

            ' Associate calibration context to source image
            MIL.McalAssociate(MilCalibration, MilImage, MIL.M_DEFAULT)
        End Sub

        ' Measuring function with MilMetrology module
        Private Shared Sub MeasureRing(ByVal MilSystem As MIL_ID, ByVal MilDisplay As MIL_ID, ByVal MilImage As MIL_ID, ByVal MeasureRingX As Double, ByVal MeasureRingY As Double)
            Dim MilMetrolContext As MIL_ID = MIL.M_NULL ' Metrology Context.
            Dim MilMetrolResult As MIL_ID = MIL.M_NULL ' Metrology Result.
            Dim MilOverlayImage As MIL_ID = MIL.M_NULL ' Overlay image.

            Dim Value As Double = 0.0

            ' Prepare for overlay annotation.
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY, MIL.M_ENABLE)
            MIL.MdispInquire(MilDisplay, MIL.M_OVERLAY_ID, MilOverlayImage)

            ' Allocate metrology context and result.
            MIL.MmetAlloc(MilSystem, MIL.M_DEFAULT, MilMetrolContext)
            MIL.MmetAllocResult(MilSystem, MIL.M_DEFAULT, MilMetrolResult)

            ' Add a first measured segment feature to context and set its search region.
            MIL.MmetAddFeature(MilMetrolContext, MIL.M_MEASURED, MIL.M_CIRCLE, MEASURED_CIRCLE_LABEL, MIL.M_DEFAULT, CType(MIL.M_NULL, IntPtr), CType(MIL.M_NULL, IntPtr), MIL.M_DEFAULT, MIL.M_DEFAULT)

            MIL.MmetSetRegion(MilMetrolContext, MIL.M_FEATURE_LABEL(MEASURED_CIRCLE_LABEL), MIL.M_DEFAULT, MIL.M_RING, MeasureRingX, MeasureRingY, RING_START_RADIUS, RING_END_RADIUS, MIL.M_NULL, MIL.M_NULL)

            ' Calculate.
            MIL.MmetCalculate(MilMetrolContext, MilImage, MilMetrolResult, MIL.M_DEFAULT)

            ' Draw region.
            MIL.MgraColor(MIL.M_DEFAULT, REGION_COLOR)
            MIL.MmetDraw(MIL.M_DEFAULT, MilMetrolResult, MilOverlayImage, MIL.M_DRAW_REGION, MIL.M_DEFAULT, MIL.M_DEFAULT)

            ' Draw features.
            MIL.MgraColor(MIL.M_DEFAULT, FEATURE_COLOR)
            MIL.MmetDraw(MIL.M_DEFAULT, MilMetrolResult, MilOverlayImage, MIL.M_DRAW_FEATURE, MIL.M_DEFAULT, MIL.M_DEFAULT)

            MIL.MmetGetResult(MilMetrolResult, MIL.M_FEATURE_LABEL(MEASURED_CIRCLE_LABEL), MIL.M_RADIUS, Value)
            Console.Write("Measured Radius: {0:0.000} cm" + Constants.vbLf, Value)

            ' Free all allocations.
            MIL.MmetFree(MilMetrolResult)
            MIL.MmetFree(MilMetrolContext)
        End Sub

        ' Print the current camera position and orientation 
        Private Shared Sub ShowCameraInformation(ByVal MilCalibration As MIL_ID)
            Dim CameraPosX As Double = 0.0
            Dim CameraPosY As Double = 0.0
            Dim CameraPosZ As Double = 0.0
            Dim CameraYaw As Double = 0.0
            Dim CameraPitch As Double = 0.0
            Dim CameraRoll As Double = 0.0

            MIL.McalGetCoordinateSystem(MilCalibration, MIL.M_CAMERA_COORDINATE_SYSTEM, MIL.M_ABSOLUTE_COORDINATE_SYSTEM, MIL.M_TRANSLATION, CType(MIL.M_NULL, IntPtr), CameraPosX, CameraPosY, CameraPosZ, CType(MIL.M_NULL, IntPtr))
            MIL.McalGetCoordinateSystem(MilCalibration, MIL.M_CAMERA_COORDINATE_SYSTEM, MIL.M_ABSOLUTE_COORDINATE_SYSTEM, MIL.M_ROTATION_YXZ, CType(MIL.M_NULL, IntPtr), CameraYaw, CameraPitch, CameraRoll, CType(MIL.M_NULL, IntPtr))

            ' Pause to show the corrected image of the grid.
            Console.Write("Camera Position in cm:          (x, y, z)           ({0:0.00}, {1:0.00}, {2:0.00})" + Constants.vbLf, CameraPosX, CameraPosY, CameraPosZ)
            Console.Write("Camera Orientation in degrees:  (yaw, pitch, roll)  ({0:0.00}, {1:0.00}, {2:0.00})" + Constants.vbLf, CameraYaw, CameraPitch, CameraRoll)
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()
        End Sub
    End Class
End Namespace
