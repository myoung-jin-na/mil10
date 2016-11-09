'*****************************************************************************
'
' File name: MgraText.vb
'
' Synopsis:  This program allocates a MIL application and system, then 
'            displays messages using TrueType fonts. It also shows how to 
'            check for errors.
'*****************************************************************************
Imports Microsoft.VisualBasic
Imports System

Imports Matrox.MatroxImagingLibrary

Namespace MgraText
    Friend Class Program

        Shared Sub Main(ByVal args() As String)
            Dim MilApplication As MIL_ID = MIL.M_NULL ' Application identifier.
            Dim MilSystem As MIL_ID = MIL.M_NULL ' System identifier.
            Dim MilDisplay As MIL_ID = MIL.M_NULL ' Display identifier.
            Dim MilImage As MIL_ID = MIL.M_NULL ' Image buffer identifier.
            Dim MilGraphicContextId As MIL_ID = MIL.M_NULL '  Graphic context identifier

            ' Allocate a default MIL application, system, display and image.
            MIL.MappAllocDefault(MIL.M_DEFAULT, MilApplication, MilSystem, MilDisplay, CType(MIL.M_NULL, IntPtr), MilImage)

            ' Allocate a graphic context 
            MIL.MgraAlloc(MilSystem, MilGraphicContextId)


            ' Perform graphic operations in the display image.
            MIL.MgraColor(MilGraphicContextId, &HF0)
            MIL.MgraFont(MilGraphicContextId, MIL.M_FONT_DEFAULT_LARGE)
            MIL.MgraText(MilGraphicContextId, MilImage, 160L, 20L, " Welcome to MIL !!! ")

            MIL.MgraControl(MilGraphicContextId, MIL.M_FONT_SIZE, 12)
            MIL.MgraFont(MilGraphicContextId, MIL.MIL_FONT_NAME(MIL.M_FONT_DEFAULT_TTF))
            MIL.MgraText(MilGraphicContextId, MilImage, 40L, 80L, "English")

            MIL.MappControl(MIL.M_DEFAULT, MIL.M_ERROR, MIL.M_PRINT_DISABLE)

            MIL.MgraControl(MilGraphicContextId, MIL.M_FONT_SIZE, 16)
            MIL.MgraFont(MilGraphicContextId, MIL.MIL_FONT_NAME(MIL.M_FONT_DEFAULT_TTF + ":Bold"))
            MIL.MgraText(MilGraphicContextId, MilImage, 40L, 140L, "Français")

            MIL.MgraControl(MilGraphicContextId, MIL.M_FONT_SIZE, 24)
            MIL.MgraFont(MilGraphicContextId, MIL.MIL_FONT_NAME(MIL.M_FONT_DEFAULT_TTF + ":Italic"))
            MIL.MgraText(MilGraphicContextId, MilImage, 40L, 220L, "Italiano")

            MIL.MgraControl(MilGraphicContextId, MIL.M_FONT_SIZE, 30)
            MIL.MgraFont(MilGraphicContextId, MIL.MIL_FONT_NAME(MIL.M_FONT_DEFAULT_TTF + ":Bold:Italic"))
            MIL.MgraText(MilGraphicContextId, MilImage, 40L, 300L, "Deutsch")

            MIL.MgraControl(MilGraphicContextId, MIL.M_FONT_SIZE, 36)
            MIL.MgraFont(MilGraphicContextId, MIL.MIL_FONT_NAME("Courier New"))
            MIL.MgraText(MilGraphicContextId, MilImage, 40L, 380L, "Español")

            If MIL.M_MIL_USE_TTF_UNICODE = 1 Then
                ' Draw Greek, Japanese and Korean
                MIL.MgraFont(MilGraphicContextId, MIL.MIL_FONT_NAME(MIL.M_FONT_DEFAULT_TTF))

                MIL.MgraControl(MilGraphicContextId, MIL.M_FONT_SIZE, 12)
                MIL.MgraText(MilGraphicContextId, MilImage, 400L, 80L, "ελληνιδ")

                MIL.MgraControl(MilGraphicContextId, MIL.M_FONT_SIZE, 24)
                MIL.MgraText(MilGraphicContextId, MilImage, 400L, 140L, "日本語")

                MIL.MgraControl(MilGraphicContextId, MIL.M_FONT_SIZE, 16)
                MIL.MgraText(MilGraphicContextId, MilImage, 400L, 220L, "한국어")

                ' Draw Arabic and Hebrew
                MIL.MgraControl(MilGraphicContextId, MIL.M_TEXT_DIRECTION, MIL.M_RIGHT_TO_LEFT)
                MIL.MgraControl(MilGraphicContextId, MIL.M_FONT_SIZE, 30)
                MIL.MgraText(MilGraphicContextId, MilImage, 400L, 320L, "עברית")

                MIL.MgraControl(MilGraphicContextId, MIL.M_FONT_SIZE, 36)
                MIL.MgraText(MilGraphicContextId, MilImage, 400L, 380L, "ﻋﺮﺑﻲ")
            End If

            ' Print a message.
            Console.WriteLine()
            Console.WriteLine("INTERNATIONAL TEXT ANNOTATION:")
            Console.WriteLine("------------------------------")
            Console.WriteLine()
            Console.WriteLine("This example demonstrates the support of TrueType fonts by MgraText.")
            Console.WriteLine()

            If MIL.MappGetError(MIL.M_GLOBAL + MIL.M_SYNCHRONOUS, CType(MIL.M_NULL, IntPtr)) <> MIL.M_NULL_ERROR Then
                Console.WriteLine("Note: Some Unicode fonts are not available")
                Console.WriteLine()
            End If

            ' Wait for a key press.
            Console.WriteLine("Press <Enter> to end.")
            Console.ReadKey()

            'Free Graphic Context
            MIL.MgraFree(MilGraphicContextId)

            ' Free defaults.
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MilImage)
        End Sub
    End Class
End Namespace
