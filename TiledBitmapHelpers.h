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

#if !defined(TILEDBITMAPHELPERS_H__4EE3AB65_FB91_4E19_9877_81FB6B6BDBFD__INCLUDED_)
#define TILEDBITMAPHELPERS_H__4EE3AB65_FB91_4E19_9877_81FB6B6BDBFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BPTBlitter.h"
#include "BPTPrimitives.h"

namespace BPT {

	//
	//	TiledIterateOverBitmap
	//

	template< typename SURF, typename FN >
	void TiledIterationOfBitmap(
		SURF& sourceSurface, const SIZE& tileSize, const SIZE& tileStepSize, FN& fn
	) {
		ASSERT((0 < tileSize.cx) && (0 < tileSize.cy) && (0 < tileStepSize.cx) && (0 < tileStepSize.cy)&&(tileSize.cx<=sourceSurface.Width())&&(tileSize.cy<=sourceSurface.Height()));

		SURF tileBitmapView;

		int w = sourceSurface.Width(); // -(tileSize.cx - 1);
		int h = sourceSurface.Height(); // -(tileSize.cy - 1);

		for (int y = 0; y < h; y += tileStepSize.cy ) {

			for (int x = 0; x < w; x += tileStepSize.cx) {

				RECT view = { x, y, x + tileSize.cx, y + tileSize.cy };

				if (tileBitmapView.MakeBitmapViewForRect(sourceSurface, view))
				{
					fn(tileBitmapView);
				}
				else
				{
					TRACE("LOOK! unable to make view %d,%d,%d,%d\n", view.left, view.top, view.right, view.bottom);
					ASSERT(false);
				}

			}
		}
	}

	template<typename T>
	struct BlitTilesOntoBitmapOp {

		TCopyROP<typename T::pixel_type> top;
		T* pTarget;
		int x, y;

		BlitTilesOntoBitmapOp(T* p) : pTarget(p), x(0), y(0)
		{
		}

		void operator()(T& tile) {

			ASSERT((0 <= x) || (x < pTarget->Width()) || (0 <= y) || (y < pTarget->Height()));

#if 0
			// debugging
			RECT rc = { x, y, x + tile.Width(), y + tile.Height() };
			T_SolidRectPrim(*pTarget, rc, T::pixel_type::rnd(), top );
#else
			T_Blit(pTarget, x, y, &tile, top);
#endif

			x += tile.Width();

			if (pTarget->Width() <= x) {

				x = 0;
				y += tile.Height();

			}

		}

	};

} // namespace BPT

#endif // TILEDBITMAPHELPERS_H__4EE3AB65_FB91_4E19_9877_81FB6B6BDBFD__INCLUDED_

