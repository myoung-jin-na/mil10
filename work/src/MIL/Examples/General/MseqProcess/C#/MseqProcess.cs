//************************************************************************************
//
// File name: MseqProcess.cs
//
// Synopsis:  This program shows the use of the MseqProcess() and MseqFeed() functions 
//            to perform real-time encoding of a sequence of captured images.
//
//            The user's preprocessing and compression code is written in a hook 
//            function that will be called by MdigProcess() for each frame grabbed 
//            (see ProcessingFunction()). The queueing for encoding of the next 
//            frame is also made in that hook function to allow fully parallel 
//            execution of the capture and the encoding.
//
//      Note: The average encoding time must be shorter than the grab time or
//            some frames will be missed. To miss frames is very frequent when 
//            the encoding is done by software. Also, if the captured images 
//            are not displayed and the frame count is not drawn or printed 
//            in the hook function, the CPU usage is reduced significantly.
//            
//            When encoding a 1080p source it is recommended to have your 
//            MIL Non-Paged Memory set to at least 64MB.
//************************************************************************************
using System;
using System.Collections.Generic;
using System.Text;

using Matrox.MatroxImagingLibrary;
using System.Runtime.InteropServices;

namespace MseqProcess
{
    // User's processing function hook data object.
    class ProcessingHookDataStruct
    {
        public MIL_ID MilDigitizer;
        public MIL_ID MilImageDisp;
        public MIL_ID MilSeqContext;
        public MIL_INT ProcessedImageCount;
        public ProcessingHookOperation ProcessingOperation;
    }

    // Optional encoding end function hook data object.
    class EncodingFrameEndHookDataStruct
    {
        public MIL_INT EncodedImageCount;
    }

    // Optional decoding end function hook data object.
    class DecodingFrameEndHookDataStruct
    {
        public MIL_INT DecodedImageCount;
        public MIL_ID MilImageDisp;
    }

    enum ProcessingHookOperation
    {
        DISPLAY,
        ENCODE
    }

    class Program
    {
        // Number of images in the buffering grab queue.
        // Generally, increasing this number gives better real-time grab.
        private const int BUFFERING_SIZE_MAX = 20;

        // Target sequence file name and location.
        // The temporary directory location can be reached with %temp% under Windows. 
        private static readonly string SEQUENCE_FILE = MIL.M_TEMP_DIR + "SeqProcess.mp4";

        // Remote target sequence file name and location if Distributed MIL is used.
        private static readonly string REMOTE_SEQUENCE_FILE = "remote:///" + SEQUENCE_FILE;

        // Main function. //
        // ---------------//
        static int Main(string[] args)
        {
            MIL_ID MilApplication = MIL.M_NULL;
            MIL_ID MilRemoteApplication = MIL.M_NULL;
            MIL_ID MilSystem = MIL.M_NULL;
            MIL_ID MilDigitizer = MIL.M_NULL;
            MIL_ID MilDisplay = MIL.M_NULL;
            MIL_ID MilImageDisp = MIL.M_NULL;
            MIL_ID[] MilGrabBufferList = new MIL_ID[BUFFERING_SIZE_MAX];
            MIL_ID MilCompressContext = MIL.M_NULL;
            MIL_ID MilDecompressContext = MIL.M_NULL;
            MIL_INT LicenseModules = 0;
            MIL_INT MilSystemLocation = MIL.M_NULL;
            MIL_INT MilGrabBufferListSize;
            MIL_INT ProcessFrameCount = 0;
            MIL_INT NbFrames = 0;
            MIL_INT n = 0;
            double EncodingDesiredFrameRate = 0.0;
            double ProcessFrameRate = 0.0;
            MIL_INT SeqProcessFilePathSize = 0;
            StringBuilder SeqProcessFilePath = null;
            ProcessingHookDataStruct ProcessingUserHookData = new ProcessingHookDataStruct();
            EncodingFrameEndHookDataStruct EncodingFrameEndUserHookData = new EncodingFrameEndHookDataStruct();
            DecodingFrameEndHookDataStruct DecodingFrameEndUserHookData = new DecodingFrameEndHookDataStruct();
            MIL_INT SeqSystemType = MIL.M_NULL;

            // Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, ref MilApplication, ref MilSystem, ref MilDisplay, ref MilDigitizer, ref MilImageDisp);

            MIL.MsysInquire(MilSystem, MIL.M_OWNER_APPLICATION, ref MilRemoteApplication);
            MilSystemLocation = MIL.MsysInquire(MilSystem, MIL.M_LOCATION, MIL.M_NULL);

            if (MIL.MappInquire(MilRemoteApplication, MIL.M_PLATFORM_OS_TYPE, MIL.M_NULL) != MIL.M_OS_WINDOWS)
            {
               if (MilSystemLocation == MIL.M_REMOTE)
                  Console.WriteLine("The Distributed MIL server must run on a Windows system.");
               else
                  Console.WriteLine("This example only works with a Windows system.");

               Console.WriteLine("Press <Enter> to end.");
               Console.ReadKey();

               MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MilDigitizer, MilImageDisp);
               return 0;

            }

            // Inquire MIL licenses.
            
            MIL.MappInquire(MilRemoteApplication, MIL.M_LICENSE_MODULES, ref LicenseModules);

            if ((LicenseModules & MIL.M_LICENSE_JPEGSTD) != MIL.M_LICENSE_JPEGSTD)
            {
                Console.WriteLine("Need a Compression/Decompression license to run this example.");
                Console.WriteLine("Press <Enter> to end.");
                Console.ReadKey();

                MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MilDigitizer, MilImageDisp);
                return 0;
            }

            // Allocate the grab buffers and clear them.
            MIL.MappControl(MIL.M_DEFAULT, MIL.M_ERROR, MIL.M_PRINT_DISABLE);
            for (MilGrabBufferListSize = 0; MilGrabBufferListSize < BUFFERING_SIZE_MAX; MilGrabBufferListSize++)
            {
                MIL.MbufAllocColor(MilSystem, MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_BAND, MIL.M_NULL),
                               MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_X, MIL.M_NULL),
                               MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_Y, MIL.M_NULL),
                               8 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_GRAB + MIL.M_PROC,
                               ref MilGrabBufferList[MilGrabBufferListSize]);

                if (MilGrabBufferList[MilGrabBufferListSize] != MIL.M_NULL)
                {
                    MIL.MbufClear(MilGrabBufferList[MilGrabBufferListSize], 0xFF);
                }
                else
                {
                    break;
                }
            }
            MIL.MappControl(MIL.M_DEFAULT, MIL.M_ERROR, MIL.M_PRINT_ENABLE);

            // Free buffers to leave space for possible temporary buffers.
            for (n = 0; n < 2 && MilGrabBufferListSize > 0; n++)
            {
                MilGrabBufferListSize--;
                MIL.MbufFree(MilGrabBufferList[MilGrabBufferListSize]);
            }

            if (MilGrabBufferListSize == 0)
            {
                Console.WriteLine("!!! No grab buffers have been allocated. Need to set more Non-Paged Memory. !!!");

                MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MilDigitizer, MilImageDisp);
                Console.WriteLine("Press <Enter> to end.");
                Console.ReadKey();
                return 1;
            }

            /* Initialize the User's processing function data structure only for Display. */
            ProcessingUserHookData.MilDigitizer = MilDigitizer;
            ProcessingUserHookData.MilSeqContext = MIL.M_NULL;
            ProcessingUserHookData.MilImageDisp = MilImageDisp;
            ProcessingUserHookData.ProcessedImageCount = 0;
            ProcessingUserHookData.ProcessingOperation = ProcessingHookOperation.DISPLAY;

            // get a handle to the HookDataStruct object in the managed heap, we will use this 
            // handle to get the object back in the callback function
            GCHandle hUserData = GCHandle.Alloc(ProcessingUserHookData);

            // Start the sequence acquisition. The preprocessing and encoding hook function 
            // is called for every frame grabbed.
            MIL_DIG_HOOK_FUNCTION_PTR ProcessingFunctionDelegate = new MIL_DIG_HOOK_FUNCTION_PTR(ProcessingFunction);

            /* Start a digProcess to show the live camera output. */
            MIL.MdigProcess(MilDigitizer, MilGrabBufferList, MilGrabBufferListSize, MIL.M_START, MIL.M_DEFAULT, ProcessingFunctionDelegate, GCHandle.ToIntPtr(hUserData));

            /* Print a message. */
            Console.Write("\nH.264 IMAGE SEQUENCE COMPRESSION.\n");
            Console.Write("---------------------------------\n\n");
            Console.Write("Press <Enter> to start compression.\r");
            Console.ReadKey();

            /* Stop digProcess. */
            MIL.MdigProcess(MilDigitizer, MilGrabBufferList, MilGrabBufferListSize, MIL.M_STOP, MIL.M_DEFAULT, ProcessingFunctionDelegate, GCHandle.ToIntPtr(hUserData));

            /* Inquire the dig process frame rate */
            MIL.MdigInquire(MilDigitizer, MIL.M_PROCESS_FRAME_RATE, ref EncodingDesiredFrameRate);
            Console.Write("Grabbing frames at {0:0.00} frames/sec.\n", EncodingDesiredFrameRate);


            // Creates a context for the H.264 compression engine. Compression will be done
            // using hardware or software depending on the system hardware configuration. 
            MIL.MseqAlloc(MilSystem, MIL.M_DEFAULT, MIL.M_SEQ_COMPRESS, MIL.M_DEFAULT, MIL.M_DEFAULT, ref MilCompressContext);

            // Specify the destination of the compressed file and the target container type.
            // The last argument specifies to generate an MP4 file.
            MIL.MseqDefine(MilCompressContext, MIL.M_SEQ_OUTPUT(0) + MIL.M_SEQ_DEST(0), MIL.M_FILE,
                (MilSystemLocation != MIL.M_REMOTE ? SEQUENCE_FILE : REMOTE_SEQUENCE_FILE),
                MIL.M_FILE_FORMAT_MP4);

            // Set the compression parameters.
            // Sets the compression parameters valid for any resolution under 1920X1080.
            // Any resolution higher than that will generate an warning that can be disabled using
            // MseqControl with M_SETTING_AUTO_ADJUSTMENT. See documentation for more details.
            MIL.MseqControl(MilCompressContext, MIL.M_CONTEXT, MIL.M_STREAM_BIT_RATE_MODE, MIL.M_VARIABLE);         // MIL.M_VARIABLE or MIL.M_CONSTANT
            MIL.MseqControl(MilCompressContext, MIL.M_CONTEXT, MIL.M_STREAM_BIT_RATE, 5000);                        // 5 Mbps bit rate
            MIL.MseqControl(MilCompressContext, MIL.M_CONTEXT, MIL.M_STREAM_BIT_RATE_MAX, 5000);                    // 5 Mbps bit rate
            MIL.MseqControl(MilCompressContext, MIL.M_CONTEXT, MIL.M_STREAM_FRAME_RATE, EncodingDesiredFrameRate);  // 60Hz frame rate.
            MIL.MseqControl(MilCompressContext, MIL.M_CONTEXT, MIL.M_STREAM_FRAME_RATE_MODE, MIL.M_VARIABLE);       // Attempts to update the file header with the encoding frame rate
                                                                                                                    // if lower than the specified frame rate.

            MIL.MseqControl(MilCompressContext, MIL.M_CONTEXT, MIL.M_STREAM_QUALITY, 100);                          // 1=best speed, 100=best quality
            MIL.MseqControl(MilCompressContext, MIL.M_CONTEXT, MIL.M_STREAM_PROFILE, MIL.M_PROFILE_HIGH);           // MIL.M_PROFILE_BASELINE, MIL.M_PROFILE_MAIN, MIL.M_PROFILE_HIGH
            MIL.MseqControl(MilCompressContext, MIL.M_CONTEXT, MIL.M_STREAM_LEVEL, MIL.M_LEVEL_4_2);                // MIL.M_LEVEL_1, MIL.M_LEVEL_1B, MIL.M_LEVEL_1_1, MIL.M_LEVEL_1_2, MIL.M_LEVEL_1_3,
                                                                                                                    // MIL.M_LEVEL_2, MIL.M_LEVEL_2_1, MIL.M_LEVEL_2_2,
                                                                                                                    // MIL.M_LEVEL_3, MIL.M_LEVEL_3_1, MIL.M_LEVEL_3_2,
                                                                                                                    // MIL.M_LEVEL_4, MIL.M_LEVEL_4_1, MIL.M_LEVEL_4_2,
                                                                                                                    // MIL.M_LEVEL_5, MIL.M_LEVEL_5_1
            MIL.MseqControl(MilCompressContext, MIL.M_CONTEXT, MIL.M_STREAM_GROUP_OF_PICTURE_SIZE, 30);             // Interval between I-Frame

            // Initialize the optional encoding end function data structure.
            EncodingFrameEndUserHookData.EncodedImageCount = 0;

            // get a handle to the HookDataStruct object in the managed heap, we will use this 
            // handle to get the object back in the callback function
            GCHandle EncodingFrameEndUserHookDataHandle = GCHandle.Alloc(EncodingFrameEndUserHookData);

            // Register the encoding end function to the sequence context.
            MIL_SEQ_HOOK_FUNCTION_PTR FrameEncodingEndFunctionDelegate = new MIL_SEQ_HOOK_FUNCTION_PTR(FrameEncodingEndFunction);
            MIL.MseqHookFunction(MilCompressContext, MIL.M_FRAME_END, FrameEncodingEndFunctionDelegate, GCHandle.ToIntPtr(EncodingFrameEndUserHookDataHandle));

            // Provide a sample image to initialize the encoding engine accordingly.
            MIL.MseqControl(MilCompressContext, MIL.M_CONTEXT, MIL.M_BUFFER_SAMPLE, MilGrabBufferList[0]);

            // Start the encoding process, waits for buffer to be fed for encoding.
            MIL.MseqProcess(MilCompressContext, MIL.M_START, MIL.M_ASYNCHRONOUS);

            // Display the type of compression used.
            Console.Write("Live image capture and compression to file using ");
            MIL.MseqInquire(MilCompressContext, MIL.M_CONTEXT, MIL.M_CODEC_TYPE, ref SeqSystemType);
            if (SeqSystemType == MIL.M_HARDWARE + MIL.M_QSV)
            {
                Console.WriteLine("Hardware acceleration.");
            }
            else // MIL.M_SOFTWARE + MIL.M_QSV
            {
                Console.WriteLine("Software implementation.");
            }

            // Set the sequence context id in the user hook data structure to start
            // feeding buffers for encoding in ProcessingFunction.
            //
            ProcessingUserHookData.MilSeqContext = MilCompressContext;
            ProcessingUserHookData.ProcessedImageCount = 0;
            ProcessingUserHookData.ProcessingOperation = ProcessingHookOperation.ENCODE;

            MIL.MdigProcess(MilDigitizer, MilGrabBufferList, MilGrabBufferListSize, MIL.M_START, MIL.M_DEFAULT, ProcessingFunctionDelegate, GCHandle.ToIntPtr(hUserData));

            // NOTE: Now the main() is free to perform other tasks while the compression is executing.
            // ---------------------------------------------------------------------------------------

            // Print a message and wait for a key press after a minimum number of frames.
            Console.WriteLine("Press <Enter> to stop.\n");
            Console.ReadKey();

            // Stop the processing.
            MIL.MdigProcess(MilDigitizer, MilGrabBufferList, MilGrabBufferListSize, MIL.M_STOP + MIL.M_WAIT, MIL.M_DEFAULT, ProcessingFunctionDelegate, GCHandle.ToIntPtr(hUserData));

            // Free the processing user data handle.
            hUserData.Free();

            // Stop the encoding process
            MIL.MseqProcess(MilCompressContext, MIL.M_STOP, MIL.M_WAIT);

            // Make sure the hook handler function delegate is not prematurely garbage collected since
            // only MIL has a reference to it.
            GC.KeepAlive(FrameEncodingEndFunctionDelegate);

            // Free the encoding user data handle.
            EncodingFrameEndUserHookDataHandle.Free();

            // Print statistics.
            MIL.MdigInquire(MilDigitizer, MIL.M_PROCESS_FRAME_COUNT, ref ProcessFrameCount);
            MIL.MdigInquire(MilDigitizer, MIL.M_PROCESS_FRAME_RATE, ref ProcessFrameRate);
            Console.WriteLine("{0} frames encoded at {1:0.00} frames/sec ({2:0.0} ms/frame).", ProcessFrameCount, ProcessFrameRate, 1000.0 / ProcessFrameRate);
            Console.WriteLine();

            MIL.MseqInquire(MilCompressContext, MIL.M_SEQ_OUTPUT(0) + MIL.M_SEQ_DEST(0), MIL.M_STREAM_FILE_NAME_SIZE, ref SeqProcessFilePathSize);
            SeqProcessFilePath = new StringBuilder((int)SeqProcessFilePathSize);
            MIL.MseqInquire(MilCompressContext, MIL.M_SEQ_OUTPUT(0) + MIL.M_SEQ_DEST(0), MIL.M_STREAM_FILE_NAME, SeqProcessFilePath);
            Console.WriteLine("The video sequence file was written to:");
            Console.WriteLine("{0}.", SeqProcessFilePath.ToString());
            Console.WriteLine();
            Console.WriteLine("It can be played back using any compatible video player.");

            // Free the grab buffers and sequence context.
            while (MilGrabBufferListSize > 0)
            {
                MIL.MbufFree(MilGrabBufferList[--MilGrabBufferListSize]);
                MilGrabBufferList[MilGrabBufferListSize] = MIL.M_NULL;
            }

            MIL.MseqFree(MilCompressContext);

            // Wait for a key to start the replay.
            Console.WriteLine("Press <Enter> to replay encoded sequence.");
            Console.ReadKey();

            MIL.MseqAlloc(MilSystem, MIL.M_DEFAULT, MIL.M_SEQ_DECOMPRESS,
                                  MIL.M_DEFAULT, MIL.M_DEFAULT, ref MilDecompressContext);

            // Specify the destination of the compressed file and the target container type.
            // The last argument specifies to generate an MP4 file.
            MIL.MseqDefine(MilDecompressContext, MIL.M_SEQ_INPUT(0), MIL.M_FILE,
                        (MilSystemLocation != MIL.M_REMOTE ? SEQUENCE_FILE : REMOTE_SEQUENCE_FILE),
                        MIL.M_FILE_FORMAT_MP4);

            double outputFrameRate = 0.0;
            MIL.MseqInquire(MilDecompressContext, MIL.M_SEQ_INPUT(0), MIL.M_STREAM_FRAME_RATE, ref outputFrameRate);

            Console.WriteLine();
            Console.WriteLine("Replaying file at {0:0.00} frames/second.", outputFrameRate);

            // Initialize the optional decoding end function data structure.
            DecodingFrameEndUserHookData.DecodedImageCount = 0;
            DecodingFrameEndUserHookData.MilImageDisp = MilImageDisp;

            // get a handle to the HookDataStruct object in the managed heap, we will use this 
            // handle to get the object back in the callback function
            GCHandle DecodingFrameEndUserHookDataHandle = GCHandle.Alloc(DecodingFrameEndUserHookData);

            // Register the decoding end function to the sequence context.
            MIL_SEQ_HOOK_FUNCTION_PTR FrameDecodingEndFunctionDelegate = new MIL_SEQ_HOOK_FUNCTION_PTR(FrameDecodingEndFunction);
            MIL.MseqHookFunction(MilDecompressContext, MIL.M_FRAME_END, FrameDecodingEndFunctionDelegate, GCHandle.ToIntPtr(DecodingFrameEndUserHookDataHandle));

            // Start the decoding process, waits for buffer to be fed for encoding.
            MIL.MseqProcess(MilDecompressContext, MIL.M_START, MIL.M_ASYNCHRONOUS);

            // Print a message and wait for a key press after a minimum number of frames.
            Console.WriteLine("Press <Enter> to stop.\n");
            Console.ReadKey();

            // Stop the play back.
            MIL.MseqProcess(MilDecompressContext, MIL.M_STOP, MIL.M_NULL);

            // Make sure the hook handler function delegate is not prematurely garbage collected since
            // only MIL has a reference to it.
            GC.KeepAlive(FrameDecodingEndFunctionDelegate);

            // Free the decoding user data handle.
            DecodingFrameEndUserHookDataHandle.Free();

            MIL.MseqFree(MilDecompressContext);

            // Wait for a key to end.
            Console.WriteLine("Press <Enter> to end.");
            Console.ReadKey();

            // Release defaults.
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MilDigitizer, MilImageDisp);

            return 0;
        }

        // User's processing function called every time a grab buffer is modified. 
        // ------------------------------------------------------------------------
        // 
        // Local defines.
        private const int STRING_LENGTH_MAX = 20;
        private const int STRING_POS_X = 20;
        private const int STRING_POS_Y = 20;

        static MIL_INT ProcessingFunction(MIL_INT HookType, MIL_ID HookId, IntPtr HookDataPtr)
        {
            // this is how to check if the user data is null, the IntPtr class
            // contains a member, Zero, which exists solely for this purpose
            if (!IntPtr.Zero.Equals(HookDataPtr))
            {
                // get the handle to the ProcessingHookDataStruct object back from the IntPtr
                GCHandle hUserData = GCHandle.FromIntPtr(HookDataPtr);

                // get a reference to the ProcessingHookDataStruct object
                ProcessingHookDataStruct UserHookDataPtr = hUserData.Target as ProcessingHookDataStruct;
                MIL_ID ModifiedBufferId = MIL.M_NULL;
                string Text;

                // Retrieve the MIL_ID of the grabbed buffer.
                MIL.MdigGetHookInfo(HookId, MIL.M_MODIFIED_BUFFER + MIL.M_BUFFER_ID, ref ModifiedBufferId);

                switch (UserHookDataPtr.ProcessingOperation)
                {
                    case ProcessingHookOperation.DISPLAY:
                        // Update the display with the last captured image.
                        MIL.MbufCopy(ModifiedBufferId, UserHookDataPtr.MilImageDisp);
                        break;

                    case ProcessingHookOperation.ENCODE:
                        // Increase the compressed images count.
                        UserHookDataPtr.ProcessedImageCount++;

                        // Print and draw the frame count (comment to reduce CPU usage).
                        Console.Write("Processing frame #{0}.\r", UserHookDataPtr.ProcessedImageCount);
                        Text = string.Format("{0}", UserHookDataPtr.ProcessedImageCount);
                        MIL.MgraText(MIL.M_DEFAULT, ModifiedBufferId, STRING_POS_X, STRING_POS_Y, Text);
                        // Enqueue the grabbed buffer for parallel encoding.
                        MIL.MseqFeed(UserHookDataPtr.MilSeqContext, ModifiedBufferId, MIL.M_DEFAULT);

                        // Update the display with the last captured image.
                        MIL.MbufCopy(ModifiedBufferId, UserHookDataPtr.MilImageDisp);
                        break;
                }
            }

            return 0;
        }

        // Optional encoding end function called every time a buffer is finished being compressed.
        // ----------------------------------------------------------------------------------------
        static MIL_INT FrameEncodingEndFunction(MIL_INT HookType, MIL_ID HookId, IntPtr HookDataPtr)
        {
            // this is how to check if the user data is null, the IntPtr class
            // contains a member, Zero, which exists solely for this purpose
            if (!IntPtr.Zero.Equals(HookDataPtr))
            {
                // get the handle to the FrameEndHookDataStruct object back from the IntPtr
                GCHandle hUserData = GCHandle.FromIntPtr(HookDataPtr);

                // get a reference to the FrameEndHookDataStruct object
                EncodingFrameEndHookDataStruct UserHookDataPtr = hUserData.Target as EncodingFrameEndHookDataStruct;

                // Frame end hook post processing.
                if (HookType == MIL.M_FRAME_END)
                {
                    MIL_ID CompressedBufferId = MIL.M_NULL;
                    MIL_INT CompressedDataPtr = MIL.M_NULL;
                    MIL_INT CompressedDataSize = 0;

                    // Increment a encoded frame counter
                    UserHookDataPtr.EncodedImageCount++;

                    // Retrieve the MIL_ID of the encoded buffer.
                    MIL.MseqGetHookInfo(HookId, MIL.M_MODIFIED_BUFFER + MIL.M_BUFFER_ID, ref CompressedBufferId);

                    // Retrieves the address of the encoded data.
                    MIL.MbufInquire(CompressedBufferId, MIL.M_HOST_ADDRESS, ref CompressedDataPtr);

                    // Retrieves the size in bytes of the encoded data.
                    MIL.MbufInquire(CompressedBufferId, MIL.M_SIZE_BYTE, ref CompressedDataSize);

                    //  -----------------------------------------------------------------------------------------------
                    //  Here you can do any action with the encoded data such as send buffer through a network stream. 
                    //  If the processing done on the compressed data is long, it is recommended to copy the           
                    //  buffer and to process it in a separate thread to avoid blocking the compressions flow.         
                    //  -----------------------------------------------------------------------------------------------
                }
            }

            return 0;
        }

        // Optional decoding end function called every time a buffer is finished being decompressed.
        // ------------------------------------------------------------------------------------------
        static MIL_INT FrameDecodingEndFunction(MIL_INT HookType, MIL_ID HookId, IntPtr HookDataPtr)
        {
            // this is how to check if the user data is null, the IntPtr class
            // contains a member, Zero, which exists solely for this purpose
            if (!IntPtr.Zero.Equals(HookDataPtr))
            {
                // get the handle to the FrameEndHookDataStruct object back from the IntPtr
                GCHandle hUserData = GCHandle.FromIntPtr(HookDataPtr);

                // get a reference to the FrameEndHookDataStruct object
                DecodingFrameEndHookDataStruct UserHookDataPtr = hUserData.Target as DecodingFrameEndHookDataStruct;

                // Frame end hook post processing.
                if (HookType == MIL.M_FRAME_END)
                {
                    MIL_ID DecompressedBufferId = MIL.M_NULL;

                    // Increment a encoded frame counter.
                    UserHookDataPtr.DecodedImageCount++;

                    // Retrieve the MIL_ID of the encoded buffer.
                    MIL.MseqGetHookInfo(HookId, MIL.M_MODIFIED_BUFFER + MIL.M_BUFFER_ID, ref DecompressedBufferId);

                    // -----------------------------------------------------------------------------------------------
                    // Here you can do any action with the decoded buffer.
                    // -----------------------------------------------------------------------------------------------

                    MIL.MbufCopy(DecompressedBufferId, UserHookDataPtr.MilImageDisp);
                }
            }

            return 0;
        }
    }
}
