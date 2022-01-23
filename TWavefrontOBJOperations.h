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

#if !defined(TWAVEFRONTOBJOPERATIONS_H__4EE3AB65_FB91_4E19_9877_81FB6B6BDBFD__INCLUDED_)
#define TWAVEFRONTOBJOPERATIONS_H__4EE3AB65_FB91_4E19_9877_81FB6B6BDBFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TFace3Vector3.h"
#include <cstdio>
#include <iterator>

namespace BPT {

	//
	//	ReadFloatFromStrtok
	//

	template< typename T >
	float ReadFloatFromStrtok(T pszWhitespace) {

		char* p = strtok(NULL, pszWhitespace);

		if (!p) return 0.0f;

		return (float)atof(p);

	}

	// ---------------------------------------------------------------------------

	// ---------------------------------------------------------------------------

	//
	//	SavePointCloud
	//

	template<typename V,typename C>
	bool SavePointCloud(const char* filename, V firstVertex, V lastVertex, C firstColor, C lastColor )
	{
		FILE* outFile = fopen(filename, "wt");

		if (!outFile) {

			return false;

		}

		size_t vertCount = std::distance(firstVertex, lastVertex);
		size_t colorCount = std::distance(firstColor, lastColor);

		ASSERT(colorCount == vertCount);

		fprintf(outFile, "# %d vertices\n", vertCount );

		for (; firstVertex != lastVertex; ++firstVertex ) {

			if (lastColor != firstColor) {

				fprintf(outFile, "v %2.8lf %2.8lf %2.8lf %2.8lf %2.8lf %2.8lf\n",
					(double)firstVertex->x, (double)firstVertex->y, (double)firstVertex->z,
					(double)firstColor->r, (double)firstColor->g, (double)firstColor->b);

				++firstColor;

			}
			else {

				fprintf(outFile, "v %2.8lf %2.8lf %2.8lf\n", (double)firstVertex->x, (double)firstVertex->y, (double)firstVertex->z );

			}

		}

		fclose(outFile);

		return true;
	}

	// ---------------------------------------------------------------------------

	//
	//	SaveTriangleStripOrderedFaces
	//

	template<typename V, typename C>
	bool SaveTriangleStripOrderedFaces(const char* filename, V firstVertex, V lastVertex, C firstColor, C lastColor)
	{
		FILE* outFile = fopen(filename, "wt");

		if (!outFile) {

			return false;

		}

		size_t vertCount = std::distance(firstVertex, lastVertex);
		size_t colorCount = std::distance(firstColor, lastColor);

		ASSERT(colorCount == vertCount);

		fprintf(outFile, "# %d vertices\n", vertCount);

		for ( auto readVertexIt = firstVertex, readColorIt = firstColor; lastVertex != readVertexIt; ) {

			fprintf(outFile, "v %2.8lf %2.8lf %2.8lf %2.8lf %2.8lf %2.8lf\n",
				(double)readVertexIt->x, (double)readVertexIt->y, (double)readVertexIt->z,
				(double)readColorIt->x, (double)readColorIt->y, (double)readColorIt->z);

			fprintf(outFile, "vn %2.8lf %2.8lf %2.8lf\n",
				(double)readVertexIt->x, (double)readVertexIt->y, (double)readVertexIt->z);
				//(double)readColorIt->x, (double)readColorIt->y, (double)readColorIt->z);

			fprintf(outFile, "vt %2.8lf %2.8lf %2.8lf\n",
				(double)readColorIt->x, (double)readColorIt->y, (double)readColorIt->z);

			++readColorIt;
			++readVertexIt;

		}

		size_t faceCount = vertCount - 3;

		fprintf(outFile, "# %d faces\n", faceCount);

		for (size_t index = 0; index <= faceCount; ++index)
		{
			fprintf(outFile, "f %d/%d/%d %d/%d/%d %d/%d/%d\n",
				index + 1, index + 1, index + 1,
				index + 2, index + 2, index + 2,
				index + 3, index + 3, index + 3
			);
		}

		fclose(outFile);

		return true;
	}

	// ---------------------------------------------------------------------------

	//
	//	SaveFacesAs3dObjFile()
	//

	template< typename ITERATOR >
	bool SaveFacesAs3dObjFile(const char* filename, ITERATOR begin, ITERATOR end) {

		FILE* outFile = fopen(filename, "wt");

		if (!outFile) {

			return false;

		}

		int nFaces = 0;

		for (ITERATOR countIT = begin; countIT != end; ++countIT) {

			++nFaces;

		}

		fprintf(outFile, "# thinkbpt.com object output routine.\n");
		//fprintf( outFile, "o untitled\n" );
		fprintf(outFile, "# %d vertices, %d faces\n", nFaces * 3, nFaces);

		// write the vertices

		{

			ITERATOR it = begin;

			for (int v = 0; v < nFaces; v++) {

				fprintf(outFile, "v %f %f %f\n", it->p1.x(), it->p1.y(), it->p1.z());
				fprintf(outFile, "v %f %f %f\n", it->p2.x(), it->p2.y(), it->p2.z());
				fprintf(outFile, "v %f %f %f\n", it->p3.x(), it->p3.y(), it->p3.z());

				++it;

			}

		}

		// write the texture UV's

		{

			ITERATOR it = begin;

			for (int uv = 0; uv < nFaces; uv++) {

				fprintf(outFile, "vt %f %f\n", it->p1.u(), it->p1.v());
				fprintf(outFile, "vt %f %f\n", it->p2.u(), it->p2.v());
				fprintf(outFile, "vt %f %f\n", it->p3.u(), it->p3.v());

				++it;

			}

		}

		// write the faces

		fprintf(outFile, "g untitled_group\n");

		{

			int index = 1;

			for (int f = 0; f < nFaces; f++) {

				int a = index++;
				int b = index++;
				int c = index++;

				fprintf(outFile, "f %d/%d %d/%d %d/%d\n", a, a, b, b, c, c);

			}

		}

		fclose(outFile);

		return true;

	}

	//
	//	SaveFacesAs3dObjFile()
	//

	template< class TFACE >
	bool SaveFacesAs3dObjFile(const char* filename, const TFACE* pFaces, const int nFaces) {

		return SaveFacesAs3dObjFile(filename, pFaces, pFaces + nFaces);

	}

	// ------------------------------------------------------------------------

	// ------------------------------------------------------------------------

	//
	//	TAddTriangle
	//

	template<
		typename FACES
	,	typename VERTICES
	,	typename COORDS
	,	typename NORMALS
	>
	void TAddTriangle(
		FACES& faces
	,	VERTICES& verts
	,	COORDS& coords
	,	NORMALS& normals
	,	const int av, const int at, const int an
	,	const int bv, const int bt, const int bn
	,	const int cv, const int ct, const int cn
	) {

		typename FACES::value_type face;

		face.p1 = verts[av - 1];
		face.p2 = verts[bv - 1];
		face.p3 = verts[cv - 1];

		face.centroid._x = (face.p1._x + face.p2._x + face.p3._x) / 3.0f;
		face.centroid._y = (face.p1._y + face.p2._y + face.p3._y) / 3.0f;
		face.centroid._z = (face.p1._z + face.p2._z + face.p3._z) / 3.0f;

		if (coords.size()) {

			face.p1._u = coords[at - 1].first;
			face.p1._v = coords[at - 1].second;

			face.p2._u = coords[bt - 1].first;
			face.p2._v = coords[bt - 1].second;

			face.p3._u = coords[ct - 1].first;
			face.p3._v = coords[ct - 1].second;

		}

		if (normals.size()) {

			face.p1._nx = normals[an - 1]._x;
			face.p1._ny = normals[an - 1]._y;
			face.p1._nz = normals[an - 1]._z;

			face.p2._nx = normals[bn - 1]._x;
			face.p2._ny = normals[bn - 1]._y;
			face.p2._nz = normals[bn - 1]._z;

			face.p3._nx = normals[cn - 1]._x;
			face.p3._ny = normals[cn - 1]._y;
			face.p3._nz = normals[cn - 1]._z;

			face.centroid._nx = (face.p1._nx + face.p2._nx + face.p3._nx) / 3.0f;
			face.centroid._ny = (face.p1._ny + face.p2._ny + face.p3._ny) / 3.0f;
			face.centroid._nz = (face.p1._nz + face.p2._nz + face.p3._nz) / 3.0f;

			//			face.centroid.Normalize();

		}

		faces.push_back(face);

	}

	//
	//	LoadFacesFrom3dObjFile()
	//

	template< typename CONTAINER >
	bool LoadFacesFrom3dObjFile(
		const char* pszFilename, const char* pszObjName, CONTAINER& container
	) {

		FILE* inFile = fopen(pszFilename, "rt");

		if (NULL == inFile) return false;

		char line[1024];

		std::vector< VECTOR3 > vertices;

		std::vector< std::pair<float, float> > textureCoords;

		std::vector< VECTOR3 > normals;

		for (int lines = 0; NULL != fgets(line, (sizeof(line) / sizeof(line[0])) - 1, inFile); lines++) {

			char* pCmd = strtok(line, " \t");

			if (!pCmd) continue;

			if (!strcmp(pCmd, "v")) { // x, y, z

				float x = ReadFloatFromStrtok(" \t");
				float y = ReadFloatFromStrtok(" \t");
				float z = ReadFloatFromStrtok(" \t");

				vertices.push_back(VECTOR3(x, y, z));

			}
			else if (!strcmp(pCmd, "vn")) { // nx, ny, nz

				float nx = ReadFloatFromStrtok(" \t");
				float ny = ReadFloatFromStrtok(" \t");
				float nz = ReadFloatFromStrtok(" \t");

				normals.push_back(VECTOR3(nx, ny, nz));

			}
			else if (!strcmp(pCmd, "vt")) { // texture coordinate

				float u = ReadFloatFromStrtok(" \t");
				float v = ReadFloatFromStrtok(" \t");

				textureCoords.push_back(std::make_pair(u, v));

			}
			else if (!strcmp(pCmd, "f")) { // face

				int data[4 * 4];

				int index = 0;

				memset(data, 0, sizeof(data));

				for (int i = 0; i < 4; i++) {

					char* pSection = strtok(NULL, " \n\r");

					if (!pSection) break;

					for (int j = 0; j < 4; j++) {

						char* pTerm = strstr(pSection, "/");

						if (pTerm) *pTerm = 0;

						int value = atoi(pSection);

						data[index + j] = value;

						if (!pTerm) break;

						if (!(*(pTerm + 1))) break;

						pSection = pTerm + 1;

					}

					index += 4;

				}

				// Insert our face

				if (3 == i) { // 1 triangle

					TAddTriangle(
						container
						, vertices
						, textureCoords
						, normals
						, data[0 + 0], data[0 + 1], data[0 + 2]
						, data[4 + 0], data[4 + 1], data[4 + 2]
						, data[8 + 0], data[8 + 1], data[8 + 2]
					);

				}
				else if (4 == i) { // two triangles...

					TAddTriangle(
						container
						, vertices
						, textureCoords
						, normals
						, data[0 + 0], data[0 + 1], data[0 + 2]
						, data[4 + 0], data[4 + 1], data[4 + 2]
						, data[8 + 0], data[8 + 1], data[8 + 2]
					);

					TAddTriangle(
						container
						, vertices
						, textureCoords
						, normals
						, data[0 + 0], data[0 + 1], data[0 + 2]
						, data[8 + 0], data[8 + 1], data[8 + 2]
						, data[12 + 0], data[12 + 1], data[12 + 2]
					);

				}

			}

		}

		fclose(inFile);

		return true;

	}

	// ---------------------------------------------------------------------------

	//
	//	TWriteOctahedronFaces()
	//

	template<typename V, typename C>
	int TWriteOctahedronFaces(FILE* outFile, const V& v, const C& c, int nextIndex)
	{

		// TODO

/*

#if 1

		float o = 1.0f / 256.0f;

		V v2 = glm::normalize(v) * o;

		fprintf(outFile, "v %2.8lf %2.8lf %2.8lf %2.8lf %2.8lf %2.8lf\n",
			v.x + v2.x, v.y + v2.y, v.z + v2.z,
			c.x, c.y, c.z
		);

		fprintf(outFile, "v %2.8lf %2.8lf %2.8lf %2.8lf %2.8lf %2.8lf\n",
			v.x + v2.z, v.y + 0.0f, v.z + -v2.x,
			c.x, c.y, c.z
		);

		fprintf(outFile, "v %2.8lf %2.8lf %2.8lf %2.8lf %2.8lf %2.8lf\n",
			v.x + -(-v2.x * v2.y), v.y + -(v2.x * v2.x) - (v2.z * v2.z), v.z + v2.z * v2.y,
			c.x, c.y, c.z
		);

		return 3;

#else

		V v2 = glm::normalize(v);

		fprintf(outFile, "v %2.8lf %2.8lf %2.8lf %2.8lf %2.8lf %2.8lf\n",
			v2.x, v2.y, v2.z,
			c.x, c.y, c.z
		);

		fprintf(outFile, "v %2.8lf %2.8lf %2.8lf %2.8lf %2.8lf %2.8lf\n",
			v2.z, 0.0f, -v2.x,
			c.x, c.y, c.z
		);

		fprintf(outFile, "v %2.8lf %2.8lf %2.8lf %2.8lf %2.8lf %2.8lf\n",
			-(-v2.x * v2.y), -(v2.x * v2.x) - (v2.z * v2.z), v2.z * v2.y,
			c.x, c.y, c.z
		);

		return 3;

#endif

*/

	}

	// ---------------------------------------------------------------------------

	//
	//	TWriteOctahedronFaces()
	//

	template<typename V, typename C>
	int TSingleTriangleFace(FILE* outFile, const V& v, const C& c, int nextIndex)
	{
		float o = 1.0f / 256.0f;

		V v2 = glm::normalize(v) * o;

		fprintf(outFile, "v %2.8lf %2.8lf %2.8lf %2.8lf %2.8lf %2.8lf\n",
			v.x + v2.x, v.y + v2.y, v.z + v2.z,
			c.x, c.y, c.z
		);

		fprintf(outFile, "v %2.8lf %2.8lf %2.8lf %2.8lf %2.8lf %2.8lf\n",
			v.x + v2.z, v.y + 0.0f, v.z + -v2.x,
			c.x, c.y, c.z
		);

		fprintf(outFile, "v %2.8lf %2.8lf %2.8lf %2.8lf %2.8lf %2.8lf\n",
			v.x + -(-v2.x * v2.y), v.y + -(v2.x * v2.x) - (v2.z * v2.z), v.z + v2.z * v2.y,
			c.x, c.y, c.z
		);

		int i = nextIndex;

		fprintf(outFile, "f %d/%d/%d\n", i + 0, i + 0, i + 0);
		fprintf(outFile, "f %d/%d/%d\n", i + 1, i + 1, i + 1);
		fprintf(outFile, "f %d/%d/%d\n", i + 2, i + 2, i + 2);

		return 3;
	}

	// ---------------------------------------------------------------------------

	//
	//	TWriteWeirdBoxOfBoxes()
	//

	template<typename V, typename C>
	int TWriteWeirdBoxOfBoxes(FILE* outFile, const V& v, const C& c, int nextIndex)
	{
		int count = 1;

		float o = 1.0f / 1024.0f;

		//V n = glm::normalize(v) * o;

		// write our 3x3x3 faces 
		// --------------------------------------------------------------------------

		for (float z = -o; z <= o; z += o)
		{
			for (float y = -o; y <= o; y += o)
			{
				for (float x = -o; x <= o; x += o)
				{
					glm::vec3 p = v + glm::normalize(v + glm::vec3(x, y, z)) * o;

					auto uv = s2cB(p);

					fprintf(outFile, "v %2.8lf %2.8lf %2.8lf %2.8lf %2.8lf %2.8lf %2.8lf %2.8lf %2.8lf\n",
						v.x + x, v.y + y, v.z + z,
						c.x, c.y, c.z,
						uv.x,uv.y,length(p) //glm::dot(uv,uv)
					);

					++count;
				}
			}
		}

		// create a bunch of faces to create the box of boxes
		// --------------------------------------------------------------------------

		for (int i = 0; i < count; i++)
		{
			int ni = nextIndex + i;
			fprintf(outFile, "f %d/%d/%d\n", ni+0, ni+0, ni+0);
			fprintf(outFile, "f %d/%d/%d\n", ni+1, ni+1, ni+1);
			fprintf(outFile, "f %d/%d/%d\n", ni+2, ni+2, ni+2);
		}

		return count;
	}

	//
	//	TSaveWeirdTriangleSoupOID
	//  -- original, inverse, difference
	//

	template<typename V, typename C>
	bool TSaveWeirdTriangleSoupOID(const char* filename, V firstVertex, V lastVertex, C firstColor, C lastColor)
	{
		FILE* outFile = fopen(filename, "wt");

		if (!outFile) {

			return false;

		}

		int vertCount = std::distance(firstVertex, lastVertex);
		int colorCount = std::distance(firstColor, lastColor);

		ASSERT(colorCount == vertCount);

		//fprintf(outFile, "# %d vertices\n", vertCount * 3);

		int nextFaceNumber = 1;

		for (auto readVertexIt = firstVertex, readColorIt = firstColor; lastVertex != readVertexIt; ) {

			//int faces = TWriteWeirdBoxOfBoxes(outFile, *readVertexIt, *readColorIt, nextFaceNumber);
			int faces = TSingleTriangleFace(outFile, *readVertexIt, *readColorIt, nextFaceNumber);

			nextFaceNumber += faces;

			++readColorIt;
			++readVertexIt;

		}

		fclose(outFile);

		return true;
	}

} // namespace BPT

#endif // TWAVEFRONTOBJOPERATIONS_H__4EE3AB65_FB91_4E19_9877_81FB6B6BDBFD__INCLUDED_

