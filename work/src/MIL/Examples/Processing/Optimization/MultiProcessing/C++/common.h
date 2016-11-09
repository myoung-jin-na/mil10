//***************************************************************************************
//
// File name: common.h
//
// Synopsis:  Includes all common headers and defines some common constants.
//

#ifndef COMMON_H
#define COMMON_H

#include <mil.h>
#include <cmath>

//Image path
#define EXAMPLE_IMAGE_PATH          M_IMAGE_PATH MIL_TEXT("Multiprocessing/")

//Font information
#define NORMAL_FONT_TYPE  MIL_FONT_NAME(M_FONT_DEFAULT_TTF)
#define BOLD_FONT_TYPE  MIL_FONT_NAME(M_FONT_DEFAULT_TTF MIL_TEXT(":Bold"))
static const MIL_DOUBLE LARGE_FONT = 16;
static const MIL_DOUBLE MEDIUM_FONT = 14;
static const MIL_DOUBLE SMALL_FONT = 12;

//Thread information
enum ThreadEvents { enRun, enKill, enInvalidEvent };
static const MIL_INT NUM_EVENTS = enInvalidEvent;
static const MIL_INT STRING_SIZE = 128;

//MP
const MIL_INT MIN_MP_CORES = 2;
const MIL_INT MAX_MP_CORES = 32;

//Structure for rectangular areas.  Used to identify button areas.
struct RectStruct
   {
   MIL_INT StartX;
   MIL_INT StartY;
   MIL_INT EndX;
   MIL_INT EndY;
   MIL_ID MilRectBuffer;
   };

//Common headers to include
#include "Dispatcher.h"
#include "MPProcessing.h"
#include "MPRotateProcessing.h"
#include "MPWarpProcessing.h"
#include "MPMenuButton.h"
#include "MPMenu.h"

#endif
