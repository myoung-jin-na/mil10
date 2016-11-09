'**************************************************************************************
'
' File name: Mpat.vb
'
' Synopsis:  This program contains 3 examples of the pattern matching module:
'
'            The first example defines a model and then searches for it in a shifted
'            version of the image (without rotation).
'
'            The second example defines a model and then searches for it in a 
'            rotated version of the image.
'
'            The third example automatically allocates a model in a wafer image and finds 
'            its horizontal and vertical displacement.
'**************************************************************************************
Imports Microsoft.VisualBasic
Imports System
Imports System.Collections.Generic
Imports System.Text

Imports Matrox.MatroxImagingLibrary

Namespace MPat
    Friend Class Program
        Shared Sub Main(ByVal args() As String)
            Dim MilApplication As MIL_ID = MIL.M_NULL ' Application identifier.
            Dim MilSystem As MIL_ID = MIL.M_NULL ' System identifier.
            Dim MilDisplay As MIL_ID = MIL.M_NULL ' Display identifier.

            Console.Write(Constants.vbLf + "GRAYSCALE PATTERN MATCHING:" + Constants.vbLf)
            Console.Write("---------------------------" + Constants.vbLf + Constants.vbLf)

            ' Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, MilApplication, MilSystem, MilDisplay, CType(MIL.M_NULL, IntPtr), CType(MIL.M_NULL, IntPtr))

            ' Run the search at 0 degree example.
            SearchModelExample(MilSystem, MilDisplay)

            ' Run the search over 360 degrees example.
            SearchRotatedModelExample(MilSystem, MilDisplay)

            ' Run the automatic model allocation example.
            AutoAllocationModelExample(MilSystem, MilDisplay)

            ' Free defaults.
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL)
        End Sub

        '****************************************************************************
        ' Find model in shifted version of the image example.

        ' Source image file name.
        Private Const FIND_IMAGE_FILE As String = MIL.M_IMAGE_PATH & "CircuitsBoard.mim"

        ' Model position and size.
        Private Const FIND_MODEL_X_POS As Integer = 153
        Private Const FIND_MODEL_Y_POS As Integer = 132
        Private Const FIND_MODEL_WIDTH As Integer = 128
        Private Const FIND_MODEL_HEIGHT As Integer = 128
        Private Const FIND_MODEL_X_CENTER As Double = (FIND_MODEL_X_POS + (FIND_MODEL_WIDTH - 1) / 2.0)
        Private Const FIND_MODEL_Y_CENTER As Double = (FIND_MODEL_Y_POS + (FIND_MODEL_HEIGHT - 1) / 2.0)

        ' Target image shifting values.
        Private Const FIND_SHIFT_X As Double = 4.5
        Private Const FIND_SHIFT_Y As Double = 7.5

        ' Minimum match score to determine acceptability of model (default).
        Private Const FIND_MODEL_MIN_MATCH_SCORE As Double = 70.0

        ' Minimum accuracy for the search.
        Private Const FIND_MODEL_MIN_ACCURACY As Double = 0.1

        Private Shared Sub SearchModelExample(ByVal MilSystem As MIL_ID, ByVal MilDisplay As MIL_ID)
            Dim MilImage As MIL_ID = MIL.M_NULL ' Image buffer identifier.
            Dim GraphicList As MIL_ID = MIL.M_NULL ' Graphic list identifier.
            Dim Model As MIL_ID = MIL.M_NULL ' Model identifier.
            Dim Result As MIL_ID = MIL.M_NULL ' Result identifier.
            Dim XOrg As Double = 0.0 ' Original model position.
            Dim YOrg As Double = 0.0
            Dim x As Double = 0.0 ' Model position.
            Dim y As Double = 0.0
            Dim ErrX As Double = 0.0 ' Model error position.
            Dim ErrY As Double = 0.0
            Dim Score As Double = 0.0 ' Model correlation score.
            Dim Time As Double = 0.0 ' Model search time.
            Dim AnnotationColor As Double = MIL.M_COLOR_GREEN ' Drawing color.

            ' Restore source image into an automatically allocated image buffer.
            MIL.MbufRestore(FIND_IMAGE_FILE, MilSystem, MilImage)

            ' Display the image buffer.
            MIL.MdispSelect(MilDisplay, MilImage)

            ' Allocate a graphic list to hold the subpixel annotations to draw.
            MIL.MgraAllocList(MilSystem, MIL.M_DEFAULT, GraphicList)

            ' Associate the graphic list to the display for annotations.
            MIL.MdispControl(MilDisplay, MIL.M_ASSOCIATED_GRAPHIC_LIST_ID, GraphicList)


            ' Allocate a normalized grayscale model.
            MIL.MpatAllocModel(MilSystem, MilImage, FIND_MODEL_X_POS, FIND_MODEL_Y_POS, FIND_MODEL_WIDTH, FIND_MODEL_HEIGHT, MIL.M_NORMALIZED, Model)

            ' Set the search accuracy to high.
            MIL.MpatSetAccuracy(Model, MIL.M_HIGH)

            ' Set the search model speed to high.
            MIL.MpatSetSpeed(Model, MIL.M_HIGH)

            ' Preprocess the model.
            MIL.MpatPreprocModel(MilImage, Model, MIL.M_DEFAULT)

            ' Draw a box around the model in the model image.
            MIL.MgraColor(MIL.M_DEFAULT, AnnotationColor)
            MIL.MpatDraw(MIL.M_DEFAULT, Model, GraphicList, MIL.M_DRAW_BOX + MIL.M_DRAW_POSITION, MIL.M_DEFAULT, MIL.M_ORIGINAL)

            ' Pause to show the original image and model position.
            Console.Write(Constants.vbLf + "A {0}x{1} model was defined in the source image." + Constants.vbLf, FIND_MODEL_WIDTH, FIND_MODEL_HEIGHT)
            Console.Write("It will be found in an image shifted by {0:0.00} in X and {1:0.00} in Y." + Constants.vbLf, FIND_SHIFT_X, FIND_SHIFT_Y)
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Clear annotations.
            MIL.MgraClear(MIL.M_DEFAULT, GraphicList)

            ' Translate the image on a subpixel level.
            MIL.MimTranslate(MilImage, MilImage, FIND_SHIFT_X, FIND_SHIFT_Y, MIL.M_DEFAULT)

            ' Allocate result buffer.
            MIL.MpatAllocResult(MilSystem, 1, Result)

            ' Dummy first call for bench measure purpose only (bench stabilization, cache effect, etc...). This first call is NOT required by the application.
            MIL.MpatFindModel(MilImage, Model, Result)
            MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_RESET + MIL.M_SYNCHRONOUS, CType(MIL.M_NULL, IntPtr))

            ' Find the model in the target buffer.
            MIL.MpatFindModel(MilImage, Model, Result)

            ' Read the time spent in MpatFindModel.
            MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_RESET + MIL.M_SYNCHRONOUS, CType(MIL.M_NULL, IntPtr))

            ' If one model was found above the acceptance threshold.
            If MIL.MpatGetNumber(Result) = 1 Then
                ' Read results and draw a box around the model occurrence.
                MIL.MpatGetResult(Result, MIL.M_POSITION_X, x)
                MIL.MpatGetResult(Result, MIL.M_POSITION_Y, y)
                MIL.MpatGetResult(Result, MIL.M_SCORE, Score)
                MIL.MgraColor(MIL.M_DEFAULT, AnnotationColor)
                MIL.MpatDraw(MIL.M_DEFAULT, Result, GraphicList, MIL.M_DRAW_BOX + MIL.M_DRAW_POSITION, MIL.M_DEFAULT, MIL.M_DEFAULT)

                ' Calculate the position errors in X and Y and inquire original model position.
                ErrX = Math.Abs((FIND_MODEL_X_CENTER + FIND_SHIFT_X) - x)
                ErrY = Math.Abs((FIND_MODEL_Y_CENTER + FIND_SHIFT_Y) - y)
                MIL.MpatInquire(Model, MIL.M_ORIGINAL_X, XOrg)
                MIL.MpatInquire(Model, MIL.M_ORIGINAL_Y, YOrg)

                ' Print out the search result of the model in the original image.
                Console.Write("Search results:" + Constants.vbLf)
                Console.Write("---------------------------------------------------" + Constants.vbLf)
                Console.Write("The model is found to be shifted by " & Constants.vbTab & "X:{0:0.00}, Y:{1:0.00}." + Constants.vbLf, x - XOrg, y - YOrg)
                Console.Write("The model position error is " & Constants.vbTab + Constants.vbTab & "X:{0:0.00}, Y:{1:0.00}" + Constants.vbLf, ErrX, ErrY)
                Console.Write("The model match score is " & Constants.vbTab + Constants.vbTab & "{0:0.0}" + Constants.vbLf, Score)
                Console.Write("The search time is " & Constants.vbTab + Constants.vbTab + Constants.vbTab & "{0:0.000} ms" + Constants.vbLf + Constants.vbLf, Time * 1000.0)

                ' Verify the results.
                If (Math.Abs((x - XOrg) - FIND_SHIFT_X) > FIND_MODEL_MIN_ACCURACY) OrElse (Math.Abs((y - YOrg) - FIND_SHIFT_Y) > FIND_MODEL_MIN_ACCURACY) OrElse (Score < FIND_MODEL_MIN_MATCH_SCORE) Then
                    Console.Write("Results verification error !" + Constants.vbLf)
                End If
            Else
                Console.Write("Model not found !" + Constants.vbLf)
            End If

            ' Wait for a key to be pressed.
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Clear annotations.
            MIL.MgraClear(MIL.M_DEFAULT, GraphicList)

            ' Free all allocations.
            MIL.MgraFree(GraphicList)
            MIL.MpatFree(Result)
            MIL.MpatFree(Model)
            MIL.MbufFree(MilImage)
        End Sub

        '****************************************************************************
        ' Find rotated model example.

        ' Source image file name. 
        Private Const ROTATED_FIND_IMAGE_FILE As String = MIL.M_IMAGE_PATH & "CircuitsBoard.mim"

        ' Image rotation values.
        Private Const ROTATED_FIND_ROTATION_DELTA_ANGLE As Integer = 10
        Private Const ROTATED_FIND_ROTATION_ANGLE_STEP As Integer = 1
        Private Const ROTATED_FIND_RAD_PER_DEG As Double = 0.01745329251

        ' Model position and size.
        Private Const ROTATED_FIND_MODEL_X_POS As Integer = 153
        Private Const ROTATED_FIND_MODEL_Y_POS As Integer = 132
        Private Const ROTATED_FIND_MODEL_WIDTH As Integer = 128
        Private Const ROTATED_FIND_MODEL_HEIGHT As Integer = 128

        Private Const ROTATED_FIND_MODEL_X_CENTER As Double = ROTATED_FIND_MODEL_X_POS + (ROTATED_FIND_MODEL_WIDTH - 1) / 2.0
        Private Const ROTATED_FIND_MODEL_Y_CENTER As Double = ROTATED_FIND_MODEL_Y_POS + (ROTATED_FIND_MODEL_HEIGHT - 1) / 2.0

        ' Minimum accuracy for the search position.
        Private Const ROTATED_FIND_MIN_POSITION_ACCURACY As Double = 0.1

        ' Minimum accuracy for the search angle.
        Private Const ROTATED_FIND_MIN_ANGLE_ACCURACY As Double = 0.25

        ' Angle range to search.
        Private Const ROTATED_FIND_ANGLE_DELTA_POS As Integer = ROTATED_FIND_ROTATION_DELTA_ANGLE
        Private Const ROTATED_FIND_ANGLE_DELTA_NEG As Integer = ROTATED_FIND_ROTATION_DELTA_ANGLE

        Private Shared Sub SearchRotatedModelExample(ByVal MilSystem As MIL_ID, ByVal MilDisplay As MIL_ID)
            Dim MilSourceImage As MIL_ID = MIL.M_NULL ' Model image buffer identifier.
            Dim MilTargetImage As MIL_ID = MIL.M_NULL ' Target image buffer identifier.
            Dim MilDisplayImage As MIL_ID = MIL.M_NULL ' Target image buffer identifier.
            Dim GraphicList As MIL_ID = MIL.M_NULL ' Graphic list.
            Dim MilModel As MIL_ID = MIL.M_NULL ' Model identifier.
            Dim MilResult As MIL_ID = MIL.M_NULL ' Result identifier.
            Dim RealX As Double = 0.0 ' Model real position in x.
            Dim RealY As Double = 0.0 ' Model real position in y.
            Dim RealAngle As Double = 0.0 ' Model real angle.
            Dim X As Double = 0.0 ' Model position in x found.
            Dim Y As Double = 0.0 ' Model position in y found.
            Dim Angle As Double = 0.0 ' Model angle found.
            Dim Score As Double = 0.0 ' Model correlation score.
            Dim Time As Double = 0.0 ' Model search time.
            Dim ErrX As Double = 0.0 ' Model error position in x.
            Dim ErrY As Double = 0.0 ' Model error position in y.
            Dim ErrAngle As Double = 0.0 ' Model error angle.
            Dim SumErrX As Double = 0.0 ' Model total error position in x.
            Dim SumErrY As Double = 0.0 ' Model total error position in y.
            Dim SumErrAngle As Double = 0.0 ' Model total error angle.
            Dim SumTime As Double = 0.0 ' Model total search time.
            Dim NbFound As Integer = 0 ' Number of models found.
            Dim AnnotationColor As Double = MIL.M_COLOR_GREEN ' Drawing color.

            ' Load target image into image buffers and display it.
            MIL.MbufRestore(ROTATED_FIND_IMAGE_FILE, MilSystem, MilSourceImage)
            MIL.MbufRestore(ROTATED_FIND_IMAGE_FILE, MilSystem, MilTargetImage)
            MIL.MbufRestore(ROTATED_FIND_IMAGE_FILE, MilSystem, MilDisplayImage)
            MIL.MdispSelect(MilDisplay, MilDisplayImage)

            ' Allocate a graphic list to hold the subpixel annotations to draw.
            MIL.MgraAllocList(MilSystem, MIL.M_DEFAULT, GraphicList)

            ' Associate the graphic list to the display for annotations.
            MIL.MdispControl(MilDisplay, MIL.M_ASSOCIATED_GRAPHIC_LIST_ID, GraphicList)

            ' Allocate a normalized grayscale model.
            MIL.MpatAllocModel(MilSystem, MilSourceImage, ROTATED_FIND_MODEL_X_POS, ROTATED_FIND_MODEL_Y_POS, ROTATED_FIND_MODEL_WIDTH, ROTATED_FIND_MODEL_HEIGHT, MIL.M_NORMALIZED + MIL.M_CIRCULAR_OVERSCAN, MilModel)

            ' Set the search model speed.
            MIL.MpatSetSpeed(MilModel, MIL.M_MEDIUM)

            ' Set the position search accuracy.
            MIL.MpatSetAccuracy(MilModel, MIL.M_HIGH)

            ' Activate the search model angle mode.
            MIL.MpatSetAngle(MilModel, MIL.M_SEARCH_ANGLE_MODE, MIL.M_ENABLE)

            ' Set the search model range angle.
            MIL.MpatSetAngle(MilModel, MIL.M_SEARCH_ANGLE_DELTA_NEG, ROTATED_FIND_ANGLE_DELTA_NEG)
            MIL.MpatSetAngle(MilModel, MIL.M_SEARCH_ANGLE_DELTA_POS, ROTATED_FIND_ANGLE_DELTA_POS)

            ' Set the search model angle accuracy.
            MIL.MpatSetAngle(MilModel, MIL.M_SEARCH_ANGLE_ACCURACY, ROTATED_FIND_MIN_ANGLE_ACCURACY)

            ' Set the search model angle interpolation mode to bilinear.
            MIL.MpatSetAngle(MilModel, MIL.M_SEARCH_ANGLE_INTERPOLATION_MODE, MIL.M_BILINEAR)

            ' Preprocess the model.
            MIL.MpatPreprocModel(MilSourceImage, MilModel, MIL.M_DEFAULT)

            ' Allocate a result buffer.
            MIL.MpatAllocResult(MilSystem, 1, MilResult)

            ' Draw the original model position
            MIL.MpatDraw(MIL.M_DEFAULT, MilModel, GraphicList, MIL.M_DRAW_BOX + MIL.M_DRAW_POSITION, MIL.M_DEFAULT, MIL.M_ORIGINAL)

            ' Pause to show the original image and model position.
            Console.Write(Constants.vbLf + "A {0}x{1} model was defined in the source image." + Constants.vbLf, ROTATED_FIND_MODEL_WIDTH, ROTATED_FIND_MODEL_HEIGHT)
            Console.Write("It will be searched in images rotated from {0} degree to {1} degree." + Constants.vbLf, -ROTATED_FIND_ROTATION_DELTA_ANGLE, ROTATED_FIND_ROTATION_DELTA_ANGLE)
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Dummy first call for bench measure purpose only (bench stabilization, cache effect, etc...). This first call is NOT required by the application.
            MIL.MpatFindModel(MilSourceImage, MilModel, MilResult)

            ' If the model was found above the acceptance threshold.
            If MIL.MpatGetNumber(MilResult) = 1 Then
                ' Search for the model in images at different angles.
                RealAngle = ROTATED_FIND_ROTATION_DELTA_ANGLE
                Do While RealAngle >= -ROTATED_FIND_ROTATION_DELTA_ANGLE
                    ' Rotate the image from the model image to target image.
                    MIL.MimRotate(MilSourceImage, MilTargetImage, RealAngle, MIL.M_DEFAULT, MIL.M_DEFAULT, MIL.M_DEFAULT, MIL.M_DEFAULT, MIL.M_BILINEAR + MIL.M_OVERSCAN_CLEAR)

                    ' Reset the timer.
                    MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_RESET + MIL.M_SYNCHRONOUS, CType(MIL.M_NULL, IntPtr))

                    ' Find the model in the target image.
                    MIL.MpatFindModel(MilTargetImage, MilModel, MilResult)

                    ' Read the time spent in MpatFindModel().
                    MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ + MIL.M_SYNCHRONOUS, Time)

                    ' Clear the annotations.
                    MIL.MgraClear(MIL.M_DEFAULT, GraphicList)

                    ' If one model was found above the acceptance threshold.
                    If MIL.MpatGetNumber(MilResult) = 1 Then
                        ' Read results and draw a box around model occurrence.
                        MIL.MpatGetResult(MilResult, MIL.M_POSITION_X, X)
                        MIL.MpatGetResult(MilResult, MIL.M_POSITION_Y, Y)
                        MIL.MpatGetResult(MilResult, MIL.M_ANGLE, Angle)
                        MIL.MpatGetResult(MilResult, MIL.M_SCORE, Score)

                        MIL.MgraColor(MIL.M_DEFAULT, AnnotationColor)
                        MIL.MpatDraw(MIL.M_DEFAULT, MilResult, GraphicList, MIL.M_DRAW_BOX + MIL.M_DRAW_POSITION, MIL.M_DEFAULT, MIL.M_DEFAULT)

                        MIL.MbufCopy(MilTargetImage, MilDisplayImage)

                        ' Calculate the angle error and the position errors for statistics.
                        ErrAngle = CalculateAngleDist(Angle, RealAngle)

                        RotateModelCenter(MilSourceImage, RealX, RealY, RealAngle)
                        ErrX = Math.Abs(X - RealX)
                        ErrY = Math.Abs(Y - RealY)

                        SumErrAngle += ErrAngle
                        SumErrX += ErrX
                        SumErrY += ErrY
                        SumTime += Time
                        NbFound += 1

                        ' Verify the precision for the position and the angle.
                        If (ErrX > ROTATED_FIND_MIN_POSITION_ACCURACY) OrElse (ErrY > ROTATED_FIND_MIN_POSITION_ACCURACY) OrElse (ErrAngle > ROTATED_FIND_MIN_ANGLE_ACCURACY) Then
                            Console.Write("Model accuracy error at angle {0:0.0} !" + Constants.vbLf + Constants.vbLf, RealAngle)
                            Console.Write("Errors are X:{0:0.000}, Y:{1:0.000} and Angle:{2:0.00}" + Constants.vbLf + Constants.vbLf, ErrX, ErrY, ErrAngle)
                            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
                            Console.ReadKey()
                        End If
                    Else
                        Console.Write("Model was not found at angle {0:0.0} !" + Constants.vbLf + Constants.vbLf, RealAngle)
                        Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
                        Console.ReadKey()
                    End If

                    RealAngle -= ROTATED_FIND_ROTATION_ANGLE_STEP
                Loop

                ' Print out the search result statistics
                ' of the models found in rotated images.
                Console.Write(Constants.vbLf + "Search statistics for the model found in the rotated images." + Constants.vbLf)
                Console.Write("------------------------------------------------------------" + Constants.vbLf)
                Console.Write("The average position error is " & Constants.vbTab + Constants.vbTab & "X:{0:0.000}, Y:{1:0.000}" + Constants.vbLf, SumErrX / NbFound, SumErrY / NbFound)
                Console.Write("The average angle error is " & Constants.vbTab + Constants.vbTab & "{0:0.000}" + Constants.vbLf, SumErrAngle / NbFound)
                Console.Write("The average search time is " & Constants.vbTab + Constants.vbTab & "{0:0.000} ms" + Constants.vbLf + Constants.vbLf, SumTime * 1000.0 / NbFound)
            Else
                Console.Write("Model was not found!" + Constants.vbLf + Constants.vbLf)
            End If

            ' Wait for a key to be pressed. 
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Free all allocations.
            MIL.MgraFree(GraphicList)
            MIL.MpatFree(MilResult)
            MIL.MpatFree(MilModel)
            MIL.MbufFree(MilTargetImage)
            MIL.MbufFree(MilSourceImage)
            MIL.MbufFree(MilDisplayImage)
        End Sub


        ' Calculate the rotated center of the model to compare the accuracy with
        ' the center of the occurrence found during pattern matching.
        Private Shared Sub RotateModelCenter(ByVal Buffer As MIL_ID, ByRef X As Double, ByRef Y As Double, ByVal Angle As Double)
            Dim BufSizeX As MIL_INT = MIL.MbufInquire(Buffer, MIL.M_SIZE_X, MIL.M_NULL)
            Dim BufSizeY As MIL_INT = MIL.MbufInquire(Buffer, MIL.M_SIZE_Y, MIL.M_NULL)

            Dim RadAngle As Double = Angle * ROTATED_FIND_RAD_PER_DEG
            Dim CosAngle As Double = Math.Cos(RadAngle)
            Dim SinAngle As Double = Math.Sin(RadAngle)

            Dim OffSetX As Double = CDbl(BufSizeX - 1) / 2.0
            Dim OffSetY As Double = CDbl(BufSizeY - 1) / 2.0

            X = (ROTATED_FIND_MODEL_X_CENTER - OffSetX) * CosAngle + (ROTATED_FIND_MODEL_Y_CENTER - OffSetY) * SinAngle + OffSetX
            Y = (ROTATED_FIND_MODEL_Y_CENTER - OffSetY) * CosAngle - (ROTATED_FIND_MODEL_X_CENTER - OffSetX) * SinAngle + OffSetY
        End Sub


        ' Calculate the absolute difference between the real angle 
        ' and the angle found.
        Private Shared Function CalculateAngleDist(ByVal Angle1 As Double, ByVal Angle2 As Double) As Double
            Dim dist As Double = Math.Abs(Angle1 - Angle2)

            Do While dist >= 360.0
                dist -= 360.0
            Loop

            If dist > 180.0 Then
                dist = 360.0 - dist
            End If

            Return dist
        End Function

        '*****************************************************************************
        ' Automatic model allocation example.

        ' Source and target images file specifications.
        Private Const AUTO_MODEL_IMAGE_FILE As String = MIL.M_IMAGE_PATH & "Wafer.mim"
        Private Const AUTO_MODEL_TARGET_IMAGE_FILE As String = MIL.M_IMAGE_PATH & "WaferShifted.mim"

        ' Model width and height
        Private Const AUTO_MODEL_WIDTH As Integer = 64
        Private Const AUTO_MODEL_HEIGHT As Integer = 64

        Private Shared Sub AutoAllocationModelExample(ByVal MilSystem As MIL_ID, ByVal MilDisplay As MIL_ID)
            Dim MilImage As MIL_ID = MIL.M_NULL ' Image buffer identifier.
            Dim MilSubImage As MIL_ID = MIL.M_NULL ' Sub-image buffer identifier.
            Dim GraphicList As MIL_ID = MIL.M_NULL ' Graphic list.
            Dim Model As MIL_ID = MIL.M_NULL ' Model identifier.
            Dim Result As MIL_ID = MIL.M_NULL ' Result buffer identifier.
            Dim AllocError As MIL_INT = 0 ' Allocation error variable.

            Dim ImageWidth As MIL_INT = 0 ' Target image dimensions
            Dim ImageHeight As MIL_INT = 0

            Dim OrgX As Double = 0.0 ' Original center of model.
            Dim OrgY As Double = 0.0
            Dim x As Double = 0.0 ' Result variables.
            Dim y As Double = 0.0
            Dim Score As Double = 0.0
            Dim AnnotationColor As Double = MIL.M_COLOR_GREEN ' Drawing color.

            ' Load model image into an image buffer.
            MIL.MbufRestore(AUTO_MODEL_IMAGE_FILE, MilSystem, MilImage)

            ' Display the image.
            MIL.MdispSelect(MilDisplay, MilImage)

            ' Allocate a graphic list to hold the subpixel annotations to draw.
            MIL.MgraAllocList(MilSystem, MIL.M_DEFAULT, GraphicList)

            ' Associate the graphic list to the display for annotations.
            MIL.MdispControl(MilDisplay, MIL.M_ASSOCIATED_GRAPHIC_LIST_ID, GraphicList)

            ' Restrict the region to be processed to the bottom right corner of the image.
            MIL.MbufInquire(MilImage, MIL.M_SIZE_X, ImageWidth)
            MIL.MbufInquire(MilImage, MIL.M_SIZE_Y, ImageHeight)
            MIL.MbufChild2d(MilImage, ImageWidth / 2, ImageHeight / 2, ImageWidth / 2, ImageHeight / 2, MilSubImage)

            ' Add an offset to the drawings so they are aligned with the processed child image.
            MIL.MgraControl(MIL.M_DEFAULT, MIL.M_DRAW_OFFSET_X, CType(-(ImageWidth / 2), Double))
            MIL.MgraControl(MIL.M_DEFAULT, MIL.M_DRAW_OFFSET_Y, CType(-(ImageHeight / 2), Double))

            ' Automatically allocate a normalized grayscale type model.
            MIL.MpatAllocAutoModel(MilSystem, MilSubImage, AUTO_MODEL_WIDTH, AUTO_MODEL_HEIGHT, MIL.M_DEFAULT, MIL.M_DEFAULT, MIL.M_NORMALIZED, MIL.M_DEFAULT, Model)

            ' Set the search accuracy to high.
            MIL.MpatSetAccuracy(Model, MIL.M_HIGH)

            ' Check for that model allocation was successful.
            MIL.MappGetError(MIL.M_DEFAULT, MIL.M_CURRENT, AllocError)
            If AllocError = 0 Then
                ' Draw a box around the model.
                MIL.MgraColor(MIL.M_DEFAULT, AnnotationColor)
                MIL.MpatDraw(MIL.M_DEFAULT, Model, GraphicList, MIL.M_DRAW_BOX + MIL.M_DRAW_POSITION, MIL.M_DEFAULT, MIL.M_ORIGINAL)
                Console.Write("A model was automatically defined in the image." + Constants.vbLf)
                Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
                Console.ReadKey()

                ' Clear the annotations.
                MIL.MgraClear(MIL.M_DEFAULT, GraphicList)

                ' Load target image into an image buffer.
                MIL.MbufLoad(AUTO_MODEL_TARGET_IMAGE_FILE, MilImage)

                ' Allocate result.
                MIL.MpatAllocResult(MilSystem, 1, Result)

                ' Find model.
                MIL.MpatFindModel(MilSubImage, Model, Result)

                ' If one model was found above the acceptance threshold set.
                If MIL.MpatGetNumber(Result) = 1 Then
                    ' Get results.
                    MIL.MpatGetResult(Result, MIL.M_POSITION_X, x)
                    MIL.MpatGetResult(Result, MIL.M_POSITION_Y, y)
                    MIL.MpatGetResult(Result, MIL.M_SCORE, Score)

                    ' Draw a box around the occurrence.
                    MIL.MgraColor(MIL.M_DEFAULT, AnnotationColor)
                    MIL.MpatDraw(MIL.M_DEFAULT, Result, GraphicList, MIL.M_DRAW_BOX + MIL.M_DRAW_POSITION, MIL.M_DEFAULT, MIL.M_DEFAULT)

                    ' Analyze and print results.
                    MIL.MpatInquire(Model, MIL.M_ORIGINAL_X, OrgX)
                    MIL.MpatInquire(Model, MIL.M_ORIGINAL_Y, OrgY)
                    Console.Write("An image misaligned by 50 pixels in X and 20 pixels in Y was loaded." + Constants.vbLf + Constants.vbLf)
                    Console.Write("The image is found to be shifted by {0:0.00} in X, and {1:0.00} in Y." + Constants.vbLf, x - OrgX, y - OrgY)
                    Console.Write("Model match score is {0:0.0} percent." + Constants.vbLf, Score)
                    Console.Write("Press <Enter> to end." + Constants.vbLf + Constants.vbLf)
                    Console.ReadKey()
                Else
                    Console.Write("Error: Pattern not found properly." + Constants.vbLf)
                    Console.Write("Press <Enter> to end." + Constants.vbLf + Constants.vbLf)
                    Console.ReadKey()
                End If

                ' Free result buffer and model.
                MIL.MpatFree(Result)
                MIL.MpatFree(Model)
            Else
                Console.Write("Error: Automatic model definition failed." + Constants.vbLf)
                Console.Write("Press <Enter> to end." + Constants.vbLf + Constants.vbLf)
                Console.ReadKey()
            End If

            ' Remove the drawing offset.
            MIL.MgraControl(MIL.M_DEFAULT, MIL.M_DRAW_OFFSET_X, 0.0)
            MIL.MgraControl(MIL.M_DEFAULT, MIL.M_DRAW_OFFSET_Y, 0.0)

            ' Free the graphic list.
            MIL.MgraFree(GraphicList)

            ' Free child buffer and defaults.
            MIL.MbufFree(MilSubImage)
            MIL.MbufFree(MilImage)
        End Sub
    End Class
End Namespace
