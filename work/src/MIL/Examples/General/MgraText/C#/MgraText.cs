//*****************************************************************************
//
// File name: MgraText.cs
//
// Synopsis:  This program allocates a MIL application and system, then 
//            displays messages using TrueType fonts. It also shows how to 
//            check for errors.
//*****************************************************************************
using System;

using Matrox.MatroxImagingLibrary;

namespace MgraText
{
    class Program
    {
        static void Main(string[] args)
        {
            MIL_ID MilApplication = MIL.M_NULL;         // Application identifier.
            MIL_ID MilSystem = MIL.M_NULL;              // System identifier.
            MIL_ID MilDisplay = MIL.M_NULL;             // Display identifier.
            MIL_ID MilImage = MIL.M_NULL;               // Image buffer identifier.
            MIL_ID MilGraphicContextId = MIL.M_NULL;    // Graphic context identifier

            // Allocate a default MIL application, system, display and image.
            MIL.MappAllocDefault(MIL.M_DEFAULT, ref MilApplication, ref MilSystem, ref MilDisplay, MIL.M_NULL, ref MilImage);

            // Allocate a graphic context
            MIL.MgraAlloc(MilSystem, ref MilGraphicContextId);

            // Perform graphic operations in the display image.
            MIL.MgraColor(MilGraphicContextId, 0xF0);
            MIL.MgraFont(MilGraphicContextId, MIL.M_FONT_DEFAULT_LARGE);
            MIL.MgraText(MilGraphicContextId, MilImage, 160L, 20L, " Welcome to MIL !!! ");

            MIL.MgraControl(MilGraphicContextId, MIL.M_FONT_SIZE, 12);
            MIL.MgraFont(MilGraphicContextId, MIL.MIL_FONT_NAME(MIL.M_FONT_DEFAULT_TTF));
            MIL.MgraText(MilGraphicContextId, MilImage, 40L, 80L, "English");

            MIL.MappControl(MIL.M_DEFAULT, MIL.M_ERROR, MIL.M_PRINT_DISABLE);

            MIL.MgraControl(MilGraphicContextId, MIL.M_FONT_SIZE, 16);
            MIL.MgraFont(MilGraphicContextId, MIL.MIL_FONT_NAME(MIL.M_FONT_DEFAULT_TTF + ":Bold"));
            MIL.MgraText(MilGraphicContextId, MilImage, 40L, 140L, "Français");

            MIL.MgraControl(MilGraphicContextId, MIL.M_FONT_SIZE, 24);
            MIL.MgraFont(MilGraphicContextId, MIL.MIL_FONT_NAME(MIL.M_FONT_DEFAULT_TTF + ":Italic"));
            MIL.MgraText(MilGraphicContextId, MilImage, 40L, 220L, "Italiano");

            MIL.MgraControl(MilGraphicContextId, MIL.M_FONT_SIZE, 30);
            MIL.MgraFont(MilGraphicContextId, MIL.MIL_FONT_NAME(MIL.M_FONT_DEFAULT_TTF + ":Bold:Italic"));
            MIL.MgraText(MilGraphicContextId, MilImage, 40L, 300L, "Deutsch");

            MIL.MgraControl(MilGraphicContextId, MIL.M_FONT_SIZE, 36);
            MIL.MgraFont(MilGraphicContextId, MIL.MIL_FONT_NAME("Courier New"));
            MIL.MgraText(MilGraphicContextId, MilImage, 40L, 380L, "Español");

            if (MIL.M_MIL_USE_TTF_UNICODE == 1)
            {
                // Draw Greek, Japanese and Korean
                MIL.MgraFont(MilGraphicContextId, MIL.MIL_FONT_NAME(MIL.M_FONT_DEFAULT_TTF));

                MIL.MgraControl(MilGraphicContextId, MIL.M_FONT_SIZE, 12);
                MIL.MgraText(MilGraphicContextId, MilImage, 400L, 80L, "ελληνιδ");

                MIL.MgraControl(MilGraphicContextId, MIL.M_FONT_SIZE, 16);
                MIL.MgraText(MilGraphicContextId, MilImage, 400L, 140L, "日本語");

                MIL.MgraControl(MilGraphicContextId, MIL.M_FONT_SIZE, 24);
                MIL.MgraText(MilGraphicContextId, MilImage, 400L, 220L, "한국어");

                // Draw Arabic and Hebrew
                MIL.MgraControl(MilGraphicContextId, MIL.M_TEXT_DIRECTION, MIL.M_RIGHT_TO_LEFT);
                MIL.MgraControl(MilGraphicContextId, MIL.M_FONT_SIZE, 30);
                MIL.MgraText(MilGraphicContextId, MilImage, 400L, 320L, "עברית");

                MIL.MgraControl(MilGraphicContextId, MIL.M_FONT_SIZE, 36);
                MIL.MgraText(MilGraphicContextId, MilImage, 400L, 380L, "ﻋﺮﺑﻲ");
            }

            // Print a message.
            Console.WriteLine();
            Console.WriteLine("INTERNATIONAL TEXT ANNOTATION:");
            Console.WriteLine("------------------------------");
            Console.WriteLine();
            Console.WriteLine("This example demonstrates the support of TrueType fonts by MgraText.");
            Console.WriteLine();

            if (MIL.MappGetError(MIL.M_GLOBAL + MIL.M_SYNCHRONOUS, MIL.M_NULL) != MIL.M_NULL_ERROR)
            {
                Console.WriteLine("Note: Some Unicode fonts are not available");
                Console.WriteLine();
            }

            // Wait for a key press.
            Console.WriteLine("Press <Enter> to end.");
            Console.ReadKey();

            // Free Graphic Context
            MIL.MgraFree(MilGraphicContextId);

            // Free defaults.
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MilImage);
        }
    }
}
