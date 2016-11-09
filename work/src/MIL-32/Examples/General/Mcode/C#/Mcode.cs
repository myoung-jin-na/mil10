//**************************************************************************************
// 
// File name: Mcode.cs
//
// Synopsis:  This program decodes a 1D Code 39 linear Barcode and a 2D DataMatrix code.
//
//**************************************************************************************
using System;
using System.Collections.Generic;
using System.Text;
using Matrox.MatroxImagingLibrary;

namespace MCode
{
    class Program
    {
        static void Main(string[] args)
        {
            const string IMAGE_FILE = MIL.M_IMAGE_PATH + "Code.mim";
            const int STRING_LENGTH_MAX = 64;

            // Regions around 1D code.
            const int BARCODE_REGION_TOP_LEFT_X = 256;
            const int BARCODE_REGION_TOP_LEFT_Y = 80;
            const int BARCODE_REGION_SIZE_X = 290;
            const int BARCODE_REGION_SIZE_Y = 60;

            // Regions around 2D code.
            const int DATAMATRIX_REGION_TOP_LEFT_X = 8;
            const int DATAMATRIX_REGION_TOP_LEFT_Y = 312;
            const int DATAMATRIX_REGION_SIZE_X = 118;
            const int DATAMATRIX_REGION_SIZE_Y = 105;

            MIL_ID MilApplication = MIL.M_NULL;                     // Application identifier.
            MIL_ID MilSystem = MIL.M_NULL;                          // System identifier.
            MIL_ID MilDisplay = MIL.M_NULL;                         // Display identifier.
            MIL_ID MilImage = MIL.M_NULL;                           // Image buffer identifier.
            MIL_ID MilOverlayImage = MIL.M_NULL;                    // Image buffer identifier.
            MIL_ID DataMatrixRegion = MIL.M_NULL;                   // Child containing DataMatrix.
            MIL_ID DataMatrixCode = MIL.M_NULL;                     // DataMatrix 2D code identifier.
            MIL_ID BarCodeRegion = MIL.M_NULL;                      // Child containing Code39.
            MIL_ID Barcode = MIL.M_NULL;                            // Code39 barcode identifier.
            MIL_ID CodeResults = MIL.M_NULL;                        // Barcode results identifier.
            MIL_INT BarcodeStatus = MIL.M_NULL;                         // Decoding status.
            MIL_INT DataMatrixStatus = MIL.M_NULL;                      // Decoding status.

            double AnnotationColor = MIL.M_COLOR_GREEN;
            double AnnotationBackColor = MIL.M_COLOR_GRAY;
            int n = 0;
            StringBuilder BarcodeString = new StringBuilder(STRING_LENGTH_MAX); // Array of characters read.
            StringBuilder DataMatrixString = new StringBuilder(STRING_LENGTH_MAX); // Array of characters read.


            // Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, ref MilApplication, ref MilSystem, ref MilDisplay, MIL.M_NULL, MIL.M_NULL);

            // Restore source image into an automatically allocated image buffer.
            MIL.MbufRestore(IMAGE_FILE, MilSystem, ref MilImage);

            // Display the image buffer.
            MIL.MdispSelect(MilDisplay, MilImage);

            // Prepare for overlay annotations.
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY, MIL.M_ENABLE);
            MIL.MdispInquire(MilDisplay, MIL.M_OVERLAY_ID, ref MilOverlayImage);

            // Prepare bar code results buffer
            MIL.McodeAllocResult(MilSystem, MIL.M_DEFAULT, ref CodeResults);

            // Pause to show the original image.
            Console.Write("\n1D and 2D CODE READING:\n");
            Console.Write("-----------------------\n\n");
            Console.Write("This program will decode a linear Code 39 and a DataMatrix code.\n");
            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();


            // 1D BARCODE READING:

            // Create a read region around the code to speedup reading.
            MIL.MbufChild2d(MilImage, BARCODE_REGION_TOP_LEFT_X, BARCODE_REGION_TOP_LEFT_Y, BARCODE_REGION_SIZE_X, BARCODE_REGION_SIZE_Y, ref BarCodeRegion);

            // Allocate CODE objects.
            MIL.McodeAlloc(MilSystem, MIL.M_DEFAULT, MIL.M_DEFAULT, ref Barcode);
            MIL.McodeModel(Barcode, MIL.M_ADD, MIL.M_CODE39, MIL.M_NULL, MIL.M_DEFAULT, MIL.M_NULL);

            // Read codes from image.
            MIL.McodeRead(Barcode, BarCodeRegion, CodeResults);

            // Get decoding status.
            MIL.McodeGetResult(CodeResults, MIL.M_STATUS + MIL.M_TYPE_MIL_INT, ref BarcodeStatus);

            // Check if decoding was successful.
            if (BarcodeStatus == MIL.M_STATUS_READ_OK)
            {
                // Get decoded string.
                MIL.McodeGetResult(CodeResults, MIL.M_STRING, BarcodeString);

                // Draw the decoded strings and read region in the overlay image.
                MIL.MgraColor(MIL.M_DEFAULT, AnnotationColor);
                MIL.MgraBackColor(MIL.M_DEFAULT, AnnotationBackColor);
                MIL.MgraText(MIL.M_DEFAULT, MilOverlayImage, BARCODE_REGION_TOP_LEFT_X + 10, BARCODE_REGION_TOP_LEFT_Y + 80, " 1D linear 39 bar code:           ");
                MIL.MgraText(MIL.M_DEFAULT, MilOverlayImage, BARCODE_REGION_TOP_LEFT_X + 200, BARCODE_REGION_TOP_LEFT_Y + 80, BarcodeString.ToString());
                MIL.MgraRect(MIL.M_DEFAULT, MilOverlayImage, BARCODE_REGION_TOP_LEFT_X, BARCODE_REGION_TOP_LEFT_Y, BARCODE_REGION_TOP_LEFT_X + BARCODE_REGION_SIZE_X, BARCODE_REGION_TOP_LEFT_Y + BARCODE_REGION_SIZE_Y);
            }

            // Free objects.
            MIL.McodeFree(Barcode);
            MIL.MbufFree(BarCodeRegion);

            // 2D CODE READING:

            // Create a read region around the code to speedup reading.
            MIL.MbufChild2d(MilImage, DATAMATRIX_REGION_TOP_LEFT_X, DATAMATRIX_REGION_TOP_LEFT_Y, DATAMATRIX_REGION_SIZE_X, DATAMATRIX_REGION_SIZE_Y, ref DataMatrixRegion);

            // Allocate CODE objects.
            MIL.McodeAlloc(MilSystem, MIL.M_DEFAULT, MIL.M_DEFAULT, ref DataMatrixCode);
            MIL.McodeModel(DataMatrixCode, MIL.M_ADD, MIL.M_DATAMATRIX, MIL.M_NULL, MIL.M_DEFAULT, MIL.M_NULL);


            // Set the foreground value for the DataMatrix since it is different from the default value.
            MIL.McodeControl(DataMatrixCode, MIL.M_FOREGROUND_VALUE, MIL.M_FOREGROUND_WHITE);

            // Read codes from image.
            MIL.McodeRead(DataMatrixCode, DataMatrixRegion, CodeResults);

            // Get decoding status.
            MIL.McodeGetResult(CodeResults, MIL.M_STATUS + MIL.M_TYPE_MIL_INT, ref DataMatrixStatus);

            // Check if decoding was successful.
            if (DataMatrixStatus == MIL.M_STATUS_READ_OK)
            {
                // Get decoded string.
                MIL.McodeGetResult(CodeResults, MIL.M_STRING, DataMatrixString);

                // Draw the decoded strings and read region in the overlay image.
                MIL.MgraColor(MIL.M_DEFAULT, AnnotationColor);
                MIL.MgraBackColor(MIL.M_DEFAULT, AnnotationBackColor);
                for (n = 0; n < DataMatrixString.Length; n++) // Replace non printable characters with space.*/
                {
                    if ((DataMatrixString[n] < '0') || (DataMatrixString[n] > 'Z'))
                    {
                        DataMatrixString[n] = ' ';
                    }
                }
                MIL.MgraText(MIL.M_DEFAULT, MilOverlayImage, DATAMATRIX_REGION_TOP_LEFT_X, DATAMATRIX_REGION_TOP_LEFT_Y + 120, " 2D Data Matrix code:                  ");
                MIL.MgraText(MIL.M_DEFAULT, MilOverlayImage, DATAMATRIX_REGION_TOP_LEFT_X + 200, DATAMATRIX_REGION_TOP_LEFT_Y + 120, DataMatrixString.ToString());
                MIL.MgraRect(MIL.M_DEFAULT, MilOverlayImage, DATAMATRIX_REGION_TOP_LEFT_X, DATAMATRIX_REGION_TOP_LEFT_Y, DATAMATRIX_REGION_TOP_LEFT_X + DATAMATRIX_REGION_SIZE_X, DATAMATRIX_REGION_TOP_LEFT_Y + DATAMATRIX_REGION_SIZE_Y);
            }

            // Free objects.
            MIL.McodeFree(DataMatrixCode);
            MIL.MbufFree(DataMatrixRegion);

            // Free results buffer.
            MIL.McodeFree(CodeResults);

            // Pause to show the results.
            if ((DataMatrixStatus == MIL.M_STATUS_READ_OK) && (BarcodeStatus == MIL.M_STATUS_READ_OK))
            {
                Console.Write("Decoding was successful and the strings were written under each code.\n");
            }
            else
            {
                Console.Write("Decoding error found.\n");
            }
            Console.Write("Press <Enter> to end.\n");
            Console.ReadKey();

            // Free other allocations.
            MIL.MbufFree(MilImage);
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL);
        }
    }
}
