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


#if !defined(AFX_TCLUT_H__089B7BB5_B7BF_4BC5_B901_94FF6EC6D902__INCLUDED_)
#define AFX_TCLUT_H__089B7BB5_B7BF_4BC5_B901_94FF6EC6D902__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace BPT {

	// ========================================================================

#if 1 // From dither studio

	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//	CLUT table
	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	//
	//	TPow2Clut
	//

	template< typename O, const int POW2 = 8 >
	class TPow2Clut {

	public: // traits

		typedef O output_type;

		enum {

			TABLE_SIZE = (1 << POW2)
			, INDEX_MASK = (1 << POW2) - 1

		};

	private: // data

		output_type m_Table[TABLE_SIZE];

	public: // interface

		output_type& RawLookup(const int i) {

			return m_Table[i];

		}

		output_type& SafeLookup(const int i) {

			return RawLookup(i & INDEX_MASK);

		}

		output_type& operator[](const int i) {

#ifdef _DEBUG

			return SafeLookup(i);

#else

			return RawLookup(i);

#endif

		}

		bool Included(const output_type& value) const {

			for (int i = 0; i < TABLE_SIZE; i++) {

				if (value == m_Table[i]) {

					return true;

				}

			}

			return false;

		}

		int FindIndex(const output_type& value, const int defValue) const {

			for (int i = 0; i < TABLE_SIZE; i++) {

				if (value == m_Table[i]) {

					return i;

				}

			}

			return defValue;

		}

		// Set
		// --------------------------------------------------------------------

		void Set(const int index, const output_type& output) {

			m_Table[(index & INDEX_MASK)] = output;

		}

		void SetN(const int startIndex, const int count, const output_type& output) {

			int index = startIndex;

			for (int i = 0; i < count; i++) {

				Set(index++, output);

}

		}

		// --------------------------------------------------------------------

		output_type* GetLowLevelTablePtr() {

			return m_Table;

		}

	};

#else

	//
	//	TPow2Clut
	//

	template< class O, const int POW2 = 8 >
	class TPow2Clut {

	public:

		typedef O output_type;

		enum {

			TABLE_SIZE = (1 << POW2)
			,INDEX_MASK = (1 << POW2) - 1;

		};

	private:

		output_type m_Table[ TABLE_SIZE ];

	public:

		// Query
		// --------------------------------------------------------------------

		output_type & RawLookup( const int i ) {

			return m_Table[ i ];

		}

		output_type & Lookup( const int i ) {

			return RawLookup( i & INDEX_MASK );

		}

		output_type & operator[]( const int i ) {

			return Lookup( i );

		}

		bool Included( const output_type & value ) const {

			for ( int i = 0; i < TABLE_SIZE; i++ ) {

				if ( value == m_Table[ i ] ) {

					return true;

				}

			}

			return false;

		}

		int FindIndex( const output_type & value, const int defValue ) const {
			
			for ( int i = 0; i < TABLE_SIZE; i++ ) {

				if ( value == m_Table[ i ] ) {

					return i;

				}

			}

			return defValue;

		}

		// Set
		// --------------------------------------------------------------------

		void Set( const int index, const output_type & output ) {

			m_Table[ (index & INDEX_MASK) ] = output;

		}

		void SetN( const int startIndex, const int count, const output_type & output ) {

			int index = startIndex;

			for ( int i = 0; i < count; i++ ) {

				Set( index++, output );

			}

		}

	};

#endif

	// ========================================================================


}; // namespace BPT

#endif // !defined(AFX_TCLUT_H__089B7BB5_B7BF_4BC5_B901_94FF6EC6D902__INCLUDED_)
