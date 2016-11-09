/*****************************************************************************/
/* 
 * File name: MImProcessing.cpp
 *
 * Synopsis:  This program show the usage of image processing. Under MIL lite, 
 *            it binarizes the image of a tissue sample to show dark particles.
 *            Under MIL full, it also uses different image processing primitives 
 *            to determine the number of cell nuclei that are larger than a 
 *            certain size and show them in pseudo-color.
 */
#include <mil.h>

/* Target MIL image file specifications. */
#define IMAGE_FILE                     M_IMAGE_PATH MIL_TEXT("Cell.mim")
#define IMAGE_SMALL_PARTICLE_RADIUS    1


int MosMain(void)
{
   MIL_ID   MilApplication,       /* Application identifier.  */
            MilRemoteApplication, /* Remote application identifier.  */
            MilSystem,            /* System identifier.       */
            MilDisplay,           /* Display identifier.      */
            MilImage,             /* Image buffer identifier. */
            MilExtremeResult = 0; /* Extreme result buffer identifier. */
   MIL_INT  MaxLabelNumber = 0;   /* Highest label value. */
   MIL_INT  LicenseModules = 0;   /* List of available MIL modules */

   /* Allocate defaults. */
   MappAllocDefault(M_DEFAULT, &MilApplication, &MilSystem, 
                             &MilDisplay, M_NULL, M_NULL);

   /* Restore source image and display it. */
   MbufRestore(IMAGE_FILE, MilSystem, &MilImage);
   MdispSelect(MilDisplay, MilImage);
   
   /* Pause to show the original image. */
   MosPrintf(MIL_TEXT("\nIMAGE PROCESSING:\n"));
   MosPrintf(MIL_TEXT("-----------------\n\n"));
   MosPrintf(MIL_TEXT("This program extracts the dark particles in the image.\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   /* Binarize the image with an automatically calculated threshold so that 
      particles are represented in white and the background removed. 
    */
   MimBinarize(MilImage, MilImage, M_BIMODAL+M_LESS_OR_EQUAL, M_NULL, M_NULL);
 
   /* Print a message for the extracted particles. */
   MosPrintf(MIL_TEXT("These particles were extracted from the original image.\n"));

   #if (!M_MIL_LITE)
      /* If MIL IM module is available, count and label the larger particles. */
      MsysInquire(MilSystem, M_OWNER_APPLICATION, &MilRemoteApplication);
      MappInquire(MilRemoteApplication, M_LICENSE_MODULES, &LicenseModules);
      if (LicenseModules & M_LICENSE_IM)
        {
        /* Pause to show the extracted particles. */
        MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
        MosGetch();
        
        /* Close small holes. */
        MimClose(MilImage, MilImage, IMAGE_SMALL_PARTICLE_RADIUS, M_BINARY);

        /* Remove small particles. */
        MimOpen(MilImage, MilImage, IMAGE_SMALL_PARTICLE_RADIUS, M_BINARY);
        
        /* Label the image. */
        MimLabel(MilImage, MilImage, M_DEFAULT);

        /*The largest label value corresponds to the number of particles in the image.*/
        MimAllocResult(MilSystem, 1L, M_EXTREME_LIST, &MilExtremeResult);
        MimFindExtreme(MilImage, MilExtremeResult, M_MAX_VALUE);
        MimGetResult(MilExtremeResult, M_VALUE, &MaxLabelNumber);
        MimFree(MilExtremeResult);

        /* Multiply the labeling result to augment the gray level of the particles. */
        MimArith(MilImage,(MIL_INT)(256L/(MIL_DOUBLE)MaxLabelNumber), MilImage, 
                                                                      M_MULT_CONST);

        /* Display the resulting particles in pseudo-color. */
        MdispLut(MilDisplay, M_PSEUDO);

        /* Print results. */
        MosPrintf(MIL_TEXT("There were %ld large particles in the original image.\n"), 
                                                                        MaxLabelNumber);
        }
   #endif

   /* Pause to show the result. */
   MosPrintf(MIL_TEXT("Press <Enter> to end.\n\n"));
   MosGetch();

   /* Free all allocations. */
   MbufFree(MilImage);
   MappFreeDefault(MilApplication, MilSystem, MilDisplay, M_NULL, M_NULL);

   return 0;
}

