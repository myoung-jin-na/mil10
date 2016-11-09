//************************************************************************************
//
// File name: Mmod.cs
// 
// Synopsis:  This program uses the Geometric Model Finder module to define geometric 
//            models and searches for these models in a target image. A simple single model 
//            example (1 model, 1 occurrence, good search conditions) is presented first, 
//            followed by a more complex example (multiple models, multiple occurrences in a
//            complex scene with bad search conditions).
//************************************************************************************
using System;
using System.Collections.Generic;
using System.Text;

using Matrox.MatroxImagingLibrary;

namespace MMod
{
    class Program
    {
        static void Main(string[] args)
        {
            MIL_ID MilApplication = MIL.M_NULL;     // Application identifier.
            MIL_ID MilSystem = MIL.M_NULL;          // System identifier.
            MIL_ID MilDisplay = MIL.M_NULL;         // Display identifier.

            // Allocate defaults.
            MIL.MappAllocDefault(MIL.M_DEFAULT, ref MilApplication, ref MilSystem, ref MilDisplay, MIL.M_NULL, MIL.M_NULL);

            // Run single model example.
            SingleModelExample(MilSystem, MilDisplay);

            // Run multiple model example.
            MultipleModelExample(MilSystem, MilDisplay);

            // Free defaults
            MIL.MappFreeDefault(MilApplication, MilSystem, MilDisplay, MIL.M_NULL, MIL.M_NULL);
        }

        //*****************************************************************************
        // Single model example.
        //
        // Source MIL image file specifications.
        private const string SINGLE_MODEL_IMAGE = MIL.M_IMAGE_PATH + "SingleModel.mim";

        // Target MIL image file specifications.
        private const string SINGLE_MODEL_TARGET_IMAGE = MIL.M_IMAGE_PATH + "SingleTarget.mim";

        //Search speed: M_VERY_HIGH for faster search, M_MEDIUM for precision and robustness.
        private const int SINGLE_MODEL_SEARCH_SPEED = MIL.M_VERY_HIGH;

        // Model specifications.
        private const int MODEL_OFFSETX = 176;
        private const int MODEL_OFFSETY = 136;
        private const int MODEL_SIZEX = 128;
        private const int MODEL_SIZEY = 128;
        private const int MODEL_MAX_OCCURRENCES = 16;

        static void SingleModelExample(MIL_ID MilSystem, MIL_ID MilDisplay)
        {
            MIL_ID MilImage = MIL.M_NULL;                               // Image buffer identifier.
            MIL_ID GraphicList = MIL.M_NULL;                            // Graphic list indentifier.
            MIL_ID MilSearchContext = MIL.M_NULL;                       // Search context.
            MIL_ID MilResult = MIL.M_NULL;                              // Result identifier.
            double ModelDrawColor = MIL.M_COLOR_RED;                    // Model draw color.
            MIL_INT[] Model = new MIL_INT[MODEL_MAX_OCCURRENCES];               // Model index.
            MIL_INT NumResults = 0;                                         // Number of results found.

            double[] Score = new double[MODEL_MAX_OCCURRENCES];         // Model correlation score.
            double[] XPosition = new double[MODEL_MAX_OCCURRENCES];     // Model X position.
            double[] YPosition = new double[MODEL_MAX_OCCURRENCES];     // Model Y position. 
            double[] Angle = new double[MODEL_MAX_OCCURRENCES];         // Model occurrence angle.
            double[] Scale = new double[MODEL_MAX_OCCURRENCES];         // Model occurrence scale.
            double Time = 0.0;                                          // Bench variable.
            int i = 0;                                                  // Loop variable.

            // Restore the model image and display it.
            MIL.MbufRestore(SINGLE_MODEL_IMAGE, MilSystem, ref MilImage);
            MIL.MdispSelect(MilDisplay, MilImage);

            // Allocate a graphic list to hold the subpixel annotations to draw.
            MIL.MgraAllocList(MilSystem, MIL.M_DEFAULT, ref GraphicList);

            // Associate the graphic list to the display for annotations.
            MIL.MdispControl(MilDisplay, MIL.M_ASSOCIATED_GRAPHIC_LIST_ID, GraphicList);

            // Allocate a Geometric Model Finder context.
            MIL.MmodAlloc(MilSystem, MIL.M_GEOMETRIC, MIL.M_DEFAULT, ref MilSearchContext);

            // Allocate a result buffer.
            MIL.MmodAllocResult(MilSystem, MIL.M_DEFAULT, ref MilResult);

            // Define the model.
            MIL.MmodDefine(MilSearchContext, MIL.M_IMAGE, MilImage, MODEL_OFFSETX, MODEL_OFFSETY, MODEL_SIZEX, MODEL_SIZEY);

            // Set the search speed.
            MIL.MmodControl(MilSearchContext, MIL.M_CONTEXT, MIL.M_SPEED, SINGLE_MODEL_SEARCH_SPEED);

            // Preprocess the search context.
            MIL.MmodPreprocess(MilSearchContext, MIL.M_DEFAULT);

            // Draw box and position it in the source image to show the model.
            MIL.MgraColor(MIL.M_DEFAULT, ModelDrawColor);
            MIL.MmodDraw(MIL.M_DEFAULT, MilSearchContext, GraphicList,
                     MIL.M_DRAW_BOX + MIL.M_DRAW_POSITION, 0, MIL.M_ORIGINAL);

            // Pause to show the model.
            Console.Write("\nGEOMETRIC MODEL FINDER:\n");
            Console.Write("-----------------------\n\n");
            Console.Write("A model context was defined with the model in the displayed image.\n");
            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();

            // Clear annotations.
            MIL.MgraClear(MIL.M_DEFAULT, GraphicList);

            // Load the single model target image.
            MIL.MbufLoad(SINGLE_MODEL_TARGET_IMAGE, MilImage);

            // Dummy first call for bench measure purpose only (bench stabilization, cache effect, etc...). This first call is NOT required by the application.
            MIL.MmodFind(MilSearchContext, MilImage, MilResult);

            // Reset the timer.
            MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_RESET + MIL.M_SYNCHRONOUS, MIL.M_NULL);

            // Find the model.
            MIL.MmodFind(MilSearchContext, MilImage, MilResult);

            // Read the find time.
            MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ + MIL.M_SYNCHRONOUS, ref Time);

            // Get the number of models found.
            MIL.MmodGetResult(MilResult, MIL.M_DEFAULT, MIL.M_NUMBER + MIL.M_TYPE_MIL_INT, ref NumResults);

            // If a model was found above the acceptance threshold.
            if ((NumResults >= 1) && (NumResults <= MODEL_MAX_OCCURRENCES))
            {
                // Get the results of the single model.
                MIL.MmodGetResult(MilResult, MIL.M_DEFAULT, MIL.M_INDEX + MIL.M_TYPE_MIL_INT, Model);
                MIL.MmodGetResult(MilResult, MIL.M_DEFAULT, MIL.M_POSITION_X, XPosition);
                MIL.MmodGetResult(MilResult, MIL.M_DEFAULT, MIL.M_POSITION_Y, YPosition);
                MIL.MmodGetResult(MilResult, MIL.M_DEFAULT, MIL.M_ANGLE, Angle);
                MIL.MmodGetResult(MilResult, MIL.M_DEFAULT, MIL.M_SCALE, Scale);
                MIL.MmodGetResult(MilResult, MIL.M_DEFAULT, MIL.M_SCORE, Score);

                // Print the results for each model found.
                Console.Write("The model was found in the target image:\n\n");
                Console.Write("Result   Model   X Position   Y Position   Angle   Scale   Score\n\n");
                for (i = 0; i < NumResults; i++)
                {
                    Console.Write("{0,-9}{1,-8}{2,-13:0.00}{3,-13:0.00}{4,-8:0.00}{5,-8:0.00}{6,-5:0.00}%\n", i, Model[i], XPosition[i], YPosition[i], Angle[i], Scale[i], Score[i]);
                }
                Console.Write("\nThe search time is {0:0.0} ms\n\n", Time * 1000.0);

                // Draw edges, position and box over the occurrences that were found.
                for (i = 0; i < NumResults; i++)
                {
                    MIL.MgraColor(MIL.M_DEFAULT, ModelDrawColor);
                    MIL.MmodDraw(MIL.M_DEFAULT, MilResult, GraphicList, MIL.M_DRAW_EDGES + MIL.M_DRAW_BOX + MIL.M_DRAW_POSITION, i, MIL.M_DEFAULT);
                }
            }
            else
            {
                Console.Write("The model was not found or the number of models found is greater than\n");
                Console.Write("the specified maximum number of occurrence !\n\n");
            }

            // Wait for a key to be pressed.
            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();

            // Free MIL objects.
            MIL.MgraFree(GraphicList);
            MIL.MbufFree(MilImage);
            MIL.MmodFree(MilSearchContext);
            MIL.MmodFree(MilResult);

        }

        //****************************************************************************
        // Multiple models example.
        // Source MIL image file specifications.
        private const string MULTI_MODELS_IMAGE = MIL.M_IMAGE_PATH + "MultipleModel.mim";

        // Target MIL image file specifications.
        private const string MULTI_MODELS_TARGET_IMAGE = MIL.M_IMAGE_PATH + "MultipleTarget.mim";

        // Search speed:  M_VERY_HIGH for faster search, M_MEDIUM for precision and robustness.
        private const int MULTI_MODELS_SEARCH_SPEED = MIL.M_VERY_HIGH;

        // Number of models.
        private const int NUMBER_OF_MODELS = 3;
        private const int MODELS_MAX_OCCURRENCES = 16;

        // Model 1 specifications.
        private const int MODEL0_OFFSETX = 34;
        private const int MODEL0_OFFSETY = 93;
        private const int MODEL0_SIZEX = 214;
        private const int MODEL0_SIZEY = 76;
        private static readonly int MODEL0_DRAW_COLOR = MIL.M_COLOR_RED;

        // Model 2 specifications.
        private const int MODEL1_OFFSETX = 73;
        private const int MODEL1_OFFSETY = 232;
        private const int MODEL1_SIZEX = 150;
        private const int MODEL1_SIZEY = 154;
        private const int MODEL1_REFERENCEX = 23;
        private const int MODEL1_REFERENCEY = 127;
        private static readonly int MODEL1_DRAW_COLOR = MIL.M_COLOR_GREEN;

        // Model 3 specifications.
        private const int MODEL2_OFFSETX = 308;
        private const int MODEL2_OFFSETY = 39;
        private const int MODEL2_SIZEX = 175;
        private const int MODEL2_SIZEY = 357;
        private const int MODEL2_REFERENCEX = 62;
        private const int MODEL2_REFERENCEY = 150;
        private static readonly int MODEL2_DRAW_COLOR = MIL.M_COLOR_BLUE;

        // Models array size.
        private const int MODELS_ARRAY_SIZE = 3;

        static void MultipleModelExample(MIL_ID MilSystem, MIL_ID MilDisplay)
        {
            MIL_ID MilImage = MIL.M_NULL;                               // Image buffer identifier.
            MIL_ID GraphicList = MIL.M_NULL;                            // Graphic list identifier.
            MIL_ID MilSearchContext = MIL.M_NULL;                       // Search context.
            MIL_ID MilResult = MIL.M_NULL;                              // Result identifier.
            double[] ModelsDrawColor = new double[MODELS_ARRAY_SIZE];   // Model drawing colors.
            MIL_INT NumResults = 0;                                     // Number of results found.
            MIL_INT[] Models = new MIL_INT[MODELS_MAX_OCCURRENCES];     // Model indices.
            MIL_INT[] ModelsOffsetX = new MIL_INT[MODELS_ARRAY_SIZE];   // Model X offsets array.
            MIL_INT[] ModelsOffsetY = new MIL_INT[MODELS_ARRAY_SIZE];   // Model Y offsets array.
            MIL_INT[] ModelsSizeX = new MIL_INT[MODELS_ARRAY_SIZE];     // Model X sizes array.
            MIL_INT[] ModelsSizeY = new MIL_INT[MODELS_ARRAY_SIZE];     // Model Y sizes array.
            double[] Score = new double[MODELS_MAX_OCCURRENCES];        // Model correlation scores.
            double[] XPosition = new double[MODELS_MAX_OCCURRENCES];    // Model X positions.
            double[] YPosition = new double[MODELS_MAX_OCCURRENCES];    // Model Y positions.
            double[] Angle = new double[MODELS_MAX_OCCURRENCES];        // Model occurrence angles.
            double[] Scale = new double[MODELS_MAX_OCCURRENCES];        // Model occurrence scales.
            double Time = 0.0;                                          // Time variable.
            int i;                                                      // Loop variable.

            // Models array specifications.
            ModelsOffsetX[0] = MODEL0_OFFSETX;
            ModelsOffsetX[1] = MODEL1_OFFSETX;
            ModelsOffsetX[2] = MODEL2_OFFSETX;

            ModelsOffsetY[0] = MODEL0_OFFSETY;
            ModelsOffsetY[1] = MODEL1_OFFSETY;
            ModelsOffsetY[2] = MODEL2_OFFSETY;

            ModelsSizeX[0] = MODEL0_SIZEX;
            ModelsSizeX[1] = MODEL1_SIZEX;
            ModelsSizeX[2] = MODEL2_SIZEX;

            ModelsSizeY[0] = MODEL0_SIZEY;
            ModelsSizeY[1] = MODEL1_SIZEY;
            ModelsSizeY[2] = MODEL2_SIZEY;

            ModelsDrawColor[0] = MODEL0_DRAW_COLOR;
            ModelsDrawColor[1] = MODEL1_DRAW_COLOR;
            ModelsDrawColor[2] = MODEL2_DRAW_COLOR;

            // Restore the model image and display it.
            MIL.MbufRestore(MULTI_MODELS_IMAGE, MilSystem, ref MilImage);
            MIL.MdispSelect(MilDisplay, MilImage);

            // Allocate a graphic list to hold the subpixel annotations to draw.
            MIL.MgraAllocList(MilSystem, MIL.M_DEFAULT, ref GraphicList);

            // Associate the graphic list to the display for annotations.
            MIL.MdispControl(MilDisplay, MIL.M_ASSOCIATED_GRAPHIC_LIST_ID, GraphicList);

            // Allocate a geometric model finder.
            MIL.MmodAlloc(MilSystem, MIL.M_GEOMETRIC, MIL.M_DEFAULT, ref MilSearchContext);

            // Allocate a result buffer.
            MIL.MmodAllocResult(MilSystem, MIL.M_DEFAULT, ref MilResult);

            // Define the models.
            for (i = 0; i < NUMBER_OF_MODELS; i++)
            {
                MIL.MmodDefine(MilSearchContext, MIL.M_IMAGE, MilImage, (double)ModelsOffsetX[i], (double)ModelsOffsetY[i], (double)ModelsSizeX[i], (double)ModelsSizeY[i]);
            }

            // Set the desired search speed.
            MIL.MmodControl(MilSearchContext, MIL.M_CONTEXT, MIL.M_SPEED, MULTI_MODELS_SEARCH_SPEED);

            // Increase the smoothness for the edge extraction in the search context.
            MIL.MmodControl(MilSearchContext, MIL.M_CONTEXT, MIL.M_SMOOTHNESS, 75);

            // Modify the acceptance and the certainty for all the models that were defined.
            MIL.MmodControl(MilSearchContext, MIL.M_DEFAULT, MIL.M_ACCEPTANCE, 40);
            MIL.MmodControl(MilSearchContext, MIL.M_DEFAULT, MIL.M_CERTAINTY, 60);

            // Set the number of occurrences to 2 for all the models that were defined.
            MIL.MmodControl(MilSearchContext, MIL.M_DEFAULT, MIL.M_NUMBER, 2);

            if (NUMBER_OF_MODELS > 1)
            {
                // Change the reference point of the second model.
                MIL.MmodControl(MilSearchContext, 1, MIL.M_REFERENCE_X, MODEL1_REFERENCEX);
                MIL.MmodControl(MilSearchContext, 1, MIL.M_REFERENCE_Y, MODEL1_REFERENCEY);
            }

            if (NUMBER_OF_MODELS > 2)
            {
                // Change the reference point of the third model.
                MIL.MmodControl(MilSearchContext, 2, MIL.M_REFERENCE_X, MODEL2_REFERENCEX);
                MIL.MmodControl(MilSearchContext, 2, MIL.M_REFERENCE_Y, MODEL2_REFERENCEY);
            }
            // Preprocess the search context.
            MIL.MmodPreprocess(MilSearchContext, MIL.M_DEFAULT);

            // Draw boxes and positions in the source image to identify the models.
            for (i = 0; i < NUMBER_OF_MODELS; i++)
            {
                MIL.MgraColor(MIL.M_DEFAULT, ModelsDrawColor[i]);
                MIL.MmodDraw(MIL.M_DEFAULT, MilSearchContext, GraphicList, MIL.M_DRAW_BOX + MIL.M_DRAW_POSITION, i, MIL.M_ORIGINAL);
            }

            // Pause to show the models.
            Console.Write("A model context was defined with the models in the displayed image.\n");
            Console.Write("Press <Enter> to continue.\n\n");
            Console.ReadKey();

            // Clear annotations.
            MIL.MgraClear(MIL.M_DEFAULT, GraphicList);

            // Load the complex target image.
            MIL.MbufLoad(MULTI_MODELS_TARGET_IMAGE, MilImage);

            // Dummy first call for bench measure purpose only (bench stabilization, cache effect, etc...). This first call is NOT required by the application.
            MIL.MmodFind(MilSearchContext, MilImage, MilResult);

            // Reset the timer.
            MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_RESET + MIL.M_SYNCHRONOUS, MIL.M_NULL);

            // Find the models.
            MIL.MmodFind(MilSearchContext, MilImage, MilResult);

            // Read the find time.
            MIL.MappTimer(MIL.M_DEFAULT, MIL.M_TIMER_READ + MIL.M_SYNCHRONOUS, ref Time);

            // Get the number of models found.
            MIL.MmodGetResult(MilResult, MIL.M_DEFAULT, MIL.M_NUMBER + MIL.M_TYPE_MIL_INT, ref NumResults);

            // If the models were found above the acceptance threshold.
            if ((NumResults >= 1) && (NumResults <= MODELS_MAX_OCCURRENCES))
            {
                // Get the results for each model.
                MIL.MmodGetResult(MilResult, MIL.M_DEFAULT, MIL.M_INDEX + MIL.M_TYPE_MIL_INT, Models);
                MIL.MmodGetResult(MilResult, MIL.M_DEFAULT, MIL.M_POSITION_X, XPosition);
                MIL.MmodGetResult(MilResult, MIL.M_DEFAULT, MIL.M_POSITION_Y, YPosition);
                MIL.MmodGetResult(MilResult, MIL.M_DEFAULT, MIL.M_ANGLE, Angle);
                MIL.MmodGetResult(MilResult, MIL.M_DEFAULT, MIL.M_SCALE, Scale);
                MIL.MmodGetResult(MilResult, MIL.M_DEFAULT, MIL.M_SCORE, Score);

                // Print information about the target image.
                Console.Write("The models were found in the target image although there is:\n");
                Console.Write("   Full rotation\n   Small scale change\n   Contrast variation\n");
                Console.Write("   Specular reflection\n   Occlusion\n   Multiple models\n");
                Console.Write("   Multiple occurrences\n\n");

                // Print the results for the found models.
                Console.Write("Result   Model   X Position   Y Position   Angle   Scale   Score\n\n");
                for (i = 0; i < NumResults; i++)
                {
                    Console.Write("{0,-9}{1,-8}{2,-13:0.00}{3,-13:#.00}{4,-8:0.00}{5,-8:0.00}{6,-5:0.00}%\n", i, Models[i], XPosition[i], YPosition[i], Angle[i], Scale[i], Score[i]);
                }
                Console.Write("\nThe search time is {0:0.0} ms\n\n", Time * 1000.0);

                // Draw edges and positions over the occurrences that were found.
                for (i = 0; i < NumResults; i++)
                {
                    MIL.MgraColor(MIL.M_DEFAULT, ModelsDrawColor[Models[i]]);
                    MIL.MmodDraw(MIL.M_DEFAULT, MilResult, GraphicList, MIL.M_DRAW_EDGES + MIL.M_DRAW_POSITION, i, MIL.M_DEFAULT);
                }
            }
            else
            {
                Console.Write("The models were not found or the number of models found is greater than\n");
                Console.Write("the defined value of maximum occurrences !\n\n");
            }

            // Wait for a key to be pressed.
            Console.Write("Press <Enter> to end.\n\n");
            Console.ReadKey();

            // Free MIL objects.
            MIL.MgraFree(GraphicList);
            MIL.MbufFree(MilImage);
            MIL.MmodFree(MilSearchContext);
            MIL.MmodFree(MilResult);
        }
    }
}
