// MdispMFCView.h : interface of the CMdispMFCView class
//
/////////////////////////////////////////////////////////////////////////////
#define MAXLINES 25
#define MAXCOLUMNS 80
#define MAXLINE (MAXLINES-1)
#pragma once

typedef struct _MOUSEPOSITION
   {
   void Set(MIL_INT DisplayPositionX, MIL_INT DisplayPositionY, MIL_DOUBLE BufferPositionX, MIL_DOUBLE BufferPositionY)
      {
      m_DisplayPositionX = DisplayPositionX;
      m_DisplayPositionY = DisplayPositionY;
      m_BufferPositionX = BufferPositionX;
      m_BufferPositionY = BufferPositionY;
      }
   _MOUSEPOSITION()
      {
      Set(M_INVALID, M_INVALID, M_INVALID, M_INVALID);
      }
   _MOUSEPOSITION& operator=(const _MOUSEPOSITION& MousePosition)
      {
      Set(MousePosition.m_DisplayPositionX, 
         MousePosition.m_DisplayPositionY,
         MousePosition.m_BufferPositionX,
         MousePosition.m_BufferPositionY);

      return *this;
      }
   MIL_INT     m_DisplayPositionX;
   MIL_INT     m_DisplayPositionY;
   MIL_DOUBLE  m_BufferPositionX;
   MIL_DOUBLE  m_BufferPositionY;
   }MOUSEPOSITION;

class CMdispMFCView : public CView
{
protected: // create from serialization only
   CMdispMFCView();
   DECLARE_DYNCREATE(CMdispMFCView)

public:
   CMdispMFCDoc* GetDocument();

// Implementation
public:
   virtual ~CMdispMFCView();
   void     RemoveFromDisplay();
   void     ResetPrimitiveCreation(){m_PrimitiveInCreation = M_NULL;}
   void     SetMousePosition(const MOUSEPOSITION& MousePosition){m_LastMousePosition = MousePosition;}
   MIL_ID   GetDisplay(){return m_MilDisplay;}
#ifdef _DEBUG
   virtual void AssertValid() const;
   virtual void Dump(CDumpContext& dc) const;
#endif

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CMdispMFCView)
   public:
   virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
   virtual void OnInitialUpdate();
   afx_msg void OnDestroy();
   protected:
   virtual void OnDraw(CDC* pDC);
   virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
   //}}AFX_VIRTUAL
protected:

// Generated message map functions
protected:
   //{{AFX_MSG(CMdispMFCView)
   afx_msg void OnGrabStart();
   afx_msg void OnUpdateGrabStart(CCmdUI* pCmdUI);
   afx_msg void OnUpdateSave(CCmdUI* pCmdUI);
   afx_msg void OnUpdateSaveAs(CCmdUI* pCmdUI);
   afx_msg void OnGrabStop();
   afx_msg void OnUpdateGrabStop(CCmdUI* pCmdUI);
   afx_msg void OnOverlay();
   afx_msg void OnUpdateOverlay(CCmdUI* pCmdUI);
   afx_msg void OnSize(UINT nType, int cx, int cy);
   afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
   afx_msg void OnTimer(UINT_PTR nIDEvent);
   afx_msg void OnZoomin();
   afx_msg void OnUpdateZoomin(CCmdUI* pCmdUI);
   afx_msg void OnZoomout();
   afx_msg void OnUpdateZoomout(CCmdUI* pCmdUI);
   afx_msg void OnNoZoom();
   afx_msg void OnUpdateNoZoom(CCmdUI* pCmdUI);
   afx_msg void OnScaleDisplay();
   afx_msg void OnUpdateScaleDisplay(CCmdUI* pCmdUI);
   afx_msg void OnViewModeChange();
   afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
   afx_msg void OnCompression(UINT nID);
   afx_msg void OnUpdateCompression(CCmdUI *pCmdUI);
   afx_msg void OnAsynchronousMode(UINT nID);
   afx_msg void OnUpdateAsynchronousMode(CCmdUI *pCmdUI);
   afx_msg void OnQFactor(UINT nID);
   afx_msg void OnUpdateQFactor(CCmdUI *pCmdUI);
   afx_msg void OnMouseUpdateText();
   afx_msg void OnRestrictCursor();
   afx_msg void OnUpdateRestrictCursor(CCmdUI *pCmdUI);
   afx_msg void OnGraphicsAnnotations();
   afx_msg void OnUpdateGraphicsAnnotations(CCmdUI *pCmdUI);
   afx_msg void OnGraRectangle();
   afx_msg void OnGraCircle();
   afx_msg void OnGraPolygon();
   afx_msg void OnUpdateGraRectangle(CCmdUI *pCmdUI);
   afx_msg void OnUpdateGraCircle(CCmdUI *pCmdUI);
   afx_msg void OnUpdateGraPolygon(CCmdUI *pCmdUI);
   afx_msg void OnGraChooseColor();
   afx_msg void OnUpdateGraChooseColor(CCmdUI *pCmdUI);
   afx_msg void OnGraFill();
   afx_msg void OnUpdateGraFill(CCmdUI *pCmdUI);

   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
private:
   //Operations
   bool IsNetworkedSystem();
   void ChangeAsynchronousMode(bool Enabled, MIL_INT FrameRate);
   void ChangeCompressionType(MIL_INT CompressionType);
   void ChangeQFactor(MIL_INT QFactor);
   void ChangeViewMode(MIL_INT ViewMode, MIL_INT ShiftValue=M_NULL);
   void InitializeOverlay();
   void ResizeDisplayWindow();
   void UpdateScaleDisplayButton();
   void UpdateComboBoxWithCurrentViewMode();
   void Zoom(MIL_DOUBLE ZoomFactorToApplyX, MIL_DOUBLE ZoomFactorToApplyY);
   bool ShowColorChooser(COLORREF& NewColor);

   
   //Attributes
   MIL_ID      m_MilOverlayImage;               //Overlay image buffer identifier 
   MIL_ID      m_MilDisplay;                    //Display identifier
   MIL_ID      m_MilGraphContext;
   MIL_ID      m_MilGraphList;
   
   MIL_INT     m_currentViewMode;               //Current view mode (M_VIEW_MODE dispControl)
   MIL_INT     m_currentShiftValue;             //Current bit-shift value(M_VIEW_BIT_SHIFT dispControl)
   MIL_INT     m_currentCompressionType;        //Current compression type (M_COMPRESSION_TYPE dispControl)
   bool        m_isInAsynchronousMode;          //Current asynchronous mode (M_ASYNC_UPDATE dispControl)
   MIL_INT     m_currentAsynchronousFrameRate;  //Current asynchronous frame rate (M_MAX_FRATE_RATE dispControl)
   MIL_INT     m_currentQFactor;                //Current Q factor (M_Q_FACTOR dispControl)
   MIL_INT     m_currentRestrictCursor;         //Current cursor restriction (M_RESTRICT_CURSOR dispControl)
   bool        m_bGraphicsAnnotations;
   
   bool        m_isScaleDisplayEnabled;         //Scale Display state(M_SCALE_DISPLAY dispControl)
   bool        m_isOverlayEnabled;              //Overlay show state
   bool        m_isOverlayInitialized;          //Overlay initialization state
   
   bool        m_isWindowed;
   bool        m_isExclusive;
   RECT        m_clientRECT;                    //RECT variable
   CPen        m_pen;
   CPen*       m_penOld;                        //CPEN variables
   MIL_TEXT_CHAR m_chText[80];                  //Array of MIL_TEXT_CHAR

   MOUSEPOSITION m_LastMousePosition;
   MIL_INT     m_PrimitiveInCreation;

   // Static data/constants
   static const UINT s_FirstCompressionTypeControlInRange;
   static const MIL_INT s_CompressionType[];
   static const UINT s_FirstAsynchronousMaxUpdateRateControlInRange;
   static const MIL_INT s_AsynchronousMaxUpdateRate[];
   static const UINT s_FirstQFactorControlInRange;
   static const MIL_INT s_QFactor[];
};

#ifndef _DEBUG  // debug version in MdispMFCView.cpp
inline CMdispMFCDoc* CMdispMFCView::GetDocument()
   { return (CMdispMFCDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
