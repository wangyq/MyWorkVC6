// TabViewMain.h: interface for the CTabViewMain class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABVIEWMAIN_H__0919917A_46B8_4415_89C7_90F3C93E6FB4__INCLUDED_)
#define AFX_TABVIEWMAIN_H__0919917A_46B8_4415_89C7_90F3C93E6FB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTabViewMain : public CDialogImpl<CTabViewMain>
{
    public:
        enum { IDD = IDD_VIEW_MAIN };

        BOOL PreTranslateMessage(MSG* pMsg)
        {
            return IsDialogMessage(pMsg);
        }

        BEGIN_MSG_MAP(CTabViewMain)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        COMMAND_ID_HANDLER(IDC_BUTTON_DIR, OnDirCmd)
        END_MSG_MAP()

        // Handler prototypes (uncomment arguments if needed):
        //	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
        //	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
        //	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

        LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
        {
            //CenterWindow(GetParent());
            return TRUE;
        }

        LRESULT OnDirCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
        {
            //
            //MessageBox(_T("dir"));
            TCHAR szDir[MAX_PATH];
            BROWSEINFO bi;
            ITEMIDLIST* pidl;
            bi.hwndOwner = this->m_hWnd;
            bi.pidlRoot = NULL;
            bi.pszDisplayName = szDir;
            bi.lpszTitle = _T("Please Select a directory:");
            bi.ulFlags = BIF_RETURNONLYFSDIRS;
            bi.lpfn = NULL;
            bi.lParam = 0;
            bi.iImage = 0;
            pidl = SHBrowseForFolder(&bi);

            if (pidl != NULL)
            {

                if (SHGetPathFromIDList(pidl, szDir))
                {
                    CEdit m_edit;
                    m_edit.Attach(GetDlgItem(IDC_EDIT_DIR));
                    m_edit.SetWindowText(szDir);
                }
            }

            return 0;
        }
};

#endif // !defined(AFX_TABVIEWMAIN_H__0919917A_46B8_4415_89C7_90F3C93E6FB4__INCLUDED_)
