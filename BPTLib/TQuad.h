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

#if !defined(AFX_TQUAD_H__43FF9B08_797F_4DAE_89C3_E69DCFF532F0__INCLUDED_)
#define AFX_TQUAD_H__43FF9B08_797F_4DAE_89C3_E69DCFF532F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ----------------------------------------------------------------------------

//#define USE_TRIANGLE_SUB_DIV

#if defined(USE_TRIANGLE_SUB_DIV)
#include "TTriangle.h"
#endif

namespace BPT {

#if defined( BPTLIB_3D_HELPERS )

	// ========================================================================
	// FVF QUAD
	// ========================================================================

	//
	//	TFVFQuad
	//
	//	The vertex data is stored so that it can be used as a triangle strip
	//
	//	A->B
	//	|<-|
	//	C->D
	//

	template< typename FVF >
	class TFVFQuad {

	private:

		FVF m_Quad[ 4 ];

	public:

		typedef FVF vertex_type;

		TFVFQuad() { /* Empty */ }

		TFVFQuad( const FVF & a, const FVF & b, const FVF & c, const FVF & d ) {

			m_Quad[ 0 ] = a;
			m_Quad[ 1 ] = b;
			m_Quad[ 2 ] = c;
			m_Quad[ 3 ] = d;

		}

		FVF * Data() {

			return m_Quad;

		}

		FVF & A() {

			return m_Quad[ 0 ];

		}

		FVF & B() {

			return m_Quad[ 1 ];

		}

		FVF & C() {

			return m_Quad[ 2 ];

		}

		FVF & D() {

			return m_Quad[ 3 ];

		}

	};

	// ========================================================================
	// Tessellated QUAD renderer
	// ========================================================================

	//
	//	TTessellatedQuadRenderer()
	//
	
	template<
		typename QUAD
		, typename LERP
		, typename FILTER
		, typename RENDERER
		,const int MAX_SUB_DIV = 32
		,const bool PERSISTENT_CACHE = false
	>
	class TTessellatedQuadRenderer {

	private:

		enum {

			MAX_VERTS_PER_LINE = (MAX_SUB_DIV + 1)
			,MAX_OUTPUT_VERTS = (MAX_VERTS_PER_LINE * MAX_VERTS_PER_LINE)

		};

	public:

		void Render(
			QUAD & quad
			,const int desiredSubDiv = 1
			,FILTER & filter = FILTER()
			,RENDERER & renderer = RENDERER()
			,bool useIndexedPrimitives = false
		) {

#if defined(USE_TRIANGLE_SUB_DIV)

			QUAD::vertex_type triangles[ 6 ];

			triangles[ 0 ] = quad.C();
			triangles[ 1 ] = quad.A();
			triangles[ 2 ] = quad.B();
			triangles[ 3 ] = quad.C();
			triangles[ 4 ] = quad.B();
			triangles[ 5 ] = quad.D();

			TTessellatedTriangleRenderer< 
				typename QUAD::vertex_type, LERP, FILTER, RENDERER, MAX_SUB_DIV, PERSISTENT_CACHE
			> triangleRenderer;

			triangleRenderer.Render( &triangles[ 0 ], desiredSubDiv, filter, renderer, useIndexedPrimitives);
			triangleRenderer.Render( &triangles[ 3 ], desiredSubDiv, filter, renderer, useIndexedPrimitives );

#else

			// Check to see if this is a simple operation
			// ----------------------------------------------------------------

			if ( 1 == desiredSubDiv ) {

				typename QUAD::vertex_type filteredQuad[ 4 ];

				filteredQuad[ 0 ] = quad.A();
				filteredQuad[ 1 ] = quad.B();
				filteredQuad[ 2 ] = quad.C();
				filteredQuad[ 3 ] = quad.D();

				filter( filteredQuad[ 0 ] );
				filter( filteredQuad[ 1 ] );
				filter( filteredQuad[ 2 ] );
				filter( filteredQuad[ 3 ] );

				renderer.DrawTriangleStrip( filteredQuad, 4, 0 );
				return;

			}

			const int subDiv = max(2, min( desiredSubDiv, MAX_SUB_DIV ) );

			// Allocate all our working space
			// ----------------------------------------------------------------

			float coefficients[ MAX_VERTS_PER_LINE ];
			typename QUAD::vertex_type outputVerts[ MAX_OUTPUT_VERTS ];

			// Setup the coefficient table
			// ----------------------------------------------------------------

			const int divVertCount = (subDiv + 1);

			coefficients[ 0 ] = 0.0f;
			coefficients[ subDiv ] = 1.0f;

			float step = 1.0f / (float)subDiv;
			float curr = step;

			for ( int i = 1; i < subDiv; i++ ) {

				coefficients[ i ] = curr;
				curr += step;

			}

			// Setup the working vertex and the control line segment lerp's
			// ----------------------------------------------------------------

			LERP ac( quad.A(), quad.C() );
			LERP bd( quad.B(), quad.D() );

			typename QUAD::vertex_type vert = quad.A();
			typename QUAD::vertex_type vertAC = quad.A();
			typename QUAD::vertex_type vertBD = quad.B();
			typename QUAD::vertex_type * outputPtr = outputVerts;

			const float * outterCoeffPtr = coefficients;

			for ( int outter = 0; outter < divVertCount; outter++ ) {

				float t = coefficients[ outter ];

				LERP acbd( 
					ac.Position( vertAC, t ),
					bd.Position( vertBD, t )
				);

				for ( int inner = 0; inner < divVertCount; inner++ ) {

					*outputPtr = acbd.Position( vert, coefficients[ inner ] );
					filter( *outputPtr );
					++outputPtr;

				}

			}

			// Finally output the triangles
			// ----------------------------------------------------------------

			if ( useIndexedPrimitives ) {

				RenderIndexedTriangleStrips( renderer, outputVerts, subDiv );

			} else {

				RenderRawTriangleStrips( renderer, outputVerts, subDiv );

			}

#endif	// !defined(USE_TRIANGLE_SUB_DIV)

		}

#if !defined(USE_TRIANGLE_SUB_DIV)

		// --------------------------------------------------------------------

		void
		RenderRawTriangleStrips( RENDERER & renderer, typename QUAD::vertex_type * vertexData, const int subDiv ) {

			const int indexesPerStrip = (subDiv + 1) * 2;

			const typename QUAD::vertex_type * vertexA = vertexData;
			const typename QUAD::vertex_type * vertexB = &vertexData[ (subDiv + 1) ];

			typename QUAD::vertex_type stripVerts[ (MAX_VERTS_PER_LINE * 2) ];

			for ( int strip = 0; strip < subDiv; strip++ ) {

				// Build the index table
				// ------------------------------------------------------------

				typename QUAD::vertex_type * vertexPtr = stripVerts;

				*vertexPtr++ = *vertexB++;
				*vertexPtr++ = *vertexA++;

				for ( int indexCounter = 0; indexCounter < subDiv; indexCounter++ ) {

					*vertexPtr++ = *vertexB++;
					*vertexPtr++ = *vertexA++;

				}

				// Call the renderer
				// ------------------------------------------------------------

				renderer.DrawTriangleStrip(
					stripVerts,
					indexesPerStrip,
					0
				);

			}

		}
	
		// --------------------------------------------------------------------

		void
		RenderIndexedTriangleStrips( RENDERER & renderer, typename QUAD::vertex_type * vertexData, const int subDiv ) {

			const int divVertCount = (subDiv + 1);
			const int outputVertexCount = (divVertCount * divVertCount);
			const int indexesPerStrip = divVertCount * 2;

			int indexA = 0;
			int indexB = divVertCount;

			WORD stripIndexTable[ (MAX_VERTS_PER_LINE * 2) ];

			for ( int strip = 0; strip < subDiv; strip++ ) {

				// Build the index table
				// ------------------------------------------------------------

				WORD * indexPtr = stripIndexTable;

				*indexPtr++ = indexB++;
				*indexPtr++ = indexA++;

				for ( int indexCounter = 0; indexCounter < subDiv; indexCounter++ ) {

					*indexPtr++ = indexB++;
					*indexPtr++ = indexA++;

				}

				// Call the renderer
				// ------------------------------------------------------------

				renderer.DrawIndexedTriangleStrip(
					vertexData,
					outputVertexCount,
					stripIndexTable,
					indexesPerStrip,
					0
				);

			}

		}

#endif	// !defined(USE_TRIANGLE_SUB_DIV)

	};

	// ========================================================================

	//
	//	TQuadToFanRenderer()
	//
	
	template<
		typename QUAD
		, typename LERP
		, typename FILTER
		, typename RENDERER
	>
	class TQuadToFanRenderer {

	private:

		void MidPoint(
			typename QUAD::vertex_type & out
			,const typename QUAD::vertex_type & a
			,const typename QUAD::vertex_type & b
		) {

			LERP ab( a, b );

			out = a;

			ab.Position( out, 0.5f );

		}

	public:

		void Render(
			QUAD & quad
			,const int desiredSubDiv = 1
			,FILTER & filter = FILTER()
			,RENDERER & renderer = RENDERER()
			,const bool indexedPrims = false
		) {

			if ( 1 == desiredSubDiv ) {

				typename QUAD::vertex_type fanVertexData[ 6 ];

				// Do the easy ones
				
				fanVertexData[ 1 ] = quad.A();
				fanVertexData[ 2 ] = quad.B();
				fanVertexData[ 3 ] = quad.D();
				fanVertexData[ 4 ] = quad.C();

				// finally the centroid

				MidPoint( fanVertexData[ 0 ], fanVertexData[ 1 ], fanVertexData[ 3 ] );

				// Filter the 6 triangles

				filter( fanVertexData[ 0 ] );
				filter( fanVertexData[ 1 ] );
				filter( fanVertexData[ 2 ] );
				filter( fanVertexData[ 3 ] );
				filter( fanVertexData[ 4 ] );

				fanVertexData[ 5 ] = fanVertexData[ 1 ];

				// finally output them to the renderer
				
				renderer.DrawTriangleFan( fanVertexData, 6, 0 );

			} else {
			
				typename QUAD::vertex_type fanVertexData[ 10 ];
				
				// Do the easy ones
				
				fanVertexData[ 1 ] = quad.A();
				fanVertexData[ 3 ] = quad.B();
				fanVertexData[ 5 ] = quad.D();
				fanVertexData[ 7 ] = quad.C();
				
				// Get the mid point's
				
				MidPoint( fanVertexData[ 2 ], quad.A(), quad.B() );
				MidPoint( fanVertexData[ 4 ], quad.B(), quad.D() );
				MidPoint( fanVertexData[ 6 ], quad.C(), quad.D() );
				MidPoint( fanVertexData[ 8 ], quad.A(), quad.C() );
				
				// finally the centroid
				
				MidPoint( fanVertexData[ 0 ], fanVertexData[ 2 ], fanVertexData[ 6 ] );

				if ( 2 == desiredSubDiv ) {

					// now filter the 10 triangles
					
					filter( fanVertexData[ 0 ] );
					filter( fanVertexData[ 1 ] );
					filter( fanVertexData[ 2 ] );
					filter( fanVertexData[ 3 ] );
					filter( fanVertexData[ 4 ] );
					filter( fanVertexData[ 5 ] );
					filter( fanVertexData[ 6 ] );
					filter( fanVertexData[ 7 ] );
					filter( fanVertexData[ 8 ] );
					
					fanVertexData[ 9 ] = fanVertexData[ 1 ];
					
					// finally output them to the renderer
					
					renderer.DrawTriangleFan( fanVertexData, 10, 0 );

				} else {

					QUAD a( fanVertexData[ 1 ], fanVertexData[ 2 ], fanVertexData[ 8 ], fanVertexData[ 0 ] );
					QUAD b( fanVertexData[ 2 ], fanVertexData[ 3 ], fanVertexData[ 0 ], fanVertexData[ 4 ] );
					QUAD c( fanVertexData[ 8 ], fanVertexData[ 0 ], fanVertexData[ 7 ], fanVertexData[ 6 ] );
					QUAD d( fanVertexData[ 0 ], fanVertexData[ 4 ], fanVertexData[ 6 ], fanVertexData[ 5 ] );
	
					int nDiv = (desiredSubDiv - 1);
	
					this->Render( a, nDiv, filter, renderer );
					this->Render( b, nDiv, filter, renderer );
					this->Render( c, nDiv, filter, renderer );
					this->Render( d, nDiv, filter, renderer );

				}

			}

		}

	};

#endif // defined( BPTLIB_3D_HELPERS )

}; // namespace BPT

#endif // !defined(AFX_TQUAD_H__43FF9B08_797F_4DAE_89C3_E69DCFF532F0__INCLUDED_)
