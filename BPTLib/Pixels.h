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

#if !defined(AFX_PIXELS_H__E965F008_DA1F_4D8A_90FA_681A85832F25__INCLUDED_)
#define AFX_PIXELS_H__E965F008_DA1F_4D8A_90FA_681A85832F25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cstdint>
#include <utility>

#define BPTLIB_DEFINE_SIMPLE_CHANNEL_ACCESS_METHODS
#define BPTLIB_INCLUDE_PIXELTYPE_COMPARISON_OPERATORS

namespace BPT {

// ----------------------------------------------------------------------------
#pragma pack( push, PIXEL_packing, 1 )
// ----------------------------------------------------------------------------

	// ========================================================================

	typedef std::int8_t  S8;
	typedef std::int16_t S16;
	typedef std::int32_t S32;
	typedef std::int64_t S64;

	typedef std::uint8_t  U8;
	typedef std::uint16_t U16;
	typedef std::uint32_t U32;
	typedef std::uint64_t U64;

	// ========================================================================
	// Pixel specialization tag's
	// ========================================================================

	struct xxx_tag { enum : unsigned { value = 0 }; };
	struct P8__tag { enum : unsigned { value = 1 }; };
	struct P16_tag { enum : unsigned { value = 2 }; };
	struct P24_tag { enum : unsigned { value = 3 }; };
	struct P32_tag { enum : unsigned { value = 4 }; };
	struct P64_tag { enum : unsigned { value = 8 }; };

	// ========================================================================
	// something common to all the POD types ???
	// ========================================================================

	template<typename T>
	struct TOperators {
		// this would be a natural place for what ????
	};

//#define BPTPIXELS_COMMON_ROOT_OPERATORS_MACRO(XXXX) /*NOP*/
#define BPTPIXELS_COMMON_ROOT_OPERATORS_MACRO(XXXX) XXXX

	// ========================================================================
	// RGB
	// ========================================================================

	//
	//	TRGB_root
	//

	template<
		typename VALUE_TYPE, 
		const int R_CHANNEL_BITS, 
		const int G_CHANNEL_BITS, 
		const int B_CHANNEL_BITS
	>
	struct TRGB_root : public TOperators<TRGB_root<VALUE_TYPE, R_CHANNEL_BITS, G_CHANNEL_BITS, B_CHANNEL_BITS>> {

		typedef VALUE_TYPE value_type;

		enum : value_type {

			A_BitCount		= 0,
			R_BitCount		= R_CHANNEL_BITS,
			G_BitCount		= G_CHANNEL_BITS,
			B_BitCount		= B_CHANNEL_BITS

		};

		// Data ---------------------------------------------------------------

		union {

			VALUE_TYPE value;

			struct {

				VALUE_TYPE	b:B_CHANNEL_BITS;
				VALUE_TYPE	g:G_CHANNEL_BITS;
				VALUE_TYPE	r:R_CHANNEL_BITS;

			};

		};

		// Alpha helper -------------------------------------------------------

		int _A() const {

			return 0;

		}

		void _A( const int value ) { /* Empty */ }

		int _R() const {

			return r;

		}

		void _R( const int value ) {

			r = value;

		}

		int _G() const {

			return g;

		}

		void _G( const int value ) {

			g = value;

		}

		int _B() const {

			return b;

		}

		void _B( const int value ) {

			b = value;

		}

	};

	// ========================================================================
	// BGR
	// ========================================================================

	//
	//	TBGR_root
	//

	template<
		typename VALUE_TYPE,
		const int R_CHANNEL_BITS, 
		const int G_CHANNEL_BITS, 
		const int B_CHANNEL_BITS
	>
	struct TBGR_root : public TOperators<TBGR_root<VALUE_TYPE, R_CHANNEL_BITS, G_CHANNEL_BITS, B_CHANNEL_BITS>> {

		typedef VALUE_TYPE value_type;

		enum : value_type {

			A_BitCount		= 0,
			R_BitCount		= R_CHANNEL_BITS,
			G_BitCount		= G_CHANNEL_BITS,
			B_BitCount		= B_CHANNEL_BITS

		};

		// Data ---------------------------------------------------------------

		union {

			VALUE_TYPE value;

			struct {

				VALUE_TYPE	r:R_CHANNEL_BITS;
				VALUE_TYPE	g:G_CHANNEL_BITS;
				VALUE_TYPE	b:B_CHANNEL_BITS;

			};

		};

		// Alpha helper -------------------------------------------------------

		int _A() const {

			return 0;

		}

		void _A( const int value ) { /* Empty */ }

		int _R() const {

			return r;

		}

		void _R( const int value ) {

			r = value;

		}

		int _G() const {

			return g;

		}

		void _G( const int value ) {

			g = value;

		}

		int _B() const {

			return b;

		}

		void _B( const int value ) {

			b = value;

		}

	};

	// ========================================================================
	// ARGB
	// ========================================================================

	//
	//	TARGB_root
	//

	template<
		typename VALUE_TYPE, 
		const int A_CHANNEL_BITS, 
		const int R_CHANNEL_BITS, 
		const int G_CHANNEL_BITS, 
		const int B_CHANNEL_BITS
	>
	struct TARGB_root : public TOperators<TARGB_root<VALUE_TYPE, A_CHANNEL_BITS, R_CHANNEL_BITS, G_CHANNEL_BITS, B_CHANNEL_BITS>> {

		typedef VALUE_TYPE value_type;

		enum : value_type {

			A_BitCount		= A_CHANNEL_BITS,
			R_BitCount		= R_CHANNEL_BITS,
			G_BitCount		= G_CHANNEL_BITS,
			B_BitCount		= B_CHANNEL_BITS

		};

		// Data ---------------------------------------------------------------

		union {

			VALUE_TYPE value;

			struct {

				VALUE_TYPE	b:B_CHANNEL_BITS;
				VALUE_TYPE	g:G_CHANNEL_BITS;
				VALUE_TYPE	r:R_CHANNEL_BITS;
				VALUE_TYPE	a:A_CHANNEL_BITS;

			};

		};

		// Alpha helper -------------------------------------------------------

		int _A() const {

			return a;

		}

		void _A( const int v ) {

			a = v;

		}

		int _R() const {

			return r;

		}

		void _R( const int v ) {

			r = v;

		}

		int _G() const {

			return g;

		}

		void _G( const int v ) {

			g = v;

		}

		int _B() const {

			return b;

		}

		void _B( const int v ) {

			b = v;

		}

	};

	// ========================================================================
	// BGRA
	// ========================================================================

	//
	//	TBGRA_root
	//

	template<
		typename VALUE_TYPE, 
		const int A_CHANNEL_BITS, 
		const int R_CHANNEL_BITS, 
		const int G_CHANNEL_BITS, 
		const int B_CHANNEL_BITS
	>
	struct TBGRA_root : public TOperators<TBGRA_root<VALUE_TYPE, A_CHANNEL_BITS, R_CHANNEL_BITS, G_CHANNEL_BITS, B_CHANNEL_BITS>> {

		typedef VALUE_TYPE value_type;

		enum : value_type {

			A_BitCount		= A_CHANNEL_BITS,
			R_BitCount		= R_CHANNEL_BITS,
			G_BitCount		= G_CHANNEL_BITS,
			B_BitCount		= B_CHANNEL_BITS

		};

		// Data ---------------------------------------------------------------

		union {

			VALUE_TYPE value;

			struct {

				VALUE_TYPE	a:A_CHANNEL_BITS;
				VALUE_TYPE	r:R_CHANNEL_BITS;
				VALUE_TYPE	g:G_CHANNEL_BITS;
				VALUE_TYPE	b:B_CHANNEL_BITS;

			};

		};

		// Channel helpers ----------------------------------------------------

		int _A() const {

			return a;

		}

		void _A( const int value ) {

			a = value;

		}

		int _R() const {

			return r;

		}

		void _R( const int value ) {

			r = value;

		}

		int _G() const {

			return g;

		}

		void _G( const int value ) {

			g = value;

		}

		int _B() const {

			return b;

		}

		void _B( const int value ) {

			b = value;

		}

	};

	// ------------------------------------------------------------------------

	//
	//	TODDEVENHelper
	//

	template< const int am, const int rm, const int gm, const int bm >
	struct TOEH {
		enum : unsigned { a = am, r = rm, g = gm, b = bm };
	};

	// ------------------------------------------------------------------------

#define BPTBIG(a,b) ((a)>(b)?(a):(b))

	//
	//	TPIXEL_data
	//

	template<
		typename RT
		,const int ALS 
		,const int RLS 
		,const int GLS 
		,const int BLS
		//,typename OM = TOEH<0,1,0,1>
		//,typename EM = TOEH<1,0,1,0>
		,const int OA = 8 
		,const int OR = 8 
		,const int OG = 8
		,const int OB = 8
	>
	struct TPIXEL_data : public RT {

		// pixel traits  ------------------------------------------------------

		typedef RT base_type;

		enum : typename RT::value_type {

			Bits				= (sizeof(RT) * 8)
			,ActiveBits			= (RT::A_BitCount + RT::R_BitCount + RT::G_BitCount + RT::B_BitCount)
			,A_LimitedMax		= (1 << RT::A_BitCount) - 1
			,R_LimitedMax		= (1 << RT::R_BitCount) - 1
			,G_LimitedMax		= (1 << RT::G_BitCount) - 1
			,B_LimitedMax		= (1 << RT::B_BitCount) - 1
			,A_L_Shift			= ALS
			,R_L_Shift			= RLS
			,G_L_Shift			= GLS
			,B_L_Shift			= BLS
			,A_BitMask			= (A_LimitedMax << A_L_Shift)
			,R_BitMask			= (R_LimitedMax << R_L_Shift)
			,G_BitMask			= (G_LimitedMax << G_L_Shift)
			,B_BitMask			= (B_LimitedMax << B_L_Shift)

#if 0

			// this stuff never was fully fleshed out and it currently is causing large numbers of warnings

			,A_LoBitMask		= (1 << A_L_Shift)
			,R_LoBitMask		= (1 << R_L_Shift)
			,G_LoBitMask		= (1 << G_L_Shift)
			,B_LoBitMask		= (1 << B_L_Shift)
			,LoBitsMask			= (A_LoBitMask | R_LoBitMask | G_LoBitMask | B_LoBitMask)
			,A_HiBitMask		= (1 << (A_L_Shift + (RT::A_BitCount - 1)))
			,R_HiBitMask		= (1 << (R_L_Shift + (RT::R_BitCount - 1)))
			,G_HiBitMask		= (1 << (G_L_Shift + (RT::G_BitCount - 1)))
			,B_HiBitMask		= (1 << (B_L_Shift + (RT::B_BitCount - 1)))
			,HiBitsMask			= (A_HiBitMask | R_HiBitMask | G_HiBitMask | B_HiBitMask)
			,OddChannelsMask	= ((OM::a*A_BitMask)|(OM::r*R_BitMask)|(OM::g*G_BitMask)|(OM::b*B_BitMask))
			,EvenChannelsMask	= ((EM::a*A_BitMask)|(EM::r*R_BitMask)|(EM::g*G_BitMask)|(EM::b*B_BitMask))
			,OddHiBitsMask		= (HiBitsMask & OddChannelsMask)
			,EvenHiBitsMask		= (HiBitsMask & EvenChannelsMask)
			,A_StoredInEvenBits	= (A_BitMask & EvenChannelsMask)
			,R_StoredInEvenBits	= (R_BitMask & EvenChannelsMask)
			,G_StoredInEvenBits	= (G_BitMask & EvenChannelsMask)
			,B_StoredInEvenBits	= (B_BitMask & EvenChannelsMask)

#endif

			,LargestChannelSize = BPTBIG(RT::R_BitCount,BPTBIG(RT::G_BitCount, RT::B_BitCount))

		};

		// Read ---------------------------------------------------------------

		__forceinline int A() const {

			return _A() << (OA - RT::A_BitCount);

		}

		__forceinline int R() const {

			return _R() << (OR - RT::R_BitCount);

		}

		__forceinline int G() const {

			return _G() << (OG - RT::G_BitCount);

		}

		__forceinline int B() const {

			return _B() << (OB - RT::B_BitCount);

		}

		// Write --------------------------------------------------------------

		__forceinline void A( const int v ) {

			_A( v >> (OA - RT::A_BitCount) );

		}

		__forceinline void R( const int v ) {

			_R(v >> (OR - RT::R_BitCount));

		}

		__forceinline void G( const int v ) {

			_G(v >> (OG - RT::G_BitCount));

		}

		__forceinline void B( const int v ) {

			_B(v >> (OB - RT::B_BitCount));

		}

	};

	// ------------------------------------------------------------------------
	// ARGB Pixel data types
	// ------------------------------------------------------------------------

	typedef TARGB_root< U8,  5, 1, 1, 1 > A5R1G1B1_root;
	typedef TARGB_root< U8,  1, 2, 3, 2 > A1R2G3B2_root;
	typedef TARGB_root< U8,  2, 2, 2, 2 > A2R2G2B2_root;
	typedef TARGB_root< U16, 4, 4, 4, 4 > A4R4G4B4_root;
	typedef TARGB_root< U16, 1, 5, 5, 5 > A1R5G5B5_root;
	typedef TARGB_root< U32,  8, 8, 8, 8 > A8R8G8B8_root;
	typedef TARGB_root< U32, 6, 6, 6, 6 > A6R6G6B6_root;
	typedef TARGB_root< U32, 2, 10, 10, 10 > A2R10G10B10_root;
	typedef TARGB_root< U64, 16, 16, 16, 16 > A16R16G16B16_root;

	typedef TPIXEL_data< A5R1G1B1_root, 3,2,1,0 > A5R1G1B1_data;
	typedef TPIXEL_data< A1R2G3B2_root, 7,5,2,0 > A1R2G3B2_data;
	typedef TPIXEL_data< A2R2G2B2_root, 6,4,2,0 > A2R2G2B2_data;
	typedef TPIXEL_data< A4R4G4B4_root, 12,8,4,0 > A4R4G4B4_data;
	typedef TPIXEL_data< A1R5G5B5_root, 15,10,5,0 > A1R5G5B5_data;
	typedef TPIXEL_data< A8R8G8B8_root, 24,16,8,0 > A8R8G8B8_data;
	typedef TPIXEL_data< A6R6G6B6_root, 18, 12, 6, 0 > A6R6G6B6_data;
	typedef TPIXEL_data< A2R10G10B10_root, 30, 20, 10, 0 > A2R10G10B10_data;
	typedef TPIXEL_data< A16R16G16B16_root, 48, 32, 16, 0 > A16R16G16B16_data;

	// ------------------------------------------------------------------------
	// BGRA Pixel data types (TODO make a mirror type of all the ARGB's)
	// ------------------------------------------------------------------------

	typedef TBGRA_root< U8,  1, 2, 3, 2 > B2G3R2A1_root;
	typedef TBGRA_root< U8,  2, 2, 2, 2 > B2G2R2A2_root;
	typedef TBGRA_root< U16, 4, 4, 4, 4 > B4G4R4A4_root;
	typedef TBGRA_root< U16, 1, 5, 5, 5 > B5G5R5A1_root;
	typedef TBGRA_root< U32, 8, 8, 8, 8 > B8G8R8A8_root;

	typedef TPIXEL_data< B2G3R2A1_root, 0,2,5,7 > B2G3R2A1_data;
	typedef TPIXEL_data< B2G2R2A2_root, 0,2,4,6 > B2G2R2A2_data;
	typedef TPIXEL_data< B4G4R4A4_root, 0,4,8,12 > B4G4R4A4_data;
	typedef TPIXEL_data< B5G5R5A1_root, 0,5,10,15 > B5G5R5A1_data;
	typedef TPIXEL_data< B8G8R8A8_root, 0,8,16,24 > B8G8R8A8_data;

	// ------------------------------------------------------------------------
	// RGB/BGR packed types
	// ------------------------------------------------------------------------

	typedef TRGB_root< U8, 1, 1, 1 > R1G1B1_root;
	typedef TRGB_root< U8, 1, 2, 1 > R1G2B1_root;
	typedef TRGB_root< U8, 1, 2, 2 > R1G2B2_root;
	typedef TRGB_root< U8, 2, 2, 2 > R2G2B2_root;
	typedef TRGB_root< U8, 2, 3, 2 > R2G3B2_root;
	typedef TRGB_root< U8, 3, 3, 2 > R3G3B2_root;
	typedef TRGB_root< U8, 2, 3, 3 > R2G3B3_root;

	typedef TRGB_root< U16, 3, 3, 3 > R3G3B3_root;
	typedef TRGB_root< U16, 3, 4, 3 > R3G4B3_root;
	typedef TRGB_root< U16, 3, 4, 4 > R3G4B4_root;

	typedef TRGB_root< U16, 4, 4, 4 > R4G4B4_root;
	typedef TRGB_root< U16, 5, 6, 5 > R5G6B5_root;
	typedef TRGB_root< U16, 5, 5, 5 > R5G5B5_root;
	typedef TRGB_root< U32, 6, 6, 6 > R6G6B6_root;

	typedef TRGB_root< U64, 16, 16, 16 > R16G16B16_root;

	typedef TPIXEL_data< R1G1B1_root, 9, 2, 1, 0 > R1G1B1_data;
	typedef TPIXEL_data< R1G2B1_root, 9, 3, 1, 0 > R1G2B1_data;
	typedef TPIXEL_data< R1G2B2_root, 9, 4, 2, 0 > R1G2B2_data;
	typedef TPIXEL_data< R2G2B2_root, 9, 4, 2, 0 > R2G2B2_data;
	typedef TPIXEL_data< R2G3B2_root, 9, 6, 2, 0 > R2G3B2_data;
	typedef TPIXEL_data< R3G3B2_root, 9, 5, 3, 0 > R3G3B2_data;
	typedef TPIXEL_data< R2G3B3_root, 9, 6, 3, 0 > R2G3B3_data;

	typedef TPIXEL_data< R3G3B3_root, 9, 6, 3, 0 > R3G3B3_data;
	typedef TPIXEL_data< R3G4B3_root, 10, 7, 4, 0 > R3G4B3_data;
	typedef TPIXEL_data< R3G4B4_root, 11, 8, 4, 0 > R3G4B4_data;

	typedef TPIXEL_data< R4G4B4_root, 16, 12, 4, 0 > R4G4B4_data;
	typedef TPIXEL_data< R5G6B5_root, 16, 11, 5, 0 > R5G6B5_data;
	typedef TPIXEL_data< R5G5B5_root, 16, 10, 5, 0 > R5G5B5_data;
	typedef TPIXEL_data< R6G6B6_root, 18, 12, 6, 0 > R6G6B6_data;

	typedef TPIXEL_data< R16G16B16_root, 48, 32, 16, 0 > R16G16B16_data;

	typedef TBGR_root< U8, 2, 2, 2 > B2G2R2_root;
	typedef TBGR_root< U8, 2, 3, 2 > B2G3R2_root;
	typedef TBGR_root< U8, 3, 3, 2 > B2G3R3_root;
	typedef TBGR_root< U8, 2, 3, 3 > B3G3R2_root;
	typedef TBGR_root< U16, 4, 4, 4 > B4G4R4_root;
	typedef TBGR_root< U16, 5, 6, 5 > B5G6R5_root;

	typedef TPIXEL_data< B2G2R2_root, 0, 2, 4, 9 > B2G2R2_data;
	typedef TPIXEL_data< B2G3R2_root, 0, 2, 5, 9 > B2G3R2_data; 
	typedef TPIXEL_data< B3G3R2_root, 0, 3, 6, 9 > B3G3R2_data;
	typedef TPIXEL_data< B2G3R3_root, 0, 3, 5, 9 > B2G3R3_data;
	typedef TPIXEL_data< B4G4R4_root, 0, 4, 12, 16 > B4G4R4_data;
	typedef TPIXEL_data< B5G6R5_root, 0, 5, 11, 16 > B5G6R5_data;

	// ------------------------------------------------------------------------

	// TODO - verify that this type is being used and efficient

	//
	//	PIXEL_RAW_ARGB_TYPE
	//

	struct PIXEL_RAW_ARGB_TYPE {

		U8 a;
		U8 r;
		U8 g;
		U8 b;

		PIXEL_RAW_ARGB_TYPE(
			const int rr, const int gg, const int bb, const int aa
		) : r( rr ), g( gg ), b( bb ), a( aa ) {

			/* Empty */

		}

#if defined( BPTLIB_INCLUDE_PIXELTYPE_COMPARISON_OPERATORS )

		__forceinline bool operator==( const PIXEL_RAW_ARGB_TYPE & rhs ) const {

			return (rhs.a == a) && (rhs.r == r) && (rhs.g == g) && (rhs.b == b);

		}

		__forceinline bool operator!=( const PIXEL_RAW_ARGB_TYPE & rhs ) const {

			if ( (rhs.a != a) || (rhs.r != r) || (rhs.g != g) || (rhs.b != b) ) {

				return true;

			}

			return false;

		}

#endif // defined( BPTLIB_INCLUDE_PIXELTYPE_COMPARISON_OPERATORS )

	};

	//
	//	TPIXEL_type
	//

	template<
		typename ST
		,typename PD
		,const int CC = 4
	>
	struct TPIXEL_type : public PD {

		typedef PD base_type;

		typedef ST specialization_type;
		typedef TPIXEL_type<ST,PD,CC> this_type;
		typedef this_type & reference;
		typedef this_type * pointer;
		typedef const this_type & const_reference;
		typedef const this_type * const_pointer;

		// More pixel traits

		enum : unsigned {

			format = ((A_BitCount * 1000) + (R_BitCount * 100) + (G_BitCount * 10) + B_BitCount)
			,channel_count = CC
			,optimized = false

		};

		// Construction helper

		__forceinline TPIXEL_type() { /* Empty */ }

		__forceinline TPIXEL_type( const int r, const int g, const int b, const int a = 0 ) {

			A( a );
			R( r );
			G( g );
			B( b );

		}

		__forceinline TPIXEL_type( const int value ) {

			A( value );
			R( value );
			G( value );
			B( value );

		}

		// ---------------------------------------------------

		__forceinline int not_grayscale() const
		{
			return (R() * 307 + G() * 604 * B() * 113) >> 10;
		}

		__forceinline this_type not_GRAYSCALE() const
		{
			int gray = not_grayscale();
			return this_type(gray, gray, gray, A());
		}

		__forceinline int desaturate() const
		{
			return (MINN(R(), MINN(G(), B())) + MAXX(R(), MAXX(G(), B()))) >> 1;
		}

		__forceinline this_type DESATURATE() const
		{
			auto gray = desaturate();
			return this_type(gray, gray, gray, A());
		}

		__forceinline this_type invert() const
		{
			this_type i;
			i.r = this_type::R_LimitedMax - r;
			i.g = this_type::G_LimitedMax - g;
			i.b = this_type::B_LimitedMax - b;
			i.A(A());
			return i;
		}

#if 1

		// General 'lazy' type conversion

		__forceinline operator PIXEL_RAW_ARGB_TYPE() const {

			return PIXEL_RAW_ARGB_TYPE( 
				R(), G(), B(), A()
			);

		}

		__forceinline this_type( const PIXEL_RAW_ARGB_TYPE & rhs ) {

			A( rhs.a );
			R( rhs.r );
			G( rhs.g );
			B( rhs.b );

		}

		__forceinline reference operator=( const PIXEL_RAW_ARGB_TYPE & rhs ) {

			A( rhs.a );
			R( rhs.r );
			G( rhs.g );
			B( rhs.b );

			return *this;

		}

#endif

#if defined( BPTLIB_DEFINE_SIMPLE_CHANNEL_ACCESS_METHODS )

		int operator[](std::size_t idx) const {
			return GetChannel(idx);
		}

		int GetChannel( const int channel ) const {

			switch ( channel & 3 ) {

			case 0: return R(); break;
			case 1: return G(); break;
			case 2: return B(); break;
			case 3: return A(); break;

			}

			return 0;

		}

		void SetChannel( const int channel, const int value ) {

			switch ( channel & 3) {

			case 0: R( value ); break;
			case 1: G( value ); break;
			case 2: B( value ); break;
			case 3: A( value ); break;

			}

		}

		static int GetChannelCount() {

			return channel_count;

		}

#endif // defined( BPTLIB_DEFINE_SIMPLE_CHANNEL_ACCESS_METHODS )

#if defined( BPTLIB_INCLUDE_PIXELTYPE_COMPARISON_OPERATORS )

		// This comparison isn't valid for the types that don't 
		// have proper value types (the 24 bpp types etc...)
		// fairly certain types without an alpha channel are comparing garbage 
		// as the a field is left uninitialized in most cases (todo refactor comparisons and the field initialization?)

		__forceinline bool operator==( const_reference rhs ) const {

			return (rhs.value == value);

		}

		__forceinline bool operator!=( const_reference rhs ) const {

			return (rhs.value != value);

		}

		__forceinline bool operator<( const_reference rhs ) const {

			return (value < rhs.value);

		}

#endif // defined( BPTLIB_INCLUDE_PIXELTYPE_COMPARISON_OPERATORS )

		static __forceinline this_type rnd() {
			return this_type(rand(),rand(),rand());
		}

	};

	// ------------------------------------------------------------------------
	// Pixel types (WITH value_types)
	// ------------------------------------------------------------------------

	typedef TPIXEL_type< P64_tag, A16R16G16B16_data, 4 > PIXEL_A16R16G16B16;
	typedef TPIXEL_type< P64_tag, R16G16B16_data, 3 > PIXEL_R16G16B16;

	typedef TPIXEL_type< P8__tag, A5R1G1B1_data, 4 > PIXEL_A5R1G1B1;
	typedef TPIXEL_type< P8__tag, A2R2G2B2_data, 4 > PIXEL_A2R2G2B2;
	typedef TPIXEL_type< P16_tag, A4R4G4B4_data, 4 > PIXEL_A4R4G4B4;
	typedef TPIXEL_type< P16_tag, A1R5G5B5_data, 4 > PIXEL_A1R5G5B5;
	typedef TPIXEL_type< P32_tag, A8R8G8B8_data, 4 > PIXEL_A8R8G8B8;
	typedef TPIXEL_type< P32_tag, A6R6G6B6_data, 4 > PIXEL_A6R6G6B6;
	typedef TPIXEL_type< P32_tag, A2R10G10B10_data, 4 > PIXEL_A2R10G10B10;

	typedef TPIXEL_type< P8__tag, B2G2R2A2_data, 4 > PIXEL_B2G2R2A2;
	typedef TPIXEL_type< P16_tag, B4G4R4A4_data, 4 > PIXEL_B4G4R4A4;
	typedef TPIXEL_type< P16_tag, B5G5R5A1_data, 4 > PIXEL_B5G5R5A1;
	typedef TPIXEL_type< P32_tag, B8G8R8A8_data, 4 > PIXEL_B8G8R8A8;

	typedef TPIXEL_type< P8__tag, R1G1B1_data, 3 > PIXEL_R1G1B1;
	typedef TPIXEL_type< P8__tag, R1G2B1_data, 3 > PIXEL_R1G2B1;
	typedef TPIXEL_type< P8__tag, R1G2B2_data, 3 > PIXEL_R1G2B2;

	typedef TPIXEL_type< P8__tag, R2G2B2_data, 3 > PIXEL_R2G2B2;
	typedef TPIXEL_type< P8__tag, R2G3B2_data, 3 > PIXEL_R2G3B2;
	typedef TPIXEL_type< P8__tag, R2G3B3_data, 3 > PIXEL_R2G3B3;
	typedef TPIXEL_type< P8__tag, R3G3B2_data, 3 > PIXEL_R3G3B2;

	typedef TPIXEL_type< P8__tag, R3G3B2_data, 3 > PIXEL_R3G3B2;
	typedef TPIXEL_type< P8__tag, R3G3B2_data, 3 > PIXEL_R3G3B2;
	typedef TPIXEL_type< P8__tag, R3G3B2_data, 3 > PIXEL_R3G3B2;
	
	typedef TPIXEL_type< P16_tag, R3G3B3_data, 3 > PIXEL_R3G3B3;
	typedef TPIXEL_type< P16_tag, R3G4B3_data, 3 > PIXEL_R3G4B3;
	typedef TPIXEL_type< P16_tag, R3G4B4_data, 3 > PIXEL_R3G4B4;

	typedef TPIXEL_type< P16_tag, R5G6B5_data, 3 > PIXEL_R5G6B5;
	typedef TPIXEL_type< P16_tag, R5G5B5_data, 3 > PIXEL_R5G5B5;
	typedef TPIXEL_type< P16_tag, R4G4B4_data, 3 > PIXEL_R4G4B4;

	typedef TPIXEL_type< P32_tag, R6G6B6_data, 3 > PIXEL_R6G6B6;

	typedef TPIXEL_type< P8__tag, B2G2R2_data, 3 > PIXEL_B2G2R2;
	typedef TPIXEL_type< P8__tag, B2G3R2_data, 3 > PIXEL_B2G3R2;
	typedef TPIXEL_type< P8__tag, B2G3R3_data, 3 > PIXEL_B2G3R3;
	typedef TPIXEL_type< P16_tag, B5G6R5_data, 3 > PIXEL_B5G6R5;
	typedef TPIXEL_type< P16_tag, B4G4R4_data, 3 > PIXEL_B4G4R4;

	// ------------------------------------------------------------------------
	// ****WARNING**** THESE TYPES ARE IMPROPER BECAUSE THE 'VALUE_TYPE' doesn't
	// REPRESENT THE ENTIRE RANGE OF THE RGB TRIPLET!!!
	// ------------------------------------------------------------------------

	typedef TRGB_root< U8, 8, 8, 8 > R8G8B8_root;
	typedef TPIXEL_data< R8G8B8_root, 24,16,8,0 > R8G8B8_data;
	typedef TPIXEL_type< P24_tag, R8G8B8_data, 3 > PIXEL_R8G8B8;

	typedef TBGR_root< U8,  8, 8, 8 > B8G8R8_root;
	typedef TPIXEL_data< B8G8R8_root, 0,8,16,24 > B8G8R8_data;
	typedef TPIXEL_type< P24_tag, B8G8R8_data, 3 > PIXEL_B8G8R8;

	// ------------------------------------------------------------------------
	// Integer pixeltypes
	// ------------------------------------------------------------------------

	//
	//	VALUE_HELPER
	//

	template< typename T >
	struct VALUE_HELPER {

		typedef VALUE_HELPER<T> this_type;
		typedef this_type & reference;
		typedef this_type * pointer;
		typedef const this_type & const_reference;
		typedef const this_type * const_pointer;

		typedef T value_type;

		T value;

#if 0

		operator int() const {

			return static_cast< int >( value );

		}

		reference operator=( const int set ) {

			value = static_cast< T >( set );

			return *this;

		}

#endif

#if defined( BPTLIB_DEFINE_SIMPLE_CHANNEL_ACCESS_METHODS )

		int GetChannel( const int channel ) const {

			return value;

		}

		void SetChannel( const int channel, const int newValue ) {

			if ( 0 == channel ) {

				value = newValue;

			}

		}

#endif // defined( BPTLIB_DEFINE_SIMPLE_CHANNEL_ACCESS_METHODS )

	};

	//
	//	TPIXEL_struct
	//

	template<
		typename SPECIALIZATION_TAG,
		typename T
	>
	struct TPIXEL_struct : public VALUE_HELPER<T> {

		typedef SPECIALIZATION_TAG specialization_type;

		typedef VALUE_HELPER<T> base_type;

		enum : T {

			Bits			= (sizeof(T) * 8)
			,format			= (255-Bits)
			,channel_count	= 1
			,optimized		= false

		};

		static int GetChannelCount() {

			return channel_count;

		}

	};

	typedef TPIXEL_struct<P8__tag,U8> PIXEL_P8;
	typedef TPIXEL_struct<P16_tag,U16> PIXEL_P16;
	typedef TPIXEL_struct<P32_tag,U32> PIXEL_P32;
	typedef TPIXEL_struct<P64_tag,U64> PIXEL_P64;

// ----------------------------------------------------------------------------
#pragma pack( pop, PIXEL_packing )
// ----------------------------------------------------------------------------

	// ------------------------------------------------------------------------
	//	changes a pixels optimized 'trait' to true instead of the default false
	//	should the base type have the 'optimized' trait?  Need's more design
	// ------------------------------------------------------------------------

	template< typename T >
	struct TOptimized : public T {

		enum {

			optimized = true

		};

		TOptimized<T>() {}
		TOptimized<T>( const int r, const int g, const int b, const int a = 0 ) : T(r,g,b,a) {}
		TOptimized<T>( const int v ) : T(v) {}

		typedef TOptimized<T> this_type;
		typedef this_type & reference;
		typedef this_type * pointer;
		typedef const this_type & const_reference;
		typedef const this_type * const_pointer;

	};

	// ---------------------------------------------------------------------------

	// diagnostics this should be a compile time report instead
	// it should also have a way of compile time asserting on incorrect sizes

#if 0
#define BPT_PIXELS_H_REPORT_SIZEOF(X) TRACE( #X " sizeof(%d)\n", sizeof(X))

BPT_PIXELS_H_REPORT_SIZEOF(BPT::PIXEL_P8);
BPT_PIXELS_H_REPORT_SIZEOF(BPT::PIXEL_P16);
BPT_PIXELS_H_REPORT_SIZEOF(BPT::PIXEL_P32);
BPT_PIXELS_H_REPORT_SIZEOF(BPT::PIXEL_P64);

BPT_PIXELS_H_REPORT_SIZEOF(BPT::PIXEL_R16G16B16);
BPT_PIXELS_H_REPORT_SIZEOF(BPT::PIXEL_A16R16G16B16);

BPT_PIXELS_H_REPORT_SIZEOF(BPT::PIXEL_A5R1G1B1);
BPT_PIXELS_H_REPORT_SIZEOF(BPT::PIXEL_A2R2G2B2);
BPT_PIXELS_H_REPORT_SIZEOF(BPT::PIXEL_A4R4G4B4);
BPT_PIXELS_H_REPORT_SIZEOF(BPT::PIXEL_A1R5G5B5);
BPT_PIXELS_H_REPORT_SIZEOF(BPT::PIXEL_A8R8G8B8);
BPT_PIXELS_H_REPORT_SIZEOF(BPT::PIXEL_A6R6G6B6);
BPT_PIXELS_H_REPORT_SIZEOF(BPT::PIXEL_A2R10G10B10);

BPT_PIXELS_H_REPORT_SIZEOF(BPT::PIXEL_B2G2R2A2);
BPT_PIXELS_H_REPORT_SIZEOF(BPT::PIXEL_B4G4R4A4);
BPT_PIXELS_H_REPORT_SIZEOF(BPT::PIXEL_B5G5R5A1);
BPT_PIXELS_H_REPORT_SIZEOF(BPT::PIXEL_B8G8R8A8);

BPT_PIXELS_H_REPORT_SIZEOF(BPT::PIXEL_R1G1B1);
BPT_PIXELS_H_REPORT_SIZEOF(BPT::PIXEL_R1G2B1);
BPT_PIXELS_H_REPORT_SIZEOF(BPT::PIXEL_R1G2B2);

BPT_PIXELS_H_REPORT_SIZEOF(BPT::PIXEL_R2G2B2);
BPT_PIXELS_H_REPORT_SIZEOF(BPT::PIXEL_R2G3B2);
BPT_PIXELS_H_REPORT_SIZEOF(BPT::PIXEL_R2G3B3);
BPT_PIXELS_H_REPORT_SIZEOF(BPT::PIXEL_R3G3B2);

BPT_PIXELS_H_REPORT_SIZEOF(BPT::PIXEL_R3G3B2);
BPT_PIXELS_H_REPORT_SIZEOF(BPT::PIXEL_R3G3B2);
BPT_PIXELS_H_REPORT_SIZEOF(BPT::PIXEL_R3G3B2);

BPT_PIXELS_H_REPORT_SIZEOF(BPT::PIXEL_R3G3B3);
BPT_PIXELS_H_REPORT_SIZEOF(BPT::PIXEL_R3G4B3);
BPT_PIXELS_H_REPORT_SIZEOF(BPT::PIXEL_R3G4B4);

BPT_PIXELS_H_REPORT_SIZEOF(BPT::PIXEL_R5G6B5);
BPT_PIXELS_H_REPORT_SIZEOF(BPT::PIXEL_R5G5B5);
BPT_PIXELS_H_REPORT_SIZEOF(BPT::PIXEL_R4G4B4);

BPT_PIXELS_H_REPORT_SIZEOF(BPT::PIXEL_R6G6B6);

BPT_PIXELS_H_REPORT_SIZEOF(BPT::PIXEL_B2G2R2);
BPT_PIXELS_H_REPORT_SIZEOF(BPT::PIXEL_B2G3R2);
BPT_PIXELS_H_REPORT_SIZEOF(BPT::PIXEL_B2G3R3);
BPT_PIXELS_H_REPORT_SIZEOF(BPT::PIXEL_B5G6R5);
BPT_PIXELS_H_REPORT_SIZEOF(BPT::PIXEL_B4G4R4);

BPT_PIXELS_H_REPORT_SIZEOF(BPT::PIXEL_R8G8B8);
BPT_PIXELS_H_REPORT_SIZEOF(BPT::PIXEL_B8G8R8);
#endif

}; // namespace BPT


#endif // !defined(AFX_PIXELS_H__E965F008_DA1F_4D8A_90FA_681A85832F25__INCLUDED_)
