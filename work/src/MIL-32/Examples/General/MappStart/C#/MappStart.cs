//*****************************************************************************
//
// File name: MappStart.cs
//
// Synopsis:  This program allocates a MIL application and system, then displays 
//            a welcoming message using graphics functions. It also shows how 
//            to check for errors.
//*****************************************************************************
using System;
using System.Collections.Generic;
using System.Text;

using Matrox.MatroxImagingLibrary;

namespace MAppStart
{
    class Program
    {
        static void Main(string[] args)
        {
            MIL_ID MilApplication = MIL.M_NULL;     // Application identifier.
            MIL_ID MilSystem = MIL.M_NULL;          // System identifier.
            MIL_ID MilDisplay = MIL.M_NULL;         // Display identifier.
            MIL_ID MilImage = MIL.M_NULL;           // Image buffer identifier.

            // Allocate a default MIL application, system, display and image.
            MIL.MappAllocDefault(MIL.M_DEFAULT, ref MilApplication, ref MilSystem, ref MilDisplay, MIL.M_NULL, ref MilImage);

            // If no allocation errors.
            if (MIL.MappGetError(MIL.M_DEFAULT, MIL.M_GLOBAL, MIL.M_NULL) == 0)
            {
                // Perform graphic operations in the display image.
                MIL.MgraColor(MIL.M_DEFAULT, 0xF0);
                MIL.MgraFont(MIL.M_DEFAULT, MIL.M_FONT_DEFAULT_LARGE);
                MIL.MgraText(MIL.M_DEFAULT, MilImage, 160L, 230L, " Welcome to MIL !!! ");
                MIL.MgraColor(MIL.M_DEFAULT, 0xC0);
                MIL.MgraRect(MIL.M_DEFAULT, MilImage, 100, 150, 530, 340);
                MIL.MgraRect(MIL.M_DEFAULT, MilImage, 120, 170, 510, 320);
                MIL.MgraRect(MIL.M_DEFAULT, MilImage, 140, 190, 490, 300);

                // Print a message.
                Console.Write("\nSYSTEM ALLOCATION:\n");
                Console.Write("------------------\n\n");
                Console.Write("System allocation successful.\n\n");
                Console.Write("     \"Welcome to MIL !!!\"\n\n");
            }
            else
            {
                Console.Write("System allocation error !\n\n");
            }

            // Wait for a key press.
            Console.Write("Press <Enter> to end.\n");
            Console.ReadKey();

            // Free defaults.
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MilImage);
        }
    }
}
