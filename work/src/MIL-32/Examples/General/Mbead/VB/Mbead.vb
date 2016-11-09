'******************************************************************************
' 
' File name: Mbead.vb
'
' Synopsis:  This program uses the Bead module to train a bead template
'            and then to inspect a defective bead in a target image.
'
'******************************************************************************
Imports Microsoft.VisualBasic
Imports System
Imports System.Collections.Generic
Imports System.Text

Imports Matrox.MatroxImagingLibrary

Namespace MBead
    Friend Class Program

        ' Utility definitions.
        Private Shared ReadOnly USER_POSITION_COLOR As MIL_INT = MIL.M_COLOR_RED
        Private Shared ReadOnly USER_TEMPLATE_COLOR As MIL_INT = MIL.M_COLOR_CYAN
        Private Shared ReadOnly TRAINED_BEAD_WIDTH_COLOR As MIL_INT = MIL.M_RGB888(255, 128, 0)
        Private Shared ReadOnly MODEL_FINDER_COLOR As MIL_INT = MIL.M_COLOR_GREEN
        Private Shared ReadOnly COORDINATE_SYSTEM_COLOR As MIL_INT = MIL.M_RGB888(164, 164, 0)
        Private Shared ReadOnly RESULT_SEARCH_BOX_COLOR As MIL_INT = MIL.M_COLOR_CYAN
        Private Shared ReadOnly PASS_BEAD_WIDTH_COLOR As MIL_INT = MIL.M_COLOR_GREEN
        Private Shared ReadOnly PASS_BEAD_POSITION_COLOR As MIL_INT = MIL.M_COLOR_GREEN
        Private Shared ReadOnly FAIL_NOT_FOUND_COLOR As MIL_INT = MIL.M_COLOR_RED
        Private Shared ReadOnly FAIL_SMALL_WIDTH_COLOR As MIL_INT = MIL.M_RGB888(255, 128, 0)
        Private Shared ReadOnly FAIL_EDGE_OFFSET_COLOR As MIL_INT = MIL.M_COLOR_RED

        '******************************************************************************
        ' Main. 
        '******************************************************************************
        Shared Sub Main(ByVal args() As String)

            Dim MilApplication As MIL_ID = MIL.M_NULL ' Application identifier.
            Dim MilSystem As MIL_ID = MIL.M_NULL ' System identifier.
            Dim MilDisplay As MIL_ID = MIL.M_NULL ' Display identifier.

            ' Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, MilApplication, MilSystem, MilDisplay, CType(MIL.M_NULL, IntPtr), CType(MIL.M_NULL, IntPtr))

            ' Run fixtured bead example.
            FixturedBeadExample(MilSystem, MilDisplay)

            ' Run predefined bead example.
            PredefinedBeadExample(MilSystem, MilDisplay)

            ' Free defaults.
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL)

        End Sub

        '*****************************************************************************
        ' Fixtured 'stripe' bead example.

        ' Target MIL image specifications.
        Private Shared ReadOnly IMAGE_FILE_TRAINING As String = MIL.M_IMAGE_PATH & "BeadTraining.mim"
        Private Shared ReadOnly IMAGE_FILE_TARGET As String = MIL.M_IMAGE_PATH & "BeadTarget.mim"

        ' Bead stripe training data definition.
        Private Const NUMBER_OF_TRAINING_POINTS As Integer = 13

        Private Shared ReadOnly TrainingPointsX() As Double = {180, 280, 400, 430, 455, 415, 370, 275, 185, 125, 105, 130, 180}
        Private Shared ReadOnly TrainingPointsY() As Double = {190, 215, 190, 200, 260, 330, 345, 310, 340, 305, 265, 200, 190}

        ' Max angle correction.
        Private Const MAX_ANGLE_CORRECTION_VALUE As Double = 20.0

        ' Max offset deviation.
        Private Const MAX_DEVIATION_OFFSET_VALUE As Double = 25.0

        ' Maximum negative width variation.
        Private Const WIDTH_DELTA_NEG_VALUE As Double = 2.0

        ' Model region  definition.
        Private Const MODEL_ORIGIN_X As Integer = 145
        Private Const MODEL_ORIGIN_Y As Integer = 115
        Private Const MODEL_SIZE_X As Integer = 275
        Private Const MODEL_SIZE_Y As Integer = 60

        Private Shared Sub FixturedBeadExample(ByVal MilSystem As MIL_ID, ByVal MIlDisplay As MIL_ID)

            Dim MilGraList As MIL_ID = MIL.M_NULL            ' Graphic list identifier.
            Dim MilImageTraining As MIL_ID = MIL.M_NULL      ' Image buffer identifier.
            Dim MilImageTarget As MIL_ID = MIL.M_NULL        ' Image buffer identifier.
            Dim MilBeadContext As MIL_ID = MIL.M_NULL        ' Bead context identifier.
            Dim MilBeadResult As MIL_ID = MIL.M_NULL         ' Bead result identifier.
            Dim MilModelFinderContext As MIL_ID = MIL.M_NULL ' Model finder context identifier.
            Dim MilModelFinderResult As MIL_ID = MIL.M_NULL  ' Model finder result identifier.
            Dim MilFixturingOffset As MIL_ID = MIL.M_NULL    ' Fixturing offset identifier.

            Dim NominalWidth As Double = 0.0                 ' Nominal width result value.
            Dim AvWidth As Double = 0.0                      ' Average width result value.
            Dim MaxGap As Double = 0.0                       ' Gap coverage result value.
            Dim GapCov As Double = 0.0                       ' Maximum gap result value.
            Dim Score As Double = 0.0                        ' Bead score result value.

            ' Restore source image into an automatically allocated image buffers.
            MIL.MbufRestore(IMAGE_FILE_TRAINING, MilSystem, MilImageTraining)
            MIL.MbufRestore(IMAGE_FILE_TARGET, MilSystem, MilImageTarget)

            ' Display the training image buffer.
            MIL.MdispSelect(MIlDisplay, MilImageTraining)

            ' Allocate a graphic list to hold the subpixel annotations to draw.
            MIL.MgraAllocList(MilSystem, MIL.M_DEFAULT, MilGraList)

            ' Associate the graphic list to the display for annotations.
            MIL.MdispControl(MIlDisplay, MIL.M_ASSOCIATED_GRAPHIC_LIST_ID, MilGraList)

            ' Original template image.
            Console.Write(Constants.vbLf + "FIXTURED BEAD INSPECTION:" + Constants.vbLf)
            Console.Write("-------------------------" + Constants.vbLf + Constants.vbLf)
            Console.Write("This program performs a bead inspection on a mechanical part." + Constants.vbLf)
            Console.Write("In the first step, a bead template context is trained using an ")
            Console.Write("image." + Constants.vbLf + "In the second step, a mechanical part, at an arbitrary ")
            Console.Write("angle and with" + Constants.vbLf + "a defective bead, is inspected." + Constants.vbLf)
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Allocate a MIL bead context.
            MIL.MbeadAlloc(MilSystem, MIL.M_DEFAULT, MIL.M_DEFAULT, MilBeadContext)

            ' Allocate a MIL bead result.
            MIL.MbeadAllocResult(MilSystem, MIL.M_DEFAULT, MilBeadResult)

            ' Add bead templates.
            MIL.MbeadTemplate(MilBeadContext, MIL.M_ADD, MIL.M_DEFAULT, MIL.M_TEMPLATE_LABEL(1), NUMBER_OF_TRAINING_POINTS, TrainingPointsX, TrainingPointsY, CType(MIL.M_NULL, IntPtr), MIL.M_DEFAULT)

            ' Set template input units to world units
            MIL.MbeadControl(MilBeadContext, MIL.M_TEMPLATE_LABEL(1), MIL.M_TEMPLATE_INPUT_UNITS, MIL.M_WORLD)

            ' Set the bead 'edge type' search properties
            MIL.MbeadControl(MilBeadContext, MIL.M_TEMPLATE_LABEL(1), MIL.M_ANGLE_ACCURACY_MAX_DEVIATION, MAX_ANGLE_CORRECTION_VALUE)

            ' Set the maximum valid bead deformation.
            MIL.MbeadControl(MilBeadContext, MIL.M_TEMPLATE_LABEL(1), MIL.M_OFFSET_MAX, MAX_DEVIATION_OFFSET_VALUE)

            ' Set the valid bead minimum width criterion.
            MIL.MbeadControl(MilBeadContext, MIL.M_TEMPLATE_LABEL(1), MIL.M_WIDTH_DELTA_NEG, WIDTH_DELTA_NEG_VALUE)

            ' Display the bead polyline.
            MIL.MgraColor(MIL.M_DEFAULT, USER_TEMPLATE_COLOR)
            MIL.MbeadDraw(MIL.M_DEFAULT, MilBeadContext, MilGraList, MIL.M_DRAW_POSITION_POLYLINE, MIL.M_USER, MIL.M_ALL, MIL.M_ALL, MIL.M_DEFAULT)

            ' Display the bead training points.
            MIL.MgraColor(MIL.M_DEFAULT, USER_POSITION_COLOR)
            MIL.MbeadDraw(MIL.M_DEFAULT, MilBeadContext, MilGraList, MIL.M_DRAW_POSITION, MIL.M_USER, MIL.M_ALL, MIL.M_ALL, MIL.M_DEFAULT)

            ' Pause to show the template image and user points.
            Console.Write("The initial points specified by the user (in red) are" + Constants.vbLf)
            Console.Write("used to train the bead information from an image." + Constants.vbLf)
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Set a 1:1 uniform calibration to the training image for further fixturing.
            MIL.McalUniform(MilImageTraining, 0, 0, 1, 1, 0, MIL.M_DEFAULT)

            ' Train the bead context.
            MIL.MbeadTrain(MilBeadContext, MilImageTraining, MIL.M_DEFAULT)

            ' Display the trained bead.
            MIL.MgraColor(MIL.M_DEFAULT, TRAINED_BEAD_WIDTH_COLOR)
            MIL.MbeadDraw(MIL.M_DEFAULT, MilBeadContext, MilGraList, MIL.M_DRAW_WIDTH, MIL.M_TRAINED, MIL.M_TEMPLATE_LABEL(1), MIL.M_ALL, MIL.M_DEFAULT)

            ' Retrieve the trained nominal width.
            MIL.MbeadInquire(MilBeadContext, MIL.M_TEMPLATE_LABEL(1), MIL.M_TRAINED_WIDTH_NOMINAL, NominalWidth)

            Console.Write("The template has been trained and is displayed in orange." + Constants.vbLf)
            Console.Write("Its nominal trained width is {0:#.##} pixels." + Constants.vbLf + Constants.vbLf, NominalWidth)

            ' Define model to further fixture the bead template.
            MIL.MmodAlloc(MilSystem, MIL.M_GEOMETRIC, MIL.M_DEFAULT, MilModelFinderContext)
            MIL.MmodAllocResult(MilSystem, MIL.M_DEFAULT, MilModelFinderResult)
            MIL.MmodDefine(MilModelFinderContext, MIL.M_IMAGE, MilImageTraining, MODEL_ORIGIN_X, MODEL_ORIGIN_Y, MODEL_SIZE_X, MODEL_SIZE_Y)

            ' Preprocess the model.
            MIL.MmodPreprocess(MilModelFinderContext, MIL.M_DEFAULT)

            ' Allocate a fixture object.
            MIL.McalAlloc(MilSystem, MIL.M_FIXTURING_OFFSET, MIL.M_DEFAULT, MilFixturingOffset)

            ' Learn the relative offset of the model.
            MIL.McalFixture(MIL.M_NULL, MilFixturingOffset, MIL.M_LEARN_OFFSET, MIL.M_MODEL_MOD, MilModelFinderContext, 0, MIL.M_DEFAULT, MIL.M_DEFAULT, MIL.M_DEFAULT)

            ' Display the model.
            MIL.MgraColor(MIL.M_DEFAULT, MODEL_FINDER_COLOR)
            MIL.MmodDraw(MIL.M_DEFAULT, MilModelFinderContext, MilGraList, MIL.M_DRAW_BOX + MIL.M_DRAW_POSITION, MIL.M_DEFAULT, MIL.M_ORIGINAL)

            Console.Write("A Model Finder model (in green) is also defined to" + Constants.vbLf)
            Console.Write("further fixture the bead verification operation." + Constants.vbLf)
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Clear the overlay annotation.
            MIL.MgraClear(MIL.M_DEFAULT, MilGraList)

            ' Display the target image buffer.
            MIL.MdispSelect(MIlDisplay, MilImageTarget)

            ' Find the location of the fixture using Model Finder.
            MIL.MmodFind(MilModelFinderContext, MilImageTarget, MilModelFinderResult)

            ' Display the found model occurrence.
            MIL.MgraColor(MIL.M_DEFAULT, MODEL_FINDER_COLOR)
            MIL.MmodDraw(MIL.M_DEFAULT, MilModelFinderResult, MilGraList, MIL.M_DRAW_BOX + MIL.M_DRAW_POSITION, MIL.M_DEFAULT, MIL.M_DEFAULT)

            ' Apply fixture offset to the target image.
            MIL.McalFixture(MilImageTarget, MilFixturingOffset, MIL.M_MOVE_RELATIVE, MIL.M_RESULT_MOD, MilModelFinderResult, 0, MIL.M_DEFAULT, MIL.M_DEFAULT, MIL.M_DEFAULT)

            ' Display the relative coordinate system.
            MIL.MgraColor(MIL.M_DEFAULT, COORDINATE_SYSTEM_COLOR)
            MIL.McalDraw(MIL.M_DEFAULT, MIL.M_NULL, MilGraList, MIL.M_DRAW_RELATIVE_COORDINATE_SYSTEM, MIL.M_DEFAULT, MIL.M_DEFAULT)

            ' Perform the inspection of the bead in the fixtured target image.
            MIL.MbeadVerify(MilBeadContext, MilImageTarget, MilBeadResult, MIL.M_DEFAULT)

            ' Display the result search boxes.
            MIL.MgraColor(MIL.M_DEFAULT, RESULT_SEARCH_BOX_COLOR)
            MIL.MbeadDraw(MIL.M_DEFAULT, MilBeadResult, MilGraList, MIL.M_DRAW_SEARCH_BOX, MIL.M_ALL, MIL.M_ALL, MIL.M_ALL, MIL.M_DEFAULT)

            Console.Write("The mechanical part's position and angle (in green) were ")
            Console.Write("located" + Constants.vbLf + "using Model Finder, and the bead's search boxes ")
            Console.Write("(in cyan) were" + Constants.vbLf + "positioned accordingly." + Constants.vbLf)
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Clear the overlay annotation.
            MIL.MgraClear(MIL.M_DEFAULT, MilGraList)

            ' Display the moved relative coordinate system.
            MIL.MgraColor(MIL.M_DEFAULT, COORDINATE_SYSTEM_COLOR)
            MIL.McalDraw(MIL.M_DEFAULT, MIL.M_NULL, MilGraList, MIL.M_DRAW_RELATIVE_COORDINATE_SYSTEM, MIL.M_DEFAULT, MIL.M_DEFAULT)

            ' Display the pass bead sections.
            MIL.MgraColor(MIL.M_DEFAULT, PASS_BEAD_WIDTH_COLOR)
            MIL.MbeadDraw(MIL.M_DEFAULT, MilBeadResult, MilGraList, MIL.M_DRAW_WIDTH, MIL.M_PASS, MIL.M_TEMPLATE_LABEL(1), MIL.M_ALL, MIL.M_DEFAULT)

            ' Display the missing bead sections.
            MIL.MgraColor(MIL.M_DEFAULT, FAIL_NOT_FOUND_COLOR)
            MIL.MbeadDraw(MIL.M_DEFAULT, MilBeadResult, MilGraList, MIL.M_DRAW_SEARCH_BOX, MIL.M_FAIL_NOT_FOUND, MIL.M_ALL, MIL.M_ALL, MIL.M_DEFAULT)

            ' Display bead sections which do not meet the minimum width criteria.
            MIL.MgraColor(MIL.M_DEFAULT, FAIL_SMALL_WIDTH_COLOR)
            MIL.MbeadDraw(MIL.M_DEFAULT, MilBeadResult, MilGraList, MIL.M_DRAW_SEARCH_BOX, MIL.M_FAIL_WIDTH_MIN, MIL.M_TEMPLATE_LABEL(1), MIL.M_ALL, MIL.M_DEFAULT)

            ' Retrieve and display general bead results.
            MIL.MbeadGetResult(MilBeadResult, MIL.M_TEMPLATE_LABEL(1), MIL.M_GENERAL, MIL.M_SCORE, Score)
            MIL.MbeadGetResult(MilBeadResult, MIL.M_TEMPLATE_LABEL(1), MIL.M_GENERAL, MIL.M_GAP_COVERAGE, GapCov)
            MIL.MbeadGetResult(MilBeadResult, MIL.M_TEMPLATE_LABEL(1), MIL.M_GENERAL, MIL.M_WIDTH_AVERAGE, AvWidth)
            MIL.MbeadGetResult(MilBeadResult, MIL.M_TEMPLATE_LABEL(1), MIL.M_GENERAL, MIL.M_GAP_MAX_LENGTH, MaxGap)

            Console.Write("The bead has been inspected:" + Constants.vbLf)
            Console.Write(" -Passing bead sections (green) cover {0:0.00}% of the bead" + Constants.vbLf, Score)
            Console.Write(" -Missing bead sections (red) cover {0:0.00}% of the bead" + Constants.vbLf, GapCov)
            Console.Write(" -Sections outside the specified width tolerances are drawn in orange" + Constants.vbLf)
            Console.Write(" -The bead's average width is {0:0.00} pixels" + Constants.vbLf, AvWidth)
            Console.Write(" -The bead's longest gap section is {0:0.00} pixels" + Constants.vbLf + Constants.vbLf, MaxGap)

            ' Pause to show the result.
            Console.Write("Press <Enter> to continue." + Constants.vbLf)
            Console.ReadKey()

            ' Free all allocations.
            MIL.MmodFree(MilModelFinderContext)
            MIL.MmodFree(MilModelFinderResult)
            MIL.MbeadFree(MilBeadContext)
            MIL.MbeadFree(MilBeadResult)
            MIL.McalFree(MilFixturingOffset)
            MIL.MbufFree(MilImageTraining)
            MIL.MbufFree(MilImageTarget)
            MIL.MgraFree(MilGraList)

        End Sub

        '*****************************************************************************
        ' Predefined 'edge' bead example.

        ' Target MIL image specifications.
        Private Shared ReadOnly CAP_FILE_TARGET As String = MIL.M_IMAGE_PATH + "Cap.mim"

        ' Template attributes definition.
        Private Const CIRCLE_CENTER_X As Double = 330.0
        Private Const CIRCLE_CENTER_Y As Double = 230.0
        Private Const CIRCLE_RADIUS As Double = 120.0

        ' Edge threshold value.
        Private Const EDGE_THRESHOLD_VALUE As Double = 25.0

        ' Max offset found and deviation tolerance.
        Private Const MAX_CONTOUR_DEVIATION_OFFSET As Double = 5.0
        Private Const MAX_CONTOUR_FOUND_OFFSET As Double = 25.0

        Private Shared Sub PredefinedBeadExample(ByVal MilSystem As MIL_ID, ByVal MIlDisplay As MIL_ID)

            Dim MilOverlayImage As MIL_ID = MIL.M_NULL       ' Overlay buffer identifier.
            Dim MilImageTarget As MIL_ID = MIL.M_NULL        ' Image buffer identifier.
            Dim MilBeadContext As MIL_ID = MIL.M_NULL        ' Bead context identifier.
            Dim MilBeadResult As MIL_ID = MIL.M_NULL         ' Bead result identifier.

            Dim MaximumOffset As Double = 0.0     ' Maximum offset result value.

            ' Restore target image into an automatically allocated image buffers.
            MIL.MbufRestore(CAP_FILE_TARGET, MilSystem, MilImageTarget)

            ' Display the training image buffer.
            MIL.MdispSelect(MIlDisplay, MilImageTarget)

            ' Prepare the overlay for annotations.
            MIL.MdispControl(MIlDisplay, MIL.M_OVERLAY, MIL.M_ENABLE)
            MIL.MdispInquire(MIlDisplay, MIL.M_OVERLAY_ID, MilOverlayImage)
            MIL.MdispControl(MIlDisplay, MIL.M_OVERLAY_CLEAR, MIL.M_TRANSPARENT_COLOR)

            ' Original template image.
            Console.Write(Constants.vbLf + "PREDEFINED BEAD INSPECTION:" + Constants.vbLf)
            Console.Write("---------------------------" + Constants.vbLf + Constants.vbLf)
            Console.Write("This program performs a bead inspection of a bottle" + Constants.vbLf)
            Console.Write("cap's contour using a predefined circular bead." + Constants.vbLf)
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Allocate a MIL bead context.
            MIL.MbeadAlloc(MilSystem, MIL.M_DEFAULT, MIL.M_DEFAULT, MilBeadContext)

            ' Allocate a MIL bead result.
            MIL.MbeadAllocResult(MilSystem, MIL.M_DEFAULT, MilBeadResult)

            ' Add the bead templates.
            MIL.MbeadTemplate(MilBeadContext, MIL.M_ADD, MIL.M_BEAD_EDGE, MIL.M_TEMPLATE_LABEL(1), 0, CType(MIL.M_NULL, IntPtr), CType(MIL.M_NULL, IntPtr), CType(MIL.M_NULL, IntPtr), MIL.M_DEFAULT)

            ' Set the bead shape properties.
            MIL.MbeadControl(MilBeadContext, MIL.M_TEMPLATE_LABEL(1), MIL.M_TRAINING_PATH, MIL.M_CIRCLE)

            MIL.MbeadControl(MilBeadContext, MIL.M_TEMPLATE_LABEL(1), MIL.M_TEMPLATE_CIRCLE_CENTER_X, CIRCLE_CENTER_X)
            MIL.MbeadControl(MilBeadContext, MIL.M_TEMPLATE_LABEL(1), MIL.M_TEMPLATE_CIRCLE_CENTER_Y, CIRCLE_CENTER_Y)
            MIL.MbeadControl(MilBeadContext, MIL.M_TEMPLATE_LABEL(1), MIL.M_TEMPLATE_CIRCLE_RADIUS, CIRCLE_RADIUS)

            ' Set the edge threshold value to extract the object shape.
            MIL.MbeadControl(MilBeadContext, MIL.M_TEMPLATE_LABEL(1), MIL.M_THRESHOLD_VALUE, EDGE_THRESHOLD_VALUE)

            ' Using the default fixed user defined nominal edge width.
            MIL.MbeadControl(MilBeadContext, MIL.M_ALL, MIL.M_WIDTH_NOMINAL_MODE, MIL.M_USER_DEFINED)

            ' Set the maximal expected contour deformation.
            MIL.MbeadControl(MilBeadContext, MIL.M_ALL, MIL.M_FOUND_OFFSET_MAX, MAX_CONTOUR_FOUND_OFFSET)

            ' Set the maximum valid bead deformation.
            MIL.MbeadControl(MilBeadContext, MIL.M_ALL, MIL.M_OFFSET_MAX, MAX_CONTOUR_DEVIATION_OFFSET)

            ' Display the bead in the overlay image.
            MIL.MgraColor(MIL.M_DEFAULT, USER_TEMPLATE_COLOR)
            MIL.MbeadDraw(MIL.M_DEFAULT, MilBeadContext, MilOverlayImage, MIL.M_DRAW_POSITION, MIL.M_USER, MIL.M_ALL, MIL.M_ALL, MIL.M_DEFAULT)

            ' The bead template is entirely defined and is trained without sample image.
            MIL.MbeadTrain(MilBeadContext, MIL.M_NULL, MIL.M_DEFAULT)

            ' Display the trained bead.
            MIL.MgraColor(MIL.M_DEFAULT, TRAINED_BEAD_WIDTH_COLOR)
            MIL.MbeadDraw(MIL.M_DEFAULT, MilBeadContext, MilOverlayImage, MIL.M_DRAW_SEARCH_BOX, MIL.M_TRAINED, MIL.M_ALL, MIL.M_ALL, MIL.M_DEFAULT)

            ' Pause to show the template image and user points.
            Console.Write("A circular template that was parametrically defined by the user" + Constants.vbLf)
            Console.Write("is displayed (in cyan). The template has been trained and the resulting" + Constants.vbLf)
            Console.Write("search is displayed (in orange)." + Constants.vbLf)
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Perform the inspection of the bead in the fixtured target image.
            MIL.MbeadVerify(MilBeadContext, MilImageTarget, MilBeadResult, MIL.M_DEFAULT)

            ' Clear the overlay annotation.
            MIL.MdispControl(MIlDisplay, MIL.M_OVERLAY_CLEAR, MIL.M_TRANSPARENT_COLOR)

            ' Display the pass bead sections.
            MIL.MgraColor(MIL.M_DEFAULT, PASS_BEAD_POSITION_COLOR)
            MIL.MbeadDraw(MIL.M_DEFAULT, MilBeadResult, MilOverlayImage, MIL.M_DRAW_POSITION, MIL.M_PASS, MIL.M_ALL, MIL.M_ALL, MIL.M_DEFAULT)

            ' Display the offset bead sections.
            MIL.MgraColor(MIL.M_DEFAULT, FAIL_EDGE_OFFSET_COLOR)
            MIL.MbeadDraw(MIL.M_DEFAULT, MilBeadResult, MilOverlayImage, MIL.M_DRAW_POSITION, MIL.M_FAIL_OFFSET, MIL.M_ALL, MIL.M_ALL, MIL.M_DEFAULT)

            ' Retrieve and display general bead results.
            MIL.MbeadGetResult(MilBeadResult, MIL.M_TEMPLATE_LABEL(1), MIL.M_GENERAL, MIL.M_OFFSET_MAX, MaximumOffset)

            Console.Write("The bottle cap shape has been inspected:" + Constants.vbLf)
            Console.Write(" -Sections outside the specified offset tolerance are drawn in red" + Constants.vbLf)
            Console.Write(" -The maximum offset value is {0:0.00} pixels." + Constants.vbLf + Constants.vbLf, MaximumOffset)

            ' Pause to show the result.
            Console.Write("Press <Enter> to terminate." + Constants.vbLf)
            Console.ReadKey()

            ' Free all allocations.
            MIL.MbeadFree(MilBeadContext)
            MIL.MbeadFree(MilBeadResult)
            MIL.MbufFree(MilImageTarget)





        End Sub

    End Class
End Namespace
