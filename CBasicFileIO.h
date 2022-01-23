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

#if !defined(TBASICFILEIO_H__9D2B4C1A_1D42_4364_A93E_65D59EF0F741__INCLUDED_)
#define TBASICFILEIO_H__9D2B4C1A_1D42_4364_A93E_65D59EF0F741__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ----------------------------------------------------------------------------

#include <Windows.h>
#include <list>
#include <utility>
#include <cstdio>

// ----------------------------------------------------------------------------

namespace BPT {

	class CBasicFileIO {

	private:

		FILE* m_Handle;

	public:

		CBasicFileIO() : m_Handle(0) { /* Empty */ }

		~CBasicFileIO() {

			if (m_Handle) {

				Close();

			}

		}

		bool Open(const char* filename, const char* access) {

			m_Handle = fopen(filename, access);

			return (0 != m_Handle);

		}

		void Close() {

			if (m_Handle) {

				fclose(m_Handle);
				m_Handle = 0;

			}

		}

		void Seek(const int pos, const int mode) {

			if (m_Handle) {

				fseek(m_Handle, pos, mode);

			}

		}

		int ReadByte() {

			if (m_Handle) {

				return (unsigned)fgetc(m_Handle) & 255;

			}

			return 0;

		}

		int Read_m16() {

			int hi = ReadByte();
			int lo = ReadByte();

			return ((hi << 8) | lo);

		}

		int Read_m32() {

			int a = ReadByte();
			int b = ReadByte();
			int c = ReadByte();
			int d = ReadByte();

			return (int)((a << 24) | (b << 16) | (c << 8) | d);

		}

		int Read_16() {

			int lo = ReadByte();
			int hi = ReadByte();

			return ((hi << 8) | lo);

		}

		int Read_32() {

			int d = ReadByte();
			int c = ReadByte();
			int b = ReadByte();
			int a = ReadByte();

			return (int)((a << 24) | (b << 16) | (c << 8) | d);

		}

		BYTE* LoadDataInto(BYTE* ptr, const size_t nBytes)
		{
			size_t read = fread(ptr, 1, nBytes, m_Handle);

			ASSERT(read == nBytes);

			return ptr;
		}

		BYTE* LoadData(const int nBytes) {

			if (m_Handle) {

				void* ptr = ::CoTaskMemAlloc(nBytes);

				if (0 == ptr) {

					return 0;

				}

				fread(ptr, 1, nBytes, m_Handle);

				return static_cast<BYTE*>(ptr);

			}

			return static_cast<BYTE*>(0);

		}

		void UnloadData(BYTE* ptr) {

			if (ptr) {

				::CoTaskMemFree(static_cast<void*>(ptr));

			}

		}

		static size_t GetFileSize(const char* filename)
		{
			struct stat stat_buf;
			int rc = stat(filename, &stat_buf);
			return rc == 0 ? stat_buf.st_size : -1;
		}
	};


}; // namespace BPT

#endif // !defined(TBASICFILEIO_H__9D2B4C1A_1D42_4364_A93E_65D59EF0F741__INCLUDED_)
