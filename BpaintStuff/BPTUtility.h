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

#if !defined(AFX_BPTUTILITY_H__3CCD45AD_8607_4793_99C8_EDD654614E2A__INCLUDED_)
#define AFX_BPTUTILITY_H__3CCD45AD_8607_4793_99C8_EDD654614E2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BPTBitmap.h"
#include <functional>
#include <algorithm>

namespace BPT {

	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// Rectangle utility functions
	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	//
	//	ReturnExternalClipRects()
	//
	//	-- This function returns between 0 and 4 sub rectagles of pRect that 
	//	-- lay outside the clipping rectangle passed in.
	//

	int __inline
	ReturnExternalClipRects(
		RECT * results, const RECT * pRect, const RECT * pClipRect
	) {

		int index = 0;

		// top line
		// --------------------------------------------------------------------

		int y1;

		if ( pRect->top < pClipRect->top ) {

			SetRect( &results[ index++ ], pRect->left, pRect->top, pRect->right, pClipRect->top );

			y1 = pClipRect->top;

		} else {

			y1 = pRect->top;

		}

		// bottom line
		// --------------------------------------------------------------------

		int y2;

		if ( pRect->bottom > pClipRect->bottom ) {

			SetRect( &results[ index++ ], pRect->left, pClipRect->bottom, pRect->right, pRect->bottom );

			y2 = pClipRect->bottom;

		} else {

			y2 = pRect->bottom;

		}

		// Left?
		// --------------------------------------------------------------------

		if ( pRect->left < pClipRect->left ) {

			SetRect( &results[ index++ ], pRect->left, y1, pClipRect->left, y2 );

		}

		// Right?
		// --------------------------------------------------------------------

		if ( pRect->right > pClipRect->right ) {

			SetRect( &results[ index++ ], pClipRect->right, y1, pRect->right, y2 );

		}

		return index;

	}

	//
	//	SizeOfRect()
	//

	SIZE __inline SizeOfRect( const RECT * pRect ) {

		SIZE size = { pRect->right - pRect->left, pRect->bottom - pRect->top };

		return size;

	}

	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// Changed rectangle notification class
	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	struct IAddDirtyRect {

		virtual void AddDirtyRect( const RECT & rect ) = 0;

	};

	template< typename OWNER >
	class TRectChangeMediator : public IAddDirtyRect {

	private:

		OWNER * m_pOwner;

		TRectChangeMediator(); // Hidden

	public:

		TRectChangeMediator( OWNER * pOwner) : m_pOwner( pOwner ) {

			/* Empty */

		}

		void AddDirtyRect( const RECT & rect ) {

			if ( m_pOwner ) {

				m_pOwner->AddDirtyRect( rect );

			}

		}

	};

	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// grid info class
	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	class CGridInfo {

	public:

		CSize m_GridSize;
		CPoint m_GridOffset;

	public:

		// --------------------------------------------------------------------

		CGridInfo(
			CSize & size = CSize( 8, 8 ), CPoint offset = CPoint( 0, 0 )
		) : m_GridSize( size ), m_GridOffset( offset ) {

			/* empty */

		}

		void SetInfo( const POINT * pOffset, const SIZE * pSize ) {

			if ( pOffset ) m_GridOffset = *pOffset; 
			if ( pSize ) m_GridSize = *pSize; 

		}

		// --------------------------------------------------------------------

		void Transform( POINT * pPoint ) {

			pPoint->x = m_GridOffset.x + ((pPoint->x / m_GridSize.cx) * m_GridSize.cx);
			pPoint->y = m_GridOffset.y + ((pPoint->y / m_GridSize.cy) * m_GridSize.cy);

		}

		POINT Transform( const POINT * pPoint ) {

			POINT result = *pPoint;;

			Transform( &result );

			return result;

		}

	};

	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// render target
	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	template< typename T >
	class TRenderTarget {

	private:

		typename T::pointer m_pCanvas;

		IAddDirtyRect * m_pDirtyRectHandler;

		CGridInfo * m_pGridInfo;

	public:

		// traits

		typedef T surface_type;

		// construction

		TRenderTarget() : m_pCanvas( 0 ), m_pDirtyRectHandler( 0 ), m_pGridInfo( 0 ) { /* Empty */ }

		// setup methods

		void SetCanvas( typename T::pointer pCanvas ) {

			m_pCanvas = pCanvas;

		}

		void SetDirtyRectHandler( IAddDirtyRect * pDirtyRectHandler ) {

			m_pDirtyRectHandler = pDirtyRectHandler;

		}

		// methods

		typename T::pointer GetCanvas() {

			return m_pCanvas;

		}

		void AddDirtyRect( const RECT & rect ) {

			if ( m_pDirtyRectHandler ) {

				m_pDirtyRectHandler->AddDirtyRect( rect );

			}

		}

		// grid addon

		bool SetGridInfo( CGridInfo * pGridInfo ) {

			m_pGridInfo = pGridInfo;
			
			return true;

		}

		CGridInfo * GetGridInfo() {

			return m_pGridInfo;

		}

	};

	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// Properties interface
	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	struct ISupportPropertyTypes {

		enum {

			UNKNOWN		= 0
			,WIDTH		= 1
			,HEIGHT		= 2

		};

	};

	template< typename PROP_VALUE = int >
	class TISupportProperties {

	public:

		typedef PROP_VALUE property_value_type;

		// flexible 'property' interface
		// --------------------------------------------------------------------

		virtual int GetPropertyCount() { return 0; }
		virtual int GetPropertyID( const int index ) { return 0; }
		virtual int GetPropertyType( const int index ) { return 0; }
		virtual void PutPropertyValue( const int index, const PROP_VALUE & value ) { /* Empty */ }
		virtual bool GetPropertyValue( const int index, PROP_VALUE & out ) { return false; }
		virtual bool GetPropertyMin( const int index, PROP_VALUE & out ) { return false; }
		virtual bool GetPropertyMax( const int index, PROP_VALUE & out ) { return false; }

	};

	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// Unary predicates
	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	//
	//	TAlwaysXPredicate<>
	//

	template< typename T, const bool X > class TAlwaysXPredicate { // : public std::unary_function<T, bool> {

	public:

		TAlwaysXPredicate() { /* Empty */ }

		__forceinline bool operator()( const T & value ) const {

			return X;

		}

	};

	//
	//	TIsValue
	//

	template< typename T > class TIsValue { // : public std::unary_function<T,bool> {

	private:

		T m_Value;

		TIsValue(); // Hidden

	public:

		TIsValue( const T & value ) : m_Value( value ) { /* Empty */ }

		__forceinline bool operator()( const T & value ) const {

			return (value == m_Value);

		}

	};

	//
	//	TIsNotValue
	//

	template< typename T > class TIsNotValue { // : public std::unary_function<T,bool> {

	private:

		T m_Value;

		TIsNotValue(); // Hidden

	public:

		TIsNotValue( const T & value ) : m_Value( value ) { /* Empty */ }

		__forceinline bool operator()( const T & value ) const {

			return (value != m_Value);

		}

	};

	//
	//	TIsInRange<>
	//

	template< typename T > class TIsInRange { //: public std::unary_function<T,bool>  {

	private:
	
		T m_Min, m_Max;

	public:
	
		TIsInRange( const T & m, const T & n ) : m_Min( m ), m_Max( n ) {}
	
		bool operator()( const T & i ) const {
	
			return (i >= m_Min) && (i <= m_Max);
	
		}
	
	};

	//
	//	TIsInCollection<>
	//

	template< typename T, typename C > class TIsInCollection { //: public std::unary_function<T,bool>  {

	private:
	
		T m_Min, m_Max;

		C & m_Collection;

		TIsInCollection(); // Hidden

	public:
	
		TIsInCollection( C & collection ) : m_Collection( collection ) {}
	
		bool operator()( const T & i ) const {

			C::iterator it = std::find( m_Collection.begin(), m_Collection.end(), i );

			return (it != m_Collection.end());
	
		}
	
	};

	template<typename T>
	bool isodd(T v)
	{
		return (0 != ((int)v % 2));
	}

}; // namespace BPT

#endif // !defined(AFX_BPTUTILITY_H__3CCD45AD_8607_4793_99C8_EDD654614E2A__INCLUDED_)
