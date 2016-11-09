'*****************************************************************************
'
' File name: MappStart.vb
'
' Synopsis:  This program allocates a MIL application and system, then displays 
'            a welcoming message using graphics functions. It also shows how 
'            to check for errors.
'*****************************************************************************
Imports Microsoft.VisualBasic
Imports System
Imports System.Collections.Generic
Imports System.Text

Imports Matrox.MatroxImagingLibrary

Namespace MAppStart
    Friend Class Program
        Shared Sub Main(ByVal args() As String)
            Dim MilApplication As MIL_ID = MIL.M_NULL ' Application identifier.
            Dim MilSystem As MIL_ID = MIL.M_NULL ' System identifier.
            Dim MilDisplay As MIL_ID = MIL.M_NULL ' Display identifier.
            Dim MilImage As MIL_ID = MIL.M_NULL ' Image buffer identifier.

            ' Allocate a default MIL application, system, display and image.
            MIL.MappAllocDefault(MIL.M_DEFAULT, MilApplication, MilSystem, MilDisplay, CType(MIL.M_NULL, IntPtr), MilImage)

            ' If no allocation errors.
            If MIL.MappGetError(MIL.M_DEFAULT, MIL.M_GLOBAL, CType(MIL.M_NULL, IntPtr)) = 0 Then
                ' Perform graphic operations in the display image.
                MIL.MgraColor(MIL.M_DEFAULT, &HF0)
                MIL.MgraFont(MIL.M_DEFAULT, MIL.M_FONT_DEFAULT_LARGE)
                MIL.MgraText(MIL.M_DEFAULT, MilImage, 160L, 230L, " Welcome to MIL !!! ")
                MIL.MgraColor(MIL.M_DEFAULT, &HC0)
                MIL.MgraRect(MIL.M_DEFAULT, MilImage, 100, 150, 530, 340)
                MIL.MgraRect(MIL.M_DEFAULT, MilImage, 120, 170, 510, 320)
                MIL.MgraRect(MIL.M_DEFAULT, MilImage, 140, 190, 490, 300)

                ' Print a message.
                Console.Write(Constants.vbLf + "SYSTEM ALLOCATION:" + Constants.vbLf)
                Console.Write("------------------" + Constants.vbLf + Constants.vbLf)
                Console.Write("System allocation successful." + Constants.vbLf + Constants.vbLf)
                Console.Write("     ""Welcome to MIL !!!""" + Constants.vbLf + Constants.vbLf)
            Else
                Console.Write("System allocation error !" + Constants.vbLf + Constants.vbLf)
            End If

            ' Wait for a key press.
            Console.Write("Press <Enter> to end." + Constants.vbLf)
            Console.ReadKey()

            ' Free defaults.
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MilImage)
        End Sub
    End Class
End Namespace
