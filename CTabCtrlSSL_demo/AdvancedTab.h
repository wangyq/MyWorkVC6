#if !defined(AFX_ADVANCEDTAB_H__8D37A83E_8EE2_4DD7_A60F_3D732A1F4BBF__INCLUDED_)
#define AFX_ADVANCEDTAB_H__8D37A83E_8EE2_4DD7_A60F_3D732A1F4BBF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdvancedTab.h : header file
//

#include "TabPageSSL.h"

/////////////////////////////////////////////////////////////////////////////
// CAdvancedTab dialog

class CAdvancedTab : public CTabPageSSL {
// Construction
public:
	CAdvancedTab (CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAdvancedTab)
	enum { IDD = IDD_TAB_ADVANCED };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdvancedTab)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAdvancedTab)
	afx_msg void OnButtonAdvanced();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADVANCEDTAB_H__8D37A83E_8EE2_4DD7_A60F_3D732A1F4BBF__INCLUDED_)
