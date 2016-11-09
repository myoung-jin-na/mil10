/*****************************************************************************/
/* 
 * File name: MOcr.cpp
 *
 * Synopsis:  This program uses the OCR module to read a SEMI font string:
 *            the example calibrates a predefined OCR font (semi font) 
 *            and uses it to read a string in the image. The string read
 *            is then printed to the screen and the calibrated font is
 *            saved to disk.
 */
#include <mil.h>

/*****************************************************************************
 OCR SEMI font read example.
 *****************************************************************************/

/* Target image character specifications. */
#define CHAR_IMAGE_FILE        M_IMAGE_PATH MIL_TEXT("OcrSemi1292.mim")
#define CHAR_SIZE_X_MIN        22.0
#define CHAR_SIZE_X_MAX        23.0
#define CHAR_SIZE_X_STEP       0.50
#define CHAR_SIZE_Y_MIN        43.0
#define CHAR_SIZE_Y_MAX        44.0
#define CHAR_SIZE_Y_STEP       0.50

/* Target reading specifications. */
#define READ_REGION_POS_X      30L  
#define READ_REGION_POS_Y      40L
#define READ_REGION_WIDTH      420L
#define READ_REGION_HEIGHT     70L
#define READ_SCORE_MIN         50.0

/* Font file names. */
#define FONT_FILE_IN           M_CONTEXT_PATH MIL_TEXT("SEMI_M12-92.mfo")
#define FONT_FILE_OUT          M_TEMP_DIR MIL_TEXT("Semi1292Calibrated.mfo")

/* Length of the string to read (null terminated) */
#define STRING_LENGTH          13L
#define STRING_CALIBRATION     MIL_TEXT("M940902-MXD5")

int MosMain(void)
   {
   MIL_ID MilApplication,               /* Application identifier.       */
          MilSystem,                    /* System identifier.            */
          MilDisplay,                   /* Display identifier.           */
          MilImage,                     /* Image buffer identifier.      */
          MilSubImage,                  /* Sub-image buffer identifier.  */
          MilFontSubImage,              /* Font display sub image.       */
          MilOverlayImage,              /* Overlay image.                */
          OcrFont,                      /* OCR font identifier.          */
          OcrResult;                    /* OCR result buffer identifier. */
   MIL_TEXT_CHAR String[STRING_LENGTH]; /* Array of characters to read.  */
   MIL_DOUBLE Score;                    /* Reading score.                */
   MIL_INT  SizeX, SizeY, Type;         /* Source image dimensions.      */

   MosPrintf(MIL_TEXT("\nOCR MODULE (SEMI font reading):\n"));
   MosPrintf(MIL_TEXT("-------------------------------\n\n"));

   /* Allocate defaults. */
   MappAllocDefault(M_DEFAULT, &MilApplication, &MilSystem, &MilDisplay, M_NULL, M_NULL);

   /* Load and display the source image into a new image buffer. */
   MbufAlloc2d(MilSystem, 
               MbufDiskInquire(CHAR_IMAGE_FILE, M_SIZE_X, &SizeX),
               MbufDiskInquire(CHAR_IMAGE_FILE, M_SIZE_Y, &SizeY)*3/2,
               MbufDiskInquire(CHAR_IMAGE_FILE, M_TYPE,   &Type),
               M_IMAGE+M_PROC+M_DISP,
               &MilImage);
   MbufClear(MilImage, 0);
   MbufLoad(CHAR_IMAGE_FILE, MilImage);
   MdispSelect(MilDisplay, MilImage);

   /* Restrict the region of the image where to read the string. */
   MbufChild2d(MilImage, READ_REGION_POS_X, READ_REGION_POS_Y,
               READ_REGION_WIDTH, READ_REGION_HEIGHT, &MilSubImage);

   /* Define the bottom of the image as the region where to copy the font representation.*/
   MbufChild2d(MilImage, 50, SizeY+10, SizeX-100, (SizeY/3)-10, &MilFontSubImage);

   /* Restore the OCR character font from disk. */
   MocrRestoreFont(FONT_FILE_IN, M_RESTORE, MilSystem, &OcrFont);

   /* Show the font representation. */
   MocrCopyFont(MilFontSubImage, OcrFont, M_COPY_FROM_FONT+M_ALL_CHAR, M_NULL );

   /* Pause to show the original image. */
   MosPrintf(MIL_TEXT("The SEMI string at the top will be read using the ")
                              MIL_TEXT("font displayed at the bottom.\n\n"));
   MosPrintf(MIL_TEXT("Calibrating SEMI font...\n\n"));

   /* Calibrate the OCR font. */
   MocrCalibrateFont(MilSubImage, OcrFont, STRING_CALIBRATION,
                     CHAR_SIZE_X_MIN, CHAR_SIZE_X_MAX, CHAR_SIZE_X_STEP,
                     CHAR_SIZE_Y_MIN, CHAR_SIZE_Y_MAX, CHAR_SIZE_Y_STEP,
                     M_DEFAULT);

   /* Set the user-specific character constraints for each string position.  */
   MocrSetConstraint(OcrFont, 0,  M_LETTER, M_NULL);        /* A to Z only   */
   MocrSetConstraint(OcrFont, 1,  M_DIGIT,  MIL_TEXT("9")); /* 9      only   */
   MocrSetConstraint(OcrFont, 2,  M_DIGIT,  M_NULL);        /* 0 to 9 only   */ 
   MocrSetConstraint(OcrFont, 3,  M_DIGIT,  M_NULL);        /* 0 to 9 only   */ 
   MocrSetConstraint(OcrFont, 4,  M_DIGIT,  M_NULL);        /* 0 to 9 only   */ 
   MocrSetConstraint(OcrFont, 5,  M_DIGIT,  M_NULL);        /* 0 to 9 only   */ 
   MocrSetConstraint(OcrFont, 6,  M_DIGIT,  M_NULL);        /* 0 to 9 only   */ 
   MocrSetConstraint(OcrFont, 7,  M_DEFAULT,MIL_TEXT("-")); /* -      only   */
   MocrSetConstraint(OcrFont, 8,  M_LETTER, MIL_TEXT("M")); /* M      only   */
   MocrSetConstraint(OcrFont, 9,  M_LETTER, MIL_TEXT("X")); /* X      only   */
   MocrSetConstraint(OcrFont, 10, M_LETTER, MIL_TEXT("ABCDEFGH")); /* SEMI checksum */
   MocrSetConstraint(OcrFont, 11, M_DIGIT,  MIL_TEXT("01234567")); /* SEMI checksum */ 

   /* Pause before the read operation. */
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   /* Allocate an OCR result buffer. */
   MocrAllocResult(MilSystem, M_DEFAULT, &OcrResult);

   /* Read the string. */
   MocrReadString(MilSubImage, OcrFont, OcrResult);

   /* Get the string and its reading score. */
   MocrGetResult(OcrResult, M_STRING, String);
   MocrGetResult(OcrResult, M_STRING_SCORE,  &Score);

   /* Print the result. */
   MosPrintf(MIL_TEXT("\nThe string read is: \"%s\" (score: %.1f%%).\n\n"), String, Score);

   /* Draw the string in the overlay under the reading region. */
   MdispControl(MilDisplay, M_OVERLAY, M_ENABLE);
   MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);
   MdispInquire(MilDisplay, M_OVERLAY_ID, &MilOverlayImage);
   MgraFont(M_DEFAULT, M_FONT_DEFAULT_LARGE);
   MgraColor(M_DEFAULT, M_COLOR_YELLOW);
   MgraText(M_DEFAULT, MilOverlayImage, READ_REGION_POS_X+(READ_REGION_WIDTH/4),
                                        READ_REGION_POS_Y+READ_REGION_HEIGHT+50, String);

   /* Save the calibrated font if the reading score was sufficiently high. */
   if (Score > READ_SCORE_MIN)
      {
      MocrSaveFont(FONT_FILE_OUT, M_SAVE, OcrFont);
      MosPrintf(MIL_TEXT("Read successful, calibrated OCR font was saved to disk.\n"));
      }
   else 
      {
      MosPrintf(MIL_TEXT("Error: Read score too low, calibrated OCR font not saved.\n"));
      }
   MosPrintf(MIL_TEXT("Press <Enter> to end.\n\n\n"));
   MosGetch();

   /* Clear the overlay. */
   MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);

   /* Free all allocations. */
   MocrFree(OcrFont);
   MocrFree(OcrResult);
   MbufFree(MilSubImage);
   MbufFree(MilFontSubImage);
   MbufFree(MilImage);
   MappFreeDefault(MilApplication, MilSystem, MilDisplay, M_NULL, M_NULL);

   return 0;
}

