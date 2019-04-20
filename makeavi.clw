; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMakeaviDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "makeavi.h"

ClassCount=4
Class1=CMakeaviApp
Class2=CMakeaviDlg
Class3=CAboutDlg

ResourceCount=4
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_MAKEAVI_DIALOG
Class4=FeedbackDlg
Resource4=IDD_FEEDBACK

[CLS:CMakeaviApp]
Type=0
HeaderFile=makeavi.h
ImplementationFile=makeavi.cpp
Filter=N

[CLS:CMakeaviDlg]
Type=0
HeaderFile=makeaviDlg.h
ImplementationFile=makeaviDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_FILE_LIST

[CLS:CAboutDlg]
Type=0
HeaderFile=makeaviDlg.h
ImplementationFile=makeaviDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_MAKEAVI_DIALOG]
Type=1
Class=CMakeaviDlg
ControlCount=19
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_PICK_FILES,button,1342242816
Control4=IDC_FILEUP,button,1342242816
Control5=IDC_FILEDOWN,button,1342242816
Control6=IDC_STATIC,button,1342177287
Control7=IDC_FILE_LIST,SysListView32,1350647821
Control8=IDC_SORT_FILES,button,1342242816
Control9=IDC_FRAMERATE,edit,1350631552
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,button,1342177287
Control12=IDC_STATIC,button,1342177287
Control13=IDC_ENABLE_CROPPING,button,1342242819
Control14=IDC_XRES,edit,1350631552
Control15=IDC_YRES,edit,1350631552
Control16=IDC_STATIC,static,1342308352
Control17=IDC_STATIC,static,1342308352
Control18=IDC_PREVIEW_AREA,static,1342308352
Control19=IDC_DELETE,button,1342242816

[DLG:IDD_FEEDBACK]
Type=1
Class=FeedbackDlg
ControlCount=3
Control1=IDCANCEL,button,1342242816
Control2=IDC_FEEDBACK_TEXT,static,1342308352
Control3=IDC_PROGRESS1,msctls_progress32,1350565888

[CLS:FeedbackDlg]
Type=0
HeaderFile=FeedbackDlg.h
ImplementationFile=FeedbackDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDCANCEL

