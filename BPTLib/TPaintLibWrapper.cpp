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


#include "stdafx.h"
#include "TPaintLibWrapper.h"

// (EXTERNAL-INCLUDES) --------------------------------------------------------

// (PUBLIC-CODE) --------------------------------------------------------------

namespace BPT {

	// (OTHER-HELPER-FUNCTIONS) -----------------------------------------------

	// (MAIN-HELPER-FUNCTIONS) ================================================

	//
	//	HELPER_LoadPicturePrim
	//

	SURF_A8R8G8B8 * 
	HELPER_LoadPicturePrim( HINSTANCE hInstance, const int resourceID )
	{
		SURF_A8R8G8B8 * newSurface = static_cast< SURF_A8R8G8B8 * >( 0 );

		// Try the various support functions until one works
		// --------------------------------------------------------------------

		// --------------------------------------------------------------------

		return static_cast< SURF_A8R8G8B8 * >( 0 );
	}

	//
	//	HELPER_LoadPicturePrim
	//

	SURF_A8R8G8B8 * 
	HELPER_LoadPicturePrim( const char * filename )
	{
		SURF_A8R8G8B8 * newSurface = static_cast< SURF_A8R8G8B8 * >( 0 );

		// Try the various support functions until one works
		// --------------------------------------------------------------------

#if 1 // TEST

		newSurface = THELPER_PSDLoad<SURF_A8R8G8B8>( filename );

		if ( newSurface ) {

			return newSurface;

		}

#endif

		// --------------------------------------------------------------------

		return static_cast< SURF_A8R8G8B8 * >( 0 );
	}

}; // namespace BPT

