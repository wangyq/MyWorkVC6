// JarLauncherDlg.h : header file
//

#if !defined(AFX_JARLAUNCHERDLG_H__0C026244_2651_48DF_8E41_8A1821ACA918__INCLUDED_)
#define AFX_JARLAUNCHERDLG_H__0C026244_2651_48DF_8E41_8A1821ACA918__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CJarLauncherDlg dialog

class CJarLauncherDlg : public CDialog
{
// Construction
public:
	CJarLauncherDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CJarLauncherDlg)
	enum { IDD = IDD_JARLAUNCHER_DIALOG };
	CTabMain	m_tabMain;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJarLauncherDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CJarLauncherDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JARLAUNCHERDLG_H__0C026244_2651_48DF_8E41_8A1821ACA918__INCLUDED_)
