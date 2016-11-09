'*****************************************************************************
' 
' File name: Mmet.vb
'
' Synopsis:  This program uses the MIL Metrology module to measure geometric 
'            features and to validate tolerance relationships between features.
'*****************************************************************************
Imports Microsoft.VisualBasic
Imports System
Imports System.Collections.Generic
Imports System.Text

Imports Matrox.MatroxImagingLibrary

Namespace MMet
    Friend Class Program
        ' Example selection.
        Private Const RUN_SIMPLE_IMAGE_EXAMPLE As Integer = MIL.M_YES
        Private Const RUN_COMPLETE_IMAGE_EXAMPLE As Integer = MIL.M_YES

        '*****************************************************************************
        ' Main.
        '*****************************************************************************
        Shared Sub Main(ByVal args() As String)
            Dim MilApplication As MIL_ID = MIL.M_NULL ' Application identifier. 
            Dim MilSystem As MIL_ID = MIL.M_NULL ' System Identifier. 
            Dim MilDisplay As MIL_ID = MIL.M_NULL ' Display identifier. 

            ' Allocate defaults. 
            MIL.MappAllocDefault(MIL.M_DEFAULT, MilApplication, MilSystem, MilDisplay, CType(MIL.M_NULL, IntPtr), CType(MIL.M_NULL, IntPtr))

            ' Print module name. 
            Console.Write(Constants.vbLf + "METROLOGY MODULE:" + Constants.vbLf)
            Console.Write("-------------------" + Constants.vbLf + Constants.vbLf)

            If RUN_SIMPLE_IMAGE_EXAMPLE = MIL.M_YES Then
                SimpleImageExample(MilSystem, MilDisplay)
            End If

            If RUN_COMPLETE_IMAGE_EXAMPLE = MIL.M_YES Then
                CompleteImageExample(MilSystem, MilDisplay)
            End If

            ' Free defaults. 
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL)
        End Sub

        '*****************************************************************************
        ' Simple example.
        '*****************************************************************************
        ' Source MIL image file specification. 
        Private Const METROL_SIMPLE_IMAGE_FILE As String = MIL.M_IMAGE_PATH & "SingleModel.mim"

        ' Region parameters 
        Private Const TOP_RING_POSITION_X As Integer = 240
        Private Const TOP_RING_POSITION_Y As Integer = 155
        Private Const TOP_RING_START_RADIUS As Integer = 2
        Private Const TOP_RING_END_RADIUS As Integer = 15

        Private Const MIDDLE_RING_POSITION_X As Integer = 240
        Private Const MIDDLE_RING_POSITION_Y As Integer = 190
        Private Const MIDDLE_RING_START_RADIUS As Integer = 2
        Private Const MIDDLE_RING_END_RADIUS As Integer = 15

        Private Const BOTTOM_RECT_POSITION_X As Integer = 320
        Private Const BOTTOM_RECT_POSITION_Y As Integer = 265
        Private Const BOTTOM_RECT_WIDTH As Integer = 170
        Private Const BOTTOM_RECT_HEIGHT As Integer = 20
        Private Const BOTTOM_RECT_ANGLE As Integer = 180

        ' Tolerance parameters 
        Private Const PERPENDICULARITY_MIN As Double = 0.5
        Private Const PERPENDICULARITY_MAX As Double = 0.5

        ' Color definitions 
        Private Shared ReadOnly FAIL_COLOR As Double = MIL.M_RGB888(255, 0, 0)
        Private Shared ReadOnly PASS_COLOR As Double = MIL.M_RGB888(0, 255, 0)
        Private Shared ReadOnly REGION_COLOR As Double = MIL.M_RGB888(0, 100, 255)
        Private Shared ReadOnly FEATURE_COLOR As Double = MIL.M_RGB888(255, 0, 255)

        Private Shared Sub SimpleImageExample(ByVal MilSystem As MIL_ID, ByVal MilDisplay As MIL_ID)
            Dim MilImage As MIL_ID = MIL.M_NULL             ' Image buffer identifier.
            Dim GraphicList As MIL_ID = MIL.M_NULL          ' Graphic list identifier.
            Dim MilMetrolContext As MIL_ID = MIL.M_NULL     ' Metrology Context
            Dim MilMetrolResult As MIL_ID = MIL.M_NULL      ' Metrology Result

            Dim Status As Double = 0.0
            Dim Value As Double = 0.0

            Dim FeatureIndexForTopConstructedPoint As MIL_INT = MIL.M_FEATURE_INDEX(1)
            Dim FeatureIndexForMiddleConstructedPoint As MIL_INT = MIL.M_FEATURE_INDEX(2)
            Dim FeatureIndexForConstructedSegment(1) As MIL_INT
            Dim FeatureIndexForTolerance(1) As MIL_INT

            FeatureIndexForConstructedSegment(0) = MIL.M_FEATURE_INDEX(3)
            FeatureIndexForConstructedSegment(1) = MIL.M_FEATURE_INDEX(4)
            FeatureIndexForTolerance(0) = MIL.M_FEATURE_INDEX(5)
            FeatureIndexForTolerance(1) = MIL.M_FEATURE_INDEX(6)

            ' Restore and display the source image. 
            MIL.MbufRestore(METROL_SIMPLE_IMAGE_FILE, MilSystem, MilImage)
            MIL.MdispSelect(MilDisplay, MilImage)

            ' Allocate a graphic list to hold the subpixel annotations to draw.
            MIL.MgraAllocList(MilSystem, MIL.M_DEFAULT, GraphicList)

            ' Associate the graphic list to the display for annotations.
            MIL.MdispControl(MilDisplay, MIL.M_ASSOCIATED_GRAPHIC_LIST_ID, GraphicList)

            ' Allocate metrology context and result. 
            MIL.MmetAlloc(MilSystem, MIL.M_DEFAULT, MilMetrolContext)
            MIL.MmetAllocResult(MilSystem, MIL.M_DEFAULT, MilMetrolResult)

            ' Add a first measured circle feature to context and set its search region 
            MIL.MmetAddFeature(MilMetrolContext, MIL.M_MEASURED, MIL.M_CIRCLE, MIL.M_DEFAULT, MIL.M_DEFAULT, CType(MIL.M_NULL, IntPtr), CType(MIL.M_NULL, IntPtr), MIL.M_DEFAULT, MIL.M_DEFAULT)

            MIL.MmetSetRegion(MilMetrolContext, MIL.M_FEATURE_INDEX(1), MIL.M_DEFAULT, MIL.M_RING, TOP_RING_POSITION_X, TOP_RING_POSITION_Y, TOP_RING_START_RADIUS, TOP_RING_END_RADIUS, MIL.M_NULL, MIL.M_NULL)

            ' Add a second measured circle feature to context and set its search region 
            MIL.MmetAddFeature(MilMetrolContext, MIL.M_MEASURED, MIL.M_CIRCLE, MIL.M_DEFAULT, MIL.M_DEFAULT, CType(MIL.M_NULL, IntPtr), CType(MIL.M_NULL, IntPtr), MIL.M_DEFAULT, MIL.M_DEFAULT)

            MIL.MmetSetRegion(MilMetrolContext, MIL.M_FEATURE_INDEX(2), MIL.M_DEFAULT, MIL.M_RING, MIDDLE_RING_POSITION_X, MIDDLE_RING_POSITION_Y, MIDDLE_RING_START_RADIUS, MIDDLE_RING_END_RADIUS, MIL.M_NULL, MIL.M_NULL)

            ' Add a first constructed point feature to context 
            MIL.MmetAddFeature(MilMetrolContext, MIL.M_CONSTRUCTED, MIL.M_POINT, MIL.M_DEFAULT, MIL.M_CENTER, FeatureIndexForTopConstructedPoint, CType(MIL.M_NULL, IntPtr), 1, MIL.M_DEFAULT)

            ' Add a second constructed point feature to context 
            MIL.MmetAddFeature(MilMetrolContext, MIL.M_CONSTRUCTED, MIL.M_POINT, MIL.M_DEFAULT, MIL.M_CENTER, FeatureIndexForMiddleConstructedPoint, CType(MIL.M_NULL, IntPtr), 1, MIL.M_DEFAULT)

            ' Add a constructed segment feature to context passing through the two points 
            MIL.MmetAddFeature(MilMetrolContext, MIL.M_CONSTRUCTED, MIL.M_SEGMENT, MIL.M_DEFAULT, MIL.M_CONSTRUCTION, FeatureIndexForConstructedSegment, MIL.M_NULL, 2, MIL.M_DEFAULT)

            ' Add a first segment feature to context and set its search region 
            MIL.MmetAddFeature(MilMetrolContext, MIL.M_MEASURED, MIL.M_SEGMENT, MIL.M_DEFAULT, MIL.M_DEFAULT, CType(MIL.M_NULL, IntPtr), CType(MIL.M_NULL, IntPtr), MIL.M_DEFAULT, MIL.M_DEFAULT)

            MIL.MmetSetRegion(MilMetrolContext, MIL.M_FEATURE_INDEX(6), MIL.M_DEFAULT, MIL.M_RECTANGLE, BOTTOM_RECT_POSITION_X, BOTTOM_RECT_POSITION_Y, BOTTOM_RECT_WIDTH, BOTTOM_RECT_HEIGHT, BOTTOM_RECT_ANGLE, MIL.M_NULL)

            ' Add perpendicularity tolerance 
            MIL.MmetAddTolerance(MilMetrolContext, MIL.M_PERPENDICULARITY, MIL.M_DEFAULT, PERPENDICULARITY_MIN, PERPENDICULARITY_MAX, FeatureIndexForTolerance, MIL.M_NULL, 2, MIL.M_DEFAULT)

            ' Calculate 
            MIL.MmetCalculate(MilMetrolContext, MilImage, MilMetrolResult, MIL.M_DEFAULT)

            ' Draw region 
            MIL.MgraColor(MIL.M_DEFAULT, REGION_COLOR)
            MIL.MmetDraw(MIL.M_DEFAULT, MilMetrolResult, GraphicList, MIL.M_DRAW_REGION, MIL.M_DEFAULT, MIL.M_DEFAULT)
            Console.Write("Regions used to calculate measured features:" + Constants.vbLf)
            Console.Write("- two measured circles" + Constants.vbLf)
            Console.Write("- one measured segment" + Constants.vbLf)
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Clear annotations.
            MIL.MgraClear(MIL.M_DEFAULT, GraphicList)

            MIL.MgraColor(MIL.M_DEFAULT, FEATURE_COLOR)
            MIL.MmetDraw(MIL.M_DEFAULT, MilMetrolResult, GraphicList, MIL.M_DRAW_FEATURE, MIL.M_DEFAULT, MIL.M_DEFAULT)
            Console.Write("Calculated features:" + Constants.vbLf)

            MIL.MmetGetResult(MilMetrolResult, MIL.M_FEATURE_INDEX(1), MIL.M_RADIUS, Value)
            Console.Write("- first measured circle:  radius={0:0.00}" + Constants.vbLf, Value)

            MIL.MmetGetResult(MilMetrolResult, MIL.M_FEATURE_INDEX(2), MIL.M_RADIUS, Value)
            Console.Write("- second measured circle: radius={0:0.00}" + Constants.vbLf, Value)

            MIL.MmetGetResult(MilMetrolResult, MIL.M_FEATURE_INDEX(5), MIL.M_LENGTH, Value)
            Console.Write("- constructed segment between the two circle centers: length={0:0.00}" + Constants.vbLf, Value)

            MIL.MmetGetResult(MilMetrolResult, MIL.M_FEATURE_INDEX(6), MIL.M_LENGTH, Value)
            Console.Write("- measured segment: length={0:0.00}" + Constants.vbLf, Value)

            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Get angularity tolerance status and value
            MIL.MmetGetResult(MilMetrolResult, MIL.M_TOLERANCE_INDEX(0), MIL.M_STATUS, Status)
            MIL.MmetGetResult(MilMetrolResult, MIL.M_TOLERANCE_INDEX(0), MIL.M_TOLERANCE_VALUE, Value)

            If Status = MIL.M_PASS Then
                MIL.MgraColor(MIL.M_DEFAULT, PASS_COLOR)
                Console.Write("Perpendicularity between the two segments: {0:0.00} degrees." + Constants.vbLf, Value)
            Else
                MIL.MgraColor(MIL.M_DEFAULT, FAIL_COLOR)
                Console.Write("Perpendicularity between the two segments - Fail." + Constants.vbLf)
            End If
            MIL.MmetDraw(MIL.M_DEFAULT, MilMetrolResult, GraphicList, MIL.M_DRAW_TOLERANCE, MIL.M_TOLERANCE_INDEX(0), MIL.M_DEFAULT)

            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Free all allocations. 
            MIL.MgraFree(GraphicList)
            MIL.MmetFree(MilMetrolResult)
            MIL.MmetFree(MilMetrolContext)
            MIL.MbufFree(MilImage)
        End Sub

        '****************************************************************************
        ' Complete example. 
        '****************************************************************************
        ' Source MIL image, calibration and model finder context file specification. 
        Private Const METROL_CALIBRATION_FILE As String = MIL.M_IMAGE_PATH & "Hook.mca"
        Private Const METROL_COMPLETE_IMAGE_FILE As String = MIL.M_IMAGE_PATH & "Hook.tif"
        Private Const METROL_MODEL_FINDER_FILE As String = MIL.M_IMAGE_PATH & "Hook.mmf"

        ' Region parameters 
        Private Const CIRCLE1_LABEL As Integer = 1
        Private Const RING1_POSITION_X As Double = 1.1
        Private Const RING1_POSITION_Y As Double = 0.8
        Private Const RING1_START_RADIUS As Double = 0.2
        Private Const RING1_END_RADIUS As Double = 0.5

        Private Const CIRCLE2_LABEL As Integer = 2
        Private Const RING2_POSITION_X As Double = 1.1
        Private Const RING2_POSITION_Y As Double = 3.0
        Private Const RING2_START_RADIUS As Double = 0.1
        Private Const RING2_END_RADIUS As Double = 0.4

        Private Const SEGMENT1_LABEL As Integer = 3
        Private Const RECT1_POSITION_X As Double = 0.1
        Private Const RECT1_POSITION_Y As Double = 2.4
        Private Const RECT1_WIDTH As Double = 1.4
        Private Const RECT1_HEIGHT As Double = 0.3
        Private Const RECT1_ANGLE As Double = 90.0

        Private Const SEGMENT2_LABEL As Integer = 4
        Private Const RECT2_POSITION_X As Double = 0.9
        Private Const RECT2_POSITION_Y As Double = 2.8
        Private Const RECT2_WIDTH As Double = 0.4
        Private Const RECT2_HEIGHT As Double = 0.2
        Private Const RECT2_ANGLE As Double = 165.0

        Private Const POINT1_LABEL As Integer = 5
        Private Const SEG1_START_X As Double = 1.6
        Private Const SEG1_START_Y As Double = 1.5
        Private Const SEG1_END_X As Double = 1.6
        Private Const SEG1_END_Y As Double = 2.4

        ' Tolerance parameters 
        Private Const MIN_DISTANCE_LABEL As Integer = 1
        Private Const ANGULARITY_LABEL As Integer = 2
        Private Const MAX_DISTANCE_LABEL As Integer = 3

        Private Const MIN_DISTANCE_VALUE_MIN As Double = 1.4
        Private Const MIN_DISTANCE_VALUE_MAX As Double = 1.6
        Private Const MAX_DISTANCE_VALUE_MIN As Double = 0.4
        Private Const MAX_DISTANCE_VALUE_MAX As Double = 0.6
        Private Const ANGULARITY_VALUE_MIN As Double = 65.0
        Private Const ANGULARITY_VALUE_MAX As Double = 75.0

        Private Shared Sub CompleteImageExample(ByVal MilSystem As MIL_ID, ByVal MilDisplay As MIL_ID)
            Dim MilImage As MIL_ID = MIL.M_NULL                 ' Image buffer identifier. 
            Dim GraphicList As MIL_ID = MIL.M_NULL              ' Overlay image. 
            Dim MilCalibration As MIL_ID = MIL.M_NULL           ' Calibration context 
            Dim MilMetrolContext As MIL_ID = MIL.M_NULL         ' Metrology Context 
            Dim MilMetrolResult As MIL_ID = MIL.M_NULL          ' Metrology Result 
            Dim MilModelFinderContext As MIL_ID = MIL.M_NULL    ' Model Finder Context 
            Dim MilModelFinderResult As MIL_ID = MIL.M_NULL     ' Model Finder Result 

            Dim Status As Double = 0.0
            Dim Value As Double = 0.0

            Dim MinDistanceFeatureLabels(1) As MIL_INT
            Dim AngularityFeatureLabels(1) As MIL_INT
            Dim MaxDistanceFeatureLabels(1) As MIL_INT
            Dim MaxDistanceFeatureIndices(1) As MIL_INT


            MinDistanceFeatureLabels(0) = CIRCLE1_LABEL
            MinDistanceFeatureLabels(1) = CIRCLE2_LABEL

            AngularityFeatureLabels(0) = SEGMENT1_LABEL
            AngularityFeatureLabels(1) = SEGMENT2_LABEL

            MaxDistanceFeatureLabels(0) = POINT1_LABEL
            MaxDistanceFeatureLabels(1) = POINT1_LABEL

            MaxDistanceFeatureIndices(0) = 0
            MaxDistanceFeatureIndices(1) = 1

            ' Restore and display the source image. 
            MIL.MbufRestore(METROL_COMPLETE_IMAGE_FILE, MilSystem, MilImage)
            MIL.MdispSelect(MilDisplay, MilImage)

            ' Restore and associate calibration context to source image 
            MIL.McalRestore(METROL_CALIBRATION_FILE, MilSystem, MIL.M_DEFAULT, MilCalibration)
            MIL.McalAssociate(MilCalibration, MilImage, MIL.M_DEFAULT)

            ' Allocate a graphic list to hold the subpixel annotations to draw.
            MIL.MgraAllocList(MilSystem, MIL.M_DEFAULT, GraphicList)

            ' Associate the graphic list to the display for annotations.
            MIL.MdispControl(MilDisplay, MIL.M_ASSOCIATED_GRAPHIC_LIST_ID, GraphicList)

            ' Allocate metrology context and result. 
            MIL.MmetAlloc(MilSystem, MIL.M_DEFAULT, MilMetrolContext)
            MIL.MmetAllocResult(MilSystem, MIL.M_DEFAULT, MilMetrolResult)

            ' Add a first measured circle feature to context and set its search region 
            MIL.MmetAddFeature(MilMetrolContext, MIL.M_MEASURED, MIL.M_CIRCLE, CIRCLE1_LABEL, MIL.M_DEFAULT, CType(MIL.M_NULL, IntPtr), CType(MIL.M_NULL, IntPtr), MIL.M_DEFAULT, MIL.M_DEFAULT)

            MIL.MmetSetRegion(MilMetrolContext, MIL.M_FEATURE_LABEL(CIRCLE1_LABEL), MIL.M_DEFAULT, MIL.M_RING, RING1_POSITION_X, RING1_POSITION_Y, RING1_START_RADIUS, RING1_END_RADIUS, MIL.M_NULL, MIL.M_NULL)

            ' Add a second measured circle feature to context and set its search region 
            MIL.MmetAddFeature(MilMetrolContext, MIL.M_MEASURED, MIL.M_CIRCLE, CIRCLE2_LABEL, MIL.M_DEFAULT, CType(MIL.M_NULL, IntPtr), CType(MIL.M_NULL, IntPtr), MIL.M_DEFAULT, MIL.M_DEFAULT)

            MIL.MmetSetRegion(MilMetrolContext, MIL.M_FEATURE_LABEL(CIRCLE2_LABEL), MIL.M_DEFAULT, MIL.M_RING, RING2_POSITION_X, RING2_POSITION_Y, RING2_START_RADIUS, RING2_END_RADIUS, MIL.M_NULL, MIL.M_NULL)

            ' Add a first measured segment feature to context and set its search region 
            MIL.MmetAddFeature(MilMetrolContext, MIL.M_MEASURED, MIL.M_SEGMENT, SEGMENT1_LABEL, MIL.M_DEFAULT, CType(MIL.M_NULL, IntPtr), CType(MIL.M_NULL, IntPtr), MIL.M_DEFAULT, MIL.M_DEFAULT)

            MIL.MmetSetRegion(MilMetrolContext, MIL.M_FEATURE_LABEL(SEGMENT1_LABEL), MIL.M_DEFAULT, MIL.M_RECTANGLE, RECT1_POSITION_X, RECT1_POSITION_Y, RECT1_WIDTH, RECT1_HEIGHT, RECT1_ANGLE, MIL.M_NULL)

            MIL.MmetControl(MilMetrolContext, MIL.M_FEATURE_LABEL(SEGMENT1_LABEL), MIL.M_EDGEL_ANGLE_RANGE, 10)

            ' Add a second measured segment feature to context and set its search region 
            MIL.MmetAddFeature(MilMetrolContext, MIL.M_MEASURED, MIL.M_SEGMENT, SEGMENT2_LABEL, MIL.M_INNER_FIT, CType(MIL.M_NULL, IntPtr), CType(MIL.M_NULL, IntPtr), MIL.M_DEFAULT, MIL.M_DEFAULT)

            MIL.MmetSetRegion(MilMetrolContext, MIL.M_FEATURE_LABEL(SEGMENT2_LABEL), MIL.M_DEFAULT, MIL.M_RECTANGLE, RECT2_POSITION_X, RECT2_POSITION_Y, RECT2_WIDTH, RECT2_HEIGHT, RECT2_ANGLE, MIL.M_NULL)

            ' Add a measured point feature to context and set its search region 
            MIL.MmetAddFeature(MilMetrolContext, MIL.M_MEASURED, MIL.M_POINT, POINT1_LABEL, MIL.M_DEFAULT, CType(MIL.M_NULL, IntPtr), CType(MIL.M_NULL, IntPtr), MIL.M_DEFAULT, MIL.M_DEFAULT)

            MIL.MmetSetRegion(MilMetrolContext, MIL.M_FEATURE_LABEL(POINT1_LABEL), MIL.M_DEFAULT, MIL.M_SEGMENT, SEG1_START_X, SEG1_START_Y, SEG1_END_X, SEG1_END_Y, MIL.M_NULL, MIL.M_NULL)

            MIL.MmetControl(MilMetrolContext, MIL.M_FEATURE_LABEL(POINT1_LABEL), MIL.M_FILTER_MODE, MIL.M_KERNEL)

            ' Set the polarity and the maximum number of points to detect along the segment region 
            MIL.MmetControl(MilMetrolContext, MIL.M_FEATURE_LABEL(POINT1_LABEL), MIL.M_EDGEL_RELATIVE_ANGLE, MIL.M_SAME_OR_REVERSE)
            MIL.MmetControl(MilMetrolContext, MIL.M_FEATURE_LABEL(POINT1_LABEL), MIL.M_NUMBER_MAX, 2)

            ' Add minimum distance tolerance 
            MIL.MmetAddTolerance(MilMetrolContext, MIL.M_DISTANCE_MIN, MIN_DISTANCE_LABEL, MIN_DISTANCE_VALUE_MIN, MIN_DISTANCE_VALUE_MAX, MinDistanceFeatureLabels, MIL.M_NULL, 2, MIL.M_DEFAULT)

            ' Add angularity tolerance 
            MIL.MmetAddTolerance(MilMetrolContext, MIL.M_ANGULARITY, ANGULARITY_LABEL, ANGULARITY_VALUE_MIN, ANGULARITY_VALUE_MAX, AngularityFeatureLabels, MIL.M_NULL, 2, MIL.M_DEFAULT)


            ' Add maximum distance tolerance 
            MIL.MmetAddTolerance(MilMetrolContext, MIL.M_DISTANCE_MAX, MAX_DISTANCE_LABEL, MAX_DISTANCE_VALUE_MIN, MAX_DISTANCE_VALUE_MAX, MaxDistanceFeatureLabels, MaxDistanceFeatureIndices, 2, MIL.M_DEFAULT)

            ' Calculate 
            MIL.MmetCalculate(MilMetrolContext, MilImage, MilMetrolResult, MIL.M_DEFAULT)

            ' Draw features 
            MIL.MgraColor(MIL.M_DEFAULT, REGION_COLOR)
            MIL.MmetDraw(MIL.M_DEFAULT, MilMetrolResult, GraphicList, MIL.M_DRAW_REGION, MIL.M_DEFAULT, MIL.M_DEFAULT)
            Console.Write("Regions used to calculate measured features:" + Constants.vbLf)
            Console.Write("- two measured circle features" + Constants.vbLf)
            Console.Write("- two measured segment features" + Constants.vbLf)
            Console.Write("- one measured points feature" + Constants.vbLf)
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Clear annotations.
            MIL.MgraClear(MIL.M_DEFAULT, GraphicList)

            MIL.MgraColor(MIL.M_DEFAULT, FEATURE_COLOR)
            MIL.MmetDraw(MIL.M_DEFAULT, MilMetrolResult, GraphicList, MIL.M_DRAW_FEATURE, MIL.M_DEFAULT, MIL.M_DEFAULT)
            Console.Write("Calculated features:" + Constants.vbLf)

            MIL.MmetGetResult(MilMetrolResult, MIL.M_FEATURE_LABEL(CIRCLE1_LABEL), MIL.M_RADIUS, Value)
            Console.Write("- first measured circle:   radius={0:0.00}mm" + Constants.vbLf, Value)

            MIL.MmetGetResult(MilMetrolResult, MIL.M_FEATURE_LABEL(CIRCLE2_LABEL), MIL.M_RADIUS, Value)
            Console.Write("- second measured circle:  radius={0:0.00}mm" + Constants.vbLf, Value)

            MIL.MmetGetResult(MilMetrolResult, MIL.M_FEATURE_LABEL(SEGMENT1_LABEL), MIL.M_LENGTH, Value)
            Console.Write("- first measured segment:  length={0:0.00}mm" + Constants.vbLf, Value)

            MIL.MmetGetResult(MilMetrolResult, MIL.M_FEATURE_LABEL(SEGMENT2_LABEL), MIL.M_LENGTH, Value)
            Console.Write("- second measured segment: length={0:0.00}mm" + Constants.vbLf, Value)

            Console.Write("- two measured points" + Constants.vbLf)

            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Get angularity tolerance status and value 
            MIL.MmetGetResult(MilMetrolResult, MIL.M_TOLERANCE_LABEL(ANGULARITY_LABEL), MIL.M_STATUS, Status)
            MIL.MmetGetResult(MilMetrolResult, MIL.M_TOLERANCE_LABEL(ANGULARITY_LABEL), MIL.M_TOLERANCE_VALUE, Value)

            If Status = MIL.M_PASS Then
                MIL.MgraColor(MIL.M_DEFAULT, PASS_COLOR)
                Console.Write("Angularity value: {0:0.00} degrees." + Constants.vbLf, Value)
            Else
                MIL.MgraColor(MIL.M_DEFAULT, FAIL_COLOR)
                Console.Write("Angularity value - Fail." + Constants.vbLf)
            End If
            MIL.MmetDraw(MIL.M_DEFAULT, MilMetrolResult, GraphicList, MIL.M_DRAW_TOLERANCE, MIL.M_TOLERANCE_LABEL(ANGULARITY_LABEL), MIL.M_DEFAULT)

            ' Get min distance tolerance status and value 
            MIL.MmetGetResult(MilMetrolResult, MIL.M_TOLERANCE_LABEL(MIN_DISTANCE_LABEL), MIL.M_STATUS, Status)
            MIL.MmetGetResult(MilMetrolResult, MIL.M_TOLERANCE_LABEL(MIN_DISTANCE_LABEL), MIL.M_TOLERANCE_VALUE, Value)

            If Status = MIL.M_PASS Then
                MIL.MgraColor(MIL.M_DEFAULT, PASS_COLOR)
                Console.Write("Min distance tolerance value: {0:0.00} mm." + Constants.vbLf, Value)
            Else
                MIL.MgraColor(MIL.M_DEFAULT, FAIL_COLOR)
                Console.Write("Min distance tolerance value - Fail." + Constants.vbLf)
            End If
            MIL.MmetDraw(MIL.M_DEFAULT, MilMetrolResult, GraphicList, MIL.M_DRAW_TOLERANCE, MIL.M_TOLERANCE_LABEL(MIN_DISTANCE_LABEL), MIL.M_DEFAULT)

            ' Get max distance tolerance status and value 
            MIL.MmetGetResult(MilMetrolResult, MIL.M_TOLERANCE_LABEL(MAX_DISTANCE_LABEL), MIL.M_STATUS, Status)
            MIL.MmetGetResult(MilMetrolResult, MIL.M_TOLERANCE_LABEL(MAX_DISTANCE_LABEL), MIL.M_TOLERANCE_VALUE, Value)

            If Status = MIL.M_PASS Then
                MIL.MgraColor(MIL.M_DEFAULT, PASS_COLOR)
                Console.Write("Max distance tolerance value: {0:0.00} mm." + Constants.vbLf, Value)
            Else
                MIL.MgraColor(MIL.M_DEFAULT, FAIL_COLOR)
                Console.Write("Max distance tolerance value - Fail." + Constants.vbLf)
            End If
            MIL.MmetDraw(MIL.M_DEFAULT, MilMetrolResult, GraphicList, MIL.M_DRAW_TOLERANCE, MIL.M_TOLERANCE_LABEL(MAX_DISTANCE_LABEL), MIL.M_DEFAULT)

            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Clear annotations.
            MIL.MgraClear(MIL.M_DEFAULT, GraphicList)

            ' Restore the model finder context and calibrate it 
            MIL.MmodRestore(METROL_MODEL_FINDER_FILE, MilSystem, MIL.M_DEFAULT, MilModelFinderContext)
            MIL.MmodControl(MilModelFinderContext, 0, MIL.M_ASSOCIATED_CALIBRATION, MilCalibration)

            ' Allocate a result buffer 
            MIL.MmodAllocResult(MilSystem, MIL.M_DEFAULT, MilModelFinderResult)

            ' Find object occurrence 
            MIL.MmodPreprocess(MilModelFinderContext, MIL.M_DEFAULT)
            MIL.MmodFind(MilModelFinderContext, MilImage, MilModelFinderResult)

            ' Get number of found occurrences 
            MIL.MmodGetResult(MilModelFinderResult, MIL.M_GENERAL, MIL.M_NUMBER, Value)

            If Value = 1 Then
                MIL.MmodDraw(MIL.M_DEFAULT, MilModelFinderResult, GraphicList, MIL.M_DRAW_POSITION + MIL.M_DRAW_BOX, MIL.M_DEFAULT, MIL.M_DEFAULT)
                Console.Write("Found occurrence using MIL Model Finder." + Constants.vbLf)
                Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
                Console.ReadKey()

                ' Clear annotations.
                MIL.MgraClear(MIL.M_DEFAULT, GraphicList)

                ' Set the new context position 
                MIL.MmetSetPosition(MilMetrolContext, MIL.M_DEFAULT, MIL.M_RESULT, MilModelFinderResult, 0, MIL.M_NULL, MIL.M_NULL, MIL.M_DEFAULT)

                ' Calculate 
                MIL.MmetCalculate(MilMetrolContext, MilImage, MilMetrolResult, MIL.M_DEFAULT)

                ' Draw features 
                MIL.MgraColor(MIL.M_DEFAULT, REGION_COLOR)
                MIL.MmetDraw(MIL.M_DEFAULT, MilMetrolResult, GraphicList, MIL.M_DRAW_REGION, MIL.M_DEFAULT, MIL.M_DEFAULT)
                Console.Write("Regions used to calculate measured features at the new location." + Constants.vbLf)
                Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
                Console.ReadKey()

                ' Clear annotations.
                MIL.MgraClear(MIL.M_DEFAULT, GraphicList)

                MIL.MgraColor(MIL.M_DEFAULT, FEATURE_COLOR)
                MIL.MmetDraw(MIL.M_DEFAULT, MilMetrolResult, GraphicList, MIL.M_DRAW_FEATURE, MIL.M_DEFAULT, MIL.M_DEFAULT)
                Console.Write("Calculated features." + Constants.vbLf)

                MIL.MmetGetResult(MilMetrolResult, MIL.M_FEATURE_LABEL(CIRCLE1_LABEL), MIL.M_RADIUS, Value)
                Console.Write("- first measured circle:   radius={0:0.00}mm" + Constants.vbLf, Value)

                MIL.MmetGetResult(MilMetrolResult, MIL.M_FEATURE_LABEL(CIRCLE2_LABEL), MIL.M_RADIUS, Value)
                Console.Write("- second measured circle:  radius={0:0.00}mm" + Constants.vbLf, Value)

                MIL.MmetGetResult(MilMetrolResult, MIL.M_FEATURE_LABEL(SEGMENT1_LABEL), MIL.M_LENGTH, Value)
                Console.Write("- first measured segment:  length={0:0.00}mm" + Constants.vbLf, Value)

                MIL.MmetGetResult(MilMetrolResult, MIL.M_FEATURE_LABEL(SEGMENT2_LABEL), MIL.M_LENGTH, Value)
                Console.Write("- second measured segment: length={0:0.00}mm" + Constants.vbLf, Value)

                Console.Write("- two measured points" + Constants.vbLf)

                Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
                Console.ReadKey()

                ' Get angularity tolerance status and value 
                MIL.MmetGetResult(MilMetrolResult, MIL.M_TOLERANCE_LABEL(ANGULARITY_LABEL), MIL.M_STATUS, Status)
                MIL.MmetGetResult(MilMetrolResult, MIL.M_TOLERANCE_LABEL(ANGULARITY_LABEL), MIL.M_TOLERANCE_VALUE, Value)

                If Status = MIL.M_PASS Then
                    MIL.MgraColor(MIL.M_DEFAULT, PASS_COLOR)
                    Console.Write("Angularity value: {0:0.00} degrees." + Constants.vbLf, Value)
                Else
                    MIL.MgraColor(MIL.M_DEFAULT, FAIL_COLOR)
                    Console.Write("Angularity value - Fail." + Constants.vbLf)
                End If
                MIL.MmetDraw(MIL.M_DEFAULT, MilMetrolResult, GraphicList, MIL.M_DRAW_TOLERANCE, MIL.M_TOLERANCE_LABEL(ANGULARITY_LABEL), MIL.M_DEFAULT)

                ' Get min distance tolerance status and value 
                MIL.MmetGetResult(MilMetrolResult, MIL.M_TOLERANCE_LABEL(MIN_DISTANCE_LABEL), MIL.M_STATUS, Status)
                MIL.MmetGetResult(MilMetrolResult, MIL.M_TOLERANCE_LABEL(MIN_DISTANCE_LABEL), MIL.M_TOLERANCE_VALUE, Value)

                If Status = MIL.M_PASS Then
                    MIL.MgraColor(MIL.M_DEFAULT, PASS_COLOR)
                    Console.Write("Min distance tolerance value: {0:0.00} mm." + Constants.vbLf, Value)
                Else
                    MIL.MgraColor(MIL.M_DEFAULT, FAIL_COLOR)
                    Console.Write("Min distance tolerance value - Fail." + Constants.vbLf)
                End If
                MIL.MmetDraw(MIL.M_DEFAULT, MilMetrolResult, GraphicList, MIL.M_DRAW_TOLERANCE, MIL.M_TOLERANCE_LABEL(MIN_DISTANCE_LABEL), MIL.M_DEFAULT)

                ' Get max distance tolerance status and value 
                MIL.MmetGetResult(MilMetrolResult, MIL.M_TOLERANCE_LABEL(MAX_DISTANCE_LABEL), MIL.M_STATUS, Status)
                MIL.MmetGetResult(MilMetrolResult, MIL.M_TOLERANCE_LABEL(MAX_DISTANCE_LABEL), MIL.M_TOLERANCE_VALUE, Value)

                If Status = MIL.M_PASS Then
                    MIL.MgraColor(MIL.M_DEFAULT, PASS_COLOR)
                    Console.Write("Max distance tolerance value: {0:0.00} mm." + Constants.vbLf, Value)
                Else
                    MIL.MgraColor(MIL.M_DEFAULT, FAIL_COLOR)
                    Console.Write("Max distance tolerance value - Fail." + Constants.vbLf)
                End If
                MIL.MmetDraw(MIL.M_DEFAULT, MilMetrolResult, GraphicList, MIL.M_DRAW_TOLERANCE, MIL.M_TOLERANCE_LABEL(MAX_DISTANCE_LABEL), MIL.M_DEFAULT)

                Console.Write("Press <Enter> to quit." + Constants.vbLf + Constants.vbLf)
                Console.ReadKey()
            Else
                Console.Write("Occurrence not found." + Constants.vbLf)
                Console.Write("Press <Enter> to quit." + Constants.vbLf + Constants.vbLf)
                Console.ReadKey()
            End If

            ' Free all allocations. 
            MIL.MgraFree(GraphicList)
            MIL.MmodFree(MilModelFinderContext)
            MIL.MmodFree(MilModelFinderResult)
            MIL.MmetFree(MilMetrolResult)
            MIL.MmetFree(MilMetrolContext)
            MIL.McalFree(MilCalibration)
            MIL.MbufFree(MilImage)
        End Sub
    End Class
End Namespace
