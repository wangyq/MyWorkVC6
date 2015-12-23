// CTabCtrlSSL_demoDlg.h : header file
//

#if !defined(AFX_CTABCTRLSSL_DEMODLG_H__661EC364_263A_4E5A_B66A_51D515472FA6__INCLUDED_)
#define AFX_CTABCTRLSSL_DEMODLG_H__661EC364_263A_4E5A_B66A_51D515472FA6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCTabCtrlSSL_demoDlg dialog
#include "TabCtrlSSL.h"
#include "ButtonSSL.h"
#include "AboutTab.h"	// Added by ClassView
#include "AdvancedTab.h"	// Added by ClassView

class CCTabCtrlSSL_demoDlg : public CDialog
{
// Construction
public:
	CCTabCtrlSSL_demoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCTabCtrlSSL_demoDlg)
	enum { IDD = IDD_CTABCTRLSSL_DEMO_DIALOG };
	CTabCtrlSSL	m_tabDemo;
	CButtonSSL	m_btnPrev;
	CButtonSSL	m_btnNext;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCTabCtrlSSL_demoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CAdvancedTab m_advancedTab;
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCTabCtrlSSL_demoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonPrev();
	afx_msg void OnButtonNext();
	//}}AFX_MSG
	afx_msg void OnButtonBasic ();
	DECLARE_MESSAGE_MAP()
private:
	CAboutTab m_aboutTab;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CTABCTRLSSL_DEMODLG_H__661EC364_263A_4E5A_B66A_51D515472FA6__INCLUDED_)
