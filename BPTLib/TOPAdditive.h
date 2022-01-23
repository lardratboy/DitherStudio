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


#if !defined(AFX_TOPADDITIVE_H__D74581BA_27F5_464B_8A99_B01E3DFE81B3__INCLUDED_)
#define AFX_TOPADDITIVE_H__D74581BA_27F5_464B_8A99_B01E3DFE81B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ============================================================================

#include "Pixels.h"
#include "MMX.h" 

// ============================================================================

namespace BPT {

	// ========================================================================

	//
	//	TAdditiveBlitter<>
	//
	//	Should work with most pixel types in 'Pixels.h'
	//
	//	-- Same type External alpha blit
	//

	template< class T >
	class TAdditiveBlitter {

	private:

		int m_Chromakey;
		int m_Chromakey_simd32;

		enum {

			// Shifted r, g, b masks
			// ----------------------------------------------------------------

			R_Mask = T::R_BitMask
			,G_Mask = T::G_BitMask
			,B_Mask = T::B_BitMask

			// The simd versions are only valid for <= 16bpp formats.
			// ----------------------------------------------------------------

			,R_Mask_simd32 = R_Mask | (R_Mask << 16)
			,G_Mask_simd32 = G_Mask | (G_Mask << 16)
			,B_Mask_simd32 = B_Mask | (B_Mask << 16)

		};

	public:

		TAdditiveBlitter( const int chromakey = 0 ) {

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

			int hCount = width;
			typedef const T::value_type * SPTR;
			typedef T::value_type * DPTR;

			SPTR srcP = reinterpret_cast<SPTR>( src );
			DPTR dstP = reinterpret_cast<DPTR>( dst );

#if defined(BPTLIB_MMX_CODE)
			
			int theWidth = width / 4;

			if ( !theWidth ) return /* NOP */;

			dstOffset = dPitch - ((theWidth * 4) * sizeof(T));
			srcOffset = sPitch - ((theWidth * 4) * sizeof(T));

			// ----------------------------------------------------------------

			LARGE_INTEGER rmask_mmx = { R_Mask_simd32, R_Mask_simd32 };
			LARGE_INTEGER gmask_mmx = { G_Mask_simd32, G_Mask_simd32 };
			LARGE_INTEGER bmask_mmx = { B_Mask_simd32, B_Mask_simd32 };
																    
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

			LARGE_INTEGER chromakey_mmx = { m_Chromakey_simd32, m_Chromakey_simd32 };
			LARGE_INTEGER transparent_mask_mmx = { 0, 0 }; 
			LARGE_INTEGER dest_areas_mmx = { 0, 0 };

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

			enum {

				R_CHANNEL_SHIFT		= (16 - (T::R_L_Shift + T::R_BitCount))
				,G_CHANNEL_SHIFT	= (16 - (T::G_L_Shift + T::G_BitCount))
				,B_CHANNEL_SHIFT	= (16 - (T::B_L_Shift + T::B_BitCount))

			};

			// ----------------------------------------------------------------

			__asm {

				mov		edi, dstP					;; dest pointer
				mov		esi, srcP 					;; source pointer
				mov		ecx, theWidth				;; operation width / 4
				mov		ebx, height					;; height
				mov		edx, dstOffset				;; dest scanline adjustment
				mov		eax, srcOffset				;; source scanline adjustment
				movq	mm6, rmask_mmx				;; read (R) mask bits
				movq	mm7, gmask_mmx				;; read (G) mask bits

// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

#if 1

				movq	mm5, chromakey_mmx			;; read the 'chromakey' for mask build op
				nop									;; pair helper

#endif

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

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
				movq	transparent_mask_mmx, mm5	;; store off the results for later use

				movq	dest_areas_mmx, mm4			;; transparent replacement areas
				nop									;; pair helper

#endif

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

				movq	mm1, mm0					;; src copy
				movq	mm2, mm0					;; ""

				movq	mm4, mm3					;; dst copy
				movq	mm5, mm3					;; ""

				pand	mm0, mm6					;; isolate the (R) channel(s)
				pand	mm3, mm6					;; ""

				pand	mm1, mm7					;; isolate the (G) channel(s)
				pand	mm4, mm7					;; ""

				movq	mm6, bmask_mmx				;; read (B) mask bits
				nop									;; pair helper

				psllw	mm0, R_CHANNEL_SHIFT		;; move the (R) bits up
				psllw	mm3, R_CHANNEL_SHIFT		;; ""

				pand	mm2, mm6					;; isolate the (B) channel(s)
				pand	mm5, mm6					;; ""
				
				psllw	mm1, G_CHANNEL_SHIFT		;; move the (G) bits up
				psllw	mm4, G_CHANNEL_SHIFT		;; ""

				psllw	mm2, B_CHANNEL_SHIFT		;; move the (B) bits up
				psllw	mm5, B_CHANNEL_SHIFT		;; ""

				paddusw	mm0, mm3					;; add (R) channel(s)
				paddusw	mm1, mm4					;; add (G) channel(s)

				paddusw	mm2, mm5					;; add (B) channel(s)
				psrlw	mm0, R_CHANNEL_SHIFT		;; move the (R) bits back into place

				psrlw	mm1, G_CHANNEL_SHIFT		;; move the (G) bits back into place
				psrlw	mm2, B_CHANNEL_SHIFT		;; move the (B) bits back into place

				pand	mm2, mm6					;; isolate the (B) channel(s)
				pand	mm1, mm7					;; isolate the (G) channel(s)

				movq	mm6, rmask_mmx				;; read the (R) mask bits
				paddw	mm1, mm2					;; combine the (B) + (G) channel(s)

				pand	mm0, mm6
				paddw	mm0, mm1					;; combine the (GB) + (R) channel(s)

// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

#if 1

				movq	mm1, transparent_mask_mmx
				movq	mm5, chromakey_mmx

				pandn	mm1, mm0
				por		mm1, dest_areas_mmx

				dec		ecx							;; cross off another iteration
				movq	[ edi - 8 ], mm1			;; write 4 blended pixels

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

					output->value = 
						min( dst_type::A_BitMask, ((d & dst_type::A_BitMask) + (s & dst_type::A_BitMask)) ) +
						min( dst_type::R_BitMask, ((d & dst_type::R_BitMask) + (s & dst_type::R_BitMask)) ) +
						min( dst_type::G_BitMask, ((d & dst_type::G_BitMask) + (s & dst_type::G_BitMask)) ) +
						min( dst_type::B_BitMask, ((d & dst_type::B_BitMask) + (s & dst_type::B_BitMask)) );

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

	// ========================================================================

	//
	//	TCheapAdditiveROP
	//	

	template< class DST_TYPE >
	class TCheapAdditiveROP {

	public:

		typedef DST_TYPE dst_type;
		typedef DST_TYPE src_type;

		__forceinline DST_TYPE & operator()( dst_type & d, const src_type & s ) const {

			d.value = 
				min( dst_type::A_BitMask, ((d.value & dst_type::A_BitMask) + (s.value & dst_type::A_BitMask)) ) +
				min( dst_type::R_BitMask, ((d.value & dst_type::R_BitMask) + (s.value & dst_type::R_BitMask)) ) +
				min( dst_type::G_BitMask, ((d.value & dst_type::G_BitMask) + (s.value & dst_type::G_BitMask)) ) +
				min( dst_type::B_BitMask, ((d.value & dst_type::B_BitMask) + (s.value & dst_type::B_BitMask)) );

			return d;

		}

		// [first, last) to output (same semantics as std::copy())
		// --------------------------------------------------------------------

		template< class ForwardIt, class OutputIt >
		__forceinline void InputMajor( ForwardIt first, ForwardIt last, OutputIt output ) const {

			while ( first != last ) {

				int o = output->value;
				int i = first->value;

				output->value = 
					min( dst_type::A_BitMask, ((o & dst_type::A_BitMask) + (i & dst_type::A_BitMask)) ) +
					min( dst_type::R_BitMask, ((o & dst_type::R_BitMask) + (i & dst_type::R_BitMask)) ) +
					min( dst_type::G_BitMask, ((o & dst_type::G_BitMask) + (i & dst_type::G_BitMask)) ) +
					min( dst_type::B_BitMask, ((o & dst_type::B_BitMask) + (i & dst_type::B_BitMask)) );

				++output;
				++first;

			}

		}

		// [first, last) write value 's'
		// --------------------------------------------------------------------

		template< class OutputIt >
		__forceinline void OutputMajor( OutputIt first, OutputIt last, const src_type & s ) const {

			// Preprocess the 'source' value
			// ----------------------------------------------------------------

			int i = s.value;
			int ia = (i & dst_type::A_BitMask);
			int ir = (i & dst_type::R_BitMask);
			int ig = (i & dst_type::G_BitMask);
			int ib = (i & dst_type::B_BitMask);

			// Perform the operation on all the dest values.
			// ----------------------------------------------------------------

			while ( first != last ) {

				int o = first->value;

				first->value = 
					min( dst_type::A_BitMask, ((o & dst_type::A_BitMask) + ia) ) +
					min( dst_type::R_BitMask, ((o & dst_type::R_BitMask) + ir) ) +
					min( dst_type::G_BitMask, ((o & dst_type::G_BitMask) + ig) ) +
					min( dst_type::B_BitMask, ((o & dst_type::B_BitMask) + ib) );

				++first;

			}

		}

	};

	//
	//	TCheapAdditiveROP2
	//	

	template< class DST_TYPE, class SRC_TYPE = DST_TYPE >
	class TCheapAdditiveROP2 {

	public:

		typedef DST_TYPE dst_type;
		typedef SRC_TYPE src_type;

		__forceinline DST_TYPE & operator()( dst_type & d, const src_type & s ) const {

			dst_type t = s;

			d.value = 
				min( DST_TYPE::A_BitMask, ((d.value & DST_TYPE::A_BitMask) + (t.value & DST_TYPE::A_BitMask)) ) +
				min( DST_TYPE::R_BitMask, ((d.value & DST_TYPE::R_BitMask) + (t.value & DST_TYPE::R_BitMask)) ) +
				min( DST_TYPE::G_BitMask, ((d.value & DST_TYPE::G_BitMask) + (t.value & DST_TYPE::G_BitMask)) ) +
				min( DST_TYPE::B_BitMask, ((d.value & DST_TYPE::B_BitMask) + (t.value & DST_TYPE::B_BitMask)) );

			return d;

		}

		// [first, last) to output (same semantics as std::copy())
		// --------------------------------------------------------------------

		template< class ForwardIt, class OutputIt >
		__forceinline void InputMajor( ForwardIt first, ForwardIt last, OutputIt output ) const {

			while ( first != last ) {

				dst_type t = *first++;
				int o = output->value;
				int i = t.value;

				output->value = 
					min( dst_type::A_BitMask, ((o & dst_type::A_BitMask) + (i & dst_type::A_BitMask)) ) +
					min( dst_type::R_BitMask, ((o & dst_type::R_BitMask) + (i & dst_type::R_BitMask)) ) +
					min( dst_type::G_BitMask, ((o & dst_type::G_BitMask) + (i & dst_type::G_BitMask)) ) +
					min( dst_type::B_BitMask, ((o & dst_type::B_BitMask) + (i & dst_type::B_BitMask)) );

				++output;

			}

		}

		// [first, last) write value 's'
		// --------------------------------------------------------------------

		template< class OutputIt >
		__forceinline void OutputMajor( OutputIt first, OutputIt last, const src_type & s ) const {

			// Preprocess the 'source' value
			// ----------------------------------------------------------------

			dst_type t = *first++;
			int i = t.value;
			int ia = (i & dst_type::A_BitMask)
			int ir = (i & dst_type::R_BitMask)
			int ig = (i & dst_type::G_BitMask)
			int ib = (i & dst_type::B_BitMask)

			// Perform the operation on all the dest values.
			// ----------------------------------------------------------------

			while ( first != last ) {

				int o = output->value;

				output->value = 
					min( dst_type::A_BitMask, ((o & dst_type::A_BitMask) + ia) ) +
					min( dst_type::R_BitMask, ((o & dst_type::R_BitMask) + ir) ) +
					min( dst_type::G_BitMask, ((o & dst_type::G_BitMask) + ig) ) +
					min( dst_type::B_BitMask, ((o & dst_type::B_BitMask) + ib) );

				++first;

			}

		}

	};

}; // namespace BPT

#endif // !defined(AFX_TOPADDITIVE_H__D74581BA_27F5_464B_8A99_B01E3DFE81B3__INCLUDED_)
