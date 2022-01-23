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


/*

	NOTES

		Each of these Transfer operations really deserve their own
		header & implimentation file.  As these grow in number and
		complexity this will become more of an issue.


*/

#if !defined(AFX_TROPS_H__E061B797_4024_44EF_9208_7DCD0ACED58A__INCLUDED_)
#define AFX_TROPS_H__E061B797_4024_44EF_9208_7DCD0ACED58A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ============================================================================

#define BPTLIB_DEFINE_SPECIALIZATION_TEMPLATES

#include "Pixels.h"
#include "TOPFixed5050.h"
#include "TOPCopy.h"
#include "TOPAlpha.h"
#include "TOPAdditive.h"
#include "TClut.h"

// ============================================================================

namespace BPT {

	// ------------------------------------------------------------------------
	// Basic ROP's
	// ------------------------------------------------------------------------

	//
	//	TTransparentSrcTransferROP
	//	

	template< typename DST_TYPE, typename SRC_TYPE = DST_TYPE >
	class TTransparentSrcTransferROP {

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

		template< class ForwardIt, class OutputIt >
		__forceinline void InputMajor( ForwardIt first, ForwardIt last, OutputIt output ) const {

			while ( first != last ) {

				SRC_TYPE s = *first++;

				if ( m_ColorKey != s ) {

					*output++ = s;

				}

			}

		}

		// [first, last) write value 's'
		// --------------------------------------------------------------------

		template< class OutputIt >
		__forceinline void OutputMajor( OutputIt first, OutputIt last, const src_type & s ) const {

			if ( m_ColorKey != s ) {

				while ( first != last ) {
	
					*first++ = s;
	
				}

			}

		}

	};

	//
	//	TClutTransferROP
	//	

	template<
		class DST_TYPE
		,class SRC_TYPE = DST_TYPE
		,class CLUT_TYPE = TPow2Clut< DST_TYPE >       
	>
	class TClutTransferROP {

	private:

		CLUT_TYPE * m_pClut;

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

		template< class ForwardIt, class OutputIt >
		__forceinline void InputMajor( ForwardIt first, ForwardIt last, OutputIt output ) const {

			while ( first != last ) {

				*output++ = (*m_pClut)[ *first++ ];

			}

		}

		// [first, last) write value 's'
		// --------------------------------------------------------------------

		template< class OutputIt >
		__forceinline void OutputMajor( OutputIt first, OutputIt last, const src_type & s ) const {

			dst_type out = (*m_pClut)[ s ];

			while ( first != last ) {

				*first++ = out;

			}

		}

	};

	// ------------------------------------------------------------------------
	// Cheap transfer operators
	// ------------------------------------------------------------------------

	//
	//	TMultiplyDstBySrc
	//	

	template< typename DST_TYPE, typename SRC_TYPE = DST_TYPE >
	class TMultiplyDstBySrc {

	public:

		typedef DST_TYPE dst_type;
		typedef SRC_TYPE src_type;

		__forceinline DST_TYPE & operator()( dst_type & d, const src_type & s ) const {

			int value = d.value;

			d._R( 
				min( dst_type::R_LimitedMax, (d._R() * s._R()) >> (src_type::R_BitCount - 1))
			);

			d._G(
				min( dst_type::G_LimitedMax, (d._G() * s._G()) >> (src_type::G_BitCount - 1))
			);

			d._B(
				min( dst_type::G_LimitedMax, (d._B() * s._B()) >> (src_type::B_BitCount - 1))
			);

			return d;

		}

		// [first, last) to output (same semantics as std::copy())
		// --------------------------------------------------------------------

		template< class ForwardIt, class OutputIt >
		__forceinline void InputMajor( ForwardIt first, ForwardIt last, OutputIt output ) const {

			while ( first != last ) {

				(*this)( *output++, *first++ );

			}

		}

		// [first, last) write value 's'
		// --------------------------------------------------------------------

		template< class OutputIt >
		__forceinline void OutputMajor( OutputIt first, OutputIt last, const src_type & s ) const {

			// Perform the operation on all the dest values.
			// ----------------------------------------------------------------

			while ( first != last ) {

				(*this)( *first++, s );

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
		, typename TRANSFER_OP = typename TCopyROP< DST_TYPE >
	>
	class TSrcPredicateClutTransfer {

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

			while ( first != last ) {

				SRC_TYPE s = *first++;

				if ( m_Predicate( s ) ) {

					m_Op( *output++, (*m_pClut)[ s ] );

				}

			}

		}

		// [first, last) write value 's'
		// --------------------------------------------------------------------

		template< typename OutputIt >
		__forceinline void OutputMajor( OutputIt first, OutputIt last, const src_type & s ) const {

			if ( m_Predicate( s ) ) {

				m_Op.OutputMajor( first, last, (*m_pClut)[ s ] );
	
			}

		}

	};

	// ------------------------------------------------------------------------

	//
	//	TTableDrivenScaleUpOP<>
	//	

	template<
		typename DST_TYPE
		, typename SRC_TYPE = DST_TYPE
		, typename SCALE_TABLE_TYPE = BYTE *
		, typename TRANSFER_OP = typename TCopyROP< DST_TYPE >
	>
	class TTableDrivenScaleUpOP {

	public:

		typedef DST_TYPE dst_type;
		typedef SRC_TYPE src_type;

	private:

		SCALE_TABLE_TYPE m_ScaleTable;
		TRANSFER_OP m_Op;

		TTableDrivenScaleUpOP(); // Hidden

	public:

		TTableDrivenScaleUpOP(
			SCALE_TABLE_TYPE scaleTable, TRANSFER_OP op = TRANSFER_OP()
		) : m_ScaleTable( scaleTable), m_Op( op ) { /* Empty */ }

		__forceinline DST_TYPE & operator()( dst_type & d, const src_type & s ) const {

			m_Op( d, s );

			return d;

		}

		// [first, last) to output
		// --------------------------------------------------------------------

		template< typename ForwardIt, typename OutputIt >
		__forceinline void InputMajor( ForwardIt first, ForwardIt last, OutputIt output ) const {

			SCALE_TABLE_TYPE pTable = m_ScaleTable;

			while ( first != last ) {

				m_Op( *output++, *first );

				if ( *pTable++ ) {

					++first;

				}

			}

		}

		// [first, last) write, read from input
		// --------------------------------------------------------------------

		template< typename OutputIt, typename ForwardIt >
		__forceinline void InputMinor( OutputIt first, OutputIt last, ForwardIt input ) const {

			SCALE_TABLE_TYPE pTable = m_ScaleTable;

			while ( first != last ) {

				m_Op( *first++, *input );

				if ( *pTable++ ) {

					++input;

				}

			}

		}

		// [first, last) write value 's'
		// --------------------------------------------------------------------

		template< typename OutputIt >
		__forceinline void OutputMajor( OutputIt first, OutputIt last, const src_type & s ) const {

			m_Op.OutputMajor( first, last, s );

		}

	};

}; // namespace BPT

#endif // !defined(AFX_TROPS_H__E061B797_4024_44EF_9208_7DCD0ACED58A__INCLUDED_)
