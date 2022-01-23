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

#if !defined(VARIOUSOPERATIONSTHATNEEDAHOME_H__4EE3AB65_FB91_4E19_9877_81FB6B6BDBFD__INCLUDED_)
#define VARIOUSOPERATIONSTHATNEEDAHOME_H__4EE3AB65_FB91_4E19_9877_81FB6B6BDBFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BPTLib/Pixels.h"
#include "BPTBlitter.h"
#include "BPTPrimitives.h"
#include "TScaleUpBlit.h"
#include "TiledBitmapHelpers.h"
#include "TStereographicOperations.h"
#include "TPixelTree.h"
#include "TQuantizedHistogram.h"
#include "TFibonacciSphereOperations.h"
#include "TWavefrontOBJOperations.h"
#include "MiscPointCloudStuff.h"

#include "BPTLib/TLerp.h"
#include "BPTLib/TFVF.h"
#include "BPTLib/TFVFFilters.h"
#include "BPTLib/TTriangle.h"
#include "BPTLib/TQuad.h"
#include "TBoxOfBoxes.h"

//#include "BPTLIB/TPSDFileCode.h" // BPTLib isn't fully compatible and has namespace collisions to boot

namespace BPT {

	// ------------------------------------------------------------------------

	//
	//	TTokenize
	//

	template<
		typename T
		, typename I1
		, typename I2
	> int
	TTokenize(
		T& collection
		, I1 it
		, I1 end
		, I2 whitespace_begin
		, I2 whitespace_end
	)
	{
		// --------------------------------------------------------------------

		I1 start;

		// --------------------------------------------------------------------

		int tokens = 0;

		while (it != end) {

			// find next non-seperator

			for (; it != end; ++it) {

				if (std::find(whitespace_begin, whitespace_end, *it) == whitespace_end) break;

			}

			// find last non-seperator

			for (start = it; it != end; ++it) {

				if (std::find(whitespace_begin, whitespace_end, *it) != whitespace_end) break;

			}

			if (start != it) {

				collection.push_back(typename T::value_type(start, it));

				++tokens;

			}

		}

		return tokens;
	}

	// ------------------------------------------------------------------------

	//
	//	TTokenizeViaExtractionIntoCollection
	//

	template< typename T, typename C, typename S > int
	TTokenizeViaExtractionIntoCollection(
		C& collection
		, const S& str
		, const S& whitespace
	)
	{
		std::list<S> tokens;

		int tokenCount = TTokenize(tokens, str.begin(), str.end(), whitespace.begin(), whitespace.end());

		std::list<S>::iterator it = tokens.begin();
		std::list<S>::iterator end = tokens.end();

		for (; end != it; ++it) {

			std::istringstream is(*it);

			T type;

			is >> type;

			collection.push_back(type);

		}

		return tokenCount;
	}

	// --------------------------------------------------------------------------------------------

	template< class SURFACE > SURFACE*
	THELPER_PSDLoad(const char* filename)
	{
		typedef TPSDFileLoader<SURFACE> LOADER;

		LOADER PSDLoader;

		LOADER::loaded_data results = PSDLoader.LoadFromFile(filename, false);

		if (results.first) {

			return results.first;

		}

		return static_cast<SURFACE*>(0);

	}

	// --------------------------------------------------------------------------------------------

	//
	//	TLoadDataAsSurface
	//	-- this itself should be a primitive that takes a input iterator stream...
	//  -- need to convert this into a different sort of layout
	//

	template<typename T>
	T* TLoadDataAsSurface(const char* filename)
	{
		CBasicFileIO fd;

		//TRACE("SIZE of temp data pixel %d\n", sizeof(typename T::pixel_type));

		if (!fd.Open(filename, "rb")) {
			return 0;
		}

		size_t fileSize = CBasicFileIO::GetFileSize(filename);

		size_t sq = (size_t)std::floor(std::sqrt(fileSize) + 0.5) + 1; // just make sure we have more than enough space (wasteful and kind of silly)

		size_t dataSize = sq * (sq * sizeof(typename T::pixel_type));

		BYTE* pData = new BYTE[dataSize];

		ASSERT(nullptr != pData);

		fd.LoadDataInto(pData, fileSize);

		T* pBitmap = new T;

		ASSERT(nullptr != pBitmap);

		SIZE dim = { (int)sq, (int)sq };

		pBitmap->Map(pData, dim, sq, true, false);

		fd.Close();

		return pBitmap;
	}

	// --------------------------------------------------------------------------------------------

	template<typename T>
	struct TBiasedDifferenceOp {
		T base;
		TBiasedDifferenceOp(T b) : base(b) {}
		void operator()(T& t) {
			t.R(t.R() - base.R() + typename T::R_LimitedMax / 2);
			t.G(t.G() - base.G() + typename T::G_LimitedMax / 2);
			t.B(t.B() - base.B() + typename T::B_LimitedMax / 2);
		}
	};

	template<typename T, typename M = int >
	struct TSumRGBOp {
		M r, g, b;
		M counted;
		TSumRGBOp() : r(0), g(0), b(0), counted(0) {}
		void operator()(T& t) {
			r += t.R();
			g += t.G();
			b += t.B();
			++counted;
		}
	};

	template<typename SURF>
	typename SURF::pixel_type TFindBitmapAverageColor(SURF& surface)
	{
		TSumRGBOp<typename SURF::pixel_type> sum;
		TBitmapOperation(surface, surface.Rect(), sum);
		ASSERT(0 != sum.counted);
		return typename SURF::pixel_type(sum.r / sum.counted, sum.g / sum.counted, sum.b / sum.counted);
	}

	template<typename SURF>
	void TBiasedDifferenceFrom(SURF& surface, typename SURF::pixel_type base)
	{
		TBiasedDifferenceOp<typename SURF::pixel_type> diff(base);
		TBitmapOperation(surface, surface.Rect(), diff);
	}

	template<typename T>
	struct TBiasedDifferenceTileFromAverageOp {
		void operator()(T& tile) {
			TBiasedDifferenceFrom(tile, TFindBitmapAverageColor(tile));
		}
	};

	template<typename T>
	struct TBiasedDifferenceTileFromCenterOp {
		void operator()(T& tile) {
			auto center = tile.Iterator(tile.Width() / 2, tile.Height() / 2);
			TBiasedDifferenceFrom(tile, *center);
		}
	};

	// ------------------------------------

	template<typename T>
	struct TNotGrayscaleOp {
		void operator()(T& t) {
			t = t.not_GRAYSCALE();
		}
	};

	template<typename T>
	struct TDesaturateOp {
		void operator()(T& t) {
			t = t.DESATURATE();
		}
	};

	// --------------------------------------------------------------------------------------------

	//
	//	TSequenceWithModuloOp
	//

	template<
		typename T,
		typename M = int,
		const int moduloPow2 = 24,
		const int moduloAndMask = (1 << moduloPow2) - 1
	>
	struct TSequenceWithModuloOp {

		M nextValue;
		M stepValue;

		TSequenceWithModuloOp(M start = M(0), M step = M(1)) : nextValue(start), stepValue(step) {}

		M next()
		{
			M r = nextValue;
			nextValue = (nextValue + stepValue) & moduloAndMask;
			return r;
		}

		void operator()(T& t) {
			t.value = next();
		}

	};

	// --------------------------------------------------------------------------------------------

	//
	//	TPerformOpForTileOp
	//

	template<typename T, typename FN>
	struct TPerformOpForTileOp {
		FN& fn;
		TPerformOpForTileOp(FN& f) :fn(f) {}
		void operator()(T& tile) {
			BPT::TBitmapOperation(tile, fn);
		}
	};

	// --------------------------------------------------------------------------------------------

	//
	//	TScanlineSubtractRGBChannelsLeftToRight
	//

	template< typename T >
	struct TScanlineSubtractRGBChannelsLeftToRight {

		template<typename I>
		__forceinline void operator()(I first, I last)
		{
			if (last == first) return /* NOP */;

			typename T::pixel_type prev = *first++;

			for (; last != first; ++first)
			{
				first->r = (first->r - prev.r) + (typename T::pixel_type::R_LimitedMax / 2);
				first->g = (first->g - prev.g) + (typename T::pixel_type::G_LimitedMax / 2);
				first->b = (first->b - prev.b) + (typename T::pixel_type::B_LimitedMax / 2);
			}
		}
	};

	// ---------------------------------------------------------------------------

	template<typename T> glm::vec3 TRGB2Vec3(T rgb) {
		return glm::vec3(
			(float)rgb.r / (float)(typename T::R_LimitedMax),
			(float)rgb.g / (float)(typename T::G_LimitedMax),
			(float)rgb.b / (float)(typename T::B_LimitedMax)
		);
	}

	template<typename T> T TVec32RGB(glm::vec3 v) {
		return T(
			(T::value_type)(v.x * (float)(typename T::R_LimitedMax)),
			(T::value_type)(v.y * (float)(typename T::G_LimitedMax)),
			(T::value_type)(v.z * (float)(typename T::B_LimitedMax))
		);
	}

	template<typename T, typename A> glm::vec4 TRGB2Vec4(T rgb, A a) {
		return glm::vec4(
			(float)rgb.r / (float)(typename T::R_LimitedMax),
			(float)rgb.g / (float)(typename T::G_LimitedMax),
			(float)rgb.b / (float)(typename T::B_LimitedMax),
			(float)a
		);
	}

	template<typename T> T TVec42RGB(glm::vec4 v) {
		return T(
			(T::value_type)(v.x * (float)(typename T::R_LimitedMax)),
			(T::value_type)(v.y * (float)(typename T::G_LimitedMax)),
			(T::value_type)(v.z * (float)(typename T::B_LimitedMax))
		);
	}

	// ---------------------------------------------------------------------------

	template<typename T>
	struct TInvertSrcTransferOp {
		void operator()(T& d, const T& s) const {
			d = s.invert();
		}
	};

	template<typename T>
	struct TBiasedDifferenceTransferOp {
		void operator()(T& d, const T& s) const {
			d.r = (d.r - s.r) + typename T::R_LimitedMax / 2;
			d.g = (d.g - s.g) + typename T::G_LimitedMax / 2;
			d.b = (d.b - s.b) + typename T::B_LimitedMax / 2;
		}
	};

	// ---------------------------------------------------------------------------

	//
	//	TMakeAnInverseAndDifferenceTripleWide
	//

	template<typename SURFACE>
	SURFACE* TMakeAnInverseAndDifferenceTripleWide(SURFACE* pSourceSurface)
	{
		SURFACE* pSurface = new SURFACE();
		if (!pSurface) return 0;

		SIZE opSize = pSourceSurface->Size();

		if (!pSurface->Create(opSize.cx * 3, opSize.cy))
		{
			delete pSurface;
			return 0;
		}

		// left original copy
		// --------------------------------------------------------------------------

		typedef typename SURFACE::pixel_type p_t;

		TCopyROP<p_t> copyOp;
		T_Blit(pSurface, opSize.cx * 0, 0, pSourceSurface, copyOp);

		// color inverse
		// --------------------------------------------------------------------------

		TInvertSrcTransferOp<p_t> iop;
		TFunctorAdaptorTransferOp<TInvertSrcTransferOp<p_t>, p_t> invertOp(iop);
		T_Blit(pSurface, opSize.cx * 1, 0, pSourceSurface, invertOp);

		// difference between inverse and original
		// --------------------------------------------------------------------------

		T_Blit(pSurface, opSize.cx * 2, 0, pSourceSurface, invertOp);

		TBiasedDifferenceTransferOp<p_t> dop;
		TFunctorAdaptorTransferOp< TBiasedDifferenceTransferOp<p_t>, p_t> differenceOp(dop);
		T_Blit(pSurface, opSize.cx * 2, 0, pSourceSurface, differenceOp);

		return pSurface;
	}

	// --------------------------------------------------------------------------------------------

	//
	//	TSortVec3Predicate
	//

	struct TSortVec3Predicate {
		__forceinline double addr(glm::vec3& v) const {

#if 1
			// need to visualize the sort 
			float len = glm::length(v); // glm::dot(v, v);
			glm::vec3 n = v / len;// glm::normalize(v);
			glm::vec2 uv = s2cB(n);
			//uv = uv * 0.5f + 0.5f;
			glm::vec3 o = glm::vec3(uv, len); // -glm::dot(glm::vec2(0.0f), uv));
			return (o.x * 256.0f + o.y * 512.0f + o.z * 1024.0f); // glm::dot(o, o) / dot(o, glm::vec3(512.0f, 1.0f, 1024.0f)); // *(glm::fract(o * o * o));
#else
			glm::vec3 offset = v;
			return ( glm::dot(offset, offset) );// glm::vec3(512.0f, 1.0f, 1024.0f));
#endif

		}
		__forceinline bool operator()(glm::vec3& a, glm::vec3& b) const {
			return addr(a) < addr(b);
		}

	};

} // namespace BPT

#endif // VARIOUSOPERATIONSTHATNEEDAHOME_H__4EE3AB65_FB91_4E19_9877_81FB6B6BDBFD__INCLUDED_

