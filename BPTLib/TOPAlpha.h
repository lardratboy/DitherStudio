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


#if !defined(AFX_TOPALPHA_H__8B28E2C5_1FE3_47A2_8A73_6B29EBF89FA9__INCLUDED_)
#define AFX_TOPALPHA_H__8B28E2C5_1FE3_47A2_8A73_6B29EBF89FA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#define HACK_ALPHA_TEST

// ============================================================================

#include "Pixels.h"
#include "MMX.h" 

// ============================================================================

namespace BPT {

	// ========================================================================

	void 
	A8R8G8B8_X1R5G5B5_BlitPrimitive(
		U16 * dest, // Dest buffer x1r5g5b5 format
		const int destPitch, // bytes per dest line
		const PIXEL_A8R8G8B8 * source, // source buffer a8r8g8b8
		const int sourcePitch, // bytes per source line
		const int width, // blend width
		const int height // blend height
	);

	// ========================================================================

	//
	//	TConstantAlphaBlitter<>
	//
	//	Should work with most pixel types in 'Pixels.h'
	//
	//	-- Same type External alpha blit
	//

	template< class T, const int A >
	class TConstantAlphaBlitter {

	private:

		enum {

			// Setup the normal traits
			// ----------------------------------------------------------------

			R_SHIFT		= T::R_L_Shift
			,G_SHIFT	= T::G_L_Shift
			,B_SHIFT	= T::B_L_Shift

			,const_alpha = A
			,PositiveAdjust = (1 << T::LargestChannelSize)
			,PostAdjust = ((PositiveAdjust * const_alpha) >> 8)

			// Shifted r, g, b masks
			// ----------------------------------------------------------------

			,R_Mask = T::R_BitMask
			,G_Mask = T::G_BitMask
			,B_Mask = T::B_BitMask

			// The simd versions are only valid for <= 16bpp formats.
			// ----------------------------------------------------------------

			,PositiveAdjust_simd32 = PositiveAdjust | (PositiveAdjust << 16)
			,PostAdjust_simd32 = PostAdjust | (PostAdjust << 16)

			,R_Mask_simd32 = R_Mask | (R_Mask << 16)
			,G_Mask_simd32 = G_Mask | (G_Mask << 16)
			,B_Mask_simd32 = B_Mask | (B_Mask << 16)

		};

	public:

		TConstantAlphaBlitter() { /* Empty */ }

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

			int opWidth = width / 2;
			typedef const DWORD * SPTR;
			typedef DWORD * DPTR;

#else

			int opWidth = width;
			typedef const T::value_type * SPTR;
			typedef T::value_type * DPTR;

#endif

			SPTR sP = reinterpret_cast<SPTR>( src );
			DPTR dP = reinterpret_cast<DPTR>( dst );

#if 0 // MMX 

			// coming soon :)

#else

			for ( int y = height; --y >= 0; ) {

				for ( int x = opWidth; --x >= 0; ) {

					// Fetch the source & dest values

					int s = *sP++;
					int d = *dP++;

					int sr = (s & R_Mask_simd32) >> T::R_L_Shift;
					int sg = (s & G_Mask_simd32) >> T::G_L_Shift;
					int sb = (s & B_Mask_simd32) >> T::B_L_Shift;

					int dr = (d & R_Mask_simd32) >> T::R_L_Shift;
					int dg = (d & G_Mask_simd32) >> T::G_L_Shift;
					int db = (d & B_Mask_simd32) >> T::B_L_Shift;

					*(dP - 1) = 
						((((((((sr + PositiveAdjust_simd32) - dr) * const_alpha) >> 8) + dr) - PostAdjust_simd32) << T::R_L_Shift) & R_Mask_simd32) +
						((((((((sg + PositiveAdjust_simd32) - dg) * const_alpha) >> 8) + dg) - PostAdjust_simd32) << T::G_L_Shift) & G_Mask_simd32) +
						((((((((sb + PositiveAdjust_simd32) - db) * const_alpha) >> 8) + db) - PostAdjust_simd32) << T::B_L_Shift) & B_Mask_simd32) ;

				}

				// Step the pointers
				// ------------------------------------------------------------

				dP = reinterpret_cast<DPTR>(
					reinterpret_cast<BYTE *>( dP ) + dstOffset
				);

				sP = reinterpret_cast<SPTR>(
					reinterpret_cast<const BYTE *>( sP ) + srcOffset
				);

			}

#endif

		}

	};

	// ========================================================================

	//
	//	TExternalAlphaBlitter<>
	//
	//	Should work with most pixel types in 'Pixels.h'
	//
	//	-- Same type External alpha blit
	//

	template< class T >
	class TExternalAlphaBlitter {

	private:

		int m_PostAdjust_simd32;
		int m_PostAdjust;
		int m_alpha_simd32;
		int m_Alpha;
		int m_Chromakey;
		int m_Chromakey_simd32;

		enum {

			// Setup the normal traits
			// ----------------------------------------------------------------

			PositiveAdjust = (1 << T::LargestChannelSize)

			// Shifted r, g, b masks
			// ----------------------------------------------------------------

			,R_Mask = T::R_BitMask
			,G_Mask = T::G_BitMask
			,B_Mask = T::B_BitMask

			// The simd versions are only valid for <= 16bpp formats.
			// ----------------------------------------------------------------

			,PositiveAdjust_simd32 = PositiveAdjust | (PositiveAdjust << 16)

			,R_Mask_simd32 = R_Mask | (R_Mask << 16)
			,G_Mask_simd32 = G_Mask | (G_Mask << 16)
			,B_Mask_simd32 = B_Mask | (B_Mask << 16)

		};

	public:

		TExternalAlphaBlitter( const int alpha, const int chromakey = 0 ) {

			SetAlpha( alpha );
			SetChromakey( chromakey );
			
		}

		void SetAlpha( const int alpha ) {

			m_Alpha = alpha;
			m_PostAdjust = ((PositiveAdjust * alpha) >> 8);
			m_PostAdjust_simd32 = m_PostAdjust | (m_PostAdjust << 16);
			m_alpha_simd32 = m_Alpha | (m_Alpha << 16);

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

			LARGE_INTEGER rmask_mmx = { R_Mask_simd32, R_Mask_simd32 };
			LARGE_INTEGER gmask_mmx = { G_Mask_simd32, G_Mask_simd32 };
			LARGE_INTEGER bmask_mmx = { B_Mask_simd32, B_Mask_simd32 };
			LARGE_INTEGER positive_mmx = { PositiveAdjust_simd32, PositiveAdjust_simd32 };
			LARGE_INTEGER postadjust_mmx = { m_PostAdjust_simd32, m_PostAdjust_simd32 };
			LARGE_INTEGER alpha_mmx = { m_alpha_simd32, m_alpha_simd32 };

// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

			LARGE_INTEGER chromakey_mmx = { m_Chromakey_simd32, m_Chromakey_simd32 };
			LARGE_INTEGER transparent_mask_mmx = { 0, 0 }; 
			LARGE_INTEGER dest_areas_mmx = { 0, 0 };

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
																    
			enum {

				R_SHIFT		= T::R_L_Shift
				,G_SHIFT	= T::G_L_Shift
				,B_SHIFT	= T::B_L_Shift

			};

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
				nop									;; pair helper

#endif

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
				
				movq	mm6, rmask_mmx				;; read (R) mask bits
				movq	mm7, gmask_mmx				;; read (G) mask bits

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
				movq	mm7, positive_mmx			;; read the positive value compensation

				psrlw	mm0, R_SHIFT				;; move the (R) bits down
				psrlw	mm3, R_SHIFT				;; ""

				pand	mm2, mm6					;; isolate the (B) channel(s)
				pand	mm5, mm6					;; ""
				
				psrlw	mm1, G_SHIFT				;; move the (G) bits down
				psrlw	mm4, G_SHIFT				;; ""

				psrlw	mm2, B_SHIFT				;; move the (B) bits down
				psrlw	mm5, B_SHIFT				;; ""

				paddw	mm0, mm7					;; add the positive compensation
				paddw	mm1, mm7					;; ""

				paddw	mm2, mm7					;; ""
				nop									;; pair helper?

				movq	mm6, alpha_mmx				;; read the alpha multiplier
				movq	mm7, postadjust_mmx			;; read the post op adjustment

				psubw	mm0, mm3					;; subtract the dest (R) channel(s)
				psubw	mm1, mm4					;; subtract the dest (G) channel(s)

				psubw	mm2, mm5					;; subtract the dest (B) channel(s)
				pmullw	mm0, mm6					;; tr = (sR + add - dR) * alpha

				pmullw	mm1, mm6					;; tg = (sG + add - dG) * alpha
				pmullw	mm2, mm6					;; tb = (sB + add - dB) * alpha

				psrlw	mm0, 8						;; tr /= 256
				psrlw	mm1, 8						;; tg /= 256

				psrlw	mm2, 8						;; tb /= 256
				paddw	mm0, mm3					;; tr += dR

				paddw	mm1, mm4					;; tg += dG
				paddw	mm2, mm5					;; tb += dB

				psubw	mm0, mm7					;; tr -= ((postiveCompensation * alpha) / 256)
				psubw	mm1, mm7					;; tg -= ((postiveCompensation * alpha) / 256)

				psubw	mm2, mm7					;; tb -= ((postiveCompensation * alpha) / 256)
				psllw	mm0, R_SHIFT				;; move the (R) bits back into place

				psllw	mm1, G_SHIFT				;; move the (G) bits back into place
				psllw	mm2, B_SHIFT				;; move the (B) bits back into place

				movq	mm6, rmask_mmx				;; read the (R) mask bits
				movq	mm7, gmask_mmx				;; read the (G) mask bits

				movq	mm3, bmask_mmx				;; read the (B) mask bits
				pand	mm0, mm6					;; limit the (R) channel(s)

				pand	mm1, mm7					;; limit the (G) channel(s)
				pand	mm2, mm3					;; limit the (B) channel(s)

				paddw	mm0, mm1					;; combine the (R) + (G) channel(s)
				paddw	mm0, mm2					;; combine the (RG) + (B) channel(s)

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

	// ========================================================================

}; // namespace BPT

#endif // !defined(AFX_TOPALPHA_H__8B28E2C5_1FE3_47A2_8A73_6B29EBF89FA9__INCLUDED_)
