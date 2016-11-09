//*****************************************************************************
//
// File name: MdigAutoFocus.cs
//
// Synopsis:  This program performs an autofocus operation using the 
//            MdigFocus() function. Since the way to move a motorized
//            camera lens is device-specific, we will not include real
//            lens movement control and image grab but will simulate 
//            the lens focus with a smooth operation. 
//
//     Note : Under MIL-Lite, the out of focus lens simulation is not supported.
//*****************************************************************************
using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

using Matrox.MatroxImagingLibrary;

namespace MDigAutoFocus
{
    class Program
    {
        // Source MIL image file specification.
        private const string IMAGE_FILE = MIL.M_IMAGE_PATH + "BaboonMono.mim";

        // Lens mechanical characteristics.
        private const int FOCUS_MAX_NB_POSITIONS = 100;
        private const int FOCUS_MIN_POSITION = 0;
        private const int FOCUS_MAX_POSITION = (FOCUS_MAX_NB_POSITIONS - 1);
        private const int FOCUS_START_POSITION = 10;

        // Autofocus search properties.
        private const int FOCUS_MAX_POSITION_VARIATION = MIL.M_DEFAULT;
        private const int FOCUS_MODE = MIL.M_SMART_SCAN;
        private const int FOCUS_SENSITIVITY = 1;

        // User Data object definition.
        public class DigHookUserData
        {
            public MIL_ID SourceImage;
            public MIL_ID FocusImage;
            public MIL_ID Display;
            public int Iteration;
        };

        static void Main(string[] args)
        {
            MIL_ID MilApplication = MIL.M_NULL;                 // Application identifier.
            MIL_ID MilSystem = MIL.M_NULL;                      // System identifier.
            MIL_ID MilDisplay = MIL.M_NULL;                     // Display identifier.
            MIL_ID MilSource = MIL.M_NULL;                      // Source image.
            MIL_ID MilCameraFocus = MIL.M_NULL;                 // Focus simulated image.
            MIL_INT FocusPos = 0;                                   // Best focus position.

            DigHookUserData UserData = new DigHookUserData();   // User data passed to the hook.

            // Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, ref MilApplication, ref MilSystem, ref MilDisplay, MIL.M_NULL, MIL.M_NULL);

            // Load the source image.
            MIL.MbufRestore(IMAGE_FILE, MilSystem, ref MilSource);
            MIL.MbufRestore(IMAGE_FILE, MilSystem, ref MilCameraFocus);
            MIL.MbufClear(MilCameraFocus, 0);

            // Select image on the display.
            MIL.MdispSelect(MilDisplay, MilCameraFocus);

            // Simulate the first image grab.
            SimulateGrabFromCamera(MilSource, MilCameraFocus, FOCUS_START_POSITION, MilDisplay);

            // Initialize user data needed within the hook function.
            UserData.SourceImage = MilSource;
            UserData.FocusImage = MilCameraFocus;
            UserData.Iteration = 0;
            UserData.Display = MilDisplay;

            // Pause to show the original image.
            Console.Write("\nAUTOFOCUS:\n");
            Console.Write("----------\n\n");
            Console.Write("Automatic focusing operation will be done on this image.\n");
            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();
            Console.Write("Autofocusing...\n\n");

            // Perform Autofocus. 
            // Since lens movement is hardware specific, no digitizer is used here.
            // We simulate the lens movement with by smoothing the image data in 
            // the hook function instead.

            // get a handle to the DigHookUserData object in the managed heap, we will use this 
            // handle to get the object back in the callback function
            GCHandle hUserData = GCHandle.Alloc(UserData);

            // We give the handle to the DigHookUserData object by casting it to a IntPtr, 
            // later in the hook handler we will receive our IntPtr back and cast it again 
            // to a GCHandle to get a handle to our object
            MIL.MdigFocus(MIL.M_NULL, MilCameraFocus, MIL.M_DEFAULT, MoveLensHookFunction, GCHandle.ToIntPtr(hUserData), FOCUS_MIN_POSITION, FOCUS_START_POSITION, FOCUS_MAX_POSITION, FOCUS_MAX_POSITION_VARIATION, FOCUS_MODE + FOCUS_SENSITIVITY, ref FocusPos);

            // Free the GCHandle when no longer used
            hUserData.Free();

            // Print the best focus position and number of iterations.
            Console.Write("The best focus position is {0}.\n", FocusPos);
            Console.Write("The best focus position found in {0} iterations.\n\n", UserData.Iteration);
            Console.Write("Press <Enter> to end.\n");
            Console.ReadKey();

            // Free all allocations.
            MIL.MbufFree(MilSource);
            MIL.MbufFree(MilCameraFocus);
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL);
        }

        //*******************************************************************************
        // Autofocus hook function responsible to move the lens.

        static MIL_INT MoveLensHookFunction(MIL_INT HookType, MIL_INT Position, IntPtr UserDataHookPtr)
        {
            // this is how to check if the user data is null, the IntPtr class
            // contains a member, Zero, which exists solely for this purpose
            if (!IntPtr.Zero.Equals(UserDataHookPtr))
            {
                // get the handle to the DigHookUserData object back from the IntPtr
                GCHandle hUserData = GCHandle.FromIntPtr(UserDataHookPtr);

                // get a reference to the DigHookUserData object
                DigHookUserData UserData = hUserData.Target as DigHookUserData;

                // Here, the lens position must be changed according to the Position parameter.
                // In that case, we simulate the lens position change followed by a grab.
                if (HookType == MIL.M_CHANGE || HookType == MIL.M_ON_FOCUS)
                {
                    SimulateGrabFromCamera(UserData.SourceImage, UserData.FocusImage, (int)Position, UserData.Display);
                    UserData.Iteration++;
                }
            }

            return 0;
        }

        //**********************************************************************************
        // Utility function to simulate a grab from a camera at different lens position
        // by smoothing the original image. It should be replaced with a true camera grab.
        //
        // Note that this lens simulation will not work under MIL-lite because it uses
        // MimConvolve().

        // Lens simulation characteristics.
        private const double FOCUS_BEST_POSITION = (FOCUS_MAX_NB_POSITIONS / 2);

        static void SimulateGrabFromCamera(MIL_ID SourceImage, MIL_ID FocusImage, MIL_INT Iteration, MIL_ID AnnotationDisplay)
        {
            int NbSmoothNeeded = 0;                 // Number of smooths needed.

            MIL_INT BufType = 0;                        // Buffer type.
            MIL_INT BufSizeX = 0;                       // Buffer size X.
            MIL_INT BufSizeY = 0;                       // Buffer size Y.
            int Smooth = 0;                         // Smooth index.
            MIL_ID TempBuffer = MIL.M_NULL;         // Temporary buffer.
            MIL_ID SourceOwnerSystem = MIL.M_NULL;  // Owner system of the source buffer.

            // Compute number of smooths needed to simulate focus.
            NbSmoothNeeded = (int)Math.Abs(Iteration - FOCUS_BEST_POSITION);

            // Buffer inquires.
            BufType = MIL.MbufInquire(FocusImage, MIL.M_TYPE, MIL.M_NULL);
            BufSizeX = MIL.MbufInquire(FocusImage, MIL.M_SIZE_X, MIL.M_NULL);
            BufSizeY = MIL.MbufInquire(FocusImage, MIL.M_SIZE_Y, MIL.M_NULL);

            if (NbSmoothNeeded == 0)
            {
                // Directly copy image source to destination.
                MIL.MbufCopy(SourceImage, FocusImage);
            }
            else if (NbSmoothNeeded == 1)
            {
                // Directly convolve image from source to destination.
                MIL.MimConvolve(SourceImage, FocusImage, MIL.M_SMOOTH);
            }
            else
            {
                SourceOwnerSystem = (MIL_ID)MIL.MbufInquire(SourceImage, MIL.M_OWNER_SYSTEM, MIL.M_NULL);

                // Allocate temporary buffer.
                MIL.MbufAlloc2d(SourceOwnerSystem, BufSizeX, BufSizeY, BufType, MIL.M_IMAGE + MIL.M_PROC, ref TempBuffer);

                // Perform first smooth.
                MIL.MimConvolve(SourceImage, TempBuffer, MIL.M_SMOOTH);

                // Perform smooths.
                for (Smooth = 1; Smooth < NbSmoothNeeded - 1; Smooth++)
                {
                    MIL.MimConvolve(TempBuffer, TempBuffer, MIL.M_SMOOTH);
                }

                // Perform last smooth.
                MIL.MimConvolve(TempBuffer, FocusImage, MIL.M_SMOOTH);

                // Free temporary buffer.
                MIL.MbufFree(TempBuffer);
            }

            // Draw position cursor.
            DrawCursor(AnnotationDisplay, Iteration);
        }

        //***************************************************************
        // Draw position of the focus lens.

        // Cursor specifications.

        private static double CURSOR_POSITION(MIL_INT BufSizeY) { return ((BufSizeY * 7) / 8); }
        private const int CURSOR_SIZE = 14;
        private static readonly int CURSOR_COLOR = MIL.M_COLOR_GREEN;

        static void DrawCursor(MIL_ID AnnotationDisplay, MIL_INT Position)
        {
            MIL_ID AnnotationImage = MIL.M_NULL;
            MIL_INT BufSizeX = 0;
            MIL_INT BufSizeY = 0;
            MIL_INT n = 0;

            // Prepare for overlay annotations.
            MIL.MdispControl(AnnotationDisplay, MIL.M_OVERLAY, MIL.M_ENABLE);
            MIL.MdispControl(AnnotationDisplay, MIL.M_OVERLAY_CLEAR, MIL.M_DEFAULT);
            MIL.MdispInquire(AnnotationDisplay, MIL.M_OVERLAY_ID, ref AnnotationImage);
            MIL.MbufInquire(AnnotationImage, MIL.M_SIZE_X, ref BufSizeX);
            MIL.MbufInquire(AnnotationImage, MIL.M_SIZE_Y, ref BufSizeY);
            MIL.MgraColor(MIL.M_DEFAULT, CURSOR_COLOR);

            // Write annotations.
            n = (BufSizeX / FOCUS_MAX_NB_POSITIONS);
            MIL.MgraLine(MIL.M_DEFAULT, AnnotationImage, 0, CURSOR_POSITION(BufSizeY) + CURSOR_SIZE, BufSizeX - 1, CURSOR_POSITION(BufSizeY) + CURSOR_SIZE);
            MIL.MgraLine(MIL.M_DEFAULT, AnnotationImage, Position * n, CURSOR_POSITION(BufSizeY) + CURSOR_SIZE, Position * n - CURSOR_SIZE, CURSOR_POSITION(BufSizeY));
            MIL.MgraLine(MIL.M_DEFAULT, AnnotationImage, Position * n, CURSOR_POSITION(BufSizeY) + CURSOR_SIZE, Position * n + CURSOR_SIZE, CURSOR_POSITION(BufSizeY));
            MIL.MgraLine(MIL.M_DEFAULT, AnnotationImage, Position * n - CURSOR_SIZE, CURSOR_POSITION(BufSizeY), Position * n + CURSOR_SIZE, CURSOR_POSITION(BufSizeY));
        }
    }
}
