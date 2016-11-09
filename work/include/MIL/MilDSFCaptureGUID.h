
/************************************************************************/
/*
*
* Filename     :  MilCaptureFilterUids.H
* Revision     :  10.00.2564
* Content      :  This file contains the GUIDs relative to the  
*                 MIL Capture DirectShow Filter.
*
* Comments     :  Some defines may be here but not yet
*                 implemented in the library.
*
* Copyright © Matrox Electronic Systems Ltd., 1992-2014.
* All Rights Reserved
*************************************************************************/

#pragma once


// {54CEB8AD-D0DB-474D-B3A8-2282F7249192}
DEFINE_GUID(CLSID_MilCapture,
   0x54ceb8ad, 0xd0db, 0x474d, 0xb3, 0xa8, 0x22, 0x82, 0xf7, 0x24, 0x91, 0x92);

//
// IMilCapture's GUID
//
// {39450A9A-2ACC-42A2-8D40-0FC3DA005767}
DEFINE_GUID(IID_IMilCapture, 
   0x39450a9a, 0x2acc, 0x42a2, 0x8d, 0x40, 0xf, 0xc3, 0xda, 0x0, 0x57, 0x67);

// {88D68975-9802-444F-9B37-1EC5E978C14A}
DEFINE_GUID(IID_IMilCapturePin, 
   0x88d68975, 0x9802, 0x444f, 0x9b, 0x37, 0x1e, 0xc5, 0xe9, 0x78, 0xc1, 0x4a);
