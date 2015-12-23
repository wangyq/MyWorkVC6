#if !defined(AFX_ABOUTTAB_H__CA963FA2_DFA0_482C_B792_544D23ABC329__INCLUDED_)
#define AFX_ABOUTTAB_H__CA963FA2_DFA0_482C_B792_544D23ABC329__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AboutTab.h : header file
//

#include "TabPageSSL.h"
#include "ButtonSSL.h"

/////////////////////////////////////////////////////////////////////////////
// CAboutTab dialog

class CAboutTab : public CTabPageSSL
{
// Construction
public:
	CAboutTab(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAboutTab)
	enum { IDD = IDD_TAB_ABOUT };
	CButtonSSL	m_btnEmail;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutTab)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAboutTab)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ABOUTTAB_H__CA963FA2_DFA0_482C_B792_544D23ABC329__INCLUDED_)
