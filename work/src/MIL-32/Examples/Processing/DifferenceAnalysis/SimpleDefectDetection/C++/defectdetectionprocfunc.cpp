//***************************************************************************************
// 
// File name: defectdetectionprocfunc.cpp
//
// Synopsis: This file contains the processing functions declarations
//           that are used to perform some simple defect detection.
//

#include <mil.h>
#include <math.h>
#include "defectdetectionprocfunc.h"

//*****************************************************************************
// Remaps to an 8 bit image. 
//*****************************************************************************
void Remap8BitImage(MIL_ID MilSrcImage,
                    MIL_ID MilDestImage,
                    MIL_INT StartIndex,
                    MIL_INT EndIndex,
                    MIL_DOUBLE MinValue,
                    MIL_DOUBLE MaxValue)
   {
   // Work on the same system of the SrcBufferId.
   MIL_ID MilSystem = MbufInquire(MilSrcImage, M_OWNER_SYSTEM, M_NULL);
     
   // Alloc the LUT.
   MIL_ID MilLut = MbufAlloc1d(MilSystem, 256, 32+M_FLOAT, M_LUT, M_NULL);
   
   // Generate the ramp.
   MgenLutRamp(MilLut, 0, MinValue, StartIndex, MinValue);
   MgenLutRamp(MilLut, StartIndex, MinValue, EndIndex, MaxValue);
   MgenLutRamp(MilLut, EndIndex, MaxValue, 255, MaxValue);
   
   // Apply the mapping.
   MimLutMap(MilSrcImage, MilDestImage, MilLut);

   // Free the buffers.
   MbufFree(MilLut);
   }

//*****************************************************************************
// Creates a gradient mask.
//*****************************************************************************
void CreateGradientMaskImage(MIL_ID MilTemplateImage,
                            MIL_ID MilTemplateLumImage,
                            MIL_DOUBLE GradientSmoothness,
                            MIL_ID *MilGradientMaskImagePtr,
                            MIL_ID *MilGradientLumMaskImagePtr)
   {
   // Get the owner system of the template image.
   MIL_ID MilSystem = MbufInquire(MilTemplateLumImage, M_OWNER_SYSTEM, M_NULL);

   // Get the size of the template image.
   MIL_INT TemplateSizeX = MbufInquire(MilTemplateLumImage, M_SIZE_X, M_NULL);
   MIL_INT TemplateSizeY = MbufInquire(MilTemplateLumImage, M_SIZE_Y, M_NULL);

   // Allocate the gradient mask images.
   MIL_ID MilGradientMaskColorImage = MbufAllocColor(MilSystem, 3, TemplateSizeX, TemplateSizeY, 8+M_UNSIGNED, M_IMAGE+M_PROC, M_NULL);
   MIL_ID MilGradientLumMaskImage = MbufAlloc2d(MilSystem, TemplateSizeX, TemplateSizeY, 8+M_UNSIGNED, M_IMAGE+M_PROC, M_NULL);
   MIL_ID MilGradientMaskImage = MbufAlloc2d(MilSystem, TemplateSizeX, TemplateSizeY, 8+M_UNSIGNED, M_IMAGE+M_PROC, M_NULL);

   // Create the luminance gradient mask.
   MimConvolve(MilTemplateLumImage, MilGradientLumMaskImage, M_SHEN_FILTER(M_EDGE_DETECT, GradientSmoothness));

   // Create the gradient mask of each band.
   MimConvolve(MilTemplateImage, MilGradientMaskColorImage, M_SHEN_FILTER(M_EDGE_DETECT, GradientSmoothness));
      
   // Create the gradient mask total image.
   MIL_ID MilGradientTotal  = MbufAlloc2d(MilSystem, TemplateSizeX, TemplateSizeY, 32+M_UNSIGNED, M_IMAGE+M_PROC, M_NULL);
   MIL_ID MilGradientSquare = MbufAlloc2d(MilSystem, TemplateSizeX, TemplateSizeY, 16+M_UNSIGNED, M_IMAGE+M_PROC, M_NULL);
   MbufClear(MilGradientTotal, 0);
   for(MIL_INT BandIdx = 0; BandIdx < 3; BandIdx++)
      {
      MIL_ID MilGradientBand = MbufChildColor(MilGradientMaskColorImage, M_RED << BandIdx, M_NULL);
      MimArith(MilGradientBand, M_NULL, MilGradientSquare, M_SQUARE);
      MimArith(MilGradientTotal, MilGradientSquare, MilGradientTotal, M_ADD);
      MbufFree(MilGradientBand);
      }
   
   // Get the square root of the total of the gradient.
   MimArith(MilGradientTotal, M_NULL, MilGradientMaskImage, M_SQUARE_ROOT);
  
   // Since the IIR filter maximum value of the IIR filter is 127, we stretch back the two buffers to 255.
   static const MIL_INT MaxColorGradient = (MIL_INT)sqrt(127.0*127.0*3.0);
   Remap8BitImage(MilGradientMaskImage, MilGradientMaskImage, 0, MaxColorGradient, 0, 255);
   Remap8BitImage(MilGradientLumMaskImage, MilGradientLumMaskImage, 0, 127, 0, 255);

   if(MilGradientMaskImagePtr != M_NULL)
      *MilGradientMaskImagePtr = MilGradientMaskImage;
   else
      MbufFree(MilGradientMaskImage);

   if(MilGradientLumMaskImagePtr != M_NULL)
      *MilGradientLumMaskImagePtr = MilGradientLumMaskImage;
   else
      MbufFree(MilGradientLumMaskImage);

   MbufFree(MilGradientTotal);
   MbufFree(MilGradientSquare);
   MbufFree(MilGradientMaskColorImage);
   }

//*****************************************************************************
// Defines the model and the fixturing offset.
//*****************************************************************************
void DefineModelAndFixture(MIL_ID MilTemplateLumImage,
                           MIL_ID MilModContext,
                           MIL_ID MilFixturingOffset,
                           MIL_DOUBLE ModelOffsetX,
                           MIL_DOUBLE ModelOffsetY,
                           MIL_DOUBLE ModelSizeX,
                           MIL_DOUBLE ModelSizeY)
   {
   // Remove the model from the context if there is one
   if(MmodInquire(MilModContext, M_CONTEXT, M_NUMBER_MODELS + M_TYPE_MIL_INT, M_NULL))
      MmodDefine(MilModContext, M_DELETE, M_ALL, M_DEFAULT, M_DEFAULT, M_DEFAULT, M_DEFAULT);

   // Define the model image from the template image.
   MmodDefine(MilModContext, M_IMAGE, MilTemplateLumImage, ModelOffsetX, ModelOffsetY, ModelSizeX, ModelSizeY);
   
   // Set the number of models to find to M_ALL and preprocess.
   MmodControl(MilModContext, 0, M_NUMBER, M_ALL);
   MmodPreprocess(MilModContext, M_DEFAULT);

   // Learn the fixture offset from the model.
   McalFixture(M_NULL, MilFixturingOffset, M_LEARN_OFFSET, M_MODEL_MOD, MilModContext, 0, M_DEFAULT, M_DEFAULT, M_DEFAULT);
   }

//*****************************************************************************
// Finds the model and returns the number of occurrences.
//*****************************************************************************
MIL_INT FindModel(MIL_ID MilModContext,
                  MIL_ID MilTargetImage,
                  MIL_ID MilModResult)
   {   
   // Find the model in the target image.
   MmodFind(MilModContext, MilTargetImage, MilModResult);

   // Get the number of occurrences.
   MIL_INT NbOfOccurrences;
   MmodGetResult(MilModResult, M_GENERAL, M_NUMBER+M_TYPE_MIL_INT, &NbOfOccurrences);

   return NbOfOccurrences;
   }

//*****************************************************************************
// Aligns the SrcImage in the destination image based on the fixturing info.
//*****************************************************************************
void AlignImageBasedOnFixture(MIL_ID MilSrcImage,
                              MIL_ID MilDestImage,
                              MIL_ID MilFixturingOffset,
                              MIL_ID MilFixtureProvider,
                              MIL_INT ResultType,
                              MIL_INT OccurenceIdx)
   {
   // Fixture the current occurrence.
   McalFixture(MilSrcImage, MilFixturingOffset, M_MOVE_RELATIVE, ResultType, MilFixtureProvider, OccurenceIdx, M_DEFAULT, M_DEFAULT, M_DEFAULT);

   // Warp the occurrence in the inspection image.
   McalTransformImage(MilSrcImage, MilDestImage, M_DEFAULT, M_BILINEAR+M_OVERSCAN_CLEAR, M_DEFAULT, M_WARP_IMAGE+M_USE_DESTINATION_CALIBRATION);
   }


//*****************************************************************************
// Extracts the defects from the difference image.
//*****************************************************************************
MIL_INT ExtractDefects(MIL_ID MilDifferenceGrayImage,
                       MIL_ID MilBlobResult,
                       MIL_ID MilBlobFeatureList,
                       MIL_DOUBLE TriangleLowerCutoff,
                       MIL_DOUBLE TriangleUpperCutoff,
                       MIL_DOUBLE BinCumulativeValue,
                       MIL_DOUBLE NormalVariation,
                       MIL_DOUBLE FixedDiffThreshold,
                       MIL_INT    CleanMorphSize,
                       const BinarizationMethod BinMethod)
   {
   // Get the owner system and the size of the difference image.
   MIL_ID MilSystem     = MbufInquire(MilDifferenceGrayImage, M_OWNER_SYSTEM, M_NULL);
   MIL_INT ImageSizeX   = MbufInquire(MilDifferenceGrayImage, M_SIZE_X, M_NULL);
   MIL_INT ImageSizeY   = MbufInquire(MilDifferenceGrayImage, M_SIZE_Y, M_NULL);

   // Allocate the binary image.
   MIL_ID MilInspectionBinImage = MbufAlloc2d(MilSystem, ImageSizeX, ImageSizeY, 1+M_UNSIGNED, M_IMAGE+M_PROC, M_NULL);
  
   // Binarize the image.
   MIL_INT BinValue;
   switch(BinMethod)
      {
      case enCumulHistPercentage:
         BinValue = MimBinarize(MilDifferenceGrayImage, M_NULL, M_PERCENTILE_VALUE, BinCumulativeValue, M_NULL);
         break;

      case enTriangleBisection:
         NormalVariation = (MIL_DOUBLE) MimBinarize(MilDifferenceGrayImage, M_NULL, M_TRIANGLE_BISECTION_BRIGHT, TriangleLowerCutoff, TriangleUpperCutoff);
      case enFixed:
         BinValue = (MIL_INT) (((NormalVariation + FixedDiffThreshold) > 255) ? 255 : (NormalVariation + FixedDiffThreshold));
         break;

      case enBiModal:
      default:
         BinValue = MimBinarize(MilDifferenceGrayImage, M_NULL, M_BIMODAL, M_NULL, M_NULL);
         break;
      }
   MimBinarize(MilDifferenceGrayImage, MilInspectionBinImage, M_GREATER_OR_EQUAL, (MIL_DOUBLE) BinValue, M_NULL);

   // Clean the binary image.
   MimOpen(MilInspectionBinImage, MilInspectionBinImage, CleanMorphSize, M_BINARY);

   // Calculate the blobs.
   MblobCalculate(MilInspectionBinImage, M_NULL, MilBlobFeatureList, MilBlobResult);

   // Free the binary image.
   MbufFree(MilInspectionBinImage);

   return BinValue;
   }

//*****************************************************************************
// Extracts the differences. 
//*****************************************************************************
void ExtractDifferences(MIL_ID MilTemplateImage,
                        MIL_ID MilTemplateLumImage,
                        MIL_ID MilTemplateGradientMask,
                        MIL_ID MilTemplateLumGradientMask,
                        MIL_ID MilWarpedTarget,
                        MIL_ID MilDifferenceGrayImage, 
                        MIL_ID MilStructElement,
                        const DifferenceExtractionMethod DiffExtractMethod)
   {
   // Get the owner system and the size of the difference image.
   MIL_ID MilSystem = MbufInquire(MilDifferenceGrayImage, M_OWNER_SYSTEM, M_NULL);
   MIL_INT ImageSizeX = MbufInquire(MilDifferenceGrayImage, M_SIZE_X, M_NULL);
   MIL_INT ImageSizeY = MbufInquire(MilDifferenceGrayImage, M_SIZE_Y, M_NULL);   

   // Create the luminance version of the Warped target if the difference method is not the color distance.
   MIL_ID MilWarpedTargetLum = M_NULL;
   if(DiffExtractMethod != enColDistance)
      {
      if(MbufInquire(MilWarpedTarget, M_SIZE_BAND, M_NULL) == 3)
         {
         // Allocate the warped target luminance image. 
         MbufAlloc2d(MilSystem, ImageSizeX, ImageSizeY, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilWarpedTargetLum);

         // Get the luminance of the warped target.
         MimConvert(MilWarpedTarget, MilWarpedTargetLum, M_RGB_TO_L);
         }
      else
         MilWarpedTargetLum = MilWarpedTarget;
      }

   // Extract the differences.
   switch(DiffExtractMethod)
      {
      case enAbsoluteDiff:
         MimArith(MilTemplateLumImage, MilWarpedTargetLum, MilDifferenceGrayImage, M_SUB_ABS);
         break;

      case enColDistance:
         McolDistance(MilTemplateImage, MilWarpedTarget, MilDifferenceGrayImage, M_NULL, M_DEFAULT, M_EUCLIDEAN, M_NO_NORMALIZE, M_DEFAULT);
         break;

      case enTopHat:
         MimMorphic(MilWarpedTargetLum, MilDifferenceGrayImage, MilStructElement, M_TOP_HAT, 1, M_GRAYSCALE);
         break;

      case enBottomHat:
      default:
         MimMorphic(MilWarpedTargetLum, MilDifferenceGrayImage, MilStructElement, M_BOTTOM_HAT, 1, M_GRAYSCALE);
         break;
      }

   // Substract the gradient mask to prevent edge effect.
   MimArith(MilDifferenceGrayImage, DiffExtractMethod == enColDistance ? MilTemplateGradientMask : MilTemplateLumGradientMask, MilDifferenceGrayImage, M_SUB+M_SATURATION);

   if(MilWarpedTargetLum && MilWarpedTargetLum != MilWarpedTarget)
      MbufFree(MilWarpedTargetLum);
   }

