; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CTabMain
LastTemplate=CTabCtrl
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "JarLauncher.h"

ClassCount=4
Class1=CJarLauncherApp
Class2=CJarLauncherDlg
Class3=CAboutDlg

ResourceCount=5
Resource1=IDD_DIALOG_PREF
Resource2=IDR_MAINFRAME
Resource3=IDD_JARLAUNCHER_DIALOG
Class4=CTabMain
Resource4=IDD_DIALOG_MAIN
Resource5=IDD_ABOUTBOX

[CLS:CJarLauncherApp]
Type=0
HeaderFile=JarLauncher.h
ImplementationFile=JarLauncher.cpp
Filter=N

[CLS:CJarLauncherDlg]
Type=0
HeaderFile=JarLauncherDlg.h
ImplementationFile=JarLauncherDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_TAB_MAIN

[CLS:CAboutDlg]
Type=0
HeaderFile=JarLauncherDlg.h
ImplementationFile=JarLauncherDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_JARLAUNCHER_DIALOG]
Type=1
Class=CJarLauncherDlg
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_TAB_MAIN,SysTabControl32,1342177280

[DLG:IDD_DIALOG_MAIN]
Type=1
Class=?
ControlCount=1
Control1=IDC_LIST_FILE,listbox,1352729347

[CLS:CTabMain]
Type=0
HeaderFile=TabMain.h
ImplementationFile=TabMain.cpp
BaseClass=CTabCtrlSSL
Filter=N
VirtualFilter=UWC

[DLG:IDD_DIALOG_PREF]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1342308352
Control2=IDC_BUTTON1,button,1342242816
Control3=IDC_CHECK1,button,1342242819
Control4=IDC_EDIT1,edit,1350631552

