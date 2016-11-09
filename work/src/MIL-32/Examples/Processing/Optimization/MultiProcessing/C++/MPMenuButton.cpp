//***************************************************************************************/
// 
// File name: MenuButton.cpp  
//
// Synopsis:  Implements the CMPMenuButton class.
//
#include "common.h"
#if M_MIL_USE_LINUX
#include <stdlib.h>
#endif
static const MIL_INT TEXT_GAP_UNPRESSED = 8;
static const MIL_INT TEXT_GAP_PRESSED = 10;

static const MIL_TEXT_CHAR* const NORMAL_UNPRESSED_BUTTON = EXAMPLE_IMAGE_PATH MIL_TEXT("NormalButtonUnpressed.mim"); 
static const MIL_TEXT_CHAR* const NORMAL_PRESSED_BUTTON = EXAMPLE_IMAGE_PATH MIL_TEXT("NormalButtonPressed.mim"); 

static const MIL_TEXT_CHAR* const SMALL_UNPRESSED_BUTTON = EXAMPLE_IMAGE_PATH MIL_TEXT("SmallButtonUnpressed.mim"); 
static const MIL_TEXT_CHAR* const SMALL_PRESSED_BUTTON = EXAMPLE_IMAGE_PATH MIL_TEXT("SmallButtonPressed.mim"); 


//*******************************************************************************
// Constructor.  Allocates and initializes the menu button
//*******************************************************************************
CMPMenuButton::CMPMenuButton()
: m_MilButtonBuffer(0), m_MilGraphicsContext(0), m_IsToggle(true), m_IsPressed(true),
  m_IsDisabled(false), m_MilPressedButton(0), m_MilUnpressedButton(0)
   {
   }

//*******************************************************************************
// Destructor.  Deallocates the menu button
//*******************************************************************************
CMPMenuButton::~CMPMenuButton()
   {
   //Destroy the button
   Destroy();
   }

//*******************************************************************************
// Create.  Creates the menu button
//*******************************************************************************
void CMPMenuButton::Create(const MIL_TEXT_CHAR* Text, MIL_ID MilParentBuffer, RectStruct Rect, 
   bool IsToggle)
   {
   //Allocate and configure graphics context for menu text
   MbufInquire(MilParentBuffer, M_OWNER_SYSTEM, &m_MilButtonSystem);
   MgraAlloc(m_MilButtonSystem, &m_MilGraphicsContext);

   MgraControl(m_MilGraphicsContext, M_BACKGROUND_MODE, M_TRANSPARENT);
   MgraColor(m_MilGraphicsContext, M_COLOR_DARK_BLUE);

   MgraControl(m_MilGraphicsContext, M_FONT_SIZE, SMALL_FONT);
   MgraFont(m_MilGraphicsContext, NORMAL_FONT_TYPE);

   //Decide which button image to use
   MIL_INT NormalButtonSizeX = MbufDiskInquire(NORMAL_PRESSED_BUTTON, M_SIZE_X, M_NULL);
   MIL_INT SmallButtonSizeX = MbufDiskInquire(SMALL_PRESSED_BUTTON, M_SIZE_X, M_NULL);

   MIL_INT UserRectSizeX = Rect.EndX-Rect.StartX+1;
   MIL_INT UserRectSizeY = Rect.EndY-Rect.StartY+1;

   if (abs((MIL_INT32)(UserRectSizeX-NormalButtonSizeX)) < abs((MIL_INT32)(UserRectSizeX-SmallButtonSizeX)))
      {
      //Choose normal button
      MbufRestore(NORMAL_PRESSED_BUTTON, m_MilButtonSystem, &m_MilPressedButton);
      MbufRestore(NORMAL_UNPRESSED_BUTTON, m_MilButtonSystem, &m_MilUnpressedButton);
      }
   else
      {
      //Choose small button
      MbufRestore(SMALL_PRESSED_BUTTON, m_MilButtonSystem, &m_MilPressedButton);
      MbufRestore(SMALL_UNPRESSED_BUTTON, m_MilButtonSystem, &m_MilUnpressedButton);
      }

   m_IsPressed = true;

   //Create the button on the menu buffer
   MbufChild2d(MilParentBuffer, 
               Rect.StartX, Rect.StartY, 
               UserRectSizeX, 
               UserRectSizeY,                
               &m_MilButtonBuffer);

   m_ButtonRect.StartX  = Rect.StartX;
   m_ButtonRect.EndX    = Rect.EndX;
   m_ButtonRect.StartY  = Rect.StartY;
   m_ButtonRect.EndY    = Rect.EndY;

   m_IsToggle = IsToggle;

   Push(Text);
   }

//*******************************************************************************
// Destroy.  Destroy the menu button
//*******************************************************************************
void CMPMenuButton::Destroy()
   {
   //Free contexts and buffers
   if (m_MilGraphicsContext)
      {
      MgraFree(m_MilGraphicsContext);
      m_MilGraphicsContext = 0;
      }

   if (m_MilButtonBuffer)
      {
      MbufFree(m_MilButtonBuffer);
      m_MilButtonBuffer=0;
      }

   if (m_MilPressedButton)
      {
      MbufFree(m_MilPressedButton);
      m_MilPressedButton=0;
      }

   if (m_MilUnpressedButton)
      {
      MbufFree(m_MilUnpressedButton);
      m_MilUnpressedButton=0;
      }
   }

//*******************************************************************************
// Push.  Push the button
//*******************************************************************************
void CMPMenuButton::Push(const MIL_TEXT_CHAR* Text)
   {
   if (m_MilButtonBuffer && !m_IsDisabled)
      {
      m_IsPressed = !m_IsPressed && m_IsToggle;

      //Load the appropriate image according to whether the button is pushed or not
      if (m_IsPressed)
         {
         //Load pressed button image
         MimResize(m_MilPressedButton, m_MilButtonBuffer, M_FILL_DESTINATION, M_FILL_DESTINATION, M_BILINEAR);
         MgraText(m_MilGraphicsContext, m_MilButtonBuffer, TEXT_GAP_PRESSED, TEXT_GAP_PRESSED, Text);
         }
      else
         {
         //Load unpressed button image
         MimResize(m_MilUnpressedButton, m_MilButtonBuffer, M_FILL_DESTINATION, M_FILL_DESTINATION, M_BILINEAR);
         MgraText(m_MilGraphicsContext, m_MilButtonBuffer, TEXT_GAP_UNPRESSED, TEXT_GAP_UNPRESSED, Text);
         }
      }
   }

//*****************************************************************************
// Disable.  Disables the button, it can no longer be pushed unless it is 
// reenabled.
//*****************************************************************************
void CMPMenuButton::Disable(const MIL_TEXT_CHAR* Text)
   { 
   m_IsDisabled = true;

   if (m_MilButtonBuffer)
      {
      MimResize(m_MilPressedButton, m_MilButtonBuffer, M_FILL_DESTINATION, M_FILL_DESTINATION, M_BILINEAR);
      MgraText(m_MilGraphicsContext, m_MilButtonBuffer, TEXT_GAP_PRESSED, TEXT_GAP_PRESSED, Text);
      }
   }

//*****************************************************************************
// Enable.  Enables the button.  The button can be pushed once it is enabled.
//*****************************************************************************
void CMPMenuButton::Enable(const MIL_TEXT_CHAR* Text)
   { 
   m_IsDisabled = false;

   if (m_MilButtonBuffer)
      {
      if (m_IsPressed)
         {
         //Load pressed button image if it is pressed
         MimResize(m_MilPressedButton, m_MilButtonBuffer, M_FILL_DESTINATION, M_FILL_DESTINATION, M_BILINEAR);
         MgraText(m_MilGraphicsContext, m_MilButtonBuffer, TEXT_GAP_PRESSED, TEXT_GAP_PRESSED, Text);
         }
      else
         {
         //Load unpressed button image
         MimResize(m_MilUnpressedButton, m_MilButtonBuffer, M_FILL_DESTINATION, M_FILL_DESTINATION, M_BILINEAR);
         MgraText(m_MilGraphicsContext, m_MilButtonBuffer, TEXT_GAP_UNPRESSED, TEXT_GAP_UNPRESSED, Text);
         }
      }
   }
