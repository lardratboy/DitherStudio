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

// CHistogramVizDlg dialog

class CHistogramVizDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHistogramVizDlg)

public:
	CHistogramVizDlg(CDitherStudioDoc* pDoc, CWnd* pParent = nullptr);   // standard constructor
	virtual ~CHistogramVizDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HISTOGRAM_VIZ };
#endif

public:

	CDitherStudioDoc::histogram_t::Options histogramOptions;
	CDitherStudioDoc::fibsphere_viz_t::Options vizOptions;

protected:

	CDitherStudioDoc* m_pDoc;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	BOOL HO_store_as_indices;
	BOOL HO_write_quantized;
	BOOL HO_write_chromakey;
	BOOL VO_sort_lattice;
	BOOL VO_sort_colors;
	BOOL VO_push_outside;
	BOOL SO_save_pointcloud;
	BOOL SO_use_colors_for_both;
	BOOL SO_fibonacci_sphere_as_colors;
};
