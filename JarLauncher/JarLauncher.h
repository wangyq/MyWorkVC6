// JarLauncher.h : main header file for the JARLAUNCHER application
//

#if !defined(AFX_JARLAUNCHER_H__F1366CC9_D603_446E_B89B_B545C18AA81F__INCLUDED_)
#define AFX_JARLAUNCHER_H__F1366CC9_D603_446E_B89B_B545C18AA81F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CJarLauncherApp:
// See JarLauncher.cpp for the implementation of this class
//

class CJarLauncherApp : public CWinApp
{
public:
	CJarLauncherApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJarLauncherApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CJarLauncherApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JARLAUNCHER_H__F1366CC9_D603_446E_B89B_B545C18AA81F__INCLUDED_)
