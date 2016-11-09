/***************************************************************************************/
/* 
* File name: DotSpacing.cpp  
*
* Synopsis:  This program contains an example of setting the dot spacing in matrix code
*            type using the Code Reader module.
*            See the PrintHeader() function below for detailed description.
*
*/
#include <mil.h>


/****************************************************************************
Example description.
****************************************************************************/
void PrintHeader()   
   {
   MosPrintf(MIL_TEXT("[EXAMPLE NAME]\n"));
   MosPrintf(MIL_TEXT("DotSpacing\n\n"));

   MosPrintf(MIL_TEXT("[SYNOPSIS]\n"));
   MosPrintf(MIL_TEXT("This program illustrates the use of the dot spacing setting to read a \n"));
   MosPrintf(MIL_TEXT("dot-printed DataMatrix code.\n\n"));

   MosPrintf(MIL_TEXT("[MODULES USED]\n"));
   MosPrintf(MIL_TEXT("Modules used: application, system, display, buffer, graphic, code.\n\n"));

   MosPrintf(MIL_TEXT("Press <Enter> to read the datamatrix in the target image, using \n"));
   MosPrintf(MIL_TEXT("default settings.\n\n"));
   MosGetch();
   }

/* Code image. */
#define IMAGE_FILE                    M_IMAGE_PATH MIL_TEXT("CodeDotSpacing/DataMatrix_Matrox.mim")

/* Functions declarations. */
void DotSpacing(MIL_ID MilSystem, MIL_ID MilSrcImage, MIL_ID MilDisplay, MIL_INT CodeType);


/*****************************************************************************
Main.
*****************************************************************************/
int MosMain(void)
   {
   MIL_ID      MilApplication,        /* Application identifier.         */
               MilSystem,             /* System identifier.              */
               MilDisplay;            /* Display identifier.             */

   /* Allocate MIL objects. */
   MappAlloc(M_NULL, M_DEFAULT, &MilApplication);
   MsysAlloc(M_DEFAULT, M_SYSTEM_HOST, M_DEFAULT, M_DEFAULT, &MilSystem);
   MdispAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &MilDisplay);

   /* Print Header. */
   PrintHeader();

   /********************/
   /* DOT SPACING: */
   /********************/
   /* Restore source image into an automatically allocated image buffer. */
   MIL_ID MilSrcImage;
   MbufRestore(IMAGE_FILE, MilSystem, &MilSrcImage);

   DotSpacing(MilSystem, MilSrcImage, MilDisplay, M_DATAMATRIX);

   /* Free source image. */
   MbufFree(MilSrcImage);

   /* Free other allocations. */
   MdispFree(MilDisplay);
   MsysFree(MilSystem);
   MappFree(MilApplication);

   return 0;
   }

void DotSpacing(MIL_ID MilSystem, MIL_ID MilSrcImage, MIL_ID MilDisplay, MIL_INT CodeType)
   {
   /* Allocate a display buffers and show the source image. */
   MIL_ID MilDisplayImage;
   MbufAlloc2d(MilSystem, 
               MbufInquire(MilSrcImage, M_SIZE_X, M_NULL),
               MbufInquire(MilSrcImage, M_SIZE_Y, M_NULL),
               MbufInquire(MilSrcImage, M_TYPE,   M_NULL),
               M_IMAGE+M_DISP, 
               &MilDisplayImage);

   MbufClear(MilDisplayImage, 0L);
   MbufCopy(MilSrcImage, MilDisplayImage);

   /* Display the image buffer. */
   MdispSelect(MilDisplay, MilDisplayImage);

   /* Prepare for overlay annotations. */
   MIL_ID MilOverlayImage;
   MdispControl(MilDisplay, M_OVERLAY, M_ENABLE);
   MdispInquire(MilDisplay, M_OVERLAY_ID, &MilOverlayImage);
   MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);

   /* Allocate CODE objects. */
   MIL_ID MilCodeContext;
   McodeAlloc(MilSystem, M_DEFAULT, M_DEFAULT, &MilCodeContext);
   McodeModel(MilCodeContext, M_ADD, CodeType, M_NULL, M_DEFAULT, M_NULL);

   /* Prepare bar code results buffer. */
   MIL_ID MilCodeResult;
   McodeAllocResult(MilSystem, M_DEFAULT, &MilCodeResult);
   MIL_INT Status = !M_STATUS_READ_OK;
   
   for(MIL_INT ii = 0; ii < 2 && (Status != M_STATUS_READ_OK); ii++)
      {
      /* Read codes from image. */
      McodeRead(MilCodeContext, MilSrcImage, MilCodeResult);

      /* Get decoding status. */
      McodeGetResult(MilCodeResult, M_STATUS+M_TYPE_MIL_INT, &Status);

      /* Check if the verification operation was successful. */
      if (Status == M_STATUS_READ_OK)
         {
         /* Set the drawing colors. */
         MgraColor(M_DEFAULT, M_COLOR_GREEN); 
         MgraBackColor(M_DEFAULT, M_COLOR_GRAY); 

         /* Draw a box around the code. */
         McodeDraw(M_DEFAULT, MilCodeResult, MilOverlayImage, M_DRAW_BOX, M_ALL, M_DEFAULT);

         /* Get decoded string. */
         MIL_INT ResultStringSize;
         McodeGetResult(MilCodeResult, M_STRING_SIZE+M_TYPE_MIL_INT, &ResultStringSize);
         MIL_TEXT_CHAR* ResultString = new MIL_TEXT_CHAR[ResultStringSize+1]; /* Array of characters read.    */
         McodeGetResult(MilCodeResult, M_STRING, ResultString);

         /* Replace non printable characters with space. */
         for(MIL_INT n=0; n < ResultStringSize; n++)
            {
            if ((ResultString[n] < MIL_TEXT('!')) || (ResultString[n] > MIL_TEXT('~')))
               ResultString[n] = MIL_TEXT(' ');
            }

         /* Add prefix to the string. */
         const MIL_TEXT_CHAR PrefixString[] = MIL_TEXT("Read string: ");
         MIL_INT OutputStringSize = ResultStringSize + MosStrlen(PrefixString) + 1;
         MIL_TEXT_CHAR* OutputString = new MIL_TEXT_CHAR[OutputStringSize]; /* Array of characters to draw. */
         MosSprintf(OutputString, OutputStringSize, MIL_TEXT("%s%s"), PrefixString, ResultString);
         
         /* Get coordinates of the bottom-left corner of the code. */
         double BottomLeftX, BottomLeftY;
         McodeGetResult(MilCodeResult, M_BOTTOM_LEFT_X, &BottomLeftX);
         McodeGetResult(MilCodeResult, M_BOTTOM_LEFT_Y, &BottomLeftY);

         /* Draw decoded string. */
         MgraText(M_DEFAULT, 
                  MilOverlayImage, 
                  BottomLeftX,  
                  BottomLeftY+20, 
                  OutputString);

         delete [] OutputString;
         delete [] ResultString;

         MosPrintf(MIL_TEXT("The decoding is successful and the string result ")
            MIL_TEXT("is displayed under the code.\n"));
         }
      else
         {
         MosPrintf(MIL_TEXT("The code read operation fails!\n\n"));
         if(ii == 0)
            {
            MosPrintf(MIL_TEXT("Press <Enter> to read the datamatrix using the dot-spacing setting.\n\n"));
            MosGetch();

            McodeControl(MilCodeContext, M_DOT_SPACING, 7);
            }
         }
      }

   MosPrintf(MIL_TEXT("Press <Enter> to end.\n\n"));
   MosGetch();

   /* Free Display image. */
   MbufFree(MilDisplayImage);

   /* Free objects. */
   McodeFree(MilCodeResult);
   McodeFree(MilCodeContext);
   }
