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


#if !defined(AFX_TPAINTLIBWRAPPER_H__E57105C8_8A3A_4E4A_A75A_1F79CEC452CA__INCLUDED_)
#define AFX_TPAINTLIBWRAPPER_H__E57105C8_8A3A_4E4A_A75A_1F79CEC452CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning( disable : 4786 )

// (SUPPORT-INCLUDES) ---------------------------------------------------------

#include "THelpers.h"
#include "TPSDFileCode.h"

// ----------------------------------------------------------------------------

namespace BPT {

	/*
	::
	::	THELPER_PSDLoad()
	::
	*/

	template< class SURFACE > SURFACE *
	THELPER_PSDLoad( const char * filename )
	{
		typedef TPSDFileLoader<SURFACE> LOADER;

		LOADER PSDLoader;

		LOADER::loaded_data results = PSDLoader.LoadFromFile( filename, false );

		if ( results.first ) {

			return results.first;

		}

		return static_cast< SURFACE * >( 0 );

	}

	// (SUPPORT-PROTOTYPES) ---------------------------------------------------

	SURF_A8R8G8B8 * HELPER_LoadPicturePrim( HINSTANCE hInstance, const int resourceID );
	SURF_A8R8G8B8 * HELPER_LoadPicturePrim( const char * filename );

	// (TEMPLATE-FUNCTIONS) ---------------------------------------------------

	//
	//	HELPER_LoadPicture()
	//

	template< class SURFACE > SURFACE * 
	HELPER_LoadPicture( HINSTANCE hInstance, const int resourceID ) {

		// Ask the common format loader to load it's bitmap
		// --------------------------------------------------------------------

		SURF_A8R8G8B8 * pWorkingBitmap = HELPER_LoadPicturePrim( hInstance, resourceID );

		if ( 0 == pWorkingBitmap ) {

			return static_cast< SURFACE * >( 0 );

		}

		// Create a new surface bitmap and blit the contents over :)
		// --------------------------------------------------------------------

		return T_NewSurfaceViaConversion<SURFACE,SURF_A8R8G8B8>( pWorkingBitmap, true );

	}

	//
	//	HELPER_LoadPicture()
	//

	template< class SURFACE > SURFACE * 
	HELPER_LoadPicture( const char * filename ) {

		// Ask the common format loader to load it's bitmap
		// --------------------------------------------------------------------

		SURF_A8R8G8B8 * pWorkingBitmap = HELPER_LoadPicturePrim( filename );

		if ( 0 == pWorkingBitmap ) {

			return static_cast< SURFACE * >( 0 );

		}

		// Create a new surface bitmap and blit the contents over :)
		// --------------------------------------------------------------------

		return T_NewSurfaceViaConversion<SURFACE,SURF_A8R8G8B8>( pWorkingBitmap, true );

	}

}; // namespace BPT

// ----------------------------------------------------------------------------

#endif // !defined(AFX_TPAINTLIBWRAPPER_H__E57105C8_8A3A_4E4A_A75A_1F79CEC452CA__INCLUDED_)
