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
#include "DitherStudioDoc.h"

// ----------------------------------------------------------------------------

#include <list>
#include <map>

// ----------------------------------------------------------------------------

#include "DitherStudioView.h"
#include "ChildFrm.h"

// ----------------------------------------------------------------------------

//
//	TDocActiveFramesAndViewsInfo<>
//

template< class DOC >
class TDocActiveFramesAndViewsInfo
{

public: // traits

	typedef std::list< CView * > view_collection;
	typedef std::map< CFrameWnd *, view_collection * > output_map_type;

public: // interface

	static void Destroy( output_map_type * pMap ) {

		if ( pMap ) {

			for ( output_map_type::iterator it = pMap->begin(); it != pMap->end(); ) {

				delete (*it).second;

				it = pMap->erase( it );

			}

			delete pMap;

		}

	}

	static output_map_type * Generate( DOC * pDocument ) {

		// Create the map to hold the info
		// --------------------------------------------------------------------

		output_map_type * pMap = new output_map_type;

		if ( !pMap ) {

			return 0;

		}

		// --------------------------------------------------------------------

		POSITION pos = pDocument->GetFirstViewPosition();

		while (pos != NULL) {

			CView * pView = pDocument->GetNextView( pos );

			if ( pView ) {

				CFrameWnd * pFrame = pView->GetParentFrame();

				if ( pFrame ) {

					output_map_type::iterator it = pMap->find( pFrame );

					if ( it != pMap->end() ) {

						(*it).second->push_back( pView );

					} else {

						if ( (*pMap)[ pFrame ] = new view_collection() ) {

							(*pMap)[ pFrame ]->push_back( pView );

						}

					}

				}

			}

		}

		// --------------------------------------------------------------------

		return pMap;

	}

};

// ----------------------------------------------------------------------------

//
//	CDitherStudioApp::GetDocTemplateStorageID()
//

int 
CDitherStudioApp::GetDocTemplateStorageID( CDocTemplate * pDocTemplate )
{
	return 1; // HACK
}

//
//	CDitherStudioApp::GetDocTemplateFromStorageID()
//

CDocTemplate * 
CDitherStudioApp::GetDocTemplateFromStorageID( const int storageID )
{
	return m_pDocTemplate; // HACK
}

// ----------------------------------------------------------------------------

//
//	CDitherStudioApp::GetDefaultDocTemplate()
//

CDocTemplate * 
CDitherStudioApp::GetDefaultDocTemplate()
{
	return m_pDocTemplate;
}

// ----------------------------------------------------------------------------

//
//	MACRO_Store_Frame_Helper()
//

#define MACRO_Store_Frame_Helper( DOCTEMP, CHECK_VAR, THE_TYPE, PARAM_1, PARAM_2 ) \
if ( CHECK_VAR->IsKindOf( RUNTIME_CLASS( THE_TYPE ) ) ) { \
	THE_TYPE * pType = (THE_TYPE *)CHECK_VAR; \
	pType->RestoreInternalSettings( PARAM_1, PARAM_2 ); \
	if ( DOCTEMP ) DOCTEMP = pType->GetDocTemplate(); \
}

//
//	MACRO_Store_View_Helper()
//

#define MACRO_Store_View_Helper( DIDBOOL, CHECK_VAR, THE_TYPE, PARAM_1, PARAM_2 ) \
if ( CHECK_VAR->IsKindOf( RUNTIME_CLASS( THE_TYPE ) ) ) { \
	THE_TYPE * pType = (THE_TYPE *)CHECK_VAR; \
	pType->RestoreInternalSettings( PARAM_1, PARAM_2 ); \
	DIDBOOL = true; \
}

// ----------------------------------------------------------------------------

//
//	CDitherStudioApp::StoreCurrentLayoutAs()
//

void CDitherStudioApp::StoreCurrentLayoutAs( CDocument * pDoc, const char * name )
{

	// Generate a list of all the current frames/views
	// ------------------------------------------------------------------------

	typedef TDocActiveFramesAndViewsInfo<CDocument> info_helper;

	info_helper::output_map_type * pMap = info_helper::Generate( pDoc );

	if ( pMap ) {

		info_helper::output_map_type::iterator it = pMap->begin();

		int frameCounter = 0;
	
		while ( it != pMap->end() ) {

			CFrameWnd * pFrameWnd = it->first;

			// Generate the section name
			// ------------------------------------------------------------

			CString sectionName;

			sectionName.Format( "%s.frame%d", name, frameCounter );

			++frameCounter;

			// Store off the frame position
			// ------------------------------------------------------------

			StoreFrameWindowSettings( pFrameWnd, sectionName );

			// Figure out the type of Frame and get the document template used 
			// to create the frame & views. (All frame window types!!!)
			// ----------------------------------------------------------------

			CDocTemplate * pDocTemplate = 0;

			// ----------------------------------------------------------------

			MACRO_Store_Frame_Helper( pDocTemplate, pFrameWnd, CChildFrame, sectionName, "Internal" );

			// More types as needed...

			// ----------------------------------------------------------------

			// For any view types that have settings save them
			// ----------------------------------------------------------------

			info_helper::view_collection * pViewCollection = it->second;

			if ( pViewCollection ) {

				info_helper::view_collection::iterator vit = pViewCollection->begin();

				int viewNumber = 0;

				while ( vit != pViewCollection->end() ) {

					CString baseEntry;

					baseEntry.Format( "View%d", viewNumber );

					bool did = false;

					// --------------------------------------------------------

					MACRO_Store_View_Helper( did, (*vit), CDitherStudioView, sectionName, baseEntry );

					// More types as needed...

					// --------------------------------------------------------

					if ( did ) {

						++viewNumber;

					}

					++vit;

				}

			}

			// If we were able to determine the document type the store off
			// the data into the layout section.
			// ----------------------------------------------------------------

			if ( pDocTemplate ) {

				// Store off the document template used
				// ------------------------------------------------------------

				sectionName += "DocTempID";

				GLOBAL_PutSettingInt( 
					sectionName, 
					GetDocTemplateStorageID(pDocTemplate), 
					name
				);

			}

			// ----------------------------------------------------------------

			++it;

		}

		// Okay write out the number of things saved.
		// --------------------------------------------------------------------

		GLOBAL_PutSettingInt( "FrameWindowCount", frameCounter, name );

		info_helper::Destroy( pMap );

	}

}

// ----------------------------------------------------------------------------

//
//	MACRO_Restore_X_Helper()
//

#define MACRO_Restore_X_Helper( DIDBOOL, CHECK_VAR, THE_TYPE, PARAM_1, PARAM_2 ) \
if ( CHECK_VAR->IsKindOf( RUNTIME_CLASS( THE_TYPE ) ) ) { \
	THE_TYPE * pType = (THE_TYPE *)CHECK_VAR; \
	pType->RestoreInternalSettings( PARAM_1, PARAM_2 ); \
	DIDBOOL = true; \
}

// ----------------------------------------------------------------------------

//
//	CDitherStudioApp::LoadCustomLayoutFrom()
//

bool CDitherStudioApp::LoadCustomLayoutFrom( CDocument * pDoc, const char * name )
{

	// Make sure we have stored something before loading...
	// ------------------------------------------------------------------------

	int frameWindowCount = GLOBAL_GetSettingInt( "FrameWindowCount", 0, name );

	if ( !frameWindowCount ) {

		return false;

	}

	// Make sure that the document doesn't get destroyed while
	// all of it's current views/frames are destroyed.
	// ------------------------------------------------------------------------

	BOOL bSaveAutoDelete = m_bAutoDelete;

	m_bAutoDelete = FALSE;

	// Destroy the current frames/views
	// ------------------------------------------------------------------------

	typedef TDocActiveFramesAndViewsInfo<CDocument> info_helper;

	info_helper::output_map_type * pMap = info_helper::Generate( pDoc );

	if ( pMap ) {

		info_helper::output_map_type::iterator it = pMap->begin();
	
		while ( it != pMap->end() ) {
	
			(it->first)->DestroyWindow();
	
			++it;

		}

	}

	// Okay let's get to creating the frame windows
	// ------------------------------------------------------------------------

	bool bError = false;

	int createdCount = 0;

	typedef std::pair< CFrameWnd *, CString > frame_window_entry;

	typedef std::list< frame_window_entry > frame_window_collection;

	frame_window_collection frameWindowsCollection;

	for ( int frameCounter = 0; frameCounter < frameWindowCount; frameCounter++ ) {
	
		// Generate the section name
		// --------------------------------------------------------------------

		CString sectionName;

		sectionName.Format( "%s.frame%d", name, frameCounter );

		// Store off the document template used
		// ------------------------------------------------------------

		CString docTypeEntry = sectionName + "DocTempID";

		int docType = GLOBAL_GetSettingInt( docTypeEntry, 0, name );

		CDocTemplate * pDocTemplate = GetDocTemplateFromStorageID( docType );

		if ( pDocTemplate ) {

			CFrameWnd * pFrameWnd = GLOBAL_CreateNewWindow( pDocTemplate, pDoc );

			if ( !pFrameWnd ) {

				bError = true;

			} else {

				++createdCount;

				frameWindowsCollection.push_back(
					frame_window_entry( pFrameWnd, sectionName )
				);

			}

		} else {

			bError = true;

		}

	}

	// Destroy our 1st helper 'map'
	// ------------------------------------------------------------------------

	if ( pMap ) {

		info_helper::Destroy( pMap );

	}

	// Create another helper 'map' to restore view settings
	// ------------------------------------------------------------------------

	pMap = info_helper::Generate( pDoc );

	// Restore the 'view' saved internal settings
	// ------------------------------------------------------------------------

	frame_window_collection::iterator it = frameWindowsCollection.begin();

	while ( it != frameWindowsCollection.end() ) {

		CFrameWnd * pFrameWnd = it->first;

		// restore the frame position
		// --------------------------------------------------------------------

		RestoreFrameWindowSettings( pFrameWnd, it->second );

		// Restore the frame internal settings
		// --------------------------------------------------------------------

		bool did = false;

		// --------------------------------------------------------------------

		MACRO_Restore_X_Helper( did, pFrameWnd, CChildFrame, it->second, "Internal" );

		// More types as needed...

		// --------------------------------------------------------------------

		// If we can view info then try to restore the info for them.
		// --------------------------------------------------------------------

		if ( pMap ) {

			info_helper::output_map_type::iterator mapIT = pMap->find( it->first );

			if ( mapIT != pMap->end() ) {

				info_helper::view_collection * pViewCollection = mapIT->second;
	
				if ( pViewCollection ) {
	
					info_helper::view_collection::iterator vit = pViewCollection->begin();
	
					int viewNumber = 0;
	
					while ( vit != pViewCollection->end() ) {
	
						CString baseEntry;
	
						baseEntry.Format( "View%d", viewNumber );

						bool did = false;

						// ----------------------------------------------------

						MACRO_Restore_X_Helper( did, (*vit), CDitherStudioView, it->second, baseEntry );

						// More types as needed...

						// ----------------------------------------------------

						if ( did ) {

							++viewNumber;

						}

						++vit;
	
					}
	
				}

			}

		}

		++it;

	}

	// Destroy our 2nd helper 'map'
	// ------------------------------------------------------------------------

	if ( pMap ) {

		info_helper::Destroy( pMap );

	}

	// Restore the auto delete setting and we're done!
	// ------------------------------------------------------------------------

	m_bAutoDelete = bSaveAutoDelete;

	// If there was an error put up a message box etc...
	// ------------------------------------------------------------------------

	if ( bError ) {

		AfxGetMainWnd()->MessageBox( 
			"A frame window must have failed construction?", "Error!",
			MB_ICONERROR | MB_OK 
		);

		return false;

	}

	return true;

}

// ============================================================================
// DOCUMENT RELATED LAYOUT CODE
// ============================================================================

//
//	CDitherStudioDoc::CreateWindowsForLayout_0()
//

bool CDitherStudioDoc::CreateWindowsForLayout_0()
{
	 CDitherStudioApp * pApp = (CDitherStudioApp *)AfxGetApp();

	 if ( !pApp ) {

		 return false;

	 }

	// ------------------------------------------------------------------------

	CFrameWnd * pFrameA = 0;
	CFrameWnd * pFrameB = 0;
	CFrameWnd * pFrameC = 0;

	// Create the edit views
	// ------------------------------------------------------------------------

	CRect rMDIClientRect;

	pFrameA = GLOBAL_CreateNewWindow(
		pApp->GetDefaultDocTemplate(), this
	);

	int width = 0;

	int height = 0;
	
	if ( pFrameA ) {
	
		// Figure out the first pos...
	
		CWnd * pWnd = pFrameA->GetParent();
	
		if ( pWnd ) {
	
			pWnd->GetClientRect( &rMDIClientRect );
	
		}

		width = rMDIClientRect.Width() / 2;
	
		// position
	
		pFrameA->MoveWindow(
			CRect( CPoint( width, 0 ), 
				CSize( rMDIClientRect.Width() - width, rMDIClientRect.Height() )
			)
		);
	
		// need to send the zoom request.
	
		pFrameA->SendMessage( WM_COMMAND, ID_VZOOM_3X );
	
	} else {

		// NEED TO HANDLE THE ERROR!!!
	
		goto HANDLE_ERROR;

	}
	
	// ----------------------------------------------------------------

	pFrameB = GLOBAL_CreateNewWindow(
		pApp->GetDefaultDocTemplate(), this
	);
	
	height = rMDIClientRect.Height() / 2;

	if ( pFrameB ) {
	
		// position
	
		pFrameB->MoveWindow(
			CRect( CPoint( 0, 0 ), 
				CSize( width, height )
			)
		);
	
	} else {
	
		// NEED TO HANDLE THE ERROR!!!
	
		goto HANDLE_ERROR;

	}
	
	// Create the grid view
	// ----------------------------------------------------------------
	
	pFrameC = GLOBAL_CreateNewWindow(
		pApp->GetDefaultDocTemplate(), this
	);
	
	if ( pFrameC ) {
	
		// position
	
		pFrameC->MoveWindow(
			CRect( CPoint( 0, height ), 
				CSize( width, rMDIClientRect.Height() - height )
			)
		);
	
		// need to send the zoom request.
	
		pFrameC->SendMessage( WM_COMMAND, ID_VZOOM_2X );

	} else {
	
		// NEED TO HANDLE THE ERROR!!!
	
		goto HANDLE_ERROR;

	}

	// Center the view (probably should call center others... :)
	// ----------------------------------------------------------------

	pFrameB->PostMessage( WM_COMMAND, ID_CENTER_VIEW );
	
	// ask the frame to do any splitting layout
	// ----------------------------------------------------------------

	pFrameA->PostMessage(WM_COMMAND, ID_HANDLE_CHILD_SPLITTER_POSITIONING);
	pFrameB->PostMessage(WM_COMMAND, ID_HANDLE_CHILD_SPLITTER_POSITIONING);
	pFrameC->PostMessage(WM_COMMAND, ID_HANDLE_CHILD_SPLITTER_POSITIONING);

	return true;

HANDLE_ERROR:

	// ????

	return false;

}

// ----------------------------------------------------------------------------

//
//	CDitherStudioDoc::CreateWindowsForLayout_1()
//

bool CDitherStudioDoc::CreateWindowsForLayout_1()
{
	 CDitherStudioApp * pApp = (CDitherStudioApp *)AfxGetApp();

	 if ( !pApp ) {

		 return false;

	 }

	// ------------------------------------------------------------------------

	CFrameWnd * pFrameA = 0;
	CFrameWnd * pFrameB = 0;
	CFrameWnd * pFrameC = 0;

	// Create the edit views
	// ------------------------------------------------------------------------

	CRect rMDIClientRect;

	pFrameA = GLOBAL_CreateNewWindow(
		pApp->GetDefaultDocTemplate(), this
	);

	int width = 0;

	int height = 0;
	
	if ( pFrameA ) {
	
		// Figure out the first pos...
	
		CWnd * pWnd = pFrameA->GetParent();
	
		if ( pWnd ) {
	
			pWnd->GetClientRect( &rMDIClientRect );
	
		}

		width = rMDIClientRect.Width() / 2;
	
		// position
	
		pFrameA->MoveWindow(
			CRect( CPoint( 0, 0 ), 
				CSize( width, rMDIClientRect.Height() )
			)
		);
	
		// need to send the zoom request.
	
		pFrameA->SendMessage( WM_COMMAND, ID_VZOOM_3X );
	
	} else {

		// NEED TO HANDLE THE ERROR!!!
	
		goto HANDLE_ERROR;

	}
	
	// ----------------------------------------------------------------

	pFrameB = GLOBAL_CreateNewWindow(
		pApp->GetDefaultDocTemplate(), this
	);
	
	height = rMDIClientRect.Height() / 2;

	if ( pFrameB ) {
	
		// position
	
		pFrameB->MoveWindow(
			CRect( CPoint( width, 0 ), 
				CSize( rMDIClientRect.Width() - width, height )
			)
		);
	
	} else {
	
		// NEED TO HANDLE THE ERROR!!!
	
		goto HANDLE_ERROR;

	}
	
	// Create the grid view
	// ----------------------------------------------------------------
	
	pFrameC = GLOBAL_CreateNewWindow(
		pApp->GetDefaultDocTemplate(), this
	);
	
	if ( pFrameC ) {
	
		// position
	
		pFrameC->MoveWindow(
			CRect( CPoint( width, height ), 
				CSize( rMDIClientRect.Width() - width, rMDIClientRect.Height() - height )
			)
		);
	
		// need to send the zoom request.
	
		pFrameC->SendMessage( WM_COMMAND, ID_VZOOM_2X );

	} else {
	
		// NEED TO HANDLE THE ERROR!!!
	
		goto HANDLE_ERROR;

	}

	// Center the view (probably should call center others... :)
	// ----------------------------------------------------------------

	pFrameB->PostMessage( WM_COMMAND, ID_CENTER_VIEW );
	
	// ask the frame to do any splitting layout
	// ----------------------------------------------------------------

	pFrameA->PostMessage(WM_COMMAND, ID_HANDLE_CHILD_SPLITTER_POSITIONING);
	pFrameB->PostMessage(WM_COMMAND, ID_HANDLE_CHILD_SPLITTER_POSITIONING);
	pFrameC->PostMessage(WM_COMMAND, ID_HANDLE_CHILD_SPLITTER_POSITIONING);

	return true;

HANDLE_ERROR:

	// ????

	return false;

}

// ----------------------------------------------------------------------------

//
//	CDitherStudioDoc::CreateWindowsForLayout_2()
//

bool CDitherStudioDoc::CreateWindowsForLayout_2()
{
	 CDitherStudioApp * pApp = (CDitherStudioApp *)AfxGetApp();

	 if ( !pApp ) {

		 return false;

	 }

	// ------------------------------------------------------------------------

	CFrameWnd * pFrameA = 0;
	CFrameWnd * pFrameB = 0;

	// Create the edit views
	// ------------------------------------------------------------------------

	CRect rMDIClientRect;

	pFrameA = GLOBAL_CreateNewWindow(
		pApp->GetDefaultDocTemplate(), this
	);

	int width = 0;

	int height = 0;
	
	if ( pFrameA ) {
	
		// Figure out the first pos...
	
		CWnd * pWnd = pFrameA->GetParent();
	
		if ( pWnd ) {
	
			pWnd->GetClientRect( &rMDIClientRect );
	
		}

		width = rMDIClientRect.Width() / 2;
	
		// position
	
		pFrameA->MoveWindow(
			CRect( CPoint( 0, 0 ), 
				CSize( width, rMDIClientRect.Height() )
			)
		);
	
		// need to send the zoom request.
	
		pFrameA->SendMessage( WM_COMMAND, ID_VZOOM_1X );
	
	} else {

		// NEED TO HANDLE THE ERROR!!!
	
		goto HANDLE_ERROR;

	}
	
	// ----------------------------------------------------------------

	pFrameB = GLOBAL_CreateNewWindow(
		pApp->GetDefaultDocTemplate(), this
	);
	
	height = rMDIClientRect.Height();

	if ( pFrameB ) {
	
		// position
	
		pFrameB->MoveWindow(
			CRect( CPoint( width, 0 ), 
				CSize( rMDIClientRect.Width() - width, height )
			)
		);
	
		// need to send the zoom request.
	
		pFrameB->SendMessage( WM_COMMAND, ID_VZOOM_2X );

	} else {
	
		// NEED TO HANDLE THE ERROR!!!
	
		goto HANDLE_ERROR;

	}
	
	// Center the view (probably should call center others... :)
	// ----------------------------------------------------------------

	pFrameB->PostMessage( WM_COMMAND, ID_CENTER_VIEW );

	// ask the frame to do any splitting layout
	// ----------------------------------------------------------------

	pFrameA->PostMessage(WM_COMMAND, ID_HANDLE_CHILD_SPLITTER_POSITIONING);
	pFrameB->PostMessage(WM_COMMAND, ID_HANDLE_CHILD_SPLITTER_POSITIONING);

	return true;

HANDLE_ERROR:

	// ????

	return false;

}

// ----------------------------------------------------------------------------

//
//	CDitherStudioDoc::PerformDefaultLayout()
//

bool 
CDitherStudioDoc::PerformDefaultLayout( const int nLayout )
{
	 CDitherStudioApp * pApp = (CDitherStudioApp *)AfxGetApp();

	 if ( !pApp ) {

		 return false;

	 }

	// Make sure that the document doesn't get destroyed while
	// all of it's current views/frames are destroyed.
	// --------------------------------------------------------------------

	BOOL bSaveAutoDelete = m_bAutoDelete;

	m_bAutoDelete = FALSE;

	// Destroy the current frames/views
	// --------------------------------------------------------------------

	typedef TDocActiveFramesAndViewsInfo<CDitherStudioDoc> info_helper;

	info_helper::output_map_type * pMap = info_helper::Generate( this );

	if ( pMap ) {

		info_helper::output_map_type::iterator it = pMap->begin();
	
		while ( it != pMap->end() ) {
	
			(it->first)->DestroyWindow();
	
			++it;
	
		}

	}

	// Dispatch to the appopriate handler
	// --------------------------------------------------------------------

	bool bReturn = false;

	switch ( nLayout ) {

	default:
		bReturn = CreateWindowsForLayout_2();
		break;

	case 1:
		bReturn = CreateWindowsForLayout_1();
		break;

	case 2:
		bReturn = CreateWindowsForLayout_0();
		break;


	}

	// --------------------------------------------------------------------

	m_bAutoDelete = bSaveAutoDelete;

	// Destroy out helper 'map'
	// --------------------------------------------------------------------

	if ( pMap ) {

		info_helper::Destroy( pMap );

	}

	return bReturn;

}

// ----------------------------------------------------------------------------

void CDitherStudioDoc::OnDefaultLayout0() 
{
	PerformDefaultLayout( 0 );
}


void CDitherStudioDoc::OnDefaultLayout1() 
{
	PerformDefaultLayout( 1 );
}

void CDitherStudioDoc::OnDefaultLayout2() 
{
	PerformDefaultLayout( 2 );
}

// ----------------------------------------------------------------------------
