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


#if !defined(AFX_RECTANGLE_H__5DEAA18D_58ED_4650_B7FE_77C13923D61D__INCLUDED_)
#define AFX_RECTANGLE_H__5DEAA18D_58ED_4650_B7FE_77C13923D61D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>

namespace BPT {

	//
	//	SRect
	//

	struct SRect : public ::RECT {

		// Construction
		// --------------------------------------------------------------------

		SRect() { /* Empty */ }

		SRect( const int l, const int t, const int r, const int b ) {

			left = l;
			top = t;
			right = r;
			bottom = b;

		}

		SRect( const int x, const int y, const SIZE & size ) {

			left = x;
			top = y;
			right = x + size.cx;
			bottom = y + size.cy;

		}

		SRect( const POINT & pt, const SIZE & size ) {

			left = pt.x;
			top = pt.y;
			right = pt.x + size.cx;
			bottom = pt.y + size.cy;

		}

		SRect( const POINT & pt, const int w, const int h ) {

			left = pt.x;
			top = pt.y;
			right = pt.x + w;
			bottom = pt.y + h;

		}

		SRect( const SIZE & size ) {

			left = 0;
			top = 0;
			right = size.cx;
			bottom = size.cy;

		}

		// Utility
		// --------------------------------------------------------------------

		int Width() const {

			return (right - left);

		}

		int Height() const {

			return (bottom - top);

		}

		SIZE Size() const {

			SIZE size = { Width(), Height() };
			return size;

		}

		bool Intersection( const SRect * a, const SRect * b ) {

			return (0 != ::IntersectRect( this, a, b ));

		}

		void Offset( const int dx, const int dy ) {

			::OffsetRect( this, dx, dy );

		}

		bool Empty() const {

			return (TRUE == ::IsRectEmpty( this ));

		}

	};

	// ------------------------------------------------------------------------
	// Functions
	// ------------------------------------------------------------------------

	//
	//	RECT_AdjustRectForHFlip
	//

	void __inline
	RECT_AdjustRectForHFlip( SRect & rect, const SRect & outterRect ) {

		rect.Offset(
			(outterRect.right - rect.right) - (rect.left - outterRect.left), 0
		);

	}

	//
	//	RECT_AdjustRectForVFlip
	//

	void __inline
	RECT_AdjustRectForVFlip( SRect & rect, const SRect & outterRect ) {

		rect.Offset(
			0, (outterRect.bottom - rect.bottom) - (rect.top - outterRect.top)
		);

	}



}; // namespace BPT

#endif // !defined(AFX_RECTANGLE_H__5DEAA18D_58ED_4650_B7FE_77C13923D61D__INCLUDED_)
