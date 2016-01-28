// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__06A85129_D1FF_4F99_82B9_DB5D21A2F4B9__INCLUDED_)
#define AFX_STDAFX_H__06A85129_D1FF_4F99_82B9_DB5D21A2F4B9__INCLUDED_

// Change these values to use different versions
#define WINVER		0x0400
//#define _WIN32_WINNT	0x0400
#define _WIN32_IE	0x0400
#define _RICHEDIT_VER	0x0100

#include <atlbase.h>

//#define _WTL_NO_CSTRING
//#define _WTL_NO_WTYPES
//#include <atlstr.h>



#include <atlapp.h>

extern CAppModule _Module;

#include <atlwin.h>

#define _WTL_USE_CSTRING
#include <atlmisc.h>

// User define 
#define MSG_ESXI_HOST_CHECK   (WM_USER+100)
#define OP_ESXI_CHECKING 1
#define OP_ESXI_FINDED  2
#define OP_ESXI_FINISHED  3
#define OP_ESXI_CHECK_START 4

class CWorker;
template <class Worker> class CThreadPool;

extern CThreadPool<CWorker> thePool;

extern int bForceStop;
extern int bThreadStopped ;

int Str2Int(LPCTSTR szStr);
BOOL isIPv4StrValid(const CString & strIP);
CString Int2IPv4Str(int ip);
BOOL IPv4StrToInt(const CString& strIP, int & ip) ;
CString GetCurrentTimeStr();

BOOL ScanEsxiHost(HWND hWnd, const CSimpleValArray<int> & arrIP, const CString strToken,int nPort);
BOOL ScanEsxiHost(LPCTSTR strHost, int nPort, CString& strResult);


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__06A85129_D1FF_4F99_82B9_DB5D21A2F4B9__INCLUDED_)
