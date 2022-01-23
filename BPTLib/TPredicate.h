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


#if !defined(AFX_TPREDICATE_H__F32A4F21_9C1F_448D_9902_0174667273F4__INCLUDED_)
#define AFX_TPREDICATE_H__F32A4F21_9C1F_448D_9902_0174667273F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ----------------------------------------------------------------------------

#include <functional>

// ----------------------------------------------------------------------------

namespace BPT {

	// ========================================================================
	// Unary predicates
	// ========================================================================

	//
	//	TIsValue
	//

	template< class T > class TIsValue : public std::unary_function<T,bool> {

	private:

		T m_Value;

		TIsValue(); // Hidden

	public:

		TIsValue( const T & value ) : m_Value( value ) { /* Empty */ }

		bool operator()( const T & value ) const {

			return (value == m_Value);

		}

	};

	//
	//	TIsNotValue
	//

	template< class T > class TIsNotValue : public std::unary_function<T,bool> {

	private:

		T m_Value;

		TIsNotValue(); // Hidden

	public:

		TIsNotValue( const T & value ) : m_Value( value ) { /* Empty */ }

		bool operator()( const T & value ) const {

			return (value != m_Value);

		}

	};

	//
	//	T100PercentAlpha
	//

	template< class T > struct T100PercentAlpha : public std::unary_function<T,bool> {

		T100PercentAlpha() { /* Empty */ }

		bool operator()( const T & value ) const {

			return (T::A_LimitedMax == value._A());

		}

	};

	//
	//	T0PercentAlpha
	//

	template< class T > struct T0PercentAlpha : public std::unary_function<T,bool> {

		T0PercentAlpha() { /* Empty */ }

		bool operator()( const T & value ) const {

			return (0 == value._A()) && (0 != T::A_LimitedMax);

		}

	};

	//
	//	TNonZeroAlpha
	//

	template< class T > struct TNonZeroAlpha : public std::unary_function<T,bool> {

		TNonZeroAlpha() { /* Empty */ }

		bool operator()( const T & value ) const {

			return (0 != value._A()) || (0 == T::A_LimitedMax);

		}

	};

	//
	//	TPartialAlpha
	//

	template< class T > class TPartialAlpha : public std::unary_function<T,bool> {

	private:

		T100PercentAlpha<T> m_Test100Percent;
		T0PercentAlpha<T> m_Test0Percent;

	public:

		TPartialAlpha() { /* Empty */ }

		bool operator()( const T & value ) const {

			if ( m_Test100Percent( value ) || m_Test0Percent( value ) ) return false;

			return true;

		}

	};

	// ========================================================================

// need to look closer at the STL for a standard way to combine predicates

	//
	//	TDoublePredicate
	//

	template< class T, class P1, class P2 > class TDoublePredicate : public std::unary_function<T,bool> {

	private:

		TDoublePredicate(); // Hidden

		P1 m_P1;
		P2 m_P2;

	public:

		TDoublePredicate( const P1 & p1, const P2 & p2 ) : m_P1( p1 ), m_P2( p2 ) { /* Empty */ }

		bool operator()( const T & value ) const {

			return (m_P1(value) && m_P2(value));

		}

	};

	// ------------------------------------------------------------------------

	template< class T >
	struct TDoubleIsNotValue : public TDoublePredicate< T, TIsNotValue<T>, TIsNotValue<T> > {

		TDoubleIsNotValue( const T a, const T b ) : 
			TDoublePredicate< T, TIsNotValue<T>, TIsNotValue<T> >(
				TIsNotValue<T>( a ), TIsNotValue<T>( b ) ) {

			/* Empty */

		}

	};

}; // namespace BPT

#endif // !defined(AFX_TPREDICATE_H__F32A4F21_9C1F_448D_9902_0174667273F4__INCLUDED_)
