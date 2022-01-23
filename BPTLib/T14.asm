;; ============================================================================
;;
;;	T14 decompression primitives
;;	
;;	Copyright 2000, Brad P. Taylor
;;
;; ============================================================================

.586
.MMX
.model flat, c

;; ----------------------------------------------------------------------------
;;	CONSTANTS
;; ----------------------------------------------------------------------------

R5G5B5_ChannelLowBits	EQU		0ADEFADEFh
R5G6B5_ChannelLowBits	EQU		07BEf7BEfh

;; ----------------------------------------------------------------------------
.data
;; ----------------------------------------------------------------------------

USE_STACK	equ		0

;; ----------------------------------------------------------------------------
.code
;; ----------------------------------------------------------------------------

;; -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
;;	COPY OPERATION MACROS
;; -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

;;
;;	M_COPY_1
;;

M_COPY_1	MACRO	srcPtr,	srcTemp, srcDelta, dstPtr, dstTemp, dstDelta, p1, p2 
	mov		srcTemp, [ srcPtr ]		;; read the data
	add		srcPtr, srcDelta		;; move the src pointer
	mov		[ dstPtr ], dstTemp		;; write the data
	add		dstPtr, dstDelta		;; move the dst pointer
	ENDM

;;
;;	M_COPY_4_16bpp
;;

M_COPY_4_16bpp	MACRO	srcPtr,	dstPtr, p1, p2
	mov		eax, [ srcPtr ]			;; read 2
	mov		[ dstPtr ], eax			;; write 2
	mov		eax, [ srcPtr + 4 ]		;; read 2
	mov		[ dstPtr + 4 ], eax		;; write 2
	add		srcPtr, 8				;; move the src pointer
	add		dstPtr, 8				;; move the dst pointer
	sub		eax, eax				;; clear out the work register
	ENDM

;;
;;	MMX_COPY_4_16bpp
;;

MMX_COPY_4_16bpp	MACRO	srcPtr,	dstPtr, p1, p2
	movq	mm0, [ srcPtr ]			;; read src data
	add		srcPtr, 8				;; move the src pointer
	movq	[ dstPtr ], mm0			;; write the data
	add		dstPtr, 8				;; move the dst pointer
	ENDM

;;
;;	M_COPY_4_32bpp
;;

M_COPY_4_32bpp	MACRO	srcPtr,	dstPtr, p1, p2
	mov		eax, [ srcPtr ]			;; read 2
	mov		[ dstPtr ], eax			;; write 2
	mov		eax, [ srcPtr + 4] 		;; read 2
	mov		[ dstPtr + 4 ], eax		;; write 2
	mov		eax, [ srcPtr + 8 ]		;; read 2
	mov		[ dstPtr + 8 ], eax		;; write 2
	mov		eax, [ srcPtr + 12 ]	;; read 2
	mov		[ dstPtr + 12 ], eax 	;; write 2
	sub		eax, eax				;; clear out the work register
	add		srcPtr, 16				;; move the dst pointer
	add		dstPtr, 16				;; move the dst pointer
	ENDM

;;
;;	MMX_COPY_4_32bpp
;;

MMX_COPY_4_32bpp	MACRO	srcPtr,	dstPtr, p1, p2
	movq	mm0, [ srcPtr ]			;; read src data
	movq	[ dstPtr ], mm0			;; write the data
	movq	mm0, [ srcPtr + 8 ]		;; read src data
	movq	[ dstPtr +8 ], mm0	 	;; write the data
	add		srcPtr, 16				;; move the src pointer
	add		dstPtr, 16				;; move the dst pointer
	ENDM

;; -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
;;	5050 MIX MACROS
;; -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

;;
;;	M_5050_1
;;

M_5050_1	MACRO	srcPtr,	srcTemp, srcDelta, dstPtr, dstTemp, dstDelta, lowBitsMask, p2
	push	ebx								;; store ebx
	mov		srcTemp, [ srcPtr ]				;; read source
	sub		ebx, ebx						;; clear temp register
	add		srcPtr, srcDelta				;; move the src pointer
	mov		bx, [ dstPtr ]					;; read dest
	add		dstPtr, dstDelta				;; move the dst pointer
	shr		eax, 1							;; reduce
	shr		ebx, 1							;; ""
	and		eax, lowBitsMask				;; channel low bits mask
	and		ebx, lowBitsMask				;; channel low bits mask
	add		eax, ebx						;; combine
	pop		ebx								;; restore ebx
	mov		[ dstPtr - dstDelta ], dstTemp	;; write the value
	sub		eax, eax						;; clear working register
	ENDM

;;
;;	M_5050_4_16bpp
;;

M_5050_4_16bpp	MACRO	srcPtr,	dstPtr, lowBitsMask, p2
	push	ebx								;; store ebx
	sub		ebx, ebx						;; clear temp register
	mov		eax, [ srcPtr ]					;; read source
	mov		ebx, [ dstPtr ]					;; read dest
	shr		eax, 1							;; reduce
	shr		ebx, 1							;; ""
	and		eax, lowBitsMask 				;; channel low bits mask
	and		ebx, lowBitsMask 				;; channel low bits mask
	add		eax, ebx						;; combine
	mov		[ dstPtr ], eax					;; write the value
	mov		eax, [ srcPtr + 4 ]				;; read source
	mov		ebx, [ dstPtr + 4 ]				;; read dest
	shr		eax, 1							;; reduce
	shr		ebx, 1							;; ""
	and		eax, lowBitsMask 				;; channel low bits mask
	and		ebx, lowBitsMask 				;; channel low bits mask
	add		dstPtr, 8
	add		eax, ebx						;; combine
	add		srcPtr, 8						;; src += 4 elements
	mov		[ dstPtr - 4 ], eax				;; write the value
	sub		eax, eax						;; clear working register
	pop		ebx								;; restore ebx
	ENDM

;;
;;	MMX_5050_4_16bpp
;;

MMX_5050_4_16bpp	MACRO	srcPtr,	dstPtr, p1, p2
	movq	mm0, [ srcPtr ]					;; read 4 src pixels
	movq	mm1, [ dstPtr ]					;; read 4 dst pixels
	psrlw	mm0, 1							;; reduce 
	psrlw	mm1, 1							;; reduce
	add		dstPtr, 8						;; advance dst ptr
	pand	mm0, mm2						;; channel low bits mask
	pand	mm1, mm2						;; channel low bits mask
	paddw	mm0, mm1						;; combine
	add		srcPtr, 8						;; advance src ptr
	movq	[ dstPtr - 8 ], mm0				;; store mixed
	ENDM

;; -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
;;	PREMULTIPLIED 5050 MIX MACROS
;; -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

;;
;;	M_PRE_5050_1
;;

M_PRE_5050_1	MACRO	srcPtr,	srcTemp, srcDelta, dstPtr, dstTemp, dstDelta, lowBitsMask, p2
	push	ebx								;; store ebx
	mov		srcTemp, [ srcPtr ]				;; read source
	sub		ebx, ebx						;; clear temp register
	add		srcPtr, srcDelta				;; move the src pointer
	mov		bx, [ dstPtr ]					;; read dest
	add		dstPtr, dstDelta				;; move the dst pointer
	shr		ebx, 1							;; ""
	and		ebx, lowBitsMask				;; channel low bits mask
	add		eax, ebx						;; combine
	pop		ebx								;; restore ebx
	mov		[ dstPtr - dstDelta ], dstTemp	;; write the value
	sub		eax, eax						;; clear working register
	ENDM

;;
;;	M_PRE_5050_4_16bpp
;;

M_PRE_5050_4_16bpp	MACRO	srcPtr,	dstPtr, lowBitsMask, p2
	push	ebx								;; store ebx
	sub		ebx, ebx						;; clear temp register
	mov		eax, [ srcPtr ]					;; read source
	mov		ebx, [ dstPtr ]					;; read dest
	shr		ebx, 1							;; ""
	and		ebx, lowBitsMask 				;; channel low bits mask
	add		eax, ebx						;; combine
	mov		[ dstPtr ], eax					;; write the value
	mov		eax, [ srcPtr + 4 ]				;; read source
	mov		ebx, [ dstPtr + 4 ]				;; read dest
	shr		ebx, 1							;; ""
	and		ebx, lowBitsMask 				;; channel low bits mask
	add		dstPtr, 8						;; advance the dst ptr
	add		eax, ebx						;; combine
	add		srcPtr, 8						;; src += 4 elements
	mov		[ dstPtr - 4 ], eax				;; write the value
	sub		eax, eax						;; clear working register
	pop		ebx								;; restore ebx
	ENDM

;;
;;	MMX_PRE_5050_4_16bpp
;;

MMX_PRE_5050_4_16bpp	MACRO	srcPtr,	dstPtr, p1, p2
	movq	mm0, [ srcPtr ]					;; read 4 src pixels
	movq	mm1, [ dstPtr ]					;; read 4 dst pixels
	psrlw	mm1, 1							;; reduce
	add		dstPtr, 8						;; advance dst ptr
	pand	mm1, mm2						;; channel low bits mask
	paddw	mm0, mm1						;; combine
	add		srcPtr, 8						;; advance src ptr
	movq	[ dstPtr - 8 ], mm0				;; store mixed
	ENDM

;; -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
;;	Reduce brightness (N) bits MACROS
;; -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

;;
;;	M_DARKEN_1
;;

M_DARKEN_1	MACRO	srcPtr,	srcTemp, srcDelta, dstPtr, dstTemp, dstDelta, lowBitsMask, bitsToReduce
	mov		srcTemp, [ dstPtr ]				;; read dest pixel
	add		dstPtr, dstDelta				;; move the dest pointer
	shr		dstTemp, bitsToReduce			;; reduce the bits
	add		srcPtr, srcDelta				;; skip the src
	and		dstTemp, lowBitsMask			;; keep in bounds
	mov		[ dstPtr - dstDelta ], dstTemp	;; write the value
	sub		eax, eax						;; clear working register
	ENDM

;;
;;	M_DARKEN_4_16bpp
;;

M_DARKEN_4_16bpp	MACRO	srcPtr,	dstPtr, lowBitsMask, bitsToReduce
	push	ebx								;; store off ebx
	add		srcPtr, 8						;; skip the source pixels
	mov		ebx, [ dstPtr + 4 ]				;; read 2 pixels
	mov		eax, [ dstPtr ]					;; read 2 pixels
	shr		eax, bitsToReduce				;; reduce the bits
	shr		ebx, bitsToReduce				;; reduce the bits
	and		eax, lowBitsMask				;; keep in bounds
	and		ebx, lowBitsMask				;; keep in bounds
	mov		[ dstPtr ], eax					;; write 2 pixels
	mov		[ dstPtr + 4 ], ebx	 			;; write 2 pixels
	add		dstPtr, 8						;; move the dest pointer
	sub		eax, eax						;; clear out working register
	pop		ebx								;; restore ebx
	ENDM

;;
;;	MMX_DARKEN_4_16bpp
;;

MMX_DARKEN_4_16bpp	MACRO	srcPtr,	dstPtr, lowBitsMask, bitsToReduce
	movq	mm0, [ dstPtr ]					;; read 4 dst pixels
	add		dstPtr, 8						;; advance the dst pointer
	psrlw	mm0, bitsToReduce				;; reduce the bits
	pand	mm0, mm2						;; keep in bounds
	add		srcPtr, 8						;; advance the src pointer
	movq	[ dstPtr - 8 ], mm0				;; write 4 pixels
	ENDM

;; -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
;;	EXTERNAL ALPHA MACROS
;; -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

;; this needs to be replaced with simple code!!!
;; this needs to be replaced with simple code!!!
;; this needs to be replaced with simple code!!!

;;
;;	MMX_EXTERNAL_ALPHA_1_16bpp
;;

MMX_EXTERNAL_ALPHA_1_16bpp	MACRO	srcPtr,	srcTemp, srcDelta, dstPtr, dstTemp, dstDelta, p1, p2 
	add		srcPtr, srcDelta			;; move src pointer
	sub		eax, eax					;; clear work register
	add		dstPtr, dstDelta			;; move dst pointer
	mov		ax, [ srcPtr - srcDelta ]	;; read source pixel
	movd	mm0, eax					;; move to the mmx register
	mov		ax, [ dstPtr - dstDelta ]	;; read source pixel
	movd	mm3, eax					;; move to the mmx register

;; ---

	movq	mm1, mm0					;; src copy
	movq	mm2, mm0					;; ""
	movq	mm4, mm3					;; dst copy
	movq	mm5, mm3					;; ""
	pand	mm0, mm6					;; isolate the (R) channel(s)
	pand	mm3, mm6					;; ""
	pand	mm1, mm7					;; isolate the (G) channel(s)
	pand	mm4, mm7					;; ""
	movq	mm6, [ QWORD PTR bmask_mmx ]	;; read (B) mask bits
	movq	mm7, [ QWORD PTR positive_mmx ]	;; read the positive value compensation
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
	movq	mm6, [ QWORD PTR alpha_mmx ]	;; read the alpha multiplier
	movq	mm7, [ QWORD PTR postadjust_mmx ]	;; read the post op adjustment
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
	movq	mm6, [ QWORD PTR rmask_mmx ];; read the (R) mask bits
	movq	mm7, [ QWORD PTR gmask_mmx ]	;; read the (G) mask bits
	movq	mm3, [ QWORD PTR bmask_mmx ]	;; read the (B) mask bits
	pand	mm0, mm6					;; limit the (R) channel(s)
	pand	mm1, mm7					;; limit the (G) channel(s)
	pand	mm2, mm3					;; limit the (B) channel(s)
	paddw	mm0, mm1					;; combine the (R) + (G) channel(s)
	paddw	mm0, mm2					;; combine the (RG) + (B) channel(s)

;; ---

	movd	eax, mm0					;; get the results
	mov		[ dstPtr - dstDelta ], ax	;; write the value
	sub		eax, eax					;; clear working register
	ENDM

;;
;;	MMX_EXTERNAL_ALPHA_4_16bpp
;;

MMX_EXTERNAL_ALPHA_4_16bpp	MACRO srcPtr, dstPtr, p1, p2
	movq	mm0, [ srcPtr ]				;; read 4 source pixels
	add		srcPtr, 8					;; advance source ptr
	movq	mm3, [ dstPtr ]				;; read 4 dest pixels
	add		dstPtr, 8  					;; advance dest pointer

;; ---

	movq	mm1, mm0					;; src copy
	movq	mm2, mm0					;; ""
	movq	mm4, mm3					;; dst copy
	movq	mm5, mm3					;; ""
	pand	mm0, mm6					;; isolate the (R) channel(s)
	pand	mm3, mm6					;; ""
	pand	mm1, mm7					;; isolate the (G) channel(s)
	pand	mm4, mm7					;; ""
	movq	mm6, [ QWORD PTR bmask_mmx ]	;; read (B) mask bits
	movq	mm7, [ QWORD PTR positive_mmx ]	;; read the positive value compensation
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
	movq	mm6, [ QWORD PTR alpha_mmx ]	;; read the alpha multiplier
	movq	mm7, [ QWORD PTR postadjust_mmx ]	;; read the post op adjustment
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
	movq	mm6, [ QWORD PTR rmask_mmx ];; read the (R) mask bits
	movq	mm7, [ QWORD PTR gmask_mmx ]	;; read the (G) mask bits
	movq	mm3, [ QWORD PTR bmask_mmx ]	;; read the (B) mask bits
	pand	mm0, mm6					;; limit the (R) channel(s)
	pand	mm1, mm7					;; limit the (G) channel(s)
	pand	mm2, mm3					;; limit the (B) channel(s)
	paddw	mm0, mm1					;; combine the (R) + (G) channel(s)
	paddw	mm0, mm2					;; combine the (RG) + (B) channel(s)

;; ---

	movq	[ dstPtr - 8 ], mm0			;; write 4 blended pixels
	ENDM

;; -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
;;	ADDITIVE MACROS
;; -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

;; this needs to be replaced with simple code!!!
;; this needs to be replaced with simple code!!!
;; this needs to be replaced with simple code!!!

;;
;;	MMX_ADDITIVE_1_16bpp
;;

MMX_ADDITIVE_1_16bpp	MACRO	srcPtr,	srcTemp, srcDelta, dstPtr, dstTemp, dstDelta, p1, p2 
	add		srcPtr, srcDelta			;; move src pointer
	sub		eax, eax					;; clear work register
	add		dstPtr, dstDelta			;; move dst pointer
	mov		ax, [ srcPtr - srcDelta ]	;; read source pixel
	movd	mm0, eax					;; move to the mmx register
	mov		ax, [ dstPtr - dstDelta ]	;; read source pixel
	movd	mm3, eax					;; move to the mmx register

;; ---

	movq	mm1, mm0					;; src copy
	movq	mm2, mm0					;; ""
	movq	mm4, mm3					;; dst copy
	movq	mm5, mm3					;; ""
	pand	mm0, mm6					;; isolate the (R) channel(s)
	pand	mm3, mm6					;; ""
	pand	mm1, mm7					;; isolate the (G) channel(s)
	pand	mm4, mm7					;; ""
	movq	mm6, [ QWORD PTR bmask_mmx ];; read (B) mask bits
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
	movq	mm6, [ QWORD PTR rmask_mmx ]	;; read the (R) mask bits
	paddw	mm1, mm2					;; combine the (B) + (G) channel(s)
	pand	mm0, mm6
	paddw	mm0, mm1					;; combine the (GB) + (R) channel(s)

;; ---

	movd	eax, mm0					;; get the results
	mov		[ dstPtr - dstDelta ], ax	;; write the value
	sub		eax, eax					;; clear working register
	ENDM

;;
;;	MMX_ADDITIVE_4_16bpp
;;

MMX_ADDITIVE_4_16bpp	MACRO srcPtr, dstPtr, p1, p2
	movq	mm0, [ srcPtr ]				;; read 4 source pixels
	add		srcPtr, 8					;; advance source ptr
	movq	mm3, [ dstPtr ]				;; read 4 dest pixels
	add		dstPtr, 8  					;; advance dest pointer

;; ---

	movq	mm1, mm0					;; src copy
	movq	mm2, mm0					;; ""
	movq	mm4, mm3					;; dst copy
	movq	mm5, mm3					;; ""
	pand	mm0, mm6					;; isolate the (R) channel(s)
	pand	mm3, mm6					;; ""
	pand	mm1, mm7					;; isolate the (G) channel(s)
	pand	mm4, mm7					;; ""
	movq	mm6, [ QWORD PTR bmask_mmx ];; read (B) mask bits
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
	movq	mm6, [ QWORD PTR rmask_mmx ]	;; read the (R) mask bits
	paddw	mm1, mm2					;; combine the (B) + (G) channel(s)
	pand	mm0, mm6
	paddw	mm0, mm1					;; combine the (GB) + (R) channel(s)

;; ---

	movq	[ dstPtr - 8 ], mm0			;; write 4 blended pixels
	ENDM

;; -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
;;	BRIGHTEN2x MACROS
;; -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

;; this needs to be replaced with simple code!!!
;; this needs to be replaced with simple code!!!
;; this needs to be replaced with simple code!!!

;;
;;	MMX_BRIGHTEN2X_1_16bpp
;;

MMX_BRIGHTEN2X_1_16bpp	MACRO	srcPtr,	srcTemp, srcDelta, dstPtr, dstTemp, dstDelta, p1, p2 
	sub		eax, eax					;; clear work register
	mov		ax, [ dstPtr ]				;; read source pixel
	add		dstPtr, dstDelta			;; move dst pointer
	movd	mm0, eax					;; move to the mmx register

;; ---

	movq	mm1, mm0					;; src copy
	movq	mm2, mm0					;; ""
	pand	mm0, mm3					;; isolate the (R) channel(s)
	pand	mm1, mm4					;; isolate the (G) channel(s)
	pand	mm2, mm5					;; isolate the (G) channel(s)
	psllw	mm0, R_CHANNEL_SHIFT		;; move the (R) bits up
	psllw	mm1, G_CHANNEL_SHIFT		;; move the (G) bits up
	psllw	mm2, B_CHANNEL_SHIFT		;; move the (B) bits up
	paddusw	mm0, mm0					;; add (R) channel(s)
	paddusw	mm1, mm1					;; add (G) channel(s)
	paddusw	mm2, mm2					;; add (B) channel(s)
	psrlw	mm0, R_CHANNEL_SHIFT		;; move the (R) bits back into place
	psrlw	mm1, G_CHANNEL_SHIFT		;; move the (G) bits back into place
	psrlw	mm2, B_CHANNEL_SHIFT		;; move the (B) bits back into place
	pand	mm0, mm3					;; isolate the (R) channel(s)
	pand	mm1, mm4					;; isolate the (G) channel(s)
	pand	mm2, mm5					;; isolate the (G) channel(s)
	paddw	mm0, mm1					;; combine the (R) + (G) channel(s)
	paddw	mm0, mm2					;; combine the (RG) + (B) channel(s)

;; ---

	movd	eax, mm0					;; get the results
	add		srcPtr, srcDelta			;; move src pointer
	mov		[ dstPtr - dstDelta ], ax	;; write the value
	sub		eax, eax					;; clear working register
	ENDM

;;
;;	MMX_BRIGHTEN2X_4_16bpp
;;

MMX_BRIGHTEN2X_4_16bpp	MACRO srcPtr, dstPtr, p1, p2
	movq	mm0, [ dstPtr ]				;; read 4 dest pixels
	add		dstPtr, 8  					;; advance dest pointer

;; ---

	movq	mm1, mm0					;; src copy
	movq	mm2, mm0					;; ""
	pand	mm0, mm3					;; isolate the (R) channel(s)
	pand	mm1, mm4					;; isolate the (G) channel(s)
	pand	mm2, mm5					;; isolate the (G) channel(s)
	psllw	mm0, R_CHANNEL_SHIFT		;; move the (R) bits up
	psllw	mm1, G_CHANNEL_SHIFT		;; move the (G) bits up
	psllw	mm2, B_CHANNEL_SHIFT		;; move the (B) bits up
	paddusw	mm0, mm0					;; add (R) channel(s)
	paddusw	mm1, mm1					;; add (G) channel(s)
	paddusw	mm2, mm2					;; add (B) channel(s)
	psrlw	mm0, R_CHANNEL_SHIFT		;; move the (R) bits back into place
	psrlw	mm1, G_CHANNEL_SHIFT		;; move the (G) bits back into place
	psrlw	mm2, B_CHANNEL_SHIFT		;; move the (B) bits back into place
	pand	mm0, mm3					;; isolate the (R) channel(s)
	pand	mm1, mm4					;; isolate the (G) channel(s)
	pand	mm2, mm5					;; isolate the (G) channel(s)
	paddw	mm0, mm1					;; combine the (R) + (G) channel(s)
	paddw	mm0, mm2					;; combine the (RG) + (B) channel(s)

;; ---

	add		srcPtr, 8					;; advance source ptr
	movq	[ dstPtr - 8 ], mm0			;; write 4 blended pixels
	ENDM

;; -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
;;	SUBTRACTIVE MACROS
;; -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

;; this needs to be replaced with simple code!!!
;; this needs to be replaced with simple code!!!
;; this needs to be replaced with simple code!!!

;;
;;	MMX_SUBTRACTIVE_1_16bpp
;;

MMX_SUBTRACTIVE_1_16bpp	MACRO	srcPtr,	srcTemp, srcDelta, dstPtr, dstTemp, dstDelta, p1, p2 
	add		srcPtr, srcDelta			;; move src pointer
	sub		eax, eax					;; clear work register
	add		dstPtr, dstDelta			;; move dst pointer
	mov		ax, [ srcPtr - srcDelta ]	;; read source pixel
	movd	mm3, eax					;; move to the mmx register
	mov		ax, [ dstPtr - dstDelta ]	;; read source pixel
	movd	mm0, eax					;; move to the mmx register

;; ---

	movq	mm1, mm0					;; src copy
	movq	mm2, mm0					;; ""
	movq	mm4, mm3					;; dst copy
	movq	mm5, mm3					;; ""
	pand	mm0, mm6					;; isolate the (R) channel(s)
	pand	mm3, mm6					;; ""
	pand	mm1, mm7					;; isolate the (G) channel(s)
	pand	mm4, mm7					;; ""
	movq	mm6, [ QWORD PTR bmask_mmx ];; read (B) mask bits
	nop									;; pair helper
	psrlw	mm0, R_L_SHIFT				;; move the (R) bits up
	psrlw	mm3, R_L_SHIFT				;; ""
	pand	mm2, mm6					;; isolate the (B) channel(s)
	pand	mm5, mm6					;; ""
	psrlw	mm1, G_L_SHIFT				;; move the (G) bits up
	psrlw	mm4, G_L_SHIFT				;; ""
	psrlw	mm2, B_L_SHIFT				;; move the (B) bits up
	psrlw	mm5, B_L_SHIFT				;; ""
	psubusw	mm0, mm3					;; add (R) channel(s)
	psubusw	mm1, mm4					;; add (G) channel(s)
	psubusw	mm2, mm5					;; add (B) channel(s)
	psllw	mm0, R_L_SHIFT				;; move the (R) bits back into place
	psllw	mm1, G_L_SHIFT				;; move the (G) bits back into place
	psllw	mm2, B_L_SHIFT				;; move the (B) bits back into place
	pand	mm2, mm6					;; isolate the (B) channel(s)
	pand	mm1, mm7					;; isolate the (G) channel(s)
	movq	mm6, [ QWORD PTR rmask_mmx ]	;; read the (R) mask bits
	paddw	mm1, mm2					;; combine the (B) + (G) channel(s)
	pand	mm0, mm6
	paddw	mm0, mm1					;; combine the (GB) + (R) channel(s)

;; ---

	movd	eax, mm0					;; get the results
	mov		[ dstPtr - dstDelta ], ax	;; write the value
	sub		eax, eax					;; clear working register
	ENDM

;;
;;	MMX_SUBTRACTIVE_4_16bpp
;;

MMX_SUBTRACTIVE_4_16bpp	MACRO srcPtr, dstPtr, p1, p2
	movq	mm3, [ srcPtr ]				;; read 4 source pixels
	add		srcPtr, 8					;; advance source ptr
	movq	mm0, [ dstPtr ]				;; read 4 dest pixels
	add		dstPtr, 8  					;; advance dest pointer

;; ---

	movq	mm1, mm0					;; src copy
	movq	mm2, mm0					;; ""
	movq	mm4, mm3					;; dst copy
	movq	mm5, mm3					;; ""
	pand	mm0, mm6					;; isolate the (R) channel(s)
	pand	mm3, mm6					;; ""
	pand	mm1, mm7					;; isolate the (G) channel(s)
	pand	mm4, mm7					;; ""
	movq	mm6, [ QWORD PTR bmask_mmx ];; read (B) mask bits
	nop									;; pair helper
	psrlw	mm0, R_L_SHIFT				;; move the (R) bits up
	psrlw	mm3, R_L_SHIFT				;; ""
	pand	mm2, mm6					;; isolate the (B) channel(s)
	pand	mm5, mm6					;; ""
	psrlw	mm1, G_L_SHIFT				;; move the (G) bits up
	psrlw	mm4, G_L_SHIFT				;; ""
	psrlw	mm2, B_L_SHIFT				;; move the (B) bits up
	psrlw	mm5, B_L_SHIFT				;; ""
	psubusw	mm0, mm3					;; add (R) channel(s)
	psubusw	mm1, mm4					;; add (G) channel(s)
	psubusw	mm2, mm5					;; add (B) channel(s)
	psllw	mm0, R_L_SHIFT				;; move the (R) bits back into place
	psllw	mm1, G_L_SHIFT				;; move the (G) bits back into place
	psllw	mm2, B_L_SHIFT				;; move the (B) bits back into place
	pand	mm2, mm6					;; isolate the (B) channel(s)
	pand	mm1, mm7					;; isolate the (G) channel(s)
	movq	mm6, [ QWORD PTR rmask_mmx ]	;; read the (R) mask bits
	paddw	mm1, mm2					;; combine the (B) + (G) channel(s)
	pand	mm0, mm6
	paddw	mm0, mm1					;; combine the (GB) + (R) channel(s)

;; ---

	movq	[ dstPtr - 8 ], mm0			;; write 4 blended pixels
	ENDM

;; -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
;;	MULTIPLY MACROS
;; -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

;; this needs to be replaced with simple code!!!
;; this needs to be replaced with simple code!!!
;; this needs to be replaced with simple code!!!

;;
;;	MMX_MULTIPLY_1_16bpp
;;

MMX_MULTIPLY_1_16bpp	MACRO	srcPtr,	srcTemp, srcDelta, dstPtr, dstTemp, dstDelta, multiplyRangePow2, p2 

	LOCAL	RNP, GNP, BNP
	RNP = (16 - (R_BITCOUNT * 2))
	GNP = (16 - (G_BITCOUNT * 2))
	BNP = (16 - (B_BITCOUNT * 2))

	add		srcPtr, srcDelta				;; move src pointer
	sub		eax, eax						;; clear work register
	add		dstPtr, dstDelta				;; move dst pointer
	mov		ax, [ srcPtr - srcDelta ]		;; read source pixel
	movd	mm0, eax						;; move to the mmx register
	mov		ax, [ dstPtr - dstDelta ]		;; read source pixel
	movd	mm3, eax						;; move to the mmx register

;; ---

	movq	mm1, mm0						;; copy src
	movq	mm2, mm0						;; ""
	movq	mm4, mm3						;; copy dst
	movq	mm5, mm3						;; ""
	pand	mm0, mm6						;; isolate the (R) channels
	pand	mm3, mm6						;; ""
	movq	mm6, [ QWORD PTR bmask_mmx ]	;; read the (B) channel mask
	pand	mm1, mm7						;; isolate the (G) channels
	pand	mm4, mm7						;; ""
	pand	mm2, mm6						;; isolate the (B) channels
	pand	mm5, mm6						;; ""
	psrlw	mm0, R_L_SHIFT					;; move the multiplier down
	psrlw	mm1, G_L_SHIFT					;; ""
	psrlw	mm2, B_L_SHIFT					;; ""

if (RNP GT R_L_SHIFT)
	psllw	mm3, (RNP - R_L_SHIFT)			;; move the mutiplicand up
else
	psrlw	mm3, (R_L_SHIFT - RNP)			;; move the mutiplicand up
endif

if (GNP GT G_L_SHIFT)
	psllw	mm4, (GNP - G_L_SHIFT)			;; ""
else
	psllw	mm4, (G_L_SHIFT - GNP)			;; ""
endif

if (BNP GT B_L_SHIFT)
	psllw	mm5, (BNP - B_L_SHIFT)			;; ""
else
	psllw	mm5, (B_L_SHIFT - BNP)			;; ""
endif

	pmullw	mm3, mm0						;; Multiply (R) channels
	pmullw	mm4, mm1						;; Multiply (G) channels
	pmullw	mm5, mm2						;; Multiply (B) channels
	paddusw	mm3, mm3						;; *= 2 for saturation
	paddusw	mm4, mm4						;; ""
	paddusw	mm5, mm5						;; ""
	psrlw	mm3, R_CHANNEL_SHIFT			;; move the (R) bits back into place
	psrlw	mm4, G_CHANNEL_SHIFT			;; move the (G) bits back into place
	psrlw	mm5, B_CHANNEL_SHIFT 			;; move the (B) bits back into place
	pand	mm4, mm7						;; isolate just the (G) bits
	pand	mm5, mm6						;; isolate just the (B) bits
	movq	mm6, [ QWORD PTR rmask_mmx ]	;; reload the (R) channel mask
	paddw	mm4, mm5						;; create (GB) combined channel
	pand	mm3, mm6						;; isolate just the (R) bits
	paddw	mm3, mm4						;; combine R + (GB)
											
;; ---

	movd	eax, mm3					;; get the results
	mov		[ dstPtr - dstDelta ], ax	;; write the value
	sub		eax, eax					;; clear working register
	ENDM

;;
;;	MMX_MULTIPLY_4_16bpp
;;

MMX_MULTIPLY_4_16bpp	MACRO srcPtr, dstPtr, multiplyRangePow2, p2

	LOCAL	RNP, GNP, BNP
	RNP = (16 - (R_BITCOUNT * 2))
	GNP = (16 - (G_BITCOUNT * 2))
	BNP = (16 - (B_BITCOUNT * 2))

	movq	mm0, [ srcPtr ]				;; read 4 source pixels
	add		srcPtr, 8					;; advance source ptr
	movq	mm3, [ dstPtr ]				;; read 4 dest pixels
	add		dstPtr, 8  					;; advance dest pointer

;; ---

	movq	mm1, mm0						;; copy src
	movq	mm2, mm0						;; ""
	movq	mm4, mm3						;; copy dst
	movq	mm5, mm3						;; ""
	pand	mm0, mm6						;; isolate the (R) channels
	pand	mm3, mm6						;; ""
	movq	mm6, [ QWORD PTR bmask_mmx ]	;; read the (B) channel mask
	pand	mm1, mm7						;; isolate the (G) channels
	pand	mm4, mm7						;; ""
	pand	mm2, mm6						;; isolate the (B) channels
	pand	mm5, mm6						;; ""
	psrlw	mm0, R_L_SHIFT					;; move the multiplier down
	psrlw	mm1, G_L_SHIFT					;; ""
	psrlw	mm2, B_L_SHIFT					;; ""

if (RNP GT R_L_SHIFT)
	psllw	mm3, (RNP - R_L_SHIFT)			;; move the mutiplicand up
else
	psrlw	mm3, (R_L_SHIFT - RNP)			;; move the mutiplicand up
endif

if (GNP GT G_L_SHIFT)
	psllw	mm4, (GNP - G_L_SHIFT)			;; ""
else
	psllw	mm4, (G_L_SHIFT - GNP)			;; ""
endif

if (BNP GT B_L_SHIFT)
	psllw	mm5, (BNP - B_L_SHIFT)			;; ""
else
	psllw	mm5, (B_L_SHIFT - BNP)			;; ""
endif

	pmullw	mm3, mm0						;; Multiply (R) channels
	pmullw	mm4, mm1						;; Multiply (G) channels
	pmullw	mm5, mm2						;; Multiply (B) channels
	paddusw	mm3, mm3						;; *= 2 for saturation
	paddusw	mm4, mm4						;; ""
	paddusw	mm5, mm5						;; ""
	psrlw	mm3, R_CHANNEL_SHIFT			;; move the (R) bits back into place
	psrlw	mm4, G_CHANNEL_SHIFT			;; move the (G) bits back into place
	psrlw	mm5, B_CHANNEL_SHIFT 			;; move the (B) bits back into place
	pand	mm4, mm7						;; isolate just the (G) bits
	pand	mm5, mm6						;; isolate just the (B) bits
	movq	mm6, [ QWORD PTR rmask_mmx ]	;; reload the (R) channel mask
	paddw	mm4, mm5						;; create (GB) combined channel
	pand	mm3, mm6						;; isolate just the (R) bits
	paddw	mm3, mm4						;; combine R + (GB)

;; ---

	movq	[ dstPtr - 8 ], mm3				;; write 4 blended pixels
	ENDM

;; -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
;;	T14 core macro
;;
;;	SINGLE_CODE -- code necessary to process a single element
;;	QUAD_CODE -- code necessary to process 4 elements
;;	XXX -- register that represents the size of a single element + work register
;;	XSIZE -- size of the element in bytes
;;
;; -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

M_T14_CORE	MACRO	SINGLE_CODE, QUAD_CODE, XXX, XSIZE, P1, P2

	LOCAL	SKIPLOOP, SKIP_CLIPPED_QUAD,  SKIP_SINGLE, SKIP_SKIP
	LOCAL	STARTWRITE, WRITELOOP, NEXTLINE, CLIPPEDQUAD, DONE
	LOCAL	HANDLE_3, HANDLE_2, HANDLE_1, HANDLESINGLE, HANDLESKIP

		sub		edx, edx				;; clear out quad pointer
		sub		eax, eax				;; clear out work reg
		sub		ebx, ebx				;; clear out single pointer
		mov		edi, dstPtr				;; set the output ptr
		mov		esi, compressedStream	;; get pointer to compressed data
		mov		ecx, skipCount			;; get line count
		mov		bx, [ esi + 2 ]			;; load the local offset to singles
		mov		dx, [ esi + 4 ]			;; load the local offset to quads
		add		ebx, esi				;; finialize singles pointer
		add		edx, esi				;; finialize quads pointer

if USE_STACK
		push	esi						;; store off esi for later use
else
		mov		lastESI, esi			;; store off esi for later use
endif

		add		esi, 6					;; skip header
		or		ecx, ecx				;; done
		jle		STARTWRITE				;; if 0 >= skipCount then write
  
	SKIPLOOP:
  
		mov		al, [ esi ]				;; code = *codes
		inc		esi						;; ++codes
		sub		eax, 2					;; decode
		jg		SKIP_SKIP				;; if > 0 then it's a skip op
		jnz		SKIP_SINGLE				;; if < 0 then it's a single op
		add		edx, (XSIZE * 4)		;; quads += 4
		sub		ecx, 4					;; skipCout -= 4
		jg		SKIPLOOP				;; more to process?
		jl		SKIP_CLIPPED_QUAD		;; clipped?
		mov		ecx, writeCount			;; set writeCount
		jmp		WRITELOOP				;; start processing
  
	SKIP_CLIPPED_QUAD:
  
		mov		eax, ecx				;; copy negative amount

if (XSIZE EQ 2)
		add		edx, ecx				;; adjust quads pointer 1/2
		add		edx, ecx				;; adjust quads pointer 1/2
else
if (XSIZE EQ 4) 
		sal		ecx, 2					;; ecx * XSIZE (assumed to be 4)
		add		edx, ecx				;; adjust the quads pointer
else
		.err
endif
endif

		mov		ecx, writeCount			;; get writeCount
		add		eax, 3					;; determine write amount
		jz		HANDLE_3				;; if -3 == skipCount
		dec		eax						;; if -2 == skipCount
		jz		HANDLE_2				;; goto handle_2 if -2 == skipCount
		jmp		HANDLE_1				;; it must be 1
  
	SKIP_SINGLE:
  
		sub		eax, eax				;; clear working var
		add		ebx, XSIZE				;; ++singles
		dec		ecx						;; --skipCount
		jnz		SKIPLOOP				;; keep processing?
		mov		ecx, writeCount			;; set writeCount
		jmp		WRITELOOP				;; start processing
  
	SKIP_SKIP:
  
		sub		ecx, eax				;; skipCount -= skipAmount
		jz		STARTWRITE				;; ended exactly here?
		jg		SKIPLOOP				;; keep processing
		neg		ecx						;; reverse the sign so it can move forward
		lea		edi, [ edi + ecx * XSIZE ] ;; add the whole skip
		neg		ecx						;; reverse the sign so it can move forward
		add		ecx, writeCount			;; adjust write amount
		jg		WRITELOOP				;; jump to the write loop
		lea		edi, [ edi + ecx * XSIZE ]	;; went too far adjust dst 
		jmp		NEXTLINE				;; next line
  
	STARTWRITE:
  
		mov		ecx, writeCount			;; set writeCount
  
	WRITELOOP:
  
		mov		al, [ esi ]				;; code = *codes
		inc		esi						;; ++codes
		sub		eax, 2					;; decode
		jg		HANDLESKIP				;; if > 0 then it's a skip op
		jnz		HANDLESINGLE			;; if < 0 then it's a single op
		sub		ecx, 4					;; writeCount -= 4
		jl		CLIPPEDQUAD				;; handle the clipped quad operation

;; ----------------------------------------------------------------------------
		QUAD_CODE	edx, edi, P1, P2
;; ----------------------------------------------------------------------------
  
		or		ecx, ecx				;; next line?
		jnz		WRITELOOP				;; not done keep decoding

	NEXTLINE:
  
		sub		eax, eax				;; clear working register
		mov		ebx, lineCounter		;; read lineCounter
		sub		ecx, ecx				;; clear working counter
		dec		ebx						;; --lineCounter
		jz		DONE					;; if 0 == lineCounter then were done!
		mov		lineCounter, ebx		;; store the adjusted lineCounter

if USE_STACK
		pop		esi						;; get the last esi
else
		mov		esi, lastESI			;; get the last esi
endif

		add		edi, adjustedPitch		;; dst += adjustedDstPitch
		mov		cx, [ esi ]				;; read the offset
		sub		ebx, ebx				;; clear out single ptr
		add		esi, ecx				;; move to the next line
		sub		edx, edx				;; clear out quad ptr
		mov		bx, [ esi + 2 ]			;; read single data offset
		mov		dx, [ esi + 4 ]			;; read quad data offset
		add		ebx, esi				;; finalize single data ptr
		add		edx, esi				;; finalize quad data ptr

if USE_STACK
		push	esi						;; store off esi for later
else
		mov		lastESI, esi			;; store off esi for later
endif

		mov		ecx, skipCount			;; get the clipping count
		add		esi, 6					;; move esi to point to the codes table
		or		ecx, ecx				;; clip?
		jnz		SKIPLOOP				;; yes
		mov		ecx, writeCount			;; set writeCount
		jmp		WRITELOOP				;; no
  
	CLIPPEDQUAD:

		add		ecx, 4					;; how many pixels to write
		cmp		ecx, 2					;; determine write count
		je		HANDLE_2				;; 2 == writeCount handle 2
		jl		HANDLE_1				;; 1 == writeCount handle 1
  
	HANDLE_3:
  
;; ----------------------------------------------------------------------------
		SINGLE_CODE	edx, XXX, XSIZE, edi, XXX, XSIZE, P1, P2
;; ----------------------------------------------------------------------------
  
		dec		ecx						;; --writeCount
		jz		NEXTLINE				;; done?
  
	HANDLE_2:
  
;; ----------------------------------------------------------------------------
		SINGLE_CODE	edx, XXX, XSIZE, edi, XXX, XSIZE, P1, P2
;; ----------------------------------------------------------------------------
  
		dec		ecx						;; --writeCount
		jz		NEXTLINE				;; done?
  
	HANDLE_1:
  
;; ----------------------------------------------------------------------------
		SINGLE_CODE	edx, XXX, XSIZE, edi, XXX, XSIZE, P1, P2
;; ----------------------------------------------------------------------------
  
		dec		ecx						;; --writeCount
		jz		NEXTLINE				;; move to the next line

		sub		eax, eax				;; clear work register
		jmp		WRITELOOP				;; keep processing
  
	HANDLESINGLE:
  
;; ----------------------------------------------------------------------------
		SINGLE_CODE	ebx, XXX, XSIZE, edi, XXX, XSIZE, P1, P2
;; ----------------------------------------------------------------------------
  
		sub		eax, eax				;; clear out work reg.
		dec		ecx						;; --writeCount
		jnz		WRITELOOP				;; not done keep decoding
		jmp		NEXTLINE				;; move to the next line
  
	HANDLESKIP:
  
		lea		edi, [ edi + eax * XSIZE ]	;; dst += skipCount
		sub		ecx, eax				;; cross off
		jg		WRITELOOP				;; handle a clipped skip op
		lea		edi, [ edi + ecx * XSIZE ]	;; adjust for clipped amount
		jmp		NEXTLINE				;; handle the move to the next line
  
	DONE:

if USE_STACK
		pop		eax
endif

		;; DONT FORGET TO PUT RETURN CODE IN THE CLIENT!!!!
		;; DONT FORGET TO PUT RETURN CODE IN THE CLIENT!!!!
		;; DONT FORGET TO PUT RETURN CODE IN THE CLIENT!!!!
  
		ENDM

;; <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
;; >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;; <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
;; >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;; <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
;; >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

;; ----------------------------------------------------------------------------
;;	SIMPLE 16bpp FUNCTION BUILDER MACRO
;; ----------------------------------------------------------------------------

;;
;;	BUILD_SIMPLE_16BPP_FN
;;
;;	FN_NAME == Function to declare
;;	HANDLE1 == Macro to use to handle 1 element
;;	HANDLE4 == Macro to use to handle 4 elements
;;	ARG1 == Handler argument 1
;;	ARG2 == Handler argument 1
;;	MM2ARG1 == Copy ARG1 into all 4 words of mm2
;;	USESMMX == do a emms at the end?
;;

BUILD_SIMPLE_16BPP_FN MACRO FN_NAME, HANDLE1, HANDLE4, ARG1, ARG2, MM2ARG1, USESMMX

FN_NAME proc uses edi esi ebx ebp,
	dstPtr:PTR DWORD, adjustedPitch:DWORD, compressedStream:PTR DWORD, 
	skipCount:DWORD, writeCount:DWORD, lineCounter:DWORD, extraParam:DWORD

		;; declare local storage
		;; --------------------------------------------------------------------

if (USE_STACK EQ 0)
		local	lastESI:DWORD
endif

		;; Prepare the mmx registers / data for this pixel format
		;; --------------------------------------------------------------------

if MM2ARG1
		mov			eax, ARG1			;; setup the mask reg.
		movd		mm2, eax			;; ""
		punpcklwd	mm2, mm2			;; ""
		punpckldq	mm2, mm2			;; ""
endif

		;; --------------------------------------------------------------------

		M_T14_CORE	HANDLE1, HANDLE4, ax, 2, ARG1, ARG2	;; MASM MACRO (I LOVE MASM!)

if USESMMX
		emms							;; exit mmx instruction
endif

		ret								;; we are outta here!

		db	'Copyright 2000, Brad P. Taylor', 0

FN_NAME endp

	ENDM

;; ----------------------------------------------------------------------------
;;	Copy functions (works for all 16bpp formats)
;; ----------------------------------------------------------------------------

BUILD_SIMPLE_16BPP_FN MMX_T14_Copy_16bpp, M_COPY_1, MMX_COPY_4_16bpp, 0, 0, 0, 1
BUILD_SIMPLE_16BPP_FN T14_Copy_16bpp, M_COPY_1, M_COPY_4_16bpp, 0, 0, 0, 0

;; ----------------------------------------------------------------------------
;;	MMX_T14_5050_xxxxxx()
;; ----------------------------------------------------------------------------

BUILD_SIMPLE_16BPP_FN MMX_T14_5050_R5G5B5, M_5050_1, MMX_5050_4_16bpp, R5G5B5_ChannelLowBits, 0, 1, 1
BUILD_SIMPLE_16BPP_FN MMX_T14_5050_R5G6B5, M_5050_1, MMX_5050_4_16bpp, R5G6B5_ChannelLowBits, 0, 1, 1

;; ----------------------------------------------------------------------------
;;	T14_5050_xxxxxx()
;; ----------------------------------------------------------------------------

BUILD_SIMPLE_16BPP_FN T14_5050_R5G5B5, M_5050_1, M_5050_4_16bpp, R5G5B5_ChannelLowBits, 0, 0, 0
BUILD_SIMPLE_16BPP_FN T14_5050_R5G6B5, M_5050_1, M_5050_4_16bpp, R5G6B5_ChannelLowBits, 0, 0, 0

;; ----------------------------------------------------------------------------
;;	MMX_T14_PreMultiplied5050_xxxxxx()
;; ----------------------------------------------------------------------------

BUILD_SIMPLE_16BPP_FN MMX_T14_PreMultiplied5050_R5G5B5, M_PRE_5050_1, MMX_PRE_5050_4_16bpp, R5G5B5_ChannelLowBits, 0, 1, 1
BUILD_SIMPLE_16BPP_FN MMX_T14_PreMultiplied5050_R5G6B5, M_PRE_5050_1, MMX_PRE_5050_4_16bpp, R5G6B5_ChannelLowBits, 0, 1, 1

;; ----------------------------------------------------------------------------
;;	T14_PreMultiplied5050_xxxxxx()
;; ----------------------------------------------------------------------------

BUILD_SIMPLE_16BPP_FN T14_PreMultiplied5050_R5G5B5, M_PRE_5050_1, M_PRE_5050_4_16bpp, R5G5B5_ChannelLowBits, 0, 0, 0
BUILD_SIMPLE_16BPP_FN T14_PreMultiplied5050_R5G6B5, M_PRE_5050_1, M_PRE_5050_4_16bpp, R5G6B5_ChannelLowBits, 0, 0, 0

;; ----------------------------------------------------------------------------
;;	MMX_T14_Darken2x_xxxxxx()
;; ----------------------------------------------------------------------------

BUILD_SIMPLE_16BPP_FN MMX_T14_Darken2x_R5G5B5, M_DARKEN_1, MMX_DARKEN_4_16bpp, R5G5B5_ChannelLowBits, 1, 1, 1
BUILD_SIMPLE_16BPP_FN MMX_T14_Darken2x_R5G6B5, M_DARKEN_1, MMX_DARKEN_4_16bpp, R5G6B5_ChannelLowBits, 1, 1, 1

;; ----------------------------------------------------------------------------
;;	T14_Darken2x_xxxxxx()
;; ----------------------------------------------------------------------------

BUILD_SIMPLE_16BPP_FN T14_Darken2x_R5G5B5, M_DARKEN_1, M_DARKEN_4_16bpp, R5G5B5_ChannelLowBits, 1, 0, 0
BUILD_SIMPLE_16BPP_FN T14_Darken2x_R5G6B5, M_DARKEN_1, M_DARKEN_4_16bpp, R5G6B5_ChannelLowBits, 1, 0, 0

;; ----------------------------------------------------------------------------
;;	EXTERNAL ALPHA FUNCTION BUILDER MACRO
;; ----------------------------------------------------------------------------

;;
;;	BUILD_EXTERNAL_ALPHA_16BPP_FN
;;
;;	RLS == R channel left shift value
;;	GLS == G channel left shift value
;;	BLS == B channel left shift value
;;	RBC == R channel bit count
;;	GBC == G channel bit count
;;	BBC == B channel bit count
;;	LCS == largest channel bit count
;;

BUILD_EXTERNAL_ALPHA_16BPP_FN MACRO FN_NAME, RLS, GLS, BLS, RBC, GBC, BBC, LCS

	;; local labels for this macro

	LOCAL SKIP_CONSTANTS, _rmask_mmx, _gmask_mmx, _bmask_mmx, _positive_mmx

	;; constants --------------------------------------------------------------

	R_SHIFT = RLS
	G_SHIFT = GLS
	B_SHIFT = BLS
	R_MASK = (((1 shl RBC) - 1) shl RLS)
	G_MASK = (((1 shl GBC) - 1) shl GLS)
	B_MASK = (((1 shl BBC) - 1) shl BLS)
	P_ADJUST = (1 shl LCS)

	;; PROC AREA

FN_NAME proc uses edi esi ebx ebp,
	dstPtr:PTR DWORD, adjustedPitch:DWORD, compressedStream:PTR DWORD, 
	skipCount:DWORD, writeCount:DWORD, lineCounter:DWORD, alpha:DWORD

		;; declare local storage
		;; --------------------------------------------------------------------

if (USE_STACK EQ 0)
		local	lastESI:DWORD
endif

		local	postadjust_mmx:QWORD
		local	alpha_mmx:QWORD

		jmp		SKIP_CONSTANTS

		;; Generate the 'constants' for this pixel format
		;; --------------------------------------------------------------------

		align	16

		_rmask_mmx		dw	R_MASK, R_MASK, R_MASK, R_MASK
		_gmask_mmx		dw	G_MASK, G_MASK, G_MASK, G_MASK
		_bmask_mmx		dw	B_MASK, B_MASK, B_MASK, B_MASK
		_positive_mmx	dw	P_ADJUST, P_ADJUST, P_ADJUST, P_ADJUST

		db	'Copyright 2000, Brad P. Taylor', 0

		rmask_mmx		= _rmask_mmx
		gmask_mmx		= _gmask_mmx
		bmask_mmx		= _bmask_mmx
		positive_mmx	= _positive_mmx

	SKIP_CONSTANTS:

		;; Prepare the mmx registers / data for this pixel format
		;; --------------------------------------------------------------------

		mov			eax, alpha						;; read the alpha value
		mov			ebx, alpha						;; read the alpha value
		shl			eax, 16							;; move to high bits
		mov			ax, bx							;; dup
		movd		mm1, eax						;; ""
		punpcklwd	mm1, mm1						;; ""
		punpckldq	mm1, mm1						;; ""
		movq		alpha_mmx, mm1					;; store off the mmx'd alpha
		movq		mm0, QWORD PTR [ positive_mmx ]	;; calculate post-adjust value
		pmullw		mm0, mm1						;; positive * alpha
		psrlw		mm0, 8							;; (positive * alpha) / 256
		movq		postadjust_mmx, mm0				;; store off the mmx'd adjust
		movq		mm6, QWORD PTR [ rmask_mmx ]	;; read (R) mask bits
		movq		mm7, QWORD PTR [ gmask_mmx ]		;; read (G) mask bits

		;; --------------------------------------------------------------------

		M_T14_CORE	MMX_EXTERNAL_ALPHA_1_16bpp, MMX_EXTERNAL_ALPHA_4_16bpp, ax, 2, 0, 0	;; MASM MACRO (I LOVE MASM!)
		emms							;; exit mmx instruction
		ret								;; we are outta here!

FN_NAME endp

	ENDM

;; ----------------------------------------------------------------------------
;;	MMX_T14_ExternalAlpha_xxxxxx()
;; ----------------------------------------------------------------------------

BUILD_EXTERNAL_ALPHA_16BPP_FN MMX_T14_ExternalAlpha_R5G5B5, 10, 5, 0, 5, 5, 5, 5
BUILD_EXTERNAL_ALPHA_16BPP_FN MMX_T14_ExternalAlpha_R5G6B5, 11, 5, 0, 5, 6, 5, 6

;; ----------------------------------------------------------------------------
;;	XADDITIVE FUNCTION BUILDER MACRO
;; ----------------------------------------------------------------------------

;;
;;	BUILD_XADDITIVE_16BPP_FN
;;
;;	FN_NAME == Function to declare
;;	HANDLE1 == Macro to use to handle 1 element
;;	HANDLE4 == Macro to use to handle 4 elements
;;	RLS == R channel left shift value
;;	GLS == G channel left shift value
;;	BLS == B channel left shift value
;;	RBC == R channel bit count
;;	GBC == G channel bit count
;;	BBC == B channel bit count
;;	MASKSMM345 == Put the r, g, b masks in mm3, 4, 5
;;	P1 == param 1 for the lowest level macro
;;	P2 == param 2 for the lowest level macro
;;

BUILD_XADDITIVE_16BPP_FN MACRO FN_NAME, HANDLE1, HANDLE4, RLS, GLS, BLS, RBC, GBC, BBC, MASKSMM345, P1, P2

	;; local labels for this macro

	LOCAL SKIP_CONSTANTS, _rmask_mmx, _gmask_mmx, _bmask_mmx

	;; constants --------------------------------------------------------------

	R_L_SHIFT = RLS
	G_L_SHIFT = GLS
	B_L_SHIFT = BLS
	R_CHANNEL_SHIFT = (16 - (RLS + RBC))
	G_CHANNEL_SHIFT = (16 - (GLS + GBC))
	B_CHANNEL_SHIFT = (16 - (BLS + BBC))
	R_MASK = (((1 shl RBC) - 1) shl RLS)
	G_MASK = (((1 shl GBC) - 1) shl GLS)
	B_MASK = (((1 shl BBC) - 1) shl BLS)
	R_BITCOUNT = RBC
	G_BITCOUNT = GBC
	B_BITCOUNT = BBC

	;; PROC AREA

FN_NAME proc uses edi esi ebx ebp,
	dstPtr:PTR DWORD, adjustedPitch:DWORD, compressedStream:PTR DWORD, 
	skipCount:DWORD, writeCount:DWORD, lineCounter:DWORD, extraParam:DWORD

		;; declare local storage
		;; --------------------------------------------------------------------

if (USE_STACK EQ 0)
		local	lastESI:DWORD
endif

		jmp		SKIP_CONSTANTS

		;; Generate the 'constants' for this pixel format
		;; --------------------------------------------------------------------

		align	16

		_rmask_mmx	dw	R_MASK, R_MASK, R_MASK, R_MASK
		_gmask_mmx	dw	G_MASK, G_MASK, G_MASK, G_MASK
		_bmask_mmx	dw	B_MASK, B_MASK, B_MASK, B_MASK

		db	'Copyright 2000, Brad P. Taylor', 0

		rmask_mmx		= _rmask_mmx
		gmask_mmx		= _gmask_mmx
		bmask_mmx		= _bmask_mmx

	SKIP_CONSTANTS:

		;; Prepare the mmx registers / data for this pixel format
		;; --------------------------------------------------------------------

if MASKSMM345

		movq		mm3, [ QWORD PTR rmask_mmx ]	;; read (R) mask bits
		movq		mm4, [ QWORD PTR gmask_mmx ]	;; read (G) mask bits
		movq		mm5, [ QWORD PTR bmask_mmx ]	;; read (B) mask bits

else

		movq		mm6, [ QWORD PTR rmask_mmx ]	;; read (R) mask bits
		movq		mm7, [ QWORD PTR gmask_mmx ]	;; read (G) mask bits

endif

		;; --------------------------------------------------------------------

		M_T14_CORE	HANDLE1, HANDLE4, ax, 2, P1, P2	;; MASM MACRO (I LOVE MASM!)
		emms							;; exit mmx instruction
		ret								;; we are outta here!

FN_NAME endp

	ENDM

;; ----------------------------------------------------------------------------
;;	MMX_T14_Additive_xxxxxx()
;; ----------------------------------------------------------------------------

BUILD_XADDITIVE_16BPP_FN MMX_T14_Additive_R5G5B5, MMX_ADDITIVE_1_16bpp, MMX_ADDITIVE_4_16bpp, 10, 5, 0, 5, 5, 5, 0, 0, 0
BUILD_XADDITIVE_16BPP_FN MMX_T14_Additive_R5G6B5, MMX_ADDITIVE_1_16bpp, MMX_ADDITIVE_4_16bpp, 11, 5, 0, 5, 6, 5, 0, 0, 0
BUILD_XADDITIVE_16BPP_FN MMX_T14_Brighten2x_R5G5B5, MMX_BRIGHTEN2X_1_16bpp, MMX_BRIGHTEN2X_4_16bpp, 10, 5, 0, 5, 5, 5, 1, 0, 0
BUILD_XADDITIVE_16BPP_FN MMX_T14_Brighten2x_R5G6B5, MMX_BRIGHTEN2X_1_16bpp, MMX_BRIGHTEN2X_4_16bpp, 11, 5, 0, 5, 6, 5, 1, 0, 0
BUILD_XADDITIVE_16BPP_FN MMX_T14_Subtractive_R5G5B5, MMX_SUBTRACTIVE_1_16bpp, MMX_SUBTRACTIVE_4_16bpp, 10, 5, 0, 5, 5, 5, 0, 0, 0
BUILD_XADDITIVE_16BPP_FN MMX_T14_Subtractive_R5G6B5, MMX_SUBTRACTIVE_1_16bpp, MMX_SUBTRACTIVE_4_16bpp, 11, 5, 0, 5, 6, 5, 0, 0, 0

;; ----------------------------------------------------------------------------
;;	MMX_T14_Multiply?_xxxxxx()
;; ----------------------------------------------------------------------------

BUILD_XADDITIVE_16BPP_FN MMX_T14_Multiply0to2_R5G5B5, MMX_MULTIPLY_1_16bpp, MMX_MULTIPLY_4_16bpp, 10, 5, 0, 5, 5, 5, 0, 1, 0
BUILD_XADDITIVE_16BPP_FN MMX_T14_Multiply0to2_R5G6B5, MMX_MULTIPLY_1_16bpp, MMX_MULTIPLY_4_16bpp, 11, 5, 0, 5, 6, 5, 0, 1, 0

;; ----------------------------------------------------------------------------

END