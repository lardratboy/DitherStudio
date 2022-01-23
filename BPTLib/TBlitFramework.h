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


#if !defined(AFX_TBLITFRAMEWORK_H__D0FFDBBD_0100_44CA_89B9_27D4A81878D7__INCLUDED_)
#define AFX_TBLITFRAMEWORK_H__D0FFDBBD_0100_44CA_89B9_27D4A81878D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Rectangle.h"
#include "TRops.h"

namespace BPT {

	// ========================================================================

	//
	//	BLITFX
	//

	struct BLITFX {

		enum {

			HFLIP				= 0x00000001
			,VFLIP				= 0x00000002

		};

		DWORD dwFlags;

		BLITFX( const DWORD flags ) : dwFlags( flags ) { /* Empty */ }

	};

	// ========================================================================

	//
	//	T_Blit
	//

	template< class DST_SURFACE, class SRC_SURFACE, class TOP >
	void T_Blit(
		DST_SURFACE & dstSurface
		,const int x
		,const int y
		,SRC_SURFACE & srcSurface
		,TOP op = TOP()
		,const BLITFX * blitFX = 0
		,const SRect * optionalDstClipRect = 0
		,const SRect * optionalSrcSubRect = 0
	) {

// ============================================================================
// EVERYTHING BELOW HERE COULD PROBABLY BE REPRESENTED BY A FUNCTION/CLASS!
// ============================================================================

		// Clip the optional clipping rect to the dest bitmap limits
		// --------------------------------------------------------------------

		SRect dstLimitsRect( dstSurface.Rect() );

		SRect clippedDstRect;

		if ( optionalDstClipRect ) {

			if ( !clippedDstRect.Intersection( &dstLimitsRect, optionalDstClipRect ) ) {

				return /* NOP */;

			}

		} else {

			clippedDstRect = dstLimitsRect;

		}

		// Get the source operation size
		// --------------------------------------------------------------------

		SRect srcLimitsRect( srcSurface.Rect() );

		SRect clippedSrcRect;

		if ( optionalSrcSubRect ) {

			if ( !clippedSrcRect.Intersection( &srcLimitsRect, optionalSrcSubRect ) ) {

				return /* NOP */;

			}

		} else {

			clippedSrcRect = srcLimitsRect;

		}

		// Perform a simple clipping operation to detect NOP
		// --------------------------------------------------------------------

		SRect dstOperation( x, y, clippedSrcRect.Size() );

		SRect clippedRect;
			
		if ( !clippedRect.Intersection( &clippedDstRect, &dstOperation ) ) {

			return /* NOP */;

		}

// ============================================================================
// EVERYTHING ABOVE HERE COULD PROBABLY BE REPRESENTED BY A FUNCTION/CLASS!
// ============================================================================

		// Setup the general loop variables
		// --------------------------------------------------------------------

		const int cx = clippedRect.right - clippedRect.left;
		const int cy = clippedRect.bottom - clippedRect.top;

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

		// Do each vertical line in the clipped operation rectangle
		// The general ddx & ddy could be removed and the code might see a
		// slight improvement in speed.
		// --------------------------------------------------------------------

		if ( 1 == ddx ) {

			for ( int ly = 0; ly < cy; ly++ ) {

				DST_SURFACE::pixel_iterator dstIT = dstSurface.Iterator( dx, dy );
				SRC_SURFACE::pixel_iterator srcIT = srcSurface.Iterator( sx, sy );

				op.InputMajor( srcIT, srcIT + cx, dstIT );
		
				dy += ddy;
				++sy;
	
			}

		} else {

			for ( int ly = 0; ly < cy; ly++ ) {

				DST_SURFACE::reverse_iterator dstIT = dstSurface.rIterator( dx, dy );
				SRC_SURFACE::pixel_iterator srcIT = srcSurface.Iterator( sx, sy );

				op.InputMajor( srcIT, srcIT + cx, dstIT );

				dy += ddy;
				++sy;
	
			}

		}

	}

	// ========================================================================
	//	Blitter boilerplate code
	//
	//	NOTES -- The boilerplate blitters were built to aid the construction
	//		of blitters where the general T_Blit + TransferOP blitter would
	//		perform poorly because of the nature of the transfer operator.
	//
	// ========================================================================

	//
	//	T_ForwardBoilerPlateBlitter
	//

	template<
		class DST_SURFACE
		,class SRC_SURFACE
		,class DP /* = DST_SURFACE::pixel_iterator */
		,class SP /* = SRC_SURFACE::pixel_iterator */
		,class BLITTER
	>
	void T_ForwardBoilerPlateBlitter(
		DST_SURFACE & dstSurface
		,SRC_SURFACE & srcSurface
		,BLITTER blitter = BLITTER()
		,const int x = 0
		,const int y = 0
		,const SRect * optionalDstClipRect = 0
		,const SRect * optionalSrcSubRect = 0
	) {

// ============================================================================
// EVERYTHING BELOW HERE COULD PROBABLY BE REPRESENTED BY A FUNCTION/CLASS!
// ============================================================================

		// Clip the optional clipping rect to the dest bitmap limits
		// --------------------------------------------------------------------

		SRect dstLimitsRect( dstSurface.Rect() );

		SRect clippedDstRect;

		if ( optionalDstClipRect ) {

			if ( !clippedDstRect.Intersection( &dstLimitsRect, optionalDstClipRect ) ) {

				return /* NOP */;

			}

		} else {

			clippedDstRect = dstLimitsRect;

		}

		// Get the source operation size
		// --------------------------------------------------------------------

		SRect srcLimitsRect( srcSurface.Rect() );

		SRect clippedSrcRect;

		if ( optionalSrcSubRect ) {

			if ( !clippedSrcRect.Intersection( &srcLimitsRect, optionalSrcSubRect ) ) {

				return /* NOP */;

			}

		} else {

			clippedSrcRect = srcLimitsRect;

		}

		// Perform a simple clipping operation to detect NOP
		// --------------------------------------------------------------------

		SRect dstOperation( x, y, clippedSrcRect.Size() );

		SRect clippedRect;
			
		if ( !clippedRect.Intersection( &clippedDstRect, &dstOperation ) ) {

			return /* NOP */;

		}

// ============================================================================
// EVERYTHING ABOVE HERE COULD PROBABLY BE REPRESENTED BY A FUNCTION/CLASS!
// ============================================================================

		// Setup the general loop variables
		// --------------------------------------------------------------------

		int cx = clippedRect.right - clippedRect.left;
		int cy = clippedRect.bottom - clippedRect.top;

		int sx = ((clippedRect.left - x) + clippedSrcRect.left);
		int sy = ((clippedRect.top - y) + clippedSrcRect.top);

		int dx = clippedRect.left;
		int dy = clippedRect.top;

		// Call the raw blitter operation, this blitter can assume that the
		// values passed are as vaild as this routine can detect reasonably.
		// --------------------------------------------------------------------

		blitter(
			reinterpret_cast<DP>(dstSurface.Iterator( dx, dy )), dstSurface.Pitch(),
			reinterpret_cast<SP>(srcSurface.Iterator( sx, sy )), srcSurface.Pitch(),
			cx, cy
		);

	}

	// ========================================================================
	// This code should probably be in a file all its own!
	// ========================================================================

	//
	//	T_DecompressorShell()
	//

	template< class DST_SURFACE, class X_SURFACE, class TOP >
	void T_DecompressorShell(
		DST_SURFACE & dstSurface
		,const int x
		,const int y
		,X_SURFACE & srcSurface
		,TOP op = TOP()
		,const BLITFX * blitFX = 0
		,const SRect * optionalDstClipRect = 0
		,const SRect * optionalSrcSubRect = 0
	) {

// ============================================================================
// EVERYTHING BELOW HERE COULD PROBABLY BE REPRESENTED BY A FUNCTION/CLASS!
// ============================================================================

		// Clip the optional clipping rect to the dest bitmap limits
		// --------------------------------------------------------------------

		SRect dstLimitsRect( dstSurface.Rect() );

		SRect clippedDstRect;

		if ( optionalDstClipRect ) {

			if ( !clippedDstRect.Intersection( &dstLimitsRect, optionalDstClipRect ) ) {

				return /* NOP */;

			}

		} else {

			clippedDstRect = dstLimitsRect;

		}

		// Get the source operation size
		// --------------------------------------------------------------------

		SRect srcLimitsRect( srcSurface.Rect() );

		SRect clippedSrcRect;

		if ( optionalSrcSubRect ) {

			if ( !clippedSrcRect.Intersection( &srcLimitsRect, optionalSrcSubRect ) ) {

				return /* NOP */;

			}

		} else {

			clippedSrcRect = srcLimitsRect;

		}

		// Perform a simple clipping operation to detect NOP
		// --------------------------------------------------------------------

		SRect dstOperation( x, y, clippedSrcRect.Size() );

		SRect clippedRect;
			
		if ( !clippedRect.Intersection( &clippedDstRect, &dstOperation ) ) {

			return /* NOP */;

		}

// ============================================================================
// EVERYTHING ABOVE HERE COULD PROBABLY BE REPRESENTED BY A FUNCTION/CLASS!
// ============================================================================

		// Setup the general loop variables
		// --------------------------------------------------------------------

		const int cx = clippedRect.right - clippedRect.left;
		const int cy = clippedRect.bottom - clippedRect.top;

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

		// Skip any vertical lines
		// --------------------------------------------------------------------

		for ( int skip = 0; skip < sy; skip++ ) {

			// do something meaningful here!
			// do something meaningful here!
			// do something meaningful here!
			// do something meaningful here!

		}

		// Do each vertical line in the clipped operation rectangle
		// The general ddx & ddy could be removed and the code might see a
		// slight improvement in speed.
		// --------------------------------------------------------------------

		if ( 1 == ddx ) {

			for ( int ly = 0; ly < cy; ly++ ) {

				DST_SURFACE::pixel_iterator dstIT = dstSurface.Iterator( dx, dy );
				SRC_SURFACE::pixel_iterator srcIT = srcSurface.Iterator( sx, sy );

				op.InputMajor( srcIT, srcIT + cx, dstIT );
		
				dy += ddy;
				++sy;
	
			}

		} else {

			for ( int ly = 0; ly < cy; ly++ ) {

				DST_SURFACE::reverse_iterator dstIT = dstSurface.rIterator( dx, dy );
				SRC_SURFACE::pixel_iterator srcIT = srcSurface.Iterator( sx, sy );

				op.InputMajor( srcIT, srcIT + cx, dstIT );

				dy += ddy;
				++sy;
	
			}

		}

	}

	// ========================================================================
	// This code should probably be in a file all its own!
	// ========================================================================

	// ------------------------------------------------------------------------
	// Texture mapping calculations is the code written by Chris Hecker
	// for the Game developers magazine. I forced the whole thing into a class
	// and made the bitmap handling and rop abstract. (BPT)
	// ------------------------------------------------------------------------

#define STRICT_ENFORCEMENT_OF_TEXTURE_COORDINATES

	//
	//	This is the vertex format
	//

	struct P3D_fl_fl
	{
		float X, Y;
		float Z, U, V;
	};
	
	//
	//	TTextureMapTriangle
	//

	template< class FVF >
	class TTextureMapTriangle {

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

				Y = ceil(pVertices[Top].Y);
				int YEnd = ceil(pVertices[Bottom].Y);
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

		template< class DST_SURFACE, class SRC_SURFACE, class TOP >
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

				op( *pDestBits++, *(DST_SURFACE::pixel_iterator(
					(BYTE *)(pTextureBits + U) + (V * TextureDeltaScan)))
				);
		
				OneOverZ += Gradients.dOneOverZdX;
				UOverZ += Gradients.dUOverZdX;
				VOverZ += Gradients.dVOverZdX;

			}

		}

		// --------------------------------------------------------------------

	public:

		TTextureMapTriangle() { /* Empty */ }

		template< class DST_SURFACE, class SRC_SURFACE, class TOP >
		void operator()(
			DST_SURFACE &Dest, FVF const *pVertices,
			SRC_SURFACE &Texture, TOP op,
			const SRect * pClipRect = 0 )
		{

			// Setup the clipping rectangle
			// ----------------------------------------------------------------

			SRect bitmapLimits( Dest.Rect() );

			SRect clippedRect;

			if ( pClipRect ) {
	
				if ( !clippedRect.Intersection( &bitmapLimits, pClipRect ) ) {
		
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

	template< class DST_SURFACE, class SRC_SURFACE, class TOP >
	void T_DrawSurfaceToQuad(
		DST_SURFACE & dstSurface, const POINT * pList, 
		SRC_SURFACE & srcSurface, TOP op,
		const SRect * pOptionalClipRect = 0
	) {

		TTextureMapTriangle<P3D_fl_fl> triangleRenderer;

		float z = 1.0f;
		float u0 = 0.0f;
		float v0 = 0.0f;

		float u1 = srcSurface.Width();
		float v1 = srcSurface.Height();

		P3D_fl_fl verts[ 5 ] = {
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

	}

	// ------------------------------------------------------------------------

	//
	//	T_ScaleUpBlit
	//
	//	-- This routine could cause a memory 'leak' to be reported at the end
	//	-- of a program, basicly if the tables are indeed cached then there is
	//	-- no way to flush them.  If this was a class things would be different!
	//

	template< class DST_SURFACE, class SRC_SURFACE, class TOP >
	void T_ScaleUpBlit(
		DST_SURFACE & dstSurface
		,const int x
		,const int y
		,SRC_SURFACE & srcSurface
		,const int w
		,const int h
		,TOP op = TOP()
		,const BLITFX * blitFX = 0
		,const SRect * optionalDstClipRect = 0
		,const SRect * optionalSrcSubRect = 0
		,const bool deleteTablesAfterOperation = false
	) {

		// Define the 'type' of the scaling table
		// --------------------------------------------------------------------

		typedef unsigned char scale_entry_type;
		typedef scale_entry_type * scale_table_type;

		typedef TTableDrivenScaleUpOP<
			TOP::dst_type, TOP::src_type, scale_table_type, TOP
		> scale_rop_type;

		// Clip the optional clipping rect to the dest bitmap limits
		// --------------------------------------------------------------------

		SRect dstLimitsRect( dstSurface.Rect() );

		SRect clippedDstRect;

		if ( optionalDstClipRect ) {

			if ( !clippedDstRect.Intersection( &dstLimitsRect, optionalDstClipRect ) ) {

				return /* NOP */;

			}

		} else {

			clippedDstRect = dstLimitsRect;

		}

		// Perform a simple clipping operation to detect NOP
		// --------------------------------------------------------------------

		SRect dstOperation( x, y, x + w, y + h );

		SRect clippedRect;
			
		if ( !clippedRect.Intersection( &clippedDstRect, &dstOperation ) ) {

			return /* NOP */;

		}

		// Get the source operation size
		// --------------------------------------------------------------------

		SRect srcLimitsRect( srcSurface.Rect() );

		SRect clippedSrcRect;

		if ( optionalSrcSubRect ) {

			if ( !clippedSrcRect.Intersection( &srcLimitsRect, optionalSrcSubRect ) ) {

				return /* NOP */;

			}

		} else {

			clippedSrcRect = srcLimitsRect;

		}

		int sw = (clippedSrcRect.right - clippedSrcRect.left);
		int sh = (clippedSrcRect.bottom - clippedSrcRect.top);

		// Make sure we are scaling up!
		// --------------------------------------------------------------------

		if ( (w < sw) || (h < sh) ) {

			return /* NOP */;

		}

		// Calculate the sub pixel coordinates & the operation size
		// --------------------------------------------------------------------

		const int cx = clippedRect.right - clippedRect.left;
		const int cy = clippedRect.bottom - clippedRect.top;

		float hScale = (float)sw / (float)w;
		float vScale = (float)sh / (float)h;

		int a = (clippedRect.left - x);
		int b = (clippedRect.top - y);

		// Setup the scale table and the scale rop
		// --------------------------------------------------------------------

		static scale_table_type pXScaleTable = 0;

		static int cached_w = -1;

		if ( (!pXScaleTable) || (cached_w != w) ) {

			if ( pXScaleTable ) delete [] pXScaleTable;

			pXScaleTable = new scale_entry_type [ w ];

			if ( !pXScaleTable ) {
	
				cached_w = -1;

				return /* error nop */;
	
			}

			cached_w = w;

		}

		static scale_table_type pYScaleTable = 0;

		static int cached_h = -1;

		if ( (!pYScaleTable) || (cached_h != h) ) {

			if ( pYScaleTable ) delete [] pYScaleTable;

			pYScaleTable = new scale_entry_type [ h ];

			if ( !pYScaleTable ) {

				cached_h = -1;
	
				return /* error nop */;
	
			}

			cached_h = h;

		}

		// Build the scale table, which is nothing more than when to increase
		// the source pointer during the blit operation. 
		// --------------------------------------------------------------------

		float fx = 0.0f;

		int lx = 0;

		for ( int i = 0; i < w; i++ ) {

			int ix = (int)fx;

			pXScaleTable[ i ] = lx - ix;

			lx = ix;

			fx += hScale;

		}

		float fy = 0.0f;

		int ly = 0;

		for ( int j = 0; j < h; j++ ) {

			int iy = (int)fy;

			pYScaleTable[ j ] = ly - iy;

			ly = fy;

			fy += vScale;

		}

		// Figure out the 'start' position
		// --------------------------------------------------------------------

		int sx = clippedSrcRect.left;

		for ( i = 0; i < a; i++ ) {

			if ( pXScaleTable[i] ) {

				++sx;

			}

		}

		int sy = clippedSrcRect.top;

		for ( j = 0; j < b; j++ ) {

			if ( pYScaleTable[j] ) {

				++sy;

			}

		}

		// Check for flipping and adjust the dest position and step amount.
		// --------------------------------------------------------------------

		int dx, dy, ddx, ddy;

		if ( blitFX ) {

			if ( BLITFX::HFLIP & blitFX->dwFlags ) {

				// use the scaling table to find the starting sub pixel

				a = w - (cx + a);

				sx = clippedSrcRect.left;
		
				for ( i = 0; i < a; i++ ) {
		
					if ( pXScaleTable[ i ] ) {
		
						++sx;
		
					}
		
				}

				dx = (clippedRect.right - 1);
				ddx = -1;

			} else {

				dx = clippedRect.left;
				ddx = 1;

			}

			if ( BLITFX::VFLIP & blitFX->dwFlags ) {

				// use the scaling table to find the starting sub pixel

				b = h - (cy + b);

				sy = clippedSrcRect.top;
		
				for ( j = 0; j < b; j++ ) {
		
					if ( pYScaleTable[j] ) {
		
						++sy;
		
					}
		
				}

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

		// Setup the scale raster operation
		// --------------------------------------------------------------------

		scale_rop_type scaleOp( pXScaleTable + a, op );

		// Do each vertical line in the clipped operation rectangle
		// The general ddx & ddy could be removed and the code might see a
		// slight improvement in speed.
		// --------------------------------------------------------------------

		scale_table_type pVStep = (pYScaleTable + b);

		if ( 1 == ddx ) {

			for ( int ly = 0; ly < cy; ly++ ) {

				DST_SURFACE::pixel_iterator dstIT = dstSurface.Iterator( dx, dy );
				SRC_SURFACE::pixel_iterator srcIT = srcSurface.Iterator( sx, sy );

				scaleOp.InputMinor( dstIT, dstIT + cx, srcIT );
		
				dy += ddy;

				if ( *pVStep++ ) {

					++sy;

				}
	
			}

		} else {

			for ( int ly = 0; ly < cy; ly++ ) {

				DST_SURFACE::reverse_iterator dstIT = dstSurface.rIterator( dx, dy );
				SRC_SURFACE::pixel_iterator srcIT = srcSurface.Iterator( sx, sy );

				scaleOp.InputMinor( dstIT, dstIT + cx, srcIT );

				dy += ddy;

				if ( *pVStep++ ) {

					++sy;

				}
	
			}

		}

		// Free up our scale tables
		// --------------------------------------------------------------------

		if ( deleteTablesAfterOperation ) {

			delete [] pYScaleTable; 
			delete [] pXScaleTable; 

			pXScaleTable = 0;
			cached_w = -1;

			pYScaleTable = 0;
			cached_h = -1;

		}

	}

}; // namespace BPT

#endif // !defined(AFX_TBLITFRAMEWORK_H__D0FFDBBD_0100_44CA_89B9_27D4A81878D7__INCLUDED_)
