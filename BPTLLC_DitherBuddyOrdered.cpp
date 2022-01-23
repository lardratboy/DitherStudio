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

#include "stdafx.h"
//#include <time.h>
#include <math.h>

#include "DitherBuddy.h"
#include "BPTLLC_DitherBuddyOrdered.h"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

namespace BPT {

	// ========================================================================

	//
	//	TCachedOrderedDitherer<>
	//

	template< class T = BYTE >
	class TCachedOrderedDitherer {

	public:

		typedef T cache_value_type;
		typedef T * dither_line_cache;

	private:

		T * m_pDitherCache;
		int m_nModMask;
		int m_nPow2;
		int m_nCacheLineSize;
		int m_nModValue;

		// --------------------------------------------------------------------

	private:

		int GenerateDitherTableElement( int x, int y, int size ) {

			int d = 0;

			while ( size-- > 0 ) {

				d = (d << 1 | (x&1 ^ y&1)) << 1 | y&1;

				x >>= 1;
				y >>= 1;

			}

			return d;

		}

		// --------------------------------------------------------------------

		void Release() {

			if ( m_pDitherCache ) {

				delete [] m_pDitherCache;

				m_pDitherCache = 0;

			}

		}

	public:

		// --------------------------------------------------------------------

		TCachedOrderedDitherer() : 
			m_pDitherCache( 0 ), 
			m_nModMask( 0 ), 
			m_nPow2( 0 ),
			m_nModValue( 0 ) {

			// Empty

		}

		~TCachedOrderedDitherer() {

			Release();

		}

		// --------------------------------------------------------------------

		bool Init(
			const int nPow2, 
			const int inputRange,
			const int outputRange,
			const int outputUpShift,
			const float brightenCoeff = 0.0625f
		) {

			// allocate the new dither table
			// ----------------------------------------------------------------

			Release();

			m_nPow2 = nPow2;

			m_nModValue = (1 << m_nPow2);

			m_nModMask = m_nModValue - 1;

			int entries = m_nModValue * m_nModValue;

			int * pDitherTable = new int [ entries ];

			if ( !pDitherTable ) {

				return false;

			}

			// Allocate the dither cache
			// ----------------------------------------------------------------

			m_nCacheLineSize = (m_nModValue * inputRange);

			m_pDitherCache = new T [ m_nModValue * m_nCacheLineSize ];

			if ( !m_pDitherCache ) {

				delete [] pDitherTable;

				return false;

			}

			// Build the new dither table values
			// ----------------------------------------------------------------

			{
				int i = 0;

				//BPT::TRACE( "Dither[ %d ][ %d ]\n", m_nModValue, m_nModValue ); 

				int s = 1; //max( 1, (inputRange / entries));

				for ( int y = 0; y < m_nModValue; y++ ) {
	
					for ( int x = 0; x < m_nModValue; x++ ) {
	
						pDitherTable[ i++ ] = (s * GenerateDitherTableElement(
							x, y, nPow2
						));

						//BPT::TRACE( ",%4d ", pDitherTable[ i - 1 ] );
	
					}

					//BPT::TRACE( "\n" );
	
				}

			}

			// Okay build the cache elements
			// ----------------------------------------------------------------

			{
				int i = 0;

				float quantCoeff = 1.0f / ((float)inputRange / (float)(outputRange - 1));

				// Build the adjusted / scaled value table.
				// ------------------------------------------------------------

				float adjustedValues[ 256 ];

				float iBrightenCoeff = (1.0f - brightenCoeff);

				for ( int a = 0; a < inputRange; a++ ) {

					float fm = (float)(inputRange - 1);
					float fx = (float)a;
					float th = (fx * 1.57079633f ) / fm;
					float termA = (sinf( th ) * fm) * brightenCoeff;
					float termB = (float)a * iBrightenCoeff;

					adjustedValues[ a ] = (termA + termB) * quantCoeff;

				}

				// ------------------------------------------------------------

				for ( int y = 0; y < m_nModValue; y++ ) {

					const int * dl = &pDitherTable[ y << m_nPow2 ];
	
					for ( int v = 0; v < inputRange; v++ ) {

						float fq = adjustedValues[ v ];
						int iq = (int)fq;
						int lo = ((iq) << outputUpShift);
						int hi = ((iq + 1) << outputUpShift);

						int l = (int)((fq - (float)iq) * entries);

						for ( int x = 0; x < m_nModValue; x++ ) {

							m_pDitherCache[ i++ ] = (l > dl[ x ]) ? hi : lo;

						}
	
					}
	
				}

			}

			// Clean up
			// ----------------------------------------------------------------

			delete [] pDitherTable;

			return true;

		}

		// --------------------------------------------------------------------

		dither_line_cache GetCacheForLine( const int y ) {

			return &m_pDitherCache[ (y & m_nModMask) * m_nCacheLineSize ];

		}

		int GetCacheInc() const {

			return m_nModValue;

		}

		int GetCacheMask() const {

			return m_nModMask;

		}

		int GetCachePow2() const {

			return m_nPow2;

		}

	};

	// ========================================================================

	//
	//	OrderedDither_Channel_inplace_32bpp()
	//

	static bool
	OrderedDither_Channel_inplace_32bpp(
		void * pBufferData
	,	const int nBufferWidth 
	,	const int nBufferHeight
	,	const int nBufferPitch
	,	const int nOutputBits
	,	const int nDitherSizePow2
	,	const int xOriginOffset
	,	const int yOriginOffset
	,	const unsigned buffer_ChannelMask
	,	const int buffer_ChannelShift
	,	const float brighten
	)
	{
		if ( 4 > nBufferWidth ) return true;

		// --------------------------------------------------------------------

		typedef TCachedOrderedDitherer<BYTE> ordered_dither_cache_type;
	
		ordered_dither_cache_type orderedDitherer;

		if ( !orderedDitherer.Init(
			nDitherSizePow2
			,256 // should this be 256?????
			,(1 << nOutputBits)
			,(8 - nOutputBits)
			,brighten
			) ) {

			return false;

		}

		// --------------------------------------------------------------------

		BYTE * pScanline = reinterpret_cast<BYTE *>( pBufferData );

		int logicalY = yOriginOffset;

		unsigned not_mask = ~buffer_ChannelMask;

		int pow2 = orderedDitherer.GetCachePow2();
		int mask = orderedDitherer.GetCacheMask();

		for ( int y = 0; y < nBufferHeight; y++, logicalY++ ) {

			unsigned * pU32Scanline = reinterpret_cast<unsigned *>( pScanline );

			pScanline += nBufferPitch;

			ordered_dither_cache_type::dither_line_cache cache = \
				orderedDitherer.GetCacheForLine( logicalY );
	
			int off = xOriginOffset & mask;

			for ( int x = nBufferWidth; 0 <= --x; pU32Scanline++ ) {

				unsigned value = *pU32Scanline;

				int channel = ((value >> buffer_ChannelShift) & 255);

#if 0
				if ( 1 ) {

//					channel += ((rand() & 7) - 4);
					channel += ((rand() & 3) - 1);

					channel = max( 0, min( channel, 255 ) );

				}
#endif

				int dithered = *(cache + (channel << pow2) + off);

				off = (++off) & mask;

#if 1 // 5/13/05

				dithered = ((dithered >> (8 - nOutputBits)) * 255) / ((1 << nOutputBits)-1);

#endif


				*pU32Scanline = (value & not_mask) | (dithered << buffer_ChannelShift);

			}

		}

		// --------------------------------------------------------------------

		return true;

	}

	// ========================================================================

	//
	//	DitherBuddy_inplace_32bpp()
	//

	bool DitherBuddy_inplace_32bpp(
		void * pBufferData
		,const int nBufferWidth 
		,const int nBufferHeight
		,const int nBufferPitch
		,const int xOriginOffset
		,const int yOriginOffset
		,const unsigned buffer_R_ChannelMask
		,const int buffer_R_Shift
		,const int buffer_R_OutputSigBits
		,const unsigned buffer_G_ChannelMask
		,const int buffer_G_Shift
		,const int buffer_G_OutputSigBits
		,const unsigned buffer_B_ChannelMask
		,const int buffer_B_Shift
		,const int buffer_B_OutputSigBits
		,const BPT::DITHER_STYLE ditherStyle
		,const int nOutputFormat
		,const int nMaxDrift
		,const float Brighten
		,const bool bSerpentine
		,const bool bForceDirection
		,const bool bUseNoise
		,const int nNoiseModulo
		,const bool bUseReset
		,const int nResetModulo
		,const bool bJustQuantize
		,const bool bStoreDifference
	)
	{
		int nDitherSizePow2 = (ditherStyle & ~ORDERED_FLAG);
		float fBrightenCoeff = 0.0625f * 1.5f; // 4.0f;

		// --------------------------------------------------------------------

		OrderedDither_Channel_inplace_32bpp(
			pBufferData
		,	nBufferWidth
		,	nBufferHeight
		,	nBufferPitch
		,	buffer_R_OutputSigBits
		,	nDitherSizePow2
		,	xOriginOffset
		,	yOriginOffset
		,	buffer_R_ChannelMask << buffer_R_Shift
		,	buffer_R_Shift
		,	fBrightenCoeff
		);

		// --------------------------------------------------------------------

		OrderedDither_Channel_inplace_32bpp(
			pBufferData
		,	nBufferWidth
		,	nBufferHeight
		,	nBufferPitch
		,	buffer_G_OutputSigBits
		,	nDitherSizePow2 // + 1 // this looks better but why exactly? (because it is buggy...
		,	xOriginOffset
		,	yOriginOffset
		,	buffer_G_ChannelMask << buffer_G_Shift
		,	buffer_G_Shift
		,	fBrightenCoeff
		);

		// --------------------------------------------------------------------

		OrderedDither_Channel_inplace_32bpp(
			pBufferData
		,	nBufferWidth
		,	nBufferHeight
		,	nBufferPitch
		,	buffer_B_OutputSigBits
		,	nDitherSizePow2
		,	xOriginOffset
		,	yOriginOffset
		,	buffer_B_ChannelMask << buffer_B_Shift
		,	buffer_B_Shift
		,	fBrightenCoeff
		);

		// --------------------------------------------------------------------

		return true;
	}

	// ------------------------------------------------------------------------

	//
	//	OrderedDither_Alpha_inplace_32bpp()
	//

	bool OrderedDither_Alpha_inplace_32bpp(
		void * pBufferData
		,const int nBufferWidth 
		,const int nBufferHeight
		,const int nBufferPitch
		,const int nOutputBits
		,const int nDitherSizePow2
		,const bool bPreserveOpaqueAndTransparent
		,const int xOriginOffset
		,const int yOriginOffset
		,const unsigned buffer_A_ChannelMask
		,const int buffer_A_Shift
		,const float brighten
		,const bool bAddNoise
	)
	{

#if 0

		return true; // disable

#else

		if ( 4 > nBufferWidth ) return true;

		// --------------------------------------------------------------------

		typedef TCachedOrderedDitherer<BYTE> ordered_dither_cache_type;
	
		ordered_dither_cache_type orderedDitherer;

		if ( !orderedDitherer.Init(
			nDitherSizePow2
			,256 // should this be 256?????
			,(1 << nOutputBits)
			,(8 - nOutputBits)
			,brighten
			) ) {

			return false;

		}

		// --------------------------------------------------------------------

		BYTE * pScanline = reinterpret_cast<BYTE *>( pBufferData );

		int logicalY = yOriginOffset;

		unsigned not_A_mask = ~buffer_A_ChannelMask;

		int pow2 = orderedDitherer.GetCachePow2();
		int mask = orderedDitherer.GetCacheMask();

		for ( int y = 0; y < nBufferHeight; y++, logicalY++ ) {

			unsigned * pU32Scanline = reinterpret_cast<unsigned *>( pScanline );

			pScanline += nBufferPitch;

			ordered_dither_cache_type::dither_line_cache cache = \
				orderedDitherer.GetCacheForLine( logicalY );
	
			int off = xOriginOffset & mask;

			for ( int x = nBufferWidth; 0 <= --x; ++pU32Scanline ) {

				unsigned value = *pU32Scanline;

				int alpha = ((value >> buffer_A_Shift) & 255);

#if 1 // use a threshold for making something opaque/transparent

				// the alpha thresholding is done in the encoder now!

				if ( (16 > alpha) || (239 < alpha) ) continue;

#endif

				if ( bPreserveOpaqueAndTransparent && ((255 == alpha) || (0 == alpha)) ) continue;

				int dithered = *(cache + (alpha << pow2) + off);

				off = (++off) & mask;

#if 1

//				dithered |= (alpha & ((1 << nOutputBits)-1));

				dithered = ((dithered >> (8 - nOutputBits)) * 255) / ((1 << nOutputBits)-1);

#endif

				*pU32Scanline = (value & not_A_mask) | (dithered << buffer_A_Shift);

			}

		}

		// --------------------------------------------------------------------

		return true;

#endif

	}

}; // namespace BPT
