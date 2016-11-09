//*****************************************************************************
//
// File name: Mfunc.cs
//
// Synopsis:  This example shows the use of the MIL Function Developer Tool Kit and how 
//            MIL and custom code can be mixed to create a custom MIL function that 
//            accesses the data pointer of a MIL buffer directly in order to process it.
//
//            The example creates a Master MIL function that registers all the parameters 
//            to MIL and calls the Slave function. The Slave function retrieves all the 
//            parameters, gets the pointers to the MIL image buffers, uses them to access 
//            the data directly and adds a constant.
//             
//            Note: The images must be 8-bit unsigned and have a valid host address.
//*****************************************************************************
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

using Matrox.MatroxImagingLibrary;

namespace MFunc
{
    class Program
    {
        // MIL function specifications.
        private const int FUNCTION_NB_PARAM = 4;
        private const int FUNCTION_OPCODE_ADD_CONSTANT = 1;
        private const int FUNCTION_PARAMETER_ERROR_CODE = 1;
        private const int FUNCTION_SUPPORTED_IMAGE_TYPE = (8 + MIL.M_UNSIGNED);

        // Target image file name.
        private const string IMAGE_FILE = MIL.M_IMAGE_PATH + "BoltsNutsWashers.mim";

        // Master MIL Function definition.
        // -------------------------------

        static MIL_INT AddConstant(MIL_ID SrcImageId, MIL_ID DstImageId, MIL_INT ConstantToAdd)
        {
            MIL_ID Func = MIL.M_NULL;
            MIL_INT SlaveReturnValue = 0;

            // Allocate a MIL function context that will be used to call a target 
            // Slave function locally on the Host to do the processing.
            MFUNCFCTPTR SlaveAddConstantDelegate = new MFUNCFCTPTR(SlaveAddConstant);
            MIL.MfuncAlloc("AddConstant",
                       FUNCTION_NB_PARAM,
                       SlaveAddConstantDelegate, MIL.M_NULL, MIL.M_NULL,
                       MIL.M_USER_MODULE_1 + FUNCTION_OPCODE_ADD_CONSTANT,
                       MIL.M_LOCAL + MIL.M_SYNCHRONOUS_FUNCTION,
                       ref Func);

            // Register the parameters.
            MIL.MfuncParamMilId(Func, 1, SrcImageId, MIL.M_IMAGE, MIL.M_IN);
            MIL.MfuncParamMilId(Func, 2, DstImageId, MIL.M_IMAGE, MIL.M_OUT);
            MIL.MfuncParamMilInt(Func, 3, ConstantToAdd);

            // To pass a pointer to MIL, we need to use a reference type such as an array
            // to be able to pin the object and prevent the garbage collector from moving the object.
            MIL_INT[] slaveReturnValueArray = new MIL_INT[1] { -1 };
            GCHandle slaveReturnValueArrayHandle = GCHandle.Alloc(slaveReturnValueArray, GCHandleType.Pinned);

            try
            {
                // Get the address of the pinned object, for an array, the address is pointing to the first element.
                IntPtr slaveReturnValuePtr = slaveReturnValueArrayHandle.AddrOfPinnedObject();
                MIL.MfuncParamDataPointer(Func, 4, slaveReturnValuePtr, MIL_INT.Size * 2, MIL.M_OUT);

                // Call the target Slave function.
                MIL.MfuncCall(Func);

                SlaveReturnValue = slaveReturnValueArray[0];

                // Make sure that the delegate survives garbage collection until the slave function is executed.
                GC.KeepAlive(SlaveAddConstantDelegate);
            }
            finally
            {
                // Free the allocated GCHandle to allow the array object to be garbage collected.
                if (slaveReturnValueArrayHandle.IsAllocated)
                {
                    slaveReturnValueArrayHandle.Free();
                }
            }

            // Free the MIL function context.
            MIL.MfuncFree(Func);

            return SlaveReturnValue;
        }

        // MIL Slave function definition.
        // ------------------------------

        static void SlaveAddConstant(MIL_ID Func)
        {
            MIL_ID SrcImageId = MIL.M_NULL;
            MIL_ID DstImageId = MIL.M_NULL;
            MIL_INT ConstantToAdd = 0;
            MIL_INT TempValue = 0;
            MIL_INT SrcImageDataPtr = MIL.M_NULL;
            MIL_INT DstImageDataPtr = MIL.M_NULL;
            MIL_INT SrcImageSizeX = 0;
            MIL_INT SrcImageSizeY = 0;
            MIL_INT SrcImageType = 0;
            MIL_INT SrcImagePitchByte = 0;
            MIL_INT DstImageSizeX = 0;
            MIL_INT DstImageSizeY = 0;
            MIL_INT DstImageType = 0;
            MIL_INT DstImagePitchByte = 0;
            int x = 0;
            int y = 0;
            IntPtr SlaveReturnValuePtr = IntPtr.Zero;

            // Read the parameters.
            MIL.MfuncParamValue(Func, 1, ref SrcImageId);
            MIL.MfuncParamValue(Func, 2, ref DstImageId);
            MIL.MfuncParamValue(Func, 3, ref ConstantToAdd);
            MIL.MfuncParamValue(Func, 4, ref SlaveReturnValuePtr);

            // Lock buffers for direct access.
            MIL.MbufControl(SrcImageId, MIL.M_LOCK, MIL.M_DEFAULT);
            MIL.MbufControl(DstImageId, MIL.M_LOCK, MIL.M_DEFAULT);

            // Read image information.
            MIL.MbufInquire(SrcImageId, MIL.M_HOST_ADDRESS, ref SrcImageDataPtr);
            MIL.MbufInquire(SrcImageId, MIL.M_SIZE_X, ref SrcImageSizeX);
            MIL.MbufInquire(SrcImageId, MIL.M_SIZE_Y, ref SrcImageSizeY);
            MIL.MbufInquire(SrcImageId, MIL.M_TYPE, ref SrcImageType);
            MIL.MbufInquire(SrcImageId, MIL.M_PITCH_BYTE, ref SrcImagePitchByte);
            MIL.MbufInquire(DstImageId, MIL.M_HOST_ADDRESS, ref DstImageDataPtr);
            MIL.MbufInquire(DstImageId, MIL.M_SIZE_X, ref DstImageSizeX);
            MIL.MbufInquire(DstImageId, MIL.M_SIZE_Y, ref DstImageSizeY);
            MIL.MbufInquire(DstImageId, MIL.M_TYPE, ref DstImageType);
            MIL.MbufInquire(DstImageId, MIL.M_PITCH_BYTE, ref DstImagePitchByte);

            // Reduce the destination area to process if necessary.
            if (SrcImageSizeX < DstImageSizeX) DstImageSizeX = SrcImageSizeX;
            if (SrcImageSizeY < DstImageSizeY) DstImageSizeY = SrcImageSizeY;

            unsafe // Unsafe code block to allow manipulating memory addresses
            {
                // If images have the proper depth, execute the operation using a custom code.
                if ((SrcImageType == DstImageType) && (SrcImageType == FUNCTION_SUPPORTED_IMAGE_TYPE) &&
                    (SrcImageDataPtr != MIL.M_NULL) && (DstImageDataPtr != MIL.M_NULL))
                {
                    // Convert the address inquired from MIL to a fixed size pointer.
                    // The MIL_INT type cannot be converted to a byte * so use the
                    // IntPtr portable type 
                    // 
                    IntPtr SrcImageDataPtrIntPtr = SrcImageDataPtr;
                    IntPtr DstImageDataPtrIntPtr = DstImageDataPtr;
                    byte* SrcImageDataAddr = (byte*)SrcImageDataPtrIntPtr;
                    byte* DstImageDataAddr = (byte*)DstImageDataPtrIntPtr;

                    // Add the constant to the image.
                    for (y = 0; y < DstImageSizeY; y++)
                    {
                        for (x = 0; x < DstImageSizeX; x++)
                        {
                            // Calculate the value to write.
                            TempValue = (MIL_INT)SrcImageDataAddr[x] + (MIL_INT)ConstantToAdd;

                            // Write the value if no overflow, else saturate.
                            if (TempValue <= 0xff)
                            {
                                DstImageDataAddr[x] = (byte)TempValue;
                            }
                            else
                            {
                                DstImageDataAddr[x] = 0xff;
                            }
                        }

                        // Move pointer to the next line taking into account the image's pitch.
                        SrcImageDataAddr += SrcImagePitchByte;
                        DstImageDataAddr += DstImagePitchByte;
                    }

                    // Return a null error code to the Master function.
                    MIL_INT.WriteMilInt(SlaveReturnValuePtr, MIL.M_NULL);
                }
                else
                {
                    // Buffer cannot be processed. Report an error.
                    MIL.MfuncErrorReport(Func, MIL.M_FUNC_ERROR + FUNCTION_PARAMETER_ERROR_CODE,
                                     "Invalid parameter.",
                                     "Image type not supported or invalid target system.",
                                     "Image depth must be 8-bit and have a valid host address.",
                                     MIL.M_NULL);

                    // Return an error code to the Master function.
                    MIL_INT.WriteMilInt(SlaveReturnValuePtr, MIL.M_FUNC_ERROR + FUNCTION_PARAMETER_ERROR_CODE);
                }
            }

            // Unlock buffers.
            MIL.MbufControl(SrcImageId, MIL.M_UNLOCK, MIL.M_DEFAULT);
            MIL.MbufControl(DstImageId, MIL.M_UNLOCK, MIL.M_DEFAULT);

            // Signal to MIL that the destination buffer was modified.
            MIL.MbufControl(DstImageId, MIL.M_MODIFIED, MIL.M_DEFAULT);
        }

        // Main to test the custom function. //
        // --------------------------------- //

        static void Main(string[] args)
        {
            MIL_ID MilApplication = MIL.M_NULL;     // Application identifier.
            MIL_ID MilSystem = MIL.M_NULL;          // System identifier.
            MIL_ID MilDisplay = MIL.M_NULL;         // Display identifier.
            MIL_ID MilImage = MIL.M_NULL;           // Image buffer identifier.
            MIL_INT ReturnValue = 0;                // Return value of the function.

            // Allocate default application, system, display and image.
            MIL.MappAllocDefault(MIL.M_DEFAULT, ref MilApplication, ref MilSystem, ref MilDisplay, MIL.M_NULL, MIL.M_NULL);

            // Load source image into a Host memory image buffer.
            MIL.MbufAlloc2d(MilSystem,
                        MIL.MbufDiskInquire(IMAGE_FILE, MIL.M_SIZE_X),
                        MIL.MbufDiskInquire(IMAGE_FILE, MIL.M_SIZE_Y),
                        8 + MIL.M_UNSIGNED,
                        MIL.M_IMAGE + MIL.M_DISP + MIL.M_HOST_MEMORY,
                        ref MilImage);
            MIL.MbufLoad(IMAGE_FILE, MilImage);

            // Display the image.
            MIL.MdispSelect(MilDisplay, MilImage);

            // Pause.
            Console.WriteLine();
            Console.WriteLine("MIL FUNCTION DEVELOPER'S TOOLKIT:");
            Console.WriteLine("----------------------------------");
            Console.WriteLine();
            Console.WriteLine("This example creates a custom MIL function that processes");
            Console.WriteLine("an image using its data pointer directly.");
            Console.WriteLine();
            Console.WriteLine("Target image was loaded.");
            Console.WriteLine("Press a key to continue.");
            Console.WriteLine();
            Console.ReadKey();

            // Run the custom function only if the target system's memory is local and accessible.
            if (MIL.MsysInquire(MilSystem, MIL.M_LOCATION, MIL.M_NULL) == MIL.M_LOCAL)
            {
                // Process the image directly with the custom MIL function.
                ReturnValue = AddConstant(MilImage, MilImage, 0x40);

                // Print a conclusion message.
                if (ReturnValue == MIL.M_NULL)
                {
                    Console.WriteLine("The white level of the image was augmented.");
                }
                else
                {
                    Console.WriteLine("An error was returned by the Slave function.");
                }
            }
            else
            {
                // Print that the example don't run remotely.
                Console.WriteLine("This example doesn't run with Distributed MIL.");
            }

            // Wait for a key to terminate.
            Console.WriteLine("Press a key to terminate.");
            Console.WriteLine();
            Console.ReadKey();

            // Free all allocations.
            MIL.MbufFree(MilImage);
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL);
        }
    }
}
