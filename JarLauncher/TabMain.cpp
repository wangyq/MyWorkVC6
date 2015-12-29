// TabMain.cpp : implementation file
//

#include "stdafx.h"
#include "JarLauncher.h"
#include "TabMain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabMain
IMPLEMENT_DYNAMIC(CTabMain,CTabCtrlSSL)

CTabMain::CTabMain()
{
}

CTabMain::~CTabMain()
{
}


BEGIN_MESSAGE_MAP(CTabMain, CTabCtrlSSL)
	//{{AFX_MSG_MAP(CTabMain)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabMain message handlers

int CTabMain::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTabCtrlSSL::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	//AfxMessageBox("tabctrl main");

	return 0;
}

void CTabMain::Init()
{
	int nPageID = 0;
	AddSSLPage (_T("Basic Tab"), nPageID++, IDD_DIALOG_MAIN);
	AddSSLPage (_T("Pref Tab"), nPageID++, IDD_DIALOG_PREF);
}

BOOL CTabMain::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CTabCtrlSSL::PreCreateWindow(cs);
}
