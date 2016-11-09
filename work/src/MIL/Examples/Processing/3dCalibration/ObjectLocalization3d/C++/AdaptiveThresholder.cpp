//***************************************************************************************/
// 
// File name: AdaptiveThresholder.cpp  
//
// Synopsis:  Implements the CAdaptiveThresholder class.
//

#include "common.h"
#include "AdaptiveThresholder.h"

//*****************************************************************************
// Constants.
//*****************************************************************************

// Number of iterations for dilation and erosion operations; define the kernel size for the
// max and min operations.
static const MIL_INT NB_MORPH_ITERATIONS = 6;

// Minimum contrast between a pixel and the average of its surrounding to consider this
// pixel as white.
static const MIL_DOUBLE CONTRAST_THRESHOLD = 15.0;

//*****************************************************************************
// Constructor. Allocates temporary images.
//*****************************************************************************
CAdaptiveThresholder::CAdaptiveThresholder(MIL_ID MilSystem, MIL_INT SizeX, MIL_INT SizeY)
   {
   MbufAlloc2d(MilSystem, SizeX, SizeY, 8+M_UNSIGNED, M_IMAGE+M_PROC, &m_MilMinImage);
   MbufAlloc2d(MilSystem, SizeX, SizeY, 8+M_UNSIGNED, M_IMAGE+M_PROC, &m_MilMaxImage);
   }

//*****************************************************************************
// Destructor. Free all MIL objects.
//*****************************************************************************
CAdaptiveThresholder::~CAdaptiveThresholder()
   {
   MbufFree(m_MilMaxImage);
   MbufFree(m_MilMinImage);
   }

//*****************************************************************************
// Perform adaptive thresholding.
//*****************************************************************************
void CAdaptiveThresholder::Binarize(MIL_ID MilGrayscaleImage, MIL_ID MilBinarizedImage)
   {
   // Grayscale dilation is the same as finding the maximum value in a neighborhood.
   MimDilate(MilGrayscaleImage, m_MilMaxImage, NB_MORPH_ITERATIONS, M_GRAYSCALE);

   // Grayscale erosion is the same as finding the minimum value in a neighborhood.
   MimErode(MilGrayscaleImage,  m_MilMinImage, NB_MORPH_ITERATIONS, M_GRAYSCALE);

   // m_MilMinImage := (Max + Min)/2 (a kind of average)
   MimArithMultiple(m_MilMaxImage, 2.0, m_MilMinImage, M_NULL, M_NULL, m_MilMinImage, M_WEIGHTED_AVERAGE, M_DEFAULT);

   // m_MilMinImage := Source - (Max + Min)/2, saturated to 0
   MimArith(MilGrayscaleImage, m_MilMinImage, m_MilMinImage, M_SUB+M_SATURATION);

   // Binarized pixel is white if   ( Source - (Max + Min)/2 ) > CONTRAST_THRESHOLD
   MimBinarize(m_MilMinImage, MilBinarizedImage, M_FIXED+M_GREATER_OR_EQUAL, CONTRAST_THRESHOLD, M_NULL);
   }
