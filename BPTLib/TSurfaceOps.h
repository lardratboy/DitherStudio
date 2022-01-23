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


#if !defined(AFX_TSURFACEOPS_H__F8B12740_CA4D_4FE9_978D_16DD92A3C340__INCLUDED_)
#define AFX_TSURFACEOPS_H__F8B12740_CA4D_4FE9_978D_16DD92A3C340__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ----------------------------------------------------------------------------

#include <math.h>
#include <map>
#include <vector>

#include "Pixels.h"
#include "TRops.h"
#include "Rectangle.h"
#include "TBlitFramework.h"
#include "TPredicate.h"

// ----------------------------------------------------------------------------

namespace BPT {

	// -------------------------------------------------------------------------
	// Query operators
	// -------------------------------------------------------------------------

	//
	//	ReadPixel
	//

	class ReadPixel {

	private:

		int m_X;
		int m_Y;

		ReadPixel(); // Hidden

	public:

		typedef ReadPixel this_type;
		typedef this_type & reference;
		typedef this_type * pointer;
		typedef const this_type & const_reference;
		typedef const this_type * const_pointer;

		// construction
		// --------------------------------------------------------------------

		ReadPixel( const int x, const int y ) : m_X( x ), m_Y( y ) { /* Empty */ }

#if defined( POINT )

		ReadPixel( const POINT & pt ) : m_X( pt.x ), m_Y( pt.y ) { /* Empty */ }

#endif // defined( POINT )

		// Information Set
		// --------------------------------------------------------------------

#if defined( POINT ) // OKAY DESIGN?

		reference operator<<( const POINT & pt ) {

			m_X = pt.x;
			m_Y = pt.y;

			return *this;

		}

#endif // defined( POINT )

		// operation
		// --------------------------------------------------------------------

		template< typename T > typename T::pixel_type operator<<( T & rhs ) {
	
			if ( rhs.ValidLocation( m_X, m_Y ) ) {

				return *rhs.Iterator( m_X, m_Y );

			}

			return T::pixel_type( 0, 0, 0, 0 );
	
		}

	};

	// -------------------------------------------------------------------------
	// Extract a sub rectangle of a bitmap
	// -------------------------------------------------------------------------

	//
	//	CreateBitmapFromRect
	//

	template< class SURFACE >
	class CreateBitmapFromRect {

	private:

		BLITFX m_BlitFX;

		SRect m_SubRect;

		CreateBitmapFromRect(); // Hidden

	public:

		typedef CreateBitmapFromRect<SURFACE> this_type;
		typedef this_type & reference;
		typedef this_type * pointer;
		typedef const this_type & const_reference;
		typedef const this_type * const_pointer;

		// construction
		// --------------------------------------------------------------------

		CreateBitmapFromRect(
			const SRect & rect,
			const BLITFX * blitFX = 0
		) : m_SubRect( rect ), m_BlitFX( 0 ) {

			if ( blitFX ) {

				m_BlitFX = *blitFX;

			}
			
		}

		// operation
		// --------------------------------------------------------------------

		template< class T > SURFACE * operator<<( T & rhs ) {

			// First try to create a T object to house the sub bitmap
			// ----------------------------------------------------------------

			SURFACE * subBitmap = new SURFACE;

			if ( 0 == subBitmap ) {

				return 0;

			}

			// Clip the capture rectangle to the bitmap limits
			// ----------------------------------------------------------------

			SRect bitmapLimits( rhs.Rect() );

			SRect captureRect;
	
			if ( !captureRect.Intersection( &bitmapLimits, &m_SubRect ) ) {
	
				return subBitmap;
	
			}

			// Size the sub bitmap to fit the capture rectangle
			// ----------------------------------------------------------------

			if ( !subBitmap->Create( captureRect.Size() ) ) {

				delete subBitmap;
				return 0;

			}

			// Blit the contents from the source bitmap to the output bitmap
			// ----------------------------------------------------------------

			T_Blit(
				*subBitmap, 0, 0, rhs, 
				TCopyOP<SURFACE::pixel_type,T::pixel_type>(),
				&m_BlitFX, 0, &captureRect
			);

			return subBitmap;

		}

	};

	// -------------------------------------------------------------------------
	// Extract a sub rectangle of a bitmap
	// -------------------------------------------------------------------------

	//
	//	CreateScaledBitmap
	//

	template< class SURFACE >
	class CreateScaledBitmap {

	private:

		SIZE m_Size;

		CreateScaledBitmap(); // Hidden

	public:

		typedef CreateScaledBitmap<SURFACE> this_type;
		typedef this_type & reference;
		typedef this_type * pointer;
		typedef const this_type & const_reference;
		typedef const this_type * const_pointer;

		// construction
		// --------------------------------------------------------------------

		CreateScaledBitmap( const SIZE & size ) : m_Size( size ) {

			/* Empty */
			
		}

		// operation
		// --------------------------------------------------------------------

		template< class T > SURFACE * operator<<( T & rhs ) {

			// First try to create a T object to house the sub bitmap
			// ----------------------------------------------------------------

			SURFACE * subBitmap = new SURFACE;

			if ( 0 == subBitmap ) {

				return 0;

			}

			// Size the sub bitmap to fit the capture rectangle
			// ----------------------------------------------------------------

			if ( !subBitmap->Create( m_Size ) ) {

				delete subBitmap;
				return 0;

			}

			// 'draw' the contents from the source bitmap to the output bitmap
			// ----------------------------------------------------------------

			POINT scaledPoints[ 4 ] = {
				{ 0, 0 },
				{ m_Size.cx - 1, 0 },
				{ m_Size.cx - 1, m_Size.cy -1 },
				{ 0, m_Size.cy - 1 }
			};

			T_DrawSurfaceToQuad(
				*subBitmap, scaledPoints, rhs,
				TCopyOP<SURFACE::pixel_type,T::pixel_type>()
			);

			return subBitmap;

		}

	};

	// -------------------------------------------------------------------------
	// Extract a sub rectangle of a bitmap
	// -------------------------------------------------------------------------

	//
	//	CreateScaledAndRotatedBitmap
	//

	template< typename SURFACE >
	class CreateScaledAndRotatedBitmap {

	private:

		typename SURFACE::pixel_type m_FillColor;

		float m_XScale;
		float m_YScale;
		float m_Angle;

		CreateScaledAndRotatedBitmap(); // Hidden

	public:

		typedef CreateScaledAndRotatedBitmap<SURFACE> this_type;
		typedef this_type & reference;
		typedef this_type * pointer;
		typedef const this_type & const_reference;
		typedef const this_type * const_pointer;

		// construction
		// --------------------------------------------------------------------

		CreateScaledAndRotatedBitmap(
			const float angleInDegrees,
			const typename SURFACE::pixel_type fillColor,
			const float xScale = 1.0f, 
			const float yScale = 1.0f
		) : m_Angle( angleInDegrees ), m_FillColor( fillColor ),
			m_XScale( xScale ), m_YScale( yScale ) {

			/* Empty */
			
		}

		// operation
		// --------------------------------------------------------------------

		template< class T > SURFACE * operator<<( T & rhs ) {

			// First try to create a T object to house the sub bitmap
			// ----------------------------------------------------------------

			SURFACE * subBitmap = new SURFACE;

			if ( 0 == subBitmap ) {

				return 0;

			}

			// Create the scaled & rotated points
			// ----------------------------------------------------------------

			int w = (int)(rhs.Width() * m_XScale + 0.5f);
			int h = (int)(rhs.Height() * m_XScale + 0.5f);

			if ( (0 >= w) || (0 >= h) ) {

				delete subBitmap;
				return 0;

			}

			int x = -w/2;
			int y = -h/2;

			POINT newPoints[ 4 ] = {
				{ x, y },
				{ x + w - 1, y },
				{ x + w - 1, y + h - 1 },
				{ x, y + h - 1 }
			};

			float angleInRadians = (3.141592654f * m_Angle) / 180.0f;

			float ca = cosf( angleInRadians );
			float sa = sinf( angleInRadians );

			POINT shift = { 0, 0 };

			for ( int j = 0; j < 4; j++ ) {

				int tx = newPoints[ j ].x;
				int ty = newPoints[ j ].y;

				// Need to write the rotation code.

				newPoints[ j ].x = (int)(tx * ca - ty * sa);
				newPoints[ j ].y = (int)(tx * sa + ty * ca);

				shift.x = min( newPoints[ j ].x, shift.x );
				shift.y = min( newPoints[ j ].y, shift.y );

			}

			for ( j = 0; j < 4; j++ ) {

				newPoints[ j ].x -= shift.x;
				newPoints[ j ].y -= shift.y;

			}

			// Size the sub bitmap to fit the transformed bitmap
			// ----------------------------------------------------------------

			SIZE newSize = { 0, 0 };

			for ( int i = 0; i < 4; i++ ) {

				newSize.cx = max( newSize.cx, (newPoints[i].x + 1) );
				newSize.cy = max( newSize.cy, (newPoints[i].y + 1) );

			}

			if ( !subBitmap->Create( newSize ) ) {

				delete subBitmap;
				return 0;

			}

			// 'draw' the contents from the source bitmap to the output bitmap
			// ----------------------------------------------------------------

			*subBitmap << ClearSurface( m_FillColor );

			T_DrawSurfaceToQuad(
				*subBitmap, newPoints, rhs,
				TCopyOP<SURFACE::pixel_type,T::pixel_type>()
			);

			return subBitmap;

		}

	};

	// -------------------------------------------------------------------------
	// Create A 'Mapped' Bitmap from an existing one
	// -------------------------------------------------------------------------

	//
	//	CreateMappedBitmapFromRect
	//

	class CreateMappedBitmapFromRect {

	private:

		SRect m_SubRect;

		CreateMappedBitmapFromRect(); // Hidden

	public:

		typedef CreateMappedBitmapFromRect this_type;
		typedef this_type & reference;
		typedef this_type * pointer;
		typedef const this_type & const_reference;
		typedef const this_type * const_pointer;

		// construction
		// --------------------------------------------------------------------

		CreateMappedBitmapFromRect( const SRect & rect ) : m_SubRect( rect ) {

			/* Empty */

		}

		// operation
		// --------------------------------------------------------------------

		template< class T > T * operator<<( T & rhs ) {

			// First try to create a T object to house the sub bitmap
			// ----------------------------------------------------------------

			T * subBitmap = new T;

			if ( 0 == subBitmap ) {

				return 0;

			}

			// Clip the capture rectangle to the bitmap limits
			// ----------------------------------------------------------------

			SRect bitmapLimits( rhs.Rect() );

			SRect captureRect;
	
			if ( !captureRect.Intersection( &bitmapLimits, &m_SubRect ) ) {
	
				return subBitmap;
	
			}

			// Map the existing data to the new bitmap :)
			// ----------------------------------------------------------------

			subBitmap->Map(
				static_cast<void *>( rhs.Iterator( captureRect.left, captureRect.top ) ),
				captureRect.Width(), captureRect.Height(), rhs.Pitch(), false, false
			);

			return subBitmap;

		}

	};

	// -------------------------------------------------------------------------
	// Bounding rectangle search
	// -------------------------------------------------------------------------

	//
	//	TFindBoundingRect
	//

	template< class UnaryPredicate >
	class TFindBoundingRect {

	private:

		UnaryPredicate m_Predicate;
		bool m_UseSubRect;
		SRect m_SubRect;

		TFindBoundingRect(); // Hidden

		// Information Set
		// --------------------------------------------------------------------

		void SetRect( const SRect * pOptionalSubRect ) {

			if ( pOptionalSubRect ) {

				m_SubRect = *pOptionalSubRect;
				m_UseSubRect = true;

			} else {

				m_UseSubRect = false;

			}

		}

	public:

		typedef TFindBoundingRect<UnaryPredicate> this_type;
		typedef this_type & reference;
		typedef this_type * pointer;
		typedef const this_type & const_reference;
		typedef const this_type * const_pointer;

		// construction
		// --------------------------------------------------------------------

		TFindBoundingRect(
			const UnaryPredicate & predicate,
			const SRect * pOptionalSubRect = 0 ) : 
			m_Predicate( predicate ) {

			SetRect( pOptionalSubRect );

		}

		// operation
		// --------------------------------------------------------------------

		template< class T > SRect operator<<( T & rhs ) {

			// Clip the operation to be within the bitmap limits
			// ----------------------------------------------------------------

			SRect bitmapLimits( rhs.Rect() );

			SRect clippedRect;

			if ( m_UseSubRect ) {

				if ( !clippedRect.Intersection( &bitmapLimits, &m_SubRect ) ) {
		
					return SRect( 0, 0, 0, 0 );
		
				}

			} else {

				clippedRect = bitmapLimits;

			}
	
			// Perform the operation
			// ----------------------------------------------------------------

			int cx = clippedRect.Width();
			int cy = clippedRect.Height();
			int sx = clippedRect.left;
			int sy = clippedRect.top;

			// Perform the operation
			// ----------------------------------------------------------------

			int x1 = cx;
			int y1 = cy;
			int x2 = 0;
			int y2 = 0;

			for ( int y = 0; y < cy; y++ ) {

				T::pixel_iterator it = rhs.Iterator( sx, sy++ );

				for ( int x = 0; x < cx; x++ ) {

					if ( m_Predicate( *it++ ) ) {

						if ( x < x1 ) x1 = x;
						if ( x > x2 ) x2 = x;
						if ( y < y1 ) y1 = y;
						if ( y > y2 ) y2 = y;

					}

				}

			}

			if ( (x1 <= x2) && (y1 <= y2) ) {

				return SRect(
					(clippedRect.left + x1), 
					(clippedRect.top + y1), 
					(clippedRect.left + x2 + 1), 
					(clippedRect.top + y2 + 1)
				);

			}

			return SRect( 0, 0, 0, 0 );

		}

	};

	// ------------------------------------------------------------------------

	//
	//	FindBoundingRect()
	//

	template< class P >	__inline
	TFindBoundingRect<P> FindBoundingRect( P color, const SRect * pRect = 0 ) {

		return TFindBoundingRect< TIsNotValue<P> >( TIsNotValue<P>( color ), pRect );

	}

	// -------------------------------------------------------------------------
	//	Create bitmap from bounding rectangle
	// -------------------------------------------------------------------------

	//
	//	TCreateBitmapFromBoundingRect
	//

	template< class UnaryPredicate, class P = POINT >
	class TCreateBitmapFromBoundingRect {

	private:

		UnaryPredicate m_Predicate;
		bool m_CreateMappedBitmap;
		bool m_UseSubRect;
		SRect m_SubRect;

		TCreateBitmapFromBoundingRect(); // Hidden

		// Information Set
		// --------------------------------------------------------------------

		void SetRect( const SRect * pOptionalSubRect ) {

			if ( pOptionalSubRect ) {

				m_SubRect = *pOptionalSubRect;
				m_UseSubRect = true;

			} else {

				m_UseSubRect = false;

			}

		}

	public:

		typedef TCreateBitmapFromBoundingRect<UnaryPredicate> this_type;
		typedef this_type & reference;
		typedef this_type * pointer;
		typedef const this_type & const_reference;
		typedef const this_type * const_pointer;

		// construction
		// --------------------------------------------------------------------

		TCreateBitmapFromBoundingRect(
			const UnaryPredicate & predicate,
			const bool createMappedBitmap = false,
			const SRect * pOptionalSubRect = 0 ) : 
			m_CreateMappedBitmap( createMappedBitmap ), m_Predicate( predicate ) {

			SetRect( pOptionalSubRect );

		}

		// operation
		// --------------------------------------------------------------------

		template< class T > struct result_type : public std::pair< T *, P > {};

		template< class T > result_type<T> * operator<<( T & rhs ) {

			// find the bound rectangle
			// ----------------------------------------------------------------

			SRect boundingRect;

			if ( m_UseSubRect ) {

				boundingRect = TFindBoundingRect<UnaryPredicate>( m_Predicate, &m_SubRect ) << rhs;

			} else {

				boundingRect = TFindBoundingRect<UnaryPredicate>( m_Predicate ) << rhs;

			}

			// Check for 'empty' bounding rect
			// ----------------------------------------------------------------

			if ( boundingRect.Empty() ) { return 0; }

			// Now do the 'bitmap'
			// ----------------------------------------------------------------

			T * subBitmap;

			if ( m_CreateMappedBitmap ) {

				subBitmap = CreateMappedBitmapFromRect( boundingRect ) << rhs;

			} else {

				subBitmap = CreateBitmapFromRect<T>( boundingRect ) << rhs;

			}

			// check for a failed 'create'
			// ----------------------------------------------------------------

			if ( !subBitmap ) { return 0; }

			// Okay create the 'result' type if not sucessful delete the bitmap
			// ----------------------------------------------------------------

			P location = { boundingRect.left, boundingRect.top };

			result_type<T> * result = new result_type<T>();

			if ( result ) {

				result->first = subBitmap;
				result->second = location;

				return result;

			}

			// Cleanup and return failure
			// ----------------------------------------------------------------

			delete subBitmap;
			return 0;

		}

	};

	// ------------------------------------------------------------------------

	//
	//	CreateBoundedBitmap()
	//

	template< class UnaryPredicate > __inline
	TCreateBitmapFromBoundingRect<UnaryPredicate> CreateBoundedBitmap(
		UnaryPredicate predicate, const bool createMapped = false, const SRect * pRect = 0 ) {

		return TCreateBitmapFromBoundingRect< UnaryPredicate >(
			predicate, createMapped, pRect
		);

	}

	//
	//	CreateChromakeyedBoundedBitmap()
	//

	template< class P > __inline
	TCreateBitmapFromBoundingRect< TIsNotValue<P> > CreateChromakeyedBoundedBitmap(
		const P & color, const bool createMapped = false, const SRect * pRect = 0 ) {

		return CreateBoundedBitmap( TIsNotValue<P>( color ), createMapped, pRect );

	}

	//
	//	CreateBoundedBitmapUsingAlpha()
	//

	template< class P > __inline
	TCreateBitmapFromBoundingRect< TNonZeroAlpha<P> > CreateBoundedBitmapUsingAlpha(
		const P & color, const bool createMapped = false, const SRect * pRect = 0 ) {

		return CreateBoundedBitmap( TNonZeroAlpha<P>(), createMapped, pRect );

	}

	// -------------------------------------------------------------------------
	//	Rectangle predicate test's
	// -------------------------------------------------------------------------

	//
	//	TDoesEntireRectSatisfyPredicate<>
	//

	template< class UnaryPredicate >
	class TDoesEntireRectSatisfyPredicate {

	private:

		UnaryPredicate m_Predicate;
		SRect m_SearchRect;

		TDoesEntireRectSatisfyPredicate(); // Hidden

	public:

		typedef TDoesEntireRectSatisfyPredicate<UnaryPredicate> this_type;
		typedef this_type & reference;
		typedef this_type * pointer;
		typedef const this_type & const_reference;
		typedef const this_type * const_pointer;

		// construction
		// --------------------------------------------------------------------

		TDoesEntireRectSatisfyPredicate( const UnaryPredicate & predicate, const SRect & rect ) : 
			m_SearchRect( rect ), m_Predicate( predicate ) { /* Empty */ }

		// operation
		// --------------------------------------------------------------------

		template< class T > bool operator<<( T & rhs ) {

			// Clip the operation to be within the bitmap limits
			// ----------------------------------------------------------------

			SRect bitmapLimits( rhs.Rect() );

			SRect clippedRect;
	
			if ( !clippedRect.Intersection( &bitmapLimits, &m_SearchRect ) ) {
	
				return false;
	
			}
	
			// Perform the operation
			// ----------------------------------------------------------------

			int cx = clippedRect.Width();
			int cy = clippedRect.Height();
			int sx = clippedRect.left;
			int sy = clippedRect.top;

			T::pixel_color searchColor = m_SearchColor;

			for ( int y = 0; y < cy; y++ ) {

				T::pixel_iterator it = rhs.Iterator( sx, sy++ );

				for ( int x = 0; x < cx; x++ ) {

					if ( !m_Predicate( *it++ ) ) {

						return false;

					}

				}

			}

			return true;

		}

	};

	//
	//	TDoesRectSatisfyPredicate<>
	//

	template< class UnaryPredicate >
	class TDoesRectSatisfyPredicate {

	private:

		UnaryPredicate m_Predicate;
		SRect m_SearchRect;

		TDoesRectSatisfyPredicate(); // Hidden

	public:

		typedef TDoesRectSatisfyPredicate<UnaryPredicate> this_type;
		typedef this_type & reference;
		typedef this_type * pointer;
		typedef const this_type & const_reference;
		typedef const this_type * const_pointer;

		// construction
		// --------------------------------------------------------------------

		TDoesRectSatisfyPredicate( const UnaryPredicate & predicate, const SRect & rect ) : 
			m_SearchRect( rect ), m_Predicate( predicate ) { /* Empty */ }

		// operation
		// --------------------------------------------------------------------

		template< class T > bool operator<<( T & rhs ) {

			// Clip the operation to be within the bitmap limits
			// ----------------------------------------------------------------

			SRect bitmapLimits( rhs.Rect() );

			SRect clippedRect;
	
			if ( !clippedRect.Intersection( &bitmapLimits, &m_SearchRect ) ) {
	
				return false;
	
			}
	
			// Perform the operation
			// ----------------------------------------------------------------

			int cx = clippedRect.Width();
			int cy = clippedRect.Height();
			int sx = clippedRect.left;
			int sy = clippedRect.top;

			for ( int y = 0; y < cy; y++ ) {

				T::pixel_iterator it = rhs.Iterator( sx, sy++ );

				for ( int x = 0; x < cx; x++ ) {

					if ( m_Predicate( *it++ ) ) {

						return true;

					}

				}

			}

			return false;

		}

	};

	// ------------------------------------------------------------------------

	//
	//	IsRectSingleColor()
	//

	template< class P >	__inline
	TDoesEntireRectSatisfyPredicate< TIsValue<P> >
	IsRectSingleColor( const SRect & rect, P color ) {

		return TDoesEntireRectSatisfyPredicate< TIsValue<P> >( TIsValue<P>( color ), rect );

	}

	//
	//	DoesRectUseColor()
	//

	template< class P >	__inline
	TDoesRectSatisfyPredicate< TIsValue<P> >
	DoesRectUseColor( const SRect & rect, P color ) {

		return TDoesRectSatisfyPredicate< TIsValue<P> >( TIsValue<P>( color ), rect );

	}

	// -------------------------------------------------------------------------
	// Create histogram 
	// -------------------------------------------------------------------------

	//
	//	CreateHistogram
	//

	class CreateHistogram {

	private:

		bool m_UseSubRect;
		SRect m_SubRect;

		// Information Set
		// --------------------------------------------------------------------

		void SetRect( const SRect * pOptionalSubRect ) {

			if ( pOptionalSubRect ) {

				m_SubRect = *pOptionalSubRect;
				m_UseSubRect = true;

			} else {

				m_UseSubRect = false;

			}

		}

	public:

		typedef CreateHistogram this_type;
		typedef this_type & reference;
		typedef this_type * pointer;
		typedef const this_type & const_reference;
		typedef const this_type * const_pointer;

		// construction
		// --------------------------------------------------------------------

		CreateHistogram( const SRect * pOptionalSubRect = 0 ) {

			SetRect( pOptionalSubRect );

		}

		// operation
		// --------------------------------------------------------------------

		template< typename T > struct result_type : public std::map<typename T::pixel_type,int> {};

		template< typename T > result_type<T> * operator<<( T & rhs ) {

			// try to create the histogram for this pixel type
			// ----------------------------------------------------------------

			result_type<T> * pHistogram = new result_type<T>();

			if ( 0 == pHistogram ) {

				return 0;

			}

			// Clip the operation to be within the bitmap limits
			// ----------------------------------------------------------------

			SRect bitmapLimits( rhs.Rect() );

			SRect clippedRect;

			if ( m_UseSubRect ) {

				if ( !clippedRect.Intersection( &bitmapLimits, &m_SubRect ) ) {
		
					return pHistogram;
		
				}

			} else {

				clippedRect = bitmapLimits;

			}
	
			// Perform the operation
			// ----------------------------------------------------------------

			int cx = clippedRect.Width();
			int cy = clippedRect.Height();
			int sx = clippedRect.left;
			int sy = clippedRect.top;

			// Perform the operation
			// ----------------------------------------------------------------

			for ( int y = 0; y < cy; y++ ) {

				T::pixel_iterator it = rhs.Iterator( sx, sy++ );

				for ( int x = 0; x < cx; x++ ) {

					T::pixel_type color = *it++;

					if ( pHistogram->find( color ) != pHistogram->end() ) {

						(*pHistogram)[ color ]++;

					} else {

						(*pHistogram)[ color ] = 1;

					}

				}

			}

			return pHistogram;

		}

	};

	// -------------------------------------------------------------------------
	// Pixel collection
	// -------------------------------------------------------------------------

	//
	//	PixelCollectionFromBitmap
	//

	class PixelCollectionFromBitmap {

	private:

		bool m_UseSubRect;
		SRect m_SubRect;

		// Information Set
		// --------------------------------------------------------------------

		void SetRect( const SRect * pOptionalSubRect ) {

			if ( pOptionalSubRect ) {

				m_SubRect = *pOptionalSubRect;
				m_UseSubRect = true;

			} else {

				m_UseSubRect = false;

			}

		}

	public:

		typedef PixelCollectionFromBitmap this_type;
		typedef this_type & reference;
		typedef this_type * pointer;
		typedef const this_type & const_reference;
		typedef const this_type * const_pointer;

		// construction
		// --------------------------------------------------------------------

		PixelCollectionFromBitmap( const SRect * pOptionalSubRect = 0 ) {

			SetRect( pOptionalSubRect );

		}

		// operation
		// --------------------------------------------------------------------

		template< typename T > std::vector<typename T::pixel_type> * operator<<( T & rhs ) {

			// try to create the histogram for this run
			// ----------------------------------------------------------------

			typedef std::vector<typename T::pixel_type> PIXEL_COLLECTION;

			PIXEL_COLLECTION * pCollection = new PIXEL_COLLECTION();

			if ( 0 == pCollection ) {

				return 0;

			}

			// Clip the operation to be within the bitmap limits
			// ----------------------------------------------------------------

			SRect bitmapLimits( rhs.Rect() );

			SRect clippedRect;

			if ( m_UseSubRect ) {

				if ( !clippedRect.Intersection( &bitmapLimits, &m_SubRect ) ) {
		
					return pCollection;
		
				}

			} else {

				clippedRect = bitmapLimits;

			}
	
			// Perform the operation
			// ----------------------------------------------------------------

			int cx = clippedRect.Width();
			int cy = clippedRect.Height();
			int sx = clippedRect.left;
			int sy = clippedRect.top;

			// In an effort to make this process a little more efficient
			// reserve enough space in the collection for width * height pixels
			// ----------------------------------------------------------------

			pCollection->reserve( cx * cy );

			// Perform the operation
			// ----------------------------------------------------------------

			for ( int y = 0; y < cy; y++ ) {

				T::pixel_iterator it = rhs.Iterator( sx, sy++ );

				for ( int x = 0; x < cx; x++ ) {

					pCollection->push_back( *it++ );

				}

			}

			return pCollection;

		}

	};

	// -------------------------------------------------------------------------
	// FUTURE
	// -------------------------------------------------------------------------

	// more more more
	// more more more
	// more more more

	// -------------------------------------------------------------------------
	// Single pixel
	// -------------------------------------------------------------------------

	//
	//	TDrawPixel<>
	//
	
	template<
		class PIXEL_TYPE = PIXEL_RAW_ARGB_TYPE,
		class TRANSFER_OP = TCopyOP<PIXEL_TYPE>
	>
	class TDrawPixel {
	
	private:

		PIXEL_TYPE m_Value;
		TRANSFER_OP m_Rop;
	
		int m_X;
		int m_Y;

		TDrawPixel(); // Hidden
	
	public:

		// construction
		// --------------------------------------------------------------------
	
		TDrawPixel(
			const int x, const int y, const PIXEL_TYPE value,
			TRANSFER_OP rop = TRANSFER_OP() ) : 
			m_X( x ), m_Y( y ), m_Value( value), m_Rop( rop ) { /* Empty */ }

#if defined( POINT )

		TDrawPixel( const POINT & pt, const PIXEL_TYPE value,
			TRANSFER_OP rop = TRANSFER_OP() ) :
			m_X( pt.x ), m_Y( pt.y ), m_Value( value), m_Rop( rop { /* Empty */ }

#endif

		// operation
		// --------------------------------------------------------------------

		template< class T > void operator()( T & rhs ) {

			if ( rhs.ValidLocation( m_X, m_Y ) ) {

				m_Rop( *rhs.Iterator( m_X, m_Y ), m_Value );

			}
	
		}
	
	};

	// ------------------------------------------------------------------------

	//
	//	DrawPixel()
	//

	template< class P >	__inline
	TDrawPixel<P> DrawPixel( const int x, const int y, const P color ) {

		return TDrawPixel<P>( x, y, color );

	}

	//
	//	DrawPixel()
	//

	template< class P, class T > __inline	
	TDrawPixel<P,T> DrawPixel( const int x, const int y, const P color, T & rop ) {

		return TDrawPixel<P,T>( x, y, color, rop );

	}

	//
	//	DrawPixel()
	//

#if defined( POINT )

	template< class P >	__inline
	TDrawPixel<P> DrawPixel( const POINT & pt, P color ) {

		return TDrawPixel<P>( pt, color );

	}

#endif // defined( POINT )

	//
	//	DrawPixel()
	//

#if defined( POINT )

	template< class P, class T > __inline	
	TDrawPixel<P,T> DrawPixel( const POINT & pt, P color, T & rop ) {

		return TDrawPixel<P,T>( pt, color, rop );

	}

#endif // defined( POINT )

	// -------------------------------------------------------------------------
	// Solid rectangle 
	// -------------------------------------------------------------------------

	//
	//	TDrawSolidRect<>
	//
	
	template<
		class PIXEL_TYPE = PIXEL_RAW_ARGB_TYPE,
		class TRANSFER_OP = TCopyOP<PIXEL_TYPE>
	>
	class TDrawSolidRect {
	
	private:

		PIXEL_TYPE m_Value;
		TRANSFER_OP m_Rop;
		SRect m_Rect;
	
		TDrawSolidRect(); // Hidden
	
	public:

		// construction
		// --------------------------------------------------------------------
	
		TDrawSolidRect( const SRect & rect, const PIXEL_TYPE value,
			TRANSFER_OP rop = TRANSFER_OP() ) :
			m_Rect( rect ), m_Value( value), m_Rop( rop ) { /* Empty */ }

		// operation
		// --------------------------------------------------------------------

		template< class T > void __forceinline operator()( T & rhs ) {

			// Clip the operation to be within the bitmap limits
			// ----------------------------------------------------------------

			SRect bitmapLimits( rhs.Rect() );

			SRect clippedRect;
	
			if ( !clippedRect.Intersection( &bitmapLimits, &m_Rect ) ) {
	
				return /* NOP */;
	
			}
	
			// Perform the operation
			// ----------------------------------------------------------------

			int cx = clippedRect.Width();
			int cy = clippedRect.Height();
			int sx = clippedRect.left;
			int sy = clippedRect.top;

			for ( int y = 0; y < cy; y++ ) {

				T::pixel_iterator it = rhs.Iterator( sx, sy++ );

				m_Rop.OutputMajor( it, it + cx, m_Value );

			}

		}
	
	};

	// ------------------------------------------------------------------------

	//
	//	DrawSolidRect
	//

	template< class P >	__inline
	TDrawSolidRect<P> DrawSolidRect( const SRect & rect, P color ) {

		return TDrawSolidRect<P>( rect, color );

	}

	//
	//	DrawSolidRect
	//

	template< class P, class T > __inline	
	TDrawSolidRect<P,T> DrawSolidRect( const SRect & rect, P color, T & rop ) {

		return TDrawSolidRect<P,T>( rect, color, rop );

	}

	// -------------------------------------------------------------------------
	// rectangle 
	// -------------------------------------------------------------------------

	//
	//	TDrawRect<>
	//
	
	template<
		class PIXEL_TYPE = PIXEL_RAW_ARGB_TYPE,
		class TRANSFER_OP = TCopyOP<PIXEL_TYPE>
	>
	class TDrawRect {
	
	private:

		PIXEL_TYPE m_Value;
		TRANSFER_OP m_Rop;
		SRect m_Rect;
	
		TDrawRect(); // Hidden
	
	public:

		// construction
		// --------------------------------------------------------------------
	
		TDrawRect( const SRect & rect, const PIXEL_TYPE value,
			TRANSFER_OP rop = TRANSFER_OP() ) :
			m_Rect( rect ), m_Value( value), m_Rop( rop ) {

			/* Empty */

		}
				
		// operation
		// --------------------------------------------------------------------

		template< class T > void operator()( T & rhs ) {

			rhs << DrawSolidRect(
				SRect( m_Rect.left, m_Rect.top, m_Rect.right, m_Rect.top + 1 ), m_Value
			);

			rhs << DrawSolidRect(
				SRect( m_Rect.left, (m_Rect.bottom - 1), m_Rect.right, m_Rect.bottom ), m_Value
			);

			rhs << DrawSolidRect(
				SRect( m_Rect.left, m_Rect.top, m_Rect.left + 1, m_Rect.bottom ), m_Value
			);

			rhs << DrawSolidRect(
				SRect( (m_Rect.right - 1), m_Rect.top, m_Rect.right, m_Rect.bottom ), m_Value
			);

		}
	
	};

	// ------------------------------------------------------------------------

	//
	//	DrawRect
	//

	template< class P >	__inline
	TDrawRect<P> DrawRect( const SRect & rect, const P color ) {

		return TDrawRect<P>( rect, color );

	}

	//
	//	DrawRect
	//

	template< class P, class T > __inline	
	TDrawRect<P,T> DrawRect( const SRect & rect, const P color, T & rop ) {

		return TDrawRect<P,T>( rect, color, rop );

	}

	// -------------------------------------------------------------------------
	// rectangle 
	// -------------------------------------------------------------------------

	//
	//	TClearSurface<>
	//
	
	template<
		class PIXEL_TYPE = PIXEL_RAW_ARGB_TYPE
	>
	class TClearSurface {
	
	private:

		PIXEL_TYPE m_Value;
	
		TClearSurface(); // Hidden
	
	public:

		// construction
		// --------------------------------------------------------------------
	
		TClearSurface( const PIXEL_TYPE value ) : m_Value( value ) {

			/* Empty */

		}
				
		// operation
		// --------------------------------------------------------------------

		template< class T > void operator()( T & rhs ) {

			DrawSolidRect( rhs.Rect(), m_Value );

		}
	
	};

	// ------------------------------------------------------------------------

	//
	//	ClearSurface()
	//

	template< class P >	__inline
	TClearSurface<P> ClearSurface( P color ) {

		return TClearSurface<P>( color );

	}

	// ------------------------------------------------------------------------
	// Draw A line primitive (uses a class for the renderer)
	// ------------------------------------------------------------------------

	//
	//	TPrimitiveLine<>
	//

	template< class PRIMITIVE, class COORDINATE = POINT >
	class TPrimitiveLine {

	private:

		COORDINATE m_A;
		COORDINATE m_B;

		PRIMITIVE m_Primitive;

		int __forceinline ABS( const int a ) {

			return (0 > (a)) ? (-a) : (a);

		}

		int __forceinline SGN( const int a ) {

			return (0 > (a)) ? (-1) : (1);

		}

	public:

		typedef TPrimitiveLine<PRIMITIVE,COORDINATE> this_type;
		typedef this_type & reference;
		typedef this_type * pointer;
		typedef const this_type & const_reference;
		typedef const this_type * const_pointer;
		typedef PRIMITIVE primitive_type;

		// construction
		// --------------------------------------------------------------------

		TPrimitiveLine(
			const COORDINATE & a, const COORDINATE & b,
			PRIMITIVE primitive = PRIMITIVE()
		) : m_A( a ), m_B( b ), m_Primitive( primitive ) {

			/* Empty */

		}

		// operation (Based on Bresenham's algorithm)
		// --------------------------------------------------------------------

		template< class T > void operator()( T & rhs ) {

			int x1 = m_A.x;
			int y1 = m_A.y;
			int x2 = m_B.x;
			int y2 = m_B.y;

			int dx = x2-x1; int ax = ABS(dx)<<1; int sx = SGN(dx);
			int dy = y2-y1; int ay = ABS(dy)<<1; int sy = SGN(dy);
			
			int x = x1;
			int y = y1;

			if ( ax > ay ) {		/* x dominant */

				int d = ay - (ax >> 1);

				for (;;) {

					COORDINATE pos = { x, y };
					m_Primitive( rhs, pos );

				    if ( x == x2 ) return;

				    if ( d >= 0 ) {

						y += sy;
						d -= ax;
				    }

				    x += sx;
				    d += ay;
				}

			} else {			/* y dominant */

				int d = ax - (ay >> 1);

				for (;;) {

					COORDINATE pos = { x, y };
					m_Primitive( rhs, pos );

				    if ( y == y2 ) return;

				    if ( d >= 0 ) {

						x += sx;
						d -= ay;

				    }

				    y += sy;
				    d += ax;

				}

			}

		}

	};

	//
	//	PrimitiveLine()
	//

	template< class P, class C > __inline	
	TPrimitiveLine<P,C> PrimitiveLine(
		const C & a, const C & b, P primitive ) {

		return TPrimitiveLine<P,C>( a, b, primitive );

	}

	// ------------------------------------------------------------------------
	// Pixel line drawing templates
	// ------------------------------------------------------------------------

	//
	//	TPixelPrimitive
	//

	template<
		typename SURFACE, 
		typename COORDINATE = POINT,
		typename TRANSFER_OP = TCopyOP<typename SURFACE::pixel_type>
	>
	class TPixelPrimitive {
		
	private:

		typename SURFACE::pixel_type m_Color;
		TRANSFER_OP m_Rop;

		TPixelPrimitive(); // hidden

	public:

		typedef TPixelPrimitive<SURFACE,COORDINATE> this_type;
		typedef this_type & reference;
		typedef this_type * pointer;
		typedef const this_type & const_reference;
		typedef const this_type * const_pointer;
		typedef typename SURFACE::pixel_type pixel_type;

		// construction
		// --------------------------------------------------------------------

		TPixelPrimitive(
			const typename SURFACE::pixel_type color,
			const TRANSFER_OP rop = TRANSFER_OP()
		) : m_Color( color ), m_Rop( rop ) {

			/* Empty */

		}

		// Operation
		// --------------------------------------------------------------------

		__forceinline void operator()( SURFACE & surface, const COORDINATE & coordinate ) {

			if ( surface.ValidLocation( coordinate.x, coordinate.y ) ) {

				m_Rop( *surface.Iterator( coordinate.x, coordinate.y ), m_Color );

			}

		}

	};

	// ------------------------------------------------------------------------

	//
	//	DrawPixelLine()
	//

	template< typename SURFACE, typename COORDINATE > __inline	
	TPrimitiveLine<	TPixelPrimitive<SURFACE,COORDINATE>, COORDINATE > DrawPixelLine(
		const COORDINATE & a, const COORDINATE & b, const typename SURFACE::pixel_type color
	) {

		return PrimitiveLine(
			a, b, TPixelPrimitive<SURFACE,COORDINATE>( color )
		);

	}

	//
	//	DrawPixelLine()
	//

	template< typename SURFACE, typename COORDINATE, typename TRANSFER_OP > __inline	
	TPrimitiveLine<	TPixelPrimitive<SURFACE,COORDINATE,TRANSFER_OP>, COORDINATE > DrawPixelLine(
		const COORDINATE & a, const COORDINATE & b, 
		const typename SURFACE::pixel_type color, TRANSFER_OP rop
	) {

		return PrimitiveLine(
			a, b, TPixelPrimitive<SURFACE,COORDINATE,TRANSFER_OP>( color, rop )
		);

	}

	// ------------------------------------------------------------------------
	// Ellipse code
	// ------------------------------------------------------------------------

	//
	//	TMidpointEllipseAlgorithm<>
	//

	template< class COORDINATE = POINT >
	class TMidpointEllipseAlgorithm {

	private:

		template< class DST_SURFACE, class PRIMITIVE >
		void __forceinline Render(
			DST_SURFACE & dstSurface
			,PRIMITIVE & primitive
			,const COORDINATE & center
			,const int x
			,const int y
			) const {

			COORDINATE a = { center.x + x, center.y + y };
			COORDINATE b = { center.x - x, center.y + y };
			COORDINATE c = { center.x + x, center.y - y };
			COORDINATE d = { center.x - x, center.y - y };

			primitive( dstSurface, a );

			if ( x ) primitive( dstSurface, b );

			if ( y ) {

				primitive( dstSurface, c );

				if ( x ) primitive( dstSurface, d );

			}

		}

		int round( const double value ) const {

			return (int)( value + 0.5 );

		}

	public:

		TMidpointEllipseAlgorithm() { /* Empty */ }

		template< class DST_SURFACE, class PRIMITIVE >
		void __forceinline operator()( 
			DST_SURFACE & dstSurface
			,const COORDINATE & center
			,const int a
			,const int b
			,PRIMITIVE & primitive
			) const {

			// prepare 'step' variables
			// ----------------------------------------------------------------

			int Rx = a;
			int Ry = b;

			int Rx2 = (Rx * Rx);
			int Ry2 = (Ry * Ry);

			int twoRx2 = Rx2 * 2;
			int twoRy2 = Ry2 * 2;

			int x = 0;
			int y = Ry;
			int px = 0;
			int py = twoRx2 * y;

			// plot the first points & do 'region' 1
			// ----------------------------------------------------------------

			Render( dstSurface, primitive, center, x, y );

			int p = round( Ry2 - (Rx2 * Ry) + (0.25 * Rx2) );

			while ( px < py ) {

				++x;

				px += twoRy2;

				if ( 0 > p ) {

					p += (Ry2 + px);

				} else {

					--y;

					py -= twoRx2;

					p += (Ry2 + px - py);

				}

				Render( dstSurface, primitive, center, x, y );

			}

			// Render 'region' 2 
			// ----------------------------------------------------------------

			p = round(
				Ry2 * (x + 0.5) * (x + 0.5) + Rx2 * (y - 1) * (y - 1) - Rx2*Ry2
			);

			while ( 0 < y ) {

				--y;

				py -= twoRx2;

				if ( 0 < p ) {

					p += (Rx2 - py);

				} else {

					++x;

					px += twoRy2;

					p += (Rx2 - py + px);

				}

				Render( dstSurface, primitive, center, x, y );

			}

		}

	};

	//
	//	TParametricEllipseAlgorithm<>
	//

	template< class COORDINATE = POINT >
	class TParametricEllipseAlgorithm {

	private:

		void __forceinline fn(
			COORDINATE & out, const COORDINATE & center, 
			const float af, const float bf, const float t ) const {

			float xt = af * cosf( t );
			float yt = bf * sinf( t );

			if ( 0.0f > xt ) xt -= 0.5f;
			if ( 0.0f < xt ) xt += 0.5f;
			if ( 0.0f > yt ) yt -= 0.5f;
			if ( 0.0f < yt ) yt += 0.5f;

			out.x = center.x + (int)xt;
			out.y = center.y + (int)yt;

		}

	public:

		template< class DST_SURFACE, class PRIMITIVE >
		void __forceinline operator()( 
			DST_SURFACE & dstSurface
			,const COORDINATE & center
			,const int a
			,const int b
			,PRIMITIVE & primitive
			) const {

			// Check to see if we have a degenerate case 
			// ----------------------------------------------------------------

			if ( (0 == a) || (0 == b) ) {

				if ( 0 == a ) {

					if ( 0 == b ) {

						primitive( dstSurface, center );

					} else {

						// vertical line

						COORDINATE aa = { center.x, center.y - b };
						COORDINATE bb = { center.x, center.y + b };

						dstSurface << PrimitiveLine( aa, bb, primitive );

					}

				} else {

					if ( 0 == a ) {

						primitive( dstSurface, center );

					} else {

						// horizontal line

						COORDINATE aa = { center.x - a, center.y };
						COORDINATE bb = { center.x + a, center.y };

						dstSurface << PrimitiveLine( aa, bb, primitive );

					}

				}

				return /* done */;

			}

			// now get down to the ellipse renderer.
			// ----------------------------------------------------------------

			float af = a;
			float bf = b;

			float pi = 3.141592654f;

			// Use an approximation of the circumfrence of an ellipse as
			// the number of line segments to draw scaled by the ratio of
			// the major to minor axis, seems to make the ellipse look smoother
			// it's still seems to have a couple of 'spikes'.

			float h;

			if ( a > b ) {

				h = (bf / af);

			} else {

				h = (af / bf);

			}

			int N = (int)(((pi * ( (af + bf) * 1.5f - sqrtf(af*bf) )) + 0.5f) * h);

			float t = 0.0f;
			float dt = (pi * 2.0f) / N;

			POINT p;

			fn( p, center, af, bf, t );

			for ( int i = 0; i < N; i++ ) {

				POINT lp = p;

				t += dt;

				fn( p, center, af, bf, t );

				// Draw a line using the line primitive

				dstSurface << PrimitiveLine( lp, p, primitive );

			}

		}

	};

	//
	//	TPrimitiveEllipse<>
	//

	template< class PRIMITIVE, class COORDINATE = POINT >
	class TPrimitiveEllipse {

	private:

		COORDINATE m_Center;

		int m_A;
		int m_B;

		PRIMITIVE m_Primitive;

	public:

		typedef TPrimitiveEllipse<PRIMITIVE,COORDINATE> this_type;
		typedef this_type & reference;
		typedef this_type * pointer;
		typedef const this_type & const_reference;
		typedef const this_type * const_pointer;
		typedef PRIMITIVE primitive_type;

		// construction
		// --------------------------------------------------------------------

		TPrimitiveEllipse(
			const COORDINATE & center, const int a, const int b,
			PRIMITIVE primitive = PRIMITIVE()
		) : m_Center( center ), m_A( a ), m_B( b ), m_Primitive( primitive ) {

			/* Empty */

		}

		// operation 
		// --------------------------------------------------------------------

		template< class T > void operator()( T & rhs ) {

#if 1
			TMidpointEllipseAlgorithm<COORDINATE> ellipseGenerator;
#else
			TParametricEllipseAlgorithm<COORDINATE> ellipseGenerator;
#endif

			ellipseGenerator( rhs, m_Center, m_A, m_B, m_Primitive );

		}

	};

	//
	//	PrimitiveEllipse()
	//

	template< class P, class C > __inline	
	TPrimitiveEllipse<P,C> PrimitiveEllipse(
		const C & c, const int a, const int b, P primitive ) {

		return TPrimitiveEllipse<P,C>( c, a, b, primitive );

	}

	//
	//	DrawPixelEllipse()
	//

	template< typename SURFACE, typename COORDINATE > __inline	
	TPrimitiveEllipse<	TPixelPrimitive<SURFACE,COORDINATE>, COORDINATE > DrawPixelEllipse(
		const COORDINATE & c, const int a, const int b, const typename SURFACE::pixel_type color
	) {

		return PrimitiveEllipse(
			c, a, b, TPixelPrimitive<SURFACE,COORDINATE>( color )
		);

	}

	//
	//	DrawPixelEllipse()
	//

	template< typename SURFACE, typename COORDINATE, typename TRANSFER_OP > __inline	
	TPrimitiveEllipse<	TPixelPrimitive<SURFACE,COORDINATE,TRANSFER_OP>, COORDINATE > DrawPixelEllipse(
		const COORDINATE & c, const int a, const int b,
		const typename SURFACE::pixel_type color, TRANSFER_OP rop
	) {

		return PrimitiveEllipse(
			c, a, b, TPixelPrimitive<SURFACE,COORDINATE,TRANSFER_OP>( color, rop )
		);

	}

	// ------------------------------------------------------------------------
	// Solid Ellipse code
	// ------------------------------------------------------------------------

	//
	//	TMidpointSolidEllipseAlgorithm<>
	//

	template< typename COORDINATE = POINT >
	class TMidpointSolidEllipseAlgorithm {

	private:

		template< class DST_SURFACE, class TOP >
		void __forceinline RenderSpans(
			DST_SURFACE & dstSurface
			,TOP op
			,const typename DST_SURFACE::pixel_type value
			,const COORDINATE & center
			,const int x
			,const int y
			) const {

			int lx = center.x - x;

			int cx = (x * 2) + 1;

			// handle left clipping
			// ----------------------------------------------------------------

			if ( 0 > lx ) {

				cx += lx;
				lx = 0;

			}

			// Handle right clipping
			// ----------------------------------------------------------------

			int over = dstSurface.Width() - (lx + cx);

			if ( 0 > over ) {

				cx += over;

			}

			// Check to see if the ellipse is completely clipped
			// ----------------------------------------------------------------

			if ( 0 > cx ) {

				return /* nop */;

			}

			// Top
			// ----------------------------------------------------------------

			int top = (center.y - y);

			if ( (0 <= top) && (dstSurface.Height() > top) ) {

				DST_SURFACE::pixel_iterator itA = dstSurface.Iterator( lx, top );

				op.OutputMajor( itA, itA + cx, value );

			}

			// Bottom
			// ----------------------------------------------------------------

			int bot = (center.y + y);

			if ( top != bot ) {

				if ( (0 <= bot) && (dstSurface.Height() > bot) ) {
	
					DST_SURFACE::pixel_iterator itA = dstSurface.Iterator( lx, bot );
	
					op.OutputMajor( itA, itA + cx, value );
	
				}

			}

		}

		int round( const double value ) const {

			return (int)( value + 0.5 );

		}

	public:

		TMidpointSolidEllipseAlgorithm() { /* Empty */ }

		template< class DST_SURFACE, class TRANSFER_OP >
		void __forceinline operator()( 
			DST_SURFACE & dstSurface
			,const COORDINATE & center
			,const int a
			,const int b
			,typename DST_SURFACE::pixel_type value
			,TRANSFER_OP op
			) const {

			// prepare 'step' variables
			// ----------------------------------------------------------------

			int Rx = a;
			int Ry = b;

			int Rx2 = (Rx * Rx);
			int Ry2 = (Ry * Ry);

			int twoRx2 = Rx2 * 2;
			int twoRy2 = Ry2 * 2;

			int x = 0;
			int y = Ry;
			int px = 0;
			int py = twoRx2 * y;

			// plot the first points & do 'region' 1
			// ----------------------------------------------------------------

			int lx = x;
			int ly = y;

			int p = round( Ry2 - (Rx2 * Ry) + (0.25 * Rx2) );

			while ( px < py ) {

				++x;

				px += twoRy2;

				if ( 0 > p ) {

					p += (Ry2 + px);

				} else {

					RenderSpans( renderTarget, center, lx, ly, pen );

					ly = --y;

					py -= twoRx2;

					p += (Ry2 + px - py);

				}

				lx = x;

			}

			RenderSpans( dstSurface, op, value, center, lx, ly );

			// Render 'region' 2 
			// ----------------------------------------------------------------

			p = round(
				Ry2 * (x + 0.5) * (x + 0.5) + Rx2 * (y - 1) * (y - 1) - Rx2*Ry2
			);

			while ( 0 < y ) {

				--y;

				py -= twoRx2;

				if ( 0 < p ) {

					p += (Rx2 - py);

				} else {

					++x;

					px += twoRy2;

					p += (Rx2 - py + px);

				}

				RenderSpans( dstSurface, op, value, center, x, y );

			}

		}

	};

	//
	//	TSolidEllipse<>
	//

	template<
		typename PIXEL_TYPE = PIXEL_RAW_ARGB_TYPE
		,typename TRANSFER_OP = TCopyOP<PIXEL_TYPE>
		,typename COORDINATE = POINT
	>
	class TSolidEllipse {

	private:

		COORDINATE m_Center;

		int m_A;
		int m_B;

		PIXEL_TYPE m_Value;
		TRANSFER_OP m_Rop;

		TSolidEllipse(); // Hidden

	public:

		typedef TSolidEllipse<PIXEL_TYPE,TRANSFER_OP,COORDINATE> this_type;
		typedef this_type & reference;
		typedef this_type * pointer;
		typedef const this_type & const_reference;
		typedef const this_type * const_pointer;

		// construction
		// --------------------------------------------------------------------

		TSolidEllipse(
			const COORDINATE & center, const int a, const int b,
			const PIXEL_TYPE value, TRANSFER_OP rop = TRANSFER_OP()
		) : m_Center( center ), m_A( a ), m_B( b ),  m_Value( value), m_Rop( rop ) {

			/* Empty */

		}

		// operation 
		// --------------------------------------------------------------------

		template< class T > void operator()( T & rhs ) {

			TMidpointSolidEllipseAlgorithm<COORDINATE> ellipseGenerator;

			ellipseGenerator( rhs, m_Center, m_A, m_B, m_Value, m_Rop );

		}

	};

	// ------------------------------------------------------------------------

	//
	//	DrawSolidEllipse()
	//

	template< typename P, typename C >	__inline
	TSolidEllipse<P,TCopyOP<P>,C> DrawSolidEllipse(
		const C & center, const int a, const int b, P color
	) {

		return TSolidEllipse<P,TCopyOP<P>,C>( center, a, b, color );

	}

	//
	//	DrawSolidEllipse()
	//

	template< typename P, typename T, typename C >	__inline
	TSolidEllipse<P,T,C> DrawSolidEllipse(
		const C & center, const int a, const int b, P color, T & rop
	) {

		return TSolidEllipse<P,T,C>( center, a, b, color, rop );

	}

	// ------------------------------------------------------------------------
	//	Curve
	// ------------------------------------------------------------------------

	//
	//	TBezierCurvePrimitive<>
	//

	template< typename PRIMITIVE, typename COORDINATE = POINT >
	class TBezierCurvePrimitive {

	private:

		PRIMITIVE m_Primitive;

		int m_nLineSegments;

		COORDINATE m_A;
		COORDINATE m_B;
		COORDINATE m_C;
		COORDINATE m_D;

		TBezierCurvePrimitive(); // Hidden

	public:

		typedef TBezierCurvePrimitive<PRIMITIVE,COORDINATE> this_type;
		typedef this_type & reference;
		typedef this_type * pointer;
		typedef const this_type & const_reference;
		typedef const this_type * const_pointer;

		// construction
		// --------------------------------------------------------------------

		TBezierCurvePrimitive(
			const COORDINATE & a, const COORDINATE & b,
			const COORDINATE & c, const COORDINATE & d,
			const int nLineSegments,
			PRIMITIVE primitive = PRIMITIVE()
		) : m_A( a ), m_B( b ),  m_C( c ), m_D( d ), 
			m_nLineSegments( nLineSegments ), 
			m_Primitive( primitive ) {

			/* Empty */

		}

		// operation 
		// --------------------------------------------------------------------

		template< class T > void operator()( T & rhs ) {

			// prepare variables
			// ----------------------------------------------------------------
	
			float cx0 = m_A.x; float cy0 = m_A.y;
			float cx1 = m_B.x; float cy1 = m_B.y;
			float cx2 = m_C.x; float cy2 = m_C.y;
			float cx3 = m_D.x; float cy3 = m_D.y;
	
			float td = 1.0f / m_nLineSegments;
			float t = td;
			float it = 1.0f - t;
	
			// draw the line segments
			// ----------------------------------------------------------------
	
			POINT lp = m_A;
	
			for ( int i = 0; i < m_nLineSegments; i++ ) {
	
				float t2 = t * t;
				float t3 = t2 * t;
				float it2 = it * it;
				float it3 = it2 * it;
	
				float x = (cx0 * it3) + (cx1 * 3.0 * t * it2) + (cx2 * 3.0 * t2 * it) + (cx3 * t3);
				float y = (cy0 * it3) + (cy1 * 3.0 * t * it2) + (cy2 * 3.0 * t2 * it) + (cy3 * t3);
	
				POINT p = { (int)(x + 0.5), (int)(y + 0.5) };

				rhs << PrimitiveLine( lp, p, m_Primitive );
	
				lp = p;
				t += td;
				it -= td;
	
			}

		}

	};

	// ------------------------------------------------------------------------

	//
	//	BezierCurvePrimitive()
	//

	template< typename P, typename C > __inline	
	TBezierCurvePrimitive<P,C> BezierCurvePrimitive(
		const C & a, const C & b, const C & c, const C & d,
		const int nLineSegments, P primitive ) {

		return TBezierCurvePrimitive<P,C>( a, b, c, d, nLineSegments, primitive );

	}

	//
	//	DrawDPaintCurvePrimitive()
	//

	template< typename P, typename C > __inline	
	TBezierCurvePrimitive<P,C> DrawDPaintCurvePrimitive(
		const C & a, const C & b, const C & c,
		const int nLineSegments, P primitive
	) {

		// Project the AB & CB line segments

		POINT m = { ((a.x + b.x + c.x) / 3), ((a.y + b.y + c.y) / 3) };

		POINT d = { ((a.x + b.x) / 2), ((a.y + b.y) / 2) };
		POINT e = { ((b.x + c.x) / 2), ((b.y + c.y) / 2) };
		POINT g = { ((d.x + e.x) / 2), ((d.y + e.y) / 2) };

		POINT p = { b.x - (m.x - g.x), b.y - (m.y - g.y) };
		POINT c1 = { (p.x - (m.x - d.x)), (p.y - (m.y - d.y)) };
		POINT c2 = { (p.x - (m.x - e.x)), (p.y - (m.y - e.y)) };

		// use the normal bezier curve function

		return TBezierCurvePrimitive<P,C>( a, c1, c2, c, nLineSegments, primitive );

	}

	// ------------------------------------------------------------------------
	// Flood fill code
	// ------------------------------------------------------------------------

	//
	//	TFloodFillPrim<>
	//
	//	Based on Paul Heckbert's seed fill algorithm in Gems I pg 275
	//	the modifications to the code presented are to improve output speed.
	//

	template<typename T>
	class TFloodFillPrim {

	private:

		// Stack data / types
		// --------------------------------------------------------------------

		struct segment {

			int y, xl, xr, dy;

			segment( const int py, const int pxl, const int pxr, const int pdy ) :
				y( py ), xl( pxl ), xr( pxr ), dy( pdy ) {

				/* Empty */

			}

		};

		std::vector< segment > m_Stack;

		// Stack related methods
		// --------------------------------------------------------------------

		void __forceinline Push(
			const int y, const int xl, const int xr, const int dy, 
			const SRect & clipRect
		) {

			int ny = y + dy;

			if ( (ny >= clipRect.top) && (ny < clipRect.bottom) ) {

				m_Stack.push_back( segment( ny, xl, xr, dy ) );

			}

		}

		void __forceinline Pop( int & y, int & xl, int & xr, int & dy ) {

			segment & back = m_Stack.back();

			y = back.y;
			xl = back.xl;
			xr = back.xr;
			dy = back.dy;

			m_Stack.pop_back();

		}

		// --------------------------------------------------------------------

	public:

		TFloodFillPrim() { /* Empty */ }

		template< class SURFACE, class PREDICATE, class TOP >
		void operator()(
			SURFACE & surface, const int xx, const int yy, 
			const typename SURFACE::pixel_type color, 
			PREDICATE predicate, TOP op,
			const SRect * pClipRect = 0 ) {

			// Reserve enough space on the 'stack' for 16x the height
			// ----------------------------------------------------------------

			m_Stack.reserve( surface.Height() * 16);

			// Determine the write 'limits' for this operation
			// ----------------------------------------------------------------

			SRect bitmapLimits( surface.Rect() );

			SRect clippedRect;

			if ( pClipRect ) {
	
				if ( !clippedRect.Intersection( &bitmapLimits, pClipRect ) ) {
		
					return /* NOP */;
		
				}

			} else {

				clippedRect = bitmapLimits;

			}

			// Make sure that initial point is with in the bitmap limits and
			// that is satisfies the predicate, if not we're done :)
			// ----------------------------------------------------------------

			int x = xx;
			int y = yy;

			if ( (x < clippedRect.left) || (y < clippedRect.top) ||
				(x >= clippedRect.right) || (y >= clippedRect.bottom) ) {

				return /* NOP */;

			}

			if ( !predicate( *surface.Iterator( x, y )) ) {

				return /* NOP */;

			}

			// Okay now that we're past the bozo test lets get down to business
			// ----------------------------------------------------------------

			Push( y, x, x, 1, clippedRect );
			
			Push( y + 1, x, x, -1, clippedRect );

#if 1
			int safetyCounter = surface.Height() * surface.Width() / 16;
#endif

			while ( !m_Stack.empty() ) {

#if 1
				if ( 0 >= --safetyCounter ) {

					break;

				}
#endif
				// Pop something off the stack to process
				// ------------------------------------------------------------

				int l, dy, x1, x2;

				Pop( y, x1, x2, dy );

				// Scan left
				// ------------------------------------------------------------

				x = x1;

				if ( x >= clippedRect.left ) {

					SURFACE::reverse_iterator rit = surface.rIterator( x1, y );
					SURFACE::reverse_iterator start = rit;

					while ( predicate( *rit ) ) {

						++rit;

						if ( --x < clippedRect.left ) {

							break;

						}

					}

					if ( start != rit ) {

						// fill (x to x1], y 

						op.OutputMajor( start, rit, color );

					}

				}

				if ( x >= x1 ) {

					goto SKIP;

				}

				// Scan right
				// ------------------------------------------------------------

				l = x + 1;

				if ( l < x1 ) {

					Push( y, l, x1 - 1, -dy, clippedRect );

				}

				x = x1 + 1;

				do {

					// Right span length
					// --------------------------------------------------------

					if ( x < clippedRect.right ) {

						int sx = x;
	
						SURFACE::pixel_iterator it = surface.Iterator( sx, y );
						SURFACE::pixel_iterator start = it;
	
						while ( predicate( *it ) ) {
	
							++it;
	
							if ( ++x >= clippedRect.right ) {
	
								break;
	
							}
	
						}
	
						if ( start != it ) {
	
							// fill [sx to x), y 
	
							op.OutputMajor( start, it, color );
	
						}

					}

					// More area's to process pushed onto the stack.
					// --------------------------------------------------------

					Push( y, l, x - 1, dy, clippedRect );

					if ( x > x2 + 1 ) {

						Push( y, x2 + 1, x - 1, -dy, clippedRect );

					}

					// Search more to the right.
					// --------------------------------------------------------

				SKIP:

					if ( ++x <= x2 ) {

						SURFACE::pixel_iterator it = surface.Iterator( x, y );

						while ( !predicate( *it++ ) ) {

							if ( ++x > x2 ) {

								break;

							}

						}

					}

					l = x;

				} while ( x <= x2 );

			}


		}

	};

	// ------------------------------------------------------------------------

	//
	//	TSimpleFloodFill<>
	//
	
	template<
		class PREDICATE
		,class PIXEL_TYPE = PIXEL_RAW_ARGB_TYPE
		,class TRANSFER_OP = TCopyOP<PIXEL_TYPE>
	>
	class TSimpleFloodFill {
	
	private:

		PREDICATE m_Predicate;
		PIXEL_TYPE m_Value;
		TRANSFER_OP m_Rop;
		int m_X;
		int m_Y;
	
		TSimpleFloodFill(); // Hidden
	
	public:

		// construction
		// --------------------------------------------------------------------
	
		TSimpleFloodFill( const int x, const int y, const PIXEL_TYPE value,
			PREDICATE & predicate, TRANSFER_OP rop = TRANSFER_OP() ) :
			m_X( x ), m_Y( y ), m_Value( value), m_Rop( rop ), m_Predicate( predicate ) { /* Empty */ }

		// operation
		// --------------------------------------------------------------------

		template< class T > void __forceinline operator()( T & rhs ) {

			TFloodFillPrim floodFiller;

			floodFiller( rhs, m_X, m_Y, m_Value, m_Predicate, m_Rop );

		}
	
	};

	// ------------------------------------------------------------------------

	//
	//	SimpleFloodFill
	//

	template< class T, class P > __inline
	TSimpleFloodFill<TIsValue<P>,P> SimpleFloodFill(
		T & surface, const int x, const int y, P color ) {

		if ( !surface.ValidLocation( x, y ) ) {

			return TSimpleFloodFill<TIsValue<P>,P>( x, y, color, TIsValue<P>( color ) );

		}

		return TSimpleFloodFill<TIsValue<P>,P>(
			x, y, color, TIsValue<P>( *surface.Iterator( x, y ) )
		);

	}

	//
	//	FloodFillTo
	//

	template< class T, class P > __inline
	TSimpleFloodFill<TDoubleIsNotValue<P>,P> FloodFillTo(
		T & surface, const int x, const int y,
		P color, P stopColor
	) {

		if ( !surface.ValidLocation( x, y ) ) {

			return TSimpleFloodFill<TDoubleIsNotValue<P>,P>( x, y, color, TDoubleIsNotValue<P>( color, color ) );

		}

		return TSimpleFloodFill<TDoubleIsNotValue<P>,P>(
			x, y, color, TDoubleIsNotValue<P>( color, stopColor )
		);

	}

	// ------------------------------------------------------------------------
	// Bitdepth conversion via 'Error diffusion'
	// ------------------------------------------------------------------------

	//
	//	TErrorDiffusionDither<>
	//
	
	template< typename D, typename S >
	class TErrorDiffusionDither {
	
	public:
	
	private:
	
		typedef std::vector<int> error_collection_type;
	
		// --------------------------------------------------------------------
	
		typename D::pixel_type Make( const int r, const int g, const int b ) {
	
			return typename D::pixel_type(
				max( 0, min( 255, r )), 
				max( 0, min( 255, g )), 
				max( 0, min( 255, b ))
			);
	
		}
	
		// --------------------------------------------------------------------
	
		void DistributeError( 
			const int i, const int d, error_collection_type & a, error_collection_type & b, const int e
		) {
	
			a[ i + d ] += (e * 7) >> 4;
			b[ i - d ] += (e * 3) >> 4;
			b[ i ] += (e * 5) >> 4;
			b[ i + d ] += (e) >> 4;
	
		}
	
		// --------------------------------------------------------------------
	
	public:
	
		// --------------------------------------------------------------------
	
		TErrorDiffusionDither() {}
	
		~TErrorDiffusionDither() {}
	
		// --------------------------------------------------------------------
	
		D * CreateDithered( S * pSource ) {
	
			// construct our error term collections
			// ----------------------------------------------------------------
	
			error_collection_type rError1;
			error_collection_type gError1;
			error_collection_type bError1;
	
			error_collection_type rError2;
			error_collection_type gError2;
			error_collection_type bError2;
	
			// prepare
			// ----------------------------------------------------------------
	
			int w = pSource->Width();
			int h = pSource->Height();
	
			// reserve
			// ----------------------------------------------------------------
	
			rError1.reserve( w );
			rError2.reserve( w );
	
			gError1.reserve( w );
			gError2.reserve( w );
	
			bError1.reserve( w );
			bError2.reserve( w );
	
			for ( int i = 0; i < w; i++ ) {
	
				rError1.push_back( 0 );
				rError2.push_back( 0 );
	
				gError1.push_back( 0 );
				gError2.push_back( 0 );
	
				bError1.push_back( 0 );
				bError2.push_back( 0 );
	
			}
	
			// ----------------------------------------------------------------
	
			D * pNew = new D;
	
			if ( !pNew ) {
	
				return 0;
	
			}
	
			if ( !pNew->Create( w, h ) ) {
	
				delete pNew;
	
				return 0;
	
			}
	
			// ----------------------------------------------------------------
	
			for ( int y = 0; y < h; y++ ) {
	
				// ------------------------------------------------------------
		
				S::pixel_iterator sit = pSource->Iterator( 0, y );
		
				// Start by adding this lines data to last lines error term
				// ------------------------------------------------------------
		
				for ( int x = 0; x < w; x++ ) {
	
					rError1[ x ] = rError2[ x ] + sit->R();
					rError2[ x ] = 0;
	
					gError1[ x ] = gError2[ x ] + sit->G();
					gError2[ x ] = 0;
	
					bError1[ x ] = bError2[ x ] + sit->B();
					bError2[ x ] = 0;
	
					++sit;
		
				}
	
				// Okay lets dither the line (serpentine style)
				// ------------------------------------------------------------
	
				if ( y & 1 ) {
	
					D::pixel_iterator dit = pNew->Iterator( 0, y );
		
					*dit++ = Make( rError1[ 0 ], gError1[ 0 ], bError1[ 0 ] );
		
					for ( int i = 1; i < (w - 1); i++ ) {
		
						// ----------------------------------------------------
		
						*dit = Make( rError1[ i ], gError1[ i ], bError1[ i ] );
		
						DistributeError( i, 1, rError1, rError2, (rError1[ i ] - dit->R()) );
						DistributeError( i, 1, gError1, gError2, (gError1[ i ] - dit->G()) );
						DistributeError( i, 1, bError1, bError2, (bError1[ i ] - dit->B()) );
		
						// ----------------------------------------------------
		
						++dit;
		
					}
		
					*dit++ = Make( rError1[ i ], gError1[ i ], bError1[ i ] );
	
				} else {
	
					D::reverse_iterator dit = pNew->rIterator( w - 1, y );
	
					int l = w - 1;
		
					*dit++ = Make( rError1[ l ], gError1[ l ], bError1[ l ] );
		
					for ( int j = 1; j < (w - 1); j++ ) {
	
						int i = l - j;
		
						// ----------------------------------------------------
		
						*dit = Make( rError1[ i ], gError1[ i ], bError1[ i ] );
		
						DistributeError( i, -1, rError1, rError2, (rError1[ i ] - dit->R()) );
						DistributeError( i, -1, gError1, gError2, (gError1[ i ] - dit->G()) );
						DistributeError( i, -1, bError1, bError2, (bError1[ i ] - dit->B()) );
		
						// ----------------------------------------------------
		
						++dit;
		
					}
		
					*dit++ = Make( rError1[ 0 ], gError1[ 0 ], bError1[ 0 ] );
	
				}
	
			}
	
			return pNew;
	
		}
	
	};

	// ------------------------------------------------------------------------
	// FUTURE
	// ------------------------------------------------------------------------

	// class TDrawPolyLine
	// class TDrawPolygon
	// class TDrawTriangle

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------

}; // namespace BPT

#endif // !defined(AFX_TSURFACEOPS_H__F8B12740_CA4D_4FE9_978D_16DD92A3C340__INCLUDED_)
