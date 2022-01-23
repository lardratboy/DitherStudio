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
// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__0661A047_BA7A_4053_AFD8_5DA83D26620B__INCLUDED_)
#define AFX_STDAFX_H__0661A047_BA7A_4053_AFD8_5DA83D26620B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define _CRT_SECURE_NO_WARNINGS

//#pragma warning(disable:4530)

// Change these values to use different versions
#define WINVER		0x0601
#define _WIN32_WINNT	0x0601
#define _WIN32_IE	0x0600

//#define _RICHEDIT_VER	0x0300
//#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#pragma warning(disable: 4786)

#include "afxdialogex.h"

// ----------------------------------------------------------------------------

#include "mmsystem.h"
#include <memory>
#include <atlimage.h>
#include <afxole.h>
#include <memory>
#include <utility>
#include <algorithm>
#include <cstdlib>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <cmath>
#include <limits>
#include <bitset>
#include <iostream>
#include "CBasicFileIO.h"

#include "angle_gl.h"
#include "EGL/egl.h"
//#include "GLES3/gl32.h"

//#include "GL/glew.h"
//#include "GLFW/glfw3.h"

// ----------------------------------------------------------------------------

// using these instead of min/max (macros) to avoid issues with including glm

#define MAXX(A,B) ((A)>=(B)?(A):(B))
#define MINN(A,B) ((A)<=(B)?(A):(B))

namespace BPT {

	#include "glm/glm.hpp"
	#include "glm/gtc/matrix_transform.hpp"

	typedef glm::vec3 vec2;
	typedef glm::vec3 vec3;
	typedef glm::vec4 vec4;
	typedef glm::mat2 mat2;
	typedef glm::mat3 mat3;
	typedef glm::mat4 mat4;

}

// ----------------------------------------------------------------------------

//#include "BPTLib\include\BPTLib.h"
#include "BPTLib/Pixels.h"
#include "BpaintStuff/BPTBitmap.h"

// ----------------------------------------------------------------------------

typedef BPT::PIXEL_A8R8G8B8 SDisplayPixelType;
//typedef BPT::PIXEL_A2R10G10B10 SDisplayPixelType; // windows doesn't like this (why not :?
//typedef BPT::PIXEL_A16R16G16B16 SDisplayPixelType; // compiles but display dibs don't like it :-/
//typedef BPT::PIXEL_R8G8B8 SDisplayPixelType; // currently broken in place dither blows up
//typedef BPT::PIXEL_R5G6B5 SDisplayPixelType; // currently broken in place dither blows up

#endif // !defined(AFX_STDAFX_H__0661A047_BA7A_4053_AFD8_5DA83D26620B__INCLUDED_)
