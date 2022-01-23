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

#if !defined(AFX_BPTRASTEROPS_H__9484DA55_390E_43E2_8A1B_8E9CD9234DCA__INCLUDED_)
#define AFX_BPTRASTEROPS_H__9484DA55_390E_43E2_8A1B_8E9CD9234DCA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ----------------------------------------------------------------------------

#include "BPTUtility.h"
#include "BPTBitmap.h"

// ----------------------------------------------------------------------------

namespace BPT {

	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// Blitter raster ops
	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	//
	//	TTransparentSrcTransferROP
	//	

	template< typename DST_TYPE, typename SRC_TYPE = DST_TYPE >
	struct TTransparentSrcTransferROP {

	public:

		typedef DST_TYPE dst_type;
		typedef SRC_TYPE src_type;

	private:

		src_type m_ColorKey;

	public:

		TTransparentSrcTransferROP( const src_type colorKey = SRC_TYPE(0) ) : 
			m_ColorKey( colorKey ) { /* Empty */ }

		__forceinline DST_TYPE & operator()( dst_type & d, const src_type & s ) const {

			if ( m_ColorKey != s ) {

				d = s;

			}

			return d;

		}

		// [first, last) to output (same semantics as std::copy())
		// --------------------------------------------------------------------

		template< typename ForwardIt, typename OutputIt >
		__forceinline void InputMajor( ForwardIt first, ForwardIt last, OutputIt output ) const {

			while ( first != last ) {

				SRC_TYPE s = *first++;

				if ( m_ColorKey != s ) {

					*output = s;

				}

				++output;

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
		__forceinline void OutputMajor( OutputIt first, OutputIt last, const src_type & s ) const {

			if ( m_ColorKey != s ) {

				while ( first != last ) {
	
					*first++ = s;
	
				}

			}

		}

		// --------------------------------------------------------------------

		void SetColorKey( const src_type & s ) {

			m_ColorKey = s;

		}

		src_type GetColorKey() const {

			return m_ColorKey;

		}

	};

	// ------------------------------------------------------------------------
	// Invert
	// ------------------------------------------------------------------------

	//
	//	TInvertDstTransferROP
	//	

	template< typename DST_TYPE, typename SRC_TYPE = DST_TYPE >
	struct TInvertDstTransferROP {

	public:

		typedef DST_TYPE dst_type;
		typedef SRC_TYPE src_type;

	public:

		TInvertDstTransferROP() { /* Empty */ }

		__forceinline DST_TYPE & operator()( dst_type & d, const src_type & s ) const {

			d.value = ~d.value;

			return d;

		}

		// [first, last) to output (same semantics as std::copy())
		// --------------------------------------------------------------------

		template< typename ForwardIt, typename OutputIt >
		__forceinline void InputMajor( ForwardIt first, ForwardIt last, OutputIt output ) const {

			while ( first != last ) {

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
		__forceinline void OutputMajor( OutputIt first, OutputIt last, const src_type & s ) const {

			while ( first != last ) {

				(*this)(*first++, s);

			}

		}

	};

	// ------------------------------------------------------------------------
	//	'memset' helper
	// ------------------------------------------------------------------------

	template< typename T >
	struct TMemsetHelper {

	public: // traits

		template<int> struct Specialized_Memset {
			template< typename OutputIt, typename src_type > __forceinline
			void Action( OutputIt first, OutputIt last, const src_type & s ) const {
				/* Empty */
			}
		};

		template<> struct Specialized_Memset<1> {

			template< class OutputIt, class src_type > __forceinline
			void Action( OutputIt first, OutputIt last, const src_type & s ) const {

				memset( (void *)&(*first), s, last - first );

			}

		};

		template<> struct Specialized_Memset<2> {

			template< typename OutputIt, typename src_type > __forceinline
			void Action( OutputIt first, OutputIt last, const src_type & s ) const {

				// Do the odd value first if there is one
				// ----------------------------------------------------------------
	
				int count = last - first;
	
				if ( count & 1 ) {
	
					*first = s;
	
					count = count / 2;
	
					if ( !count ) {
	
						return;
	
					}
	
					++first;
	
				} else {
	
					count /= 2;
	
				}
	
				// Do the DWORD's
				// ----------------------------------------------------------------

				typedef unsigned U32;
	
				U32 combined = (U32)s | ((U32)s << 16);
	
				U32 * dst = reinterpret_cast<U32 *>( &(*first) );
				U32 * end = dst + count;
	
				while ( dst != end ) {
	
					*dst++ = combined;
	
				}

			}

		};

		template<> struct Specialized_Memset<4> {

			template< class OutputIt, class src_type > __forceinline
			void Action( OutputIt first, OutputIt last, const src_type & s ) const {

				while ( first != last ) {
		
					*first++ = s;
	
				}

			}

		};

		template<> struct Specialized_Memset<0> {

			template< class OutputIt, class src_type > __forceinline
			void Action( OutputIt first, OutputIt last, const src_type & s ) const {

				while ( first != last ) {
		
					*first++ = s;
	
				}

			}

		};

	public: // 'interface'

		template< typename OutputIt, typename src_type > __forceinline
		void op( OutputIt first, OutputIt last, const src_type & s ) {

			// Hack test to see if we have a forward iterator (could be horribly wrong)
			// Hack test to see if we have a forward iterator (could be horribly wrong)
			// Hack test to see if we have a forward iterator (could be horribly wrong)
			// ----------------------------------------------------------------

			BYTE * a = reinterpret_cast< BYTE *>( &(*first) );
			BYTE * b = reinterpret_cast< BYTE *>( &(*(first + 1)) );

			if ( 0 < (b - a) ) {

				Specialized_Memset<sizeof(src_type)> dummy_var;

				dummy_var.Action( first, last, s );

			} else {

				Specialized_Memset<0> dummy_var;

				dummy_var.Action( first, last, s );

			}


		}

	};

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------

	//
	//	TCopyROP
	//	

	template< typename DST_TYPE, typename SRC_TYPE = DST_TYPE >
	struct TCopyROP {

	public:

		typedef DST_TYPE dst_type;
		typedef SRC_TYPE src_type;

	public:

		TCopyROP() { /* Empty */ }

		__forceinline DST_TYPE & operator()( dst_type & d, const src_type & s ) const {

			d = s;

			return d;

		}

		// [first, last) write value 's'
		// --------------------------------------------------------------------

		template< typename OutputIt >
		__forceinline void OutputMajor( OutputIt first, OutputIt last, const src_type & s ) const {

#if 1
			// TODO think about enabling specialization ?

			while (first != last) {

				*first++ = s;

			}

#else

			dst_type * a = &(*first);
			dst_type * b = &(*(first + 1));

			if ( (1 == (b - a)) && (sizeof(src_type) == sizeof(dst_type)) ) {

				// Need to find a memset type for this 'size'

				TMemsetHelper<dst_type> helper;
				
				helper.op( first, last, s );

			} else {

				while ( first != last ) {
		
					*first++ = s;
	
				}

			}

#endif

		}

		// "Specializations"
		// --------------------------------------------------------------------

	private:

		template<bool> struct Specialized_InputMajor {
			template< typename ForwardIt, typename OutputIt > __forceinline
			void Action( ForwardIt first, ForwardIt last, OutputIt output ) const {
				/* Empty */
			}
		};

		template<> struct Specialized_InputMajor<false> {

			template< typename ForwardIt, typename OutputIt > __forceinline
			void Action( ForwardIt first, ForwardIt last, OutputIt output ) const {

				while ( first != last ) {
	
					*output++ = *first++;
	
				}

			}

		};

		template<> struct Specialized_InputMajor<true> {

			template< typename ForwardIt, typename OutputIt > __forceinline
			void Action( ForwardIt _first, ForwardIt _last, OutputIt _output ) const {

				memcpy(
					reinterpret_cast<void *>( &(*_output) ),
					reinterpret_cast<void *>( &(*_first) ),
					(_last - _first) * sizeof(*_output)
				);

			}

		};

	public:

		// [first, last) to output (same semantics as std::copy())
		// --------------------------------------------------------------------

		template< typename ForwardIt, typename OutputIt >
		__forceinline void InputMajor( ForwardIt first, ForwardIt last, OutputIt output ) const {

#if 1
			// TODO think about enabling specialization ?

			while (first != last) {

				*output++ = *first++;

			}

#else

			// Hack test to see if we have a forward iterator (could be horribly wrong)
			// Hack test to see if we have a forward iterator (could be horribly wrong)
			// Hack test to see if we have a forward iterator (could be horribly wrong)
			// ----------------------------------------------------------------

			BYTE* a = reinterpret_cast<BYTE*>(output); // &(*output) );
			BYTE* b = reinterpret_cast<BYTE*>(output + 1); // &(*(output + 1)) );
			BYTE* c = reinterpret_cast<BYTE*>(first); // &(*first) );
			BYTE* d = reinterpret_cast<BYTE*>(first + 1); // &(*(first + 1)) );

			// ----------------------------------------------------------------

			int bma = (b - a);
			int dmc = (d - c);

			// ----------------------------------------------------------------

#if 0

			if ( (0 < bma) && (0 < dmc) && (bma == dmc) ) {
				
				Specialized_InputMajor<true> dummy_var;

#else

			if ( (0 < bma) && (0 < dmc) ) {
				
				Specialized_InputMajor<(sizeof(src_type) == sizeof(dst_type))> dummy_var;

#endif

				dummy_var.Action( first, last, output );
	
			} else {
	
				Specialized_InputMajor<false> dummy_var;

				dummy_var.Action( first, last, output );
	
			}

#endif

		}

		// define an OutputMajor transfer based on input major (assumes a lot about the iterator behavior)

		template< typename OutputIt, typename InputIt >
		__forceinline void OutputMajor(OutputIt first, OutputIt last, InputIt input) const {
			InputMajor(input, input + (last - first), first);
		}

	};

	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// CLUT raster ops
	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	//
	//	TClutTransferROP
	//	

	template<
		typename DST_TYPE
		, typename SRC_TYPE = DST_TYPE
		, typename CLUT_TYPE = TPow2Clut< DST_TYPE >
	>
	struct TClutTransferROP {

	private:

		CLUT_TYPE * m_pClut;

		TClutTransferROP(); // Hidden

	public:

		typedef DST_TYPE dst_type;
		typedef SRC_TYPE src_type;

		TClutTransferROP( CLUT_TYPE * pClut) : 
			m_pClut( pClut ) { /* Empty */ }

		__forceinline DST_TYPE & operator()( dst_type & d, const src_type & s ) const {

			d = (*m_pClut)[ s ];

			return d;

		}

		// [first, last) to output (same semantics as std::copy())
		// --------------------------------------------------------------------

		template< typename ForwardIt, typename OutputIt >
		__forceinline void InputMajor( ForwardIt first, ForwardIt last, OutputIt output ) const {

			CLUT_TYPE & clut = *m_pClut;

			while ( first != last ) {

				*output++ = clut[ *first++ ];

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
		__forceinline void OutputMajor( OutputIt first, OutputIt last, const src_type & s ) const {

			dst_type out = (*m_pClut)[ s ];

			while ( first != last ) {

				*first++ = out;

			}

		}

	};

	//
	//	TSrcPredicateClutTransfer
	//	

	template<
		typename DST_TYPE
		, typename PREDICATE
		, typename SRC_TYPE = DST_TYPE
		, typename CLUT_TYPE = TPow2Clut< DST_TYPE >
		, typename TRANSFER_OP = TCopyROP< DST_TYPE >
	>
	struct TSrcPredicateClutTransfer {

	private:

		TRANSFER_OP m_Op;
		PREDICATE m_Predicate;
		CLUT_TYPE * m_pClut;

		TSrcPredicateClutTransfer(); // Hidden

	public: // Traits

		typedef PREDICATE predicate_type;
		typedef TRANSFER_OP transfer_type;
		typedef DST_TYPE dst_type;
		typedef SRC_TYPE src_type;

	public: // interface

		TSrcPredicateClutTransfer(
			CLUT_TYPE * pClut, PREDICATE predicate = PREDICATE(),
			TRANSFER_OP op = TRANSFER_OP() ) : 
			m_pClut( pClut ), m_Predicate( predicate ), m_Op( op ) { /* Empty */ }

		__forceinline DST_TYPE & operator()( dst_type & d, const src_type & s ) const {

			if ( m_Predicate( s ) ) {

				m_Op( d, (*m_pClut)[ s ] );

			}

			return d;

		}

		// [first, last) to output (same semantics as std::copy())
		// --------------------------------------------------------------------

		template< typename ForwardIt, typename OutputIt >
		__forceinline void InputMajor( ForwardIt first, ForwardIt last, OutputIt output ) const {

			CLUT_TYPE & clut = *m_pClut;

			while ( first != last ) {

				src_type s = *first++;

				if ( m_Predicate( s ) )  {

					m_Op( *output++, clut[ s ] );

				} else {

					++output;

				}

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
		__forceinline void OutputMajor( OutputIt first, OutputIt last, const src_type & s ) const {

			if ( m_Predicate( s ) ) {

				m_Op.OutputMajor( first, last, (*m_pClut)[ s ] );
	
			}

		}

	};

	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	//
	//	TClut5050TransferROP
	//	

	template<
		typename DST_TYPE
		, typename SRC_TYPE = DST_TYPE
		, typename CLUT_TYPE = TPow2Clut< DST_TYPE >
		, const int LOWBITS = 0x3def
	>
	struct TClut5050TransferROP {

	private:

		CLUT_TYPE * m_pClut;

		TClut5050TransferROP(); // Hidden

	public:

		typedef DST_TYPE dst_type;
		typedef SRC_TYPE src_type;

		TClut5050TransferROP( CLUT_TYPE * pClut) : 
			m_pClut( pClut ) { /* Empty */ }

		__forceinline DST_TYPE & operator()( dst_type & d, const src_type & s ) const {

			d = ((d >> 1) & LOWBITS) + (((*m_pClut)[ s ] >> 1) & LOWBITS);

			return d;

		}

		// [first, last) to output (same semantics as std::copy())
		// --------------------------------------------------------------------

		template< typename ForwardIt, typename OutputIt >
		__forceinline void InputMajor( ForwardIt first, ForwardIt last, OutputIt output ) const {

			CLUT_TYPE & clut = *m_pClut;

			while ( first != last ) {

				*output++ = ((*output >> 1) & LOWBITS) + ((clut[ *first++ ] >> 1) & LOWBITS);

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
		__forceinline void OutputMajor( OutputIt first, OutputIt last, const src_type & s ) const {

			dst_type out = (((*m_pClut)[ s ] >> 1) & LOWBITS);

			while ( first != last ) {

				*first++ = ((*first >> 1) & LOWBITS) + out;

			}

		}

	};

	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// BRUSH raster op
	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	//
	//	TMultiModeBrushTransferROP
	//	

	template< typename DST_TYPE, typename SRC_TYPE = DST_TYPE >
	struct TMultiModeBrushTransferROP {

	public:

		typedef DST_TYPE dst_type;
		typedef SRC_TYPE src_type;

		enum MODE {

			SOURCE	= 0
			,SINGLE	= 1

		};

	private:

		dst_type m_WriteColor;
		src_type m_ColorKey;
		MODE m_Mode;

	public:

		TMultiModeBrushTransferROP(
			const MODE mode = SOURCE, 
			const dst_type writeColor = DST_TYPE(0),
			const src_type colorKey = SRC_TYPE(0) ) : 
			m_Mode( mode ), m_WriteColor( writeColor), 
			m_ColorKey( colorKey ) { /* Empty */ }

		__forceinline DST_TYPE & operator()( dst_type & d, const src_type & s ) const {

			if ( SOURCE == m_Mode ) {

				if ( m_ColorKey != s ) {
	
					d = s;
	
				}

			} else {

				if ( m_ColorKey != s ) {
	
					d = m_WriteColor;
	
				}

			}

			return d;

		}

		// [first, last) to output (same semantics as std::copy())
		// --------------------------------------------------------------------

		template< typename ForwardIt, typename OutputIt >
		__forceinline void InputMajor( ForwardIt first, ForwardIt last, OutputIt output ) const {

			if ( SOURCE == m_Mode ) {

				while ( first != last ) {
	
					SRC_TYPE s = *first++;
	
					if ( m_ColorKey != s ) {
	
						*output = s;
	
					}
	
					++output;
	
				}

			} else {

				while ( first != last ) {
	
					if ( m_ColorKey != *first++ ) {
	
						*output = m_WriteColor;
	
					}
	
					++output;
	
				}

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
		__forceinline void OutputMajor( OutputIt first, OutputIt last, const src_type & s ) const {

			if ( m_ColorKey != s ) {

				if ( SOURCE == m_Mode ) {

					while ( first != last ) {
		
						*first++ = s;
		
					}

				} else {

					while ( first != last ) {
		
						*first++ = m_WriteColor;
		
					}

				}

			}

		}

		// --------------------------------------------------------------------

		void SetColorKey( const src_type & s ) {

			m_ColorKey = s;

		}

		src_type GetColorKey() const {

			return m_ColorKey;

		}

		void SetWriteColor( const dst_type & w ) {

			m_WriteColor = w;

		}

		dst_type GetWriteColor() const {

			return m_WriteColor;

		}

	};

	// ------------------------------------------------------------------------
	// 16bpp 50/50
	// ------------------------------------------------------------------------

	//
	//	T16bpp_5050_OP
	//	

	template< typename T, const int LOWBITS >
	struct T16bpp_5050_OP {

	public:

		typedef T dst_type;
		typedef T src_type;

	public:

		T16bpp_5050_OP() { /* Empty */ }

		__forceinline dst_type & operator()( dst_type & d, const src_type & s ) const {

			d = ((d >> 1) & LOWBITS) + ((s >> 1) & LOWBITS);

			return d;

		}

		// [first, last) to output (same semantics as std::copy())
		// --------------------------------------------------------------------

		template< typename ForwardIt, typename OutputIt >
		__forceinline void InputMajor( ForwardIt first, ForwardIt last, OutputIt output ) const {

			while ( first != last ) {

				*output++ = ((*output >> 1) & LOWBITS) + ((*first++ >> 1) & LOWBITS);

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
		__forceinline void OutputMajor( OutputIt first, OutputIt last, const src_type & s ) const {

			dst_type t = (s >> 1) & LOWBITS;

			while ( first != last ) {
	
				*first = ((*first >> 1) & LOWBITS) + t;

				++first;

			}

		}

	};

	// ------------------------------------------------------------------------

	//
	//	TSrcChromakeyClutTransfer
	//	
	//	Note the use of int is just there to aid the compiler in doing it's 
	//	optimizations, this code isn't as nifty as it could be. (as if)
	//	

	template<
		typename DST_TYPE
		, typename SRC_TYPE = DST_TYPE
		, typename CLUT_TYPE = TPow2Clut< DST_TYPE >
	>
	struct TSrcChromakeyClutTransfer {

	private:

		CLUT_TYPE & m_rClut;
		/* SRC_TYPE */ int m_Chromakey;

		TSrcChromakeyClutTransfer(); // Hidden

	public: // Traits

		typedef DST_TYPE dst_type;
		typedef SRC_TYPE src_type;

	public: // interface

		TSrcChromakeyClutTransfer(
			CLUT_TYPE & rClut, const SRC_TYPE chromakey
		) : m_rClut( rClut ), m_Chromakey( chromakey ) { /* Empty */ }

		__forceinline DST_TYPE & operator()( dst_type & d, const src_type & s ) const {

			if ( m_Chromakey != s ) {

				d = m_rClut[ s ];

			}

			return d;

		}

		// [first, last) to output (same semantics as std::copy())
		// --------------------------------------------------------------------

		template< typename ForwardIt, typename OutputIt >
		__forceinline void InputMajor( ForwardIt first, ForwardIt last, OutputIt output ) const {

			while ( first != last ) {

				/* src_type */ int s = *first++;

				if ( m_Chromakey != s ) {

					*output++ = m_rClut[ s ];

				} else {

					++output;

				}

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
		__forceinline void OutputMajor( OutputIt first, OutputIt last, const src_type & s ) const {

			if ( m_Chromakey != s ) {

				dst_type d = m_rClut[ s ];

				while ( first != last ) {

					*first++ = d;

				}
	
			}

		}

	};

	// ========================================================================

	//
	//	TTransparentTableSrcTransferROP
	//	

	template< typename DST_TYPE, typename TABLE_TYPE, typename SRC_TYPE = DST_TYPE >
	struct TTransparentTableSrcTransferROP {

	public:

		typedef TABLE_TYPE table_type;
		typedef DST_TYPE dst_type;
		typedef SRC_TYPE src_type;

	private:

		table_type m_ColorKeyTable;

		TTransparentTableSrcTransferROP(); // Hidden

	public:

		TTransparentTableSrcTransferROP( const table_type colorKeyTable ) : 
			m_ColorKeyTable( colorKeyTable ) { /* Empty */ }

		__forceinline DST_TYPE & operator()( dst_type & d, const src_type & s ) const {

			if ( m_ColorKeyTable[ s ] ) {

				d = s;

			}

			return d;

		}

		// [first, last) to output (same semantics as std::copy())
		// --------------------------------------------------------------------

		template< typename ForwardIt, typename OutputIt >
		__forceinline void InputMajor( ForwardIt first, ForwardIt last, OutputIt output ) const {

			while ( first != last ) {

				SRC_TYPE s = *first++;

				if ( m_ColorKeyTable[ s ] ) {

					*output = s;

				}

				++output;

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
		__forceinline void OutputMajor( OutputIt first, OutputIt last, const src_type & s ) const {

			if ( m_ColorKeyTable[ s ] ) {

				while ( first != last ) {
	
					*first++ = s;
	
				}

			}

		}

	};

	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	//
	//	TSingleColorTransferROP
	//	

	template<
		typename DST_TYPE
		, typename SRC_TYPE = DST_TYPE
	>
	struct TSingleColorTransferROP {

	private:

		DST_TYPE m_SingleColor;

	public:

		typedef DST_TYPE dst_type;
		typedef SRC_TYPE src_type;

		TSingleColorTransferROP( DST_TYPE singleColor = DST_TYPE(0) ) : 
			m_SingleColor( singleColor ) { /* Empty */ }

		__forceinline DST_TYPE & operator()( dst_type & d, const src_type & s ) const {

			d = m_SingleColor;

			return d;

		}

		// [first, last) to output (same semantics as std::copy())
		// --------------------------------------------------------------------

		template< typename ForwardIt, typename OutputIt >
		__forceinline void InputMajor( ForwardIt first, ForwardIt last, OutputIt output ) const {

			while ( first != last ) {

				*output++ = m_SingleColor;

				++first;

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
		__forceinline void OutputMajor( OutputIt first, OutputIt last, const src_type & s ) const {

			while ( first != last ) {

				*first++ = m_SingleColor;

			}

		}

		// --------------------------------------------------------------------

		void SetColor( const DST_TYPE value ) {

			m_SingleColor = value;

		}

	};

	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// Blitter raster ops
	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	//
	//	TSingleColorTransparentSrcTransferROP
	//	

	template< typename DST_TYPE, typename SRC_TYPE = DST_TYPE >
	struct TSingleColorTransparentSrcTransferROP {

	public:

		typedef DST_TYPE dst_type;
		typedef SRC_TYPE src_type;

	private:

		src_type m_ColorKey;
		dst_type m_OutputColor;

	public:

		TSingleColorTransparentSrcTransferROP(
			const dst_type outColor = DST_TYPE(~0),
			const src_type colorKey = SRC_TYPE(0)
		) : m_OutputColor( outColor ), m_ColorKey( colorKey ) { /* Empty */ }

		__forceinline DST_TYPE & operator()( dst_type & d, const src_type & s ) const {

			if ( m_ColorKey != s ) {

				d = m_OutputColor;

			}

			return d;

		}

		// [first, last) to output (same semantics as std::copy())
		// --------------------------------------------------------------------

		template< typename ForwardIt, typename OutputIt >
		__forceinline void InputMajor( ForwardIt first, ForwardIt last, OutputIt output ) const {

			while ( first != last ) {

				if ( m_ColorKey != *first++ ) {

					*output = m_OutputColor;

				}

				++output;

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
		__forceinline void OutputMajor( OutputIt first, OutputIt last, const src_type & s ) const {

			if ( m_ColorKey != s ) {

				while ( first != last ) {
	
					*first++ = m_OutputColor;
	
				}

			}

		}

	};

	// ------------------------------------------------------------------------
	// Swap things around
	// ------------------------------------------------------------------------

	//
	//	SwapRedAndGChannelsOp
	//	

	template< typename DST_TYPE, typename SRC_TYPE = DST_TYPE >
	struct SwapRedAndGreenChannelsOp {

	public:

		typedef DST_TYPE dst_type;
		typedef SRC_TYPE src_type;

	public:

		SwapRedAndGreenChannelsOp() { /* Empty */ }

		__forceinline DST_TYPE& operator()(dst_type& d, const src_type& s) const {

			d = dst_type(d.G(), d.R(), d.B());

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

	//
	//	SwapRedAndBlueChannelsOp
	//	

	template< typename DST_TYPE, typename SRC_TYPE = DST_TYPE >
	struct SwapRedAndBlueChannelsOp {

	public:

		typedef DST_TYPE dst_type;
		typedef SRC_TYPE src_type;

	public:

		SwapRedAndBlueChannelsOp() { /* Empty */ }

		__forceinline DST_TYPE& operator()(dst_type& d, const src_type& s) const {

			d = dst_type(d.B(), d.G(), d.R());

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

	// -------------------------------

	//
	//	RotateRightRGBChannelsOp
	//	

	template< typename DST_TYPE, typename SRC_TYPE = DST_TYPE >
	struct RotateRightRGBChannelsOp {

	public:

		typedef DST_TYPE dst_type;
		typedef SRC_TYPE src_type;

	public:

		RotateRightRGBChannelsOp() { /* Empty */ }

		__forceinline DST_TYPE& operator()(dst_type& d, const src_type& s) const {

			d = dst_type(s.B(), s.R(), s.G());

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

	// -------------------------------
	// Isolations (could/should be a single class with specializations)
	// -------------------------------

	//
	//	IsolateRChannel
	//	

	template< typename DST_TYPE, typename SRC_TYPE = DST_TYPE >
	struct IsolateRChannel {

	public:

		typedef DST_TYPE dst_type;
		typedef SRC_TYPE src_type;

	public:

		IsolateRChannel() { /* Empty */ }

		__forceinline DST_TYPE& operator()(dst_type& d, const src_type& s) const {

			d = dst_type(s.R(), 0, 0);

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

	//
	//	IsolateGChannel
	//	

	template< typename DST_TYPE, typename SRC_TYPE = DST_TYPE >
	struct IsolateGChannel {

	public:

		typedef DST_TYPE dst_type;
		typedef SRC_TYPE src_type;

	public:

		IsolateGChannel() { /* Empty */ }

		__forceinline DST_TYPE& operator()(dst_type& d, const src_type& s) const {

			d = dst_type(0, s.G(), 0);

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

	//
	//	IsolateBChannel
	//	

	template< typename DST_TYPE, typename SRC_TYPE = DST_TYPE >
	struct IsolateBChannel {

	public:

		typedef DST_TYPE dst_type;
		typedef SRC_TYPE src_type;

	public:

		IsolateBChannel() { /* Empty */ }

		__forceinline DST_TYPE& operator()(dst_type& d, const src_type& s) const {

			d = dst_type(0, 0, s.B());

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

	// --------------------------------

	//
	//	TransferOnlyRChannel
	//	

	template< typename DST_TYPE, typename SRC_TYPE = DST_TYPE >
	struct TransferOnlyRChannel {

	public:

		typedef DST_TYPE dst_type;
		typedef SRC_TYPE src_type;

	public:

		TransferOnlyRChannel() { /* Empty */ }

		__forceinline DST_TYPE& operator()(dst_type& d, const src_type& s) const {

			d.R(s.R());

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

	//
	//	TransferOnlyGChannel
	//	

	template< typename DST_TYPE, typename SRC_TYPE = DST_TYPE >
	struct TransferOnlyGChannel {

	public:

		typedef DST_TYPE dst_type;
		typedef SRC_TYPE src_type;

	public:

		TransferOnlyGChannel() { /* Empty */ }

		__forceinline DST_TYPE& operator()(dst_type& d, const src_type& s) const {

			d.G( s.G() );

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

	//
	//	TransferOnlyBChannel
	//	

	template< typename DST_TYPE, typename SRC_TYPE = DST_TYPE >
	struct TransferOnlyBChannel {

	public:

		typedef DST_TYPE dst_type;
		typedef SRC_TYPE src_type;

	public:

		TransferOnlyBChannel() { /* Empty */ }

		__forceinline DST_TYPE& operator()(dst_type& d, const src_type& s) const {

			d.B( s.B() );

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

	// ---------------------------------------------------------------------------

	//
	//	TFunctorTransferOp
	//	

	template< typename FN, typename DST_TYPE, typename SRC_TYPE = DST_TYPE >
	struct TFunctorAdaptorTransferOp {

	public:

		typedef DST_TYPE dst_type;
		typedef SRC_TYPE src_type;
		FN& op;

	public:

		TFunctorAdaptorTransferOp(FN& o) : op(o) { /* Empty */ }

		__forceinline DST_TYPE& operator()(dst_type& d, const src_type& s) const {

			op(d, s);

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

}; // namespace BPT

#endif // !defined(AFX_BPTRASTEROPS_H__9484DA55_390E_43E2_8A1B_8E9CD9234DCA__INCLUDED_)
