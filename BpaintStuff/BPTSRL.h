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

#if !defined(AFX_BPTSRL_H__CCC1EB59_863C_4490_84CA_C1F171D48D87__INCLUDED_)
#define AFX_BPTSRL_H__CCC1EB59_863C_4490_84CA_C1F171D48D87__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ----------------------------------------------------------------------------

//#define SRL_RESERVE_MAX_AMOUNTS
//#define SRL_USE_STD_COLLECTION

// Based on CObject for debug memory leak tracking 

// ----------------------------------------------------------------------------

#include <vector>
#include <utility>

#include "BPTBlitter.h"
//#include "BPTFileio.h"

// ----------------------------------------------------------------------------

namespace BPT {

	struct TSRLDebug : public CObject {};

	// ========================================================================
	// 
	// SRL control BYTE format
	// 
	//	xxxxxxx1 == SKIP code of (xxxxxxx + 1) 
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
	//	TSRLCompressor
	//
	
	template< class T >
	class TSRLCompressor : public TSRLDebug {

	public:

		typedef T VALUE_TYPE;
		typedef unsigned char CODE_ENTRY;
		typedef std::vector<VALUE_TYPE> VALUE_COLLECTION;
		typedef std::vector<CODE_ENTRY> CODE_COLLECTION;
		typedef std::pair< CODE_COLLECTION, VALUE_COLLECTION > INFO;

		enum : unsigned {

			SKIP_FLAG		= 0x01 // TODO SRL2
			,RUN_FLAG		= 0x02

		};

	protected:
	
		enum RUNTYPE : unsigned {

			SKIP, // TODO SRL2
			RUN,
			LITERAL

		};

		enum : unsigned {

			MAX_SKIP_LENGTH			= 127 // TODO SRL2
			,MAX_RUN_LENGTH			= 63
			,MAX_LITERAL_LENGTH		= 63

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

		template< class InIt, class Pred >
		bool operator()( INFO & info, InIt first, InIt last, Pred shouldStore ) {

			// Figure out what type of code to start with skip or literal
			// ----------------------------------------------------------------
	
			RUNTYPE type;

			InIt it = first;

			T value = *it++;

			int count = 1;

			bool bStore = shouldStore( value );

			if ( bStore ) {

				type = LITERAL;

				info.second.push_back( value );

			} else {

				type = SKIP;

			}
	
			// Run through the remaining values collecting runs
			// ----------------------------------------------------------------

			while ( it != last ) {

				bool bLastStore = bStore;

				T lastValue = value;

				value = *it++;

				bStore = shouldStore( value );

				if ( (value == lastValue) && (bLastStore == bStore) ) {

					if ( (LITERAL == type) && (1 != count) ) {

						EmitCode( info.first, type, count - 1 );

						count = 1;

					}

					++count;

					if ( bStore ) {

						if ( count > MAX_RUN_LENGTH ) {

							EmitCode( info.first, RUN, MAX_RUN_LENGTH );

							info.second.push_back( value );

							type = LITERAL;

							count = 1;

						} else {

							type = RUN;

						}

					} else {

						if ( count > MAX_SKIP_LENGTH ) {

							EmitCode( info.first, SKIP, MAX_SKIP_LENGTH );

							count = 1;

						}

					}

				} else {

					if ( (LITERAL == type) && bStore ) {

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

						if ( bStore ) {

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
	
	}; // class TSRLCompressor

	// ========================================================================
	// Decode
	// ========================================================================

	//
	//	TDecodeSRL<>
	//

	template< class SRL >
	class TDecodeSRL /* : public CObject */ {

	private:

	public:

		TDecodeSRL() { /* Empty */ }

		// --------------------------------------------------------------------

		template<
			class DST_IT,
			class CODE_IT,
			class VALUE_IT,
			class TOP
		>
		__forceinline void operator()(
			DST_IT dstIT, CODE_IT codes, VALUE_IT values,
			int skipCount, int writeCount, TOP top ) {

			// Handle any skipping
			// ----------------------------------------------------------------

			int runCount = 0;
			int code = 0;

			while ( 0 < skipCount ) {

				int sCode = static_cast<int>( *codes++ );

				if ( sCode & SRL::SKIP_FLAG ) {

					runCount = (sCode >> 1) + 1;

					skipCount -= runCount;

					if ( 0 > skipCount ) {

						writeCount += skipCount;
						dstIT -= skipCount;
						break;

					}

				} else if ( sCode & SRL::RUN_FLAG ) {

					runCount = (sCode >> 2) + 1;

					skipCount -= runCount;

					if ( 0 > skipCount ) {

						runCount = -skipCount;

						goto HANDLE_RUN;

					}

					++values;

				} else {

					runCount = (sCode >> 2) + 1;

					values += runCount;

					skipCount -= runCount;

					if ( 0 > skipCount ) {

						values += skipCount;
						runCount = -skipCount;
						goto HANDLE_LITERAL;

					}


				}

			}

			// Finally get down
			// ----------------------------------------------------------------

			while ( 0 < writeCount ) {

				code = static_cast<int>( *codes++ );

				if ( code & SRL::SKIP_FLAG ) {

					runCount = (code >> 1) + 1;
					writeCount -= runCount;
					dstIT += runCount;

				} else if ( code & SRL::RUN_FLAG ) {

					runCount = (code >> 2) + 1;

				HANDLE_RUN:

					typename SRL::VALUE_TYPE value = *values++;

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

		// --------------------------------------------------------------------

		template<
			class CODE_IT,
			class VALUE_IT
		>
		__forceinline bool HitTest(
			CODE_IT codes, VALUE_IT values, int skipCount, 
			typename SRL::VALUE_TYPE * pOptionalOutValue = 0 ) {

			// Handle any 'skipping' to get to the correct value
			// ----------------------------------------------------------------

			while ( 0 < skipCount ) {

				int sCode = static_cast<int>( *codes++ );

				if ( sCode & SRL::SKIP_FLAG ) {

					int runCount = (sCode >> 1) + 1;

					skipCount -= runCount;

					if ( 0 > skipCount ) {

						return false;

					}

				} else if ( sCode & SRL::RUN_FLAG ) {

					int runCount = (sCode >> 2) + 1;

					skipCount -= runCount;

					if ( 0 > skipCount ) {

						goto HANDLE_RUN;

					}

					++values;

				} else {

					int runCount = (sCode >> 2) + 1;

					values += runCount;

					skipCount -= runCount;

					if ( 0 > skipCount ) {

						values += skipCount;

						goto HANDLE_LITERAL;

					}


				}

			}

			// ----------------------------------------------------------------

			if ( SRL::SKIP_FLAG & static_cast<int>( *codes++ ) ) {

				return false;

    		}

			HANDLE_RUN:
			HANDLE_LITERAL:

			if ( pOptionalOutValue ) {

				*pOptionalOutValue = *values;

			}

			return true;

		}

	}; // class TDecodeSRL

	// ========================================================================
	// ========================================================================

	template< class SRL >
	class TSRLCompressedImage /* : public CObject */ {

	public: // Traits

		typedef typename SRL::VALUE_TYPE storage_type;
		typedef SRL srl_type;
		typedef TSRLCompressedImage<SRL> this_type;

	protected: // Data

		typename srl_type::INFO * m_CompressedInfo;

		SIZE m_Size;

		srl_type m_Compressor;

		TDecodeSRL<SRL> m_Decompressor;

		size_t m_CompressedSizeEstimate;

	public: // Interface

		TSRLCompressedImage() {

			m_CompressedSizeEstimate = 0;

			m_Size.cx = 0;
			m_Size.cy = 0;
			m_CompressedInfo = 0;

		};

		~TSRLCompressedImage() {

			Destroy();
		
		}

		// --------------------------------------------------------------------

		RECT Rect() const {

			return { 0, 0, m_Size.cx, m_Size.cy };

		}

		SIZE Size() const {

			return m_Size;

		}

		size_t UncompressedSizeEstimate() const {

			return m_Size.cx * m_Size.cy * sizeof(storage_type);

		}

		size_t CompressedSizeEstimate() const {

			return m_CompressedSizeEstimate;

		}

		bool HasData() const {

			return (0 != m_Size.cx) && (0 != m_Size.cy);

		}

		// --------------------------------------------------------------------

		void Destroy() {

			if ( m_CompressedInfo ) {

				delete [] m_CompressedInfo;

				m_CompressedInfo = 0;

			}

			m_Size.cx = 0;
			m_Size.cy = 0;

			m_CompressedSizeEstimate = 0;

		}

		// --------------------------------------------------------------------

		template< class SURFACE, class PREDICATE >
		bool Create( SURFACE & srcSurface, PREDICATE predicate, const RECT * pRect = 0 ) {

			// Destroy any existing information
			// ----------------------------------------------------------------

			Destroy();

			// Determine/limit the size of the operation and reserve elements
			// ----------------------------------------------------------------

			RECT surfaceRect = srcSurface.Rect();

			if ( pRect ) {

				if ( !IntersectRect( &surfaceRect, &surfaceRect, pRect ) ) {

					return false;

				}

			}

			int cx = surfaceRect.right - surfaceRect.left;
			int cy = surfaceRect.bottom - surfaceRect.top;

			m_CompressedInfo = new typename srl_type::INFO [ cy ];

			if ( !m_CompressedInfo ) {

				return false;

			}

			// For each of the lines in the surface call the compressor
			// ----------------------------------------------------------------

			int codesCount = 0;
			int valuesCount = 0;

			for ( int y = 0; y < cy; y++ ) {

				typename SURFACE::pixel_iterator it = srcSurface.Iterator(
					surfaceRect.left, y + surfaceRect.top
				);

				if ( !m_Compressor( m_CompressedInfo[ y ], it, it + cx, predicate ) ) {

					Destroy();

					return false;

				}

				codesCount += m_CompressedInfo[ y ].first.size();
				valuesCount += m_CompressedInfo[ y ].second.size();

			}

			m_CompressedSizeEstimate =
				(sizeof( typename srl_type::INFO ) * cy) +
				(codesCount * sizeof( typename srl_type::CODE_ENTRY )) +
				(valuesCount * sizeof( typename srl_type::VALUE_TYPE ));

			m_Size.cx = cx;
			m_Size.cy = cy;

			return true;

		}

		// --------------------------------------------------------------------

		//
		//	BlitCore
		//
		//	-- This can accept other forms of the decompressor!
		//
	
		template< class DST_SURFACE, class TOP, class DECOMPRESSOR >
		void __forceinline BlitCore(
			DECOMPRESSOR & decompressor,
			DST_SURFACE & dstSurface
			,const int x
			,const int y
			,TOP op = TOP()
			,const BLITFX * blitFX = 0
			,const RECT * optionalDstClipRect = 0
			,const RECT * optionalSrcSubRect = 0
		) {

			// Check for an empty bitmap.
			// ----------------------------------------------------------------

			if ( (0 == m_Size.cx) || (0 == m_Size.cy) ) {

				return /* NOP */;

			}
	
			// Clip the optional clipping rect to the dest bitmap limits
			// ----------------------------------------------------------------
	
			RECT dstLimitsRect = dstSurface.Rect();
	
			RECT clippedDstRect;
	
			if ( optionalDstClipRect ) {
	
				if ( !IntersectRect( &clippedDstRect, &dstLimitsRect, optionalDstClipRect) ) {
	
					return /* NOP */;
	
				}
	
			} else {
	
				clippedDstRect = dstLimitsRect;
	
			}
	
			// Get the source operation size
			// ----------------------------------------------------------------
	
			RECT srcLimitsRect = { 0, 0, m_Size.cx, m_Size.cy };
	
			RECT clippedSrcRect;
	
			if ( optionalSrcSubRect ) {
	
				if ( !IntersectRect( &clippedSrcRect, &srcLimitsRect, optionalSrcSubRect) ) {
	
					return /* NOP */;
	
				}
	
			} else {
	
				clippedSrcRect = srcLimitsRect;
	
			}
	
			// Perform a simple clipping operation to detect NOP
			// ----------------------------------------------------------------
	
			SIZE clippedSrcRectSize = SizeOfRect( &clippedSrcRect );
	
			RECT dstOperation = {
				x, y, x + clippedSrcRectSize.cx, y + clippedSrcRectSize.cy
			};
	
			RECT clippedRect;
				
			if ( !IntersectRect( &clippedRect, &clippedDstRect, &dstOperation) ) {
	
				return /* NOP */;
	
			}
	
			// Setup the general loop variables
			// ----------------------------------------------------------------
	
			int cx = clippedRect.right - clippedRect.left;
			int cy = clippedRect.bottom - clippedRect.top;
	
			int sx = ((clippedRect.left - x) + clippedSrcRect.left);
			int sy = ((clippedRect.top - y) + clippedSrcRect.top);
	
			// Check for flipping and adjust the dest position and step amount.
			// ----------------------------------------------------------------
	
			int dx, dy, ddx, ddy;
	
			if ( blitFX ) {
	
				if ( BLITFX::HFLIP & blitFX->dwFlags ) {
	
					sx = (clippedSrcRect.right - (sx + cx));
					dx = (clippedRect.right - 1);
					ddx = -1;
	
				} else {
	
					dx = clippedRect.left;
					ddx = 1;
	
				}
	
				if ( BLITFX::VFLIP & blitFX->dwFlags ) {
	
					sy = (clippedSrcRect.bottom - (sy + cy));
					dy = (clippedRect.bottom - 1);
					ddy = -1;
	
				} else {
	
					dy = clippedRect.top;
					ddy = 1;
	
				}
	
			} else {
	
				dx = clippedRect.left;
				ddx = 1;
	
				dy = clippedRect.top;
				ddy = 1;
	
			}
	
			// Process the non clipped spans
			// ----------------------------------------------------------------

			if ( 1 == ddx ) {
	
				for ( int ly = 0; ly < cy; ly++ ) {
	
					typename DST_SURFACE::pixel_iterator dstIT = dstSurface.Iterator( dx, dy );

					// call the decompressor

					typename srl_type::INFO * pInfo = &m_CompressedInfo[ sy ];

					decompressor(
						dstIT, 
						pInfo->first.begin(), pInfo->second.begin(),
						sx, cx, op
					);

					// advance the locations

					dy += ddy;
					++sy;
		
				}
	
			} else {
	
				for ( int ly = 0; ly < cy; ly++ ) {
	
					typename DST_SURFACE::reverse_iterator dstIT = dstSurface.rIterator( dx, dy );

					// call the decompressor

					typename srl_type::INFO * pInfo = &m_CompressedInfo[ sy ];

					decompressor(
						dstIT, 
						pInfo->first.begin(), pInfo->second.begin(),
						sx, cx, op
					);

					// advance the locations
	
					dy += ddy;
					++sy;
		
				}
	
			}
	
		}

		// --------------------------------------------------------------------

		template< class DST_SURFACE, class TOP >
		void __forceinline Blit(
			DST_SURFACE & dstSurface
			,const int x
			,const int y
			,TOP op = TOP()
			,const BLITFX * blitFX = 0
			,const RECT * optionalDstClipRect = 0
			,const RECT * optionalSrcSubRect = 0
		) {

			BlitCore(
				m_Decompressor, dstSurface, x, y, op, blitFX, 
				optionalDstClipRect, optionalSrcSubRect
			);

		}

		template< class DST_SURFACE, class TOP >
		void __forceinline Blit(
			DST_SURFACE* pDstSurface
			, const int x
			, const int y
			, TOP op = TOP()
			, const BLITFX* blitFX = 0
			, const RECT* optionalDstClipRect = 0
			, const RECT* optionalSrcSubRect = 0
		) {

			Blit(
				*pDstSurface, x, y, op, blitFX,
				optionalDstClipRect, optionalSrcSubRect
			);

		}

		// --------------------------------------------------------------------

		bool HitTest( const int x, const int y, storage_type * pOptionalOutValue = 0 ) {

			// clip
			// ----------------------------------------------------------------

			if ( (0 > x) || (0 > y) || (x >= m_Size.cx) || (y >= m_Size.cy) ) {

				return false;

			}

			// Handle the actual hit test
			// ----------------------------------------------------------------

			typename srl_type::INFO * pInfo = &m_CompressedInfo[ y ];

			return m_Decompressor.HitTest( 
				pInfo->first.begin(), pInfo->second.begin(), x,
				pOptionalOutValue
			);

		}

		// --------------------------------------------------------------------

		bool SameAs( const this_type * pOther ) const {

			// do the simple tests
			// ----------------------------------------------------------------

			if ( (m_Size.cx != pOther->m_Size.cx) || (m_Size.cy != pOther->m_Size.cy) ) {

				return false;

			}

			// Use the estimated compressed size as a checksum like value
			// and return that there isn't a match if the estimated sizes
			// are different. 
			// ----------------------------------------------------------------

			if ( pOther->CompressedSizeEstimate() && CompressedSizeEstimate() ) {
				
				if ( pOther->CompressedSizeEstimate() != CompressedSizeEstimate() ) {

					return false;

				}

			}

			// Now start comparing stored data
			// ----------------------------------------------------------------

			for ( int y = 0; y < m_Size.cy; y++ ) {

				typename srl_type::INFO * pInfoA = &m_CompressedInfo[ y ];
				typename srl_type::INFO * pInfoB = &pOther->m_CompressedInfo[ y ];

				// Check to see if the collections are the same size if not
				// there is no match ( early bail out gotta love that :)
				// ------------------------------------------------------------

				if ( (pInfoA->first.size() != pInfoB->first.size()) || 
					(pInfoA->second.size() != pInfoB->second.size()) ) {

					return false;

				}

				// compare the codes/control for both collections
				// ------------------------------------------------------------

				typename srl_type::INFO::first_type::iterator codesAIT = pInfoA->first.begin();

				typename srl_type::INFO::first_type::iterator codesBIT = pInfoB->first.begin();

				while ( codesAIT != pInfoA->first.end() ) {

					// --------------------------------------------------------

					if ( *codesAIT++ != *codesBIT++ ) {

						return false;

					}

				}

				// compare the data for both data collections
				// ------------------------------------------------------------

				typename srl_type::INFO::second_type::iterator dataAIT = pInfoA->second.begin();

				typename srl_type::INFO::second_type::iterator dataBIT = pInfoB->second.begin();

				while ( dataAIT != pInfoA->second.end() ) {

					// --------------------------------------------------------

					if ( *dataAIT++ != *dataBIT++ ) {

						return false;

					}

				}

			}

			// If we've gotten here then it must be the same
			// ----------------------------------------------------------------

			return true;

		}

		// --------------------------------------------------------------------

		//
		//	CreatePackedSurface()
		//

		template<typename S>
		bool CreatePackedSurface(S& surface,bool bCreateWideSurface = false)
		{
			do {

				if (!HasData()) break;

				// count the values

				size_t total = 0;

				for (int y = 0; y < this->m_Size.cy; y++) {

					typename srl_type::INFO* pInfo = &m_CompressedInfo[y];

					total += pInfo->second.size();

				}

				size_t sq = (size_t)std::floor(std::sqrt(total) + 0.5) + 1; // just make sure we have more than enough space (wasteful and kind of silly)

				size_t sqTotal = sq * sq;

				if (!surface.Create(sq, sq))
				{
					ASSERT(false);
					break;
				}

				if (sqTotal > total)
				{
					// this clears the entire surface when there is probably only a few pixels (wasteful)
					surface.ClearBuffer(0);
				}

				// making an assumption about the surface memory layout (need a 2D iterator for surfaces...)

				auto pWriter = surface.Iterator(0, 0);

				size_t wrote = 0;

				for (int y = 0; y < this->m_Size.cy; y++) {

					typename srl_type::INFO* pInfo = &m_CompressedInfo[y];

					std::copy(pInfo->second.begin(), pInfo->second.end(), pWriter);

					int count = pInfo->second.size();

					pWriter += count;
					wrote += count;

				}

				ASSERT(wrote == total);

				return true;

			} while (false);

			// fall back to creating a 1x1 black surface 

			if (surface.Create(1, 1))
			{
				surface.ClearBuffer(0);
				return true;
			}

			return false;
		}


		// --------------------------------------------------------------------

#if 0

		// TODO :-)

		bool SaveTo( class CShowcaseFileIO * io );
		bool LoadFrom( class CShowcaseFileIO * io );

#endif

	}; // class

	// --------------------------------------------------------------------------

	//
	//
	//

	//template<typename T>


}; // namespace BPT

#endif // !defined(AFX_BPTSRL_H__CCC1EB59_863C_4490_84CA_C1F171D48D87__INCLUDED_)
