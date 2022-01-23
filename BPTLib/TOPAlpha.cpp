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


#include "stdafx.h"
#include "TOPAlpha.h"

// ============================================================================

namespace BPT {

	/*
	::
	::	A8R8G8B8_X1R5G5B5_BlitPrimitive()
	::
	::	-- This code is based on < http://developer.intel.com/software/idap/resources/technical_collateral/mmx/AP554.HTM#PERFORMANCE >
	::
	*/
	
	void 
	A8R8G8B8_X1R5G5B5_BlitPrimitive(
		U16 * dest, // Dest buffer x1r5g5b5 format
		const int destPitch, // bytes per dest line
		const PIXEL_A8R8G8B8 * source, // source buffer a8r8g8b8
		const int sourcePitch, // bytes per source line
		const int width, // blend width
		const int height // blend height
	)
	{
		// Error check
		// --------------------------------------------------------------------

		int clippedWidth = width - (width & 3);

		if ( (0 >= clippedWidth) ) return /* Nop */;

		// Declare data for the assembly code
		// --------------------------------------------------------------------

		__int64 zeros	= 0x0000000000000000;
		__int64 roundf  = 0x0000008000800080;
		__int64 maskr   = 0x0000000000007c00;
		__int64 maskg   = 0x00000000000003e0;
		__int64 maskb   = 0x000000000000001f;
		__int64 maskrH  = 0x7c00000000000000;
		__int64 maskgH  = 0x03e0000000000000;
		__int64 maskbH  = 0x001f000000000000;

		// Finally do something
		// --------------------------------------------------------------------

		_asm {

	        mov     esi, source             ; esi = p
	        mov     edi, dest               ; edi = q

	        mov     eax, clippedWidth       ; eax  = clippedWidth
	        mov     ecx, height             ; ecx  = height

			mov		ebx, esi				; next source line
			mov		edx, edi				; next dest line
			add		ebx, sourcePitch		;
			add		edx, destPitch			;

	L1:     movq    mm7, [edi]              ; mm7 = qqq4 qqq3 qqq2 qqq1
	        pxor    mm5, mm5                ; mm5 = 0000 0000 0000 0000
	
	L2:     movq    mm6, [esi]              ; mm6 = a2r2 g2b2 a1r1 g1b1
	        movq    mm0, mm6                ; mm0 = xxxx xxxx aarr ggbb
	        add     esi, 8                  ; esi += 8
	
	L3:     punpcklbw mm0, zeros            ; mm0 = 00aa 00rr 00gg 00bb
	        movq    mm2, mm7                ; mm2 = xxxx xxxx xxxx qqqq
	
	        pand    mm2, maskr              ; mm2 = 0000 0000 0000 qq00
	        movq    mm1, mm0                ; mm1 = 00aa xxxx xxxx xxxx
	
	        movq    mm3, mm7                ; mm3 = xxxx xxxx xxxx qqqq
	        punpckhwd mm1, mm1              ; mm1 = 00aa 00aa xxxx xxxx
	
	        pand    mm3, maskg              ; mm3 = 0000 0000 0000 0qq0
	        psllq   mm2, 25                 ; mm2 = 0000 00qr 0000 0000
	
	        movq    mm4, mm7                ; mm4 = xxxx xxxx xxxx qqqq
	        psllq   mm3, 14                 ; mm3 = 0000 0000 00qg 0000
	
	        pand    mm4, maskb              ; mm4 = 0000 0000 0000 00qq
	        punpckhdq mm1, mm1              ; mm1 = 00aa 00aa 00aa 00aa
	
	        por     mm2, mm3                ; mm2 = 0000 00qr 00qg 0000
	        psllq   mm4, 3                  ; mm4 = 0000 0000 0000 00qb
	
	        por     mm2, mm4                ; mm2 = 0000 00qr 00qg 00qb
	        psrlq   mm6, 32                 ; mm6 >>= 32
	
	        psubw   mm0, mm2                ; mm0 = p - q
	        psllw   mm2, 8                  ; mm2 = 0000 qr00 qg00 qb00
	
	        paddw   mm2, roundf             ; mm2 = q + round'g factor
	        pmullw  mm0, mm1                ; mm0 = (p-q)*aa
	
	        psrlq   mm5, 16                 ; mm5 >>= 16
	
	        psrlq   mm7, 16                 ; mm7 >>= 16
	        nop
	
	        paddw   mm2, mm0                ; mm2 = (p-q)*aa+q + round'g factor
	        movq    mm0, mm6                ; mm0 = xxxx xxxx aarr ggbb
	
	        movq    mm3, mm2                ; mm3 = mm2
	        psllq   mm2, 15                 ; mm2 = rrxx xxxx xxxx xxxx
	
	        movq    mm4, mm3                ; mm4 = mm3
	        psllq   mm3, 26                 ; mm3 = xggx xxxx xxxx xxxx
	
	        pand    mm2, maskrH             ; mm2 = rr00 0000 0000 0000
	        psllq   mm4, 37                 ; mm4 = xxbb xxxx xxxx xxxx
	
	        pand    mm3, maskgH             ; mm3 = 0gg0 0000 0000 0000
	        por     mm5, mm2                ; mm2 = sss0 0000 0000 0000
	
	        pand    mm4, maskbH             ; mm4 = 00bb 0000 0000 0000
	        por     mm5, mm3                ; mm5 = ssss SSSS SSSS SSSS
	
	        por     mm5, mm4
	        dec     eax                     ; -- eax
	
	        test    eax, 1
	        jnz     L3
	        test    eax, 2
	        jnz     L2
	        movq    [edi], mm5              ; save 4 alpha blended words in q
	        cmp     eax, 0
	        je      L4
	        add     edi, 8                  ; edi += 8
	        jmp     L1
	
		L4: dec		ecx						; --height
			jz		L5						; done if counter is now 0

			mov		eax, clippedWidth		; reset the width
			mov		esi, ebx				; next source line
			mov		edi, edx				; next dest line
			add		ebx, sourcePitch		;
			add		edx, destPitch			;
			jmp		L1						; start the next line

		L5: emms

		}


	}

}; // namespace BPT


