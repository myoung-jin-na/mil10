//************************************************************************************
//
// File name: MdispWindowLeveling.cs
//
// Synopsis:  This MIL program shows how to display a 10-bit monochrome Medical image
//            and applies a LUT to do interactive Window Leveling.
//************************************************************************************
using System;
using System.Collections.Generic;
using System.Text;

using Matrox.MatroxImagingLibrary;

namespace MDispWindowLeveling
{
    class Program
    {
        // Image file to load.
        private const string IMAGE_NAME = "ArmsMono10bit.mim";
        private const string IMAGE_FILE = MIL.M_IMAGE_PATH + IMAGE_NAME;

        // Draw the LUT shape (if disabled reduces CPU usage).
        private const int DRAW_LUT_SHAPE = MIL.M_YES;
        static void Main(string[] args)
        {
            MIL_ID MilApplication = MIL.M_NULL;         // Application identifier.
            MIL_ID MilSystem = MIL.M_NULL;              // System identifier.
            MIL_ID MilDisplay = MIL.M_NULL;             // Display identifier.
            MIL_ID MilImage = MIL.M_NULL;               // Image buffer identifier.
            MIL_ID MilOriginalImage = MIL.M_NULL;       // Image buffer identifier.
            MIL_ID MilLut = MIL.M_NULL;                 // Lut buffer identifier.
            MIL_INT ImageSizeX = 0;
            MIL_INT ImageSizeY = 0;
            MIL_INT ImageMaxValue = 0;
            MIL_INT DisplaySizeBit = 0;
            MIL_INT DisplayMaxValue = 0;
            MIL_INT Start = 0;
            MIL_INT End = 0;
            MIL_INT Step = 0;
            MIL_INT InflectionLevel = 0;

            ConsoleKeyInfo Ch = new ConsoleKeyInfo();

            // Allocate the MIL Application, System and Display.
            MIL.MappAllocDefault(MIL.M_DEFAULT, ref MilApplication, ref MilSystem, ref MilDisplay, MIL.M_NULL, MIL.M_NULL);

            // Restore target image from disk.
            MIL.MbufRestore(IMAGE_FILE, MilSystem, ref MilImage);

            // Dynamically calculates the maximum value of the image using processing.
            MIL_ID MilExtremeResult = MIL.M_NULL;
            MIL.MimAllocResult((MIL_ID)MIL.MbufInquire(MilImage, MIL.M_OWNER_SYSTEM, MIL.M_NULL), 1, MIL.M_EXTREME_LIST, ref MilExtremeResult);
            MIL.MimFindExtreme(MilImage, MilExtremeResult, MIL.M_MAX_VALUE);
            MIL.MimGetResult(MilExtremeResult, MIL.M_VALUE, ref ImageMaxValue);
            MIL.MimFree(MilExtremeResult);

            // Set the maximum value of the image to indicate to MIL how to initialize 
            // the default display LUT.
            //
            MIL.MbufControl(MilImage, MIL.M_MAX, (double)ImageMaxValue);

            // Display the image (to specify a user-defined window, use MIL.MdispSelectWindow()).
            MIL.MdispSelect(MilDisplay, MilImage);

            // Determine the maximum displayable value of the current display.
            MIL.MdispInquire(MilDisplay, MIL.M_SIZE_BIT, ref DisplaySizeBit);
            DisplayMaxValue = (1 << (int)DisplaySizeBit) - 1;

            // Print key information.
            Console.Write("\nINTERACTIVE WINDOW LEVELING:\n");
            Console.Write("----------------------------\n\n");

            Console.Write("Image name : {0}\n", IMAGE_NAME);

            Console.Write("Image size : {0} x {1}\n", MIL.MbufInquire(MilImage, MIL.M_SIZE_X, ref ImageSizeX), MIL.MbufInquire(MilImage, MIL.M_SIZE_Y, ref ImageSizeY));
            Console.Write("Image max  : {0,4}\n", ImageMaxValue);
            Console.Write("Display max: {0,4}\n\n", DisplayMaxValue);

            // Allocate a LUT buffer according to the image maximum value and display pixel depth.
            MIL.MbufAlloc1d(MilSystem, ImageMaxValue + 1, ((DisplaySizeBit > 8) ? 16 : 8) + MIL.M_UNSIGNED, MIL.M_LUT, ref MilLut);

            // Generate a LUT with a full range ramp and set its maximum value.
            MIL.MgenLutRamp(MilLut, 0, 0, ImageMaxValue, (double)DisplayMaxValue);
            MIL.MbufControl(MilLut, MIL.M_MAX, (double)DisplayMaxValue);

            // Set the display LUT.
            MIL.MdispLut(MilDisplay, MilLut);

            // Interactive Window Leveling using keyboard.
            Console.Write("Keys assignement:\n\n");
            Console.Write("Arrow keys :    Left=move Left, Right=move Right, Down=Narrower, Up=Wider.\n");
            Console.Write("Intensity keys: L=Lower,  U=Upper,  R=Reset.\n");
            Console.Write("Press <Enter> to end.\n\n");

            // Modify LUT shape according to the arrow keys and update it.
            Start = 0;
            End = ImageMaxValue;
            InflectionLevel = DisplayMaxValue;
            Step = (ImageMaxValue + 1) / 128;
            Step = Math.Max(Step, 4);
            while (Ch.Key != ConsoleKey.Enter)
            {
                switch (Ch.Key)
                {
                    // Left arrow: Move region left.
                    case ConsoleKey.LeftArrow:
                        { Start -= Step; End -= Step; break; }

                    // Right arrow: Move region right.
                    case ConsoleKey.RightArrow:
                        { Start += Step; End += Step; break; }

                    // Down arrow: Narrow region.
                    case ConsoleKey.DownArrow:
                        { Start += Step; End -= Step; break; }

                    // Up arrow: Widen region.
                    case ConsoleKey.UpArrow:
                        { Start -= Step; End += Step; break; }

                    // L key: Lower inflexion point.
                    case ConsoleKey.L:
                        { InflectionLevel--; break; }

                    // U key: Upper inflexion point.
                    case ConsoleKey.U:
                        { InflectionLevel++; break; }

                    // R key: Reset the LUT to full image range.
                    case ConsoleKey.R:
                        { Start = 0; End = ImageMaxValue; InflectionLevel = DisplayMaxValue; break; }
                }

                // Saturate.
                End = Math.Min(End, ImageMaxValue);
                Start = Math.Min(Start, End);
                End = Math.Max(End, Start);
                Start = Math.Max(Start, 0);
                End = Math.Max(End, 0);
                InflectionLevel = Math.Max(InflectionLevel, 0);
                InflectionLevel = Math.Min(InflectionLevel, DisplayMaxValue);
                Console.Write("Inflection points: Low=({0},0), High=({1},{2}).   \r", Start, End, InflectionLevel);

                // Generate a LUT with 3 slopes and saturated at both ends.
                MIL.MgenLutRamp(MilLut, 0, 0, Start, 0);
                MIL.MgenLutRamp(MilLut, Start, 0, End, (double)InflectionLevel);
                MIL.MgenLutRamp(MilLut, End, (double)InflectionLevel, ImageMaxValue, (double)DisplayMaxValue);

                // Update the display LUT.
                MIL.MdispLut(MilDisplay, MilLut);

                // Draw the current LUT's shape in the image.
                // Note: This simple annotation method requires
                //       significant update and CPU time.
                //
                if (DRAW_LUT_SHAPE == MIL.M_YES)
                {
                    if (MilOriginalImage == MIL.M_NULL)
                    {
                        MIL.MbufRestore(IMAGE_FILE, MilSystem, ref MilOriginalImage);
                    }
                    DrawLutShape(MilDisplay, MilOriginalImage, MilImage, Start, End, InflectionLevel, ImageMaxValue, DisplayMaxValue);
                }

                // If its an arrow key, get the second code.
                Ch = Console.ReadKey(true);
            }
            Console.Write("\n\n");

            // Free all allocations.
            MIL.MbufFree(MilLut);
            MIL.MbufFree(MilImage);
            if (MilOriginalImage != MIL.M_NULL)
            {
                MIL.MbufFree(MilOriginalImage);
            }
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL);
        }

        // Function to draw the current LUT's shape in the image.
        //
        //   Note: This simple annotation method requires significant update
        //  and CPU time since it repaints the entire image every time.
        //
        static void DrawLutShape(MIL_ID MilDisplay, MIL_ID MilOriginalImage, MIL_ID MilImage, MIL_INT Start, MIL_INT End, MIL_INT InflexionIntensity, MIL_INT ImageMaxValue, MIL_INT DisplayMaxValue)
        {
            double Xstart = 0.0;
            double Xend = 0.0;
            double Xstep = 0.0;
            double Ymin = 0.0;
            double Yinf = 0.0;
            double Ymax = 0.0;
            double Ystep = 0.0;
            MIL_INT ImageSizeX = 0;
            MIL_INT ImageSizeY = 0;


            // Inquire image dimensions.
            MIL.MbufInquire(MilImage, MIL.M_SIZE_X, ref ImageSizeX);
            MIL.MbufInquire(MilImage, MIL.M_SIZE_Y, ref ImageSizeY);

            // Calculate the drawing parameters.
            Xstep = (double)ImageSizeX / (double)ImageMaxValue;
            Xstart = Start * Xstep;
            Xend = End * Xstep;
            Ystep = ((double)ImageSizeY / 4.0) / (double)DisplayMaxValue;
            Ymin = ((double)ImageSizeY - 2);
            Yinf = Ymin - (InflexionIntensity * Ystep);
            Ymax = Ymin - (DisplayMaxValue * Ystep);

            // To increase speed, disable display update until all annotations are done.
            MIL.MdispControl(MilDisplay, MIL.M_UPDATE, MIL.M_DISABLE);

            // Restore the original image.
            MIL.MbufCopy(MilOriginalImage, MilImage);

            // Draw axis max and min values.
            MIL.MgraColor(MIL.M_DEFAULT, (double)ImageMaxValue);
            MIL.MgraText(MIL.M_DEFAULT, MilImage, 4, (int)Ymin - 22, "0");
            MIL.MgraText(MIL.M_DEFAULT, MilImage, 4, (int)Ymax - 16, String.Format("{0}", DisplayMaxValue));
            MIL.MgraText(MIL.M_DEFAULT, MilImage, ImageSizeX - 38, (int)Ymin - 22, String.Format("{0}", ImageMaxValue));

            // Draw LUT Shape (X axis is display values and Y is image values).
            MIL.MgraLine(MIL.M_DEFAULT, MilImage, 0, (int)Ymin, (int)Xstart, (int)Ymin);
            MIL.MgraLine(MIL.M_DEFAULT, MilImage, (int)Xstart, (int)Ymin, (int)Xend, (int)Yinf);
            MIL.MgraLine(MIL.M_DEFAULT, MilImage, (int)Xend, (int)Yinf, ImageSizeX - 1, (int)Ymax);

            // Enable display update to show the result.
            MIL.MdispControl(MilDisplay, MIL.M_UPDATE, MIL.M_ENABLE);
        }
    }
}
