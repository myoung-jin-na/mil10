'*****************************************************************************
'
' File name: MimSegment.vb
'
' Synopsis:  This program uses the watershed and the edge detection functions 
'            to remove the background of an image with a non-linear lighting.
'            Then, the watershed and the distance functions are used to separate
'            the touching objects.
'*****************************************************************************
Imports Microsoft.VisualBasic
Imports System
Imports System.Collections.Generic
Imports System.Text

Imports Matrox.MatroxImagingLibrary

Namespace MImSegment
    Friend Class Program
        ' Source image specifications.
        Private Const IMAGE_FILE As String = MIL.M_IMAGE_PATH & "pills.mim"

        ' Minimal distance and gradient variations for the watershed calculation.
        Private Const WATERSHED_MINIMAL_GRADIENT_VARIATION As Integer = 45
        Private Const WATERSHED_MINIMAL_DISTANCE_VARIATION As Integer = 2

        ' Position used to fetch the label of the background.
        Private Const PIXEL_FETCH_POSITION_X As Integer = 2
        Private Const PIXEL_FETCH_POSITION_Y As Integer = 2

        Shared Sub Main(ByVal args() As String)
            Dim MilApplication As MIL_ID = MIL.M_NULL ' Application identifier.
            Dim MilSystem As MIL_ID = MIL.M_NULL ' System identifier.
            Dim MilDisplay As MIL_ID = MIL.M_NULL ' Display identifier.
            Dim MilImage As MIL_ID = MIL.M_NULL ' Image buffer identifier.
            Dim MilImageWatershed As MIL_ID = MIL.M_NULL ' Image buffer identifier.
            Dim lFetchedValue() As Integer = {0} ' Label of the background.

            ' Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, MilApplication, MilSystem, MilDisplay, CType(MIL.M_NULL, IntPtr), CType(MIL.M_NULL, IntPtr))

            ' Restore the source image into two automatically allocated
            ' image buffers and select one of them to the display.
            MIL.MbufRestore(IMAGE_FILE, MilSystem, MilImageWatershed)
            MIL.MbufRestore(IMAGE_FILE, MilSystem, MilImage)
            MIL.MdispSelect(MilDisplay, MilImage)

            ' Pause to show the original image.
            Console.Write(Constants.vbLf + "SEGMENTATION:" + Constants.vbLf)
            Console.Write("-------------" + Constants.vbLf + Constants.vbLf)
            Console.Write("An edge detection followed by a watershed will be used to remove" + Constants.vbLf)
            Console.Write("the background." + Constants.vbLf + "Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Perform a edge detection on the original image.
            MIL.MimEdgeDetect(MilImageWatershed, MilImageWatershed, MIL.M_NULL, MIL.M_SOBEL, MIL.M_REGULAR_EDGE_DETECT, MIL.M_NULL)

            ' Find the basins of the edge detected image that have a minimal gray scale
            ' variation of WATERSHED_MINIMAL_GRADIENT_VARIATION.
            MIL.MimWatershed(MilImageWatershed, MIL.M_NULL, MilImageWatershed, WATERSHED_MINIMAL_GRADIENT_VARIATION, MIL.M_MINIMA_FILL + MIL.M_BASIN)

            ' Fetch the label of the background, clip it to zero and clip the other labels to 
            ' the maximum value of the buffer.
            MIL.MbufGet2d(MilImageWatershed, PIXEL_FETCH_POSITION_X, PIXEL_FETCH_POSITION_Y, 1, 1, lFetchedValue)
            MIL.MimClip(MilImageWatershed, MilImageWatershed, MIL.M_EQUAL, lFetchedValue(0), 0, 0, 0)
            MIL.MimClip(MilImageWatershed, MilImage, MIL.M_NOT_EQUAL, 0, 0, &HFF, 0)

            ' Pause to show the binarized image.
            Console.Write("A distance transformation followed by a watershed will be used " + Constants.vbLf)
            Console.Write("to separate the touching pills." + Constants.vbLf)
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Perform a distance transformation on the binarized image.
            MIL.MimDistance(MilImage, MilImageWatershed, MIL.M_CHAMFER_3_4)

            ' Find the watersheds of the image.
            MIL.MimWatershed(MilImageWatershed, MIL.M_NULL, MilImageWatershed, WATERSHED_MINIMAL_DISTANCE_VARIATION, MIL.M_STRAIGHT_WATERSHED + MIL.M_MAXIMA_FILL + MIL.M_SKIP_LAST_LEVEL)

            ' AND the watershed image with the binarized image to separate the touching pills.
            MIL.MimArith(MilImageWatershed, MilImage, MilImage, MIL.M_AND)

            ' Pause to show the segmented image.
            Console.Write("Here are the segmented pills." + Constants.vbLf)
            Console.Write("Press <Enter> to end." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Free all allocations.
            MIL.MbufFree(MilImageWatershed)
            MIL.MbufFree(MilImage)
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL)
        End Sub
    End Class
End Namespace
