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

#if !defined(TSTEREOGRAPHICOPERATIONS_H__4EE3AB65_FB91_4E19_9877_81FB6B6BDBFD__INCLUDED_)
#define TSTEREOGRAPHICOPERATIONS_H__4EE3AB65_FB91_4E19_9877_81FB6B6BDBFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BPTBlitter.h"
#include "BPTPrimitives.h"

namespace BPT {

	//#include "glm/glm.hpp"

	// https://en.wikipedia.org/wiki/Stereographic_projection


#if 1

	template<typename C = glm::vec2, typename S = glm::vec3>
	__forceinline glm::vec3 c2sA(C p)
	{
		C p2 = p * p;
		float md = (1.0f + p2.x + p2.y);
		float omd = 1.0f / md;
		return S(p.x * omd, p.y * omd, (-1.0 + p2.x + p2.y) / (2.0 * md));
	}

	template<typename C = glm::vec2, typename S = glm::vec3>
	__forceinline C s2cA(S p)
	{
		float oow = 1.0f / (0.5f - p.z);
		return glm::vec2(p.x, p.y) * oow;
	}

	template<typename C = glm::vec2, typename S = glm::vec3>
	__forceinline S c2sB(C p)
	{
		C p2 = p * p;
		float omd = 1.0f / (1.0f + p2.x + p2.y);
		return S(2.0f * p.x, 2.0f * p.y, (-1.0f + p2.x + p2.y)) * omd;
	}

	template<typename C = glm::vec2, typename S = glm::vec3>
	__forceinline C s2cB(S p)
	{
		float oow = 1.0f / (1.0f - p.z);
		return C(p.x * oow, p.y * oow);
	}

#else

	template<typename C = glm::vec2,typename S = glm::vec3>
	__forceinline glm::vec3 c2sA(glm::vec2 p)
	{
		glm::vec2 p2 = p * p;
		float md = (1.0f + p2.x + p2.y);
		float omd = 1.0f / md;
		return glm::vec3(p.x * omd, p.y * omd, (-1.0 + p2.x + p2.y) / (2.0 * md));
	}

	__forceinline glm::vec2 s2cA(glm::vec3 p)
	{
		float oow = 1.0f / (0.5f - p.z);
		return glm::vec2(p.x, p.y) * oow;
	}

	__forceinline glm::vec3 c2sB(glm::vec2 p)
	{
		glm::vec2 p2 = p * p;
		float omd = 1.0f / (1.0f + p2.x + p2.y);
		return glm::vec3(2.0f * p.x, 2.0f * p.y, (-1.0f + p2.x + p2.y)) * omd;
	}

	__forceinline glm::vec2 s2cB(glm::vec3 p)
	{
		float oow = 1.0f / (1.0f - p.z);
		return glm::vec2(p.x * oow, p.y * oow);
	}

#endif

	// -------------------------------------

	template<typename T>
	struct Ts2cB_PixelOP {
	__forceinline void operator()(T& t) const {

			int w = t.desaturate();

			glm::vec3 s(t.R(), t.G(), t.B());
			s /= 255.0f;
			s = normalize(s);

			glm::vec2 c = s2cB(s);

			c *= 255.0f;
			c += w;

			int y = w;
			int x = int(c.x);
			int z = int(c.y);

			t.R(x);
			t.G(y);
			t.B(z);

		}
	};

	template<typename T>
	struct Tc2sB_PixelOP {
	__forceinline void operator()(T& t) const {

			int w = t.G();

			glm::vec2 c(t.R(), t.B());

			c -= w;
			c /= 255.0;
			c = normalize(c);

			glm::vec3 s = c2sB(c);

			s *= 255.0;
			s += w;

			int x = int(s.x);
			int y = int(s.y);
			int z = int(s.z);

			t.R(x);
			t.G(y);
			t.B(z);

		}
	};

	// --------------------------------

	template<typename SURF>
	void Ts2cB_Bitmap(SURF& surface)
	{
		Ts2cB_PixelOP<typename SURF::pixel_type> op;
		TBitmapOperation(surface, surface.Rect(), op);
	}

	template<typename SURF>
	void Tc2sB_Bitmap(SURF& surface)
	{
		Tc2sB_PixelOP<typename SURF::pixel_type> op;
		TBitmapOperation(surface, surface.Rect(), op);
	}

	// ---

	template<typename S2C, typename C2S>
	struct TS2CC2S_EncodeDecodePairOPS {

		// TODO combine the above functions and use the two template arguments passed to make it happen

	};

} // namespace BPT

#endif // TSTEREOGRAPHICOPERATIONS_H__4EE3AB65_FB91_4E19_9877_81FB6B6BDBFD__INCLUDED_

