/***********************************************************************************/
/* 
 * File name: MImHistogram.cpp
 *
 * Synopsis:  This program loads an image of a tissue sample, calculates its intensity
 *            histogram and draws it.
 */
#include <mil.h>

/* Target MIL image file specifications. */
#define IMAGE_FILE  M_IMAGE_PATH MIL_TEXT("Cell.mim")

/* Number of possible pixel intensities. */
#define HIST_NUM_INTENSITIES  256
#define HIST_SCALE_FACTOR     8
#define HIST_X_POSITION       250
#define HIST_Y_POSITION       450


/* Main function. */
int MosMain(void)
   { 
   MIL_ID     MilApplication,                   /* Application identifier       */
              MilSystem,                        /* System identifier.           */
              MilDisplay,                       /* Display identifier.          */
              MilImage,                         /* Image buffer identifier.     */
              MilOverlayImage,                  /* Overlay buffer identifier.   */
              HistResult;                       /* Histogram buffer identifier. */
   MIL_INT    HistValues[HIST_NUM_INTENSITIES]; /* Histogram values.            */
   MIL_INT    XStart[HIST_NUM_INTENSITIES], YStart[HIST_NUM_INTENSITIES],
              XEnd[HIST_NUM_INTENSITIES],   YEnd[HIST_NUM_INTENSITIES];
   MIL_DOUBLE AnnotationColor = M_COLOR_RED;
   MIL_INT    i; 

   /* Allocate the default system and image buffer. */
   MappAllocDefault(M_DEFAULT, &MilApplication, &MilSystem, &MilDisplay, M_NULL, M_NULL);
 
   /* Restore source image into an automatically allocated image buffer. */
   MbufRestore(IMAGE_FILE, MilSystem, &MilImage);
  
   /* Display the image buffer and prepare for overlay annotations. */
   MdispSelect(MilDisplay, MilImage);
   MdispControl(MilDisplay, M_OVERLAY, M_ENABLE);
   MdispInquire(MilDisplay, M_OVERLAY_ID, &MilOverlayImage);
   
   /* Allocate a histogram result buffer. */
   MimAllocResult(MilSystem, HIST_NUM_INTENSITIES, M_HIST_LIST, &HistResult);
 
   /* Calculate the histogram. */
   MimHistogram(MilImage, HistResult);
 
   /* Get the results. */
   MimGetResult(HistResult, M_VALUE, HistValues);

   /* Draw the histogram in the overlay. */
   MgraColor(M_DEFAULT, AnnotationColor);
   for(i=0; i<HIST_NUM_INTENSITIES; i++)
      {
      XStart[i] = i+HIST_X_POSITION+1,
      YStart[i] = HIST_Y_POSITION;
      XEnd[i]   = i+HIST_X_POSITION+1, 
      YEnd[i] = HIST_Y_POSITION-(HistValues[i]/HIST_SCALE_FACTOR);
      }
   MgraLines(M_DEFAULT, MilOverlayImage, HIST_NUM_INTENSITIES, 
                        XStart, YStart, XEnd, YEnd, M_DEFAULT);

   /* Print a message. */
   MosPrintf(MIL_TEXT("\nHISTOGRAM:\n"));
   MosPrintf(MIL_TEXT("----------\n\n"));
   MosPrintf(MIL_TEXT("The histogram of the image was calculated and drawn.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to end.\n\n"));
   MosGetch();
 
   /* Free all allocations. */
   MimFree(HistResult);
   MbufFree(MilImage);
   MappFreeDefault(MilApplication, MilSystem, MilDisplay, M_NULL, M_NULL);

   return 0;
}

