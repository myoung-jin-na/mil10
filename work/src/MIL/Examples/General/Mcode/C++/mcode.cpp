/**************************************************************************************/
/* 
 * File name: MCode.cpp
 *
 * Synopsis:  This program decodes a 1D Code 39 linear Barcode and a 2D DataMatrix code.
 */
#include <mil.h>

/* Target image character specifications. */
#define IMAGE_FILE                    M_IMAGE_PATH MIL_TEXT("Code.mim")

/* Regions around 1D code. */
#define BARCODE_REGION_TOP_LEFT_X     256L
#define BARCODE_REGION_TOP_LEFT_Y      80L
#define BARCODE_REGION_SIZE_X         290L
#define BARCODE_REGION_SIZE_Y          60L

/* Regions around 2D code. */
#define DATAMATRIX_REGION_TOP_LEFT_X    8L
#define DATAMATRIX_REGION_TOP_LEFT_Y  312L
#define DATAMATRIX_REGION_SIZE_X      118L
#define DATAMATRIX_REGION_SIZE_Y      105L

/* Maximum length of the string to read. */
#define STRING_LENGTH_MAX              64L


int MosMain(void)
{
   MIL_ID      MilApplication,        /* Application identifier.         */
               MilSystem,             /* System identifier.              */
               MilDisplay,            /* Display identifier.             */
               MilImage,              /* Image buffer identifier.        */
               MilOverlayImage,       /* Image buffer identifier.        */
               DataMatrixRegion,      /* Child containing DataMatrix.    */
               DataMatrixCode,        /* DataMatrix 2D code identifier.  */
               BarCodeRegion,         /* Child containing Code39         */
               Barcode,               /* Code39 barcode identifier.      */
               CodeResults;           /* Barcode results identifier.     */
   MIL_INT     BarcodeStatus;         /* Decoding status.                */
   MIL_INT     DataMatrixStatus;      /* Decoding status.                */
   MIL_DOUBLE  AnnotationColor = M_COLOR_GREEN, AnnotationBackColor = M_COLOR_GRAY;
   MIL_INT     n; 
   MIL_TEXT_CHAR OutputString    [STRING_LENGTH_MAX]; /* Array of characters to draw. */
   MIL_TEXT_CHAR BarcodeString   [STRING_LENGTH_MAX]; /* Array of characters read.    */
   MIL_TEXT_CHAR DataMatrixString[STRING_LENGTH_MAX]; /* Array of characters read.    */


   /* Allocate defaults. */
   MappAllocDefault(M_DEFAULT, &MilApplication, &MilSystem, &MilDisplay, M_NULL, M_NULL);

   /* Restore source image into an automatically allocated image buffer. */
   MbufRestore(IMAGE_FILE, MilSystem, &MilImage);

   /* Display the image buffer. */
   MdispSelect(MilDisplay, MilImage);
       
   /* Prepare for overlay annotations. */
   MdispControl(MilDisplay, M_OVERLAY, M_ENABLE);
   MdispInquire(MilDisplay, M_OVERLAY_ID, &MilOverlayImage);

   /* Prepare bar code results buffer */
   McodeAllocResult(MilSystem, M_DEFAULT, &CodeResults);

    /* Pause to show the original image. */
   MosPrintf(MIL_TEXT("\n1D and 2D CODE READING:\n"));
   MosPrintf(MIL_TEXT("-----------------------\n\n"));
   MosPrintf(MIL_TEXT("This program will decode a linear Code 39 and a DataMatrix code.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();


   /* 1D BARCODE READING: */
   
   /* Create a read region around the code to speedup reading. */
   MbufChild2d(MilImage, BARCODE_REGION_TOP_LEFT_X, BARCODE_REGION_TOP_LEFT_Y,
               BARCODE_REGION_SIZE_X, BARCODE_REGION_SIZE_Y, &BarCodeRegion);
   
   /* Allocate CODE objects. */
   McodeAlloc(MilSystem, M_DEFAULT, M_DEFAULT, &Barcode);
   McodeModel(Barcode, M_ADD, M_CODE39, M_NULL, M_DEFAULT, M_NULL);

   /* Read codes from image. */
   McodeRead(Barcode, BarCodeRegion, CodeResults);
    
   /* Get decoding status. */
   McodeGetResult(CodeResults, M_STATUS+M_TYPE_MIL_INT, &BarcodeStatus);

   /* Check if decoding was successful. */
   if (BarcodeStatus == M_STATUS_READ_OK)
      {
      /* Get decoded string. */
      McodeGetResult(CodeResults, M_STRING, BarcodeString);

      /* Draw the decoded strings and read region in the overlay image. */
      MgraColor(M_DEFAULT, AnnotationColor); 
      MgraBackColor(M_DEFAULT, AnnotationBackColor); 
      MosSprintf(OutputString, STRING_LENGTH_MAX, MIL_TEXT("\"%s\""), BarcodeString); 
      MgraText(M_DEFAULT, MilOverlayImage, BARCODE_REGION_TOP_LEFT_X+10,
               BARCODE_REGION_TOP_LEFT_Y+80, MIL_TEXT(" 1D linear 39 bar code:           "));
      MgraText(M_DEFAULT, MilOverlayImage, BARCODE_REGION_TOP_LEFT_X+200, 
               BARCODE_REGION_TOP_LEFT_Y+80, OutputString);
      MgraRect(M_DEFAULT, MilOverlayImage,
               BARCODE_REGION_TOP_LEFT_X, BARCODE_REGION_TOP_LEFT_Y,
               BARCODE_REGION_TOP_LEFT_X+BARCODE_REGION_SIZE_X, 
               BARCODE_REGION_TOP_LEFT_Y+BARCODE_REGION_SIZE_Y);
      }

   /* Free objects. */
   McodeFree(Barcode);
   MbufFree(BarCodeRegion);

   /* 2D CODE READING: */
  
   /* Create a read region around the code to speedup reading. */
   MbufChild2d(MilImage, DATAMATRIX_REGION_TOP_LEFT_X, DATAMATRIX_REGION_TOP_LEFT_Y,
               DATAMATRIX_REGION_SIZE_X, DATAMATRIX_REGION_SIZE_Y, &DataMatrixRegion);
  
   /* Allocate CODE objects. */
   McodeAlloc(MilSystem, M_DEFAULT, M_DEFAULT, &DataMatrixCode);
   McodeModel(DataMatrixCode, M_ADD, M_DATAMATRIX, M_NULL, M_DEFAULT, M_NULL);


   /* Set the foreground value for the DataMatrix since it is different 
      from the default value. */
   McodeControl(DataMatrixCode, M_FOREGROUND_VALUE, M_FOREGROUND_WHITE); 
    
   /* Read codes from image. */
   McodeRead(DataMatrixCode, DataMatrixRegion, CodeResults);
    
   /* Get decoding status. */
   McodeGetResult(CodeResults, M_STATUS + M_TYPE_MIL_INT, &DataMatrixStatus);

   /* Check if decoding was successful. */
   if (DataMatrixStatus == M_STATUS_READ_OK)
      {
      /* Get decoded string. */
      McodeGetResult(CodeResults, M_STRING, DataMatrixString);

      /* Draw the decoded strings and read region in the overlay image. */
      MgraColor(M_DEFAULT, AnnotationColor); 
      MgraBackColor(M_DEFAULT, AnnotationBackColor); 
      /* Replace non printable characters with space.*/
      for (n=0; DataMatrixString[n] != MIL_TEXT('\0'); n++) 
          if ((DataMatrixString[n] < MIL_TEXT('0')) || (DataMatrixString[n] > MIL_TEXT('Z')))
              DataMatrixString[n] = MIL_TEXT(' ');
      MosSprintf(OutputString, STRING_LENGTH_MAX, MIL_TEXT("\"%s\" "), DataMatrixString); 
      MgraText(M_DEFAULT, 
               MilOverlayImage, 
               DATAMATRIX_REGION_TOP_LEFT_X,  
               DATAMATRIX_REGION_TOP_LEFT_Y+120, 
               MIL_TEXT(" 2D Data Matrix code:                  "));
      MgraText(M_DEFAULT, MilOverlayImage, DATAMATRIX_REGION_TOP_LEFT_X+200, 
               DATAMATRIX_REGION_TOP_LEFT_Y+120, OutputString);
      MgraRect(M_DEFAULT, MilOverlayImage,
               DATAMATRIX_REGION_TOP_LEFT_X, DATAMATRIX_REGION_TOP_LEFT_Y,
               DATAMATRIX_REGION_TOP_LEFT_X+DATAMATRIX_REGION_SIZE_X, 
               DATAMATRIX_REGION_TOP_LEFT_Y+DATAMATRIX_REGION_SIZE_Y);
      }

   /* Free objects. */
   McodeFree(DataMatrixCode);
   MbufFree(DataMatrixRegion);

   /* Free results buffer. */
   McodeFree(CodeResults);

   /* Pause to show the results. */
   if ( (DataMatrixStatus == M_STATUS_READ_OK) && (BarcodeStatus == M_STATUS_READ_OK) )
      MosPrintf(MIL_TEXT("Decoding was successful and the strings were ")
                                   MIL_TEXT("written under each code.\n"));
   else
      MosPrintf(MIL_TEXT("Decoding error found.\n"));    
   MosPrintf(MIL_TEXT("Press <Enter> to end.\n\n"));
   MosGetch();

   /* Free other allocations. */
   MbufFree(MilImage);
   MappFreeDefault(MilApplication, MilSystem, MilDisplay, M_NULL, M_NULL);

   return 0;
}
