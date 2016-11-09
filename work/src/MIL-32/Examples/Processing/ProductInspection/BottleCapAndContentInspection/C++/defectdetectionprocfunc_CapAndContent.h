//***************************************************************************************
// 
// File name: defectdetectionprocfunc_CapAndContent.h
//
// Synopsis: This file contains the processing functions declarations
//           that are used to perform some simple defect detection.
//

#ifndef DEFECT_DETECTION_PROC_FUNC_CAP_AND_CONTENT_H
#define DEFECT_DETECTION_PROC_FUNC_CAP_AND_CONTENT_H

//*****************************************************************************
// Enumerations and constants.
//*****************************************************************************
enum DifferenceExtractionMethod
   {
   enAbsoluteDiff = 0,
   enColDistance,
   enBottomHat,
   enTopHat,
   enNbDiffMethod,
   };

static MIL_CONST_TEXT_PTR DifferenceExtractionMethodsTags[4] =
   {
   MIL_TEXT("Template absolute value of difference"),
   MIL_TEXT("Template color distance"),
   MIL_TEXT("Target morphological bottom hat filtering"),
   MIL_TEXT("Target morphological top hat filtering"),
   };

enum BinarizationMethod
   {
   enBiModal = 0,
   enTriangleBisection,
   enCumulHistPercentage,
   enFixed,
   enNbBinMethod
   };

static MIL_CONST_TEXT_PTR BinarizationMethodsTags[4] =
   {
   MIL_TEXT("Bimodal"),
   MIL_TEXT("Triangle Bisection"),
   MIL_TEXT("Cumulative Histogram Percentile"),
   MIL_TEXT("Fixed")
   };

//*****************************************************************************
// Processing functions declarations.
//*****************************************************************************
void Remap8BitImage(MIL_ID MilSrcImage,
                    MIL_ID MilDestImage,
                    MIL_INT StartIndex,
                    MIL_INT EndIndex,
                    MIL_DOUBLE MinValue,
                    MIL_DOUBLE MaxValue);

void CreateGradientMaskImage(MIL_ID MilTemplateImage,
                             MIL_ID MilTemplateLumImage,
                             MIL_DOUBLE GradientSmoothness,
                             MIL_ID *MilGradientMaskImagePtr,
                             MIL_ID *MilGradientLumMaskImagePtr);

void DefineModelAndFixture(MIL_ID MilTemplateLumImage,
                           MIL_ID MilModContext,
                           MIL_ID MilFixturingOffset,
                           MIL_DOUBLE ModelOffsetX,
                           MIL_DOUBLE ModelOffsetY,
                           MIL_DOUBLE ModelSizeX,
                           MIL_DOUBLE ModelSizeY);

MIL_INT FindModel(MIL_ID MilModContext,
                  MIL_ID MilTargetImage,
                  MIL_ID MilModResult);

void AlignImageBasedOnFixture(MIL_ID MilSrcImage,
                              MIL_ID MilDestImage,
                              MIL_ID MilFixturingOffset,
                              MIL_ID MilFixtureProvider,
                              MIL_INT ResultType,
                              MIL_INT OccurenceIdx);

MIL_INT ExtractDefects(MIL_ID MilDifferenceGrayImage,
                       MIL_ID MilExtractedDefectsImage,
                       MIL_DOUBLE TriangleLowerCutoff,
                       MIL_DOUBLE TriangleUpperCutoff,
                       MIL_DOUBLE BinCumulativeValue,
                       MIL_DOUBLE NormalVariation,
                       MIL_DOUBLE FixedDiffThreshold,
                       MIL_INT    CleanMorphSize,
                       const BinarizationMethod BinMethod);

void ExtractDifferences(MIL_ID MilTemplateImage,
                        MIL_ID MilTemplateLumImage,
                        MIL_ID MilTemplateGradientMask,
                        MIL_ID MilTemplateLumGradientMask,
                        MIL_ID MilWarpedTarget,
                        MIL_ID MilDifferenceGrayImage, 
                        MIL_ID MilStructElement,
                        const DifferenceExtractionMethod DiffExtractMethod);

#endif // DEFECT_DETECTION_PROC_FUNC_CAP_AND_CONTENT_H