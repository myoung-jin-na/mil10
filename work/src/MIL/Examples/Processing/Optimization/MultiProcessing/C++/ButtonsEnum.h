//***************************************************************************************
//
// File name: ButtonsList.h
//
// Synopsis:  enumeration of all buttons used for the menu.
//

#ifndef BUTTONSLIST_H
#define BUTTONSLIST_H

//Enumeration of the buttons.  This will serve as identifiers for the buttons.
enum MPButtons 
   {
   enTotalThreads,
   enTotalThreadsLeft,
   enTotalThreadsRight,
   enShowDisplay,
   enProcThread,
   enProcThreadLeft,
   enProcThreadRight,
   enApplyToAllThreads,
   enRunProcessing, 
   enRunDisplay, 
   enMemoryBank,
   enMemoryBankLeft,
   enMemoryBankRight,
   enMP,  
   enCoreMax, 
   enCoreMaxLeft, 
   enCoreMaxRight,
   enCoreSharing, 
   enMPPriority,
   enMPPriorityLeft,
   enMPPriorityRight,
   enCore0,    enCore1,    enCore2,    enCore3,
   enCore4,    enCore5,    enCore6,    enCore7, 
   enCore8,    enCore9,    enCore10,   enCore11, 
   enCore12,   enCore13,   enCore14,   enCore15, 
   enCore16,   enCore17,   enCore18,   enCore19, 
   enCore20,   enCore21,   enCore22,   enCore23, 
   enCore24,   enCore25,   enCore26,   enCore27,
   enCore28,   enCore29,   enCore30,   enCore31
   };

//Enumeration of processing information 
enum MPInfo    
   {  
   enCurProcFPS,
   enTotalFPS
   }; 

//Constants to help mark the start and end points of the enumerations
static const MIL_INT FIRST_BUTTON = enTotalThreads;
static const MIL_INT LAST_BUTTON = enCore31;
static const MIL_INT NUM_BUTTONS = LAST_BUTTON+1;
static const MIL_INT FIRST_CORE = enCore0;
static const MIL_INT LAST_CORE = enCore31;
static const MIL_INT MAX_CORES = (LAST_CORE-FIRST_CORE)+1;
static const MIL_INT FIRST_INFO = enCurProcFPS;
static const MIL_INT LAST_INFO = enTotalFPS;
static const MIL_INT NUM_INFO = LAST_INFO+1;

#endif