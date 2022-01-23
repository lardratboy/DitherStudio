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

#if !defined(AFX_TMULTICHANNELSURFACE_H__C79DE455_C71E_4E1E_ACC4_C9B0B013E3A6__INCLUDED_)
#define AFX_TMULTICHANNELSURFACE_H__C79DE455_C71E_4E1E_ACC4_C9B0B013E3A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// (SUPPORT-INCLUDES) ---------------------------------------------------------

#include "THelpers.h"
#include <limits.h>
#include <list>
#include <vector>

// ----------------------------------------------------------------------------

namespace BPT {

	// ========================================================================

	//
	//	TMultiChannelPixel
	//

	template< class CONTAINER >
	class TMultiChannelPixel {

	private:

		CONTAINER m_Channels;

		int m_ChannelCount;

		TMultiChannelPixel(); // Hidden

	public:

		typedef TMultiChannelPixel<CONTAINER> this_type;
		typedef this_type & reference;
		typedef this_type * pointer;
		typedef const this_type & const_reference;
		typedef const this_type * const_pointer;

		// construction
		// --------------------------------------------------------------------

		TMultiChannelPixel( CONTAINER & initContainer ) :
			m_Channels( initContainer ), m_ChannelCount( initContainer.size() ) {

			/* Empty */

		}

		// template Assignment operator???
		// --------------------------------------------------------------------

		template< class T >
		reference operator=( const T & rhs ) {

			CONTAINER::iterator it = m_Channels.begin();
			CONTAINER::iterator end = m_Channels.end();

			int count = rhs.GetChannelCount();

			for ( int c = 0; c < count; c++ ) {

				if ( end == it ) {

					break;

				}

				(*it)->SetChannel( 0, rhs.GetChannel( c ) );

				++it;

			}

			return *this;

		}

		// Conversion operator
		// --------------------------------------------------------------------

		operator PIXEL_RAW_ARGB_TYPE() const {

			// GROSS HACK WARNING
			// GROSS HACK WARNING
			// GROSS HACK WARNING

			pointer t = const_cast< pointer >( this );

			return PIXEL_RAW_ARGB_TYPE(
				t->GetChannel( 0 ),
				t->GetChannel( 1 ),
				t->GetChannel( 2 ),
				t->GetChannel( 3 )
			);

			// GROSS HACK WARNING
			// GROSS HACK WARNING
			// GROSS HACK WARNING

		}

		// Other useful operator's (comparison & copy)
		// --------------------------------------------------------------------

		bool operator==( const_reference rhs ) const {

			// Check for the simple things first

			if ( rhs.GetChannelCount() != GetChannelCount() ) {

				return false;

			}

			// now compare each value

			CONTAINER::iterator a = m_Channels.begin();
			CONTAINER::iterator b = rhs.m_Channels.begin();

			for ( int i = 0; i < count; i++ ) {

				if ( *a != *b ) {

					return false;

				}

				++a;
				++b;

			}

			return true;

		}

		bool operator!=( const_reference rhs ) const {

			return !(*this == rhs);

		}

		// Channel interface methods
		// --------------------------------------------------------------------

		int GetChannel( const int channel ) {

			if ( (0 <= channel) && (channel < m_ChannelCount) ) {

				CONTAINER::iterator it = m_Channels.begin();

				for ( int i = 0; i < channel; i++ ) {

					++it;

				}

				return (*it)->value;

			}

			return 0;

		}

		void SetChannel( const int channel, const int newValue ) {

			if ( (0 <= channel) && (channel < m_ChannelCount) ) {

				CONTAINER::iterator it = m_Channels.begin();

				for ( int i = 0; i < channel; i++ ) {

					++it;

				}

				(*it)->value = newValue;

			}

		}

		int GetChannelCount() const {

			return m_ChannelCount;

		}

	};

	//
	//	TMultiChannelIterator
	//

	template< class CONTAINER >
	class TMultiChannelIterator {

	private:

		TMultiChannelIterator(); // Hidden

		CONTAINER m_ChannelIterators;

	public:

		typedef TMultiChannelIterator<CONTAINER> this_type;
		typedef this_type & reference;
		typedef this_type * pointer;
		typedef const this_type & const_reference;
		typedef const this_type * const_pointer;
		typedef TMultiChannelPixel<CONTAINER> pixel_type;

		// Construction
		// --------------------------------------------------------------------

		TMultiChannelIterator( CONTAINER & initContainer ) :
			m_ChannelIterators( initContainer) {

			/* Empty */

		}

		// dereference operator's
		// --------------------------------------------------------------------

		pixel_type operator*() {

			return pixel_type( m_ChannelIterators );

		}

		pixel_type operator->() {

			return pixel_type( m_ChannelIterators );

		}

		// increment pre & post
		// --------------------------------------------------------------------

		reference operator++() {

			CONTAINER::iterator it = m_ChannelIterators.begin();
			CONTAINER::iterator end = m_ChannelIterators.end();

			for ( ; it != end; it++ ) {

				++(*it);

			}

			return *this;

		}

		this_type operator++(int) {

			this_type temp = *this;

			++*this;

			return temp;

		}

		// decrement pre & post
		// --------------------------------------------------------------------

		reference operator--() {

			CONTAINER::iterator it = m_ChannelIterators.begin();
			CONTAINER::iterator end = m_ChannelIterators.end();

			for ( ; it != end; it++ ) {

				--(*it);

			}

			return *this;

		}

		this_type operator--(int) {

			this_type temp = *this;

			--*this;

			return temp;

		}

		// Pointer advancement
		// --------------------------------------------------------------------

		reference operator+=( const int v ) {

			CONTAINER::iterator it = m_ChannelIterators.begin();
			CONTAINER::iterator end = m_ChannelIterators.end();

			for ( ; it != end; it++ ) {

				(*it) += v;

			}

			return *this;

		}

		reference operator-=( const int v ) {

			CONTAINER::iterator it = m_ChannelIterators.begin();
			CONTAINER::iterator end = m_ChannelIterators.end();

			for ( ; it != end; it++ ) {

				(*it) -= v;

			}

			return *this;

		}

	};

	// ========================================================================

	//
	//	TMultiChannelSurface
	//

	template< typename CHANNEL_SURFACE, typename CONTAINER = std::vector< CHANNEL_SURFACE * > >
	class TMultiChannelSurface : public ARootSurface {

	private:

		SIZE m_Size;

		int m_ChannelCount;

		CONTAINER m_ChannelSurfaces;

		typedef std::vector<typename CHANNEL_SURFACE::pixel_iterator> SURFACE_PIXEL_ITERATOR_CONTAINER;

	public:

		// --------------------------------------------------------------------
		// --------------------------------------------------------------------

		typedef TMultiChannelSurface<CHANNEL_SURFACE,CONTAINER> this_type;
		typedef this_type & reference;
		typedef this_type * pointer;
		typedef const this_type & const_reference;
		typedef const this_type * const_pointer;
		typedef CHANNEL_SURFACE channel_surface_type;
		typedef TMultiChannelIterator< SURFACE_PIXEL_ITERATOR_CONTAINER > pixel_iterator;
		typedef typename pixel_iterator::pixel_type pixel_type;

		// Is there an iterator type?

		// --------------------------------------------------------------------
		// Construction / destruction
		// --------------------------------------------------------------------

		TMultiChannelSurface() {

			m_ChannelCount = 0;
			m_Size.cx = 0;
			m_Size.cy = 0;
		
		}

		virtual ~TMultiChannelSurface() {

			Release();

		}

		// --------------------------------------------------------------------
		//	Release helper
		// --------------------------------------------------------------------

		virtual void Release() {

			m_ChannelCount = 0;
			m_Size.cx = 0;
			m_Size.cy = 0;

			while ( !m_ChannelSurfaces.empty() ) {

				delete m_ChannelSurfaces.back();
				m_ChannelSurfaces.pop_back();

			}

		}

		// --------------------------------------------------------------------
		//	Creation?
		// --------------------------------------------------------------------

		bool Create(
			const int cx, 
			const int cy, 
			const int channels = 4,
			const int pitch = 0
		) {

			Release();

			for ( int i = 0; i < channels; i++ ) {

				// create a new surface object
				// ------------------------------------------------------------

				CHANNEL_SURFACE * surface = new CHANNEL_SURFACE;

				if ( !surface ) {

					Release();
					return false;

				}

				// size the surface object
				// ------------------------------------------------------------

				if ( !surface->Create( cx, cy, pitch ) ) {

					Release();
					return false;

				}

				// Add the surface to our collection
				// ------------------------------------------------------------

				m_ChannelSurfaces.push_back( surface );

			}

			// Finalize the object
			// ----------------------------------------------------------------

			m_Size.cx = cx;
			m_Size.cy = cy;
			m_ChannelCount = channels;

			return true;

		}

		bool Create(
			const SIZE & size,
			const int channels = 4,
			const int pitch = 0
		) {


			return Create( size.cx, size.cy, channels, pitch );

		}

		// what about clone?

		// --------------------------------------------------------------------
		// Root object implmentation
		// --------------------------------------------------------------------

		virtual int getPitch() const {

			// What should go here?

			return getWidth() * getSizeofPixel();

		}

		virtual int getWidth() const {

			return Width();

		}

		virtual int getHeight() const {

			return Height();

		}

		virtual int getFormat() const {

			return CHANNEL_SURFACE::pixel_type::format;

		}

		virtual void * getData() const {

			// What should go here?
			// What should go here?
			// What should go here?

			return 0;

		}

		virtual void releaseData( void * ptr ) const {

			// What should go here?
			// What should go here?
			// What should go here?

		}

		virtual int getSizeofPixel() const {

			return sizeof( CHANNEL_SURFACE::pixel_type ) * ChannelCount();

		}

		// --------------------------------------------------------------------
		//	Object state functions
		// --------------------------------------------------------------------

		int ChannelCount() const {

			return m_ChannelCount;

		}

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

		int GetMaxChannels() {

			return MAX_INT;

		}

		// --------------------------------------------------------------------
		// Iterator
		// --------------------------------------------------------------------

		pixel_iterator Iterator( const int x, const int y ) {

			SURFACE_PIXEL_ITERATOR_CONTAINER surfaceIterators;

			CONTAINER::iterator it = m_ChannelSurfaces.begin();
			CONTAINER::iterator end = m_ChannelSurfaces.end();

			for ( ; it != end; it++ ) {

				surfaceIterators.push_back( (*it)->Iterator( x, y ) );

			}

			return pixel_iterator( surfaceIterators );

		}

		// --------------------------------------------------------------------
		// --------------------------------------------------------------------

		CHANNEL_SURFACE * GetChannelSurface( const int channel ) {

			CONTAINER::iterator it = m_ChannelSurfaces.begin();
			CONTAINER::iterator end = m_ChannelSurfaces.end();

			for ( int counter = 0; it != end; it++, counter++ ) {

				if ( counter == channel ) {

					return *it;

				}

			}

			return static_cast< CHANNEL_SURFACE * >( 0 );

		}

	};

	// ========================================================================
	// ========================================================================

}; // namespace BPT

#endif // !defined(AFX_TMULTICHANNELSURFACE_H__C79DE455_C71E_4E1E_ACC4_C9B0B013E3A6__INCLUDED_)
