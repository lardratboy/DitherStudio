# Microsoft Developer Studio Project File - Name="DitherStudio" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=DitherStudio - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DitherStudio.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DitherStudio.mak" CFG="DitherStudio - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DitherStudio - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "DitherStudio - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "DitherStudio - Win32 DLL Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DitherStudio - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Build\"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I ".\BPaintStuff\\" /I "D:\Testbed\DitherBuddyDemo" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 winmm.lib D:\Testbed\DitherBuddyDemo\Release\DitherBuddyDemo.lib /nologo /subsystem:windows /machine:I386 /out:".\Build\DitherStudios.exe"

!ELSEIF  "$(CFG)" == "DitherStudio - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Build\"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I ".\BPaintStuff\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:".\Build\DitherStudiod.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "DitherStudio - Win32 DLL Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "DitherStudio___Win32_DLL_Release"
# PROP BASE Intermediate_Dir "DitherStudio___Win32_DLL_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "DLL_Release"
# PROP Intermediate_Dir "DLL_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I ".\BPaintStuff\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I ".\BPaintStuff\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:".\Build\DitherStudio.exe"

!ENDIF 

# Begin Target

# Name "DitherStudio - Win32 Release"
# Name "DitherStudio - Win32 Debug"
# Name "DitherStudio - Win32 DLL Release"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Group "Toolbar Bitmaps"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\zoom_tb.bmp
# End Source File
# End Group
# Begin Group "Icons"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\res\DitherStudio.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\custom_l.bmp
# End Source File
# Begin Source File

SOURCE=.\DitherStudio.rc
# End Source File
# Begin Source File

SOURCE=.\res\DitherStudio.rc2
# End Source File
# Begin Source File

SOURCE=.\res\DitherStudioDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\move_cur.cur
# End Source File
# Begin Source File

SOURCE=.\others.cur
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\res\view_too.bmp
# End Source File
# End Group
# Begin Group "BPaint based code"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BpaintStuff\BPTBitmap.h
# End Source File
# Begin Source File

SOURCE=.\BpaintStuff\BPTBlitter.h
# End Source File
# Begin Source File

SOURCE=.\BpaintStuff\BPTDib.h
# End Source File
# Begin Source File

SOURCE=.\BpaintStuff\BPTDrawingAlgorithms.h
# End Source File
# Begin Source File

SOURCE=.\BpaintStuff\BPTPrimitives.h
# End Source File
# Begin Source File

SOURCE=.\BpaintStuff\BPTRasterOps.h
# End Source File
# Begin Source File

SOURCE=.\BpaintStuff\BPTUtility.h
# End Source File
# End Group
# Begin Group "Views"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DitherStudioView.cpp
# End Source File
# Begin Source File

SOURCE=.\DitherStudioView.h
# End Source File
# End Group
# Begin Group "Frame Windows"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# End Group
# Begin Group "Doc & App"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DitherStudio.cpp
# End Source File
# Begin Source File

SOURCE=.\DitherStudio.h
# End Source File
# Begin Source File

SOURCE=.\DitherStudioDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\DitherStudioDoc.h
# End Source File
# Begin Source File

SOURCE=.\UILayoutCode.cpp
# End Source File
# End Group
# Begin Group "Misc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "External Source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ExternalSource\IJLBitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\ExternalSource\IJLBitmap.h
# End Source File
# End Group
# Begin Group "Dither Logic"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SimpleErrorDiffusion.cpp
# End Source File
# Begin Source File

SOURCE=.\SimpleErrorDiffusion.h
# End Source File
# End Group
# End Target
# End Project
