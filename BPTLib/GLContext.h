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

#if !defined(BPT_GLCONTEXT_H_DECC0574_30B3_48da_BCD7_133CC728698B_INCLUDED_)
#define BPT_GLCONTEXT_H_DECC0574_30B3_48da_BCD7_133CC728698B_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// (DEPENDENT-INCLUDES) -------------------------------------------------------

// ----------------------------------------------------------------------------

namespace BPT {

	// ------------------------------------------------------------------------

	//
	//	GLContext
	//

	class GLContext {

	protected:

		EGLDisplay m_eglDisplay;
		EGLint m_verMajor;
		EGLint m_verMinor;
		EGLConfig m_windowConfig;
		EGLSurface m_eglSurface;
		EGLContext m_eglContext;

		HDC		m_hDC;
		HWND	m_hWnd;

#if 0
		HGLRC	m_hRC;
#endif

		void clear_handles() {

			m_hWnd = 0;
			m_hDC = 0;

#if 1
			m_eglDisplay = 0;
			m_windowConfig = 0;
			m_eglSurface = 0;
			m_eglContext = 0;
#endif

#if 0
			m_hRC = 0;
#endif

		}

	public:

		// --------------------------------------------------------------------

		GLContext() {

			clear_handles();

		}

		~GLContext() {

			Release();

		}

		// --------------------------------------------------------------------

		bool Attach( HWND hWnd, const int colorBits = 32, const int depthBits = 24 )
		{

#if 1

			Release();

			// Get the device context for the passed window
			// ----------------------------------------------------------------

			m_hDC = GetDC(hWnd);

			if (!m_hDC) {

				return false;

			}

			m_hWnd = hWnd;

			// ----------------------------------------------------------------

			m_eglDisplay = eglGetDisplay(m_hDC);
			eglInitialize(m_eglDisplay, &m_verMajor, &m_verMinor);
			eglBindAPI(EGL_OPENGL_ES_API);

			EGLint configAttributes[] =
			{
				EGL_BUFFER_SIZE, 0,
				EGL_RED_SIZE, 8,
				EGL_GREEN_SIZE, 8,
				EGL_BLUE_SIZE, 8,
				EGL_ALPHA_SIZE, 0,
				EGL_COLOR_BUFFER_TYPE, EGL_RGB_BUFFER,
				EGL_DEPTH_SIZE, 24,
				EGL_LEVEL, 0,
				EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
				EGL_SAMPLE_BUFFERS, 0,
				EGL_SAMPLES, 0,
				EGL_STENCIL_SIZE, 0,
				EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
				EGL_TRANSPARENT_TYPE, EGL_NONE,
				EGL_TRANSPARENT_RED_VALUE, EGL_DONT_CARE,
				EGL_TRANSPARENT_GREEN_VALUE, EGL_DONT_CARE,
				EGL_TRANSPARENT_BLUE_VALUE, EGL_DONT_CARE,
				EGL_CONFIG_CAVEAT, EGL_DONT_CARE,
				EGL_CONFIG_ID, EGL_DONT_CARE,
				EGL_MAX_SWAP_INTERVAL, EGL_DONT_CARE,
				EGL_MIN_SWAP_INTERVAL, EGL_DONT_CARE,
				EGL_NATIVE_RENDERABLE, EGL_DONT_CARE,
				EGL_NATIVE_VISUAL_TYPE, EGL_DONT_CARE,
				EGL_NONE
			};

			EGLint numConfigs;
			eglChooseConfig(m_eglDisplay, configAttributes, &m_windowConfig, 1, &numConfigs);

			EGLint surfaceAttributes[] = { EGL_NONE };
			m_eglSurface = eglCreateWindowSurface(m_eglDisplay, m_windowConfig, m_hWnd, surfaceAttributes);

			EGLint contextAttributes[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
			m_eglContext = eglCreateContext(m_eglDisplay, m_windowConfig, NULL, contextAttributes);

			return MakeCurrent();

#else

			Release();

			// Get the device context for the passed window
			// ----------------------------------------------------------------

			m_hDC = GetDC( hWnd );

			if ( !m_hDC ) {

				return false;

			}

			m_hWnd = hWnd;

			// set the pixel format for the DC
			// ----------------------------------------------------------------

			PIXELFORMATDESCRIPTOR pfd;
			ZeroMemory( &pfd, sizeof( pfd ) );

			pfd.nSize = sizeof( pfd );
			pfd.nVersion = 1;
			pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
			pfd.iPixelType = PFD_TYPE_RGBA;
			pfd.cColorBits = colorBits;
			pfd.cDepthBits = depthBits;
			pfd.iLayerType = PFD_MAIN_PLANE;

			// match our request to a pixel format
			// ----------------------------------------------------------------

			int format = ChoosePixelFormat( m_hDC, &pfd );

			if ( !SetPixelFormat( m_hDC, format, &pfd ) ) {

				Release();
		
				return false;

			}

			// create the OpenGL render context
			// ----------------------------------------------------------------

			m_hRC = wglCreateContext( m_hDC );

			if ( !m_hRC ) {

				Release();

				return false;

			}

			// make it the current render context

			if ( !MakeCurrent() ) {

				Release();

				return false;

			}

			GLenum err = glewInit();

			// TODO setup a context we can use shaders with...

			return true;

#endif

		}

		void Release() {

#if 1

			if (m_hWnd && m_hDC) {

				ReleaseDC(m_hWnd, m_hDC);

			}

#else

			wglMakeCurrent( m_hDC, 0 );

			if ( m_hRC ) {

				wglDeleteContext( m_hRC );

			}

			if ( m_hWnd && m_hDC ) {

				ReleaseDC( m_hWnd, m_hDC );

			}

#endif

			clear_handles();

		}

		// --------------------------------------------------------------------

#if 0
		bool IsValid() const {

			return (0 != m_hRC);

		}
#endif

		bool MakeCurrent() {

#if 1

			return 0 != eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext);

#else

			return (FALSE != wglMakeCurrent( m_hDC, m_hRC ));

#endif

		}

		void SwapBuffers() {

#if 1
			eglSwapBuffers(m_eglDisplay, m_eglSurface);

#else

			::SwapBuffers( m_hDC );

#endif

		}

		// --------------------------------------------------------------------

	};

}; // namespace BPT

// ----------------------------------------------------------------------------

#endif // !defined(BPT_GLCONTEXT_H_DECC0574_30B3_48da_BCD7_133CC728698B_INCLUDED_)
