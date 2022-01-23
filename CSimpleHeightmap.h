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

#if 0
#if !defined(CSIMPLEHEIGHTMAP_H__4EE3AB65_FB91_4E19_9877_81FB6B6BDBFD__INCLUDED_)
#define CSIMPLEHEIGHTMAP_H__4EE3AB65_FB91_4E19_9877_81FB6B6BDBFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BPTLib/Pixels.h"
#include "BPTLib/GLContext.h"
#include "TFace3Vector3.h"

namespace BPT {

	// ----------------------------------------------------------------------------

	//
	//	TGLLazyHackFace3RendererForIterator()
	//	
	//

	template< class ITERATOR > void
		TGLLazyHackFace3RendererForIterator(ITERATOR s_it, ITERATOR s_end)
	{
		ITERATOR it = s_it;
		ITERATOR end = s_end;

		glBegin(GL_TRIANGLES);

		while (it != end) {

			// -------------------------

			glColor3f(it->p1._nx, it->p1._ny, it->p1._nz);
			glVertex3f(it->p1.x(), it->p1.y(), it->p1.z());
			glNormal3f(it->p1._nx, it->p1._ny, it->p1._nz);
			glTexCoord2f(it->p1.u(), it->p1.v());

			// -------------------------

			glColor3f(it->p2._nx, it->p2._ny, it->p2._nz);
			glVertex3f(it->p2.x(), it->p2.y(), it->p2.z());
			glNormal3f(it->p2._nx, it->p2._ny, it->p2._nz);
			glTexCoord2f(it->p2.u(), it->p2.v());

			// -------------------------

			glColor3f(it->p3._nx, it->p3._ny, it->p3._nz);
			glVertex3f(it->p3.x(), it->p3.y(), it->p3.z());
			glNormal3f(it->p3._nx, it->p3._ny, it->p3._nz);
			glTexCoord2f(it->p3.u(), it->p3.v());

			++it;

		}

		glEnd();
	}

	// 
	//
	//	RenderFaces()
	//

	void RenderFaces(const BPT::FACE3* pFaces, const int nFaces)
	{
		TGLLazyHackFace3RendererForIterator(pFaces, pFaces + nFaces);
	}

	// ----------------------------------------------------------------------------
	// 
	//
	//	CSimpleHeightmap
	//

	class CSimpleHeightmap {

	protected:

		FACE3* m_pFaces;
		int m_nFaces;
		GLuint m_glDisplayList;

	protected:

		template< typename T > typename T::value_type GetHeight(T& value) {

			return value. TChannel<T>::B(value) - (TChannel<T>::BMax / 2);

		}

	public:

		CSimpleHeightmap() : m_pFaces(0), m_nFaces(0), m_glDisplayList(0) {}

		~CSimpleHeightmap() {

			Release();

		}

		void Release() {

			//			DestroyDisplayList();
			if (m_glDisplayList) {

				glDeleteLists(m_glDisplayList, 1); // BAD BAD HACK !!!!

			}


			if (m_pFaces) {

				delete[] m_pFaces;

				m_pFaces = 0;

			}

			m_nFaces = 0;

		}

		template<typename SURFACE>
		bool CreateFromSurface( SURFACE & surface, float xScale = 1.0f, float yScale = 1.0f, float zScale = 1.0f)
		{
			Release();

			SIZE surfaceSize = surface.Size();

			m_nFaces = surfaceSize.cx * surfaceSize.cy * 2;

			m_pFaces = new FACE3[m_nFaces];

			if (!m_pFaces) {

				Release();

				return false;

			}

			// process the faces :)

			float du = 1.0f / (surfaceSize.cx - 1);
			float dv = 1.0f / (surfaceSize.cy - 1);
			float oo255 = yScale / 255.0f;

			float v0 = 0.0f;
			float v1 = dv;

			FACE3* pFace = m_pFaces;

			float uAdjust = 0.5f;
			float vAdjust = 0.5f;

			for (int y = 0; y < (surfaceSize.cy - 1); y++) {

				typename surface_type::pixel_iterator itY0 = surface.Iterator(0, y + 0);
				typename surface_type::pixel_iterator itY1 = surface.Iterator(0, y + 1);

				float u0 = 0.0f;
				float u1 = du;

				for (int x = 0; x < (surfaceSize.cx - 1); x++) {

					float a = (float)(signed)GetHeight(*itY0++) * oo255;
					float b = (float)(signed)GetHeight(*itY0) * oo255;
					float c = (float)(signed)GetHeight(*itY1++) * oo255;
					float d = (float)(signed)GetHeight(*itY1) * oo255;

					pFace->p3 = VECTOR3((u0 - uAdjust) * xScale, c, (v1 - vAdjust) * zScale, u0, v1);
					pFace->p2 = VECTOR3((u0 - uAdjust) * xScale, a, (v0 - vAdjust) * zScale, u0, v0);
					pFace->p1 = VECTOR3((u1 - uAdjust) * xScale, b, (v0 - vAdjust) * zScale, u1, v0);

					++pFace;

					pFace->p3 = VECTOR3((u0 - uAdjust) * xScale, c, (v1 - vAdjust) * zScale, u0, v1);
					pFace->p2 = VECTOR3((u1 - uAdjust) * xScale, b, (v0 - vAdjust) * zScale, u1, v0);
					pFace->p1 = VECTOR3((u1 - uAdjust) * xScale, d, (v1 - vAdjust) * zScale, u1, v1);

					++pFace;

					u0 += du;
					u1 += du;

				}

				v0 += dv;
				v1 += dv;

			}

			return true;

		}

		const FACE3* GetFaces() const {

			return m_pFaces;

		}

		int FaceCount() const {

			return m_nFaces;

		}

		// --------------------------------------------------------------------

		template< class GLWRAPPER > void Draw(
			GLWRAPPER& gl
			, const GLfloat rxScale
			, const GLfloat gyScale
			, const GLfloat bzScale
		) {

			if (m_pFaces) {

				RenderFaces(gl, m_pFaces, m_nFaces, rxScale, gyScale, bzScale);

			}

		}

		template< class GLWRAPPER > void DestroyDisplayList(GLWRAPPER& gl) {

			if (m_glDisplayList) {

				gl.glDeleteLists(m_glDisplayList, 1);

				m_glDisplayList = 0;

			}

		}

		template< class GLWRAPPER > bool BuildDisplayList(
			GLWRAPPER& gl
			, const GLfloat rxScale
			, const GLfloat gyScale
			, const GLfloat bzScale
		) {

			DestroyDisplayList(gl);

			m_glDisplayList = gl.glGenLists(1);

			if (0 == m_glDisplayList) {

				return false;

			}

			gl.glNewList(m_glDisplayList, GL_COMPILE);

			Draw(gl, rxScale, gyScale, bzScale);

			gl.glEndList();

			return true;

		}

		template< class GLWRAPPER > void DrawViaDisplayList(
			GLWRAPPER& gl
			, const GLfloat rxScale
			, const GLfloat gyScale
			, const GLfloat bzScale
		) {

			if (!m_glDisplayList) {

				if (!BuildDisplayList(gl, rxScale, gyScale, bzScale)) {

					Draw(gl, rxScale, gyScale, bzScale);

					return;

				}

			}

			gl.glCallList(m_glDisplayList);

		}

	};

} // namespace BPT

#endif // CSIMPLEHEIGHTMAP_H__4EE3AB65_FB91_4E19_9877_81FB6B6BDBFD__INCLUDED_
#endif

