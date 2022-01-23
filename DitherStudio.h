// ----------------------------------------------------------------------------
/*
The MIT License (MIT)

Copyright (c) 2000- Brad P. Taylor

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
// ----------------------------------------------------------------------------


#if !defined(AFX_DITHERSTUDIO_H__D733C8E0_12E1_48B3_9615_2A25A304CB47__INCLUDED_)
#define AFX_DITHERSTUDIO_H__D733C8E0_12E1_48B3_9615_2A25A304CB47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////

#define MACRO_BEGIN_DOCTEMP_ENUM_TABLE() enum DocTemplates { UNKNOWN = 0x12340000
#define MACRO_END_DOCTEMP_ENUM_TABLE() };
#define MACRO_ADD_DOCTEMPLATE( TEMPLATE ) ,ID_##TEMPLATE

/////////////////////////////////////////////////////////////////////////////
// CDitherStudioApp:
// See DitherStudio.cpp for the implementation of this class
//

class CDitherStudioApp : public CWinApp
{

private:

	HMENU m_hMenuShared;
	HACCEL m_hAccelTable;

	CMultiDocTemplate * m_pDocTemplate;

private:

	void SharedResourceAddDocTemplate( CMultiDocTemplate * pTemplate );

public:

	CDitherStudioApp();

	// ------------------------------------------------------------------------

	MACRO_BEGIN_DOCTEMP_ENUM_TABLE()
	MACRO_ADD_DOCTEMPLATE( m_pDocTemplate )
	MACRO_END_DOCTEMP_ENUM_TABLE()

	// ------------------------------------------------------------------------

	CDocTemplate * GetDefaultDocTemplate();

	int GetDocTemplateStorageID( CDocTemplate * pDocTemplate );

	CDocTemplate * GetDocTemplateFromStorageID( const int storageID );

	// ------------------------------------------------------------------------

	void StoreFrameWindowSettings( CFrameWnd * pWnd, const char * name );
	void RestoreFrameWindowSettings( CFrameWnd * pWnd, const char * name );
	bool PrepareForNewDocument( CDocument * pExcludeDocmentFromClose = 0 );
	void StoreCurrentLayoutAs( CDocument * pDoc, const char * name );
	bool LoadCustomLayoutFrom( CDocument * pDoc, const char * name );

	// ------------------------------------------------------------------------
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDitherStudioApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CDitherStudioApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

// Global functions
// ----------------------------------------------------------------------------

int GLOBAL_GetSettingInt( const char * pEntry, const int defaultValue = 0, const char * pSection = 0 );
void GLOBAL_PutSettingInt( const char * pEntry, const int value = 0, const char * pSection = 0 );
int GLOBAL_GetSettingIntHelper( const char * pBase, const char * pPostfix, const int value );
void GLOBAL_PutSettingIntHelper( const char * pBase, const char * pPostfix, const int value );

CFrameWnd *
GLOBAL_CreateNewWindow(
	CDocTemplate * pTemplate, CDocument * pDocument, const char * pSettingsName = 0
);

#endif // !defined(AFX_DITHERSTUDIO_H__D733C8E0_12E1_48B3_9615_2A25A304CB47__INCLUDED_)
