//********************************************************************************
//
// File name: MdigGrab.cs
//
// Synopsis:  This program demonstrates how to grab from a camera in
//            continuous and monoshot mode.
//********************************************************************************
using System;
using System.Collections.Generic;
using System.Text;

using Matrox.MatroxImagingLibrary;

namespace MDigGrab
{
    class Program
    {
        static void Main(string[] args)
        {
            MIL_ID MilApplication = MIL.M_NULL;         // Application identifier.
            MIL_ID MilSystem = MIL.M_NULL;              // System identifier.
            MIL_ID MilDisplay = MIL.M_NULL;             // Display identifier.
            MIL_ID MilDigitizer = MIL.M_NULL;           // Digitizer identifier.
            MIL_ID MilImage = MIL.M_NULL;               // Image buffer identifier.

            // Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, ref MilApplication, ref MilSystem, ref MilDisplay, ref MilDigitizer, ref MilImage);

            // Grab continuously.
            MIL.MdigGrabContinuous(MilDigitizer, MilImage);

            // When a key is pressed, halt.
            Console.Write("\nDIGITIZER ACQUISITION:\n");
            Console.Write("----------------------\n\n");
            Console.Write("Continuous image grab in progress.\n");
            Console.Write("Press <Enter> to stop.\n\n");
            Console.ReadKey();

            // Stop continuous grab.
            MIL.MdigHalt(MilDigitizer);

            // Pause to show the result.
            Console.Write("Continuous grab stopped.\n\n");
            Console.Write("Press <Enter> to do a single image grab.\n\n");
            Console.ReadKey();

            // Monoshot grab.
            MIL.MdigGrab(MilDigitizer, MilImage);

            // Pause to show the result.
            Console.Write("Displaying the grabbed image.\n");
            Console.Write("Press <Enter> to end.\n\n");
            Console.ReadKey();

            // Free defaults.
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MilDigitizer, MilImage);
        }
    }
}
