// MdispMFCDoc.h : interface of the CMdispMFCDoc class
//
/////////////////////////////////////////////////////////////////////////////
const int TIMER_FRAMERATE = 1;

class CMdispMFCDoc : public CDocument
{
private:
   MIL_INT64   m_bufferAttributes;   // Buffer attributes that will be allocated

protected: // Create from serialization only
   CMdispMFCDoc();
   DECLARE_DYNCREATE(CMdispMFCDoc)

// Attributes
public:
   MIL_ID   m_MilImage;          // Image buffer identifier.
   MIL_INT  m_imageSizeX;
   MIL_INT  m_imageSizeY;
   MIL_INT  m_NbBands;

// Operations
public:

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CMdispMFCDoc)
   public:
   virtual BOOL OnNewDocument();
   virtual void Serialize(CArchive& ar);
   virtual void OnCloseDocument();
   virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
   virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
   //}}AFX_VIRTUAL

// Implementation
public:
   virtual ~CMdispMFCDoc();
#ifdef _DEBUG
   virtual void AssertValid() const;
   virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
   //{{AFX_MSG(CMdispMFCDoc)
      // NOTE - the ClassWizard will add and remove member functions here.
      //    DO NOT EDIT what you see in these blocks of generated code !
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
