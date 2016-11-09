/******************************************************************************/
/*
/* File name: MgraText.cpp
/*
/* Synopsis:  This program allocates a MIL application and system, then displays
/*            messages using TrueType fonts. It also shows how to check for errors.
*/
#include <mil.h>

int MosMain(void)
{
   MIL_ID MilApplication,      /* Application identifier.    */
          MilSystem,           /* System identifier.         */
          MilDisplay,          /* Display identifier.        */
          MilImage,            /* Image buffer identifier.   */
          MilGraphicContextId; /* Graphic context identifier */ 

   /* Allocate a default MIL application, system, display and image. */
   MappAllocDefault(M_DEFAULT, &MilApplication, &MilSystem, 
                                       &MilDisplay, M_NULL, &MilImage);

   /* Allocate a graphic context */
   MgraAlloc(MilSystem,  &MilGraphicContextId);

   /* Perform graphic operations in the display image. */
   MgraColor(MilGraphicContextId, 0xF0);
   MgraFont(MilGraphicContextId, M_FONT_DEFAULT_LARGE);
   MgraText(MilGraphicContextId, MilImage, 160L, 20L, MIL_TEXT(" Welcome to MIL !!! "));

   MgraControl(MilGraphicContextId,M_FONT_SIZE, 12);
   MgraFont(MilGraphicContextId, MIL_FONT_NAME(M_FONT_DEFAULT_TTF));
   MgraText(MilGraphicContextId, MilImage, 40L, 80L, MIL_TEXT("English"));

   MappControl(M_DEFAULT, M_ERROR, M_PRINT_DISABLE);

   MgraControl(MilGraphicContextId,M_FONT_SIZE, 16);
   MgraFont(MilGraphicContextId, 
                     MIL_FONT_NAME(M_FONT_DEFAULT_TTF MIL_TEXT(":Bold")));
   MgraText(MilGraphicContextId, MilImage, 40L, 140L, MIL_TEXT("Français"));

   MgraControl(MilGraphicContextId,M_FONT_SIZE, 24);
   MgraFont(MilGraphicContextId, MIL_FONT_NAME(M_FONT_DEFAULT_TTF MIL_TEXT(":Italic")));
   MgraText(MilGraphicContextId, MilImage, 40L, 220L, MIL_TEXT("Italiano"));

   MgraControl(MilGraphicContextId,M_FONT_SIZE, 30);
   MgraFont(MilGraphicContextId, 
               MIL_FONT_NAME(M_FONT_DEFAULT_TTF MIL_TEXT(":Bold:Italic")));
   MgraText(MilGraphicContextId, MilImage, 40L ,300L, MIL_TEXT("Deutsch"));

   /* Trying OS specific font. */
   MgraControl(MilGraphicContextId,M_FONT_SIZE, 36);
#if M_MIL_USE_LINUX
   MgraFont(MilGraphicContextId, MIL_FONT_NAME(MIL_TEXT("Monospace")));
#else
   MgraFont(MilGraphicContextId, MIL_FONT_NAME(MIL_TEXT("Courier New")));
#endif
   MgraText(MilGraphicContextId, MilImage, 40L ,380L, MIL_TEXT("Español"));


#if M_MIL_USE_TTF_UNICODE
   /* Draw Greek, Japanese and Korean */
   MgraFont(MilGraphicContextId, MIL_FONT_NAME(M_FONT_DEFAULT_TTF));
   MgraControl(MilGraphicContextId,M_FONT_SIZE, 12);
   MgraText(MilGraphicContextId, MilImage, 400L,  80L, MIL_TEXT("ελληνιδ"));
      
   MgraControl(MilGraphicContextId,M_FONT_SIZE, 16);
   MgraText(MilGraphicContextId, MilImage, 400L, 140L, MIL_TEXT("日本語"));

   MgraControl(MilGraphicContextId,M_FONT_SIZE, 24);
   MgraText(MilGraphicContextId, MilImage, 400L,  220L, MIL_TEXT("한국어"));
      
   /* Draw Arabic and Hebrew */
   MgraControl(MilGraphicContextId,M_TEXT_DIRECTION, M_RIGHT_TO_LEFT);
   MgraControl(MilGraphicContextId,M_FONT_SIZE, 30);
   MgraText(MilGraphicContextId, MilImage, 400L, 320L, MIL_TEXT("עברית"));
      
   MgraControl(MilGraphicContextId,M_FONT_SIZE, 36);
   MgraText(MilGraphicContextId, MilImage, 400L,  380L, MIL_TEXT("ﻋﺮﺑﻲ"));
 #endif     
   
   /* Print a message. */
   MosPrintf(MIL_TEXT("\nINTERNATIONAL TEXT ANNOTATION:\n"));
   MosPrintf(MIL_TEXT("------------------------------\n"));
   MosPrintf(MIL_TEXT("\nThis example demonstrates the support"));
   MosPrintf(MIL_TEXT(" of TrueType fonts by MgraText.\n\n"));

   if(MappGetError(M_GLOBAL + M_SYNCHRONOUS, 0) != M_NULL_ERROR)
      MosPrintf(MIL_TEXT("Note: Some Unicode fonts are not available\n\n"));

   /* Wait for a key press. */
   MosPrintf(MIL_TEXT("Press <Enter> to end.\n"));
   MosGetch();

   /* Free Graphic Context. */
   MgraFree(MilGraphicContextId);
     
   /* Free defaults. */
   MappFreeDefault(MilApplication, MilSystem, MilDisplay, M_NULL, MilImage);

   return 0;
}


 
 

 
