'********************************************************************************
'
' File name: MdigGrab.vb
'
' Synopsis:  This program demonstrates how to grab from a camera in
'            continuous and monoshot mode.
'********************************************************************************
Imports Microsoft.VisualBasic
Imports System
Imports System.Collections.Generic
Imports System.Text

Imports Matrox.MatroxImagingLibrary

Namespace MDigGrab
    Friend Class Program
        Shared Sub Main(ByVal args() As String)
            Dim MilApplication As MIL_ID = MIL.M_NULL ' Application identifier.
            Dim MilSystem As MIL_ID = MIL.M_NULL ' System identifier.
            Dim MilDisplay As MIL_ID = MIL.M_NULL ' Display identifier.
            Dim MilDigitizer As MIL_ID = MIL.M_NULL ' Digitizer identifier.
            Dim MilImage As MIL_ID = MIL.M_NULL ' Image buffer identifier.

            ' Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, MilApplication, MilSystem, MilDisplay, MilDigitizer, MilImage)

            ' Grab continuously.
            MIL.MdigGrabContinuous(MilDigitizer, MilImage)

            ' When a key is pressed, halt.
            Console.Write(Constants.vbLf + "DIGITIZER ACQUISITION:" + Constants.vbLf)
            Console.Write("----------------------" + Constants.vbLf + Constants.vbLf)
            Console.Write("Continuous image grab in progress." + Constants.vbLf)
            Console.Write("Press <Enter> to stop." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Stop continuous grab.
            MIL.MdigHalt(MilDigitizer)

            ' Pause to show the result.
            Console.Write("Continuous grab stopped." + Constants.vbLf + Constants.vbLf)
            Console.Write("Press <Enter> to do a single image grab." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Monoshot grab.
            MIL.MdigGrab(MilDigitizer, MilImage)

            ' Pause to show the result.
            Console.Write("Displaying the grabbed image." + Constants.vbLf)
            Console.Write("Press <Enter> to end." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Free defaults.
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MilDigitizer, MilImage)
        End Sub
    End Class
End Namespace
