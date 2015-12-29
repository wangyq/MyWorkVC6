#if !defined(AFX_TABMAIN_H__DF1E1120_A753_4774_B861_D75611B0B704__INCLUDED_)
#define AFX_TABMAIN_H__DF1E1120_A753_4774_B861_D75611B0B704__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabMain.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTabMain window

class CTabMain : public CTabCtrlSSL
{
	DECLARE_DYNAMIC(CTabMain)

// Construction
public:
	CTabMain();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabMain)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	void Init();
	virtual ~CTabMain();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTabMain)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABMAIN_H__DF1E1120_A753_4774_B861_D75611B0B704__INCLUDED_)
