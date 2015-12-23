// CTabCtrlSSL_demoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CTabCtrlSSL_demo.h"
#include "CTabCtrlSSL_demoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCTabCtrlSSL_demoDlg dialog

CCTabCtrlSSL_demoDlg::CCTabCtrlSSL_demoDlg (CWnd* pParent /*=NULL*/)
	: CDialog(CCTabCtrlSSL_demoDlg::IDD, pParent) {
	//{{AFX_DATA_INIT(CCTabCtrlSSL_demoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCTabCtrlSSL_demoDlg::DoDataExchange (CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCTabCtrlSSL_demoDlg)
	DDX_Control(pDX, IDC_TAB, m_tabDemo);
	DDX_Control(pDX, IDC_BUTTON_PREV, m_btnPrev);
	DDX_Control(pDX, IDC_BUTTON_NEXT, m_btnNext);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCTabCtrlSSL_demoDlg, CDialog)
	//{{AFX_MSG_MAP(CCTabCtrlSSL_demoDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_PREV, OnButtonPrev)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnButtonNext)
	//}}AFX_MSG_MAP
	ON_COMMAND (IDC_BUTTON_BASIC, OnButtonBasic)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCTabCtrlSSL_demoDlg message handlers

BOOL CCTabCtrlSSL_demoDlg::OnInitDialog () {
	CDialog::OnInitDialog ();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon (m_hIcon, TRUE);			// Set big icon
	SetIcon (m_hIcon, FALSE);		// Set small icon
	
	// Setup the buttons
	m_btnNext.SetSSLButtonBitmap (IDB_BITMAP_NEXT);
	m_btnNext.SetSSLImageAlign (SSL_IMAGE_RIGHT | SSL_IMAGE_VCENTER);
	m_btnNext.SetSSLTextAlign (SSL_TEXT_LEFT | SSL_TEXT_VCENTER);
	m_btnPrev.SetSSLButtonBitmap (IDB_BITMAP_PREV);
	m_btnPrev.SetSSLImageAlign (SSL_IMAGE_LEFT | SSL_IMAGE_VCENTER);
	m_btnPrev.SetSSLTextAlign (SSL_TEXT_RIGHT | SSL_TEXT_VCENTER);

	// Setup the tab control
	int nPageID = 0;
	m_tabDemo.AddSSLPage (_T("Basic Tab"), nPageID++, IDD_TAB_BASIC);
	m_advancedTab.Create (IDD_TAB_ADVANCED, this);
	m_tabDemo.AddSSLPage (_T("Advanced Page"), nPageID++, &m_advancedTab);
	m_aboutTab.Create (IDD_TAB_ABOUT, this);
	m_tabDemo.AddSSLPage (_T("About"), nPageID++, &m_aboutTab);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCTabCtrlSSL_demoDlg::OnPaint () {
	if (IsIconic ())	{
		CPaintDC dc(this); // device context for painting

		SendMessage (WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc (), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics (SM_CXICON);
		int cyIcon = GetSystemMetrics (SM_CYICON);
		CRect rect;
		GetClientRect (&rect);
		int x = (rect.Width () - cxIcon + 1) / 2;
		int y = (rect.Height () - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon (x, y, m_hIcon);
	}
	else {
		CDialog::OnPaint ();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCTabCtrlSSL_demoDlg::OnQueryDragIcon () {
	return (HCURSOR) m_hIcon;
}

void CCTabCtrlSSL_demoDlg::OnButtonBasic () {
	AfxMessageBox (_T("Button click handled by parent dialog"));
}
void CCTabCtrlSSL_demoDlg::OnButtonPrev () {
	int nCurPage = m_tabDemo.GetSSLActivePage ();
	int nPrevPage = nCurPage;
	if (0 == nCurPage) {
		// Already at first tab, so go to last tab
		nPrevPage = m_tabDemo.GetSSLPageCount () - 1;
	}
	else {
		nPrevPage = nCurPage - 1;
	}
	
	m_tabDemo.ActivateSSLPage (nPrevPage);
}

void CCTabCtrlSSL_demoDlg::OnButtonNext () {
	int nCurPage = m_tabDemo.GetSSLActivePage ();
	int nNextPage = nCurPage;
	if (m_tabDemo.GetSSLPageCount () - 1 == nCurPage) {
		// Already at last tab, so go to first tab
		nNextPage = 0;
	}
	else {
		nNextPage = nCurPage + 1;
	}
	
	m_tabDemo.ActivateSSLPage (nNextPage);
}
