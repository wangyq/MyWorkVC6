// TabViewMain.h: interface for the CTabViewMain class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABVIEWMAIN_H__0919917A_46B8_4415_89C7_90F3C93E6FB4__INCLUDED_)
#define AFX_TABVIEWMAIN_H__0919917A_46B8_4415_89C7_90F3C93E6FB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

template <class T>
class CMySimpleValArray : public CSimpleValArray< T >
{
public:
	void InsertAt(int nIndex, T val)
	{
		ATLASSERT(nIndex >= 0 && nIndex <= m_nSize); //maybe the last location!

		//
		if( nIndex == m_nSize ){
			this->Add(val);  //allocate space!

		} else{ //not the last location!

			this->Add(val);  //allocate space!
			for( int i = this->GetSize()-1; i>nIndex ; i--){
				m_aT[i] = m_aT[i-1] ;	
			}
			m_aT[nIndex] = val;

		}		

	}
	//=======================
	// insert value with order: 
	// bAsc = true, ascending, 
	// bAsc = false, descending
	void InsertOrdered(T val, bool bAsc=true)
	{
		this->Add(val); //resize space

		int i = 0;
		if( bAsc ){ //asc order
			//bubble order
			for( i=m_nSize-1; i>0; i--)
			{ // the last is val
				if( m_aT[i-1] > val )
				{
					m_aT[i] = m_aT[i-1];
				} else{//find location
					break;
				}
			} // end of for
			
			
		} else { //desc order
			//bubble order
			for( i=m_nSize-1; i>0; i--)
			{ // the last is val
				if( m_aT[i-1] < val )
				{
					m_aT[i] = m_aT[i-1];
				} else{//find location
					break;
				}
			} // end of for
		}// end of if
		
		m_aT[i] = val;  //insert val!
	}

};


class CWorker:public CWorkerBase<int, true,0>{ //autoexit = true, exit request = 0
public:
	typedef struct tagThreadParam{
		HWND hWnd;
		CString strToken;
		CString strTokenEsxi;
		int nPort;		
	}ThreadParam,*PThreadParam;
static ThreadParam theWorkerParam;


public:

	BOOL Initialize(void * /*pvParam*/, bool bFirstStartup = false) ;

	void Execute(
		 RequestType request,
		 void *pvWorkerParam,
		 OVERLAPPED *pOverlapped);
	
	void Terminate(void* /*pvParam*/ , bool bLastExecute = false);
	
};

class CTabViewMain : public CDialogImpl<CTabViewMain>
{
private:

    enum {
        E_STATE_NORMAL = 1,
        E_STATE_RUNNING = 2,
        E_STATE_FORCESTOP = 3,
		E_INCREMENT_OUTPUT_BUFFER = (1<<20),   // 1M,the CEdit output buffer Increment.
		E_INCREMENT_OUTPUT_BUFFER_MAX = 8*(1<<20), //8M MAX output buffer.
        E_STATE_END
    };

    int m_bState ; //current running state!

    CButton m_btnScan;
    CEdit m_ctrIPAddr;
    CEdit m_ctrlEsxiHost;

    CString m_strEsxiHostToken;
	CString m_strEsxiHostTokenEsxi;
    int m_iPort;

    //======================
    void LoadTokenAndPort() {

#define MAX_SIZE 255
#define PORT 902
#define TOKEN "VMware Authentication Daemon"
#define TOKEN_ESXI "VMXARGS"

        int last = 0;
        TCHAR szStr[MAX_SIZE + 2];
        if ((last = AtlLoadString(IDS_ESXI_HOST_PORT, szStr, MAX_SIZE)) > 0) {
            m_iPort = Str2Int(szStr);  //get port
        } else {
            m_iPort = PORT;
        }

		//load token
        if ((last = AtlLoadString(IDS_ESXI_HOST_TOKEN, szStr, MAX_SIZE)) > 0) {
            szStr[last] = _T('\0');
            m_strEsxiHostToken = CString(szStr);  //get token
        } else {
            m_strEsxiHostToken = CString(TOKEN);
        }

		//load esxi_token
		if ((last = AtlLoadString(IDS_ESXI_HOST_TOKEN_ESXI, szStr, MAX_SIZE)) > 0) {
            szStr[last] = _T('\0');
            m_strEsxiHostTokenEsxi = CString(szStr);  //get esxi_token
        } else {
            m_strEsxiHostTokenEsxi = CString(TOKEN_ESXI);
        }
    }


#if 0
    //Split string
    //wrong method, the element of  Array CString is temporatly variable!
    void SplitString(CString strSrc, CString strSplit, CSimpleValArray<CString> & arrStr) {

        //CSimpleValArray<CString> arrStr;
        int iFirst = 0, iLast = 0; //
        CString tmp;
        do {
            iLast = strSrc.Find((LPCTSTR)strSplit);
            if (-1 == iLast) {
                //not found now!
                tmp = strSrc.Mid(iFirst);
                if (!tmp.IsEmpty()) {
                    arrStr.Add(tmp); //
                }
                break;
            } else {
                tmp = strSrc.Mid(iFirst, iLast - iFirst);
                if (!tmp.IsEmpty()) {
                    arrStr.Add(tmp); //
                }
                iFirst = iLast + 1; //ignore the find char!
            }

        } while (TRUE);

        //return arrStr;
    }
#endif //if 0

    //Available is : 192.168.0.10-120, or 192.168.1.40 or 172.16.1.0/16
    BOOL ProcessIPNet(const CString& strIP, CSimpleValArray<int> & arrIP) {

        if (strIP.IsEmpty()) return TRUE;

        int ip = 0;
        int iPos = -1;

        if ((iPos = strIP.Find(_T("/"))) != -1) {//172.16.1.0/16
			CString strBase = strIP.Left(iPos);
			if (!IPv4StrToInt(strBase, ip)) return FALSE;

			int masklen = Str2Int(strIP.Mid(iPos + 1)); //16
			if( masklen<=0 || masklen>32 ) return FALSE;

			if( masklen == 32 ){
				arrIP.Add(ip);
			} else{
				int start = ip & (((1<<masklen)-1)<<(32-masklen));
				int count = 1<<(32-masklen);
				for(int i=0;i<count;i++){
					arrIP.Add(start+i);
				}
			}
        } else if ((iPos = strIP.Find(_T("-"))) != -1) {//192.168.0.20-120
            int start, end;

            CString strBase = strIP.Left(iPos); //192.168.0.20
            if (!IPv4StrToInt(strBase, ip)) return FALSE;

            end = Str2Int(strIP.Mid(iPos + 1));  //120

            iPos = strBase.ReverseFind(_T('.'));
            start = Str2Int(strBase.Mid(iPos + 1)); //20

            if (start > end) return FALSE;

            for (int i = 0; i <= (end - start); i++) {
                arrIP.Add(ip + i);
            }
        } else { // not found. so it is : 192.168.0.20
            if (IPv4StrToInt(strIP, ip)) {
                arrIP.Add(ip);
            } else {
                return FALSE;  //wrong ip address!
            }

        }
        return TRUE;
    }

    //Available format is : 192.168.0.10-200, 192.168.1.30, 172.16.1.0/16
    //
    BOOL ParseIPAddress(const CString& strIP, CSimpleValArray<int> & arrIP) {

        if (strIP.IsEmpty()) {
            return FALSE;
        }

        int iFirst = 0, iLast = 0;
        CString strItem;
        do {
            iLast = strIP.Find(_T(","), iFirst); //find the split char
            if (-1 == iLast) {
                strItem = strIP.Mid(iFirst);

            } else {
                strItem = strIP.Mid(iFirst, iLast - iFirst);
                iFirst = iLast + 1;
            }
            strItem.TrimLeft();
            strItem.TrimRight();
            if (!ProcessIPNet(strItem, arrIP)) return FALSE;  //wrong format!

        } while (iLast != -1);

        return TRUE;
    }

    // return :
    // TRUE if start successful, FALSE otherwise.
    BOOL StartRunning() {
#define MAX_SIZE 255

        TCHAR szIP[MAX_SIZE + 2] ;
        CString strIP("");

        int iLast = 0;
        iLast = m_ctrIPAddr.GetWindowText(szIP, MAX_SIZE);
        if (iLast > 0) {
            szIP[iLast] = _T('\0');
            strIP = CString(szIP);
            strIP.TrimLeft();
            strIP.TrimRight();
        }
        if (strIP.IsEmpty()) {
            MessageBox(_T("IP address is NULL!"), _T("IP Address"), MB_ICONEXCLAMATION);
            m_ctrIPAddr.SetWindowText("");
            m_ctrIPAddr.SetFocus();
            return FALSE;
        }

        CSimpleValArray<int> arrIP;
        if (!ParseIPAddress(strIP, arrIP)) {
            MessageBox(_T("Wrong format of IP address!"), _T("IP Address"), MB_ICONEXCLAMATION);
            return FALSE;
        }

        /*if (!ScanEsxiHost(m_hWnd, arrIP, m_strEsxiHostToken, m_iPort)) {
            MessageBox(_T("Can not START scanning thread!"), _T("Thread run"), MB_ICONEXCLAMATION);
            return FALSE;
        }*/
		
		if( !thePool.Start(arrIP.GetSize()) ){
			MessageBox(_T("Can not START scanning thread!"), _T("Thread run"), MB_ICONEXCLAMATION);
            return FALSE;
		}
		
		//::PostMessage(m_hWnd, MSG_ESXI_HOST_CHECK, OP_ESXI_CHECK_START, (LPARAM)0); //check begin

		arrIP.Add(0);// end flags
		for(int i=0;i<arrIP.GetSize();i++){
			thePool.AddWorkerRequest(arrIP[i]);
		}

        return TRUE;
    }
    //////////////////////////////////////////////////////////////////////////

public:
    enum { IDD = IDD_DIALOG_VIEW_MAIN };

    BOOL PreTranslateMessage(MSG* pMsg) {
        return IsDialogMessage(pMsg);
    }

    BEGIN_MSG_MAP(CTabViewMain)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    COMMAND_ID_HANDLER(IDC_BUTTON_SCAN, OnBtnScan)
    MESSAGE_HANDLER(MSG_ESXI_HOST_CHECK, OnCheckEsxi)
    REFLECT_NOTIFICATIONS()
    END_MSG_MAP()

    // Handler prototypes (uncomment arguments if needed):
    //  LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
    //  LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    //  LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
        CenterWindow(GetParent());

        m_bState = E_STATE_NORMAL;

        m_btnScan.Attach(GetDlgItem(IDC_BUTTON_SCAN));
        m_ctrIPAddr.Attach(GetDlgItem(IDC_EDIT_IPADDR));

        m_ctrlEsxiHost.Attach(GetDlgItem(IDC_EDIT_ESXI_HOST));
		//m_ctrlEsxiHost.SetLimitText(0); //set maximum buffer size!

        LoadTokenAndPort(); //load token and port to scan for.


		//== init search parameter
		CWorker::theWorkerParam.hWnd = m_hWnd;
		CWorker::theWorkerParam.strToken = m_strEsxiHostToken;
		CWorker::theWorkerParam.strTokenEsxi = m_strEsxiHostTokenEsxi;
		CWorker::theWorkerParam.nPort = m_iPort;

		//
		CString strNumPerCore;
		strNumPerCore.LoadString(IDS_THREAD_NUMBER_PER_CORE);
		int nNumPerCore = Str2Int(strNumPerCore);
		
		SYSTEM_INFO si;  
		GetSystemInfo(&si);
		thePool.Initialize((void*)(&(CWorker::theWorkerParam)),si.dwNumberOfProcessors, nNumPerCore);

        return TRUE;
    }

	//==========================
	// scan 
	void DoOK()
	{
		m_btnScan.SendMessage(BM_CLICK);
		//m_btnScan.PostMessage(BM_CLICK);   // other windows send to BUTTON
		//m_btnScan.PostMessage(BN_CLICKED); //button's parent window to receive a BN_CLICKED message
	}

    //===========================
    LRESULT OnCheckEsxi(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
        unsigned int ip = (int)lParam;
        CString strIP = Int2IPv4Str(ip);
        CString strInfo;
        static CMySimpleValArray<unsigned int> arrHosts, arrHostsEsxi;

        if (wParam == OP_ESXI_CHECK_START) {
			arrHosts.RemoveAll();
			arrHostsEsxi.RemoveAll();

            strInfo.Format(_T("[%s] Start scanning : \r\n"), GetCurrentTimeStr());

        } else if (wParam == OP_ESXI_CHECKING) {
            strInfo.Format("Check %s ...\r\n", strIP);
        } else if (wParam == OP_ESXI_FINDED) {
			arrHosts.InsertOrdered(ip); //

            strInfo.Format("Possible Vmware host found:  %s ...\r\n", strIP);
        } else if (wParam == OP_ESXI_FINDED_ESXI) {
			arrHostsEsxi.InsertOrdered(ip); //

            strInfo.Format("Possible Vmware Esxi host found:  %s ...\r\n", strIP);
        } else if (wParam == OP_ESXI_FINISHED) {
            m_btnScan.EnableWindow(TRUE); //enable the button!

            CString tmp;
            tmp.Format(_T("[%s] Scan finished!\r\n"), GetCurrentTimeStr());
            strInfo += tmp;

            tmp.Format(_T("==> Number Esxi Host found is %d .\r\n"), arrHosts.GetSize() + arrHostsEsxi.GetSize());
            strInfo += tmp;
            for (int i = 0; i < arrHosts.GetSize(); i++) {
                strInfo += Int2IPv4Str(arrHosts[i]);
			strInfo += _T(" ----> Vmware Host");
                strInfo += _T("\r\n");
            } //end of for
			for ( i = 0; i < arrHostsEsxi.GetSize(); i++) {
				strInfo += Int2IPv4Str(arrHostsEsxi[i]);
				strInfo += _T(" ----> Vmware Esxi Host");
                strInfo += _T("\r\n");
			}

            strInfo += _T("\r\n");

            //======== restore scan button ======
            m_btnScan.SetWindowText(_T("Scan"));
            m_btnScan.EnableWindow(TRUE);  //enable now
            m_bState = E_STATE_NORMAL;     //normal state!
            //======= restore bForeStop  ========
            bForceStop = FALSE;

        } else {
            strInfo = _T("Unkown msg!\r\n");
        }
        int nLength = m_ctrlEsxiHost.SendMessage(WM_GETTEXTLENGTH);
		int nLimit = m_ctrlEsxiHost.GetLimitText();
		if( nLimit - nLength < strInfo.GetLength() ){ //Clear output content!
			//int nMax = (nLimit + E_INCREMENT_OUTPUT_BUFFER) ;
			//nMax = nMax < E_INCREMENT_OUTPUT_BUFFER_MAX? nMax: E_INCREMENT_OUTPUT_BUFFER_MAX;
			m_ctrlEsxiHost.SetWindowText(_T(""));
			//m_ctrlEsxiHost.SetLimitText(nMax); //incrment buffer size!
			nLength = 0;//
		}
        m_ctrlEsxiHost.SetSel(nLength,  nLength);
        m_ctrlEsxiHost.ReplaceSel(strInfo);

        return TRUE;
    }

    LRESULT OnBtnScan(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
        //
		//thePool.Start();

		/*for(int i=0;i<10;i++){
			thePool.AddWorkerRequest(i);
		}
		thePool.Start();
		thePool.PostWorkerEndRequest();
		*/
	
        if (m_bState == E_STATE_NORMAL) {
            if (StartRunning()) {
                m_bState = E_STATE_RUNNING;
                m_btnScan.SetWindowText(_T("Stop"));
            }
        } else if (m_bState == E_STATE_RUNNING) {//
            m_bState = E_STATE_FORCESTOP;  //force stop!
            bForceStop = TRUE;  //request work thread to stop!
			thePool.Stop();     //force stop
            m_btnScan.EnableWindow(FALSE); //disable the button!
        } else { //nothing to do!
        }

        return 0;
    }

	bool CanExit(){
		bool bOk = false;
		if (m_bState == E_STATE_NORMAL) {
			bOk = true;
		}else if (m_bState == E_STATE_RUNNING) {//
			MessageBox(_T("Worker Thread is running! Please stop it first!"),_T("Stop"),MB_ICONEXCLAMATION|MB_OKCANCEL);
		}else if (m_bState == E_STATE_FORCESTOP) {//
			MessageBox(_T("Worker Thread is stopping! Please wait a second."),_T("Exit"),MB_ICONINFORMATION);
		} else{//unkown state
			bOk = true;
		}

		return bOk;
	}
};

#endif // !defined(AFX_TABVIEWMAIN_H__0919917A_46B8_4415_89C7_90F3C93E6FB4__INCLUDED_)
