'*****************************************************************************
'
' File name: Mmeas.vb
'
' Synopsis:  This program consists of 3 examples that use the Measurement module 
'            to calculate the position, width and angle of objects in an image. 
'            The first one measures the position, width and angle of a stripe
'            in an image, and marks its center and edges. The second one measures
'            the average position, width and angle of a row of pins on a chip.
'            Finally the third example uses the fixturing capability to measure
'            the gap width of objects relative to the object's positions.
'*****************************************************************************
Imports Microsoft.VisualBasic
Imports System
Imports System.Collections.Generic
Imports System.Text

Imports Matrox.MatroxImagingLibrary

Namespace MMeas
    Friend Class Program
        ' Example selection.
        Private Const RUN_SINGLE_MEASUREMENT_EXAMPLE As Integer = MIL.M_YES
        Private Const RUN_MULTIPLE_MEASUREMENT_EXAMPLE As Integer = MIL.M_YES
        Private Const RUN_FIXTURED_MEASUREMENT_EXAMPLE As Integer = MIL.M_YES

        Shared Sub Main(ByVal args() As String)
            Dim MilApplication As MIL_ID = MIL.M_NULL ' Application identifier.
            Dim MilSystem As MIL_ID = MIL.M_NULL ' System Identifier.
            Dim MilDisplay As MIL_ID = MIL.M_NULL ' Display identifier.

            ' Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, MilApplication, MilSystem, MilDisplay, CType(MIL.M_NULL, IntPtr), CType(MIL.M_NULL, IntPtr))

            ' Print module name.
            Console.Write(Constants.vbLf + "MEASUREMENT MODULE:" + Constants.vbLf)
            Console.Write("-------------------" + Constants.vbLf + Constants.vbLf)

            If RUN_SINGLE_MEASUREMENT_EXAMPLE = MIL.M_YES Then
                SingleMeasurementExample(MilSystem, MilDisplay)
            End If

            If RUN_MULTIPLE_MEASUREMENT_EXAMPLE = MIL.M_YES Then
                MultipleMeasurementExample(MilSystem, MilDisplay)
            End If

            If RUN_FIXTURED_MEASUREMENT_EXAMPLE = MIL.M_YES Then
                FixturedMeasurementExample(MilSystem, MilDisplay)
            End If

            ' Free defaults.
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL)
        End Sub

        '****************************************************************************
        'Single measurement example. 
        '****************************************************************************

        ' Source MIL image file specification.
        Private Const MEAS_IMAGE_FILE As String = MIL.M_IMAGE_PATH & "lead.mim"

        ' Processing region specification.
        Private Const MEAS_BOX_WIDTH As Integer = 128
        Private Const MEAS_BOX_HEIGHT As Integer = 100
        Private Const MEAS_BOX_POS_X As Integer = 166
        Private Const MEAS_BOX_POS_Y As Integer = 130

        ' Target stripe typical specifications.
        Private Const STRIPE_POLARITY_LEFT As Integer = MIL.M_POSITIVE
        Private Const STRIPE_POLARITY_RIGHT As Integer = MIL.M_NEGATIVE
        Private Const STRIPE_WIDTH As Integer = 45
        Private Const STRIPE_WIDTH_VARIATION As Integer = 10

        Private Shared Sub SingleMeasurementExample(ByVal MilSystem As MIL_ID, ByVal MilDisplay As MIL_ID)
            Dim MilImage As MIL_ID = MIL.M_NULL ' Image buffer identifier.
            Dim MilGraphicList As MIL_ID = MIL.M_NULL ' Graphic list identifier.
            Dim StripeMarker As MIL_ID = MIL.M_NULL ' Stripe marker identifier.
            Dim StripeCenterX As Double = 0.0 ' Stripe X center position.
            Dim StripeCenterY As Double = 0.0 ' Stripe Y center position.
            Dim StripeWidth As Double = 0.0 ' Stripe width.
            Dim StripeAngle As Double = 0.0 ' Stripe angle.
            Dim CrossColor As Double = MIL.M_COLOR_YELLOW ' Cross drawing color.
            Dim BoxColor As Double = MIL.M_COLOR_RED ' Box drawing color.

            ' Restore and display the source image.
            MIL.MbufRestore(MEAS_IMAGE_FILE, MilSystem, MilImage)
            MIL.MdispSelect(MilDisplay, MilImage)

            ' Allocate a graphic list to hold the subpixel annotations to draw.
            MIL.MgraAllocList(MilSystem, MIL.M_DEFAULT, MilGraphicList)

            ' Associate the graphic list to the display.
            MIL.MdispControl(MilDisplay, MIL.M_ASSOCIATED_GRAPHIC_LIST_ID, MilGraphicList)

            ' Allocate a stripe marker.
            MIL.MmeasAllocMarker(MilSystem, MIL.M_STRIPE, MIL.M_DEFAULT, StripeMarker)

            ' Specify the stripe characteristics.
            MIL.MmeasSetMarker(StripeMarker, MIL.M_POLARITY, STRIPE_POLARITY_LEFT, STRIPE_POLARITY_RIGHT)

            MIL.MmeasSetScore(StripeMarker, _
                              MIL.M_STRIPE_WIDTH_SCORE, _
                              STRIPE_WIDTH - STRIPE_WIDTH_VARIATION, _
                              STRIPE_WIDTH - STRIPE_WIDTH_VARIATION, _
                              STRIPE_WIDTH + STRIPE_WIDTH_VARIATION, _
                              STRIPE_WIDTH + STRIPE_WIDTH_VARIATION, _
                              MIL.M_DEFAULT, _
                              MIL.M_DEFAULT, _
                              MIL.M_DEFAULT)

            MIL.MmeasSetMarker(StripeMarker, MIL.M_BOX_ANGLE_MODE, MIL.M_ENABLE, MIL.M_NULL)

            ' Specify the search region size and position.
            MIL.MmeasSetMarker(StripeMarker, MIL.M_BOX_ORIGIN, MEAS_BOX_POS_X, MEAS_BOX_POS_Y)
            MIL.MmeasSetMarker(StripeMarker, MIL.M_BOX_SIZE, MEAS_BOX_WIDTH, MEAS_BOX_HEIGHT)

            ' Draw the contour of the measurement region.
            MIL.MgraColor(MIL.M_DEFAULT, BoxColor)
            MIL.MmeasDraw(MIL.M_DEFAULT, StripeMarker, MilGraphicList, MIL.M_DRAW_SEARCH_REGION, MIL.M_DEFAULT, MIL.M_MARKER)

            ' Pause to show the original image.
            Console.Write("Position, width and angle of the stripe in the highlighted box" + Constants.vbLf)
            Console.Write("will be calculated and the center and edges will be marked." + Constants.vbLf)
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Clear the annotations.
            MIL.MgraClear(MIL.M_DEFAULT, MilGraphicList)

            ' Find the stripe and measure its width and angle.
            MIL.MmeasFindMarker(MIL.M_DEFAULT, MilImage, StripeMarker, MIL.M_DEFAULT)

            ' Get the stripe position, width and angle.
            MIL.MmeasGetResult(StripeMarker, MIL.M_POSITION, StripeCenterX, StripeCenterY)
            MIL.MmeasGetResult(StripeMarker, MIL.M_STRIPE_WIDTH, StripeWidth)
            MIL.MmeasGetResult(StripeMarker, MIL.M_ANGLE, StripeAngle)

            ' Draw the contour of the found measurement region.
            MIL.MgraColor(MIL.M_DEFAULT, BoxColor)
            MIL.MmeasDraw(MIL.M_DEFAULT, StripeMarker, MilGraphicList, MIL.M_DRAW_SEARCH_REGION, MIL.M_DEFAULT, MIL.M_RESULT)

            ' Draw a cross on the center, left edge and right edge of the found stripe.
            MIL.MgraColor(MIL.M_DEFAULT, CrossColor)
            MIL.MmeasDraw(MIL.M_DEFAULT, StripeMarker, MilGraphicList, MIL.M_DRAW_POSITION, MIL.M_DEFAULT, MIL.M_RESULT)
            MIL.MmeasDraw(MIL.M_DEFAULT, StripeMarker, MilGraphicList, MIL.M_DRAW_POSITION + MIL.M_EDGE_FIRST + MIL.M_EDGE_SECOND, MIL.M_DEFAULT, MIL.M_RESULT)

            ' Print the result.
            Console.Write("The stripe in the image is at position {0:0.00},{1:0.00} and" + Constants.vbLf, StripeCenterX, StripeCenterY)
            Console.Write("is {0:0.00} pixels wide with an angle of {1:0.00} degrees." + Constants.vbLf, StripeWidth, StripeAngle)
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Remove the graphic list association to the display.
            MIL.MdispControl(MilDisplay, MIL.M_ASSOCIATED_GRAPHIC_LIST_ID, MIL.M_NULL)

            ' Free all allocations.
            MIL.MgraFree(MilGraphicList)
            MIL.MmeasFree(StripeMarker)
            MIL.MbufFree(MilImage)
        End Sub

        '****************************************************************************
        ' Multiple measurement example. 
        '****************************************************************************

        ' Source MIL image file specification.
        Private Const MULT_MEAS_IMAGE_FILE As String = MIL.M_IMAGE_PATH & "chip.mim"

        ' Processing region specification.
        Private Const MULT_MEAS_BOX_WIDTH As Integer = 230
        Private Const MULT_MEAS_BOX_HEIGHT As Integer = 7
        Private Const MULT_MEAS_BOX_POS_X As Integer = 220
        Private Const MULT_MEAS_BOX_POS_Y As Integer = 171

        ' Target stripe specifications.
        Private Const MULT_STRIPES_ORIENTATION As Integer = MIL.M_VERTICAL
        Private Const MULT_STRIPES_POLARITY_LEFT As Integer = MIL.M_POSITIVE
        Private Const MULT_STRIPES_POLARITY_RIGHT As Integer = MIL.M_NEGATIVE
        Private Const MULT_STRIPES_NUMBER As Integer = 12

        Private Shared Sub MultipleMeasurementExample(ByVal MilSystem As MIL_ID, ByVal MilDisplay As MIL_ID)
            Dim MilImage As MIL_ID = MIL.M_NULL ' Image buffer identifier.
            Dim MilGraphicList As MIL_ID = MIL.M_NULL ' Graphic list identifier.
            Dim StripeMarker As MIL_ID = MIL.M_NULL ' Stripe marker identifier.
            Dim MeanAngle As Double = 0.0 ' Stripe mean angle.
            Dim MeanWidth As Double = 0.0 ' Stripe mean width.
            Dim MeanSpacing As Double = 0.0 ' Stripe mean spacing.
            Dim CrossColor As Double = MIL.M_COLOR_YELLOW ' Cross drawing color.
            Dim BoxColor As Double = MIL.M_COLOR_RED ' Box drawing color.

            ' Restore and display the source image.
            MIL.MbufRestore(MULT_MEAS_IMAGE_FILE, MilSystem, MilImage)
            MIL.MdispSelect(MilDisplay, MilImage)

            ' Allocate a graphic list to hold the subpixel annotations to draw.
            MIL.MgraAllocList(MilSystem, MIL.M_DEFAULT, MilGraphicList)

            ' Associate the graphic list to the display.
            MIL.MdispControl(MilDisplay, MIL.M_ASSOCIATED_GRAPHIC_LIST_ID, MilGraphicList)

            ' Allocate a stripe marker.
            MIL.MmeasAllocMarker(MilSystem, MIL.M_STRIPE, MIL.M_DEFAULT, StripeMarker)

            ' Specify the stripe characteristics.
            MIL.MmeasSetMarker(StripeMarker, MIL.M_NUMBER, MULT_STRIPES_NUMBER, MIL.M_NULL)
            MIL.MmeasSetMarker(StripeMarker, MIL.M_POLARITY, MULT_STRIPES_POLARITY_LEFT, MULT_STRIPES_POLARITY_RIGHT)
            MIL.MmeasSetMarker(StripeMarker, MIL.M_ORIENTATION, MULT_STRIPES_ORIENTATION, MIL.M_NULL)

            ' Specify the measurement box size and position.
            MIL.MmeasSetMarker(StripeMarker, MIL.M_BOX_ORIGIN, MULT_MEAS_BOX_POS_X, MULT_MEAS_BOX_POS_Y)
            MIL.MmeasSetMarker(StripeMarker, MIL.M_BOX_SIZE, MULT_MEAS_BOX_WIDTH, MULT_MEAS_BOX_HEIGHT)

            ' Draw the contour of the measurement region.
            MIL.MgraColor(MIL.M_DEFAULT, BoxColor)
            MIL.MmeasDraw(MIL.M_DEFAULT, StripeMarker, MilGraphicList, MIL.M_DRAW_SEARCH_REGION, MIL.M_DEFAULT, MIL.M_MARKER)

            ' Pause to show the original image.
            Console.Write("The position and angle of a row of pins on a chip will be calculated." + Constants.vbLf)
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Find the stripe and measure its width and angle.
            MIL.MmeasFindMarker(MIL.M_DEFAULT, MilImage, StripeMarker, MIL.M_POSITION + MIL.M_ANGLE + MIL.M_STRIPE_WIDTH)

            ' Draw the contour of the measurement region.
            MIL.MgraColor(MIL.M_DEFAULT, BoxColor)
            MIL.MmeasDraw(MIL.M_DEFAULT, StripeMarker, MilGraphicList, MIL.M_DRAW_SEARCH_REGION, MIL.M_DEFAULT, MIL.M_RESULT)

            ' Draw a cross at the center of each stripe found.
            MIL.MgraColor(MIL.M_DEFAULT, CrossColor)
            MIL.MmeasDraw(MIL.M_DEFAULT, StripeMarker, MilGraphicList, MIL.M_DRAW_POSITION, MIL.M_ALL, MIL.M_RESULT)

            ' Get the stripe's width, angle and spacing.
            MIL.MmeasGetResult(StripeMarker, MIL.M_ANGLE + MIL.M_MEAN, MeanAngle)
            MIL.MmeasGetResult(StripeMarker, MIL.M_STRIPE_WIDTH + MIL.M_MEAN, MeanWidth)
            MIL.MmeasGetResult(StripeMarker, MIL.M_SPACING + MIL.M_MEAN, MeanSpacing)

            ' Print the results.
            Console.Write("The center and angle of each pin have been marked." + Constants.vbLf + Constants.vbLf)
            Console.Write("The statistics for the pins are:" + Constants.vbLf)
            Console.Write("Average angle   : {0,5:0.00}" + Constants.vbLf, MeanAngle)
            Console.Write("Average width   : {0,5:0.00}" + Constants.vbLf, MeanWidth)
            Console.Write("Average spacing : {0,5:0.00}" + Constants.vbLf, MeanSpacing)
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Remove the graphic list association to the display.
            MIL.MdispControl(MilDisplay, MIL.M_ASSOCIATED_GRAPHIC_LIST_ID, MIL.M_NULL)

            ' Free all allocations.
            MIL.MgraFree(MilGraphicList)
            MIL.MmeasFree(StripeMarker)
            MIL.MbufFree(MilImage)
        End Sub

        '*****************************************************************************
        ' Fixtured measurement example. 
        '*****************************************************************************

        ' Source MIL image file specification.
        Private Const FIXTURED_MEAS_IMAGE_FILE As String = MIL.M_IMAGE_PATH + "Fuse.mim"

        ' Processing region specification.
        Private Const FIXTURED_MEAS_BOX_OFFSET_X As Integer = 400
        Private Const FIXTURED_MEAS_BOX_OFFSET_Y As Integer = 290
        Private Const FIXTURED_MEAS_BOX_WIDTH As Integer = 100
        Private Const FIXTURED_MEAS_BOX_HEIGHT As Integer = 15

        ' Model region specification.
        Private Const FIXTURED_MODEL_OFFSET_X As Integer = 395
        Private Const FIXTURED_MODEL_OFFSET_Y As Integer = 200
        Private Const FIXTURED_MODEL_SIZE_X As Integer = 110
        Private Const FIXTURED_MODEL_SIZE_Y As Integer = 120

        Private Const FIXTURED_IMAGE_SIZE_X As Integer = 512
        Private Const FIXTURED_IMAGE_SIZE_Y As Integer = 384

        ' Target stripe typical specifications.
        Private Const FIXTURED_STRIPE_POLARITY_LEFT As Integer = MIL.M_POSITIVE
        Private Const FIXTURED_STRIPE_POLARITY_RIGHT As Integer = MIL.M_OPPOSITE

        Private Shared Sub FixturedMeasurementExample(ByVal MilSystem As MIL_ID, ByVal MilDisplay As MIL_ID)

            Dim MilSourceImage As MIL_ID = MIL.M_NULL             ' Source image buffer identifier.
            Dim MilImage As MIL_ID = MIL.M_NULL                   ' Image buffer identifier.
            Dim MilModContext As MIL_ID = MIL.M_NULL              ' Model finder context identifier.
            Dim MilModResult As MIL_ID = MIL.M_NULL               ' Model finder result identifier.
            Dim MilFixturingOffset As MIL_ID = MIL.M_NULL         ' Fixturing object identifier.
            Dim StripeMarker As MIL_ID = MIL.M_NULL               ' Stripe marker identifier.
            Dim MilGraphicList As MIL_ID = MIL.M_NULL             ' Graphic list identifier.

            Dim StripeWidth As Double = 0.0                       ' Stripe width.
            Dim PositionX As Double = 0.0                         ' Occurence position X.
            Dim PositionY As Double = 0.0                         ' Occurence position Y.

            Dim SizeX As MIL_INT = 0                              ' Source image size X.
            Dim SizeY As MIL_INT = 0                              ' Source image size Y.
            Dim NbOccurrences As MIL_INT = 0                      ' Number of found occurrences.
            Dim NbStripes As MIL_INT = 0                          ' Number of found stripes.
            Dim Index As MIL_INT = 0                              ' Occurrence index.

            ' Restore the source image.
            MIL.MbufRestore(FIXTURED_MEAS_IMAGE_FILE, MilSystem, MilSourceImage)
            MIL.MbufInquire(MilSourceImage, MIL.M_SIZE_X, SizeX)
            MIL.MbufInquire(MilSourceImage, MIL.M_SIZE_Y, SizeY)

            ' Allocate, then compute the source image luminance.
            MIL.MbufAlloc2d(MilSystem, SizeX, SizeY, 8 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_PROC + MIL.M_DISP, MilImage)
            MIL.MimConvert(MilSourceImage, MilImage, MIL.M_RGB_TO_L)

            ' Allocate a graphic list to hold the subpixel annotations to draw.
            MIL.MgraAllocList(MilSystem, MIL.M_DEFAULT, MilGraphicList)

            ' Select the image to the display.
            MIL.MdispSelect(MilDisplay, MilImage)

            ' Associate the graphic list to the display.
            MIL.MdispControl(MilDisplay, MIL.M_ASSOCIATED_GRAPHIC_LIST_ID, MilGraphicList)

            ' Allocate a stripe marker.
            MIL.MmeasAllocMarker(MilSystem, MIL.M_STRIPE, MIL.M_DEFAULT, StripeMarker)

            ' Set inputs units to world in order to fixture the region.
            MIL.MmeasSetMarker(StripeMarker, MIL.M_SEARCH_REGION_INPUT_UNITS, MIL.M_WORLD, MIL.M_NULL)

            ' Calibrate the destination image to receive the world units annotations.
            MIL.McalUniform(MilImage, 0.0, 0.0, 1.0, 1.0, 0.0, MIL.M_DEFAULT)

            ' Specify the stripe characteristics.
            MIL.MmeasSetMarker(StripeMarker, MIL.M_BOX_ORIGIN, FIXTURED_MEAS_BOX_OFFSET_X, FIXTURED_MEAS_BOX_OFFSET_Y)
            MIL.MmeasSetMarker(StripeMarker, MIL.M_POLARITY, FIXTURED_STRIPE_POLARITY_LEFT, FIXTURED_STRIPE_POLARITY_RIGHT)
            MIL.MmeasSetMarker(StripeMarker, MIL.M_SEARCH_REGION_CLIPPING, MIL.M_ENABLE, MIL.M_NULL)

            ' Specify the search region size and position.
            MIL.MmeasSetMarker(StripeMarker, MIL.M_BOX_SIZE, FIXTURED_MEAS_BOX_WIDTH, FIXTURED_MEAS_BOX_HEIGHT)

            ' Draw the contour of the measurement region.
            MIL.MgraColor(MIL.M_DEFAULT, MIL.M_COLOR_BLUE)
            MIL.MgraControl(MIL.M_DEFAULT, MIL.M_INPUT_UNITS, MIL.M_WORLD)
            MIL.MmeasDraw(MIL.M_DEFAULT, StripeMarker, MilGraphicList, MIL.M_DRAW_SEARCH_REGION, MIL.M_DEFAULT, MIL.M_MARKER)
            MIL.MgraControl(MIL.M_DEFAULT, MIL.M_INPUT_UNITS, MIL.M_PIXEL)

            Console.Write("A measurement stripe marker (in blue) is defined." + Constants.vbLf)

            ' Define model to further fixture the measurement marker.
            MIL.MmodAlloc(MilSystem, MIL.M_GEOMETRIC, MIL.M_DEFAULT, MilModContext)

            MIL.MmodAllocResult(MilSystem, MIL.M_DEFAULT, MilModResult)

            MIL.MmodDefine(MilModContext, MIL.M_IMAGE, MilImage, FIXTURED_MODEL_OFFSET_X, FIXTURED_MODEL_OFFSET_Y, FIXTURED_MODEL_SIZE_X, FIXTURED_MODEL_SIZE_Y)
            MIL.MmodControl(MilModContext, MIL.M_DEFAULT, MIL.M_NUMBER, MIL.M_ALL)
            MIL.MmodControl(MilModContext, MIL.M_CONTEXT, MIL.M_SPEED, MIL.M_VERY_HIGH)

            ' Preprocess the model.
            MIL.MmodPreprocess(MilModContext, MIL.M_DEFAULT)

            ' Display the model.
            MIL.MgraColor(MIL.M_DEFAULT, MIL.M_COLOR_GREEN)
            MIL.MmodDraw(MIL.M_DEFAULT, MilModContext, MilGraphicList, MIL.M_DRAW_BOX + MIL.M_DRAW_POSITION, MIL.M_DEFAULT, MIL.M_ORIGINAL)
            Console.Write("A Model Finder model (in green) is defined to" + Constants.vbLf)
            Console.Write("further fixture the measurement operation." + Constants.vbLf + Constants.vbLf)

            Console.Write("The stripe marker determines the gap between" + Constants.vbLf)
            Console.Write("the fuse connectors. Model Finder tracks the" + Constants.vbLf)
            Console.Write("fuses while the attached fixturing automatically" + Constants.vbLf)
            Console.Write("relocates the marker relative to the found fuses." + Constants.vbLf)

            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Allocate a fixture object.
            MIL.McalAlloc(MilSystem, MIL.M_FIXTURING_OFFSET, MIL.M_DEFAULT, MilFixturingOffset)

            ' Learn the relative offset of the model.
            MIL.McalFixture(MIL.M_NULL, MilFixturingOffset, MIL.M_LEARN_OFFSET, MIL.M_MODEL_MOD, MilModContext, 0, MIL.M_DEFAULT, MIL.M_DEFAULT, MIL.M_DEFAULT)

            ' Find the location of the fixtures using Model Finder.
            MIL.MmodFind(MilModContext, MilImage, MilModResult)

            ' Display and retrieve the number of occurrences found.
            MIL.MgraClear(MIL.M_DEFAULT, MilGraphicList)
            MIL.MmodDraw(MIL.M_DEFAULT, MilModResult, MilGraphicList, MIL.M_DRAW_POSITION + MIL.M_DRAW_BOX, MIL.M_DEFAULT, MIL.M_DEFAULT)

            MIL.MmodGetResult(MilModResult, MIL.M_DEFAULT, MIL.M_NUMBER + MIL.M_TYPE_MIL_INT, NbOccurrences)

            Console.Write("Locating the parts: {0} occurrences are found." + Constants.vbLf, NbOccurrences)
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            Console.Write("The measurement tool is moved relative to each piece." + Constants.vbLf)
            Console.Write("A graphic list is used to display the results with" + Constants.vbLf)
            Console.Write("subpixel annotations." + Constants.vbLf + Constants.vbLf)

            ' Clear the annotations.
            MIL.MgraClear(MIL.M_DEFAULT, MilGraphicList)

            For Index = 0 To NbOccurrences - 1
                ' Display the found model occurrence position.
                MIL.MgraColor(MIL.M_DEFAULT, MIL.M_COLOR_GREEN)
                MIL.MmodDraw(MIL.M_DEFAULT, MilModResult, MilGraphicList, MIL.M_DRAW_POSITION, Index, MIL.M_DEFAULT)

                MIL.MmodGetResult(MilModResult, Index, MIL.M_POSITION_X + MIL.M_TYPE_MIL_DOUBLE, PositionX)
                MIL.MmodGetResult(MilModResult, Index, MIL.M_POSITION_Y + MIL.M_TYPE_MIL_DOUBLE, PositionY)

                MIL.MgraText(MIL.M_DEFAULT, MilGraphicList, PositionX - 20, PositionY, Index.ToString())


                ' Apply a fixture offset to the implicit 1:1 calibration of the target image.
                MIL.McalFixture(MilImage, MilFixturingOffset, MIL.M_MOVE_RELATIVE, MIL.M_RESULT_MOD, MilModResult, Index, MIL.M_DEFAULT, MIL.M_DEFAULT, MIL.M_DEFAULT)

                ' Find the stripe and measure its width and angle.
                MIL.MmeasFindMarker(MIL.M_DEFAULT, MilImage, StripeMarker, MIL.M_POSITION + MIL.M_STRIPE_WIDTH)

                ' Get the number of found results.
                MIL.MmeasGetResult(StripeMarker, MIL.M_NUMBER + MIL.M_TYPE_MIL_INT, NbStripes, CType(MIL.M_NULL, IntPtr))

                If (NbStripes = 1) Then

                    ' Get the stripe width.
                    MIL.MmeasGetResult(StripeMarker, MIL.M_STRIPE_WIDTH, StripeWidth, CType(MIL.M_NULL, IntPtr))

                    ' Draw the contour of the found measurement region.
                    MIL.MgraColor(MIL.M_DEFAULT, MIL.M_COLOR_BLUE)
                    MIL.MmeasDraw(MIL.M_DEFAULT, StripeMarker, MilGraphicList, MIL.M_DRAW_SEARCH_REGION, MIL.M_DEFAULT, MIL.M_RESULT)

                    ' Draw a cross on the center, left edge and right edge of the found stripe.
                    MIL.MgraColor(MIL.M_DEFAULT, MIL.M_COLOR_RED)
                    MIL.MmeasDraw(MIL.M_DEFAULT, StripeMarker, MilGraphicList, MIL.M_DRAW_WIDTH, MIL.M_DEFAULT, MIL.M_RESULT)

                    ' Print the result.
                    Console.Write("The gap (in red) of occurrence {0} is {1:#.##} pixels wide." + Constants.vbLf, Index, StripeWidth)

                Else

                    Console.Write("The gap of occurrence {0} could not be measured." + Constants.vbLf, Index)
                End If
            Next

            Console.Write("Press <Enter> to end." + Constants.vbLf)
            Console.ReadKey()

            ' Free all allocations.
            MIL.MgraFree(MilGraphicList)
            MIL.MmeasFree(StripeMarker)
            MIL.MmodFree(MilModContext)
            MIL.MmodFree(MilModResult)
            MIL.McalFree(MilFixturingOffset)
            MIL.MbufFree(MilImage)
            MIL.MbufFree(MilSourceImage)
        End Sub
    End Class
End Namespace
