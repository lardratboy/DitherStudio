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


#if !defined(AFX_T14COMPRESSION_H__611BA8B6_4CE9_44AB_8918_54EF4C785401__INCLUDED_)
#define AFX_T14COMPRESSION_H__611BA8B6_4CE9_44AB_8918_54EF4C785401__INCLUDED_

// ----------------------------------------------------------------------------

#include <list>
#include <vector>
#include <utility>
#include "Pixels.h"
#include "Rectangle.h"
#include "TBlitFramework.h"
#include "MMX.h"

// ----------------------------------------------------------------------------

extern "C" void __cdecl MMX_T14_Copy_16bpp(
	void * dstPtr, const int adjustedPitch, const void * compressedStream,
	const int skipCOunt, const int writeCount, const int height
	,const int extraParam
);

extern "C" void __cdecl T14_Copy_16bpp(
	void * dstPtr, const int adjustedPitch, const void * compressedStream,
	const int skipCOunt, const int writeCount, const int height
	,const int extraParam
);

// (5050-555) -----------------------------------------------------------------

extern "C" void __cdecl MMX_T14_5050_R5G5B5(
	void * dstPtr, const int adjustedPitch, const void * compressedStream,
	const int skipCOunt, const int writeCount, const int height
	,const int extraParam
);

extern "C" void __cdecl T14_5050_R5G5B5(
	void * dstPtr, const int adjustedPitch, const void * compressedStream,
	const int skipCOunt, const int writeCount, const int height
	,const int extraParam
);

extern "C" void __cdecl MMX_T14_PreMultiplied5050_R5G5B5(
	void * dstPtr, const int adjustedPitch, const void * compressedStream,
	const int skipCOunt, const int writeCount, const int height
	,const int extraParam
);

extern "C" void __cdecl T14_PreMultiplied5050_R5G5B5(
	void * dstPtr, const int adjustedPitch, const void * compressedStream,
	const int skipCOunt, const int writeCount, const int height
	,const int extraParam
);

// (5050-565) -----------------------------------------------------------------

extern "C" void __cdecl MMX_T14_5050_R5G6B5(
	void * dstPtr, const int adjustedPitch, const void * compressedStream,
	const int skipCOunt, const int writeCount, const int height
	,const int extraParam
);

extern "C" void __cdecl T14_5050_R5G6B5(
	void * dstPtr, const int adjustedPitch, const void * compressedStream,
	const int skipCOunt, const int writeCount, const int height
	,const int extraParam
);

extern "C" void __cdecl MMX_T14_PreMultiplied5050_R5G6B5(
	void * dstPtr, const int adjustedPitch, const void * compressedStream,
	const int skipCOunt, const int writeCount, const int height
	,const int extraParam
);

extern "C" void __cdecl T14_PreMultiplied5050_R5G6B5(
	void * dstPtr, const int adjustedPitch, const void * compressedStream,
	const int skipCOunt, const int writeCount, const int height
	,const int extraParam
);

// (ALPHA) --------------------------------------------------------------------

extern "C" void __cdecl MMX_T14_ExternalAlpha_R5G5B5(
	void * dstPtr, const int adjustedPitch, const void * compressedStream,
	const int skipCOunt, const int writeCount, const int height,
	const int alpha
);

extern "C" void __cdecl MMX_T14_ExternalAlpha_R5G6B5(
	void * dstPtr, const int adjustedPitch, const void * compressedStream,
	const int skipCOunt, const int writeCount, const int height,
	const int alpha
);

// (ADDITIVE) -----------------------------------------------------------------

extern "C" void __cdecl MMX_T14_Additive_R5G5B5(
	void * dstPtr, const int adjustedPitch, const void * compressedStream,
	const int skipCOunt, const int writeCount, const int height
	,const int extraParam
);

extern "C" void __cdecl MMX_T14_Additive_R5G6B5(
	void * dstPtr, const int adjustedPitch, const void * compressedStream,
	const int skipCOunt, const int writeCount, const int height
	,const int extraParam
);

// (SUBTRACTIVE) --------------------------------------------------------------

extern "C" void __cdecl MMX_T14_Subtractive_R5G5B5(
	void * dstPtr, const int adjustedPitch, const void * compressedStream,
	const int skipCOunt, const int writeCount, const int height
	,const int extraParam
);

extern "C" void __cdecl MMX_T14_Subtractive_R5G6B5(
	void * dstPtr, const int adjustedPitch, const void * compressedStream,
	const int skipCOunt, const int writeCount, const int height
	,const int extraParam
);

// (DARKEN) -------------------------------------------------------------------

extern "C" void __cdecl MMX_T14_Darken2x_R5G5B5(
	void * dstPtr, const int adjustedPitch, const void * compressedStream,
	const int skipCOunt, const int writeCount, const int height
	,const int extraParam
);

extern "C" void __cdecl MMX_T14_Darken2x_R5G6B5(
	void * dstPtr, const int adjustedPitch, const void * compressedStream,
	const int skipCOunt, const int writeCount, const int height
	,const int extraParam
);

extern "C" void __cdecl T14_Darken2x_R5G5B5(
	void * dstPtr, const int adjustedPitch, const void * compressedStream,
	const int skipCOunt, const int writeCount, const int height
	,const int extraParam
);

extern "C" void __cdecl T14_Darken2x_R5G6B5(
	void * dstPtr, const int adjustedPitch, const void * compressedStream,
	const int skipCOunt, const int writeCount, const int height
	,const int extraParam
);

// (BRIGHTEN) -----------------------------------------------------------------

extern "C" void __cdecl MMX_T14_Brighten2x_R5G5B5(
	void * dstPtr, const int adjustedPitch, const void * compressedStream,
	const int skipCOunt, const int writeCount, const int height
	,const int extraParam
);

extern "C" void __cdecl MMX_T14_Brighten2x_R5G6B5(
	void * dstPtr, const int adjustedPitch, const void * compressedStream,
	const int skipCOunt, const int writeCount, const int height
	,const int extraParam
);

// (MULTIPLY) -----------------------------------------------------------------

extern "C" void __cdecl MMX_T14_Multiply0to2_R5G5B5(
	void * dstPtr, const int adjustedPitch, const void * compressedStream,
	const int skipCOunt, const int writeCount, const int height
	,const int extraParam
);

extern "C" void __cdecl MMX_T14_Multiply0to2_R5G6B5(
	void * dstPtr, const int adjustedPitch, const void * compressedStream,
	const int skipCOunt, const int writeCount, const int height
	,const int extraParam
);

// ----------------------------------------------------------------------------

namespace BPT {

	// ========================================================================
	// 
	// T14 control BYTE format
	// 
	// (control byte - 2) == 0 is a quad literal
	// (control byte - 2) < 0 is a single literal
	// (control byte - 2) != 0 & > 0 then it's a skip run.
	// 
	// The stream's format varies depending on type that was compressed.
	// 
	// ========================================================================

	// ========================================================================
	// Compress data into code & value streams
	// ========================================================================

	//
	//	TT14Results
	//

	template< typename T, typename C = U8 >
	class TT14Results {

	public:

		// trait's and other generic programming helpers
		// --------------------------------------------------------------------

		enum RUNTYPE {
	
			SKIP, STORE
	
		};

		typedef T value_type;
		typedef C CODE_ENTRY;
		typedef std::vector<T> VALUE_COLLECTION;
		typedef std::vector<CODE_ENTRY> CODE_COLLECTION;

		// Data
		// --------------------------------------------------------------------

		VALUE_COLLECTION singles;
		VALUE_COLLECTION quads;
		CODE_COLLECTION codes;

		// Methods
		// --------------------------------------------------------------------

		void Reserve( const int reserveCount ) {

			singles.reserve( singles.size() + reserveCount );
			quads.reserve( quads.size() + reserveCount );
			codes.reserve( codes.size() + reserveCount );

		}

		template< typename ForwardIt > void
		EmitCode( RUNTYPE type, const int count, ForwardIt runStartIt ) {

			if ( SKIP == type ) {

				for ( int countLeft = count; 0 < countLeft; ) {

					int len = min( countLeft, 253 );
					codes.push_back( len + 2 );
					countLeft -= len;

				}

			} else {

				for ( int countLeft = count; 0 < countLeft; ) {

					if ( 4 <= countLeft ) {

						countLeft -= 4;

						codes.push_back( 2 );

						for ( int counter = 0; counter < 4; counter++ ) {

							quads.push_back( *runStartIt++ );

						}

					} else {

						singles.push_back( *runStartIt++ );
						codes.push_back( 1 );
						--countLeft;

					}

				}

			}
	
		}

	};

	//
	//	TT14Compressor
	//
	//	-- This class could be made more general
	//
	
	class TT14Compressor {

	public:

		template< typename INFO, typename ForwardIt, typename Pred >
		bool operator()( INFO & info, ForwardIt first, ForwardIt last, Pred shouldStore ) {

			// Try to improve the efficiency of this routine by reserving
			// additional space in the collections for the new info.
			// ----------------------------------------------------------------

			info.Reserve( last - first );

			// Figure out what type of code to start with skip or store
			// ----------------------------------------------------------------
	
			INFO::RUNTYPE type;

			ForwardIt runStartIt;
			ForwardIt it = first;

			bool value = shouldStore( *it++ );

			int count = 1;

			if ( value ) {

				runStartIt = (it - 1);
				type = INFO::STORE;

			} else {

				runStartIt = last;
				type = INFO::SKIP;

			}
	
			// Run through the remaining values collecting runs
			// ----------------------------------------------------------------

			while ( it != last ) {

				bool lastValue = value;

				value = shouldStore( *it++ );

				if ( value == lastValue ) {

					++count;

				} else {

					// Dump the last code and start a new one
					// --------------------------------------------------------

					info.EmitCode( type, count, runStartIt );

					// Start a new run 
					// --------------------------------------------------------

					if ( value ) {

						runStartIt = (it - 1);
						type = INFO::STORE;
						count = 1;

					} else {

						runStartIt = last;
						type = INFO::SKIP;
						count = 1;

					}

				}
	
			}

			// Dump the last code to the code collection

			info.EmitCode( type, count, runStartIt );

			return true;
	
		}
	
	};

	// ========================================================================
	// Decode
	// ========================================================================

	//
	//	TDecodeT14<>
	//
	//	This code support 'clipping' both at the begining of the data and at
	//	the end of it.  This comes in handy for doing partial decompression of
	//	the data.  
	//

	class TDecodeT14 {

	private:

	public:

		TDecodeT14() { /* Empty */ }

		template<
			typename DST_IT,
			typename CODE_IT,
			typename VALUE_IT,
			typename TOP
		>
		__forceinline void operator()(
			DST_IT dstIT, CODE_IT codes, VALUE_IT singles, VALUE_IT quads,
			int skipCount, int writeCount, TOP top ) {

			int code;

			// Handle any skipping
			// ----------------------------------------------------------------

			while ( 0 < skipCount ) {

				code = (*codes++) - 2;

				if ( 0 == code ) { // quad

					skipCount -= 4;

					if ( 0 > skipCount ) {

						quads += (4 + skipCount);

						writeCount += skipCount;

						if ( 0 > writeCount ) {

							if ( 2 == (writeCount - skipCount) ) goto WRITE_2;
							goto WRITE_1;

						}

						if ( -3 == skipCount ) goto WRITE_3;
						if ( -2 == skipCount ) goto WRITE_2;

						goto WRITE_1;

					}

					quads += 4;

				} else if ( 0 > code ) { // single

					--skipCount;
					++singles;

				} else { // skip

					skipCount -= code;

					if ( 0 > skipCount ) {

						writeCount += skipCount;

						if ( 0 >= writeCount ) {

							return /* done */;

						}

						dstIT -= skipCount;

					}

				}

			}

			// Finally get down
			// ----------------------------------------------------------------

			while ( 0 < writeCount ) {

				code = (*codes++) - 2;

				if ( 0 == code ) { // quad

					writeCount -= 4;

					if ( 0 > writeCount ) {

						if ( -1 == writeCount ) goto WRITE_3;
						if ( -2 == writeCount ) goto WRITE_2;

						goto WRITE_1;

						/* WRITE_4: */ top( *dstIT++, *quads++ );
						WRITE_3: top( *dstIT++, *quads++ );
						WRITE_2: top( *dstIT++, *quads++ );
						WRITE_1: top( *dstIT++, *quads++ );
	
					} else {

						top.InputMajor( quads, quads + 4, dstIT );

						quads += 4;
						dstIT += 4;

					}

				} else if ( 0 > code ) { // single

					--writeCount;

					top( *dstIT++, *singles++ );

				} else { // skip

					writeCount -= code;

					if ( 0 > writeCount ) {

						return /* done */;

					}

					dstIT += code;

				}

			}

		}

	};

	// ========================================================================
	//	Compressed bitmap template
	// ========================================================================

	// ========================================================================
	//	Compressed bitmap sub rect 
	// ========================================================================

	// ========================================================================
	//	Compressed bitmap sub rect 
	// ========================================================================

	//
	//	T_T14SurfaceCompressor
	//
	//	-- This really should be more general!!!!!
	//	-- This really should be more general!!!!!
	//	-- This really should be more general!!!!!
	//

	template< typename OUTPUT, typename SURFACE, typename PREDICATE, typename INFO >
	bool T_T14SurfaceCompressor(
		OUTPUT & output
		,SURFACE & srcSurface
		,PREDICATE predicate = PREDICATE()
		,const SRect * optionalSrcSubRect = 0
	) {

		TT14Compressor compressor;

		// Get the source operation size
		// --------------------------------------------------------------------

		SRect srcLimitsRect( srcSurface.Rect() );

		SRect clippedSrcRect;

		if ( optionalSrcSubRect ) {

			if ( !clippedSrcRect.Intersection( &srcLimitsRect, optionalSrcSubRect ) ) {

				return false;

			}

		} else {

			clippedSrcRect = srcLimitsRect;

		}

		// Setup the general loop variables
		// --------------------------------------------------------------------

		const int cx = clippedSrcRect.Width();
		const int cy = clippedSrcRect.Height();

		int sx = clippedSrcRect.left;
		int sy = clippedSrcRect.top;

		// Compress each scanline seperately
		// --------------------------------------------------------------------

		typename OUTPUT::collection_type & outputCollection = output.GetCollection();

		for ( int ly = 0; ly < cy; ly++ ) {

			INFO * scanline = new INFO;

			if ( !scanline ) {

				return false;

			}

			SURFACE::pixel_iterator srcIT = srcSurface.Iterator( sx, sy++ );

			if ( !compressor( *scanline, srcIT, srcIT + cx, predicate ) ) {

				return false;

			}

			outputCollection.push_back( scanline );

		}

		return true;

	}

	// ========================================================================

	//
	//	TCompressedSurfaceHack
	//

	template< typename DATA > class TCompressedSurfaceHack {

	private:

		TCompressedSurfaceHack(); // Hidden

		DATA * m_Data;
		SIZE m_Size;

	public:

		typedef typename DATA::collection_type::iterator scanline_iterator;

		TCompressedSurfaceHack( DATA * data, const SIZE & size ) :
			m_Data( data ), m_Size( size ) { /* Empty */ }

		int Width() const {

			return m_Size.cx;

		}

		int Height() const {

			return m_Size.cy;

		}

		SIZE Size() const {

			return m_Size;

		}

		SRect Rect() const {

			return SRect( Size() );

		}

		scanline_iterator ScanlineIterator() {

			return m_Data->GetCollection().begin();

		}

	};

	// ========================================================================
	// This code should probably be in a file all its own!
	// ========================================================================

	//
	//	T_T14Decompressor()
	//

	template< typename DST_SURFACE, typename X_SURFACE, typename TOP >
	void T_T14Decompressor(
		DST_SURFACE & dstSurface
		,const int x
		,const int y
		,X_SURFACE & srcSurface
		,TOP op = TOP()
		,const BLITFX * blitFX = 0
		,const SRect * optionalDstClipRect = 0
		,const SRect * optionalSrcSubRect = 0
	) {

		TDecodeT14 decoder;

// ============================================================================
// EVERYTHING BELOW HERE COULD PROBABLY BE REPRESENTED BY A FUNCTION/CLASS!
// ============================================================================

		// Clip the optional clipping rect to the dest bitmap limits
		// --------------------------------------------------------------------

		SRect dstLimitsRect( dstSurface.Rect() );

		SRect clippedDstRect;

		if ( optionalDstClipRect ) {

			if ( !clippedDstRect.Intersection( &dstLimitsRect, optionalDstClipRect ) ) {

				return /* NOP */;

			}

		} else {

			clippedDstRect = dstLimitsRect;

		}

		// Get the source operation size
		// --------------------------------------------------------------------

		SRect srcLimitsRect( srcSurface.Rect() );

		SRect clippedSrcRect;

		if ( optionalSrcSubRect ) {

			if ( !clippedSrcRect.Intersection( &srcLimitsRect, optionalSrcSubRect ) ) {

				return /* NOP */;

			}

		} else {

			clippedSrcRect = srcLimitsRect;

		}

		// Perform a simple clipping operation to detect NOP
		// --------------------------------------------------------------------

		SRect dstOperation( x, y, clippedSrcRect.Size() );

		SRect clippedRect;
			
		if ( !clippedRect.Intersection( &clippedDstRect, &dstOperation ) ) {

			return /* NOP */;

		}

// ============================================================================
// EVERYTHING ABOVE HERE COULD PROBABLY BE REPRESENTED BY A FUNCTION/CLASS!
// ============================================================================

		// Setup the general loop variables
		// --------------------------------------------------------------------

		const int cx = clippedRect.right - clippedRect.left;
		const int cy = clippedRect.bottom - clippedRect.top;

		int sx = ((clippedRect.left - x) + clippedSrcRect.left);
		int sy = ((clippedRect.top - y) + clippedSrcRect.top);

		// Check for flipping and adjust the dest position and step amount.
		// --------------------------------------------------------------------

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

		// Skip any vertical lines
		// --------------------------------------------------------------------

		X_SURFACE::scanline_iterator scanlineIT = srcSurface.ScanlineIterator();

		for ( int skip = 0; skip < sy; skip++ ) {

			++scanlineIT;

		}

		// Do each vertical line in the clipped operation rectangle
		// The general ddx & ddy could be removed and the code might see a
		// slight improvement in speed.
		// --------------------------------------------------------------------

		if ( 1 == ddx ) {

			for ( int ly = 0; ly < cy; ly++ ) {

				DST_SURFACE::pixel_iterator dstIT = dstSurface.Iterator( dx, dy );

				decoder(
					dstIT,
					(*scanlineIT)->codes.begin(),
					(*scanlineIT)->singles.begin(),
					(*scanlineIT)->quads.begin(),
					sx, cx, op
				);
		
				dy += ddy;

				++scanlineIT;
	
			}

		} else {

			for ( int ly = 0; ly < cy; ly++ ) {

				DST_SURFACE::reverse_iterator dstIT = dstSurface.rIterator( dx, dy );
	
				decoder(
					dstIT,
					(*scanlineIT)->codes.begin(),
					(*scanlineIT)->singles.begin(),
					(*scanlineIT)->quads.begin(),
					sx, cx, op
				);
		
				dy += ddy;

				++scanlineIT;
			}

		}

	}

	// ========================================================================
	//	Stream encoder
	// ========================================================================

	//
	//	TConvertT14InfoToCompressedStream
	//

	template<
		typename STREAM = typename TCompressedStreamSurface<>,
		const int SINGLE_ALIGNMENT = 2,
		const int QUAD_ALIGNMENT = 8
	>
	class TConvertT14InfoToCompressedStream {

	public:

		TConvertT14InfoToCompressedStream() { /* Empty */ }

		typedef STREAM stream_type;

		template< typename T > stream_type *
		Convert( T & compressedInfo, const SIZE & size ) {

			typedef T::compressed_type P;

			// Start by calulating the overall size needed for the stream buffer
			// --------------------------------------------------------------------
	
			int headerSize = 3 * sizeof( STREAM::size_type );

			int streamDataSize = 0;
	
			T::collection_type::iterator it = compressedInfo.GetCollection().begin();
	
			while ( it != compressedInfo.GetCollection().end() ) {
	
				int codesSize = (*it)->codes.size();
	
				int singlesOffset = headerSize + codesSize;
	
				if ( singlesOffset % SINGLE_ALIGNMENT ) {
	
					singlesOffset += (SINGLE_ALIGNMENT - (singlesOffset % SINGLE_ALIGNMENT));
	
				}
	
				int singlesSize = ((*it)->singles.size() * sizeof(P));
				int quadsOffset = singlesOffset + singlesSize;
	
				if ( quadsOffset % QUAD_ALIGNMENT ) {
	
					quadsOffset += (QUAD_ALIGNMENT - (quadsOffset % QUAD_ALIGNMENT));
	
				}
	
				int quadsSize = ((*it)->quads.size() * sizeof(P));
				streamDataSize += (quadsOffset + quadsSize);
	
				++it;
	
			}
	
			stream_type::data_type * streamDataPtr = new stream_type::data_type [ streamDataSize ];
	
			if ( streamDataPtr ) {
	
				// Build the data
				// -----------------------------------------------------------------
	
				BYTE * workPtr = reinterpret_cast<BYTE *>( streamDataPtr );
		
				it = compressedInfo.GetCollection().begin();
		
				while ( it != compressedInfo.GetCollection().end() ) {
	
					BYTE * writePtr = workPtr;
	
					// Build the various offsets
					// --------------------------------------------------------------
		
					int codeCount = (*it)->codes.size();
					int codesSize = codeCount;
	
					int singlesOffset = headerSize + codesSize;
	
					if ( singlesOffset % SINGLE_ALIGNMENT ) {
		
						singlesOffset += (SINGLE_ALIGNMENT - (singlesOffset % SINGLE_ALIGNMENT));
		
					}
		
					int singlesCount = (*it)->singles.size();
					int singlesSize = (singlesCount * sizeof(P));
					int quadsOffset = singlesOffset + singlesSize;
		
					if ( quadsOffset % QUAD_ALIGNMENT ) {
		
						quadsOffset += (QUAD_ALIGNMENT - (quadsOffset % QUAD_ALIGNMENT));
		
					}
		
					int quadsCount = (*it)->quads.size();
					int quadsSize = (quadsCount * sizeof(P));
	
					int lineSize = (quadsOffset + quadsSize);
	
					workPtr += lineSize;
	
					// Write the data at the offset's
					// --------------------------------------------------------------
	
					*(STREAM::size_type *)( writePtr + (sizeof(STREAM::size_type) * 0) ) = lineSize;
					*(STREAM::size_type *)( writePtr + (sizeof(STREAM::size_type) * 1) ) = singlesOffset;
					*(STREAM::size_type *)( writePtr + (sizeof(STREAM::size_type) * 2) ) = quadsOffset;
	
					// Write the codes
					// --------------------------------------------------------------
	
					BYTE * codesOutPtr = writePtr + headerSize;

					T::collection_value_type::CODE_COLLECTION::iterator codesIt = (*it)->codes.begin();
	
					for ( int c = 0; c < codeCount; c++ ) {
	
						*codesOutPtr++ = *codesIt++;
	
					}
	
					// Write the singles
					// --------------------------------------------------------------
	
					P * singlesOutput = reinterpret_cast<P*>(writePtr + singlesOffset);
	
					T::collection_value_type::VALUE_COLLECTION::iterator singlesIt = (*it)->singles.begin();
	
					for ( int s = 0; s < singlesCount; s++ ) {
	
						*singlesOutput++ = *singlesIt++;
	
					}
	
					// Write the quads
					// --------------------------------------------------------------
	
					P * quadsOutput = reinterpret_cast<P*>(writePtr + quadsOffset);
	
					T::collection_value_type::VALUE_COLLECTION::iterator quadsIt = (*it)->quads.begin();
	
					for ( int q = 0; q < quadsCount; q++ ) {
	
						*quadsOutput++ = *quadsIt++;
	
					}
	
					++it;
	
				}
	
				// Create the compressed stream object
				// -----------------------------------------------------------------
	
				stream_type * pCompressedStream = new stream_type();

				if ( pCompressedStream ) {

					if ( pCompressedStream->AttachData(
						streamDataPtr, size, streamDataSize, 
						stream_type::MEMORY_OPTION::OWN
					) ) {

						return pCompressedStream;

					}

					delete [] streamDataPtr;

				}

				delete streamDataPtr;

			}

			return 0;
	
		}

	};

	// ========================================================================
	//	All in one MMX decompressor
	// ========================================================================

	//
	//	TCompressedStreamSurface
	//
	//	THIS NEEDS MORE DESIGN!!!
	//

	template< typename T = U8, typename S = U16 > class TCompressedStreamSurface {

	public:

		enum MEMORY_OPTION { UNKNOWN, OWN, SHARE, COPY };

	private:

		T * m_Data;
		SIZE m_Size;
		int m_DataCount;
		MEMORY_OPTION m_MemoryOption;

		void Release() {

			if ( (OWN == m_MemoryOption) ) {

				m_MemoryOption = UNKNOWN;
				delete [] m_Data;

			}

			m_DataCount = 0;
			m_Data = 0;

		}

	public:

		typedef T data_type;
		typedef S size_type;

		TCompressedStreamSurface() : m_Data(0), m_DataCount(0), m_MemoryOption( UNKNOWN ) {

			m_Size.cx = 0;
			m_Size.cy = 0;

		}

		~TCompressedStreamSurface() {

			Release();

		}

		bool AttachData(
			T * data, const SIZE & size, const int dataCount,
			MEMORY_OPTION memoryOption = SHARE ) {

			// Release any existing data
			// ----------------------------------------------------------------

			Release();

			// Deal with the memory option
			// ----------------------------------------------------------------

			if ( (SHARE == memoryOption) || (OWN == memoryOption) ) {

				m_MemoryOption = memoryOption;
				m_Data = data;

			} else {

				m_MemoryOption = OWN;

				T * memory = new T [ dataCount ];

				if ( 0 == memory ) {

					return false;

				}

				std::copy( data, data + dataCount, memory );

			}

			// finalize the object
			// ----------------------------------------------------------------

			m_DataCount =  dataCount;
			m_Size = size;

			return true;

		}

		// (simple query methods) ---------------------------------------------

		int Width() const {

			return m_Size.cx;

		}

		int Height() const {

			return m_Size.cy;

		}

		SIZE Size() const {

			return m_Size;

		}

		SRect Rect() const {

			return SRect( Size() );

		}

		S * StreamData() {

			return reinterpret_cast<S *>( m_Data );

		}

		// (lowest level) -----------------------------------------------------

		void * RawData() const {

			return m_Data;

		}

		int DataSize() const {

			return sizeof(T) * m_DataCount;

		}

	};

	/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

		Compressed stream format description

		for each line

		S == size
		SO == singles offset from line address
		QO == quads offset from line address
		
		the singles data is aligned to word boundries
		the quads data is aligned to quad word boundries

		[ S:word ][ SO:word ][ QO:word ]<code(s)>[pad]<single(s)>[pad]<quad(s)>
		[ S:word ][ SO:word ][ QO:word ]<code(s)>[pad]<single(s)>[pad]<quad(s)>
		[ S:word ][ SO:word ][ QO:word ]<code(s)>[pad]<single(s)>[pad]<quad(s)>
		[ S:word ][ SO:word ][ QO:word ]<code(s)>[pad]<single(s)>[pad]<quad(s)>

	*/

	//
	//	T_T14ForwardBlitterPrim<>
	//

	class T_T14ForwardBlitterPrim {

	public:

		__forceinline void operator()(
			WORD * dstPtr, const int pitch, const void * compressedStream, 
			const int skipCount, const int writeCount, const int height
		) {
	
			// Prep work
			// ----------------------------------------------------------------
	
			void * lastESI = 0;
			int adjustedPitch = pitch - (writeCount * sizeof(WORD));
			int lineCounter = height;

#if defined(BPTLIB_MMX_CODE)
			if ( !CMMX::Available() ) {
#endif
	
				T14_Copy_16bpp(
					(void *)dstPtr, adjustedPitch, compressedStream, 
					skipCount, writeCount, height, 0
				);

#if defined(BPTLIB_MMX_CODE)

			} else {

				MMX_T14_Copy_16bpp(
					(void *)dstPtr, adjustedPitch, compressedStream, 
					skipCount, writeCount, height, 0
				);

			}

#endif

		}

	};

	//
	//	T_XStreamDecompressorShell()
	//
	//	(NEED TO DESCRIBE A STREAM)
	//

	template< typename DST_SURFACE, typename STREAM_SURFACE, typename DECOMPRESSOR >
	void T_XStreamDecompressorShell(
		DST_SURFACE & dstSurface
		,const int x
		,const int y
		,STREAM_SURFACE & srcSurface
		,DECOMPRESSOR & decompressor
		,const SRect * optionalDstClipRect = 0
		,const SRect * optionalSrcSubRect = 0
	) {

// ============================================================================
// EVERYTHING BELOW HERE COULD PROBABLY BE REPRESENTED BY A FUNCTION/CLASS!
// ============================================================================

		// Clip the optional clipping rect to the dest bitmap limits
		// --------------------------------------------------------------------

		SRect dstLimitsRect( dstSurface.Rect() );

		SRect clippedDstRect;

		if ( optionalDstClipRect ) {

			if ( !clippedDstRect.Intersection( &dstLimitsRect, optionalDstClipRect ) ) {

				return /* NOP */;

			}

		} else {

			clippedDstRect = dstLimitsRect;

		}

		// Get the source operation size
		// --------------------------------------------------------------------

		SRect srcLimitsRect( srcSurface.Rect() );

		SRect clippedSrcRect;

		if ( optionalSrcSubRect ) {

			if ( !clippedSrcRect.Intersection( &srcLimitsRect, optionalSrcSubRect ) ) {

				return /* NOP */;

			}

		} else {

			clippedSrcRect = srcLimitsRect;

		}

		// Perform a simple clipping operation to detect NOP
		// --------------------------------------------------------------------

		SRect dstOperation( x, y, clippedSrcRect.Size() );

		SRect clippedRect;
			
		if ( !clippedRect.Intersection( &clippedDstRect, &dstOperation ) ) {

			return /* NOP */;

		}

// ============================================================================
// EVERYTHING ABOVE HERE COULD PROBABLY BE REPRESENTED BY A FUNCTION/CLASS!
// ============================================================================

		// Setup the general loop variables
		// --------------------------------------------------------------------

		int cx = clippedRect.right - clippedRect.left;
		int cy = clippedRect.bottom - clippedRect.top;

		int sx = ((clippedRect.left - x) + clippedSrcRect.left);
		int sy = ((clippedRect.top - y) + clippedSrcRect.top);

		// Skip any vertical lines
		// --------------------------------------------------------------------

		const WORD * streamPtr = srcSurface.StreamData();

		for ( int skip = 0; skip < sy; skip++ ) {

			int size = *streamPtr;

			streamPtr = reinterpret_cast<const WORD *>(
				reinterpret_cast<const BYTE *>( streamPtr ) + size
			);

		}

		// Dispatch to the core decompressor
		// --------------------------------------------------------------------

// THE CAST IS TO GET THE VC COMPILER TO WORK!!!!

		decompressor(
			(WORD *)dstSurface.Iterator( clippedRect.left, clippedRect.top ),
			dstSurface.Pitch(),
			reinterpret_cast<const void *>( streamPtr ),
			sx, cx, cy
		);

	}

}; // namespace BPT

#endif // !defined(AFX_T14COMPRESSION_H__611BA8B6_4CE9_44AB_8918_54EF4C785401__INCLUDED_)
