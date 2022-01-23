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


#if !defined(AFX_MMX_H__154776AB_0DCF_4F24_8D73_BAB0471958D3__INCLUDED_)
#define AFX_MMX_H__154776AB_0DCF_4F24_8D73_BAB0471958D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace BPT {

	// ========================================================================

	//
	//	CMMX
	//

	class CMMX {

	private:

		static bool s_IsAvailable;
		static bool s_IsEnabled;

	public:

		// --------------------------------------------------------------------

		static bool Available() {

			return (s_IsAvailable && s_IsEnabled);

		}

		// --------------------------------------------------------------------

		static void ForceAvailability( const bool available = true ) {

			s_IsAvailable = available;

		}

		static void Enable( const bool enable = true ) {

			s_IsEnabled = enable;

		}

		static void Detect();

	};

	// ========================================================================

}; // namespace BPT

#endif // !defined(AFX_MMX_H__154776AB_0DCF_4F24_8D73_BAB0471958D3__INCLUDED_)
