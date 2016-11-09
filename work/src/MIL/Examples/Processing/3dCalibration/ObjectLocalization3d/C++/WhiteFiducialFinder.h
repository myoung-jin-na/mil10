//***************************************************************************************
//
// File name: WhiteFiducialFinder.h
//
// Synopsis:  Defines a class used to find the hole inside the black fiducial.
//

#ifndef WHITE_FIDUCIAL_FINDER_H
#define WHITE_FIDUCIAL_FINDER_H

//*****************************************************************************
// Class used to find the hole inside the black fiducial.
//*****************************************************************************
class CWhiteFiducialFinder
   {
   public:
      explicit CWhiteFiducialFinder(MIL_ID MilSystem);
      ~CWhiteFiducialFinder();

      bool Find(MIL_ID MilBinarizedImage, MIL_DOUBLE ApproxBlobX, MIL_DOUBLE ApproxBlobY);

      void Draw(MIL_ID MilOverlayImage) const;

      inline MIL_DOUBLE GetCoGX() const;
      inline MIL_DOUBLE GetCoGY() const;

   private:
      // Initialization value, meaning the fiducial index is invalid.
      static const MIL_INT NOT_FOUND = 0;

      // Disallow copy.
      CWhiteFiducialFinder(const CWhiteFiducialFinder&);
      CWhiteFiducialFinder& operator=(const CWhiteFiducialFinder&);

      void ReserveArraySpace(MIL_INT MinArraySize);
      void AllocateArrays(MIL_INT ArraySize);
      void DestroyArrays();

      MIL_ID      m_MilFeatureList; // MIL object used to specified features to calculate with milblob.
      MIL_ID      m_MilBlobResult;  // MIL object used to contain found blobs.

      MIL_DOUBLE* m_CoGXArray;      // X of center of gravity of the blobs.
      MIL_DOUBLE* m_CoGYArray;      // Y of center of gravity of the blobs.
      MIL_INT     m_ArraySize;      // Maximum size of the above arrays.

      MIL_INT     m_FiducialIndex;  // Index of the blob that is the white hole.
   };

//*****************************************************************************
// Return the X coordinate of the center of gravity of the white fiducial.
// Find() must have been called before using this function.
//*****************************************************************************
inline MIL_DOUBLE CWhiteFiducialFinder::GetCoGX() const
   {
   return m_CoGXArray[m_FiducialIndex];
   }

//*****************************************************************************
// Return the Y coordinate of the center of gravity of the white fiducial.
// Find() must have been called before using this function.
//*****************************************************************************
inline MIL_DOUBLE CWhiteFiducialFinder::GetCoGY() const
   {
   return m_CoGYArray[m_FiducialIndex];
   }

#endif
