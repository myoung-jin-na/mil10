//*****************************************************************************
//
// File name: MimProcessing.cs
//
// Synopsis:  This program show the usage of image processing. Under MIL lite, 
//            it binarizes the image of a tissue sample to show dark particles.
//            Under MIL full, it also uses different image processing primitives 
//            to determine the number of cell nuclei that are larger than a 
//            certain size and show them in pseudo-color.
//*****************************************************************************
using System;
using System.Collections.Generic;
using System.Text;

using Matrox.MatroxImagingLibrary;

namespace MImProcessing
{
    class Program
    {
        // Target MIL image file specifications.
        private const string IMAGE_FILE = MIL.M_IMAGE_PATH + "Cell.mim";
        private const int IMAGE_SMALL_PARTICLE_RADIUS = 1;

        static void Main(string[] args)
        {
            MIL_ID MilApplication = MIL.M_NULL;         // Application identifier.
            MIL_ID MilRemoteApplication = MIL.M_NULL;   // Remote Application identifier if running on a remote computer
            MIL_ID MilSystem = MIL.M_NULL;              // System identifier.
            MIL_ID MilDisplay = MIL.M_NULL;             // Display identifier.
            MIL_ID MilImage = MIL.M_NULL;               // Image buffer identifier.
            MIL_ID MilExtremeResult = 0;                // Extreme result buffer identifier.
            int MaxLabelNumber = 0;                     // Highest label value.
            MIL_INT LicenseModules = 0;                     // List of available MIL modules.


            // Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, ref MilApplication, ref MilSystem, ref MilDisplay, MIL.M_NULL, MIL.M_NULL);

            // Restore source image and display it.
            MIL.MbufRestore(IMAGE_FILE, MilSystem, ref MilImage);
            MIL.MdispSelect(MilDisplay, MilImage);

            // Pause to show the original image.
            Console.Write("\nIMAGE PROCESSING:\n");
            Console.Write("-----------------\n\n");
            Console.Write("This program extracts the dark particles in the image.\n");
            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();

            // Binarize the image with an automatically calculated threshold so that 
            // particles are represented in white and the background removed.
            MIL.MimBinarize(MilImage, MilImage, MIL.M_BIMODAL + MIL.M_LESS_OR_EQUAL, MIL.M_NULL, MIL.M_NULL);

            // Print a message for the extracted particles.
            Console.Write("These particles were extracted from the original image.\n");

            // If MIL IM module is available, count and label the larger particles.
            MIL.MsysInquire(MilSystem, MIL.M_OWNER_APPLICATION, ref MilRemoteApplication);
            MIL.MappInquire(MilRemoteApplication, MIL.M_LICENSE_MODULES, ref LicenseModules);

            if ((LicenseModules & MIL.M_LICENSE_IM) != 0)
            {
                // Pause to show the extracted particles.
                Console.Write("Press <Enter> to continue.\n\n");
                Console.ReadKey();

                // Close small holes.
                MIL.MimClose(MilImage, MilImage, IMAGE_SMALL_PARTICLE_RADIUS, MIL.M_BINARY);

                // Remove small particles.
                MIL.MimOpen(MilImage, MilImage, IMAGE_SMALL_PARTICLE_RADIUS, MIL.M_BINARY);

                // Label the image.
                MIL.MimLabel(MilImage, MilImage, MIL.M_DEFAULT);

                // The largest label value corresponds to the number of particles in the image.
                MIL.MimAllocResult(MilSystem, 1, MIL.M_EXTREME_LIST, ref MilExtremeResult);
                MIL.MimFindExtreme(MilImage, MilExtremeResult, MIL.M_MAX_VALUE);
                MIL.MimGetResult(MilExtremeResult, MIL.M_VALUE, ref MaxLabelNumber);
                MIL.MimFree(MilExtremeResult);

                // Multiply the labeling result to augment the gray level of the particles.
                MIL.MimArith(MilImage, (int)(256 / (double)MaxLabelNumber), MilImage, MIL.M_MULT_CONST);

                // Display the resulting particles in pseudo-color.
                MIL.MdispLut(MilDisplay, MIL.M_PSEUDO);

                // Print results.
                Console.Write("There were {0} large particles in the original image.\n", MaxLabelNumber);
            }

            // Pause to show the result.
            Console.Write("Press <Enter> to end.\n\n");
            Console.ReadKey();

            // Free all allocations.
            MIL.MbufFree(MilImage);
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL);
        }
    }
}
