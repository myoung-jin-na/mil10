//***************************************************************************************/
// 
// File name: BestPlaneFitter.cpp  
//
// Synopsis:  Implementation of the CBestPlaneFitter, a class that fits a plane on 
//            the best plane of a depth map.
//

#include <Mil.h>
#include "BestPlaneFitter.h"
#include <math.h>

// Floor plane fitting constants.
static const MIL_DOUBLE SUB_FACTOR = 0.5;
static const MIL_INT    SMOOTH_BORDER_SIZE = (MIL_INT)(10 * SUB_FACTOR);
static const MIL_INT    DEPTH_SMOOTHNESS = 80;
static const MIL_INT    BASIN_ERODE_ITER = 2;
static const MIL_INT    MIN_VARIATION = 20;
static const MIL_DOUBLE FIT_OUTLIER_DISTANCE = 10;   // in mm
static const MIL_INT    FLOOR_MESH_SPACING = 100;    // in mm

//****************************************************************************
// Constructor.
//****************************************************************************
CBestPlaneFitter::CBestPlaneFitter(MIL_ID MilSystem, MIL_INT SrcImageSizeX, MIL_INT SrcImageSizeY)
   :  m_MilSystem(MilSystem),
      m_MilBlobResult(M_NULL),
      m_MilBlobFeatureList(M_NULL),

      m_MilSubDepthMapImage(M_NULL),
      m_MilBorderDepthMap(M_NULL),
      m_MilSmoothDepthMap(M_NULL),
      m_MilValidMask(M_NULL),
      m_MilValidMask8(M_NULL),
      m_MilAngleImage(M_NULL),
      m_MilAngleEdgeImage(M_NULL),
      m_MilLaplacianImage(M_NULL),
      m_MilBasinsImage(M_NULL),

      m_MilPlaneGeometry(M_NULL),

      m_MilRotationMatrix(M_NULL),
      
      m_Status(false),
      m_NbChains(0),
      m_MaxNbChains(0)
   {
   // Allocate blob.
   MblobAllocFeatureList(MilSystem, &m_MilBlobFeatureList);
   MblobAllocResult(MilSystem, &m_MilBlobResult);
   MblobSelectFeature(m_MilBlobFeatureList, M_AREA + M_SORT1_DOWN);
   MblobSelectFeature(m_MilBlobFeatureList, M_NUMBER_OF_CHAINED_PIXELS);
   
   // Allocate the mil rotation matrix.
   MbufCreate2d(MilSystem, 3, 3, 32 + M_FLOAT, M_ARRAY, M_DEFAULT,
                M_DEFAULT, (void*)m_RotationMatrix, &m_MilRotationMatrix);

   // Allocate the plane geometry.
   M3dmapAlloc(MilSystem, M_GEOMETRY, M_DEFAULT, &m_MilPlaneGeometry);

   // Allocate the fit images.
   MIL_INT SizeX = (MIL_INT)(SrcImageSizeX * SUB_FACTOR);
   MIL_INT SizeY = (MIL_INT)(SrcImageSizeY * SUB_FACTOR);
   MbufAlloc2d(MilSystem, SizeX, SizeY, 16 + M_UNSIGNED, M_IMAGE + M_PROC, &m_MilSubDepthMapImage);
   MbufAlloc2d(MilSystem, SizeX, SizeY, 16 + M_UNSIGNED, M_IMAGE + M_PROC, &m_MilBorderDepthMap);
   MbufAlloc2d(MilSystem, SizeX, SizeY, 32 + M_FLOAT   , M_IMAGE + M_PROC, &m_MilSmoothDepthMap);
   MbufAlloc2d(MilSystem, SizeX, SizeY, 16 + M_UNSIGNED, M_IMAGE + M_PROC, &m_MilValidMask);
   MbufAlloc2d(MilSystem, SizeX, SizeY, 8  + M_UNSIGNED, M_IMAGE + M_PROC, &m_MilValidMask8);
   MbufAlloc2d(MilSystem, SizeX, SizeY, 8  + M_UNSIGNED, M_IMAGE + M_PROC, &m_MilAngleImage);
   MbufAlloc2d(MilSystem, SizeX, SizeY, 8  + M_UNSIGNED, M_IMAGE + M_PROC, &m_MilAngleEdgeImage);
   MbufAlloc2d(MilSystem, SizeX, SizeY, 8  + M_UNSIGNED, M_IMAGE + M_PROC, &m_MilLaplacianImage);
   MbufAlloc2d(MilSystem, SizeX, SizeY, 16 + M_UNSIGNED, M_IMAGE + M_PROC, &m_MilBasinsImage);
   }

//****************************************************************************
// Destructor.
//****************************************************************************
CBestPlaneFitter::~CBestPlaneFitter()
   {
   // Delete the arrays.
   FreeWorldChains();

   // Free the images.
   FreeFitImages();

   // Free the plane geometry.
   M3dmapFree(m_MilPlaneGeometry);

   // Free the rotation matrix.
   MbufFree(m_MilRotationMatrix);

   // Free blob.
   MblobFree(m_MilBlobResult);
   MblobFree(m_MilBlobFeatureList);
   }

//****************************************************************************
// Function that frees the images.
//****************************************************************************
void CBestPlaneFitter::FreeFitImages()
   {
   if(m_MilSubDepthMapImage)
      {
      MbufFree(m_MilSubDepthMapImage);
      MbufFree(m_MilBorderDepthMap);
      MbufFree(m_MilSmoothDepthMap);
      MbufFree(m_MilValidMask);
      MbufFree(m_MilValidMask8);
      MbufFree(m_MilAngleImage);
      MbufFree(m_MilAngleEdgeImage);
      MbufFree(m_MilLaplacianImage);
      MbufFree(m_MilBasinsImage);
      m_MilSubDepthMapImage = M_NULL;
      }
   }

//****************************************************************************
// Function that frees the chain arrays.
//****************************************************************************
void CBestPlaneFitter::FreeWorldChains()
   {
   if(m_MaxNbChains)
      {
      delete [] m_pWorldChainZ;
      delete [] m_pWorldChainY;
      delete [] m_pWorldChainX;
      m_MaxNbChains = 0;
      }
   }

//****************************************************************************
// Function to calculate the plane in the depth map. Here are the steps of
// the algorithm:
//    - Subsample the depth map.
//    - Get a mask of the valid data of the depth map.
//    - Add a border around the valid data of the depth map to reduce the 
//      influence of the invalid data(MIL_UINT16_MAX) in the filtering.
//      -- Create a morphological contour with Subtract(Original - Eroded).
//      -- Get the depth value of the contour with a And operation.
//      -- Dilate the depth contour.
//      -- Put back the valid depth data in the dilated contour image with a conditional copy.
//    - Smooth the image into a float image to keep as much precision as possible.
//    - Get the gradient angle image and keep only the valid data.
//    - Get the laplacian. The laplacian should be big where the gradient is changing.
//    - Get the gradient magnitude of the angle image. Regions with gradients close to 0
//      are regions where the angle doesn't change. In a depth map, that corresponds to region where
//      the plane stays the same.
//    - Add the laplacian and the gradient image together.
//    - Find the planes region by doing watershed on the previously calculated image.
//    - Take the biggest basin, erode it by a few iteration and fit a plane with the corresponding depth
//      map data.
//****************************************************************************
bool CBestPlaneFitter::CalculateBestPlane(MIL_ID MilDepthMapImage)
   {
   // Init the status.
   m_Status = false;

   // Get the information of the depth map.
   MIL_DOUBLE OrgWorldPosX;
   MIL_DOUBLE OrgWorldPosY;
   MIL_DOUBLE OrgWorldPosZ;
   MIL_DOUBLE OrgPixelSizeX;
   MIL_DOUBLE OrgPixelSizeY;
   MIL_DOUBLE OrgGrayLevelSizeZ;  
   McalInquire(MilDepthMapImage, M_WORLD_POS_X, &OrgWorldPosX);
   McalInquire(MilDepthMapImage, M_WORLD_POS_Y, &OrgWorldPosY);
   McalInquire(MilDepthMapImage, M_WORLD_POS_Z, &OrgWorldPosZ);
   McalInquire(MilDepthMapImage, M_PIXEL_SIZE_X, &OrgPixelSizeX);
   McalInquire(MilDepthMapImage, M_PIXEL_SIZE_Y, &OrgPixelSizeY);
   McalInquire(MilDepthMapImage, M_GRAY_LEVEL_SIZE_Z, &OrgGrayLevelSizeZ);

   // Create the subsampled depth map. The world position must be adjusted since the resize operation
   // performs a translation and a scaling operation since the scaling is done with the center located
   // in the corner of the image.
   MimResize(MilDepthMapImage, m_MilSubDepthMapImage, SUB_FACTOR, SUB_FACTOR, M_NEAREST_NEIGHBOR);
   MIL_DOUBLE PixelSizeX = OrgPixelSizeX / SUB_FACTOR;
   MIL_DOUBLE PixelSizeY = OrgPixelSizeY / SUB_FACTOR;
   
   // Get the world position in the corner of the image.
   MIL_DOUBLE WorldPosX = OrgWorldPosX - (0.5 * OrgPixelSizeX);
   MIL_DOUBLE WorldPosY = OrgWorldPosY - (0.5 * OrgPixelSizeY);

   // Put the world position in the center of the first pixel of the subsampled image.
   WorldPosX += (0.5 * PixelSizeX);
   WorldPosY += (0.5 * PixelSizeY);
      
   // Calibrate the image.
   McalUniform(m_MilSubDepthMapImage, WorldPosX, WorldPosY, PixelSizeX, PixelSizeY, 0, M_DEFAULT);
   McalControl(m_MilSubDepthMapImage, M_GRAY_LEVEL_SIZE_Z, OrgGrayLevelSizeZ);
   McalControl(m_MilSubDepthMapImage, M_WORLD_POS_Z, OrgWorldPosZ);
      
   // Calculate the valid mask image. In depth maps the maximum value of the buffer is used to indicate missing data.
   MimBinarize(m_MilSubDepthMapImage, m_MilValidMask, M_NOT_EQUAL, MIL_UINT16_MAX, M_NULL);
   MimBinarize(m_MilSubDepthMapImage, m_MilValidMask8, M_NOT_EQUAL, MIL_UINT16_MAX, M_NULL);

   // Calculate the depth map with a dilated border.
   MimErode(m_MilValidMask, m_MilBorderDepthMap, 1, M_BINARY);
   MimArith(m_MilValidMask, m_MilBorderDepthMap, m_MilBorderDepthMap, M_SUB);
   MimArith(m_MilBorderDepthMap, m_MilSubDepthMapImage, m_MilBorderDepthMap, M_AND);
   MimDilate(m_MilBorderDepthMap, m_MilBorderDepthMap, SMOOTH_BORDER_SIZE, M_GRAYSCALE);
   MbufCopyCond(m_MilSubDepthMapImage, m_MilBorderDepthMap, m_MilSubDepthMapImage,
                M_NOT_EQUAL, MIL_UINT16_MAX);

   // Get a float smooth version of the depth map.
   MimConvolve(m_MilBorderDepthMap, m_MilSmoothDepthMap, M_DERICHE_FILTER(M_SMOOTH, DEPTH_SMOOTHNESS));  
   
   // Find the angle of the smoothed depth map.
   MimEdgeDetect(m_MilSmoothDepthMap, M_NULL, m_MilAngleImage, M_SOBEL, M_REGULAR_EDGE_DETECT, M_NULL);

   // Find the laplacian of the smoothed depth map.
   MimConvolve(m_MilSmoothDepthMap, m_MilLaplacianImage, M_LAPLACIAN_EDGE2);
   MimShift(m_MilLaplacianImage, m_MilLaplacianImage, -1);
    
   // Keep only the valid data.
   MimArith(m_MilAngleImage, m_MilValidMask8, m_MilAngleImage, M_AND);

   // Calculate the basins.
   MimEdgeDetect(m_MilAngleImage, m_MilAngleEdgeImage, M_NULL, M_SOBEL, M_DEFAULT, M_NULL);
   MimArith(m_MilAngleEdgeImage, m_MilLaplacianImage, m_MilAngleEdgeImage, M_ADD+M_SATURATION);
   MimWatershed(m_MilAngleEdgeImage, M_NULL, m_MilBasinsImage, MIN_VARIATION, M_BASIN);
   MimArith(m_MilBasinsImage, m_MilValidMask, m_MilBasinsImage, M_AND);

   // Calculate the basins blobs.  
   MblobControl(m_MilBlobResult, M_BLOB_IDENTIFICATION, M_LABELED_TOUCHING);
   MblobCalculate(m_MilBasinsImage, M_NULL, m_MilBlobFeatureList, m_MilBlobResult);

   MIL_INT NbBlobs;
   if(MblobGetNumber(m_MilBlobResult, &NbBlobs))
      {
      // Allocate the labels array.
      MIL_INT* Labels = new MIL_INT[NbBlobs];

      // Get the labels of the blobs and select the biggest blob.
      MblobGetResult(m_MilBlobResult, M_LABEL_VALUE + M_TYPE_MIL_INT, &Labels[0]);
      MblobSelect(m_MilBlobResult, M_INCLUDE_ONLY, M_LABEL_VALUE, M_EQUAL, (MIL_DOUBLE)Labels[0], M_NULL);

      // Fill the holes of the biggest blob.
      MblobFill(m_MilBlobResult, m_MilValidMask8, M_EXCLUDED_BLOBS, 0);
      
      // Erode the big basin a little since the data on the border should not be really accurate.
      MimErode(m_MilValidMask8, m_MilValidMask8, BASIN_ERODE_ITER, M_BINARY);

      // Perform a fit on the data of the biggest basin.
      M3dmapSetGeometry(m_MilPlaneGeometry, M_PLANE, M_FIT, (MIL_DOUBLE)m_MilSubDepthMapImage,
                        (MIL_DOUBLE)m_MilValidMask8, FIT_OUTLIER_DISTANCE, M_DEFAULT, M_DEFAULT);

      // Recalculate the blob to get its chains. Put the blob identification to whole image
      // since the previous erode operation might have created more than one blob.
      MblobControl(m_MilBlobResult, M_BLOB_IDENTIFICATION, M_INDIVIDUAL);
      MblobCalculate(m_MilValidMask8, M_NULL, m_MilBlobFeatureList, m_MilBlobResult);

      MIL_INT SingleNbBlobs;
      if(MblobGetNumber(m_MilBlobResult, &SingleNbBlobs))
         {
         // Reset the labels array if necessary.
         if(SingleNbBlobs > NbBlobs)
            {
            delete [] Labels;
            Labels = new MIL_INT[SingleNbBlobs];
            }
         
         // Get the labels of the blobs.
         MblobGetResult(m_MilBlobResult, M_LABEL_VALUE + M_TYPE_MIL_INT, &Labels[0]);
         
         // If the plane fitting has succeeded.
         if(M3dmapInquire(m_MilPlaneGeometry, M_DEFAULT, M_STATUS, M_NULL) == M_SUCCESS)
            {
            // Get the plane parameters.
            M3dmapInquire(m_MilPlaneGeometry, M_DEFAULT, M_FIT_PARAM_AX, &m_Ax);
            M3dmapInquire(m_MilPlaneGeometry, M_DEFAULT, M_FIT_PARAM_AY, &m_Ay);
            M3dmapInquire(m_MilPlaneGeometry, M_DEFAULT, M_FIT_PARAM_Z0, &m_Z0);

            // Calculate the rotation matrix.
            CalculatePlaneRotationMatrix();

            // Get the world chains of the plane fitting region.
            m_Status = GetDepthWorldChains(Labels[0]);
            }
         }

      // Free the labels array.
      delete [] Labels;
      }
   return m_Status;
   }

//****************************************************************************
// Function to transform the world chains.
//****************************************************************************
void CBestPlaneFitter::MoveWorldChains(MIL_ID MilCalibration,
                                       MIL_INT SrcCoordinateSystem,
                                       MIL_INT DstCoordinateSystem)
   {
   McalTransformCoordinate3dList(MilCalibration,
                                 SrcCoordinateSystem, 
                                 DstCoordinateSystem, 
                                 m_NbChains,
                                 m_pWorldChainX,
                                 m_pWorldChainY,
                                 m_pWorldChainZ,
                                 m_pWorldChainX,
                                 m_pWorldChainY,
                                 m_pWorldChainZ,                                
                                 M_DEFAULT);
   }

//****************************************************************************
// Function to move the coordinate system on the plane.
//****************************************************************************
void CBestPlaneFitter::MoveCoordinateSystemOnPlane(MIL_ID MilCalibration,
                                                   MIL_INT TargetCoordinateSystem,
                                                   MIL_INT ReferenceCoordinateSystem) const
   {
   // Move the tool coordinate system on the plane. We move the coordinate system so that the 
   // camera coordinate system moves towards the fitted plane from the center of the camera.
   McalSetCoordinateSystem(MilCalibration,
                           TargetCoordinateSystem,
                           ReferenceCoordinateSystem,
                           M_TRANSLATION + M_ASSIGN,
                           M_NULL,
                           0,
                           0,
                           m_Z0,
                           M_DEFAULT);
      
   // Apply the rotation matrix between the translated camera coordinate system and the 
   // fitted plane.
   McalSetCoordinateSystem(MilCalibration,
                           TargetCoordinateSystem, 
                           TargetCoordinateSystem, 
                           M_ROTATION_MATRIX + M_COMPOSE_WITH_CURRENT,
                           m_MilRotationMatrix,
                           M_DEFAULT,
                           M_DEFAULT,
                           M_DEFAULT,
                           M_DEFAULT);
   }

//****************************************************************************
// Function to draw the plane in an image. Typically MilImage is the overlay.
//****************************************************************************
void CBestPlaneFitter::DrawPlaneInImage(MIL_ID MilGraContext,
                                        MIL_ID MilImage,
                                        MIL_INT TransparentColor) const
   {
   // Get the size of the input image.
   MIL_INT ImageSizeX = MbufInquire(MilImage, M_SIZE_X, M_NULL);
   MIL_INT ImageSizeY = MbufInquire(MilImage, M_SIZE_Y, M_NULL);

   // Get the bounding box of the world chains.
   MIL_DOUBLE MinX = m_pWorldChainX[0];
   MIL_DOUBLE MinY = m_pWorldChainY[0];
   MIL_DOUBLE MaxX = m_pWorldChainX[0];
   MIL_DOUBLE MaxY = m_pWorldChainY[0];
   for(MIL_INT ChainIdx = 1; ChainIdx < m_NbChains; ChainIdx++)
      {
      MinX = m_pWorldChainX[ChainIdx] < MinX ? m_pWorldChainX[ChainIdx] : MinX;
      MinY = m_pWorldChainY[ChainIdx] < MinY ? m_pWorldChainY[ChainIdx] : MinY;
      MaxX = m_pWorldChainX[ChainIdx] > MaxX ? m_pWorldChainX[ChainIdx] : MaxX;
      MaxY = m_pWorldChainY[ChainIdx] > MaxY ? m_pWorldChainY[ChainIdx] : MaxY;                 
      }

   // Draw the lines on the fitted plane.
   MIL_INT LinesMinX = ((MIL_INT)(MinX / FLOOR_MESH_SPACING)) * FLOOR_MESH_SPACING;
   MIL_INT LinesMaxX = ((MIL_INT)(MaxX / FLOOR_MESH_SPACING)) * FLOOR_MESH_SPACING; 
   MIL_INT LinesMinY = ((MIL_INT)(MinY / FLOOR_MESH_SPACING)) * FLOOR_MESH_SPACING;
   MIL_INT LinesMaxY = ((MIL_INT)(MaxY / FLOOR_MESH_SPACING)) * FLOOR_MESH_SPACING;
   MIL_INT NbLinesX = (LinesMaxX - LinesMinX) / FLOOR_MESH_SPACING + 1;
   MIL_INT NbLinesY = (LinesMaxY - LinesMinY) / FLOOR_MESH_SPACING + 1;
   MIL_INT NbLines = NbLinesX + NbLinesY;
   MIL_DOUBLE* pStartX = new MIL_DOUBLE[NbLines];
   MIL_DOUBLE* pStartY = new MIL_DOUBLE[NbLines];
   MIL_DOUBLE* pEndX = new MIL_DOUBLE[NbLines];
   MIL_DOUBLE* pEndY = new MIL_DOUBLE[NbLines];
   MIL_INT LineIdx = 0;
   for(MIL_DOUBLE Y = (MIL_DOUBLE)LinesMinY; Y <= LinesMaxY; Y += FLOOR_MESH_SPACING, LineIdx++)
      {
      pStartX[LineIdx] = MinX;
      pEndX[LineIdx] = MaxX;
      pStartY[LineIdx] = Y;
      pEndY[LineIdx] = Y;
      }

   for(MIL_DOUBLE X = (MIL_DOUBLE)LinesMinX; X <= LinesMaxX; X += FLOOR_MESH_SPACING, LineIdx++)
      {
      pStartX[LineIdx] = X;
      pEndX[LineIdx] = X;
      pStartY[LineIdx] = MinY;
      pEndY[LineIdx] = MaxY;
      }           
   MgraLines(MilGraContext, MilImage, NbLines, pStartX, pStartY, pEndX, pEndY, M_DEFAULT);
   delete [] pEndY;
   delete [] pEndX;
   delete [] pStartY;
   delete [] pStartX;
   
   // Draw the polygon of the blob in the image, masking with transparent color the data outside the blob.
   MIL_ID MilTransparentMask = MbufAllocColor(m_MilSystem, 3, ImageSizeX, ImageSizeY, 8+M_UNSIGNED, M_IMAGE+M_PROC, M_NULL);
   MbufClear(MilTransparentMask, (MIL_DOUBLE)TransparentColor);
   McalAssociate(MilImage, MilTransparentMask, M_DEFAULT);

   MIL_INT MaskDrawColor = TransparentColor == 0 ? 1 : 0;
   MgraColor(MilGraContext, (MIL_DOUBLE)MaskDrawColor);
   MgraLines(MilGraContext, MilTransparentMask, m_NbChains, m_pWorldChainX, m_pWorldChainY, M_NULL, M_NULL, M_POLYGON+M_FILLED);
   MbufCopyCond(MilTransparentMask, MilImage, MilTransparentMask, M_NOT_EQUAL, (MIL_DOUBLE)MaskDrawColor);
   MbufFree(MilTransparentMask);
   }

//****************************************************************************
// Function to calculate the rotation matrix
//****************************************************************************
void CBestPlaneFitter::CalculatePlaneRotationMatrix()
   {
   // Get the unit vector of the normal plane.
   MIL_DOUBLE PlaneNormLength = sqrt(1 + m_Ax*m_Ax + m_Ay*m_Ay);
   MIL_DOUBLE Nz = 1 / PlaneNormLength;
   MIL_DOUBLE Nx = -m_Ax / PlaneNormLength;
   MIL_DOUBLE Ny = -m_Ay / PlaneNormLength;

   // Create the vector in the X direction and normalize it. The vector is created by getting the cross product between the 
   // Y direction vector of the camera coordinate system and the plane normal. Note that the depth map coordinates generated 
   // by the Kinect are expressed in the camera coordinate system.
   MIL_DOUBLE PlaneXVecx = Nz;
   MIL_DOUBLE PlaneXVecy = 0;
   MIL_DOUBLE PlaneXVecz = -Nx;
   MIL_DOUBLE PlaneXVecNorm = sqrt(PlaneXVecx*PlaneXVecx + PlaneXVecz*PlaneXVecz);
   PlaneXVecx /= PlaneXVecNorm;
   PlaneXVecz /= PlaneXVecNorm;

   // Create the vector in the Y direction. The Y direction vector is simply the cross
   // product of the normal and the X vector.
   MIL_DOUBLE PlaneYVecx = (Ny * PlaneXVecz   - PlaneXVecy   * Nz);
   MIL_DOUBLE PlaneYVecy = (PlaneXVecx   * Nz - Nx * PlaneXVecz  );
   MIL_DOUBLE PlaneYVecz = (Nx * PlaneXVecy   - PlaneXVecx * Ny  );   

   // Set the rotation matrix.
   m_RotationMatrix[0][0] = (MIL_FLOAT)PlaneXVecx;
   m_RotationMatrix[1][0] = (MIL_FLOAT)PlaneXVecy;
   m_RotationMatrix[2][0] = (MIL_FLOAT)PlaneXVecz;
   m_RotationMatrix[0][1] = (MIL_FLOAT)PlaneYVecx;
   m_RotationMatrix[1][1] = (MIL_FLOAT)PlaneYVecy;
   m_RotationMatrix[2][1] = (MIL_FLOAT)PlaneYVecz;
   m_RotationMatrix[0][2] = (MIL_FLOAT)Nx;
   m_RotationMatrix[1][2] = (MIL_FLOAT)Ny;
   m_RotationMatrix[2][2] = (MIL_FLOAT)Nz;
   }

//****************************************************************************
// Function to get the world chains of the plane region blob.
//****************************************************************************
bool CBestPlaneFitter::GetDepthWorldChains(MIL_INT BlobLabel)
   {
   // Get the world positions of the blob chains.
   MblobGetResultSingle(m_MilBlobResult, BlobLabel, M_NUMBER_OF_CHAINED_PIXELS + M_TYPE_MIL_INT, &m_NbChains);
   if(m_NbChains > 1)
      {
      // The the current number of chains is greater than the maximum number of chains.
      if(m_NbChains > m_MaxNbChains)
         {
         FreeWorldChains();
         m_pWorldChainX = new MIL_DOUBLE[m_NbChains];
         m_pWorldChainY = new MIL_DOUBLE[m_NbChains];
         m_pWorldChainZ = new MIL_DOUBLE[m_NbChains];
         m_MaxNbChains = m_NbChains;
         }

      // Get the world coordinates of the chains.
      MblobGetResultSingle(m_MilBlobResult, BlobLabel, M_CHAIN_X, m_pWorldChainX);
      MblobGetResultSingle(m_MilBlobResult, BlobLabel, M_CHAIN_Y, m_pWorldChainY);
      McalTransformCoordinateList(m_MilSubDepthMapImage,
                                  M_PIXEL_TO_WORLD,
                                  m_NbChains,
                                  m_pWorldChainX,
                                  m_pWorldChainY,
                                  m_pWorldChainX,
                                  m_pWorldChainY);

      // Calculate the Z of the chains based on the plane equation.
      for(MIL_INT ChainIdx = 0; ChainIdx < m_NbChains; ChainIdx++)
         m_pWorldChainZ[ChainIdx] = m_Z0 + m_Ax * m_pWorldChainX[ChainIdx] + m_Ay * m_pWorldChainY[ChainIdx];
      
      return true;
      }   
   return false;
   }