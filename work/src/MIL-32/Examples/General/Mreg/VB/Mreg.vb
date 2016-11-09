'******************************************************************************
'
' File name: Mreg.vb
'
' Synopsis:  This program uses the registration module to form a mosaic of 
'            three images taken from a camera at unknown translation.
'******************************************************************************
Imports Microsoft.VisualBasic
Imports System
Imports System.Collections.Generic
Imports System.Text

Imports Matrox.MatroxImagingLibrary

Namespace MReg
    Friend Class Program
        ' Number of images to register.
        Private Const NUM_IMAGES_TO_REGISTER As Integer = 3

        ' MIL image file specifications.
        Private Const IMAGE_FILES_SOURCE As String = MIL.M_IMAGE_PATH & "CircuitBoardPart{0}.mim"

        Shared Sub Main(ByVal args() As String)
            Dim MilApplication As MIL_ID = MIL.M_NULL ' Application identifier.
            Dim MilSystem As MIL_ID = MIL.M_NULL ' System identifier.
            Dim MilDisplay As MIL_ID = MIL.M_NULL ' Display identifier.
            Dim MilGraphicList As MIL_ID = MIL.M_NULL ' Graphic list identifier.
            Dim MilSourceImages(NUM_IMAGES_TO_REGISTER - 1) As MIL_ID ' Source images buffer identifiers.
            Dim MilMosaicImage As MIL_ID = MIL.M_NULL ' Mosaic image buffer identifier.
            Dim MilRegContext As MIL_ID = MIL.M_NULL ' Registration context identifier.
            Dim MilRegResult As MIL_ID = MIL.M_NULL ' Registration result identifier.
            Dim Result As MIL_INT = 0 ' Result of the registration.
            Dim MosaicSizeX As MIL_INT = 0 ' Size of the mosaic.
            Dim MosaicSizeY As MIL_INT = 0
            Dim MosaicSizeBand As MIL_INT = 0 ' Characteristics of mosaic image.
            Dim MosaicType As MIL_INT = 0

            Dim ImageFilesSource(NUM_IMAGES_TO_REGISTER - 1) As String

            ' Allocate defaults
            MIL.MappAllocDefault(MIL.M_DEFAULT, MilApplication, MilSystem, MilDisplay, CType(MIL.M_NULL, IntPtr), CType(MIL.M_NULL, IntPtr))

            ' Load source image names.
            For i As Integer = 0 To NUM_IMAGES_TO_REGISTER - 1
                ImageFilesSource(i) = String.Format(IMAGE_FILES_SOURCE, i)
            Next i

            ' Print module name.
            Console.Write(Constants.vbLf + "REGISTRATION MODULE:" + Constants.vbLf)
            Console.Write("---------------------" + Constants.vbLf + Constants.vbLf)

            ' Print comment.
            Console.Write("This program will make a mosaic from many source images." + Constants.vbLf)
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Restore the source images.
            For i As Integer = 0 To NUM_IMAGES_TO_REGISTER - 1
                MIL.MbufRestore(ImageFilesSource(i), MilSystem, MilSourceImages(i))
            Next i

            ' Display the source images.
            For i As Integer = 0 To NUM_IMAGES_TO_REGISTER - 1
                MIL.MdispSelect(MilDisplay, MilSourceImages(i))

                ' Pause to show each image.
                Console.Write("image {0}." + Constants.vbLf, i)
                Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
                Console.ReadKey()
            Next i

            ' Allocate a graphic list to hold the subpixel annotations to draw.
            MIL.MgraAllocList(MilSystem, MIL.M_DEFAULT, MilGraphicList)
            MIL.MdispControl(MilDisplay, MIL.M_ASSOCIATED_GRAPHIC_LIST_ID, MilGraphicList)

            ' Allocate a new empty registration context.
            MIL.MregAlloc(MilSystem, MIL.M_CORRELATION, MIL.M_DEFAULT, MilRegContext)

            ' Allocate a new empty registration result buffer.
            MIL.MregAllocResult(MilSystem, MIL.M_DEFAULT, MilRegResult)

            ' Set the transformation type to translation.
            MIL.MregControl(MilRegContext, MIL.M_CONTEXT, MIL.M_TRANSFORMATION_TYPE, MIL.M_TRANSLATION)

            ' By default, each image will be registered with the previous in the list
            ' No need to set other location parameters.

            ' Set range to 100% in order to search all possible translations.
            MIL.MregControl(MilRegContext, MIL.M_CONTEXT, MIL.M_LOCATION_DELTA, 100)

            ' Calculate the registration on the images.
            MIL.MregCalculate(MilRegContext, MilSourceImages, MilRegResult, NUM_IMAGES_TO_REGISTER, MIL.M_DEFAULT)

            ' Verify if registration is successful.
            MIL.MregGetResult(MilRegResult, MIL.M_GENERAL, MIL.M_RESULT + MIL.M_TYPE_MIL_INT, Result)
            If Result = MIL.M_SUCCESS Then
                ' Get the size of the required mosaic buffer.
                MIL.MregGetResult(MilRegResult, MIL.M_GENERAL, MIL.M_MOSAIC_SIZE_X + MIL.M_TYPE_MIL_INT, MosaicSizeX)
                MIL.MregGetResult(MilRegResult, MIL.M_GENERAL, MIL.M_MOSAIC_SIZE_Y + MIL.M_TYPE_MIL_INT, MosaicSizeY)

                ' The mosaic type will be the same as the source images.
                MIL.MbufInquire(MilSourceImages(0), MIL.M_SIZE_BAND, MosaicSizeBand)
                MIL.MbufInquire(MilSourceImages(0), MIL.M_TYPE, MosaicType)

                ' Allocate mosaic image.
                MIL.MbufAllocColor(MilSystem, MosaicSizeBand, MosaicSizeX, MosaicSizeY, MosaicType, MIL.M_IMAGE + MIL.M_PROC + MIL.M_DISP, MilMosaicImage)

                ' Compose the mosaic from the source images.
                MIL.MregTransformImage(MilRegResult, MilSourceImages, MilMosaicImage, NUM_IMAGES_TO_REGISTER, MIL.M_BILINEAR + MIL.M_OVERSCAN_CLEAR, MIL.M_DEFAULT)

                ' Display the mosaic image and prepare for overlay annotations.
                MIL.MdispSelect(MilDisplay, MilMosaicImage)
                MIL.MgraColor(MIL.M_DEFAULT, MIL.M_RGB888(0, &HFF, 0))

                ' Pause to show the mosaic.
                Console.Write("mosaic image." + Constants.vbLf)
                Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
                Console.ReadKey()

                ' Draw the box of all source images in the mosaic.
                MIL.MregDraw(MIL.M_DEFAULT, MilRegResult, MilGraphicList, MIL.M_DRAW_BOX, MIL.M_ALL, MIL.M_DEFAULT)

                ' Draw a cross at the center of each image in the mosaic.
                For i As Integer = 0 To NUM_IMAGES_TO_REGISTER - 1
                    ' Coordinates of the center of the source image.
                    Dim SourcePosX As Double = 0.5 * CDbl(MIL.MbufInquire(MilSourceImages(i), MIL.M_SIZE_X, MIL.M_NULL))
                    Dim SourcePosY As Double = 0.5 * CDbl(MIL.MbufInquire(MilSourceImages(i), MIL.M_SIZE_Y, MIL.M_NULL))

                    ' Transform the coordinates to the mosaic.
                    Dim MosaicPosX As Double = 0
                    Dim MosaicPosY As Double = 0
                    MIL.MregTransformCoordinate(MilRegResult, i, MIL.M_MOSAIC, SourcePosX, SourcePosY, MosaicPosX, MosaicPosY, MIL.M_DEFAULT)
                    Dim MosaicPosXMilInt As Integer = CInt(Fix(MosaicPosX + 0.5))
                    Dim MosaicPosYMilInt As Integer = CInt(Fix(MosaicPosY + 0.5))

                    ' Draw the cross in the mosaic.
                    MIL.MgraLine(MIL.M_DEFAULT, MilGraphicList, MosaicPosXMilInt - 4, MosaicPosYMilInt, MosaicPosXMilInt + 4, MosaicPosYMilInt)
                    MIL.MgraLine(MIL.M_DEFAULT, MilGraphicList, MosaicPosXMilInt, MosaicPosYMilInt - 4, MosaicPosXMilInt, MosaicPosYMilInt + 4)
                Next i

                Console.Write("The bounding boxes and the center of all source images" + Constants.vbLf)
                Console.Write("have been drawn in the mosaic." + Constants.vbLf)
            Else
                Console.Write("Error: Registration was not successful." + Constants.vbLf)
            End If

            ' Pause to show results.
            Console.Write(Constants.vbLf + "Press <Enter> to end." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Free all allocations.
            MIL.MgraFree(MilGraphicList)
            If MilMosaicImage <> MIL.M_NULL Then
                MIL.MbufFree(MilMosaicImage)
            End If

            MIL.MregFree(MilRegContext)
            MIL.MregFree(MilRegResult)

            For i As Integer = 0 To NUM_IMAGES_TO_REGISTER - 1
                MIL.MbufFree(MilSourceImages(i))
            Next i

            ' Free defaults.
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL)
        End Sub
    End Class
End Namespace
