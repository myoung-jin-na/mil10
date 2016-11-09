'*********************************************************************************
'
' File name:  MmodelTracking.vb
'
' Synopsis:  This program shows how to track a unique object using 
'            pattern recognition. It allocates a model in the field of 
'            view of the camera and finds it in a loop. It also prints 
'            the coordinates of the found model and draws a box around it.
'            It searches using 2 methods, the normalized grayscale 
'            correlation (MIL.Mpat), which is very fast and with the Geometric 
'            Model Finder (MIL.Mmod), which is independent of the model rotation 
'            and scale but slower.
'
'            Note:  Display update and annotations drawing can require
'                   significant CPU usage.
'*********************************************************************************
Imports Microsoft.VisualBasic
Imports System
Imports System.Collections.Generic
Imports System.Text

Imports Matrox.MatroxImagingLibrary

Namespace MModelTracking
    Friend Class Program
        ' Model specification.
        Private Const MODEL_WIDTH As Integer = 128
        Private Const MODEL_HEIGHT As Integer = 128

        Private Shared Function MODEL_POS_X_INIT(ByVal TargetImage As MIL_ID) As MIL_INT
            Return CType((MIL.MbufInquire(TargetImage, MIL.M_SIZE_X, MIL.M_NULL) / 2), MIL_INT)
        End Function
        Private Shared Function MODEL_POS_Y_INIT(ByVal TargetImage As MIL_ID) As MIL_INT
            Return CType((MIL.MbufInquire(TargetImage, MIL.M_SIZE_Y, MIL.M_NULL) / 2), MIL_INT)
        End Function


        ' Minimum score to consider the object found (in percent).
        Private Const MODEL_MIN_MATCH_SCORE As Double = 50.0

        ' Drawing color
        Private Const DRAW_COLOR As Integer = &HFF ' White

        ' Example selection.
        Private Const RUN_PAT_TRACKING_EXAMPLE As Integer = MIL.M_YES
        Private Const RUN_MOD_TRACKING_EXAMPLE As Integer = MIL.M_YES

        '****************************************************************************
        ' Main.
        '****************************************************************************
        Shared Sub Main(ByVal args() As String)
            Dim MilApplication As MIL_ID = MIL.M_NULL ' Application identifier.
            Dim MilSystem As MIL_ID = MIL.M_NULL ' System identifier.
            Dim MilDisplay As MIL_ID = MIL.M_NULL ' Display identifier.
            Dim MilDigitizer As MIL_ID = MIL.M_NULL ' Digitizer identifier.
            Dim MilDisplayImage As MIL_ID = MIL.M_NULL ' Display image identifier.
            Dim MilModelImage As MIL_ID = MIL.M_NULL ' Model image identifier.

            ' Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, MilApplication, MilSystem, MilDisplay, MilDigitizer, MilDisplayImage)

            ' Allocate a model image buffer.
            MIL.MbufAlloc2d(MilSystem, MIL.MbufInquire(MilDisplayImage, MIL.M_SIZE_X, MIL.M_NULL), MIL.MbufInquire(MilDisplayImage, MIL.M_SIZE_Y, MIL.M_NULL), 8, MIL.M_IMAGE + MIL.M_PROC, MilModelImage)

            Console.Write(Constants.vbLf + "MODEL TRACKING:" + Constants.vbLf)
            Console.Write("---------------" + Constants.vbLf + Constants.vbLf)

            ' Get the model image.
            GetModelImage(MilSystem, MilDisplay, MilDigitizer, MilDisplayImage, MilModelImage)

            If RUN_PAT_TRACKING_EXAMPLE = MIL.M_YES Then
                ' Finds the model using pattern matching.
                MpatTrackingExample(MilSystem, MilDisplay, MilDigitizer, MilDisplayImage, MilModelImage)
            End If

            If RUN_MOD_TRACKING_EXAMPLE = MIL.M_YES Then
                ' Finds the model using geometric model finder.
                MmodTrackingExample(MilSystem, MilDisplay, MilDigitizer, MilDisplayImage, MilModelImage)
            End If

            ' Free allocated buffers.
            MIL.MbufFree(MilModelImage)

            ' Free defaults.
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MilDigitizer, MilDisplayImage)
        End Sub

        '****************************************************************************
        ' Get Model Image Function.
        '****************************************************************************
        Private Shared Sub GetModelImage(ByVal MilSystem As MIL_ID, ByVal MilDisplay As MIL_ID, ByVal MilDigitizer As MIL_ID, ByVal MilDisplayImage As MIL_ID, ByVal MilModelImage As MIL_ID)
            Dim MilOverlayImage As MIL_ID = MIL.M_NULL ' Overlay image.
            Dim DrawColor As Double = DRAW_COLOR ' Drawing color.

            ' Prepare for overlay annotations.
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY, MIL.M_ENABLE)
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY_CLEAR, MIL.M_DEFAULT)
            MIL.MdispInquire(MilDisplay, MIL.M_OVERLAY_ID, MilOverlayImage)

            ' Draw the position of the model to define in the overlay.
            MIL.MgraColor(MIL.M_DEFAULT, DrawColor)
            MIL.MgraRect(MIL.M_DEFAULT, MilOverlayImage, MODEL_POS_X_INIT(MilOverlayImage) - CType((MODEL_WIDTH / 2), MIL_INT), MODEL_POS_Y_INIT(MilOverlayImage) - CType((MODEL_HEIGHT / 2), MIL_INT), MODEL_POS_X_INIT(MilOverlayImage) + CType((MODEL_WIDTH / 2), MIL_INT), MODEL_POS_Y_INIT(MilOverlayImage) + CType((MODEL_HEIGHT / 2), MIL_INT))

            ' Grab continuously.
            Console.Write("Model definition:" + Constants.vbLf + Constants.vbLf)
            Console.Write("Place a unique model to find in the marked rectangle." + Constants.vbLf)
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)

            ' Grab a reference model image.
            MIL.MdigGrabContinuous(MilDigitizer, MilDisplayImage)
            Console.ReadKey()
            MIL.MdigHalt(MilDigitizer)

            ' Copy the grabbed image to the Model image to keep it.
            MIL.MbufCopy(MilDisplayImage, MilModelImage)

            ' Clear and disable the overlay.
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY, MIL.M_DISABLE)
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY_CLEAR, MIL.M_DEFAULT)
        End Sub

        '****************************************************************************
        ' Tracking object with pattern matching module.
        '****************************************************************************
        Private Shared Sub MpatTrackingExample(ByVal MilSystem As MIL_ID, ByVal MilDisplay As MIL_ID, ByVal MilDigitizer As MIL_ID, ByVal MilDisplayImage As MIL_ID, ByVal MilModelImage As MIL_ID)
            Dim MilImage() As MIL_ID = {MIL.M_NULL, MIL.M_NULL} ' Processing image buffer identifiers.
            Dim Model As MIL_ID = MIL.M_NULL ' Model identifier.
            Dim Result As MIL_ID = MIL.M_NULL ' Result identifier.
            Dim DrawColor As Double = DRAW_COLOR ' Model drawing color.
            Dim Found As MIL_INT = 0 ' Number of found models. int NbFindDone = 0;
            Dim NbFindDone As Integer = 0 ' Number of loops to find model done.
            Dim OrgX As Double = 0.0 ' Original center of model.
            Dim OrgY As Double = 0.0
            Dim x As Double = 0.0 ' Result variables.
            Dim y As Double = 0.0
            Dim Score As Double = 0.0
            Dim Time As Double = 0.0 ' Timer.

            ' Print a start message.
            Console.Write(Constants.vbLf + "GRAYSCALE PATTERN MATCHING:" + Constants.vbLf)
            Console.Write("---------------------------" + Constants.vbLf + Constants.vbLf)

            ' Display the model image.
            MIL.MbufCopy(MilModelImage, MilDisplayImage)

            ' Allocate normalized grayscale type model.
            MIL.MpatAllocModel(MilSystem, MilModelImage, MODEL_POS_X_INIT(MilModelImage) - CType(MODEL_WIDTH / 2, MIL_INT), MODEL_POS_Y_INIT(MilModelImage) - CType(MODEL_HEIGHT / 2, MIL_INT), MODEL_WIDTH, MODEL_HEIGHT, MIL.M_NORMALIZED, Model)

            ' Allocate result.
            MIL.MpatAllocResult(MilSystem, 1, Result)

            ' Draw box around the model.
            MIL.MgraColor(MIL.M_DEFAULT, DrawColor)
            MIL.MpatDraw(MIL.M_DEFAULT, Model, MilDisplayImage, MIL.M_DRAW_BOX, MIL.M_DEFAULT, MIL.M_ORIGINAL)

            ' Set minimum acceptance for search.
            MIL.MpatSetAcceptance(Model, MODEL_MIN_MATCH_SCORE)

            ' Set speed.
            MIL.MpatSetSpeed(Model, MIL.M_HIGH)

            ' Set accuracy.
            MIL.MpatSetAccuracy(Model, MIL.M_LOW)

            ' Preprocess model.
            MIL.MpatPreprocModel(MilModelImage, Model, MIL.M_DEFAULT)

            ' Inquire about center of model.
            MIL.MpatInquire(Model, MIL.M_ORIGINAL_X, OrgX)
            MIL.MpatInquire(Model, MIL.M_ORIGINAL_Y, OrgY)

            ' Print the original position.
            Console.Write("A Grayscale Model was defined." + Constants.vbLf)
            Console.Write("Model dimensions:  {0} x {1}." + Constants.vbLf, MODEL_WIDTH, MODEL_HEIGHT)
            Console.Write("Model center:      X={0:0.00}, Y={0:0.00}." + Constants.vbLf, OrgX, OrgY)
            Console.Write("Model is scale and rotation dependant." + Constants.vbLf)
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Allocate 2 grab buffers.
            MIL.MbufAlloc2d(MilSystem, MIL.MbufInquire(MilModelImage, MIL.M_SIZE_X, MIL.M_NULL), MIL.MbufInquire(MilModelImage, MIL.M_SIZE_Y, MIL.M_NULL), 8, MIL.M_IMAGE + MIL.M_GRAB + MIL.M_PROC, MilImage(0))
            MIL.MbufAlloc2d(MilSystem, MIL.MbufInquire(MilModelImage, MIL.M_SIZE_X, MIL.M_NULL), MIL.MbufInquire(MilModelImage, MIL.M_SIZE_Y, MIL.M_NULL), 8, MIL.M_IMAGE + MIL.M_GRAB + MIL.M_PROC, MilImage(1))

            ' Grab continuously and perform the find operation using double buffering.
            Console.Write(Constants.vbLf + "Continuously finding the Grayscale model." + Constants.vbLf)
            Console.Write("Press <Enter> to stop." + Constants.vbLf + Constants.vbLf)

            ' Grab a first target image into first buffer (done twice for timer reset accuracy).
            MIL.MdigControl(MilDigitizer, MIL.M_GRAB_MODE, MIL.M_ASYNCHRONOUS)
            MIL.MdigGrab(MilDigitizer, MilImage(NbFindDone Mod 2))
            MIL.MdigGrab(MilDigitizer, MilImage(NbFindDone Mod 2))
            MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_RESET, Time)

            ' Loop, processing one buffer while grabbing the other.
            Do
                ' Grab a target image into the other buffer.
                MIL.MdigGrab(MilDigitizer, MilImage((NbFindDone + 1) Mod 2))

                ' Read the time.
                MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ, Time)

                ' Find model.
                MIL.MpatFindModel(MilImage(NbFindDone Mod 2), Model, Result)

                ' Get results.
                MIL.MpatGetNumber(Result, Found)
                MIL.MpatGetResult(Result, MIL.M_POSITION_X, x)
                MIL.MpatGetResult(Result, MIL.M_POSITION_Y, y)
                MIL.MpatGetResult(Result, MIL.M_SCORE, Score)

                ' Print a message based upon the score.
                If Found > 0 Then
                    ' Draw a box around the model and print the results.
                    MIL.MpatDraw(MIL.M_DEFAULT, Result, MilImage(NbFindDone Mod 2), MIL.M_DRAW_BOX + MIL.M_DRAW_POSITION, MIL.M_DEFAULT, MIL.M_DEFAULT)
                    Console.Write("Found: X={0,7:0.00}, Y={1,7:0.00}, Score={2,5:0.0}% ({3:0.0} fps).    " & Constants.vbCr, x, y, Score, (NbFindDone + 1) / Time)
                Else
                    ' Print the "not found" message.
                    Console.Write("Not found ! (score<{0,5:0.0}%)                ({1:0.0} fps).     " & Constants.vbCr, MODEL_MIN_MATCH_SCORE, (NbFindDone + 1) / Time)
                End If

                ' Copy target image to the display.
                MIL.MbufCopy(MilImage(NbFindDone Mod 2), MilDisplayImage)

                ' Increment find count
                NbFindDone += 1
            Loop While Not Console.KeyAvailable

            Console.ReadKey()
            Console.Write(Constants.vbLf + Constants.vbLf)

            ' Wait for end of last grab.
            MIL.MdigGrabWait(MilDigitizer, MIL.M_GRAB_END)

            ' Free all allocated objects.
            MIL.MpatFree(Result)
            MIL.MpatFree(Model)
            MIL.MbufFree(MilImage(1))
            MIL.MbufFree(MilImage(0))
        End Sub


        '****************************************************************************
        ' Tracking object with Geometric Model Finder module
        '****************************************************************************
        Private Const MODEL_MAX_OCCURRENCES As Integer = 16

        Private Shared Sub MmodTrackingExample(ByVal MilSystem As MIL_ID, ByVal MilDisplay As MIL_ID, ByVal MilDigitizer As MIL_ID, ByVal MilDisplayImage As MIL_ID, ByVal MilModelImage As MIL_ID)
            Dim MilImage() As MIL_ID = {MIL.M_NULL, MIL.M_NULL} ' Processing image buffer identifiers.
            Dim SearchContext As MIL_ID = MIL.M_NULL ' Search context identifier.
            Dim Result As MIL_ID = MIL.M_NULL ' Result identifier.
            Dim DrawColor As Double = DRAW_COLOR ' Model drawing color.
            Dim Found As MIL_INT = 0 ' Number of models found.
            Dim NbFindDone As Integer = 0 ' Number of loops to find model done.
            Dim OrgX As Double = 0.0 ' Original center of model.
            Dim OrgY As Double = 0.0
            Dim Score(MODEL_MAX_OCCURRENCES - 1) As Double ' Model correlation score.
            Dim x(MODEL_MAX_OCCURRENCES - 1) As Double ' Model X position.
            Dim y(MODEL_MAX_OCCURRENCES - 1) As Double ' Model Y position.
            Dim Angle(MODEL_MAX_OCCURRENCES - 1) As Double ' Model occurrence angle.
            Dim Scale(MODEL_MAX_OCCURRENCES - 1) As Double ' Model occurrence scale.
            Dim Time As Double = 0.0 ' Timer.

            ' Print a start message.
            Console.Write(Constants.vbLf + "GEOMETRIC MODEL FINDER (scale and rotation independent):" + Constants.vbLf)
            Console.Write("--------------------------------------------------------" + Constants.vbLf + Constants.vbLf)

            ' Display model image.
            MIL.MbufCopy(MilModelImage, MilDisplayImage)

            ' Allocate a context and define a geometric model.
            MIL.MmodAlloc(MilSystem, MIL.M_GEOMETRIC, MIL.M_DEFAULT, SearchContext)
            MIL.MmodDefine(SearchContext, MIL.M_IMAGE, MilModelImage, CDbl(MODEL_POS_X_INIT(MilModelImage)) - (MODEL_WIDTH / 2), CDbl(MODEL_POS_Y_INIT(MilModelImage)) - (MODEL_HEIGHT / 2), MODEL_WIDTH, MODEL_HEIGHT)

            ' Allocate result.
            MIL.MmodAllocResult(MilSystem, MIL.M_DEFAULT, Result)

            ' Draw a box around the model.
            MIL.MgraColor(MIL.M_DEFAULT, DrawColor)
            MIL.MmodDraw(MIL.M_DEFAULT, SearchContext, MilDisplayImage, MIL.M_DRAW_BOX, MIL.M_DEFAULT, MIL.M_ORIGINAL)

            ' Set speed to VERY HIGH for fast but less precise search.
            MIL.MmodControl(SearchContext, MIL.M_CONTEXT, MIL.M_SPEED, MIL.M_VERY_HIGH)

            ' Set minimum acceptance for the search.
            MIL.MmodControl(SearchContext, MIL.M_DEFAULT, MIL.M_ACCEPTANCE, MODEL_MIN_MATCH_SCORE)

            ' Preprocess model.
            MIL.MmodPreprocess(SearchContext, MIL.M_DEFAULT)

            ' Inquire about center of model.
            MIL.MmodInquire(SearchContext, MIL.M_DEFAULT, MIL.M_ORIGINAL_X, OrgX)
            MIL.MmodInquire(SearchContext, MIL.M_DEFAULT, MIL.M_ORIGINAL_Y, OrgY)

            ' Print the original position.
            Console.Write("The Geometric target model was defined." + Constants.vbLf)
            Console.Write("Model dimensions: {0} x {1}." + Constants.vbLf, MODEL_WIDTH, MODEL_HEIGHT)
            Console.Write("Model center:     X={0:0.00}, Y={0:0.00}." + Constants.vbLf, OrgX, OrgY)
            Console.Write("Model is scale and rotation independent." + Constants.vbLf)
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Allocate 2 grab buffers.
            MIL.MbufAlloc2d(MilSystem, MIL.MbufInquire(MilModelImage, MIL.M_SIZE_X, MIL.M_NULL), MIL.MbufInquire(MilModelImage, MIL.M_SIZE_Y, MIL.M_NULL), 8, MIL.M_IMAGE + MIL.M_GRAB + MIL.M_PROC, MilImage(0))
            MIL.MbufAlloc2d(MilSystem, MIL.MbufInquire(MilModelImage, MIL.M_SIZE_X, MIL.M_NULL), MIL.MbufInquire(MilModelImage, MIL.M_SIZE_Y, MIL.M_NULL), 8, MIL.M_IMAGE + MIL.M_GRAB + MIL.M_PROC, MilImage(1))

            ' Grab continuously grab and perform the find operation using double buffering.
            Console.Write(Constants.vbLf + "Continuously finding the Geometric Model." + Constants.vbLf)
            Console.Write("Press a <Enter> to stop." + Constants.vbLf + Constants.vbLf)

            ' Grab a first target image into first buffer (done twice for timer reset accuracy).
            MIL.MdigControl(MilDigitizer, MIL.M_GRAB_MODE, MIL.M_ASYNCHRONOUS)
            MIL.MdigGrab(MilDigitizer, MilImage(NbFindDone Mod 2))
            MIL.MdigGrab(MilDigitizer, MilImage(NbFindDone Mod 2))
            MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_RESET, Time)

            ' Loop, processing one buffer while grabbing the other.
            Do
                ' Grab a target image into the other buffer.
                MIL.MdigGrab(MilDigitizer, MilImage((NbFindDone + 1) Mod 2))

                ' Read the time.
                MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ, Time)

                ' Find model.
                MIL.MmodFind(SearchContext, MilImage(NbFindDone Mod 2), Result)

                ' Get the number of occurrences found.
                MIL.MmodGetResult(Result, MIL.M_DEFAULT, MIL.M_NUMBER + MIL.M_TYPE_MIL_INT, Found)

                ' Print a message based on the score.
                If (Found >= 1) AndAlso (Found < MODEL_MAX_OCCURRENCES) Then
                    ' Get results.
                    MIL.MmodGetResult(Result, MIL.M_DEFAULT, MIL.M_POSITION_X, x)
                    MIL.MmodGetResult(Result, MIL.M_DEFAULT, MIL.M_POSITION_Y, y)
                    MIL.MmodGetResult(Result, MIL.M_DEFAULT, MIL.M_SCALE, Scale)
                    MIL.MmodGetResult(Result, MIL.M_DEFAULT, MIL.M_ANGLE, Angle)
                    MIL.MmodGetResult(Result, MIL.M_DEFAULT, MIL.M_SCORE, Score)

                    ' Draw a box and a cross where the model was found and print the results.
                    MIL.MmodDraw(MIL.M_DEFAULT, Result, MilImage(NbFindDone Mod 2), MIL.M_DRAW_BOX + MIL.M_DRAW_POSITION + MIL.M_DRAW_EDGES, MIL.M_DEFAULT, MIL.M_DEFAULT)
                    Console.Write("Found: X={0,6:0.0}, Y={1,6:0.0}, Angle={2,6:0.0}, Scale={3,5:0.00}, Score={4,5:0.0}% ({5,5:0.0} fps)." & Constants.vbCr, x(0), y(0), Angle(0), Scale(0), Score(0), (NbFindDone + 1) / Time)
                Else
                    ' Print the "not found" message.
                    Console.Write("Not found! (score<{0,5:0.0}%)                                          ({1,5:0.0} fps)." & Constants.vbCr, MODEL_MIN_MATCH_SCORE, (NbFindDone + 1) / Time)
                End If

                ' Copy target image to the display.
                MIL.MbufCopy(MilImage(NbFindDone Mod 2), MilDisplayImage)

                ' Increment the counter.
                NbFindDone += 1
            Loop While Not Console.KeyAvailable

            Console.ReadKey()
            Console.Write(Constants.vbLf + Constants.vbLf)

            ' Wait for the end of last grab.
            MIL.MdigGrabWait(MilDigitizer, MIL.M_GRAB_END)

            ' Free all allocations.
            MIL.MmodFree(Result)
            MIL.MmodFree(SearchContext)
            MIL.MbufFree(MilImage(1))
            MIL.MbufFree(MilImage(0))
        End Sub
    End Class
End Namespace
