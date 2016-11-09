'****************************************************************************
'
' File name: Medge.vb
'
' Synopsis:  This program uses the MIL Edge Finder module to find and measure
'            the outer diameter of good seals in a target image. 
'****************************************************************************
Imports Microsoft.VisualBasic
Imports System
Imports System.Collections.Generic
Imports System.Text

Imports Matrox.MatroxImagingLibrary

Namespace MEdge
    Friend Class Program
        ' Source MIL image file specifications.
        Private Const CONTOUR_IMAGE As String = MIL.M_IMAGE_PATH & "Seals.mim"
        Private Const CONTOUR_MAX_RESULTS As Integer = 100
        Private Const CONTOUR_MAXIMUM_ELONGATION As Double = 0.8
        Private Shared ReadOnly CONTOUR_DRAW_COLOR As Integer = MIL.M_COLOR_GREEN
        Private Shared ReadOnly CONTOUR_LABEL_COLOR As Integer = MIL.M_COLOR_RED

        Shared Sub Main(ByVal args() As String)
            Dim MilApplication As MIL_ID = MIL.M_NULL                   ' Application identifier.
            Dim MilSystem As MIL_ID = MIL.M_NULL                        ' System Identifier.
            Dim MilDisplay As MIL_ID = MIL.M_NULL                       ' Display identifier.
            Dim MilImage As MIL_ID = MIL.M_NULL                         ' Image buffer identifier.
            Dim GraphicList As MIL_ID = MIL.M_NULL                      ' Graphic list identifier.
            Dim MilEdgeContext As MIL_ID = MIL.M_NULL                   ' Edge context.
            Dim MilEdgeResult As MIL_ID = MIL.M_NULL                    ' Edge result identifier.
            Dim EdgeDrawColor As Double = CONTOUR_DRAW_COLOR            ' Edge draw color.
            Dim LabelDrawColor As Double = CONTOUR_LABEL_COLOR          ' Text draw color.
            Dim NumEdgeFound As MIL_INT = 0                             ' Number of edges found.
            Dim NumResults As MIL_INT = 0                               ' Number of results found.
            Dim i As Integer = 0
            Dim MeanFeretDiameter(CONTOUR_MAX_RESULTS - 1) As Double    ' Edge mean Feret diameter.

            ' Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, MilApplication, MilSystem, MilDisplay, CType(MIL.M_NULL, IntPtr), CType(MIL.M_NULL, IntPtr))

            ' Restore the image and display it.
            MIL.MbufRestore(CONTOUR_IMAGE, MilSystem, MilImage)
            MIL.MdispSelect(MilDisplay, MilImage)

            ' Allocate a graphic list to hold the subpixel annotations to draw.
            MIL.MgraAllocList(MilSystem, MIL.M_DEFAULT, GraphicList)

            ' Associate the graphic list to the display for annotations.
            MIL.MdispControl(MilDisplay, MIL.M_ASSOCIATED_GRAPHIC_LIST_ID, GraphicList)

            ' Pause to show the original image.
            Console.Write(Constants.vbLf + "EDGE MODULE:" + Constants.vbLf)
            Console.Write("------------" + Constants.vbLf + Constants.vbLf)
            Console.Write("This program determines the outer seal diameters in the displayed image " + Constants.vbLf)
            Console.Write("by detecting and analyzing contours with the Edge Finder module." + Constants.vbLf)
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Allocate a Edge Finder context.
            MIL.MedgeAlloc(MilSystem, MIL.M_CONTOUR, MIL.M_DEFAULT, MilEdgeContext)

            ' Allocate a result buffer.
            MIL.MedgeAllocResult(MilSystem, MIL.M_DEFAULT, MilEdgeResult)

            ' Enable features to compute.
            MIL.MedgeControl(MilEdgeContext, MIL.M_MOMENT_ELONGATION, MIL.M_ENABLE)
            MIL.MedgeControl(MilEdgeContext, MIL.M_FERET_MEAN_DIAMETER + MIL.M_SORT1_DOWN, MIL.M_ENABLE)

            ' Calculate edges and features.
            MIL.MedgeCalculate(MilEdgeContext, MilImage, MIL.M_NULL, MIL.M_NULL, MIL.M_NULL, MilEdgeResult, MIL.M_DEFAULT)

            ' Get the number of edges found.
            MIL.MedgeGetResult(MilEdgeResult, MIL.M_DEFAULT, MIL.M_NUMBER_OF_CHAINS + MIL.M_TYPE_MIL_INT, NumEdgeFound)

            ' Draw edges in the source image to show the result.
            MIL.MgraColor(MIL.M_DEFAULT, EdgeDrawColor)
            MIL.MedgeDraw(MIL.M_DEFAULT, MilEdgeResult, GraphicList, MIL.M_DRAW_EDGES, MIL.M_DEFAULT, MIL.M_DEFAULT)

            ' Pause to show the edges.
            Console.Write("{0} edges were found in the image." + Constants.vbLf, NumEdgeFound)
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Exclude elongated edges.
            MIL.MedgeSelect(MilEdgeResult, MIL.M_EXCLUDE, MIL.M_MOMENT_ELONGATION, MIL.M_LESS, CONTOUR_MAXIMUM_ELONGATION, MIL.M_NULL)

            ' Exclude inner chains.
            MIL.MedgeSelect(MilEdgeResult, MIL.M_EXCLUDE, MIL.M_INCLUDED_EDGES, MIL.M_INSIDE_BOX, MIL.M_NULL, MIL.M_NULL)

            ' Draw remaining edges and their index to show the result.
            MIL.MgraClear(MIL.M_DEFAULT, GraphicList)
            MIL.MgraColor(MIL.M_DEFAULT, EdgeDrawColor)
            MIL.MedgeDraw(MIL.M_DEFAULT, MilEdgeResult, GraphicList, MIL.M_DRAW_EDGES, MIL.M_DEFAULT, MIL.M_DEFAULT)

            ' Pause to show the results.
            Console.Write("Elongated edges and inner edges of each seal were removed." + Constants.vbLf)
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Get the number of edges found.
            MIL.MedgeGetResult(MilEdgeResult, MIL.M_DEFAULT, MIL.M_NUMBER_OF_CHAINS + MIL.M_TYPE_MIL_INT, NumResults)

            ' If the right number of edges were found.
            If (NumResults >= 1) AndAlso (NumResults <= CONTOUR_MAX_RESULTS) Then
                ' Draw the index of each edge.
                MIL.MgraColor(MIL.M_DEFAULT, LabelDrawColor)
                MIL.MedgeDraw(MIL.M_DEFAULT, MilEdgeResult, GraphicList, MIL.M_DRAW_INDEX, MIL.M_DEFAULT, MIL.M_DEFAULT)

                ' Get the mean Feret diameters.
                MIL.MedgeGetResult(MilEdgeResult, MIL.M_DEFAULT, MIL.M_FERET_MEAN_DIAMETER, MeanFeretDiameter)

                ' Print the results.
                Console.Write("Mean diameter of the {0} outer edges are:" + Constants.vbLf + Constants.vbLf, NumResults)
                Console.Write("Index   Mean diameter " + Constants.vbLf)
                For i = 0 To CType(NumResults - 1, Integer)
                    Console.Write("{0,-11}{1,-13:0.00}" + Constants.vbLf, i, MeanFeretDiameter(i))
                Next i
            Else
                Console.Write("Edges have not been found or the number of found edges is greater than" + Constants.vbLf)
                Console.Write("the specified maximum number of edges !" + Constants.vbLf + Constants.vbLf)
            End If

            ' Wait for a key press.
            Console.Write(Constants.vbLf + "Press <Enter> to end." + Constants.vbLf)
            Console.ReadKey()

            ' Free MIL objects.
            MIL.MgraFree(GraphicList)
            MIL.MbufFree(MilImage)
            MIL.MedgeFree(MilEdgeContext)
            MIL.MedgeFree(MilEdgeResult)

            ' Free defaults.
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL)
        End Sub
    End Class
End Namespace
