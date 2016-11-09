//***************************************************************************************
//
// File name: MdigProcess.cs
//
// Synopsis:  This program shows the use of the MdigProcess() function and its multiple
//            buffering acquisition to do robust real-time processing.
//
//            The user's processing code to execute is located in a callback function 
//            that will be called for each frame acquired (see ProcessingFunction()).
//
//      Note: The average processing time must be shorter than the grab time or some
//            frames will be missed. Also, if the processing results are not displayed
//            and the frame count is not drawn or printed, the CPU usage is reduced 
//            significantly.
//***************************************************************************************
using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

using Matrox.MatroxImagingLibrary;

namespace MDigProcess
{
    class Program
    {
        // Number of images in the buffering grab queue.
        // Generally, increasing this number gives a better real-time grab.
        private const int BUFFERING_SIZE_MAX = 20;

        // User's processing function hook data object.
        public class HookDataStruct
        {
            public MIL_ID MilDigitizer;
            public MIL_ID MilImageDisp;
            public int ProcessedImageCount;
        };

        // Main function.
        static void Main(string[] args)
        {
            MIL_ID MilApplication = MIL.M_NULL;
            MIL_ID MilSystem = MIL.M_NULL;
            MIL_ID MilDigitizer = MIL.M_NULL;
            MIL_ID MilDisplay = MIL.M_NULL;
            MIL_ID MilImageDisp = MIL.M_NULL;
            MIL_ID[] MilGrabBufferList = new MIL_ID[BUFFERING_SIZE_MAX];
            int MilGrabBufferListSize = 0;
            MIL_INT ProcessFrameCount = 0;
            double ProcessFrameRate = 0;

            HookDataStruct UserHookData = new HookDataStruct();

            // Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, ref MilApplication, ref MilSystem, ref MilDisplay,
                                                        ref MilDigitizer, ref MilImageDisp);

            // Allocate the grab buffers and clear them.
            MIL.MappControl(MIL.M_DEFAULT, MIL.M_ERROR, MIL.M_PRINT_DISABLE);
            for (MilGrabBufferListSize = 0; MilGrabBufferListSize < BUFFERING_SIZE_MAX; MilGrabBufferListSize++)
            {
                MIL.MbufAlloc2d(MilSystem,
                                MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_X, MIL.M_NULL),
                                MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_Y, MIL.M_NULL), 
                                8 + MIL.M_UNSIGNED, 
                                MIL.M_IMAGE + MIL.M_GRAB + MIL.M_PROC, 
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
            for (int n = 0; n < 2 && MilGrabBufferListSize > 0; n++)
            {
                MilGrabBufferListSize--;
                MIL.MbufFree(MilGrabBufferList[MilGrabBufferListSize]);
            }

            // Print a message.
            Console.WriteLine();
            Console.WriteLine("MULTIPLE BUFFERED PROCESSING.");
            Console.WriteLine("-----------------------------");
            Console.WriteLine();
            Console.Write("Press <Enter> to start processing.\r");

            // Grab continuously on the display and wait for a key press.
            MIL.MdigGrabContinuous(MilDigitizer, MilImageDisp);
            Console.ReadKey();

            // Halt continuous grab.
            MIL.MdigHalt(MilDigitizer);

            // Initialize the user's processing function data structure.
            UserHookData.MilDigitizer = MilDigitizer;
            UserHookData.MilImageDisp = MilImageDisp;
            UserHookData.ProcessedImageCount = 0;

            // get a handle to the HookDataStruct object in the managed heap, we will use this 
            // handle to get the object back in the callback function
            GCHandle hUserData = GCHandle.Alloc(UserHookData);
            MIL_DIG_HOOK_FUNCTION_PTR ProcessingFunctionPtr = new MIL_DIG_HOOK_FUNCTION_PTR(ProcessingFunction);

            // Start the processing. The processing function is called with every frame grabbed.
            MIL.MdigProcess(MilDigitizer, MilGrabBufferList, MilGrabBufferListSize, MIL.M_START, MIL.M_DEFAULT, ProcessingFunctionPtr, GCHandle.ToIntPtr(hUserData));

            // Here the main() is free to perform other tasks while the processing is executing.
            // ---------------------------------------------------------------------------------

            // Print a message and wait for a key press after a minimum number of frames.
            Console.WriteLine("Press <Enter> to stop.                    ");
            Console.WriteLine();
            Console.ReadKey();

            // Stop the processing.
            MIL.MdigProcess(MilDigitizer, MilGrabBufferList, MilGrabBufferListSize, MIL.M_STOP, MIL.M_DEFAULT, ProcessingFunctionPtr, GCHandle.ToIntPtr(hUserData));

            // Free the GCHandle when no longer used
            hUserData.Free();

            // Print statistics.
            MIL.MdigInquire(MilDigitizer, MIL.M_PROCESS_FRAME_COUNT, ref ProcessFrameCount);
            MIL.MdigInquire(MilDigitizer, MIL.M_PROCESS_FRAME_RATE, ref ProcessFrameRate);
            Console.WriteLine();
            Console.WriteLine();
            Console.WriteLine("{0} frames grabbed at {1:0.0} frames/sec ({2:0.0} ms/frame).", ProcessFrameCount, ProcessFrameRate, 1000.0 / ProcessFrameRate);
            Console.WriteLine("Press <Enter> to end.");
            Console.WriteLine();
            Console.ReadKey();

            // Free the grab buffers.
            while (MilGrabBufferListSize > 0)
            {
                MIL.MbufFree(MilGrabBufferList[--MilGrabBufferListSize]);
            }

            // Release defaults.
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MilDigitizer, MilImageDisp);
        }

        // User's processing function called every time a grab buffer is ready.
        // -----------------------------------------------------------------------

        // Local defines.
        private const int STRING_LENGTH_MAX = 20;
        private const int STRING_POS_X = 20;
        private const int STRING_POS_Y = 20;
        static MIL_INT ProcessingFunction(MIL_INT HookType, MIL_ID HookId, IntPtr HookDataPtr)
        {
            MIL_ID ModifiedBufferId = MIL.M_NULL;

            // this is how to check if the user data is null, the IntPtr class
            // contains a member, Zero, which exists solely for this purpose
            if (!IntPtr.Zero.Equals(HookDataPtr))
            {
                // get the handle to the DigHookUserData object back from the IntPtr
                GCHandle hUserData = GCHandle.FromIntPtr(HookDataPtr);

                // get a reference to the DigHookUserData object
                HookDataStruct UserData = hUserData.Target as HookDataStruct;

                // Retrieve the MIL_ID of the grabbed buffer.
                MIL.MdigGetHookInfo(HookId, MIL.M_MODIFIED_BUFFER + MIL.M_BUFFER_ID, ref ModifiedBufferId);

                // Increment the frame counter.
                UserData.ProcessedImageCount++;

                // Print and draw the frame count (remove to reduce CPU usage).
                Console.Write("Processing frame #{0}.\r", UserData.ProcessedImageCount);
                MIL.MgraText(MIL.M_DEFAULT, ModifiedBufferId, STRING_POS_X, STRING_POS_Y, String.Format("{0}", UserData.ProcessedImageCount));

                // Execute the processing and update the display.
                MIL.MimArith(ModifiedBufferId, MIL.M_NULL, UserData.MilImageDisp, MIL.M_NOT);
            }

            return 0;
        }
    }
}
