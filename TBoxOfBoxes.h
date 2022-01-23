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

#if !defined( BOXOFBOXES_7B58565E_0448_461f_98CC_3BD765D59AA0 )
#define BOXOFBOXES_7B58565E_0448_461f_98CC_3BD765D59AA0

#if _MSC_VER > 1000
#pragma once
#endif

// ----------------------------------------------------------------------------

#include <bitset>

namespace BPT {

	//
	//	AABitCube
	//

	template<const size_t WIDTH = 256,const size_t HEIGHT = 256,const size_t DEPTH = 256>
	struct AABitCube
	{
		enum : uint32_t
		{
			width = WIDTH,
			height = HEIGHT,
			depth = DEPTH
		};

		typedef std::bitset<WIDTH*HEIGHT*DEPTH> cubebitset_t;

		cubebitset_t bitCube;

		size_t toIndex(int x, int y, int z) const
		{
			size_t b = size_t(z) * (WIDTH * HEIGHT);
			return b + size_t(y) * WIDTH + x;
		}

		bool operator()(int x, int y, int z) const
		{
			if ((0 <= x) && (x < WIDTH) && (0 <= y) && (y < HEIGHT) && (0 <= z) && (z < HEIGHT))
			{
				return bm.test(toIndex(x, y, z));
			}

			return false;
		}

		template<typename I>
		void SetBitsForVerts(I first, I last, const float scale, const float bias)
		{
			for (auto it = first; it != last; ++it)
			{
				auto p = (*it) * scale + bias;

				int x = (int)p.x;
				int y = (int)p.y;
				int z = (int)p.z;

				if ((0 <= x) && (x < WIDTH) && (0 <= y) && (y < HEIGHT) && (0 <= z) && (z < HEIGHT))
				{
					bm.set(toIndex(x, y, z));
				}
			}
		}
	};

	//
	//	TCreateBoxOfBoxes()
	//

	template< typename FN, typename VERTS, typename NORMALS, typename UVS, typename INDICES >
	HRESULT
	TCreateBoxOfBoxes(
		FN & fnDefinedBox
	,	VERTS &verts
	,	NORMALS &normals
	,	UVS &uvs
	,	INDICES &indices
	,	const int nextIndex
	,	const int xCount
	,	const int yCount
	,	const int zCount
	,	const glm::vec3 & firstBoxCenter
	,	const glm::vec3 & innerBoxDimension
	,	const glm::vec3 & innerBoxDelta
	)
	{
		// Figure out the number of defined boxes
		// ----------------------------------------------------------------

		int definedBoxes = 0;

		{
			for (int z = 0; z < zCount; z++) {

				for (int y = 0; y < yCount; y++) {

					for (int x = 0; x < xCount; x++) {

						if (fnDefinedBox(x, y, z)) {

							++definedBoxes;

						}

					}

				}

			}

		}

		if (0 == definedBoxes) {

			return E_FAIL;

		}

		// ----------------------------------------------------------------

		int quads = 6 * definedBoxes;
		int wantFaces = quads * 2;
		int wantVerts = quads * 4;
		int numFaces = wantFaces;
		int numVertices = wantVerts;

		// Fill in the index table
		// ----------------------------------------------------------------

		{
			auto pIndices = std::back_inserter(indices);

			int baseIndex = nextIndex;

			for (int quad = 0; quad < quads; quad++) {

				*pIndices++ = (baseIndex + 3);
				*pIndices++ = (baseIndex + 0);
				*pIndices++ = (baseIndex + 1);
				*pIndices++ = (baseIndex + 3);
				*pIndices++ = (baseIndex + 1);
				*pIndices++ = (baseIndex + 2);

				baseIndex += 4;

			}

		}

		// Fill in the vertex table
		// ----------------------------------------------------------------

		{
			// Fill in the verts
			// ----------------------------------------------------------------

			// Setup the reset position for the box

			glm::vec3 reset[8];

			glm::vec3 half = innerBoxDimension * 0.5f;

			// front

			reset[0].x = firstBoxCenter.x - half.x;
			reset[0].y = firstBoxCenter.y + half.y;
			reset[0].z = firstBoxCenter.z - half.z;

			reset[1].x = firstBoxCenter.x + half.x;
			reset[1].y = firstBoxCenter.y + half.y;
			reset[1].z = firstBoxCenter.z - half.z;

			reset[2].x = firstBoxCenter.x + half.x;
			reset[2].y = firstBoxCenter.y - half.y;
			reset[2].z = firstBoxCenter.z - half.z;

			reset[3].x = firstBoxCenter.x - half.x;
			reset[3].y = firstBoxCenter.y - half.y;
			reset[3].z = firstBoxCenter.z - half.z;

			// back

			reset[4].x = firstBoxCenter.x - half.x;
			reset[4].y = firstBoxCenter.y + half.y;
			reset[4].z = firstBoxCenter.z + half.z;

			reset[5].x = firstBoxCenter.x + half.x;
			reset[5].y = firstBoxCenter.y + half.y;
			reset[5].z = firstBoxCenter.z + half.z;

			reset[6].x = firstBoxCenter.x + half.x;
			reset[6].y = firstBoxCenter.y - half.y;
			reset[6].z = firstBoxCenter.z + half.z;

			reset[7].x = firstBoxCenter.x - half.x;
			reset[7].y = firstBoxCenter.y - half.y;
			reset[7].z = firstBoxCenter.z + half.z;

			// fill in the normal table

			glm::vec3 faceNormals[6] = {
					glm::vec3(+0.0f, +0.0f, -1.0f)
				,	glm::vec3(+0.0f, +1.0f, +0.0f)
				,	glm::vec3(+0.0f, +0.0f, +1.0f)
				,	glm::vec3(+0.0f, -1.0f, +0.0f)
				,	glm::vec3(-1.0f, +0.0f, +0.0f)
				,	glm::vec3(+1.0f, +0.0f, +0.0f)
			};

			// fill in the uv table (each face could have different uv's)

			glm::vec3 faceUVs[6][4] = {

				{
						glm::vec3(0.0f, 0.0f, 0.0f)
					,	glm::vec3(1.0f, 0.0f, 0.0f)
					,	glm::vec3(1.0f, 1.0f, 0.0f)
					,	glm::vec3(0.0f, 1.0f, 0.0f)
				}
				,{
						glm::vec3(0.0f, 0.0f, 0.0f)
					,	glm::vec3(1.0f, 0.0f, 0.0f)
					,	glm::vec3(1.0f, 1.0f, 0.0f)
					,	glm::vec3(0.0f, 1.0f, 0.0f)
				}
				,{
						glm::vec3(0.0f, 0.0f, 0.0f)
					,	glm::vec3(1.0f, 0.0f, 0.0f)
					,	glm::vec3(1.0f, 1.0f, 0.0f)
					,	glm::vec3(0.0f, 1.0f, 0.0f)
				}
				,{
						glm::vec3(0.0f, 0.0f, 0.0f)
					,	glm::vec3(1.0f, 0.0f, 0.0f)
					,	glm::vec3(1.0f, 1.0f, 0.0f)
					,	glm::vec3(0.0f, 1.0f, 0.0f)
				}
				,{
						glm::vec3(0.0f, 0.0f, 0.0f)
					,	glm::vec3(1.0f, 0.0f, 0.0f)
					,	glm::vec3(1.0f, 1.0f, 0.0f)
					,	glm::vec3(0.0f, 1.0f, 0.0f)
				}
				,{
						glm::vec3(0.0f, 0.0f, 0.0f)
					,	glm::vec3(1.0f, 0.0f, 0.0f)
					,	glm::vec3(1.0f, 1.0f, 0.0f)
					,	glm::vec3(0.0f, 1.0f, 0.0f)
				}
			};

			// fill in the vertex generation order

			int faceIndices[6][4] = {

					{ 0, 1, 2, 3 }		// front
				,	{ 4, 5, 1, 0 }		// top
				,	{ 7, 6, 5, 4 }		// back
				,	{ 3, 2, 6, 7 }		// bottom
				,	{ 4, 0, 3, 7 }		// left
				,	{ 1, 5, 6, 2 }		// right

			};

			// build each 'face' in the proper order

			for (int f = 0; f < 6; f++) {

				glm::vec3 verts[8];

				glm::vec3 normal = faceNormals[f];

				glm::vec3* pUVs = faceUVs[f];

				int* idx = faceIndices[f];

				// reset z's

				for (int r = 0; r < 4; r++) { verts[idx[r]].z = reset[idx[r]].z; }

				for (int z = 0; z < zCount; z++) {

					// reset y's

					for (int r = 0; r < 4; r++) { verts[idx[r]].y = reset[idx[r]].y; }

					for (int y = 0; y < yCount; y++) {

						// reset x's

						for (int r = 0; r < 4; r++) { verts[idx[r]].x = reset[idx[r]].x; }

						for (int x = 0; x < xCount; x++) {

							// check to see if this box is solid

							if (!fnDefinedBox(x, y, z)) {

								for (int a = 0; a < 4; a++) {

									verts[idx[a]].x += innerBoxDelta.x;

								}

								continue;

							}

							for (int a = 0; a < 4; a++) {

								verts.push_back(verts[idx[a]]);
								normals.push_back(normal);
								uvs.push_back(pUVs[a]);

								// advance x's

								verts[idx[a]].x += innerBoxDelta.x;

							}

						}

						// advance y's

						for (int a = 0; a < 4; a++) { verts[idx[a]].y += innerBoxDelta.y; }

					}

					// advance z's

					for (int a = 0; a < 4; a++) { verts[idx[a]].z += innerBoxDelta.z; }

				}

			}

		}

		return S_OK;

	}

}; // namespace BPT

#endif /* defined(BOXOFBOXES_7B58565E_0448_461f_98CC_3BD765D59AA0) */

