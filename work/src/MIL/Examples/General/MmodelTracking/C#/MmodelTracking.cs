//*********************************************************************************
//
// File name: MmodelTracking.cs
//
// Synopsis:  This program shows how to track a unique object using 
//            pattern recognition. It allocates a model in the field of 
//            view of the camera and finds it in a loop. It also prints 
//            the coordinates of the found model and draws a box around it.
//            It searches using 2 methods, the normalized grayscale 
//            correlation (MIL.Mpat), which is very fast and with the Geometric 
//            Model Finder (MIL.Mmod), which is independent of the model rotation 
//            and scale but slower.
//
//            Note:  Display update and annotations drawing can require
//                   significant CPU usage.
//*********************************************************************************
using System;
using System.Collections.Generic;
using System.Text;

using Matrox.MatroxImagingLibrary;

namespace MModelTracking
{
    class Program
    {
        // Model specification.
        private const int MODEL_WIDTH = 128;
        private const int MODEL_HEIGHT = 128;

        static MIL_INT MODEL_POS_X_INIT(MIL_ID TargetImage) { return (MIL.MbufInquire(TargetImage, MIL.M_SIZE_X, MIL.M_NULL) / 2); }
        static MIL_INT MODEL_POS_Y_INIT(MIL_ID TargetImage) { return (MIL.MbufInquire(TargetImage, MIL.M_SIZE_Y, MIL.M_NULL) / 2); }

        // Minimum score to consider the object found (in percent).
        private const double MODEL_MIN_MATCH_SCORE = 50.0;

        // Drawing color
        private const int DRAW_COLOR = 0xFF; // White

        // Example selection.
        private const int RUN_PAT_TRACKING_EXAMPLE = MIL.M_YES;
        private const int RUN_MOD_TRACKING_EXAMPLE = MIL.M_YES;

        //****************************************************************************
        // Main.
        //****************************************************************************
        static void Main(string[] args)
        {
            MIL_ID MilApplication = MIL.M_NULL;   // Application identifier.
            MIL_ID MilSystem = MIL.M_NULL;        // System identifier.
            MIL_ID MilDisplay = MIL.M_NULL;       // Display identifier.
            MIL_ID MilDigitizer = MIL.M_NULL;     // Digitizer identifier.
            MIL_ID MilDisplayImage = MIL.M_NULL;  // Display image identifier.
            MIL_ID MilModelImage = MIL.M_NULL;    // Model image identifier.

            // Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, ref MilApplication, ref MilSystem, ref MilDisplay, ref MilDigitizer, ref MilDisplayImage);

            // Allocate a model image buffer.
            MIL.MbufAlloc2d(MilSystem, MIL.MbufInquire(MilDisplayImage, MIL.M_SIZE_X, MIL.M_NULL), MIL.MbufInquire(MilDisplayImage, MIL.M_SIZE_Y, MIL.M_NULL), 8, MIL.M_IMAGE + MIL.M_PROC, ref MilModelImage);

            Console.Write("\nMODEL TRACKING:\n");
            Console.Write("---------------\n\n");

            // Get the model image.
            GetModelImage(MilSystem, MilDisplay, MilDigitizer, MilDisplayImage, MilModelImage);

            if (RUN_PAT_TRACKING_EXAMPLE == MIL.M_YES)
            {
                // Finds the model using pattern matching.
                MpatTrackingExample(MilSystem, MilDisplay, MilDigitizer, MilDisplayImage, MilModelImage);
            }

            if (RUN_MOD_TRACKING_EXAMPLE == MIL.M_YES)
            {
                // Finds the model using geometric model finder.
                MmodTrackingExample(MilSystem, MilDisplay, MilDigitizer, MilDisplayImage, MilModelImage);
            }

            // Free allocated buffers.
            MIL.MbufFree(MilModelImage);

            // Free defaults.
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MilDigitizer, MilDisplayImage);
        }

        //****************************************************************************
        // Get Model Image Function.
        //****************************************************************************
        static void GetModelImage(MIL_ID MilSystem, MIL_ID MilDisplay, MIL_ID MilDigitizer, MIL_ID MilDisplayImage, MIL_ID MilModelImage)
        {
            MIL_ID MilOverlayImage = MIL.M_NULL;         // Overlay image.
            double DrawColor = DRAW_COLOR;              // Drawing color.

            // Prepare for overlay annotations.
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY, MIL.M_ENABLE);
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY_CLEAR, MIL.M_DEFAULT);
            MIL.MdispInquire(MilDisplay, MIL.M_OVERLAY_ID, ref MilOverlayImage);

            // Draw the position of the model to define in the overlay.
            MIL.MgraColor(MIL.M_DEFAULT, DrawColor);
            MIL.MgraRect(MIL.M_DEFAULT, MilOverlayImage, MODEL_POS_X_INIT(MilOverlayImage) - (MODEL_WIDTH / 2), MODEL_POS_Y_INIT(MilOverlayImage) - (MODEL_HEIGHT / 2), MODEL_POS_X_INIT(MilOverlayImage) + (MODEL_WIDTH / 2), MODEL_POS_Y_INIT(MilOverlayImage) + (MODEL_HEIGHT / 2));

            // Grab continuously.
            Console.Write("Model definition:\n\n");
            Console.Write("Place a unique model to find in the marked rectangle.\n");
            Console.Write("Press <Enter> to continue.\n\n");

            // Grab a reference model image.
            MIL.MdigGrabContinuous(MilDigitizer, MilDisplayImage);
            Console.ReadKey();
            MIL.MdigHalt(MilDigitizer);

            // Copy the grabbed image to the Model image to keep it.
            MIL.MbufCopy(MilDisplayImage, MilModelImage);

            // Clear and disable the overlay.
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY, MIL.M_DISABLE);
            MIL.MdispControl(MilDisplay, MIL.M_OVERLAY_CLEAR, MIL.M_DEFAULT);
        }

        //****************************************************************************
        // Tracking object with pattern matching module.
        //****************************************************************************
        static void MpatTrackingExample(MIL_ID MilSystem, MIL_ID MilDisplay, MIL_ID MilDigitizer, MIL_ID MilDisplayImage, MIL_ID MilModelImage)
        {
            MIL_ID[] MilImage = new MIL_ID[2] { MIL.M_NULL, MIL.M_NULL };   // Processing image buffer identifiers.
            MIL_ID Model = MIL.M_NULL;                                      // Model identifier.
            MIL_ID Result = MIL.M_NULL;                                     // Result identifier.
            double DrawColor = DRAW_COLOR;                                  // Model drawing color.
            MIL_INT Found = 0;                                                  // Number of found models.
            int NbFindDone = 0;                                             // Number of loops to find model done.
            double OrgX = 0.0;                                              // Original center of model.
            double OrgY = 0.0;
            double x = 0.0;                                                 // Result variables.
            double y = 0.0;
            double Score = 0.0;
            double Time = 0.0;                                              // Timer.

            // Print a start message.
            Console.Write("\nGRAYSCALE PATTERN MATCHING:\n");
            Console.Write("---------------------------\n\n");

            // Display the model image.
            MIL.MbufCopy(MilModelImage, MilDisplayImage);

            // Allocate normalized grayscale type model.
            MIL.MpatAllocModel(MilSystem, MilModelImage, MODEL_POS_X_INIT(MilModelImage) - (MODEL_WIDTH / 2), MODEL_POS_Y_INIT(MilModelImage) - (MODEL_HEIGHT / 2), MODEL_WIDTH, MODEL_HEIGHT, MIL.M_NORMALIZED, ref Model);
            // Allocate result.
            MIL.MpatAllocResult(MilSystem, 1, ref Result);

            // Draw box around the model.
            MIL.MgraColor(MIL.M_DEFAULT, DrawColor);
            MIL.MpatDraw(MIL.M_DEFAULT, Model, MilDisplayImage, MIL.M_DRAW_BOX, MIL.M_DEFAULT, MIL.M_ORIGINAL);

            // Set minimum acceptance for search.
            MIL.MpatSetAcceptance(Model, MODEL_MIN_MATCH_SCORE);

            // Set speed.
            MIL.MpatSetSpeed(Model, MIL.M_HIGH);

            // Set accuracy.
            MIL.MpatSetAccuracy(Model, MIL.M_LOW);

            // Preprocess model.
            MIL.MpatPreprocModel(MilModelImage, Model, MIL.M_DEFAULT);

            // Inquire about center of model.
            MIL.MpatInquire(Model, MIL.M_ORIGINAL_X, ref OrgX);
            MIL.MpatInquire(Model, MIL.M_ORIGINAL_Y, ref OrgY);

            // Print the original position.
            Console.Write("A Grayscale Model was defined.\n");
            Console.Write("Model dimensions:  {0} x {1}.\n", MODEL_WIDTH, MODEL_HEIGHT);
            Console.Write("Model center:      X={0:0.00}, Y={0:0.00}.\n", OrgX, OrgY);
            Console.Write("Model is scale and rotation dependant.\n");
            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();

            // Allocate 2 grab buffers.
            MIL.MbufAlloc2d(MilSystem, MIL.MbufInquire(MilModelImage, MIL.M_SIZE_X, MIL.M_NULL), MIL.MbufInquire(MilModelImage, MIL.M_SIZE_Y, MIL.M_NULL), 8, MIL.M_IMAGE + MIL.M_GRAB + MIL.M_PROC, ref MilImage[0]);
            MIL.MbufAlloc2d(MilSystem, MIL.MbufInquire(MilModelImage, MIL.M_SIZE_X, MIL.M_NULL), MIL.MbufInquire(MilModelImage, MIL.M_SIZE_Y, MIL.M_NULL), 8, MIL.M_IMAGE + MIL.M_GRAB + MIL.M_PROC, ref MilImage[1]);

            // Grab continuously and perform the find operation using double buffering.
            Console.Write("\nContinuously finding the Grayscale model.\n");
            Console.Write("Press <Enter> to stop.\n\n");

            // Grab a first target image into first buffer (done twice for timer reset accuracy).
            MIL.MdigControl(MilDigitizer, MIL.M_GRAB_MODE, MIL.M_ASYNCHRONOUS);
            MIL.MdigGrab(MilDigitizer, MilImage[NbFindDone % 2]);
            MIL.MdigGrab(MilDigitizer, MilImage[NbFindDone % 2]);
            MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_RESET, ref Time);

            // Loop, processing one buffer while grabbing the other.
            do
            {
                // Grab a target image into the other buffer.
                MIL.MdigGrab(MilDigitizer, MilImage[(NbFindDone + 1) % 2]);

                // Read the time.
                MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ, ref Time);

                // Find model.
                MIL.MpatFindModel(MilImage[NbFindDone % 2], Model, Result);

                // Get results.
                MIL.MpatGetNumber(Result, ref Found);
                MIL.MpatGetResult(Result, MIL.M_POSITION_X, ref x);
                MIL.MpatGetResult(Result, MIL.M_POSITION_Y, ref y);
                MIL.MpatGetResult(Result, MIL.M_SCORE, ref Score);

                // Print a message based upon the score.
                if (Found > 0)
                {
                    // Draw a box around the model and print the results.
                    MIL.MpatDraw(MIL.M_DEFAULT, Result, MilImage[NbFindDone % 2], MIL.M_DRAW_BOX + MIL.M_DRAW_POSITION, MIL.M_DEFAULT, MIL.M_DEFAULT);
                    Console.Write("Found: X={0,7:0.00}, Y={1,7:0.00}, Score={2,5:0.0}% ({3:0.0} fps).    \r", x, y, Score, (NbFindDone + 1) / Time);
                }
                else
                {
                    // Print the "not found" message.
                    Console.Write("Not found ! (score<{0,5:0.0}%)                ({1:0.0} fps).     \r", MODEL_MIN_MATCH_SCORE, (NbFindDone + 1) / Time);
                }

                // Copy target image to the display.
                MIL.MbufCopy(MilImage[NbFindDone % 2], MilDisplayImage);

                // Increment find count
                NbFindDone++;
            }
            while (!Console.KeyAvailable);

            Console.ReadKey();
            Console.Write("\n\n");

            // Wait for end of last grab.
            MIL.MdigGrabWait(MilDigitizer, MIL.M_GRAB_END);

            // Free all allocated objects.
            MIL.MpatFree(Result);
            MIL.MpatFree(Model);
            MIL.MbufFree(MilImage[1]);
            MIL.MbufFree(MilImage[0]);
        }


        /*****************************************************************************
        Tracking object with Geometric Model Finder module
        *****************************************************************************/
        private const int MODEL_MAX_OCCURRENCES = 16;

        static void MmodTrackingExample(MIL_ID MilSystem, MIL_ID MilDisplay, MIL_ID MilDigitizer, MIL_ID MilDisplayImage, MIL_ID MilModelImage)
        {
            MIL_ID[] MilImage = new MIL_ID[2] { MIL.M_NULL, MIL.M_NULL };       // Processing image buffer identifiers.
            MIL_ID SearchContext = MIL.M_NULL;                                  // Search context identifier.          
            MIL_ID Result = MIL.M_NULL;                                         // Result identifier.                  
            double DrawColor = DRAW_COLOR;                                      // Model drawing color.                
            MIL_INT Found = 0;                                                      // Number of models found.             
            int NbFindDone = 0;                                                 // Number of loops to find model done. 
            double OrgX = 0.0;                                                  // Original center of model.
            double OrgY = 0.0;
            double[] Score = new double[MODEL_MAX_OCCURRENCES];                 // Model correlation score.
            double[] x = new double[MODEL_MAX_OCCURRENCES];                     // Model X position.
            double[] y = new double[MODEL_MAX_OCCURRENCES];                     // Model Y position.
            double[] Angle = new double[MODEL_MAX_OCCURRENCES];                 // Model occurrence angle.
            double[] Scale = new double[MODEL_MAX_OCCURRENCES];                 // Model occurrence scale.
            double Time = 0.0;                                                  // Timer.

            // Print a start message.
            Console.Write("\nGEOMETRIC MODEL FINDER (scale and rotation independent):\n");
            Console.Write("--------------------------------------------------------\n\n");

            // Display model image.
            MIL.MbufCopy(MilModelImage, MilDisplayImage);

            // Allocate a context and define a geometric model.
            MIL.MmodAlloc(MilSystem, MIL.M_GEOMETRIC, MIL.M_DEFAULT, ref SearchContext);
            MIL.MmodDefine(SearchContext, MIL.M_IMAGE, MilModelImage, (double)MODEL_POS_X_INIT(MilModelImage) - (MODEL_WIDTH / 2), (double)MODEL_POS_Y_INIT(MilModelImage) - (MODEL_HEIGHT / 2), MODEL_WIDTH, MODEL_HEIGHT);

            // Allocate result.
            MIL.MmodAllocResult(MilSystem, MIL.M_DEFAULT, ref Result);

            // Draw a box around the model.
            MIL.MgraColor(MIL.M_DEFAULT, DrawColor);
            MIL.MmodDraw(MIL.M_DEFAULT, SearchContext, MilDisplayImage, MIL.M_DRAW_BOX, MIL.M_DEFAULT, MIL.M_ORIGINAL);

            // Set speed to VERY HIGH for fast but less precise search.
            MIL.MmodControl(SearchContext, MIL.M_CONTEXT, MIL.M_SPEED, MIL.M_VERY_HIGH);

            // Set minimum acceptance for the search.
            MIL.MmodControl(SearchContext, MIL.M_DEFAULT, MIL.M_ACCEPTANCE, MODEL_MIN_MATCH_SCORE);

            // Preprocess model.
            MIL.MmodPreprocess(SearchContext, MIL.M_DEFAULT);

            // Inquire about center of model.
            MIL.MmodInquire(SearchContext, MIL.M_DEFAULT, MIL.M_ORIGINAL_X, ref OrgX);
            MIL.MmodInquire(SearchContext, MIL.M_DEFAULT, MIL.M_ORIGINAL_Y, ref OrgY);

            // Print the original position.
            Console.Write("The Geometric target model was defined.\n");
            Console.Write("Model dimensions: {0} x {1}.\n", MODEL_WIDTH, MODEL_HEIGHT);
            Console.Write("Model center:     X={0:0.00}, Y={0:0.00}.\n", OrgX, OrgY);
            Console.Write("Model is scale and rotation independent.\n");
            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();

            // Allocate 2 grab buffers.
            MIL.MbufAlloc2d(MilSystem, MIL.MbufInquire(MilModelImage, MIL.M_SIZE_X, MIL.M_NULL), MIL.MbufInquire(MilModelImage, MIL.M_SIZE_Y, MIL.M_NULL), 8, MIL.M_IMAGE + MIL.M_GRAB + MIL.M_PROC, ref MilImage[0]);
            MIL.MbufAlloc2d(MilSystem, MIL.MbufInquire(MilModelImage, MIL.M_SIZE_X, MIL.M_NULL), MIL.MbufInquire(MilModelImage, MIL.M_SIZE_Y, MIL.M_NULL), 8, MIL.M_IMAGE + MIL.M_GRAB + MIL.M_PROC, ref MilImage[1]);

            // Grab continuously grab and perform the find operation using double buffering.
            Console.Write("\nContinuously finding the Geometric Model.\n");
            Console.Write("Press a <Enter> to stop.\n\n");

            // Grab a first target image into first buffer (done twice for timer reset accuracy).
            MIL.MdigControl(MilDigitizer, MIL.M_GRAB_MODE, MIL.M_ASYNCHRONOUS);
            MIL.MdigGrab(MilDigitizer, MilImage[NbFindDone % 2]);
            MIL.MdigGrab(MilDigitizer, MilImage[NbFindDone % 2]);
            MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_RESET, ref Time);

            // Loop, processing one buffer while grabbing the other.
            do
            {
                // Grab a target image into the other buffer.
                MIL.MdigGrab(MilDigitizer, MilImage[(NbFindDone + 1) % 2]);

                // Read the time.
                MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ, ref Time);

                // Find model.
                MIL.MmodFind(SearchContext, MilImage[NbFindDone % 2], Result);

                // Get the number of occurrences found.
                MIL.MmodGetResult(Result, MIL.M_DEFAULT, MIL.M_NUMBER + MIL.M_TYPE_MIL_INT, ref Found);

                // Print a message based on the score.
                if ((Found >= 1) && (Found < MODEL_MAX_OCCURRENCES))
                {
                    // Get results.
                    MIL.MmodGetResult(Result, MIL.M_DEFAULT, MIL.M_POSITION_X, x);
                    MIL.MmodGetResult(Result, MIL.M_DEFAULT, MIL.M_POSITION_Y, y);
                    MIL.MmodGetResult(Result, MIL.M_DEFAULT, MIL.M_SCALE, Scale);
                    MIL.MmodGetResult(Result, MIL.M_DEFAULT, MIL.M_ANGLE, Angle);
                    MIL.MmodGetResult(Result, MIL.M_DEFAULT, MIL.M_SCORE, Score);

                    // Draw a box and a cross where the model was found and print the results.
                    MIL.MmodDraw(MIL.M_DEFAULT, Result, MilImage[NbFindDone % 2], MIL.M_DRAW_BOX + MIL.M_DRAW_POSITION + MIL.M_DRAW_EDGES, MIL.M_DEFAULT, MIL.M_DEFAULT);
                    Console.Write("Found: X={0,6:0.0}, Y={1,6:0.0}, Angle={2,6:0.0}, Scale={3,5:0.00}, Score={4,5:0.0}% ({5,5:0.0} fps).\r", x[0], y[0], Angle[0], Scale[0], Score[0], (NbFindDone + 1) / Time);
                }
                else
                {
                    // Print the "not found" message.
                    Console.Write("Not found! (score<{0,5:0.0}%)                                          ({1,5:0.0} fps).\r", MODEL_MIN_MATCH_SCORE, (NbFindDone + 1) / Time);
                }

                // Copy target image to the display.
                MIL.MbufCopy(MilImage[NbFindDone % 2], MilDisplayImage);

                // Increment the counter.
                NbFindDone++;
            }
            while (!Console.KeyAvailable);

            Console.ReadKey();
            Console.Write("\n\n");

            // Wait for the end of last grab.
            MIL.MdigGrabWait(MilDigitizer, MIL.M_GRAB_END);

            // Free all allocations.
            MIL.MmodFree(Result);
            MIL.MmodFree(SearchContext);
            MIL.MbufFree(MilImage[1]);
            MIL.MbufFree(MilImage[0]);
        }
    }
}
