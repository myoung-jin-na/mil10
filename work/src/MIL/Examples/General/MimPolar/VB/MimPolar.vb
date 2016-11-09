'*****************************************************************************
'
' File name: MimPolar.vb
'
' Synopsis:  This program uses the polar-to-rectangular transformation
'            to unroll a string.
'*****************************************************************************
Imports Microsoft.VisualBasic
Imports System
Imports System.Collections.Generic
Imports System.Text

Imports Matrox.MatroxImagingLibrary

Namespace MImPolar
    Friend Class Program
        ' Target image specifications.
        Private Const IMAGE_FILE As String = MIL.M_IMAGE_PATH & "Polar.mim"

        ' Points used to compute the parameters of the circle.
        Private Const POINT1_X As Integer = 147
        Private Const POINT1_Y As Integer = 359
        Private Const POINT2_X As Integer = 246
        Private Const POINT2_Y As Integer = 404
        Private Const POINT3_X As Integer = 354
        Private Const POINT3_Y As Integer = 368

        ' Polar stripe features.
        Private Const DELTA_RADIUS As Integer = 25
        Private Const START_ANGLE As Integer = 210
        Private Const END_ANGLE As Integer = 330

        Shared Sub Main(ByVal args() As String)
            Dim MilApplication As MIL_ID = MIL.M_NULL ' Application identifier.
            Dim MilSystem As MIL_ID = MIL.M_NULL ' System identifier.
            Dim MilDisplay As MIL_ID = MIL.M_NULL ' Display identifier.
            Dim MilImage As MIL_ID = MIL.M_NULL ' Image buffer identifier.
            Dim MilPolarImage As MIL_ID = MIL.M_NULL ' Destination buffer identifier.
            Dim SizeRadius As Double = 0.0
            Dim SizeAngle As Double = 0.0
            Dim CenterX As Double = 0.0
            Dim CenterY As Double = 0.0
            Dim Radius As Double = 0.0
            Dim OffsetX As Integer = 0
            Dim OffsetY As Integer = 0
            Dim SizeX As Integer = 0
            Dim SizeY As Integer = 0

            ' Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, MilApplication, MilSystem, MilDisplay, CType(MIL.M_NULL, IntPtr), CType(MIL.M_NULL, IntPtr))

            ' Load the source image and display it.
            MIL.MbufRestore(IMAGE_FILE, MilSystem, MilImage)
            MIL.MdispSelect(MilDisplay, MilImage)

            ' Calculate the parameters of the circle.
            GenerateCircle(POINT1_X, POINT1_Y, POINT2_X, POINT2_Y, POINT3_X, POINT3_Y, CenterX, CenterY, Radius)

            ' Get the size of the destination buffer.
            MIL.MimPolarTransform(MilImage, MIL.M_NULL, CenterX, CenterY, Radius + DELTA_RADIUS, Radius - DELTA_RADIUS, START_ANGLE, END_ANGLE, MIL.M_RECTANGULAR_TO_POLAR, MIL.M_NEAREST_NEIGHBOR + MIL.M_OVERSCAN_ENABLE, SizeAngle, SizeRadius)

            ' Allocate the destination buffer.
            OffsetX = CInt(Fix((MIL.MbufInquire(MilImage, MIL.M_SIZE_X, MIL.M_NULL) / 2) - CType((SizeAngle / 2), MIL_INT)))
            OffsetY = 20
            SizeX = CInt(Math.Ceiling(SizeAngle))
            SizeY = CInt(Math.Ceiling(SizeRadius))
            MIL.MbufChild2d(MilImage, OffsetX, OffsetY, SizeX, SizeY, MilPolarImage)

            ' Print a message.
            Console.Write(Constants.vbLf + "POLAR TRANSFORMATION:" + Constants.vbLf)
            Console.Write("---------------------" + Constants.vbLf + Constants.vbLf)
            Console.Write("A string will be unrolled using a polar-to-rectangular transformation." + Constants.vbLf)
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Unroll the string.
            MIL.MimPolarTransform(MilImage, MilPolarImage, CenterX, CenterY, Radius + DELTA_RADIUS, Radius - DELTA_RADIUS, START_ANGLE, END_ANGLE, MIL.M_RECTANGULAR_TO_POLAR, MIL.M_NEAREST_NEIGHBOR + MIL.M_OVERSCAN_ENABLE, SizeAngle, SizeRadius)

            ' Print a message on the Host screen.
            Console.Write("Press <Enter> to end." + Constants.vbLf)
            Console.ReadKey()

            ' Free buffers.
            MIL.MbufFree(MilPolarImage)
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MilImage)
        End Sub

        ' GenerateCircle() function returns the center and the radius of a circle 
        ' defined by 3 non-collinear points.
        Private Shared Sub GenerateCircle(ByVal X1 As Double, ByVal Y1 As Double, ByVal X2 As Double, ByVal Y2 As Double, ByVal X3 As Double, ByVal Y3 As Double, ByRef CenterX As Double, ByRef CenterY As Double, ByRef Radius As Double)
            Dim Slope1 As Double = 0.0
            Dim Slope2 As Double = 0.0
            Dim MidPoint1X As Double = 0.0
            Dim MidPoint1Y As Double = 0.0
            Dim MidPoint2X As Double = 0.0
            Dim MidPoint2Y As Double = 0.0
            Dim Offset1 As Double = 0.0
            Dim Offset2 As Double = 0.0

            ' Compute the middle points of the two segments.
            MidPoint1X = (X1 + X2) / 2
            MidPoint1Y = (Y1 + Y2) / 2
            MidPoint2X = (X2 + X3) / 2
            MidPoint2Y = (Y2 + Y3) / 2

            ' Compute the slope between points 1 and 2, and between 
            ' points 2 and 3.
            If ((Y2 - Y1) <> 0.0) AndAlso ((Y3 - Y2) <> 0.0) Then
                Slope1 = -(X2 - X1) / (Y2 - Y1)
                Slope2 = -(X3 - X2) / (Y3 - Y2)

                Offset1 = MidPoint1Y - Slope1 * MidPoint1X
                Offset2 = MidPoint2Y - Slope2 * MidPoint2X

                CenterX = (Offset2 - Offset1) / (Slope1 - Slope2)
                CenterY = Slope1 * (CenterX) + Offset1
            ElseIf ((Y2 - Y1) = 0.0) AndAlso ((Y3 - Y2) <> 0.0) Then
                Slope2 = -(X3 - X2) / (Y3 - Y2)
                Offset2 = MidPoint2Y - Slope2 * MidPoint2X

                CenterX = MidPoint1X
                CenterY = Slope2 * (CenterX) + Offset2
            ElseIf ((Y2 - Y1) <> 0.0) AndAlso ((Y3 - Y2) = 0.0) Then
                Slope1 = -(X2 - X1) / (Y2 - Y1)
                Offset1 = MidPoint1Y - Slope1 * MidPoint1X

                CenterX = MidPoint2X
                CenterY = Slope1 * (CenterX) + Offset1
            End If

            ' Compute the radius.
            Radius = Math.Sqrt(Math.Pow(CenterX - X1, 2) + Math.Pow(CenterY - Y1, 2))
        End Sub
    End Class
End Namespace
