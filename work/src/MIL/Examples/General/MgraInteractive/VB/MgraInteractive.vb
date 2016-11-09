'*****************************************************************************
' 
' File name: MgraInteractive.vb
'
' Synopsis:  This program uses the capabilities of MIL's interactive
'            graphics, along with the Blob analysis module, to count 
'            objects within a user-defined region.
'*****************************************************************************
Imports Microsoft.VisualBasic
Imports System
Imports System.Collections.Generic
Imports System.Text

Imports Matrox.MatroxImagingLibrary
Imports System.Runtime.InteropServices

Namespace MGraInteractive
    Public Class STestParameters
        Public MilDisplay As MIL_ID
        Public MilGraphicsList As MIL_ID
        Public MilGraphicsContext As MIL_ID
        Public MilBinImage As MIL_ID
        Public MilBlobFeatureList As MIL_ID
        Public MilBlobResult As MIL_ID
        Public RegionLabel As MIL_INT
    End Class

    Public Class Program
        Shared Sub Main(ByVal args() As String)
            Dim MilApplication As MIL_ID = MIL.M_NULL   ' Application identifier.
            Dim MilSystem As MIL_ID = MIL.M_NULL        ' System Identifier.
            Dim MilDisplay As MIL_ID = MIL.M_NULL       ' Display identifier.

            'Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, MilApplication, MilSystem, MilDisplay, CType(MIL.M_NULL, IntPtr), CType(MIL.M_NULL, IntPtr))

            'Print example name.
            Console.WriteLine()
            Console.WriteLine("INTERACTIVE REGIONS AND SUBPIXEL ANNOTATIONS:")
            Console.WriteLine("---------------------------------------------")
            Console.WriteLine()
            Console.WriteLine("This program determines the number of blobs in a region")
            Console.WriteLine("defined interactively by the user. The extracted blob's")
            Console.WriteLine("features are drawn with subpixel accuracy in a zoomable")
            Console.WriteLine("display.")
            Console.WriteLine()

            'Run Interactivity Example.
            InteractivityExample(MilSystem, MilDisplay)

            'Free defaults.
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL)

        End Sub

        '*****************************************************************************
        'Interactivity example.

        'Source MIL image file specification.
        Private Shared ReadOnly IMAGE_FILE As String = MIL.M_IMAGE_PATH & "Seals.mim"

        'Threshold for image binarization.
        Private Const IMAGE_THRESHOLD_VALUE As Integer = 110
        'Initial region parameters.
        Private Const RECTANGLE_POSITION_X As Integer = 160
        Private Const RECTANGLE_POSITION_Y As Integer = 310
        Private Const RECTANGLE_WIDTH As Integer = 200
        Private Const RECTANGLE_HEIGHT As Integer = 175
        Private Const RECTANGLE_ANGLE As Integer = 0

        Private Shared Sub InteractivityExample(ByVal MilSystem As MIL_ID, ByVal MilDisplay As MIL_ID)
            Dim MilImage As MIL_ID = MIL.M_NULL             ' Image buffer identifier.
            Dim MilGraphicsList As MIL_ID = MIL.M_NULL      ' Graphics list identifier.
            Dim MilGraphicsContext As MIL_ID = MIL.M_NULL   ' Graphics context identifier
            Dim MilBinImage As MIL_ID = MIL.M_NULL          ' Binary image buffer identifier.
            Dim MilBlobFeatureList As MIL_ID = MIL.M_NULL   ' Feature list identifier.
            Dim MilBlobResult As MIL_ID = MIL.M_NULL        ' Blob result buffer identifier.

            Dim SizeX As MIL_INT = 0                        ' Size X of the source buffer.
            Dim SizeY As MIL_INT = 0                        ' Size Y of the source buffer.
            Dim RegionLabel As MIL_INT                      ' Label value of the region.

            Dim DataStructure As New STestParameters()      ' Hook function data structure.

            ' Restore the source image.
            MIL.MbufRestore(IMAGE_FILE, MilSystem, MilImage)

            ' Display the buffer.
            MIL.MdispSelect(MilDisplay, MilImage)

            ' Allocate a graphics list to hold the subpixel annotations.
            MIL.MgraAllocList(MilSystem, MIL.M_DEFAULT, MilGraphicsList)

            ' Associate the graphics list to the display for annotations.
            MIL.MdispControl(MilDisplay, MIL.M_ASSOCIATED_GRAPHIC_LIST_ID, MilGraphicsList)

            ' Allocate a graphics context for the draw operations.
            MIL.MgraAlloc(MilSystem, MilGraphicsContext)

            ' Enable the interactive mode.
            MIL.MdispControl(MilDisplay, MIL.M_GRAPHIC_LIST_INTERACTIVE, MIL.M_ENABLE)

            ' Add a selectable rectangular region.
            MIL.MgraRectAngle(MilGraphicsContext, MilGraphicsList, RECTANGLE_POSITION_X, RECTANGLE_POSITION_Y, RECTANGLE_WIDTH, RECTANGLE_HEIGHT, RECTANGLE_ANGLE, MIL.M_CENTER_AND_DIMENSION)

            ' Retrieve the label of the rectangle graphic.
            MIL.MgraInquireList(MilGraphicsList, MIL.M_LIST, MIL.M_DEFAULT, MIL.M_LAST_LABEL, RegionLabel)

            ' Disable the selectable mode for the next annotations to the graphics list.
            MIL.MgraControl(MilGraphicsContext, MIL.M_SELECTABLE, MIL.M_DISABLE)

            ' Allocate a binary image buffer for fast processing.
            MIL.MbufInquire(MilImage, MIL.M_SIZE_X, SizeX)
            MIL.MbufInquire(MilImage, MIL.M_SIZE_Y, SizeY)
            MIL.MbufAlloc2d(MilSystem, SizeX, SizeY, 1 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_PROC, MilBinImage)

            ' Binarize the source image.
            MIL.MimBinarize(MilImage, MilBinImage, MIL.M_FIXED + MIL.M_LESS, IMAGE_THRESHOLD_VALUE, MIL.M_NULL)

            ' Allocate a blob feature list and a blob result.
            MIL.MblobAllocFeatureList(MilSystem, MilBlobFeatureList)
            MIL.MblobAllocResult(MilSystem, MilBlobResult)

            ' Select the blob features to calculate (Center Of Gravity and Box).
            MIL.MblobSelectFeature(MilBlobFeatureList, MIL.M_CENTER_OF_GRAVITY)
            MIL.MblobSelectFeature(MilBlobFeatureList, MIL.M_BOX)

            ' Programmatically initialize the selected state of the rectangle region.
            MIL.MgraControlList(MilGraphicsList, MIL.M_GRAPHIC_LABEL(RegionLabel), MIL.M_DEFAULT, MIL.M_GRAPHIC_SELECTED, MIL.M_TRUE)

            ' Perform and display a first count of the number of objects
            ' within the initial region.                                
            CountObjects(MilDisplay, MilGraphicsList, MilGraphicsContext, MilBinImage, MilBlobFeatureList, MilBlobResult)

            ' Initialize the hook data structure, then associate the hook function to
            ' the "MIL.M_GRAPHIC_MODIFIED" event. The hook function will be called
            ' with any region interaction by the user.                               
            DataStructure.MilDisplay = MilDisplay
            DataStructure.MilGraphicsList = MilGraphicsList
            DataStructure.MilGraphicsContext = MilGraphicsContext
            DataStructure.MilBinImage = MilBinImage
            DataStructure.RegionLabel = RegionLabel
            DataStructure.MilBlobFeatureList = MilBlobFeatureList
            DataStructure.MilBlobResult = MilBlobResult

            Dim DataStructureHandle As GCHandle = GCHandle.Alloc(DataStructure)
            Dim HookHandlerDelegate As MIL_GRA_HOOK_FUNCTION_PTR = New MIL_GRA_HOOK_FUNCTION_PTR(AddressOf HookHandler)
            MIL.MgraHookFunction(MilGraphicsList, MIL.M_GRAPHIC_MODIFIED, HookHandlerDelegate, GCHandle.ToIntPtr(DataStructureHandle))

            If (MIL.MdispInquire(MilDisplay, MIL.M_DISPLAY_TYPE, CType(MIL.M_NULL, IntPtr)) <> MIL.M_AUXILIARY) Then
                Console.WriteLine("You can try using your mouse to interactively modify the")
                Console.WriteLine("displayed region, such as moving, resizing, or rotating the")
                Console.WriteLine("region. If you do so, the results and annotations will be")
                Console.WriteLine("immediately updated")
                Console.WriteLine()
            Else
                Console.WriteLine("***NOTE: This example does not implement the interactivity")
                Console.WriteLine("         with an auxiliary display.")
                Console.WriteLine()
            End If
            Console.WriteLine("Press <Enter> to exit.")
            Console.ReadKey()

            MIL.MgraHookFunction(MilGraphicsList, MIL.M_GRAPHIC_MODIFIED + MIL.M_UNHOOK, HookHandlerDelegate, GCHandle.ToIntPtr(DataStructureHandle))
            DataStructureHandle.Free()

            ' Free all allocations.
            MIL.MblobFree(MilBlobResult)
            MIL.MblobFree(MilBlobFeatureList)
            MIL.MbufFree(MilBinImage)
            MIL.MgraFree(MilGraphicsContext)
            MIL.MgraFree(MilGraphicsList)
            MIL.MbufFree(MilImage)
        End Sub

        Private Shared Function HookHandler(ByVal HookType As MIL_INT, ByVal EventId As MIL_ID, ByVal UserDataPtr As IntPtr) As MIL_INT
            ' this is how to check if the user data is null, the IntPtr class
            ' contains a member, Zero, which exists solely for this purpose
            If (Not IntPtr.Zero.Equals(UserDataPtr)) Then
                ' get the handle to the DigHookUserData object back from the IntPtr
                Dim hUserData As GCHandle = GCHandle.FromIntPtr(UserDataPtr)

                ' get a reference to the DigHookUserData object
                Dim DataStructure As STestParameters = TryCast(hUserData.Target, STestParameters)

                ' Check that the modified graphic is the rectangular region.
                Dim ModifiedGraphicLabel As MIL_INT = 0
                MIL.MgraGetHookInfo(EventId, MIL.M_GRAPHIC_LABEL_VALUE, ModifiedGraphicLabel)

                If ModifiedGraphicLabel = DataStructure.RegionLabel Then

                    ' Count objects and draw the corresponding annotations.
                    CountObjects(DataStructure.MilDisplay, DataStructure.MilGraphicsList, DataStructure.MilGraphicsContext, DataStructure.MilBinImage, DataStructure.MilBlobFeatureList, DataStructure.MilBlobResult)
                End If
            End If
            Return MIL.M_NULL
        End Function


        Private Const MAX_TEXT_SIZE As Integer = 100

        Private Shared Sub CountObjects(ByVal MilDisplay As MIL_ID, ByVal MilGraphicList As MIL_ID, ByVal MilGraphicContext As MIL_ID, ByVal MilBinImage As MIL_ID, ByVal MilBlobFeatureList As MIL_ID, ByVal MilBlobResult As MIL_ID)
            Dim NumberOfBlobs As MIL_INT = 0
            Dim NumberOfPrimitives As MIL_INT = 0
            Dim Index As MIL_INT

            Dim TextLabel As String

            ' Disable the display update for better performance
            MIL.MdispControl(MilDisplay, MIL.M_UPDATE, MIL.M_DISABLE)

            ' Remove all elements from the graphics list, except the rectangle region primitive at index 0.
            MIL.MgraInquireList(MilGraphicList, MIL.M_LIST, MIL.M_DEFAULT, MIL.M_NUMBER_OF_GRAPHICS, NumberOfPrimitives)
            For Index = NumberOfPrimitives - 1 To 1 Step -1
                MIL.MgraControlList(MilGraphicList, MIL.M_GRAPHIC_INDEX(1), MIL.M_DEFAULT, MIL.M_DELETE, MIL.M_DEFAULT)
            Next

            ' Set the input region. The blob analysis will be done from the (filled) interactive rectangle.
            MIL.MbufSetRegion(MilBinImage, MilGraphicList, MIL.M_DEFAULT, MIL.M_RASTERIZE + MIL.M_FILL_REGION, MIL.M_DEFAULT)

            'Calculate the blobs and their features.
            MIL.MblobCalculate(MilBinImage, MIL.M_NULL, MilBlobFeatureList, MilBlobResult)

            ' Get the total number of blobs.
            MIL.MblobGetNumber(MilBlobResult, NumberOfBlobs)

            ' Set the input units to display unit for the count annotations.
            MIL.MgraControl(MilGraphicContext, MIL.M_INPUT_UNITS, MIL.M_DISPLAY)
            TextLabel = String.Format(" Number of blobs found: {0:00} ", NumberOfBlobs)

            MIL.MgraColor(MilGraphicContext, MIL.M_COLOR_WHITE)
            MIL.MgraText(MilGraphicContext, MilGraphicList, 10, 10, TextLabel)

            ' Restore the input units to pixel units for result annotations.
            MIL.MgraControl(MilGraphicContext, MIL.M_INPUT_UNITS, MIL.M_PIXEL)

            ' Draw blob center of gravity annotation.
            MIL.MgraColor(MilGraphicContext, MIL.M_COLOR_RED)
            MIL.MblobDraw(MilGraphicContext, MilBlobResult, MilGraphicList, MIL.M_DRAW_CENTER_OF_GRAVITY, MIL.M_INCLUDED_BLOBS, MIL.M_DEFAULT)

            ' Draw blob bounding box annotations.
            MIL.MgraColor(MilGraphicContext, MIL.M_COLOR_GREEN)
            MIL.MblobDraw(MilGraphicContext, MilBlobResult, MilGraphicList, MIL.M_DRAW_BOX, MIL.M_INCLUDED_BLOBS, MIL.M_DEFAULT)

            ' Enable the display to update the drawings.
            MIL.MdispControl(MilDisplay, MIL.M_UPDATE, MIL.M_ENABLE)

        End Sub
    End Class
End Namespace
