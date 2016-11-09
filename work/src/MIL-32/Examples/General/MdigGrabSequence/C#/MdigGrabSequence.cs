///*******************************************************************************
//
// File name: MdigGrabSequence.cs
//
// Synopsis:  This example shows how to grab a sequence, archive it, and play 
//            it back in real time from an AVI file.
//
// NOTE:      This example assumes that the hard disk is sufficiently fast 
//            to keep up with the grab. Also, removing the sequence display or 
//            the text annotation while grabbing will reduce the CPU usage and
//            might help if some frames are missed during acquisition. 
//            If the disk or system are not fast enough, set GRAB_SCALE to 0.5, 
//            FRAME_NUMBER_ANNOTATION to false or SAVE_SEQUENCE_TO_DISK to false.
///*******************************************************************************
using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

using Matrox.MatroxImagingLibrary;

namespace MDigGrabSequence
{
    class Program
    {
        // Sequence file name
        private const string SEQUENCE_FILE = MIL.M_TEMP_DIR + "MilSequence.avi";

        // Image acquisition scale.
        private const double GRAB_SCALE = 1.0;

        // Quantization factor to use during the compression.
        // Valid values are 1 to 99 (higher to lower quality).
        private const int COMPRESSION_Q_FACTOR = 50;

        // Annotation flag. Set to false to draw the frame number in the saved image.
        private static readonly MIL_INT FRAME_NUMBER_ANNOTATION = MIL.M_YES;

        // Archive flag. Set to false to disable AVI Import/Export to disk.
        private static bool SAVE_SEQUENCE_TO_DISK = true;

        // Maximum number of images for the multiple buffering grab.
        private const int NB_GRAB_IMAGE_MAX = 22;

        public class HookDataObject // User's archive function hook data structure.
        {
            public MIL_ID MilSystem;
            public MIL_ID MilDisplay;
            public MIL_ID MilImageDisp;
            public MIL_ID MilCompressedImage;
            public int NbGrabbedFrames;
            public int NbArchivedFrames;
            public bool SaveSequenceToDisk;
        };

        // Main function.
        // --------------
        static void Main(string[] args)
        {
            MIL_ID MilApplication = MIL.M_NULL;
            MIL_ID MilRemoteApplication = MIL.M_NULL;   // Remote Application identifier if running on a remote computer
            MIL_ID MilSystem = MIL.M_NULL;
            MIL_ID MilDigitizer = MIL.M_NULL;
            MIL_ID MilDisplay = MIL.M_NULL;
            MIL_ID MilImageDisp = MIL.M_NULL;
            MIL_ID[] MilGrabImages = new MIL_ID[NB_GRAB_IMAGE_MAX];
            MIL_ID MilCompressedImage = MIL.M_NULL;
            ConsoleKeyInfo Selection = new ConsoleKeyInfo('1', ConsoleKey.D1, false, false, false);

            int NbFrames = 0;
            int n = 0;
            int NbFramesReplayed = 0;
            double FrameRate = 0;
            double TimeWait = 0;
            double TotalReplay = 0;
            double GrabScale = GRAB_SCALE;
            HookDataObject UserHookData = new HookDataObject();
            MIL_INT LicenseModules = 0;
            MIL_INT FrameCount = 0;
            MIL_INT FrameMissed = 0;
            MIL_INT CompressAttribute = 0;

            // Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, ref MilApplication, ref MilSystem, ref MilDisplay, ref MilDigitizer, MIL.M_NULL);

            // Allocate an image and display it.
            MIL.MbufAllocColor(MilSystem,
                                MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_BAND, MIL.M_NULL),
                                (MIL_INT)(MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_X) * GrabScale),
                                (MIL_INT)(MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_Y) * GrabScale),
                                8 + MIL.M_UNSIGNED,
                                MIL.M_IMAGE + MIL.M_GRAB + MIL.M_DISP,
                                ref MilImageDisp);

            MIL.MbufClear(MilImageDisp, 0x0);
            MIL.MdispSelect(MilDisplay, MilImageDisp);

            // Grab continuously on display at the specified scale.
            MIL.MdigControl(MilDigitizer, MIL.M_GRAB_SCALE, GrabScale);
            MIL.MdigGrabContinuous(MilDigitizer, MilImageDisp);

            // Print a message
            Console.WriteLine();
            Console.WriteLine("SEQUENCE ACQUISITION:");
            Console.WriteLine("--------------------");
            Console.WriteLine();

            // Inquire MIL licenses.
            MIL.MsysInquire(MilSystem, MIL.M_OWNER_APPLICATION, ref MilRemoteApplication);
            MIL.MappInquire(MilRemoteApplication, MIL.M_LICENSE_MODULES, ref LicenseModules);

            // If sequence is saved to disk, select between grabbing an 
            // uncompressed, JPEG or JPEG2000 sequence. 
            if (SAVE_SEQUENCE_TO_DISK && ((LicenseModules & (MIL.M_LICENSE_JPEGSTD | MIL.M_LICENSE_JPEG2000)) != 0))
            {
                Console.WriteLine("Choose the sequence format:");
                Console.WriteLine("1) Uncompressed images.");
                if ((LicenseModules & MIL.M_LICENSE_JPEGSTD) != 0)
                {
                    Console.WriteLine("2) Compressed images with a lossy JPEG algorithm.");
                }
                if ((LicenseModules & MIL.M_LICENSE_JPEG2000) != 0)
                {
                    Console.WriteLine("3) Compressed images with a lossy JPEG 2000 algorithm.");
                }
                Selection = Console.ReadKey();
            }
            else
            {
                Console.WriteLine("Press <Enter> to record images.");
                Console.ReadKey();
            }


            // Set the buffer attribute.
            switch (Selection.Key)
            {
                case ConsoleKey.NumPad1:
                case ConsoleKey.D1:
                case ConsoleKey.Enter:
                    Console.WriteLine();
                    Console.WriteLine("Recording uncompressed images...");
                    Console.WriteLine();
                    CompressAttribute = MIL.M_NULL;
                    break;

                case ConsoleKey.NumPad2:
                case ConsoleKey.D2:
                    Console.WriteLine();
                    Console.WriteLine("Recording JPEG images...");
                    Console.WriteLine();
                    CompressAttribute = MIL.M_COMPRESS + MIL.M_JPEG_LOSSY;
                    break;

                case ConsoleKey.NumPad3:
                case ConsoleKey.D3:
                    Console.WriteLine();
                    Console.WriteLine("Recording JPEG 2000 images...");
                    Console.WriteLine();
                    CompressAttribute = MIL.M_COMPRESS + MIL.M_JPEG2000_LOSSY;
                    break;

                default:
                    Console.WriteLine();
                    Console.WriteLine("Invalid selection !.");
                    Console.WriteLine();
                    Console.WriteLine("Using uncompressed images.");
                    Console.WriteLine();
                    CompressAttribute = MIL.M_NULL;
                    while (Console.KeyAvailable)
                    {
                        Console.ReadKey();
                    }
                    break;
            }

            // Allocate a compressed buffer if required.
            if (CompressAttribute != MIL.M_NULL)
            {
                MIL.MbufAllocColor(MilSystem,
                                    MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_BAND, MIL.M_NULL),
                                    (int)(MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_X, MIL.M_NULL) * GrabScale),
                                    (int)(MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_Y, MIL.M_NULL) * GrabScale),
                                    8 + MIL.M_UNSIGNED,
                                    MIL.M_IMAGE + CompressAttribute,
                                    ref MilCompressedImage);

                MIL.MbufControl(MilCompressedImage, MIL.M_Q_FACTOR, COMPRESSION_Q_FACTOR);
            }

            // Allocate the grab buffers to hold the sequence buffering.
            MIL.MappControl(MIL.M_DEFAULT, MIL.M_ERROR, MIL.M_PRINT_DISABLE);
            for (NbFrames = 0, n = 0; n < NB_GRAB_IMAGE_MAX; n++)
            {
                MIL.MbufAllocColor(MilSystem,
                                    MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_BAND, MIL.M_NULL),
                                    (int)(MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_X, MIL.M_NULL) * GrabScale),
                                    (int)(MIL.MdigInquire(MilDigitizer, MIL.M_SIZE_Y, MIL.M_NULL) * GrabScale),
                                    8 + MIL.M_UNSIGNED,
                                    MIL.M_IMAGE + MIL.M_GRAB,
                                    ref MilGrabImages[n]);

                if (MilGrabImages[n] != MIL.M_NULL)
                {
                    NbFrames++;
                    MIL.MbufClear(MilGrabImages[n], 0xFF);
                }
                else
                {
                    break;
                }
            }
            MIL.MappControl(MIL.M_DEFAULT, MIL.M_ERROR, MIL.M_PRINT_ENABLE);

            // Free buffers to leave some space for possible temporary buffers.
            for (n = 0; n < 2 && NbFrames > 0; n++)
            {
                NbFrames--;
                MIL.MbufFree(MilGrabImages[NbFrames]);
            }

            // Halt continuous grab.
            MIL.MdigHalt(MilDigitizer);

            // Open the AVI file if required.
            if (SAVE_SEQUENCE_TO_DISK)
            {
                Console.WriteLine("Saving the sequence to an AVI file...");
                Console.WriteLine();
                MIL.MbufExportSequence(SEQUENCE_FILE, MIL.M_DEFAULT, MIL.M_NULL, MIL.M_NULL, MIL.M_DEFAULT, MIL.M_OPEN);
            }

            // Initialize User's archiving function hook data structure.
            UserHookData.MilSystem = MilSystem;
            UserHookData.MilDisplay = MilDisplay;
            UserHookData.MilImageDisp = MilImageDisp;
            UserHookData.MilCompressedImage = MilCompressedImage;
            UserHookData.SaveSequenceToDisk = SAVE_SEQUENCE_TO_DISK;
            UserHookData.NbGrabbedFrames = 0;
            UserHookData.NbArchivedFrames = 0;

            // get a handle to the DigHookUserData object in the managed heap, we will use this 
            // handle to get the object back in the callback function
            GCHandle UserHookDataHandle = GCHandle.Alloc(UserHookData);
            MIL_DIG_HOOK_FUNCTION_PTR UserHookFunctionDelegate = new MIL_DIG_HOOK_FUNCTION_PTR(ArchiveFunction);

            // Acquire the sequence. The processing hook function will
            // be called for each image grabbed to archive and display it. 
            // If sequence is not saved to disk, stop after NbFrames.
            MIL.MdigProcess(MilDigitizer, MilGrabImages, NbFrames, SAVE_SEQUENCE_TO_DISK ? MIL.M_START : MIL.M_SEQUENCE, MIL.M_DEFAULT, UserHookFunctionDelegate, GCHandle.ToIntPtr(UserHookDataHandle));

            // Wait for a key press.
            Console.WriteLine("Press <Enter> to continue.");
            Console.WriteLine();
            Console.ReadKey(true);

            // Stop the sequence acquisition.
            MIL.MdigProcess(MilDigitizer, MilGrabImages, NbFrames, MIL.M_STOP, MIL.M_DEFAULT, UserHookFunctionDelegate, GCHandle.ToIntPtr(UserHookDataHandle));

            // Free the GCHandle when no longer used
            UserHookDataHandle.Free();

            // Read and print final statistics.
            MIL.MdigInquire(MilDigitizer, MIL.M_PROCESS_FRAME_COUNT, ref FrameCount);
            MIL.MdigInquire(MilDigitizer, MIL.M_PROCESS_FRAME_RATE, ref FrameRate);
            MIL.MdigInquire(MilDigitizer, MIL.M_PROCESS_FRAME_MISSED, ref FrameMissed);

            Console.WriteLine();
            Console.WriteLine();
            Console.WriteLine("{0} frames archived ({1} missed), at {2:0.0} frames/sec ({3:0.0}ms/frame).", UserHookData.NbArchivedFrames, FrameMissed, FrameRate, 1000.0 / FrameRate);

            // Sequence file closing if required.
            if (SAVE_SEQUENCE_TO_DISK)
            {
                MIL.MbufExportSequence(SEQUENCE_FILE, MIL.M_DEFAULT, MIL.M_NULL, MIL.M_NULL, FrameRate, MIL.M_CLOSE);
            }

            // Playback the sequence until a key is pressed.
            if (UserHookData.NbArchivedFrames > 0)
            {
                do
                {
                    // If sequence must be loaded.
                    if (SAVE_SEQUENCE_TO_DISK)
                    {
                        // Inquire information about the sequence.
                        Console.WriteLine();
                        Console.WriteLine("Playing back sequence from the AVI file...");
                        Console.WriteLine();
                        Console.WriteLine("Press <Enter> to end.");
                        Console.WriteLine();
                        Console.WriteLine();
                        MIL.MbufDiskInquire(SEQUENCE_FILE, MIL.M_NUMBER_OF_IMAGES, ref FrameCount);
                        MIL.MbufDiskInquire(SEQUENCE_FILE, MIL.M_FRAME_RATE, ref FrameRate);
                        MIL.MbufDiskInquire(SEQUENCE_FILE, MIL.M_COMPRESSION_TYPE, ref CompressAttribute);

                        // Open the sequence file.
                        MIL.MbufImportSequence(SEQUENCE_FILE, MIL.M_DEFAULT, MIL.M_NULL, MIL.M_NULL, MIL.M_NULL, MIL.M_NULL, MIL.M_NULL, MIL.M_OPEN);
                    }

                    // Copy the images to the screen respecting the sequence frame rate.
                    TotalReplay = 0.0;
                    NbFramesReplayed = 0;
                    for (n = 0; n < FrameCount; n++)
                    {
                        // Reset the time.
                        MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_RESET, MIL.M_NULL);

                        // If image was saved to disk.
                        if (SAVE_SEQUENCE_TO_DISK)
                        {
                            // Load image directly to the display.
                            MIL.MbufImportSequence(SEQUENCE_FILE, MIL.M_DEFAULT, MIL.M_LOAD, MIL.M_NULL, ref MilImageDisp, n, 1, MIL.M_READ);
                            NbFramesReplayed++;
                            Console.Write("Frame #{0}             \r", NbFramesReplayed);
                        }
                        else
                        {
                            // Copy the grabbed image to the display.
                            MIL.MbufCopy(MilGrabImages[n], MilImageDisp);
                            NbFramesReplayed++;
                            Console.Write("Frame #{0}             \r", NbFramesReplayed);
                        }


                        // Check for a pressed key to exit.
                        if (Console.KeyAvailable && (n >= (NB_GRAB_IMAGE_MAX - 1)))
                        {
                            Console.ReadKey(true);
                            break;
                        }

                        // Wait to have a proper frame rate.
                        MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ, ref TimeWait);
                        TotalReplay += TimeWait;
                        TimeWait = (1 / FrameRate) - TimeWait;
                        MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_WAIT, ref TimeWait);
                        TotalReplay += (TimeWait > 0) ? TimeWait : 0.0;
                    }

                    // Close the sequence file.
                    if (SAVE_SEQUENCE_TO_DISK)
                    {
                        MIL.MbufImportSequence(SEQUENCE_FILE, MIL.M_DEFAULT, MIL.M_NULL, MIL.M_NULL, MIL.M_NULL, MIL.M_NULL, MIL.M_NULL, MIL.M_CLOSE);
                    }

                    // Print statistics.
                    Console.WriteLine();
                    Console.WriteLine();
                    Console.WriteLine("{0} frames replayed, at a frame rate of {1:0.0} frames/sec ({2:0.0} ms/frame).", NbFramesReplayed, n / TotalReplay, 1000.0 * TotalReplay / n);
                    Console.WriteLine();
                    Console.WriteLine("Press <Enter> to end (or any other key to playback again).");
                    Console.WriteLine();
                }
                while (Console.ReadKey(true).Key != ConsoleKey.Enter);
            }

            // Free all allocated buffers.
            MIL.MbufFree(MilImageDisp);
            for (n = 0; n < NbFrames; n++)
            {
                MIL.MbufFree(MilGrabImages[n]);
            }

            if (MilCompressedImage != MIL.M_NULL)
            {
                MIL.MbufFree(MilCompressedImage);
            }

            // Free defaults.
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MilDigitizer, MIL.M_NULL);
        }

        // User's archive function called each time a new buffer is grabbed.
        // -------------------------------------------------------------------*/

        // Local defines for the annotations.
        private const int STRING_LENGTH_MAX = 20;
        private const int STRING_POS_X = 20;
        private const int STRING_POS_Y = 20;

        static MIL_INT ArchiveFunction(MIL_INT HookType, MIL_ID HookId, IntPtr HookDataPtr)
        {
            GCHandle HookDataHandle = GCHandle.FromIntPtr(HookDataPtr);
            HookDataObject UserHookDataPtr = HookDataHandle.Target as HookDataObject;
            MIL_ID ModifiedImage = 0;

            // Retrieve the MIL_ID of the grabbed buffer.
            MIL.MdigGetHookInfo(HookId, MIL.M_MODIFIED_BUFFER + MIL.M_BUFFER_ID, ref ModifiedImage);

            // Increment the frame count.
            UserHookDataPtr.NbGrabbedFrames++;

            // Draw the frame count in the image if enabled.
            if (FRAME_NUMBER_ANNOTATION == MIL.M_YES)
            {
                MIL.MgraText(MIL.M_DEFAULT, ModifiedImage, STRING_POS_X, STRING_POS_Y, UserHookDataPtr.NbGrabbedFrames.ToString());
            }

            // Compress the new image.
            if (UserHookDataPtr.MilCompressedImage != MIL.M_NULL)
            {
                MIL.MbufCopy(ModifiedImage, UserHookDataPtr.MilCompressedImage);
            }

            // Archive the new image.
            if (UserHookDataPtr.SaveSequenceToDisk)
            {
                MIL_ID ImageToExport;
                if (UserHookDataPtr.MilCompressedImage != MIL.M_NULL)
                {
                    ImageToExport = UserHookDataPtr.MilCompressedImage;
                }
                else
                {
                    ImageToExport = ModifiedImage;
                }

                MIL.MbufExportSequence(SEQUENCE_FILE, MIL.M_DEFAULT, ref ImageToExport, 1, MIL.M_DEFAULT, MIL.M_WRITE);
                UserHookDataPtr.NbArchivedFrames++;
                Console.Write("Frame #{0}               \r", UserHookDataPtr.NbArchivedFrames);
            }



            // Copy the new grabbed image to the display.
            MIL.MbufCopy(ModifiedImage, UserHookDataPtr.MilImageDisp);

            return 0;
        }
    }
}
