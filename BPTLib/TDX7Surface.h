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


#if !defined(AFX_TDX7SURFACE_H__550E3DFF_6C46_47D1_9EC3_92354690F9CB__INCLUDED_)
#define AFX_TDX7SURFACE_H__550E3DFF_6C46_47D1_9EC3_92354690F9CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TSurface.h"

// ----------------------------------------------------------------------------

#if defined( BPTLIB_SUPPORT_DX7 )

#include <ddraw.h>

#endif // defined( BPTLIB_SUPPORT_DX7 )

// ----------------------------------------------------------------------------

namespace BPT {

#if defined( BPTLIB_SUPPORT_DX7 )

	//
	//	CanMap()
	//

	template< class TSURFACE >
	bool CanMap( TSURFACE & tSurface, const DDSURFACEDESC2 * lpDDSurfaceDesc2 ) {

		// This should eventually do something

		return true;

	}

	//
	//	Map()
	//

	template< class TSURFACE >
	bool Map( TSURFACE & tSurface, const DDSURFACEDESC2 * lpDDSurfaceDesc2 ) {

		if ( !CanMap<TSURFACE>( tSurface, lpDDSurfaceDesc2 ) ) {

			return false;

		}

		return tSurface.Map(
			lpDDSurfaceDesc2->lpSurface,
			lpDDSurfaceDesc2->dwWidth,
			lpDDSurfaceDesc2->dwHeight,
			lpDDSurfaceDesc2->lPitch,
			false, false
		);

	}

	//
	//	Map()
	//

	template< class TSURFACE >
	bool Map( TSURFACE & tSurface, IDirectDrawSurface7 * surfacePtr ) {

		tSurface.Release();

		DDSURFACEDESC2 surfDesc;

		surfDesc.dwSize = sizeof( surfDesc );

		HRESULT hr = surfacePtr->Lock( 0, &surfDesc, 0, 0 );

		if ( FAILED( hr ) ) {

			return false;

		}

		if ( !Map<TSURFACE>( tSurface, &surfDesc ) ) {

			surfacePtr->Unlock( 0 );
			return false;

		}

		return true;

	}

	//
	//	UnMap()
	//

	template< class TSURFACE >
	bool UnMap( TSURFACE & tSurface, IDirectDrawSurface7 * surfacePtr ) {

		tSurface.Release();

		return SUCCEEDED( surfacePtr->Unlock( 0 ) );

	}

#endif // defined( BPTLIB_SUPPORT_DX7 )

}; // namespace BPT

#endif // !defined(AFX_TDX7SURFACE_H__550E3DFF_6C46_47D1_9EC3_92354690F9CB__INCLUDED_)
