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

#if !defined(AFX_TFVF_H__0A005014_DF28_432A_A55E_82BFA4683E4B__INCLUDED_)
#define AFX_TFVF_H__0A005014_DF28_432A_A55E_82BFA4683E4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ----------------------------------------------------------------------------

#if defined( BPTLIB_SUPPORT_DX7 )

#include <d3d.h>

#endif // defined( BPTLIB_SUPPORT_DX7 )

// ----------------------------------------------------------------------------

namespace BPT {

#if defined( BPTLIB_SUPPORT_DX7 )

// ----------------------------------------------------------------------------
#pragma pack( push, FVF_packing, 1 )
// ----------------------------------------------------------------------------

	// ========================================================================
	// TRANSFORMED FVF's
	// ========================================================================

	//
	//	FVF_XYZRHWDIFFUSE
	//

	struct FVF_XYZRHWDIFFUSE {

		enum {

			Flags = D3DFVF_XYZRHW | D3DFVF_DIFFUSE 

		};

		// D3DFVF_XYZRHW

		float x;
		float y;
		float z;
		float rhw;

		// D3DFVF_DIFFUSE

		D3DCOLOR color;

	};

	//
	//	FVF_XYZRHWDIFFUSESPECULAR
	//

	struct FVF_XYZRHWDIFFUSESPECULAR {

		enum {

			Flags = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR 

		};

		// D3DFVF_XYZRHW

		float x;
		float y;
		float z;
		float rhw;

		// D3DFVF_DIFFUSE

		D3DCOLOR color;

		// D3DFVF_SPECULAR

		D3DCOLOR specular;

	};

	//
	//	FVF_XYZRHWTEX1
	//

	struct FVF_XYZRHWTEX1 {

		enum {

			Flags = D3DFVF_XYZRHW | D3DFVF_TEX1 

		};

		// D3DFVF_XYZRHW

		float x;
		float y;
		float z;
		float rhw;

		// D3DFVF_TEX1

		float tu1;
		float tv1;

	};

	//
	//	FVF_XYZRHWTEX2
	//

	struct FVF_XYZRHWTEX2 {

		enum {

			Flags = D3DFVF_XYZRHW | D3DFVF_TEX2

		};

		// D3DFVF_XYZRHW

		float x;
		float y;
		float z;
		float rhw;

		// D3DFVF_TEX1

		float tu1;
		float tv1;

		// D3DFVF_TEX2

		float tu2;
		float tv2;

	};

	//
	//	FVF_XYZRHWDIFFUSETEX1
	//

	struct FVF_XYZRHWDIFFUSETEX1 {

		enum {

			Flags = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 

		};

		// D3DFVF_XYZRHW

		float x;
		float y;
		float z;
		float rhw;

		// D3DFVF_DIFFUSE

		D3DCOLOR color;

		// D3DFVF_TEX1

		float tu1;
		float tv1;

	};

	//
	//	FVF_XYZRHWDIFFUSETEX2
	//

	struct FVF_XYZRHWDIFFUSETEX2 {

		enum {

			Flags = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX2 

		};

		// D3DFVF_XYZRHW

		float x;
		float y;
		float z;
		float rhw;

		// D3DFVF_DIFFUSE

		D3DCOLOR color;

		// D3DFVF_TEX1

		float tu1;
		float tv1;

		// D3DFVF_TEX2

		float tu2;
		float tv2;

	};

	// ========================================================================
	// UNTRANSFORMED & UNLIT FVF's
	// ========================================================================

	//
	//	FVF_XYZ
	//

	struct FVF_XYZ {

		enum {

			Flags = D3DFVF_XYZ

		};

		// D3DFVF_XYZ

		float x;
		float y;
		float z;

	};

	//
	//	FVF_XYZTEX1
	//

	struct FVF_XYZTEX1 {

		enum {

			Flags = D3DFVF_XYZ | D3DFVF_TEX1

		};

		// D3DFVF_XYZ

		float x;
		float y;
		float z;

		// D3DFVF_TEX1

		float tu1;
		float tv1;

	};

	//
	//	FVF_XYZNORMALTEX1
	//

	struct FVF_XYZNORMALTEX1 {

		enum {

			Flags = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1

		};

		// D3DFVF_XYZ

		float x;
		float y;
		float z;

		// D3DFVF_NORMAL

		float nx;
		float ny;
		float nz;

		// D3DFVF_TEX1

		float tu1;
		float tv1;

	};

	//
	//	FVF_XYZNORMALTEX2
	//

	struct FVF_XYZNORMALTEX2 {

		enum {

			Flags = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2

		};

		// D3DFVF_XYZ

		float x;
		float y;
		float z;

		// D3DFVF_NORMAL

		float nx;
		float ny;
		float nz;

		// D3DFVF_TEX1

		float tu1;
		float tv1;

		// D3DFVF_TEX2

		float tu2;
		float tv2;

	};

	// ========================================================================
	// UNTRANSFORMED & LIT FVF's
	// ========================================================================

	//
	//	FVF_XYZDIFFUSE
	//

	struct FVF_XYZDIFFUSE {

		enum {

			Flags = D3DFVF_XYZ | D3DFVF_DIFFUSE 

		};

		// D3DFVF_XYZ

		float x;
		float y;
		float z;

		// D3DFVF_DIFFUSE

		D3DCOLOR color;

	};

	//
	//	FVF_XYZDIFFUSESPECULAR
	//

	struct FVF_XYZDIFFUSESPECULAR {

		enum {

			Flags = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_SPECULAR 

		};

		// D3DFVF_XYZ

		float x;
		float y;
		float z;

		// D3DFVF_DIFFUSE

		D3DCOLOR color;

		// D3DFVF_SPECULAR

		D3DCOLOR specular;

	};

	//
	//	FVF_XYZDIFFUSETEX1
	//

	struct FVF_XYZDIFFUSETEX1 {

		enum {

			Flags = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1

		};

		// D3DFVF_XYZ

		float x;
		float y;
		float z;

		// D3DFVF_DIFFUSE

		D3DCOLOR color;

		// D3DFVF_TEX1

		float tu1;
		float tv1;

	};

	//
	//	FVF_XYZDIFFUSETEX2
	//

	struct FVF_XYZDIFFUSETEX2 {

		enum {

			Flags = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX2

		};

		// D3DFVF_XYZ

		float x;
		float y;
		float z;

		// D3DFVF_DIFFUSE

		D3DCOLOR color;

		// D3DFVF_TEX1

		float tu1;
		float tv1;

		// D3DFVF_TEX2

		float tu2;
		float tv2;

	};

	//
	//	FVF_XYZDIFFUSESPECULARTEX1
	//

	struct FVF_XYZDIFFUSESPECULARTEX1 {

		enum {

			Flags = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1

		};

		// D3DFVF_XYZ

		float x;
		float y;
		float z;

		// D3DFVF_DIFFUSE

		D3DCOLOR color;

		// D3DFVF_SPECULAR

		D3DCOLOR specular;

		// D3DFVF_TEX1

		float tu1;
		float tv1;

	};

	//
	//	FVF_XYZDIFFUSESPECULARTEX2
	//

	struct FVF_XYZDIFFUSESPECULARTEX2 {

		enum {

			Flags = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX2

		};

		// D3DFVF_XYZ

		float x;
		float y;
		float z;

		// D3DFVF_DIFFUSE

		D3DCOLOR color;

		// D3DFVF_SPECULAR

		D3DCOLOR specular;

		// D3DFVF_TEX1

		float tu1;
		float tv1;

		// D3DFVF_TEX2

		float tu2;
		float tv2;

	};

// ----------------------------------------------------------------------------
#pragma pack( pop, FVF_packing )
// ----------------------------------------------------------------------------

#endif // defined( BPTLIB_SUPPORT_DX7 )

}; // namespace BPT

#endif // !defined(AFX_TFVF_H__0A005014_DF28_432A_A55E_82BFA4683E4B__INCLUDED_)
