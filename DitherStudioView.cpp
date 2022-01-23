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
#include "BpaintStuff/BPTPrimitives.h"
#include "DitherStudioView.h"
#include "TScaleUpBlit.h"
//#include "VariousOperationsThatNeedAHome.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------

#define BPTVIEW_USE_CENTER_BORDER_CODE

//#define USE_WINDOW_COLOR_FOR_BORDER

/////////////////////////////////////////////////////////////////////////////

CDitherStudioView * CDitherStudioView::s_pControllingScrollView = 0;
int CDitherStudioView::s_nScrollControlTimer = 0;
bool CDitherStudioView::s_bFavorLeft = false;
bool CDitherStudioView::s_bFavorTop = false;

/////////////////////////////////////////////////////////////////////////////
// CDitherStudioView

IMPLEMENT_DYNCREATE(CDitherStudioView, CScrollView)

BEGIN_MESSAGE_MAP(CDitherStudioView, CScrollView)
	//{{AFX_MSG_MAP(CDitherStudioView)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_CANCELMODE()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MBUTTONDBLCLK()
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_VZOOM_DEC, OnVzoomDec)
	ON_UPDATE_COMMAND_UI(ID_VZOOM_DEC, OnUpdateVzoomDec)
	ON_COMMAND(ID_VZOOM_INC, OnVzoomInc)
	ON_UPDATE_COMMAND_UI(ID_VZOOM_INC, OnUpdateVzoomInc)
	ON_COMMAND(ID_VCMD_CENTER_ON_CURSOR, OnVcmdCenterOnCursor)
	ON_UPDATE_COMMAND_UI(ID_VCMD_CENTER_ON_CURSOR, OnUpdateVcmdCenterOnCursor)
	ON_COMMAND(ID_VCMD_CURSOR_ZOOM, OnVcmdCursorZoom)
	ON_UPDATE_COMMAND_UI(ID_VCMD_CURSOR_ZOOM, OnUpdateVcmdCursorZoom)
	ON_WM_TIMER()
	ON_WM_SETCURSOR()
	ON_WM_CREATE()
	ON_COMMAND(ID_VSET_IMEDIATE_UPDATES, OnVsetImediateUpdates)
	ON_UPDATE_COMMAND_UI(ID_VSET_IMEDIATE_UPDATES, OnUpdateVsetImediateUpdates)
	ON_COMMAND(ID_VSET_CENTER_SYNC_SCROLL, OnVsetCenterSyncScroll)
	ON_UPDATE_COMMAND_UI(ID_VSET_CENTER_SYNC_SCROLL, OnUpdateVsetCenterSyncScroll)
	ON_COMMAND(ID_VSET_DISABLE_AUTO_SCROLL, OnVsetDisableAutoScroll)
	ON_UPDATE_COMMAND_UI(ID_VSET_DISABLE_AUTO_SCROLL, OnUpdateVsetDisableAutoScroll)
	ON_COMMAND(ID_VSET_SHOW_FATBITS, OnVsetShowFatbits)
	ON_UPDATE_COMMAND_UI(ID_VSET_SHOW_FATBITS, OnUpdateVsetShowFatbits)
	ON_COMMAND(ID_VSET_SHOW_GRID, OnVsetShowGrid)
	ON_UPDATE_COMMAND_UI(ID_VSET_SHOW_GRID, OnUpdateVsetShowGrid)
	ON_COMMAND(ID_VCMD_REQUEST_OTHERS_CENTER_ON_CURSOR, OnVcmdRequestOthersCenterOnCursor)
	ON_COMMAND(ID_VSET_SWITCH_MODE, OnVsetSwitchMode)
	ON_UPDATE_COMMAND_UI(ID_VSET_SWITCH_MODE, OnUpdateVsetSwitchMode)
	ON_COMMAND(ID_CENTER_VIEW, OnCenterView)
	ON_COMMAND(ID_2_VSET_SWITCH_MODE, On2VsetSwitchMode)
	ON_UPDATE_COMMAND_UI(ID_2_VSET_SWITCH_MODE, OnUpdate2VsetSwitchMode)
	ON_COMMAND(ID_SHOW_DITHERED_IMAGE, OnShowDitheredImage)
	ON_UPDATE_COMMAND_UI(ID_SHOW_DITHERED_IMAGE, OnUpdateShowDitheredImage)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_VZOOM_1X,ID_VZOOM_10X,OnZoom)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VZOOM_1X,ID_VZOOM_10X,OnUpdateZoom)
	ON_WM_DESTROY()
	ON_WM_DROPFILES()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDitherStudioView construction/destruction

CDitherStudioView::CDitherStudioView() :
	m_pDisplayDibSection(0) 
	,m_pNormalDibSection(0)
	,m_LogicalCanvasSize(0,0)
	,m_PhysicalCanvasSize(0,0)
	,m_Zoom(2,2)
	,m_LastZoom(3,3)
	,m_bCenterSync( true )
	,m_bImediateUpdates( true )
	,m_bRawView( false )
	,m_bDontSyncPositions( false )
	,m_bReportViewPosChanges( true )
	,m_bFatBitLines( false )
	,m_bCenterOtherViewsOnMButton( true )
	,m_bLiveTrackOtherViews( false )
	,m_bShowGridLines( false )
{

	// Get the fat bits color
	// ------------------------------------------------------------------------

#if 0 // need to fix

	dib_type::bitmap_type::pixel_type fatColor = GLOBAL_GetSettingInt(
		"FatBitsColor", MakeDisplayPixelType( 0, 0, 0 )
	);

	m_FatBitsHColor = GLOBAL_GetSettingInt( "FatBitsHColor", fatColor );
	m_FatBitsVColor = GLOBAL_GetSettingInt( "FatBitsVColor", fatColor );

#endif
	
	// Get the fat bits color
	// ------------------------------------------------------------------------

#if 0 // need to fix

	dib_type::bitmap_type::pixel_type gridColor = GLOBAL_GetSettingInt(
		"GridLinesColor", MakeDisplayPixelType( 255, 255, 255 )
	);

	m_GridLinesHColor = GLOBAL_GetSettingInt( "GridLinesHColor", gridColor );
	m_GridLinesVColor = GLOBAL_GetSettingInt( "GridLinesVColor", gridColor );

#endif

	// ------------------------------------------------------------------------

	m_BackgroundRectCount = 0;

	SetRectEmpty( &m_ForegroundRect );

	for ( int i = 0; i < 4; i++ ) {

		SetRectEmpty( &m_BackgroundRects[ i ] );

	}

}

CDitherStudioView::~CDitherStudioView()
{
}

BOOL CDitherStudioView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDitherStudioView drawing

void CDitherStudioView::OnDraw(CDC* pDC)
{
	CRect clipBox;

	if ( NULLREGION != pDC->GetClipBox( &clipBox ) ) {

		clipBox.InflateRect( 1, 1 );

		// Make sure our display canvas is setup and correct
		// --------------------------------------------------------------------

		CRect lpClipBox( clipBox );

		pDC->LPtoDP( &lpClipBox );

#if !defined(BPTVIEW_USE_GDI_STRETCH)

		M_EnsureCanvasRect( &clipBox, &lpClipBox );

#endif

		// Finally do some drawing
		// --------------------------------------------------------------------

		if ( m_pDisplayDibSection ) {

			// Draw the bitmap data
			// ----------------------------------------------------------------

			CPoint backOff( 0, 0 );

			CPoint at( m_ForegroundRect.left, m_ForegroundRect.top );

			if ( CSize(1,1) == m_Zoom ) {

				BPT::T_BlitAt( pDC->GetSafeHdc(), at.x, at.y, *m_pDisplayDibSection );

			} else {

				CPoint scroll = GetScrollPosition();

				backOff = scroll;

				CRect limitedSrcRect( CPoint( 0, 0 ), m_LogicalCanvasSize );

#if !defined(BPTVIEW_USE_GDI_STRETCH)

				BPT::T_BlitAt(
					pDC->GetSafeHdc(), scroll.x + at.x, scroll.y + at.y, *m_pDisplayDibSection,
					1.0f, 1.0f, &limitedSrcRect
				);

#else
	
				BPT::T_BlitAt(
					pDC->GetSafeHdc(), at.x, at.y, *m_pNormalDibSection,
					(float)m_Zoom.cx, (float)m_Zoom.cy, &limitedSrcRect
				);

#endif

			}

			// Fill in the background rects if any
			// ----------------------------------------------------------------

			if ( m_BackgroundRectCount ) {

#if defined( USE_WINDOW_COLOR_FOR_BORDER )

				CBrush br( GetSysColor( COLOR_WINDOW ) );

				int patbltOp = PATCOPY;

				CBrush * pOldBrush = pDC->SelectObject( &br );

#else

				int patbltOp = WHITENESS;

#endif

				clipBox.OffsetRect( -backOff.x, -backOff.y );

				for ( int i = 0; i < m_BackgroundRectCount; i++ ) {

					CRect intersection;

					if ( intersection.IntersectRect( &clipBox, &m_BackgroundRects[ i ] ) ) {

						pDC->PatBlt(
							intersection.left + backOff.x, intersection.top + backOff.y, 
							intersection.Width(), intersection.Height(),
							patbltOp
						);

					}

				}

#if defined( USE_WINDOW_COLOR_FOR_BORDER )

				if ( pOldBrush ) {

					pDC->SelectObject( pOldBrush );

				}

#endif

			}

		} else {

			pDC->PatBlt(
				clipBox.left, clipBox.top, 
				clipBox.right - clipBox.left,
				clipBox.bottom - clipBox.top,
				BLACKNESS
			);
	
		}

	}

}

void CDitherStudioView::OnInitialUpdate()
{
	// perform the simple init stuff

	CScrollView::OnInitialUpdate();

	M_SetupCanvasStructures( CSize( 1, 1 ), false, true );

#if 0

	// try to make the size of the window the same size as the scroll view
	// unless that would make it larger than the MDI client rect.

	CFrameWnd * pFrame = GetParentFrame();

	if ( pFrame ) {

		CWnd * pWnd = pFrame->GetParent();

		if ( pWnd ) {

			CRect grandparentsClientRect;

			pWnd->GetClientRect( &grandparentsClientRect );

			pFrame->MoveWindow( &grandparentsClientRect );

			ResizeParentToFit();

#if 0 // center 

			CRect rect;

			pFrame->GetWindowRect( &rect );

			int x = (grandparentsClientRect.Width() - rect.Width()) / 2;
			int y = (grandparentsClientRect.Height() - rect.Height()) / 2;

			rect.OffsetRect( x - rect.left, y - rect.top );

			pFrame->MoveWindow( &rect );

#endif

		}

	}

#endif

	SyncTitleBar();

	// Try to force the menu to be what we really want.
	// ------------------------------------------------------------------------

#if 0

	doc_type* pDoc = GetDocument();

	if ( pDoc ) {

		pDoc->EnsureProperMenu( this );

	}

#endif

}

/////////////////////////////////////////////////////////////////////////////
// CDitherStudioView diagnostics

#ifdef _DEBUG
void CDitherStudioView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CDitherStudioView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CDitherStudioView::doc_type* CDitherStudioView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDitherStudioDoc)));
	return (CDitherStudioView::doc_type*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDitherStudioView message handlers

BOOL CDitherStudioView::OnEraseBkgnd(CDC* pDC) 
{
	CBrush br( GetSysColor( COLOR_WINDOW ) );
	
	FillOutsideRect( pDC, &br );

	return TRUE;
	
}

void CDitherStudioView::OnSize(UINT nType, int cx, int cy) 
{
	// if the display DIBSECTION is the same as the normal dibsection
	// then we're done :)
	// ------------------------------------------------------------------------

#if !defined(BPTVIEW_USE_GDI_STRETCH)

	if ( m_pDisplayDibSection == &m_ZoomDibSection ) {

		int nw = MINN( (m_pNormalDibSection->Width() * m_Zoom.cx), cx );
		int nh = MINN( (m_pNormalDibSection->Height() * m_Zoom.cy), cy );
		nw = ((nw + m_Zoom.cx - 1) / m_Zoom.cx) * m_Zoom.cx;
		nh = ((nh + m_Zoom.cy - 1) / m_Zoom.cy) * m_Zoom.cy;

		if ( M_NeedToResizeZoomDib( nw, nh ) ) {
	
			if ( !m_ZoomDibSection.Create( nw, nh ) ) {
	
				TRACE("Unable to create zoom dib section!");
	
			}
	
		}

	}

#endif

	// Setup the background / foreground rects
	// ------------------------------------------------------------------------

	SyncBorderSizes();

}

// ============================================================================

//
//	CDitherStudioView::OnCancelMode()
//

void CDitherStudioView::OnCancelMode() 
{
	CScrollView::OnCancelMode();

	if ( this == GetCapture() ) {

		ReleaseCapture();

	}

	doc_type* pDoc = GetDocument();

	ASSERT_VALID(pDoc);

	pDoc->Dispatch_OnCancelMode( this );

}

//
//	CDitherStudioView::OnLButtonDown()
//

void CDitherStudioView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	doc_type* pDoc = GetDocument();

	ASSERT_VALID(pDoc);

	M_ClientToLogical( point );

	pDoc->Dispatch_OnLButtonDown( this, nFlags, point );

	SetCapture();
}

//
//	CDitherStudioView::OnLButtonUp()
//

void CDitherStudioView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if ( this == GetCapture() ) ReleaseCapture();

	doc_type* pDoc = GetDocument();

	ASSERT_VALID(pDoc);

	M_ClientToLogical( point );

	pDoc->Dispatch_OnLButtonUp( this, nFlags, point );

}

//
//	CDitherStudioView::OnMouseMove()
//

void CDitherStudioView::OnMouseMove(UINT nFlags, CPoint point) 
{
	doc_type* pDoc = GetDocument();

	ASSERT_VALID(pDoc);

	M_ClientToLogical( point );

	pDoc->Dispatch_OnMouseMove( this, nFlags, point );

#if 0

	if ( (MK_MBUTTON & nFlags) || m_bLiveTrackOtherViews ) {

		if ( m_bCenterOtherViewsOnMButton ) {

			OnVcmdRequestOthersCenterOnCursor();

		}

	}

#endif

}

//
//	CDitherStudioView::OnRButtonDown()
//

void CDitherStudioView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	doc_type* pDoc = GetDocument();

	ASSERT_VALID(pDoc);

	M_ClientToLogical( point );

	pDoc->Dispatch_OnRButtonDown( this, nFlags, point );

	SetCapture();
}

//
//	CDitherStudioView::OnRButtonUp()
//

void CDitherStudioView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	if ( this == GetCapture() ) ReleaseCapture();

	doc_type* pDoc = GetDocument();

	ASSERT_VALID(pDoc);

	M_ClientToLogical( point );

	pDoc->Dispatch_OnRButtonUp( this, nFlags, point );

}

//
//	CDitherStudioView::OnMButtonDown()
//

void CDitherStudioView::OnMButtonDown(UINT nFlags, CPoint point) 
{
	doc_type* pDoc = GetDocument();

	ASSERT_VALID(pDoc);

	M_ClientToLogical( point );

	pDoc->Dispatch_OnMButtonDown( this, nFlags, point );

#if 0

	if ( m_bCenterOtherViewsOnMButton ) {

		OnVcmdRequestOthersCenterOnCursor();

	}

#endif

	SetCapture();
}

//
//	CDitherStudioView::OnMButtonUp()
//

void CDitherStudioView::OnMButtonUp(UINT nFlags, CPoint point) 
{
	if ( this == GetCapture() ) ReleaseCapture();

	doc_type* pDoc = GetDocument();

	ASSERT_VALID(pDoc);

	M_ClientToLogical( point );

	pDoc->Dispatch_OnMButtonUp( this, nFlags, point );

#if 0

	if ( m_bCenterOtherViewsOnMButton ) {

		OnVcmdRequestOthersCenterOnCursor();

	}

#endif

}

//
//	CDitherStudioView::OnMButtonDblClk()
//

void CDitherStudioView::OnMButtonDblClk(UINT nFlags, CPoint point)
{

#if 0

	doc_type* pDoc = GetDocument();

	ASSERT_VALID(pDoc);

	M_ClientToLogical( point );

	M_CenterLogicalPoint( point );

	if ( m_bCenterOtherViewsOnMButton ) {

		if ( MK_CONTROL & nFlags ) {

			m_bLiveTrackOtherViews = !m_bLiveTrackOtherViews;

		}

		OnVcmdRequestOthersCenterOnCursor();

	}

	CursorPosToLogical( point );

	pDoc->Dispatch_OnMouseMove( this, nFlags, point );

#endif

}

// ============================================================================

//
//	CDitherStudioView::M_GetCanvasDib()
//

CDitherStudioView::dib_type * 
CDitherStudioView::M_GetCanvasDib()
{
	doc_type* pDoc = GetDocument();

	ASSERT_VALID(pDoc);

	// Get down to business and ask for either the edit or composite dib
	// ------------------------------------------------------------------------

	if ( m_bRawView ) {

		return pDoc->GetRawViewDib();

	}

	return pDoc->GetDitheredViewDib();

}

//
//	CDitherStudioView::M_NeedToResizeZoomDib()
//

#if !defined(BPTVIEW_USE_GDI_STRETCH)
bool
CDitherStudioView::M_NeedToResizeZoomDib( const int cx, const int cy )
{
	return
		((m_ZoomDibSection.Width() < cx) ||
		(m_ZoomDibSection.Height() < cy));
}
#endif

//
//	CDitherStudioView::M_EnsureCanvasRect()
//

#if !defined(BPTVIEW_USE_GDI_STRETCH)
void
CDitherStudioView::M_EnsureCanvasRect( const CRect * pLPRect, const CRect *pDPRect )
{
	// if the display DIBSECTION is the same as the normal dibsection
	// then we're done :)
	// ------------------------------------------------------------------------

	if ( m_pDisplayDibSection == m_pNormalDibSection ) {

		return /* nop */;

	}

	RECT smallestUpdateRect;
	RECT * pOperationClipRect;

	// Change the size of the zoom bitmap?
	// ------------------------------------------------------------------------

	CRect clientRect;
	GetClientRect( &clientRect );

	int nw = MINN( (m_pNormalDibSection->Width() * m_Zoom.cx), clientRect.Width() );
	int nh = MINN( (m_pNormalDibSection->Height() * m_Zoom.cy), clientRect.Height() );
	nw = ((nw + m_Zoom.cx - 1) / m_Zoom.cx) * m_Zoom.cx;
	nh = ((nh + m_Zoom.cy - 1) / m_Zoom.cy) * m_Zoom.cy;

	if ( M_NeedToResizeZoomDib( nw, nh ) ) {

		if ( !m_ZoomDibSection.Create( nw, nh ) ) {

			TRACE("Unable to create zoom dib section!");

			ASSERT(false);

		}

		pOperationClipRect = 0;

	} else {

		if ( pDPRect ) {

			smallestUpdateRect = *pDPRect;

			OffsetRect( &smallestUpdateRect, -m_ForegroundRect.left, -m_ForegroundRect.top );

			pOperationClipRect = &smallestUpdateRect;

			TRACE("smallest operation (%d,%d) %dx%d\n", 
				smallestUpdateRect.left, smallestUpdateRect.top, 
				smallestUpdateRect.right - smallestUpdateRect.left, 
				smallestUpdateRect.bottom - smallestUpdateRect.top);


		} else {

			pOperationClipRect = 0;

		}

	}

	// Need to 'zoom' the canvas... 
	// ------------------------------------------------------------------------

	CPoint scroll = GetScrollPosition();

#if 1

	// this isn't bug free yet either but at least it doesn't crash as often needs more work

	BPT::T_FixedPointScaleUpBlit(
		m_Zoom.cx,
		m_Zoom.cy,
		*m_ZoomDibSection.GetBitmapPtr(),
		0, //-scroll.x,
		0, //-scroll.y,
		*m_pNormalDibSection->GetBitmapPtr(),
		BPT::TCopyROP< dib_type::bitmap_type::pixel_type >(),
		0,
		pOperationClipRect

	);

#else

	// This code is buggy and causes pagefaults don't use unless you are willing to fix it...

	BPT::T_TableScaleUpBlit(
		*m_ZoomDibSection.GetBitmapPtr(),
		-scroll.x,
		-scroll.y,
		*m_pNormalDibSection->GetBitmapPtr(),
		(m_pNormalDibSection->Width() * m_Zoom.cx),
		(m_pNormalDibSection->Height() * m_Zoom.cy),
		BPT::TCopyROP< dib_type::bitmap_type::pixel_type >(),
		0,
		pOperationClipRect
	);

#endif

	// Render the 'fat bits'
	// ------------------------------------------------------------------------

	if ( m_bFatBitLines && ((1 != m_Zoom.cx) && (1 != m_Zoom.cy)) ) {

		dib_type::bitmap_type * pBitmap = m_ZoomDibSection.GetBitmapPtr();
	
		int fw = m_pNormalDibSection->Width();
		int fh = m_pNormalDibSection->Height();

		int fsw = fw * m_Zoom.cx;
		int fsh = fh * m_Zoom.cy;
	
		// Draw the -h- fat bit lines
		// ------------------------------------------------------------------------
	
		int yo = -scroll.y;
	
		for ( int fy = 0; fy < fh; fy++ ) {
	
			BPT::T_HLinePrim(
				*pBitmap, 0, yo, fsw, m_FatBitsHColor,
				BPT::TCopyROP< dib_type::bitmap_type::pixel_type >(),
				pOperationClipRect
			);
	
			yo += m_Zoom.cy;
	
		}
	
		// Draw the -v- fat bit lines
		// ------------------------------------------------------------------------
	
		int xo = -scroll.x;
	
		for ( int fx = 0; fx < fw; fx++ ) {
	
			BPT::T_VLinePrim(
				*pBitmap, xo, 0, fsh, m_FatBitsVColor,
				BPT::TCopyROP< dib_type::bitmap_type::pixel_type >(),
				pOperationClipRect
			);
	
			xo += m_Zoom.cx;
	
		}

	}

}
#endif

//
//	SyncBorderSizes()
//

void
CDitherStudioView::SyncBorderSizes()
{
	// ------------------------------------------------------------------------

	m_LogicalCanvasSize.cx = m_PhysicalCanvasSize.cx * m_Zoom.cx;
	m_LogicalCanvasSize.cy = m_PhysicalCanvasSize.cy * m_Zoom.cy;

	// ------------------------------------------------------------------------

	CRect clientRect;

	GetClientRect( &clientRect );

	// ------------------------------------------------------------------------

	int lastCount = m_BackgroundRectCount;

#if defined( BPTVIEW_USE_CENTER_BORDER_CODE )

	if ( (clientRect.Width() > m_LogicalCanvasSize.cx) ||
		(clientRect.Height() > m_LogicalCanvasSize.cy) ) {

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// ICKY ICKY ICKY

		CRect backgroundClipRect = clientRect;

		// find the center

		CSize clientSize = clientRect.Size();

		if ( clientSize.cx > m_LogicalCanvasSize.cx ) {

			m_ViewCanvasSize.cx = clientSize.cx;

			m_ForegroundRect.left = (clientSize.cx - m_LogicalCanvasSize.cx) / 2;

		} else {

			m_ViewCanvasSize.cx = m_LogicalCanvasSize.cx;

			m_ForegroundRect.left = 0;

			backgroundClipRect.right += (m_LogicalCanvasSize.cx - clientSize.cx);

		}

		m_ForegroundRect.right = m_ForegroundRect.left + m_LogicalCanvasSize.cx;

		// --------

		if ( clientSize.cy > m_LogicalCanvasSize.cy ) {

			m_ViewCanvasSize.cy = clientSize.cy;

			m_ForegroundRect.top = (clientSize.cy - m_LogicalCanvasSize.cy) / 2;

		} else {

			m_ViewCanvasSize.cy = m_LogicalCanvasSize.cy;

			m_ForegroundRect.top = 0;

			backgroundClipRect.bottom += (m_LogicalCanvasSize.cy - clientSize.cy);

		}

		m_ForegroundRect.bottom = m_ForegroundRect.top + m_LogicalCanvasSize.cy;

		// --------

		m_BackgroundRectCount = BPT::ReturnExternalClipRects(
			m_BackgroundRects, &backgroundClipRect, &m_ForegroundRect
		);

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// ICKY ICKY ICKY

	} else {

#endif

		m_BackgroundRectCount = 0;
	
		m_ForegroundRect.left = 0;
		m_ForegroundRect.top = 0;
		m_ForegroundRect.right = m_LogicalCanvasSize.cx;
		m_ForegroundRect.bottom = m_LogicalCanvasSize.cy;

		m_ViewCanvasSize = m_LogicalCanvasSize;

#if defined( BPTVIEW_USE_CENTER_BORDER_CODE )

	}

#endif

#if 1

	SetScrollSizes(MM_TEXT, m_ViewCanvasSize );

#endif

	// ------------------------------------------------------------------------

	InvalidateRect( NULL, false );

}

//
//	CDitherStudioView::M_SetupCanvasStructures()
//

void 
CDitherStudioView::M_SetupCanvasStructures( const CSize & zoom, const bool invalidateWindow, const bool bCenter )
{
	// ------------------------------------------------------------------------

	m_pNormalDibSection = M_GetCanvasDib();

	CSize size( m_pNormalDibSection->Width(), m_pNormalDibSection->Height() );

	// Set the display dib
	// ------------------------------------------------------------------------

#if defined(BPTVIEW_USE_GDI_STRETCH)

	if (CSize(1, 1) != zoom) {

		m_LastZoom = zoom;

	}

	m_pDisplayDibSection = m_pNormalDibSection;

#else

	if ( CSize(1,1) == zoom ) {

		m_pDisplayDibSection = m_pNormalDibSection;

	} else {

		m_pDisplayDibSection = &m_ZoomDibSection;

		m_LastZoom = zoom;

	}

#endif

	// Setup the helper variables
	// ------------------------------------------------------------------------

	m_Zoom = zoom;

	m_PhysicalCanvasSize = size;

	// Deal with the centering issues if any
	// ------------------------------------------------------------------------

	SyncBorderSizes();

	// Finally setup the scrolling information
	// ------------------------------------------------------------------------

#if 0

	SetScrollSizes(MM_TEXT, m_ViewCanvasSize );

#endif

	if ( invalidateWindow ) {

		InvalidateRect( NULL, false );

	}

	if ( bCenter ) {
		
		M_CenterLogicalPoint( CPoint( size.cx/2, size.cy/2 ) );

	}

}

//
//	CDitherStudioView::M_SetZoom()
//

void 
CDitherStudioView::M_SetZoom( const CSize & zoom, const bool invalidateWindow )
{
	if ( (0 >= zoom.cx) || (0 >= zoom.cy) ) {

		return;

	}

	if ( zoom != m_Zoom ) {

		M_SetupCanvasStructures( zoom, invalidateWindow, false );

		SyncTitleBar();

	}

}

//
//	CDitherStudioView::M_ClientToLogical()
//

void
CDitherStudioView::M_ClientToLogical( CPoint & pt )
{
	ASSERT(0 != m_Zoom.cx);
	ASSERT(0 != m_Zoom.cy);

	CPoint scrollPt = GetDeviceScrollPosition();

	pt.x = ((pt.x - m_ForegroundRect.left) + scrollPt.x) / m_Zoom.cx;
	pt.y = ((pt.y - m_ForegroundRect.top) + scrollPt.y) / m_Zoom.cy;

}

//
//	CDitherStudioView::M_ViewDPtoLP()
//

void
CDitherStudioView::M_ViewDPtoLP( LPPOINT lpPoints, const int nCount )
{
    ASSERT(0 != m_Zoom.cx);
    ASSERT(0 != m_Zoom.cy);
    ASSERT(m_nMapMode > 0);

	{
		CWindowDC dc( this );
		OnPrepareDC( &dc );
		dc.DPtoLP( lpPoints, nCount );
	}

	for ( int i = 0; i < nCount; i++ ) {

		// adjust for the 'border'

		lpPoints[ i ].x -= m_ForegroundRect.left;
		lpPoints[ i ].y -= m_ForegroundRect.top;

		// adjust for the zoom

		lpPoints[ i ].x /= m_Zoom.cx;
		lpPoints[ i ].y /= m_Zoom.cy;

	}
}

//
//	CDitherStudioView::M_ViewLPtoDP()
//

void
CDitherStudioView::M_ViewLPtoDP( LPPOINT lpPoints, const int nCount )
{
    ASSERT(m_nMapMode > 0);

	for ( int i = 0; i < nCount; i++ ) {

		lpPoints[ i ].x *= m_Zoom.cx;
		lpPoints[ i ].y *= m_Zoom.cy;

		// adjust for the 'border'

		lpPoints[ i ].x += m_ForegroundRect.left;
		lpPoints[ i ].y += m_ForegroundRect.top;

	}

    CWindowDC dc( this );
    OnPrepareDC( &dc );
    dc.LPtoDP( lpPoints, nCount );
}

//
//	CDitherStudioView::M_GetLogicalViewRect()
//

void 
CDitherStudioView::M_GetLogicalViewRect( CRect & rect )
{
    ASSERT(0 != m_Zoom.cx);
    ASSERT(0 != m_Zoom.cy);

	GetClientRect( &rect );

	rect += GetScrollPosition();

	rect.left /= m_Zoom.cx;
	rect.top /= m_Zoom.cy;
	rect.right = ((rect.right + m_Zoom.cx - 1) / m_Zoom.cx);
	rect.bottom = ((rect.bottom + m_Zoom.cy - 1) / m_Zoom.cy);

}

//
//	CDitherStudioView::M_ScrollToLogicalPosition()
//

void 
CDitherStudioView::M_ScrollToLogicalPosition( const CPoint & pt )
{
	CPoint npt( pt.x * m_Zoom.cx, pt.y * m_Zoom.cy );

	CSize totalSize = GetTotalSize();

	CRect clientRect;

	GetClientRect( &clientRect );

	CSize limit(
		MAXX( 0, (totalSize.cx - clientRect.Width()) ), 
		MAXX( 0, (totalSize.cy - clientRect.Height()) )
	);

	npt.x = MAXX( 0, MINN( npt.x, limit.cx ));
	npt.y = MAXX( 0, MINN( npt.y, limit.cy ));

	// call OnScrollBy() to get the other views to sync up.
	// ------------------------------------------------------------------------

	CPoint xpt = GetScrollPosition();
	OnScrollBy( CSize( npt.x - xpt.x, npt.y - xpt.y ), true );

}

//
//	CDitherStudioView::M_CenterLogicalPoint()
//

void 
CDitherStudioView::M_CenterLogicalPoint( const CPoint & logicalPt )
{
	CRect logicalRect;

	M_GetLogicalViewRect( logicalRect );

	M_ScrollToLogicalPosition(
		CPoint( 
			logicalPt.x - (logicalRect.Width() / 2),
			logicalPt.y - (logicalRect.Height() / 2)
		)
	);
}

//
//	CDitherStudioView::M_MakeLogicalPointVisible()
//

void 
CDitherStudioView::M_MakeLogicalPointVisible( const CPoint & logicalPt, const CSize * pBorder )
{
	// Determine the current logical rect with border adjustment.
	// ------------------------------------------------------------------------

	CRect logicalRect;

	M_GetLogicalViewRect( logicalRect );

	CSize border = (pBorder) ? *pBorder : CSize(0,0);

	logicalRect.left += border.cx;
	logicalRect.top += border.cy;
	logicalRect.right -= border.cx;
	logicalRect.bottom -= border.cy;

	// Determine if we need to scroll and if so scroll the "minimum".
	// ------------------------------------------------------------------------

	if ( !logicalRect.PtInRect( logicalPt ) ) {

		CPoint pt;

		// determine new x scroll position
		// --------------------------------------------------------------------

		pt.x = logicalRect.left - border.cx;

		if ( logicalPt.x >= logicalRect.right ) {

			pt.x += (logicalPt.x - (logicalRect.right));

		} else if ( logicalPt.x < logicalRect.left ) {

			pt.x += (logicalPt.x - logicalRect.left);

		}

		// determine new y scroll position
		// --------------------------------------------------------------------

		pt.y = logicalRect.top - border.cy;

		if ( logicalPt.y >= logicalRect.bottom ) {

			pt.y += (logicalPt.y - (logicalRect.bottom));

		} else if ( logicalPt.y < logicalRect.top ) {

			pt.y += (logicalPt.y - logicalRect.top);

		}

		// Ask the scrolling primitive to scroll.
		// --------------------------------------------------------------------

		M_ScrollToLogicalPosition( pt );

	}

}

//
//	CDitherStudioView::M_InvalidateLogicalRect()
//

void 
CDitherStudioView::M_InvalidateLogicalRect( const CRect * pRect, const bool bErase )
{
	if ( pRect ) {

		// Scale up the coordinates

		CRect phRect(
			pRect->left * m_Zoom.cx, 
			pRect->top * m_Zoom.cy,
			pRect->right * m_Zoom.cx + 1,
			pRect->bottom * m_Zoom.cy + 1
		);

		// Convert logical coords to physical coords

		CWindowDC dc( this );
		OnPrepareDC( &dc );
		dc.LPtoDP( &phRect );

		// adjust for the potential border.

		phRect.OffsetRect( m_ForegroundRect.left, m_ForegroundRect.top );

		// increase

		--phRect.left;
		--phRect.top;

		++phRect.right;
		++phRect.bottom;

		// finally do something

		InvalidateRect( &phRect, bErase );

	} else {

		InvalidateRect( NULL, bErase );

	}

}

void 
CDitherStudioView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// Check to see if there was a hint and if so what type

	if ( pHint ) {

		if ( pHint->IsKindOf( RUNTIME_CLASS( CDocRectChange ) ) ) {

			CDocRectChange * pDocRequest = ((CDocRectChange *)pHint);

			CRect rect( pDocRequest->GetRect() );

			switch ( lHint ) {

			default:
			case CDocRectChange::INVALIDATE_RECT: // Update message

				InvalidateLogicalRect( &rect, false );

				if ( m_bImediateUpdates &&
					(CDocRectChange::SILENT_INVALIDATE_RECT != lHint) ) {

					UpdateWindow();

				}
				break;

			case CDocRectChange::SYNC_VIEW_RECT: // Scroll message
				if ( this != pSender ) {

					AutoSyncViewPos( rect );

				}
				break;

			case CDocRectChange::CENTER_VIEW_REQUEST: // Scroll message
				if ( this != pSender ) {

					bool oldMode = m_bReportViewPosChanges;

					m_bReportViewPosChanges = false;

					M_CenterLogicalPoint( rect.CenterPoint() );

					m_bReportViewPosChanges = oldMode;

				}
				break;

			case CDocRectChange::NEW_CANVAS_SIZE: // Change the canvas size

				M_SetupCanvasStructures( m_Zoom, true, false );

				UpdateWindow();

				break;

			case CDocRectChange::MOVE_TOP_LEFT_REQUEST:
				if ( this == pDocRequest->GetView() ) {

//					bool oldMode = m_bReportViewPosChanges;

					M_ScrollToLogicalPosition( rect.CenterPoint() );

//					m_bReportViewPosChanges = oldMode;

				}
				break;

			case CDocRectChange::SCROLL_RELATIVE:
				if ( this == pDocRequest->GetView() ) {
					OnScrollBy( CSize( rect.left, rect.top ), TRUE );
				}
				break;

			}

		}

	}

}

BOOL CDitherStudioView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	if ( !( MK_CONTROL & nFlags) ) {

		return CScrollView::OnMouseWheel(nFlags, zDelta, pt);

	}

	CPoint crsrPos;
	
	CursorPosToLogical( crsrPos );

	zDelta = -zDelta; // reverse to match google maps

	if ( 0 < zDelta ) {

		if ( 1 < m_Zoom.cx ) {

			ZoomDelta( -1, -1, &crsrPos );

		}

	} else if ( 0 > zDelta ) {

		if ( 32 > m_Zoom.cx ) {

			ZoomDelta( +1, +1, &crsrPos );

		}

	}

	return TRUE;

}

void
CDitherStudioView::ZoomDelta( const int dx, const int dy, const CPoint * pCenter )
{
	// ------------------------------------------------------------------------

	CPoint center;

	if ( pCenter ) {

		center = *pCenter;

	} else {

		CRect logicalRect;
	
		M_GetLogicalViewRect( logicalRect );
	
		center = CPoint(
			((logicalRect.right - 1) + logicalRect.left) / 2, 
			((logicalRect.bottom - 1) + logicalRect.top) / 2
		);

	}

	// ------------------------------------------------------------------------

	CSize newZoom = m_Zoom;

	newZoom.cx += dx;
	newZoom.cy += dy;

	M_SetZoom( newZoom, true );

	// ------------------------------------------------------------------------

	M_CenterLogicalPoint( center );

}

void CDitherStudioView::OnVzoomDec() 
{
	CPoint pt;

	ZoomDelta( -1, -1, &CursorPosToLogical( pt ) );
}

void CDitherStudioView::OnUpdateVzoomDec(CCmdUI* pCmdUI) 
{
	if ( 1 < m_Zoom.cx ) {

		pCmdUI->Enable( TRUE );

	} else {

		pCmdUI->Enable( FALSE );

	}
}

void CDitherStudioView::OnVzoomInc() 
{
	CPoint pt;

	ZoomDelta( +1, +1, &CursorPosToLogical( pt ) );
}

void CDitherStudioView::OnUpdateVzoomInc(CCmdUI* pCmdUI) 
{
	if ( 32 > m_Zoom.cx ) {

		pCmdUI->Enable( TRUE );

	} else {

		pCmdUI->Enable( FALSE );

	}
}

BOOL CDitherStudioView::OnScrollBy(CSize sizeScroll, BOOL bDoScroll) 
{
	if ( CScrollView::OnScrollBy( sizeScroll, bDoScroll ) ) {

		if ( bDoScroll ) {

			if ( !s_pControllingScrollView ) {

				s_pControllingScrollView = this;

			}

			if ( this != s_pControllingScrollView ) {

				return TRUE;

			}

			// Reset the timer (destroy any existing)
			// ----------------------------------------------------------------

			if ( s_nScrollControlTimer ) {

				KillTimer( s_nScrollControlTimer );

			}

			s_nScrollControlTimer = SetTimer(
				SCROLL_CONTROL_TIMER_INTERNAL_ID, 500, 0
			);

			if ( !s_nScrollControlTimer ) {

				s_pControllingScrollView = 0;

			}

			// Detect the type of scrolling and have the other views try to
			// keep that direction's pixels in view. (might be lame)
			// ----------------------------------------------------------------

			s_bFavorLeft = (0 > sizeScroll.cx);
			s_bFavorTop = (0 > sizeScroll.cy);

			// Get the current logical rect
			// ----------------------------------------------------------------

			if ( m_bReportViewPosChanges ) {
	
				CRect logicalRect;
				M_GetLogicalViewRect( logicalRect );
	
				// Need to report the new logcial view coordinates.
		
				doc_type* pDoc = GetDocument();
				ASSERT_VALID(pDoc);
		
				CDocRectChange logicalViewRect( logicalRect );
		
				pDoc->UpdateAllViews( this, CDocRectChange::SYNC_VIEW_RECT, &logicalViewRect );

			}

			SyncTitleBar();

		}

		return TRUE;

	}

	return FALSE;
}

// ----------------------------------------------------------------------------

CPoint &  
CDitherStudioView::CursorPosToLogical( CPoint & pt )
{
	GetCursorPos( &pt );
	ScreenToClient( &pt );
	M_ClientToLogical( pt );
	return pt;
}

void 
CDitherStudioView::ZoomLogicalPosition( const CPoint & pt, const CSize & zoom )
{
	if ( zoom != m_Zoom ) {

		ZoomDelta( zoom.cx - m_Zoom.cx, zoom.cy - m_Zoom.cy, &pt );

	} else {

		M_CenterLogicalPoint( pt );
	
	}

}

// ----------------------------------------------------------------------------

void CDitherStudioView::OnVcmdCenterOnCursor() 
{
	// Figure out where the cursor is for the view
	// ask the view to scroll that point to the center.

	CPoint pt;

	M_CenterLogicalPoint( CursorPosToLogical( pt ) );

}

void CDitherStudioView::OnUpdateVcmdCenterOnCursor(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
}

// ----------------------------------------------------------------------------

void CDitherStudioView::OnVcmdCursorZoom() 
{
	// Get the view relative logical cursor position

	CPoint pt;

	CursorPosToLogical( pt );

	// Okay now have some fun!

	if ( CSize( 1, 1 ) == m_Zoom ) {

		ZoomLogicalPosition( pt, m_LastZoom );

	} else {

		ZoomLogicalPosition( pt, CSize( 1, 1 ) );

	}
}

void CDitherStudioView::OnUpdateVcmdCursorZoom(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
}

// ----------------------------------------------------------------------------

void
CDitherStudioView::OnZoom( UINT nID )
{
	int d = ((nID - ID_VZOOM_1X) + 1) - m_Zoom.cx;
	ZoomDelta( d, d );
}

void 
CDitherStudioView::OnUpdateZoom( CCmdUI* pCmdUI )
{
	pCmdUI->Enable();
	pCmdUI->SetCheck( (pCmdUI->m_nID == (m_Zoom.cx + ID_VZOOM_1X - 1)) ? 1 : 0 );

}

// ----------------------------------------------------------------------------

void 
CDitherStudioView::AutoSyncViewPos( const CRect & rect )
{
	// ------------------------------------------------------------------------

	if ( m_bDontSyncPositions ) {

		return;

	}

	// ------------------------------------------------------------------------

	CRect logicalRect;
	
	M_GetLogicalViewRect( logicalRect );

	// Check to see if were in simple center sync mode
	// ------------------------------------------------------------------------

	if ( m_bCenterSync ) {

		M_CenterLogicalPoint( rect.CenterPoint() );
		return;

	}

	// If not in center mode switch to keep visible mode.
	// ------------------------------------------------------------------------

	int x;

	if ( s_bFavorLeft ) {

		x = MINN( rect.left, MAXX( rect.right - 1, logicalRect.left ) );

	} else {

		x = MINN( rect.right - logicalRect.Width(), MAXX( rect.right - 1, logicalRect.left ) );

	}

	int y;

	if ( s_bFavorTop ) {

		y = MINN( rect.top, MAXX( rect.bottom - 1, logicalRect.top ) );

	} else {

		y = MINN( rect.bottom - logicalRect.Height(), MAXX( rect.bottom - 1, logicalRect.top ) );

	}

	M_ScrollToLogicalPosition( CPoint( x, y ) );

}

void CDitherStudioView::OnTimer(UINT_PTR nIDEvent) // was UINT
{
	CScrollView::OnTimer(nIDEvent);

	if ( SCROLL_CONTROL_TIMER_INTERNAL_ID == nIDEvent ) {

		s_pControllingScrollView = 0;

		if ( s_nScrollControlTimer ) {

			KillTimer( s_nScrollControlTimer );
			s_nScrollControlTimer = 0;

		}

	}

}

BOOL CDitherStudioView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	doc_type* pDoc = GetDocument();
	
	ASSERT_VALID(pDoc);

	// --------------------------------------------------------------------

	CPoint point;

	CursorPosToLogical( point );
	
	// --------------------------------------------------------------------

	HCURSOR hCrsr = pDoc->GetViewCursor( this, point, nHitTest );
	
	if ( hCrsr ) {

		SetCursor( hCrsr );

		return TRUE;

	}

	return CScrollView::OnSetCursor(pWnd, nHitTest, message);
}

int CDitherStudioView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Do this here to avoid assert.

	SetScrollSizes( MM_TEXT, CSize(1,1) );

	DragAcceptFiles(TRUE);
	
	return 0;
}

void CDitherStudioView::OnVsetImediateUpdates() 
{
	m_bImediateUpdates = !m_bImediateUpdates;
}

void CDitherStudioView::OnUpdateVsetImediateUpdates(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();

	pCmdUI->SetCheck( m_bImediateUpdates ? 1 : 0 );
}

void CDitherStudioView::OnVsetCenterSyncScroll() 
{
	m_bCenterSync = !m_bCenterSync;
}

void CDitherStudioView::OnUpdateVsetCenterSyncScroll(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
	pCmdUI->SetCheck( m_bCenterSync ? 1 : 0 );
}

void CDitherStudioView::OnVsetDisableAutoScroll() 
{
	m_bDontSyncPositions = !m_bDontSyncPositions;
}

void CDitherStudioView::OnUpdateVsetDisableAutoScroll(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
	pCmdUI->SetCheck( m_bDontSyncPositions ? 1 : 0 );
}

void CDitherStudioView::OnVsetShowFatbits() 
{
	m_bFatBitLines = !m_bFatBitLines;
	InvalidateRect( NULL, FALSE );
}

void CDitherStudioView::OnUpdateVsetShowFatbits(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( (1 != m_Zoom.cx) );
	pCmdUI->SetCheck( m_bFatBitLines ? 1 : 0 );
}

void CDitherStudioView::OnVsetShowGrid() 
{
	m_bShowGridLines = !m_bShowGridLines;
	InvalidateRect( NULL, FALSE );
}

void CDitherStudioView::OnUpdateVsetShowGrid(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( TRUE );
	pCmdUI->SetCheck( m_bShowGridLines ? 1 : 0 );
}

void CDitherStudioView::OnVcmdRequestOthersCenterOnCursor() 
{
	doc_type* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPoint pt;

	CRect rect( CursorPosToLogical( pt ), CSize( 1, 1 ) );
		
	CDocRectChange logicalViewRect( rect );
		
	pDoc->UpdateAllViews( this, CDocRectChange::CENTER_VIEW_REQUEST, &logicalViewRect );
}

void CDitherStudioView::OnVsetSwitchMode() 
{
	m_bRawView = !m_bRawView;

	// Need to switch the dib section used for blitting!

	m_pNormalDibSection = M_GetCanvasDib();

#if defined(BPTVIEW_USE_GDI_STRETCH)

	m_pDisplayDibSection = m_pNormalDibSection;

#else

	if ( m_pDisplayDibSection != &m_ZoomDibSection ) {

		m_pDisplayDibSection = m_pNormalDibSection;

	}

#endif

	// Invalidate the window

	InvalidateRect( NULL, FALSE );

	// This should probably tell the document somehow else

	doc_type* pDoc = GetDocument();

	ASSERT_VALID(pDoc);

	pDoc->ForceFullRedraw();

	SyncTitleBar();
}

void CDitherStudioView::OnUpdateVsetSwitchMode(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();

	pCmdUI->SetCheck( (!m_bRawView) ? 1 : 0 );
}

void CDitherStudioView::On2VsetSwitchMode() 
{
	OnVsetSwitchMode();
}

void CDitherStudioView::OnUpdate2VsetSwitchMode(CCmdUI* pCmdUI) 
{
	OnUpdateVsetSwitchMode( pCmdUI );
}

void CDitherStudioView::OnCenterView() 
{
	M_CenterLogicalPoint( CPoint( m_PhysicalCanvasSize.cx/2, m_PhysicalCanvasSize.cy/2 ) );
}

// ----------------------------------------------------------------------------

void CDitherStudioView::SyncTitleBar() {

	// ------------------------------------------------------------------------

	CString docTitle = GetDocument()->GetTitle();

	// ------------------------------------------------------------------------

	CString modeStr;

	if ( m_bRawView ) {

		modeStr = "RAW";

	} else {

		modeStr = "Dithered";

	}

	// ------------------------------------------------------------------------

	CRect logicalRect;

	M_GetLogicalViewRect( logicalRect );

	// ------------------------------------------------------------------------

 	CString finalTitle;

	finalTitle.Format( "%s, zoom x %d, view %d, %d , %d, %d ( %s )",
		(LPCSTR)modeStr, m_Zoom.cx, 
		logicalRect.left, logicalRect.top, logicalRect.right - 1, logicalRect.bottom - 1, 
		(LPCSTR)docTitle
	);

	// ------------------------------------------------------------------------

	SetWindowText( finalTitle );

	GetParentFrame()->SetWindowText( finalTitle );

}

// ----------------------------------------------------------------------------

//
//	CDitherStudioView::GetInternalSetting()
//

int CDitherStudioView::GetInternalSetting(
	const char * sectionName, const char * baseEntryName,
	const char * entry, const int value
)
{
	CString finalEntryName;

	finalEntryName.Format( "%s.%s", baseEntryName, entry );

#if 1 // need to replace this eventually

	return value;

#else

	return GLOBAL_GetSettingInt( finalEntryName, value, sectionName );

#endif

}

//
//	CDitherStudioView::PutInternalSetting()
//

void CDitherStudioView::PutInternalSetting(
	const char * sectionName, const char * baseEntryName, 
	const char * entry, const int value
)
{
	CString finalEntryName;

	finalEntryName.Format( "%s.%s", baseEntryName, entry );

#if 0 // need to replace this eventually

	GLOBAL_PutSettingInt( finalEntryName, value, sectionName );

#endif

}

// ----------------------------------------------------------------------------

//
//	CDitherStudioView::StoreInternalSettings()
//

bool CDitherStudioView::StoreInternalSettings(
	const char * sectionName, const char * baseEntryName
)
{
	// -------------------------------------------------------------------------

	PutInternalSetting( sectionName, baseEntryName, "Zoom.cx", m_Zoom.cx );
	PutInternalSetting( sectionName, baseEntryName, "Zoom.cy", m_Zoom.cy );

	// -------------------------------------------------------------------------

	PutInternalSetting( sectionName, baseEntryName, "LastZoom.cx", m_LastZoom.cx );
	PutInternalSetting( sectionName, baseEntryName, "LastZoom.cy", m_LastZoom.cy );

	// -------------------------------------------------------------------------

	PutInternalSetting( sectionName, baseEntryName, "bCenterSync", m_bCenterSync ? 1 : 0 );

	// -------------------------------------------------------------------------

	PutInternalSetting( sectionName, baseEntryName, "bImediateUpdates", m_bImediateUpdates ? 1 : 0  );

	// -------------------------------------------------------------------------

	PutInternalSetting( sectionName, baseEntryName, "bDontSyncPositions", m_bDontSyncPositions ? 1 : 0  );

	// -------------------------------------------------------------------------

	PutInternalSetting( sectionName, baseEntryName, "bFatBitLines", m_bFatBitLines ? 1 : 0  );

	// -------------------------------------------------------------------------

	PutInternalSetting( sectionName, baseEntryName, "bCenterOtherViewsOnMButton", m_bCenterOtherViewsOnMButton ? 1 : 0  );
	
	// -------------------------------------------------------------------------

	return true;
}

//
//	CDitherStudioView::RestoreInternalSettings()
//

bool CDitherStudioView::RestoreInternalSettings(
	const char * sectionName, const char * baseEntryName
)
{
	// -------------------------------------------------------------------------

	m_Zoom.cx = GetInternalSetting( sectionName, baseEntryName, "Zoom.cx", m_Zoom.cx );
	m_Zoom.cy = GetInternalSetting( sectionName, baseEntryName, "Zoom.cy", m_Zoom.cy );

	// -------------------------------------------------------------------------

	m_LastZoom.cx = GetInternalSetting( sectionName, baseEntryName, "LastZoom.cx", m_LastZoom.cx );
	m_LastZoom.cy = GetInternalSetting( sectionName, baseEntryName, "LastZoom.cy", m_LastZoom.cy );

	// -------------------------------------------------------------------------

	m_bCenterSync = (1 == GetInternalSetting( sectionName, baseEntryName, "bCenterSync", m_bCenterSync ? 1 : 0 ));

	// -------------------------------------------------------------------------

	m_bImediateUpdates = (1 == GetInternalSetting( sectionName, baseEntryName, "bImediateUpdates", m_bImediateUpdates ? 1 : 0 ));

	// -------------------------------------------------------------------------

	m_bDontSyncPositions = (1 == GetInternalSetting( sectionName, baseEntryName, "bDontSyncPositions", m_bDontSyncPositions ? 1 : 0 ));

	// -------------------------------------------------------------------------

	m_bFatBitLines = (1 == GetInternalSetting( sectionName, baseEntryName, "bFatBitLines", m_bFatBitLines ? 1 : 0 ));

	// -------------------------------------------------------------------------

	m_bCenterOtherViewsOnMButton = (1 == GetInternalSetting(
		sectionName, baseEntryName, "bCenterOtherViewsOnMButton", 
		m_bCenterOtherViewsOnMButton ? 1 : 0 ));

	// -------------------------------------------------------------------------

	M_SetupCanvasStructures( m_Zoom, true, false );

	// -------------------------------------------------------------------------

	SyncTitleBar();

	return true;
}

// ----------------------------------------------------------------------------

void CDitherStudioView::OnShowDitheredImage() 
{
	OnVsetSwitchMode();
}

void CDitherStudioView::OnUpdateShowDitheredImage(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
	pCmdUI->SetCheck( !m_bRawView ? 1 : 0 );
}

// ----------------------------------------------------------------------------


void CDitherStudioView::OnDestroy()
{
	CScrollView::OnDestroy();

	if ((this == s_pControllingScrollView) && s_nScrollControlTimer) {

		KillTimer(s_nScrollControlTimer);

		s_nScrollControlTimer = 0;

	}

	// TODO: Add your message handler code here
}


void CDitherStudioView::OnDropFiles(HDROP hDropInfo)
{
	// TODO: Add your message handler code here and/or call default

	char szFilename[MAX_PATH];

	UINT r = DragQueryFile(hDropInfo, 0, szFilename, MAX_PATH);

	if (0 != r) {

		doc_type* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		pDoc->ActuallyProcessLoadRequest(szFilename);

	}

	CScrollView::OnDropFiles(hDropInfo);
}
