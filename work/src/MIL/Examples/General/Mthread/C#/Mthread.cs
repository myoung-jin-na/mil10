//*******************************************************************************
//
// File name: MThread.cs
//
// Synopsis:  This program shows how to use threads in a MIL application and
//            synchronize them with event. It creates 4 processing threads that 
//            are used to work in 4 different regions of a display buffer.
//
//     Thread usage:
//      - The main thread starts a processing thread in each of the 4 different
//        quarters of a display buffer. The main thread then waits for a key to
//        be pressed to stop them.
//      - The top-left and bottom-left threads work in a loop, as follows: the
//        top-left thread adds a constant to its buffer, then sends an event to
//        the bottom-left thread. The bottom-left thread waits for the event
//        from the top-left thread, rotates the top-left buffer image, then sends an
//        event to the top-left thread to start a new loop.
//      - The top-right and bottom-right threads work the same way as the
//        top-left and bottom-left threads, except that the bottom-right thread
//        performs an edge detection operation, rather than a rotation.
//
//      Note : - Under MIL-Lite, the threads will do graphic annotations instead.
//             - Comment out the MIL.MdispSelect() if you wish to avoid benchmarking 
//               the display update overhead on CPU usage and processing rate.
//*******************************************************************************
using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.Runtime.InteropServices;

using Matrox.MatroxImagingLibrary;

namespace MThread
{
    class Program
    {
        // Local constants.
        private const string IMAGE_FILE = MIL.M_IMAGE_PATH + "Bird.mim";
        private const int IMAGE_WIDTH = 256;
        private const int IMAGE_HEIGHT = 240;
        private const int STRING_LENGTH_MAX = 40;
        private const int STRING_POS_X = 10;
        private const int STRING_POS_Y = 220;
        private const int DRAW_RADIUS_NUMBER = 5;
        private const int DRAW_RADIUS_STEP = 10;
        private const int DRAW_CENTER_POSX = 196;
        private const int DRAW_CENTER_POSY = 180;

        // Thread parameters object.
        public class THREAD_PARAM
        {
            public MIL_ID Id;
            public MIL_ID System;
            public MIL_ID OrgImage;
            public MIL_ID SrcImage;
            public MIL_ID DstImage;
            public MIL_ID ReadyEvent;
            public MIL_ID DoneEvent;
            public int NumberOfIteration;
            public int Radius;
            public int Exit;
            public MIL_INT LicenseModules;
            public THREAD_PARAM SlaveThreadParam;
        }

        // Main function:
        // --------------
        static void Main(string[] args)
        {
            MIL_ID MilApplication = MIL.M_NULL;                 // Application identifier.
            MIL_ID MilRemoteApplication = MIL.M_NULL;           // Remote Application identifier if running on a remote computer
            MIL_ID MilSystem = MIL.M_NULL;                      // System identifier.
            MIL_ID MilDisplay = MIL.M_NULL;                     // Display identifier.
            MIL_ID MilImage = MIL.M_NULL;                       // Image buffer identifiers.
            MIL_ID MilOrgImage = MIL.M_NULL;
            THREAD_PARAM TParTopLeft = new THREAD_PARAM();      // Parameters passed to top left thread.
            THREAD_PARAM TParBotLeft = new THREAD_PARAM();      // Parameters passed to bottom left thread.
            THREAD_PARAM TParTopRight = new THREAD_PARAM();     // Parameters passed to top right thread.
            THREAD_PARAM TParBotRight = new THREAD_PARAM();     // Parameters passed to bottom right thread.
            double Time = 0.0;                                  // Timer variables.
            double FramesPerSecond = 0.0;
            MIL_INT LicenseModules = 0;                             // List of available MIL modules.

            // Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, ref MilApplication, ref MilSystem, ref MilDisplay, MIL.M_NULL, MIL.M_NULL);

            // Allocate and display the main image buffer.
            MIL.MbufAlloc2d(MilSystem, IMAGE_WIDTH * 2, IMAGE_HEIGHT * 2, 8 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_PROC + MIL.M_DISP, ref MilImage);
            MIL.MbufClear(MilImage, 0);
            MIL.MdispSelect(MilDisplay, MilImage);

            // Allocate an image buffer to keep the original.
            MIL.MbufAlloc2d(MilSystem, IMAGE_WIDTH, IMAGE_HEIGHT, 8 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_PROC, ref MilOrgImage);

            // Allocate a destination child buffer for each thread.
            MIL.MbufChild2d(MilImage, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT, ref TParTopLeft.SrcImage);
            MIL.MbufChild2d(MilImage, 0, IMAGE_HEIGHT, IMAGE_WIDTH, IMAGE_HEIGHT, ref TParBotLeft.DstImage);
            MIL.MbufChild2d(MilImage, IMAGE_WIDTH, 0, IMAGE_WIDTH, IMAGE_HEIGHT, ref TParTopRight.SrcImage);
            MIL.MbufChild2d(MilImage, IMAGE_WIDTH, IMAGE_HEIGHT, IMAGE_WIDTH, IMAGE_HEIGHT, ref TParBotRight.DstImage);

            // Allocate synchronization events.
            MIL.MthrAlloc(MilSystem, MIL.M_EVENT, MIL.M_DEFAULT, MIL.M_NULL, MIL.M_NULL, ref TParTopLeft.DoneEvent);
            MIL.MthrAlloc(MilSystem, MIL.M_EVENT, MIL.M_DEFAULT, MIL.M_NULL, MIL.M_NULL, ref TParBotLeft.DoneEvent);
            MIL.MthrAlloc(MilSystem, MIL.M_EVENT, MIL.M_DEFAULT, MIL.M_NULL, MIL.M_NULL, ref TParTopRight.DoneEvent);
            MIL.MthrAlloc(MilSystem, MIL.M_EVENT, MIL.M_DEFAULT, MIL.M_NULL, MIL.M_NULL, ref TParBotRight.DoneEvent);

            // Inquire MIL licenses.
            MIL.MsysInquire(MilSystem, MIL.M_OWNER_APPLICATION, ref MilRemoteApplication);
            MIL.MappInquire(MilRemoteApplication, MIL.M_LICENSE_MODULES, ref LicenseModules);

            // Initialize remaining thread parameters.
            TParTopLeft.System = MilSystem;
            TParTopLeft.OrgImage = MilOrgImage;
            TParTopLeft.DstImage = TParTopLeft.SrcImage;
            TParTopLeft.ReadyEvent = TParBotLeft.DoneEvent;
            TParTopLeft.NumberOfIteration = 0;
            TParTopLeft.Radius = 0;
            TParTopLeft.Exit = 0;
            TParTopLeft.LicenseModules = LicenseModules;
            TParTopLeft.SlaveThreadParam = TParBotLeft;

            TParBotLeft.System = MilSystem;
            TParBotLeft.OrgImage = 0;
            TParBotLeft.SrcImage = TParTopLeft.DstImage;
            TParBotLeft.ReadyEvent = TParTopLeft.DoneEvent;
            TParBotLeft.NumberOfIteration = 0;
            TParBotLeft.Radius = 0;
            TParBotLeft.Exit = 0;
            TParBotLeft.LicenseModules = LicenseModules;
            TParBotLeft.SlaveThreadParam = null;

            TParTopRight.System = MilSystem;
            TParTopRight.OrgImage = MilOrgImage;
            TParTopRight.DstImage = TParTopRight.SrcImage;
            TParTopRight.ReadyEvent = TParBotRight.DoneEvent;
            TParTopRight.NumberOfIteration = 0;
            TParTopRight.Radius = 0;
            TParTopRight.Exit = 0;
            TParTopRight.LicenseModules = LicenseModules;
            TParTopRight.SlaveThreadParam = TParBotRight;

            TParBotRight.System = MilSystem;
            TParBotRight.OrgImage = 0;
            TParBotRight.SrcImage = TParTopRight.DstImage;
            TParBotRight.ReadyEvent = TParTopRight.DoneEvent;
            TParBotRight.NumberOfIteration = 0;
            TParBotRight.Radius = 0;
            TParBotRight.Exit = 0;
            TParBotRight.LicenseModules = LicenseModules;
            TParBotRight.SlaveThreadParam = null;

            // Initialize the original image to process.
            MIL.MbufLoad(IMAGE_FILE, MilOrgImage);

            // Start the 4 threads.
            MIL_THREAD_FUNCTION_PTR TopThreadDelegate = new MIL_THREAD_FUNCTION_PTR(TopThread);
            MIL_THREAD_FUNCTION_PTR BotLeftThreadDelegate = new MIL_THREAD_FUNCTION_PTR(BotLeftThread);
            MIL_THREAD_FUNCTION_PTR BotRightThreadDelegate = new MIL_THREAD_FUNCTION_PTR(BotRightThread);

            GCHandle TParTopLeftHandle = GCHandle.Alloc(TParTopLeft);
            GCHandle TParBotLeftHandle = GCHandle.Alloc(TParBotLeft);
            GCHandle TParTopRightHandle = GCHandle.Alloc(TParTopRight);
            GCHandle TParBotRightHandle = GCHandle.Alloc(TParBotRight);

            MIL.MthrAlloc(MilSystem, MIL.M_THREAD, MIL.M_DEFAULT, TopThreadDelegate, GCHandle.ToIntPtr(TParTopLeftHandle), ref TParTopLeft.Id);
            MIL.MthrAlloc(MilSystem, MIL.M_THREAD, MIL.M_DEFAULT, BotLeftThreadDelegate, GCHandle.ToIntPtr(TParBotLeftHandle), ref TParBotLeft.Id);
            MIL.MthrAlloc(MilSystem, MIL.M_THREAD, MIL.M_DEFAULT, TopThreadDelegate, GCHandle.ToIntPtr(TParTopRightHandle), ref TParTopRight.Id);
            MIL.MthrAlloc(MilSystem, MIL.M_THREAD, MIL.M_DEFAULT, BotRightThreadDelegate, GCHandle.ToIntPtr(TParBotRightHandle), ref TParBotRight.Id);

            // Start the timer.
            MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_RESET + MIL.M_SYNCHRONOUS, MIL.M_NULL);

            // Set events to start operation of top left and top right threads.
            MIL.MthrControl(TParTopLeft.ReadyEvent, MIL.M_EVENT_SET, MIL.M_SIGNALED);
            MIL.MthrControl(TParTopRight.ReadyEvent, MIL.M_EVENT_SET, MIL.M_SIGNALED);

            // Report that the threads are started and wait for a key press to stop them.
            Console.Write("\nMULTI-THREADING:\n");
            Console.Write("----------------\n\n");
            Console.Write("4 threads running...\n");
            Console.Write("Press <Enter> to stop.\n\n");
            Console.ReadKey();

            // Signal the threads to exit.
            TParTopLeft.Exit = 1;
            TParTopRight.Exit = 1;

            // Wait for all threads to terminate.
            MIL.MthrWait(TParTopLeft.Id, MIL.M_THREAD_END_WAIT);
            MIL.MthrWait(TParBotLeft.Id, MIL.M_THREAD_END_WAIT);
            MIL.MthrWait(TParTopRight.Id, MIL.M_THREAD_END_WAIT);
            MIL.MthrWait(TParBotRight.Id, MIL.M_THREAD_END_WAIT);

            // Stop the timer and calculate the number of frames per second processed.
            MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ + MIL.M_SYNCHRONOUS, ref Time);
            FramesPerSecond = (TParTopLeft.NumberOfIteration + TParBotLeft.NumberOfIteration + TParTopRight.NumberOfIteration + TParBotRight.NumberOfIteration) / Time;

            // Print statistics.
            Console.Write("Top left iterations done:     {0,4}.\n", TParTopLeft.NumberOfIteration);
            Console.Write("Bottom left iterations done:  {0,4}.\n", TParBotLeft.NumberOfIteration);
            Console.Write("Top right iterations done:    {0,4}.\n", TParTopRight.NumberOfIteration);
            Console.Write("Bottom right iterations done: {0,4}.\n\n", TParBotRight.NumberOfIteration);
            Console.Write("Processing speed for the 4 threads: {0:0.0} Images/Sec.\n\n", FramesPerSecond);
            Console.Write("Press <Enter> to end.\n\n");
            Console.ReadKey();

            // Free threads.
            MIL.MthrFree(TParTopLeft.Id);
            MIL.MthrFree(TParBotLeft.Id);
            MIL.MthrFree(TParTopRight.Id);
            MIL.MthrFree(TParBotRight.Id);

            // Free events.
            MIL.MthrFree(TParTopLeft.DoneEvent);
            MIL.MthrFree(TParBotLeft.DoneEvent);
            MIL.MthrFree(TParTopRight.DoneEvent);
            MIL.MthrFree(TParBotRight.DoneEvent);

            // Free buffers.
            MIL.MbufFree(TParTopLeft.SrcImage);
            MIL.MbufFree(TParTopRight.SrcImage);
            MIL.MbufFree(TParBotLeft.DstImage);
            MIL.MbufFree(TParBotRight.DstImage);
            MIL.MbufFree(MilOrgImage);
            MIL.MbufFree(MilImage);

            // Free the GCHandles
            TParTopLeftHandle.Free();
            TParBotLeftHandle.Free();
            TParTopRightHandle.Free();
            TParBotRightHandle.Free();

            // Free defaults.
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL);
        }

        // Top left and top right threads' function (Add an offset):
        // ---------------------------------------------------------
        static uint TopThread(IntPtr ThreadParameters)
        {
            GCHandle threadParamHandle = GCHandle.FromIntPtr(ThreadParameters);
            THREAD_PARAM TPar = threadParamHandle.Target as THREAD_PARAM;

            while (TPar.Exit == 0)
            {
                // Wait for bottom ready event before proceeding.
                MIL.MthrWait(TPar.ReadyEvent, MIL.M_EVENT_WAIT);

                // For better visual effect, reset SrcImage to the original image regularly.
                if ((TPar.NumberOfIteration % 192) == 0)
                {
                    MIL.MbufCopy(TPar.OrgImage, TPar.SrcImage);
                }

                if ((TPar.LicenseModules & MIL.M_LICENSE_IM) != 0)
                {
                    // Add a constant to the image.
                    MIL.MimArith(TPar.SrcImage, 1L, TPar.DstImage, MIL.M_ADD_CONST + MIL.M_SATURATION);
                }
                else
                {
                    // Under Mil lite draw a variable size rectangle in the image.
                    TPar.Radius = TPar.SlaveThreadParam.Radius = (TPar.NumberOfIteration % DRAW_RADIUS_NUMBER) * DRAW_RADIUS_STEP;
                    MIL.MgraColor(MIL.M_DEFAULT, 0xff);
                    MIL.MgraRectFill(MIL.M_DEFAULT, TPar.DstImage, DRAW_CENTER_POSX - TPar.Radius, DRAW_CENTER_POSY - TPar.Radius, DRAW_CENTER_POSX + TPar.Radius, DRAW_CENTER_POSY + TPar.Radius);
                }

                // Increment iteration count and draw text.
                TPar.NumberOfIteration++;
                MIL.MgraColor(MIL.M_DEFAULT, 0xFF);
                MIL.MgraText(MIL.M_DEFAULT, TPar.DstImage, STRING_POS_X, STRING_POS_Y, String.Format("{0}", TPar.NumberOfIteration));

                // Signal to the bottom thread that the first part of processing is completed.
                MIL.MthrControl(TPar.DoneEvent, MIL.M_EVENT_SET, MIL.M_SIGNALED);

                // For a smoother display effect, optional yield to the other threads.
                BalanceThreadScheduling();
            }

            // Require the bottom thread to exit.
            TPar.SlaveThreadParam.Exit = 1;

            // Signal the bottom thread end to wake up.
            MIL.MthrControl(TPar.DoneEvent, MIL.M_EVENT_SET, MIL.M_SIGNALED);

            // before exiting the thread, make sure all the commands are executed
            MIL.MthrWait(TPar.System, MIL.M_THREAD_WAIT, MIL.M_NULL);

            return 1;
        }


        // Bottom-left thread function (Rotate):
        // -------------------------------------
        static uint BotLeftThread(IntPtr ThreadParameters)
        {
            GCHandle threadParamHandle = GCHandle.FromIntPtr(ThreadParameters);
            THREAD_PARAM TPar = threadParamHandle.Target as THREAD_PARAM;

            while (TPar.Exit == 0)
            {
                // Wait for the event in top-left function to be ready before proceeding.
                MIL.MthrWait(TPar.ReadyEvent, MIL.M_EVENT_WAIT);

                // If processing available, add a constant to the image.
                if ((TPar.LicenseModules & MIL.M_LICENSE_IM) != 0)
                {
                    MIL.MimRotate(TPar.SrcImage, TPar.DstImage, (TPar.NumberOfIteration * 5) % 360, MIL.M_DEFAULT, MIL.M_DEFAULT, MIL.M_DEFAULT, MIL.M_DEFAULT, MIL.M_NEAREST_NEIGHBOR + MIL.M_OVERSCAN_CLEAR);
                }
                else // else copy the top image and draw a filled circle.
                {
                    MIL.MbufCopy(TPar.SrcImage, TPar.DstImage);
                    MIL.MgraColor(MIL.M_DEFAULT, 0x80);
                    MIL.MgraArcFill(MIL.M_DEFAULT, TPar.DstImage, DRAW_CENTER_POSX, DRAW_CENTER_POSY, TPar.Radius, TPar.Radius, 0, 360);
                }

                // Print iteration count and draw a circle.
                TPar.NumberOfIteration++;
                MIL.MgraColor(MIL.M_DEFAULT, 0xFF);
                MIL.MgraText(MIL.M_DEFAULT, TPar.DstImage, STRING_POS_X, STRING_POS_Y, String.Format("{0}", TPar.NumberOfIteration));

                // Signal to the top thread that the last part of the processing is completed.
                MIL.MthrControl(TPar.DoneEvent, MIL.M_EVENT_SET, MIL.M_SIGNALED);

                // For a smoother display effect, optional yield to the other threads.
                BalanceThreadScheduling();
            }

            // before exiting the thread, make sure all the commands are executed
            MIL.MthrWait(TPar.System, MIL.M_THREAD_WAIT, MIL.M_NULL);

            return 1;
        }

        // Bottom-right thread function (Edge Detect):
        // -------------------------------------------
        static uint BotRightThread(IntPtr ThreadParameters)
        {
            GCHandle threadParamHandle = GCHandle.FromIntPtr(ThreadParameters);
            THREAD_PARAM TPar = threadParamHandle.Target as THREAD_PARAM;

            while (TPar.Exit == 0)
            {
                // Wait for the event in top-right function to be ready before proceeding.
                MIL.MthrWait(TPar.ReadyEvent, MIL.M_EVENT_WAIT);

                // If processing available, add a constant to the image.
                if ((TPar.LicenseModules & MIL.M_LICENSE_IM) != 0)
                {
                    MIL.MimConvolve(TPar.SrcImage, TPar.DstImage, MIL.M_EDGE_DETECT);
                }
                else // else copy the top image and draw a filled circle.
                {
                    MIL.MbufCopy(TPar.SrcImage, TPar.DstImage);
                    MIL.MgraColor(MIL.M_DEFAULT, 0x40);
                    MIL.MgraArcFill(MIL.M_DEFAULT, TPar.DstImage, DRAW_CENTER_POSX, DRAW_CENTER_POSY, TPar.Radius / 2, TPar.Radius / 2, 0, 360);
                }

                // Increment iteration count and draw text.
                TPar.NumberOfIteration++;
                MIL.MgraColor(MIL.M_DEFAULT, 0xFF);
                MIL.MgraText(MIL.M_DEFAULT, TPar.DstImage, STRING_POS_X, STRING_POS_Y, String.Format("{0}", TPar.NumberOfIteration));

                // Signal to the top thread that the last part of the processing is completed.
                MIL.MthrControl(TPar.DoneEvent, MIL.M_EVENT_SET, MIL.M_SIGNALED);

                // For a smoother display effect, optional yield to the other threads.
                BalanceThreadScheduling();
            }

            // before exiting the thread, make sure all the commands are executed
            MIL.MthrWait(TPar.System, MIL.M_THREAD_WAIT, MIL.M_NULL);

            return 1;
        }

        // Thread Scheduling Adjustment Handling
        // -------------------------------------
        //
        // This function is used to give a smoother display effect. It balances the number of
        // iterations of each thread by yielding its time slice frequently.
        //
        // NOTE: In some situations, Windows adjusts the priority or quantum value of threads.
        //       The intent of these adjustments is to improve system throughput and
        //       responsiveness. However, like any scheduling algorithms these adjustments are
        //       not perfect, and they might not be beneficial to all applications. The
        //       following function is introduced to minimize the impact of thread scheduling
        //       adjustment as performed by Windows. Refer to the "Inside Windows NT
        //       (Second Edition, Microsoft Press, 1998, ISBN 1-57231-677-2)" manual for more
        //       details on thread scheduling under Windows.
        static void BalanceThreadScheduling()
        {
            Thread.Sleep(0);
        }

    }
}
