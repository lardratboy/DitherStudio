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

#if !defined(TPIXELTREE_H__4EE3AB65_FB91_4E19_9877_81FB6B6BDBFD__INCLUDED_)
#define TPIXELTREE_H__4EE3AB65_FB91_4E19_9877_81FB6B6BDBFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <deque>
#include <list>
#include <vector>
#include <iterator>
#include <algorithm>

#include "BPTBlitter.h"
#include "BPTPrimitives.h"

namespace BPT {

	//
	//	TPixelTreeNode
	//

	struct TPixelTreeNode {
	};

	//
	//	TPixelTree
	//

	template<typename T>
	struct TPixelTree {
		void operator()(T& t) {
			// TODO
		}
	};

	//
	//	TFillPixelTreeOp
	//

	template<typename T>
	struct TFillPixelTreeOp {
		T& tree;
		TFillPixelTreeOp(T& t) : tree(t) {}
		void operator()(T& t) {
			tree(t);
		}
	};

} // namespace BPT

#endif // TPIXELTREE_H__4EE3AB65_FB91_4E19_9877_81FB6B6BDBFD__INCLUDED_

