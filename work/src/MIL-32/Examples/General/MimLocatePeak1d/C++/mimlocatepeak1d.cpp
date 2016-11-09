/*****************************************************************************/
/* 
 * File name: MImLocatePeak1d.cpp
 *
 * Synopsis:  This program finds the peak in each column of an input sequence
 *            and reconstruct the height of a 3d object using it.
 */

#include <mil.h>

#if M_MIL_USE_WINDOWS && !M_MIL_USE_CE
   /*Include DirectX display only in Windows*/
   #include "MdispD3D.h"
#endif

/* Input sequence specifications. */
#define SEQUENCE_FILE    M_IMAGE_PATH MIL_TEXT("HandWithLaser.avi")

/* Peak detection parameters. */
#define MAX_LINE_WIDTH          10
#define MIN_INTENSITY          100
#define NB_FIXED_POINT           4

/* D3D display parameters */
#define D3D_DISPLAY_SIZE_X     640
#define D3D_DISPLAY_SIZE_Y     480
#define D3D_MESH_SCALING_X    -1.0
#define D3D_MESH_SCALING_Y     4.0
#define D3D_MESH_SCALING_Z    -0.13

int MosMain(void)
{
   MIL_ID      MilApplication,  /* Application identifier.     */
               MilSystem,       /* System identifier.          */
               MilDisplay,      /* Display identifier.         */
               MilDisplayImage, /* Image buffer identifier.    */
               MilImage,        /* Image buffer identifier.    */
               MilPosYImage,    /* Image buffer identifier.    */
               MilValImage,     /* Image buffer identifier.    */
               MilExtreme = M_NULL;      /* Result buffer identifier.   */
   MIL_INT     SizeX,
               SizeY,
               NumberOfImages;
   MIL_DOUBLE  FrameRate;
   MIL_INT     n;
   MIL_DOUBLE  PreviousTime,
               StartTime,
               EndTime,
               TotalProcessTime,
               WaitTime;
   MIL_INT     ExtremePosY[2] = {0, 0};


   /* Allocate defaults. */
   MappAllocDefault(M_DEFAULT, &MilApplication, &MilSystem,
                    &MilDisplay, M_NULL, M_NULL);

   /* Inquire characteristics of the input sequence. */
   MbufDiskInquire(SEQUENCE_FILE, M_SIZE_X, &SizeX);
   MbufDiskInquire(SEQUENCE_FILE, M_SIZE_Y, &SizeY);
   MbufDiskInquire(SEQUENCE_FILE, M_NUMBER_OF_IMAGES, &NumberOfImages);
   MbufDiskInquire(SEQUENCE_FILE, M_FRAME_RATE, &FrameRate);

   /* Allocate buffers to hold images. */
   MbufAlloc2d(MilSystem, SizeX, SizeY         ,  8+M_UNSIGNED, M_IMAGE+M_PROC, &MilImage);
   MbufAlloc2d(MilSystem, SizeX, SizeY         ,  8+M_UNSIGNED, M_IMAGE+M_DISP, 
                                                                         &MilDisplayImage);
   MbufAlloc2d(MilSystem, SizeX, NumberOfImages, 16+M_UNSIGNED, M_IMAGE+M_PROC, &MilPosYImage);
   MbufAlloc2d(MilSystem, SizeX, NumberOfImages,  8+M_UNSIGNED, M_IMAGE+M_PROC, &MilValImage);

   /* Select display. */
   MdispSelect(MilDisplay, MilDisplayImage);

   /* Print a message. */
   MosPrintf(MIL_TEXT("\nEXTRACTING 3D IMAGE FROM A LASER LINE:\n"));
   MosPrintf(MIL_TEXT("--------------------------------------\n\n"));
   MosPrintf(MIL_TEXT("The position of a laser line is being extracted from ")
      MIL_TEXT("an image\n"));
   MosPrintf(MIL_TEXT("to generate a depth image.\n\n"));

   /* Open the sequence file for reading. */
   MbufImportSequence(SEQUENCE_FILE, M_DEFAULT, M_NULL, M_NULL, NULL, M_NULL, M_NULL, M_OPEN);

   /* The function must be called once before the timing loop for more accurate 
      time (dll load, ...).
   */
   MimLocatePeak1d(MilImage, 
                   MilPosYImage, 
                   MilValImage, 
                   0, 
                   MAX_LINE_WIDTH, 
                   MIN_INTENSITY, 
                   M_1D_COLUMNS+M_FIXED_POINT+NB_FIXED_POINT, 
                   M_DEFAULT);

   /* Read and process all images in the input sequence. */
   MappTimer(M_DEFAULT, M_TIMER_READ+M_SYNCHRONOUS, &PreviousTime);
   TotalProcessTime = 0.0;

   for (n = 0; n < NumberOfImages; n++)
      {
      /* Read image from sequence. */
      MbufImportSequence(SEQUENCE_FILE, M_DEFAULT, M_LOAD, M_NULL, 
                                   &MilImage, M_DEFAULT, 1, M_READ);

      /* Display the image. */
      MbufCopy(MilImage, MilDisplayImage);

      /* Locate the peak in each column of the image. */
      MappTimer(M_DEFAULT, M_TIMER_READ+M_SYNCHRONOUS, &StartTime);
      MimLocatePeak1d(MilImage, 
                      MilPosYImage, 
                      MilValImage, 
                      n, 
                      MAX_LINE_WIDTH, 
                      MIN_INTENSITY, 
                      M_1D_COLUMNS+M_FIXED_POINT+NB_FIXED_POINT, 
                      M_DEFAULT);
      MappTimer(M_DEFAULT, M_TIMER_READ+M_SYNCHRONOUS, &EndTime);
      TotalProcessTime += EndTime - StartTime;

      /* Wait to have a proper frame rate. */
      WaitTime = (1.0/FrameRate) - (EndTime - PreviousTime);
      if (WaitTime > 0)
         MappTimer(M_DEFAULT, M_TIMER_WAIT, &WaitTime);
      MappTimer(M_DEFAULT, M_TIMER_READ+M_SYNCHRONOUS, &PreviousTime);
      }

   /* Close the sequence file. */
   MbufImportSequence(SEQUENCE_FILE, M_DEFAULT, M_NULL, M_NULL, NULL, M_NULL, M_NULL, M_CLOSE);

   MosPrintf(MIL_TEXT("%ld images processed in %7.2lf s (%7.2lf ms/image).\n"), 
             NumberOfImages, TotalProcessTime, 
             TotalProcessTime / (MIL_DOUBLE)NumberOfImages * 1000.0);

   /* Pause to show the result. */
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   MosPrintf(MIL_TEXT("The reconstructed images are being displayed.\n"));

   /* Draw extracted peak position in each column of each image. */
   for (n = 0; n < NumberOfImages; n++)
      {
      MbufClear(MilImage, 0);
      MimDraw(M_DEFAULT, MilPosYImage, MilValImage, MilImage, 
              M_DRAW_PEAKS+M_1D_COLUMNS+M_LINES, (MIL_DOUBLE)n,
              1, M_FIXED_POINT+NB_FIXED_POINT);

      /* Display the result image. */
      MbufCopy(MilImage, MilDisplayImage);
      }

   /* Pause to show the result. */
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

#if M_MIL_USE_WINDOWS && !M_MIL_USE_CE
   /* Try to allocate D3D display */
   MIL_DISP_D3D_HANDLE DispHandle;
   DispHandle = MdepthD3DAlloc(MilPosYImage, MilValImage, 
                               D3D_DISPLAY_SIZE_X, 
                               D3D_DISPLAY_SIZE_Y, 
                               D3D_MESH_SCALING_X,
                               D3D_MESH_SCALING_Y,
                               D3D_MESH_SCALING_Z,
                               M_DEFAULT,
                               M_DEFAULT,
                               M_DEFAULT,
                               0);

   if (DispHandle != NULL)
      {
      MosPrintf(MIL_TEXT("The depth buffer is displayed using D3D.\n"));

      /* Hide Mil Display */
      MdispControl(MilDisplay, M_WINDOW_SHOW, M_DISABLE );

      MdispD3DShow(DispHandle);
      MdispD3DPrintHelp(DispHandle);

      /* Pause to show the result. */
      MosPrintf(MIL_TEXT("Press <Enter> to end.\n"));
      MosGetch();

      MdispD3DHide(DispHandle);
      MdispD3DFree(DispHandle);
      }
   else
#endif
      {
      MosPrintf(MIL_TEXT("The depth buffer is displayed using MIL.\n"));

      /* Find the remapping for result buffers. */
      MimAllocResult( MilSystem, M_DEFAULT, M_STAT_LIST, &MilExtreme);

      MimStat(MilPosYImage, MilExtreme, M_MIN+M_MAX,M_NOT_EQUAL, 0xFFFF, M_NULL);
      MimGetResult(MilExtreme, M_MIN + M_TYPE_MIL_INT, &ExtremePosY[0]);
      MimGetResult(MilExtreme, M_MAX + M_TYPE_MIL_INT, &ExtremePosY[1]);

      MimFree(MilExtreme);

      /* Free the display and reallocate a new one of the proper dimension for results. */
      MbufFree(MilDisplayImage);
      MbufAlloc2d(MilSystem, 
                  (MIL_INT)((MIL_DOUBLE)SizeX*(D3D_MESH_SCALING_X > 0 ? D3D_MESH_SCALING_X : -D3D_MESH_SCALING_X)), 
                  (MIL_INT)((MIL_DOUBLE)NumberOfImages*D3D_MESH_SCALING_Y),  
                  8+M_UNSIGNED, 
                  M_IMAGE+M_PROC+M_DISP, 
                  &MilDisplayImage);

      MdispSelect(MilDisplay, MilDisplayImage);

      /* Display the height buffer. */
      MimClip(MilPosYImage, MilPosYImage, M_GREATER, (MIL_DOUBLE)ExtremePosY[1], M_NULL, 
                                                     (MIL_DOUBLE)ExtremePosY[1], M_NULL);
      MimArith(MilPosYImage, (MIL_DOUBLE)ExtremePosY[0], MilPosYImage, M_SUB_CONST);
      MimArith(MilPosYImage, ((ExtremePosY[1]-ExtremePosY[0])/255.0)+1, 
                                                         MilPosYImage, M_DIV_CONST);
      MimResize(MilPosYImage, MilDisplayImage, M_FILL_DESTINATION, 
                                                   M_FILL_DESTINATION, M_BILINEAR);

      /* Pause to show the result. */
      MosPrintf(MIL_TEXT("Press <Enter> to end.\n"));
      MosGetch();
      }

   /* Free all allocations. */
   MbufFree(MilImage);
   MbufFree(MilDisplayImage);
   MbufFree(MilPosYImage);
   MbufFree(MilValImage);
   MappFreeDefault(MilApplication, MilSystem, MilDisplay, M_NULL, M_NULL);

   return 0;
}
