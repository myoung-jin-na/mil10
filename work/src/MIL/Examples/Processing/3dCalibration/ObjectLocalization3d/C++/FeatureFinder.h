//***************************************************************************************
//
// File name: FeatureFinder.h
//
// Synopsis:  Defines a class used to find fiducials in the image.
//

#ifndef FEATURE_FINDER_H
#define FEATURE_FINDER_H

// Forward declarations.
struct SObjectFeatures;

class CAdaptiveThresholder;
class CBlackFiducialFinder;
class CWhiteFiducialFinder;
class CPharmacodeReader   ;

//*****************************************************************************
// Class used to find fiducials in the image.
//*****************************************************************************
class CFeatureFinder
   {
   public:
      CFeatureFinder(MIL_ID MilSystem, MIL_INT SizeX, MIL_INT SizeY);
      ~CFeatureFinder();

      void Find(MIL_ID MilDisplay, MIL_ID MilDisplayImage, SObjectFeatures* pObjectFeatures);

   private:
      // Disallow copy.
      CFeatureFinder(const CFeatureFinder&);
      CFeatureFinder& operator=(const CFeatureFinder&);

      // Objects used for subparts of the algorithm.
      CAdaptiveThresholder* m_pAdaptiveThresholder;
      CBlackFiducialFinder* m_pBlackFiducialFinder;
      CWhiteFiducialFinder* m_pWhiteFiducialFinder;
      CPharmacodeReader*    m_pPharmacodeReader   ;

      // Temporary MIL objects used by the algorithm.
      MIL_ID                m_MilBinarizedImage;
      MIL_ID                m_MilBinarizedChild;
   };

#endif
