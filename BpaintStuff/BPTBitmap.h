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

#if !defined(AFX_BPTBITMAP_H__B21246CA_A04D_44AE_BD3B_AD720ED5ABCC__INCLUDED_)
#define AFX_BPTBITMAP_H__B21246CA_A04D_44AE_BD3B_AD720ED5ABCC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <iterator>
#include "BPTRasterOps.h"
#include "BPTBlitter.h"

namespace BPT {

	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// Pixel type
	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#if 0
	typedef unsigned long PIXEL_RGB32;
	typedef unsigned short PIXEL_RGB16;
	typedef unsigned char PIXEL_8;
#endif

	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// Bitmap type
	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	template< typename T > class TSimpleBitmap {

	protected:

		SIZE m_Size;
		BYTE * m_Data;
		size_t m_DataSize;
		int m_Pitch;
		bool m_OwnsData;

	public:

		// Traits
		// --------------------------------------------------------------------

		typedef TSimpleBitmap<T> this_type;
		typedef this_type & reference;
		typedef this_type * pointer;
		typedef const this_type & const_reference;
		typedef const this_type * const_pointer;
		typedef TCopyROP<T> default_transfer_op;

		// Iterators etc.
		// --------------------------------------------------------------------

		typedef T pixel_type;
		typedef pixel_type * pixel_iterator;
		typedef std::reverse_iterator<pixel_iterator> reverse_iterator;
		typedef std::pair< pixel_iterator, pixel_iterator > iterator_pair_t;
		typedef std::pair< reverse_iterator, reverse_iterator > rIterator_pair_t;

		// construction / destruction
		// --------------------------------------------------------------------

		TSimpleBitmap() {

			m_DataSize = 0;
			m_Pitch = 0;
			m_OwnsData = false;
			m_Data = 0;
			m_Size.cx = 0;
			m_Size.cy = 0;

		}

		~TSimpleBitmap() {

			Destroy();

		}

		// Setup / shutdown
		// --------------------------------------------------------------------

		bool Map(
			BYTE * data, 
			const SIZE & size, 
			const int pitch, 
			const bool ownData = false,
			const bool callDestroy = true
		) {

			if ( callDestroy ) {

				Destroy();

			}

			m_OwnsData = ownData;
			m_Data = data;
			m_Size = size;
			m_Pitch = pitch;
			m_DataSize = (pitch * size.cy);

			return true;

		}

		bool Create( const SIZE & size, const int forcePitchValue = 0 ) {

			Destroy();

			// TODO
			// what about alignment it would be awesome if the start of a scanline would be aligned with some 

			int pitch = (size.cx * sizeof(T)); 

			if ( forcePitchValue ) {

				if ( pitch > forcePitchValue ) {

					return false;

				}

				pitch = forcePitchValue;

			}

			// TODO - alignment

			BYTE * pData = new BYTE [ pitch * size.cy ];

			if ( !pData ) {

				return false;

			}

			return Map( pData, size, pitch, true, false );

		}

		bool Create( const int w, const int h, const int forcePitchValue = 0 ) {

			SIZE size = { w, h };

			return Create( size, forcePitchValue );

		}

		void Destroy() {

			if ( m_Data && m_OwnsData ) {

				delete [] m_Data;

				m_OwnsData = false;

			}

			m_Data = 0;
			m_Pitch = 0;
			m_DataSize = 0;
			m_Size.cx = 0;
			m_Size.cy = 0;

		}

		// --------------------------------------------------------------------

		void ReinterpretDimensions(const int w,const int h) {

			ASSERT((w * h) == (m_Size.cx * m_Size.cy));

			m_Size.cx = w;
			m_Size.cy = h;
			m_Pitch = w * sizeof(pixel_type);

		}

		bool MakeBitmapViewForRect( this_type & source, const RECT& view ) {

			if ((0 > view.left) || (view.right > source.Width()) || (0 > view.top) || (view.bottom > source.Height())) return false;

			SIZE viewSize = { view.right - view.left, view.bottom - view.top };

			pixel_iterator it = source.Iterator(view.left, view.top);

			if (nullptr == it) return false;

			return Map(reinterpret_cast<BYTE*>(it), viewSize, source.Pitch(), false, false);

		}

		// --------------------------------------------------------------------

		bool Copy(this_type* p, BLITFX fx = BLITFX())
		{
			if (!Create(p->Size())) return false;

			T_Blit(this, 0, 0, p, default_transfer_op(), &fx);

			return true;
		}

		bool Copy(this_type& r, BLITFX fx = BLITFX())
		{
			return Copy(&r, fx);
		}

		this_type* Clone(BLITFX fx = BLITFX()) const {

			this_type* p = new this_type;

			if (!p->Create(Size(), Pitch()))
			{
				return nullptr;
			}

			T_Blit(p, 0, 0, this, default_transfer_op(),&fx);

			return p;

		}

		template< typename S, typename TOP = TCopyROP<typename S::pixel_type, pixel_type> >
		S* Convert(BLITFX fx = BLITFX()) const {

			S* p = new S;

			if (!p->Create(Size()))
			{
				return nullptr;
			}

			T_Blit(p, 0, 0, this, TOP(),&fx);

			return p;
		}

		this_type* Extract(const RECT& view,BLITFX fx = BLITFX()) const {

			this_type* p = new this_type;

			SIZE s = SizeOfRect(&view);

			if (!p->Create(s.cx,s.cy))
			{
				return nullptr;
			}

			T_Blit(p, -view.left, -view.top, this, default_transfer_op(), &fx);

			return p;

		}

		// Misc operations
		// --------------------------------------------------------------------

		void ClearBuffer( const T value ) {

			if ( m_Data ) {
	
				for ( int y = 0; y < m_Size.cy; y++ ) {

					pixel_iterator it = Iterator( 0, y );

					for ( int x = 0; x < m_Size.cx; x++ ) {

						*it++ = value;

					}

				}

			}

		}

		// Query
		// --------------------------------------------------------------------

		__forceinline RECT Rect() const {

			RECT rect = { 0, 0, m_Size.cx, m_Size.cy };

			return rect;

		}

		__forceinline SIZE Size() const {

			return m_Size;

		}

		__forceinline int Width() const {

			return m_Size.cx;

		}

		__forceinline int Height() const {

			return m_Size.cy;

		}

		__forceinline int Pitch() const {

			return m_Pitch;

		}

#if 1 // revision for using BPTLIB pixel's

		__forceinline int ChannelCount() {

			return pixel_type::GetChannelCount();

		}

#endif

		// Simplistic iterator concept.
		// --------------------------------------------------------------------

		__forceinline pixel_iterator Iterator( const int x, const int y ) const {

			if ( m_Data ) {

				ASSERT((0 <= x) && (x < Width()) && (0 <= y) && (y < Height()));

				return pixel_iterator(
					reinterpret_cast<T *>(
						&m_Data[ (x * sizeof(T)) + (y * m_Pitch) ]
					)
				);

			}

			return pixel_iterator( nullptr );

		}

		__forceinline reverse_iterator rIterator( const int x, const int y ) const {

			return reverse_iterator( Iterator( x, y ) + 1 );

		}

		// --------------------------------------------------------------------

		// TODO test this - really need a test framework

		__forceinline POINT PointFromIterator(pixel_iterator it) const
		{
			if (!m_Data) return { -1,-1 };

			const BYTE* p = reinterpret_cast<const BYTE*>(it);
			auto offset = (p - m_Data);

			POINT pt = { (offset / sizeof(T)) % Width(), (offset / Pitch()) };

			return pt;
		}

		// --------------------------------------------------------------------

		__forceinline iterator_pair_t safer_Iterator(const int x, const int y,const int count) const
		{
			auto remaining = count - (x - Width() - 1); // TODO - validate

			if (0 > remaining)
			{
				remaining += count;
			}

			if (remaining > count)
			{
				remaining = count;
			}

			//TRACE("remaining %d (%d, %d, %d)\n", remaining, x, y, count);

			if ((0 >= remaining) || (0 > y) || (y >= Height()))
			{
				return iterator_pair_t(nullptr, nullptr); /* NOP */
			}

			pixel_iterator it = Iterator(x, y);

			return iterator_pair_t(it, it + remaining);
		}

		__forceinline rIterator_pair_t safer_rIterator(const int x, const int y, const int count) const
		{
			auto remaining = count - (x - Width() - 1); // TODO - validate

			if (0 > remaining)
			{
				remaining += count;
			}

			if (remaining > count)
			{
				remaining = count;
			}

			//TRACE("remaining %d (%d, %d, %d)\n", remaining, x, y, count);

			if ((0 >= remaining) || (0 > y) || (y >= Height()))
			{
				return rIterator_pair_t(nullptr, nullptr); /* NOP */
			}

			reverse_iterator it = rIterator(x, y);

			return rIterator_pair_t(it, it + remaining);
		}

		// --------------------------------------------------------------------

		__forceinline void LineAdvanceIterator( pixel_iterator & it, const int multiplier = 1 ) {

			it = reinterpret_cast<T *>(
				(reinterpret_cast< BYTE * >(it) + (m_Pitch * multiplier) )
			);

		}

		// --------------------------------------------------------------------

		__forceinline void * GetDataPtr() {

			return m_Data;

		}

	};

	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//	CLUT table
	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	//
	//	TPow2Clut
	//

	template< typename O, const int POW2 = 8 >
	class TPow2Clut {

	public: // traits

		typedef O output_type;

		enum {

			TABLE_SIZE = (1 << POW2)
			, INDEX_MASK = (1 << POW2) - 1

		};

	private: // data

		output_type m_Table[TABLE_SIZE];

	public: // interface

		output_type& RawLookup(const int i) {

			return m_Table[i];

		}

		output_type& SafeLookup(const int i) {

			return RawLookup(i & INDEX_MASK);

		}

		output_type& operator[](const int i) {

#ifdef _DEBUG

			return SafeLookup(i);

#else

			return RawLookup(i);

#endif

		}

		bool Included(const output_type& value) const {

			for (int i = 0; i < TABLE_SIZE; i++) {

				if (value == m_Table[i]) {

					return true;

				}

			}

			return false;

		}

		int FindIndex(const output_type& value, const int defValue) const {

			for (int i = 0; i < TABLE_SIZE; i++) {

				if (value == m_Table[i]) {

					return i;

				}

			}

			return defValue;

		}

		// Set
		// --------------------------------------------------------------------

		void Set(const int index, const output_type& output) {

			m_Table[(index & INDEX_MASK)] = output;

		}

		void SetN(const int startIndex, const int count, const output_type& output) {

			int index = startIndex;

			for (int i = 0; i < count; i++) {

				Set(index++, output);

			}

		}

		// --------------------------------------------------------------------

		output_type* GetLowLevelTablePtr() {

			return m_Table;

		}

	};

#if 0

	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//	256 color picture class
	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	class C8bppPicture : public TSimpleBitmap<PIXEL_8> {

	public: // Traits

		typedef C8bppPicture this_type;
		typedef TSimpleBitmap<PIXEL_8> base_type;
		typedef base_type::pixel_type pixel_type;
		typedef base_type::pixel_iterator pixel_iterator;
		typedef base_type::reverse_iterator reverse_iterator;
		typedef RGBQUAD color_value_type;
			
	private: // Data

	public: // interface

		color_value_type m_PaletteData[ 256 ];

	};

#endif

	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// Misc creation helpers
	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	//
	//	T_Create90DegreeRotatedBitmap()
	//

	template< typename T >
	T * T_Create90DegreeRotatedBitmap( T & src ) {

		// Create and size the output bitmap
		// --------------------------------------------------------------------

		T * pNew = new T;

		if ( !pNew ) {

			return 0;

		}

		int src_w = src.Width();
		int src_h = src.Height();

		if ( !pNew->Create( src_h, src_w ) ) {

			delete pNew;

			return 0;

		}

		// Now go through the process of transfering the pixels...
		// --------------------------------------------------------------------

		for ( int y = 0; y < src_w; y++ ) {

			T::pixel_iterator dstIT = pNew->Iterator( 0, y );

			T::pixel_iterator srcIT = src.Iterator( y, src_h - 1 );

			for ( int x = 0; x < src_h; x++ ) {

				*dstIT++ = *srcIT;

				src.LineAdvanceIterator( srcIT, -1 );

			}

		}

		// --------------------------------------------------------------------

		return pNew;

	}

	// ------------------------------------------------------------------------

	//
	//	T_CreateOutlinedBitmap<>
	//

	template< typename T >
	T * T_CreateOutlinedBitmap(
		T & src, const typename T::pixel_type chromakey, const typename T::pixel_type outlineColor
	) {

		// Create and size the output bitmap
		// --------------------------------------------------------------------

		T * pNew = new T;

		if ( !pNew ) {

			return 0;

		}

		if ( !pNew->Create( src.Width() + 2, src.Height() + 2 ) ) {

			delete pNew;

			return 0;

		}

		// --------------------------------------------------------------------

		pNew->ClearBuffer( chromakey );

		typedef BPT::TMultiModeBrushTransferROP< typename T::pixel_type > helper_type;

		// top

		BPT::T_Blit( 
			*pNew, 1, 0, src, helper_type(
				helper_type::SINGLE, outlineColor, chromakey
			 )
		);

		// left

		BPT::T_Blit( 
			*pNew, 0, 1, src, helper_type(
				helper_type::SINGLE, outlineColor, chromakey
			 )
		);

		// right

		BPT::T_Blit( 
			*pNew, 2, 1, src, helper_type(
				helper_type::SINGLE, outlineColor, chromakey
			 )
		);

		// bottom

		BPT::T_Blit( 
			*pNew, 1, 2, src, helper_type(
				helper_type::SINGLE, outlineColor, chromakey
			 )
		);

		// put the original back down

		BPT::T_Blit( 
			*pNew, 1, 1, src, helper_type(
				helper_type::SOURCE, outlineColor, chromakey
			 )
		);

		// we're done!

		return pNew;

	}

	// ------------------------------------------------------------------------

	//
	//	T_TransparentAwareCreateScaledBitmap()
	//

	template< typename T, const int SHIFT, typename TOP >
	T * T_TransparentAwareCreateScaledBitmap(
		T & src, const int w, const int h, 
		const typename T::pixel_type chromaKey, TOP op ) {

		// Validate
		// --------------------------------------------------------------------

		if ( (0 >= w) || (0 >= h) ) {

			return 0;

		}

		// Create and size the output bitmap
		// --------------------------------------------------------------------

		T * pNew = new T;

		if ( !pNew ) {

			return 0;

		}

		if ( !pNew->Create( w, h ) ) {

			delete pNew;

			return 0;

		}

		pNew->ClearBuffer( chromaKey );

		// Setup the fractional step values
		// --------------------------------------------------------------------

		int sw = src.Width();
		int sh = src.Height();

		int wd = (1 != sw) ? sw - 1 : 1;
		int xd = ((w - 1) << SHIFT) / wd;

		int hd = (1 != sh) ? sh - 1 : 1;
		int yd = ((h - 1) << SHIFT) / hd;

		// Now go through the process of transfering the pixels...
		// This assumes that the pixel_iterator type is random access!
		// --------------------------------------------------------------------

		int yf = 0;

		for ( int y = 0; y < sh; y++ ) {

			T::pixel_iterator dstIT = pNew->Iterator( 0, (yf >> SHIFT) );
				
			T::pixel_iterator srcIT = src.Iterator( 0, y );

			yf += yd;

			int xf = 0;

			for ( int x = 0; x < sw; x++ ) {

				T::pixel_type t = *srcIT++;

				if ( chromaKey != t ) {

					int offset = (xf >> SHIFT);

					if ( chromaKey != *(dstIT + offset) ) {

						op( *(dstIT + offset), t );

					} else {

						*(dstIT + offset) = t;

					}

				}

				xf += xd;

			}

		}

		// --------------------------------------------------------------------

		return pNew;

	}

}; // namespace BPT

#endif // !defined(AFX_BPTBITMAP_H__B21246CA_A04D_44AE_BD3B_AD720ED5ABCC__INCLUDED_)
