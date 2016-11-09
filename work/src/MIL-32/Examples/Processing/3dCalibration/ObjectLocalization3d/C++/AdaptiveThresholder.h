//***************************************************************************************
//
// File name: AdaptiveThresholder.h
//
// Synopsis:  Defines a class used to binarize an image using local thresholds.
//

#ifndef ADAPTIVE_THRESHOLDER_H
#define ADAPTIVE_THRESHOLDER_H

//*****************************************************************************
// Class used to wrap the "adaptive thresholding" operation and its necessary
// resources. Can be used to binarize an image using local thresholds.
//*****************************************************************************
class CAdaptiveThresholder
   {
   public:
      CAdaptiveThresholder(MIL_ID MilSystem, MIL_INT SizeX, MIL_INT SizeY);
      ~CAdaptiveThresholder();

      void Binarize(MIL_ID MilGrayscaleImage, MIL_ID MilBinarizedImage);

   private:
      // Disallow copy
      CAdaptiveThresholder(const CAdaptiveThresholder&);
      CAdaptiveThresholder& operator=(const CAdaptiveThresholder&);

      MIL_ID m_MilMinImage;   // Temporary image used to hold an eroded version of the source image
      MIL_ID m_MilMaxImage;   // Temporary image used to hold a dilated version of the source image
   };

#endif
