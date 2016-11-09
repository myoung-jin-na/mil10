//*****************************************************************************
//
// File name: MimDeinterlace.cs 
//
// Synopsis:  : This program performs deinterlacing on an image. 
//
//              First, a static image is deinterlaced using the Discard
//              algorithm. Then, if a digitizer is present, the grabbed 
//              images are deinterlaced live using the same algorithms.
//*****************************************************************************
using System;
using System.Collections.Generic;
using System.Text;

using Matrox.MatroxImagingLibrary;

namespace MImDeinterlace
{
    class Program
    {
        private const string IMAGE_FILE = MIL.M_IMAGE_PATH + "/Preprocessing/InterlacedBird.mim";

        private const int MOTION_DETECT_NUM_FRAMES = 3;
        private const int MOTION_DETECT_THRESHOLD = 20;

        private const int NUM_GRAB_IMAGES = (MOTION_DETECT_NUM_FRAMES + 1);

        private static int GRAB_DEINTERLACE_METHOD = MIL.M_DISCARD;


        // Main function.
        // --------------
        static void Main(string[] args)
        {
            MIL_ID MilApplication = MIL.M_NULL;        /* Application identifier.         */
            MIL_ID MilSystem = MIL.M_NULL;             /* System identifier.              */
            MIL_ID MilDisplay = MIL.M_NULL;            /* Display identifier.             */
            MIL_ID MilDisplayImage = MIL.M_NULL;       /* Image buffer identifier.        */
            MIL_ID MilSourceImage = MIL.M_NULL;        /* Image buffer identifier.        */
            MIL_ID MilDeinterlaceContext = MIL.M_NULL; /* Deinterlace context identifier. */
            MIL_INT ImageSizeBand = 0;
            MIL_INT ImageWidth = 0;
            MIL_INT ImageHeight = 0;
            MIL_INT ImageType = 0;

            /* Allocate defaults. */
            MIL.MappAllocDefault(MIL.M_DEFAULT, ref MilApplication, ref MilSystem, ref MilDisplay, MIL.M_NULL, MIL.M_NULL);

            /* Restore the source image. */
            MIL.MbufRestore(IMAGE_FILE, MilSystem, ref MilSourceImage);

            /* Allocate a display buffers and show the source image. */
            MIL.MbufInquire(MilSourceImage, MIL.M_SIZE_BAND, ref ImageSizeBand);
            MIL.MbufInquire(MilSourceImage, MIL.M_SIZE_X, ref ImageWidth);
            MIL.MbufInquire(MilSourceImage, MIL.M_SIZE_Y, ref ImageHeight);
            MIL.MbufInquire(MilSourceImage, MIL.M_TYPE, ref ImageType);
            MIL.MbufAllocColor(MilSystem, ImageSizeBand, ImageWidth, ImageHeight, ImageType, MIL.M_IMAGE + MIL.M_PROC + MIL.M_DISP, ref MilDisplayImage);
            MIL.MbufCopy(MilSourceImage, MilDisplayImage);
            MIL.MdispSelect(MilDisplay, MilDisplayImage);

            /* Allocate a deinterlacing context. */
            MIL.MimAlloc(MilSystem, MIL.M_DEINTERLACE_CONTEXT, MIL.M_DEFAULT, ref MilDeinterlaceContext);

            /* Print a message. */
            Console.Write("\nDEINTERLACING:\n");
            Console.Write("----------------\n\n");
            Console.Write("This image has been grabbed using an interlaced camera.\n");
            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();

            /* Deinterlace using MIL.M_DISCARD. */
            MIL.MimControl(MilDeinterlaceContext, MIL.M_DEINTERLACE_TYPE, MIL.M_DISCARD);
            MIL.MimDeinterlace(MilDeinterlaceContext, ref MilSourceImage, ref MilDisplayImage, 1, 1, MIL.M_DEFAULT);

            /* Display message. */
            Console.Write("The image has been deinterlaced using the DISCARD algorithm.\n");
            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();

            /* Free objects. */
            MIL.MbufFree(MilSourceImage);
            MIL.MbufFree(MilDisplayImage);
            MIL.MdispFree(MilDisplay);
            MIL.MimFree(MilDeinterlaceContext);

            /* If the system has a digitizer, use it to do live deinterlacing. */
            if (MIL.MsysInquire(MilSystem, MIL.M_DIGITIZER_NUM, MIL.M_NULL) > 0)
            {
                GrabDeinterlace(MilSystem);
            }

            /* Free objects. */
            MIL.MappFreeDefault(MilApplication, MilSystem, MIL.M_NULL, MIL.M_NULL, MIL.M_NULL);

        }

        static void GrabDeinterlace(MIL_ID MilSystem)
        {
            MIL_ID MilDigitizer = MIL.M_NULL;          /* Digitizer identifier.           */
            MIL_ID MilDisplay = MIL.M_NULL;            /* Display identifier.             */
            MIL_ID MilDisplayImage = MIL.M_NULL;       /* Image buffer identifier.        */
            MIL_ID MilDeinterlaceContext = MIL.M_NULL; /* Deinterlace context identifier. */
            MIL_ID[] MilGrabImages = new MIL_ID[NUM_GRAB_IMAGES];
            MIL_ID[] MilPreviousImages = new MIL_ID[MOTION_DETECT_NUM_FRAMES];
            MIL_INT ImageSizeBand = 0;
            MIL_INT ImageWidth = 0;
            MIL_INT ImageHeight = 0; ;
            int i = 0;
            int CurrentGrabIndex = 0;
            int NextGrabIndex = 0;

            /* Try to allocate a digitizer. */
            MIL.MdigAlloc(MilSystem, MIL.M_DEFAULT, "M_DEFAULT", MIL.M_DEFAULT, ref MilDigitizer);
            if (MilDigitizer == MIL.M_NULL)
                return;

            /* Inquire characteristics of the digitizer. */
            MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_BAND, ref ImageSizeBand);
            MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_X, ref ImageWidth);
            MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_Y, ref ImageHeight);

            /* Allocate grab images. */
            for (i = 0; i < NUM_GRAB_IMAGES; i++)
            {
                MIL.MbufAllocColor(MilSystem,
                               ImageSizeBand,
                               ImageWidth,
                               ImageHeight,
                               8 + MIL.M_UNSIGNED,
                               MIL.M_IMAGE + MIL.M_GRAB + MIL.M_PROC,
                               ref MilGrabImages[i]);
            }

            /* Allocate a deinterlacing context. */
            MIL.MimAlloc(MilSystem, MIL.M_DEINTERLACE_CONTEXT, MIL.M_DEFAULT, ref MilDeinterlaceContext);

            /* Allocate new display and display image. */
            MIL.MbufAllocColor(MilSystem,
                           ImageSizeBand,
                           ImageWidth,
                           ImageHeight,
                           8 + MIL.M_UNSIGNED,
                           MIL.M_IMAGE + MIL.M_GRAB + MIL.M_DISP + MIL.M_PROC,
                           ref MilDisplayImage);
            MIL.MdispAlloc(MilSystem,
                       MIL.M_DEFAULT,
                       "M_DEFAULT",
                       MIL.M_DEFAULT,
                       ref MilDisplay);
            MIL.MbufClear(MilDisplayImage, 0);
            MIL.MdispSelect(MilDisplay, MilDisplayImage);

            /* Print a message. */
            Console.Write("Normal live grab is displayed.\n");
            Console.Write("Press <Enter> to continue.\n\n");

            /* Grab continuously on display and wait for a key press. */
            MIL.MdigGrabContinuous(MilDigitizer, MilDisplayImage);
            Console.ReadKey();

            /* Halt continuous grab. */
            MIL.MdigHalt(MilDigitizer);

            /* Configure the Deinterlace context for the appropriate live deinterlacing method. */
            MIL.MimControl(MilDeinterlaceContext, MIL.M_DEINTERLACE_TYPE, GRAB_DEINTERLACE_METHOD);
            if (GRAB_DEINTERLACE_METHOD == MIL.M_ADAPTIVE_DISCARD || GRAB_DEINTERLACE_METHOD == MIL.M_ADAPTIVE_AVERAGE)
            {
                MIL.MimControl(MilDeinterlaceContext, MIL.M_MOTION_DETECT_NUM_FRAMES, MOTION_DETECT_NUM_FRAMES);
                MIL.MimControl(MilDeinterlaceContext, MIL.M_MOTION_DETECT_THRESHOLD, MOTION_DETECT_THRESHOLD);
            }

            /* Set the digitizer to asynchronous grab. */
            MIL.MdigControl(MilDigitizer, MIL.M_GRAB_MODE, MIL.M_ASYNCHRONOUS);

            /* Send the first grab to the digitizer. */
            MIL.MdigGrab(MilDigitizer, MilGrabImages[0]);
            CurrentGrabIndex = 0;
            NextGrabIndex = 1;

            /* Print a message. */
            Console.Write("Deinterlaced live grab is displayed.\n");
            Console.Write("Press <Enter> to stop.\n\n");

            /* Grab and process. */
            while (!Console.KeyAvailable)
            {
                /* Send the next grab command to the digitizer. This will block until the current grab is finished. */
                MIL.MdigGrab(MilDigitizer, MilGrabImages[NextGrabIndex]);

                if ((GRAB_DEINTERLACE_METHOD != MIL.M_ADAPTIVE_DISCARD) && (GRAB_DEINTERLACE_METHOD != MIL.M_ADAPTIVE_AVERAGE))
                {
                    /* Deinterlace the current grabbed image. */
                    MIL.MimDeinterlace(MilDeinterlaceContext, ref MilGrabImages[CurrentGrabIndex], ref MilDisplayImage, 1, 1, MIL.M_DEFAULT);
                }
                else
                {
                    /* Build the table of the last MOTION_DETECT_NUM_FRAMES grabbed frames. */
                    for (i = 0; i < MOTION_DETECT_NUM_FRAMES; i++)
                    {
                        int PreviousGrabIndex = NextGrabIndex - MOTION_DETECT_NUM_FRAMES + i;
                        if (PreviousGrabIndex < 0)
                            PreviousGrabIndex += NUM_GRAB_IMAGES;
                        MilPreviousImages[i] = MilGrabImages[PreviousGrabIndex];
                    }

                    /* Deinterlace the previous grabbed images. */
                    MIL.MimDeinterlace(MilDeinterlaceContext, MilPreviousImages, ref MilDisplayImage, MOTION_DETECT_NUM_FRAMES, 1, MIL.M_DEFAULT);
                }

                /* Increment grab index. */
                CurrentGrabIndex = NextGrabIndex;
                NextGrabIndex = (NextGrabIndex + 1) % NUM_GRAB_IMAGES;
            }
            Console.ReadKey();

            /* Display the last grabbed image. */
            Console.Write("Last deinterlaced image displayed.\n");
            Console.Write("Press <Enter> to end.\n\n");
            Console.ReadKey();

            /* Free objects. */
            MIL.MimFree(MilDeinterlaceContext);
            MIL.MbufFree(MilDisplayImage);
            MIL.MdispFree(MilDisplay);
            for (i = 0; i < NUM_GRAB_IMAGES; i++)
            {
                MIL.MbufFree(MilGrabImages[i]);
            }
            MIL.MdigFree(MilDigitizer);
        }
    }
}
