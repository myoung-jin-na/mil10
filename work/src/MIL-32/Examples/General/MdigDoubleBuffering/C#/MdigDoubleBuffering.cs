//***************************************************************************************
//
// File name: MdigDoubleBuffering.cs
//
// Synopsis:  This program performs a double-buffering image acquisition that alternates 
//            between 2 target buffers. This permits the processing of one buffer while 
//            acquiring the next. 
//
//            The example also uses a hook callback function to the start of frames in 
//            order to print the index of the current frame being acquired.
//
//     Note:  The double-buffering method is not recommended for real-time processing, 
//            especially when the CPU usage is high. For more robust real-time behavior,
//            use the MdigProcess() function. See MdigProcess.cpp for a complete example.
//***************************************************************************************

using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

using Matrox.MatroxImagingLibrary;

namespace MDigDoubleBuffering
{
    public class UserDataObject
    {
        public MIL_INT NbGrabStart;
    }

    class Program
    {
        const int STRING_LENGTH_MAX = 20;

        // Main function.
        static void Main(string[] args)
        {
            MIL_ID MilApplication = MIL.M_NULL;
            MIL_ID MilSystem = MIL.M_NULL;
            MIL_ID MilDigitizer = MIL.M_NULL;
            MIL_ID MilDisplay = MIL.M_NULL;
            MIL_ID[] MilImage = new MIL_ID[2];
            MIL_ID MilImageDisp = MIL.M_NULL;
            MIL_ID Default = MIL.M_DEFAULT;
            int NbProc = 0;
            int n = 0;
            double Time = 0.0;
            StringBuilder Text = new StringBuilder("0", STRING_LENGTH_MAX);
            UserDataObject userObject = new UserDataObject();

            // Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, ref MilApplication, ref MilSystem, ref MilDisplay, ref MilDigitizer, ref MilImageDisp);

            // Allocate 2 grab buffers.
            for (n = 0; n < 2; n++)
            {
                MIL.MbufAlloc2d(MilSystem, 
                                MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_X, MIL.M_NULL),
                                MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_Y, MIL.M_NULL), 
                                8 + MIL.M_UNSIGNED, 
                                MIL.M_IMAGE + MIL.M_GRAB + MIL.M_PROC, 
                                ref MilImage[n]);
            }

            // Hook a function to the start of each frame to print the current frame index.
            userObject.NbGrabStart = 0;
            GCHandle userObjectHandle = GCHandle.Alloc(userObject);

            MIL_DIG_HOOK_FUNCTION_PTR grabStartDelegate = new MIL_DIG_HOOK_FUNCTION_PTR(GrabStart);
            MIL.MdigHookFunction(MilDigitizer, MIL.M_GRAB_START, grabStartDelegate, GCHandle.ToIntPtr(userObjectHandle));

            // Print a message.
            Console.WriteLine();
            Console.WriteLine("DOUBLE BUFFERING ACQUISITION AND PROCESSING:");
            Console.WriteLine("--------------------------------------------");
            Console.WriteLine();
            Console.WriteLine("Press <Enter> to stop.");
            Console.WriteLine();

            // Put the digitizer in asynchronous mode to be able to process while grabbing.
            MIL.MdigControl(MilDigitizer, MIL.M_GRAB_MODE, MIL.M_ASYNCHRONOUS);

            // Grab the first buffer.
            MIL.MdigGrab(MilDigitizer, MilImage[0]);

            // Process one buffer while grabbing the other.
            n = 0;
            do
            {
                // Grab the other buffer while processing the previous one.
                MIL.MdigGrab(MilDigitizer, MilImage[1 - n]);

                // Synchronize and start the timer.
                if (NbProc == 0)
                {
                    MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_RESET + MIL.M_SYNCHRONOUS, MIL.M_NULL);
                }

                // Write the frame counter.
                MIL.MgraText(Default, MilImage[n], 32, 32, string.Format("{0}", NbProc + 1));

                // Process the first buffer already grabbed. 
                MIL.MimArith(MilImage[n], MIL.M_NULL, MilImageDisp, MIL.M_NOT);

                // Count processed buffers.
                NbProc++;

                // Toggle grab buffers.
                n = 1 - n;
            } while (!Console.KeyAvailable);

            // Wait until the end of the last grab and stop the timer.
            MIL.MdigGrabWait(MilDigitizer, MIL.M_GRAB_END);
            MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ + MIL.M_SYNCHRONOUS, ref Time);
            Console.ReadKey();

            // Print statistics.
            Console.WriteLine("{0} frames grabbed, at a frame rate of {1:0.00} frames/sec ({2:0.00} ms/frame).", NbProc, NbProc / Time, 1000.0 * Time / NbProc);
            Console.WriteLine("Press <Enter> to end.");
            Console.WriteLine();
            Console.ReadKey();

            // Unhook the function at the start of each frame.
            MIL.MdigHookFunction(MilDigitizer, MIL.M_GRAB_START + MIL.M_UNHOOK, grabStartDelegate, GCHandle.ToIntPtr(userObjectHandle));

            // Free GCHandle to allow the garbage collector to reclaim the object.
            userObjectHandle.Free();

            // Free allocations.
            for (n = 0; n < 2; n++)
            {
                MIL.MbufFree(MilImage[n]);
            }

            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MilDigitizer, MilImageDisp);
        }

        
        // Grab Start hook function:
        // This function is called at the start of each frame captured.
        //
        private static MIL_INT GrabStart(MIL_INT HookType, MIL_ID EventId, IntPtr UserObjectPtr)
        {
            if (UserObjectPtr != IntPtr.Zero)
            {
                GCHandle userObjectHandle = GCHandle.FromIntPtr(UserObjectPtr);
                UserDataObject userData = userObjectHandle.Target as UserDataObject;
                if (userData != null)
                {
                    // Increment grab start count and print it.
                    userData.NbGrabStart++;
                    Console.Write("#{0}\r", userData.NbGrabStart);
                }
            }

            return(0);
        }
    }
}
