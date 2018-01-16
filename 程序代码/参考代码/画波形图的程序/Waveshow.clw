; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=Wavepic
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Waveshow.h"

ClassCount=4
Class1=CWaveshowApp
Class2=CWaveshowDlg
Class3=CAboutDlg

ResourceCount=5
Resource1=IDD_WAVESHOW_DIALOG
Resource2=IDR_MAINFRAME
Resource3=IDD_WAVEPIC
Resource4=IDD_ABOUTBOX
Class4=Wavepic
Resource5=IDR_MENU1

[CLS:CWaveshowApp]
Type=0
HeaderFile=Waveshow.h
ImplementationFile=Waveshow.cpp
Filter=D

[CLS:CWaveshowDlg]
Type=0
HeaderFile=WaveshowDlg.h
ImplementationFile=WaveshowDlg.cpp
Filter=W
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CWaveshowDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=WaveshowDlg.h
ImplementationFile=WaveshowDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_WAVESHOW_DIALOG]
Type=1
Class=CWaveshowDlg
ControlCount=0

[MNU:IDR_MENU1]
Type=1
Class=CWaveshowDlg
Command1=IDM_SHOWWAVE
Command2=IDM_QUIT
CommandCount=2

[DLG:IDD_WAVEPIC]
Type=1
Class=Wavepic
ControlCount=17
Control1=IDC_COORD,static,1342177287
Control2=IDC_STATIC,static,1342308352
Control3=IDC_LEVEL1,button,1342308361
Control4=IDC_LEVEL2,button,1342177289
Control5=IDC_LEVEL3,button,1342177289
Control6=IDC_STATIC,button,1342177287
Control7=IDC_LEVEL4,button,1342177289
Control8=IDC_RADIO5,button,1342308361
Control9=IDC_RADIO6,button,1342177289
Control10=IDC_RADIO7,button,1342177289
Control11=IDC_STATIC,button,1342177287
Control12=IDC_RADIO8,button,1342177289
Control13=IDC_BTN_STEADY,button,1342242816
Control14=IDC_SLI_POS,msctls_trackbar32,1342242842
Control15=IDC_STATIC,button,1342177287
Control16=IDC_SLI_HEX,msctls_trackbar32,1342242840
Control17=IDC_STATIC,button,1342177287

[CLS:Wavepic]
Type=0
HeaderFile=Wavepic.h
ImplementationFile=Wavepic.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_BTN_STEADY
VirtualFilter=dWC

