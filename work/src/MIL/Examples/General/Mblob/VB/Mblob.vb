'******************************************************************************
'
' File name: Mblob.vb
'
' Synopsis:  This program loads an image of some nuts, bolts and washers, 
'            determines the number of each of these, finds and marks
'            their center of gravity using the Blob analysis module.
'******************************************************************************
Imports Microsoft.VisualBasic
Imports System
Imports System.Collections.Generic
Imports System.Text

Imports Matrox.MatroxImagingLibrary

Namespace MBlob
    Friend Class Program
        ' Target MIL image file specifications.
        Private Const IMAGE_FILE As String = MIL.M_IMAGE_PATH & "BoltsNutsWashers.mim"
        Private Const IMAGE_THRESHOLD_VALUE As Integer = 26

        ' Minimum and maximum area of blobs.
        Private Const MIN_BLOB_AREA As Integer = 50
        Private Const MAX_BLOB_AREA As Integer = 50000

        ' Radius of the smallest particles to keep.
        Private Const MIN_BLOB_RADIUS As Integer = 3

        ' Minimum hole compactness corresponding to a washer.
        Private Const MIN_COMPACTNESS As Double = 1.5
        Shared Sub Main(ByVal args() As String)

            Dim MilApplication As MIL_ID = MIL.M_NULL           ' Application identifier.
            Dim MilSystem As MIL_ID = MIL.M_NULL                ' System identifier.
            Dim MilDisplay As MIL_ID = MIL.M_NULL               ' Display identifier.
            Dim MilImage As MIL_ID = MIL.M_NULL                 ' Image buffer identifier.
            Dim MilGraphicList As MIL_ID = MIL.M_NULL           ' Graphic list indetifier.
            Dim MilBinImage As MIL_ID = MIL.M_NULL              ' Binary image buffer identifier.
            Dim MilBlobResult As MIL_ID = MIL.M_NULL            ' Blob result buffer identifier.
            Dim MilBlobFeatureList As MIL_ID = MIL.M_NULL       ' Feature list identifier.
            Dim TotalBlobs As MIL_INT = 0                       ' Total number of blobs.
            Dim BlobsWithHoles As MIL_INT = 0                   ' Number of blobs with holes.
            Dim BlobsWithRoughHoles As MIL_INT = 0              ' Number of blobs with rough holes.
            Dim n As Integer = 0                                ' Counter.
            Dim SizeX As MIL_INT = 0                            ' Size X of the source buffer
            Dim SizeY As MIL_INT = 0                            ' Size Y of the source buffer

            Dim CogX() As Double = Nothing                      ' X coordinate of center of gravity.
            Dim CogY() As Double = Nothing                      ' Y coordinate of center of gravity.

            ' Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, MilApplication, MilSystem, MilDisplay, CType(MIL.M_NULL, IntPtr), CType(MIL.M_NULL, IntPtr))

            ' Restore source image into image buffer.
            MIL.MbufRestore(IMAGE_FILE, MilSystem, MilImage)

            ' Allocate a graphic list to hold the subpixel annotations to draw.
            MIL.MgraAllocList(MilSystem, MIL.M_DEFAULT, MilGraphicList)

            ' Associate the graphic list to the display.
            MIL.MdispControl(MilDisplay, MIL.M_ASSOCIATED_GRAPHIC_LIST_ID, MilGraphicList)

            ' Display the buffer.
            MIL.MdispSelect(MilDisplay, MilImage)

            ' Allocate a binary image buffer for fast processing.
            MIL.MbufInquire(MilImage, MIL.M_SIZE_X, SizeX)
            MIL.MbufInquire(MilImage, MIL.M_SIZE_Y, SizeY)
            MIL.MbufAlloc2d(MilSystem, SizeX, SizeY, 1 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_PROC, MilBinImage)

            ' Pause to show the original image.
            Console.Write(Constants.vbLf + "BLOB ANALYSIS:" + Constants.vbLf)
            Console.Write("--------------" + Constants.vbLf + Constants.vbLf)
            Console.Write("This program determines the number of bolts, nuts and washers" + Constants.vbLf)
            Console.Write("in the image and finds their center of gravity." + Constants.vbLf)
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Binarize image.
            MIL.MimBinarize(MilImage, MilBinImage, MIL.M_FIXED + MIL.M_GREATER_OR_EQUAL, IMAGE_THRESHOLD_VALUE, MIL.M_NULL)

            ' Remove small particles and then remove small holes.
            MIL.MimOpen(MilBinImage, MilBinImage, MIN_BLOB_RADIUS, MIL.M_BINARY)
            MIL.MimClose(MilBinImage, MilBinImage, MIN_BLOB_RADIUS, MIL.M_BINARY)

            ' Allocate a feature list.
            MIL.MblobAllocFeatureList(MilSystem, MilBlobFeatureList)

            ' Enable the Area and Center Of Gravity feature calculation.
            MIL.MblobSelectFeature(MilBlobFeatureList, MIL.M_AREA)
            MIL.MblobSelectFeature(MilBlobFeatureList, MIL.M_CENTER_OF_GRAVITY)

            ' Allocate a blob result buffer.
            MIL.MblobAllocResult(MilSystem, MilBlobResult)

            ' Calculate selected features for each blob.
            MIL.MblobCalculate(MilBinImage, MIL.M_NULL, MilBlobFeatureList, MilBlobResult)

            ' Exclude blobs whose area is too small.
            MIL.MblobSelect(MilBlobResult, MIL.M_EXCLUDE, MIL.M_AREA, MIL.M_LESS_OR_EQUAL, MIN_BLOB_AREA, MIL.M_NULL)

            ' Get the total number of selected blobs.
            MIL.MblobGetNumber(MilBlobResult, TotalBlobs)
            Console.Write("There are {0} objects ", TotalBlobs)

            ' Read and print the blob's center of gravity.
            CogX = New Double(CType((TotalBlobs - 1), Integer)) {}
            CogY = New Double(CType((TotalBlobs - 1), Integer)) {}
            If CogX IsNot Nothing AndAlso CogY IsNot Nothing Then
                ' Get the results.
                MIL.MblobGetResult(MilBlobResult, MIL.M_CENTER_OF_GRAVITY_X, CogX)
                MIL.MblobGetResult(MilBlobResult, MIL.M_CENTER_OF_GRAVITY_Y, CogY)

                ' Print the center of gravity of each blob.
                Console.Write("and their centers of gravity are:" + Constants.vbLf)
                For n = 0 To CType((TotalBlobs - 1), Integer)
                    Console.Write("Blob #{0}: X={1,5:0.0}, Y={2,5:0.0}" + Constants.vbLf, n, CogX(n), CogY(n))
                Next n

            Else
                Console.Write(Constants.vbLf + "Error: Not enough memory." + Constants.vbLf)
            End If

            ' Draw a cross at the center of gravity of each blob.
            MIL.MgraColor(MIL.M_DEFAULT, MIL.M_COLOR_RED)
            MIL.MblobDraw(MIL.M_DEFAULT, MilBlobResult, MilGraphicList, MIL.M_DRAW_CENTER_OF_GRAVITY, MIL.M_INCLUDED_BLOBS, MIL.M_DEFAULT)

            ' Reverse what is considered to be the background so that
            ' holes are seen as being blobs.

            MIL.MblobControl(MilBlobResult, MIL.M_FOREGROUND_VALUE, MIL.M_ZERO)

            ' Add a feature to distinguish between types of holes.Since area
            ' has already been added to the feature list, and the processing 
            ' mode has been changed, all blobs will be re-included and the area 
            ' of holes will be calculated automatically.

            MIL.MblobSelectFeature(MilBlobFeatureList, MIL.M_COMPACTNESS)

            ' Calculate selected features for each blob.
            MIL.MblobCalculate(MilBinImage, MIL.M_NULL, MilBlobFeatureList, MilBlobResult)

            ' Exclude small holes and large (the area around objects) holes.
            MIL.MblobSelect(MilBlobResult, MIL.M_EXCLUDE, MIL.M_AREA, MIL.M_OUT_RANGE, MIN_BLOB_AREA, MAX_BLOB_AREA)

            ' Get the number of blobs with holes.
            MIL.MblobGetNumber(MilBlobResult, BlobsWithHoles)

            ' Exclude blobs whose holes are compact (i.e.nuts).
            MIL.MblobSelect(MilBlobResult, MIL.M_EXCLUDE, MIL.M_COMPACTNESS, MIL.M_LESS_OR_EQUAL, MIN_COMPACTNESS, MIL.M_NULL)

            ' Get the number of blobs with holes that are NOT compact.
            MIL.MblobGetNumber(MilBlobResult, BlobsWithRoughHoles)

            ' Print results.
            Console.Write(Constants.vbLf + "Identified objects:" + Constants.vbLf)
            Console.Write("{0} bolts" + Constants.vbLf, TotalBlobs - BlobsWithHoles)
            Console.Write("{0} nuts" + Constants.vbLf, BlobsWithHoles - BlobsWithRoughHoles)
            Console.Write("{0} washers" + Constants.vbLf + Constants.vbLf, BlobsWithRoughHoles)
            Console.Write("Press <Enter> to end." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Free all allocations.
            MIL.MgraFree(MilGraphicList)
            MIL.MblobFree(MilBlobResult)
            MIL.MblobFree(MilBlobFeatureList)
            MIL.MbufFree(MilBinImage)
            MIL.MbufFree(MilImage)
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL)
        End Sub
    End Class
End Namespace
