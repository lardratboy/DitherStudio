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

#if !defined(TFACE3VECTOR3_H__4EE3AB65_FB91_4E19_9877_81FB6B6BDBFD__INCLUDED_)
#define TFACE3VECTOR3_H__4EE3AB65_FB91_4E19_9877_81FB6B6BDBFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cmath>
#include <algorithm>

namespace BPT {

	struct VECTOR3 {

		// --------------------------------------------------------------------

		float _x, _y, _z, _u, _v, _nx, _ny, _nz;

		// --------------------------------------------------------------------

		VECTOR3(
			const float x = 0.0f,
			const float y = 0.0f,
			const float z = 0.0f,
			const float u = 0.0f,
			const float v = 0.0f,
			const float nx = 0.0f,
			const float ny = 0.0f,
			const float nz = 0.0f
		) : _x(x), _y(y), _z(z), _u(u), _v(v), _nx(nx), _ny(ny), _nz(nz) {}

		// --------------------------------------------------------------------

		static VECTOR3 Midpoint(const VECTOR3& lhs, const VECTOR3& rhs) {

			return VECTOR3(
				(lhs._x + rhs._x) * 0.5f,
				(lhs._y + rhs._y) * 0.5f,
				(lhs._z + rhs._z) * 0.5f,
				(lhs._u + rhs._u) * 0.5f,
				(lhs._v + rhs._v) * 0.5f
			);

		}

		static VECTOR3 CrossProduct(const VECTOR3& lhs, const VECTOR3& rhs) {

			return VECTOR3(
				(lhs._y * rhs._z) - (lhs._z * rhs._y),
				(lhs._z * rhs._x) - (lhs._x * rhs._z),
				(lhs._x * rhs._y) - (lhs._y * rhs._x),
				0.0f,
				0.0f
			);

		}

		static float DotProduct(const VECTOR3& lhs, const VECTOR3& rhs) {

			return (lhs._x * rhs._x) + (lhs._y * rhs._y) + (lhs._z * rhs._z);

		}

#if 0
		VECTOR3 operator+(const VECTOR3& lhs, const VECTOR3& rhs) {

			return VECTOR3(
				lhs._x + rhs._x,
				lhs._y + rhs._y,
				lhs._z + rhs._z,
				lhs._u + rhs._u,
				lhs._v + rhs._v
			);

		}

		VECTOR3 operator-(const VECTOR3& lhs, const VECTOR3& rhs) {

			return VECTOR3(
				lhs._x - rhs._x,
				lhs._y - rhs._y,
				lhs._z - rhs._z,
				lhs._u - rhs._u,
				lhs._v - rhs._v
			);

		}

		VECTOR3 operator*(const VECTOR3& lhs, const float rhs) {

			return VECTOR3(
				lhs._x * rhs,
				lhs._y * rhs,
				lhs._z * rhs,
				lhs._u * rhs,
				lhs._v * rhs
			);

		}

		VECTOR3 operator*(const float lhs, const VECTOR3& rhs) {

			return VECTOR3(
				rhs._x * lhs,
				rhs._y * lhs,
				rhs._z * lhs,
				rhs._u * lhs,
				rhs._v * lhs
			);

		}

		VECTOR3 operator/(const VECTOR3& lhs, const float rhs) {

			if (0.0f == rhs) {

				return VECTOR3();

			}

			float oorhs = 1.0f / rhs;

			return VECTOR3(
				lhs._x * oorhs,
				lhs._y * oorhs,
				lhs._z * oorhs,
				lhs._u * oorhs,
				lhs._v * oorhs
			);

		}

#endif

		// query
		// --------------------------------------------------------------------

		float LengthSquared() const {

			return (_x * _x) + (_y * _y) + (_z * _z);

		}

		float Length() const {

			return sqrtf(LengthSquared());

		}

		float x() const {

			return _x;

		}

		float y() const {

			return _y;

		}

		float z() const {

			return _z;

		}

		float u() const {

			return _u;

		}

		float v() const {

			return _v;

		}

		// action
		// --------------------------------------------------------------------

		void Normalize() {

			float fLen = Length();

			if (0.0f != fLen) {

				float ool = 1.0f / fLen;

				_x *= ool;
				_y *= ool;
				_z *= ool;

			}
			else {

				_x = 0.0f;
				_y = 0.0f;
				_z = 0.0f;

			}

		}

	}; // struct VECTOR3

	struct FACE3 {

		VECTOR3 p1;
		VECTOR3 p2;
		VECTOR3 p3;
		VECTOR3 centroid;

	}; // struct FACE3

} // namespace BPT

#endif // TFACE3VECTOR3_H__4EE3AB65_FB91_4E19_9877_81FB6B6BDBFD__INCLUDED_

