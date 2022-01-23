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

#if !defined(AFX_BPTDIB_H__4EE3AB65_FB91_4E19_9877_81FB6B6BDBFD__INCLUDED_)
#define AFX_BPTDIB_H__4EE3AB65_FB91_4E19_9877_81FB6B6BDBFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ----------------------------------------------------------------------------

#include "BPTBitmap.h"

// ----------------------------------------------------------------------------

namespace BPT {

//#define BPT_USE_STRETCHDIBITS_FOR_ALL_BLITS
#define BPTDIB_USE_BITMAPV5HEADER

	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//	Abstract Dib base type
	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	class ADib {

	public:

		virtual void Destroy() = 0;
		virtual bool Create( const int width, const int height, HDC hdc = static_cast<HDC>(0) ) = 0;
		virtual bool Create( const SIZE & size, HDC hdc = static_cast<HDC>(0) ) = 0;
		virtual int BitDepth() const = 0;
		virtual RECT Rect() const = 0;
		virtual SIZE Size() const = 0;
		virtual int Width() const = 0;
		virtual int Height() const = 0;
		virtual int Pitch() const = 0;
		virtual HBITMAP GetHBitmap() const = 0;
		virtual const void * GetBitmapBits() const = 0;
		virtual const void* GetBITMAPINFO() const = 0;

	};

	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// DIBSECTION wrapper (16, 24, or 32 top down dib's only!!!)
	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	template< typename PIXELTYPE, const int ALIGNMENT = 4 >
	class TDIBSection : public ADib {

	public: // traits

		typedef TSimpleBitmap<PIXELTYPE> bitmap_type;
		typedef PIXELTYPE pixel_type;
		typedef TDIBSection<PIXELTYPE> this_type;
		typedef this_type & reference;
		typedef this_type * pointer;
		typedef const this_type & const_reference;
		typedef const this_type * const_pointer;

	protected: // data

#if defined(BPTDIB_USE_BITMAPV5HEADER)
		BITMAPV5HEADER m_BITMAPV5HEADER;
#else
		BITMAPINFO m_BitmapInfo;
#endif

		void * m_BitmapBits;
		HBITMAP m_hBitmap;
		bitmap_type m_FakeBitmap;
		SIZE m_Size;
		int m_Pitch;

	protected: // internal methods

		int CalcPitch( const int width ) {

			int rawPitch = width * sizeof( PIXELTYPE );

			int leftOver = (rawPitch % ALIGNMENT);

			if ( !leftOver ) {

				return rawPitch;

			}

			return rawPitch + (ALIGNMENT - leftOver);

		}

	public: // interface

		// --------------------------------------------------------------------

		TDIBSection() : m_hBitmap(0), m_Pitch(0) {

#if defined(BPTDIB_USE_BITMAPV5HEADER)
			::ZeroMemory(&m_BITMAPV5HEADER, sizeof(m_BITMAPV5HEADER));
#else
			::ZeroMemory( &m_BitmapInfo, sizeof(m_BitmapInfo) );
#endif

			m_Size.cx = 0;
			m_Size.cy = 0;
		
		}

		virtual ~TDIBSection() {

			Destroy();

		}

		// --------------------------------------------------------------------

		virtual void Destroy() {

			m_FakeBitmap.Destroy();

			if ( m_hBitmap ) {

				// Ask GDI to destroy it's concept of this bitmap

				::DeleteObject( (HGDIOBJ) m_hBitmap );

				// Release all of our information

#if defined(BPTDIB_USE_BITMAPV5HEADER)
				::ZeroMemory(&m_BITMAPV5HEADER, sizeof(m_BITMAPV5HEADER));
#else
				::ZeroMemory( &m_BitmapInfo, sizeof(m_BitmapInfo) );
#endif

				m_BitmapBits = 0;
				m_hBitmap = 0;

			}

			m_Size.cx = 0;
			m_Size.cy = 0;

		}

		// --------------------------------------------------------------------

		bool Create( const int width, const int height, HDC hdc = static_cast<HDC>(0) ) {

			TRACE("Creating dib size (%dx%d)\n", width, height);

			// Release any previous information
			// ----------------------------------------------------------------

			Destroy();

			// Fill in the bitmap info structure 
			// ----------------------------------------------------------------

#if defined(BPTDIB_USE_BITMAPV5HEADER)

			::ZeroMemory(&m_BITMAPV5HEADER, sizeof(m_BITMAPV5HEADER));

			m_BITMAPV5HEADER.bV5Size = sizeof(m_BITMAPV5HEADER);
			m_BITMAPV5HEADER.bV5Width = width;
			m_BITMAPV5HEADER.bV5Height = -height;
			m_BITMAPV5HEADER.bV5Planes = 1;
			m_BITMAPV5HEADER.bV5BitCount = pixel_type::ActiveBits;
			m_BITMAPV5HEADER.bV5Compression = BI_BITFIELDS;
			m_BITMAPV5HEADER.bV5SizeImage = CalcPitch(width) * height;
			m_BITMAPV5HEADER.bV5XPelsPerMeter = width;
			m_BITMAPV5HEADER.bV5YPelsPerMeter = height;
			m_BITMAPV5HEADER.bV5RedMask = pixel_type::R_BitMask;
			m_BITMAPV5HEADER.bV5GreenMask = pixel_type::G_BitMask;
			m_BITMAPV5HEADER.bV5BlueMask = pixel_type::B_BitMask;
			m_BITMAPV5HEADER.bV5AlphaMask = pixel_type::A_BitMask;
			m_BITMAPV5HEADER.bV5CSType = LCS_sRGB;
			m_BITMAPV5HEADER.bV5Intent = LCS_GM_IMAGES;

#else
			::ZeroMemory( &m_BitmapInfo, sizeof(m_BitmapInfo) );

			m_BitmapInfo.bmiHeader.biSize = sizeof(m_BitmapInfo.bmiHeader);
			m_BitmapInfo.bmiHeader.biBitCount = (sizeof(PIXELTYPE) * 8);
			m_BitmapInfo.bmiHeader.biClrUsed = 0;
			m_BitmapInfo.bmiHeader.biClrImportant = 0;
			m_BitmapInfo.bmiHeader.biCompression = BI_RGB;
			m_BitmapInfo.bmiHeader.biSizeImage = 0;
			m_BitmapInfo.bmiHeader.biPlanes = 1;
			m_BitmapInfo.bmiHeader.biXPelsPerMeter = 0;
			m_BitmapInfo.bmiHeader.biYPelsPerMeter = 0; 
			m_BitmapInfo.bmiHeader.biWidth = width;
			m_BitmapInfo.bmiHeader.biHeight = -height;

#endif

			// Figure out what hdc to use
			// ----------------------------------------------------------------

			HDC usehdc = hdc;

			if ( !hdc ) {

				usehdc = GetDC( NULL );

			}

			// Ask GDI to create a DIBSECTION
			// ----------------------------------------------------------------

#if defined(BPTDIB_USE_BITMAPV5HEADER)
			m_hBitmap = ::CreateDIBSection(
				usehdc, reinterpret_cast<const BITMAPINFO *>(&m_BITMAPV5HEADER), DIB_RGB_COLORS,
				static_cast<void**>(&m_BitmapBits),
				0, 0
			);
#else
			m_hBitmap = ::CreateDIBSection(
				usehdc, &m_BitmapInfo, DIB_RGB_COLORS, 
				static_cast< void ** >( &m_BitmapBits ),
				0, 0
			);
#endif

			// If we created the hdc then destroy it.
			// ----------------------------------------------------------------

			if ( !hdc ) {

				ReleaseDC( NULL, usehdc );

			}

			// Failure?
			// ----------------------------------------------------------------

			if ( !m_hBitmap ) {

				return false;

			}

			m_Pitch = CalcPitch( width );

			m_Size.cx = width;
			m_Size.cy = height;

			if ( !m_FakeBitmap.Map(
				(BYTE *)m_BitmapBits, m_Size, m_Pitch, false, false) ) {

				Destroy();

				return false;

			}
			
			return true;

		}

		bool Create( const SIZE & size, HDC hdc = static_cast<HDC>(0) ) {

			return Create( size.cx, size.cy, hdc );

		}

		// Query
		// --------------------------------------------------------------------

		int BitDepth() const {

			return pixel_type::ActiveBits; // m_BitmapInfo.bmiHeader.biBitCount;

		}

		int Pitch() const {

			return m_Pitch;

		}

		RECT Rect() const {

			RECT rect = { 0, 0, m_Size.cx, m_Size.cy };

			return rect;

		}

		SIZE Size() const {

			return m_Size;

		}

		int Width() const {

			return m_Size.cx;

		}

		int Height() const {

			return m_Size.cy;

		}

		// --------------------------------------------------------------------

		HBITMAP GetHBitmap() const {

			return m_hBitmap;

		}

		const void * GetBITMAPINFO() const {

#if defined(BPTDIB_USE_BITMAPV5HEADER)
			return &m_BITMAPV5HEADER;
#else
			return &m_BitmapInfo;
#endif

		}

		const void * GetBitmapBits() const {

			return m_BitmapBits;

		}

		// --------------------------------------------------------------------

		bitmap_type * GetBitmapPtr() {

			return &m_FakeBitmap;

		}

	};

	// ========================================================================

	//
	//	TDibDC<>
	//

	template< typename T >
	class TDibDC : public CDC {

	public: // traits

		typedef TDibDC<T> this_type;

		typedef T dib_type;

	private:

		dib_type * m_pDib;

		CBitmap * m_pOldBitmap;

		bool m_bSetup;

	public:

		TDibDC( dib_type * pDib ) : m_pDib( pDib ), m_bSetup( false ), m_pOldBitmap( 0 ) {

			if ( pDib ) {

				if ( CreateCompatibleDC( NULL ) ) {

					m_pOldBitmap = SelectObject(
						CBitmap::FromHandle( pDib->GetHBitmap() )
					);

					m_bSetup = true;

				}

			}
		
		}

		~TDibDC() {

			if ( m_bSetup && m_pOldBitmap ) {

				SelectObject( m_pOldBitmap );

			}

		}

		bool IsSetup() const {

			return m_bSetup;

		}

	};

	// ========================================================================

	//
	//	DIBSECTION_BlitAt()
	//
	//	-- This should really call another primitive, but for now this
	//	-- will work just fine :-)
	//

	template< typename DIBSECTION > void
	T_BlitAt(
		HDC hdc, const int x, const int y, DIBSECTION & dibSection,
		const float xScale = 1.0f, const float yScale = 1.0f,
		const RECT * pOptionalSrcSubRect = 0
	) {

		// Determine the operation size

		RECT srcLimitRect = dibSection.Rect();

		RECT operationRect;

		if ( pOptionalSrcSubRect ) {

			if ( !IntersectRect( &operationRect, pOptionalSrcSubRect, &srcLimitRect ) ) {

				return /* NOP */;

			}

		} else {

			operationRect = srcLimitRect;

		}

		// Get down to business

		HBITMAP hBitmap = dibSection.GetHBitmap();

		if ( hBitmap ) {

			int w = operationRect.right - operationRect.left;
			int h = operationRect.bottom - operationRect.top;

#if defined(BPT_USE_STRETCHDIBITS_FOR_ALL_BLITS)

			int oldMode = SetStretchBltMode( hdc, STRETCH_DELETESCANS );

			StretchDIBits(
				hdc, x, y,
				(int)(w * xScale + 0.5f),
				(int)(h * yScale + 0.5f),
				operationRect.left,
				operationRect.top,
				w,
				h,
				dibSection.GetBitmapBits(),
				reinterpret_cast<const BITMAPINFO * >(dibSection.GetBITMAPINFO()),
				DIB_RGB_COLORS,
				SRCCOPY
			);

			SetStretchBltMode( hdc, oldMode );

#else

			HDC memDC = ::CreateCompatibleDC( hdc );

			if ( memDC ) {

				HBITMAP hbmOld = (HBITMAP)::SelectObject( memDC, hBitmap );

				if ( (1.0f != xScale) || (1.0f != yScale) ) {

					int oldMode = SetStretchBltMode( hdc, STRETCH_DELETESCANS );
	
					::StretchBlt(
						hdc, x, y,
						(int)(w * xScale + 0.5f),
						(int)(h * yScale + 0.5f),
						memDC,
						operationRect.left,
						operationRect.top,
						w,
						h,
						SRCCOPY
					);
	
					SetStretchBltMode( hdc, oldMode );

				} else {

					::BitBlt(
						hdc,
						x, y,
						w,
						h,
						memDC,
						operationRect.left,
						operationRect.top,
						SRCCOPY
					);

				}

				::SelectObject( memDC, hbmOld );
				::DeleteDC( memDC );

			}

#endif

		}

	}

}; // namespace BPT

#endif // !defined(AFX_BPTDIB_H__4EE3AB65_FB91_4E19_9877_81FB6B6BDBFD__INCLUDED_)
