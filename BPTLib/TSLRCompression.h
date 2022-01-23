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


#if !defined(AFX_TSLRCOMPRESSION_H__C45ACB11_046F_48B6_B353_F787C12A310B__INCLUDED_)
#define AFX_TSLRCOMPRESSION_H__C45ACB11_046F_48B6_B353_F787C12A310B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ----------------------------------------------------------------------------

#include <vector>
#include <utility>

// ----------------------------------------------------------------------------

namespace BPT {

	// ========================================================================
	// 
	// SLR control BYTE format
	// 
	//	xxxxxxx1 == skip of (xxxxxxx + 1) 
	//	xxxxxx10 == run of (xxxxxx + 1) of the next value in the value stream
	//  xxxxxx00 == literal run (xxxxxx + 1) of next n values from the stream
	// 
	// The stream format varies depending on type that was compressed.
	// 
	// ========================================================================

	// ========================================================================
	// Compress data into code & value streams
	// ========================================================================

	//
	//	TSLRCompressor
	//
	
	template< class InIt, class T, class Pred >
	class TSLRCompressor {

	public:

		typedef unsigned char CODE_ENTRY;
		typedef std::vector<T> VALUE_COLLECTION;
		typedef std::vector<CODE_ENTRY> CODE_COLLECTION;
		typedef std::pair< CODE_COLLECTION, VALUE_COLLECTION > INFO;

		enum {

			SKIP_FLAG		= 0x01
			,RUN_FLAG		= 0x02

		};

	private:
	
		enum RUNTYPE {

			SKIP, RUN, LITERAL

		};

		enum {

			MAX_SKIP_LENGTH			= 128
			,MAX_RUN_LENGTH			= 64
			,MAX_LITERAL_LENGTH		= 64

		};

		void EmitCode( CODE_COLLECTION & out, RUNTYPE type, const int length ) {

			if ( SKIP == type ) {

				out.push_back( ((length - 1) << 1) | SKIP_FLAG );

			} else if ( RUN == type ) {

				out.push_back( ((length - 1) << 2) | RUN_FLAG );

			} else {

				out.push_back( ((length - 1) << 2) );

			}

		}
	
	public:

		bool operator()( INFO & out, InIt first, InIt last, Pred shouldStore ) {

			// Try to improve the efficiency of this routine by reserving
			// additional space in the collections for the new info.
			// ----------------------------------------------------------------

			VALUE_COLLECTION::size_type valueReserveCount = 
				info.second.size() + (last - first);

			info.second.reserve( valueReserveCount );

			VALUE_COLLECTION::size_type codeReserveCount =
				info.first.size() + ((last - first) / 2);

			info.first.reserve( codeReserveCount );

			// Figure out what type of code to start with skip or literal
			// ----------------------------------------------------------------
	
			RUNTYPE type;

			InIt it = first;

			T value = *it++;

			int count = 1;

			if ( shouldStore( value ) ) {

				type = LITERAL;
				info.second.push_back( value );

			} else {

				type = SKIP;

			}
	
			// Run through the remaining values collecting runs
			// ----------------------------------------------------------------

			while ( it != last ) {

				T lastValue = value;

				value = *it++;

				if ( value == lastValue ) {

					++count;

					if ( shouldStore( value ) ) {

						if ( count > MAX_RUN_LENGTH ) {

							EmitCode( info.first, RUN, MAX_RUN_LENGTH );
							info.second.push_back( value );
							type = LITERAL;
							count = 1;

						} else {

							type = RUN;

						}

					} else {

						type = SKIP;

						if ( count > MAX_SKIP_LENGTH ) {

							EmitCode( info.first, SKIP, MAX_SKIP_LENGTH );
							count = 1;

						}

					}

				} else {

					if ( (LITERAL == type) && shouldStore( value ) ) {

						info.second.push_back( value );

						++count;

						if ( count > MAX_LITERAL_LENGTH ) {

							EmitCode( info.first, LITERAL, MAX_LITERAL_LENGTH );
							count = 1;

						}

					} else {

						// Add the last 'code' to the codes collection

						EmitCode( info.first, type, count );

						// now determine what type of code to start

						count = 1;

						if ( shouldStore( value ) ) {

							info.second.push_back( value );
							type = LITERAL;

						} else {

							type = SKIP;

						}

					}

				}
	
			}

			// Dump the last code to the code collection

			EmitCode( info.first, type, count );

			return true;
	
		}
	
	};

	// ========================================================================
	// Decode
	// ========================================================================

	//
	//	TDecodeSLR<>
	//

	class TDecodeSLR {

	private:

	public:

		TDecodeSLR() { /* Empty */ }

		template<
			class DST_IT,
			class CODE_IT,
			class VALUE_IT,
			class VALUE,
			class TOP
		>
		__forceinline void operator()(
			DST_IT dstIT, CODE_IT codes, VALUE_IT values,
			int skipCount, int writeCount, TOP top ) {

			// Handle any skipping
			// ----------------------------------------------------------------

			int runCount;

			while ( 0 < skipCount ) {

				int code = static_cast<int>( *codes++ );

				if ( code & TSLRCompressor::SKIP_FLAG ) {

					runCount = (code >> 1) + 1;
					skipCount -= runCount;

					if ( 0 > skipCount ) {

						runCount += skipCount;
						writeCount -= runCount;
						dstIT += runCount;
						break;

					}

				} else if ( code & TSLRCompressor::RUN_FLAG ) {

					runCount = (code >> 2) + 1;
					skipCount -= runCOunt;

					if ( 0 > skipCount ) {

						runCount += skipCount;
						goto HANDLE_RUN;

					}

					++values;

				} else {

					runCount = (code >> 2) + 1;
					skipCount -= runCOunt;

					if ( 0 > skipCount ) {

						values -= skipCount;
						runCount += skipCount;
						goto HANDLE_LITERAL;

					}

					values += runCount;

				}

			}

			// Finally get down
			// ----------------------------------------------------------------

			while ( 0 < writeCount ) {

				int code = static_cast<int>( *codes++ );

				if ( code & TSLRCompressor::SKIP_FLAG ) {

					runCount = (code >> 1) + 1;
					writeCount -= runCount;
					dstIT += runCount;

				} else if ( code & TSLRCompressor::RUN_FLAG ) {

					runCount = (code >> 2) + 1;

				HANDLE_RUN:

					T value = *values++;

					writeCount -= runCount;

					if ( 0 > writeCount ) {

						runCount += writeCount;

					}

					top.OutputMajor( dstIT, dstIT + runCount, value );
					dstIT += runCount;

				} else {

					runCount = (code >> 2) + 1;

				HANDLE_LITERAL:

					writeCount -= runCount;

					if ( 0 > writeCount ) {

						runCount += writeCount;

					}

					top.InputMajor( values, values + runCount, dstIT );
					values += runCount;
					dstIT += runCount;

				}

			}

		}

	};

}; // namespace BPT

#endif // !defined(AFX_TSLRCOMPRESSION_H__C45ACB11_046F_48B6_B353_F787C12A310B__INCLUDED_)
