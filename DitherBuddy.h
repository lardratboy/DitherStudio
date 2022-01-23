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

#if !defined(AFX_BPTLLC_DITHERBUDDY_H__D145CC32_FDCC_49a3_9DDB_400DD95197B1__INCLUDED_)
#define AFX_BPTLLC_DITHERBUDDY_H__D145CC32_FDCC_49a3_9DDB_400DD95197B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ----------------------------------------------------------------------------

namespace BPT {

	// ========================================================================

	enum DITHER_STYLE : unsigned {

		NO_DITHERING				= 0x0000
		, FLOYD_STEINBERG			= 0x0001
		, JARVIS_JUDICE_NINKE		= 0x0002
		, STUCKI					= 0x0003
		, STEVENSON_ARCE			= 0x0004
		, BURKES					= 0x0005
		, SIERRA					= 0x0006
		, OTHER_FROM_WEB			= 0x0007
		, ORDERED_FLAG				= 0x1000
		, ORDERED_2x2				= (ORDERED_FLAG | 0x0001)
		, ORDERED_4x4				= (ORDERED_FLAG | 0x0002)
		, ORDERED_8x8				= (ORDERED_FLAG | 0x0003)
		//, ORDERED_16x16			= (ORDERED_FLAG | 0x0004) // this is borked because the way the dithering tables work (yikes)

	}; // DITHER_STYLE

	// ========================================================================

	//
	//	TCheapQuantizeOP
	//	

	template< typename DST_TYPE, const int MASK >
	class TCheapQuantizeOP {

	public:

		typedef DST_TYPE dst_type;
		typedef DST_TYPE src_type;

	public:

		TCheapQuantizeOP() { /* Empty */ }

		__forceinline DST_TYPE& operator()(dst_type& d, const src_type& s) const {

			d.value = (s.value & MASK);

			return d;

		}

		// [first, last) to output (same semantics as std::copy())
		// --------------------------------------------------------------------

		template< typename ForwardIt, typename OutputIt >
		__forceinline void InputMajor(ForwardIt first, ForwardIt last, OutputIt output) const {

			while (first != last) {

				(*this)(*output++, *first++);

			}

		}

		// [first, last) write value 's'
		// --------------------------------------------------------------------

		// define an OutputMajor transfer based on input major (assumes a lot about the iterator behavior)

		template< typename OutputIt, typename InputIt >
		__forceinline void OutputMajor(OutputIt first, OutputIt last, InputIt input) const {
			InputMajor(input, input + (last - first), first);
		}

		template< typename OutputIt >
		__forceinline void OutputMajor(OutputIt first, OutputIt last, const src_type& s) const {

			dst_type v = (s.value & MASK);

			while (first != last) {

				(*this)(*first++, v);

			}

		}

	};

	// ========================================================================
	// 
	//
	//	TQuantizeAsOP
	//	

	template< typename DST_TYPE, typename AS_TYPE >
	class TQuantizeAsOP {

	public:

		typedef DST_TYPE dst_type;
		typedef DST_TYPE src_type;
		typedef AS_TYPE as_type;

	public:

		TQuantizeAsOP() { /* Empty */ }

		__forceinline dst_type& operator()(dst_type& d, const src_type& s) const {

			// this could be done faster 

			as_type as = s;
			int ar = as.r;
			int ag = as.g;
			int ab = as.b;

			int r = ((ar * dst_type::R_LimitedMax) + as_type::R_LimitedMax / 2) / as_type::R_LimitedMax;
			int g = ((ag * dst_type::G_LimitedMax) + as_type::G_LimitedMax / 2) / as_type::G_LimitedMax;
			int b = ((ab * dst_type::B_LimitedMax) + as_type::B_LimitedMax / 2) / as_type::B_LimitedMax;

#if 1
			d.r = r;
			d.g = g;
			d.b = b;
#else
			d = dst_type(r, g, b );
#endif

			return d;

		}

		// [first, last) to output (same semantics as std::copy())
		// --------------------------------------------------------------------

		template< typename ForwardIt, typename OutputIt >
		__forceinline void InputMajor(ForwardIt first, ForwardIt last, OutputIt output) const {

			while (first != last) {

				(*this)(*output++, *first++);

			}

		}

		// [first, last) write value 's'
		// --------------------------------------------------------------------

		// define an OutputMajor transfer based on input major (assumes a lot about the iterator behavior)

		template< typename OutputIt, typename InputIt >
		__forceinline void OutputMajor(OutputIt first, OutputIt last, InputIt input) const {
			InputMajor(input, input + (last - first), first);
		}

		template< typename OutputIt >
		__forceinline void OutputMajor(OutputIt first, OutputIt last, const src_type& s) const {

			dst_type v = (s.value & MASK);

			while (first != last) {

				(*this)(*first++, v);

			}

		}

	};

	// ========================================================================

#if 0
	// 
	// 
	//
	//	TBlinnQuantizeAsOP
	//	

	template< typename DST_TYPE, typename CONVERSION_TYPE >
	class TBlinnQuantizeAsOP {

	public:

		typedef DST_TYPE dst_type;
		typedef DST_TYPE src_type;
		typedef CONVERSION_TYPE conversion_type;

	public:

		TBlinnQuantizeAsOP() { /* Empty */ }

		__forceinline dst_type& operator()(dst_type& d, const src_type& s) const {

			// This uses a generalized version of Jim Blinn's /255 technique to divide by
			// other powers of 2^x - 1 values, this allows for correct scaling for 
			// low bit depth values to be scaled to higher bit depths without division.

			//BPT::PIXEL_B2G2R2::R_BitCount 

			int rt = (s.R() * conversion_type::R_LimitedMax) + src_type::R_BitCount;
			int gt = (s.G() * conversion_type::G_LimitedMax) + src_type::G_BitCount;
			int bt = (s.B() * conversion_type::B_LimitedMax) + src_type::B_BitCount;
			int at = (s.A() * conversion_type::A_LimitedMax) + src_type::A_BitCount;

#if 1
			d.R((rt + (rt >> src_type::R_BitCount)) >> src_type:R_BitCount);
			d.G((gt + (gt >> src_type::G_BitCount)) >> src_type:G_BitCount);
			d.B((bt + (bt >> src_type::B_BitCount)) >> src_type:B_BitCount);
			d.A((at + (at >> src_type::A_BitCount)) >> src_type:A_BitCount);
#else
			d = dst_type(
				  (rt + (rt >> src_type::R_BitCount)) >> src_type:R_BitCount
				, (gt + (gt >> src_type::G_BitCount)) >> src_type:G_BitCount
				, (bt + (bt >> src_type::B_BitCount)) >> src_type:B_BitCount
				, (at + (at >> src_type::A_BitCount)) >> src_type:A_BitCount
			);
#endif

			return d;

		}

		// [first, last) to output (same semantics as std::copy())
		// --------------------------------------------------------------------

		template< typename ForwardIt, typename OutputIt >
		__forceinline void InputMajor(ForwardIt first, ForwardIt last, OutputIt output) const {

			while (first != last) {

				(*this)(*output++, *first++);

			}

		}

		// [first, last) write value 's'
		// --------------------------------------------------------------------

		// define an OutputMajor transfer based on input major (assumes a lot about the iterator behavior)

		template< typename OutputIt, typename InputIt >
		__forceinline void OutputMajor(OutputIt first, OutputIt last, InputIt input) const {
			InputMajor(input, input + (last - first), first);
		}

		template< typename OutputIt >
		__forceinline void OutputMajor(OutputIt first, OutputIt last, const src_type& s) const {

			while (first != last) {

				(*this)(*first++, s);

			}

		}

	};

#endif

}; // namespace BPT

// ----------------------------------------------------------------------------

#endif // !defined(AFX_BPTLLC_DITHERBUDDY_H__D145CC32_FDCC_49a3_9DDB_400DD95197B1__INCLUDED_)
