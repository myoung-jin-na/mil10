'*****************************************************************************
'
' File name: MimProcessing.vb
'
' Synopsis:  This program show the usage of image processing. Under MIL lite, 
'            it binarizes the image of a tissue sample to show dark particles.
'            Under MIL full, it also uses different image processing primitives 
'            to determine the number of cell nuclei that are larger than a 
'            certain size and show them in pseudo-color.
'*****************************************************************************
Imports Microsoft.VisualBasic
Imports System
Imports System.Collections.Generic
Imports System.Text

Imports Matrox.MatroxImagingLibrary

Namespace MImProcessing
    Friend Class Program
        ' Target MIL image file specifications.
        Private Const IMAGE_FILE As String = MIL.M_IMAGE_PATH & "Cell.mim"
        Private Const IMAGE_SMALL_PARTICLE_RADIUS As Integer = 1

        Shared Sub Main(ByVal args() As String)
            Dim MilApplication As MIL_ID = MIL.M_NULL ' Application identifier.
            Dim MilRemoteApplication As MIL_ID = MIL.M_NULL ' Remote application identifier.
            Dim MilSystem As MIL_ID = MIL.M_NULL ' System identifier.
            Dim MilDisplay As MIL_ID = MIL.M_NULL ' Display identifier.
            Dim MilImage As MIL_ID = MIL.M_NULL ' Image buffer identifier.
            Dim MilExtremeResult As MIL_ID = 0 ' Extreme result buffer identifier.
            Dim MaxLabelNumber As Integer = 0 ' Highest label value.
            Dim LicenseModules As MIL_INT = 0 ' List of available MIL modules. 


            ' Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, MilApplication, MilSystem, MilDisplay, CType(MIL.M_NULL, IntPtr), CType(MIL.M_NULL, IntPtr))

            ' Restore source image and display it.
            MIL.MbufRestore(IMAGE_FILE, MilSystem, MilImage)
            MIL.MdispSelect(MilDisplay, MilImage)

            ' Pause to show the original image.
            Console.Write(Constants.vbLf + "IMAGE PROCESSING:" + Constants.vbLf)
            Console.Write("-----------------" + Constants.vbLf + Constants.vbLf)
            Console.Write("This program extracts the dark particles in the image." + Constants.vbLf)
            Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Binarize the image with an automatically calculated threshold so that 
            ' particles are represented in white and the background removed.
            MIL.MimBinarize(MilImage, MilImage, MIL.M_BIMODAL + MIL.M_LESS_OR_EQUAL, MIL.M_NULL, MIL.M_NULL)

            ' Print a message for the extracted particles.
            Console.Write("These particles were extracted from the original image." + Constants.vbLf)

            ' If MIL IM module is available, count and label the larger particles.
            MIL.MsysInquire(MilSystem, MIL.M_OWNER_APPLICATION, MilRemoteApplication)
            MIL.MappInquire(MilRemoteApplication, MIL.M_LICENSE_MODULES, LicenseModules)

            If (LicenseModules And MIL.M_LICENSE_IM) <> 0 Then
                ' Pause to show the extracted particles.
                Console.Write("Press <Enter> to continue." + Constants.vbLf + Constants.vbLf)
                Console.ReadKey()

                ' Close small holes.
                MIL.MimClose(MilImage, MilImage, IMAGE_SMALL_PARTICLE_RADIUS, MIL.M_BINARY)

                ' Remove small particles.
                MIL.MimOpen(MilImage, MilImage, IMAGE_SMALL_PARTICLE_RADIUS, MIL.M_BINARY)

                ' Label the image.
                MIL.MimLabel(MilImage, MilImage, MIL.M_DEFAULT)

                ' The largest label value corresponds to the number of particles in the image.
                MIL.MimAllocResult(MilSystem, 1, MIL.M_EXTREME_LIST, MilExtremeResult)
                MIL.MimFindExtreme(MilImage, MilExtremeResult, MIL.M_MAX_VALUE)
                MIL.MimGetResult(MilExtremeResult, MIL.M_VALUE, MaxLabelNumber)
                MIL.MimFree(MilExtremeResult)

                ' Multiply the labeling result to augment the gray level of the particles.
                MIL.MimArith(MilImage, CInt(Fix(256 / CDbl(MaxLabelNumber))), MilImage, MIL.M_MULT_CONST)

                ' Display the resulting particles in pseudo-color.
                MIL.MdispLut(MilDisplay, MIL.M_PSEUDO)

                ' Print results.
                Console.Write("There were {0} large particles in the original image." + Constants.vbLf, MaxLabelNumber)
            End If

            ' Pause to show the result.
            Console.Write("Press <Enter> to end." + Constants.vbLf + Constants.vbLf)
            Console.ReadKey()

            ' Free all allocations.
            MIL.MbufFree(MilImage)
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL)
        End Sub
    End Class
End Namespace
