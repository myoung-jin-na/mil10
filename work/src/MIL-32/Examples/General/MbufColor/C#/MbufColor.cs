//**********************************************************************************
// 
// File name: MbufColor.cs
//
// Synopsis:  This program demonstrates color buffer manipulations. It allocates 
//            a displayable color image buffer, displays it, and loads a color   
//            image into the left part. After that, color annotations are done 
//            in each band of the color buffer. Finaly, to increase the image
//            luminance, the image is converted to Hue, Luminance and Saturation 
//            (HLS), a certain offset is added to the luminance component and 
//            the image is converted back to Red, Green, Blue (RGB) into the 
//            right part to display the result. 
//          
//            The example also demonstrates how to display multiple images 
//            in a single display using child buffers.
//**********************************************************************************
using System;
using System.Collections.Generic;
using System.Text;

using Matrox.MatroxImagingLibrary;

namespace MBufColor
{
    class Program
    {
        // Source MIL image file specifications.
        private const string IMAGE_FILE = MIL.M_IMAGE_PATH + "Bird.mim";

        // Luminance offset to add to the image.
        private const int IMAGE_LUMINANCE_OFFSET = 40;

        // Main function.
        static void Main(string[] args)
        {
            MIL_ID MilApplication = MIL.M_NULL;             // Application identifier.
            MIL_ID MilSystem = MIL.M_NULL;                  // System identifier.
            MIL_ID MilDisplay = MIL.M_NULL;                 // Display identifier.
            MIL_ID MilImage = MIL.M_NULL;                   // Image buffer identifier.
            MIL_ID MilLeftSubImage = MIL.M_NULL;            // Sub-image buffer identifier for original image.
            MIL_ID MilRightSubImage = MIL.M_NULL;           // Sub-image buffer identifier for processed image.
            MIL_ID MilLumSubImage = MIL.M_NULL;             // Sub-image buffer identifier for luminance.
            MIL_ID MilRedBandSubImage = MIL.M_NULL;         // Sub-image buffer identifier for red component.
            MIL_ID MilGreenBandSubImage = MIL.M_NULL;       // Sub-image buffer identifier for green component.
            MIL_ID MilBlueBandSubImage = MIL.M_NULL;        // Sub-image buffer identifier for blue component.
            MIL_INT SizeX = 0;
            MIL_INT SizeY = 0;
            MIL_INT SizeBand = 0;
            MIL_INT Type = 0;

            // Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, ref MilApplication, ref MilSystem, ref MilDisplay, MIL.M_NULL, MIL.M_NULL);

            // Allocate a color display buffer twice the size of the source image and display it.
            MIL.MbufAllocColor(MilSystem,
                           MIL.MbufDiskInquire(IMAGE_FILE, MIL.M_SIZE_BAND, ref SizeBand),
                           MIL.MbufDiskInquire(IMAGE_FILE, MIL.M_SIZE_X, ref SizeX) * 2,
                           MIL.MbufDiskInquire(IMAGE_FILE, MIL.M_SIZE_Y, ref SizeY),
                           MIL.MbufDiskInquire(IMAGE_FILE, MIL.M_TYPE, ref Type),
                           MIL.M_IMAGE + MIL.M_DISP + MIL.M_PROC, ref MilImage);
            MIL.MbufClear(MilImage, 0);
            MIL.MdispSelect(MilDisplay, MilImage);

            // Define 2 child buffers that maps to the left and right part of the display 
            // buffer, to put the source and destination color images.
            //
            MIL.MbufChild2d(MilImage, 0, 0, SizeX, SizeY, ref MilLeftSubImage);
            MIL.MbufChild2d(MilImage, SizeX, 0, SizeX, SizeY, ref MilRightSubImage);

            // Load the color source image on the left.
            MIL.MbufLoad(IMAGE_FILE, MilLeftSubImage);

            // Define child buffers that map to the red, green and blue components
            // of the source image.
            //
            MIL.MbufChildColor(MilLeftSubImage, MIL.M_RED, ref MilRedBandSubImage);
            MIL.MbufChildColor(MilLeftSubImage, MIL.M_GREEN, ref MilGreenBandSubImage);
            MIL.MbufChildColor(MilLeftSubImage, MIL.M_BLUE, ref MilBlueBandSubImage);

            // Write color text annotations to show access in each individual band of the image.
            //
            // Note that this is typically done more simply by using:
            //  MIL.MgraColor(MIL.M_DEFAULT, MIL.M_RGB(0xFF,0x90,0x00));
            //  MIL.MgraText(MIL.M_DEFAULT, MilLeftSubImage, ...);

            MIL.MgraColor(MIL.M_DEFAULT, 0xFF);
            MIL.MgraText(MIL.M_DEFAULT, MilRedBandSubImage, SizeX / 16, SizeY / 8, " TOUCAN ");
            MIL.MgraColor(MIL.M_DEFAULT, 0x90);
            MIL.MgraText(MIL.M_DEFAULT, MilGreenBandSubImage, SizeX / 16, SizeY / 8, " TOUCAN ");
            MIL.MgraColor(MIL.M_DEFAULT, 0x00);
            MIL.MgraText(MIL.M_DEFAULT, MilBlueBandSubImage, SizeX / 16, SizeY / 8, " TOUCAN ");

            // Print a message.
            Console.Write("\nCOLOR OPERATIONS:\n");
            Console.Write("-----------------\n\n");
            Console.Write("A color source image was loaded on the left and color text\n");
            Console.Write("annotations were written in it.\n");
            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();

            // Convert image to Hue, Luminance, Saturation color space (HSL).
            MIL.MimConvert(MilLeftSubImage, MilRightSubImage, MIL.M_RGB_TO_HSL);

            // Create a child buffer that maps to the luminance component.
            MIL.MbufChildColor(MilRightSubImage, MIL.M_LUMINANCE, ref MilLumSubImage);

            // Add an offset to the luminance component.
            MIL.MimArith(MilLumSubImage, IMAGE_LUMINANCE_OFFSET, MilLumSubImage, MIL.M_ADD_CONST + MIL.M_SATURATION);

            // Convert image back to Red, Green, Blue color space (RGB) for display.
            MIL.MimConvert(MilRightSubImage, MilRightSubImage, MIL.M_HSL_TO_RGB);

            // Print a message.
            Console.Write("Luminance was increased using color image processing.\n");

            // Print a message.
            Console.Write("Press <Enter> to end.\n");
            Console.ReadKey();

            // Release sub-images and color image buffer.
            MIL.MbufFree(MilLumSubImage);
            MIL.MbufFree(MilRedBandSubImage);
            MIL.MbufFree(MilGreenBandSubImage);
            MIL.MbufFree(MilBlueBandSubImage);
            MIL.MbufFree(MilRightSubImage);
            MIL.MbufFree(MilLeftSubImage);
            MIL.MbufFree(MilImage);

            // Release defaults.
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL);
        }
    }
}
