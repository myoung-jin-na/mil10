'**********************************************************************************
' 
' File name: MbufColor.vb
'
' Synopsis:  This program demonstrates color buffer manipulations. It allocates 
'            a displayable color image buffer, displays it, and loads a color   
'            image into the left part. After that, color annotations are done 
'            in each band of the color buffer. Finaly, to increase the image
'            luminance, the image is converted to Hue, Luminance and Saturation 
'            (HLS), a certain offset is added to the luminance component and 
'            the image is converted back to Red, Green, Blue (RGB) into the 
'            right part to display the result. 
'          
'            The example also demonstrates how to display multiple images 
'            in a single display using child buffers.
'**********************************************************************************
Imports Microsoft.VisualBasic
Imports System
Imports System.Collections.Generic
Imports System.Text

Imports Matrox.MatroxImagingLibrary

Namespace MBufColor
    Friend Class Program
        ' Source MIL image file specifications.
        Private Const IMAGE_FILE As String = MIL.M_IMAGE_PATH & "Bird.mim"

        ' Luminance offset to add to the image.
        Private Const IMAGE_LUMINANCE_OFFSET As Integer = 40

        ' Main function.
        Shared Sub Main(ByVal args() As String)
            Dim MilApplication As MIL_ID = MIL.M_NULL ' Application identifier.
            Dim MilSystem As MIL_ID = MIL.M_NULL ' System identifier.
            Dim MilDisplay As MIL_ID = MIL.M_NULL ' Display identifier.
            Dim MilImage As MIL_ID = MIL.M_NULL ' Image buffer identifier.
            Dim MilLeftSubImage As MIL_ID = MIL.M_NULL ' Sub-image buffer identifier for original image.
            Dim MilRightSubImage As MIL_ID = MIL.M_NULL ' Sub-image buffer identifier for processed image.
            Dim MilLumSubImage As MIL_ID = MIL.M_NULL ' Sub-image buffer identifier for luminance.
            Dim MilRedBandSubImage As MIL_ID = MIL.M_NULL ' Sub-image buffer identifier for red component.
            Dim MilGreenBandSubImage As MIL_ID = MIL.M_NULL ' Sub-image buffer identifier for green component.
            Dim MilBlueBandSubImage As MIL_ID = MIL.M_NULL ' Sub-image buffer identifier for blue component.
            Dim SizeX As MIL_INT = 0
            Dim SizeY As MIL_INT = 0
            Dim SizeBand As MIL_INT = 0
            Dim Type As MIL_INT = 0

            ' Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, MilApplication, MilSystem, MilDisplay, CType(MIL.M_NULL, IntPtr), CType(MIL.M_NULL, IntPtr))

            ' Allocate a color display buffer twice the size of the source image and display it.
            MIL.MbufAllocColor(MilSystem, MIL.MbufDiskInquire(IMAGE_FILE, MIL.M_SIZE_BAND, SizeBand), MIL.MbufDiskInquire(IMAGE_FILE, MIL.M_SIZE_X, SizeX) * 2, MIL.MbufDiskInquire(IMAGE_FILE, MIL.M_SIZE_Y, SizeY), MIL.MbufDiskInquire(IMAGE_FILE, MIL.M_TYPE, Type), MIL.M_IMAGE + MIL.M_DISP + MIL.M_PROC, MilImage)
            MIL.MbufClear(MilImage, 0)
            MIL.MdispSelect(MilDisplay, MilImage)

            ' Define 2 child buffers that maps to the left and right part of the display 
            ' buffer, to put the source and destination color images.
            '
            MIL.MbufChild2d(MilImage, 0, 0, SizeX, SizeY, MilLeftSubImage)
            MIL.MbufChild2d(MilImage, SizeX, 0, SizeX, SizeY, MilRightSubImage)

            ' Load the color source image on the left.
            MIL.MbufLoad(IMAGE_FILE, MilLeftSubImage)

            ' Define child buffers that map to the red, green and blue components
            ' of the source image.
            '
            MIL.MbufChildColor(MilLeftSubImage, MIL.M_RED, MilRedBandSubImage)
            MIL.MbufChildColor(MilLeftSubImage, MIL.M_GREEN, MilGreenBandSubImage)
            MIL.MbufChildColor(MilLeftSubImage, MIL.M_BLUE, MilBlueBandSubImage)

            ' Write color text annotations to show access in each individual band of the image.
            '
            ' Note that this is typically done more simply by using:
            '  MIL.MgraColor(MIL.M_DEFAULT, MIL.M_RGB(0xFF,0x90,0x00));
            '  MIL.MgraText(MIL.M_DEFAULT, MilLeftSubImage, ...);

            MIL.MgraColor(MIL.M_DEFAULT, &HFF)
            MIL.MgraText(MIL.M_DEFAULT, MilRedBandSubImage, SizeX / 16, SizeY / 8, " TOUCAN ")
            MIL.MgraColor(MIL.M_DEFAULT, &H90)
            MIL.MgraText(MIL.M_DEFAULT, MilGreenBandSubImage, SizeX / 16, SizeY / 8, " TOUCAN ")
            MIL.MgraColor(MIL.M_DEFAULT, &H0)
            MIL.MgraText(MIL.M_DEFAULT, MilBlueBandSubImage, SizeX / 16, SizeY / 8, " TOUCAN ")

            ' Print a message.
            Console.Write(Constants.vbLf + "COLOR OPERATIONS:" + Constants.vbLf)
            Console.Write("-----------------" + Constants.vbLf + Constants.vbLf)
            Console.Write("A color source image was loaded on the left and color text" + Constants.vbLf)
            Console.Write("annotations were written in it." + Constants.vbLf)
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Convert image to Hue, Luminance, Saturation color space (HSL).
            MIL.MimConvert(MilLeftSubImage, MilRightSubImage, MIL.M_RGB_TO_HSL)

            ' Create a child buffer that maps to the luminance component.
            MIL.MbufChildColor(MilRightSubImage, MIL.M_LUMINANCE, MilLumSubImage)

            ' Add an offset to the luminance component.
            MIL.MimArith(MilLumSubImage, IMAGE_LUMINANCE_OFFSET, MilLumSubImage, MIL.M_ADD_CONST + MIL.M_SATURATION)

            ' Convert image back to Red, Green, Blue color space (RGB) for display.
            MIL.MimConvert(MilRightSubImage, MilRightSubImage, MIL.M_HSL_TO_RGB)

            ' Print a message.
            Console.Write("Luminance was increased using color image processing." + Constants.vbLf)

            ' Print a message.
            Console.Write("Press <Enter> to end." + Constants.vbLf)
            Console.ReadKey()

            ' Release sub-images and color image buffer.
            MIL.MbufFree(MilLumSubImage)
            MIL.MbufFree(MilRedBandSubImage)
            MIL.MbufFree(MilGreenBandSubImage)
            MIL.MbufFree(MilBlueBandSubImage)
            MIL.MbufFree(MilRightSubImage)
            MIL.MbufFree(MilLeftSubImage)
            MIL.MbufFree(MilImage)

            ' Release defaults.
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL)
        End Sub
    End Class
End Namespace
