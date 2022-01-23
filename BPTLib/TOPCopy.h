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


#if !defined(AFX_TOPCOPY_H__EDA74F1A_FF0C_414F_B999_5D325FEE13B1__INCLUDED_)
#define AFX_TOPCOPY_H__EDA74F1A_FF0C_414F_B999_5D325FEE13B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ============================================================================

#include "Pixels.h"

// ============================================================================

namespace BPT {

	// ------------------------------------------------------------------------
	//	'memset' helper
	// ------------------------------------------------------------------------

	template< typename T >
	class TMemsetHelper {

	public: // traits

		template<int> struct Specialized_Memset {
			template< class OutputIt, class src_type > __forceinline
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

			template< class OutputIt, class src_type > __forceinline
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

				U32 combined = (U32)s | ((U32)s << 16);
	
				U32 * dst = reinterpret_cast<U32 *>( first );
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

	public: // 'interface'

		template< typename OutputIt, typename src_type > __forceinline
		void op( OutputIt first, OutputIt last, const src_type & s ) {

			Specialized_Memset<sizeof(src_type)> dummy_var;
			dummy_var.Action( first, last, s );

		}

	};

	// ------------------------------------------------------------------------

	//
	//	TCopyOP
	//	

	template< typename DST_TYPE, typename SRC_TYPE = DST_TYPE >
	class TCopyOP {

	public:

		typedef DST_TYPE dst_type;
		typedef SRC_TYPE src_type;

		// --------------------------------------------------------------------

		__forceinline DST_TYPE & operator()( dst_type & d, const src_type & s ) const {

			d = s;

			return d;

		}

		// [first, last) write value 's'
		// --------------------------------------------------------------------

		template< typename OutputIt >
		__forceinline void OutputMajor( OutputIt first, OutputIt last, const src_type & s ) const {

			dst_type d = s;

			src_type * a = &(*first);
			src_type * b = &(*(first + 1));

			// ----------------------------------------------------------------

			if ( (1 == (b - a)) && (src_type::format == dst_type::format) ) {
			
				TMemsetHelper<dst_type> helper;

				helper.op( first, last, s );

			} else {

				while ( first != last ) {

					*first++ = d;

				}

			}

		}

#if defined(BPTLIB_DEFINE_SPECIALIZATION_TEMPLATES)

		// Here is a try at template specialization for 16 bit processing
		// --------------------------------------------------------------------

		template<> __forceinline void OutputMajor<U16 *>(
			U16 * first, U16 * last, U16 value ) const {

			// Do the odd pixel value first if there is one
			// ----------------------------------------------------------------

			int count = last - first;

			if ( count & 1 ) {

				*first = value;

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

			U32 combined = (U32)value | ((U32)value << 16);

			U32 * dst = reinterpret_cast<U32 *>( first );
			U32 * end = dst + count;

			while ( dst != end ) {

				*dst++ = combined;

			}

		}

		// Specific specialization for some 16bpp pixel types. (There has to
		// be a more eligant way to do this!)
		// --------------------------------------------------------------------

		template<> __forceinline void OutputMajor<PIXEL_A1R5G5B5 *>(
			PIXEL_A1R5G5B5 * first, PIXEL_A1R5G5B5 * last, const PIXEL_A1R5G5B5 & s ) const {

			OutputMajor(
				reinterpret_cast<U16*>(first), 
				reinterpret_cast<U16*>(last), 
				s.value
			);

		}

		template<> __forceinline void OutputMajor<PIXEL_B5G5R5A1 *>(
			PIXEL_B5G5R5A1 * first, PIXEL_B5G5R5A1 * last, const PIXEL_B5G5R5A1 & s ) const {

			OutputMajor(
				reinterpret_cast<U16*>(first), 
				reinterpret_cast<U16*>(last), 
				s.value
			);

		}

		template<> __forceinline void OutputMajor<PIXEL_R5G6B5 *>(
			PIXEL_R5G6B5 * first, PIXEL_R5G6B5 * last, const PIXEL_R5G6B5 & s ) const {

			OutputMajor(
				reinterpret_cast<U16*>(first), 
				reinterpret_cast<U16*>(last), 
				s.value
			);

		}

		template<> __forceinline void OutputMajor<PIXEL_B5G6R5 *>(
			PIXEL_B5G6R5 * first, PIXEL_B5G6R5 * last, const PIXEL_B5G6R5 & s ) const {

			OutputMajor(
				reinterpret_cast<U16*>(first), 
				reinterpret_cast<U16*>(last), 
				s.value
			);

		}

#endif

		// Specialzation
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

			// Hack test to see if we have a forward iterator (could be horribly wrong)
			// Hack test to see if we have a forward iterator (could be horribly wrong)
			// Hack test to see if we have a forward iterator (could be horribly wrong)
			// ----------------------------------------------------------------

			dst_type * a = &(*output);
			dst_type * b = &(*(output + 1));
			src_type * c = &(*first);
			src_type * d = &(*(first + 1));

			// ----------------------------------------------------------------

			if ( (1 == (b - a)) && (1 == (d - c)) ) {
			
				Specialized_InputMajor<(src_type::format == dst_type::format)>dummy_var;
				dummy_var.Action( first, last, output );

			} else {

				Specialized_InputMajor<false>dummy_var;
				dummy_var.Action( first, last, output );

			}

		}

	};

}; // namespace BPT

#endif // !defined(AFX_TOPCOPY_H__EDA74F1A_FF0C_414F_B999_5D325FEE13B1__INCLUDED_)
