// MdispMFCView.cpp : implementation of the CMdispMFCView class
//

#include "stdafx.h"
#include "MdispMFC.h"

#include "MdispMFCDoc.h"
#include "MdispMFCView.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Static data member initialization
const MIL_INT CMdispMFCView::s_CompressionType[] = 
                                 {M_NULL, M_JPEG_LOSSY, M_JPEG_LOSSLESS};
const MIL_INT CMdispMFCView::s_AsynchronousMaxUpdateRate[] = 
                                 {M_DISABLE, 1, 5, 10, 15, 30, M_INFINITE};
const MIL_INT CMdispMFCView::s_QFactor[] = 
                                 {60, 70, 75, 80, 82, 85, 87, 90, 92, 95, 99};

const UINT CMdispMFCView::s_FirstCompressionTypeControlInRange = ID_COMPRESSION_NONE;
const UINT CMdispMFCView::s_FirstAsynchronousMaxUpdateRateControlInRange 
                                                       = ID_ASYNCHRONOUSMODE_DISABLED;
const UINT CMdispMFCView::s_FirstQFactorControlInRange = ID_QFACTOR_60;

MIL_INT MFTYPE MouseFct(MIL_INT HookType, MIL_ID EventID, void* UserDataPtr)
   {
   CMdispMFCView* pCurrentView = (CMdispMFCView*)UserDataPtr;
   
   if(pCurrentView)
      {
      MOUSEPOSITION MousePosition;
      MdispGetHookInfo(EventID, M_MOUSE_POSITION_X,         &MousePosition.m_DisplayPositionX);
      MdispGetHookInfo(EventID, M_MOUSE_POSITION_Y,         &MousePosition.m_DisplayPositionY);
      MdispGetHookInfo(EventID, M_MOUSE_POSITION_BUFFER_X,  &MousePosition.m_BufferPositionX);
      MdispGetHookInfo(EventID, M_MOUSE_POSITION_BUFFER_Y,  &MousePosition.m_BufferPositionY);
      
      pCurrentView->SetMousePosition(MousePosition);
      pCurrentView->SendMessage(WM_COMMAND, ID_MOUSE_UPDATE_TEXT);
      }
   return 0;
   }
MIL_INT MFTYPE GraphicListModifiedHookFct(MIL_INT HookType, MIL_ID EventID, void* UserDataPtr)
   {
   CMdispMFCView* pCurrentView = (CMdispMFCView*)UserDataPtr;

   if(pCurrentView)
      {
      MIL_INT State = M_NULL;
      MgraGetHookInfo(EventID, M_INTERACTIVE_GRAPHIC_STATE, &State);

      if((State != M_STATE_WAITING_FOR_CREATION) && (State != M_STATE_BEING_CREATED))
         {
         pCurrentView->ResetPrimitiveCreation();
         }
      }
   return 0;
   }
/////////////////////////////////////////////////////////////////////////////
// CMdispMFCView

IMPLEMENT_DYNCREATE(CMdispMFCView, CView)

BEGIN_MESSAGE_MAP(CMdispMFCView, CView)
   //{{AFX_MSG_MAP(CMdispMFCView)
   ON_COMMAND(ID_GRAB_START, OnGrabStart)
   ON_UPDATE_COMMAND_UI(ID_GRAB_START, OnUpdateGrabStart)
   ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateSave)
   ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateSaveAs)
   ON_COMMAND(ID_GRAB_STOP, OnGrabStop)
   ON_COMMAND(ID_MOUSE_UPDATE_TEXT, OnMouseUpdateText)
   ON_UPDATE_COMMAND_UI(ID_GRAB_STOP, OnUpdateGrabStop)
   ON_WM_DESTROY()
   ON_COMMAND(ID_OVERLAY, OnOverlay)
   ON_UPDATE_COMMAND_UI(ID_OVERLAY, OnUpdateOverlay)
   ON_WM_SIZE()
   ON_WM_KEYDOWN()
   ON_WM_TIMER()
   ON_COMMAND(ID_ZOOMIN, OnZoomin)
   ON_UPDATE_COMMAND_UI(ID_ZOOMIN, OnUpdateZoomin)
   ON_COMMAND(ID_ZOOMOUT, OnZoomout)
   ON_UPDATE_COMMAND_UI(ID_ZOOMOUT, OnUpdateZoomout)
   ON_COMMAND(ID_NOZOOM, OnNoZoom)
   ON_UPDATE_COMMAND_UI(ID_NOZOOM, OnUpdateNoZoom)
   ON_COMMAND(ID_SCALEDISPLAY, OnScaleDisplay)
   ON_UPDATE_COMMAND_UI(ID_SCALEDISPLAY, OnUpdateScaleDisplay)
   ON_CONTROL(CBN_SELCHANGE, IDC_CB_VIEW_MODE, OnViewModeChange)
   // Standard printing commands
   ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
   ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
   ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
   ON_WM_SIZING()
   ON_COMMAND_RANGE(ID_COMPRESSION_NONE, ID_COMPRESSION_LOSSLESS, &CMdispMFCView::OnCompression)
   ON_UPDATE_COMMAND_UI_RANGE(ID_COMPRESSION_NONE, ID_COMPRESSION_LOSSLESS, &CMdispMFCView::OnUpdateCompression)
   ON_COMMAND_RANGE(ID_ASYNCHRONOUSMODE_DISABLED, ID_ASYNCHRONOUSMODE_MAXIMUMRATE, &CMdispMFCView::OnAsynchronousMode)
   ON_UPDATE_COMMAND_UI_RANGE(ID_ASYNCHRONOUSMODE_DISABLED, ID_ASYNCHRONOUSMODE_MAXIMUMRATE, &CMdispMFCView::OnUpdateAsynchronousMode)
   ON_COMMAND_RANGE(ID_QFACTOR_DEFAULT, ID_QFACTOR_99, &CMdispMFCView::OnQFactor)
   ON_UPDATE_COMMAND_UI_RANGE(ID_QFACTOR_DEFAULT, ID_QFACTOR_99, &CMdispMFCView::OnUpdateQFactor)
   ON_COMMAND(ID_RESTRICTCURSOR, &CMdispMFCView::OnRestrictCursor)
   ON_UPDATE_COMMAND_UI(ID_RESTRICTCURSOR, &CMdispMFCView::OnUpdateRestrictCursor)
   ON_COMMAND(ID_DISPLAY_GRAPHICSANNOTATIONS, &CMdispMFCView::OnGraphicsAnnotations)
   ON_UPDATE_COMMAND_UI(ID_DISPLAY_GRAPHICSANNOTATIONS, &CMdispMFCView::OnUpdateGraphicsAnnotations)
   ON_COMMAND(ID_GRA_RECTANGLE, &CMdispMFCView::OnGraRectangle)
   ON_UPDATE_COMMAND_UI(ID_GRA_RECTANGLE, &CMdispMFCView::OnUpdateGraRectangle)
   ON_COMMAND(ID_GRA_CIRCLE, &CMdispMFCView::OnGraCircle)
   ON_UPDATE_COMMAND_UI(ID_GRA_CIRCLE, &CMdispMFCView::OnUpdateGraCircle)
   ON_COMMAND(ID_GRA_POLYGON, &CMdispMFCView::OnGraPolygon)
   ON_UPDATE_COMMAND_UI(ID_GRA_POLYGON, &CMdispMFCView::OnUpdateGraPolygon)
   ON_COMMAND(ID_GRA_COLOR, &CMdispMFCView::OnGraChooseColor)
   ON_UPDATE_COMMAND_UI(ID_GRA_COLOR, &CMdispMFCView::OnUpdateGraChooseColor)
   ON_COMMAND(ID_GRA_FILL, &CMdispMFCView::OnGraFill)
   ON_UPDATE_COMMAND_UI(ID_GRA_FILL, &CMdispMFCView::OnUpdateGraFill)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMdispMFCView construction/destruction
CMdispMFCView::CMdispMFCView()
   {
    // TODO: add construction code here
    m_MilOverlayImage               = M_NULL;   // Overlay image buffer identifier. 
    m_MilDisplay                    = M_NULL;   // Display identifier.
    m_MilGraphContext               = M_NULL;
    m_MilGraphList                  = M_NULL;

    m_isWindowed                    = true;
    m_isExclusive                   = false;
    m_isOverlayEnabled              = false;    // Overlay state
    m_isOverlayInitialized          = false;
    m_isScaleDisplayEnabled         = false;
    m_currentViewMode               = M_TRANSPARENT;
    m_currentShiftValue             = M_NULL;
    m_isInAsynchronousMode          = false;
    m_currentCompressionType        = M_NULL;
    m_currentAsynchronousFrameRate  = M_INFINITE;
    m_currentQFactor                = M_DEFAULT;
    m_currentRestrictCursor         = M_ENABLE;
    m_bGraphicsAnnotations          = false;
    m_PrimitiveInCreation           = M_NULL;
   }

CMdispMFCView::~CMdispMFCView()
   {
   }

BOOL CMdispMFCView::PreCreateWindow(CREATESTRUCT& cs)
   {
   // TODO: Modify the Window class or styles here by modifying
   //  the CREATESTRUCT cs
   return CView::PreCreateWindow(cs);
   }

/////////////////////////////////////////////////////////////////////////////
// CMdispMFCView diagnostics

#ifdef _DEBUG
void CMdispMFCView::AssertValid() const
   {
   CView::AssertValid();
   }

void CMdispMFCView::Dump(CDumpContext& dc) const
   {
   CView::Dump(dc);
   }

CMdispMFCDoc* CMdispMFCView::GetDocument() // non-debug version is inline
   {
   ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMdispMFCDoc)));
   return (CMdispMFCDoc*)m_pDocument;
   }
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMdispMFCView message handlers

void CMdispMFCView::OnGrabStart() 
   {
   // TODO: Add your command handler code here

   /////////////////////////////////////////////////////////////////////////
   // MIL: Write code that will be executed on a grab start
   /////////////////////////////////////////////////////////////////////////
   // If there is a grab in a view, halt the grab before starting a new one
   if(((CMdispMFCApp*)AfxGetApp())->m_isGrabStarted)
      ((CMdispMFCApp*)AfxGetApp())->m_pGrabView->SendMessage(WM_COMMAND, ID_GRAB_STOP, 0);

   // Start a continuous grab in this view
   MdigGrabContinuous(((CMdispMFCApp*)AfxGetApp())->m_MilDigitizer, 
                              ((CMdispMFCDoc*)GetDocument())->m_MilImage);

   // Update the variable GrabIsStarted
   ((CMdispMFCApp*)AfxGetApp())->m_isGrabStarted = TRUE;

   // GrabInViewPtr is now a pointer to m_pGrabView view
   ((CMdispMFCApp*)AfxGetApp())->m_pGrabView = this;

   // Document has been modified
   ((CMdispMFCDoc*)GetDocument())->SetModifiedFlag(TRUE);

   /////////////////////////////////////////////////////////////////////////   
   // MIL: Write code that will be executed on a grab start
   /////////////////////////////////////////////////////////////////////////
   }

void CMdispMFCView::OnUpdateGrabStart(CCmdUI* pCmdUI)
   {
   // TODO: Add your command update UI handler code here
   CView *GrabOwnerViewPtr = ((CMdispMFCApp*)AfxGetApp())->m_pGrabView;

   // Check if there is a valid frame grabber in the system
   if( 0 == ((CMdispMFCApp*)AfxGetApp())->m_numberOfDigitizer )
      {
      // NO FRAME GRABBER: Disable grab feature
      pCmdUI->Enable(0);
      }
   else
      {
      // FRAME GRABBER PRESENT: Enable grab feature
      pCmdUI->Enable(!(GrabOwnerViewPtr &&
                      (GetDocument() == GrabOwnerViewPtr->GetDocument())     &&
                      (((CMdispMFCApp*)AfxGetApp())->m_isGrabStarted == TRUE)  &&
                      ((CMdispMFCApp*)AfxGetApp())->m_numberOfDigitizer));
      }
   }

void CMdispMFCView::OnUpdateSave(CCmdUI* pCmdUI)
   {
   pCmdUI->Enable(((CMdispMFCApp*)AfxGetApp())->m_isGrabStarted == FALSE);
   }

void CMdispMFCView::OnUpdateSaveAs(CCmdUI* pCmdUI)
   {
   pCmdUI->Enable(((CMdispMFCApp*)AfxGetApp())->m_isGrabStarted == FALSE);
   }

void CMdispMFCView::OnMouseUpdateText()
   {
   ((CChildFrame*)GetParentFrame())->UpdateStatusBarWithMouse(m_LastMousePosition.m_DisplayPositionX, m_LastMousePosition.m_DisplayPositionY,
                                                              m_LastMousePosition.m_BufferPositionX,  m_LastMousePosition.m_BufferPositionY);

   // Reset mouse position
   m_LastMousePosition.Set(M_INVALID, M_INVALID, M_INVALID, M_INVALID);
   }

void CMdispMFCView::OnGrabStop() 
   {
   // TODO: Add your command handler code here
 
   /////////////////////////////////////////////////////////////////////////
   // MIL: Write code that will be executed on a grab stop 
   /////////////////////////////////////////////////////////////////////////

   // Halt the grab
   MdigHalt(((CMdispMFCApp*)AfxGetApp())->m_MilDigitizer);
   ((CMdispMFCApp*)AfxGetApp())->m_isGrabStarted = FALSE;

   /////////////////////////////////////////////////////////////////////////
   // MIL: Write code that will be executed on a grab stop 
   /////////////////////////////////////////////////////////////////////////
   }

void CMdispMFCView::OnUpdateGrabStop(CCmdUI* pCmdUI) 
   {
   CView *GrabOwnerViewPtr = ((CMdispMFCApp*)AfxGetApp())->m_pGrabView;

   pCmdUI->Enable(GrabOwnerViewPtr &&
                  (GetDocument() == GrabOwnerViewPtr->GetDocument())    &&
                  (((CMdispMFCApp*)AfxGetApp())->m_isGrabStarted == TRUE) &&
                  ((CMdispMFCApp*)AfxGetApp())->m_numberOfDigitizer);
   
   }

void CMdispMFCView::OnOverlay() 
   {
   /////////////////////////////////////////////////////////////////////////
   // MIL: Write code that will be executed when 'add overlay' is selected
   /////////////////////////////////////////////////////////////////////////

   // Enable overlay
   if (!m_isOverlayEnabled)
      {
      MdispControl(m_MilDisplay, M_OVERLAY, M_ENABLE);
      
      //If overlay buffer as not been initialized yet, do it now.
      if(!m_isOverlayInitialized)
         InitializeOverlay();
      
      m_isOverlayEnabled = true;
      }
   
   // Disable overlay
   else
      {
      // Disable the overlay display. [CALL TO MIL] 
      MdispControl(m_MilDisplay, M_OVERLAY, M_DISABLE);
      
      m_isOverlayInitialized = false;
      m_isOverlayEnabled     = false;
      }
   
   /////////////////////////////////////////////////////////////////////////
   // MIL: Write code that will be executed when 'add overlay' is selected
   /////////////////////////////////////////////////////////////////////////
   }

void CMdispMFCView::OnUpdateOverlay(CCmdUI* pCmdUI) 
   {
    pCmdUI->SetCheck(m_isOverlayEnabled);
   }

void CMdispMFCView::OnInitialUpdate() 
   {
   //Init timer used for frame rate
   SetTimer(TIMER_FRAMERATE,500,M_NULL);

  //Retrieve size of the status bar
  CSize statusBarSize = ((CChildFrame*)GetParentFrame())->GetStatusBarSize();

   // TODO: Add your specialized code here and/or call the base class

   /////////////////////////////////////////////////////////////////////////
   // MIL: Code that will be executed when a view is first attached to the document
   /////////////////////////////////////////////////////////////////////////
   if (((CMdispMFCDoc*)GetDocument())->m_MilImage)
      {
      // Allocate a display using default display type [CALL TO MIL]
      MdispAlloc(((CMdispMFCApp*)AfxGetApp())->m_MilSystem,   M_DEFAULT, 
                                 MIL_TEXT("M_DEFAULT"), M_DEFAULT, &m_MilDisplay);

      if(m_MilDisplay)
         {
         MIL_INT DisplayType = MdispInquire(m_MilDisplay, M_DISPLAY_TYPE, M_NULL);

         // Check display type [CALL TO MIL]
         if((DisplayType&(M_WINDOWED|M_EXCLUSIVE)) != M_WINDOWED)       
            m_isWindowed = false;

         if(DisplayType&(M_EXCLUSIVE))
            m_isExclusive = true;

         // Initially set view mode to default
         ChangeViewMode(M_DEFAULT);

         if(IsNetworkedSystem())
            {
            // Check compression type [CALL TO MIL]
            MdispInquire(m_MilDisplay, M_COMPRESSION_TYPE, &m_currentCompressionType);

            // Check asynchronous mode [CALL TO MIL]
            m_isInAsynchronousMode = (MdispInquire(m_MilDisplay, 
                                                         M_ASYNC_UPDATE, M_NULL) == M_ENABLE);

            // Check asynchronous frame rate [CALL TO MIL]
            MdispInquire(m_MilDisplay, M_UPDATE_RATE_MAX, &m_currentAsynchronousFrameRate);

            // Check Q factor [CALL TO MIL]
            MdispInquire(m_MilDisplay, M_Q_FACTOR, &m_currentQFactor);
            }

         if(m_isExclusive)
            {
            MdispInquire(m_MilDisplay, M_RESTRICT_CURSOR,    &m_currentRestrictCursor);
            }

         // Select the buffer from it's display object and given window [CALL TO MIL]
         MdispSelectWindow(m_MilDisplay, 
                        ((CMdispMFCDoc*)GetDocument())->m_MilImage, m_isWindowed?m_hWnd:0);

         // Allow panning and zooming with the mouse [CALL TO MIL]
         MdispControl(m_MilDisplay, M_MOUSE_USE, M_ENABLE);

         // Allow mouse cursor handling [CALL TO MIL]
         MdispControl(m_MilDisplay, M_MOUSE_CURSOR_CHANGE, M_ENABLE);
      }

      CView::OnInitialUpdate();

      //Resize display window if not currently maximized
      if(!GetParent()->IsZoomed()&& !m_isExclusive)
         ResizeDisplayWindow();

      // In windowed mode, resize window to image.
      if(m_isWindowed)
         {
         //May need to resize frame
         GetParentFrame()->RecalcLayout();
         }
       
      // Hook a function to mouse-movement event, to update cursor position in status bar.
      MdispHookFunction(m_MilDisplay, M_MOUSE_MOVE, MouseFct, (void*)this);
      }

   /////////////////////////////////////////////////////////////////////////
   // MIL: Code that will be executed when a view is first attached to the document
   /////////////////////////////////////////////////////////////////////////
   }

void CMdispMFCView::OnDestroy() 
   {
   //Frame rate timer not needed anymore
   KillTimer(TIMER_FRAMERATE);

   /////////////////////////////////////////////////////////////////////////
   // MIL: Write code that will be executed when the window is destroyed
   /////////////////////////////////////////////////////////////////////////
   RemoveFromDisplay();

   /////////////////////////////////////////////////////////////////////////
   // MIL: Write code that will be executed when the window is destroyed
   /////////////////////////////////////////////////////////////////////////   
   CView::OnDestroy();
   }

// Function used to halt the grab, deselect the buffer and free the display
void CMdispMFCView::RemoveFromDisplay()
   {
   //Halt grab if in process in THIS view
   if ((((CMdispMFCApp*)AfxGetApp())->m_pGrabView == this) && 
        ((CMdispMFCApp*)AfxGetApp())->m_isGrabStarted)
      {
      //Signal to the digitizer to halt the grab [CALL TO MIL]
      MdigHalt(((CMdispMFCApp*)AfxGetApp())->m_MilDigitizer);
            
      ((CMdispMFCApp*)AfxGetApp())->m_isGrabStarted = FALSE;
      }

   if (((CMdispMFCDoc*)GetDocument())->m_MilImage && m_MilDisplay)
      {
      //Deselect the buffer from its display object and given window [CALL TO MIL]
      MdispDeselect(m_MilDisplay,((CMdispMFCDoc*)GetDocument())->m_MilImage);

      // Hook from mouse-movement event.
      MdispHookFunction(m_MilDisplay, M_MOUSE_MOVE+M_UNHOOK, MouseFct, (void*)this);

      //Free the display [CALL TO MIL]
      if(m_MilGraphList)
         {
         MgraFree(m_MilGraphList);
         m_MilGraphList = M_NULL;
         }
      if(m_MilGraphContext)
         {
         MgraFree(m_MilGraphContext);
         m_MilGraphContext = M_NULL;
         }
      MdispFree(m_MilDisplay);
      m_MilDisplay = M_NULL;
      }
   }

void CMdispMFCView::OnDraw(CDC* pDC) 
   {
   /////////////////////////////////////////////////////////////////////////
   // MIL: Write code that will be executed when 'OnDraw' window message is received
   /////////////////////////////////////////////////////////////////////////
   RECT clientRECT;

   if(!m_MilDisplay)
      {
      //Retrieve current client rect of the window
      GetClientRect(&clientRECT);
      pDC->SetBkMode(TRANSPARENT);
      pDC->SetTextColor(RGB(255,0,0));
      pDC->DrawText("Display Allocation Failed!", &clientRECT,DT_LEFT);
      }
   // In non-windowed mode, write message in window
   if(!m_isWindowed)
      {
      //Retrieve current client rectangle of the window
      GetClientRect(&clientRECT);
      
      //Signal to the given device context to draw the string
      pDC->SetBkMode(TRANSPARENT);
      pDC->SetTextColor(RGB(0,0,0));
      pDC->DrawText("Image displayed on external screen", &clientRECT,DT_LEFT);
      }
   }

void CMdispMFCView::OnSize(UINT nType, int cx, int cy) 
   {
   CView::OnSize(nType, cx, cy);

    /////////////////////////////////////////////////////////////////////////
   // MIL: Write code that will be executed when 'OnSize' window message is received
   /////////////////////////////////////////////////////////////////////////
   
   //Update status bar with current zoom factor
   if(m_MilDisplay)
      {
      MIL_DOUBLE ZoomX = 1, ZoomY = 1;
      MdispInquire(m_MilDisplay, M_ZOOM_FACTOR_X, &ZoomX);
      MdispInquire(m_MilDisplay, M_ZOOM_FACTOR_Y, &ZoomY);
      ((CChildFrame*)GetParentFrame())->UpdateStatusBarWithScale(ZoomX, ZoomY);
      }
   }

void CMdispMFCView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
   {
   /////////////////////////////////////////////////////////////////////////
   // MIL: Trap special keystrokes and call MIL. 
   // e.g. Cancel interactive primitive edition on Escape (if applies)
   /////////////////////////////////////////////////////////////////////////
   if(m_bGraphicsAnnotations)
      {
      if (nChar == VK_ESCAPE)
         {
         MIL_INT CurrentInteractiveState = M_NULL;
         MgraInquireList(m_MilGraphList, M_LIST, M_DEFAULT, M_INTERACTIVE_GRAPHIC_STATE, &CurrentInteractiveState);

         switch(CurrentInteractiveState)
            {
         case M_STATE_IDLE                :
         case M_STATE_GRAPHIC_HOVERED     :
         case M_STATE_HANDLE_HOVERED      :
            MgraControlList(m_MilGraphList, M_ALL, M_DEFAULT, M_GRAPHIC_SELECTED, M_FALSE);
            break;

         case M_STATE_GRAPHIC_DRAGGED     :
         case M_STATE_HANDLE_DRAGGED      :
         case M_STATE_WAITING_FOR_CREATION:
         case M_STATE_BEING_CREATED       :
            MgraInteractive(M_NULL, m_MilGraphList, M_CANCEL, M_DEFAULT, M_DEFAULT);
            break;

         default:
            break;
            }
         }

      if (nChar == VK_DELETE)
         {
         MgraControlList(m_MilGraphList, M_ALL_SELECTED, M_DEFAULT, M_DELETE, M_DEFAULT);
         }
      }

   CView::OnKeyDown(nChar, nRepCnt, nFlags);
   }

void CMdispMFCView::OnTimer(UINT_PTR nIDEvent) 
   {
   /////////////////////////////////////////////////////////////////////////
   // MIL: update status bar with current display update rate
   /////////////////////////////////////////////////////////////////////////
   if (m_MilDisplay && (nIDEvent == TIMER_FRAMERATE))
      {
      MIL_DOUBLE CurrentFrameRate = M_NULL;
      MdispInquire(m_MilDisplay, M_UPDATE_RATE, &CurrentFrameRate);
      ((CChildFrame*)GetParentFrame())->UpdateStatusBarWithFrameRate(CurrentFrameRate);

      //Update status bar with current zoom factor
      MIL_DOUBLE ZoomX =1, ZoomY = 1;
      MdispInquire(m_MilDisplay, M_ZOOM_FACTOR_X, &ZoomX);
      MdispInquire(m_MilDisplay, M_ZOOM_FACTOR_Y, &ZoomY);
      ((CChildFrame*)GetParentFrame())->UpdateStatusBarWithScale(ZoomX, ZoomY);
      }
   
   CView::OnTimer(nIDEvent);
   }

void CMdispMFCView::OnZoomin() 
   {
   /////////////////////////////////////////////////////////////////////////
   // MIL: Write code that will be executed when 'Zoom In' button or menu is clicked
   /////////////////////////////////////////////////////////////////////////
   if(m_MilDisplay)
      {
      //Calculate new zoom factors
      MIL_DOUBLE ZoomX = 1, ZoomY = 1;
      MdispInquire(m_MilDisplay, M_ZOOM_FACTOR_X, &ZoomX);
      MdispInquire(m_MilDisplay, M_ZOOM_FACTOR_Y, &ZoomY);
      
      if((ZoomX <= 8) && (ZoomY<=8))
         {
         ZoomX*=2;
         ZoomY*=2;
         }
      
      //Perform zooming with MIL (using MdispZoom)
      Zoom(ZoomX, ZoomY);
      }
   }

void CMdispMFCView::OnUpdateZoomin(CCmdUI* pCmdUI) 
   {
   if(m_isScaleDisplayEnabled)
      pCmdUI->Enable(0);
   else
      pCmdUI->Enable(1);
   }

void CMdispMFCView::OnZoomout() 
   {
   /////////////////////////////////////////////////////////////////////////
   // MIL: Write code that will be executed when 'Zoom Out' button or menu is clicked
   /////////////////////////////////////////////////////////////////////////
   if(m_MilDisplay)
      {
      //Calculate new zoom factors
      MIL_DOUBLE ZoomX = 1, ZoomY = 1;
      MdispInquire(m_MilDisplay, M_ZOOM_FACTOR_X, &ZoomX);
      MdispInquire(m_MilDisplay, M_ZOOM_FACTOR_Y, &ZoomY);
      
      if((ZoomX >= 0.125) && (ZoomY>=0.125))
         {
         ZoomX/=2;
         ZoomY/=2;
         }

      //Perform zooming with MIL (using MdispZoom)
      Zoom(ZoomX, ZoomY);
      }
   }

void CMdispMFCView::OnUpdateZoomout(CCmdUI* pCmdUI) 
   {
   if(m_isScaleDisplayEnabled)
      pCmdUI->Enable(0);
   else
      pCmdUI->Enable(1);
   }

void CMdispMFCView::OnNoZoom() 
   {
   /////////////////////////////////////////////////////////////////////////
   // MIL: Write code that will be executed when 'No Zoom' button or menu is clicked
   /////////////////////////////////////////////////////////////////////////
   if(m_MilDisplay)
      {
      //Perform zooming with MIL
      Zoom(1, 1);
      MdispPan(m_MilDisplay, M_NULL, M_NULL);
      }
   }

void CMdispMFCView::OnUpdateNoZoom(CCmdUI* pCmdUI) 
   {
   if(m_isScaleDisplayEnabled)
      pCmdUI->Enable(0);
   else
      pCmdUI->Enable(1);
   }

void CMdispMFCView::OnScaleDisplay() 
   {
   /////////////////////////////////////////////////////////////////////////
   // MIL: Write code that will be executed when 'Scale Display' button or menu is clicked
   /////////////////////////////////////////////////////////////////////////
   if(m_MilDisplay)
      {
      //If Scale Display is not currently enabled, enable it
      if(!m_isScaleDisplayEnabled)
         {
         //Using MIL, enable Scale Display Mode [CALL TO MIL]
         MdispControl(m_MilDisplay, M_SCALE_DISPLAY, M_ENABLE);

         m_isScaleDisplayEnabled = true;
         }

      //If Fill Display is currently enabled, disable it
      else
         {
         //Using MIL, disable Scale Display Mode [CALL TO MIL]
         MdispControl(m_MilDisplay, M_SCALE_DISPLAY, M_DISABLE);

         m_isScaleDisplayEnabled = false;
         }

      //Queue an update
      if(m_isWindowed)
         InvalidateRect(M_NULL);
      }
   }

void CMdispMFCView::OnUpdateScaleDisplay(CCmdUI* pCmdUI) 
   {
   // TODO: Add your command update UI handler code here
   pCmdUI->SetCheck(m_isScaleDisplayEnabled);
   pCmdUI->Enable(1);
   }

void CMdispMFCView::ResizeDisplayWindow()
   {
   /////////////////////////////////////////////////////////////////////////
   // MIL: Write code that will be executed when window needs to be resized
   /////////////////////////////////////////////////////////////////////////
   //Retrieve parent size
   CRect parentRECT;
   GetParent()->GetParent()->GetClientRect(&parentRECT);

   //Retrieve size of the status bar
   CSize statusBarSize = ((CChildFrame*)GetParentFrame())->GetStatusBarSize();

   // Get active multimedia window rectangle, adding the height of the status bar    
   CRect MMRect;
   MIL_DOUBLE ZoomX, ZoomY = 0;
   MdispInquire(m_MilDisplay, M_ZOOM_FACTOR_X, &ZoomX);
   MdispInquire(m_MilDisplay, M_ZOOM_FACTOR_Y, &ZoomY);

   int dispX = (int)(GetDocument()->m_imageSizeX * ZoomX);
   int dispY = (int)(GetDocument()->m_imageSizeY * ZoomY);

   if(!m_isWindowed)
      {
      dispX = (int)MdispInquire(m_MilDisplay, M_SIZE_X, M_NULL);
      dispY = (int)MdispInquire(m_MilDisplay, M_SIZE_Y, M_NULL);
      }

   // Dimension must be adjusted
   long PosOffset = 4;   

   MMRect.SetRect(0, 0, dispX, dispY);

   // Dimension must be adjusted
   // Resize frame to the dimensions of the multimedia window
   GetParent()->CalcWindowRect(&MMRect, CWnd::adjustBorder);
   MMRect.OffsetRect(-MMRect.left,-MMRect.top);

   if(MMRect.Width() > parentRECT.Width())
      MMRect.right = parentRECT.right;

   if(MMRect.Height() > parentRECT.Height())
      MMRect.bottom = parentRECT.bottom;
   else
      MMRect.bottom = MMRect.bottom + statusBarSize.cy;

      
      GetParent()->SetWindowPos( NULL, 
                              0,       
                              0, 
                              MMRect.right + PosOffset,
                              MMRect.bottom + PosOffset, 
                              SWP_NOACTIVATE|SWP_NOZORDER);

   GetParent()->InvalidateRect(NULL,FALSE);
   }

void CMdispMFCView::OnViewModeChange()
   {
   /////////////////////////////////////////////////////////////////////////
   // MIL: Write code that will be executed when the active view mode 
   //      was changed from the toolbar combo box
   /////////////////////////////////////////////////////////////////////////
   if(m_MilDisplay)
      {
      //Retrieve combo box from Application Main Frame
      CComboBox* pViewModeComboBox = 
                     ((CMainFrame*)AfxGetMainWnd())->GetToolbarViewModeCombo() ;

      if (pViewModeComboBox)
         {
         //Retrieves the index of the currently selected item
         int  index = pViewModeComboBox->GetCurSel();

         if(index!=CB_ERR)
            {
            switch(index)
               {
               case 0:
                  ChangeViewMode(M_DEFAULT);
                  break;
               case 1:
                  ChangeViewMode(M_TRANSPARENT);
                  break;
               case 2:
                  ChangeViewMode(M_AUTO_SCALE);
                  break;
               case 3:
                  ChangeViewMode(M_MULTI_BYTES);
                  break;
               case 4:
                  ChangeViewMode(M_BIT_SHIFT,2);
                  break;
               case 5:
                  ChangeViewMode(M_BIT_SHIFT,4);
                  break;
               case 6:
                  ChangeViewMode(M_BIT_SHIFT,8);
                  break;
               default:
                 ChangeViewMode(M_DEFAULT);
               }
            }
         }
      }
   }

void CMdispMFCView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
   {
   UpdateComboBoxWithCurrentViewMode();

   CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
   }
void CMdispMFCView::UpdateComboBoxWithCurrentViewMode()
   {
   CComboBox *ViewModeComboBox = ((CMainFrame*)AfxGetMainWnd())->GetToolbarViewModeCombo();

   if(ViewModeComboBox)
      {
      if(m_currentViewMode == M_DEFAULT)
         ViewModeComboBox->SelectString(0,MIL_TEXT("M_DEFAULT"));
      else if(m_currentViewMode == M_TRANSPARENT)
         ViewModeComboBox->SelectString(0,MIL_TEXT("M_TRANSPARENT"));
      else if(m_currentViewMode==M_AUTO_SCALE)
         ViewModeComboBox->SelectString(0,MIL_TEXT("M_AUTO_SCALE"));
      else if(m_currentViewMode==M_MULTI_BYTES)
         ViewModeComboBox->SelectString(0,MIL_TEXT("M_MULTI_BYTES"));
      else if(m_currentViewMode==M_BIT_SHIFT)
         {
         if(m_currentShiftValue == 2)
            ViewModeComboBox->SelectString(0,MIL_TEXT("M_BIT_SHIFT:2"));
         else if(m_currentShiftValue == 4)
            ViewModeComboBox->SelectString(0,MIL_TEXT("M_BIT_SHIFT:4"));
         else if(m_currentShiftValue == 8)
            ViewModeComboBox->SelectString(0,MIL_TEXT("M_BIT_SHIFT:8"));
         }

      ViewModeComboBox->UpdateData(FALSE);
      }
   }
     
void CMdispMFCView::Zoom(MIL_DOUBLE ZoomFactorToApplyX, MIL_DOUBLE ZoomFactorToApplyY)
   {
   /////////////////////////////////////////////////////////////////////////
   // MIL: Apply current zoom factor on display 
   /////////////////////////////////////////////////////////////////////////

   //Apply zoom  [CALL TO MIL]
   MdispZoom(m_MilDisplay, ZoomFactorToApplyX, ZoomFactorToApplyY);
   }

void CMdispMFCView::ChangeViewMode(MIL_INT ViewMode, MIL_INT ShiftValue)
   {
   if(m_MilDisplay)
      {
      //Apply view mode on display [CALL TO MIL]
      MdispControl(m_MilDisplay, M_VIEW_MODE, ViewMode);

      if(ViewMode == M_BIT_SHIFT)
         MdispControl(m_MilDisplay, M_VIEW_BIT_SHIFT, ShiftValue);

      //Check if control worked correctly before considering it as successful [CALL TO MIL]
      if(MdispInquire(m_MilDisplay, M_VIEW_MODE,M_NULL)==ViewMode)
         {
         //Make sure View Mode combo box shows current view mode
         m_currentViewMode      = ViewMode;
         m_currentShiftValue = ShiftValue;

         UpdateComboBoxWithCurrentViewMode();
         }
      }
   }

void CMdispMFCView::InitializeOverlay()
   {
   // Initialize overlay if not already done
   if ((!m_isOverlayInitialized) && (m_MilDisplay))
      {
      //Verify the validity of windowed display [CALL TO MIL]
      if ((((CMdispMFCDoc*)GetDocument())->m_MilImage) && (m_MilDisplay) )
         {
         ////////////////////////////
         // Prepare overlay buffer //
         ////////////////////////////

         // Enable display overlay annotations.
         MdispControl(m_MilDisplay, M_OVERLAY, M_ENABLE);

         // Inquire the overlay buffer associated with the display.
         MdispInquire(m_MilDisplay, M_OVERLAY_ID, &m_MilOverlayImage);

         // Clear the overlay to transparent.
         MdispControl(m_MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);

         // Disable the overlay display update to accelerate annotations.
         MdispControl(m_MilDisplay, M_OVERLAY_SHOW, M_DISABLE);

         // Draw MIL monochrome overlay annotation *
         //*****************************************

         // Inquire MilOverlayImage size X and Y [CALL TO MIL] 
         long imageWidth  = (long)MbufInquire(m_MilOverlayImage,M_SIZE_X,M_NULL);
         long imageHeight = (long)MbufInquire(m_MilOverlayImage,M_SIZE_Y,M_NULL);

         // Set graphic text to transparent background. [CALL TO MIL]
         MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);

         // Set drawing color to white. [CALL TO MIL]
         MgraColor(M_DEFAULT, M_COLOR_WHITE);

         // Print a string in the overlay image buffer. [CALL TO MIL]
         MgraText(M_DEFAULT, m_MilOverlayImage, (imageWidth/9), (imageHeight/5),    
                                                        MIL_TEXT(" -------------------- "));
         MgraText(M_DEFAULT, m_MilOverlayImage, (imageWidth/9), ((imageHeight/5)+25), 
                                                        MIL_TEXT(" - MIL Overlay Text - "));
         MgraText(M_DEFAULT, m_MilOverlayImage, (imageWidth/9), ((imageHeight/5)+50), 
                                                        MIL_TEXT(" -------------------- "));

         // Print a green string in the green component overlay image buffer. [CALL TO MIL]
         MgraColor(M_DEFAULT, M_COLOR_GREEN);
         MgraText(M_DEFAULT, m_MilOverlayImage, imageWidth*11/18, (imageHeight/5),    
                                                        MIL_TEXT(" -------------------- "));
         MgraText(M_DEFAULT, m_MilOverlayImage, imageWidth*11/18, ((imageHeight/5)+25), 
                                                        MIL_TEXT(" - MIL Overlay Text - "));
         MgraText(M_DEFAULT, m_MilOverlayImage, imageWidth*11/18, ((imageHeight/5)+50), 
                                                        MIL_TEXT(" -------------------- "));

         // Draw GDI color overlay annotation *
         //************************************

         // Disable hook to MIL error because control might not be supported 
         MappControl(M_DEFAULT, M_ERROR_HOOKS, M_DISABLE);

         // Create a device context to draw in the overlay buffer with GDI.  [CALL TO MIL]
         MbufControl(m_MilOverlayImage, M_DC_ALLOC, M_DEFAULT);

         // Re-enable hook to MIL error 
         MappControl(M_DEFAULT, M_ERROR_HOOKS, M_ENABLE);

         // Retrieve the HDC of the overlay [CALL TO MIL]
         HDC OverlayDC = (HDC)MbufInquire(m_MilOverlayImage, M_DC_HANDLE, M_NULL);

         if(OverlayDC != M_NULL)
            {
            // Attach the device context.
            CDC NewDC;
            NewDC.Attach(OverlayDC);

            NewDC.SetBkMode(TRANSPARENT);

            // Create a blue pen. 
            m_pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
            m_penOld = NewDC.SelectObject(&m_pen);

            // Draw a cross in the overlay buffer. 
            NewDC.MoveTo(0,(imageHeight/2));
            NewDC.LineTo(imageWidth,(imageHeight/2));
            NewDC.MoveTo((imageWidth/2),0);
            NewDC.LineTo((imageWidth/2),imageHeight);

            // Write text in the overlay buffer. 
            MosStrcpy(m_chText, 80, MIL_TEXT("GDI Overlay Text "));
            NewDC.SetTextColor(RGB(255, 0, 0));
            NewDC.TextOut((imageWidth*3/18),(imageHeight*17/24), m_chText, 
                                                         (MIL_INT32)MosStrlen(m_chText));
            NewDC.SetTextColor(RGB(255, 255, 0));
            NewDC.TextOut((imageWidth*12/18),(imageHeight*17/24), m_chText, 
                                                         (MIL_INT32)MosStrlen(m_chText));

            // Deselect and destroy the blue pen.
            NewDC.SelectObject(m_penOld);
            m_pen.DeleteObject();

            // Detach the device context.
            NewDC.Detach();

            // Delete created device context.  [CALL TO MIL]
            MbufControl(m_MilOverlayImage, M_DC_FREE, M_DEFAULT);

            // Signal to MIL that the overlay buffer was modified. [CALL TO MIL]
            MbufControl(m_MilOverlayImage, M_MODIFIED, M_DEFAULT);
            }

         // Now that overlay buffer is correctly prepared, display it [CALL TO MIL]
         MdispControl(m_MilDisplay, M_OVERLAY_SHOW, M_ENABLE);

         // Overlay is now initialized
         m_isOverlayInitialized = true;
         }
      }
   }

void CMdispMFCView::OnSizing(UINT fwSide, LPRECT pRect)
   {
   /////////////////////////////////////////////////////////////////////////
   // MIL: Write code that will be executed when window is resized
   /////////////////////////////////////////////////////////////////////////
   if(m_MilDisplay && !m_isWindowed)
      CView::OnSizing(fwSide, pRect);
   }

void CMdispMFCView::OnCompression(UINT nID)
   {
   /////////////////////////////////////////////////////////////////////////
   // MIL: Write code that will be executed when 
   // 'Distributed MIL / Compression / *' button or menu is clicked
   /////////////////////////////////////////////////////////////////////////
   UINT CompressionTypeIndex = nID - s_FirstCompressionTypeControlInRange;
   ASSERT(CompressionTypeIndex < (sizeof(s_CompressionType) / sizeof(MIL_INT)));
   ChangeCompressionType(s_CompressionType[CompressionTypeIndex]);
   }

void CMdispMFCView::OnUpdateCompression(CCmdUI *pCmdUI)
   {
   // TODO: Add your command update UI handler code here
   pCmdUI->Enable(IsNetworkedSystem());

   UINT CompressionTypeIndex = pCmdUI->m_nID - s_FirstCompressionTypeControlInRange;
   ASSERT(CompressionTypeIndex < (sizeof(s_CompressionType) / sizeof(MIL_INT)));
   pCmdUI->SetRadio(m_currentCompressionType == s_CompressionType[CompressionTypeIndex]);
   }

void CMdispMFCView::OnAsynchronousMode(UINT nID)
   {
   /////////////////////////////////////////////////////////////////////////
   // MIL: Write code that will be executed when
   // 'Distributed MIL / Asynchronous Mode / *' button or menu is clicked
   /////////////////////////////////////////////////////////////////////////
   UINT AsynchronousModeIndex = nID - s_FirstAsynchronousMaxUpdateRateControlInRange;
   ASSERT(AsynchronousModeIndex < (sizeof(s_AsynchronousMaxUpdateRate) / sizeof(MIL_INT)));
   ChangeAsynchronousMode(s_AsynchronousMaxUpdateRate[AsynchronousModeIndex] != M_DISABLE,
                          s_AsynchronousMaxUpdateRate[AsynchronousModeIndex]);
   }

void CMdispMFCView::OnUpdateAsynchronousMode(CCmdUI *pCmdUI)
   {
   // TODO: Add your command update UI handler code here
   pCmdUI->Enable(IsNetworkedSystem());

   UINT AsynchronousModeIndex = pCmdUI->m_nID - s_FirstAsynchronousMaxUpdateRateControlInRange;
   ASSERT(AsynchronousModeIndex < (sizeof(s_AsynchronousMaxUpdateRate) / sizeof(MIL_INT)));

   bool SetRadio = m_isInAsynchronousMode ||
                  (s_AsynchronousMaxUpdateRate[AsynchronousModeIndex] == M_DISABLE);
   if(m_isInAsynchronousMode)
      SetRadio = (s_AsynchronousMaxUpdateRate[AsynchronousModeIndex] == 
                                                      m_currentAsynchronousFrameRate);

   pCmdUI->SetRadio(SetRadio);
   }

void CMdispMFCView::OnQFactor(UINT nID)
   {
   /////////////////////////////////////////////////////////////////////////
   // MIL: Write code that will be executed when
   // 'Distributed MIL / Q factor / *' button or menu is clicked
   /////////////////////////////////////////////////////////////////////////
   UINT QFactorIndex = nID - s_FirstQFactorControlInRange;
   ASSERT(QFactorIndex < (sizeof(s_QFactor) / sizeof(MIL_INT)));
   ChangeQFactor(s_QFactor[QFactorIndex]);
   }

void CMdispMFCView::OnUpdateQFactor(CCmdUI *pCmdUI)
   {
   // TODO: Add your command update UI handler code here
   pCmdUI->Enable(IsNetworkedSystem());

   UINT QFactorIndex = pCmdUI->m_nID - s_FirstQFactorControlInRange;
   ASSERT(QFactorIndex < (sizeof(s_QFactor) / sizeof(MIL_INT)));

   pCmdUI->SetRadio(s_QFactor[QFactorIndex] == m_currentQFactor);
   }

void CMdispMFCView::ChangeCompressionType(MIL_INT CompressionType)
   {
   if(m_MilDisplay)
      {
      // Apply compression type to display [CALL TO MIL]
      MdispControl(m_MilDisplay, M_COMPRESSION_TYPE, CompressionType);
   
      // Check if control worked correctly before considering it successful [CALL TO MIL]
      if(MdispInquire(m_MilDisplay, M_COMPRESSION_TYPE, M_NULL) == CompressionType)
         {
         m_currentCompressionType = CompressionType;
         }
      }
   }

void CMdispMFCView::ChangeAsynchronousMode(bool Enabled, MIL_INT FrameRate)
   {
   if(Enabled && (FrameRate != m_currentAsynchronousFrameRate))
      {
      if(m_MilDisplay)
         {
         // Apply asynchronous frame rate to display [CALL TO MIL]
         MdispControl(m_MilDisplay, M_UPDATE_RATE_MAX, FrameRate);
      
         // Check if control worked correctly before considering it successful [CALL TO MIL]
         if(MdispInquire(m_MilDisplay, M_UPDATE_RATE_MAX, M_NULL) == FrameRate)
            {
            m_currentAsynchronousFrameRate = FrameRate;
            }
         }
      }

   if((Enabled && !m_isInAsynchronousMode) ||
      (!Enabled && m_isInAsynchronousMode))
      {
      if(m_MilDisplay)
         {
         // Apply asynchronous update to display [CALL TO MIL]
         MdispControl(m_MilDisplay, M_ASYNC_UPDATE, (Enabled ? M_ENABLE : M_DISABLE));
      
         // Check if control worked correctly before considering it successful [CALL TO MIL]
         if(MdispInquire(m_MilDisplay, M_ASYNC_UPDATE, M_NULL) == 
                                                         (Enabled ? M_ENABLE : M_DISABLE))
            {
            m_isInAsynchronousMode = Enabled;
            }
         }
      }
   }

void CMdispMFCView::ChangeQFactor(MIL_INT QFactor)
   {
   if(m_MilDisplay)
      {
      // Apply Q factor to display [CALL TO MIL]
      MdispControl(m_MilDisplay, M_Q_FACTOR, QFactor);
   
      // Check if control worked correctly before considering it successful [CALL TO MIL]
      if(MdispInquire(m_MilDisplay, M_Q_FACTOR, M_NULL) == QFactor)
         {
         m_currentQFactor = QFactor;
         }
      }
   }

bool CMdispMFCView::IsNetworkedSystem()
   {
   bool NetworkedSystem = false;
   MIL_ID SystemId = ((CMdispMFCApp*)AfxGetApp())->m_MilSystem;

   // Check if system is networked (DistributedMIL) [CALL TO MIL]
   if(SystemId)
      NetworkedSystem = (MsysInquire(SystemId, M_LOCATION, M_NULL) == M_REMOTE);

   return NetworkedSystem;
   }

void CMdispMFCView::OnRestrictCursor()
   {
   /////////////////////////////////////////////////////////////////////////
   // MIL: Write code that will be executed when 'Restrict Cursor' menu item is clicked
   /////////////////////////////////////////////////////////////////////////
   if(m_MilDisplay)
      {
      // Apply mouse restriction to display [CALL TO MIL]
      if(m_currentRestrictCursor == M_DISABLE)
         {
         MdispControl(m_MilDisplay, M_RESTRICT_CURSOR, M_ENABLE);
         }
      else
         {
         MdispControl(m_MilDisplay, M_RESTRICT_CURSOR, M_DISABLE);
         }

      // Check if control worked correctly before considering it successful [CALL TO MIL]
      MdispInquire(m_MilDisplay, M_RESTRICT_CURSOR, &m_currentRestrictCursor);
      }
   }

void CMdispMFCView::OnUpdateRestrictCursor(CCmdUI *pCmdUI)
   {
   // TODO: Add your command update UI handler code here
   pCmdUI->Enable(m_isExclusive);
   pCmdUI->SetCheck(m_currentRestrictCursor == M_ENABLE);
   }

void CMdispMFCView::OnGraphicsAnnotations()
   {
   m_bGraphicsAnnotations = !m_bGraphicsAnnotations;
   /////////////////////////////////////////////////////////////////////////
   // MIL: Write code that will be executed when 'Graphics Annotations' menu item is clicked
   /////////////////////////////////////////////////////////////////////////
   if(m_MilDisplay)
      {
      if(m_bGraphicsAnnotations)
         {
         if(!m_MilGraphContext && !m_MilGraphList)
            {
            MIL_INT BufSizeX  = 0, BufSizeY = 0;
            MIL_INT Offset    = 15;

            MgraAlloc(((CMdispMFCApp*)AfxGetApp())->m_MilSystem, &m_MilGraphContext);
            MgraAllocList(((CMdispMFCApp*)AfxGetApp())->m_MilSystem, M_DEFAULT, &m_MilGraphList);
            MdispControl(m_MilDisplay, M_ASSOCIATED_GRAPHIC_LIST_ID, m_MilGraphList);

            MdispControl(m_MilDisplay, M_UPDATE_GRAPHIC_LIST, M_DISABLE);
            MbufInquire(((CMdispMFCDoc*)GetDocument())->m_MilImage, M_SIZE_X, &BufSizeX);
            MbufInquire(((CMdispMFCDoc*)GetDocument())->m_MilImage, M_SIZE_Y, &BufSizeY);

            MgraClear(m_MilGraphContext, m_MilGraphList);

            MgraColor(m_MilGraphContext, M_COLOR_LIGHT_BLUE);
            MgraRect(m_MilGraphContext, m_MilGraphList, Offset, Offset, BufSizeX - Offset, BufSizeY - Offset);

            MgraColor(m_MilGraphContext, M_COLOR_GREEN);
            MgraControl(m_MilGraphContext, M_BACKGROUND_MODE, M_TRANSPARENT);
            MgraControl(m_MilGraphContext, M_TEXT_ALIGN_HORIZONTAL, M_CENTER);
            MgraControl(m_MilGraphContext, M_TEXT_ALIGN_VERTICAL, M_CENTER);
            MgraControl(m_MilGraphContext, M_FONT_SIZE, 24);
            MgraFont(m_MilGraphContext, MIL_FONT_NAME(MIL_TEXT("Arial:Bold")));
            MgraText(m_MilGraphContext, m_MilGraphList, BufSizeX/2, Offset, MIL_TEXT("Interactive Graphic Annotations"));

            MdispControl(m_MilDisplay, M_UPDATE_GRAPHIC_LIST, M_ENABLE);
            MdispControl(m_MilDisplay, M_GRAPHIC_LIST_INTERACTIVE, M_ENABLE);
            
            MgraHookFunction(m_MilGraphList, M_INTERACTIVE_GRAPHIC_STATE_MODIFIED, GraphicListModifiedHookFct, (void*)this);
            }
         }
      else
         {
         MgraHookFunction(m_MilGraphList, M_INTERACTIVE_GRAPHIC_STATE_MODIFIED+M_UNHOOK, GraphicListModifiedHookFct, (void*)this);
         MdispControl(m_MilDisplay, M_ASSOCIATED_GRAPHIC_LIST_ID, M_NULL);

         if(m_MilGraphList)
            {
            MgraFree(m_MilGraphList);
            m_MilGraphList = M_NULL;
            }
         if(m_MilGraphContext)
            {
            MgraFree(m_MilGraphContext);
            m_MilGraphContext = M_NULL;
            }
         }
      }
   }

void CMdispMFCView::OnUpdateGraphicsAnnotations(CCmdUI *pCmdUI)
   {
    pCmdUI->SetCheck(m_bGraphicsAnnotations);
   }
void CMdispMFCView::OnGraRectangle()
   {
   if(m_bGraphicsAnnotations)
      {
      MgraColor(m_MilGraphContext, M_COLOR_WHITE);
      MgraInteractive(m_MilGraphContext, m_MilGraphList, M_GRAPHIC_TYPE_RECT, M_DEFAULT, M_DEFAULT);
      m_PrimitiveInCreation = M_GRAPHIC_TYPE_RECT;
      }
   }
void CMdispMFCView::OnGraCircle()
   {
   if(m_bGraphicsAnnotations)
      {
      MgraColor(m_MilGraphContext, M_COLOR_YELLOW);
      MgraInteractive(m_MilGraphContext, m_MilGraphList, M_GRAPHIC_TYPE_ARC, M_DEFAULT, M_DEFAULT);
      m_PrimitiveInCreation = M_GRAPHIC_TYPE_ARC;
      }
   }
void CMdispMFCView::OnGraPolygon()
   {
   if(m_bGraphicsAnnotations)
      {
      MgraColor(m_MilGraphContext, M_COLOR_MAGENTA);
      MgraInteractive(m_MilGraphContext, m_MilGraphList, M_GRAPHIC_TYPE_POLYGON, M_DEFAULT, M_DEFAULT);
      m_PrimitiveInCreation = M_GRAPHIC_TYPE_POLYGON;
      }
   }
void CMdispMFCView::OnUpdateGraRectangle(CCmdUI *pCmdUI)
   {
   // TODO: Add your command update UI handler code here
   pCmdUI->Enable(m_bGraphicsAnnotations);
   pCmdUI->SetCheck(m_PrimitiveInCreation == M_GRAPHIC_TYPE_RECT);
   }
void CMdispMFCView::OnUpdateGraCircle(CCmdUI *pCmdUI)
   {
   // TODO: Add your command update UI handler code here
   pCmdUI->Enable(m_bGraphicsAnnotations);
   pCmdUI->SetCheck(m_PrimitiveInCreation == M_GRAPHIC_TYPE_ARC);
   }
void CMdispMFCView::OnUpdateGraPolygon(CCmdUI *pCmdUI)
   {
   // TODO: Add your command update UI handler code here
   pCmdUI->Enable(m_bGraphicsAnnotations);
   pCmdUI->SetCheck(m_PrimitiveInCreation == M_GRAPHIC_TYPE_POLYGON);
   }
void CMdispMFCView::OnGraChooseColor()
   {
   COLORREF NewColor = M_NULL;
   if(ShowColorChooser(NewColor))
      {
      //Convert color into MIL color
      NewColor = M_RGB888(GetRValue(NewColor), GetGValue(NewColor), GetBValue(NewColor));
      MgraControlList(m_MilGraphList, M_ALL_SELECTED, M_DEFAULT, M_COLOR, (MIL_INT)NewColor);
      MgraControlList(m_MilGraphList, M_ALL, M_DEFAULT, M_GRAPHIC_SELECTED, M_FALSE);
      }
   }
void CMdispMFCView::OnUpdateGraChooseColor(CCmdUI *pCmdUI)
   {
   if(m_bGraphicsAnnotations)
      {
      pCmdUI->Enable(true);
      }

   else
      {
      pCmdUI->Enable(false);
      }
   }
void CMdispMFCView::OnGraFill()
   {
      MgraControlList(m_MilGraphList, M_ALL_SELECTED, M_DEFAULT, M_FILLED, M_TRUE);
      MgraControlList(m_MilGraphList, M_ALL, M_DEFAULT, M_GRAPHIC_SELECTED, M_FALSE);
   }
void CMdispMFCView::OnUpdateGraFill(CCmdUI *pCmdUI)
   {
   if(m_bGraphicsAnnotations)
      {
      pCmdUI->Enable(true);
      }

   else
      {
      pCmdUI->Enable(false);
      }
   }
bool CMdispMFCView::ShowColorChooser(COLORREF& NewColor)
   {
   bool Success = false;
   CColorDialog ColorDlg(NewColor, CC_FULLOPEN);

   if(ColorDlg.DoModal() == IDOK)
      {
      Success = true;
      NewColor = ColorDlg.GetColor();
      }
   return Success;
   }