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

#if !defined(AFX_BPTBLITTER_H__681A3A57_7AAF_4AD7_A5BA_BE2A3BD11191__INCLUDED_)
#define AFX_BPTBLITTER_H__681A3A57_7AAF_4AD7_A5BA_BE2A3BD11191__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <math.h>
#include <utility>

#include "BPTRasterOps.h"

#define STRICT_ENFORCEMENT_OF_TEXTURE_COORDINATES

namespace BPT {

	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// Blitter template function
	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	//
	//	BLITFX
	//

	struct BLITFX {

		enum : DWORD {

			DEFAULT				= 0x00000000
		,	HFLIP				= 0x00000001
		,	VFLIP				= 0x00000002

		};

		DWORD dwFlags;

		BLITFX( DWORD flags = DEFAULT ) : dwFlags( flags ) { /* Empty */ }

	};

	// ------------------------------------------------------------------------

	//
	//	T_Blit
	//

	template< typename DST_SURFACE, typename SRC_SURFACE, typename TOP, const bool SRC_MAJOR_TRANSFER = true >
	void __forceinline T_Blit(
		DST_SURFACE & dstSurface
		,const int x
		,const int y
		,SRC_SURFACE & srcSurface
		,TOP op = TOP()
		,const BLITFX * blitFX = 0
		,const RECT * optionalDstClipRect = 0
		,const RECT * optionalSrcSubRect = 0
	) {

		// Clip the optional clipping rect to the dest bitmap limits
		// --------------------------------------------------------------------

		RECT dstLimitsRect = dstSurface.Rect();

		RECT clippedDstRect;

		if ( optionalDstClipRect ) {

			if ( !IntersectRect( &clippedDstRect, &dstLimitsRect, optionalDstClipRect) ) {

				return /* NOP */;

			}

		} else {

			clippedDstRect = dstLimitsRect;

		}

		// Get the source operation size
		// --------------------------------------------------------------------

		RECT srcLimitsRect = srcSurface.Rect();

		RECT clippedSrcRect;

		if ( optionalSrcSubRect ) {

			if ( !IntersectRect( &clippedSrcRect, &srcLimitsRect, optionalSrcSubRect) ) {

				return /* NOP */;

			}

		} else {

			clippedSrcRect = srcLimitsRect;

		}

		// Perform a simple clipping operation to detect NOP
		// --------------------------------------------------------------------

		SIZE clippedSrcRectSize = SizeOfRect( &clippedSrcRect );

		RECT dstOperation = {
			x, y, x + clippedSrcRectSize.cx, y + clippedSrcRectSize.cy
		};

		RECT clippedRect;
			
		if ( !IntersectRect( &clippedRect, &clippedDstRect, &dstOperation) ) {

			return /* NOP */;

		}

		// Setup the general loop variables
		// --------------------------------------------------------------------

		int cx = clippedRect.right - clippedRect.left;
		int cy = clippedRect.bottom - clippedRect.top;

		int sx = ((clippedRect.left - x) + clippedSrcRect.left);
		int sy = ((clippedRect.top - y) + clippedSrcRect.top);

		// Check for flipping and adjust the dest position and step amount.
		// --------------------------------------------------------------------

		int dx, dy, ddx, ddy;

		if ( blitFX ) {

			if ( BLITFX::HFLIP & blitFX->dwFlags ) {

				sx = (clippedSrcRect.right - (sx + cx));
				dx = (clippedRect.right - 1);
				ddx = -1;

			} else {

				dx = clippedRect.left;
				ddx = 1;

			}

			if ( BLITFX::VFLIP & blitFX->dwFlags ) {

				sy = (clippedSrcRect.bottom - (sy + cy));
				dy = (clippedRect.bottom - 1);
				ddy = -1;

			} else {

				dy = clippedRect.top;
				ddy = 1;

			}

		} else {

			dx = clippedRect.left;
			ddx = 1;

			dy = clippedRect.top;
			ddy = 1;

		}

		// Process the non clipped spans
		// --------------------------------------------------------------------

		if ( 1 == ddx ) {

			for ( int ly = 0; ly < cy; ly++ ) {

				DST_SURFACE::pixel_iterator dstIT = dstSurface.Iterator( dx, dy );
				SRC_SURFACE::pixel_iterator srcIT = srcSurface.Iterator( sx, sy );

				if (SRC_MAJOR_TRANSFER) // this better get optimized away!
				{
					op.InputMajor(srcIT, srcIT + cx, dstIT);
				}
				else
				{
					op.OutputMajor(dstIT, dstIT + cx, srcIT);
				}

				dy += ddy;
				++sy;
	
			}

		} else {

			for ( int ly = 0; ly < cy; ly++ ) {

				DST_SURFACE::reverse_iterator dstIT = dstSurface.rIterator( dx, dy );
				SRC_SURFACE::pixel_iterator srcIT = srcSurface.Iterator( sx, sy );

				if (SRC_MAJOR_TRANSFER) // this better get optimized away!
				{
					op.InputMajor(srcIT, srcIT + cx, dstIT);
				}
				else
				{
					op.OutputMajor(dstIT, dstIT + cx, srcIT);
				}

				dy += ddy;
				++sy;
	
			}

		}

	}

	//
	//	T_Blit
	//

	template< typename DST_SURFACE, typename SRC_SURFACE, typename TOP >
	void __forceinline T_Blit(
		DST_SURFACE * pDstSurface
		,const int x
		,const int y
		,SRC_SURFACE * pSrcSurface
		,TOP op = TOP()
		,const BLITFX * blitFX = 0
		,const RECT * optionalDstClipRect = 0
		,const RECT * optionalSrcSubRect = 0
	) {

		T_Blit(
			*pDstSurface, x, y, *pSrcSurface, op, blitFX, 
			optionalDstClipRect, optionalSrcSubRect
		);

	}

	// ---------------------------------------------------------------------------

	// BITMAP OPERATIONS NEED SOME DESIGN HELP - specifically with naming of the operations
	// op is too generic of a suffix op is both for per 'pixel' transfers and operations...

	//
	//	TBitmapScanlineOperation
	//

	template< typename DST_SURFACE, typename SCANLINEOP >
	void __forceinline TBitmapScanlineOperation(
		DST_SURFACE& dstSurface
		, const RECT& rect
		, SCANLINEOP& scanlineOp
		, const RECT* pOptionalClipRect = 0
	) {

		// Clip the operation to be within the bitmap limits
		// --------------------------------------------------------------------

		RECT bitmapLimits = dstSurface.Rect();

		RECT clippedRect;

		if (!IntersectRect(&clippedRect, &bitmapLimits, &rect)) {

			return /* NOP */;

		}

		// Clip the bitmap limited rect to the clipping rectangle if one
		// --------------------------------------------------------------------

		if (pOptionalClipRect) {

			if (!IntersectRect(&clippedRect, &clippedRect, pOptionalClipRect)) {

				return /* NOP */;

			}

		}

		// Perform the operation
		// --------------------------------------------------------------------

		int cx = clippedRect.right - clippedRect.left;
		int cy = clippedRect.bottom - clippedRect.top;
		int sx = clippedRect.left;
		int sy = clippedRect.top;

		for (int y = 0; y < cy; y++) {

			auto it = dstSurface.Iterator(sx, sy++);
			auto end = it + cx;

			scanlineOp(it, end);

		}

	}

	template< typename DST_SURFACE, typename SCANLINEOP >
	void __forceinline TBitmapScanlineOperation(
		DST_SURFACE * pDstSurface
		, const RECT& rect
		, SCANLINEOP& scanlineOp
		, const RECT* pOptionalClipRect = 0
	)
	{
		TBitmapScanlineOperation(*pDstSurface, rect, scanlineOp, pOptionalClipRect);
	}

	template< typename DST_SURFACE, typename SCANLINEOP >
	void __forceinline TBitmapScanlineOperation(
		DST_SURFACE& dstSurface, SCANLINEOP& scanlineOp
	)
	{
		TBitmapScanlineOperation(dstSurface, dstSurface.Rect(), scanlineOp);
	}

	template< typename DST_SURFACE, typename SCANLINEOP >
	void __forceinline TBitmapScanlineOperation(
		DST_SURFACE* pDstSurface, SCANLINEOP& scanlineOp
	)
	{
		TBitmapScanlineOperation(*pDstSurface, scanlineOp);
	}

	// ---------------------------------------------------------------------------

	//
	//	TOp2ScanlineOPAdaptor
	//

	template< typename DST_SURFACE, typename FN >
	struct TOp2ScanlineOPAdaptor {

		FN& fn;

		TOp2ScanlineOPAdaptor(FN& f) :fn(f) {}

		template<typename I>
		__forceinline void operator()(I first, I last)
		{
			for (; last != first; ++first)
			{
				fn(*first);
			}
		}
	};

	// ---------------------------------------------------------------------------

	//
	//	TBitmapOperation
	//

	template< typename DST_SURFACE, typename FN >
	void __forceinline TBitmapOperation(
		DST_SURFACE& dstSurface,
		const RECT& rect,
		FN& fn,
		const RECT* pOptionalClipRect = 0
	)
	{
		TOp2ScanlineOPAdaptor<DST_SURFACE,FN> scanlineOp(fn);
		TBitmapScanlineOperation(dstSurface, rect, scanlineOp, pOptionalClipRect);
	}

	template< typename DST_SURFACE, typename FN >
	void __forceinline TBitmapOperation(
		DST_SURFACE* pDstSurface,
		const RECT& rect,
		FN& fn,
		const RECT* pOptionalClipRect = 0
	)
	{
		TBitmapOperation(*pDstSurface, rect, fn, pOptionalClipRect);
	}

	template< typename DST_SURFACE, typename FN >
	void __forceinline TBitmapOperation(DST_SURFACE& dstSurface, FN& fn)
	{
		TBitmapOperation(dstSurface, dstSurface.Rect(), fn);
	}

	template< typename DST_SURFACE, typename FN >
	void __forceinline TBitmapOperation(DST_SURFACE * dstSurface, FN& fn)
	{
		TBitmapOperation(*dstSurface, fn);
	}

	// ------------------------------------------------------------------------
	// Texture mapping calculations is the code written by Chris Hecker
	// for the Game developers magazine. I forced the whole thing into a class
	// and made the bitmap handling and rop abstract. (BPT)
	// ------------------------------------------------------------------------

	//
	//	TPerspectiveCorrectTextureMapTriangle
	//

	class TPerspectiveCorrectTextureMapTriangle {

	public:

		struct FVF {

			float X, Y, Z, U, V;

		};
	
	private:

		// --------------------------------------------------------------------

		struct gradients_fl_fl {

			float aOneOverZ[3];				// 1/z for each vertex
			float aUOverZ[3];				// u/z for each vertex
			float aVOverZ[3];				// v/z for each vertex
			float dOneOverZdX, dOneOverZdY;	// d(1/z)/dX, d(1/z)/dY
			float dUOverZdX, dUOverZdY;		// d(u/z)/dX, d(u/z)/dY
			float dVOverZdX, dVOverZdY;		// d(v/z)/dX, d(v/z)/dY

			gradients_fl_fl( FVF const *pVertices ) {

				int Counter;
				
				float OneOverdX = 1 / (((pVertices[1].X - pVertices[2].X) *
						(pVertices[0].Y - pVertices[2].Y)) -
						((pVertices[0].X - pVertices[2].X) *
						(pVertices[1].Y - pVertices[2].Y)));
			
				float OneOverdY = -OneOverdX;
			
				for(Counter = 0;Counter < 3;Counter++)
				{
					float const OneOverZ = 1/pVertices[Counter].Z;
					aOneOverZ[Counter] = OneOverZ;
					aUOverZ[Counter] = pVertices[Counter].U * OneOverZ;
					aVOverZ[Counter] = pVertices[Counter].V * OneOverZ;
				}
			
				dOneOverZdX = OneOverdX * (((aOneOverZ[1] - aOneOverZ[2]) *
						(pVertices[0].Y - pVertices[2].Y)) -
						((aOneOverZ[0] - aOneOverZ[2]) *
						(pVertices[1].Y - pVertices[2].Y)));
				dOneOverZdY = OneOverdY * (((aOneOverZ[1] - aOneOverZ[2]) *
						(pVertices[0].X - pVertices[2].X)) -
						((aOneOverZ[0] - aOneOverZ[2]) *
						(pVertices[1].X - pVertices[2].X)));
			
				dUOverZdX = OneOverdX * (((aUOverZ[1] - aUOverZ[2]) *
						(pVertices[0].Y - pVertices[2].Y)) -
						((aUOverZ[0] - aUOverZ[2]) *
						(pVertices[1].Y - pVertices[2].Y)));
				dUOverZdY = OneOverdY * (((aUOverZ[1] - aUOverZ[2]) *
						(pVertices[0].X - pVertices[2].X)) -
						((aUOverZ[0] - aUOverZ[2]) *
						(pVertices[1].X - pVertices[2].X)));
			
				dVOverZdX = OneOverdX * (((aVOverZ[1] - aVOverZ[2]) *
						(pVertices[0].Y - pVertices[2].Y)) -
						((aVOverZ[0] - aVOverZ[2]) *
						(pVertices[1].Y - pVertices[2].Y)));
				dVOverZdY = OneOverdY * (((aVOverZ[1] - aVOverZ[2]) *
						(pVertices[0].X - pVertices[2].X)) -
						((aVOverZ[0] - aVOverZ[2]) *
						(pVertices[1].X - pVertices[2].X)));

			}

		};

		// --------------------------------------------------------------------

		struct edge_fl_fl {

			float X, XStep;					// fractional x and dX/dY
			int Y, Height;					// current y and vertical count
			float OneOverZ, OneOverZStep;	// 1/z and step
			float UOverZ, UOverZStep;		// u/z and step
			float VOverZ, VOverZStep;		// v/z and step

			__forceinline int Step( void ) {

				X += XStep; Y++; Height--;
				UOverZ += UOverZStep; VOverZ += VOverZStep; OneOverZ += OneOverZStep;
				return Height;

			}
		
			edge_fl_fl(
				gradients_fl_fl const &Gradients, 
				FVF const *pVertices, int Top,
				int Bottom ) {

				Y = (int)ceil(pVertices[Top].Y);
				int YEnd = (int)ceil(pVertices[Bottom].Y);
				Height = YEnd - Y;
			
				float YPrestep = Y - pVertices[Top].Y;
				
				float RealHeight = pVertices[Bottom].Y - pVertices[Top].Y;
				float RealWidth = pVertices[Bottom].X - pVertices[Top].X;
			
				X = ((RealWidth * YPrestep)/RealHeight) + pVertices[Top].X;
				XStep = RealWidth/RealHeight;
				float XPrestep = X - pVertices[Top].X;
			
				OneOverZ = Gradients.aOneOverZ[Top] + YPrestep * Gradients.dOneOverZdY
								+ XPrestep * Gradients.dOneOverZdX;
				OneOverZStep = XStep * Gradients.dOneOverZdX + Gradients.dOneOverZdY;
			
				UOverZ = Gradients.aUOverZ[Top] + YPrestep * Gradients.dUOverZdY
								+ XPrestep * Gradients.dUOverZdX;
				UOverZStep = XStep * Gradients.dUOverZdX + Gradients.dUOverZdY;
			
				VOverZ = Gradients.aVOverZ[Top] + YPrestep * Gradients.dVOverZdY
								+ XPrestep * Gradients.dVOverZdX;
				VOverZStep = XStep * Gradients.dVOverZdX + Gradients.dVOverZdY;

			}

		};
		
		// --------------------------------------------------------------------

		template< typename DST_SURFACE, typename SRC_SURFACE, typename TOP >
		void __forceinline DrawScanLine(
			DST_SURFACE &Dest, gradients_fl_fl const &Gradients,
			edge_fl_fl *pLeft, edge_fl_fl *pRight, SRC_SURFACE &Texture,
			TOP & op, const int leftEdge, const int rightEdge
		)
		{
			int XStart = ceil(pLeft->X);
			int XEnd = ceil(pRight->X);

			// clip the left edge

			if ( 0 > XStart ) {

				XStart = 0;

			}

			int lClip = XStart - leftEdge;

			if ( 0 > lClip ) {

				XStart -= lClip;

				if ( XStart >= XEnd ) {

					return /* nop */;

				}

			}

			// clip the right edge

			if ( rightEdge < XEnd) {

				XEnd = rightEdge;

			}

			int rClip = XEnd - rightEdge;

			if ( 0 < rClip ) {

				XEnd -= rClip;

				if ( XStart >= XEnd ) {

					return /* nop */;

				}

			}

			// calc the operation width and bail if empty

			int Width = (XEnd - XStart);

			if ( 0 >= Width ) {

				return /* nop */;

			}

			DST_SURFACE::pixel_iterator pDestBits = Dest.Iterator( XStart, pLeft->Y );
			SRC_SURFACE::pixel_iterator pTextureBits = Texture.Iterator( 0, 0 );

			long TextureDeltaScan = Texture.Pitch();
			float XPrestep = XStart - pLeft->X;
			float OneOverZ = pLeft->OneOverZ + XPrestep * Gradients.dOneOverZdX;
			float UOverZ = pLeft->UOverZ + XPrestep * Gradients.dUOverZdX;
			float VOverZ = pLeft->VOverZ + XPrestep * Gradients.dVOverZdX;
		
#if defined(STRICT_ENFORCEMENT_OF_TEXTURE_COORDINATES)

			int maxU = Texture.Width() - 1;
			int maxV = Texture.Height() - 1;

#endif

			while( Width-- > 0 ) {

				float Z = 1/OneOverZ;
				int U = UOverZ * Z;
				int V = VOverZ * Z;

#if defined(STRICT_ENFORCEMENT_OF_TEXTURE_COORDINATES)

				U = max( 0, min( U, maxU ) );
				V = max( 0, min( V, maxV ) );

#endif

				op( *pDestBits++, *(SRC_SURFACE::pixel_iterator(
					(BYTE *)(pTextureBits + U) + (V * TextureDeltaScan)))
				);
		
				OneOverZ += Gradients.dOneOverZdX;
				UOverZ += Gradients.dUOverZdX;
				VOverZ += Gradients.dVOverZdX;

			}

		}

		// --------------------------------------------------------------------

	public:

		TPerspectiveCorrectTextureMapTriangle() { /* Empty */ }

		template< typename DST_SURFACE, typename SRC_SURFACE, typename TOP >
		void operator()(
			DST_SURFACE &Dest, FVF const *pVertices,
			SRC_SURFACE &Texture, TOP op,
			const RECT * pClipRect = 0 )
		{

			// Setup the clipping rectangle
			// ----------------------------------------------------------------

			RECT bitmapLimits = Dest.Rect();

			RECT clippedRect;

			if ( pClipRect ) {
	
				if ( !IntersectRect( &clippedRect, &bitmapLimits, pClipRect ) ) {
		
					return /* NOP */;
		
				}

			} else {

				clippedRect = bitmapLimits;

			}

			// ----------------------------------------------------------------

			int Top, Middle, Bottom, MiddleForCompare, BottomForCompare;

			float Y0 = pVertices[0].Y, Y1 = pVertices[1].Y,
							Y2 = pVertices[2].Y;
		
			// sort vertices in y

			if(Y0 < Y1) {
				if(Y2 < Y0) {
					Top = 2; Middle = 0; Bottom = 1;
					MiddleForCompare = 0; BottomForCompare = 1;
				} else {
					Top = 0;
					if(Y1 < Y2) {
						Middle = 1; Bottom = 2;
						MiddleForCompare = 1; BottomForCompare = 2;
					} else {
						Middle = 2; Bottom = 1;
						MiddleForCompare = 2; BottomForCompare = 1;
					}
				}
			} else {
				if(Y2 < Y1) {
					Top = 2; Middle = 1; Bottom = 0;
					MiddleForCompare = 1; BottomForCompare = 0;
				} else {
					Top = 1;
					if(Y0 < Y2) {
						Middle = 0; Bottom = 2;
						MiddleForCompare = 3; BottomForCompare = 2;
					} else {
						Middle = 2; Bottom = 0;
						MiddleForCompare = 2; BottomForCompare = 3;
					}
				}
			}
		
			gradients_fl_fl Gradients(pVertices);
			edge_fl_fl TopToBottom(Gradients,pVertices,Top,Bottom);
			edge_fl_fl TopToMiddle(Gradients,pVertices,Top,Middle);
			edge_fl_fl MiddleToBottom(Gradients,pVertices,Middle,Bottom);
			edge_fl_fl *pLeft, *pRight;
			int MiddleIsLeft;
		
			// the triangle is clockwise, so if bottom > middle then middle is right

			if( BottomForCompare > MiddleForCompare ) {

				MiddleIsLeft = 0;
				pLeft = &TopToBottom; pRight = &TopToMiddle;

			} else {

				MiddleIsLeft = 1;
				pLeft = &TopToMiddle; pRight = &TopToBottom;

			}
		
			int Height = TopToMiddle.Height;
		
			while( Height-- ) {

				if ( (pLeft->Y >= clippedRect.top) && (pLeft->Y < clippedRect.bottom) ) {

					DrawScanLine(
						Dest,Gradients,pLeft,pRight,Texture, op,
						clippedRect.left, clippedRect.right
					);

				}

				TopToMiddle.Step(); TopToBottom.Step();

			}
		
			Height = MiddleToBottom.Height;
		
			if( MiddleIsLeft ) {

				pLeft = &MiddleToBottom; pRight = &TopToBottom;

			} else {

				pLeft = &TopToBottom; pRight = &MiddleToBottom;

			}
			
			while( Height-- ) {

				if ( (pLeft->Y >= clippedRect.top) && (pLeft->Y < clippedRect.bottom) ) {

					DrawScanLine(
						Dest,Gradients,pLeft,pRight,Texture, op,
						clippedRect.left, clippedRect.right
					);

				}

				MiddleToBottom.Step(); TopToBottom.Step();

			}

		}

	};

	// ------------------------------------------------------------------------
	
	//
	//	T_DrawSurfaceToQuad()
	//

	template< typename DST_SURFACE, typename SRC_SURFACE, typename TOP >
	void T_DrawSurfaceToQuad(
		DST_SURFACE & dstSurface, const POINT * pList, 
		SRC_SURFACE & srcSurface, TOP op,
		const RECT * pOptionalClipRect = 0
	) {

		TPerspectiveCorrectTextureMapTriangle triangleRenderer;

		float z = 1.0f;
		float u0 = 0.0f;
		float v0 = 0.0f;

		float u1 = srcSurface.Width();
		float v1 = srcSurface.Height();

#if 1

		// Alternative vertex order.

		TPerspectiveCorrectTextureMapTriangle::FVF verts[ 6 ] = {
			{ pList[3].x, pList[3].y, z, u0, v1 },
			{ pList[0].x, pList[0].y, z, u0, v0 },
			{ pList[1].x, pList[1].y, z, u1, v0 },
			{ pList[3].x, pList[3].y, z, u0, v1 },
			{ pList[1].x, pList[1].y, z, u1, v0 },
			{ pList[2].x, pList[2].y, z, u1, v1 }
		};

		triangleRenderer(
			dstSurface, verts, srcSurface, op, pOptionalClipRect
		);

		triangleRenderer(
			dstSurface, verts + 3, srcSurface, op, pOptionalClipRect
		);

#else

		TPerspectiveCorrectTextureMapTriangle::FVF verts[ 5 ] = {
			{ pList[0].x, pList[0].y, z, u0, v0 },
			{ pList[1].x, pList[1].y, z, u1, v0 },
			{ pList[2].x, pList[2].y, z, u1, v1 },
			{ pList[3].x, pList[3].y, z, u0, v1 },
			{ pList[0].x, pList[0].y, z, u0, v0 }
		};

		triangleRenderer(
			dstSurface, verts, srcSurface, op, pOptionalClipRect
		);

		triangleRenderer(
			dstSurface, verts + 2, srcSurface, op, pOptionalClipRect
		);

#endif

	}


}; // namespace BPT

#endif // !defined(AFX_BPTBLITTER_H__681A3A57_7AAF_4AD7_A5BA_BE2A3BD11191__INCLUDED_)
