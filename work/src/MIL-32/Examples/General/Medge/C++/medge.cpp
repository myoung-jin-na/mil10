/*****************************************************************************/
/* 
 * File name: MEdge.cpp
 *
 * Synopsis:  This program uses the MIL Edge Finder module to find and measure
 *            the outer diameter of good seals in a target image. 
 */
#include <mil.h>

/* Source MIL image file specifications. */
#define CONTOUR_IMAGE               M_IMAGE_PATH MIL_TEXT("Seals.mim")
#define CONTOUR_MAX_RESULTS         100
#define CONTOUR_MAXIMUM_ELONGATION  0.8
#define CONTOUR_DRAW_COLOR          M_COLOR_GREEN
#define CONTOUR_LABEL_COLOR         M_COLOR_RED


 /* Main function. */
int MosMain(void)
   {
   MIL_ID      MilApplication,                         /* Application identifier.   */
               MilSystem,                              /* System Identifier.        */
               MilDisplay,                             /* Display identifier.       */
               MilImage,                               /* Image buffer identifier.  */
               GraphicList,                            /* Graphic list identifier.  */
               MilEdgeContext,                         /* Edge context.             */ 
               MilEdgeResult;                          /* Edge result identifier.   */   
   MIL_DOUBLE  EdgeDrawColor = CONTOUR_DRAW_COLOR,     /* Edge draw color.          */
               LabelDrawColor = CONTOUR_LABEL_COLOR;   /* Text draw color.          */
   MIL_INT     NumEdgeFound  = 0L,                     /* Number of edges found.    */
               NumResults    = 0L,                     /* Number of results found.  */   
               i;                                      /* Loop variable.            */
   MIL_DOUBLE MeanFeretDiameter[CONTOUR_MAX_RESULTS]; /* Edge mean Feret diameter. */

   /* Allocate defaults. */
   MappAllocDefault(M_DEFAULT, &MilApplication, &MilSystem, &MilDisplay, M_NULL, M_NULL);

   /* Restore the image and display it. */
   MbufRestore(CONTOUR_IMAGE, MilSystem, &MilImage);
   MdispSelect(MilDisplay, MilImage);

   /* Allocate a graphic list to hold the subpixel annotations to draw. */
   MgraAllocList(MilSystem, M_DEFAULT, &GraphicList);

   /* Associate the graphic list to the display for annotations. */
   MdispControl(MilDisplay, M_ASSOCIATED_GRAPHIC_LIST_ID, GraphicList);

   /* Pause to show the original image. */ 
   MosPrintf(MIL_TEXT("\nEDGE MODULE:\n"));  
   MosPrintf(MIL_TEXT("------------\n\n"));  
   MosPrintf(MIL_TEXT("This program determines the outer seal diameters ")
             MIL_TEXT("in the displayed image \n"));
   MosPrintf(MIL_TEXT("by detecting and analyzing contours ")
             MIL_TEXT("with the Edge Finder module.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));  
   MosGetch();
   
   /* Allocate a Edge Finder context. */
   MedgeAlloc(MilSystem, M_CONTOUR, M_DEFAULT, &MilEdgeContext); 

   /* Allocate a result buffer. */
   MedgeAllocResult(MilSystem, M_DEFAULT, &MilEdgeResult);

   /* Enable features to compute. */
   MedgeControl(MilEdgeContext, M_MOMENT_ELONGATION,                M_ENABLE);
   MedgeControl(MilEdgeContext, M_FERET_MEAN_DIAMETER+M_SORT1_DOWN, M_ENABLE);

   /* Calculate edges and features. */
   MedgeCalculate(MilEdgeContext, MilImage, M_NULL, M_NULL, M_NULL, MilEdgeResult, 
                                                                    M_DEFAULT);

   /* Get the number of edges found. */
   MedgeGetResult(MilEdgeResult, M_DEFAULT, M_NUMBER_OF_CHAINS+M_TYPE_MIL_INT, 
                                                               &NumEdgeFound, M_NULL);

   /* Draw edges in the source image to show the result. */
   MgraColor(M_DEFAULT, EdgeDrawColor);
   MedgeDraw(M_DEFAULT, MilEdgeResult, GraphicList, M_DRAW_EDGES, M_DEFAULT, 
                                                                      M_DEFAULT);

   /* Pause to show the edges. */
   MosPrintf(MIL_TEXT("%d edges were found in the image.\n"), NumEdgeFound);
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));  
   MosGetch();
 
   /* Exclude elongated edges. */
   MedgeSelect(MilEdgeResult, M_EXCLUDE, M_MOMENT_ELONGATION, 
                                         M_LESS, CONTOUR_MAXIMUM_ELONGATION, M_NULL);   

   /* Exclude inner chains. */
   MedgeSelect(MilEdgeResult, M_EXCLUDE, M_INCLUDED_EDGES, M_INSIDE_BOX, M_NULL, M_NULL);

   /* Draw remaining edges and their index to show the result. */
   MgraClear(M_DEFAULT, GraphicList);
   MgraColor(M_DEFAULT, EdgeDrawColor);
   MedgeDraw(M_DEFAULT, MilEdgeResult, GraphicList, M_DRAW_EDGES, M_DEFAULT,
                                                                      M_DEFAULT);

   /* Pause to show the results. */
   MosPrintf(MIL_TEXT("Elongated edges and inner edges of each seal were removed.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();
      
   /* Get the number of edges found. */
   MedgeGetResult(MilEdgeResult, M_DEFAULT, M_NUMBER_OF_CHAINS+M_TYPE_MIL_INT, 
                                                                  &NumResults, M_NULL);

   /* If the right number of edges were found. */
   if ( (NumResults >= 1) && (NumResults <= CONTOUR_MAX_RESULTS) )
      {
      /* Draw the index of each edge. */
      MgraColor(M_DEFAULT, LabelDrawColor);
      MedgeDraw(M_DEFAULT, MilEdgeResult, GraphicList, M_DRAW_INDEX, 
                                                                 M_DEFAULT, M_DEFAULT);  

      /* Get the mean Feret diameters. */      
      MedgeGetResult(MilEdgeResult, M_DEFAULT, M_FERET_MEAN_DIAMETER, 
                                                            MeanFeretDiameter, M_NULL);

      /* Print the results. */
      MosPrintf(MIL_TEXT("Mean diameter of the %ld outer edges are:\n\n"), NumResults);
      MosPrintf(MIL_TEXT("Index   Mean diameter \n"));
      for (i=0; i<NumResults; i++)
         {
         MosPrintf(MIL_TEXT("%-11d%-13.2f\n"), i, MeanFeretDiameter[i]);
         }
      }
   else
      {
      MosPrintf(MIL_TEXT("Edges have not been found or the number of ")
                              MIL_TEXT("found edges is greater than\n"));
      MosPrintf(MIL_TEXT("the specified maximum number of edges !\n\n"));
      }
    
   /* Wait for a key press. */ 
   MosPrintf(MIL_TEXT("\nPress <Enter> to end.\n"));
   MosGetch();

   /* Free MIL objects. */
   MgraFree(GraphicList);
   MbufFree(MilImage);
   MedgeFree(MilEdgeContext);
   MedgeFree(MilEdgeResult);

   /* Free defaults. */    
   MappFreeDefault(MilApplication, MilSystem, MilDisplay, M_NULL, M_NULL);

   return 0;
   }
