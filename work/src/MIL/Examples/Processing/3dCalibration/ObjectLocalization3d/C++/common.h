//***************************************************************************************
//
// File name: common.h
//
// Synopsis:  Includes all common headers and defines some common constants.
//

#ifndef COMMON_H
#define COMMON_H

#include <mil.h>
#include <float.h>
#include <math.h>

// Directory containing all images used in this example.
#define EXAMPLE_IMAGE_PATH    M_IMAGE_PATH MIL_TEXT("ObjectLocalization3d/")

// Minimum number of features to find on the object to be able to locate it.
const MIL_INT MIN_NB_FEATURES = 4;

// Maximum number of features that could be detected in a single image.
const MIL_INT MAX_NB_FEATURES = 20;


//*****************************************************************************
// Structure that contains all the necessary coordinates information for a call
// to McalList().
//*****************************************************************************
struct SObjectFeatures
   {
   MIL_DOUBLE m_XPixArray  [MAX_NB_FEATURES];
   MIL_DOUBLE m_YPixArray  [MAX_NB_FEATURES];
   MIL_DOUBLE m_XWorldArray[MAX_NB_FEATURES];
   MIL_DOUBLE m_YWorldArray[MAX_NB_FEATURES];
   MIL_DOUBLE m_ZWorldArray[MAX_NB_FEATURES];
   MIL_INT    m_NumPoint;
   };

#endif
