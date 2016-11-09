//******************************************************************************
//
// File name: Mreg.cs
//
// Synopsis:  This program uses the registration module to form a mosaic of 
//            three images taken from a camera at unknown translation.
//******************************************************************************
using System;
using System.Collections.Generic;
using System.Text;

using Matrox.MatroxImagingLibrary;

namespace MReg
{
    class Program
    {
        // Number of images to register.
        private const int NUM_IMAGES_TO_REGISTER = 3;

        // MIL image file specifications.
        private const string IMAGE_FILES_SOURCE = MIL.M_IMAGE_PATH + "CircuitBoardPart{0}.mim";

        static void Main(string[] args)
        {
            MIL_ID MilApplication = MIL.M_NULL;                             // Application identifier.
            MIL_ID MilSystem = MIL.M_NULL;                                  // System identifier.
            MIL_ID MilDisplay = MIL.M_NULL;                                 // Display identifier.
            MIL_ID MilGraphicList = MIL.M_NULL;                             // graphic list identifier.
            MIL_ID[] MilSourceImages = new MIL_ID[NUM_IMAGES_TO_REGISTER];  // Source images buffer identifiers.
            MIL_ID MilMosaicImage = MIL.M_NULL;                             // Mosaic image buffer identifier.
            MIL_ID MilRegContext = MIL.M_NULL;                              // Registration context identifier.
            MIL_ID MilRegResult = MIL.M_NULL;                               // Registration result identifier.
            MIL_INT Result = 0;                                             // Result of the registration.
            MIL_INT MosaicSizeX = 0;                                        // Size of the mosaic.
            MIL_INT MosaicSizeY = 0;
            MIL_INT MosaicSizeBand = 0;                                     // Characteristics of mosaic image.
            MIL_INT MosaicType = 0;

            string[] ImageFilesSource = new string[NUM_IMAGES_TO_REGISTER];

            // Allocate defaults
            MIL.MappAllocDefault(MIL.M_DEFAULT, ref MilApplication, ref MilSystem, ref MilDisplay, MIL.M_NULL, MIL.M_NULL);

            // Load source image names.
            for (int i = 0; i < NUM_IMAGES_TO_REGISTER; i++)
            {
                ImageFilesSource[i] = String.Format(IMAGE_FILES_SOURCE, i);
            }

            // Print module name.
            Console.Write("\nREGISTRATION MODULE:\n");
            Console.Write("---------------------\n\n");

            // Print comment.
            Console.Write("This program will make a mosaic from many source images.\n");
            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();

            // Restore the source images.
            for (int i = 0; i < NUM_IMAGES_TO_REGISTER; i++)
            {
                MIL.MbufRestore(ImageFilesSource[i], MilSystem, ref MilSourceImages[i]);
            }

            // Display the source images.
            for (int i = 0; i < NUM_IMAGES_TO_REGISTER; i++)
            {
                MIL.MdispSelect(MilDisplay, MilSourceImages[i]);

                // Pause to show each image.
                Console.Write("image {0}.\n", i);
                Console.Write("Press <Enter> to continue.\n\n");
                Console.ReadKey();
            }

            // Allocate a graphic list to hold the subpixel annotations to draw.
            MIL.MgraAllocList(MilSystem, MIL.M_DEFAULT, ref MilGraphicList);
            MIL.MdispControl(MilDisplay, MIL.M_ASSOCIATED_GRAPHIC_LIST_ID, MilGraphicList);

            // Allocate a new empty registration context.
            MIL.MregAlloc(MilSystem, MIL.M_CORRELATION, MIL.M_DEFAULT, ref MilRegContext);

            // Allocate a new empty registration result buffer.
            MIL.MregAllocResult(MilSystem, MIL.M_DEFAULT, ref MilRegResult);

            // Set the transformation type to translation.
            MIL.MregControl(MilRegContext, MIL.M_CONTEXT, MIL.M_TRANSFORMATION_TYPE, MIL.M_TRANSLATION);

            // By default, each image will be registered with the previous in the list
            // No need to set other location parameters.

            // Set range to 100% in order to search all possible translations.
            MIL.MregControl(MilRegContext, MIL.M_CONTEXT, MIL.M_LOCATION_DELTA, 100);

            // Calculate the registration on the images.
            MIL.MregCalculate(MilRegContext, MilSourceImages, MilRegResult, NUM_IMAGES_TO_REGISTER, MIL.M_DEFAULT);

            // Verify if registration is successful.
            MIL.MregGetResult(MilRegResult, MIL.M_GENERAL, MIL.M_RESULT + MIL.M_TYPE_MIL_INT, ref Result);
            if (Result == MIL.M_SUCCESS)
            {
                // Get the size of the required mosaic buffer.
                MIL.MregGetResult(MilRegResult, MIL.M_GENERAL, MIL.M_MOSAIC_SIZE_X + MIL.M_TYPE_MIL_INT, ref MosaicSizeX);
                MIL.MregGetResult(MilRegResult, MIL.M_GENERAL, MIL.M_MOSAIC_SIZE_Y + MIL.M_TYPE_MIL_INT, ref MosaicSizeY);

                // The mosaic type will be the same as the source images.
                MIL.MbufInquire(MilSourceImages[0], MIL.M_SIZE_BAND, ref MosaicSizeBand);
                MIL.MbufInquire(MilSourceImages[0], MIL.M_TYPE, ref MosaicType);

                // Allocate mosaic image.
                MIL.MbufAllocColor(MilSystem, MosaicSizeBand, MosaicSizeX, MosaicSizeY, MosaicType, MIL.M_IMAGE + MIL.M_PROC + MIL.M_DISP, ref MilMosaicImage);

                // Compose the mosaic from the source images.
                MIL.MregTransformImage(MilRegResult, MilSourceImages, MilMosaicImage, NUM_IMAGES_TO_REGISTER, MIL.M_BILINEAR + MIL.M_OVERSCAN_CLEAR, MIL.M_DEFAULT);

                // Display the mosaic image and prepare for overlay annotations.
                MIL.MdispSelect(MilDisplay, MilMosaicImage);
                MIL.MgraColor(MIL.M_DEFAULT, MIL.M_RGB888(0, 0xFF, 0));

                // Pause to show the mosaic.
                Console.Write("mosaic image.\n");
                Console.Write("Press <Enter> to continue.\n\n");
                Console.ReadKey();

                // Draw the box of all source images in the mosaic.
                MIL.MregDraw(MIL.M_DEFAULT, MilRegResult, MilGraphicList, MIL.M_DRAW_BOX, MIL.M_ALL, MIL.M_DEFAULT);

                // Draw a cross at the center of each image in the mosaic.
                for (int i = 0; i < NUM_IMAGES_TO_REGISTER; i++)
                {
                    // Coordinates of the center of the source image.
                    double SourcePosX = 0.5 * (double)MIL.MbufInquire(MilSourceImages[i], MIL.M_SIZE_X, MIL.M_NULL);
                    double SourcePosY = 0.5 * (double)MIL.MbufInquire(MilSourceImages[i], MIL.M_SIZE_Y, MIL.M_NULL);

                    // Transform the coordinates to the mosaic.
                    double MosaicPosX = 0;
                    double MosaicPosY = 0;
                    MIL.MregTransformCoordinate(MilRegResult, i, MIL.M_MOSAIC, SourcePosX, SourcePosY, ref MosaicPosX, ref MosaicPosY, MIL.M_DEFAULT);
                    int MosaicPosXMilInt = (int)(MosaicPosX + 0.5);
                    int MosaicPosYMilInt = (int)(MosaicPosY + 0.5);

                    // Draw the cross in the mosaic.
                    MIL.MgraLine(MIL.M_DEFAULT, MilGraphicList, MosaicPosXMilInt - 4, MosaicPosYMilInt, MosaicPosXMilInt + 4, MosaicPosYMilInt);
                    MIL.MgraLine(MIL.M_DEFAULT, MilGraphicList, MosaicPosXMilInt, MosaicPosYMilInt - 4, MosaicPosXMilInt, MosaicPosYMilInt + 4);
                }

                Console.Write("The bounding boxes and the center of all source images\n");
                Console.Write("have been drawn in the mosaic.\n");
            }
            else
            {
                Console.Write("Error: Registration was not successful.\n");
            }

            // Pause to show results.
            Console.Write("\nPress <Enter> to end.\n\n");
            Console.ReadKey();

            // Free all allocations.
            MIL.MgraFree(MilGraphicList);
            if (MilMosaicImage != MIL.M_NULL)
            {
                MIL.MbufFree(MilMosaicImage);
            }

            MIL.MregFree(MilRegContext);
            MIL.MregFree(MilRegResult);

            for (int i = 0; i < NUM_IMAGES_TO_REGISTER; i++)
            {
                MIL.MbufFree(MilSourceImages[i]);
            }

            // Free defaults.
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL);
        }
    }
}
