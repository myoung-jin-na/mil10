//*****************************************************************************
// 
// File name: MgraInteractive.cs
//
// Synopsis:  This program uses the capabilities of MIL's interactive
//            graphics, along with the Blob analysis module, to count 
//            objects within a user-defined region.
//*****************************************************************************
using System;
using System.Collections.Generic;
using System.Text;

using Matrox.MatroxImagingLibrary;
using System.Runtime.InteropServices;

namespace MGraInteractive
{
    public class STestParameters
    {
        public MIL_ID MilDisplay;
        public MIL_ID MilGraphicsList;
        public MIL_ID MilGraphicsContext;
        public MIL_ID MilBinImage;
        public MIL_ID MilBlobFeatureList;
        public MIL_ID MilBlobResult;
        public MIL_INT RegionLabel;
    }

    public class Program
    {
        static void Main(string[] args)
        {
            MIL_ID MilApplication = MIL.M_NULL;     // Application identifier.
            MIL_ID MilSystem = MIL.M_NULL;          // System Identifier.
            MIL_ID MilDisplay = MIL.M_NULL;         // Display identifier.

            // Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, ref MilApplication, ref MilSystem, ref MilDisplay, MIL.M_NULL, MIL.M_NULL);

            // Print example name.
            Console.WriteLine();
            Console.WriteLine("INTERACTIVE REGIONS AND SUBPIXEL ANNOTATIONS:");
            Console.WriteLine("---------------------------------------------");
            Console.WriteLine();
            Console.WriteLine("This program determines the number of blobs in a region");
            Console.WriteLine("defined interactively by the user. The extracted blob's");
            Console.WriteLine("features are drawn with subpixel accuracy in a zoomable");
            Console.WriteLine("display.");
            Console.WriteLine();

            // Run Interactivity Example.
            InteractivityExample(MilSystem, MilDisplay);

            // Free defaults.
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL);
        }

        //*****************************************************************************
        // Interactivity example.

        // Source MIL image file specification.
        private static readonly string IMAGE_FILE = MIL.M_IMAGE_PATH + "Seals.mim";

        // Threshold for image binarization.
        private const int IMAGE_THRESHOLD_VALUE = 110;

        // Initial region parameters.
        private const int RECTANGLE_POSITION_X = 160;
        private const int RECTANGLE_POSITION_Y = 310;
        private const int RECTANGLE_WIDTH = 200;
        private const int RECTANGLE_HEIGHT = 175;
        private const int RECTANGLE_ANGLE = 0;

        private static void InteractivityExample(MIL_ID MilSystem, MIL_ID MilDisplay)
        {
            MIL_ID MilImage = MIL.M_NULL;                           // Image buffer identifier.
            MIL_ID MilGraphicsList = MIL.M_NULL;                    // Graphics list identifier.
            MIL_ID MilGraphicsContext = MIL.M_NULL;                 // Graphics context identifier.
            MIL_ID MilBinImage = MIL.M_NULL;                        // Binary image buffer identifier.
            MIL_ID MilBlobFeatureList = MIL.M_NULL;                 // Feature list identifier.
            MIL_ID MilBlobResult = MIL.M_NULL;                      // Blob result buffer identifier.

            MIL_INT SizeX = 0;                                      // Size X of the source buffer.
            MIL_INT SizeY = 0;                                      // Size Y of the source buffer.
            MIL_INT RegionLabel = 0;                                // Label value of the region.

            STestParameters DataStructure = new STestParameters();  // Hook function data structure.

            // Restore the source image.
            MIL.MbufRestore(IMAGE_FILE, MilSystem, ref MilImage);

            // Display the buffer.
            MIL.MdispSelect(MilDisplay, MilImage);

            // Allocate a graphics list to hold the subpixel annotations.
            MIL.MgraAllocList(MilSystem, MIL.M_DEFAULT, ref MilGraphicsList);

            // Associate the graphics list to the display for annotations.
            MIL.MdispControl(MilDisplay, MIL.M_ASSOCIATED_GRAPHIC_LIST_ID, MilGraphicsList);

            // Allocate a graphics context for the draw operations.
            MIL.MgraAlloc(MilSystem, ref MilGraphicsContext);

            // Enable the interactive mode.
            MIL.MdispControl(MilDisplay, MIL.M_GRAPHIC_LIST_INTERACTIVE, MIL.M_ENABLE);

            // Add a selectable rectangular region.
            MIL.MgraRectAngle(MilGraphicsContext, MilGraphicsList, RECTANGLE_POSITION_X, RECTANGLE_POSITION_Y, RECTANGLE_WIDTH, RECTANGLE_HEIGHT, RECTANGLE_ANGLE, MIL.M_CENTER_AND_DIMENSION);

            // Retrieve the label of the rectangle graphic.
            MIL.MgraInquireList(MilGraphicsList, MIL.M_LIST, MIL.M_DEFAULT, MIL.M_LAST_LABEL, ref RegionLabel);

            // Disable the selectable mode for the next annotations to the graphics list.
            MIL.MgraControl(MilGraphicsContext, MIL.M_SELECTABLE, MIL.M_DISABLE);

            // Allocate a binary image buffer for fast processing.
            MIL.MbufInquire(MilImage, MIL.M_SIZE_X, ref SizeX);
            MIL.MbufInquire(MilImage, MIL.M_SIZE_Y, ref SizeY);
            MIL.MbufAlloc2d(MilSystem, SizeX, SizeY, 1 + MIL.M_UNSIGNED, MIL.M_IMAGE + MIL.M_PROC, ref MilBinImage);

            // Binarize the source image.
            MIL.MimBinarize(MilImage, MilBinImage, MIL.M_FIXED + MIL.M_LESS, IMAGE_THRESHOLD_VALUE, MIL.M_NULL);

            // Allocate a blob feature list and a blob result.
            MIL.MblobAllocFeatureList(MilSystem, ref MilBlobFeatureList);
            MIL.MblobAllocResult(MilSystem, ref MilBlobResult);

            // Select the blob features to calculate (Center Of Gravity and Box).
            MIL.MblobSelectFeature(MilBlobFeatureList, MIL.M_CENTER_OF_GRAVITY);
            MIL.MblobSelectFeature(MilBlobFeatureList, MIL.M_BOX);

            // Programmatically initialize the selected state of the rectangle region.
            MIL.MgraControlList(MilGraphicsList, MIL.M_GRAPHIC_LABEL(RegionLabel), MIL.M_DEFAULT, MIL.M_GRAPHIC_SELECTED, MIL.M_TRUE);

            // Perform and display a first count of the number of objects
            // within the initial region.                                
            CountObjects(MilDisplay, MilGraphicsList, MilGraphicsContext, MilBinImage, MilBlobFeatureList, MilBlobResult);

            // Initialize the hook data structure, then associate the hook function to
            // the "MIL.M_GRAPHIC_MODIFIED" event. The hook function will be called
            // with any region interaction by the user.
            DataStructure.MilDisplay = MilDisplay;
            DataStructure.MilGraphicsList = MilGraphicsList;
            DataStructure.MilGraphicsContext = MilGraphicsContext;
            DataStructure.MilBinImage = MilBinImage;
            DataStructure.RegionLabel = RegionLabel;
            DataStructure.MilBlobFeatureList = MilBlobFeatureList;
            DataStructure.MilBlobResult = MilBlobResult;

            GCHandle DataStructureHandle = GCHandle.Alloc(DataStructure);
            MIL_GRA_HOOK_FUNCTION_PTR HookHandlerDelegate = new MIL_GRA_HOOK_FUNCTION_PTR(HookHandler);
            MIL.MgraHookFunction(MilGraphicsList, MIL.M_GRAPHIC_MODIFIED, HookHandlerDelegate, GCHandle.ToIntPtr(DataStructureHandle));

            if (MIL.MdispInquire(MilDisplay, MIL.M_DISPLAY_TYPE, MIL.M_NULL) != MIL.M_AUXILIARY)
            {
                Console.WriteLine("You can try using your mouse to interactively modify the");
                Console.WriteLine("displayed region, such as moving, resizing, or rotating the");
                Console.WriteLine("region. If you do so, the results and annotations will be");
                Console.WriteLine("immediately updated\n");
                Console.WriteLine();
            }
            else
            {
                Console.WriteLine("***NOTE: This example does not implement the interactivity");
                Console.WriteLine("         with an auxiliary display.");
                Console.WriteLine();
            }
            Console.WriteLine("Press <Enter> to exit.");
            Console.ReadKey();

            MIL.MgraHookFunction(MilGraphicsList, MIL.M_GRAPHIC_MODIFIED + MIL.M_UNHOOK, HookHandlerDelegate, GCHandle.ToIntPtr(DataStructureHandle));
            DataStructureHandle.Free();

            // Free all allocations.
            MIL.MblobFree(MilBlobResult);
            MIL.MblobFree(MilBlobFeatureList);
            MIL.MbufFree(MilBinImage);
            MIL.MgraFree(MilGraphicsContext);
            MIL.MgraFree(MilGraphicsList);
            MIL.MbufFree(MilImage);
        }

        private static MIL_INT HookHandler(MIL_INT HookType, MIL_ID EventId, IntPtr UserDataPtr)
        {
            // this is how to check if the user data is null, the IntPtr class
            // contains a member, Zero, which exists solely for this purpose
            if (!IntPtr.Zero.Equals(UserDataPtr))
            {
                // get the handle to the DigHookUserData object back from the IntPtr
                GCHandle hUserData = GCHandle.FromIntPtr(UserDataPtr);

                // get a reference to the DigHookUserData object
                STestParameters DataStructure = hUserData.Target as STestParameters;

                // Check that the modified graphic is the rectangular region.
                MIL_INT ModifiedGraphicLabel = 0;
                MIL.MgraGetHookInfo(EventId, MIL.M_GRAPHIC_LABEL_VALUE, ref ModifiedGraphicLabel);

                if (ModifiedGraphicLabel == DataStructure.RegionLabel)
                {

                    // Count objects and draw the corresponding annotations.
                    CountObjects(DataStructure.MilDisplay,
                                 DataStructure.MilGraphicsList,
                                 DataStructure.MilGraphicsContext,
                                 DataStructure.MilBinImage,
                                 DataStructure.MilBlobFeatureList,
                                 DataStructure.MilBlobResult);
                }
            }

            return MIL.M_NULL;
        }


        private const int MAX_TEXT_SIZE = 100;

        private static void CountObjects(MIL_ID MilDisplay, MIL_ID MilGraphicsList, MIL_ID MilGraphicsContext, MIL_ID MilBinImage, MIL_ID MilBlobFeatureList, MIL_ID MilBlobResult)
        {
            MIL_INT NumberOfBlobs = 0;
            MIL_INT NumberOfPrimitives = 0;
            MIL_INT Index;

            string TextLabel;

            // Disable the display update for better performance.
            MIL.MdispControl(MilDisplay, MIL.M_UPDATE, MIL.M_DISABLE);

            // Remove all elements from the graphics list, except the rectangle
            // region primitive at index 0.
            MIL.MgraInquireList(MilGraphicsList, MIL.M_LIST, MIL.M_DEFAULT, MIL.M_NUMBER_OF_GRAPHICS, ref NumberOfPrimitives);
            for (Index = NumberOfPrimitives-1; Index > 0; Index--)
            {
                MIL.MgraControlList(MilGraphicsList, MIL.M_GRAPHIC_INDEX(Index), MIL.M_DEFAULT, MIL.M_DELETE, MIL.M_DEFAULT);
            }

            // Set the input region. The blob analysis will be done
            // from the (filled) interactive rectangle.           
            MIL.MbufSetRegion(MilBinImage, MilGraphicsList, MIL.M_DEFAULT, MIL.M_RASTERIZE + MIL.M_FILL_REGION, MIL.M_DEFAULT);

            // Calculate the blobs and their features.
            MIL.MblobCalculate(MilBinImage, MIL.M_NULL, MilBlobFeatureList, MilBlobResult);

            // Get the total number of blobs.
            MIL.MblobGetNumber(MilBlobResult, ref NumberOfBlobs);

            // Set the input units to display unit for the count annotations.
            MIL.MgraControl(MilGraphicsContext, MIL.M_INPUT_UNITS, MIL.M_DISPLAY);
            TextLabel = string.Format(" Number of blobs found: {0:00} ", NumberOfBlobs);

            MIL.MgraColor(MilGraphicsContext, MIL.M_COLOR_WHITE);
            MIL.MgraText(MilGraphicsContext, MilGraphicsList, 10, 10, TextLabel);

            // Restore the input units to pixel units for result annotations.
            MIL.MgraControl(MilGraphicsContext, MIL.M_INPUT_UNITS, MIL.M_PIXEL);

            // Draw blob center of gravity annotation.
            MIL.MgraColor(MilGraphicsContext, MIL.M_COLOR_RED);
            MIL.MblobDraw(MilGraphicsContext, MilBlobResult, MilGraphicsList, MIL.M_DRAW_CENTER_OF_GRAVITY, MIL.M_INCLUDED_BLOBS, MIL.M_DEFAULT);

            // Draw blob bounding box annotations.
            MIL.MgraColor(MilGraphicsContext, MIL.M_COLOR_GREEN);
            MIL.MblobDraw(MilGraphicsContext, MilBlobResult, MilGraphicsList, MIL.M_DRAW_BOX, MIL.M_INCLUDED_BLOBS, MIL.M_DEFAULT);

            // Enable the display to update the drawings.
            MIL.MdispControl(MilDisplay, MIL.M_UPDATE, MIL.M_ENABLE);
        }
    }
}
