/******************************************************************************/
/*
 * File name: MAppStart.cpp 
 *
 * Synopsis:  This program allocates a MIL application and system, then displays 
 *            a welcoming message using graphics functions. It also shows how 
 *            to check for errors.
 */
#include <mil.h>

int MosMain(void)
{
   MIL_ID MilApplication,  /* Application identifier.  */
          MilSystem,       /* System identifier.       */
          MilDisplay,      /* Display identifier.      */
          MilImage;        /* Image buffer identifier. */

   /* Allocate a default MIL application, system, display and image. */
   MappAllocDefault(M_DEFAULT, &MilApplication, &MilSystem, 
                                       &MilDisplay, M_NULL, &MilImage);

   /* If no allocation errors. */
   if (!MappGetError(M_GLOBAL, M_NULL))
      {
      /* Perform graphic operations in the display image. */ 
      MgraColor(M_DEFAULT, 0xF0);
      MgraFont(M_DEFAULT, M_FONT_DEFAULT_LARGE);
      MgraText(M_DEFAULT, MilImage, 160L, 230L, MIL_TEXT(" Welcome to MIL !!! "));
      MgraColor(M_DEFAULT, 0xC0);
      MgraRect(M_DEFAULT, MilImage, 100L, 150L, 530L, 340L);
      MgraRect(M_DEFAULT, MilImage, 120L, 170L, 510L, 320L);
      MgraRect(M_DEFAULT, MilImage, 140L, 190L, 490L, 300L);
      
      /* Print a message. */
      MosPrintf(MIL_TEXT("\nSYSTEM ALLOCATION:\n"));
      MosPrintf(MIL_TEXT("------------------\n\n"));
      MosPrintf(MIL_TEXT("System allocation successful.\n\n"));
      MosPrintf(MIL_TEXT("     \"Welcome to MIL !!!\"\n\n"));
      }
   else
      MosPrintf(MIL_TEXT("System allocation error !\n\n"));

   /* Wait for a key press. */
   MosPrintf(MIL_TEXT("Press <Enter> to end.\n"));
   MosGetch();

   /* Free defaults. */
   MappFreeDefault(MilApplication, MilSystem, MilDisplay, M_NULL, MilImage);

   return 0;
}

