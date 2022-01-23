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

#if !defined(AFX_DITHERSTUDIOVIEW_H__0B55D4EE_EFBF_4962_ACE4_1CAEBF99B05A__INCLUDED_)
#define AFX_DITHERSTUDIOVIEW_H__0B55D4EE_EFBF_4962_ACE4_1CAEBF99B05A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ----------------------------------------------------------------------------

#include "DitherStudioDoc.h"

//#include "BPTDib.h"
//#include "BPTBlitter.h"

#define BPTVIEW_USE_GDI_STRETCH

// ----------------------------------------------------------------------------

class CDitherStudioView : public CScrollView
{

public:

	typedef CDitherStudioDoc::dib_type dib_type;
	typedef CDitherStudioView this_type;
	typedef CDitherStudioDoc doc_type;

protected: // create from serialization only

	CDitherStudioView();

	DECLARE_DYNCREATE(CDitherStudioView)

protected: // static data

	enum {

		SCROLL_CONTROL_TIMER_INTERNAL_ID = 1234

	};

	static CDitherStudioView * s_pControllingScrollView;
	static int s_nScrollControlTimer;
	static bool s_bFavorLeft;
	static bool s_bFavorTop;

protected: // data

	bool m_bRawView;
	bool m_bImediateUpdates;
	bool m_bCenterSync;
	bool m_bDontSyncPositions;
	bool m_bReportViewPosChanges;
	bool m_bFatBitLines;
	bool m_bCenterOtherViewsOnMButton;
	bool m_bLiveTrackOtherViews;
	bool m_bShowGridLines;

	CSize m_Zoom;
	CSize m_LastZoom;
	CSize m_PhysicalCanvasSize;
	CSize m_LogicalCanvasSize;
	CSize m_ViewCanvasSize;

	dib_type * m_pDisplayDibSection;
	dib_type * m_pNormalDibSection;

#if !defined(BPTVIEW_USE_GDI_STRETCH)

	dib_type m_ZoomDibSection;

#endif

	RECT m_BackgroundRects[ 4 ];
	int m_BackgroundRectCount;
	RECT m_ForegroundRect;

	dib_type::bitmap_type::pixel_type m_FatBitsHColor;
	dib_type::bitmap_type::pixel_type m_FatBitsVColor;

	dib_type::bitmap_type::pixel_type m_GridLinesHColor;
	dib_type::bitmap_type::pixel_type m_GridLinesVColor;

private: // internal methods

	void SyncBorderSizes();
	void SyncTitleBar();

	int GetInternalSetting( const char * sectionName, const char * baseEntryName, const char * entry, const int value );
	void PutInternalSetting( const char * sectionName, const char * baseEntryName, const char * entry, const int value );

protected: // methods that can be overriden

	virtual void M_GetLogicalViewRect( CRect & rect );
	virtual void M_ScrollToLogicalPosition( const CPoint & pt );
	virtual void M_ClientToLogical( CPoint & pt );
	virtual void M_ViewDPtoLP( LPPOINT lpPoints, const int nCount = 1 );
	virtual void M_ViewLPtoDP( LPPOINT lpPoints, const int nCount = 1 );

	virtual void M_CenterLogicalPoint( const CPoint & logicalPt );
	virtual void M_MakeLogicalPointVisible( const CPoint & logicalPt, const CSize * pBorder = 0 );
	virtual void M_SetZoom( const CSize & zoom, const bool invalidateWindow );
	virtual dib_type * M_GetCanvasDib();
	virtual void M_SetupCanvasStructures( const CSize & zoom, const bool invalidateWindow, const bool bCenter );
	virtual void M_InvalidateLogicalRect( const CRect * pRect, const bool bErase = true );

	virtual void ZoomDelta( const int dx, const int dy, const CPoint * pCenter = 0 );
	virtual void AutoSyncViewPos( const CRect & rect );

#if !defined(BPTVIEW_USE_GDI_STRETCH)
	virtual bool M_NeedToResizeZoomDib(const int cx, const int cy);
	virtual void M_EnsureCanvasRect(const CRect* pDPRect, const CRect* pLPRect);
#endif

public: // interface?

	CPoint & CursorPosToLogical( CPoint & pt );

	void ZoomLogicalPosition( const CPoint & pt, const CSize & zoom );

	void SetZoom( const CSize & zoom, const bool invalidateWindow ) {

		M_SetZoom( zoom, invalidateWindow );

	}

	void InvalidateLogicalRect( const CRect * pRect, const bool bErase = true ) {

		M_InvalidateLogicalRect( pRect, bErase );

	}

	void CenterLogicalPoint( const CPoint & logicalPt ) {

		M_CenterLogicalPoint( logicalPt );

	}

	void ScrollToLogicalPosition( const CPoint & pt ) {

		M_ScrollToLogicalPosition( pt );

	}

	void MakeLogicalPointVisible( const CPoint & logicalPt, const CSize * pBorder = 0 ) {

		M_MakeLogicalPointVisible( logicalPt, pBorder );

	}

	CRect GetCurrentLogicalRect() {

		CRect logicalRect;

		M_GetLogicalViewRect( logicalRect );

		return logicalRect;

	}

	dib_type * GetCanvasDib() {

		return M_GetCanvasDib();

	}

// Attributes
public:

	doc_type * GetDocument();

	BOOL PUBLIC_OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) {

		return OnSetCursor( pWnd, nHitTest, message );

	}

	UINT PUBLIC_OnNcHitTest( CPoint point ) {

		return OnNcHitTest( point );

	}

	bool StoreInternalSettings( const char * sectionName, const char * baseEntryName );
	bool RestoreInternalSettings( const char * sectionName, const char * baseEntryName );

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDitherStudioView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll = TRUE);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDitherStudioView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDitherStudioView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCancelMode();
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnVzoomDec();
	afx_msg void OnUpdateVzoomDec(CCmdUI* pCmdUI);
	afx_msg void OnVzoomInc();
	afx_msg void OnUpdateVzoomInc(CCmdUI* pCmdUI);
	afx_msg void OnVcmdCenterOnCursor();
	afx_msg void OnUpdateVcmdCenterOnCursor(CCmdUI* pCmdUI);
	afx_msg void OnVcmdCursorZoom();
	afx_msg void OnUpdateVcmdCursorZoom(CCmdUI* pCmdUI);
	afx_msg void OnTimer(UINT_PTR nIDEvent); // was UINT
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnVsetImediateUpdates();
	afx_msg void OnUpdateVsetImediateUpdates(CCmdUI* pCmdUI);
	afx_msg void OnVsetCenterSyncScroll();
	afx_msg void OnUpdateVsetCenterSyncScroll(CCmdUI* pCmdUI);
	afx_msg void OnVsetDisableAutoScroll();
	afx_msg void OnUpdateVsetDisableAutoScroll(CCmdUI* pCmdUI);
	afx_msg void OnVsetShowFatbits();
	afx_msg void OnUpdateVsetShowFatbits(CCmdUI* pCmdUI);
	afx_msg void OnVsetShowGrid();
	afx_msg void OnUpdateVsetShowGrid(CCmdUI* pCmdUI);
	afx_msg void OnVcmdRequestOthersCenterOnCursor();
	afx_msg void OnVsetSwitchMode();
	afx_msg void OnUpdateVsetSwitchMode(CCmdUI* pCmdUI);
	afx_msg void OnCenterView();
	afx_msg void On2VsetSwitchMode();
	afx_msg void OnUpdate2VsetSwitchMode(CCmdUI* pCmdUI);
	afx_msg void OnShowDitheredImage();
	afx_msg void OnUpdateShowDitheredImage(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnZoom( UINT nID );
	afx_msg void OnUpdateZoom( CCmdUI* pCmdUI );
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnDropFiles(HDROP hDropInfo);
};

#ifndef _DEBUG  // debug version in bpaintView.cpp
inline CDitherStudioView::doc_type * CDitherStudioView::GetDocument()
   { return (CDitherStudioView::doc_type*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DITHERSTUDIOVIEW_H__0B55D4EE_EFBF_4962_ACE4_1CAEBF99B05A__INCLUDED_)
