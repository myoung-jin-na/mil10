//*******************************************************************************
//
// File name: Mocr.cs
//
// Synopsis:  This program uses the OCR module to read a SEMI font string:
//            the example calibrates a predefined OCR font (semi font) 
//            and uses it to read a string in the image. The string read
//            is then printed to the screen and the calibrated font is
//            saved to disk.
//*******************************************************************************
using System;
using System.Collections.Generic;
using System.Text;

using Matrox.MatroxImagingLibrary;

namespace MOcr
{
    class Program
    {
        // *****************************************************************************
        // OCR SEMI font read example.
        // *****************************************************************************

        // Target image character specifications.
        private const string CHAR_IMAGE_FILE = MIL.M_IMAGE_PATH + "OcrSemi1292.mim";
        private const double CHAR_SIZE_X_MIN = 22.0;
        private const double CHAR_SIZE_X_MAX = 23.0;
        private const double CHAR_SIZE_X_STEP = 0.50;
        private const double CHAR_SIZE_Y_MIN = 43.0;
        private const double CHAR_SIZE_Y_MAX = 44.0;
        private const double CHAR_SIZE_Y_STEP = 0.50;

        // Target reading specifications.
        private const int READ_REGION_POS_X = 30;
        private const int READ_REGION_POS_Y = 40;
        private const int READ_REGION_WIDTH = 420;
        private const int READ_REGION_HEIGHT = 70;
        private const double READ_SCORE_MIN = 50.0;

        // Font file names. 
        private const string FONT_FILE_IN = MIL.M_CONTEXT_PATH + "SEMI_M12-92.mfo";
        private const string FONT_FILE_OUT = MIL.M_TEMP_DIR + "Semi1292Calibrated.mfo";

        // Length of the string to read (null terminated).
        private const int STRING_LENGTH = 13;
        private const string STRING_CALIBRATION = "M940902-MXD5";

        static void Main(string[] args)
        {
            MIL_ID MilApplication = MIL.M_NULL;               // Application identifier.
            MIL_ID MilSystem = MIL.M_NULL;                    // System identifier.
            MIL_ID MilDisplay = MIL.M_NULL;                   // Display identifier.
            MIL_ID MilImage = MIL.M_NULL;                     // Image buffer identifier.
            MIL_ID MilSubImage = MIL.M_NULL;                  // Sub-image buffer identifier.
            MIL_ID MilFontSubImage = MIL.M_NULL;              // Font display sub image.
            MIL_ID MilOverlayImage = MIL.M_NULL;              // Overlay image.
            MIL_ID OcrFont = MIL.M_NULL;                      // OCR font identifier.
            MIL_ID OcrResult = MIL.M_NULL;                    // OCR result buffer identifier.
            double Score = 0;                                 // Reading score.
            MIL_INT SizeX = 0;                                   // Source image size x.
            MIL_INT SizeY = 0;                                   // Source image size y.
            MIL_INT Type = 0;                                    // Source image type.

            StringBuilder ReadString = new StringBuilder(STRING_LENGTH); // Characters to read.

            Console.Write("\nOCR MODULE (SEMI font reading):\n");
            Console.Write("-------------------------------\n\n");

            // Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, ref MilApplication, ref MilSystem, ref MilDisplay, MIL.M_NULL, MIL.M_NULL);

            // Load and display the source image into a new image buffer.
            MIL.MbufAlloc2d(MilSystem,
                        MIL.MbufDiskInquire(CHAR_IMAGE_FILE, MIL.M_SIZE_X, ref SizeX),
                        MIL.MbufDiskInquire(CHAR_IMAGE_FILE, MIL.M_SIZE_Y, ref SizeY) * 3 / 2,
                        MIL.MbufDiskInquire(CHAR_IMAGE_FILE, MIL.M_TYPE, ref Type),
                        MIL.M_IMAGE + MIL.M_PROC + MIL.M_DISP,
                        ref MilImage);
            MIL.MbufClear(MilImage, 0);
            MIL.MbufLoad(CHAR_IMAGE_FILE, MilImage);
            MIL.MdispSelect(MilDisplay, MilImage);

            // Restrict the region of the image where to read the string.
            MIL.MbufChild2d(MilImage, READ_REGION_POS_X, READ_REGION_POS_Y,
                        READ_REGION_WIDTH, READ_REGION_HEIGHT, ref MilSubImage);

            // Define the bottom of the image as the region where to copy the font representation.
            MIL.MbufChild2d(MilImage, 50, SizeY + 10, SizeX - 100, (SizeY / 3) - 10, ref MilFontSubImage);


            // Restore the OCR character font from disk.
            MIL.MocrRestoreFont(FONT_FILE_IN, MIL.M_RESTORE, MilSystem, ref OcrFont);

            /* Show the font representation. */
            MIL.MocrCopyFont(MilFontSubImage, OcrFont, MIL.M_COPY_FROM_FONT + MIL.M_ALL_CHAR, "");

            // Pause to show the original image.
            Console.Write("The SEMI string at the top will be read using the font displayed at the bottom.\n\n");
            Console.Write("Calibrating SEMI font...\n\n");

            // Calibrate the OCR font.
            MIL.MocrCalibrateFont(MilSubImage, OcrFont, STRING_CALIBRATION,
                              CHAR_SIZE_X_MIN, CHAR_SIZE_X_MAX, CHAR_SIZE_X_STEP,
                              CHAR_SIZE_Y_MIN, CHAR_SIZE_Y_MAX, CHAR_SIZE_Y_STEP,
                              MIL.M_DEFAULT);

            // Set the user-specific character constraints for each string position.
            MIL.MocrSetConstraint(OcrFont, 0, MIL.M_LETTER);            // A to Z only
            MIL.MocrSetConstraint(OcrFont, 1, MIL.M_DIGIT, "9");            // 9      only
            MIL.MocrSetConstraint(OcrFont, 2, MIL.M_DIGIT);             // 0 to 9 only
            MIL.MocrSetConstraint(OcrFont, 3, MIL.M_DIGIT);             // 0 to 9 only
            MIL.MocrSetConstraint(OcrFont, 4, MIL.M_DIGIT);             // 0 to 9 only
            MIL.MocrSetConstraint(OcrFont, 5, MIL.M_DIGIT);             // 0 to 9 only
            MIL.MocrSetConstraint(OcrFont, 6, MIL.M_DIGIT);             // 0 to 9 only
            MIL.MocrSetConstraint(OcrFont, 7, MIL.M_DEFAULT, "-");          // -      only
            MIL.MocrSetConstraint(OcrFont, 8, MIL.M_LETTER, "M");           // M      only
            MIL.MocrSetConstraint(OcrFont, 9, MIL.M_LETTER, "X");           // X      only
            MIL.MocrSetConstraint(OcrFont, 10, MIL.M_LETTER, "ABCDEFGH");   // SEMI checksum
            MIL.MocrSetConstraint(OcrFont, 11, MIL.M_DIGIT, "01234567");    // SEMI checksum

            // Pause before the read operation.
            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();

            // Allocate an OCR result buffer.
            MIL.MocrAllocResult(MilSystem, MIL.M_DEFAULT, ref OcrResult);

            // Read the string.
            MIL.MocrReadString(MilSubImage, OcrFont, OcrResult);

            // Get the string and its reading score.
            MIL.MocrGetResult(OcrResult, MIL.M_STRING, ReadString);
            MIL.MocrGetResult(OcrResult, MIL.M_STRING_SCORE, ref Score);

            // Print the result.
            Console.Write("\nThe string read is: \"{0}\" (score: {1:P1}).\n\n", ReadString.ToString(), Score / 100);

            // Draw the string in the overlay under the reading region.
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY, MIL.M_ENABLE);
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY_CLEAR, MIL.M_DEFAULT);
            MIL.MdispInquire(MilDisplay, MIL.M_OVERLAY_ID, ref MilOverlayImage);
            MIL.MgraFont(MIL.M_DEFAULT, MIL.M_FONT_DEFAULT_LARGE);
            MIL.MgraColor(MIL.M_DEFAULT, MIL.M_COLOR_YELLOW);
            MIL.MgraText(MIL.M_DEFAULT, MilOverlayImage,
                READ_REGION_POS_X + (READ_REGION_WIDTH / 4),
                READ_REGION_POS_Y + READ_REGION_HEIGHT + 50,
                ReadString.ToString());

            // Save the calibrated font if the reading score was sufficiently high.
            if (Score > READ_SCORE_MIN)
            {
                MIL.MocrSaveFont(FONT_FILE_OUT, MIL.M_SAVE, OcrFont);
                Console.Write("Read successful, calibrated OCR font was saved to disk.\n");
            }
            else
            {
                Console.Write("Error: Read score too low, calibrated OCR font not saved.\n");
            }
            Console.Write("Press <Enter> to end.\n\n\n");
            Console.ReadKey();

            // Clear the overlay.
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY_CLEAR, MIL.M_DEFAULT);

            // Free all allocations.
            MIL.MocrFree(OcrFont);
            MIL.MocrFree(OcrResult);
            MIL.MbufFree(MilSubImage);
            MIL.MbufFree(MilFontSubImage);
            MIL.MbufFree(MilImage);
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL);
        }
    }
}
