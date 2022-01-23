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


#if !defined(AFX_TOPFIXED5050_H__8BE0A5D7_E21B_4D2A_9BA4_52553FA6E548__INCLUDED_)
#define AFX_TOPFIXED5050_H__8BE0A5D7_E21B_4D2A_9BA4_52553FA6E548__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ============================================================================

#include "Pixels.h"
#include "MMX.h"

// ============================================================================

namespace BPT {

	// ------------------------------------------------------------------------
	// TFixed5050Primitive16bpp
	// ------------------------------------------------------------------------

	//
	//	Fixed5050Primitive16bpp
	//

	template< class DST_TYPE >
	class Fixed5050Primitive16bpp {

	public:

		__forceinline void 
		OptimizedSource( U16 * first, U16 * last, const U16 * source ) {

			// Do the odd pixel value first if there is one
			// --------------------------------------------------------
					
			int count = last - first;
					
			if ( count & 1 ) {
					
				*first = ((*first >> 1) & ~DST_TYPE::HiBitsMask) + *source++;
					
				count = count / 2;
					
				if ( !count ) {
					
					return;
					
				}
					
				++first;
					
			} else {
					
				count /= 2;
					
			}
					
			// Do the DWORD's
			// --------------------------------------------------------

#if !defined(BPTLIB_MMX_CODE)

			U32 * dst32 = reinterpret_cast<U32 *>( first );
			const U32 * src32 = reinterpret_cast<const U32 *>( source );
			U32 * end32 = dst32 + count;
					
			while ( dst32 != end32 ) {
					
				*dst32 = 
					((*dst32 >> 1) & ~(DST_TYPE::HiBitsMask | (DST_TYPE::HiBitsMask<<16))) +
					*src32++;
					
				++dst32;
					
			}

#else

			// MMX CODE
			// --------------------------------------------------------

			U32 * dst32 = reinterpret_cast<U32 *>( first );
			const U32 * src32 = reinterpret_cast<const U32 *>( source );

			if ( count & 1 ) {

				*dst32 =
					((*dst32 >> 1) & ~(DST_TYPE::HiBitsMask | (DST_TYPE::HiBitsMask<<16))) +
					*src32++;
						
				++dst32;

			}

			count /= 2;

			if ( !count ) {

				return;

			}

			// Do the QWORD's (Will eventually be MMX!)
			// --------------------------------------------------------

			if ( CMMX::Available() ) {

				LARGE_INTEGER HIMASKS;

				HIMASKS.LowPart = ~(DST_TYPE::HiBitsMask | (DST_TYPE::HiBitsMask << 16));
				HIMASKS.HighPart = ~(DST_TYPE::HiBitsMask | (DST_TYPE::HiBitsMask << 16));

				_asm {

					mov		esi, src32			;; source pointer
					mov		edi, dst32			;; destination pointer
					mov		eax, count			;; end of the source pointer
					movq	mm2, qword ptr [ HIMASKS ]	;; get the mask bits

				START:

					movq	mm1, [ edi ]		;; read 4 dest pixels
					add		edi, 8				;; advance dest pointer
												
					movq	mm0, [ esi ]		;; read 4 source pixels
					psrlw	mm1, 1				;; reduce dest percision
												
					add		esi, 8				;; advance source pointer
					pand	mm1, mm2			;; truncate the hi bits
												
					paddw	mm1, mm0			;; combine
					dec		eax					;; decrement loop counter

					movq	[ edi - 8 ], mm1	;; write mixed dest value
					jnz		START				;; loop back for more

					emms						;; 50 cycles OUCH!

				}

				// FUN!

				return;

			}

			// Just in case the MMX code isn't available 
			// ----------------------------------------------------------------

			U32 * end32 = dst32 + (count * 2);

			while ( dst32 != end32 ) {
					
				*dst32 = 
					((*dst32 >> 1) & ~(DST_TYPE::HiBitsMask | (DST_TYPE::HiBitsMask<<16))) +
					*src32++;
					
				++dst32;
					
				*dst32 = 
					((*dst32 >> 1) & ~(DST_TYPE::HiBitsMask | (DST_TYPE::HiBitsMask<<16))) +
					*src32++;
					
				++dst32;

			}

#endif

		}

		__forceinline void 
		UnOptimizedSource( U16 * first, U16 * last, const U16 * source ) {

			// Do the odd pixel value first if there is one
			// --------------------------------------------------------
					
			int count = last - first;
					
			if ( count & 1 ) {
					
				*first = 
					((*first >> 1) & ~DST_TYPE::HiBitsMask) +
					((*source >> 1) & ~DST_TYPE::HiBitsMask) ;
					
				count = count / 2;
					
				if ( !count ) {
					
					return;
					
				}
					
				++first;
				++source;
					
			} else {
					
				count /= 2;
					
			}
					
			// Do the DWORD's
			// --------------------------------------------------------
					
#if !defined(BPTLIB_MMX_CODE)

			U32 * dst32 = reinterpret_cast<U32 *>( first );
			const U32 * src32 = reinterpret_cast<const U32 *>( source );
			U32 * end32 = dst32 + count;
					
			while ( dst32 != end32 ) {
					
				*dst32 = 
					((*dst32 >> 1) & ~(DST_TYPE::HiBitsMask | (DST_TYPE::HiBitsMask<<16))) +
					((*src32 >> 1) & ~(DST_TYPE::HiBitsMask | (DST_TYPE::HiBitsMask<<16)));
					
				++dst32;
				++src32;
					
			}

#else

			// MMX CODE
			// --------------------------------------------------------

			U32 * dst32 = reinterpret_cast<U32 *>( first );
			const U32 * src32 = reinterpret_cast<const U32 *>( source );

			if ( count & 1 ) {

				*dst32 =
					((*dst32 >> 1) & ~(DST_TYPE::HiBitsMask | (DST_TYPE::HiBitsMask<<16))) +
					((*src32 >> 1) & ~(DST_TYPE::HiBitsMask | (DST_TYPE::HiBitsMask<<16)));
						
				++dst32;
				++src32;

			}

			count /= 2;

			if ( !count ) {

				return;

			}

			// Do the QWORD's (Will eventually be MMX!)
			// --------------------------------------------------------

			if ( 1 ) { // CMMX::Available() ) {

				LARGE_INTEGER HIMASKS;

				HIMASKS.LowPart = ~(DST_TYPE::HiBitsMask | (DST_TYPE::HiBitsMask << 16));
				HIMASKS.HighPart = ~(DST_TYPE::HiBitsMask | (DST_TYPE::HiBitsMask << 16));

				_asm {

					mov		esi, src32			;; source pointer
					mov		edi, dst32			;; destination pointer
					mov		eax, count			;; end of the source pointer
					movq	mm2, qword ptr [ HIMASKS ]	;; get the mask bits

				START:

					movq	mm1, [ edi ]		;; read 4 dest pixels
					movq	mm0, [ esi ]		;; read 4 source pixels

					psrlw	mm1, 1				;; reduce dest percision
					psrlw	mm0, 1				;; reduce dest percision

					pand	mm1, mm2			;; truncate the hi bits
					pand	mm0, mm2			;; truncate the hi bits
												
					add		edi, 8				;; advance dest pointer
					add		esi, 8				;; advance source pointer

					paddw	mm1, mm0			;; combine
					dec		eax					;; decrement loop counter

					movq	[ edi - 8 ], mm1	;; write mixed dest value
					jnz		START				;; loop back for more

					emms						;; 50 cycles OUCH!

				}

				// FUN!

				return;

			}

			// Just in case the MMX code isn't available 
			// ----------------------------------------------------------------

			U32 * end32 = dst32 + (count * 2);

			while ( dst32 != end32 ) {
					
				*dst32 = 
					((*dst32 >> 1) & ~(DST_TYPE::HiBitsMask | (DST_TYPE::HiBitsMask<<16))) +
					((*src32 >> 1) & ~(DST_TYPE::HiBitsMask | (DST_TYPE::HiBitsMask<<16)));
					
				++dst32;
				++src32;
					
				*dst32 = 
					((*dst32 >> 1) & ~(DST_TYPE::HiBitsMask | (DST_TYPE::HiBitsMask<<16))) +
					((*src32 >> 1) & ~(DST_TYPE::HiBitsMask | (DST_TYPE::HiBitsMask<<16)));
					
				++dst32;
				++src32;

			}

#endif

		}

	};

	// ------------------------------------------------------------------------
	//	MIX 50:50
	// ------------------------------------------------------------------------

	//
	//	TFixed5050OP
	//	

	template< class DST_TYPE, class SRC_TYPE = DST_TYPE >
	class TFixed5050OP {

	public:

		typedef DST_TYPE dst_type;
		typedef SRC_TYPE src_type;

		__forceinline DST_TYPE & operator()( dst_type & d, const src_type & s ) const {

			DST_TYPE t = s;

			if ( src_type::optimized ) {

				d.value = ((d.value >> 1) & ~dst_type::HiBitsMask) + t.value;

			} else {

				d.value = 
					((d.value >> 1) & ~dst_type::HiBitsMask) +
					((t.value >> 1) & ~dst_type::HiBitsMask) ;

			}

			return d;

		}

		// [first, last) write value 's'
		// --------------------------------------------------------------------

		template< class OutputIt >
		__forceinline void OutputMajor( OutputIt first, OutputIt last, const src_type & s ) const {

			DST_TYPE t = s;

			int processed = (src_type::optimized) ? t.value : ((t.value >> 1) & ~dst_type::HiBitsMask);

			while ( first != last ) {

				first->value = ((first->value >> 1) & ~dst_type::HiBitsMask) + processed;

				++first;

			}

		}

		// Specialzation
		// --------------------------------------------------------------------

	private:

		template<bool SIMDTEST> struct Specialized_InputMajor {
			template< class ForwardIt, class OutputIt > __forceinline
			void Action( ForwardIt first, ForwardIt last, OutputIt output ) const {
				/* Empty */
			}
		};

		// (NON-SIMD) ---------------------------------------------------------

		template<> struct Specialized_InputMajor<false> {

			template< class ForwardIt, class OutputIt > __forceinline
			void Action( ForwardIt first, ForwardIt last, OutputIt output ) const {

				if ( src_type::optimized ) {

					DST_TYPE t;

					while ( first != last ) {
		
						t = *first++;
		
						output->value = ((output->value >> 1) & ~dst_type::HiBitsMask) + t.value;
		
						++output;
		
					}

				} else {

					DST_TYPE t;

					while ( first != last ) {
		
						t = *first++;
		
						output->value = 
							((output->value >> 1) & ~dst_type::HiBitsMask) +
							((t.value >> 1) & ~dst_type::HiBitsMask) ;
		
						++output;
		
					}

				}

			}

		};

		// (TRY-SIMD) ---------------------------------------------------------

/*

	NOTE the interior of this Action should be done in another class
	or template function as it stands its too big!!!

*/

		template<> struct Specialized_InputMajor<true> {

			template< class ForwardIt, class OutputIt > __forceinline
			void Action( ForwardIt _first, ForwardIt _last, OutputIt _output ) const {

				// Change the type of the param's to U16's (could go horribly wrong)
				// Change the type of the param's to U16's (could go horribly wrong)
				// Change the type of the param's to U16's (could go horribly wrong)
				// Change the type of the param's to U16's (could go horribly wrong)

				U16 * first = reinterpret_cast<U16 *>( _first );
				U16 * last = reinterpret_cast<U16 *>( _last );
				U16 * output = reinterpret_cast<U16 *>( &(*_output) );

				// ------------------------------------------------------------

				Fixed5050Primitive16bpp<dst_type> primitive;

				if ( src_type::optimized ) {

					primitive.OptimizedSource( output, output + (last - first), first );
	
				} else {
	
					primitive.UnOptimizedSource( output, output + (last - first), first );

				}

			}

		};

	public:

		// [first, last) to output (same semantics as std::copy())
		// --------------------------------------------------------------------

		template< class ForwardIt, class OutputIt >
		__forceinline void InputMajor( ForwardIt first, ForwardIt last, OutputIt output ) const {

			// Hack test to see if we have a forward iterators (could be horribly wrong)
			// Hack test to see if we have a forward iterators (could be horribly wrong)
			// Hack test to see if we have a forward iterators (could be horribly wrong)
			// ----------------------------------------------------------------

			dst_type * a = &(*output);
			dst_type * b = &(*(output + 1));
			src_type * c = &(*first);
			src_type * d = &(*(first + 1));

			// ----------------------------------------------------------------

			if ( (1 == (b - a)) && (1 == (d - c)) ) {
			
				Specialized_InputMajor<
					(src_type::specialization_type::value == dst_type::specialization_type::value) &&
					(P16_tag::value == dst_type::specialization_type::value)
				>dummy_var;
				
				dummy_var.Action( first, last, output );

			} else {

				Specialized_InputMajor<false>dummy_var;
				dummy_var.Action( first, last, output );

			}

		}

	};

	// ------------------------------------------------------------------------
	//	
	//	SHADE Dest to 50%
	//	
	//	-- I really need to build a better unary operator concept!
	//	
	// ------------------------------------------------------------------------

	//
	//	T50PercentDestShadeOP
	//	

	template< class DST_TYPE, class SRC_TYPE = DST_TYPE >
	class T50PercentDestShadeOP {

	public:

		typedef DST_TYPE dst_type;
		typedef SRC_TYPE src_type;

		// Specialzation
		// --------------------------------------------------------------------

	private:

		template<bool SIMDTEST> struct Specialized_UnaryOp {
			template< class OutputIt > __forceinline
			void Action( OutputIt first, OutputIt last ) const {
				/* Empty */
			}
		};

		// (NON-SIMD) ---------------------------------------------------------

		template<> struct Specialized_UnaryOp<false> {

			template< class OutputIt > __forceinline
			void Action( OutputIt first, OutputIt last ) const {

				while ( first != last ) {
		
					first->value = ((first->value >> 1) & ~dst_type::HiBitsMask);

					++first;

				}

			}

		};

		// (TRY-SIMD) ---------------------------------------------------------

/*

	NOTE the interior of this Action should be done in another class
	or template function as it stands its too big!!!

*/

		template<> struct Specialized_UnaryOp<true> {

			template< class OutputIt > __forceinline
			void Action( OutputIt _first, OutputIt _last ) const {

				// Change the type of the param's to U16's (could go horribly wrong)
				// Change the type of the param's to U16's (could go horribly wrong)
				// Change the type of the param's to U16's (could go horribly wrong)
				// Change the type of the param's to U16's (could go horribly wrong)

				U16 * output = reinterpret_cast<U16 *>( &(*_first) );

				// Do the odd pixel value first if there is one
				// ------------------------------------------------------------
		
				int count = _last - _first;
		
				if ( count & 1 ) {
		
					*output = ((*output >> 1) & ~dst_type::HiBitsMask);
		
					count = count / 2;
		
					if ( !count ) {
		
						return;
		
					}
		
					++output;
		
				} else {
		
					count /= 2;
		
				}
		
				// Do the DWORD's
				// ------------------------------------------------------------
		
				U32 * dst = reinterpret_cast<U32 *>( output );
				U32 * end = dst + count;
		
				while ( dst != end ) {
		
					*dst = ((*dst >> 1) & ~(dst_type::HiBitsMask | (dst_type::HiBitsMask<<16)));
		
					++dst;
	
				}

			}

		};

	public:

		// [first, last) to output (same semantics as std::copy())
		// --------------------------------------------------------------------

		template< class OutputIt >
		__forceinline void UnaryOp( OutputIt first, OutputIt last ) const {

			// Hack test to see if we have a forward iterators (could be horribly wrong)
			// Hack test to see if we have a forward iterators (could be horribly wrong)
			// Hack test to see if we have a forward iterators (could be horribly wrong)
			// ----------------------------------------------------------------

			dst_type * a = &(*first);
			dst_type * b = &(*(first + 1));

			// ----------------------------------------------------------------

			if ( (1 == (b - a)) ) {
			
				Specialized_UnaryOp<
					(P16_tag::value == dst_type::specialization_type::value)
				> dummy_var;
				
				dummy_var.Action( first, last );

			} else {

				Specialized_UnaryOp<false> dummy_var;
				dummy_var.Action( first, last );

			}

		}

		// Single operators
		// --------------------------------------------------------------------

		__forceinline DST_TYPE & operator()( dst_type & d, const src_type & s ) const {

			d.value = ((d.value >> 1) & ~dst_type::HiBitsMask);

			return d;

		}

		__forceinline DST_TYPE & operator()( dst_type & d ) const {

			d.value = ((d.value >> 1) & ~dst_type::HiBitsMask);

			return d;

		}

		// [first, last) to output (same semantics as std::copy())
		// --------------------------------------------------------------------

		template< class ForwardIt, class OutputIt >
		__forceinline void InputMajor( ForwardIt first, ForwardIt last, OutputIt output ) const {

			UnaryOp( output, output + (last - first) );

		}

		// [first, last) write value 's'
		// --------------------------------------------------------------------

		template< class OutputIt >
		__forceinline void OutputMajor( OutputIt first, OutputIt last, const src_type & s ) const {

			UnaryOp( first, last );

		}

	};

	// ========================================================================

	//
	//	TFixed5050Blitter<>
	//
	//	Should work with most pixel types in 'Pixels.h'
	//
	//

	template< class T >
	class TFixed5050Blitter {

	private:

		int m_Chromakey;
		int m_Chromakey_simd32;

		enum {

			HiBits = T::HiBitsMask
			,HiBits_simd32 = HiBits | (HiBits << 16)

		};

	public:

		TFixed5050Blitter( const int chromakey = 0 ) {

			SetChromakey( chromakey );
			
		}

		void SetChromakey( const int chromakey ) {

			m_Chromakey = chromakey;
			m_Chromakey_simd32 = chromakey | (chromakey << 16);

		}

		__forceinline void operator()(
			T * dst, const int dPitch, const T * src, const int sPitch,
			const int width, const int height
		) {

			// Setup the pointers
			// ----------------------------------------------------------------

			int byteWidth = (width * sizeof(T));
			int dstOffset = dPitch - byteWidth;
			int srcOffset = sPitch - byteWidth;

			// Line by line operation
			// ----------------------------------------------------------------

#if 1

			dstOffset += (width & 1) * sizeof(T);
			srcOffset += (width & 1) * sizeof(T);

			int hCount = width / 2;
			typedef const DWORD * SPTR;
			typedef DWORD * DPTR;

#else

			int hCount = width;
			typedef const T::value_type * SPTR;
			typedef T::value_type * DPTR;

#endif

			SPTR srcP = reinterpret_cast<SPTR>( src );
			DPTR dstP = reinterpret_cast<DPTR>( dst );

#if defined(BPTLIB_MMX_CODE)
			
			int theWidth = width / 4;

			if ( !theWidth ) return /* NOP */;

			dstOffset = dPitch - ((theWidth * 4) * sizeof(T));
			srcOffset = sPitch - ((theWidth * 4) * sizeof(T));

			// ----------------------------------------------------------------

			LARGE_INTEGER LoBits_mmx = { ~HiBits_simd32, ~HiBits_simd32 };

// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

			LARGE_INTEGER chromakey_mmx = { m_Chromakey_simd32, m_Chromakey_simd32 };

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
																    
			// ----------------------------------------------------------------

			__asm {

				mov		edi, dstP					;; dest pointer
				mov		esi, srcP 					;; source pointer

				mov		ecx, theWidth				;; operation width / 4
				mov		ebx, height					;; height

				mov		edx, dstOffset				;; dest scanline adjustment
				mov		eax, srcOffset				;; source scanline adjustment

// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

#if 1

				movq	mm5, chromakey_mmx			;; read the 'chromakey' for mask build op
				movq	mm6, chromakey_mmx			;; read the 'chromakey' for mask build op

#endif

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
				
				movq	mm1, LoBits_mmx				;; high bits mask

//				align	16

			LOOPPOINT:

				movq	mm3, [ edi ]				;; read 4 dest pixels
				add		edi, 8						;; advance dest pointer

				movq	mm0, [ esi ]				;; read 4 source pixels
				add		esi, 8						;; advance source ptr

// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

#if 1

				movq	mm4, mm3					;; copy dest
				pcmpeqw	mm5, mm0					;; build the operation mask

				pand	mm4, mm5					;; keep the areas where the source is transparent

#endif

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

				psrlw	mm3, 1						;; reduce percision
				psrlw	mm0, 1						;; ""

				pand	mm3, mm1					;; remove highbits
				pand	mm0, mm1					;; ""

				paddw	mm0, mm3

// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

#if 1

				pandn	mm5, mm0
				por		mm5, mm4

				dec		ecx							;; cross off another iteration
				movq	[ edi - 8 ], mm5			;; write 4 blended pixels
				movq	mm5, mm6

#else

				dec		ecx							;; cross off another iteration
				movq	[ edi - 8 ], mm0			;; write 4 blended pixels

#endif

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

				jnz		LOOPPOINT					;; loop until done a whole span
				dec		ebx							;; cross off a whole span
				jz		DONE						;; if done bail
				add		edi, edx					;; next dest line
				add		esi, eax					;; next source 
				mov		ecx, theWidth 				;; reset counter

				jmp		LOOPPOINT

			DONE:
				
				emms

			}

#else

			for ( int y = height; --y >= 0; ) {

				for ( int x = hCount; --x >= 0; ) {

					// Fetch the source & dest values

					int s = *srcP++;
					int d = *dstP++;

					int sr = (s & R_Mask_simd32) >> T::R_L_Shift;
					int sg = (s & G_Mask_simd32) >> T::G_L_Shift;
					int sb = (s & B_Mask_simd32) >> T::B_L_Shift;

					int dr = (d & R_Mask_simd32) >> T::R_L_Shift;
					int dg = (d & G_Mask_simd32) >> T::G_L_Shift;
					int db = (d & B_Mask_simd32) >> T::B_L_Shift;

					*(dstP - 1) = 
						((((((((sr + PositiveAdjust_simd32) - dr) * m_Alpha) >> 8) + dr) - m_PostAdjust_simd32) << T::R_L_Shift) & R_Mask_simd32) +
						((((((((sg + PositiveAdjust_simd32) - dg) * m_Alpha) >> 8) + dg) - m_PostAdjust_simd32) << T::G_L_Shift) & G_Mask_simd32) +
						((((((((sb + PositiveAdjust_simd32) - db) * m_Alpha) >> 8) + db) - m_PostAdjust_simd32) << T::B_L_Shift) & B_Mask_simd32) ;

				}

				// Step the pointers
				// ------------------------------------------------------------

				dstP = reinterpret_cast<DPTR>(
					reinterpret_cast<BYTE *>( dstP ) + dstOffset
				);

				srcP = reinterpret_cast<SPTR>(
					reinterpret_cast<const BYTE *>( srcP ) + srcOffset
				);

			}

#endif

		}

	};

}; // namespace BPT

#endif // !defined(AFX_TOPFIXED5050_H__8BE0A5D7_E21B_4D2A_9BA4_52553FA6E548__INCLUDED_)
