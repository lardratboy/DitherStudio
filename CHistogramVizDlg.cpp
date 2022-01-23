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
#include "CHistogramVizDlg.h"


// CHistogramVizDlg dialog

IMPLEMENT_DYNAMIC(CHistogramVizDlg, CDialogEx)

CHistogramVizDlg::CHistogramVizDlg(CDitherStudioDoc* pDoc, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HISTOGRAM_VIZ, pParent),
	m_pDoc(pDoc),
	histogramOptions(CDitherStudioDoc::histogram_t::Options::DO_NOT_WRITE_TO_SOURCE),
	vizOptions(CDitherStudioDoc::fibsphere_viz_t::Options::DEFAULT)
	, HO_store_as_indices(FALSE)
	, HO_write_quantized(FALSE)
	, HO_write_chromakey(FALSE)
	, VO_sort_lattice(FALSE)
	, VO_sort_colors(FALSE)
	, VO_push_outside(FALSE)
	, SO_save_pointcloud(FALSE)
	, SO_use_colors_for_both(FALSE)
	, SO_fibonacci_sphere_as_colors(FALSE)
{
}

CHistogramVizDlg::~CHistogramVizDlg()
{
}

void CHistogramVizDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_HISTOGRAM_STORE_AS_INDICES_CHECK, HO_store_as_indices);
	DDX_Check(pDX, IDC_HISTOGRAM_WRITE_QUANTIZED_CHECK, HO_write_quantized);
	DDX_Check(pDX, IDC_HISTOGRAM_WRITE_CHROMAKEY__CHECK, HO_write_chromakey);
	DDX_Check(pDX, IDC_VIZ_SORT_LATTICE_CHECK, VO_sort_lattice);
	DDX_Check(pDX, IDC_VIZ_SORT_COLORS_CHECK, VO_sort_colors);
	DDX_Check(pDX, IDC_VIZ_PUSH_OUTSIDE_UNIT_SPHERE, VO_push_outside);
	DDX_Check(pDX, IDC_SAVE_POINT_CLOUD_CHECK, SO_save_pointcloud);
	DDX_Check(pDX, IDC_SAVE_COLORS_FOR_BOTH_CHECK, SO_use_colors_for_both);
	DDX_Check(pDX, IDC_USE_FIBONACCI_AS_COLORS_CHECK, SO_fibonacci_sphere_as_colors);
}


BEGIN_MESSAGE_MAP(CHistogramVizDlg, CDialogEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CHistogramVizDlg message handlers


CDitherStudioDoc::histogram_t::Options histogramOptions;
CDitherStudioDoc::fibsphere_viz_t::Options vizOptions;


void CHistogramVizDlg::OnClose()
{
	// ------------------------------------

	// transfer the checkboxes to actual bits for the operations

	unsigned histOptionsFlags = 0;

	if (HO_store_as_indices) {

		histOptionsFlags = CDitherStudioDoc::histogram_t::Options::STORE_AS_INDICES;

	}
	else if (HO_write_chromakey) {

		histOptionsFlags = CDitherStudioDoc::histogram_t::Options::WRITE_CHROMAKEY_FOR_DUPLICATES;

	}
	else if (HO_write_quantized) {

		histOptionsFlags = CDitherStudioDoc::histogram_t::Options::WRITE_QUANTIZED_VALUES;

	}

	unsigned vizOptionFlags = 0;

	if (VO_sort_lattice) {

		vizOptionFlags |= CDitherStudioDoc::fibsphere_viz_t::Options::SORT_LATTICE_POINTS_BY_INDEX;

	}

	if (VO_sort_colors) {

		vizOptionFlags |= CDitherStudioDoc::fibsphere_viz_t::Options::SORT_COLORS_BY_INDEX;

	}

	if (VO_push_outside) {

		vizOptionFlags |= CDitherStudioDoc::fibsphere_viz_t::Options::PUSH_OUTSIDE_THE_UNIT_SPHERE_FOR_DECENDENTS;

	}

	// ------------------------------------

	this->histogramOptions = (CDitherStudioDoc::histogram_t::Options)histOptionsFlags;
	this->vizOptions = (CDitherStudioDoc::fibsphere_viz_t::Options)vizOptionFlags;

	CDialogEx::OnClose();
}
