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

#if !defined(AFX_TTRIANGLE_H__C2D0F8DC_E1FA_4DB2_AE2D_10F3A33A1FBF__INCLUDED_)
#define AFX_TTRIANGLE_H__C2D0F8DC_E1FA_4DB2_AE2D_10F3A33A1FBF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ----------------------------------------------------------------------------

namespace BPT {

#if defined( BPTLIB_3D_HELPERS )

	// ========================================================================
	// Tessellated TRIANGLE renderer
	// ========================================================================
	
	//
	//	TTessellatedTriangleRenderer()
	//
	
	template<
		class FVF
		,class LERP
		,class FILTER
		,class RENDERER
		,const int MAX_SUB_DIV = 32
		,const bool PERSISTENT_CACHE = false
	>
	class TTessellatedTriangleRenderer {

	private:

		// --------------------------------------------------------------------

		struct CACHE_DATA {
	
			enum {
	
				MAX_VERTEX_COUNT = ((MAX_SUB_DIV * MAX_SUB_DIV) * 2)
				,STRIP_VERTEX_COUNT = ((MAX_SUB_DIV * 2) + 1)
	
			};
	
			FVF vertexData[ MAX_VERTEX_COUNT ];
			FVF stripData[ STRIP_VERTEX_COUNT ];
			WORD indexData[ STRIP_VERTEX_COUNT ];
			
		};

		typedef CACHE_DATA * CACHE;

		// --------------------------------------------------------------------

	public:

		typedef FVF vertex_type;

		void Render(
			FVF * vertexData
			,const int desiredSubDiv = 1
			,FILTER & filter = FILTER()
			,RENDERER & renderer = RENDERER()
			,bool useIndexedPrimitives = false
		) {

			// build the cache the first time called
			// ----------------------------------------------------------------

			static CACHE s_Cache = 0;

			if ( !s_Cache ) {

				s_Cache = new CACHE_DATA;

			}

			// Check to see if this is a simple operation
			// ----------------------------------------------------------------

			if ( (1 == desiredSubDiv) || (0 == s_Cache) ) {

				FVF filteredTriangle[ 3 ];

				filteredTriangle[ 0 ] = vertexData[ 0 ];
				filteredTriangle[ 1 ] = vertexData[ 1 ];
				filteredTriangle[ 2 ] = vertexData[ 2 ];

				filter( filteredTriangle[ 0 ] );
				filter( filteredTriangle[ 1 ] );
				filter( filteredTriangle[ 2 ] );

				renderer.DrawTriangleStrip( filteredTriangle, 3, 0 );
				return;

			}

			// Create the output vertex list.
			// ----------------------------------------------------------------

			FVF * outputVerts = s_Cache->vertexData;

			LERP ac( vertexData[ 0 ], vertexData[ 2 ] );
			LERP ab( vertexData[ 0 ], vertexData[ 1 ] );

			FVF * outputPtr = outputVerts;

			int subDiv = max( 2, min( desiredSubDiv, MAX_SUB_DIV ));

			FVF vert( vertexData[ 0 ] );
			FVF vertAC( vert );
			FVF vertAB( vert );

			float outterStep = 1.0f / subDiv;
			float outterPos = outterStep;

			// Do the first one special

			*outputPtr = vert;
			filter( *outputPtr );
			++outputPtr;

			// Calculate the raw verts.

			for ( int n = 1; n <= subDiv; n++ ) {

				// Get this new line segment to process

				LERP acab( 
					ac.Position( vertAC, outterPos ),
					ab.Position( vertAB, outterPos )
				);

				outterPos += outterStep;

				// Calculate the new sub division points

				float innerStep = (1.0f / float(n));
				float innerPos = 0.0f;

				for ( int vertCounter = 0; vertCounter <= n; vertCounter++ ) {

					*outputPtr = acab.Position( vert, innerPos );
					innerPos += innerStep;

					filter( *outputPtr );
					++outputPtr;

				}

			}

			// Finally output the triangles
			// ----------------------------------------------------------------

			if ( useIndexedPrimitives ) {

				RenderIndexedTriangleStrips( renderer, outputVerts, subDiv, s_Cache );

			} else {

				RenderRawTriangleStrips( renderer, outputVerts, subDiv, s_Cache );

			}

			// Cache management
			// ----------------------------------------------------------------

			if ( !PERSISTENT_CACHE ) {

				delete s_Cache;

				s_Cache = 0;

			}

		}

	private:

		// --------------------------------------------------------------------

		void
		RenderRawTriangleStrips(
			RENDERER & renderer
			,FVF * vertexData
			,const int subDiv
			,CACHE s_Cache
		) {

			const FVF * vertexA = vertexData;
			const FVF * vertexB = &vertexData[ 1 ];

			FVF * stripVerts = s_Cache->stripData;

			for ( int strip = 1; strip <= subDiv; strip++ ) {

				// Build the index table
				// ------------------------------------------------------------

				FVF * vertexPtr = stripVerts;

				for ( int triangleCounter = 0; triangleCounter < strip; triangleCounter++ ) {

					*vertexPtr++ = *vertexB++;
					*vertexPtr++ = *vertexA++;

				}

				*vertexPtr++ = *vertexB++;

				// Call the renderer
				// ------------------------------------------------------------

				renderer.DrawTriangleStrip(
					stripVerts,
					((strip * 2) + 1),
					0
				);

			}

		}

		// --------------------------------------------------------------------

		void
		RenderIndexedTriangleStrips(
			RENDERER & renderer
			,FVF * vertexData
			,const int subDiv
			,CACHE s_Cache
		) {

			// Calculate the total verts in the table
			// ----------------------------------------------------------------

			int totalVertexCount = 0;

			for ( int n = 0; n <= subDiv; n++ ) {

				totalVertexCount += (n + 1);

			}

			// Build the triangle strips / indexes
			// ----------------------------------------------------------------

			int indexA = 0;
			int indexB = 1;

			WORD * indexTable = s_Cache->indexData;

			for ( int strip = 1; strip <= subDiv; strip++ ) {

				// Build the index table
				// ------------------------------------------------------------

				WORD * indexPtr = indexTable;

				for ( int triangleCounter = 0; triangleCounter < strip; triangleCounter++ ) {

					*indexPtr++ = indexB++;
					*indexPtr++ = indexA++;

				}

				*indexPtr++ = indexB++;

				// Call the renderer
				// ------------------------------------------------------------

				renderer.DrawIndexedTriangleStrip(
					vertexData,
					totalVertexCount,
					indexTable,
					((strip * 2) + 1),
					0
				);

			}

		}

	};

#endif // defined( BPTLIB_3D_HELPERS )

}; // namespace BPT

#endif // !defined(AFX_TTRIANGLE_H__C2D0F8DC_E1FA_4DB2_AE2D_10F3A33A1FBF__INCLUDED_)
