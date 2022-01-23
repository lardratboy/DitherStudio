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

#if !defined(MISCPOINTCLOUDSTUFF_H__4EE3AB65_FB91_4E19_9877_81FB6B6BDBFD__INCLUDED_)
#define MISCPOINTCLOUDSTUFF_H__4EE3AB65_FB91_4E19_9877_81FB6B6BDBFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <limits>
#include <utility>

namespace BPT {

	//
	//	TVec32RGBTransferOp
	//	transfer vec3 collection to rgb op
	//

	template<typename T, typename I>
	struct TVec32RGBTransferOp
	{
		I it, first, last;

		TVec32RGBTransferOp(I start, I end, I reset) : it(start), first(reset), last(end) {}

		void operator()(T& t)
		{
			auto v = *it;

			// [-1 to 1] to [0 to 1]
			v *= 0.5f;
			v += 0.5f;

			t = TVec32RGB<T>(v);
			if (++it == last) it = first;
		}
	};

	//
	//	TFindBoundingAABBOp
	//

	template<typename V, typename I>
	std::pair<V, V> TFindBoundingAABBOp(I first, I last)
	{
		V a = V(std::numeric_limits<float>::max());
		V b = V(std::numeric_limits<float>::min());

		while (last != first)
		{
			V t = *first++;

			a = glm::min(a, t);
			b = glm::max(b, t);

		}

		return std::make_pair(a, b);
	}

	//
	//	TNormalizeToAABBOp
	//

	template<typename V, typename I>
	void TNormalizeToAABBOp(I first, I last, std::pair<V, V> aabb)
	{
		V r = (aabb.second - aabb.first);
		float ooe = 1.0f / (MAXX(r.x, MAXX(r.y, r.z)));

		while (last != first)
		{
			V t = (*first) * ooe;

			*first++ = t;
		}

	}

	//
	// TAABBNormalizeOp
	//

	template<typename V, typename I>
	void TNormalizeToAABBOp(I first, I last)
	{
		auto aabb = TFindBoundingAABBOp<V, I>(first, last);

		TNormalizeToAABBOp<V, I>(first, last, aabb);
	}

	//
	//	TVec3CollectionsToDoubleWideSurface
	//

	template<typename SURFACE, typename A, typename B>
	SURFACE* TVec3CollectionsToDoubleWideSurface(A& a, B& b)
	{
		ASSERT(a.size() == b.size());

		SURFACE* pSurface = new SURFACE();

		if (!pSurface) return 0;

		int sqrSize = (int)(std::floor(std::sqrtf((float)a.size()) + 0.5));

		SIZE opSize = { sqrSize, sqrSize };

		if (!pSurface->Create(sqrSize * 2, sqrSize))
		{
			delete pSurface;
			return 0;
		}

		RECT opRect = { 0, 0, opSize.cx, opSize.cy };
		TVec32RGBTransferOp<typename SURFACE::pixel_type, typename A::iterator> transferAOp(a.begin(), a.end(), a.begin());
		TBitmapOperation(pSurface, opRect, transferAOp);

		OffsetRect(&opRect, opSize.cx, 0);
		TVec32RGBTransferOp<typename SURFACE::pixel_type, typename B::iterator> transferBOp(b.begin(), b.end(), b.begin());
		TBitmapOperation(pSurface, opRect, transferBOp);

		return pSurface;
	}

	//
	//	TXYZLoadPointCloudInto
	//

	template<typename V, typename C>
	bool TXYZLoadPointCloudInto(const char* pszFilename, V& vertices, C& colors)
	{
		FILE* inFile = fopen(pszFilename, "rt");

		if (NULL == inFile) return false;

		char line[1024];

		for (int lines = 0; NULL != fgets(line, (sizeof(line) / sizeof(line[0])) - 1, inFile); lines++) {

			float x, y, z, xr, yg, zb;

			if (6 == sscanf(line, "%f %f %f %f %f %f", &x, &y, &z, &xr, &yg, &zb)) {

				vertices.push_back(V::value_type(x, y, z));
				colors.push_back(C::value_type(xr, yg, zb));

			}

		}

		fclose(inFile);

		return true;
	}
} // namespace BPT

#endif // MISCPOINTCLOUDSTUFF_H__4EE3AB65_FB91_4E19_9877_81FB6B6BDBFD__INCLUDED_


