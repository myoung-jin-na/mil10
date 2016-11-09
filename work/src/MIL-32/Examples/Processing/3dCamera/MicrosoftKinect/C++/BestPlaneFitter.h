//***************************************************************************************/
// 
// File name: BestPlaneFitter.h  
//
// Synopsis:  Declaration of the CBestPlaneFitter, a class that fits a plane on 
//            the best plane of a depth map.
//

#ifndef __BEST_PLANE_FITTER_H
#define __BEST_PLANE_FITTER_H

class CBestPlaneFitter
   {
   public:
      
      // Constructor.
      CBestPlaneFitter(MIL_ID MilSystem, MIL_INT SrcImageSizeX, MIL_INT SrcImageSizeY);

      // Destructor.
      virtual ~CBestPlaneFitter();

      // Function to calculate the plane in the depth map.
      bool CalculateBestPlane(MIL_ID MilDepthMapImage);

      // Function to move the world chains.
      void MoveWorldChains(MIL_ID MilCalibration,
                           MIL_INT SrcCoordinateSystem,
                           MIL_INT DstCoordinateSystem);  
      
      // Function to move the coordinate system on the plane.
      void MoveCoordinateSystemOnPlane(MIL_ID MilCalibration,
                                       MIL_INT TargetCoordinateSystem,
                                       MIL_INT PlaneReferenceCoordinateSystem) const;

      // Function to draw the plane in an image.
      void DrawPlaneInImage(MIL_ID MilGraContext,
                            MIL_ID MilImage,
                            MIL_INT TransparentColor) const;

   protected:
   private:

      // Function to get the world chains of the plane region blob.
      bool GetDepthWorldChains(MIL_INT BlobLabel);

      // Function that calculates the rotation matrix to the plane.
      void CalculatePlaneRotationMatrix();

      // Function that frees the images.
      void FreeFitImages();

      // Functions that frees the chains arrays.
      void FreeWorldChains();

      // The owner system.
      MIL_ID m_MilSystem;

      // The Blob result and feature list.
      MIL_ID m_MilBlobResult;
      MIL_ID m_MilBlobFeatureList;

      // The rotation matrix that will be used to move the coordinate system.
      MIL_FLOAT m_RotationMatrix[3][3];
      MIL_ID    m_MilRotationMatrix;

      // The images necessary to fit the plane.
      MIL_ID m_MilSubDepthMapImage;
      MIL_ID m_MilBorderDepthMap;
      MIL_ID m_MilSmoothDepthMap;
      MIL_ID m_MilValidMask;
      MIL_ID m_MilValidMask8;
      MIL_ID m_MilAngleImage;
      MIL_ID m_MilAngleEdgeImage;
      MIL_ID m_MilLaplacianImage;
      MIL_ID m_MilBasinsImage;

      // The plane 3dmap geometry.
      MIL_ID m_MilPlaneGeometry;

      // The plane fitting result.
      bool        m_Status;
      MIL_INT     m_NbChains;
      MIL_INT     m_MaxNbChains;
      MIL_DOUBLE* m_pWorldChainX;
      MIL_DOUBLE* m_pWorldChainY;
      MIL_DOUBLE* m_pWorldChainZ;
      MIL_DOUBLE  m_Z0;
      MIL_DOUBLE  m_Ax;
      MIL_DOUBLE  m_Ay;
   };

#endif // __BEST_PLANE_FITTER_H