'************************************************************************************
'
' File name: Mmod.vb
' 
' Synopsis:  This program uses the Geometric Model Finder module to define geometric 
'            models and searches for these models in a target image. A simple single model 
'            example (1 model, 1 occurrence, good search conditions) is presented first, 
'            followed by a more complex example (multiple models, multiple occurrences in a
'            complex scene with bad search conditions).
'************************************************************************************
Imports Microsoft.VisualBasic
Imports System
Imports System.Collections.Generic
Imports System.Text

Imports Matrox.MatroxImagingLibrary

Namespace MMod
    Friend Class Program
        Shared Sub Main(ByVal args() As String)
            Dim MilApplication As MIL_ID = MIL.M_NULL ' Application identifier.
            Dim MilSystem As MIL_ID = MIL.M_NULL ' System identifier.
            Dim MilDisplay As MIL_ID = MIL.M_NULL ' Display identifier.

            ' Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, MilApplication, MilSystem, MilDisplay, CType(MIL.M_NULL, IntPtr), CType(MIL.M_NULL, IntPtr))

            ' Run single model example.
            SingleModelExample(MilSystem, MilDisplay)

            ' Run multiple model example.
            MultipleModelExample(MilSystem, MilDisplay)

            ' Free defaults
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL)
        End Sub

        '*****************************************************************************
        ' Single model example.
        '
        ' Source MIL image file specifications.
        Private Const SINGLE_MODEL_IMAGE As String = MIL.M_IMAGE_PATH & "SingleModel.mim"

        ' Target MIL image file specifications.
        Private Const SINGLE_MODEL_TARGET_IMAGE As String = MIL.M_IMAGE_PATH & "SingleTarget.mim"

        'Search speed: M_VERY_HIGH for faster search, M_MEDIUM for precision and robustness.
        Private Const SINGLE_MODEL_SEARCH_SPEED As Integer = MIL.M_VERY_HIGH

        ' Model specifications.
        Private Const MODEL_OFFSETX As Integer = 176
        Private Const MODEL_OFFSETY As Integer = 136
        Private Const MODEL_SIZEX As Integer = 128
        Private Const MODEL_SIZEY As Integer = 128
        Private Const MODEL_MAX_OCCURRENCES As Integer = 16

        Private Shared Sub SingleModelExample(ByVal MilSystem As MIL_ID, ByVal MilDisplay As MIL_ID)
            Dim MilImage As MIL_ID = MIL.M_NULL                 ' Image buffer identifier.
            Dim GraphicList As MIL_ID = MIL.M_NULL              ' Graphic list identifier
            Dim MilSearchContext As MIL_ID = MIL.M_NULL         ' Search context.
            Dim MilResult As MIL_ID = MIL.M_NULL                ' Result identifier.
            Dim ModelDrawColor As Double = MIL.M_COLOR_RED      ' Model draw color.
            Dim Model(MODEL_MAX_OCCURRENCES - 1) As MIL_INT     ' Model index.
            Dim NumResults As Integer = 0                       ' Number of results found. double[] Score = New double[MODEL_MAX_OCCURRENCES]; ' Model correlation score.
            Dim Score(MODEL_MAX_OCCURRENCES - 1) As Double
            Dim XPosition(MODEL_MAX_OCCURRENCES - 1) As Double  ' Model X position.
            Dim YPosition(MODEL_MAX_OCCURRENCES - 1) As Double  ' Model Y position.
            Dim Angle(MODEL_MAX_OCCURRENCES - 1) As Double      ' Model occurrence angle.
            Dim Scale(MODEL_MAX_OCCURRENCES - 1) As Double      ' Model occurrence scale.
            Dim Time As Double = 0.0                            ' Bench variable.
            Dim i As Integer = 0                                ' Loop variable.

            ' Restore the model image and display it.
            MIL.MbufRestore(SINGLE_MODEL_IMAGE, MilSystem, MilImage)
            MIL.MdispSelect(MilDisplay, MilImage)

            ' Allocate a graphic list to hold the subpixel annotations to draw.
            MIL.MgraAllocList(MilSystem, MIL.M_DEFAULT, GraphicList)

            ' Associate the graphic list to the display for annotations.
            MIL.MdispControl(MilDisplay, MIL.M_ASSOCIATED_GRAPHIC_LIST_ID, GraphicList)

            ' Allocate a Geometric Model Finder context.
            MIL.MmodAlloc(MilSystem, MIL.M_GEOMETRIC, MIL.M_DEFAULT, MilSearchContext)

            ' Allocate a result buffer.
            MIL.MmodAllocResult(MilSystem, MIL.M_DEFAULT, MilResult)

            ' Define the model.
            MIL.MmodDefine(MilSearchContext, MIL.M_IMAGE, MilImage, MODEL_OFFSETX, MODEL_OFFSETY, MODEL_SIZEX, MODEL_SIZEY)

            ' Set the search speed.
            MIL.MmodControl(MilSearchContext, MIL.M_CONTEXT, MIL.M_SPEED, SINGLE_MODEL_SEARCH_SPEED)

            ' Preprocess the search context.
            MIL.MmodPreprocess(MilSearchContext, MIL.M_DEFAULT)

            ' Draw box and position it in the source image to show the model.
            MIL.MgraColor(MIL.M_DEFAULT, ModelDrawColor)
            MIL.MmodDraw(MIL.M_DEFAULT, MilSearchContext, GraphicList, MIL.M_DRAW_BOX + MIL.M_DRAW_POSITION, 0, MIL.M_ORIGINAL)

            ' Pause to show the model.
            Console.Write(Constants.vbLf + "GEOMETRIC MODEL FINDER:" + Constants.vbLf)
            Console.Write("-----------------------" + Constants.vbLf + Constants.vbLf)
            Console.Write("A model context was defined with the model in the displayed image." + Constants.vbLf)
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Clear annotations.
            MIL.MgraClear(MIL.M_DEFAULT, GraphicList)

            ' Load the single model target image.
            MIL.MbufLoad(SINGLE_MODEL_TARGET_IMAGE, MilImage)

            ' Dummy first call for bench measure purpose only (bench stabilization, cache effect, etc...). This first call is NOT required by the application.
            MIL.MmodFind(MilSearchContext, MilImage, MilResult)

            ' Reset the timer.
            MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_RESET + MIL.M_SYNCHRONOUS, CType(MIL.M_NULL, IntPtr))

            ' Find the model.
            MIL.MmodFind(MilSearchContext, MilImage, MilResult)

            ' Read the find time.
            MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ + MIL.M_SYNCHRONOUS, Time)

            ' Get the number of models found.
            MIL.MmodGetResult(MilResult, MIL.M_DEFAULT, MIL.M_NUMBER + MIL.M_TYPE_MIL_INT, NumResults)

            ' If a model was found above the acceptance threshold.
            If (NumResults >= 1) AndAlso (NumResults <= MODEL_MAX_OCCURRENCES) Then
                ' Get the results of the single model.
                MIL.MmodGetResult(MilResult, MIL.M_DEFAULT, MIL.M_INDEX + MIL.M_TYPE_MIL_INT, Model)
                MIL.MmodGetResult(MilResult, MIL.M_DEFAULT, MIL.M_POSITION_X, XPosition)
                MIL.MmodGetResult(MilResult, MIL.M_DEFAULT, MIL.M_POSITION_Y, YPosition)
                MIL.MmodGetResult(MilResult, MIL.M_DEFAULT, MIL.M_ANGLE, Angle)
                MIL.MmodGetResult(MilResult, MIL.M_DEFAULT, MIL.M_SCALE, Scale)
                MIL.MmodGetResult(MilResult, MIL.M_DEFAULT, MIL.M_SCORE, Score)

                ' Print the results for each model found.
                Console.Write("The model was found in the target image:" + Constants.vbLf + Constants.vbLf)
                Console.Write("Result   Model   X Position   Y Position   Angle   Scale   Score" + Constants.vbLf + Constants.vbLf)
                For i = 0 To CType((NumResults - 1), Integer)
                    Console.Write("{0,-9}{1,-8}{2,-13:0.00}{3,-13:0.00}{4,-8:0.00}{5,-8:0.00}{6,-5:0.00}%" + Constants.vbLf, i, Model(i), XPosition(i), YPosition(i), Angle(i), Scale(i), Score(i))
                Next i
                Console.Write(Constants.vbLf + "The search time is {0:0.0} ms" + Constants.vbLf + Constants.vbLf, Time * 1000.0)

                ' Draw edges, position and box over the occurrences that were found.
                For i = 0 To CType((NumResults - 1), Integer)
                    MIL.MgraColor(MIL.M_DEFAULT, ModelDrawColor)
                    MIL.MmodDraw(MIL.M_DEFAULT, MilResult, GraphicList, MIL.M_DRAW_EDGES + MIL.M_DRAW_BOX + MIL.M_DRAW_POSITION, i, MIL.M_DEFAULT)
                Next i
            Else
                Console.Write("The model was not found or the number of models found is greater than" + Constants.vbLf)
                Console.Write("the specified maximum number of occurrence !" + Constants.vbLf + Constants.vbLf)
            End If

            ' Wait for a key to be pressed.
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Free MIL objects.
            MIL.MgraFree(GraphicList)
            MIL.MbufFree(MilImage)
            MIL.MmodFree(MilSearchContext)
            MIL.MmodFree(MilResult)

        End Sub

        '****************************************************************************
        ' Multiple models example.
        ' Source MIL image file specifications.
        Private Const MULTI_MODELS_IMAGE As String = MIL.M_IMAGE_PATH & "MultipleModel.mim"

        ' Target MIL image file specifications.
        Private Const MULTI_MODELS_TARGET_IMAGE As String = MIL.M_IMAGE_PATH & "MultipleTarget.mim"

        ' Search speed:  M_VERY_HIGH for faster search, M_MEDIUM for precision and robustness.
        Private Const MULTI_MODELS_SEARCH_SPEED As Integer = MIL.M_VERY_HIGH

        ' Number of models.
        Private Const NUMBER_OF_MODELS As Integer = 3
        Private Const MODELS_MAX_OCCURRENCES As Integer = 16

        ' Model 1 specifications.
        Private Const MODEL0_OFFSETX As Integer = 34
        Private Const MODEL0_OFFSETY As Integer = 93
        Private Const MODEL0_SIZEX As Integer = 214
        Private Const MODEL0_SIZEY As Integer = 76
        Private Shared ReadOnly MODEL0_DRAW_COLOR As Integer = MIL.M_COLOR_RED

        ' Model 2 specifications.
        Private Const MODEL1_OFFSETX As Integer = 73
        Private Const MODEL1_OFFSETY As Integer = 232
        Private Const MODEL1_SIZEX As Integer = 150
        Private Const MODEL1_SIZEY As Integer = 154
        Private Const MODEL1_REFERENCEX As Integer = 23
        Private Const MODEL1_REFERENCEY As Integer = 127
        Private Shared ReadOnly MODEL1_DRAW_COLOR As Integer = MIL.M_COLOR_GREEN

        ' Model 3 specifications.
        Private Const MODEL2_OFFSETX As Integer = 308
        Private Const MODEL2_OFFSETY As Integer = 39
        Private Const MODEL2_SIZEX As Integer = 175
        Private Const MODEL2_SIZEY As Integer = 357
        Private Const MODEL2_REFERENCEX As Integer = 62
        Private Const MODEL2_REFERENCEY As Integer = 150
        Private Shared ReadOnly MODEL2_DRAW_COLOR As Integer = MIL.M_COLOR_BLUE

        ' Models array size.
        Private Const MODELS_ARRAY_SIZE As Integer = 3

        Private Shared Sub MultipleModelExample(ByVal MilSystem As MIL_ID, ByVal MilDisplay As MIL_ID)
            Dim MilImage As MIL_ID = MIL.M_NULL                     ' Image buffer identifier.
            Dim GraphicList As MIL_ID = MIL.M_NULL                  ' Graphic list identifier.
            Dim MilSearchContext As MIL_ID = MIL.M_NULL             ' Search context.
            Dim MilResult As MIL_ID = MIL.M_NULL                    ' Result identifier.
            Dim ModelsDrawColor(MODELS_ARRAY_SIZE - 1) As Double    ' Model drawing colors.
            Dim NumResults As MIL_INT = 0                           ' Number of results found.
            Dim Models(MODELS_MAX_OCCURRENCES - 1) As MIL_INT       ' Model indices.
            Dim ModelsOffsetX(MODELS_ARRAY_SIZE - 1) As MIL_INT     ' Model X offsets array.
            Dim ModelsOffsetY(MODELS_ARRAY_SIZE - 1) As MIL_INT     ' Model Y offsets array.
            Dim ModelsSizeX(MODELS_ARRAY_SIZE - 1) As MIL_INT       ' Model X sizes array.
            Dim ModelsSizeY(MODELS_ARRAY_SIZE - 1) As MIL_INT       ' Model Y sizes array.
            Dim Score(MODELS_MAX_OCCURRENCES - 1) As Double         ' Model correlation scores.
            Dim XPosition(MODELS_MAX_OCCURRENCES - 1) As Double     ' Model X positions.
            Dim YPosition(MODELS_MAX_OCCURRENCES - 1) As Double     ' Model Y positions.
            Dim Angle(MODELS_MAX_OCCURRENCES - 1) As Double         ' Model occurrence angles.
            Dim Scale(MODELS_MAX_OCCURRENCES - 1) As Double         ' Model occurrence scales.
            Dim Time As Double = 0.0                                ' Time variable.
            Dim i As Integer                                        ' Loop variable.

            ' Models array specifications.
            ModelsOffsetX(0) = MODEL0_OFFSETX
            ModelsOffsetX(1) = MODEL1_OFFSETX
            ModelsOffsetX(2) = MODEL2_OFFSETX

            ModelsOffsetY(0) = MODEL0_OFFSETY
            ModelsOffsetY(1) = MODEL1_OFFSETY
            ModelsOffsetY(2) = MODEL2_OFFSETY

            ModelsSizeX(0) = MODEL0_SIZEX
            ModelsSizeX(1) = MODEL1_SIZEX
            ModelsSizeX(2) = MODEL2_SIZEX

            ModelsSizeY(0) = MODEL0_SIZEY
            ModelsSizeY(1) = MODEL1_SIZEY
            ModelsSizeY(2) = MODEL2_SIZEY

            ModelsDrawColor(0) = MODEL0_DRAW_COLOR
            ModelsDrawColor(1) = MODEL1_DRAW_COLOR
            ModelsDrawColor(2) = MODEL2_DRAW_COLOR

            ' Restore the model image and display it.
            MIL.MbufRestore(MULTI_MODELS_IMAGE, MilSystem, MilImage)
            MIL.MdispSelect(MilDisplay, MilImage)

            ' Allocate a graphic list to hold the subpixel annotations to draw.
            MIL.MgraAllocList(MilSystem, MIL.M_DEFAULT, GraphicList)

            ' Associate the graphic list to the display for annotations.
            MIL.MdispControl(MilDisplay, MIL.M_ASSOCIATED_GRAPHIC_LIST_ID, GraphicList)

            ' Allocate a geometric model finder.
            MIL.MmodAlloc(MilSystem, MIL.M_GEOMETRIC, MIL.M_DEFAULT, MilSearchContext)

            ' Allocate a result buffer.
            MIL.MmodAllocResult(MilSystem, MIL.M_DEFAULT, MilResult)

            ' Define the models.
            For i = 0 To NUMBER_OF_MODELS - 1
                MIL.MmodDefine(MilSearchContext, MIL.M_IMAGE, MilImage, CDbl(ModelsOffsetX(i)), CDbl(ModelsOffsetY(i)), CDbl(ModelsSizeX(i)), CDbl(ModelsSizeY(i)))
            Next i

            ' Set the desired search speed.
            MIL.MmodControl(MilSearchContext, MIL.M_CONTEXT, MIL.M_SPEED, MULTI_MODELS_SEARCH_SPEED)

            ' Increase the smoothness for the edge extraction in the search context.
            MIL.MmodControl(MilSearchContext, MIL.M_CONTEXT, MIL.M_SMOOTHNESS, 75)

            ' Modify the acceptance and the certainty for all the models that were defined.
            MIL.MmodControl(MilSearchContext, MIL.M_DEFAULT, MIL.M_ACCEPTANCE, 40)
            MIL.MmodControl(MilSearchContext, MIL.M_DEFAULT, MIL.M_CERTAINTY, 60)

            ' Set the number of occurrences to 2 for all the models that were defined.
            MIL.MmodControl(MilSearchContext, MIL.M_DEFAULT, MIL.M_NUMBER, 2)

            If NUMBER_OF_MODELS > 1 Then
                ' Change the reference point of the second model.
                MIL.MmodControl(MilSearchContext, 1, MIL.M_REFERENCE_X, MODEL1_REFERENCEX)
                MIL.MmodControl(MilSearchContext, 1, MIL.M_REFERENCE_Y, MODEL1_REFERENCEY)
            End If

            If NUMBER_OF_MODELS > 2 Then
                ' Change the reference point of the third model.
                MIL.MmodControl(MilSearchContext, 2, MIL.M_REFERENCE_X, MODEL2_REFERENCEX)
                MIL.MmodControl(MilSearchContext, 2, MIL.M_REFERENCE_Y, MODEL2_REFERENCEY)
            End If
            ' Preprocess the search context.
            MIL.MmodPreprocess(MilSearchContext, MIL.M_DEFAULT)

            ' Draw boxes and positions in the source image to identify the models.
            For i = 0 To NUMBER_OF_MODELS - 1
                MIL.MgraColor(MIL.M_DEFAULT, ModelsDrawColor(i))
                MIL.MmodDraw(MIL.M_DEFAULT, MilSearchContext, GraphicList, MIL.M_DRAW_BOX + MIL.M_DRAW_POSITION, i, MIL.M_ORIGINAL)
            Next i

            ' Pause to show the models.
            Console.Write("A model context was defined with the models in the displayed image." + Constants.vbLf)
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Clear the overlay image.
            MIL.MgraClear(MIL.M_DEFAULT, GraphicList)

            ' Load the complex target image.
            MIL.MbufLoad(MULTI_MODELS_TARGET_IMAGE, MilImage)

            ' Dummy first call for bench measure purpose only (bench stabilization, cache effect, etc...). This first call is NOT required by the application.
            MIL.MmodFind(MilSearchContext, MilImage, MilResult)

            ' Reset the timer.
            MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_RESET + MIL.M_SYNCHRONOUS, CType(MIL.M_NULL, IntPtr))

            ' Find the models.
            MIL.MmodFind(MilSearchContext, MilImage, MilResult)

            ' Read the find time.
            MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ + MIL.M_SYNCHRONOUS, Time)

            ' Get the number of models found.
            MIL.MmodGetResult(MilResult, MIL.M_DEFAULT, MIL.M_NUMBER + MIL.M_TYPE_MIL_INT, NumResults)

            ' If the models were found above the acceptance threshold.
            If (NumResults >= 1) AndAlso (NumResults <= MODELS_MAX_OCCURRENCES) Then
                ' Get the results for each model.
                MIL.MmodGetResult(MilResult, MIL.M_DEFAULT, MIL.M_INDEX + MIL.M_TYPE_MIL_INT, Models)
                MIL.MmodGetResult(MilResult, MIL.M_DEFAULT, MIL.M_POSITION_X, XPosition)
                MIL.MmodGetResult(MilResult, MIL.M_DEFAULT, MIL.M_POSITION_Y, YPosition)
                MIL.MmodGetResult(MilResult, MIL.M_DEFAULT, MIL.M_ANGLE, Angle)
                MIL.MmodGetResult(MilResult, MIL.M_DEFAULT, MIL.M_SCALE, Scale)
                MIL.MmodGetResult(MilResult, MIL.M_DEFAULT, MIL.M_SCORE, Score)

                ' Print information about the target image.
                Console.Write("The models were found in the target image although there is:" + Constants.vbLf)
                Console.Write("   Full rotation" + Constants.vbLf + "   Small scale change" + Constants.vbLf + "   Contrast variation" + Constants.vbLf)
                Console.Write("   Specular reflection" + Constants.vbLf + "   Occlusion" + Constants.vbLf + "   Multiple models" + Constants.vbLf)
                Console.Write("   Multiple occurrences" + Constants.vbLf + Constants.vbLf)

                ' Print the results for the found models.
                Console.Write("Result   Model   X Position   Y Position   Angle   Scale   Score" + Constants.vbLf + Constants.vbLf)
                For i = 0 To CType((NumResults - 1), Integer)
                    Console.Write("{0,-9}{1,-8}{2,-13:0.00}{3,-13:#.00}{4,-8:0.00}{5,-8:0.00}{6,-5:0.00}%" + Constants.vbLf, i, Models(i), XPosition(i), YPosition(i), Angle(i), Scale(i), Score(i))
                Next i
                Console.Write(Constants.vbLf + "The search time is {0:0.0} ms" + Constants.vbLf + Constants.vbLf, Time * 1000.0)

                ' Draw edges and positions over the occurrences that were found.
                For i = 0 To CType((NumResults - 1), Integer)
                    MIL.MgraColor(MIL.M_DEFAULT, ModelsDrawColor(CType(Models(i), Integer)))
                    MIL.MmodDraw(MIL.M_DEFAULT, MilResult, GraphicList, MIL.M_DRAW_EDGES + MIL.M_DRAW_POSITION, i, MIL.M_DEFAULT)
                Next i
            Else
                Console.Write("The models were not found or the number of models found is greater than" + Constants.vbLf)
                Console.Write("the defined value of maximum occurrences !" + Constants.vbLf + Constants.vbLf)
            End If

            ' Wait for a key to be pressed.
            Console.Write("Press <Enter> to end." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Free MIL objects.
            MIL.MgraFree(GraphicList)
            MIL.MbufFree(MilImage)
            MIL.MmodFree(MilSearchContext)
            MIL.MmodFree(MilResult)
        End Sub
    End Class
End Namespace
