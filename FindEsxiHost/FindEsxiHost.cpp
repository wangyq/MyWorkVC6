// FindEsxiHost.cpp : main source file for FindEsxiHost.exe
//

#include "stdafx.h"



#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlctrlw.h>
#include <atlctrlx.h>

#include "resource.h"

#include "aboutdlg.h"

#include "TabViewAbout.h"
#include "TabViewMain.h"
#include "TabCtrlMain.h"

#include "maindlg.h"

#pragma  comment(lib,"ws2_32.lib")

CAppModule _Module;

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
    CMessageLoop theLoop;
    _Module.AddMessageLoop(&theLoop);

    CMainDlg dlgMain;

    if (dlgMain.Create(NULL) == NULL) {
        ATLTRACE(_T("Main dialog creation failed!\n"));
        return 0;
    }

    dlgMain.ShowWindow(nCmdShow);

    int nRet = theLoop.Run();

    _Module.RemoveMessageLoop();
    return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
    HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//  HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
    ATLASSERT(SUCCEEDED(hRes));

    // this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
    ::DefWindowProc(NULL, 0, 0, 0L);

    AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES);  // add flags to support other controls

    hRes = _Module.Init(NULL, hInstance);
    ATLASSERT(SUCCEEDED(hRes));

    int nRet = Run(lpstrCmdLine, nCmdShow);

    _Module.Term();
    ::CoUninitialize();

    return nRet;
}

//////////////////////////////////////////////////////////////////////////

CString GetCurrentTimeStr()
{
	CString strTime;

	SYSTEMTIME st;
	GetLocalTime(&st);
	strTime.Format(_T("%4d-%2d-%2d %2d:%2d:%2d"),st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);
	return strTime;
}

int Str2Int(LPCTSTR szStr)
{
	int res = 0;
	if( szStr !=0 ){
		const TCHAR* pChar = szStr;
		while( *pChar != _T('\0')){
			if( (*pChar <_T('0')) || (*pChar >_T('9')) )
				break;
			res = res*10 + (*pChar - _T('0'));
			pChar++;
		}
	}
	
	return res;
}

//check whether ipv4 string , for example "192.168.0.0",is valid.
BOOL isIPv4StrValid(const CString & strIP)
{
    int ip = 0;
    if (strIP.IsEmpty()) return FALSE;
    CString tmp = strIP;
    tmp.TrimLeft();
    tmp.TrimRight();
    return IPv4StrToInt(strIP, ip);
}

//
CString Int2IPv4Str(int ip)
{
    CString tmp;
    tmp.Format("%d.%d.%d.%d", (ip >> 24) & 0xFF, (ip >> 16) & 0xFF, (ip >> 8) & 0xFF, (ip) & 0xFF);
    return tmp;
}

//here ip str must be valid and no space char in left and right.
BOOL IPv4StrToInt(const CString& strIP, int & ip)
{
    BOOL bOK = FALSE;

    if (strIP.IsEmpty()) return FALSE;

    int iFirst = 0, iLast = 0, iPos = 0;
    int segment = 0, num = 0;
    ip = 0;
    iPos = iFirst + iLast;
    do {

        if ((strIP.GetAt(iPos) >= _T('0')) && (strIP.GetAt(iPos) <= _T('9'))) {
            segment = segment * 10 + (strIP.GetAt(iPos) - _T('0'));
            iLast ++; //move next
        } else if (strIP.GetAt(iPos) == _T('.')) {
            if ((segment < 0) || (segment > 255)) {
                break;
            }
            ip = (ip << 8) + segment;

            segment = 0;
            iFirst = iPos + 1;
            iLast = 0;
            num++;
        } else {
            break; //wrong ip address!
        }
        //next position.
        iPos = iFirst + iLast;

        if (iPos >= strIP.GetLength()) { //finished now!
            if ((segment < 0) || (segment > 255)) {
                break;
            }
            num++;
            if (num != 4) break;

            ip = (ip << 8) + segment; //update ip value!
            bOK = TRUE;
            break;
        }

        //check index if right!
        if ((iLast > 3) || (num > 4)) {
            break;
        }
    } while (TRUE);
    if (!bOK) ip = 0;   //
    return bOK;
}

struct tagThreadParam{
	HWND hWnd;
	CString strToken;
	int nPort;
	CSimpleValArray<int> arrIP;
};

int bForceStop = FALSE;     //Only set by button scan.
int bThreadStopped = TRUE;  //Only set by work thread itself!

DWORD dwThreadID = 0;
HANDLE hWorkThread = NULL;
static struct tagThreadParam theParam;  //using global variable to pass parameter!

//Do check every host to find whether it is a esxi host!
DWORD WINAPI ThreadFun(LPVOID lpParam){
	struct tagThreadParam *pParam = (struct tagThreadParam *)lpParam;
	
	CSimpleValArray<int> & arrIP = pParam->arrIP;
	int nPort = pParam->nPort;
	const CString& strToken = pParam->strToken;
	
	CString strResult;

	bThreadStopped = FALSE; //NOW begin
	
	::PostMessage(pParam->hWnd,MSG_ESXI_HOST_CHECK,OP_ESXI_CHECK_START,(LPARAM)0); //check begin

	for(int i=0; (i<arrIP.GetSize()) && !bForceStop ;i++){
		::PostMessage(pParam->hWnd,MSG_ESXI_HOST_CHECK,OP_ESXI_CHECKING,(LPARAM)arrIP[i]);
		
		if( !ScanEsxiHost(Int2IPv4Str(arrIP[i]).GetBuffer(15),nPort,strResult)){
			continue;  //not found host!
		}
		if( strResult.Find(strToken) != -1 ){//find esxi host now!
			::PostMessage(pParam->hWnd,MSG_ESXI_HOST_CHECK,OP_ESXI_FINDED,(LPARAM)arrIP[i]);
		}
	}

	::PostMessage(pParam->hWnd,MSG_ESXI_HOST_CHECK,OP_ESXI_FINISHED,(LPARAM)0);

	bThreadStopped = TRUE; //now stopped!

	return TRUE;
}
//
BOOL ScanEsxiHost(HWND hWnd, const CSimpleValArray<int> & arrIP, const CString strToken,int nPort){
	theParam.hWnd = hWnd;
	theParam.strToken = strToken;
	theParam.nPort = nPort;
	//theParam.arrIP = arrIP;  //here copy the arrIP
	theParam.arrIP.RemoveAll(); //first , must remove all .
	for(int i=0;i<arrIP.GetSize();i++){
		theParam.arrIP.Add(arrIP[i]);
	}

	hWorkThread = CreateThread(NULL,0,ThreadFun,(LPVOID)&theParam,0,&dwThreadID);
	if( !hWorkThread ){
		return FALSE;
	}
	//Close kernel Object!
	CloseHandle(hWorkThread);
	hWorkThread = NULL;

	return TRUE;
}


BOOL ScanEsxiHost(LPCTSTR strHost, int nPort, CString& strResult)
{
#define MAX_MSG_SIZE 256


    WORD sockVersion = MAKEWORD(2, 2);
    WSADATA data;
    if (WSAStartup(sockVersion, &data) != 0) {
        return FALSE;
    }

    SOCKET sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sclient == INVALID_SOCKET) {
        //MessageBox("invalid socket !");
        return FALSE;
    }

    sockaddr_in serAddr;
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(nPort);
    serAddr.sin_addr.S_un.S_addr = inet_addr(strHost); //ÍøÂç×Ö½ÚÐò
    if (connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR) {
        //MessageBox("connect error !");
        closesocket(sclient);
        return FALSE;
    }
    
	char * sendData = "Hello world,haha!first send some data to server! \r\n";  //first send some data to server
    send(sclient, sendData, strlen(sendData), 0);

    char recData[MAX_MSG_SIZE + 1];
    int ret = recv(sclient, recData, MAX_MSG_SIZE, 0);
    if (ret > 0) {
        recData[ret] = '\0';
        strResult.Format("%s", recData);
        //::MessageBox(NULL,recData);
    }
    closesocket(sclient);
    WSACleanup();

    return TRUE;
}