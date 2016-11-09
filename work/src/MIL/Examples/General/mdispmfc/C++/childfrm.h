// ChildFrm.h : interface of the CChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

class CChildFrame : public CMDIChildWnd
{
   DECLARE_DYNCREATE(CChildFrame)
public:
   CChildFrame();
   void  UpdateStatusBarWithFrameRate(MIL_DOUBLE CurrentRate);
   void  UpdateStatusBarWithScale(MIL_DOUBLE CurrentScaleX, MIL_DOUBLE CurrentScaleY);
   void  UpdateStatusBarWithMouse(MIL_INT DispPositionX, MIL_INT DispPositionY, MIL_DOUBLE BufPositionX, MIL_DOUBLE BufPositionY);
   CSize GetStatusBarSize();
// Attributes
public:

// Operations
public:

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CChildFrame)
   virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
   //}}AFX_VIRTUAL

// Implementation
public:
   virtual ~CChildFrame();
#ifdef _DEBUG
   virtual void AssertValid() const;
   virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
   CStatusBar m_wndStatusBar;
   //{{AFX_MSG(CChildFrame)
   afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
