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

#include "stdafx.h"
#include "DitherStudio.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "DitherStudioDoc.h"
#include "DitherStudioView.h"

#include <list>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDitherStudioApp

BEGIN_MESSAGE_MAP(CDitherStudioApp, CWinApp)
	//{{AFX_MSG_MAP(CDitherStudioApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDitherStudioApp construction

CDitherStudioApp::CDitherStudioApp()
{
	m_hAccelTable = NULL;
	m_hMenuShared = NULL;
	m_pDocTemplate = NULL;
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDitherStudioApp object

CDitherStudioApp theApp;

/////////////////////////////////////////////////////////////////////////////

//
//	CBpaintApp::SharedResourceAddDocTemplate()
//

void CDitherStudioApp::SharedResourceAddDocTemplate( CMultiDocTemplate * pTemplate )
{

	// Replace the menu with the shared one.
	// ------------------------------------------------------------------------

	if ( m_hMenuShared ) {

		HMENU hOldMenu = pTemplate->m_hMenuShared;

		pTemplate->m_hMenuShared = m_hMenuShared;

		DestroyMenu( hOldMenu );

	}

	// Replace the 'accelerator' with the shared one
	// ------------------------------------------------------------------------

	if ( m_hAccelTable ) {

		HACCEL hOldAccelTable = pTemplate->m_hAccelTable;

		pTemplate->m_hAccelTable = m_hAccelTable;

		DestroyAcceleratorTable( hOldAccelTable );

	}

	// ------------------------------------------------------------------------

	AddDocTemplate( pTemplate );

}

/////////////////////////////////////////////////////////////////////////////
// CDitherStudioApp initialization

BOOL CDitherStudioApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	//Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("www.thinkbpt.com"));

	LoadStdProfileSettings(16);  // Load standard INI file options (including MRU)

	// ========================================================================
	// Setup the primary document template (owns the menus/accel...)
	// ========================================================================

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	m_pDocTemplate = new CMultiDocTemplate(
		IDR_DITHERTYPE,
		RUNTIME_CLASS(CDitherStudioDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CDitherStudioView));
	AddDocTemplate(m_pDocTemplate);

	// ========================================================================

	m_hMenuShared = m_pDocTemplate->m_hMenuShared;
	m_hAccelTable = m_pDocTemplate->m_hAccelTable;

	// Futher document templates
	// ========================================================================

	// ========================================================================

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	m_pMainWnd->DragAcceptFiles(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Load in the size and position of this frame window
	// ------------------------------------------------------------------------

	RestoreFrameWindowSettings( pMainFrame, "MainFrameWnd" );

	m_nCmdShow = SW_SHOW;

	// The main window has been initialized, so show and update it.

	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	// ------------------------------------------------------------------------

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CDitherStudioApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CDitherStudioApp message handlers

// ----------------------------------------------------------------------------

bool CDitherStudioApp::PrepareForNewDocument( CDocument * pExcludeDocmentFromClose ) {

	if ( pExcludeDocmentFromClose ) {

		typedef std::list< CDocument * > close_collection_type;

		close_collection_type closeDocumentsList;

		// Build the list of documents to close
		// -------------------------------------------------------------------

		POSITION pos = GetFirstDocTemplatePosition();

		while ( NULL != pos ) {

			CDocTemplate * pDocTemplate = GetNextDocTemplate( pos );

			if ( pDocTemplate ) {

				POSITION docPos = pDocTemplate->GetFirstDocPosition();

				while ( NULL != docPos ) {

					CDocument * pDoc = pDocTemplate->GetNextDoc( docPos );

					if ( pDoc && (pDoc != pExcludeDocmentFromClose) ) {

						closeDocumentsList.push_back( pDoc );

					}

				}

			}
			
		}

		// Okay now iterate through the documents to close collection
		// ----------------------------------------------------------------

		close_collection_type::iterator it;

		for ( it = closeDocumentsList.begin(); it != closeDocumentsList.end(); it++ ) {

			if ( (*it)->SaveModified() ) {

				(*it)->OnCloseDocument();

			} else {

				return false;

			}

		}

	} else {

		if ( SaveAllModified() ) {

			TRACE( "Closing all documents()\n" );

			CloseAllDocuments( FALSE );

		} else {

			TRACE( "Save all modifed returned FALSE\n" );

		}

	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////

void
CDitherStudioApp::StoreFrameWindowSettings( CFrameWnd * pWnd, const char * name )
{
	// Retrieve the current settings
	// ------------------------------------------------------------------------

	WINDOWPLACEMENT wp;
	wp.length = sizeof(WINDOWPLACEMENT);
	pWnd->GetWindowPlacement( &wp );

	// Store the settings under the 'Settings name' section
	// ------------------------------------------------------------------------

	CString sectionString;
	sectionString.Format( "Settings %s", name );

	// Store off the useful parts of the WINDOWPLACEMENT structure.
	// ------------------------------------------------------------------------

	WriteProfileInt( sectionString, _T("flags"), wp.flags );
	WriteProfileInt( sectionString, _T("showCmd"), wp.showCmd );
	WriteProfileInt( sectionString, _T("left"), wp.rcNormalPosition.left );
	WriteProfileInt( sectionString, _T("top"), wp.rcNormalPosition.top );
	WriteProfileInt( sectionString, _T("right"), wp.rcNormalPosition.right );
	WriteProfileInt( sectionString, _T("bottom"), wp.rcNormalPosition.bottom );

	// Ask to save off the toolbar settings for this frame.
	// ------------------------------------------------------------------------

	pWnd->SaveBarState( sectionString );

}

void 
CDitherStudioApp::RestoreFrameWindowSettings( CFrameWnd * pWnd, const char * name )
{
	// Retrieve the current settings
	// ------------------------------------------------------------------------

	WINDOWPLACEMENT wp;
	wp.length = sizeof(WINDOWPLACEMENT);
	pWnd->GetWindowPlacement( &wp );

	// Read the settings from the 'Settings name' section
	// ------------------------------------------------------------------------

	CString sectionString;
	sectionString.Format( "Settings %s", name );

	// Read the values using the current settings as the default.
	// ------------------------------------------------------------------------

	wp.flags = GetProfileInt( sectionString, _T("flags"), wp.flags );
	wp.showCmd = GetProfileInt( sectionString, _T("showCmd"), wp.showCmd );
	wp.rcNormalPosition.left = GetProfileInt( sectionString, _T("left"), wp.rcNormalPosition.left );
	wp.rcNormalPosition.top = GetProfileInt( sectionString, _T("top"), wp.rcNormalPosition.top );
	wp.rcNormalPosition.right = GetProfileInt( sectionString, _T("right"), wp.rcNormalPosition.right );
	wp.rcNormalPosition.bottom = GetProfileInt( sectionString, _T("bottom"), wp.rcNormalPosition.bottom );

	// Update the window placement.
	// ------------------------------------------------------------------------

	pWnd->SetWindowPlacement( &wp );

	// Ask to save off the toolbar settings for this frame.
	// ------------------------------------------------------------------------

	pWnd->LoadBarState( sectionString );

}

/////////////////////////////////////////////////////////////////////////////

//
//	GLOBAL_GetSettingInt()
//

int
GLOBAL_GetSettingInt( const char * pEntry, const int defaultValue, const char * pSection )
{
	// Get the section to use
	// ------------------------------------------------------------------------

	CString section;

	if ( pSection ) {

		section = pSection;

	} else {

		section = "General Settings";

	}

	CString entry = pEntry;

	// Use the global 
	// ------------------------------------------------------------------------

	CWinApp * pApp = AfxGetApp();

	if ( pApp ) {

		return pApp->GetProfileInt( section, entry, defaultValue );

	}

	return defaultValue;
}

//
//	GLOBAL_PutSettingInt()
//

void
GLOBAL_PutSettingInt( const char * pEntry, const int value, const char * pSection )
{
	// Get the section to use
	// ------------------------------------------------------------------------

	CString section;

	if ( pSection ) {

		section = pSection;

	} else {

		section = "General Settings";

	}

	CString entry = pEntry;

	// Use the global 
	// ------------------------------------------------------------------------

	CWinApp * pApp = AfxGetApp();

	if ( pApp ) {

		pApp->WriteProfileInt( section, entry, value );

	}
	
}

//
//	GLOBAL_GetSettingIntHelper()
//

int 
GLOBAL_GetSettingIntHelper( const char * pBase, const char * pPostfix, const int value )
{
	if ( pBase ) {

		CString entry;

		if ( pPostfix ) {

			entry.Format( "%s.%s", pBase, pPostfix );

		} else {

			entry = pBase;

		}

		return GLOBAL_GetSettingInt( entry, value );

	}

	return value;
}

//
//	GLOBAL_GetSettingIntHelper()
//

void 
GLOBAL_PutSettingIntHelper( const char * pBase, const char * pPostfix, const int value )
{
	if ( pBase ) {

		CString entry;

		if ( pPostfix ) {

			entry.Format( "%s.%s", pBase, pPostfix );

		} else {

			entry = pBase;

		}

		GLOBAL_PutSettingInt( entry, value );

	}
}

// ----------------------------------------------------------------------------

//
//	GLOBAL_CreateNewWindow()
//

CFrameWnd *
GLOBAL_CreateNewWindow(
	CDocTemplate * pTemplate, CDocument * pDocument, const char * pSettingsName
)
{
	ASSERT_VALID( pTemplate );
	ASSERT_VALID( pDocument );

	CFrameWnd * pFrame = pTemplate->CreateNewFrame( pDocument, NULL );

	if ( 0 == pFrame ) {

		TRACE0( "Warning: failed to create new frame.\n" );
		return 0;

	}

	ASSERT_KINDOF( CFrameWnd, pFrame );

	// Read the window positions

	if ( pSettingsName ) {

		CDitherStudioApp * pApp = static_cast<CDitherStudioApp *>( AfxGetApp() );
		
		if ( pApp ) {

			pApp->RestoreFrameWindowSettings( pFrame, pSettingsName );

		}

	}

	// Okay ask the template to perform it's initialization of the frame window.

	pTemplate->InitialUpdateFrame( pFrame, pDocument );

	return pFrame;
}


BOOL CDitherStudioApp::OnIdle(LONG lCount) 
{
	// TODO: Add your specialized code here and/or call the base class

	Sleep(5); // give back to the system

	return CWinApp::OnIdle(lCount);
}
