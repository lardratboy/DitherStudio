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

#if !defined(TSCALEUPBLIT_H__4EE3AB65_FB91_4E19_9877_81FB6B6BDBFD__INCLUDED_)
#define TSCALEUPBLIT_H__4EE3AB65_FB91_4E19_9877_81FB6B6BDBFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BPTBlitter.h"

namespace BPT {

	// -------------------------------------------------------------------------------

	//
	//	TScanlineFixedPointScaleOP<>
	//	

	template<
		typename DST_TYPE
		, typename SRC_TYPE = DST_TYPE
		, typename TRANSFER_OP = TCopyROP< DST_TYPE >
		, const int SHIFT = 8
		, typename MATH_TYPE = int
	>
	struct TScanlineFixedPointScaleOP {

		public:

			typedef DST_TYPE dst_type;
			typedef SRC_TYPE src_type;

		private:

			MATH_TYPE m_x;
			MATH_TYPE m_dx;
			TRANSFER_OP m_Op;

			TScanlineFixedPointScaleOP(); // Hidden

		public:

			TScanlineFixedPointScaleOP(
				const int x, const int dx,
				TRANSFER_OP op = TRANSFER_OP()
			) : m_x(x), m_dx(dx), m_Op(op) { /* Empty */
			}

			__forceinline DST_TYPE& operator()(dst_type& d, const src_type& s) const {

				return m_Op(d, s);

			}

			// [first, last) to output (same semantics as std::copy())
			// --------------------------------------------------------------------

			template< typename OutputIt, typename InputIt >
			__forceinline void OutputMajor(OutputIt first, OutputIt last, InputIt input) const {

				for ( int x = m_x; last != first; )
				{
					(*this)( *first++, *(input + (x >> SHIFT)) );
					x += m_dx;
				}

			}

			// [first, last) write value 's'
			// --------------------------------------------------------------------

			// define an InputMajor transfer based on output major (assumes a lot about the iterator behavior)

			template< typename ForwardIt, typename OutputIt >
			__forceinline void InputMajor(ForwardIt first, ForwardIt last, OutputIt output) const {
				OutputMajor(output, output + (last - first), first);
			}


			template< typename OutputIt >
			__forceinline void OutputMajor(OutputIt first, OutputIt last, const src_type& s) const {
				m_Op.OutputMajor(first, last, s); // TODO test this
			}

	};

	// ------------------------------------------------------------------------

	template<typename R>
	void ScaleRECT(R& r,const int x, const int y)
	{
		r.left *= x;
		r.right *= x;
		r.top *= y;
		r.bottom *= y;
	}

	// Need to merge all these blitter setups into a class (already did this several times in multiple places...

	//
	//	T_FixedPointScaleUpBlit
	//

	template< typename DST_SURFACE, typename SRC_SURFACE, typename TOP, const int SHIFT = 8>
	void __forceinline
	T_FixedPointScaleUpBlit(
		int scaleX,
		int scaleY,
		DST_SURFACE& dstSurface,
		const int x,
		const int y,
		SRC_SURFACE& srcSurface,
		TOP op = TOP(),
		const BLITFX* blitFX = 0,
		const RECT* optionalDstClipRect = 0,
		const RECT* optionalSrcSubRect = 0
	) {

		// Clip the optional clipping rect to the dest bitmap limits
		// --------------------------------------------------------------------

		RECT dstLimitsRect = dstSurface.Rect();

		RECT clippedDstRect;

		if (optionalDstClipRect) {

			if (!IntersectRect(&clippedDstRect, &dstLimitsRect, optionalDstClipRect)) {

				return /* NOP */;

			}

		}
		else {

			clippedDstRect = dstLimitsRect;

		}

		// Get the source operation size in source coordinate space
		// --------------------------------------------------------------------

		RECT srcLimitsRect = srcSurface.Rect();

		RECT clippedSrcRect;

		if (optionalSrcSubRect) {

			if (!IntersectRect(&clippedSrcRect, &srcLimitsRect, optionalSrcSubRect)) {

				return /* NOP */;

			}

		}
		else {

			clippedSrcRect = srcLimitsRect;

		}

		// scale the source to dest coordinate space

		ScaleRECT(srcLimitsRect, scaleX, scaleY);
		ScaleRECT(clippedSrcRect, scaleX, scaleY);

		// Perform a simple clipping operation to detect NOP (in scaled coordinate space)
		// --------------------------------------------------------------------

		SIZE clippedSrcRectSize = SizeOfRect(&clippedSrcRect);

		RECT dstOperation = {
			x, y, x + clippedSrcRectSize.cx, y + clippedSrcRectSize.cy
		};

		RECT clippedRect;

		if (!IntersectRect(&clippedRect, &clippedDstRect, &dstOperation)) {

			return /* NOP */;

		}

		// Setup the general loop variables
		// --------------------------------------------------------------------

		int cx = clippedRect.right - clippedRect.left;
		int cy = clippedRect.bottom - clippedRect.top;

		int sx = ((clippedRect.left - x) + clippedSrcRect.left);
		int sy = ((clippedRect.top - y) + clippedSrcRect.top);

		// Check for flipping and adjust the dest position and step amount.
		// --------------------------------------------------------------------

		int dx, dy, ddx, ddy;

		if (blitFX) {

			if (BLITFX::HFLIP & blitFX->dwFlags) {

				sx = (clippedSrcRect.right - (sx + cx));
				dx = (clippedRect.right - 1);
				ddx = -1;

			}
			else {

				dx = clippedRect.left;
				ddx = 1;

			}

			if (BLITFX::VFLIP & blitFX->dwFlags) {

				sy = (clippedSrcRect.bottom - (sy + cy));
				dy = (clippedRect.bottom - 1);
				ddy = -1;

			}
			else {

				dy = clippedRect.top;
				ddy = 1;

			}

		}
		else {

			dx = clippedRect.left;
			ddx = 1;

			dy = clippedRect.top;
			ddy = 1;

		}

		// scale back to source coordinate space and setup stepping sizes
		// --------------------------------------------------------------------

		sx = (sx << SHIFT) / scaleX;
		sy = (sy << SHIFT) / scaleY;

		int wd = (1 != cx) ? cx - 1 : 1;
		int xd = ((srcSurface.Width() - 1 - (sx >> SHIFT)) << SHIFT) / wd;

		int hd = (1 != cy) ? cy - 1 : 1;
		int yd = ((srcSurface.Height() - 1 - (sy >> SHIFT)) << SHIFT) / hd;

		// probably should check for a NOP and what about possible over reach ?

		int aw = ((xd >> SHIFT) * (cx >> SHIFT));
		int ah = ((yd >> SHIFT) * (cy >> SHIFT));

		ASSERT((aw <= srcSurface.Width()) && (ah <= srcSurface.Height()));

		// Process the non clipped spans
		// --------------------------------------------------------------------

		TScanlineFixedPointScaleOP<DST_SURFACE::pixel_type, SRC_SURFACE::pixel_type, TOP, SHIFT> scalineOp(sx, xd, op);

		if (1 == ddx) {

			for (int ly = 0; ly < cy; ly++) {

				DST_SURFACE::pixel_iterator dstIT = dstSurface.Iterator(dx, dy);
				SRC_SURFACE::pixel_iterator srcIT = srcSurface.Iterator(0, (sy >> SHIFT));

				scalineOp.OutputMajor(dstIT, dstIT + cx, srcIT);

				dy += ddy;
				sy += yd;

			}

		}
		else {

			for (int ly = 0; ly < cy; ly++) {

				DST_SURFACE::reverse_iterator dstIT = dstSurface.rIterator(dx, dy);
				SRC_SURFACE::pixel_iterator srcIT = srcSurface.Iterator(0, (sy >> SHIFT));

				scalineOp.OutputMajor(dstIT, dstIT + cx, srcIT);

				dy += ddy;
				sy += yd;

			}

		}

	}

	// ------------------------------------------------------------------------

	//
	//	T_CreateScaledBitmap()
	//	- this probably should use the blitter
	//

	template< typename T, const int SHIFT = 8 >
	T* T_CreateScaledBitmap(T& src, const int w, const int h) {

		// Validate
		// --------------------------------------------------------------------

		if ((0 >= w) || (0 >= h)) {

			return 0;

		}

		// Create and size the output bitmap
		// --------------------------------------------------------------------

		T* pNew = new T;

		if (!pNew) {

			return 0;

		}

		if (!pNew->Create(w, h)) {

			delete pNew;

			return 0;

		}

		// Setup the fractional step values
		// --------------------------------------------------------------------

		int wd = (1 != w) ? w - 1 : 1;
		int xd = ((src.Width() - 1) << SHIFT) / wd;

		int hd = (1 != h) ? h - 1 : 1;
		int yd = ((src.Height() - 1) << SHIFT) / hd;

		// Now go through the process of transfering the pixels...
		// This assumes that the pixel_iterator type is random access!
		// --------------------------------------------------------------------

		int yf = 0;

		for (int y = 0; y < h; y++) {

			T::pixel_iterator dstIT = pNew->Iterator(0, y);

			T::pixel_iterator srcIT = src.Iterator(0, (yf >> SHIFT));

			yf += yd;

			int xf = 0;

			for (int x = 0; x < w; x++) {

				*dstIT++ = *(srcIT + (xf >> SHIFT));

				xf += xd;

			}

		}

		// --------------------------------------------------------------------

		return pNew;

	}

	// ------------------------------------------------------------------------

	//
	//	TTableDrivenScaleUpOP<>
	//	

	template<
		typename DST_TYPE
		, typename SRC_TYPE = DST_TYPE
		, typename SCALE_TABLE_TYPE = const U8*
		, typename TRANSFER_OP = TCopyROP< DST_TYPE >
	>
		struct TTableDrivenScaleUpOP {

		public:

			typedef DST_TYPE dst_type;
			typedef SRC_TYPE src_type;

		private:

			SCALE_TABLE_TYPE m_ScaleTable;
			SCALE_TABLE_TYPE m_ScaleTableEnd;
			TRANSFER_OP m_Op;

			TTableDrivenScaleUpOP(); // Hidden

		public:

			TTableDrivenScaleUpOP(
				SCALE_TABLE_TYPE scaleTableIt, SCALE_TABLE_TYPE scaleTableEnd, TRANSFER_OP op = TRANSFER_OP()
			) : m_ScaleTable(scaleTableIt), m_ScaleTableEnd(scaleTableEnd), m_Op(op) { /* Empty */
			}

			__forceinline DST_TYPE& operator()(dst_type& d, const src_type& s) const {

				return m_Op(d, s);

			}

			// [first, last) to output
			// --------------------------------------------------------------------

			template< typename ForwardIt, typename OutputIt >
			__forceinline void InputMajor(ForwardIt first, ForwardIt last, OutputIt output) const {

				SCALE_TABLE_TYPE pTable = m_ScaleTable;

				if (pTable == m_ScaleTableEnd) return /* NOP */;

				ASSERT((last != first));

				while (first != last) {

					(void)(*this)(*output++, *first);

					if (*pTable) {

						++first;

					}

					if (++pTable == m_ScaleTableEnd) break;

				}

			}

			// [first, last) write, read from input
			// --------------------------------------------------------------------

			template< typename OutputIt, typename ForwardIt >
			__forceinline void InputMinor(OutputIt first, OutputIt last, ForwardIt input) const {

				SCALE_TABLE_TYPE pTable = m_ScaleTable;

				if (pTable == m_ScaleTableEnd) return /* NOP */;

				ASSERT((last != first));

				while (first != last) {

					(void)(*this)(*first++, *input);

					if (*pTable) {

						++input;

					}

					if (++pTable == m_ScaleTableEnd) break;

				}

			}

			template< typename OutputIt, typename ForwardIt >
			__forceinline void InputMinor(OutputIt first, OutputIt last, ForwardIt input, ForwardIt inputEnd) const {

				SCALE_TABLE_TYPE pTable = m_ScaleTable;

				if (pTable == m_ScaleTableEnd) return /* NOP */;

				ASSERT((last != first) && (inputEnd != input));

				while (first != last) {

					(void)(*this)(*first++, *input);

					if (*pTable) {

						if (inputEnd == ++input) break;

					}

					if (++pTable == m_ScaleTableEnd) break;

				}

			}

			// [first, last) write value 's'
			// --------------------------------------------------------------------

			template< typename OutputIt >
			__forceinline void OutputMajor(OutputIt first, OutputIt last, const src_type& s) const {

				m_Op.OutputMajor(first, last, s); // TODO test this

			}

	};

	// ------------------------------------------------------------------------

	/*
		LEGACY table based scale up approach (super buggy) keeping around to eventually fix to get closure
		this code is soooooooooooo silly I don't know why I wrote it this way
	*/

	//
	//	T_TableScaleUpBlit
	//
	//	-- This routine could cause a memory 'leak' to be reported at the end
	//	-- of a program, basicly if the tables are indeed cached then there is
	//	-- no way to flush them.  If this was a class things would be different!
	//

	template< typename DST_SURFACE, typename SRC_SURFACE, typename TOP >
	void T_TableScaleUpBlit(
		DST_SURFACE& dstSurface
		, const int x
		, const int y
		, SRC_SURFACE& srcSurface
		, const int w
		, const int h
		, TOP op = TOP()
		, const BLITFX* blitFX = 0
		, const RECT* optionalDstClipRect = 0
		, const RECT* optionalSrcSubRect = 0
		, const bool deleteTablesAfterOperation = false
	) {

		// Define the 'type' of the scaling table
		// --------------------------------------------------------------------

		typedef unsigned char scale_entry_type;
		typedef scale_entry_type* scale_table_type;

		typedef TTableDrivenScaleUpOP<
			TOP::dst_type, TOP::src_type, scale_table_type, TOP
		> scale_rop_type;

		// Clip the optional clipping rect to the dest bitmap limits
		// --------------------------------------------------------------------

		RECT dstLimitsRect = dstSurface.Rect();

		RECT clippedDstRect;

		if (optionalDstClipRect) {

			if (!IntersectRect(&clippedDstRect, &dstLimitsRect, optionalDstClipRect)) {

				return /* NOP */;

			}

		}
		else {

			clippedDstRect = dstLimitsRect;

		}

		// Perform a simple clipping operation to detect NOP
		// --------------------------------------------------------------------

		RECT dstOperation = { x, y, x + w, y + h };

		RECT clippedRect;

		if (!IntersectRect(&clippedRect, &clippedDstRect, &dstOperation)) {

			return /* NOP */;

		}

		// Get the source operation size
		// --------------------------------------------------------------------

		RECT srcLimitsRect = srcSurface.Rect();

		RECT clippedSrcRect;

		if (optionalSrcSubRect) {

			if (!IntersectRect(&clippedSrcRect, &srcLimitsRect, optionalSrcSubRect)) {

				return /* NOP */;

			}

		}
		else {

			clippedSrcRect = srcLimitsRect;

		}

		int sw = (clippedSrcRect.right - clippedSrcRect.left);
		int sh = (clippedSrcRect.bottom - clippedSrcRect.top);

		// Make sure we are scaling up!
		// --------------------------------------------------------------------

		if ((w < sw) || (h < sh))
		{
			return /* NOP */;
		}

		// Calculate the sub pixel coordinates & the operation size
		// --------------------------------------------------------------------

		const int cx = clippedRect.right - clippedRect.left;
		const int cy = clippedRect.bottom - clippedRect.top;

		float hScale = (float)sw / (float)w;
		float vScale = (float)sh / (float)h;

		int a = (clippedRect.left - x);
		int b = (clippedRect.top - y);

		// Setup the scale table and the scale rop
		// --------------------------------------------------------------------

		const auto cache_alignment = 4096;

		static scale_table_type pXScaleTable = 0;
		static int cached_w = -1;

		if ((!pXScaleTable) || (cached_w < w)) {

			if (pXScaleTable) delete[] pXScaleTable;

			auto alignedSize = ((w + cache_alignment - 1) / cache_alignment) * cache_alignment;

			TRACE("increasing x cache from %d to %d\n", cached_w, alignedSize);

			pXScaleTable = new scale_entry_type[alignedSize];

			if (!pXScaleTable) {

				cached_w = -1;

				return /* error nop */;

			}

			cached_w = alignedSize;

		}

		scale_table_type pXScaleTableEnd = pXScaleTable + w;

		// -----------------

		static scale_table_type pYScaleTable = 0;
		static int cached_h = -1;

		if ((!pYScaleTable) || (cached_h < h)) {

			if (pYScaleTable) delete[] pYScaleTable;

			auto alignedSize = ((h + cache_alignment - 1) / cache_alignment) * cache_alignment;
			TRACE("increasing y cache from %d to %d\n", cached_h, alignedSize);

			pYScaleTable = new scale_entry_type[alignedSize];
			//pYScaleTableEnd = pYScaleTable + alignedSize;

			if (!pYScaleTable) {

				cached_h = -1;

				return /* error nop */;

			}

			cached_h = alignedSize;

		}

		scale_table_type pYScaleTableEnd = pYScaleTable + h;

		// Build the scale table, which is nothing more than when to increase
		// the source pointer during the blit operation. (this should be
		// RLE encoded type of thing (see notes 2021.10.18)
		// --------------------------------------------------------------------

		float fx = 0.0f;

		int lx = 0;

		int i = 0;

		for (; i < w; i++) {

			int ix = (int)fx;

			pXScaleTable[i] = lx - ix;

			lx = ix;

			fx += hScale;

		}

		float fy = 0.0f;

		int ly = 0;
		int j = 0;

		for (; j < h; j++) {

			int iy = (int)fy;

			pYScaleTable[j] = ly - iy;

			ly = iy; // was fy;

			fy += vScale;

		}

		// Figure out the 'start' position
		// --------------------------------------------------------------------

		int sx = clippedSrcRect.left;

		for (i = 0; i < a; i++) {

			if (pXScaleTable[i]) {

				++sx;

			}

		}

		int sy = clippedSrcRect.top;

		for (j = 0; j < b; j++) {

			if (pYScaleTable[j]) {

				++sy;

			}

		}

		// Check for flipping and adjust the dest position and step amount.
		// --------------------------------------------------------------------

		int dx, dy, ddx, ddy;

		if (blitFX) {

			if (BLITFX::HFLIP & blitFX->dwFlags) {

				// use the scaling table to find the starting sub pixel

				a = w - (cx + a);

				sx = clippedSrcRect.left;

				for (i = 0; i < a; i++) {

					if (pXScaleTable[i]) {

						++sx;

					}

				}

				dx = (clippedRect.right - 1);
				ddx = -1;

			}
			else {

				dx = clippedRect.left;
				ddx = 1;

			}

			if (BLITFX::VFLIP & blitFX->dwFlags) {

				// use the scaling table to find the starting sub pixel

				b = h - (cy + b);

				sy = clippedSrcRect.top;

				for (j = 0; j < b; j++) {

					if (pYScaleTable[j]) {

						++sy;

					}

				}

				dy = (clippedRect.bottom - 1);
				ddy = -1;

			}
			else {

				dy = clippedRect.top;
				ddy = 1;

			}

		}
		else {

			dx = clippedRect.left;
			ddx = 1;

			dy = clippedRect.top;
			ddy = 1;

		}

		// todo validate this
		// bpt.2021.10.16 - bugfix keep sx/sy inside the bounds of the clipped src area

		sx = max(clippedSrcRect.left, min(sx, clippedSrcRect.right - 1));
		sy = max(clippedSrcRect.top, min(sy, clippedSrcRect.bottom - 1));

		ASSERT((a < cached_w) && (b < cached_h) && (0 <= a) && (0 <= b));

		// Do each vertical line in the clipped operation rectangle
		// The general ddx & ddy could be removed and the code might see a
		// slight improvement in speed.
		// --------------------------------------------------------------------

		do {

			scale_table_type pVStep = (pYScaleTable + b);

			if (pVStep == pYScaleTableEnd)
			{
				break; /* NOP */
			}

			// Setup the scale raster operation
			// --------------------------------------------------------------------

			ASSERT((pXScaleTable + a) < pXScaleTableEnd);

			if ((pXScaleTable + a) == pXScaleTableEnd)
			{
				break; /* NOP */
			}

			scale_rop_type scaleOp(pXScaleTable + a, pXScaleTableEnd, op);

			if (1 == ddx) {

				for (int ly = 0; ly < cy; ly++) {

					auto dstRange = dstSurface.safer_Iterator(dx, dy, dstSurface.Width());
					auto srcRange = srcSurface.safer_Iterator(sx, sy, srcSurface.Width());

					if ((dstRange.second != dstRange.first) && (srcRange.second != srcRange.first))
					{
						scaleOp.InputMinor(dstRange.first, dstRange.second, srcRange.first, srcRange.second);
					}

					dy += ddy;

					if (*pVStep++) {

						if (pVStep == pYScaleTableEnd) break;
						++sy;

					}

				}

			}
			else {

				for (int ly = 0; ly < cy; ly++) {

					auto dstRange = dstSurface.safer_rIterator(dx, dy, dstSurface.Width());
					auto srcRange = srcSurface.safer_Iterator(sx, sy, srcSurface.Width());

					if ((dstRange.second != dstRange.first) && (srcRange.second != srcRange.first))
					{
						scaleOp.InputMinor(dstRange.first, dstRange.second, srcRange.first, srcRange.second);
					}

					dy += ddy;

					if (*pVStep++) {

						if (pVStep == pYScaleTableEnd) break;
						++sy;

					}

				}

			}

		} while (false);

		// Free up our scale tables
		// --------------------------------------------------------------------

		if (deleteTablesAfterOperation) {

			delete[] pYScaleTable;
			delete[] pXScaleTable;

			pXScaleTable = 0;
			cached_w = -1;

			pYScaleTable = 0;
			cached_h = -1;

		}

	}

} // namespace BPT

#endif // TSCALEUPBLIT_H__4EE3AB65_FB91_4E19_9877_81FB6B6BDBFD__INCLUDED_

