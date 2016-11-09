'*****************************************************************************
'
' File name: Mfunc.vb
'
' Synopsis:  This example shows the use of the MIL Function Developer Tool Kit and how 
'            MIL and custom code can be mixed to create a custom MIL function that 
'            accesses the data pointer of a MIL buffer directly in order to process it.
'
'            The example creates a Master MIL function that registers all the parameters 
'            to MIL and calls the Slave function. The Slave function retrieves all the 
'            parameters, gets the pointers to the MIL image buffers, uses them to access 
'            the data directly and adds a constant.
'             
'            Note: The images must be 8-bit unsigned and have a valid host address.
'*****************************************************************************
Imports Microsoft.VisualBasic
Imports System
Imports System.Collections.Generic
Imports System.Runtime.InteropServices
Imports System.Text

Imports Matrox.MatroxImagingLibrary

Namespace MFunc
    Friend Class Program
        ' MIL function specifications.
        Private Const FUNCTION_NB_PARAM As Integer = 4
        Private Const FUNCTION_OPCODE_ADD_CONSTANT As Integer = 1
        Private Const FUNCTION_PARAMETER_ERROR_CODE As Integer = 1
        Private Const FUNCTION_SUPPORTED_IMAGE_TYPE As Integer = (8 + MIL.M_UNSIGNED)

        ' Target image file name.
        Private Const IMAGE_FILE As String = MIL.M_IMAGE_PATH & "BoltsNutsWashers.mim"

        ' Master MIL Function definition.
        ' -------------------------------

        Private Shared Function AddConstant(ByVal SrcImageId As MIL_ID, ByVal DstImageId As MIL_ID, ByVal ConstantToAdd As MIL_INT) As MIL_INT
            Dim Func As MIL_ID = MIL.M_NULL
            Dim SlaveReturnValue As MIL_INT = 0

            ' Allocate a MIL function context that will be used to call a target 
            ' Slave function locally on the Host to do the processing.
            Dim SlaveAddConstantDelegate As New MFUNCFCTPTR(AddressOf SlaveAddConstant)
            MIL.MfuncAlloc("AddConstant", _
                           FUNCTION_NB_PARAM, _
                           SlaveAddConstantDelegate, MIL.M_NULL, MIL.M_NULL, _
                           MIL.M_USER_MODULE_1 + FUNCTION_OPCODE_ADD_CONSTANT, _
                           MIL.M_LOCAL + MIL.M_SYNCHRONOUS_FUNCTION, _
                           Func)

            ' Register the parameters.
            MIL.MfuncParamMilId(Func, 1, SrcImageId, MIL.M_IMAGE, MIL.M_IN)
            MIL.MfuncParamMilId(Func, 2, DstImageId, MIL.M_IMAGE, MIL.M_OUT)
            MIL.MfuncParamMilInt(Func, 3, ConstantToAdd)

            ' To pass a pointer to MIL, we need to use a reference type such as an array
            ' to be able to pin the object and prevent the garbage collector from moving the object.
            Dim slaveReturnValueArray As MIL_INT() = New MIL_INT() {-1}
            Dim slaveReturnValueArrayHandle As GCHandle = GCHandle.Alloc(slaveReturnValueArray, GCHandleType.Pinned)

            Try

                ' Get the address of the pinned object, for an array, the address is pointing to the first element.
                Dim slaveReturnValuePtr As IntPtr = slaveReturnValueArrayHandle.AddrOfPinnedObject()
                MIL.MfuncParamDataPointer(Func, 4, slaveReturnValuePtr, MIL_INT.Size * 2, MIL.M_OUT)

                ' Call the target Slave function.
                MIL.MfuncCall(Func)

                SlaveReturnValue = slaveReturnValueArray(0)

                ' Make sure that the delegate survives garbage collection until the slave function is executed.
                GC.KeepAlive(SlaveAddConstantDelegate)
            Finally
                ' Free the allocated GCHandle to allow the array object to be garbage collected.
                If (slaveReturnValueArrayHandle.IsAllocated) Then
                    slaveReturnValueArrayHandle.Free()
                End If
            End Try

            ' Free the MIL function context.
            MIL.MfuncFree(Func)

            Return SlaveReturnValue
        End Function

        ' MIL Slave function definition.
        ' ------------------------------

        Private Shared Sub SlaveAddConstant(ByVal Func As MIL_ID)
            Dim SrcImageId As MIL_ID = MIL.M_NULL
            Dim DstImageId As MIL_ID = MIL.M_NULL
            Dim ConstantToAdd As MIL_INT = 0
            Dim SrcImageDataPtr As MIL_INT = MIL.M_NULL
            Dim DstImageDataPtr As MIL_INT = MIL.M_NULL
            Dim SrcImageSizeX As MIL_INT = 0
            Dim SrcImageSizeY As MIL_INT = 0
            Dim SrcImageType As MIL_INT = 0
            Dim SrcImagePitchByte As MIL_INT = 0
            Dim DstImageSizeX As MIL_INT = 0
            Dim DstImageSizeY As MIL_INT = 0
            Dim DstImageType As MIL_INT = 0
            Dim DstImagePitchByte As MIL_INT = 0
            Dim x As Integer = 0
            Dim y As Integer = 0
            Dim TempValue As Integer = 0
            Dim SlaveReturnValuePtr As IntPtr = IntPtr.Zero

            ' Read the parameters.
            MIL.MfuncParamValue(Func, 1, SrcImageId)
            MIL.MfuncParamValue(Func, 2, DstImageId)
            MIL.MfuncParamValue(Func, 3, ConstantToAdd)
            MIL.MfuncParamValue(Func, 4, SlaveReturnValuePtr)

            ' Lock buffers for direct access.
            MIL.MbufControl(SrcImageId, MIL.M_LOCK, MIL.M_DEFAULT)
            MIL.MbufControl(DstImageId, MIL.M_LOCK, MIL.M_DEFAULT)

            ' Read image information.
            MIL.MbufInquire(SrcImageId, MIL.M_HOST_ADDRESS, SrcImageDataPtr)
            MIL.MbufInquire(SrcImageId, MIL.M_SIZE_X, SrcImageSizeX)
            MIL.MbufInquire(SrcImageId, MIL.M_SIZE_Y, SrcImageSizeY)
            MIL.MbufInquire(SrcImageId, MIL.M_TYPE, SrcImageType)
            MIL.MbufInquire(SrcImageId, MIL.M_PITCH_BYTE, SrcImagePitchByte)
            MIL.MbufInquire(DstImageId, MIL.M_HOST_ADDRESS, DstImageDataPtr)
            MIL.MbufInquire(DstImageId, MIL.M_SIZE_X, DstImageSizeX)
            MIL.MbufInquire(DstImageId, MIL.M_SIZE_Y, DstImageSizeY)
            MIL.MbufInquire(DstImageId, MIL.M_TYPE, DstImageType)
            MIL.MbufInquire(DstImageId, MIL.M_PITCH_BYTE, DstImagePitchByte)

            ' Reduce the destination area to process if necessary.
            If SrcImageSizeX < DstImageSizeX Then
                DstImageSizeX = SrcImageSizeX
            End If
            If SrcImageSizeY < DstImageSizeY Then
                DstImageSizeY = SrcImageSizeY
            End If

            ' If images have the proper depth, execute the operation using a custom code.
            If ((SrcImageType = DstImageType) And (SrcImageType = FUNCTION_SUPPORTED_IMAGE_TYPE) And _
                (SrcImageDataPtr <> MIL.M_NULL) And (DstImageDataPtr <> MIL.M_NULL)) Then
                Dim SrcImagePixels(CType((SrcImageSizeX - 1), Integer), CType((SrcImageSizeY - 1), Integer)) As Byte
                Dim DstImagePixels(CType((DstImageSizeX - 1), Integer), CType((DstImageSizeY - 1), Integer)) As Byte

                MIL.MbufGet(SrcImageId, SrcImagePixels)

                ' Add the constant to the image.
                For y = 0 To CType((DstImageSizeY - 1), Integer)
                    For x = 0 To CType((DstImageSizeX - 1), Integer)
                        ' Calculate the value to write.
                        TempValue = CType(SrcImagePixels(x, y) + ConstantToAdd, Integer)

                        ' Write the value if no overflow, else saturate.
                        If TempValue <= &HFF Then
                            DstImagePixels(x, y) = CByte(TempValue)
                        Else
                            DstImagePixels(x, y) = &HFF
                        End If
                    Next x
                Next y

                MIL.MbufPut(DstImageId, DstImagePixels)

                ' Return a null error code to the Master function.
                MIL_INT.WriteMilInt(SlaveReturnValuePtr, MIL.M_NULL)
            Else
                ' Report a MIL error.
                MIL.MfuncErrorReport(Func, MIL.M_FUNC_ERROR + FUNCTION_PARAMETER_ERROR_CODE, _
                                     "Invalid parameter.", _
                                     "Image type not supported or invalid target system.", _
                                     "Image depth must be 8-bit and have a valid host address.", _
                                     MIL.M_NULL)

                ' Return an error code to the Master function.

                MIL_INT.WriteMilInt(SlaveReturnValuePtr, MIL.M_FUNC_ERROR + FUNCTION_PARAMETER_ERROR_CODE)
            End If

            ' Unlock buffers.
            MIL.MbufControl(SrcImageId, MIL.M_UNLOCK, MIL.M_DEFAULT)
            MIL.MbufControl(DstImageId, MIL.M_UNLOCK, MIL.M_DEFAULT)

            ' Signal to MIL that the destination buffer was modified.
            MIL.MbufControl(DstImageId, MIL.M_MODIFIED, MIL.M_DEFAULT)
        End Sub

        ' Main to test the custom function. //
        ' --------------------------------- //

        Shared Sub Main(ByVal args() As String)
            Dim MilApplication As MIL_ID = MIL.M_NULL   ' Application identifier.
            Dim MilSystem As MIL_ID = MIL.M_NULL        ' System identifier.
            Dim MilDisplay As MIL_ID = MIL.M_NULL       ' Display identifier.
            Dim MilImage As MIL_ID = MIL.M_NULL         ' Image buffer identifier.
            Dim ReturnValue As MIL_INT = 0              ' Return value of the function.

            ' Allocate default application, system, display and image.
            MIL.MappAllocDefault(MIL.M_DEFAULT, MilApplication, MilSystem, MilDisplay, CType(MIL.M_NULL, IntPtr), CType(MIL.M_NULL, IntPtr))

            ' Load source image into a Host memory image buffer.
            MIL.MbufAlloc2d(MilSystem, MIL.MbufDiskInquire(IMAGE_FILE, MIL.M_SIZE_X), MIL.MbufDiskInquire(IMAGE_FILE, MIL.M_SIZE_Y), 8 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_DISP + MIL.M_HOST_MEMORY, MilImage)
            MIL.MbufLoad(IMAGE_FILE, MilImage)

            ' Display the image.
            MIL.MdispSelect(MilDisplay, MilImage)

            ' Pause.
            Console.WriteLine()
            Console.WriteLine("MIL FUNCTION DEVELOPER'S TOOLKIT:")
            Console.WriteLine("----------------------------------")
            Console.WriteLine()
            Console.WriteLine("This example creates a custom MIL function that processes")
            Console.WriteLine("an image using its data pointer directly.")
            Console.WriteLine()
            Console.WriteLine("Target image was loaded.")
            Console.WriteLine("Press a key to continue.")
            Console.WriteLine()
            Console.ReadKey()

            ' Run the custom function only if the target system's memory is local and accessible.
            If MIL.MsysInquire(MilSystem, MIL.M_LOCATION, MIL.M_NULL) = MIL.M_LOCAL Then
                ' Process the image directly with the custom MIL function.
                ReturnValue = AddConstant(MilImage, MilImage, &H40)

                ' Print a conclusion message.
                If (ReturnValue = MIL.M_NULL) Then
                    Console.WriteLine("The white level of the image was augmented.")
                Else
                    Console.WriteLine("An error was returned by the Slave function.")
                End If
            Else
                Console.WriteLine("This example doesn't run with Distributed MIL.")
            End If

            Console.WriteLine("Press a key to terminate.")
            Console.WriteLine()
            Console.ReadKey()

            ' Free all allocations.
            MIL.MbufFree(MilImage)
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL)
        End Sub
    End Class
End Namespace
