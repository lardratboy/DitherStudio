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
#pragma once

#include "DitherStudioDoc.h"
#include "BPTLib/GLContext.h"
#include "TFace3Vector3.h"
#include "TWavefrontOBJOperations.h"

// C3DOBJView

class C3DOBJView : public CView
{

public:

	typedef CDitherStudioDoc::dib_type dib_type;
	typedef C3DOBJView this_type;
	typedef CDitherStudioDoc doc_type;

protected:

	BPT::GLContext m_GLContext;

	CPoint m_ButtonDownPos;
	GLfloat m_xAngle, m_xButtonDownAngle;
	GLfloat m_yAngle, m_yButtonDownAngle;
	GLfloat m_zDistance;
	GLfloat m_zButtonDownDistance;
	GLfloat m_fMinZDistance;
	GLfloat m_fMaxZDistance;
	BPT::VECTOR3 m_LookAtPos;
	BPT::VECTOR3 m_ButtonDownLookAtPos;
	BPT::VECTOR3 m_VisibleRegion;
	bool m_bFlipFlopScales;
	bool m_bShowUniqueColors;
	bool m_bRebuildRequested;
	BPT::mat4 m_projectionMat;
	BPT::mat4 m_viewMat;
	GLint shaderProgram;

	C3DOBJView();

	void ResetView();

	DECLARE_DYNCREATE(C3DOBJView)

public:

	virtual ~C3DOBJView();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	doc_type* GetDocument();

	void RequestRebuild();

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
//	afx_msg void OnPaint();
	virtual void OnDraw(CDC* /*pDC*/);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCancelMode();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	void RenderScene();
	void TrackMouse(UINT nFlags, CPoint cursorPos);
	void OnButtonDown(UINT nFlags, CPoint point);
	bool ChangeZDistance(const GLfloat zDistance);
	void RenderGridLines();
	void RenderPointClouds();
	afx_msg void OnReset3dView();
	afx_msg void OnUpdateReset3dView(CCmdUI* pCmdUI);
	afx_msg void On3dFlipFlop();
	afx_msg void OnUpdate3dFlipFlop(CCmdUI* pCmdUI);
	afx_msg void On3dShowUnique();
	afx_msg void OnUpdate3dShowUnique(CCmdUI* pCmdUI);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};

#ifndef _DEBUG  // debug version in bpaintView.cpp
inline C3DOBJView::doc_type* C3DOBJView::GetDocument()
{
	return (C3DOBJView::doc_type*)m_pDocument;
}
#endif

