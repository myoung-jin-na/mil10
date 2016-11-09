//*****************************************************************************
//
// File name:  Mstr.cs
//
// Synopsis:  This program uses the String Reader module to define a font 
//            from an image containing a mosaic of Quebec license plates.
//            Two String Models are then defined and parameterized to read 
//            only some valid Quebec license plates.
//            A license plate reading is then performed in a target image 
//            of a car on the road.
//
//*****************************************************************************
using System;
using System.Collections.Generic;
using System.Text;

using Matrox.MatroxImagingLibrary;

namespace MStr
{
    class Program
    {
        // MIL image file specifications.
        private const string IMAGE_FILE_DEFINITION = MIL.M_IMAGE_PATH + "QcPlates.mim";
        private const string IMAGE_FILE_TO_READ = MIL.M_IMAGE_PATH + "LicPlate.mim";

        // String containing all characters used for font definition.
        private const string TEXT_DEFINITION = "AVS300CVK781JNK278 EBX380QKN918HCC839 YRH900ZQR756977AMQ GPK742389EYE569ESQ";

        // Font normalization size Y.
        private const int NORMALIZATION_SIZE_Y = 20;

        // Max size of plate string.
        private const int STRING_MAX_SIZE = 32;

        static void Main(string[] args)
        {
            MIL_ID MilApplication = MIL.M_NULL;     // Application identifier.
            MIL_ID MilSystem = MIL.M_NULL;          // System identifier.
            MIL_ID MilDisplay = MIL.M_NULL;         // Display identifier.
            MIL_ID MilImage = MIL.M_NULL;           // Image buffer identifier.
            MIL_ID MilOverlayImage = MIL.M_NULL;    // Overlay image.
            MIL_ID MilStrContext = MIL.M_NULL;      // String context identifier.
            MIL_ID MilStrResult = MIL.M_NULL;       // String result buffer identifier.
            MIL_INT NumberOfStringRead = 0;             // Total number of strings to read.
            double Score = 0.0;                     // String score.
            StringBuilder StringResult = new StringBuilder(STRING_MAX_SIZE + 1); // String of characters read.
            double Time = 0.0;                      // Time variable.

            // Print module name.
            Console.Write("\nSTRING READER MODULE:\n");
            Console.Write("---------------------\n\n");

            // Allocate defaults
            MIL.MappAllocDefault(MIL.M_DEFAULT, ref MilApplication, ref MilSystem, ref MilDisplay, MIL.M_NULL, MIL.M_NULL);

            // Restore the font definition image
            MIL.MbufRestore(IMAGE_FILE_DEFINITION, MilSystem, ref MilImage);

            // Display the image and prepare for overlay annotations.
            MIL.MdispSelect(MilDisplay, MilImage);
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY, MIL.M_ENABLE);
            MIL.MdispInquire(MilDisplay, MIL.M_OVERLAY_ID, ref MilOverlayImage);

            // Allocate a new empty String Reader context.
            MIL.MstrAlloc(MilSystem, MIL.M_FONT_BASED, MIL.M_DEFAULT, ref MilStrContext);

            // Allocate a new empty String Reader result buffer.
            MIL.MstrAllocResult(MilSystem, MIL.M_DEFAULT, ref MilStrResult);

            MIL.MstrControl(MilStrContext, MIL.M_CONTEXT, MIL.M_ENCODING, MIL.M_UNICODE);

            // Add a new empty user defined font to the context.
            MIL.MstrControl(MilStrContext, MIL.M_CONTEXT, MIL.M_FONT_ADD, MIL.M_USER_DEFINED);

            // Add user defined characters from the license plate mosaic image.
            MIL.MstrEditFont(MilStrContext, MIL.M_FONT_INDEX(0), MIL.M_CHAR_ADD, MIL.M_USER_DEFINED + MIL.M_FOREGROUND_BLACK, MilImage, TEXT_DEFINITION);

            // Draw all the characters in the font in the overlay image.
            MIL.MgraColor(MIL.M_DEFAULT, MIL.M_COLOR_GREEN);
            MIL.MstrDraw(MIL.M_DEFAULT, MilStrContext, MilOverlayImage, MIL.M_DRAW_CHAR, MIL.M_FONT_INDEX(0), MIL.M_ORIGINAL);

            // Normalize the characters of the font to an appropriate size.
            MIL.MstrEditFont(MilStrContext, MIL.M_FONT_INDEX(0), MIL.M_CHAR_NORMALIZE, MIL.M_SIZE_Y, NORMALIZATION_SIZE_Y);

            // Add 2 new empty strings models to the context for the 2 valid types of 
            // plates (3 letters followed by 3 numbers or 3 numbers followed by 3 letters)
            // Note that the read time increases with the number of strings in the context.
            MIL.MstrControl(MilStrContext, MIL.M_CONTEXT, MIL.M_STRING_ADD, MIL.M_USER_DEFINED);
            MIL.MstrControl(MilStrContext, MIL.M_CONTEXT, MIL.M_STRING_ADD, MIL.M_USER_DEFINED);

            // Set number of strings to read.
            MIL.MstrControl(MilStrContext, MIL.M_CONTEXT, MIL.M_STRING_NUMBER, 1);

            // Set number of expected characters for all string models to 6 characters.
            MIL.MstrControl(MilStrContext, MIL.M_STRING_INDEX(MIL.M_ALL), MIL.M_STRING_SIZE_MIN, 6);
            MIL.MstrControl(MilStrContext, MIL.M_STRING_INDEX(MIL.M_ALL), MIL.M_STRING_SIZE_MAX, 6);

            // Set default constraint to uppercase letter for all string models
            MIL.MstrSetConstraint(MilStrContext, MIL.M_STRING_INDEX(0), MIL.M_DEFAULT, MIL.M_LETTER + MIL.M_UPPERCASE, IntPtr.Zero);
            MIL.MstrSetConstraint(MilStrContext, MIL.M_STRING_INDEX(1), MIL.M_DEFAULT, MIL.M_LETTER + MIL.M_UPPERCASE, IntPtr.Zero);

            // Set constraint of 3 last characters to digit for the first string model
            MIL.MstrSetConstraint(MilStrContext, MIL.M_STRING_INDEX(0), 3, MIL.M_DIGIT, IntPtr.Zero);
            MIL.MstrSetConstraint(MilStrContext, MIL.M_STRING_INDEX(0), 4, MIL.M_DIGIT, IntPtr.Zero);
            MIL.MstrSetConstraint(MilStrContext, MIL.M_STRING_INDEX(0), 5, MIL.M_DIGIT, IntPtr.Zero);

            // Set constraint of 3 first characters to digit for the second string model
            MIL.MstrSetConstraint(MilStrContext, MIL.M_STRING_INDEX(1), 0, MIL.M_DIGIT, IntPtr.Zero);
            MIL.MstrSetConstraint(MilStrContext, MIL.M_STRING_INDEX(1), 1, MIL.M_DIGIT, IntPtr.Zero);
            MIL.MstrSetConstraint(MilStrContext, MIL.M_STRING_INDEX(1), 2, MIL.M_DIGIT, IntPtr.Zero);

            // Pause to show the font definition.
            Console.Write("This program has defined a font with this Quebec plates mosaic image.\n");
            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();

            // Clear the display overlay.
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY_CLEAR, MIL.M_DEFAULT);

            // Load image to read into image buffer.
            MIL.MbufLoad(IMAGE_FILE_TO_READ, MilImage);

            // Preprocess the String Reader context.
            MIL.MstrPreprocess(MilStrContext, MIL.M_DEFAULT);

            // Dummy first call for bench measure purpose only (bench stabilization, cache effect, etc...). This first call is NOT required by the application.
            MIL.MstrRead(MilStrContext, MilImage, MilStrResult);

            // Reset the timer.
            MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_RESET + MIL.M_SYNCHRONOUS, MIL.M_NULL);

            // Perform the read operation on the specified target image.
            MIL.MstrRead(MilStrContext, MilImage, MilStrResult);

            // Read the time.
            MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ + MIL.M_SYNCHRONOUS, ref Time);

            // Get number of strings read and show the result.
            MIL.MstrGetResult(MilStrResult, MIL.M_GENERAL, MIL.M_STRING_NUMBER + MIL.M_TYPE_MIL_INT, ref NumberOfStringRead);
            if (NumberOfStringRead >= 1)
            {
                Console.Write("The license plate was read successfully ({0:#.##} ms)\n\n", Time * 1000);

                // Draw read result.
                MIL.MgraColor(MIL.M_DEFAULT, MIL.M_COLOR_BLUE);
                MIL.MstrDraw(MIL.M_DEFAULT, MilStrResult, MilOverlayImage, MIL.M_DRAW_STRING, MIL.M_ALL, MIL.M_DEFAULT);
                MIL.MgraColor(MIL.M_DEFAULT, MIL.M_COLOR_GREEN);
                MIL.MstrDraw(MIL.M_DEFAULT, MilStrResult, MilOverlayImage, MIL.M_DRAW_STRING_BOX, MIL.M_ALL, MIL.M_DEFAULT);

                // Print the read result.
                Console.Write(" String                   Score\n");
                Console.Write(" -----------------------------------\n");
                MIL.MstrGetResult(MilStrResult, 0, MIL.M_STRING + MIL.M_TYPE_TEXT_CHAR, StringResult);
                MIL.MstrGetResult(MilStrResult, 0, MIL.M_STRING_SCORE, ref Score);
                Console.Write(" {0}                  {1:0.0}\n", StringResult.ToString(), Score);
            }
            else
            {
                Console.Write("Error: Plate was not read.\n");
            }

            // Pause to show results.
            Console.Write("\nPress <Enter> to end.\n\n");
            Console.ReadKey();

            // Free all allocations.
            MIL.MstrFree(MilStrContext);
            MIL.MstrFree(MilStrResult);
            MIL.MbufFree(MilImage);

            // Free defaults.
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL);
        }
    }
}
