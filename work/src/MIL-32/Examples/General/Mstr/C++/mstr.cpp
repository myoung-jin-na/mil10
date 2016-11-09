/*******************************************************************************/
/*
* File name: MStr.cpp
*
* Synopsis:  This program uses the String Reader module to define a font 
*            from an image containing a mosaic of Quebec license plates.
*            Two String Models are then defined and parameterized to read 
*            only some valid Quebec license plates.
*            A license plate reading is then performed in a target image 
*            of a car on the road.
*/ 
#include <mil.h>  

/* MIL image file specifications. */  
#define IMAGE_FILE_DEFINITION M_IMAGE_PATH MIL_TEXT("QcPlates.mim")
#define IMAGE_FILE_TO_READ    M_IMAGE_PATH MIL_TEXT("LicPlate.mim")

/* String containing all characters used for font definition. */
#define TEXT_DEFINITION       "AVS300CVK781JNK278 EBX380QKN918HCC839 "\
	"YRH900ZQR756977AMQ GPK742389EYE569ESQ"

/* Font normalization size Y. */
#define NORMALIZATION_SIZE_Y    20L

/* Max size of plate string. */
#define STRING_MAX_SIZE         32L


/*****************************************************************************/
/* Main. */
int MosMain(void)
{ 
	MIL_ID MilApplication,                          /* Application identifier.          */
		   MilSystem,                               /* System identifier.               */
		   MilDisplay,                              /* Display identifier.              */
		   MilImage,                                /* Image buffer identifier.         */
		   MilOverlayImage,                         /* Overlay image.                   */
		   MilStrContext,                           /* String context identifier.       */
		   MilStrResult;                            /* String result buffer identifier. */
	MIL_INT NumberOfStringRead;                     /* Total number of strings to read. */
	MIL_DOUBLE Score;                               /* String score.                    */
	MIL_TEXT_CHAR StringResult[STRING_MAX_SIZE+1];  /* String of characters read.       */
	MIL_DOUBLE Time = 0.0;                          /* Time variable.                   */  

	/* Print module name. */
	MosPrintf(MIL_TEXT("\nSTRING READER MODULE:\n"));
	MosPrintf(MIL_TEXT("---------------------\n\n"));

	/* Allocate defaults */
	MappAllocDefault(M_DEFAULT, &MilApplication, &MilSystem, &MilDisplay, M_NULL, M_NULL);

	/* Restore the font definition image */ 
	MbufRestore(IMAGE_FILE_DEFINITION, MilSystem, &MilImage);

	/* Display the image and prepare for overlay annotations. */
	MdispSelect(MilDisplay, MilImage);
	MdispControl(MilDisplay, M_OVERLAY, M_ENABLE);
	MdispInquire(MilDisplay, M_OVERLAY_ID, &MilOverlayImage);

	/* Allocate a new empty String Reader context. */
	MstrAlloc( MilSystem, M_FONT_BASED, M_DEFAULT, &MilStrContext);

	/* Allocate a new empty String Reader result buffer. */
	MstrAllocResult(MilSystem, M_DEFAULT, &MilStrResult);

	/* Add a new empty user defined font to the context. */
	MstrControl(MilStrContext, M_CONTEXT, M_FONT_ADD, M_USER_DEFINED);

	/* Add user defined characters from the license plate mosaic image. */
	MstrEditFont(MilStrContext, M_FONT_INDEX(0), M_CHAR_ADD, 
		M_USER_DEFINED + M_FOREGROUND_BLACK, 
		MilImage, (void *)TEXT_DEFINITION, M_NULL);

	/* Draw all the characters in the font in the overlay image. */
	MgraColor(M_DEFAULT, M_COLOR_GREEN);
	MstrDraw(M_DEFAULT, MilStrContext, MilOverlayImage, M_DRAW_CHAR, 
		M_FONT_INDEX(0), M_NULL, M_ORIGINAL);

	/* Normalize the characters of the font to an appropriate size. */
	MstrEditFont(MilStrContext, M_FONT_INDEX(0), M_CHAR_NORMALIZE,
		M_SIZE_Y, NORMALIZATION_SIZE_Y, M_NULL, M_NULL);

	/* Add 2 new empty strings models to the context for the 2 valid types of 
	plates (3 letters followed by 3 numbers or 3 numbers followed by 3 letters)
	Note that the read time increases with the number of strings in the context.
	*/
	MstrControl(MilStrContext, M_CONTEXT, M_STRING_ADD, M_USER_DEFINED);
	MstrControl(MilStrContext, M_CONTEXT, M_STRING_ADD, M_USER_DEFINED);

	/* Set number of strings to read. */
	MstrControl(MilStrContext, M_CONTEXT, M_STRING_NUMBER, 1);

	/* Set number of expected characters for all string models to 6 characters. */
	MstrControl(MilStrContext, M_STRING_INDEX(M_ALL), M_STRING_SIZE_MIN, 6);
	MstrControl(MilStrContext, M_STRING_INDEX(M_ALL), M_STRING_SIZE_MAX, 6);

	/* Set default constraint to uppercase letter for all string models */
	MstrSetConstraint(MilStrContext, M_STRING_INDEX(0), M_DEFAULT, 
		M_LETTER + M_UPPERCASE, M_NULL );
	MstrSetConstraint(MilStrContext, M_STRING_INDEX(1), M_DEFAULT, 
		M_LETTER + M_UPPERCASE, M_NULL );

	/* Set constraint of 3 last characters to digit for the first string model */
	MstrSetConstraint(MilStrContext, M_STRING_INDEX(0), 3, M_DIGIT, M_NULL );
	MstrSetConstraint(MilStrContext, M_STRING_INDEX(0), 4, M_DIGIT, M_NULL );
	MstrSetConstraint(MilStrContext, M_STRING_INDEX(0), 5, M_DIGIT, M_NULL );

	/* Set constraint of 3 first characters to digit for the second string model */
	MstrSetConstraint(MilStrContext, M_STRING_INDEX(1), 0, M_DIGIT, M_NULL );
	MstrSetConstraint(MilStrContext, M_STRING_INDEX(1), 1, M_DIGIT, M_NULL );
	MstrSetConstraint(MilStrContext, M_STRING_INDEX(1), 2, M_DIGIT, M_NULL );

	/* Pause to show the font definition. */ 
	MosPrintf(MIL_TEXT("This program has defined a font with this ")
		MIL_TEXT("Quebec plates mosaic image.\n"));
	MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
	MosGetch();

	/* Clear the display overlay. */
	MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);

	/* Load image to read into image buffer. */
	MbufLoad(IMAGE_FILE_TO_READ, MilImage);

	/* Preprocess the String Reader context. */
	MstrPreprocess(MilStrContext, M_DEFAULT);

	/* Dummy first call for bench measure purpose only (bench stabilization, 
	cache effect, etc...). This first call is NOT required by the application. */
	MstrRead(MilStrContext, MilImage, MilStrResult);

	/* Reset the timer. */
	MappTimer(M_DEFAULT, M_TIMER_RESET+M_SYNCHRONOUS, M_NULL);

	/* Perform the read operation on the specified target image. */
	MstrRead(MilStrContext, MilImage, MilStrResult);

	/* Read the time. */
	MappTimer(M_DEFAULT, M_TIMER_READ+M_SYNCHRONOUS, &Time);

	/* Get number of strings read and show the result. */
	MstrGetResult(MilStrResult, M_GENERAL, M_STRING_NUMBER + M_TYPE_MIL_INT, 
		&NumberOfStringRead);
	if( NumberOfStringRead >= 1)
	{
		MosPrintf(MIL_TEXT("The license plate was read successfully (%.2lf ms)\n\n"), 
			Time*1000 );

		/* Draw read result. */
		MgraColor(M_DEFAULT, M_COLOR_BLUE);
		MstrDraw(M_DEFAULT, MilStrResult, MilOverlayImage, M_DRAW_STRING, M_ALL, 
			M_NULL, M_DEFAULT); 
		MgraColor(M_DEFAULT, M_COLOR_GREEN);
		MstrDraw(M_DEFAULT, MilStrResult, MilOverlayImage, M_DRAW_STRING_BOX, M_ALL, 
			M_NULL, M_DEFAULT);

		/* Print the read result. */
		MosPrintf(MIL_TEXT(" String                  Score\n") );
		MosPrintf(MIL_TEXT(" -----------------------------------\n") );
		MstrGetResult(MilStrResult, 0, M_STRING+M_TYPE_TEXT_CHAR, StringResult);
		MstrGetResult(MilStrResult, 0, M_STRING_SCORE, &Score);
		MosPrintf(MIL_TEXT(" %s                  %.1lf\n"), StringResult, Score ); 
	}
	else
	{
		MosPrintf(MIL_TEXT("Error: Plate was not read.\n"));
	}

	/* Pause to show results. */ 
	MosPrintf(MIL_TEXT("\nPress <Enter> to end.\n\n"));
	MosGetch();

	/* Free all allocations. */
	MstrFree(MilStrContext);
	MstrFree(MilStrResult);
	MbufFree(MilImage);

	/* Free defaults. */ 
	MappFreeDefault(MilApplication, MilSystem, MilDisplay, M_NULL, M_NULL);

	return 0;
}



