/************************************************************************************/
/*
 * File name: MdispWindowLeveling.cpp
 *
 * Synopsis:  This MIL program shows how to display a 10-bit monochrome Medical image
 *            and applies a LUT to do interactive Window Leveling.
 */
#include <mil.h>
#include <stdlib.h>

/* Image file to load. */
#define IMAGE_NAME      MIL_TEXT("ArmsMono10bit.mim")
#define IMAGE_FILE      M_IMAGE_PATH IMAGE_NAME

/* Draw the LUT shape (if disabled reduces CPU usage). */
#define DRAW_LUT_SHAPE  M_YES

/* Utility function and macros. */
void DrawLutShape(MIL_ID MilDisplay, 
                  MIL_ID MilOriginalImage, 
                  MIL_ID MilImage, 
                  MIL_INT Start,
                  MIL_INT End, 
                  MIL_INT InflexionIntensity, 
                  MIL_INT ImageMaxValue, 
                  MIL_INT DisplayMaxValue);
#define MosMin(a, b) (((a) < (b)) ? (a) : (b))
#define MosMax(a, b) (((a) > (b)) ? (a) : (b))

int MosMain(void)
{
   MIL_ID MilApplication,       /* Application identifier.    */
          MilSystem,            /* System identifier.         */
          MilDisplay,           /* Display identifier.        */
          MilImage,             /* Image buffer identifier.   */
          MilOriginalImage = 0, /* Image buffer identifier.   */
          MilLut;               /* Lut buffer identifier.     */
   MIL_INT ImageSizeX, ImageSizeY, ImageMaxValue;
   MIL_INT DisplaySizeBit, DisplayMaxValue;
   MIL_INT Start, End, Step, InflectionLevel;
   MIL_INT Ch;
   

   /* Allocate the MIL Application, System and Display. */
   MappAllocDefault(M_DEFAULT, &MilApplication, &MilSystem, &MilDisplay, M_NULL, M_NULL);

   /* Restore target image from disk. */
   MbufRestore(IMAGE_FILE, MilSystem, &MilImage);

   /* Dynamically calculates the maximum value of the image using processing. */
   MIL_ID MilExtremeResult = M_NULL;
   MimAllocResult((MIL_ID)MbufInquire(MilImage, M_OWNER_SYSTEM, M_NULL),
                  1L, M_EXTREME_LIST, &MilExtremeResult);
   MimFindExtreme(MilImage, MilExtremeResult, M_MAX_VALUE);
   MimGetResult(MilExtremeResult, M_VALUE, &ImageMaxValue);
   MimFree(MilExtremeResult);

   /* Set the maximum value of the image to indicate to MIL how to initialize 
      the default display LUT.
   */
   MbufControl(MilImage, M_MAX, (MIL_DOUBLE)ImageMaxValue);

   /* Display the image (to specify a user-defined window, use MdispSelectWindow()). */
   MdispSelect(MilDisplay, MilImage);

   /* Determine the maximum displayable value of the current display. */
   MdispInquire(MilDisplay, M_SIZE_BIT, &DisplaySizeBit);
   DisplayMaxValue = (1<<DisplaySizeBit)-1;

   /* Print key information. */
   MosPrintf(MIL_TEXT("\nINTERACTIVE WINDOW LEVELING:\n"));
   MosPrintf(MIL_TEXT("----------------------------\n\n"));

   MosPrintf(MIL_TEXT("Image name : %s\n"),IMAGE_NAME);

   MosPrintf(MIL_TEXT("Image size : %ld x %ld\n"), 
             MbufInquire(MilImage, M_SIZE_X, &ImageSizeX), 
             MbufInquire(MilImage, M_SIZE_Y, &ImageSizeY));
   MosPrintf(MIL_TEXT("Image max  : %4ld\n"),   ImageMaxValue);
   MosPrintf(MIL_TEXT("Display max: %4ld\n\n"), DisplayMaxValue);
   
   /* Allocate a LUT buffer according to the image maximum value and 
     display pixel depth. */
   MbufAlloc1d(MilSystem, ImageMaxValue+1, 
                               ((DisplaySizeBit>8) ? 16 : 8)+M_UNSIGNED, M_LUT, &MilLut);

   /* Generate a LUT with a full range ramp and set its maximum value. */
   MgenLutRamp(MilLut, 0, 0, ImageMaxValue, (MIL_DOUBLE)DisplayMaxValue);
   MbufControl(MilLut, M_MAX, (MIL_DOUBLE)DisplayMaxValue);

   /* Set the display LUT. */
   MdispLut(MilDisplay, MilLut);

   /* Interactive Window Leveling using keyboard. */
   MosPrintf(MIL_TEXT("Keys assignement:\n\n"));
   MosPrintf(MIL_TEXT("Arrow keys :    Left=move Left, Right=move Right, ")
             MIL_TEXT("Down=Narrower, Up=Wider.\n"));
   MosPrintf(MIL_TEXT("Intensity keys: L=Lower,  U=Upper,  R=Reset.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to end.\n\n"));

   /* Modify LUT shape according to the arrow keys and update it. */
   Ch = 0;
   Start = 0;
   End = ImageMaxValue;
   InflectionLevel = DisplayMaxValue;
   Step = (ImageMaxValue+1)/128;
   Step = MosMax(Step,4);
   while (Ch != '\r')
      {
      switch (Ch)
         {
         /* Left arrow: Move region left. */
         case 0x4B:
            { Start-=Step; End-=Step; break; }

         /* Right arrow: Move region right. */
         case 0x4D:
            { Start+=Step; End+=Step; break; }

         /* Down arrow: Narrow region. */
         case 0x50:
            { Start+=Step; End-=Step; break; }

         /* Up arrow: Widen region. */
         case 0x48:
            { Start-=Step, End+=Step; break; }

         /* L key: Lower inflexion point. */
         case 'L':
         case 'l':
            { InflectionLevel--; break; }

         /* U key: Upper inflexion point. */
         case 'U':
         case 'u':
            { InflectionLevel++; break; }

         /* R key: Reset the LUT to full image range. */
         case 'R':
         case 'r':
            { Start=0; End=ImageMaxValue; InflectionLevel=DisplayMaxValue; break; }

         }

      /* Saturate. */
      End   = MosMin(End,ImageMaxValue);
      Start = MosMin(Start,End);
      End   = MosMax(End,Start);
      Start = MosMax(Start,0);
      End   = MosMax(End,0);
      InflectionLevel = MosMax(InflectionLevel, 0);
      InflectionLevel = MosMin(InflectionLevel, DisplayMaxValue);
      MosPrintf(MIL_TEXT("Inflection points: Low=(%ld,0), High=(%ld,%ld).   \r"),
                                                    Start, End, InflectionLevel);

      /* Generate a LUT with 3 slopes and saturated at both ends. */
      MgenLutRamp(MilLut, 0, 0, Start, 0);
      MgenLutRamp(MilLut, Start, 0, End, (MIL_DOUBLE)InflectionLevel);
      MgenLutRamp(MilLut, End, (MIL_DOUBLE)InflectionLevel, ImageMaxValue, 
                                               (MIL_DOUBLE)DisplayMaxValue);

      /* Update the display LUT. */
      MdispLut(MilDisplay, MilLut);

      /* Draw the current LUT's shape in the image.
         Note: This simple annotation method requires
               significant update and CPU time.
      */
      if (DRAW_LUT_SHAPE)
         {
         if (!MilOriginalImage)
            MbufRestore(IMAGE_FILE, MilSystem, &MilOriginalImage);
         DrawLutShape(MilDisplay, MilOriginalImage, MilImage, Start, End,
                                  InflectionLevel, ImageMaxValue, DisplayMaxValue);
         }

      /* If its an arrow key, get the second code. */
      if ((Ch = MosGetch()) == 0xE0)
           Ch = MosGetch();
      }
   MosPrintf(MIL_TEXT("\n\n"));

   /* Free all allocations. */
   MbufFree(MilLut);
   MbufFree(MilImage);
   if (MilOriginalImage)
      MbufFree(MilOriginalImage);
   MappFreeDefault(MilApplication, MilSystem, MilDisplay, M_NULL, M_NULL);

   return 0;
}

/* Function to draw the current LUT's shape in the image.

   Note: This simple annotation method requires significant update
          and CPU time since it repaints the entire image every time.
*/
void DrawLutShape(MIL_ID MilDisplay, 
                  MIL_ID MilOriginalImage, 
                  MIL_ID MilImage, 
                  MIL_INT Start,
                  MIL_INT End, 
                  MIL_INT InflexionIntensity, 
                  MIL_INT ImageMaxValue, 
                  MIL_INT DisplayMaxValue)
   {
    MIL_INT        ImageSizeX, ImageSizeY;
    MIL_DOUBLE     Xstart, Xend, Xstep, Ymin, Yinf, Ymax, Ystep;
    MIL_TEXT_CHAR  String[8];

    /* Inquire image dimensions. */
    MbufInquire(MilImage, M_SIZE_X, &ImageSizeX);
    MbufInquire(MilImage, M_SIZE_Y, &ImageSizeY);

    /* Calculate the drawing parameters. */
    Xstep  = (MIL_DOUBLE)ImageSizeX/(MIL_DOUBLE)ImageMaxValue;
    Xstart = Start*Xstep;
    Xend   = End*Xstep;
    Ystep  = ((MIL_DOUBLE)ImageSizeY/4.0)/(MIL_DOUBLE)DisplayMaxValue;
    Ymin   = ((MIL_DOUBLE)ImageSizeY-2);
    Yinf   = Ymin-(InflexionIntensity*Ystep);
    Ymax   = Ymin-(DisplayMaxValue*Ystep);

    /* To increase speed, disable display update until all annotations are done. */
    MdispControl(MilDisplay, M_UPDATE, M_DISABLE);

    /* Restore the original image. */
    MbufCopy(MilOriginalImage, MilImage);

    /* Draw axis max and min values. */
    MgraColor(M_DEFAULT, (MIL_DOUBLE)ImageMaxValue);
    MgraText(M_DEFAULT, MilImage, 4, (MIL_INT)Ymin-22, MIL_TEXT("0"));
    MosSprintf(String, 8, MIL_TEXT("%ld"), DisplayMaxValue);
    MgraText(M_DEFAULT, MilImage, 4, (MIL_INT)Ymax-16, String);
    MosSprintf(String, 8, MIL_TEXT("%ld"), ImageMaxValue);
    MgraText(M_DEFAULT, MilImage, ImageSizeX-38 , (MIL_INT)Ymin-22, String);

    /* Draw LUT Shape (X axis is display values and Y is image values). */
    MgraLine(M_DEFAULT, MilImage, 0, (MIL_INT)Ymin, (MIL_INT)Xstart, (MIL_INT)Ymin);
    MgraLine(M_DEFAULT, MilImage, (MIL_INT)Xstart, (MIL_INT)Ymin, 
                                                      (MIL_INT)Xend, (MIL_INT)Yinf);
    MgraLine(M_DEFAULT, MilImage, (MIL_INT)Xend, (MIL_INT)Yinf, 
                                                       ImageSizeX-1, (MIL_INT)Ymax);

    /* Enable display update to show the result. */
    MdispControl(MilDisplay, M_UPDATE, M_ENABLE);
   }

