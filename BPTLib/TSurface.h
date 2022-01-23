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


#if !defined(AFX_TSURFACE_H__D93FA0EF_022F_437D_828F_383805BFA086__INCLUDED_)
#define AFX_TSURFACE_H__D93FA0EF_022F_437D_828F_383805BFA086__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ----------------------------------------------------------------------------

#define BPTLIB_USE_COM_FOR_SURFACE_MEMORY_MANAGMENT

#include <iterator>
#include <utility>

#if defined(BPTLIB_USE_COM_FOR_SURFACE_MEMORY_MANAGMENT)
#include <objbase.h>
#include <objidl.h>
#endif

#include "TIterator.h"
#include "Rectangle.h"

// ----------------------------------------------------------------------------

namespace BPT {

	//
	//	ARootSurface
	//
	//	-- Common interface for all surfaces
	//
	//

	class ARootSurface {

	public:

		virtual int getPitch() const = 0;
		virtual int getWidth() const = 0;
		virtual int getHeight() const = 0;
		virtual int getFormat() const = 0;
		virtual void * getData() const = 0;
		virtual int getSizeofPixel() const = 0;
		virtual void releaseData( void * ptr ) const = 0;

	};

	//
	//	TSurface<>
	//
	//	-- Surface/Bitmap template
	//
	//	NOTE: When creating a bitmap it doesn't 'new' the pixel data
	//	so care should be used in the design of the PIXELTYPE's that
	//	are passed in that no construction is necessary and that the
	//	types can be mapped onto normal memory with no issues.
	//
	
	template<
		typename PIXELTYPE
		,const int ALIGNMENT = 1
		,typename ITERATOR = PIXELTYPE *
	>
	class TSurface : public ARootSurface
	{
	
	public:

		typedef TSurface< PIXELTYPE, ALIGNMENT, ITERATOR > this_type;
		typedef this_type & reference;
		typedef this_type * pointer;
		typedef const this_type & const_reference;
		typedef const this_type * const_pointer;
		typedef PIXELTYPE pixel_type;
		typedef ITERATOR pixel_iterator;

		// build a reverse iterator using the STL types

#if 1
		typedef std::reverse_iterator<pixel_iterator> reverse_iterator;
#else
		typedef std::reverse_iterator<
			pixel_iterator, PIXELTYPE,
			PIXELTYPE &, PIXELTYPE *, ptrdiff_t
		> reverse_iterator;
#endif

		// Need iterator type!
	
		enum {
	
			alignment = ALIGNMENT
	
		};
	
	private:

		// --------------------------------------------------------------------
		// Data
		// --------------------------------------------------------------------

		void * m_RawDataPtr;
		bool m_OwnedData;
		int m_Pitch;
		SIZE m_Size;
	
	public:

		// --------------------------------------------------------------------
		// Root object implmentation
		// --------------------------------------------------------------------

		virtual int getPitch() const {

			return Pitch();

		}

		virtual int getWidth() const {

			return Width();

		}

		virtual int getHeight() const {

			return Height();

		}

		virtual int getFormat() const {

			return Format();

		}

		virtual void * getData() const {

			return m_RawDataPtr;

		}

		virtual void releaseData( void * ptr ) const {

			/* Empty */

		}

		virtual int getSizeofPixel() const {

			return sizeof( pixel_type );

		}

		// --------------------------------------------------------------------
		// Helpers
		// --------------------------------------------------------------------

	protected:

		int CalcPitch( const int width, const int height, const int pitch = 0 ) {

			if ( pitch ) {

				return pitch;

			}

			int rawPitch = width * sizeof( PIXELTYPE );

			int leftOver = (rawPitch % alignment);

			if ( !leftOver ) {

				return rawPitch;

			}

			return rawPitch + (alignment - leftOver);

		}

	public:

		// --------------------------------------------------------------------
		// construction / Destruction
		// --------------------------------------------------------------------

		TSurface() : m_RawDataPtr(0), m_OwnedData(false), m_Pitch(0) {

			m_Size.cx = 0;
			m_Size.cy = 0;

		}

		virtual ~TSurface() {

			Release();

		}

		// --------------------------------------------------------------------
		//	Release helper
		// --------------------------------------------------------------------

		virtual void Release() {

			if ( m_RawDataPtr ) {

				if ( m_OwnedData ) {

#if defined(BPTLIB_USE_COM_FOR_SURFACE_MEMORY_MANAGMENT)

					::CoTaskMemFree( m_RawDataPtr );

#else

					delete m_RawDataPtr;

#endif

					m_OwnedData = false;

				}

				m_RawDataPtr = 0;
				m_Pitch = 0;
				m_Size.cx = 0;
				m_Size.cy = 0;

			}

		}

		// --------------------------------------------------------------------
		// Misc helper functions
		// --------------------------------------------------------------------

		pixel_type Color( const int r, const int g, const int b, const int a ) {

			return pixel_type( r, g, b, a );

		}

		int GetMaxChannels() {

			return pixel_type::GetChannelCount();

		}

		int ChannelCount() {

			return pixel_type::GetChannelCount();

		}

		// --------------------------------------------------------------------
		//	Object state functions
		// --------------------------------------------------------------------

		int Width() const {

			return m_Size.cx;

		}

		int Height() const {

			return m_Size.cy;

		}

		SIZE Size() const {

			return m_Size;

		}

		SRect Rect() const {

			return SRect( 0, 0, Width(), Height() );

		}

		int Pitch() const {

			return m_Pitch;

		}

		int Format() const {

			return pixel_type::format;

		}

		int Bits() const {

			return pixel_type::Bits;

		}

		bool HasPixels() const {

			return (0 != m_RawDataPtr);

		}

		bool ValidLocation( const int x, const int y ) const {

			return ((0 <= x) && (0 <= y) && (m_Size.cx > x) && (m_Size.cy > y));

		}
	
		// --------------------------------------------------------------------
		// General creation
		// --------------------------------------------------------------------

		bool Create( const int width, const int height, const int pitch = 0 ) {

			Release();

			int thePitch = CalcPitch( width, height, pitch );

#if defined(BPTLIB_USE_COM_FOR_SURFACE_MEMORY_MANAGMENT)

			void * data = ::CoTaskMemAlloc( thePitch * height );

#else

			void * data = ::malloc( thePitch * height );

#endif

			if ( 0 == data ) {

				return false;

			}

			return Map( data, width, height, thePitch, true );

		}

		bool Create( const SIZE & size, const int pitch = 0  ) {

			return Create( size.cx, size.cy, pitch );

		}

		pointer Clone() const {

			pointer clone = new this_type;

			if ( 0 == clone ) {

				return 0;

			}

			if ( HasPixels() ) {

				if ( !clone->Create( Width(), Height(), Pitch() ) ) {

					delete clone;
					return 0;

				}

				memcpy( clone->m_RawDataPtr, m_RawDataPtr, (Pitch() * Height()) );

			}

			return clone;

		}

		// --------------------------------------------------------------------
		// PIXEL source mapping
		// --------------------------------------------------------------------

		bool Map(
			void * data, const int width, const int height, 
			const int pitch = 0, const bool ownData = false,
			const bool callRelease = false ) {

			if ( callRelease ) {

				Release();

			}

			m_OwnedData = ownData;
			m_RawDataPtr = data;
			m_Size.cx = width;
			m_Size.cy = height;
			m_Pitch = CalcPitch( width, height, pitch );

			return true;

		}

		// --------------------------------------------------------------------
		// Iterator
		// --------------------------------------------------------------------

		pixel_iterator Iterator( const int x, const int y ) {

			if ( !HasPixels() ) {

				return pixel_iterator( 0 );

			}

			int offset = (y * Pitch()) + (x * sizeof(PIXELTYPE));

			return static_cast< pixel_iterator >(
				static_cast< void * >(
					static_cast< unsigned char * >( m_RawDataPtr ) + offset
				)
			);

		}

		reverse_iterator rIterator( const int x, const int y ) {

			return reverse_iterator( Iterator( x + 1, y ) );

		}

		// --------------------------------------------------------------------
		// Surface stream operator's
		// --------------------------------------------------------------------

		template< class T > reference operator<<( T & rhs ) {
	
			rhs( *this );
			return *this;
	
		}

	};

}; // namespace BPT

#endif // !defined(AFX_TSURFACE_H__D93FA0EF_022F_437D_828F_383805BFA086__INCLUDED_)
