; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDitherStudioDoc
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "DitherStudio.h"
LastPage=0

ClassCount=6
Class1=CDitherStudioApp
Class2=CDitherStudioDoc
Class3=CDitherStudioView
Class4=CMainFrame

ResourceCount=8
Resource1=IDR_VIEW_TOOLBAR
Resource2=IDD_ABOUTBOX
Resource3=IDR_MENU1
Class5=CChildFrame
Class6=CAboutDlg
Resource4=IDR_DITHERTYPE
Resource5=IDR_MAINFRAME
Resource6=IDR_ZOOM_TOOLBAR
Resource7=IDR_CUSTOM_LAYOUT
Resource8=IDD_COPYRIGHT

[CLS:CDitherStudioApp]
Type=0
HeaderFile=DitherStudio.h
ImplementationFile=DitherStudio.cpp
Filter=N
BaseClass=CWinApp
VirtualFilter=AC
LastObject=CDitherStudioApp

[CLS:CDitherStudioDoc]
Type=0
HeaderFile=DitherStudioDoc.h
ImplementationFile=DitherStudioDoc.cpp
Filter=N
BaseClass=CDocument
VirtualFilter=DC
LastObject=ID_DEFAULT_LAYOUT_2

[CLS:CDitherStudioView]
Type=0
HeaderFile=DitherStudioView.h
ImplementationFile=DitherStudioView.cpp
Filter=C
BaseClass=CScrollView
VirtualFilter=VWC
LastObject=CDitherStudioView


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
BaseClass=CMDIFrameWnd
VirtualFilter=fWC
LastObject=CMainFrame


[CLS:CChildFrame]
Type=0
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp
Filter=M


[CLS:CAboutDlg]
Type=0
HeaderFile=DitherStudio.cpp
ImplementationFile=DitherStudio.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_APP_EXIT
Command3=ID_VIEW_TOOLBAR
Command4=ID_VIEW_STATUS_BAR
Command5=ID_APP_ABOUT
CommandCount=5

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_LOAD_IMAGE
Command2=ID_EDIT_PASTE
Command3=ID_MATCH_DEPTH
Command4=ID_R5G5B5
Command5=ID_R5G6B5
Command6=ID_R4G4B4
Command7=ID_USE_NOISE
Command8=ID_NOISE_LEVEL_1
Command9=ID_NOISE_LEVEL_2
Command10=ID_NOISE_LEVEL_3
Command11=ID_NOISE_LEVEL_4
Command12=ID_USE_RESET
Command13=ID_RESET_LEVEL_1
Command14=ID_RESET_LEVEL_2
Command15=ID_RESET_LEVEL_3
Command16=ID_RESET_LEVEL_4
Command17=ID_VERTICAL_MODE
Command18=ID_SERPENTINE_MODE
Command19=ID_FORCE_LEFT
Command20=ID_STYLE_FLOYD_STEINBERG
Command21=ID_STYLE_JARVIS
Command22=ID_STYLE_STUCKI
Command23=ID_STYLE_STEVENSON
Command24=ID_STYLE_BURKES
Command25=ID_STYLE_SIERRA
Command26=ID_STYLE_OTHER_1
Command27=ID_AUTO_FOCUS
Command28=ID_APP_ABOUT
CommandCount=28

[MNU:IDR_DITHERTYPE]
Type=1
Class=CDitherStudioView
Command1=ID_LOAD_IMAGE
Command2=ID_APP_EXIT
Command3=ID_VIEW_TOOLBAR
Command4=ID_VIEW_STATUS_BAR
Command5=ID_2_VSET_SWITCH_MODE
Command6=ID_VCMD_CENTER_ON_CURSOR
Command7=ID_VCMD_CURSOR_ZOOM
Command8=ID_VCMD_REQUEST_OTHERS_CENTER_ON_CURSOR
Command9=ID_VIEW_EXPAND
Command10=ID_AUTO_FOCUS
Command11=ID_VSET_SHOW_FATBITS
Command12=ID_VSET_IMEDIATE_UPDATES
Command13=ID_VSET_CENTER_SYNC_SCROLL
Command14=ID_VSET_DISABLE_AUTO_SCROLL
Command15=ID_VSET_SHOW_GRID
Command16=ID_VFRAME_SHOW_ZOOM_TOOLBAR
Command17=ID_VFRAME_SHY_ZOOM_TOOLBAR
Command18=ID_VZOOM_1X
Command19=ID_VZOOM_2X
Command20=ID_VZOOM_3X
Command21=ID_VZOOM_4X
Command22=ID_VZOOM_5X
Command23=ID_VZOOM_6X
Command24=ID_VZOOM_7X
Command25=ID_VZOOM_8X
Command26=ID_VZOOM_9X
Command27=ID_VZOOM_10X
Command28=ID_VZOOM_DEC
Command29=ID_VZOOM_INC
Command30=ID_DEFAULT_LAYOUT_0
Command31=ID_DEFAULT_LAYOUT_1
Command32=ID_DEFAULT_LAYOUT_2
Command33=ID_CUSTOM_LAYOUT_0
Command34=ID_APP_ABOUT
Command35=ID_WINDOW_NEW
Command36=ID_WINDOW_CASCADE
Command37=ID_WINDOW_TILE_HORZ
Command38=ID_WINDOW_ARRANGE
CommandCount=38

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_VZOOM_DEC
Command2=ID_VZOOM_INC
Command3=ID_AUTO_FOCUS
Command4=ID_VSET_SHOW_FATBITS
Command5=ID_MATCH_DEPTH
Command6=ID_VIEW_EXPAND
Command7=ID_VSET_SWITCH_MODE
Command8=ID_LOAD_IMAGE
Command9=ID_VCMD_CURSOR_ZOOM
Command10=ID_VCMD_CENTER_ON_CURSOR
CommandCount=10

[TB:IDR_ZOOM_TOOLBAR]
Type=1
Class=?
Command1=ID_VZOOM_1X
Command2=ID_VZOOM_2X
Command3=ID_VZOOM_3X
Command4=ID_VZOOM_4X
Command5=ID_VZOOM_5X
Command6=ID_VZOOM_6X
Command7=ID_VZOOM_7X
Command8=ID_VZOOM_8X
Command9=ID_VZOOM_9X
Command10=ID_VZOOM_10X
Command11=ID_VZOOM_INC
Command12=ID_VZOOM_DEC
Command13=ID_VZOOM_VISUAL
CommandCount=13

[TB:IDR_CUSTOM_LAYOUT]
Type=1
Class=?
Command1=ID_BUTTON32801
CommandCount=1

[MNU:IDR_MENU1]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_MRU_FILE1
Command7=ID_APP_EXIT
CommandCount=7

[DLG:IDD_COPYRIGHT]
Type=1
Class=?
ControlCount=1
Control1=IDOK,button,1342242817

[TB:IDR_VIEW_TOOLBAR]
Type=1
Class=?
Command1=ID_SHOW_DITHERED_IMAGE
Command2=ID_VZOOM_INC
Command3=ID_VZOOM_DEC
CommandCount=3

