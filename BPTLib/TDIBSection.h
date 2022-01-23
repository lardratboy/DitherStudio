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


#if !defined(AFX_TDIBSECTION_H__2C7291C7_3695_4B22_A5B6_C1391CE06844__INCLUDED_)
#define AFX_TDIBSECTION_H__2C7291C7_3695_4B22_A5B6_C1391CE06844__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ----------------------------------------------------------------------------

#if !defined(STRICT)
#define STRICT
#endif

#include <windows.h>

#include "TSurface.h"
#include "Rectangle.h"

#define BPTLIB_USE_MEMDC_FOR_DIBSECTION

// ----------------------------------------------------------------------------

namespace BPT {

	//
	//	TDIBSection
	//
	//	NOTES:
	//
	//		This was designed for (16,24,32bpp) ONLY!!!
	//		This code only creates TOP-DOWN DIB's!!!
	//

	template< class PIXELTYPE >
	class TDIBSection : public TSurface< PIXELTYPE, 4 > {

	private:

		BITMAPINFO m_BitmapInfo;
		void * m_BitmapBits;
		HBITMAP m_hBitmap;

#if defined(BPTLIB_USE_MEMDC_FOR_DIBSECTION)
		HBITMAP m_hOldBitmap;
		HDC m_hMemDC;
#endif // defined(BPTLIB_USE_MEMDC_FOR_DIBSECTION)


	public:

		// --------------------------------------------------------------------

		typedef TSurface< PIXELTYPE, 4 > base_type;
		typedef TDIBSection< PIXELTYPE > this_type;
		typedef this_type & reference;
		typedef this_type * pointer;
		typedef const this_type & const_reference;
		typedef const this_type * const_pointer;

		// --------------------------------------------------------------------

		TDIBSection() : m_hBitmap(0) {

			::ZeroMemory( &m_BitmapInfo, sizeof(m_BitmapInfo) );
		
#if defined(BPTLIB_USE_MEMDC_FOR_DIBSECTION)

			m_hMemDC = 0;
			m_hOldBitmap = 0;

#endif // defined(BPTLIB_USE_MEMDC_FOR_DIBSECTION)

		}

		virtual ~TDIBSection() {

			Release();

		}

		// --------------------------------------------------------------------

		virtual void Release() {

			base_type::Release();

			::ZeroMemory( &m_BitmapInfo, sizeof(m_BitmapInfo) );

			// Unhook the dibsection from the memory dc

#if defined(BPTLIB_USE_MEMDC_FOR_DIBSECTION)

			if ( m_hOldBitmap && m_hMemDC ) {

				::SelectObject( m_hMemDC, m_hOldBitmap );

				m_hOldBitmap = 0;

			}

#endif // defined(BPTLIB_USE_MEMDC_FOR_DIBSECTION)

			// Ask GDI to destroy it's concept of this bitmap

			if ( m_hBitmap ) {

				::DeleteObject( (HGDIOBJ) m_hBitmap );

				m_BitmapBits = 0;
				m_hBitmap = 0;

			}

#if defined(BPTLIB_USE_MEMDC_FOR_DIBSECTION)

			if ( m_hMemDC ) {

				::DeleteDC( m_hMemDC );

				m_hMemDC = 0;

			}

#endif // defined(BPTLIB_USE_MEMDC_FOR_DIBSECTION)

		}

		// --------------------------------------------------------------------

		bool Create( const int width, const int height, HDC hdc = static_cast<HDC>(0) ) {

			// Release any previous information
			// ----------------------------------------------------------------

			Release();

			// Fill in the bitmap info structure 
			// ----------------------------------------------------------------

			::ZeroMemory( &m_BitmapInfo, sizeof(m_BitmapInfo) );

			m_BitmapInfo.bmiHeader.biSize = sizeof(m_BitmapInfo.bmiHeader);
			m_BitmapInfo.bmiHeader.biBitCount = PIXELTYPE::Bits;
			m_BitmapInfo.bmiHeader.biClrUsed = 0;
			m_BitmapInfo.bmiHeader.biClrImportant = 0;
			m_BitmapInfo.bmiHeader.biCompression = BI_RGB;
			m_BitmapInfo.bmiHeader.biSizeImage = 0;
			m_BitmapInfo.bmiHeader.biPlanes = 1;
			m_BitmapInfo.bmiHeader.biXPelsPerMeter = 0;
			m_BitmapInfo.bmiHeader.biYPelsPerMeter = 0; 
			m_BitmapInfo.bmiHeader.biWidth = width;
			m_BitmapInfo.bmiHeader.biHeight = -height;

			// Figure out what hdc to use
			// ----------------------------------------------------------------

#if defined(BPTLIB_USE_MEMDC_FOR_DIBSECTION)

			m_hMemDC = CreateCompatibleDC( hdc );

			if ( !m_hMemDC ) {

				Release();

				return false;

			}

#endif // defined(BPTLIB_USE_MEMDC_FOR_DIBSECTION)

			// Ask GDI to create a DIBSECTION
			// ----------------------------------------------------------------

			m_hBitmap = ::CreateDIBSection( 
#if defined(BPTLIB_USE_MEMDC_FOR_DIBSECTION)
				m_hMemDC, 
#else
				NULL,
#endif
				&m_BitmapInfo, DIB_RGB_COLORS, 
				static_cast< void ** >( &m_BitmapBits ),
				0, 0
			);

			// Failure?
			// ----------------------------------------------------------------

			if ( !m_hBitmap ) {

				Release();

				return false;

			}

			// Select our bitmap into place
			// ----------------------------------------------------------------

#if defined(BPTLIB_USE_MEMDC_FOR_DIBSECTION)

			m_hOldBitmap = (HBITMAP)::SelectObject( m_hMemDC, m_hBitmap );

#endif // defined(BPTLIB_USE_MEMDC_FOR_DIBSECTION)

			// Finally
			// ----------------------------------------------------------------

			return Map( m_BitmapBits, width, height, 0, false );

		}

		bool Create( const SIZE & size, HDC hdc = static_cast<HDC>(0) ) {

			return Create( size.cx, size.cy, hdc );

		}

		// --------------------------------------------------------------------

		HBITMAP GetHBitmap() const {

			return m_hBitmap;

		}

		const BITMAPINFO * GetBITMAPINFO() const {

			return &m_BitmapInfo;

		}

		const void * GetBitmapBits() const {

			return m_BitmapBits;

		}

		HDC GetMemoryDC() const {

			return m_hMemDC;

		}

		// --------------------------------------------------------------------
		// Surface stream operator's
		// --------------------------------------------------------------------

		template< class T > reference operator<<( T & rhs ) {
	
			rhs( *this );
			return *this;
	
		}

	};

	// ========================================================================

	//
	//	DIBSECTION_BlitAt()
	//
	//	-- This should really call another primitive, but for now this
	//	-- will work just fine :-)
	//

	template< class DIBSECTION > void
	T_BlitAt(
		HDC hdc, const int x, const int y, DIBSECTION & dibSection,
		const float xScale = 1.0f, const float yScale = 1.0f,
		const SRect * optionalSrcSubRect = 0
	) {

		// Determine the operation size

		SRect srcLimitRect( dibSection.Rect() );

		SRect operationRect;

		if ( optionalSrcSubRect ) {

			if ( !operationRect.Intersection( optionalSrcSubRect, &srcLimitRect ) ) {

				return /* NOP */;

			}

		} else {

			operationRect = srcLimitRect;

		}

		// Get down to business

		HBITMAP hBitmap = dibSection.GetHBitmap();

		if ( hBitmap ) {

#if 0

			int oldMode = SetStretchBltMode( hdc, STRETCH_DELETESCANS );

			StretchDIBits(
				hdc, x, y,
				(int)(operationRect.Width() * xScale + 0.5f),
				(int)(operationRect.Height() * yScale + 0.5f),
				operationRect.left,
				operationRect.top,
				operationRect.Width(),
				operationRect.Height(),
				dibSection.GetBitmapBits(),
				dibSection.GetBITMAPINFO(),
				DIB_RGB_COLORS,
				SRCCOPY
			);

			SetStretchBltMode( hdc, oldMode );

#else

			// Memory DC fun!
			// ----------------------------------------------------------------

#if defined(BPTLIB_USE_MEMDC_FOR_DIBSECTION)
			HDC hMemoryDC = dibSection.GetMemoryDC();
#else
			HDC hMemoryDC = 0;
#endif

			HDC memDC = hMemoryDC;

			HBITMAP hbmOld = 0;
			
			if ( !hMemoryDC ) {
				
				memDC = ::CreateCompatibleDC( hdc );

				if ( !memDC ) {

					// error!!!!!!!!!!!!!!!!!!!!!

					return /* nop */;

				}

				HBITMAP hbmOld = (HBITMAP)::SelectObject( memDC, hBitmap );

			}

			// ----------------------------------------------------------------

			if ( (1.0f != xScale) || (1.0f != yScale) ) {

				int oldMode = SetStretchBltMode( hdc, STRETCH_DELETESCANS );
	
				::StretchBlt(
					hdc, x, y,
					(int)(operationRect.Width() * xScale + 0.5f),
					(int)(operationRect.Height() * yScale + 0.5f),
					memDC,
					operationRect.left,
					operationRect.top,
					operationRect.Width(),
					operationRect.Height(),
					SRCCOPY
				);
	
				SetStretchBltMode( hdc, oldMode );

			} else {

				::BitBlt(
					hdc,
					x, y,
					operationRect.Width(),
					operationRect.Height(),
					memDC,
					operationRect.left,
					operationRect.top,
					SRCCOPY
				);

			}

			if ( !hMemoryDC ) {

				::SelectObject( memDC, hbmOld );
				::DeleteDC( memDC );

			}

#endif

			GdiFlush();

		} else {

			if ( dibSection.HasPixels() ) {

				SIZE size = {
					(int)(operationRect.Width() * xScale + 0.5f),
					(int)(operationRect.Height() * yScale + 0.5f)
				};

				SRect clipRect( x, y, size );
				char msg[ 256 ];

				::sprintf( msg, "Invalid DIBSection (%dx%d)?", dibSection.Width(), dibSection.Height() );
				::PatBlt( hdc, x, y, size.cx, size.cy, BLACKNESS );
				int oldBKMode = ::SetBkMode( hdc, TRANSPARENT );
				::COLORREF oldColor = ::SetTextColor( hdc, RGB(255,255,255) );
				::ExtTextOut( hdc, x, y, ETO_CLIPPED, &clipRect, msg, ::strlen( msg ), 0 );
				::SetTextColor( hdc, oldColor );
				::SetBkMode( hdc, oldBKMode );

			}

		}

	}

	// ========================================================================

}; // namespace BPT

#endif // !defined(AFX_TDIBSECTION_H__2C7291C7_3695_4B22_A5B6_C1391CE06844__INCLUDED_)
