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

#if !defined(AFX_BPTPRIMITIVES_H__0F0CFB9D_2380_497C_A185_73C7168CFE34__INCLUDED_)
#define AFX_BPTPRIMITIVES_H__0F0CFB9D_2380_497C_A185_73C7168CFE34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BPTUtility.h"
#include "BPTDrawingAlgorithms.h"

namespace BPT {

	// ------------------------------------------------------------------------

	//
	//	T_SolidRectPrim
	//

	template< typename DST_SURFACE, typename TOP >
	void __forceinline T_SolidRectPrim(
		DST_SURFACE & dstSurface
		,const RECT & rect
		,const typename DST_SURFACE::pixel_type value
		,TOP op = TOP()
		,const RECT * pOptionalClipRect = 0
	) {

		// Clip the operation to be within the bitmap limits
		// --------------------------------------------------------------------

		RECT bitmapLimits = dstSurface.Rect();

		RECT clippedRect;
	
		if ( !IntersectRect( &clippedRect, &bitmapLimits, &rect ) ) {
	
			return /* NOP */;
	
		}

		// Clip the bitmap limited rect to the clipping rectangle if one
		// --------------------------------------------------------------------

		if ( pOptionalClipRect ) {

			if ( !IntersectRect( &clippedRect, &clippedRect, pOptionalClipRect ) ) {

				return /* NOP */;

			}

		}
	
		// Perform the operation
		// --------------------------------------------------------------------

		int cx = clippedRect.right - clippedRect.left;
		int cy = clippedRect.bottom - clippedRect.top;
		int sx = clippedRect.left;
		int sy = clippedRect.top;

		for ( int y = 0; y < cy; y++ ) {

			typename DST_SURFACE::pixel_iterator it = dstSurface.Iterator( sx, sy++ );

			op.OutputMajor( it, it + cx, value );

		}

	}

	//
	//	T_RectPrim
	//

	template< typename DST_SURFACE, typename TOP >
	void __forceinline T_RectPrim(
		DST_SURFACE & dstSurface
		,const RECT & rect
		,const typename DST_SURFACE::pixel_type value
		,TOP op = TOP()
		,const RECT * pOptionalClipRect = 0
	) {

		RECT a = { rect.left, rect.top, rect.right, rect.top + 1 };

		T_SolidRectPrim( dstSurface, a, value, op, pOptionalClipRect );

		if ( 1 != (rect.bottom - rect.top) ) {

			RECT b = { rect.left, rect.bottom - 1, rect.right, rect.bottom };

			T_SolidRectPrim( dstSurface, b, value, op, pOptionalClipRect );

			RECT c = { rect.left, rect.top + 1, rect.left + 1, rect.bottom - 1 };
	
			T_SolidRectPrim( dstSurface, c, value, op, pOptionalClipRect );
	
			if ( 1 != (rect.right - rect.left) ) {
	
				RECT d = { rect.right - 1, rect.top + 1, rect.right, rect.bottom - 1 };

				T_SolidRectPrim( dstSurface, d, value, op, pOptionalClipRect );
	
			}

		}

	}

	// ------------------------------------------------------------------------

	//
	//	T_HLinePrim
	//

	template< typename DST_SURFACE, typename TOP >
	void __forceinline T_HLinePrim(
		DST_SURFACE & dstSurface
		,const int x
		,const int y
		,const int width
		,const typename DST_SURFACE::pixel_type value
		,TOP op = TOP()
		,const RECT * pOptionalClipRect = 0
	) {

		RECT a = { x, y, x + width, y + 1 };

		T_SolidRectPrim( dstSurface, a, value, op, pOptionalClipRect );

	}
	
	//
	//	T_VLinePrim
	//

	template< typename DST_SURFACE, typename TOP >
	void __forceinline T_VLinePrim(
		DST_SURFACE & dstSurface
		,const int x
		,const int y
		,const int height
		,const typename DST_SURFACE::pixel_type value
		,TOP op = TOP()
		,const RECT * pOptionalClipRect = 0
	) {

		RECT a = { x, y, x + 1, y + height };

		T_SolidRectPrim( dstSurface, a, value, op, pOptionalClipRect );

	}
	// ------------------------------------------------------------------------

	//
	//	T_CropRectPrim
	//

	template< typename DST_SURFACE, typename TOP >
	void __forceinline T_CropRectPrim(
		DST_SURFACE & dstSurface
		,const RECT & rect
		,const typename DST_SURFACE::pixel_type value
		,TOP op = TOP()
		,const RECT * pOptionalClipRect = 0
	) {

		// Clip the operation to be within the bitmap limits
		// --------------------------------------------------------------------

		RECT bitmapLimits = dstSurface.Rect();

		RECT clippedRect;
	
		if ( !IntersectRect( &clippedRect, &bitmapLimits, &rect ) ) {
	
			return /* NOP */;
	
		}
	
		// Perform the operation
		// --------------------------------------------------------------------

		RECT rects[ 4 ];

		int count = ReturnExternalClipRects( rects, &bitmapLimits, &clippedRect );

		for ( int i = 0; i < count; i++ ) {

			T_SolidRectPrim( dstSurface, rect[ i ], value, op, pOptionalClipRect );

		}

	}

	// ------------------------------------------------------------------------

	//
	//	TSimplePenPrimitive<>
	//

	template< typename RT, typename TOP >
	class TSimplePenPrimitive {

	private:

		typename RT::pixel_type m_Value;
		TOP m_Op;
		RECT * m_pClip;
		RECT m_ClipRect;

	public:

		TSimplePenPrimitive(
			const typename RT::pixel_type value, TOP op = TOP(),
			const RECT * pOptionalClipRect = 0
		) : m_Value( value ), m_Op( op ) {

			if ( pOptionalClipRect ) {

				m_ClipRect = *pOptionalClipRect;
				m_pClip = &m_ClipRect;

			} else {

				m_pClip = 0;

			}

		}

		void Brush( RT & renderTarget, const int x, const int y ) {

			RECT rect = { x, y, x + 1, y + 1 };

			T_SolidRectPrim( renderTarget, rect, m_Value, m_Op, m_pClip );

		}

		void Rectangle( RT & renderTarget, const RECT & rect ) {
		
			T_SolidRectPrim( renderTarget, rect, m_Value, m_Op, m_pClip );

		}

		// Helper methods (can be overriden but why bother)
		// --------------------------------------------------------------------

		void Brush( RT & renderTarget, const POINT & at ) {

			Brush( renderTarget, at.x, at.y );
		
		}

		void Pixel( RT & renderTarget, const POINT & at ) {

			RECT rect = { at.x, at.y, at.x + 1, at.y + 1 };

			Rectangle( renderTarget, rect );
		
		}

		void operator()( RT & renderTarget, const int x, const int y ) {

			Brush( renderTarget, x, y );

		}

		void operator()( RT & renderTarget, const POINT & at ) {

			Brush( renderTarget, at );

		}

		void operator()( RT & renderTarget, const RECT & rect ) {

			Rectangle( renderTarget, rect );

		}

	};

	// ------------------------------------------------------------------------

	//
	//	T_SolidEllipsePrim
	//

	template< typename DST_SURFACE, typename TOP >
	void __forceinline T_SolidEllipsePrim(
		DST_SURFACE & dstSurface
		,const POINT & center
		,const int a
		,const int b
		,const typename DST_SURFACE::pixel_type value
		,TOP op = TOP()
		,const RECT * pOptionalClipRect = 0
	) {

		typedef TSimplePenPrimitive<DST_SURFACE,TOP> SIMPLE_PEN;

		SIMPLE_PEN pen( value, op, pOptionalClipRect );

		T_SolidEllipse<DST_SURFACE,SIMPLE_PEN>(
			dstSurface, center, a, b, pen
		);

	}

	//
	//	T_EllipsePrim
	//

	template< typename DST_SURFACE, typename TOP >
	void __forceinline T_EllipsePrim(
		DST_SURFACE & dstSurface
		,const POINT & center
		,const int a
		,const int b
		,const typename DST_SURFACE::pixel_type value
		,TOP op = TOP()
		,const RECT * pOptionalClipRect = 0
	) {

		typedef TSimplePenPrimitive<DST_SURFACE,TOP> SIMPLE_PEN;

		SIMPLE_PEN pen( value, op, pOptionalClipRect );

		T_Ellipse<DST_SURFACE,SIMPLE_PEN>(
			dstSurface, center, a, b, pen
		);

	}

	//
	//	T_LinePrim
	//

	template< typename DST_SURFACE, typename TOP >
	void __forceinline T_LinePrim(
		DST_SURFACE & dstSurface
		,const POINT & from
		,const POINT & to
		,const typename DST_SURFACE::pixel_type value
		,TOP op = TOP()
		,const RECT * pOptionalClipRect = 0
	) {

		typedef TSimplePenPrimitive<DST_SURFACE,TOP> SIMPLE_PEN;

		SIMPLE_PEN pen( value, op, pOptionalClipRect );

		T_LineDDA<DST_SURFACE,SIMPLE_PEN>(
			dstSurface, from, to, pen
		);

	}

	// ------------------------------------------------------------------------

	//
	//	T_FindBoundingRect()
	//

	template< typename SURFACE, typename PREDICATE >
	bool __forceinline T_FindBoundingRect(
		RECT & outRect
		,SURFACE & surface
		,PREDICATE solidPredicate
		,const RECT * pOptionalRect = 0
	) {

		// Assume empty
		// ----------------------------------------------------------------

		SetRectEmpty( &outRect );

		// Clip the operation to be within the bitmap limits
		// ----------------------------------------------------------------

		RECT bitmapLimits = surface.Rect();

		RECT clippedRect;

		if ( pOptionalRect ) {

			if ( !IntersectRect( &clippedRect, &bitmapLimits, pOptionalRect ) ) {

				return false;
		
			}

		} else {

			clippedRect = bitmapLimits;

		}
	
		// Perform the operation
		// ----------------------------------------------------------------

		int cx = clippedRect.right - clippedRect.left;
		int cy = clippedRect.bottom - clippedRect.top;
		int sx = clippedRect.left;
		int sy = clippedRect.top;

		// Perform the operation
		// ----------------------------------------------------------------

		int x1 = cx;
		int y1 = cy;
		int x2 = 0;
		int y2 = 0;

		for ( int y = 0; y < cy; y++ ) {

			SURFACE::pixel_iterator it = surface.Iterator( sx, sy++ );

			for ( int x = 0; x < cx; x++ ) {

				if ( solidPredicate( *it++ ) ) {

					if ( x < x1 ) x1 = x;
					if ( x > x2 ) x2 = x;
					if ( y < y1 ) y1 = y;
					if ( y > y2 ) y2 = y;

				}

			}

		}

		if ( (x1 <= x2) && (y1 <= y2) ) {

			outRect.left = clippedRect.left + x1;
			outRect.top = clippedRect.top + y1;
			outRect.right = clippedRect.left + x2 + 1;
			outRect.bottom = clippedRect.top + y2 + 1;

			return true;

		}

		return false;

	}

	// ------------------------------------------------------------------------

	//
	//	T_DrawEmbossedRectOutline()
	//	

	template< typename DST_SURFACE, typename TOP >
	void __forceinline T_DrawEmbossedRectOutline(
		DST_SURFACE & dstSurface
		,const RECT & rect
		,const typename DST_SURFACE::pixel_type hColor
		,const typename DST_SURFACE::pixel_type bColor
		,const typename DST_SURFACE::pixel_type sColor
		,TOP op = TOP()
		,const RECT * pOptionalClipRect = 0
	) {

		int w = rect.right - rect.left;
		int h = rect.bottom - rect.top;

		T_HLinePrim( dstSurface, rect.left, rect.top, w - 1, hColor, op, pOptionalClipRect );
		T_VLinePrim( dstSurface, rect.left, rect.top + 1, h - 1, hColor, op, pOptionalClipRect );

		T_VLinePrim( dstSurface, rect.right - 1, rect.top, h, sColor, op, pOptionalClipRect );
		T_HLinePrim( dstSurface, rect.left + 1, rect.bottom - 1, w - 2, sColor, op, pOptionalClipRect );

	}

	// ------------------------------------------------------------------------

	//
	//	T_DrawEmbossedBorderRect()
	//	

	template< typename DST_SURFACE, typename TOP >
	void __forceinline T_DrawEmbossedBorderRect(
		DST_SURFACE & dstSurface
		,const RECT & rect
		,const int thickness
		,const typename DST_SURFACE::pixel_type hColor
		,const typename DST_SURFACE::pixel_type bColor
		,const typename DST_SURFACE::pixel_type sColor
		,TOP op = TOP()
		,const RECT * pOptionalClipRect = 0
	) {

		// Draw the outter embossed

		T_DrawEmbossedRectOutline( 
			dstSurface, rect,
			hColor, bColor, sColor, 
			op, pOptionalClipRect
		);

		// Draw the inner rects

		for ( int i = 0; i <= thickness; i++ ) {

			CRect innerRect( 
				rect.left + (i + 1), rect.top + (i + 1), 
				rect.right - (i + 1), rect.bottom - (i + 1)
			);

			if ( thickness != i ) {

				T_RectPrim( 
					dstSurface, innerRect, bColor, op, pOptionalClipRect
				);

			} else {

				// Draw the inner most reversed embossed rect

				T_DrawEmbossedRectOutline( 
					dstSurface, innerRect,
					sColor, bColor, hColor, 
					op, pOptionalClipRect
				);

			}

		}

	}

	// ------------------------------------------------------------------------

	//
	//	T_DrawEmbossedRect()
	//	

	template< typename DST_SURFACE, typename TOP >
	void __forceinline T_DrawEmbossedRect(
		DST_SURFACE & dstSurface
		,const RECT & rect
		,const int level
		,const typename DST_SURFACE::pixel_type hColor
		,const typename DST_SURFACE::pixel_type bColor
		,const typename DST_SURFACE::pixel_type sColor
		,TOP op = TOP()
		,const RECT * pOptionalClipRect = 0
	) {

		// --------------------------------------------------------------------

		CRect innerRect( rect.left + 1, rect.top + 1, rect.right - 1, rect.bottom - 1 );

		if ( 1 < level ) {

			T_DrawEmbossedRect(
				dstSurface, innerRect, level - 1,
				hColor, bColor, sColor, op,
				pOptionalClipRect
			);

		} else {

			T_SolidRectPrim( dstSurface, innerRect, bColor, op, pOptionalClipRect );

		}

		// --------------------------------------------------------------------

		T_DrawEmbossedRectOutline( 
			dstSurface, rect,
			hColor, bColor, sColor, 
			op, pOptionalClipRect
		);

	}

	// ------------------------------------------------------------------------

}; // namespace BPT

#endif // !defined(AFX_BPTPRIMITIVES_H__0F0CFB9D_2380_497C_A185_73C7168CFE34__INCLUDED_)
