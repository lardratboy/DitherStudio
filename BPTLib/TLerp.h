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

#if !defined(AFX_TLERP_H__0D291987_4105_4D5D_96C4_D6D2AE8BB812__INCLUDED_)
#define AFX_TLERP_H__0D291987_4105_4D5D_96C4_D6D2AE8BB812__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ----------------------------------------------------------------------------

namespace BPT {

#if defined( BPTLIB_3D_HELPERS )

	// ========================================================================
	// 'Lerp' helper functions
	// ========================================================================

	//
	//	TNOP_Lerp
	//

	template< typename T >
	class TNOP_Lerp {

	public:

		typedef T type_to_lerp;

	private:

		TNOP_Lerp(); // Hidden

	public:

		TNOP_Lerp( const T & a, const T & b ) { /* Empty */ }
		T & Position( T & out, const float t ) { return out; }

	};

	//
	//	TXY_Lerp
	//

	template< typename T >
	class TXY_Lerp {

	public:

		typedef T type_to_lerp;

	private:

		float x;
		float y;

		float dx;
		float dy;

		TXY_Lerp(); // Hidden

	public:

		TXY_Lerp( const T & a, const T & b ) : 
			x( a.x ), y( a.y ),
			dx( b.x - a.x ), dy( b.y - a.y ) {
			/* Empty */
		}

		T & Position( T & out, const float t ) {

			out.x = (x + (dx * t));
			out.y = (y + (dy * t));

			return out;

		}

	};

	//
	//	TXYZ_Lerp
	//

	template< typename T >
	class TXYZ_Lerp {

	public:

		typedef T type_to_lerp;

	private:

		float x;
		float y;
		float z;

		float dx;
		float dy;
		float dz;

		TXYZ_Lerp(); // Hidden

	public:

		TXYZ_Lerp( const T & a, const T & b ) : 
			x( a.x ), y( a.y ), z( a.z ),
			dx( b.x - a.x ), dy( b.y - a.y ), dz( b.z - a.z ) {
			/* Empty */
		}

		T & Position( T & out, const float t ) {

			out.x = (x + (dx * t));
			out.y = (y + (dy * t));
			out.z = (z + (dz * t));

			return out;

		}

	};

	//
	//	TNORMAL_Lerp
	//

	template< typename T >
	class TNORMAL_Lerp {

	public:

		typedef T type_to_lerp;

	private:

		float nx;
		float ny;
		float nz;

		float dnx;
		float dny;
		float dnz;

		TNORMAL_Lerp(); // Hidden

	public:

		TNORMAL_Lerp( const T & a, const T & b ) : 
			nx( a.nx ), ny( a.ny ), nz( a.nz ),
			dnx( b.nx - a.nx ), dny( b.ny - a.ny ), dnz( b.nz - a.nz ) {
			/* Empty */
		}

		T & Position( T & out, const float t ) {

			out.nx = (nx + (dnx * t));
			out.ny = (ny + (dny * t));
			out.ny = (nz + (dnz * t));

			return out;

		}

	};

	//
	//	TTEX1_Lerp
	//

	template< typename T >
	class TTEX1_Lerp {

	public:

		typedef T type_to_lerp;

	private:

		float tu1;
		float tv1;

		float dtu1;
		float dtv1;

		TTEX1_Lerp(); // Hidden

	public:

		TTEX1_Lerp( const T & a, const T & b ) : 
			tu1( a.tu1 ), tv1( a.tv1 ), dtu1( b.tu1 - a.tu1 ), dtv1( b.tv1 - a.tv1 ) {
			/* Empty */
		}

		T & Position( T & out, const float t ) {

			out.tu1 = (tu1 + (dtu1 * t));
			out.tv1 = (tv1 + (dtv1 * t));

			return out;

		}

	};

	//
	//	TTEX2_Lerp
	//

	template< typename T >
	class TTEX2_Lerp {

	public:

		typedef T type_to_lerp;

	private:

		float tu1;
		float tv1;
		float tu2;
		float tv2;

		float dtu1;
		float dtv1;
		float dtu2;
		float dtv2;

		TTEX2_Lerp(); // Hidden

	public:

		TTEX2_Lerp( const T & a, const T & b ) : 
			tu1( a.tu1 ), tv1( a.tv1 ), tu2( a.tu2 ), tv2( a.tv2 ), 
			dtu1( b.tu1 - a.tu1 ), dtv1( b.tv1 - a.tv1 ),
			dtu2( b.tu2 - a.tu2 ), dtv1( b.tv2 - a.tv2 ) {
			/* Empty */
		}

		T & Position( T & out, const float t ) {

			out.tu1 = (tu1 + (dtu1 * t));
			out.tv1 = (tv1 + (dtv1 * t));
			out.tu2 = (tu2 + (dtu2 * t));
			out.tv2 = (tv2 + (dtv2 * t));

			return out;

		}

	};

	// ========================================================================
	// Color Lerp helpers
	// ========================================================================

	//
	//	TDIFFUSE_Lerp
	//

	template< typename T >
	class TDIFFUSE_Lerp {

	public:

		typedef T type_to_lerp;

	private:

		TDIFFUSE_Lerp(); // Hidden

	public:

		TDIFFUSE_Lerp( const T & a, const T & b ) { /* Empty */ }

		T & Position( T & out, const float t ) {

			return out;

		}

	};

	//
	//	TSPECULAR_Lerp
	//

	template< typename T >
	class TSPECULAR_Lerp {

	public:

		typedef T type_to_lerp;

	private:

		TSPECULAR_Lerp(); // Hidden

	public:

		TSPECULAR_Lerp( const T & a, const T & b ) { /* Empty */ }

		T & Position( T & out, const float t ) {

			return out;

		}

	};

	// ========================================================================
	//  Lerp helpers continued.
	// ========================================================================

	//
	//	TLerp2
	//	

	template< typename T, typename LERP1, typename LERP2 >
	class TLerp2 {

	public:

		typedef T type_to_lerp;

	private:

		LERP1 m_Lerp1;
		LERP2 m_Lerp2;

		TLerp2(); // Hidden

	public:

		TLerp2( const T & a, const T & b ) : m_Lerp1( a, b ), m_Lerp2( a, b ) {
			/* Empty */
		}

		T & Position( T & out, const float t ) {

			m_Lerp1.Position( out, t );
			m_Lerp2.Position( out, t );

			return out;

		}
	};

	//
	//	TLerp3
	//	

	template< typename T, typename LERP1, typename LERP2, typename LERP3 >
	class TLerp3 {

	public:

		typedef T type_to_lerp;

	private:

		LERP1 m_Lerp1;
		LERP2 m_Lerp2;
		LERP3 m_Lerp3;

		TLerp3(); // Hidden

	public:

		TLerp3( const T & a, const T & b ) :
			m_Lerp1( a, b ), m_Lerp2( a, b ), m_Lerp3( a, b ) {
			/* Empty */
		}

		T & Position( T & out, const float t ) {

			m_Lerp1.Position( out, t );
			m_Lerp2.Position( out, t );
			m_Lerp3.Position( out, t );

			return out;

		}
	};

	//
	//	TLerp4
	//	

	template< typename T, typename LERP1, typename LERP2, typename LERP3, typename LERP4 >
	class TLerp4 {

	public:

		typedef T type_to_lerp;

	private:

		LERP1 m_Lerp1;
		LERP2 m_Lerp2;
		LERP3 m_Lerp3;
		LERP4 m_Lerp4;

		TLerp4(); // Hidden

	public:

		TLerp4( const T & a, const T & b ) :
			m_Lerp1( a, b ), m_Lerp2( a, b ), m_Lerp3( a, b ), m_Lerp4( a, b ) {
			/* Empty */
		}

		T & Position( T & out, const float t ) {

			m_Lerp1.Position( out, t );
			m_Lerp2.Position( out, t );
			m_Lerp3.Position( out, t );
			m_Lerp4.Position( out, t );

			return out;

		}
	};

	// ========================================================================
	// Common Lerp functions
	// ========================================================================

	//
	//	TXYZTEX1_Lerp
	//

	template< typename T >
	class TXYZTEX1_Lerp : public TLerp2< T, TXYZ_Lerp<T>, TTEX1_Lerp<T> > {

	public:

		typedef T type_to_lerp;

	private:

		typedef TLerp2< T, TXYZ_Lerp<T>, TTEX1_Lerp<T> > base_type;

		TXYZTEX1_Lerp(); // Hidden

	public:

		TXYZTEX1_Lerp( const T & a, const T & b ) : base_type( a, b ) {
			/* Empty */
		}

	};

	//
	//	TXYZTEX2_Lerp
	//

	template< typename T >
	class TXYZTEX2_Lerp : public TLerp2< T, TXYZ_Lerp<T>, TTEX2_Lerp<T> > {

	public:

		typedef T type_to_lerp;

	private:

		typedef TLerp2< T, TXYZ_Lerp<T>, TTEX2_Lerp<T> > base_type;

		TXYZTEX2_Lerp(); // Hidden

	public:

		TXYZTEX2_Lerp( const T & a, const T & b ) : base_type( a, b ) {
			/* Empty */
		}

	};

	//
	//	TXYDIFFUSETEX1_Lerp
	//

	template< typename T >
	class TXYDIFFUSETEX1_Lerp : public TLerp3< T, TXY_Lerp<T>, TDIFFUSE_Lerp<T>, TTEX1_Lerp<T> > {

	public:

		typedef T type_to_lerp;

	private:

		typedef TLerp3< T, TXY_Lerp<T>, TDIFFUSE_Lerp<T>, TTEX1_Lerp<T> > base_type;

		TXYDIFFUSETEX1_Lerp(); // Hidden

	public:

		TXYDIFFUSETEX1_Lerp( const T & a, const T & b ) : base_type( a, b ) {
			/* Empty */
		}

	};

	//
	//	TXYDIFFUSE_Lerp
	//

	template< typename T >
	class TXYDIFFUSE_Lerp : public TLerp2< T, TXY_Lerp<T>, TDIFFUSE_Lerp<T> > {

	public:

		typedef T type_to_lerp;

	private:

		typedef TLerp2< T, TXY_Lerp<T>, TDIFFUSE_Lerp<T> > base_type;

		TXYDIFFUSE_Lerp(); // Hidden

	public:

		TXYDIFFUSE_Lerp( const T & a, const T & b ) : base_type( a, b ) {
			/* Empty */
		}

	};

	//
	//	TXYTEX1_Lerp
	//

	template< typename T >
	class TXYTEX1_Lerp : public TLerp2< T, TXY_Lerp<T>, TTEX1_Lerp<T> > {

	public:

		typedef T type_to_lerp;

	private:

		typedef TLerp2< T, TXY_Lerp<T>, TTEX1_Lerp<T> > base_type;

		TXYTEX1_Lerp(); // Hidden

	public:

		TXYTEX1_Lerp( const T & a, const T & b ) : base_type( a, b ) {
			/* Empty */
		}

	};

#endif // defined( BPTLIB_3D_HELPERS )

}; // namespace BPT

#endif // !defined(AFX_TLERP_H__0D291987_4105_4D5D_96C4_D6D2AE8BB812__INCLUDED_)
