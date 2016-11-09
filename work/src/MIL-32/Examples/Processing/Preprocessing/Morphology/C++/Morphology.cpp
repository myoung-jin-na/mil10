/***************************************************************************************/
/* 
* File name: Morphology.cpp  
*
* Synopsis:  This program contains examples of morphological operations
*            used in different situations.
*            See the PrintHeader() function below for detailed description.
*
*/
#include <mil.h>


/****************************************************************************
Example description.
****************************************************************************/
void PrintHeader()   
   {
   MosPrintf(MIL_TEXT("[EXAMPLE NAME]\n")
             MIL_TEXT("Morphology\n\n")

             MIL_TEXT("[SYNOPSIS]\n")
             MIL_TEXT("This program performs various grayscale\n")
             MIL_TEXT("and binary morphological operations.\n\n")

             MIL_TEXT("[MODULES USED]\n")
             MIL_TEXT("Modules used: application, system, display, buffer,\n")
             MIL_TEXT("graphic, image processing.\n\n"));

   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();
   }

//**********************************
// TOP-HAT FILTERING declarations
//**********************************

static const MIL_TEXT_CHAR* TopHatFilteringFilename = M_IMAGE_PATH MIL_TEXT("/Preprocessing/Dust.tif");

void TopHatFiltering(const MIL_TEXT_CHAR* SrcFilename, 
                     MIL_ID MilSystem, 
                     MIL_ID MilDisplay);


//********************************
// OBJECT SKELETON declarations
//********************************

static const MIL_TEXT_CHAR* ObjectSkeletonFilename = M_IMAGE_PATH MIL_TEXT("/Preprocessing/Circuit.tif");

void ObjectSkeleton(const MIL_TEXT_CHAR* SrcFilename, 
                    MIL_ID MilSystem, 
                    MIL_ID MilDisplay);

//************************************
// OBJECT SEGMENTATION declarations
//************************************

static const MIL_TEXT_CHAR* ObjectSegmentationFilename = M_IMAGE_PATH MIL_TEXT("/Preprocessing/Connector.tif");

void ObjectSegmentation(const MIL_TEXT_CHAR* SrcFilename, 
                        MIL_ID MilSystem, 
                        MIL_ID MilDisplay);

//*********************************************
// MORPHOLOGICAL RECONSTRUCTION declarations
//*********************************************

static const MIL_TEXT_CHAR* MorphologicalReconstructionFilename = M_IMAGE_PATH MIL_TEXT("/Preprocessing/Retina.tif");

void MorphologicalReconstruction(const MIL_TEXT_CHAR* SrcFilename, 
                                 MIL_ID MilSystem, 
                                 MIL_ID MilDisplay);

void MorphoReconstruction(MIL_ID  MilSystem,
                          MIL_ID  SrcImage, 
                          MIL_ID  Seed, 
                          MIL_ID  DstImage, 
                          MIL_INT MaxIter);

//**************************
// Utility sub-functions.
//**************************
void AllocDisplayImage(MIL_ID MilSystem, 
                       MIL_ID SrcImage, 
                       MIL_ID MilDisplay, 
                       MIL_ID& DispProcImage, 
                       MIL_ID& MilOverlay);

void AllocGenPseudoColorLUT(MIL_ID  MilSystem, 
                            MIL_ID  MilDisplay,
                            MIL_INT StartIndex, 
                            MIL_INT EndIndex, 
                            MIL_ID& PseudoColorLut);

//*****************************************************************************
// Main.
//*****************************************************************************
int MosMain(void)
   {
   // Allocate the MIL objects.
   MIL_ID MilApplication = MappAlloc(M_NULL, M_DEFAULT, M_NULL); // Application identifier.
   MIL_ID MilSystem = MsysAlloc(M_DEFAULT, M_SYSTEM_HOST, M_DEFAULT, M_DEFAULT, M_NULL); // System identifier.
   MIL_ID MilDisplay = MdispAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, M_NULL); // Display identifier.

   // Print Header.
   PrintHeader();

   //*********************
   // TOP-HAT FILTERING
   //*********************
   TopHatFiltering(TopHatFilteringFilename, MilSystem, MilDisplay);

   /*******************/
   /* OBJECT SKELETON */
   /*******************/
   ObjectSkeleton(ObjectSkeletonFilename, MilSystem, MilDisplay);

   //***********************
   // OBJECT SEGMENTATION
   //***********************
   ObjectSegmentation(ObjectSegmentationFilename, MilSystem, MilDisplay);

   //***********************
   // OBJECT SEGMENTATION
   //***********************
   MorphologicalReconstruction(MorphologicalReconstructionFilename, MilSystem, MilDisplay);

   // Free allocated objects.
   MdispFree(MilDisplay);
   MsysFree(MilSystem);
   MappFree(MilApplication);

   return 0;
   }

//************************
// TOP-HAT FILTERING
//************************
void TopHatFiltering(const MIL_TEXT_CHAR* SrcFilename, 
                     MIL_ID MilSystem, 
                     MIL_ID MilDisplay)
   {
   MosPrintf(MIL_TEXT("[TOP_HAT FILTERING]\n\n")

             MIL_TEXT("In this example a top-hat filtering operation with\n")
             MIL_TEXT("a dedicated structuring element is used to enhance\n")
             MIL_TEXT("defects in a scene with non-uniform illumination.\n\n"));

   // Restore the image.
   MIL_ID MilSrcImage = MbufRestore(SrcFilename, MilSystem, M_NULL);

   // Allocate a display image.
   MIL_ID MilDispProcImage,         // Display and destination buffer.
          MilOverlayImage;          // Overlay buffer.
   AllocDisplayImage(MilSystem, MilSrcImage, MilDisplay, MilDispProcImage, MilOverlayImage);

   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   // Allocate a vertical structuring element to minimize geometric aberration.
   MIL_ID MilStructElement = MbufAlloc2d(MilSystem, 1, 8, 32, M_STRUCT_ELEMENT, M_NULL);
   MbufClear(MilStructElement, 0);

   // Apply the top-hat filetring.
   MimMorphic(MilSrcImage, MilDispProcImage, MilStructElement, M_TOP_HAT, 1, M_GRAYSCALE);

   MosPrintf(MIL_TEXT("The result of the top-hat filtering is displayed.\n\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   // Segment the image.
   MimBinarize(MilDispProcImage, MilDispProcImage, M_PERCENTILE_VALUE+M_GREATER, 95, M_NULL);

   MosPrintf(MIL_TEXT("The 5%% brightest pixels are thresholded and displayed.\n\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   // Remove small binary noise.
   MimOpen(MilDispProcImage, MilDispProcImage, 1, M_BINARY);

   MosPrintf(MIL_TEXT("A morphological opening is applied to remove small noise.\n\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   // Release the allocated objects
   MbufFree(MilSrcImage);
   MbufFree(MilStructElement);
   MbufFree(MilDispProcImage);
   }

//*******************
// OBJECT SKELETON
//*******************
void ObjectSkeleton(const MIL_TEXT_CHAR* SrcFilename, 
                    MIL_ID MilSystem, 
                    MIL_ID MilDisplay)
   {
   MosPrintf(MIL_TEXT("[OBJECT SKELETON]\n\n")

             MIL_TEXT("In this example a binary thinning operation is used to extract\n")
             MIL_TEXT("the paths in a network. The result of a distance transformation\n")
             MIL_TEXT("is combined with the path in order to determine the thickness of\n")
             MIL_TEXT("the network.\n")
             MIL_TEXT("A pseudo color display is used to enhance the final result.\n\n"));

   // Restore the image.
   MIL_ID MilSrcImage = MbufRestore(SrcFilename, MilSystem, M_NULL);

   // Allocate a display image.
   MIL_ID MilDispProcImage,         // Display and destination buffer.
          MilOverlayImage;          // Overlay buffer.
   AllocDisplayImage(MilSystem, MilSrcImage, MilDisplay, MilDispProcImage, MilOverlayImage);

   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   MIL_INT SizeX, SizeY;
   MbufInquire(MilSrcImage, M_SIZE_X, &SizeX);
   MbufInquire(MilSrcImage, M_SIZE_Y, &SizeY);

   // Allocate the intermediate destination buffers.
   MIL_ID MilDistanceImage = MbufAlloc2d(MilSystem, SizeX, SizeY, 8L+M_UNSIGNED, M_IMAGE+M_PROC, M_NULL);
   MIL_ID MilSkeletonImage = MbufAlloc2d(MilSystem, SizeX, SizeY, 8L+M_UNSIGNED, M_IMAGE+M_PROC, M_NULL);

   // Segment the source image.
   MimBinarize(MilSrcImage, MilSkeletonImage, M_FIXED+M_LESS, 25, M_NULL);
   
   // Close small holes.
   MimClose(MilSkeletonImage, MilSkeletonImage, 1, M_BINARY);

   // Display the segmentation result.
   MbufCopy(MilSkeletonImage, MilOverlayImage);

   MosPrintf(MIL_TEXT("The source image is thresholded and displayed.\n\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   // Compute the distance transform of the object.
   MimDistance(MilSkeletonImage, MilDistanceImage, M_CHAMFER_3_4); 

   // Perform the binary thinning to get the object skeleton.
   MimThin(MilSkeletonImage, MilSkeletonImage, M_TO_SKELETON, M_BINARY3); 

   // Display the thinning result.
   MbufCopy(MilSkeletonImage, MilOverlayImage);
   MosPrintf(MIL_TEXT("A binary thinning is applied and the result skeleton is displayed.\n\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   // Combine the skeleton with the distance image.
   MimArith(MilSkeletonImage, MilDistanceImage, MilDistanceImage, M_AND);
   
   // Allocate and generate the pseudo color LUT */
   MIL_ID MilStatResult = MimAllocResult(MilSystem, 1, M_STAT_LIST, M_NULL);
   MimStat(MilDistanceImage, MilStatResult, M_MAX, M_NULL, M_NULL, M_NULL);
   MIL_INT MaxValue;
   MimGetResult(MilStatResult, M_MAX+M_TYPE_MIL_INT, &MaxValue);

   MIL_ID MilPseudoColorLut;
   AllocGenPseudoColorLUT(MilSystem, MilDisplay, 1, MaxValue-5, MilPseudoColorLut);
   
   // Display the thinning result in pseudo color.
   MimLutMap(MilDistanceImage, MilOverlayImage, MilPseudoColorLut);
   MosPrintf(MIL_TEXT("The thickness of the object is retrieved by combining the object's skeleton\n"));
   MosPrintf(MIL_TEXT("with a distance transform result of the object. The maximum distance value\n"));
   MosPrintf(MIL_TEXT("is %d pixels. A LUT mapping is used to display the skeleton in pseudo colors\n"), MaxValue);
   MosPrintf(MIL_TEXT("based on the object's thickness: blue = thin sections to red = thick sections.\n\n"));

   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   // Release the allocated objects.
   MbufFree(MilSrcImage);
   MbufFree(MilDistanceImage);
   MbufFree(MilSkeletonImage);
   MbufFree(MilPseudoColorLut);
   MbufFree(MilDispProcImage);
   MimFree(MilStatResult);
   }

//***********************
// OBJECT SEGMENTATION
//***********************
void ObjectSegmentation(const MIL_TEXT_CHAR* SrcFilename, 
                        MIL_ID MilSystem, 
                        MIL_ID MilDisplay)
   {
   MosPrintf(MIL_TEXT("[OBJECT SEGMENTATION]\n\n")

             MIL_TEXT("In this example, combinations of binary morphological operations\n")
             MIL_TEXT("are used to segment the object into its principal components.\n\n"));

   // Restore the image.
   MIL_ID MilSrcImage = MbufRestore(SrcFilename, MilSystem, M_NULL);

   // Allocate a display image.
   MIL_ID MilDispProcImage,         // Display and destination buffer.
          MilOverlayImage;          // Overlay buffer.
   AllocDisplayImage(MilSystem, MilSrcImage, MilDisplay, MilDispProcImage, MilOverlayImage);

   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   // Allocate destination buffers.
   MIL_INT SizeX, SizeY;
   MbufInquire(MilSrcImage, M_SIZE_X, &SizeX);
   MbufInquire(MilSrcImage, M_SIZE_Y, &SizeY);
   MIL_ID MilDstImage1 = MbufAlloc2d(MilSystem, SizeX, SizeY, 8L+M_UNSIGNED, M_IMAGE+M_PROC, M_NULL);
   MIL_ID MilDstImage2 = MbufAlloc2d(MilSystem, SizeX, SizeY, 8L+M_UNSIGNED, M_IMAGE+M_PROC, M_NULL);

   // Binarize the source image.
   MimBinarize(MilSrcImage, MilSrcImage, M_BIMODAL+M_GREATER, M_NULL, M_NULL);

   // Segment the horizontal components using a horizontal structuring element.
   MIL_ID MilStructElement = MbufAlloc2d(MilSystem, 10, 1, 32, M_STRUCT_ELEMENT, M_NULL);
   MbufControlNeighborhood(MilStructElement, M_OVERSCAN, M_MIRROR);
   MbufClear(MilStructElement, 1);
   MimMorphic(MilSrcImage,  MilDstImage1,  MilStructElement, M_OPEN, 1, M_BINARY);
   MbufFree(MilStructElement);

   MbufCopy(MilDstImage1, MilOverlayImage);
   MosPrintf(MIL_TEXT("A horizontal opening is used to remove the vertical structures.\n\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   // Segment the vertical components using a vertical structuring element.
   MilStructElement = MbufAlloc2d(MilSystem, 1, 40, 32, M_STRUCT_ELEMENT, M_NULL);
   MbufControlNeighborhood(MilStructElement, M_OVERSCAN, M_MIRROR);
   MbufClear(MilStructElement, 1);
   MimMorphic(MilSrcImage,  MilDstImage2,  MilStructElement, M_OPEN, 1, M_BINARY);
   MimDilate(MilDstImage2, MilDstImage2, 1, M_BINARY);
   MbufFree(MilStructElement);

   MbufCopy(MilDstImage2, MilOverlayImage);
   MosPrintf(MIL_TEXT("A vertical opening is used to isolate the vertical structures.\n\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   // Combine results.
   MimShift(MilDstImage1, MilDstImage1, -2);
   MimShift(MilDstImage2, MilDstImage2, -1);
   MimArith(MilDstImage1, MilDstImage2, MilDstImage1, M_OR);
   MbufCopy(MilDstImage1,MilDstImage2);
   MbufCopyCond(MilSrcImage, MilDstImage1, MilDstImage2, M_EQUAL, 0);

   // Display the segmentation result using pseudo colors.
   MIL_ID MilPseudoColorLut;
   AllocGenPseudoColorLUT(MilSystem, MilDisplay, 255>>2, 255, MilPseudoColorLut);
   MimLutMap(MilDstImage1, MilOverlayImage, MilPseudoColorLut);
   MosPrintf(MIL_TEXT("Results are combined and displayed using pseudo colors.\n"));
   MosPrintf(MIL_TEXT("The twisted pin's sections appear with emphasis in red.\n\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   // Release the allocated objects.
   MbufFree(MilSrcImage);
   MbufFree(MilDispProcImage);
   MbufFree(MilDstImage1);
   MbufFree(MilDstImage2);
   MbufFree(MilPseudoColorLut);
   }

//********************************
// MORPHOLOGICAL RECONSTRUCTION
//********************************
void MorphologicalReconstruction(const MIL_TEXT_CHAR* SrcFilename, 
                                 MIL_ID MilSystem, 
                                 MIL_ID MilDisplay)
   {
   MosPrintf(MIL_TEXT("[MORPHOLOGICAL RECONSTRUCTION]\n\n")

             MIL_TEXT("In this example, a combination of morphological operations and image\n")
             MIL_TEXT("arithmetics is used to perform a morphological reconstruction operation.\n\n"));

   // Restore the image.
   MIL_ID MilSrcImage = MbufRestore(SrcFilename, MilSystem, M_NULL);

   // Allocate a display image.
   MIL_ID MilDispProcImage,         // Display and destination buffer.
          MilOverlayImage;          // Overlay buffer.
   AllocDisplayImage(MilSystem, MilSrcImage, MilDisplay, MilDispProcImage, MilOverlayImage);

   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   // Allocate the seed and the destination buffers.
   MIL_INT SizeX, SizeY;
   MbufInquire(MilSrcImage, M_SIZE_X, &SizeX);
   MbufInquire(MilSrcImage, M_SIZE_Y, &SizeY);
   MIL_ID MilSeedImage = MbufAlloc2d(MilSystem, SizeX, SizeY, 8L+M_UNSIGNED, M_IMAGE+M_PROC, M_NULL);
   MIL_ID MilDstImage  = MbufAlloc2d(MilSystem, SizeX, SizeY, 8L+M_UNSIGNED, M_IMAGE+M_PROC, M_NULL);

   // Generate a seed buffer for the reconstruction.
   MimDilate(MilSrcImage, MilSeedImage, 5, M_GRAYSCALE);
   
   MbufCopy(MilSeedImage, MilDispProcImage);

   MosPrintf(MIL_TEXT("The reconstruction's seed image is displayed.\n\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
   MosGetch();

   // Perform the reconstruction.
   MorphoReconstruction(MilSystem, MilSrcImage, MilSeedImage, MilDstImage, 100);

   // Display the result.
   MbufCopy(MilDstImage, MilDispProcImage);

   MosPrintf(MIL_TEXT("The reconstruction is displayed.\n\n"));
   MosPrintf(MIL_TEXT("Press <Enter> to end.\n\n"));
   MosGetch();

   // Release the allocated objects.
   MbufFree(MilSrcImage);
   MbufFree(MilSeedImage);
   MbufFree(MilDstImage);
   MbufFree(MilDispProcImage);
   }

///////////////////////////////////////////////
// Morphological reconstruction: core algorithm
///////////////////////////////////////////////
//
// Successive dilations of a seed image until 
// contours fits the source image. 
//
// ... : Seed image
// *** : Source image
//
//                     ***
//                    *   *
//        **         *     *
//      **  *       *       *
//     *      *     *  .    *
//    *   ..  *   *  .   .  *
//   *  ..  .. * *  .     . *
// *  ..      .....       . *
// ..                     ...............
//
// ...: Seed image
// ***: Reconstruction
//
//                   *******
//      ******      *       *
//     *      *     *  .    *
//    *   ..  *   *  .   .  *
//   *  ..  .. * *  .     . *
// *  ..      .....       . *
// ..                     ...............
//
//////////////////////////////////////////////
void MorphoReconstruction(MIL_ID  MilSystem,
                          MIL_ID  MilSrcImage, 
                          MIL_ID  MilSeedImage, 
                          MIL_ID  MilDstImage, 
                          MIL_INT MaxIter)
   {
   // Allocate a Mim result object.
   MIL_ID MilCountResult = MimAllocResult(MilSystem, 1, M_COUNT_LIST, M_NULL);

   // Retrieve source image properties.
   MIL_INT SizeX, SizeY;
   MbufInquire(MilSrcImage, M_SIZE_X, &SizeX);
   MbufInquire(MilSrcImage, M_SIZE_Y, &SizeY);

   // Allocate temporary buffers.
   MIL_ID MilCondImage    = MbufAlloc2d(MilSystem, SizeX, SizeY, 8L+M_UNSIGNED, M_IMAGE+M_PROC, M_NULL);
   MIL_ID MilPrevDstImage = MbufAlloc2d(MilSystem, SizeX, SizeY, 8L+M_UNSIGNED, M_IMAGE+M_PROC, M_NULL);
   
   // Init conditions.
   MbufCopy(MilSeedImage, MilDstImage);
   MbufCopy(MilSeedImage, MilPrevDstImage);

   // Perform the morpological reconstruction.
   MimArith(MilDstImage, MilSrcImage, MilCondImage, M_SUB+M_SATURATION);
   MbufCopyCond(MilSrcImage, MilPrevDstImage, MilCondImage, M_EQUAL, 0);
   MimErode(MilPrevDstImage, MilDstImage, 1, M_GRAYSCALE);

   MIL_INT CountDiff=1;
   for(MIL_INT ii=0; ii<MaxIter && (CountDiff>0); ii++)
      {
      MimArith(MilDstImage, MilSrcImage, MilCondImage, M_SUB+M_SATURATION);
      MbufCopyCond(MilSrcImage, MilDstImage, MilCondImage, M_EQUAL, 0);
      MimCountDifference(MilDstImage, MilPrevDstImage, MilCountResult);
      MIL_INT CountDiff;
      MimGetResult(MilCountResult, M_VALUE+M_TYPE_MIL_INT, &CountDiff);
      if(CountDiff>0)
         {
         MbufCopy(MilDstImage, MilPrevDstImage);
         MimErode(MilPrevDstImage, MilDstImage, 1, M_GRAYSCALE);
         }
      }
   MbufFree(MilCondImage);
   MbufFree(MilPrevDstImage);
   MimFree(MilCountResult);
   };


/***************************/
void AllocDisplayImage(MIL_ID MilSystem, 
                       MIL_ID MilSrcImage, 
                       MIL_ID MilDisplay, 
                       MIL_ID& MilDispProcImage, 
                       MIL_ID& MilOverlayImage)
   {
   // Retrieve the source image size.
   MIL_INT SrcSizeX, SrcSizeY;
   MbufInquire(MilSrcImage, M_SIZE_X, &SrcSizeX);
   MbufInquire(MilSrcImage, M_SIZE_Y, &SrcSizeY);

   // Allocate the display image.
   MbufAlloc2d(MilSystem, 
               SrcSizeX,
               SrcSizeY,
               8L+M_UNSIGNED,
               M_IMAGE+M_PROC+M_DISP, 
               &MilDispProcImage);

   MbufCopy(MilSrcImage, MilDispProcImage);

   // Display the image buffer.
   MdispSelect(MilDisplay, MilDispProcImage);

   /* Prepare for overlay annotations. */
   MdispControl(MilDisplay, M_OVERLAY, M_ENABLE);
   MdispInquire(MilDisplay, M_OVERLAY_ID, &MilOverlayImage);
   MdispControl(MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);
   }

/***************************/
void AllocGenPseudoColorLUT(MIL_ID  MilSystem,
                            MIL_ID  MilDisplay,
                            MIL_INT StartIndex, 
                            MIL_INT EndIndex, 
                            MIL_ID& MilPseudoColorLut)
   {
   // Calculate the interpolation param for H.
   MIL_DOUBLE Slope  = 160.0/(MIL_DOUBLE)(StartIndex-EndIndex);
   MIL_DOUBLE Offset = -Slope*(MIL_DOUBLE)EndIndex;

   // Generate the H LUT value.
   MIL_UINT8 HLut[256];
   for(MIL_INT ii=0; ii<256; ii++)
      {
      if(ii<StartIndex)
         HLut[ii]=160;
      else if(ii>EndIndex)
         HLut[ii]=0;
      else
         HLut[ii]=static_cast<MIL_UINT8>(Slope*ii+Offset+0.5);
      }

   // Convert the HSL values to RGB.
   MIL_ID MilTmpBuffer = MbufAllocColor(MilSystem, 3, 256, 1, 8L+M_UNSIGNED, M_IMAGE+M_PROC, M_NULL);
   MbufClear(MilTmpBuffer, M_RGB888(0, 230, 120));
   MbufPutColor(MilTmpBuffer, M_SINGLE_BAND, 0, HLut);
   MimConvert(MilTmpBuffer, MilTmpBuffer, M_HSL_TO_RGB);

   // Map the '0' index to the keying color for overlay transparency.
   MIL_INT KeyingColor;
   MdispInquire(MilDisplay, M_TRANSPARENT_COLOR, &KeyingColor);
   MIL_ID MilTmpChild = MbufChild2d(MilTmpBuffer, 0, 0, 1, 1, M_NULL);
   MbufClear(MilTmpChild, (double) KeyingColor);

   // Copy values to the LUT buffer.
   MilPseudoColorLut = MbufAllocColor(MilSystem, 3, 256, 1, 8L+M_UNSIGNED, M_LUT, M_NULL);
   MbufCopy(MilTmpBuffer, MilPseudoColorLut);

   // Release temporary object.
   MbufFree(MilTmpChild);
   MbufFree(MilTmpBuffer);
   };