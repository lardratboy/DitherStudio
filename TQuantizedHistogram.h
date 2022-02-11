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

#if !defined(TQUANTIZEDHISTOGRAM_H__4EE3AB65_FB91_4E19_9877_81FB6B6BDBFD__INCLUDED_)
#define TQUANTIZEDHISTOGRAM_H__4EE3AB65_FB91_4E19_9877_81FB6B6BDBFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <set>
#include <unordered_map>
#include <map>
#include <cmath>
#include <deque>
#include <list>
#include <vector>
#include <iterator>
#include <algorithm>

#include "BPTSRL.h"
#include "BPTBlitter.h"
#include "BPTPrimitives.h"
#include "TFibonacciSphereOperations.h"
#include "TStereographicOperations.h"

namespace BPT {

	// ------------------------------------------------------------------------

	//#include "glm/glm.hpp"

	// ------------------------------------------------------------------------

	// THIS CODE MAKES ALL SORTS OF ASSUMPTIONS ABOUT THE PIXEL PACKING OF THE TYPES
	// THIS REALLY NEEDS TO BE ABSTRACTED FURTHER!!!!!!
	// NOT TO MENTION THE NUMBER OF TEMPORARY SURFACES IS EXCESSIVE

	//
	//	TQuantizedHistogram
	//	Optionally can write back to the source
	//

	template<typename TARGET,typename SOURCE>
	struct TQuantizedHistogram
	{
		enum Options : unsigned {
			DO_NOT_WRITE_TO_SOURCE				= 0x0000
		,	WRITE_CHROMAKEY_FOR_DUPLICATES		= 0x0001
		,	STORE_AS_INDICES					= 0x0002
		,	WRITE_QUANTIZED_VALUES				= 0x0004
		};

		Options options;

		typedef std::uint32_t storage_type;

		struct node_t {
			storage_type index;
			storage_type count;
			SOURCE * address;
		};

		TSimpleBitmap<node_t> histogram;

		storage_type nextUniqueIndex;

		node_t* pWriteTable;

		typedef std::map<SOURCE, int> neighbors_t;
		typedef std::unordered_map< storage_type, neighbors_t > similar_neighborhood_map_t;

		similar_neighborhood_map_t neighborhood;

		SOURCE chromakey;

		TQuantizedHistogram(Options o) : options(o), nextUniqueIndex(0), chromakey(255, 0, 255)
		{
			ASSERT(TARGET::ActiveBits <= 24); // 24 bit sanity check (until 64 bit)

			storage_type total = 1 << TARGET::ActiveBits;

			neighborhood.reserve(total);

			ASSERT((TARGET::R_BitMask | TARGET::G_BitMask | TARGET::B_BitMask) <= total);

			storage_type square = (storage_type)(std::ceil(std::sqrt(total)+0.5));

			histogram.Create(square, square);

			pWriteTable = reinterpret_cast<node_t*>(histogram.GetDataPtr());

			ASSERT(nullptr != pWriteTable);

			node_t clearNode = { 0, 0, 0 };

			histogram.ClearBuffer( clearNode );
		}

		void quantizeInto(SOURCE& s) const
		{
			// this could be done much faster and probably more accurately too while I'm at it
			// also why isn't this part of the pixel classes seems stupid not to have it there

			TARGET as = s;
			int ar = as.r;
			int ag = as.g;
			int ab = as.b;

			int r = ((ar * SOURCE::R_LimitedMax) + TARGET::R_LimitedMax / 2) / TARGET::R_LimitedMax;
			int g = ((ag * SOURCE::G_LimitedMax) + TARGET::G_LimitedMax / 2) / TARGET::G_LimitedMax;
			int b = ((ab * SOURCE::B_LimitedMax) + TARGET::B_LimitedMax / 2) / TARGET::B_LimitedMax;

			s.r = r;
			s.g = g;
			s.b = b;
		}

		storage_type pixel_to_index(SOURCE&s) const
		{
			TARGET t = s; // gross but it works probably need to optimize paths between pixel types

			return t.value & (TARGET::R_BitMask| TARGET::G_BitMask| TARGET::B_BitMask); // THIS ASSUMES THEY ARE ALL PACKED TOGETHER INTO THE LOWER BITS....
		}

		void operator()(SOURCE& s)
		{
			storage_type index = pixel_to_index(s);

			node_t * p = pWriteTable + index;

			p->count++;

			if (1 == p->count)
			{
				p->index = ++nextUniqueIndex;
				p->address = &s;
			}
			else
			{
				if (Options::WRITE_CHROMAKEY_FOR_DUPLICATES & options) {

					s = chromakey;

				}
			}

			if (Options::WRITE_QUANTIZED_VALUES & options) {

				quantizeInto(s);

			} else if (Options::STORE_AS_INDICES & options) {

				//s.value = (index * 255) / ((1 << TARGET::ActiveBits) - 1);// (index * ((1 << SOURCE::ActiveBits) - 1)) / ((1 << TARGET::ActiveBits) - 1);
				//s.value = (index * ((1 << SOURCE::ActiveBits) - 1)) / ((1 << TARGET::ActiveBits) - 1);
				s.value = p->index; // index;

			}

			// statistics on the actual colors as children of the quantized index

			neighborhood[p->index][s]++;
		}
	};

	//
	//	TFillQuantizedHistogramOp
	//

	template<typename T>
	struct TFillQuantizedHistogramOp
	{
		T& histogram;
		TFillQuantizedHistogramOp(T& h) : histogram(h) {}
		template<typename S> void operator()(S& s) {
			histogram(s);
		}
	};

	// ---------------------------------------------------------------------------

	//
	//	TQuantized2DPalettedSRLImageOperation
	// 
	//	-- the actual compression type should be another template argument
	//

	template<typename TARGET,typename SOURCE>
	struct TQuantized2DPalettedSRLImageOperation {

		typedef TARGET target_surface_t;
		typedef SOURCE source_surface_t;
		typedef typename SOURCE::pixel_type source_pixel_type;
		typedef typename TARGET::pixel_type target_pixel_type;
		typedef TQuantizedHistogram<target_pixel_type, source_pixel_type> histogram_t;

		//typedef TSRL2CompressedImage< TSRL2Compressor< source_pixel_type > > compressed_image_type;
		typedef TSRLCompressedImage< TSRLCompressor< source_pixel_type > > compressed_image_type;

		std::shared_ptr<source_surface_t> pModifiedSourceCopy;
		std::shared_ptr<compressed_image_type> pCompressedResult;
		histogram_t histogram;

		TQuantized2DPalettedSRLImageOperation( typename histogram_t::Options o)
			: pModifiedSourceCopy(nullptr), pCompressedResult(nullptr), histogram(o)
		{
		}

		bool Create(SOURCE& source, source_pixel_type chromakey, bool bKeepModifiedOriginal )
		{
			typename SOURCE::default_transfer_op top;

			// create a temporary copy (if the size of the packed bitmap was known here a this could be avoided)

			source_surface_t temp;
			temp.Copy(source);

			// build our histogram

			TFillQuantizedHistogramOp<histogram_t> histogramBuilder(histogram);

			TBitmapOperation(temp, histogramBuilder);

			TRACE("unique colors %d\n", histogram.nextUniqueIndex);

			// create our optimized palette based on histogram (might need to do by similarity first then count?)

			// TODO

			// create compressed image to isolate unique colors exposed by the histogram building process

			pCompressedResult = std::shared_ptr<compressed_image_type>(new compressed_image_type);

			TIsNotValue<source_pixel_type> predicate(histogram.chromakey);

			bool bResult = pCompressedResult->Create(temp, predicate);

			ASSERT(bResult);

			size_t compSize = pCompressedResult->CompressedSizeEstimate();
			size_t ucompSize = pCompressedResult->UncompressedSizeEstimate();

			TRACE("COMPRESSION TEST compressed size estimate %d/%d (%5.2f%%)\n", compSize, ucompSize, (float)compSize * 100.0 / (float)ucompSize);

			// create packed surface and blit it to the corner for lack of a better place

			source_surface_t packedSurface; // temporary bitmap

			pModifiedSourceCopy = std::shared_ptr<source_surface_t>(new source_surface_t);

			if (pCompressedResult->CreatePackedSurface(packedSurface))
			{
				if (bKeepModifiedOriginal)
				{
					bool bResult = pModifiedSourceCopy->Create(
						packedSurface.Width() + source.Width(),
						packedSurface.Height() + source.Height()
					);

					ASSERT(bResult);

					pModifiedSourceCopy->ClearBuffer(histogram.chromakey);

					// put palette in place

					T_Blit(*pModifiedSourceCopy, 0, 0, packedSurface, top);

					// now put the in place unique colors in place

					pCompressedResult->Blit(*pModifiedSourceCopy, packedSurface.Width(), packedSurface.Height(), top);
				}
				else
				{
					bool bResult = pModifiedSourceCopy->Create(
						packedSurface.Width(), packedSurface.Height()
					);

					ASSERT(bResult);

					T_Blit(*pModifiedSourceCopy, 0, 0, packedSurface, top);
				}

			}
			else
			{
				ASSERT(FALSE);
			}

			return true;

		}

	}; // TQuantized2DPalettedSRLImageOperation

	// ---------------------------------------------------------------------------

	//
	//	TFibonacciSphereVisualizationForHistogram
	//

	template<typename H> struct TFibonacciSphereVisualizationForHistogram {

		enum Options : unsigned {
			DEFAULT										= 0,
			PUSH_OUTSIDE_THE_UNIT_SPHERE_FOR_DECENDENTS = 0x00001,
			SORT_LATTICE_POINTS_BY_INDEX				= 0x00002,
			SORT_COLORS_BY_INDEX						= 0x00004,
			CONVERT_LATTICE_S2CA_STEREOGRAPHIC_AND_T	= 0x00010,
			CONVERT_LATTICE_S2CB_STEREOGRAPHIC_AND_T	= 0x00020,
			CONVERT_COLORS_S2CA_STEREOGRAPHIC_AND_T		= 0x00040,
			CONVERT_COLORS_S2CB_STEREOGRAPHIC_AND_T		= 0x00080,
		};

		Options options;

		typedef TFibonacciLattice<glm::vec3> fibLattice_t;
		fibLattice_t fibLattice; // TFibonacciLattice<glm::vec3> fibLattice;

		typedef std::vector<glm::vec3> colors_t;
		colors_t colors; // std::vector<glm::vec3> colors;

		TFibonacciSphereVisualizationForHistogram() {}

		bool Create(H& histogram, Options options) {

			fibLattice.Create(histogram.neighborhood.size());

			//TRACE("generating sphere with %d points\n", histogram.neighborhood.size());

			if (Options::SORT_LATTICE_POINTS_BY_INDEX & options)
			{
				std::sort(fibLattice.lattice.begin(), fibLattice.lattice.end(), TSortVec3Predicate());
			}

			// ------------------------------------------------------------------------

			auto neighborhoodSize = histogram.neighborhood.size();
			float ooneighborhoodSize = 1.0; // / neighborhoodSize;

			colors.resize(neighborhoodSize);

			auto it = histogram.neighborhood.begin();
			auto end = histogram.neighborhood.end();

			for (; end != it; ++it)
			{
				size_t index = it->first - 1; // indices start at 1 
				size_t count = it->second.size();

				//TRACE("index %d count %d\n", index, count);

				auto firstColor = it->second.begin();
				auto lastColor = it->second.end();

				ASSERT(lastColor != firstColor);

				// add colors to the color table and project points after
				// really the first one should be the quantized but with the
				// map it probably will end up being the most popular source

				colors[index] = TRGB2Vec3(firstColor->first);// (float)firstColor->second* ooneighborhoodSize);
				glm::vec3 pt = fibLattice.lattice[index];

				//TRACE("\tRGB (%2.8f,%2.8f,%2.8f) count %d\n", colors[index].r, colors[index].g, colors[index].b, firstColor->second);

				// TODO - think about other visualizations like mini spheres where the quantized value (the one that can be represented
				// visually (say 8 bits per channel) and the orbiting decendants are on their own sphere orbiting the quantized color
				// kind of makes me think of 3d pool, which would make a fun game. still like a cosmoball 2.0 idea need to chat with Clay

				const float stepSize = 1.0f / 16.0; // need to be smarter about this?
				float offset = 2.0 * stepSize;

				for (++firstColor; lastColor != firstColor; firstColor++)
				{
					glm::vec3 cv = TRGB2Vec3(firstColor->first); // (float)firstColor->second* ooneighborhoodSize);

					//TRACE("\t\tRGB (%2.8f,%2.8f,%2.8f) count %d\n", cv.r, cv.g, cv.b, firstColor->second);

					glm::vec3 p;

					if (Options::PUSH_OUTSIDE_THE_UNIT_SPHERE_FOR_DECENDENTS & options)
					{
						p = pt + pt * offset; // I like this it looks neat on a sphere (in a cube?)
					}
					else
					{
						p = pt;
					}

					offset += stepSize;

					colors.push_back(cv);
					fibLattice.lattice.push_back(p);
				}
			}

			TRACE("quantized color count %d (actual color count %d)\n", neighborhoodSize, colors.size());

			// -------------

			if (Options::SORT_COLORS_BY_INDEX & options)
			{
				std::sort(colors.begin(), colors.end(), TSortVec3Predicate());
			}

			if (Options::CONVERT_COLORS_S2CA_STEREOGRAPHIC_AND_T & options)
			{
				for (auto first = colors.begin(), last = colors.end(); last != first; ++first)
				{
					glm::vec3 v3 = *first;
					glm::vec2 v2 = BPT::s2cA(v3);
					*first = glm::vec3(v2, glm::length(v3));
				}
			}
			else if (Options::CONVERT_COLORS_S2CB_STEREOGRAPHIC_AND_T & options)
			{
				for (auto first = colors.begin(), last = colors.end(); last != first; ++first)
				{
					glm::vec3 v3 = *first;
					glm::vec2 v2 = BPT::s2cB(v3);
					*first = glm::vec3(v2, glm::length(v3));
				}
			}

			// -------------

			if (Options::CONVERT_LATTICE_S2CA_STEREOGRAPHIC_AND_T & options)
			{
				for (auto first = fibLattice.lattice.begin(), last = fibLattice.lattice.end(); last != first; ++first)
				{
					glm::vec2 v2 = BPT::s2cA(*first);
					*first = glm::vec3(v2, glm::length(*first));
				}
			}
			else if (Options::CONVERT_LATTICE_S2CB_STEREOGRAPHIC_AND_T & options)
			{
				for (auto first = fibLattice.lattice.begin(), last = fibLattice.lattice.end(); last != first; ++first)
				{
					glm::vec2 v2 = BPT::s2cB(*first);
					*first = glm::vec3(v2, glm::length(*first));
				}
			}

			return true;
		}

	}; // TFibonacciSphereVisualizationForHistogram

	// ---------------------------------------------------------------------------

} // namespace BPT

#endif // TQUANTIZEDHISTOGRAM_H__4EE3AB65_FB91_4E19_9877_81FB6B6BDBFD__INCLUDED_

