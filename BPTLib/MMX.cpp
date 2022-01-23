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
#include "MMX.h"
#include <windows.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

namespace BPT {

	// ------------------------------------------------------------------------

	bool CMMX::s_IsAvailable = false;
	bool CMMX::s_IsEnabled = false;

	// ------------------------------------------------------------------------

	void
	CMMX::Detect() {

		// We load the CPU-detection DLL. If we aren't able to load the DLL,
		// we'll have to assume that MMX Technology is not present, just to
		// be safe.
     
		HINSTANCE hLibrary = LoadLibrary( "CPUINF32.DLL" );

		if ( hLibrary ) {

			// -----------------------------------------------------------------

		    typedef DWORD (FAR* VOIDFUNC)();
		    VOIDFUNC WinCPUFeatures =
		        (VOIDFUNC)GetProcAddress(hLibrary, "wincpufeatures");
		
		    // MMX(tm) Technology has been assigned bit 23 of the feature flags.
		    
		    const DWORD MMXTECHNOLOGY_FLAG = 0x00800000;

		    BOOL bResult = WinCPUFeatures != NULL && 
		        (WinCPUFeatures() & MMXTECHNOLOGY_FLAG);
		
		    FreeLibrary(hLibrary);

			// -----------------------------------------------------------------

			ForceAvailability( (FALSE != bResult) );
			Enable( (FALSE != bResult) );

		} else {

			ForceAvailability( false );
			Enable( false );

		}

	}

}; // namespace BPT


