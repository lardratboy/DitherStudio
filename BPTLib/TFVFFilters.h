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

#if !defined(AFX_TFVFFILTERS_H__684847A2_264F_4B02_9730_84E35A20A976__INCLUDED_)
#define AFX_TFVFFILTERS_H__684847A2_264F_4B02_9730_84E35A20A976__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ----------------------------------------------------------------------------

#if defined( BPTLIB_SUPPORT_DX7 )
#include <d3d.h>
#endif // defined( BPTLIB_SUPPORT_DX7 )

// ----------------------------------------------------------------------------

namespace BPT {

//#define BPTLIB_2D_FRIENDLY

#if defined( BPTLIB_3D_HELPERS )

	// ========================================================================
	// Combine / lerp filters
	// ========================================================================

	//
	//	T2CompositeFVFFilter
	//

	template< typename FILTER_A, typename FILTER_B >
	class T2CompositeFVFFilter {

	private:

		FILTER_A m_FilterA;
		FILTER_B m_FilterB;

		T2CompositeFVFFilter(); // Hidden

	public:

		typedef typename FILTER_A::vertex_type vertex_type;

		T2CompositeFVFFilter( const FILTER_A & a, const FILTER_B & b ) : 
			m_FilterA( a ), m_FilterB( b ) {
			/* Empty */
		}

		vertex_type & operator()( vertex_type & vert ) {

			 return m_FilterB( m_FilterA( vert ) );

		}

	};

	//
	//	T3CompositeFVFFilter
	//

	template< typename FILTER_A, typename FILTER_B, typename FILTER_C >
	class T3CompositeFVFFilter {

	private:

		FILTER_A m_FilterA;
		FILTER_B m_FilterB;
		FILTER_C m_FilterC;

		T3CompositeFVFFilter(); // Hidden

	public:

		typedef typename FILTER_A::vertex_type vertex_type;

		T3CompositeFVFFilter( const FILTER_A & a, const FILTER_B & b, const FILTER_C & c ) : 
			m_FilterA( a ), m_FilterB( b ), m_FilterC( c ) {
			/* Empty */
		}

		vertex_type & operator()( vertex_type & vert ) {

			 return m_FilterC( m_FilterB( m_FilterA( vert ) ) );

		}

	};

	//
	//	TLERPFVFFilters
	//

	template< typename FILTER_A, typename FILTER_B, typename LERP >
	class TLERPFVFFilters {

	private:

		FILTER_A m_FilterA;
		FILTER_B m_FilterB;

		float m_T;

		TLERPFVFFilters(); // Hidden

	public:

		typedef typename FILTER_A::vertex_type vertex_type;

		TLERPFVFFilters( const FILTER_A & a, const FILTER_B & b, const float t = 0.0f ) : 
			m_FilterA( a ), m_FilterB( b ), m_T( t ) {
			/* Empty */
		}

		void SetT( const float t ) {

			m_T = t;

		}

		vertex_type & operator()( vertex_type & vert ) {

			// Perform both filters

			vertex_type a( vert );
			vertex_type b( vert );

			// Setup the LERP object

			LERP lerp( m_FilterA( a ), m_FilterB( b ) );

			lerp.Postion( vert, m_T );

			return vert;

		}

	};

	// ========================================================================

	//
	//	TNOP_VertexFilter
	//

	template< typename FVF >
	class TNOP_VertexFilter {

	public:

		typedef FVF vertex_type;

		TNOP_VertexFilter() { /* Empty */ }

		FVF & operator()( FVF & vert ) { return vert; }

	};
	
	// ========================================================================
	// 2D Filters
	// ========================================================================

	//
	//	TMULXY_VertexFilter
	//

	template< typename FVF >
	class TMULXY_VertexFilter {

	private:

		TMULXY_VertexFilter(); // Hidden

		float m_XMul;
		float m_YMul;

	public:

		typedef FVF vertex_type;

		TMULXY_VertexFilter( const float x, const float y ) : m_XMul( x ), m_YMul( y ) {
			/* Empty */
		}

		FVF & operator()( FVF & vert ) {

			vert.x *= m_XMul;
			vert.y *= m_YMul;

			return vert;

		}

	};

	//
	//	TMULXYADDDXDY_VertexFilter
	//

	template< typename FVF >
	class TMULXYADDDXDY_VertexFilter {

	private:

		TMULXYADDDXDY_VertexFilter(); // Hidden

		float m_XMul;
		float m_YMul;
		float m_DX;
		float m_DY;

	public:

		typedef FVF vertex_type;

		TMULXYADDDXDY_VertexFilter( const float x, const float y, const float dx, const float dy ) : 
			m_XMul( x ), m_YMul( y ), m_DX( dx ), m_DY( dy ) {
			/* Empty */
		}

		FVF & operator()( FVF & vert ) {

			vert.x = (vert.x * m_XMul) + m_DX;
			vert.y = (vert.y * m_YMul) + m_DY;

			return vert;

		}

	};
	
	//
	//	TTranslateAndScale_VertexFilter
	//

	template< typename FVF >
	class TTranslateAndScale_VertexFilter {

	private:

		TTranslateAndScale_VertexFilter(); // Hidden

		float m_XMul;
		float m_YMul;
		float m_ZMul;
		float m_DX;
		float m_DY;
		float m_DZ;

	public:

		typedef FVF vertex_type;

		TTranslateAndScale_VertexFilter(
			const float x
			,const float y
			,const float z
			,const float dx
			,const float dy
			,const float dz
		) : 
			m_XMul( x )
			,m_YMul( y )
			,m_ZMul( z )
			,m_DX( dx )
			,m_DY( dy )
			,m_DZ( dz )
		{
			/* Empty */
		}

		FVF & operator()( FVF & vert ) {

			vert.x = (vert.x * m_XMul) + m_DX;
			vert.y = (vert.y * m_YMul) + m_DY;
			vert.z = (vert.z * m_ZMul) + m_DZ;

			return vert;

		}

	};
	//
	//	T2dSpheroid_VertexFilter
	//
	//	Link: http://mathworld.wolfram.com/Spheroid.html
	//

	template< typename FVF >
	class T2dSpheroid_VertexFilter {

	private:

		T2dSpheroid_VertexFilter(); // Hidden

		float m_X;
		float m_Y;
		float m_R;

	public:

		typedef FVF vertex_type;

		T2dSpheroid_VertexFilter( const float x, const float y, const float r ) : 
			m_X( x ), m_Y( y ), m_R( r ) {
			/* Empty */
		}

		FVF & operator()( FVF & vert ) {

			float u = ((vert.x) * g_2_PI) - g_PI;
			float v = ((vert.y) * g_PI) - (g_PI / 2.0f);

			vert.x = m_X + m_R * (sinf(u) * sinf(v));
			vert.y = m_Y + m_R * cosf(v);
			vert.z = m_R * (cosf(u) * sinf(v));

			return vert;

		}

	};

	// ========================================================================
	// 'Ellipsoid' and related Filters
	// ========================================================================

	//
	//	TEllipsoid_VertexFilter
	//
	//	Link: http://mathworld.wolfram.com/Ellipsoid.html
	//

	template< typename FVF >
	class TEllipsoid_VertexFilter {

	private:

		TEllipsoid_VertexFilter(); // Hidden

		float m_X;
		float m_Y;
		float m_Z;

		float m_A;
		float m_B;
		float m_C;

	public:

		typedef FVF vertex_type;

		TEllipsoid_VertexFilter( 
			const float x, const float y, const float z,
			const float a, const float b, const float c ) : 
			m_X( x ), m_Y( y ), m_Z( z ), m_A( a ), m_B( b ), m_C( c ) {
			/* Empty */
		}

		FVF & operator()( FVF & vert ) {

			// This filter still needs to be tested.

			float u = ((vert.x) * g_PI) - g_PI_DIV_2;

#if defined(BPTLIB_2D_FRIENDLY)
			float v = ((vert.y) * g_2_PI) - g_PI;
#else
			float v = ((vert.y) * g_2_PI) + g_PI;
#endif
			float z = (1.0f + vert.z);

#if defined(BPTLIB_2D_FRIENDLY)

			// 2d friendly

			vert.x = m_X + (m_A * z) * (sinf(u) * sinf(v));
			vert.y = m_Y + (m_B * z) * cosf(v);
			vert.z = m_Z + (m_C * z) * (cosf(u) * sinf(v));

#else

			vert.x = m_X + (m_A * z) * (cosf(u) * sinf(v));
			vert.y = m_Y + (m_B * z) * (sinf(u) * sinf(v));
			vert.z = m_Z + (m_C * z) * cosf(v);

#endif

			return vert;

		}

	};

	//
	//	TEllipsoid_NormalFilter
	//
	//	Link: http://mathworld.wolfram.com/Ellipsoid.html
	//

	template< typename FVF >
	class TEllipsoid_NormalFilter {

	private:

		TEllipsoid_NormalFilter(); // Hidden

		float m_X;
		float m_Y;
		float m_Z;

		float m_A;
		float m_B;
		float m_C;

	public:

		typedef FVF vertex_type;

		TEllipsoid_NormalFilter( 
			const float x, const float y, const float z,
			const float a, const float b, const float c ) : 
			m_X( x ), m_Y( y ), m_Z( z ), m_A( a ), m_B( b ), m_C( c ) {
			/* Empty */
		}

		FVF & operator()( FVF & vert ) {

			// This filter still needs to be tested.

			float u = ((vert.x) * g_PI) - g_PI_DIV_2;

#if defined(BPTLIB_2D_FRIENDLY)
			float v = ((vert.y) * g_2_PI) - g_PI;
#else
			float v = ((vert.y) * g_2_PI) + g_PI;
#endif

			float z = (1.0f + vert.z);
			float az = (m_A * z);
			float bz = (m_B * z);
			float cz = (m_C * z);

			// check for division by 0 and return a pure z normal

			if ( (0.0f == az) || (0.0f == bz) || (0.0f == cz) ) {

				vert.nx = 0.0f;
				vert.ny = 0.0f;
				vert.nz = 1.0f;

				return vert;

			}

			// THIS NEEDS TO BE VALIDATED!!!
			// THIS NEEDS TO BE VALIDATED!!!
			// Finally get down to business and calculate the normal
			// THIS NEEDS TO BE VALIDATED!!!
			// THIS NEEDS TO BE VALIDATED!!!

#if defined(BPTLIB_2D_FRIENDLY)

			// 2d friendly

			vert.nx = (sinf(u) * sinf(v)) / az;
			vert.ny = cosf(v) / bz;
			vert.nz = (cosf(u) * sinf(v)) / cz;

#else

			vert.nx = (cosf(u) * sinf(v)) / az;
			vert.ny = (sinf(u) * sinf(v)) / bz;
			vert.nz = cosf(v) / cz;

#endif

			return vert;

		}

	};

	// ========================================================================
	// 'SuperEllipsoid' filters
	// ========================================================================

	//
	//	TSuperEllipsoid_VertexFilter
	//
	//	Link: http://www.swin.edu.au/astronomy/pbourke//modelling/superellipse/
	//	Link: http://mathworld.wolfram.com/Superellipsoid.html
	//

	template< typename FVF >
	class TSuperEllipsoid_VertexFilter {

	private:

		TSuperEllipsoid_VertexFilter(); // Hidden

		float m_X;
		float m_Y;
		float m_Z;

		float m_A;
		float m_B;
		float m_C;

		float m_N1;
		float m_N2;

	private:

		// These really should be part of a useful math utility class

		float signf( const float s ) {

			return (s < 0.0f) ? -1.0f : 1.0f;

		}

		float cosN( const float s, const float p ) {

			float v = cosf( s );

			return powf( fabsf(v), p ) * signf( v );

		}

		float sinN( const float s, const float p ) {

			float v = sinf( s );

			return powf( fabsf(v), p ) * signf( v );

		}

	public:

		typedef FVF vertex_type;

		TSuperEllipsoid_VertexFilter( 
			const float x, const float y, const float z,
			const float a, const float b, const float c,
			const float n1, const float n2 ) : 
			m_X( x ), m_Y( y ), m_Z( z ), m_A( a ), m_B( b ), m_C( c ), m_N1( n1 ), m_N2( n2 ) {
			/* Empty */
		}

		FVF & operator()( FVF & vert ) {

			// This filter still needs to be tested.

			float u = ((vert.y) * g_PI) - (g_PI / 2.0f);
			float v = ((vert.x) * g_2_PI) - g_PI;

			float cosn1u = cosN( u, m_N1 );
			float cosn2v = cosN( v, m_N2 );
			float sinn1u = sinN( u, m_N1 );
			float sinn2v = sinN( v, m_N2 );

			float z = (1.0f + vert.z);

#if defined(BPTLIB_2D_FRIENDLY)

			// 2d friendly

			vert.z = m_Z + (m_A * z) * (cosn1u * cosn2v);
			vert.x = m_X + (m_B * z) * (cosn1u * sinn2v);
			vert.y = m_Y + (m_C * z) * (sinn1u);

#else

			vert.x = m_X + (m_A * z) * (cosn1u * cosn2v);
			vert.y = m_Y + (m_B * z) * (cosn1u * sinn2v);
			vert.z = m_Z + (m_C * z) * (sinn1u);

#endif

			return vert;

		}

	};

	//
	//	TSuperEllipsoid_NormalFilter
	//
	//	Link: http://www.swin.edu.au/astronomy/pbourke//modelling/superellipse/
	//	Link: http://mathworld.wolfram.com/Superellipsoid.html
	//

	template< typename FVF >
	class TSuperEllipsoid_NormalFilter {

	private:

		TSuperEllipsoid_NormalFilter(); // Hidden

		float m_X;
		float m_Y;
		float m_Z;

		float m_A;
		float m_B;
		float m_C;

		float m_N1;
		float m_N2;

	private:

		// These really should be part of a useful math utility class

		float signf( const float s ) {

			return (s < 0.0f) ? -1.0f : 1.0f;

		}

		float cosN( const float s, const float p ) {

			float v = cosf( s );

			return powf( fabsf(v), p ) * signf( v );

		}

		float sinN( const float s, const float p ) {

			float v = sinf( s );

			return powf( fabsf(v), p ) * signf( v );

		}

	public:

		typedef FVF vertex_type;

		TSuperEllipsoid_NormalFilter( 
			const float x, const float y, const float z,
			const float a, const float b, const float c,
			const float n1, const float n2 ) : 
			m_X( x ), m_Y( y ), m_Z( z ), m_A( a ), m_B( b ), m_C( c ), m_N1( n1 ), m_N2( n2 ) {
			/* Empty */
		}

		FVF & operator()( FVF & vert ) {

			// This filter still needs to be tested.

			float u = ((vert.y) * g_PI) - (g_PI / 2.0f);
			float v = ((vert.x) * g_2_PI) - g_PI;

			float cos2mn1u = cosN( u, 2.0f - m_N1 );
			float cos2mn2v = cosN( v, 2.0f - m_N2 );
			float sin2mn1u = sinN( u, 2.0f - m_N1 );
			float sin2mn2v = sinN( v, 2.0f - m_N2 );

			float z = (1.0f + vert.z);
			float az = (m_A * z);
			float bz = (m_B * z);
			float cz = (m_C * z);

			// check for division by 0 and return a pure z normal

			if ( (0.0f == az) || (0.0f == bz) || (0.0f == cz) ) {

				vert.nx = 0.0f;
				vert.ny = 0.0f;
				vert.nz = 1.0f;

				return vert;

			}

			// THIS NEEDS TO BE VALIDATED!!!
			// THIS NEEDS TO BE VALIDATED!!!
			// Finally get down to business and calculate the normal
			// THIS NEEDS TO BE VALIDATED!!!
			// THIS NEEDS TO BE VALIDATED!!!

#if defined(BPTLIB_2D_FRIENDLY)

			// 2d friendly

			vert.nz = (cos2mn1u * cos2mn2v) / az;
			vert.nx = (cos2mn1u * sin2mn2v) / bz;
			vert.ny = (sin2mn1u) / cz;

#else

			vert.nx = (cos2mn1u * cos2mn2v) / az;
			vert.ny = (cos2mn1u * sin2mn2v) / bz;
			vert.nz = (sin2mn1u) / cz;

#endif

			return vert;

		}

	};

	// ========================================================================
	// Silly filters
	// ========================================================================

	//
	//	TSillyJitter_VertexFilter
	//

	template< typename FVF >
	class TSillyJitter_VertexFilter {

	private:

		float m_X;
		float m_Y;
		float m_Z;

		TSillyJitter_VertexFilter(); // Hidden

	public:

		typedef FVF vertex_type;

		TSillyJitter_VertexFilter( const float x, const float y, const float z ) : 
			 m_X( x / (float)RAND_MAX ) 
			,m_Y( y / (float)RAND_MAX )
			,m_Z( z / (float)RAND_MAX ) {
			/* Empty */
		}

		FVF & operator()( FVF & vert ) {

			vert.x += (float)rand() * m_X;
			vert.y += (float)rand() * m_Y;
			vert.z += (float)rand() * m_Z;

			return vert;

		}

	};

	//
	//	TSillyWave_VertexFilter
	//

	template< typename FVF >
	class TSillyWave_VertexFilter {

	private:

		float m_X;
		float m_Y;
		float m_Z;
		float m_T;

		TSillyWave_VertexFilter(); // Hidden

	public:

		typedef FVF vertex_type;

		TSillyWave_VertexFilter( const float x, const float y, const float z, const float t ) : 
			 m_X( x ) 
			,m_Y( y )
			,m_Z( z )
			,m_T( t )
		{
			/* Empty */
		}

		FVF & operator()( FVF & vert ) {

			float k = (vert.x + vert.y + vert.z) * m_T;

			vert.x += sinf( k ) * m_X;
			vert.y += cosf( k ) * m_Y;
			vert.z += sinf( k ) * m_Z;

			return vert;

		}

	};

	// ========================================================================
	// D3D relient filters
	// ========================================================================

#if defined( BPTLIB_SUPPORT_DX7 )

	//
	//	TD3DMATRIX_VertexFilter
	//

	template< typename FVF >
	class TD3DMATRIX_VertexFilter {

	private:

		D3DMATRIX m_Mat;

		TD3DMATRIX_VertexFilter(); // Hidden

	public:

		typedef FVF vertex_type;

		TD3DMATRIX_VertexFilter( const D3DMATRIX & mat ) : m_Matrix( mat ) {
			/* Empty */
		}

		FVF & operator()( FVF & vert ) {

			// SIMD!!!!

		    float x = (vert.x*m_Mat._11) + (vert.y*m_Mat._21) + (vert.z*m_Mat._31) + m_Mat._41;
		    float y = (vert.x*m_Mat._12) + (vert.y*m_Mat._22) + (vert.z*m_Mat._32) + m_Mat._42;
		    float z = (vert.x*m_Mat._13) + (vert.y*m_Mat._23) + (vert.z*m_Mat._33) + m_Mat._43;
		    float w = (vert.x*m_Mat._14) + (vert.y*m_Mat._24) + (vert.z*m_Mat._34) + m_Mat._44;

#if defined( D3DMATH_H )

			if( fabs( w ) < g_EPSILON ) {
	
		        return vert;
	
			}

#endif // defined( D3DMATH_H )

			float oow = 1.0f / w;

			vert.x = x * oow;
			vert.y = y * oow;
			vert.z = z * oow;

			return vert;

		}

	};

#endif // defined( BPTLIB_SUPPORT_DX7 )

	// ========================================================================
	// ========================================================================

#endif // defined( BPTLIB_3D_HELPERS )

}; // namespace BPT

#endif // !defined(AFX_TFVFFILTERS_H__684847A2_264F_4B02_9730_84E35A20A976__INCLUDED_)
