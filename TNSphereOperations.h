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

#if !defined(TNSPHEREOPERATIONS_H__4EE3AB65_FB91_4E19_9877_81FB6B6BDBFD__INCLUDED_)
#define TNSPHEREOPERATIONS_H__4EE3AB65_FB91_4E19_9877_81FB6B6BDBFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TFace3Vector3.h"
#include <cmath>
#include <algorithm>

namespace BPT {

	//
	//	CreateNSphere
	//

	template< class TFACE >
	int CreateNSphere(TFACE* f, const int iterations, const bool bReverseFaces = false) {

		// TFACE should expose a vector type...

		VECTOR3 p[9];

		p[0] = VECTOR3(+0.0f, +1.0f, +0.0f, 0.5f, 0.5f);
		p[1] = VECTOR3(-1.0f, +0.0f, +0.0f, 0.5f, 0.0f);
		p[2] = VECTOR3(+0.0f, +0.0f, +1.0f, 1.0f, 0.5f);
		p[3] = VECTOR3(+1.0f, +0.0f, +0.0f, 0.5f, 1.0f);
		p[4] = VECTOR3(+0.0f, +0.0f, -1.0f, 0.0f, 0.5f);
		p[5] = VECTOR3(+0.0f, -1.0f, +0.0f, 0.0f, 0.0f);
		p[6] = VECTOR3(+0.0f, -1.0f, +0.0f, 1.0f, 0.0f);
		p[7] = VECTOR3(+0.0f, -1.0f, +0.0f, 1.0f, 1.0f);
		p[8] = VECTOR3(+0.0f, -1.0f, +0.0f, 0.0f, 1.0f);

		VECTOR3 pa, pb, pc;

		int nt = 0;

		// Adjust our edges

		{
			for (int i = 0; i < 6; i++) {

				p[i].Normalize();

			}
		}

		if (bReverseFaces) {

			f[0].p1 = p[1]; f[0].p2 = p[0]; f[0].p3 = p[2];
			f[1].p1 = p[2]; f[1].p2 = p[0]; f[1].p3 = p[3];
			f[2].p1 = p[3]; f[2].p2 = p[0]; f[2].p3 = p[4];
			f[3].p1 = p[4]; f[3].p2 = p[0]; f[3].p3 = p[1];
			f[4].p1 = p[2]; f[4].p2 = p[6]; f[4].p3 = p[1];
			f[5].p1 = p[3]; f[5].p2 = p[7]; f[5].p3 = p[2];
			f[6].p1 = p[4]; f[6].p2 = p[8]; f[6].p3 = p[3];
			f[7].p1 = p[1]; f[7].p2 = p[5]; f[7].p3 = p[4];

		}
		else {

			f[0].p1 = p[2]; f[0].p2 = p[0]; f[0].p3 = p[1];
			f[1].p1 = p[3]; f[1].p2 = p[0]; f[1].p3 = p[2];
			f[2].p1 = p[4]; f[2].p2 = p[0]; f[2].p3 = p[3];
			f[3].p1 = p[1]; f[3].p2 = p[0]; f[3].p3 = p[4];
			f[4].p1 = p[1]; f[4].p2 = p[6]; f[4].p3 = p[2];
			f[5].p1 = p[2]; f[5].p2 = p[7]; f[5].p3 = p[3];
			f[6].p1 = p[3]; f[6].p2 = p[8]; f[6].p3 = p[4];
			f[7].p1 = p[4]; f[7].p2 = p[5]; f[7].p3 = p[1];

		}

		nt = 8;

		if (iterations < 1) return(nt);

		for (int it = 0; it < iterations; it++) {

			int ntold = nt;

			for (int i = 0; i < ntold; i++) {

				// bisect the face edges

				pa = VECTOR3::Midpoint(f[i].p1, f[i].p2);
				pb = VECTOR3::Midpoint(f[i].p2, f[i].p3);
				pc = VECTOR3::Midpoint(f[i].p3, f[i].p1);

				// normalize them to be points on the unit sphere

				pa.Normalize();
				pb.Normalize();
				pc.Normalize();

				// build 3 new faces

				f[nt].p1 = f[i].p1; f[nt].p2 = pa; f[nt].p3 = pc; nt++;
				f[nt].p1 = pa; f[nt].p2 = f[i].p2; f[nt].p3 = pb; nt++;
				f[nt].p1 = pb; f[nt].p2 = f[i].p3; f[nt].p3 = pc; nt++;

				// change existing face position to the new points

				f[i].p1 = pa;
				f[i].p2 = pb;
				f[i].p3 = pc;

			}

		}

		return(nt);
	}

} // namespace BPT

#endif // TFIBONACCISPHEREOPERATIONS_H__4EE3AB65_FB91_4E19_9877_81FB6B6BDBFD__INCLUDED_

