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

#if !defined(AFX_THELPERS_H__6C1A751F_3802_4A44_BAE8_A10D7FB7B0BF__INCLUDED_)
#define AFX_THELPERS_H__6C1A751F_3802_4A44_BAE8_A10D7FB7B0BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// (SUPPORT-INCLUDES) ---------------------------------------------------------

#include "Pixels.h"
#include "TSurface.h"
#include "TDibSection.h"
#include "TBlitFramework.h"
#include "TSurfaceOps.h"

// ----------------------------------------------------------------------------

namespace BPT {

	// ========================================================================
	// Common surface types
	// ========================================================================

	typedef TSurface< PIXEL_A4R4G4B4 > SURF_A4R4G4B4;
	typedef TSurface< PIXEL_A1R5G5B5 > SURF_A1R5G5B5;
	typedef TSurface< PIXEL_A8R8G8B8 > SURF_A8R8G8B8;
	typedef TSurface< PIXEL_B4G4R4A4 > SURF_B4G4R4A4;
	typedef TSurface< PIXEL_B5G5R5A1 > SURF_B5G5R5A1;
	typedef TSurface< PIXEL_B8G8R8A8 > SURF_B8G8R8A8;
	typedef TSurface< PIXEL_R5G6B5 > SURF_R5G6B5;
	typedef TSurface< PIXEL_R5G5B5 > SURF_R5G5B5;
	typedef TSurface< PIXEL_B5G6R5 > SURF_B5G6R5;
	typedef TSurface< PIXEL_P8 > SURF_8BPP;
	typedef TSurface< PIXEL_P16 > SURF_16BPP;
	typedef TSurface< PIXEL_P32 > SURF_32BPP;

	// ========================================================================
	// Common DIBSECTIONS types
	// ========================================================================

	typedef TDIBSection< PIXEL_A1R5G5B5 > DIB_A1R5G5B5;
	typedef TDIBSection< PIXEL_A8R8G8B8 > DIB_A8R8G8B8;
	typedef TDIBSection< PIXEL_B5G5R5A1 > DIB_B5G5R5A1;
	typedef TDIBSection< PIXEL_B8G8R8A8 > DIB_B8G8R8A8;
	typedef TDIBSection< PIXEL_R5G6B5 > DIB_R5G6B5;
	typedef TDIBSection< PIXEL_R5G5B5 > DIB_R5G5B5;
	typedef TDIBSection< PIXEL_B5G6R5 > DIB_B5G6R5;

	// ========================================================================
	// Surface type conversion helpers
	// ========================================================================

	//
	//	T_NewSurfaceViaConversion()
	//

	template< class DST_SURFACE, class SRC_SURFACE > DST_SURFACE * 
	T_NewSurfaceViaConversion(
		SRC_SURFACE * pSurfaceToConvert,
		const bool deleteSourceSurface = false,
		const BLITFX * optionalBlitFX = 0
	) {

		// Use a SurfaceOp to convert the bitmap
		// ----------------------------------------------------------------

		DST_SURFACE * pNewSurface = 
			CreateBitmapFromRect<DST_SURFACE>(
				pSurfaceToConvert->Rect(), optionalBlitFX
			) << *pSurfaceToConvert;

		// If the client requested delete the source surface
		// ----------------------------------------------------------------

		if ( deleteSourceSurface ) {

			delete pSurfaceToConvert;

		}

		return pNewSurface;

	}

	// ========================================================================

	//
	//	T_NewSurfaceViaDiffusionDither()
	//

	template< class DST_SURFACE, class SRC_SURFACE > DST_SURFACE * 
	T_NewSurfaceViaDiffusionDither(
		SRC_SURFACE * pSurfaceToConvert,
		const bool deleteSourceSurface = false
	) {

		// Use a SurfaceOp to convert the bitmap
		// ----------------------------------------------------------------

		TErrorDiffusionDither<DST_SURFACE,SRC_SURFACE> ditherObj;

		DST_SURFACE * pNewSurface = ditherObj.CreateDithered( pSurfaceToConvert );

		// If the client requested delete the source surface
		// ----------------------------------------------------------------

		if ( deleteSourceSurface ) {

			delete pSurfaceToConvert;

		}

		return pNewSurface;

	}

}; // namespace BPT

// ----------------------------------------------------------------------------

#endif // !defined(AFX_THELPERS_H__6C1A751F_3802_4A44_BAE8_A10D7FB7B0BF__INCLUDED_)
