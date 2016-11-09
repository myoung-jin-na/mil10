'**************************************************************************************
'
' File name: Mcol.vb
'
' Synopsis:  This program contains 3 examples of the color module usage:
'
'           The first example performs color segmentation of an image
'            by classifying each pixel with one out of 7 color samples.
'            The ratio of each color in the image is then calculated.
'
'            The second example performs color matching of circular regions
'            in objects located with model finder.
'
'            The third example performs color separation in order to
'            separate 2 types of ink on a piece of paper.
'**************************************************************************************
Imports Microsoft.VisualBasic
Imports System
Imports System.Collections.Generic
Imports System.Text
Imports Matrox.MatroxImagingLibrary

Namespace MCol
    Friend Class Program
        ' Display image margin
        Private Const DISPLAY_CENTER_MARGIN_X As Integer = 5

        ' Color patch sizes
        Private Const COLOR_PATCH_SIZEX As Integer = 30
        Private Const COLOR_PATCH_SIZEY As Integer = 40

        Shared Sub Main(ByVal args() As String)
            '****************************************************************************
            ' Main.
            '****************************************************************************

            Dim MilApplication As MIL_ID = MIL.M_NULL ' Application identifier.
            Dim MilSystem As MIL_ID = MIL.M_NULL ' System identifier.
            Dim MilDisplay As MIL_ID = MIL.M_NULL ' Display identifier.

            ' Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, MilApplication, MilSystem, MilDisplay, CType(MIL.M_NULL, IntPtr), CType(MIL.M_NULL, IntPtr))

            ' Run the color segmentation example.
            ColorSegmentationExample(MilSystem, MilDisplay)

            ' Run the color matching example.
            ColorMatchingExample(MilSystem, MilDisplay)

            ' Run the color projection example.
            ColorSeparationExample(MilSystem, MilDisplay)

            ' Free defaults.
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL)
        End Sub

        '***************************************************************************
        ' Color Segmentation using color samples. 
        '***************************************************************************

        ' Image filenames
        Private Const CANDY_SAMPLE_IMAGE_FILE As String = MIL.M_IMAGE_PATH & "CandySamples.mim"
        Private Const CANDY_TARGET_IMAGE_FILE As String = MIL.M_IMAGE_PATH & "Candy.mim"

        ' Number of samples
        Private Const NUM_SAMPLES As Integer = 6

        ' Draw spacing and offset
        Private Const CANDY_SAMPLES_XSPACING As Integer = 35
        Private Const CANDY_SAMPLES_YOFFSET As Integer = 145

        ' Match parameters
        Private Const MATCH_MODE As Integer = MIL.M_MIN_DIST_VOTE ' Minimal distance vote mode.
        Private Const DISTANCE_TYPE As Integer = MIL.M_MAHALANOBIS ' Mahalanobis distance.
        Private Const TOLERANCE_MODE As Integer = MIL.M_SAMPLE_STDDEV ' Standard deviation tolerance mode.
        Private Const TOLERANCE_VALUE As Double = 6.0 ' Mahalanobis tolerance value.
        Private Const RED_TOLERANCE_VALUE As Double = 6.0
        Private Const YELLOW_TOLERANCE_VALUE As Double = 12.0
        Private Const PINK_TOLERANCE_VALUE As Double = 5.0

        Private Shared Sub ColorSegmentationExample(ByVal MilSystem As MIL_ID, ByVal MilDisplay As MIL_ID)
            Dim SourceChild As MIL_ID = MIL.M_NULL ' Source image buffer identifier.
            Dim DestChild As MIL_ID = MIL.M_NULL ' Dest image buffer identifier.
            Dim MatchContext As MIL_ID = MIL.M_NULL ' Color matching context identifier.
            Dim MatchResult As MIL_ID = MIL.M_NULL ' Color matching result identifier.
            Dim DisplayImage As MIL_ID = MIL.M_NULL ' Display image buffer identifier.

            Dim SourceSizeX As MIL_INT = 0
            Dim SourceSizeY As MIL_INT = 0 ' Source image sizes
            Dim SampleIndex As MIL_INT = 0
            Dim SpacesIndex As MIL_INT = 0 ' Indices

            Dim MatchScore As Double = 0.0 ' Color matching score.

            ' Blank spaces to align the samples names evenly.
            Dim Spaces() As String = {"", " ", "  ", "   "}

            ' Color samples names.
            Dim SampleNames() As String = {"Green", "Red", "Yellow", "Purple", "Blue", "Pink"}

            ' Color samples position: {OffsetX, OffsetY} 
            Dim SamplesROI(,) As Double = {{58, 143}, {136, 148}, {217, 144}, {295, 142}, {367, 143}, {442, 147}}

            ' Color samples size.
            Const SampleSizeX As Double = 36, SampleSizeY As Double = 32

            ' Array for match sample colors.
            Dim SampleMatchColor(NUM_SAMPLES - 1, 2) As MIL_INT

            Console.Write(Constants.vbLf + "COLOR SEGMENTATION:" + Constants.vbLf)
            Console.Write("-------------------" + Constants.vbLf)

            ' Allocate the parent display image.
            MIL.MbufDiskInquire(CANDY_SAMPLE_IMAGE_FILE, MIL.M_SIZE_X, SourceSizeX)
            MIL.MbufDiskInquire(CANDY_SAMPLE_IMAGE_FILE, MIL.M_SIZE_Y, SourceSizeY)
            MIL.MbufAllocColor(MilSystem, 3, 2 * SourceSizeX + DISPLAY_CENTER_MARGIN_X, SourceSizeY, 8 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_DISP + MIL.M_PROC, DisplayImage)
            MIL.MbufClear(DisplayImage, MIL.M_COLOR_BLACK)

            ' Create a source and dest child in the display image.
            MIL.MbufChild2d(DisplayImage, 0, 0, SourceSizeX, SourceSizeY, SourceChild)
            MIL.MbufChild2d(DisplayImage, SourceSizeX + DISPLAY_CENTER_MARGIN_X, 0, SourceSizeX, SourceSizeY, DestChild)

            ' Load the source image into the source child.
            MIL.MbufLoad(CANDY_SAMPLE_IMAGE_FILE, SourceChild)

            ' Allocate a color matching context.
            MIL.McolAlloc(MilSystem, MIL.M_COLOR_MATCHING, MIL.M_RGB, MIL.M_DEFAULT, MIL.M_DEFAULT, MatchContext)

            ' Define each color sample in the context.
            For i As Integer = 0 To NUM_SAMPLES - 1
                MIL.McolDefine(MatchContext, SourceChild, MIL.M_SAMPLE_LABEL(i + 1), MIL.M_IMAGE, SamplesROI(i, 0), SamplesROI(i, 1), SampleSizeX, SampleSizeY)
            Next i

            ' Set the color matching parameters.
            MIL.McolSetMethod(MatchContext, MATCH_MODE, DISTANCE_TYPE, MIL.M_DEFAULT, MIL.M_DEFAULT)
            MIL.McolControl(MatchContext, MIL.M_CONTEXT, MIL.M_DISTANCE_TOLERANCE_MODE, TOLERANCE_MODE)
            MIL.McolControl(MatchContext, MIL.M_ALL, MIL.M_DISTANCE_TOLERANCE, TOLERANCE_VALUE)

            ' Adjust tolerances for the red, yellow and pink samples.
            MIL.McolControl(MatchContext, MIL.M_SAMPLE_INDEX(1), MIL.M_DISTANCE_TOLERANCE, RED_TOLERANCE_VALUE)
            MIL.McolControl(MatchContext, MIL.M_SAMPLE_INDEX(2), MIL.M_DISTANCE_TOLERANCE, YELLOW_TOLERANCE_VALUE)
            MIL.McolControl(MatchContext, MIL.M_SAMPLE_INDEX(5), MIL.M_DISTANCE_TOLERANCE, PINK_TOLERANCE_VALUE)

            ' Preprocess the context.
            MIL.McolPreprocess(MatchContext, MIL.M_DEFAULT)

            ' Fill the samples colors array.
            For i As Integer = 0 To NUM_SAMPLES - 1
                MIL.McolInquire(MatchContext, MIL.M_SAMPLE_LABEL(i + 1), MIL.M_MATCH_SAMPLE_COLOR_BAND_0 + MIL.M_TYPE_MIL_INT, SampleMatchColor(i, 0))
                MIL.McolInquire(MatchContext, MIL.M_SAMPLE_LABEL(i + 1), MIL.M_MATCH_SAMPLE_COLOR_BAND_1 + MIL.M_TYPE_MIL_INT, SampleMatchColor(i, 1))
                MIL.McolInquire(MatchContext, MIL.M_SAMPLE_LABEL(i + 1), MIL.M_MATCH_SAMPLE_COLOR_BAND_2 + MIL.M_TYPE_MIL_INT, SampleMatchColor(i, 2))
            Next i

            ' Draw the samples.
            DrawSampleColors(DestChild, SampleMatchColor, SampleNames, NUM_SAMPLES, CANDY_SAMPLES_XSPACING, CANDY_SAMPLES_YOFFSET)

            ' Select the image buffer for display.
            MIL.MdispSelect(MilDisplay, DisplayImage)

            ' Pause to show the original image.
            Console.Write("Color samples are defined for each possible candy color." + Constants.vbLf)
            Console.Write("Press <Enter> to do color matching." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Load the target image.*/
            MIL.MbufClear(DisplayImage, MIL.M_COLOR_BLACK)
            MIL.MbufLoad(CANDY_TARGET_IMAGE_FILE, SourceChild)

            ' Allocate a color matching result buffer.
            MIL.McolAllocResult(MilSystem, MIL.M_COLOR_MATCHING_RESULT, MIL.M_DEFAULT, MatchResult)

            ' Enable controls to draw the labeled color image.
            MIL.McolControl(MatchContext, MIL.M_CONTEXT, MIL.M_GENERATE_PIXEL_MATCH, MIL.M_ENABLE)
            MIL.McolControl(MatchContext, MIL.M_CONTEXT, MIL.M_GENERATE_SAMPLE_COLOR_LUT, MIL.M_ENABLE)

            ' Match with target image.
            MIL.McolMatch(MatchContext, SourceChild, MIL.M_DEFAULT, MIL.M_NULL, MatchResult, MIL.M_DEFAULT)

            ' Retrieve and display results.
            Console.Write("Each pixel of the mixture is matched " & "with one of the color samples." + Constants.vbLf)
            Console.Write(Constants.vbLf + "Color segmentation results:" + Constants.vbLf)
            Console.Write("---------------------------" + Constants.vbLf)

            For SampleIndex = 0 To NUM_SAMPLES - 1
                MIL.McolGetResult(MatchResult, MIL.M_DEFAULT, MIL.M_SAMPLE_INDEX(CType(SampleIndex, Integer)), MIL.M_SCORE, MatchScore)
                SpacesIndex = 6 - SampleNames(CType(SampleIndex, Integer)).Length
                Console.Write("Ratio of {0}{1} sample = {2,5:0.00}%" + Constants.vbLf, SampleNames(CType(SampleIndex, Integer)), Spaces(CType(SpacesIndex, Integer)), MatchScore)
            Next SampleIndex
            Console.Write(Constants.vbLf + "Results reveal the low proportion of Blue candy." + Constants.vbLf)

            ' Draw the colored label image in the destination child.
            MIL.McolDraw(MIL.M_DEFAULT, MatchResult, DestChild, MIL.M_DRAW_PIXEL_MATCH_USING_COLOR, MIL.M_ALL, MIL.M_ALL, MIL.M_DEFAULT)

            ' Pause to show the result image.
            Console.Write(Constants.vbLf + "Press <Enter> to end." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Free all allocations.
            MIL.MbufFree(DestChild)
            MIL.MbufFree(SourceChild)
            MIL.MbufFree(DisplayImage)
            MIL.McolFree(MatchResult)
            MIL.McolFree(MatchContext)
        End Sub

        '****************************************************************************
        ' Color matching in labeled regions.
        '****************************************************************************
        ' Image filenames
        Private Const FUSE_SAMPLES_IMAGE As String = MIL.M_IMAGE_PATH & "FuseSamples.mim"
        Private Const FUSE_TARGET_IMAGE As String = MIL.M_IMAGE_PATH & "Fuse.mim"

        ' Model Finder context filename
        Private Const FINDER_CONTEXT As String = MIL.M_IMAGE_PATH & "FuseModel.mmf"

        ' Number of fuse sample objects
        Private Const NUM_FUSES As Integer = 4

        ' Draw spacing and offset
        Private Const FUSE_SAMPLES_XSPACING As Integer = 40
        Private Const FUSE_SAMPLES_YOFFSET As Integer = 145

        Private Shared Sub ColorMatchingExample(ByVal MilSystem As MIL_ID, ByVal MilDisplay As MIL_ID)
            Dim DisplayImage As MIL_ID = MIL.M_NULL ' Display image buffer identifier.
            Dim SourceChild As MIL_ID = MIL.M_NULL ' Source image buffer identifier.
            Dim DestChild As MIL_ID = MIL.M_NULL ' Dest image buffer identifier.
            Dim ColMatchContext As MIL_ID = MIL.M_NULL ' Color matching context identifier.
            Dim ColMatchResult As MIL_ID = MIL.M_NULL ' Color matching result identifier.
            Dim ModelImage As MIL_ID = MIL.M_NULL ' Model image buffer identifier.
            Dim AreaImage As MIL_ID = MIL.M_NULL ' Area image buffer identifier.
            Dim OverlayID As MIL_ID = MIL.M_NULL ' Overlay image buffer identifier.
            Dim OverlaySourceChild As MIL_ID = MIL.M_NULL ' Overlay source child identifier.
            Dim OverlayDestChild As MIL_ID = MIL.M_NULL ' Overlay dest child identifier.
            Dim FuseFinderCtx As MIL_ID = MIL.M_NULL ' Model finder context identifier.
            Dim FuseFinderRes As MIL_ID = MIL.M_NULL ' Model finder result identifier.

            ' Image sizes
            Dim SizeX As MIL_INT = 0
            Dim SizeY As MIL_INT = 0

            ' Color sample names
            Dim SampleNames() As String = {"Green", " Blue", " Red", "Yellow"}

            ' Sample ROIs coordinates: OffsetX, OffsetY, SizeX, SizeY
            Dim SampleROIs(,) As MIL_INT = {{54, 139, 28, 14}, {172, 137, 30, 23}, {296, 135, 31, 23}, {417, 134, 27, 22}}

            ' Array of match sample colors.
            Dim SampleMatchColor(NUM_FUSES - 1, 2) As MIL_INT

            Console.Write(Constants.vbLf + "COLOR IDENTIFICATION:" + Constants.vbLf)
            Console.Write("---------------------" + Constants.vbLf)

            ' Allocate the parent display image.
            MIL.MbufDiskInquire(FUSE_TARGET_IMAGE, MIL.M_SIZE_X, SizeX)
            MIL.MbufDiskInquire(FUSE_TARGET_IMAGE, MIL.M_SIZE_Y, SizeY)
            MIL.MbufAllocColor(MilSystem, 3, 2 * SizeX + DISPLAY_CENTER_MARGIN_X, SizeY, 8 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_DISP + MIL.M_PROC, DisplayImage)
            MIL.MbufClear(DisplayImage, MIL.M_COLOR_BLACK)

            ' Allocate the model, area and label images.
            MIL.MbufAlloc2d(MilSystem, SizeX, SizeY, 8 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_PROC + MIL.M_DISP, ModelImage)
            MIL.MbufAlloc2d(MilSystem, SizeX, SizeY, 8 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_PROC + MIL.M_DISP, AreaImage)

            ' Create a source and destination child in the display image.
            MIL.MbufChild2d(DisplayImage, 0, 0, SizeX, SizeY, SourceChild)
            MIL.MbufChild2d(DisplayImage, SizeX + DISPLAY_CENTER_MARGIN_X, 0, SizeX, SizeY, DestChild)

            ' Load the sample source image.
            MIL.MbufLoad(FUSE_SAMPLES_IMAGE, SourceChild)

            ' Display the image buffer.
            MIL.MdispSelect(MilDisplay, DisplayImage)

            ' Prepare the overlay.
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY, MIL.M_ENABLE)
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY_CLEAR, MIL.M_DEFAULT)
            MIL.MdispInquire(MilDisplay, MIL.M_OVERLAY_ID, OverlayID)
            MIL.MbufChild2d(OverlayID, 0, 0, SizeX, SizeY, OverlaySourceChild)
            MIL.MbufChild2d(OverlayID, SizeX + DISPLAY_CENTER_MARGIN_X, 0, SizeX, SizeY, OverlayDestChild)

            ' Prepare the model finder context and result.
            MIL.MmodRestore(FINDER_CONTEXT, MilSystem, MIL.M_DEFAULT, FuseFinderCtx)
            MIL.MmodPreprocess(FuseFinderCtx, MIL.M_DEFAULT)
            MIL.MmodAllocResult(MilSystem, MIL.M_DEFAULT, FuseFinderRes)

            ' Allocate a color match context and result.
            MIL.McolAlloc(MilSystem, MIL.M_COLOR_MATCHING, MIL.M_RGB, MIL.M_DEFAULT, MIL.M_DEFAULT, ColMatchContext)
            MIL.McolAllocResult(MilSystem, MIL.M_COLOR_MATCHING_RESULT, MIL.M_DEFAULT, ColMatchResult)

            ' Define the color samples in the context.
            For i As Integer = 0 To NUM_FUSES - 1
                MIL.McolDefine(ColMatchContext, SourceChild, MIL.M_SAMPLE_LABEL(i + 1), MIL.M_IMAGE, CDbl(SampleROIs(i, 0)), CDbl(SampleROIs(i, 1)), CDbl(SampleROIs(i, 2)), CDbl(SampleROIs(i, 3)))
            Next i

            ' Preprocess the context.
            MIL.McolPreprocess(ColMatchContext, MIL.M_DEFAULT)

            ' Fill the samples colors array.
            For i As Integer = 0 To NUM_FUSES - 1
                MIL.McolInquire(ColMatchContext, MIL.M_SAMPLE_LABEL(i + 1), MIL.M_MATCH_SAMPLE_COLOR_BAND_0 + MIL.M_TYPE_MIL_INT, SampleMatchColor(i, 0))
                MIL.McolInquire(ColMatchContext, MIL.M_SAMPLE_LABEL(i + 1), MIL.M_MATCH_SAMPLE_COLOR_BAND_1 + MIL.M_TYPE_MIL_INT, SampleMatchColor(i, 1))
                MIL.McolInquire(ColMatchContext, MIL.M_SAMPLE_LABEL(i + 1), MIL.M_MATCH_SAMPLE_COLOR_BAND_2 + MIL.M_TYPE_MIL_INT, SampleMatchColor(i, 2))
            Next i

            ' Draw the color samples.
            DrawSampleColors(DestChild, SampleMatchColor, SampleNames, NUM_FUSES, FUSE_SAMPLES_XSPACING, FUSE_SAMPLES_YOFFSET)

            ' Draw the sample ROIs in the source image overlay.
            MIL.MgraColor(MIL.M_DEFAULT, MIL.M_COLOR_RED)
            For SampleIndex As Integer = 0 To NUM_FUSES - 1
                Dim XEnd As MIL_INT = SampleROIs(SampleIndex, 0) + SampleROIs(SampleIndex, 2) - 1
                Dim YEnd As MIL_INT = SampleROIs(SampleIndex, 1) + SampleROIs(SampleIndex, 3) - 1
                MIL.MgraRect(MIL.M_DEFAULT, OverlaySourceChild, SampleROIs(SampleIndex, 0), SampleROIs(SampleIndex, 1), XEnd, YEnd)
            Next SampleIndex

            ' Pause to show the source image.
            Console.Write("Colors are defined using one color sample region per fuse." + Constants.vbLf)
            Console.Write("Press <Enter> to process the target image." + Constants.vbLf)
            Console.ReadKey()

            ' Clear the overlay.
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY_CLEAR, MIL.M_DEFAULT)

            ' Load the target image into the source child.
            MIL.MbufLoad(FUSE_TARGET_IMAGE, SourceChild)

            ' Get the grayscale model image and copy it into the display dest child.
            MIL.MimConvert(SourceChild, ModelImage, MIL.M_RGB_TO_L)
            MIL.MbufCopy(ModelImage, DestChild)

            ' Find the Model.
            MIL.MmodFind(FuseFinderCtx, ModelImage, FuseFinderRes)

            ' Draw the blob image: labeled circular areas centered at each found fuse occurrence.
            Dim Number As MIL_INT = 0
            MIL.MmodGetResult(FuseFinderRes, MIL.M_DEFAULT, MIL.M_NUMBER + MIL.M_TYPE_MIL_INT, Number)
            MIL.MbufClear(AreaImage, 0)
            For ii As MIL_INT = 0 To Number - 1
                Dim X As Double = 0.0
                Dim Y As Double = 0.0

                ' Get the position
                MIL.MmodGetResult(FuseFinderRes, ii, MIL.M_POSITION_X, X)
                MIL.MmodGetResult(FuseFinderRes, ii, MIL.M_POSITION_Y, Y)

                ' Set the label color
                MIL.MgraColor(MIL.M_DEFAULT, CDbl(ii) + 1)

                ' Draw the filled circle
                MIL.MgraArcFill(MIL.M_DEFAULT, AreaImage, X, Y, 20, 20, 0, 360)
            Next ii

            ' Enable controls to draw the labeled color image.
            MIL.McolControl(ColMatchContext, MIL.M_CONTEXT, MIL.M_SAVE_AREA_IMAGE, MIL.M_ENABLE)
            MIL.McolControl(ColMatchContext, MIL.M_CONTEXT, MIL.M_GENERATE_SAMPLE_COLOR_LUT, MIL.M_ENABLE)

            ' Perform the color matching.
            MIL.McolMatch(ColMatchContext, SourceChild, MIL.M_DEFAULT, AreaImage, ColMatchResult, MIL.M_DEFAULT)

            ' Draw the label image into the overlay child.
            MIL.McolDraw(MIL.M_DEFAULT, ColMatchResult, OverlayDestChild, MIL.M_DRAW_AREA_MATCH_USING_COLOR, MIL.M_ALL, MIL.M_ALL, MIL.M_DEFAULT)

            ' Draw the model position over the colored areas.
            MIL.MgraColor(MIL.M_DEFAULT, MIL.M_COLOR_BLUE)
            MIL.MmodDraw(MIL.M_DEFAULT, FuseFinderRes, OverlayDestChild, MIL.M_DRAW_BOX + MIL.M_DRAW_POSITION, MIL.M_ALL, MIL.M_DEFAULT)

            ' Enable the display update.
            MIL.MdispControl(MilDisplay, MIL.M_UPDATE, MIL.M_ENABLE)

            ' Pause to show the resulting image.
            Console.Write(Constants.vbLf + "Fuses are located using the Model Finder tool." + Constants.vbLf)
            Console.Write("The color of each target area is identified." + Constants.vbLf)
            Console.Write("Press <Enter> to end." + Constants.vbLf)
            Console.ReadKey()

            ' Free all allocations.
            MIL.MmodFree(FuseFinderRes)
            MIL.MmodFree(FuseFinderCtx)
            MIL.MbufFree(AreaImage)
            MIL.MbufFree(ModelImage)
            MIL.MbufFree(SourceChild)
            MIL.MbufFree(DestChild)
            MIL.MbufFree(OverlaySourceChild)
            MIL.MbufFree(OverlayDestChild)
            MIL.MbufFree(DisplayImage)
            MIL.McolFree(ColMatchContext)
            MIL.McolFree(ColMatchResult)
        End Sub

        '****************************************************************************
        ' Perform color separation of colored inks on a piece of paper.
        '****************************************************************************
        ' Source image
        Private Const WRITING_IMAGE_FILE As String = MIL.M_IMAGE_PATH & "stamp.mim"

        ' Color triplets
        Private Shared ReadOnly BACKGROUND_COLOR() As MIL_INT = {245, 234, 206}
        Private Shared ReadOnly WRITING_COLOR() As MIL_INT = {141, 174, 174}
        Private Shared ReadOnly STAMP_COLOR() As MIL_INT = {226, 150, 118}

        ' Drawing spacing
        Private Const PATCHES_XSPACING As Integer = 70

        Private Shared Sub ColorSeparationExample(ByVal MilSystem As MIL_ID, ByVal MilDisplay As MIL_ID)
            Dim DisplayImage As MIL_ID = MIL.M_NULL ' Display image buffer identifier.
            Dim SourceChild As MIL_ID = MIL.M_NULL ' Source image buffer identifier.
            Dim DestChild As MIL_ID = MIL.M_NULL ' Destination image buffer identifier.
            Dim Child As MIL_ID = MIL.M_NULL ' Child buffer identifier.
            Dim ColorsArray As MIL_ID = MIL.M_NULL ' Array buffer identifier.

            ' Source image sizes.
            Dim SourceSizeX As MIL_INT = 0
            Dim SourceSizeY As MIL_INT = 0

            ' Color samples' names
            Dim ColorNames() As String = {"BACKGROUND", "WRITING", "STAMP"}

            ' Array with color patches to draw.
            Dim Colors(,) As MIL_INT = {{245, 234, 206}, {141, 174, 174}, {226, 150, 118}}

            ' Samples' color coordinates
            Dim BackgroundColor() As Byte = {245, 234, 206}
            Dim SelectedColor() As Byte = {141, 174, 174}
            Dim RejectedColor() As Byte = {226, 150, 118}

            Console.Write(Constants.vbLf + "COLOR SEPARATION:" + Constants.vbLf)
            Console.Write("-----------------" + Constants.vbLf)

            ' Allocate an array buffer and fill it with the color coordinates.
            MIL.MbufAlloc2d(MilSystem, 3, 3, 8 + MIL.M_UNSIGNED, MIL.M_ARRAY, ColorsArray)
            MIL.MbufPut2d(ColorsArray, 0, 0, 3, 1, BackgroundColor)
            MIL.MbufPut2d(ColorsArray, 0, 1, 3, 1, SelectedColor)
            MIL.MbufPut2d(ColorsArray, 0, 2, 3, 1, RejectedColor)

            ' Allocate the parent display image.
            MIL.MbufDiskInquire(WRITING_IMAGE_FILE, MIL.M_SIZE_X, SourceSizeX)
            MIL.MbufDiskInquire(WRITING_IMAGE_FILE, MIL.M_SIZE_Y, SourceSizeY)
         MIL.MbufAllocColor(MilSystem, 3, 2 * SourceSizeX + DISPLAY_CENTER_MARGIN_X, SourceSizeY, 8 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_DISP + MIL.M_PROC, DisplayImage)
            MIL.MbufClear(DisplayImage, MIL.M_COLOR_BLACK)

            ' Clear the overlay.
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY_CLEAR, MIL.M_DEFAULT)

            ' Create a source and dest child in the display image
            MIL.MbufChild2d(DisplayImage, 0, 0, SourceSizeX, SourceSizeY, SourceChild)
            MIL.MbufChild2d(DisplayImage, SourceSizeX + DISPLAY_CENTER_MARGIN_X, 0, SourceSizeX, SourceSizeY, DestChild)

            ' Load the source image into the display image source child.
            MIL.MbufLoad(WRITING_IMAGE_FILE, SourceChild)

            ' Draw the color patches.
            DrawSampleColors(DestChild, Colors, ColorNames, 3, PATCHES_XSPACING, -1)

            ' Display the image.
            MIL.MdispSelect(MilDisplay, DisplayImage)

            ' Pause to show the source image and color patches.
            Console.Write("The writing will be separated from the stamp using the following triplets:" + Constants.vbLf)
            Console.Write("the background color: beige [{0}, {1}, {2}]," + Constants.vbLf, BackgroundColor(0), BackgroundColor(1), BackgroundColor(2))
            Console.Write("the writing color   : green [{0}, {1}, {2}]," + Constants.vbLf, SelectedColor(0), SelectedColor(1), SelectedColor(2))
            Console.Write("the stamp color     : red   [{0}, {1}, {2}]." + Constants.vbLf + Constants.vbLf, RejectedColor(0), RejectedColor(1), RejectedColor(2))
            Console.Write("Press <Enter> to extract the writing." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Perform the color projection.
            MIL.McolProject(SourceChild, ColorsArray, DestChild, MIL.M_NULL, MIL.M_COLOR_SEPARATION, MIL.M_DEFAULT, CType(MIL.M_NULL, IntPtr))

            ' Wait for a key.
            Console.Write("Press <Enter> to extract the stamp." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Switch the order of the selected vs rejected colors in the color array.
            MIL.MbufPut2d(ColorsArray, 0, 2, 3, 1, SelectedColor)
            MIL.MbufPut2d(ColorsArray, 0, 1, 3, 1, RejectedColor)

            ' Perform the color projection.
            MIL.McolProject(SourceChild, ColorsArray, DestChild, MIL.M_NULL, MIL.M_COLOR_SEPARATION, MIL.M_DEFAULT, CType(MIL.M_NULL, IntPtr))

            ' Wait for a key.
            Console.Write("Press <Enter> to end." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Free all allocations.
            MIL.MbufFree(ColorsArray)
            MIL.MbufFree(SourceChild)
            MIL.MbufFree(DestChild)
            MIL.MbufFree(DisplayImage)
        End Sub

        '****************************************************************************
        ' Draw the samples as color patches.
        Private Shared Sub DrawSampleColors(ByVal DestImage As MIL_ID, ByVal pSamplesColors(,) As MIL_INT, ByVal pSampleNames() As String, ByVal NumSamples As MIL_INT, ByVal XSpacing As MIL_INT, ByVal YOffset As MIL_INT)
            Dim DestSizeX As MIL_INT = MIL.MbufInquire(DestImage, MIL.M_SIZE_X, MIL.M_NULL)
            Dim DestSizeY As MIL_INT = MIL.MbufInquire(DestImage, MIL.M_SIZE_Y, MIL.M_NULL)
            Dim OffsetX As Double = CType((DestSizeX - (NumSamples * COLOR_PATCH_SIZEX) - ((NumSamples - 1) * XSpacing)), Double) / 2.0
            Dim OffsetY As Double
            If YOffset > 0 Then
                OffsetY = YOffset
            Else
                OffsetY = CType((DestSizeY - COLOR_PATCH_SIZEY), Double) / 2.0
            End If
            Dim TextOffsetX As Double
            MIL.MgraFont(MIL.M_DEFAULT, MIL.M_FONT_DEFAULT_SMALL)

            For SampleIndex As Integer = 0 To CType(NumSamples, Integer) - 1
                MIL.MgraColor(MIL.M_DEFAULT, MIL.M_RGB888(CType(pSamplesColors(SampleIndex, 0), Integer), CType(pSamplesColors(SampleIndex, 1), Integer), CType(pSamplesColors(SampleIndex, 2), Integer)))
                MIL.MgraRectFill(MIL.M_DEFAULT, DestImage, OffsetX, OffsetY, OffsetX + COLOR_PATCH_SIZEX, OffsetY + COLOR_PATCH_SIZEY)
                MIL.MgraColor(MIL.M_DEFAULT, MIL.M_COLOR_YELLOW)
                TextOffsetX = OffsetX + COLOR_PATCH_SIZEX / 2.0 - 4.0 * pSampleNames(SampleIndex).Length + 0.5
                MIL.MgraText(MIL.M_DEFAULT, DestImage, TextOffsetX, OffsetY - 20, pSampleNames(SampleIndex))
                OffsetX += CType((COLOR_PATCH_SIZEX + XSpacing), Double)
            Next SampleIndex
        End Sub

    End Class
End Namespace
