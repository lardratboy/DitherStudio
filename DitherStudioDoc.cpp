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
#include "DitherStudioView.h"
//#include "ExternalSource\IJLBitmap.H"
#include "VariousOperationsThatNeedAHome.h"
#include "CHistogramVizDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

#include <list>
#include <string>
#include <utility>

namespace BPT {

	class CSimpleXMLParse {

	public: // traits

		typedef std::string node_name_type;
		typedef std::string node_value_type;

		struct node_data_type {

			typedef std::list<class node_info_type *> collection_type;

			collection_type m_SubTags;
			collection_type m_Attributes;
			node_value_type m_Value;

		};

		typedef std::pair< node_name_type, node_data_type > node_info_type;

	private: // data

	public: // interface

		template< class IN_STREAM >
		bool ParseFrom( IN_STREAM & inStream ) {

			// fun!

			return true;

		}

		template< class OUT_STREAM >
		bool OutputTo( OUT_STREAM & outStream ) {

			// fun

			return true;

		}

	};

}; // namespace BPTXML

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------


/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC( CDocRectChange, CObject )

/////////////////////////////////////////////////////////////////////////////
// CDitherStudioDoc

IMPLEMENT_DYNCREATE(CDitherStudioDoc, CDocument)

BEGIN_MESSAGE_MAP(CDitherStudioDoc, CDocument)
	//{{AFX_MSG_MAP(CDitherStudioDoc)
	ON_COMMAND(ID_LOAD_IMAGE, OnLoadImage)
	ON_COMMAND(ID_AUTO_FOCUS, OnAutoFocus)
	ON_UPDATE_COMMAND_UI(ID_AUTO_FOCUS, OnUpdateAutoFocus)
	ON_COMMAND(ID_DEFAULT_LAYOUT_0, OnDefaultLayout0)
	ON_COMMAND(ID_DEFAULT_LAYOUT_1, OnDefaultLayout1)
	ON_COMMAND(ID_R1G1B1, OnR1g1b1)
	ON_UPDATE_COMMAND_UI(ID_R1G1B1, OnUpdateR1g1b1)
	ON_COMMAND(ID_R1G2B1, OnR1g2b1)
	ON_UPDATE_COMMAND_UI(ID_R1G2B1, OnUpdateR1g2b1)
	ON_COMMAND(ID_R1G2B2, OnR1g2b2)
	ON_UPDATE_COMMAND_UI(ID_R1G2B2, OnUpdateR1g2b2)
	ON_COMMAND(ID_R2G2B2, OnR2g2b2)
	ON_UPDATE_COMMAND_UI(ID_R2G2B2, OnUpdateR2g2b2)
	ON_COMMAND(ID_R2G3B2, OnR2g3b2)
	ON_UPDATE_COMMAND_UI(ID_R2G3B2, OnUpdateR2g3b2)
	ON_COMMAND(ID_R2G3B3, OnR2g3b3)
	ON_UPDATE_COMMAND_UI(ID_R2G3B3, OnUpdateR2g3b3)
	ON_COMMAND(ID_R3G3B3, OnR3g3b3)
	ON_UPDATE_COMMAND_UI(ID_R3G3B3, OnUpdateR3g3b3)
	ON_COMMAND(ID_R3G4B3, OnR3g4b3)
	ON_UPDATE_COMMAND_UI(ID_R3G4B3, OnUpdateR3g4b3)
	ON_COMMAND(ID_R3G4B4, OnR3g4b4)
	ON_UPDATE_COMMAND_UI(ID_R3G4B4, OnUpdateR3g4b4)
	ON_COMMAND(ID_R4G4B4, OnR4g4b4)
	ON_UPDATE_COMMAND_UI(ID_R4G4B4, OnUpdateR4g4b4)
	ON_COMMAND(ID_R5G5B5, OnR5g5b5)
	ON_UPDATE_COMMAND_UI(ID_R5G5B5, OnUpdateR5g5b5)
	ON_COMMAND(ID_R5G6B5, OnR5g6b5)
	ON_UPDATE_COMMAND_UI(ID_R5G6B5, OnUpdateR5g6b5)
	ON_COMMAND(ID_MATCH_DEPTH, OnMatchDepth)
	ON_UPDATE_COMMAND_UI(ID_MATCH_DEPTH, OnUpdateMatchDepth)
	ON_COMMAND(ID_TRUNCATE_RGB_MODE, OnTruncateMatchMode)
	ON_UPDATE_COMMAND_UI(ID_TRUNCATE_RGB_MODE, OnUpdateTruncateMatchMode)
	ON_COMMAND(ID_NOISE_LEVEL_1, OnNoiseLevel1)
	ON_UPDATE_COMMAND_UI(ID_NOISE_LEVEL_1, OnUpdateNoiseLevel1)
	ON_COMMAND(ID_NOISE_LEVEL_2, OnNoiseLevel2)
	ON_UPDATE_COMMAND_UI(ID_NOISE_LEVEL_2, OnUpdateNoiseLevel2)
	ON_COMMAND(ID_NOISE_LEVEL_3, OnNoiseLevel3)
	ON_UPDATE_COMMAND_UI(ID_NOISE_LEVEL_3, OnUpdateNoiseLevel3)
	ON_COMMAND(ID_NOISE_LEVEL_4, OnNoiseLevel4)
	ON_UPDATE_COMMAND_UI(ID_NOISE_LEVEL_4, OnUpdateNoiseLevel4)
	ON_COMMAND(ID_USE_NOISE, OnUseNoise)
	ON_UPDATE_COMMAND_UI(ID_USE_NOISE, OnUpdateUseNoise)
	ON_COMMAND(ID_RESET_LEVEL_1, OnResetLevel1)
	ON_UPDATE_COMMAND_UI(ID_RESET_LEVEL_1, OnUpdateResetLevel1)
	ON_COMMAND(ID_RESET_LEVEL_2, OnResetLevel2)
	ON_UPDATE_COMMAND_UI(ID_RESET_LEVEL_2, OnUpdateResetLevel2)
	ON_COMMAND(ID_RESET_LEVEL_3, OnResetLevel3)
	ON_UPDATE_COMMAND_UI(ID_RESET_LEVEL_3, OnUpdateResetLevel3)
	ON_COMMAND(ID_RESET_LEVEL_4, OnResetLevel4)
	ON_UPDATE_COMMAND_UI(ID_RESET_LEVEL_4, OnUpdateResetLevel4)
	ON_COMMAND(ID_USE_RESET, OnUseReset)
	ON_UPDATE_COMMAND_UI(ID_USE_RESET, OnUpdateUseReset)
	ON_COMMAND(ID_VERTICAL_MODE, OnVerticalMode)
	ON_UPDATE_COMMAND_UI(ID_VERTICAL_MODE, OnUpdateVerticalMode)
	ON_COMMAND(ID_SERPENTINE_MODE, OnSerpentineMode)
	ON_UPDATE_COMMAND_UI(ID_SERPENTINE_MODE, OnUpdateSerpentineMode)
	ON_COMMAND(ID_FORCE_LEFT, OnForceLeft)
	ON_UPDATE_COMMAND_UI(ID_FORCE_LEFT, OnUpdateForceLeft)
	ON_COMMAND(ID_STYLE_FLOYD_STEINBERG, OnStyleFloydSteinberg)
	ON_UPDATE_COMMAND_UI(ID_STYLE_FLOYD_STEINBERG, OnUpdateStyleFloydSteinberg)
	ON_COMMAND(ID_STYLE_JARVIS, OnStyleJarvis)
	ON_UPDATE_COMMAND_UI(ID_STYLE_JARVIS, OnUpdateStyleJarvis)
	ON_COMMAND(ID_STYLE_STUCKI, OnStyleStucki)
	ON_UPDATE_COMMAND_UI(ID_STYLE_STUCKI, OnUpdateStyleStucki)
	ON_COMMAND(ID_STYLE_STEVENSON, OnStyleStevenson)
	ON_UPDATE_COMMAND_UI(ID_STYLE_STEVENSON, OnUpdateStyleStevenson)
	ON_COMMAND(ID_STYLE_BURKES, OnStyleBurkes)
	ON_UPDATE_COMMAND_UI(ID_STYLE_BURKES, OnUpdateStyleBurkes)
	ON_COMMAND(ID_STYLE_OTHER_1, OnStyleOther1)
	ON_UPDATE_COMMAND_UI(ID_STYLE_OTHER_1, OnUpdateStyleOther1)
	ON_COMMAND(ID_STYLE_SIERRA, OnStyleSierra)
	ON_UPDATE_COMMAND_UI(ID_STYLE_SIERRA, OnUpdateStyleSierra)
	ON_COMMAND(ID_ORDERED_2x2, OnOrdered2x2)
	ON_UPDATE_COMMAND_UI(ID_ORDERED_2x2, OnUpdateOrdered2x2)
	ON_COMMAND(ID_ORDERED_4x4, OnOrdered4x4)
	ON_UPDATE_COMMAND_UI(ID_ORDERED_4x4, OnUpdateOrdered4x4)
	ON_COMMAND(ID_ORDERED_8x8, OnOrdered8x8)
	ON_UPDATE_COMMAND_UI(ID_ORDERED_8x8, OnUpdateOrdered8x8)
	ON_COMMAND(ID_ORDERED_16x16, OnOrdered16x16)
	ON_UPDATE_COMMAND_UI(ID_ORDERED_16x16, OnUpdateOrdered16x16)
	ON_COMMAND(ID_DEFAULT_LAYOUT_2, OnDefaultLayout2)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_EDIT_COPY, &CDitherStudioDoc::OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, &CDitherStudioDoc::OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, &CDitherStudioDoc::OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CDitherStudioDoc::OnUpdateEditPaste)
	ON_COMMAND(ID_FILE_SAVE_AS, &CDitherStudioDoc::OnFileSaveAs)
	ON_COMMAND(ID_SWAP_R_AND_G, &CDitherStudioDoc::OnSwapRAndG)
	ON_UPDATE_COMMAND_UI(ID_SWAP_R_AND_G, &CDitherStudioDoc::OnUpdateSwapRAndG)
	ON_COMMAND(ID_SWAP_R_AND_B, &CDitherStudioDoc::OnSwapRAndB)
	ON_UPDATE_COMMAND_UI(ID_SWAP_R_AND_B, &CDitherStudioDoc::OnUpdateSwapRAndB)
	ON_COMMAND(ID_INVERT_COLORS, &CDitherStudioDoc::OnInvertColors)
	ON_UPDATE_COMMAND_UI(ID_INVERT_COLORS, &CDitherStudioDoc::OnUpdateInvertColors)
	ON_COMMAND(ID_ROTATE_COLOR_CHANELS, &CDitherStudioDoc::OnRotateColorChanels)
	ON_UPDATE_COMMAND_UI(ID_ROTATE_COLOR_CHANELS, &CDitherStudioDoc::OnUpdateRotateColorChanels)
	ON_COMMAND(ID_SPREAD_OUT_CHANNELS, &CDitherStudioDoc::OnSpreadOutChannels)
	ON_UPDATE_COMMAND_UI(ID_SPREAD_OUT_CHANNELS, &CDitherStudioDoc::OnUpdateSpreadOutChannels)
	ON_COMMAND(ID_ROTATE_90, &CDitherStudioDoc::OnRotate90)
	ON_UPDATE_COMMAND_UI(ID_ROTATE_90, &CDitherStudioDoc::OnUpdateRotate90)
	ON_COMMAND(ID_FLIP_HORIZONTAL, &CDitherStudioDoc::OnFlipHorizontal)
	ON_UPDATE_COMMAND_UI(ID_FLIP_HORIZONTAL, &CDitherStudioDoc::OnUpdateFlipHorizontal)
	ON_COMMAND(ID_FLIP_VERTICAL, &CDitherStudioDoc::OnFlipVertical)
	ON_UPDATE_COMMAND_UI(ID_FLIP_VERTICAL, &CDitherStudioDoc::OnUpdateFlipVertical)
	ON_COMMAND(ID_3X3_PIXEL_NEIGHBORHOOD, &CDitherStudioDoc::On3x3PixelNeighborhood)
	ON_UPDATE_COMMAND_UI(ID_3X3_PIXEL_NEIGHBORHOOD, &CDitherStudioDoc::OnUpdate3x3PixelNeighborhood)
	ON_COMMAND(ID_SCALE_2X, &CDitherStudioDoc::OnScale2x)
	ON_UPDATE_COMMAND_UI(ID_SCALE_2X, &CDitherStudioDoc::OnUpdateScale2x)
	ON_COMMAND(ID_SCALE_3X, &CDitherStudioDoc::OnScale3x)
	ON_UPDATE_COMMAND_UI(ID_SCALE_3X, &CDitherStudioDoc::OnUpdateScale3x)
	ON_COMMAND(ID_R6G6B6, &CDitherStudioDoc::OnR6g6b6)
	ON_UPDATE_COMMAND_UI(ID_R6G6B6, &CDitherStudioDoc::OnUpdateR6g6b6)
	ON_COMMAND(ID_INTERLEAVE_CHANNELS, &CDitherStudioDoc::OnInterleaveChannels)
	ON_UPDATE_COMMAND_UI(ID_INTERLEAVE_CHANNELS, &CDitherStudioDoc::OnUpdateInterleaveChannels)
	ON_COMMAND(ID_OPERATION_A, &CDitherStudioDoc::OnOperationA)
	ON_UPDATE_COMMAND_UI(ID_OPERATION_A, &CDitherStudioDoc::OnUpdateOperationA)
	ON_COMMAND(ID_2D_PALETTE_MODE, &CDitherStudioDoc::On2dPaletteMode)
	ON_UPDATE_COMMAND_UI(ID_2D_PALETTE_MODE, &CDitherStudioDoc::OnUpdate2dPaletteMode)
	ON_COMMAND(ID_3D_PALETTE_MODE, &CDitherStudioDoc::On3dPaletteMode)
	ON_UPDATE_COMMAND_UI(ID_3D_PALETTE_MODE, &CDitherStudioDoc::OnUpdate3dPaletteMode)
	ON_COMMAND(ID_NO_DITHERING, &CDitherStudioDoc::OnNoDithering)
	ON_UPDATE_COMMAND_UI(ID_NO_DITHERING, &CDitherStudioDoc::OnUpdateNoDithering)
	ON_COMMAND(ID_OPERATION_B, &CDitherStudioDoc::OnOperationB)
	ON_UPDATE_COMMAND_UI(ID_OPERATION_B, &CDitherStudioDoc::OnUpdateOperationB)
	ON_COMMAND(ID_CAPTURE_DITHER_RESULT_AS_DOC, &CDitherStudioDoc::OnCaptureDitherResultAsDoc)
	ON_UPDATE_COMMAND_UI(ID_CAPTURE_DITHER_RESULT_AS_DOC, &CDitherStudioDoc::OnUpdateCaptureDitherResultAsDoc)
	ON_COMMAND(ID_GRAYSCALE, &CDitherStudioDoc::OnGrayscale)
	ON_UPDATE_COMMAND_UI(ID_GRAYSCALE, &CDitherStudioDoc::OnUpdateGrayscale)
	ON_COMMAND(ID_DESATURATE, &CDitherStudioDoc::OnDesaturate)
	ON_UPDATE_COMMAND_UI(ID_DESATURATE, &CDitherStudioDoc::OnUpdateDesaturate)
	ON_COMMAND(ID_R8G8B8, &CDitherStudioDoc::OnR8g8b8)
	ON_UPDATE_COMMAND_UI(ID_R8G8B8, &CDitherStudioDoc::OnUpdateR8g8b8)
	ON_COMMAND(ID_ISOLATE_UNIQUE_COLORS, &CDitherStudioDoc::OnIsolateUniqueColors)
	ON_UPDATE_COMMAND_UI(ID_ISOLATE_UNIQUE_COLORS, &CDitherStudioDoc::OnUpdateIsolateUniqueColors)
	ON_COMMAND(ID_MISC_XYZ_SORT_AFTER_LOAD, &CDitherStudioDoc::OnMiscXyzSortAfterLoad)
	ON_UPDATE_COMMAND_UI(ID_MISC_XYZ_SORT_AFTER_LOAD, &CDitherStudioDoc::OnUpdateMiscXyzSortAfterLoad)
	ON_COMMAND(ID_MISC_ORIGINAL_INVERSE_DIFFERENCE, &CDitherStudioDoc::OnMiscOriginalInverseDifference)
	ON_UPDATE_COMMAND_UI(ID_MISC_ORIGINAL_INVERSE_DIFFERENCE, &CDitherStudioDoc::OnUpdateMiscOriginalInverseDifference)
	ON_COMMAND(ID_MISC_SPLITBITSINTOSTREAMS, &CDitherStudioDoc::OnMiscSplitbitsintostreams)
	ON_UPDATE_COMMAND_UI(ID_MISC_SPLITBITSINTOSTREAMS, &CDitherStudioDoc::OnUpdateMiscSplitbitsintostreams)
	ON_COMMAND(ID_MISC_SHOWORIGINALPOSITIONSOFUNIQUE, &CDitherStudioDoc::OnMiscShoworiginalpositionsofunique)
	ON_UPDATE_COMMAND_UI(ID_MISC_SHOWORIGINALPOSITIONSOFUNIQUE, &CDitherStudioDoc::OnUpdateMiscShoworiginalpositionsofunique)
		ON_COMMAND(ID_CROP_HALF_HORIZONTAL, &CDitherStudioDoc::OnCropHalfHorizontal)
		ON_UPDATE_COMMAND_UI(ID_CROP_HALF_HORIZONTAL, &CDitherStudioDoc::OnUpdateCropHalfHorizontal)
		ON_UPDATE_COMMAND_UI(ID_MISC_SORTBASEDONCOLORADDRESS, &CDitherStudioDoc::OnUpdateMiscSortbasedoncoloraddress)
		ON_COMMAND(ID_MISC_SORTBASEDONCOLORADDRESS, &CDitherStudioDoc::OnMiscSortbasedoncoloraddress)
		END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDitherStudioDoc construction/destruction

CDitherStudioDoc::CDitherStudioDoc() : 
	m_CanvasSize(0,0)
	,m_pLoadedImage(0)
	,m_bAutoFocus(false)
	,m_nDitherMode(ID_R5G5B5)
	,m_bMatchDitherDepth(true)
	,m_bTruncateMatchDitherMode(false)
	,m_bUseNoise(false)
	,m_nNoiseModulo(8)
	,m_bUseReset(false)
	,m_nResetModulo(11)
	,m_bVerticalMode(false)
	,m_bSerpentine(true)
	,m_bForceDirection(true)
	,m_DitherStyle(BPT::ORDERED_4x4) // BPT::FLOYD_STEINBERG)
	,m_ButtonDownPos(0,0)
	,m_bDragMode(false)
	,m_hDragMoveCursor(NULL)
	,m_bCenterOthersMode(false)
	,m_hCenterOthersCursor(NULL)
	,m_bSpreadOutModeActive(false)
	,m_bInterleaveModeActive(false)
	,m_pLastHistogram(nullptr)
	,m_pLastFibSphereViz(nullptr)
	,m_bXYZSortAfterLoad(true)
	,m_bKeepIsolatedColorsInPlace(false)
{
	// ------------------------------------------------------------------------

	m_nNoiseLevel1 = GLOBAL_GetSettingInt( "NoiseLevel1", 2 );
	m_nNoiseLevel2 = GLOBAL_GetSettingInt( "NoiseLevel2", 4 );
	m_nNoiseLevel3 = GLOBAL_GetSettingInt( "NoiseLevel3", 8 );
	m_nNoiseLevel4 = GLOBAL_GetSettingInt( "NoiseLevel4", 16 );
	m_nNoiseModulo = m_nNoiseLevel1;

	// ------------------------------------------------------------------------

#if 1

	m_nResetLevel1 = GLOBAL_GetSettingInt( "ResetLevel1", 63 );
	m_nResetLevel2 = GLOBAL_GetSettingInt( "ResetLevel2", 27 );
	m_nResetLevel3 = GLOBAL_GetSettingInt( "ResetLevel3", 13 );
	m_nResetLevel4 = GLOBAL_GetSettingInt( "ResetLevel4", 4 );

#else

	m_nResetLevel1 = GLOBAL_GetSettingInt( "ResetLevel1", 12 );
	m_nResetLevel2 = GLOBAL_GetSettingInt( "ResetLevel2", 8 );
	m_nResetLevel3 = GLOBAL_GetSettingInt( "ResetLevel3", 6 );
	m_nResetLevel4 = GLOBAL_GetSettingInt( "ResetLevel4", 4 );

#endif

	m_nResetModulo = m_nResetLevel1;

}

CDitherStudioDoc::~CDitherStudioDoc()
{
	GLOBAL_PutSettingInt( "bAutoFocus", m_bAutoFocus ? 1 : 0 );

	if (nullptr != m_pLastHistogram) {
		delete m_pLastHistogram;
		m_pLastHistogram = nullptr;
	}

	if (nullptr != m_pLastFibSphereViz) {
		delete m_pLastFibSphereViz;
		m_pLastFibSphereViz = nullptr;
	}
}

BOOL CDitherStudioDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	// ------------------------------------------------------------------------

	if ( NULL == m_hDragMoveCursor ) {

		m_hDragMoveCursor = AfxGetApp()->LoadCursor( IDC_MOVE_CURSOR );

	}

	if ( NULL == m_hCenterOthersCursor ) {

		m_hCenterOthersCursor = AfxGetApp()->LoadCursor( IDC_CENTER_OTHERS_POINTER );

	}

	m_bAutoFocus = (1 == GLOBAL_GetSettingInt( "bAutoFocus", 0));

#if 1

	BuildDefaultImage();

	RebuildDibs();

#else

	m_CanvasSize.cx = 32;
	m_CanvasSize.cy = 32;

	if ( !m_RawDib.Create( m_CanvasSize ) ) {

		return FALSE;

	}

	if ( !m_DitherDib.Create( m_CanvasSize ) ) {

		return FALSE;

	}

#endif

	// Post a request to do the UI layout (should be read in...)
	// ------------------------------------------------------------------------

	AfxGetMainWnd()->PostMessage( WM_COMMAND, ID_DEFAULT_LAYOUT_0 );
	
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CDitherStudioDoc serialization

void CDitherStudioDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDitherStudioDoc diagnostics

#ifdef _DEBUG
void CDitherStudioDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDitherStudioDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDitherStudioDoc commands

// ============================================================================

//
//	CDitherStudioDoc::ForceFullRedraw()
//

void 
CDitherStudioDoc::ForceFullRedraw()
{
	UpdateAllViews( 
		NULL, CDocRectChange::INVALIDATE_RECT, 
		&CDocRectChange( CRect( CPoint(0,0), m_CanvasSize ) )
	);
}

// ============================================================================

//
//	CDitherStudioDoc::GetViewCursor()
//

HCURSOR 
CDitherStudioDoc::GetViewCursor( CView * pView, const CPoint point, const UINT nHitTest )
{
	if ( m_bDragMode ) {

		return m_hDragMoveCursor;

	} else if ( m_bCenterOthersMode ) {

		return m_hCenterOthersCursor;

	}

	return 0;
}

// ============================================================================

//
//	CDitherStudioDoc::HandleAutoFocus()
//

void CDitherStudioDoc::HandleAutoFocus( CView * pView )
{
	if ( m_bAutoFocus && (NULL == ::GetCapture()) && pView ) {

		if ( pView->GetSafeHwnd() != GetFocus() ) {

			CFrameWnd * pFrame = pView->GetParentFrame();
	
			if ( pFrame ) {
	
				CFrameWnd * ppFrame = pFrame->GetParentFrame();
	
				if ( ppFrame ) {
	
					ppFrame->ActivateFrame();
	
				}
	
				pFrame->ActivateFrame();
				pFrame->SetFocus();
				pFrame->SetActiveView( pView );

				// 'promote' the view title to the frame 

				CString viewTitle;
				pView->GetWindowText( viewTitle );
				pFrame->SetWindowText( viewTitle );
	
			}

		}

	}
}

// ============================================================================

void 
CDitherStudioDoc::MassageEditMessageInfo(
	CView * pView, UINT & nFlags, CPoint & point
){

	HandleAutoFocus( pView );

}

// ============================================================================

void CDitherStudioDoc::Dispatch_OnLButtonDown(CView * pView, UINT nFlags, CPoint point) {

	MassageEditMessageInfo( pView, nFlags, point );

	if ( pView && pView->IsKindOf( RUNTIME_CLASS(CDitherStudioView) ) ) {

 		CRect logicalRect = ((CDitherStudioView*)pView)->GetCurrentLogicalRect();

		m_ViewScrollOrigin.x = logicalRect.left;
		m_ViewScrollOrigin.y = logicalRect.top;

		m_ButtonDownPos.x = point.x - m_ViewScrollOrigin.x;
		m_ButtonDownPos.y = point.y - m_ViewScrollOrigin.y;

		m_bDragMode = true;

		HCURSOR hCrsr = GetViewCursor( pView, point, HTCLIENT );

		if ( hCrsr ) {

			SetCursor( hCrsr );

		}

	}

}

void CDitherStudioDoc::Dispatch_OnLButtonUp(CView * pView, UINT nFlags, CPoint point) {

	MassageEditMessageInfo( pView, nFlags, point );

	m_bDragMode = false;

}


void CDitherStudioDoc::Dispatch_OnMouseMove(CView * pView, UINT nFlags, CPoint point)
{
	
	MassageEditMessageInfo( pView, nFlags, point ); 

	if ( MK_RBUTTON & nFlags ) {

		CRect rect( point, CSize( 1, 1 ) );
		
		CDocRectChange logicalViewRect( rect );
		
		UpdateAllViews( pView, CDocRectChange::CENTER_VIEW_REQUEST, &logicalViewRect );

	} else if ( m_bDragMode ) {

		CRect logicalRect = ((CDitherStudioView*)pView)->GetCurrentLogicalRect();

		CRect rect(
			CPoint( 
				m_ViewScrollOrigin.x - ((point.x - logicalRect.left) - m_ButtonDownPos.x), 
				m_ViewScrollOrigin.y - ((point.y - logicalRect.top ) - m_ButtonDownPos.y)
			),
			CSize(1,1)
		);

		CDocRectChange change( rect, pView );
		
		UpdateAllViews( NULL, CDocRectChange::MOVE_TOP_LEFT_REQUEST, &change );

	}

}

void CDitherStudioDoc::Dispatch_OnRButtonDown(CView * pView, UINT nFlags, CPoint point) {
	
	MassageEditMessageInfo( pView, nFlags, point );

	m_bCenterOthersMode = true;

	HCURSOR hCrsr = GetViewCursor( pView, point, HTCLIENT );

	if ( hCrsr ) {

		SetCursor( hCrsr );

	}
	
}

void CDitherStudioDoc::Dispatch_OnRButtonUp(CView * pView, UINT nFlags, CPoint point) {
	
	MassageEditMessageInfo( pView, nFlags, point ); 

	m_bCenterOthersMode = false;

}

void CDitherStudioDoc::Dispatch_OnMButtonDown(CView * pView, UINT nFlags, CPoint point) { MassageEditMessageInfo( pView, nFlags, point ); }
void CDitherStudioDoc::Dispatch_OnMButtonUp(CView * pView, UINT nFlags, CPoint point) { MassageEditMessageInfo( pView, nFlags, point ); }

void CDitherStudioDoc::Dispatch_OnCancelMode(CView * pView) {

	m_bDragMode = false;
	m_bCenterOthersMode = false;

}

// ============================================================================

//
//	CDitherStudioDoc::GetRawViewDib()
//

CDitherStudioDoc::dib_type * 
CDitherStudioDoc::GetRawViewDib()
{
	return &m_RawDib;
}

//
//	CDitherStudioDoc::GetDitheredViewDib()
//

CDitherStudioDoc::dib_type * 
CDitherStudioDoc::GetDitheredViewDib()
{
	return &m_DitherDib;
}

// ============================================================================

//
//	CDitherStudioDoc::LoadFromHBitmap()
//

CDitherStudioDoc::bitmap_type*
CDitherStudioDoc::LoadFromHBitmap(HBITMAP hBitmap)
{
	CWaitCursor waitCrsr;

#if 1

	if (NULL == hBitmap) return 0;

	CImage im;
	im.Attach(hBitmap);

	dib_type temporaryDibSection;

	int w = im.GetWidth();
	int h = im.GetHeight();

	if (!temporaryDibSection.Create(w,h)) {

		TRACE("Unable to create temporary bitmap %dx%dx%dbpp\n",
			w, h, sizeof(dib_type::bitmap_type::pixel_type) * 8
		);

		return 0;

	}

	BPT::TDibDC<dib_type> dc(&temporaryDibSection);
	im.Draw(dc, 0, 0);

	bitmap_type* pNewImage = new bitmap_type;

	if (!pNewImage) {

		TRACE("Unable to create bitmap object?\n");

		return 0;

	}

	if (!pNewImage->Create(w, h)) {

		TRACE("Unable to create return bitmap %dx%dx%dbpp\n",
			w, h, sizeof(dib_type::bitmap_type::pixel_type) * 8
		);

		delete pNewImage;

		return 0;

	}

	// ----------------------------------------------------------------

	BPT::T_Blit(*pNewImage, 0, 0, *temporaryDibSection.GetBitmapPtr(), copy_op_type());

	// ----------------------------------------------------------------

	return pNewImage;

#else


	CBitmap loader;

	if (!loader.Attach(hBitmap)) {

		TRACE("CBitmap::Attach failed!\n");
		return 0;

	}

	// Get the bitmap info
	// --------------------------------------------------------------------

	BITMAP bmpInfo;

	if (!loader.GetBitmap(&bmpInfo)) {

		TRACE("Unable to get BITMAP info\n");
		return 0;

	}

	CSize fileSize(bmpInfo.bmWidth, bmpInfo.bmHeight);

	// Create and clear the display bitmap
	// --------------------------------------------------------------------

	dib_type temporaryDibSection;

	if (!temporaryDibSection.Create(fileSize.cx, fileSize.cy)) {

		TRACE("Unable to create temporary bitmap %dx%dx%dbpp\n",
			fileSize.cx, fileSize.cy, sizeof(dib_type::bitmap_type::pixel_type) * 8
		);
		return 0;

	}

	dib_type::bitmap_type* pTemporaryBitmap = temporaryDibSection.GetBitmapPtr();

	if (!pTemporaryBitmap) {

		TRACE("Unable to get internal bitmap from dib?\n");
		return 0;

	}

	// Render our loaded image into the dib for later conversion.
	// ------------------------------------------------------------------------

	int bitmapBitsSize = (bmpInfo.bmWidthBytes * bmpInfo.bmHeight);

	std::shared_ptr<BYTE> pData( new BYTE[bitmapBitsSize] );

	if (!pData) {

		TRACE("Unable to allocate working buffer\n");
		return 0;

	}

	// --------------------------------------------------------------------

	int loadedSize = loader.GetBitmapBits(bitmapBitsSize, (LPVOID)pData.get());

	if (bitmapBitsSize < loadedSize) {

		TRACE("Unable to get bitmap bits\n");

		return 0;

	}
	else if (bitmapBitsSize != loadedSize) {

		TRACE("loaded size %d expected %d (difference %d) %dx%dx%dbpp :-/\n",
			loadedSize, bitmapBitsSize, bitmapBitsSize - loadedSize,
			fileSize.cx, fileSize.cy, sizeof(dib_type::bitmap_type::pixel_type) * 8
		);

		// check to see if this is because the pitch was reported correctly
		// but the data was not aligned to that?

	}

	// --------------------------------------------------------------------

	if (24 == bmpInfo.bmBitsPixel) {

		BPT::TSimpleBitmap< BPT::PIXEL_R8G8B8 > mappedDib;

		if (!mappedDib.Map(
			pData.get(), CSize(bmpInfo.bmWidth, bmpInfo.bmHeight),
			bmpInfo.bmWidthBytes, false, true)) {

			TRACE("Unable to map bitmap bits\n");

			return 0;

		}

		// Okay blit this bad boy
		// --------------------------------------------------------------------

		BPT::T_Blit(
			*pTemporaryBitmap, 0, 0, mappedDib,
			BPT::TCopyROP< bitmap_type::pixel_type, BPT::PIXEL_R8G8B8 >()
		);

	} else if ( (32 == bmpInfo.bmBitsPixel) || (8 == bmpInfo.bmBitsPixel) ) {

		BPT::TDibDC<dib_type> dc(&temporaryDibSection);
		CImage im;
		im.Attach((HBITMAP)loader.Detach());
		im.Draw(dc, 0, 0);

	} else {

		TRACE("Unsupported bitdepth %d\n", bmpInfo.bmBitsPixel);
		return 0;

	}

	// Now transfer the contents of the temporary dib to the new bitmap
	// ----------------------------------------------------------------

	bitmap_type* pNewImage = new bitmap_type;

	if (!pNewImage) {

		TRACE("Unable to create bitmap object?\n");

		return 0;

	}

	if (!pNewImage->Create(fileSize.cx, fileSize.cy)) {

		TRACE("Unable to create return bitmap %dx%dx%dbpp\n",
			fileSize.cx, fileSize.cy, sizeof(dib_type::bitmap_type::pixel_type) * 8
		);

		delete pNewImage;

		return 0;

	}

	// ----------------------------------------------------------------

	BPT::T_Blit(*pNewImage, 0, 0, *pTemporaryBitmap, copy_op_type());

	// ----------------------------------------------------------------

	return pNewImage;

#endif

}

//
//	CDitherStudioDoc::LoadImageSurface()
//

CDitherStudioDoc::bitmap_type * 
CDitherStudioDoc::LoadImageSurface( const char * filename )
{
	CWaitCursor waitCrsr;

#if 0
	if (0 != strstr(filename, ".psd")) {

		bitmap_type* pPSDLoader = BPT::THELPER_PSDLoad<bitmap_type>(filename);

		return pPSDLoader;

	}
#endif

	CImage imageLoader;

	HRESULT hr = imageLoader.Load(filename);

	if (FAILED(hr)) {

		TRACE("CImage::Load failed to load file \"%s\"\n", filename);
		return 0;

	}

	HBITMAP hBitmap = imageLoader.Detach();

	if (NULL == hBitmap) {

		TRACE("CBitmap::Detach failed for \"%s\"\n", filename);
		return 0;

	}

	bitmap_type* pSurface = LoadFromHBitmap( hBitmap );

	DeleteObject(hBitmap);

	return pSurface;
}

// ============================================================================

//
//	LoadBinaryDataAsSurface
//

CDitherStudioDoc::bitmap_type* CDitherStudioDoc::LoadBinaryDataAsSurface(const char* filename)
{
	typedef BPT::TSimpleBitmap< BPT::PIXEL_R8G8B8 > r8g8b8_surf_t;
	auto pLoadedR8G8B8 = BPT::TLoadDataAsSurface<r8g8b8_surf_t>(filename);

	bitmap_type* pSurface = new bitmap_type;

	pSurface->Create(pLoadedR8G8B8->Width(), pLoadedR8G8B8->Height());

	BPT::TCopyROP<pixel_type, r8g8b8_surf_t::pixel_type > top;

	BPT::T_Blit(pSurface, 0, 0, pLoadedR8G8B8, top);

	delete pLoadedR8G8B8;

	return pSurface;
}


// ============================================================================

void CDitherStudioDoc::ChangeDocumentToSurface(bitmap_type* pSurface,bool bClearSpreadMode)
{
	CWaitCursor waitCrsr;

	// ------------------------------------------------------------------------

	if (bClearSpreadMode)
	{
		m_bSpreadOutModeActive = false;
		m_bInterleaveModeActive = false;
	}

	if (m_pLoadedImage) {

		// TODO to implment UNDO/REDO need to store off the last instead of deleting it
		// however there are a number of in place changes that need to be tracked for undo/redo
		// would be nice if there was a filename concept as well

		delete m_pLoadedImage;

	}

	m_pLoadedImage = pSurface;

	if (!m_pLoadedImage) {

		BuildDefaultImage();

	}
	else {

		m_CanvasSize = m_pLoadedImage->Size();

		//TRACE("OKAY: Loaded image size %dx%d\n", m_CanvasSize.cx, m_CanvasSize.cy);

	}

	// Need to rebuild the dibs
	// ------------------------------------------------------------------------

	RebuildDibs();

	// Reset the views with the new canvas size & center the views
	// ------------------------------------------------------------------------

	UpdateAllViews(
		NULL, CDocRectChange::NEW_CANVAS_SIZE,
		&CDocRectChange(CRect(CPoint(0, 0), m_CanvasSize))
	);

	UpdateAllViews(
		NULL, CDocRectChange::CENTER_VIEW_REQUEST,
		&CDocRectChange(CRect(CPoint(m_CanvasSize.cx / 2, m_CanvasSize.cy / 2), CSize(1, 1)))
	);

}

void CDitherStudioDoc::ProcessHBITMAPAsNewDocument(HBITMAP hBitmap)
{
	bitmap_type* pSurface = LoadFromHBitmap(hBitmap);

	ChangeDocumentToSurface(pSurface,true);
}

// -------------------------------------------

namespace BPT {


} // namespace BPT

// -------------------------------------------

void CDitherStudioDoc::ActuallyProcessLoadRequest(LPCTSTR lpszPathName)
{
	bitmap_type* pSurface = LoadImageSurface(lpszPathName);

	do {

		if (pSurface) break;

		m_DitherStyle = BPT::NO_DITHERING; // there must be a better way of doing this change
		m_nDitherMode = ID_R8G8B8;

		if (0 != strstr(lpszPathName, ".xyz"))
		{
			typedef std::vector<BPT::glm::vec3> verts_t;
			typedef std::vector<BPT::glm::vec3> color_t;

			verts_t vertices;
			color_t colors;

			if (BPT::TXYZLoadPointCloudInto(lpszPathName, vertices, colors)) {

				ASSERT(vertices.size() == colors.size());

				BPT::TNormalizeToAABBOp<verts_t::value_type>(vertices.begin(), vertices.end());
				BPT::TNormalizeToAABBOp<color_t::value_type>(colors.begin(), colors.end());

				if (m_bXYZSortAfterLoad)
				{
					std::sort(vertices.begin(), vertices.end(), BPT::TSortVec3Predicate());
					std::sort(colors.begin(), colors.end(), BPT::TSortVec3Predicate());
				}

				pSurface = BPT::TVec3CollectionsToDoubleWideSurface<bitmap_type,verts_t,color_t>(vertices, colors);
				if (pSurface) break;

			}
		}

		pSurface = LoadBinaryDataAsSurface(lpszPathName);

	} while (false);

	ChangeDocumentToSurface(pSurface,true);
}


//
//	CDitherStudioDoc::OnLoadImage()
//

void 
CDitherStudioDoc::OnLoadImage() 
{
	CFileDialog opf(
		TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_LONGNAMES, 
		_T(
			"All Supported Formats (*.png,*.jpg,*.tif,*.gif,*bmp)|*.png;*.jpg;*.tif;*.gif;*.bmp|"
			//"Photoshop Files (*.psd)|*.psd|"
			"BMP Files (*.bmp)|*.bmp|"
			"GIF Files (*.gif)|*.gif|"
			"JPG Files (*.jpg)|*.jpg|"
			"PNG Files (*.png)|*.png|"
			"Tiff Files (*.tiff,*.tif)|*.tiff;*.tif|"
			//"BPainT Files (*.bpt)|*.bpt|"
			"All Files (*.*)|*.*|"
			"|"
		)
		,AfxGetMainWnd()
	);

	if ( IDOK != opf.DoModal() ) {

		return;

	}

	ActuallyProcessLoadRequest(opf.GetPathName());// opf.GetFileName());
}

// ============================================================================

	// do something fun here

// ============================================================================

//
//	CDitherStudioDoc::RebuildRawDib()
//

void
CDitherStudioDoc::RebuildRawDib()
{
	CWaitCursor waitCrsr;

	// ------------------------------------------------------------------------

	CSize size = m_RawDib.Size();

	if ( m_CanvasSize != size ) {

		if ( !m_RawDib.Create( m_CanvasSize ) ) {

			TRACE( "m_RawDib failed to create (%dx%d)\n", m_CanvasSize.cx, m_CanvasSize.cy );

			// FATAL ERROR!!!

			return;

		}

	}

	// ----------------------------------------------------------------

	dib_type::bitmap_type * pDibBitmap = m_RawDib.GetBitmapPtr();

	if ( !pDibBitmap ) {

		TRACE( "Unable to get 'RAW' dib internal bitmap?\n" );

		// FATAL ERROR!!!

		return;

	}

	// ----------------------------------------------------------------

	bool bPromoteMode = !m_bTruncateMatchDitherMode;

	if ( m_pLoadedImage ) {

		if ( m_bMatchDitherDepth ) {

			switch ( m_nDitherMode ) {

			default:
			case ID_R8G8B8:
				BPT::T_Blit( *pDibBitmap, 0, 0, *m_pLoadedImage, copy_op_type() );
				break;

			case ID_R1G1B1:
				if (bPromoteMode) {
					BPT::T_Blit(*pDibBitmap, 0, 0, *m_pLoadedImage, promote_r1g1b1_op_type());
				}
				else {
					BPT::T_Blit(*pDibBitmap, 0, 0, *m_pLoadedImage, truncate_r1g1b1_op_type());
				}
				break;

			case ID_R1G2B2:
				if (bPromoteMode) {
					BPT::T_Blit(*pDibBitmap, 0, 0, *m_pLoadedImage, promote_r1g2b2_op_type());
				}
				else {
					BPT::T_Blit(*pDibBitmap, 0, 0, *m_pLoadedImage, truncate_r1g2b2_op_type());
				}
				break;

			case ID_R1G2B1:
				if (bPromoteMode) {
					BPT::T_Blit(*pDibBitmap, 0, 0, *m_pLoadedImage, promote_r1g2b1_op_type());
				}
				else {
					BPT::T_Blit(*pDibBitmap, 0, 0, *m_pLoadedImage, truncate_r1g2b1_op_type());
				}
				break;

			case ID_R2G2B2:
				if (bPromoteMode) {
					BPT::T_Blit(*pDibBitmap, 0, 0, *m_pLoadedImage, promote_r2g2b2_op_type());
				}
				else {
					BPT::T_Blit(*pDibBitmap, 0, 0, *m_pLoadedImage, truncate_r2g2b2_op_type());
				}
				break;

			case ID_R2G3B2:
				if (bPromoteMode) {
					BPT::T_Blit(*pDibBitmap, 0, 0, *m_pLoadedImage, promote_r2g3b2_op_type());
				}
				else {
					BPT::T_Blit(*pDibBitmap, 0, 0, *m_pLoadedImage, truncate_r2g3b2_op_type());
				}
				break;

			case ID_R2G3B3:
				if (bPromoteMode) {
					BPT::T_Blit(*pDibBitmap, 0, 0, *m_pLoadedImage, promote_r2g3b3_op_type());
				}
				else {
					BPT::T_Blit(*pDibBitmap, 0, 0, *m_pLoadedImage, truncate_r2g3b3_op_type());
				}
				break;

			case ID_R3G3B3:
				if (bPromoteMode) {
					BPT::T_Blit(*pDibBitmap, 0, 0, *m_pLoadedImage, promote_r3g3b3_op_type());
				}
				else {
					BPT::T_Blit(*pDibBitmap, 0, 0, *m_pLoadedImage, truncate_r3g3b3_op_type());
				}
				break;

			case ID_R3G4B3:
				if (bPromoteMode) {
					BPT::T_Blit(*pDibBitmap, 0, 0, *m_pLoadedImage, promote_r3g4b3_op_type());
				}
				else {
					BPT::T_Blit(*pDibBitmap, 0, 0, *m_pLoadedImage, truncate_r3g4b3_op_type());
				}
				break;

			case ID_R3G4B4:
				if (bPromoteMode) {
					BPT::T_Blit(*pDibBitmap, 0, 0, *m_pLoadedImage, promote_r3g4b4_op_type());
				}
				else {
					BPT::T_Blit(*pDibBitmap, 0, 0, *m_pLoadedImage, truncate_r3g4b4_op_type());
				}
				break;

			case ID_R4G4B4:
				if (bPromoteMode) {
					BPT::T_Blit(*pDibBitmap, 0, 0, *m_pLoadedImage, promote_r4g4b4_op_type());
				}
				else {
					BPT::T_Blit(*pDibBitmap, 0, 0, *m_pLoadedImage, truncate_r4g4b4_op_type());
				}
				break;

			case ID_R5G5B5:
				if (bPromoteMode) {
					BPT::T_Blit(*pDibBitmap, 0, 0, *m_pLoadedImage, promote_r5g5b5_op_type());
				}
				else {
					BPT::T_Blit(*pDibBitmap, 0, 0, *m_pLoadedImage, truncate_r5g5b5_op_type());
				}
				break;

			case ID_R5G6B5:
				if (bPromoteMode) {
					BPT::T_Blit(*pDibBitmap, 0, 0, *m_pLoadedImage, promote_r5g6b5_op_type());
				}
				else {
					BPT::T_Blit(*pDibBitmap, 0, 0, *m_pLoadedImage, truncate_r5g6b5_op_type());
				}
				break;

			case ID_R6G6B6:
				if (bPromoteMode) {
					BPT::T_Blit(*pDibBitmap, 0, 0, *m_pLoadedImage, promote_r6g6b6_op_type());
				}
				else {
					BPT::T_Blit(*pDibBitmap, 0, 0, *m_pLoadedImage, truncate_r6g6b6_op_type());
				}
				break;

			}

		} else {

			BPT::T_Blit( *pDibBitmap, 0, 0, *m_pLoadedImage, copy_op_type() );

		}

	} else {

		// Draw the error indicator!

	}
}

// ============================================================================

// #define USE_DITHER_BUDDDY_DLL

#if defined(USE_DITHER_BUDDDY_DLL)
#include "DitherBuddyDemo.h"
#endif


//
//	CDitherStudioDoc::DitherIntoBitmapPrim()
//

void
CDitherStudioDoc::DitherIntoBitmapPrim(
	bitmap_type * pDest, bitmap_type * pSource, const int ditherMode
)
{

#if defined(USE_DITHER_BUDDDY_DLL)

	BPT::T_Blit( *pDest, 0, 0, *pSource, copy_op_type() );

#endif

	// ------------------------------------------------------------------------

DWORD inTime = timeGetTime();

	switch ( ditherMode ) {
	
	default:
	case ID_NO_DITHERING:
	case ID_R8G8B8:
		BPT::T_Blit( *pDest, 0, 0, *pSource, copy_op_type() );
		break;

	case ID_R1G1B1: {

		BPT::TEST_TErrorDiffusionDither<
			bitmap_type,
			(1 << 7), (1 << 7), (1 << 7),
			255, 255, 255,
			16, 8, 0
		> dither111Object(
			m_bUseNoise, m_nNoiseModulo, m_bUseReset, m_nResetModulo,
			m_bSerpentine, m_bForceDirection
		);

		dither111Object.DitherInto(pDest, pSource, m_DitherStyle, 1, 1, 1);
	}
	break;

	case ID_R1G2B1: {

		BPT::TEST_TErrorDiffusionDither<
			bitmap_type,
			(1 << 7), (3 << 6), (1 << 7),
			255, 255, 255,
			16, 8, 0
		> dither121Object(
			m_bUseNoise, m_nNoiseModulo, m_bUseReset, m_nResetModulo,
			m_bSerpentine, m_bForceDirection
		);

		dither121Object.DitherInto(pDest, pSource, m_DitherStyle, 1, 2, 1);
      }
	  break;

	case ID_R1G2B2: {

		BPT::TEST_TErrorDiffusionDither<
			bitmap_type,
			(1 << 7), (3 << 6), (3 << 6),
			255, 255, 255,
			16, 8, 0
		> dither122Object(
			m_bUseNoise, m_nNoiseModulo, m_bUseReset, m_nResetModulo,
			m_bSerpentine, m_bForceDirection
		);

		dither122Object.DitherInto(pDest, pSource, m_DitherStyle, 1, 2, 2);
	}
	break;

	case ID_R2G2B2: {

		BPT::TEST_TErrorDiffusionDither<
				bitmap_type,
				(3 << 6), (3 << 6), (3 << 6),
				255, 255, 255,
				16, 8, 0
		> dither222Object(
				m_bUseNoise, m_nNoiseModulo, m_bUseReset, m_nResetModulo,
				m_bSerpentine, m_bForceDirection
			);

			dither222Object.DitherInto(pDest, pSource, m_DitherStyle, 2, 2, 2);

		}
		break;

	case ID_R2G3B2: {

		BPT::TEST_TErrorDiffusionDither<
			bitmap_type,
			(3 << 6), (7 << 5), (3 << 6),
			255, 255, 255,
			16, 8, 0
		> dither232Object(
				m_bUseNoise, m_nNoiseModulo, m_bUseReset, m_nResetModulo,
				m_bSerpentine, m_bForceDirection
			);

			dither232Object.DitherInto(pDest, pSource, m_DitherStyle, 2, 3, 2);

		}
		break;

	case ID_R2G3B3: {

		BPT::TEST_TErrorDiffusionDither<
				bitmap_type,
				(3 << 6), (7 << 5), (7 << 5),
				255, 255, 255,
				16, 8, 0
			> dither233Object(
				m_bUseNoise, m_nNoiseModulo, m_bUseReset, m_nResetModulo,
				m_bSerpentine, m_bForceDirection
			);

			dither233Object.DitherInto(pDest, pSource, m_DitherStyle, 2, 3, 3);

		}
		break;

	case ID_R3G3B3: {

		BPT::TEST_TErrorDiffusionDither<
			bitmap_type,
			(7 << 5), (7 << 5), (7 << 5),
			255, 255, 255,
			16, 8, 0
		> dither333Object(
			m_bUseNoise, m_nNoiseModulo, m_bUseReset, m_nResetModulo,
			m_bSerpentine, m_bForceDirection
		);

		dither333Object.DitherInto(pDest, pSource, m_DitherStyle, 3, 3, 3);

	  }
	  break;

	case ID_R3G4B3: {

		BPT::TEST_TErrorDiffusionDither<
			bitmap_type,
			(7 << 5), (15 << 4), (7 << 5),
			255, 255, 255,
			16, 8, 0
		> dither343Object(
			m_bUseNoise, m_nNoiseModulo, m_bUseReset, m_nResetModulo,
			m_bSerpentine, m_bForceDirection
		);

		dither343Object.DitherInto(pDest, pSource, m_DitherStyle, 3, 4, 3);

      }
	  break;

	case ID_R3G4B4: {

		BPT::TEST_TErrorDiffusionDither<
			bitmap_type,
			(7 << 5), (15 << 4), (15 << 4),
			255, 255, 255,
			16, 8, 0
		> dither344Object(
			m_bUseNoise, m_nNoiseModulo, m_bUseReset, m_nResetModulo,
			m_bSerpentine, m_bForceDirection
		);

		dither344Object.DitherInto(pDest, pSource, m_DitherStyle, 3, 4, 4);

	  }
	  break;

	case ID_R4G4B4: {

		BPT::TEST_TErrorDiffusionDither<
				bitmap_type, 
				(15<<4), (15<<4), (15<<4), 
				255, 255, 255,
				16, 8, 0
			> dither444Object(
				m_bUseNoise, m_nNoiseModulo, m_bUseReset, m_nResetModulo,
				m_bSerpentine, m_bForceDirection
			);
	
			dither444Object.DitherInto( pDest, pSource, m_DitherStyle, 4, 4, 4);

		}
		break;
	
	case ID_R5G5B5: {
	
		BPT::TEST_TErrorDiffusionDither<
				bitmap_type, 
				(31<<3), (31<<3), (31<<3), 
				255, 255, 255,
				16, 8, 0
			> dither555Object(
				m_bUseNoise, m_nNoiseModulo, m_bUseReset, m_nResetModulo,
				m_bSerpentine, m_bForceDirection
			);
	
			dither555Object.DitherInto( pDest, pSource, m_DitherStyle, 5, 5, 5);

		}
		break;
	
	case ID_R5G6B5: {
	
		BPT::TEST_TErrorDiffusionDither<
				bitmap_type, 
				(31<<3), (63<<2), (31<<3), 
				255, 255, 255,
				16, 8, 0
			> dither565Object(
				m_bUseNoise, m_nNoiseModulo, m_bUseReset, m_nResetModulo,
				m_bSerpentine, m_bForceDirection
			);
	
			dither565Object.DitherInto( pDest, pSource, m_DitherStyle, 5, 6, 5);
	
		}
		break;

	case ID_R6G6B6: {

		BPT::TEST_TErrorDiffusionDither<
			bitmap_type,
			(63 << 3), (63 << 3), (63 << 3),
			255, 255, 255,
			16, 8, 0
		> dither666Object(
			m_bUseNoise, m_nNoiseModulo, m_bUseReset, m_nResetModulo,
			m_bSerpentine, m_bForceDirection
		);

		dither666Object.DitherInto(pDest, pSource, m_DitherStyle, 6, 6, 6);

	  }
	  break;
	
	}

DWORD outTime = timeGetTime();
TRACE("Dithering operation took %dms\n", outTime - inTime);

}

// ============================================================================

void
CDitherStudioDoc::DitherIntoBitmapDispatch(
	bitmap_type * pDest, bitmap_type * pSource, const int ditherMode
)
{
	// no dithering mode means match the RAW view 

	if (BPT::NO_DITHERING == m_DitherStyle) {

		BPT::T_Blit(pDest, 0, 0, m_RawDib.GetBitmapPtr(), copy_op_type());
		return;

	}

	// --------------------------------------------------------------

	// TODO - apply noise here

	// --------------------------------------------------------------

	if ( m_bVerticalMode ) {

		bitmap_type * pRotated = BPT::T_Create90DegreeRotatedBitmap( *pSource );
	
		if ( pRotated ) {
	
			DitherIntoBitmapPrim( pRotated, pRotated, ditherMode );
	
			bitmap_type * pDeRotated = BPT::T_Create90DegreeRotatedBitmap( *pRotated );
	
			if ( pDeRotated ) {
	
				BPT::T_Blit( 
					*pDest, 0, 0, *pDeRotated, copy_op_type(),
					&BPT::BLITFX( BPT::BLITFX::HFLIP | BPT::BLITFX::VFLIP )
				);
	
				delete pDeRotated;

				return;
	
			}
	
			delete pRotated;
	
		}
		
	}
	
	DitherIntoBitmapPrim( pDest, pSource, ditherMode );
}

// ============================================================================

//
//	CDitherStudioDoc::RebuildDitherDib()
//

void
CDitherStudioDoc::RebuildDitherDib()
{
	CWaitCursor waitCrsr;

	// ------------------------------------------------------------------------

	CSize size = m_DitherDib.Size();

	if ( m_CanvasSize != size ) {

		if ( !m_DitherDib.Create( m_CanvasSize ) ) {

			TRACE( "m_DitherDib failed to create (%dx%d)\n", m_CanvasSize.cx, m_CanvasSize.cy );

			return;

		}

	}

	// ----------------------------------------------------------------

	dib_type::bitmap_type * pDibBitmap = m_DitherDib.GetBitmapPtr();

	if ( !pDibBitmap ) {

		TRACE( "Unable to get 'DITHER' dib internal bitmap?\n" );

		// FATAL ERROR!!!

		return;

	}

	// THIS WILL EVENTUALLY 'BEGIN' THE DITHER OPERATION...
	// ----------------------------------------------------------------

	if ( m_pLoadedImage ) {

		DitherIntoBitmapDispatch( pDibBitmap, m_pLoadedImage, m_nDitherMode );

	} else {

		// Draw the error indicator!

	}
}

// ============================================================================

//
//	CDitherStudioDoc::RebuildDibs()
//

void
CDitherStudioDoc::RebuildDibs()
{
	RebuildRawDib();
	RebuildDitherDib();
}

// ============================================================================


void CDitherStudioDoc::OnAutoFocus() 
{
	m_bAutoFocus = !m_bAutoFocus;
}

void CDitherStudioDoc::OnUpdateAutoFocus(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
	pCmdUI->SetCheck( m_bAutoFocus ? 1 : 0 );
}

// ============================================================================

void CDitherStudioDoc::SmartSetDitherMode( const int nMode )
{
	if ( nMode != m_nDitherMode ) {

		m_nDitherMode = nMode;

		if ( m_bMatchDitherDepth ) {

			RebuildDibs();

		} else {

			RebuildDitherDib();

		}

		ForceFullRedraw();

	}
}

// ============================================================================

void CDitherStudioDoc::SmartSetMatchMode( const bool bMatchDitherDepth )
{
	if ( bMatchDitherDepth != m_bMatchDitherDepth ) {

		m_bMatchDitherDepth = bMatchDitherDepth;

		RebuildRawDib();

		ForceFullRedraw();

	}
}


void CDitherStudioDoc::SmartSetTruncateMode( const bool bTruncateMode )
{
	if (bTruncateMode != m_bTruncateMatchDitherMode) {

		m_bTruncateMatchDitherMode = bTruncateMode;

		RebuildRawDib();

		ForceFullRedraw();

	}
}

// ============================================================================

void CDitherStudioDoc::OnR1g1b1()
{
	SmartSetDitherMode(ID_R1G1B1);
}

void CDitherStudioDoc::OnUpdateR1g1b1(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pLoadedImage ? TRUE : FALSE);
	pCmdUI->SetCheck((ID_R1G1B1 == m_nDitherMode) ? 1 : 0);
}

void CDitherStudioDoc::OnR1g2b1()
{
	SmartSetDitherMode(ID_R1G2B1);
}

void CDitherStudioDoc::OnUpdateR1g2b1(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pLoadedImage ? TRUE : FALSE);
	pCmdUI->SetCheck((ID_R1G2B1 == m_nDitherMode) ? 1 : 0);
}

void CDitherStudioDoc::OnR1g2b2()
{
	SmartSetDitherMode(ID_R1G2B2);
}

void CDitherStudioDoc::OnUpdateR1g2b2(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pLoadedImage ? TRUE : FALSE);
	pCmdUI->SetCheck((ID_R1G2B2 == m_nDitherMode) ? 1 : 0);
}

void CDitherStudioDoc::OnR2g2b2()
{
	SmartSetDitherMode(ID_R2G2B2);
}

void CDitherStudioDoc::OnUpdateR2g2b2(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pLoadedImage ? TRUE : FALSE);
	pCmdUI->SetCheck((ID_R2G2B2 == m_nDitherMode) ? 1 : 0);
}

// ----

void CDitherStudioDoc::OnR2g3b2()
{
	SmartSetDitherMode(ID_R2G3B2);
}

void CDitherStudioDoc::OnUpdateR2g3b2(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pLoadedImage ? TRUE : FALSE);
	pCmdUI->SetCheck((ID_R2G3B2 == m_nDitherMode) ? 1 : 0);
}

// ----

void CDitherStudioDoc::OnR2g3b3()
{
	SmartSetDitherMode(ID_R2G3B3);
}

void CDitherStudioDoc::OnUpdateR2g3b3(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pLoadedImage ? TRUE : FALSE);
	pCmdUI->SetCheck((ID_R2G3B3 == m_nDitherMode) ? 1 : 0);
}

// ----

void CDitherStudioDoc::OnR3g3b3()
{
	SmartSetDitherMode(ID_R3G3B3);
}

void CDitherStudioDoc::OnUpdateR3g3b3(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pLoadedImage ? TRUE : FALSE);
	pCmdUI->SetCheck((ID_R3G3B3 == m_nDitherMode) ? 1 : 0);
}

// ----

void CDitherStudioDoc::OnR3g4b3()
{
	SmartSetDitherMode(ID_R3G4B3);
}

void CDitherStudioDoc::OnUpdateR3g4b3(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pLoadedImage ? TRUE : FALSE);
	pCmdUI->SetCheck((ID_R3G4B3 == m_nDitherMode) ? 1 : 0);
}

// ----

void CDitherStudioDoc::OnR3g4b4()
{
	SmartSetDitherMode(ID_R3G4B4);
}

void CDitherStudioDoc::OnUpdateR3g4b4(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pLoadedImage ? TRUE : FALSE);
	pCmdUI->SetCheck((ID_R3G4B4 == m_nDitherMode) ? 1 : 0);
}

// ----

void CDitherStudioDoc::OnR4g4b4()
{
	SmartSetDitherMode( ID_R4G4B4 );
}

void CDitherStudioDoc::OnUpdateR4g4b4(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_pLoadedImage ? TRUE : FALSE );
	pCmdUI->SetCheck( (ID_R4G4B4 == m_nDitherMode) ? 1 : 0 );
}

// ----

void CDitherStudioDoc::OnR5g5b5()
{
	SmartSetDitherMode( ID_R5G5B5 );
}

void CDitherStudioDoc::OnUpdateR5g5b5(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_pLoadedImage ? TRUE : FALSE );
	pCmdUI->SetCheck( (ID_R5G5B5 == m_nDitherMode) ? 1 : 0 );
}

// ----

void CDitherStudioDoc::OnR5g6b5()
{
	SmartSetDitherMode( ID_R5G6B5 );
}

void CDitherStudioDoc::OnUpdateR5g6b5(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_pLoadedImage ? TRUE : FALSE );
	pCmdUI->SetCheck( (ID_R5G6B5 == m_nDitherMode) ? 1 : 0 );
}

void CDitherStudioDoc::OnR6g6b6()
{
	SmartSetDitherMode(ID_R6G6B6);
}


void CDitherStudioDoc::OnUpdateR6g6b6(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pLoadedImage ? TRUE : FALSE);
	pCmdUI->SetCheck((ID_R6G6B6 == m_nDitherMode) ? 1 : 0);
}

void CDitherStudioDoc::OnR8g8b8()
{
	SmartSetDitherMode(ID_R8G8B8);
}


void CDitherStudioDoc::OnUpdateR8g8b8(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pLoadedImage ? TRUE : FALSE);
	pCmdUI->SetCheck((ID_R8G8B8 == m_nDitherMode) ? 1 : 0);
}


// ============================================================================

void CDitherStudioDoc::OnMatchDepth() 
{
	SmartSetMatchMode( !m_bMatchDitherDepth );
}

void CDitherStudioDoc::OnUpdateMatchDepth(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_pLoadedImage ? TRUE : FALSE );
	pCmdUI->SetCheck( m_bMatchDitherDepth ? 1 : 0 );
}

void CDitherStudioDoc::OnTruncateMatchMode()
{
	SmartSetTruncateMode(!m_bTruncateMatchDitherMode);
}

void CDitherStudioDoc::OnUpdateTruncateMatchMode(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bMatchDitherDepth ? TRUE : FALSE);
	pCmdUI->SetCheck(m_bTruncateMatchDitherMode ? 1 : 0);
}

// ============================================================================

void CDitherStudioDoc::SmartSetNoiseLevel( const int nNoiseModulo )
{
	if ( nNoiseModulo != m_nNoiseModulo ) {

		m_nNoiseModulo = nNoiseModulo;

		RebuildDitherDib();

		ForceFullRedraw();

	}
}

void CDitherStudioDoc::SmartSetDitherStyle(const BPT::DITHER_STYLE style)
{
	if (style == m_DitherStyle) return /* NOP */;
	m_DitherStyle = style;
	RebuildDitherDib();
	ForceFullRedraw();
}

// ============================================================================

void CDitherStudioDoc::OnNoiseLevel1() 
{
	SmartSetNoiseLevel( m_nNoiseLevel1 );
}

void CDitherStudioDoc::OnUpdateNoiseLevel1(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( (m_pLoadedImage && m_bUseNoise)  ? TRUE : FALSE );
	pCmdUI->SetCheck( m_nNoiseLevel1 == m_nNoiseModulo ? 1 : 0 );

}

void CDitherStudioDoc::OnNoiseLevel2() 
{
	SmartSetNoiseLevel( m_nNoiseLevel2 );
}

void CDitherStudioDoc::OnUpdateNoiseLevel2(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( (m_pLoadedImage && m_bUseNoise) ? TRUE : FALSE );
	pCmdUI->SetCheck( m_nNoiseLevel2 == m_nNoiseModulo ? 1 : 0 );
}

void CDitherStudioDoc::OnNoiseLevel3() 
{
	SmartSetNoiseLevel( m_nNoiseLevel3 );
}

void CDitherStudioDoc::OnUpdateNoiseLevel3(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( (m_pLoadedImage && m_bUseNoise) ? TRUE : FALSE );
	pCmdUI->SetCheck( m_nNoiseLevel3 == m_nNoiseModulo ? 1 : 0 );
}

void CDitherStudioDoc::OnNoiseLevel4() 
{
	SmartSetNoiseLevel( m_nNoiseLevel4 );
}

void CDitherStudioDoc::OnUpdateNoiseLevel4(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( (m_pLoadedImage && m_bUseNoise) ? TRUE : FALSE );
	pCmdUI->SetCheck( m_nNoiseLevel4 == m_nNoiseModulo ? 1 : 0 );
	
}

void CDitherStudioDoc::OnUseNoise() 
{
	m_bUseNoise = !m_bUseNoise;
	RebuildDitherDib();
	ForceFullRedraw();
}

void CDitherStudioDoc::OnUpdateUseNoise(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_pLoadedImage ? TRUE : FALSE );
	pCmdUI->SetCheck( m_bUseNoise ? 1 : 0 );
}

// ============================================================================

void CDitherStudioDoc::SmartSetResetLevel( const int nResetModulo )
{
	if ( nResetModulo != m_nResetModulo ) {

		m_nResetModulo = nResetModulo;

		RebuildDitherDib();

		ForceFullRedraw();

	}
}

// ============================================================================

void CDitherStudioDoc::OnResetLevel1() 
{
	SmartSetResetLevel( m_nResetLevel1 );
}

void CDitherStudioDoc::OnUpdateResetLevel1(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( (m_pLoadedImage && m_bUseReset)  ? TRUE : FALSE );
	pCmdUI->SetCheck( m_nResetLevel1 == m_nResetModulo ? 1 : 0 );

}

void CDitherStudioDoc::OnResetLevel2() 
{
	SmartSetResetLevel( m_nResetLevel2 );
}

void CDitherStudioDoc::OnUpdateResetLevel2(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( (m_pLoadedImage && m_bUseReset) ? TRUE : FALSE );
	pCmdUI->SetCheck( m_nResetLevel2 == m_nResetModulo ? 1 : 0 );
}

void CDitherStudioDoc::OnResetLevel3() 
{
	SmartSetResetLevel( m_nResetLevel3 );
}

void CDitherStudioDoc::OnUpdateResetLevel3(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( (m_pLoadedImage && m_bUseReset) ? TRUE : FALSE );
	pCmdUI->SetCheck( m_nResetLevel3 == m_nResetModulo ? 1 : 0 );
}

void CDitherStudioDoc::OnResetLevel4() 
{
	SmartSetResetLevel( m_nResetLevel4 );
}

void CDitherStudioDoc::OnUpdateResetLevel4(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( (m_pLoadedImage && m_bUseReset) ? TRUE : FALSE );
	pCmdUI->SetCheck( m_nResetLevel4 == m_nResetModulo ? 1 : 0 );
	
}

void CDitherStudioDoc::OnUseReset() 
{
	m_bUseReset = !m_bUseReset;
	RebuildDitherDib();
	ForceFullRedraw();
}

void CDitherStudioDoc::OnUpdateUseReset(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_pLoadedImage ? TRUE : FALSE );
	pCmdUI->SetCheck( m_bUseReset ? 1 : 0 );
}

// ============================================================================


void CDitherStudioDoc::OnVerticalMode() 
{
	m_bVerticalMode = !m_bVerticalMode;
	RebuildDitherDib();
	ForceFullRedraw();
}

void CDitherStudioDoc::OnUpdateVerticalMode(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_pLoadedImage ? TRUE : FALSE );
	pCmdUI->SetCheck( m_bVerticalMode ? 1 : 0 );
}

// ============================================================================

void CDitherStudioDoc::OnSerpentineMode() 
{
	m_bSerpentine = !m_bSerpentine;
	RebuildDitherDib();
	ForceFullRedraw();
}

void CDitherStudioDoc::OnUpdateSerpentineMode(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_pLoadedImage ? TRUE : FALSE );
	pCmdUI->SetCheck( m_bSerpentine ? 1 : 0 );
	
}

void CDitherStudioDoc::OnForceLeft() 
{
	m_bForceDirection = !m_bForceDirection;
	RebuildDitherDib();
	ForceFullRedraw();
}

void CDitherStudioDoc::OnUpdateForceLeft(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_pLoadedImage && (!m_bSerpentine) ? TRUE : FALSE );
	pCmdUI->SetCheck( m_bForceDirection ? 0 : 1 );
}

// ----------------------------------------------------------------------------


void CDitherStudioDoc::OnNoDithering()
{
	m_DitherStyle = BPT::NO_DITHERING;
	RebuildDitherDib();
	ForceFullRedraw();
}


void CDitherStudioDoc::OnUpdateNoDithering(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pLoadedImage ? TRUE : FALSE);
	pCmdUI->SetCheck((BPT::NO_DITHERING == m_DitherStyle) ? 1 : 0);
}


void CDitherStudioDoc::OnStyleFloydSteinberg() 
{
	m_DitherStyle = BPT::FLOYD_STEINBERG;
	RebuildDitherDib();
	ForceFullRedraw();
}

void CDitherStudioDoc::OnUpdateStyleFloydSteinberg(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_pLoadedImage ? TRUE : FALSE );
	pCmdUI->SetCheck( (BPT::FLOYD_STEINBERG == m_DitherStyle) ? 1 : 0 );
}

// ---------

void CDitherStudioDoc::OnStyleJarvis() 
{
	m_DitherStyle = BPT::JARVIS_JUDICE_NINKE;
	RebuildDitherDib();
	ForceFullRedraw();
}

void CDitherStudioDoc::OnUpdateStyleJarvis(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_pLoadedImage ? TRUE : FALSE );
	pCmdUI->SetCheck( (BPT::JARVIS_JUDICE_NINKE == m_DitherStyle) ? 1 : 0 );
}

// ---------

void CDitherStudioDoc::OnStyleStucki() 
{
	m_DitherStyle = BPT::STUCKI;
	RebuildDitherDib();
	ForceFullRedraw();
}

void CDitherStudioDoc::OnUpdateStyleStucki(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_pLoadedImage ? TRUE : FALSE );
	pCmdUI->SetCheck( (BPT::STUCKI == m_DitherStyle) ? 1 : 0 );
}

// ---------

void CDitherStudioDoc::OnStyleStevenson() 
{
	m_DitherStyle = BPT::STEVENSON_ARCE;
	RebuildDitherDib();
	ForceFullRedraw();
}

void CDitherStudioDoc::OnUpdateStyleStevenson(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_pLoadedImage ? TRUE : FALSE );
	pCmdUI->SetCheck( (BPT::STEVENSON_ARCE == m_DitherStyle) ? 1 : 0 );
}

// ---------

void CDitherStudioDoc::OnStyleBurkes() 
{
	m_DitherStyle = BPT::BURKES;
	RebuildDitherDib();
	ForceFullRedraw();
}

void CDitherStudioDoc::OnUpdateStyleBurkes(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_pLoadedImage ? TRUE : FALSE );
	pCmdUI->SetCheck( (BPT::BURKES == m_DitherStyle) ? 1 : 0 );
}

// ---------

void CDitherStudioDoc::OnStyleOther1() 
{
	m_DitherStyle = BPT::OTHER_FROM_WEB;
	RebuildDitherDib();
	ForceFullRedraw();
}

void CDitherStudioDoc::OnUpdateStyleOther1(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_pLoadedImage ? TRUE : FALSE );
	pCmdUI->SetCheck( (BPT::OTHER_FROM_WEB == m_DitherStyle) ? 1 : 0 );
}

// ---------

void CDitherStudioDoc::OnStyleSierra() 
{
	m_DitherStyle = BPT::SIERRA;
	RebuildDitherDib();
	ForceFullRedraw();
}

void CDitherStudioDoc::OnUpdateStyleSierra(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_pLoadedImage ? TRUE : FALSE );
	pCmdUI->SetCheck( (BPT::SIERRA == m_DitherStyle) ? 1 : 0 );
}

// ------------

void CDitherStudioDoc::OnOrdered2x2()
{
	SmartSetDitherStyle(BPT::ORDERED_2x2);
}

void CDitherStudioDoc::OnUpdateOrdered2x2(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pLoadedImage ? TRUE : FALSE);
	pCmdUI->SetCheck((BPT::ORDERED_2x2 == m_DitherStyle) ? 1 : 0);
}

// ------------

void CDitherStudioDoc::OnOrdered4x4()
{
	SmartSetDitherStyle(BPT::ORDERED_4x4);
}

void CDitherStudioDoc::OnUpdateOrdered4x4(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pLoadedImage ? TRUE : FALSE);
	pCmdUI->SetCheck((BPT::ORDERED_4x4 == m_DitherStyle) ? 1 : 0);
}

// ------------

void CDitherStudioDoc::OnOrdered8x8()
{
	SmartSetDitherStyle(BPT::ORDERED_8x8);
}

void CDitherStudioDoc::OnUpdateOrdered8x8(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pLoadedImage ? TRUE : FALSE);
	pCmdUI->SetCheck((BPT::ORDERED_8x8 == m_DitherStyle) ? 1 : 0);
}

// ------------

void CDitherStudioDoc::OnOrdered16x16()
{
	//SmartSetDitherStyle(BPT::ORDERED_8x8); // 16x16 is the same as 8x8 it seems
}

void CDitherStudioDoc::OnUpdateOrdered16x16(CCmdUI* pCmdUI)
{
	//pCmdUI->Enable(m_pLoadedImage ? TRUE : FALSE);
	//pCmdUI->SetCheck((BPT::ORDERED_16x16 == m_DitherStyle) ? 1 : 0);
}

// ============================================================================

//
//	CDitherStudioDoc::fHSVTest()
//

COLORREF 
CDitherStudioDoc::fHSVTest( float h0t1, float s0t1, float v0t2, float c )
{
	float h = h0t1;// * 360.0f;
	float s = s0t1;
	float v = s0t1;
		
	if ( 360.0f == h ) h = 0.0f;
		
		h = h / 60.0f;
		
		float i = floorf( h );
		float f = h - i;
		float p = v * (1.0f - s);
		float q = v * (1 - (s * f));
		float t = v * (1 - (s * (1 - f)));
		
		switch ( (int)i ) {
		
		case 0: return RGB( v * c, t * c, p * c ); break;
		case 1: return RGB( q * c, v * c, p * c ); break;
		case 2: return RGB( p * c, v * c, t * c ); break;
		case 3: return RGB( p * c, q * c, v * c ); break;
		case 4: return RGB( t * c, p * c, v * c ); break;
		case 5: return RGB( v * c, p * c, q * c ); break;
		
	}
		
	return RGB(0,0,0);
}

bool 
CDitherStudioDoc::BuildDefaultImage()
{
	// ------------------------------------------------------------------------

	if ( m_pLoadedImage ) {

		delete m_pLoadedImage;

	}

	// ------------------------------------------------------------------------

#if defined(DEBUG)
	m_CanvasSize.cx = 256;// 360;
	m_CanvasSize.cy = 256;// 360;
#else
	m_CanvasSize.cx = 1024;// 360;
	m_CanvasSize.cy = 1024;// 360;
#endif

	m_pLoadedImage = new bitmap_type;

	if ( m_pLoadedImage ) {

		if ( !m_pLoadedImage->Create( m_CanvasSize.cx, m_CanvasSize.cy ) ) {

			TRACE( "Unable to create image size (%dx%d)\n", m_CanvasSize.cx, m_CanvasSize.cy );

			// THIS IS AN FATAL ERROR!!!!

			return false;

		}

	}

	// Generate the color spread
	// ------------------------------------------------------------------------

	float h = 0.0f;

	float hd = 360.0f / (float)m_CanvasSize.cx;
		
	int n = m_CanvasSize.cy;
		
	for ( int hc = 0; hc < m_CanvasSize.cy; hc++ ) {

		bitmap_type::pixel_iterator pit = m_pLoadedImage->Iterator( 0, hc );
		
		// core
		
		int z = 0;
		
		for ( int vc = 0; vc < m_CanvasSize.cx/2; vc++ ) {
					
			float v = (float)vc / (float)((m_CanvasSize.cx / 2) - 1);
		
			for ( int sc = 0; sc < 2; sc++ ) {
		
				float s1 = (float) z / (float)(n - 1);

//				float s2 = 1.0f - sinf( (3.1415 * (float) z) / (float)(n - 1) );
		
//				COLORREF color = fHSVTest( h, 1.0f - s, 1.0f - v );

//				float s = (s1 + (s2 * 3.0f)) / 4.0f;

				float s = s1; // (s1 + (s2 * 3.0f)) / 4.0f;

				COLORREF color = fHSVTest( h, 1.0f - s, 1.0f - v );

				*pit++ = bitmap_type::pixel_type(
					GetRValue(color), GetGValue(color), GetBValue(color)
				);
		
				++z;
		
			}
		
		}
		
		h = h + hd;
		
	}

	return true;

}

// ----------------------------------------------------------------------------

BOOL CDitherStudioDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  Add your specialized creation code here

	ActuallyProcessLoadRequest(lpszPathName);

	return TRUE;
}

// -----------------------------------------------------------------------------------

void CDitherStudioDoc::OnEditCopy()
{
	if (OpenClipboard(AfxGetMainWnd()->GetSafeHwnd())) {

		// TODO: Add your command handler code here

		HBITMAP hBitmap = m_DitherDib.GetHBitmap();

		//https://stackoverflow.com/questions/37503097/winapi-correctly-copying-a-hbitmap-to-the-clipboard

		DIBSECTION ds;

		::GetObject(hBitmap, sizeof(DIBSECTION), &ds);

		//make sure compression is BI_RGB
		ds.dsBmih.biCompression = BI_RGB;
		ds.dsBmih.biHeight = -std::abs(ds.dsBmih.biHeight);

		HDC hdc = ::GetDC(NULL);

		HBITMAP hbitmap_ddb = ::CreateDIBitmap(
			hdc, &ds.dsBmih, CBM_INIT, ds.dsBm.bmBits, (BITMAPINFO*)&ds.dsBmih, DIB_RGB_COLORS);

		::ReleaseDC(NULL, hdc);

		::EmptyClipboard();
		::SetClipboardData(CF_BITMAP, hbitmap_ddb);

		::CloseClipboard();
	}
}


void CDitherStudioDoc::OnUpdateEditCopy(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pLoadedImage ? TRUE : FALSE);
}

void CDitherStudioDoc::OnEditPaste()
{
	if (OpenClipboard(AfxGetMainWnd()->GetSafeHwnd())) {

		HBITMAP hBitmap = (HBITMAP)GetClipboardData(CF_BITMAP);

		if (NULL != hBitmap)
		{
			ProcessHBITMAPAsNewDocument(hBitmap);
		}
		else
		{
			// paste binary data as pixels :-)
			// think about that for drag and drop as well 
			// no reason why you can't drag something goofy 
			// and see it...
		}

		CloseClipboard();
	}
}


void CDitherStudioDoc::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
	bool bitmapAvailable = false;

	if (OpenClipboard(AfxGetMainWnd()->GetSafeHwnd())) {

		HBITMAP hBitmap = (HBITMAP)GetClipboardData(CF_BITMAP);
		bitmapAvailable = (NULL != hBitmap);

		CloseClipboard();
	}

	pCmdUI->Enable(m_pLoadedImage && bitmapAvailable ? TRUE : FALSE);
}


void CDitherStudioDoc::OnFileSaveAs()
{
	CFileDialog opf(
		FALSE, NULL, NULL, OFN_OVERWRITEPROMPT | OFN_LONGNAMES,
		_T(
			"All Supported Formats (*.png)|*.png|"
			"PNG Files (*.png)|*.png|"
			"All Files (*.*)|*.*|"
			"|"
		)
		, AfxGetMainWnd()
	);

	if (IDOK != opf.DoModal()) {

		return;

	}

	CImage im;

	im.Attach(m_DitherDib.GetHBitmap());
	HRESULT hr = im.Save(opf.GetPathName());
	im.Detach();
}

// ----------------------------------------------------------------------------------------

void CDitherStudioDoc::OnSwapRAndG()
{
	BPT::T_Blit(
		m_pLoadedImage, 0, 0, m_pLoadedImage,
		BPT::SwapRedAndGreenChannelsOp<pixel_type>()
	);

	RebuildDibs();
	ForceFullRedraw();
}

void CDitherStudioDoc::OnUpdateSwapRAndG(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pLoadedImage ? TRUE : FALSE);
}

// ----------------------------------------------------------------------------------------

void CDitherStudioDoc::OnSwapRAndB()
{
	BPT::T_Blit(
		m_pLoadedImage, 0, 0, m_pLoadedImage,
		BPT::SwapRedAndBlueChannelsOp<pixel_type>()
	);

	RebuildDibs();
	ForceFullRedraw();
}

void CDitherStudioDoc::OnUpdateSwapRAndB(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pLoadedImage ? TRUE : FALSE);
}

// ----------------------------------------------------------------------------------------

void CDitherStudioDoc::OnInvertColors()
{
	BPT::T_Blit(
		m_pLoadedImage, 0, 0, m_pLoadedImage,
		BPT::TInvertDstTransferROP<pixel_type>()
	);

	RebuildDibs();
	ForceFullRedraw();
}


void CDitherStudioDoc::OnUpdateInvertColors(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pLoadedImage ? TRUE : FALSE);
}

// ----------------------------------------------------------------------------------------

void CDitherStudioDoc::OnRotateColorChanels()
{
	BPT::T_Blit(
		m_pLoadedImage, 0, 0, m_pLoadedImage,
		BPT::RotateRightRGBChannelsOp<pixel_type>()
	);

	RebuildDibs();
	ForceFullRedraw();
}


void CDitherStudioDoc::OnUpdateRotateColorChanels(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pLoadedImage ? TRUE : FALSE);
}

// ----------------------------------------------------------------------------------------

void CDitherStudioDoc::OnSpreadOutChannels()
{
	CWaitCursor wc;

	bitmap_type* pTransformed;

	if (m_bSpreadOutModeActive) {

		// TODO - put channels back inplace

		int w = m_pLoadedImage->Width() / 3;
		int h = m_pLoadedImage->Height();

		pTransformed = new bitmap_type;
		pTransformed->Create( w, h);

		BPT::T_Blit(
			pTransformed, -w * 0, 0, m_pLoadedImage,
			BPT::TransferOnlyRChannel<pixel_type>()
		);

		BPT::T_Blit(
			pTransformed, -w * 1, 0, m_pLoadedImage,
			BPT::TransferOnlyGChannel<pixel_type>()
		);

		BPT::T_Blit(
			pTransformed, -w * 2, 0, m_pLoadedImage,
			BPT::TransferOnlyBChannel<pixel_type>()
		);

	}
	else {

		// spread the channels out horizontally

		int w = m_pLoadedImage->Width();
		int h = m_pLoadedImage->Height();

		pTransformed = new bitmap_type;
		pTransformed->Create(w * 3, h);

		BPT::T_Blit(
			pTransformed, w * 0, 0, m_pLoadedImage,
			BPT::IsolateRChannel<pixel_type>()
		);

		BPT::T_Blit(
			pTransformed, w * 1, 0, m_pLoadedImage,
			BPT::IsolateGChannel<pixel_type>()
		);

		BPT::T_Blit(
			pTransformed, w * 2, 0, m_pLoadedImage,
			BPT::IsolateBChannel<pixel_type>()
		);

	}

	m_bSpreadOutModeActive = !m_bSpreadOutModeActive;

	ChangeDocumentToSurface(pTransformed, false);
}

void CDitherStudioDoc::OnUpdateSpreadOutChannels(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pLoadedImage ? TRUE : FALSE);
	pCmdUI->SetCheck(m_bSpreadOutModeActive ? TRUE : FALSE);
}

// ----------------------------------------------------------------------------------------

void CDitherStudioDoc::PerformInterleave()
{
	CWaitCursor wc;

	bitmap_type* pTransformed;

	// spread the channels out horizontally

	int w = m_pLoadedImage->Width();
	int h = m_pLoadedImage->Height();

	pTransformed = new bitmap_type;
	pTransformed->Create(w * 3, h);

	BPT::T_Blit(
		pTransformed, w * 0, 0, m_pLoadedImage,
		BPT::IsolateRChannel<pixel_type>()
	);

	BPT::T_Blit(
		pTransformed, w * 1, 0, m_pLoadedImage,
		BPT::IsolateGChannel<pixel_type>()
	);

	BPT::T_Blit(
		pTransformed, w * 2, 0, m_pLoadedImage,
		BPT::IsolateBChannel<pixel_type>()
	);

	// rework the size so that the horizontal copies are stacked vertically

	pTransformed->ReinterpretDimensions(w, h * 3);

	ChangeDocumentToSurface(pTransformed, false);
}

void CDitherStudioDoc::PerformDeinterleave()
{
	CWaitCursor wc;

	bitmap_type* pTransformed;

	// TODO - put channels back inplace

	int w = m_pLoadedImage->Width();
	int h = m_pLoadedImage->Height() / 3;

	// rework the size so that the vertical copies are stacked horizontal

	m_pLoadedImage->ReinterpretDimensions(w * 3, h);

	pTransformed = new bitmap_type;
	pTransformed->Create(w, h);

	BPT::T_Blit(
		pTransformed, -w * 0, 0, m_pLoadedImage,
		BPT::TransferOnlyRChannel<pixel_type>()
	);

	BPT::T_Blit(
		pTransformed, -w * 1, 0, m_pLoadedImage,
		BPT::TransferOnlyGChannel<pixel_type>()
	);

	BPT::T_Blit(
		pTransformed, -w * 2, 0, m_pLoadedImage,
		BPT::TransferOnlyBChannel<pixel_type>()
	);

	ChangeDocumentToSurface(pTransformed, false);
}

// ------------------------------------------------------

void CDitherStudioDoc::OnInterleaveChannels()
{
	m_bInterleaveModeActive = !m_bInterleaveModeActive;

	if (m_bInterleaveModeActive)
	{
		PerformInterleave();
	}
	else
	{
		PerformDeinterleave();
	}
}

void CDitherStudioDoc::OnUpdateInterleaveChannels(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pLoadedImage ? TRUE : FALSE);
	pCmdUI->SetCheck(m_bInterleaveModeActive ? TRUE : FALSE);
}

// ----------------------------------------------------------------------------------------

void CDitherStudioDoc::OnRotate90()
{
	bitmap_type *pRotated = BPT::T_Create90DegreeRotatedBitmap(*m_pLoadedImage);

	ChangeDocumentToSurface(pRotated,true);
}

void CDitherStudioDoc::OnUpdateRotate90(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pLoadedImage ? TRUE : FALSE);
}

// ----------------------------------------------------------------------------------------

void CDitherStudioDoc::OnFlipHorizontal()
{
	BPT::BLITFX bfx = BPT::BLITFX(BPT::BLITFX::HFLIP);

	// perform the operation using the dither dib as a temporary blit target (wasteful)

	BPT::T_Blit(*m_DitherDib.GetBitmapPtr(), 0, 0, *m_pLoadedImage, BPT::TCopyROP<pixel_type>());

	BPT::T_Blit(
		*m_pLoadedImage, 0, 0, *m_DitherDib.GetBitmapPtr(),
		BPT::TCopyROP<pixel_type>(),
		&bfx
	);

	RebuildDibs();
	ForceFullRedraw();
}

void CDitherStudioDoc::OnUpdateFlipHorizontal(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pLoadedImage ? TRUE : FALSE);
}

// ----------------------------------------------------------------------------------------

void CDitherStudioDoc::OnFlipVertical()
{
	BPT::BLITFX bfx = BPT::BLITFX(BPT::BLITFX::VFLIP);

	// perform the operation using the dither dib as a temporary blit target (wasteful)

	BPT::T_Blit( *m_DitherDib.GetBitmapPtr(), 0, 0, *m_pLoadedImage, BPT::TCopyROP<pixel_type>());

	BPT::T_Blit(
		*m_pLoadedImage, 0, 0, *m_DitherDib.GetBitmapPtr(),
		BPT::TCopyROP<pixel_type>(),
		&bfx
	);

	RebuildDibs();
	ForceFullRedraw();
}

void CDitherStudioDoc::OnUpdateFlipVertical(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pLoadedImage ? TRUE : FALSE);
}

// ----------------------------------------------------------------------------------------

void CDitherStudioDoc::On3x3PixelNeighborhood()
{
	CWaitCursor c;

	int w = m_pLoadedImage->Width();
	int h = m_pLoadedImage->Height();

	SIZE tileSize = { 3, 3 };
	SIZE tileStepSize = { 1, 1 };

#if 1

	// testing

	bitmap_type * pTransformed = BPT::T_CreateScaledBitmap(*m_pLoadedImage,w * tileSize.cx, h * tileSize.cy);

#else

	bitmap_type* pTransformed = new bitmap_type;
	pTransformed->Create(w * tileSize.cx, h * tileSize.cy);

	// transfer the tiles

	BPT::BlitTilesOntoBitmapOp<bitmap_type> tileProcessor(pTransformed);
	BPT::TiledIterationOfBitmap<bitmap_type>(*m_pLoadedImage, tileSize, tileStepSize, tileProcessor);

#endif

	// modify the tiles to be differenced from the average of the block

	//BPT::TBiasedDifferenceTileFromAverageOp<bitmap_type> postProcessor;
	BPT::TBiasedDifferenceTileFromCenterOp<bitmap_type> postProcessor;
	BPT::TiledIterationOfBitmap<bitmap_type>(*pTransformed, tileSize, tileSize, postProcessor);

	ChangeDocumentToSurface(pTransformed,true);
	RebuildDibs();
	ForceFullRedraw();
}


void CDitherStudioDoc::OnUpdate3x3PixelNeighborhood(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pLoadedImage ? TRUE : FALSE);
}

// ---------------------------------------------------------

void CDitherStudioDoc::ScaleLoadedImage(int scale)
{
	bitmap_type* pTransformed = BPT::T_CreateScaledBitmap(
		*m_pLoadedImage, m_pLoadedImage->Width() * scale, m_pLoadedImage->Height() );

	ChangeDocumentToSurface(pTransformed,true);
	RebuildDibs();
	ForceFullRedraw();
}

void CDitherStudioDoc::OnScale2x()
{
	ScaleLoadedImage(2);
}


void CDitherStudioDoc::OnUpdateScale2x(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pLoadedImage ? TRUE : FALSE);
}


void CDitherStudioDoc::OnScale3x()
{
	ScaleLoadedImage(3);
}


void CDitherStudioDoc::OnUpdateScale3x(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pLoadedImage ? TRUE : FALSE);
}

// ========================================================================================

namespace BPT {


} // namespace BPT

//
//	On2dPaletteMode
//

void CDitherStudioDoc::On2dPaletteMode()
{
	CWaitCursor c;

	BPT::Ts2cB_Bitmap(*m_pLoadedImage);

	RebuildDibs();
	ForceFullRedraw();
}


void CDitherStudioDoc::OnUpdate2dPaletteMode(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pLoadedImage ? TRUE : FALSE);
}


void CDitherStudioDoc::On3dPaletteMode()
{
	CWaitCursor c;

	BPT::Tc2sB_Bitmap(*m_pLoadedImage);

	RebuildDibs();
	ForceFullRedraw();
}


void CDitherStudioDoc::OnUpdate3dPaletteMode(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pLoadedImage ? TRUE : FALSE);
}

// ----------------------------

void CDitherStudioDoc::OnCaptureDitherResultAsDoc()
{
	CWaitCursor c;

	BPT::T_Blit(this->m_pLoadedImage, 0, 0, m_DitherDib.GetBitmapPtr(), copy_op_type());

	m_DitherStyle = BPT::NO_DITHERING; // there must be a better way of doing this change
	m_nDitherMode = ID_R8G8B8;

	RebuildDibs();
	ForceFullRedraw();

#if 0 // test compression

	compressed_image_type* pImage = new compressed_image_type;

	BPT::TIsNotValue<pixel_type> predicate(pixel_type(0));

	bool bResult = pImage->Create(*pTransformed, predicate);

	size_t compSize = pImage->CompressedSizeEstimate();
	size_t ucompSize = pImage->UncompressedSizeEstimate();

	TRACE("COMPRESSION TEST (%d) %p compressed size estimate %d/%d (%5.2f%%)\n", bResult, pImage, compSize, ucompSize, (float)compSize * 100.0 / (float)ucompSize);

	delete pImage;

#endif

}


void CDitherStudioDoc::OnUpdateCaptureDitherResultAsDoc(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pLoadedImage ? TRUE : FALSE);
}


void CDitherStudioDoc::OnGrayscale()
{
	CWaitCursor c;

	BPT::TBitmapOperation(m_pLoadedImage, BPT::TNotGrayscaleOp<pixel_type>());

	RebuildDibs();
	ForceFullRedraw();
}


void CDitherStudioDoc::OnUpdateGrayscale(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pLoadedImage ? TRUE : FALSE);
}


void CDitherStudioDoc::OnDesaturate()
{
	CWaitCursor c;

	BPT::TBitmapOperation(m_pLoadedImage, BPT::TDesaturateOp<pixel_type>());

	RebuildDibs();
	ForceFullRedraw();
}


void CDitherStudioDoc::OnUpdateDesaturate(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pLoadedImage ? TRUE : FALSE);
}

// ----------------------------------------------------------------------------------------

namespace BPT {


} // namespace BPT

void CDitherStudioDoc::OnOperationA()
{
	CWaitCursor c;

#if 1

	BPT::TScanlineSubtractRGBChannelsLeftToRight<bitmap_type> subtractOp;

	BPT::TBitmapScanlineOperation(m_pLoadedImage, subtractOp);

#else

	//BPT::TBiasedDifferenceTileFromAverageOp<bitmap_type> postProcessor;

	SIZE tileSize = { 8, 8 };
	SIZE tileStep = { 8, 8 };

	typedef BPT::TSequenceWithModuloOp<pixel_type> op_t;

	op_t op(0,1);

	BPT::TPerformOpForTileOp<bitmap_type,op_t> tileProcessor(op);

	BPT::TiledIterationOfBitmap<bitmap_type>(*m_pLoadedImage, tileSize, tileStep, tileProcessor);

#endif

	RebuildDibs();
	ForceFullRedraw();
}

void CDitherStudioDoc::OnUpdateOperationA(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pLoadedImage ? TRUE : FALSE);
}

// ----------------------------------------------------------------------------

namespace BPT {


} // namespace BPT

void CDitherStudioDoc::OnOperationB()
{
	CWaitCursor c;

	// setup dialog

	CHistogramVizDlg dlg(this,AfxGetMainWnd());

	dlg.histogramOptions = histogram_t::Options::DO_NOT_WRITE_TO_SOURCE;
	dlg.vizOptions = fibsphere_viz_t::Options::DEFAULT;
	dlg.SO_save_pointcloud = TRUE;

	INT_PTR res = dlg.DoModal();

	if (IDOK != res) return;

	// -----------------------------------------------------------------------

	if (nullptr != m_pLastHistogram) delete m_pLastHistogram;
	if (nullptr != m_pLastFibSphereViz) delete m_pLastFibSphereViz;

	m_pLastHistogram = new histogram_t(dlg.histogramOptions);
	BPT::TFillQuantizedHistogramOp<histogram_t> histogramBuilder(*m_pLastHistogram);
	TBitmapOperation(*m_DitherDib.GetBitmapPtr(), histogramBuilder);

	// ------------------------------------------------------------------------

	//TRACE("********** Histogram next unique index %d\n", m_pLastHistogram->nextUniqueIndex);

	m_pLastFibSphereViz = new fibsphere_viz_t;
	m_pLastFibSphereViz->Create(*m_pLastHistogram, dlg.vizOptions);

	if (dlg.SO_save_pointcloud)
	{
		CFileDialog opf(
			FALSE, NULL, NULL, OFN_OVERWRITEPROMPT | OFN_LONGNAMES,
			_T(
				"All Supported Formats (*.obj)|*.obj|"
				"OBJ Files (*.obj)|*.obj|"
				"All Files (*.*)|*.*|"
				"|"
			)
			, AfxGetMainWnd()
		);

		if (IDOK == opf.DoModal()) {

			fibsphere_viz_t& viz = *m_pLastFibSphereViz; // shorten the name 

#if 1

			BPT::TSaveWeirdTriangleSoupOID(
				opf.GetPathName(),
				viz.colors.begin(), viz.colors.end(),
				viz.colors.begin(), viz.colors.end()
			);

#else

			if (dlg.SO_use_colors_for_both) {

				BPT::SavePointCloud(
					opf.GetPathName(),
					viz.colors.begin(), viz.colors.end(),
					viz.colors.begin(), viz.colors.end());

			}
			else if (dlg.SO_fibonacci_sphere_as_colors) {

				BPT::SavePointCloud(
					opf.GetPathName(),
					viz.colors.begin(), viz.colors.end(),
					viz.fibLattice.lattice.begin(), viz.fibLattice.lattice.end());

			}
			else {

				BPT::SavePointCloud(
					opf.GetPathName(),
					viz.fibLattice.lattice.begin(), viz.fibLattice.lattice.end(),
					viz.colors.begin(), viz.colors.end());

			}

#endif

		}
	}

	// ------------------------------------------------------------------------

	// ------------------------------------------------------------------------

	m_DitherStyle = BPT::NO_DITHERING; // there must be a better way of doing this change
	m_nDitherMode = ID_R8G8B8;

	//ChangeDocumentToSurface(pTransformed, true);
	RebuildDibs();
	ForceFullRedraw();
}

void CDitherStudioDoc::OnUpdateOperationB(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pLoadedImage ? TRUE : FALSE);
}

// -------------------------------------------------------------

void CDitherStudioDoc::OnIsolateUniqueColors()
{
	CWaitCursor c;

	typedef BPT::TSimpleBitmap<BPT::PIXEL_R6G6B6> target_t;
	typedef BPT::TQuantized2DPalettedSRLImageOperation<target_t, bitmap_type> quantizer_op_t;

	// TODO - make m_bKeepIsolatedColorsInPlace work!!

	auto quantBuildOptions = quantizer_op_t::histogram_t::Options::WRITE_CHROMAKEY_FOR_DUPLICATES;

	quantizer_op_t builder(quantBuildOptions);
	//quantizer_op_t builder(quantizer_op_t::histogram_t::Options::WRITE_QUANTIZED_VALUES);
	//quantizer_op_t builder(quantizer_op_t::histogram_t::Options::STORE_AS_INDICES);

	bool bResult = builder.Create(*m_DitherDib.GetBitmapPtr(), pixel_type(255, 0, 255), m_bKeepIsolatedColorsInPlace);

	ASSERT(bResult);

	bitmap_type* pTransformed = builder.pModifiedSourceCopy->Clone();

	// --------------------------

	m_DitherStyle = BPT::NO_DITHERING; // there must be a better way of doing this change
	m_nDitherMode = ID_R8G8B8;

	ChangeDocumentToSurface(pTransformed, true);
	RebuildDibs();
	ForceFullRedraw();
}


void CDitherStudioDoc::OnUpdateIsolateUniqueColors(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pLoadedImage ? TRUE : FALSE);
}

// -------------------------------------------------------------

void CDitherStudioDoc::OnMiscXyzSortAfterLoad()
{
	m_bXYZSortAfterLoad = !m_bXYZSortAfterLoad;
}

void CDitherStudioDoc::OnUpdateMiscXyzSortAfterLoad(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_bXYZSortAfterLoad ? 1 : 0);
}


void CDitherStudioDoc::OnMiscOriginalInverseDifference()
{
	bitmap_type* pTripleWide = BPT::TMakeAnInverseAndDifferenceTripleWide(m_pLoadedImage);

	m_DitherStyle = BPT::NO_DITHERING; // there must be a better way of doing this change
	m_nDitherMode = ID_R8G8B8;

	ChangeDocumentToSurface(pTripleWide, true);
	RebuildDibs();
	ForceFullRedraw();
}


void CDitherStudioDoc::OnUpdateMiscOriginalInverseDifference(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pLoadedImage ? TRUE : FALSE);
}

// ----------------------------------------------------------------------------

namespace BPT {

	//
	//	TSimpleBitStream
	//

	template< typename T=U8, typename V=std::vector<T> >
	struct TSimpleBitStream : public V
	{
		U8 cache;
		U8 mask;

		TSimpleBitStream(U8 initCache = 0, U8 initMask = 0x01 ) : cache(initCache), mask(initMask)
		{
		}

		void FlushCache()
		{
			push_back(cache);
			mask = 0x01;
			cache = 0;
		}

		void OutputBit(bool bSet)
		{
			if (bSet) cache |= mask;

			if (0x80 == mask)
			{
				FlushCache();
			}
			else
			{
				mask <<= 1;
			}
		}

	};

	//
	//	TSplitBitsIntoStreams()
	//	-- this assumes 8 bits, this will need to be rewritten to be more general (boost type???
	//

	template<typename T>
	struct TSplitBitsIntoStreams {

		typedef TSplitBitsIntoStreams<T> this_type;
		typedef T surface_type;
		typedef typename T::pixel_type pixel_type;
		typedef typename pixel_type::value_type value_type;

		surface_type* pPackedSurface;
		TSimpleBitStream<> bitStreams[8];

		TSplitBitsIntoStreams() : pPackedSurface(0)
		{
		}

		~TSplitBitsIntoStreams()
		{
			if (pPackedSurface) delete pPackedSurface;
		}

		void FlushChannelCaches()
		{
			// need better iteration logic (should use new style for loops and constructs)
			for (int i = 0; i < 8; ++i )
			{
				bitStreams[i].FlushCache();
			}
		}

		void ReserveCaches(int reserveSize)
		{
			for (int i = 0; i < 8; ++i)
			{
				bitStreams[i].reserve(reserveSize);
			}
		}

		void DeconstructChannel(value_type v)
		{
			for ( value_type mask = 0x01, i = 0; i < 8; ++i, mask<<=1)
			{
				bitStreams[i].OutputBit(0 != (v & mask));
			}
		}

		void DeconstructPixel(const pixel_type& d)
		{
			DeconstructChannel(d.r);
			DeconstructChannel(d.g);
			DeconstructChannel(d.b);
		}

		bool DeconstructSurface(T* pSourceSurface, SIZE& tileSize)
		{
			this_type* that = this;
			auto op = [&that](T& view) {
				BPT::TBitmapOperation(view, [&that](const pixel_type& d) {
					that->DeconstructPixel(d);
				});
			};

			TiledIterationOfBitmap(*pSourceSurface, tileSize, tileSize, op);

			FlushChannelCaches();

			return true;
		}

		void ReconstructSurfaceFromStreams(T* pDestSurface, SIZE& tileSize)
		{
			SIZE chunkSize = {pDestSurface->Width() / 8, pDestSurface->Height() };

			this_type* that = this;

			int currentChunk = 0;

			auto op = [&that,&currentChunk](T& view) {
				auto reset = that->bitStreams[currentChunk].begin();
				auto first = reset;
				auto last = that->bitStreams[currentChunk].end();
				BPT::TBitmapOperation(view, [&that,&first,&last,&reset](pixel_type& d) {
					d.r = *first;
					if (++first == last) first = reset;
					d.g = *first;
					if (++first == last) first = reset;
					d.b = *first;
					if (++first == last) first = reset;
				});
				++currentChunk;
			};

			TiledIterationOfBitmap(*pDestSurface, chunkSize, chunkSize, op);
		}

		bool Create(T* pSourceSurface, SIZE & tileSize )
		{
			ReserveCaches((pSourceSurface->Width() * pSourceSurface->Height()) / 8);

			bool bResult = DeconstructSurface(pSourceSurface, tileSize);

			// now turn the bitStreams back into pixels with reordered bits

			SIZE wideSize = { pSourceSurface->Width() * 8, pSourceSurface->Height() };

			pPackedSurface = new surface_type;

			ASSERT(pPackedSurface);

			if (!pPackedSurface->Create(wideSize))
			{
				TRACE("Look unable to create packed surface!");
				ASSERT(false);
			}

			ReconstructSurfaceFromStreams(pPackedSurface, tileSize);

			return true;
		}

	};

} // namespace BPT

void CDitherStudioDoc::OnMiscSplitbitsintostreams()
{
	// TODO: Add your command handler code here

	BPT::TSplitBitsIntoStreams<bitmap_type> splitterOp;

	SIZE tileSize = { 16, 16 };
	bool bResult = splitterOp.Create(m_pLoadedImage, tileSize);

	m_DitherStyle = BPT::NO_DITHERING; // there must be a better way of doing this change
	m_nDitherMode = ID_R8G8B8;

	ChangeDocumentToSurface(splitterOp.pPackedSurface, true);

	splitterOp.pPackedSurface = 0; // detach ownership of the packedSurface to the document

	RebuildDibs();
	ForceFullRedraw();

}


void CDitherStudioDoc::OnUpdateMiscSplitbitsintostreams(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pLoadedImage ? TRUE : FALSE);
}

void CDitherStudioDoc::OnMiscShoworiginalpositionsofunique()
{
	m_bKeepIsolatedColorsInPlace = !m_bKeepIsolatedColorsInPlace;
}


void CDitherStudioDoc::OnUpdateMiscShoworiginalpositionsofunique(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pLoadedImage ? TRUE : FALSE);
	pCmdUI->SetCheck(m_bKeepIsolatedColorsInPlace ? 1 : 0);
}


void CDitherStudioDoc::OnCropHalfHorizontal()
{
	RECT view = { 0, 0, m_pLoadedImage->Width() / 2, m_pLoadedImage->Height() };

	bitmap_type* pTransformed = m_pLoadedImage->Extract(view);

	ChangeDocumentToSurface(pTransformed, true);
	RebuildDibs();
	ForceFullRedraw();
}

void CDitherStudioDoc::OnUpdateCropHalfHorizontal(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pLoadedImage ? TRUE : FALSE);
}


void CDitherStudioDoc::OnUpdateMiscSortbasedoncoloraddress(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pLoadedImage ? TRUE : FALSE);
}


void CDitherStudioDoc::OnMiscSortbasedoncoloraddress()
{
	// only work with tightly packed images (should be all given 32 bpp...
	ASSERT((m_pLoadedImage->Width() * sizeof(pixel_type)) == m_pLoadedImage->Pitch());

	bitmap_type::pixel_iterator it = m_pLoadedImage->Iterator(0, 0);
	bitmap_type::pixel_iterator end = it + m_pLoadedImage->Width() * m_pLoadedImage->Height();

	std::sort(it, end, [](const pixel_type& a, const pixel_type& b) -> bool {
		if (a == b) return (&a < &b);
		return a < b;
	});

	RebuildDibs();
	ForceFullRedraw();
}
