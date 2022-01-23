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

#if !defined(AFX_DITHERSTUDIODOC_H__AAF20D8B_284A_4328_8B92_09B06ECFDAA7__INCLUDED_)
#define AFX_DITHERSTUDIODOC_H__AAF20D8B_284A_4328_8B92_09B06ECFDAA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ----------------------------------------------------------------------------

#include "BpaintStuff/BPTDib.h"
#include "BpaintStuff/BPTBlitter.h"
#include "SimpleErrorDiffusion.h"
#include "BPTSRL.h"
#include "TQuantizedHistogram.h"

// ----------------------------------------------------------------------------

class CDocRectChange : public CObject {

public:

	DECLARE_DYNAMIC( CDocRectChange )

private:

	RECT m_Rect;

	CView * m_pView;

public:

	enum {

		INVALIDATE_RECT				= 0
		,SYNC_VIEW_RECT				= 1
		,CENTER_VIEW_REQUEST		= 2
		,SILENT_INVALIDATE_RECT		= 3
		,NEW_CANVAS_SIZE			= 4
		,MOVE_TOP_LEFT_REQUEST		= 5
		,SCROLL_RELATIVE			= 6

	};

	CDocRectChange() {

		SetRectEmpty( &m_Rect );

		m_pView = 0;

	}

	CDocRectChange( const RECT & rect, CView * pView = 0 ) : 
		m_Rect( rect ), m_pView( pView ) {

		/* Empty */

	}

	const RECT & GetRect() const {

		return m_Rect;

	}

	CView * GetView() const {

		return m_pView;

	}

};

	// ------------------------------------------------------------------------
	// Quantize
	// ------------------------------------------------------------------------

namespace BPT {


}; // namespace BPT

// ----------------------------------------------------------------------------

class CDitherStudioDoc : public CDocument
{

public: // traits

	typedef SDisplayPixelType pixel_type;
	typedef BPT::TDIBSection<pixel_type> dib_type;
	typedef BPT::TSimpleBitmap<pixel_type> bitmap_type;
	typedef bitmap_type::default_transfer_op copy_op_type; // BPT::TCopyROP<pixel_type> copy_op_type;
	typedef BPT::TSRLCompressedImage< BPT::TSRLCompressor< pixel_type > > compressed_image_type;

	// ------------------------------------------------------------------------

public: // temporary hack to get stuff displaying 

	typedef typename BPT::TQuantizedHistogram<BPT::PIXEL_R6G6B6, pixel_type> histogram_t;
	histogram_t* m_pLastHistogram;

	typedef typename BPT::TFibonacciSphereVisualizationForHistogram<histogram_t> fibsphere_viz_t;
	fibsphere_viz_t* m_pLastFibSphereViz;

	// ------------------------------------------------------------------------

	typedef BPT::TCheapQuantizeOP<pixel_type, 0x808080> truncate_r1g1b1_op_type;
	typedef BPT::TCheapQuantizeOP<pixel_type, 0x80c080> truncate_r1g2b1_op_type;
	typedef BPT::TCheapQuantizeOP<pixel_type, 0x80c0c0> truncate_r1g2b2_op_type;

	typedef BPT::TCheapQuantizeOP<pixel_type, 0xc0c0c0> truncate_r2g2b2_op_type;
	typedef BPT::TCheapQuantizeOP<pixel_type, 0xc0e0c0> truncate_r2g3b2_op_type;
	typedef BPT::TCheapQuantizeOP<pixel_type, 0xc0e0e0> truncate_r2g3b3_op_type;

	typedef BPT::TCheapQuantizeOP<pixel_type, 0xe0e0e0> truncate_r3g3b3_op_type;
	typedef BPT::TCheapQuantizeOP<pixel_type, 0xe0efe0> truncate_r3g4b3_op_type;
	typedef BPT::TCheapQuantizeOP<pixel_type, 0xe0efef> truncate_r3g4b4_op_type;

	typedef BPT::TCheapQuantizeOP<pixel_type, 0xf0f0f0> truncate_r4g4b4_op_type;
	typedef BPT::TCheapQuantizeOP<pixel_type, 0xf8f8f8> truncate_r5g5b5_op_type;
	typedef BPT::TCheapQuantizeOP<pixel_type, 0xf8fcf8> truncate_r5g6b5_op_type;

	typedef BPT::TCheapQuantizeOP<pixel_type, 0xfcfcfc> truncate_r6g6b6_op_type;

	typedef BPT::TQuantizeAsOP< pixel_type, BPT::PIXEL_R1G1B1 > promote_r1g1b1_op_type;
	typedef BPT::TQuantizeAsOP< pixel_type, BPT::PIXEL_R1G2B1 > promote_r1g2b1_op_type;
	typedef BPT::TQuantizeAsOP< pixel_type, BPT::PIXEL_R1G2B2 > promote_r1g2b2_op_type;

	typedef BPT::TQuantizeAsOP< pixel_type, BPT::PIXEL_R2G2B2 > promote_r2g2b2_op_type;
	typedef BPT::TQuantizeAsOP< pixel_type, BPT::PIXEL_R2G3B2 > promote_r2g3b2_op_type;
	typedef BPT::TQuantizeAsOP< pixel_type, BPT::PIXEL_R2G3B3 > promote_r2g3b3_op_type;

	typedef BPT::TQuantizeAsOP< pixel_type, BPT::PIXEL_R3G3B3 > promote_r3g3b3_op_type;
	typedef BPT::TQuantizeAsOP< pixel_type, BPT::PIXEL_R3G4B3 > promote_r3g4b3_op_type;
	typedef BPT::TQuantizeAsOP< pixel_type, BPT::PIXEL_R3G4B4 > promote_r3g4b4_op_type;

	typedef BPT::TQuantizeAsOP< pixel_type, BPT::PIXEL_R4G4B4 > promote_r4g4b4_op_type;
	typedef BPT::TQuantizeAsOP< pixel_type, BPT::PIXEL_R5G5B5 > promote_r5g5b5_op_type;
	typedef BPT::TQuantizeAsOP< pixel_type, BPT::PIXEL_R5G6B5 > promote_r5g6b5_op_type;
	typedef BPT::TQuantizeAsOP< pixel_type, BPT::PIXEL_R6G6B6 > promote_r6g6b6_op_type;

	// ------------------------------------------------------------------------

protected: // data

	// ------------------------------------------------------------------------

	bool m_bAutoFocus;
	CSize m_CanvasSize;
	dib_type m_RawDib;
	dib_type m_DitherDib;
	bitmap_type * m_pLoadedImage;
	int m_nDitherMode;
	bool m_bMatchDitherDepth;
	bool m_bTruncateMatchDitherMode;
	bool m_bVerticalMode;
	CPoint m_ButtonDownPos;
	CPoint m_ViewScrollOrigin;
	bool m_bDragMode;
	HCURSOR m_hDragMoveCursor;
	HCURSOR m_hCenterOthersCursor;
	bool m_bCenterOthersMode;
	enum BPT::DITHER_STYLE m_DitherStyle;

	bool m_bSpreadOutModeActive;
	bool m_bInterleaveModeActive;
	bool m_bXYZSortAfterLoad;
	bool m_bKeepIsolatedColorsInPlace;

	// ------------------------------------------------------------------------

	bool m_bUseNoise;
	int m_nNoiseModulo;
	bool m_bUseReset;
	int m_nResetModulo;
	bool m_bSerpentine;
	bool m_bForceDirection;

	// ------------------------------------------------------------------------

	int m_nNoiseLevel1;
	int m_nNoiseLevel2;
	int m_nNoiseLevel3;
	int m_nNoiseLevel4;

	// ------------------------------------------------------------------------

	int m_nResetLevel1;
	int m_nResetLevel2;
	int m_nResetLevel3;
	int m_nResetLevel4;

	// ------------------------------------------------------------------------

protected: // create from serialization only

	CDitherStudioDoc();

	DECLARE_DYNCREATE(CDitherStudioDoc)

	// --------------------------------------------------------------------

	bitmap_type * LoadFromHBitmap(HBITMAP hBitmap);
	bitmap_type * LoadImageSurface( const char * filename );

	void ChangeDocumentToSurface(bitmap_type* pSurface, bool bClearSpreadMode);
	void ProcessHBITMAPAsNewDocument(HBITMAP hBitmap);

	bitmap_type * LoadBinaryDataAsSurface(const char* filename);

	// --------------------------------------------------------------------

	void DitherIntoBitmapPrim(
		bitmap_type * pDest, bitmap_type * pSource, const int ditherMode
	);

	void DitherIntoBitmapDispatch(
		bitmap_type * pDest, bitmap_type * pSource, const int ditherMode
	);

	// --------------------------------------------------------------------

	void RebuildDibs();
	void RebuildRawDib();
	void RebuildDitherDib();

	void SmartSetDitherMode( const int nMode );
	void SmartSetMatchMode( const bool bMatchDitherDepth );
	void SmartSetTruncateMode(const bool bTruncateMode);
	void SmartSetNoiseLevel( const int nNoiseModulo );
	void SmartSetResetLevel( const int nResetModulo );

	void SmartSetDitherStyle( const BPT::DITHER_STYLE style );

	// --------------------------------------------------------------------

	bool CreateWindowsForLayout_0();
	bool CreateWindowsForLayout_1();
	bool CreateWindowsForLayout_2();

	bool PerformDefaultLayout( const int nLayout );

	// --------------------------------------------------------------------

	COLORREF fHSVTest( float h0t1, float s0t1, float v0t2, float c = 255.0f );

	bool BuildDefaultImage();

	void ScaleLoadedImage(int scale);

	void PerformInterleave();
	void PerformDeinterleave();

	// --------------------------------------------------------------------

public:

	// --------------------------------------------------------------------

	void ForceFullRedraw();

	// --------------------------------------------------------------------

	HCURSOR GetViewCursor( CView * pView, const CPoint point, const UINT nHitTest );

	// --------------------------------------------------------------------

	void HandleAutoFocus( CView * pView );
	void MassageEditMessageInfo( CView * pView, UINT & nFlags, CPoint & point);
	void Dispatch_OnLButtonDown(CView * pView, UINT nFlags, CPoint point);
	void Dispatch_OnLButtonUp(CView * pView, UINT nFlags, CPoint point);
	void Dispatch_OnMouseMove(CView * pView, UINT nFlags, CPoint point);
	void Dispatch_OnRButtonDown(CView * pView, UINT nFlags, CPoint point);
	void Dispatch_OnRButtonUp(CView * pView, UINT nFlags, CPoint point);
	void Dispatch_OnMButtonDown(CView * pView, UINT nFlags, CPoint point);
	void Dispatch_OnMButtonUp(CView * pView, UINT nFlags, CPoint point);
	void Dispatch_OnCancelMode(CView * pView);

	// --------------------------------------------------------------------

	dib_type * GetRawViewDib();
	dib_type * GetDitheredViewDib();

	// --------------------------------------------------------------------

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDitherStudioDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDitherStudioDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDitherStudioDoc)
	afx_msg void OnLoadImage();
	afx_msg void OnAutoFocus();
	afx_msg void OnUpdateAutoFocus(CCmdUI* pCmdUI);
	afx_msg void OnDefaultLayout0();
	afx_msg void OnDefaultLayout1();
	afx_msg void OnR1g1b1();
	afx_msg void OnUpdateR1g1b1(CCmdUI* pCmdUI);
	afx_msg void OnR1g2b1();
	afx_msg void OnUpdateR1g2b1(CCmdUI* pCmdUI);
	afx_msg void OnR1g2b2();
	afx_msg void OnUpdateR1g2b2(CCmdUI* pCmdUI);
	afx_msg void OnR2g2b2();
	afx_msg void OnUpdateR2g2b2(CCmdUI* pCmdUI);
	afx_msg void OnR2g3b2();
	afx_msg void OnUpdateR2g3b2(CCmdUI* pCmdUI);
	afx_msg void OnR2g3b3();
	afx_msg void OnUpdateR2g3b3(CCmdUI* pCmdUI);
	afx_msg void OnR4g4b4();
	afx_msg void OnUpdateR4g4b4(CCmdUI* pCmdUI);
	afx_msg void OnR5g5b5();
	afx_msg void OnUpdateR5g5b5(CCmdUI* pCmdUI);
	afx_msg void OnR5g6b5();
	afx_msg void OnUpdateR5g6b5(CCmdUI* pCmdUI);
	afx_msg void OnR3g3b3();
	afx_msg void OnUpdateR3g3b3(CCmdUI* pCmdUI);
	afx_msg void OnR3g4b3();
	afx_msg void OnUpdateR3g4b3(CCmdUI* pCmdUI);
	afx_msg void OnR3g4b4();
	afx_msg void OnUpdateR3g4b4(CCmdUI* pCmdUI);
	afx_msg void OnMatchDepth();
	afx_msg void OnUpdateMatchDepth(CCmdUI* pCmdUI);
	afx_msg void OnTruncateMatchMode();
	afx_msg void OnUpdateTruncateMatchMode(CCmdUI* pCmdUI);
	afx_msg void OnNoiseLevel1();
	afx_msg void OnUpdateNoiseLevel1(CCmdUI* pCmdUI);
	afx_msg void OnNoiseLevel2();
	afx_msg void OnUpdateNoiseLevel2(CCmdUI* pCmdUI);
	afx_msg void OnNoiseLevel3();
	afx_msg void OnUpdateNoiseLevel3(CCmdUI* pCmdUI);
	afx_msg void OnNoiseLevel4();
	afx_msg void OnUpdateNoiseLevel4(CCmdUI* pCmdUI);
	afx_msg void OnUseNoise();
	afx_msg void OnUpdateUseNoise(CCmdUI* pCmdUI);
	afx_msg void OnResetLevel1();
	afx_msg void OnUpdateResetLevel1(CCmdUI* pCmdUI);
	afx_msg void OnResetLevel2();
	afx_msg void OnUpdateResetLevel2(CCmdUI* pCmdUI);
	afx_msg void OnResetLevel3();
	afx_msg void OnUpdateResetLevel3(CCmdUI* pCmdUI);
	afx_msg void OnResetLevel4();
	afx_msg void OnUpdateResetLevel4(CCmdUI* pCmdUI);
	afx_msg void OnUseReset();
	afx_msg void OnUpdateUseReset(CCmdUI* pCmdUI);
	afx_msg void OnVerticalMode();
	afx_msg void OnUpdateVerticalMode(CCmdUI* pCmdUI);
	afx_msg void OnSerpentineMode();
	afx_msg void OnUpdateSerpentineMode(CCmdUI* pCmdUI);
	afx_msg void OnForceLeft();
	afx_msg void OnUpdateForceLeft(CCmdUI* pCmdUI);
	afx_msg void OnStyleFloydSteinberg();
	afx_msg void OnUpdateStyleFloydSteinberg(CCmdUI* pCmdUI);
	afx_msg void OnStyleJarvis();
	afx_msg void OnUpdateStyleJarvis(CCmdUI* pCmdUI);
	afx_msg void OnStyleStucki();
	afx_msg void OnUpdateStyleStucki(CCmdUI* pCmdUI);
	afx_msg void OnStyleStevenson();
	afx_msg void OnUpdateStyleStevenson(CCmdUI* pCmdUI);
	afx_msg void OnStyleBurkes();
	afx_msg void OnUpdateStyleBurkes(CCmdUI* pCmdUI);
	afx_msg void OnStyleOther1();
	afx_msg void OnUpdateStyleOther1(CCmdUI* pCmdUI);
	afx_msg void OnStyleSierra();
	afx_msg void OnUpdateStyleSierra(CCmdUI* pCmdUI);
	afx_msg void OnOrdered2x2();
	afx_msg void OnUpdateOrdered2x2(CCmdUI* pCmdUI);
	afx_msg void OnOrdered4x4();
	afx_msg void OnUpdateOrdered4x4(CCmdUI* pCmdUI);
	afx_msg void OnOrdered8x8();
	afx_msg void OnUpdateOrdered8x8(CCmdUI* pCmdUI);
	afx_msg void OnOrdered16x16();
	afx_msg void OnUpdateOrdered16x16(CCmdUI* pCmdUI);
	afx_msg void OnDefaultLayout2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	void ActuallyProcessLoadRequest(LPCTSTR lpszPathName);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnFileSaveAs();
	afx_msg void OnSwapRAndG();
	afx_msg void OnUpdateSwapRAndG(CCmdUI* pCmdUI);
	afx_msg void OnSwapRAndB();
	afx_msg void OnUpdateSwapRAndB(CCmdUI* pCmdUI);
	afx_msg void OnInvertColors();
	afx_msg void OnUpdateInvertColors(CCmdUI* pCmdUI);
	afx_msg void OnRotateColorChanels();
	afx_msg void OnUpdateRotateColorChanels(CCmdUI* pCmdUI);
	afx_msg void OnSpreadOutChannels();
	afx_msg void OnUpdateSpreadOutChannels(CCmdUI* pCmdUI);
	afx_msg void OnRotate90();
	afx_msg void OnUpdateRotate90(CCmdUI* pCmdUI);
	afx_msg void OnFlipHorizontal();
	afx_msg void OnUpdateFlipHorizontal(CCmdUI* pCmdUI);
	afx_msg void OnFlipVertical();
	afx_msg void OnUpdateFlipVertical(CCmdUI* pCmdUI);
	afx_msg void On3x3PixelNeighborhood();
	afx_msg void OnUpdate3x3PixelNeighborhood(CCmdUI* pCmdUI);
	afx_msg void OnScale2x();
	afx_msg void OnUpdateScale2x(CCmdUI* pCmdUI);
	afx_msg void OnScale3x();
	afx_msg void OnUpdateScale3x(CCmdUI* pCmdUI);
	afx_msg void OnR6g6b6();
	afx_msg void OnUpdateR6g6b6(CCmdUI* pCmdUI);
	afx_msg void OnInterleaveChannels();
	afx_msg void OnUpdateInterleaveChannels(CCmdUI* pCmdUI);
	afx_msg void On2dPaletteMode();
	afx_msg void OnUpdate2dPaletteMode(CCmdUI* pCmdUI);
	afx_msg void On3dPaletteMode();
	afx_msg void OnUpdate3dPaletteMode(CCmdUI* pCmdUI);
	afx_msg void OnNoDithering();
	afx_msg void OnUpdateNoDithering(CCmdUI* pCmdUI);
	afx_msg void OnCaptureDitherResultAsDoc();
	afx_msg void OnUpdateCaptureDitherResultAsDoc(CCmdUI* pCmdUI);
	afx_msg void OnOperationA();
	afx_msg void OnUpdateOperationA(CCmdUI* pCmdUI);
	afx_msg void OnOperationB();
	afx_msg void OnUpdateOperationB(CCmdUI* pCmdUI);
	afx_msg void OnGrayscale();
	afx_msg void OnUpdateGrayscale(CCmdUI* pCmdUI);
	afx_msg void OnDesaturate();
	afx_msg void OnUpdateDesaturate(CCmdUI* pCmdUI);
	afx_msg void OnR8g8b8();
	afx_msg void OnUpdateR8g8b8(CCmdUI* pCmdUI);
	afx_msg void OnIsolateUniqueColors();
	afx_msg void OnUpdateIsolateUniqueColors(CCmdUI* pCmdUI);
	afx_msg void OnMiscXyzSortAfterLoad();
	afx_msg void OnUpdateMiscXyzSortAfterLoad(CCmdUI* pCmdUI);
	afx_msg void OnMiscOriginalInverseDifference();
	afx_msg void OnUpdateMiscOriginalInverseDifference(CCmdUI* pCmdUI);
	afx_msg void OnMiscSplitbitsintostreams();
	afx_msg void OnUpdateMiscSplitbitsintostreams(CCmdUI* pCmdUI);
	afx_msg void OnMiscShoworiginalpositionsofunique();
	afx_msg void OnUpdateMiscShoworiginalpositionsofunique(CCmdUI* pCmdUI);
	afx_msg void OnCropHalfHorizontal();
	afx_msg void OnUpdateCropHalfHorizontal(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMiscSortbasedoncoloraddress(CCmdUI* pCmdUI);
	afx_msg void OnMiscSortbasedoncoloraddress();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DITHERSTUDIODOC_H__AAF20D8B_284A_4328_8B92_09B06ECFDAA7__INCLUDED_)
