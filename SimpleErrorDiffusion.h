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

#if !defined(AFX_SIMPLEERRORDIFFUSION_H__1A9F64FE_5C9C_4876_BC07_1C9C8C16558D__INCLUDED_)
#define AFX_SIMPLEERRORDIFFUSION_H__1A9F64FE_5C9C_4876_BC07_1C9C8C16558D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "DitherBuddy.h"
#include "BPTLLC_DitherBuddyOrdered.h"

// ----------------------------------------------------------------------------

namespace BPT {

	// ------------------------------------------------------------------------

	//
	//	TEST_TErrorDiffusionDither<>
	//
	
	template<
		typename SURFACE
		,const int DRM, const int DGM, const int DBM
		,const int SRM, const int SGM, const int SBM
		,const int RSL, const int GSL, const int BSL
	>
	class TEST_TErrorDiffusionDither {
	
	public:
	
	private:

		typedef float error_term_storage;
	
		typedef std::vector<error_term_storage> error_collection_type;

		typedef std::vector<error_collection_type *> error_collection_collection_type;

		// ------------------------------------------------------------------------

		struct SDistributionEntry {

			int v;
			int o;

			float coeff;

			SDistributionEntry( 
				const int iv, const int io, const float icoeff
			) : v(iv), o(io), coeff( icoeff ) {

			}

		};

		typedef std::vector<SDistributionEntry> error_distribution_collection_type;
	
		// ------------------------------------------------------------------------
	
		typename typename SURFACE::pixel_type __forceinline Make(
			const error_term_storage r, 
			const error_term_storage g, 
			const error_term_storage b
		) {
	
			return typename SURFACE::pixel_type(
				(((MAXX( 0, MINN( 255, (int)r )) & DRM))),
				(((MAXX( 0, MINN( 255, (int)g )) & DGM))),
				(((MAXX( 0, MINN( 255, (int)b )) & DBM)))
			);
	
		}

		// ------------------------------------------------------------------------

		int __forceinline GetSourceR(typename SURFACE::pixel_type p ) {

			return p.R();

		}
	
		int __forceinline GetSourceG(typename SURFACE::pixel_type p ) {

			return p.G();

		}

		int __forceinline GetSourceB(typename SURFACE::pixel_type p ) {

			return p.B();

		}

		// ------------------------------------------------------------------------

		int __forceinline GetDestR(typename SURFACE::pixel_type p ) {

			return p.R();

		}
	
		int __forceinline GetDestG(typename SURFACE::pixel_type p ) {

			return p.G();

		}

		int __forceinline GetDestB(typename SURFACE::pixel_type p ) {

			return p.B();

		}

		// ------------------------------------------------------------------------

		bool m_bUseNoise;
		bool m_bUseReset;
		int m_nNoiseModulo;
		int m_nNoiseAdjust;
		int m_nResetModulo;
		bool m_bSerpentine;
		bool m_bForceDirection;
	
		// ------------------------------------------------------------------------
	
		error_term_storage __forceinline Noise() {

			if ( m_bUseNoise && m_nNoiseModulo ) {

				return (error_term_storage)((rand() % m_nNoiseModulo) - m_nNoiseAdjust);

			}
	
			return (error_term_storage)0;
	
		}
	
		bool __forceinline StoreTerm( const int x, const int y ) {

			// This really needs to use the reset modulo as 
			// a level into an ordered dither table :)  I think that would
			// help with the diagnal patterns

			if ( m_bUseReset && m_nResetModulo ) {

#if 1

				static int resetTable[ 4 ][ 4 ] = {
					{  0,  8,  2, 10 },
					{ 12,  4, 14,  6 },
					{  3, 11,  1,  9 },
					{ 15,  7, 13,  5 }
				};

				return (m_nResetModulo > resetTable[ x & 3 ][ y & 3 ]);

#else

#if 0

				return ((x * x + y * y) % m_nResetModulo) ? true : false;

#else

				return ((x + y) % m_nResetModulo) ? true : false;

#endif

#endif

			}
	
			return true;
	
		}
	
		// ------------------------------------------------------------------------
	
		void __forceinline DistributeError( 
			const int x, const int y, const int d, 
			error_collection_collection_type & errors,
			error_distribution_collection_type & distributionTable,
			const error_term_storage e
		) {
	
			error_term_storage et = e + Noise(); 
	
			if ( StoreTerm( x, y ) ) {

				error_distribution_collection_type::iterator it;

				for ( it = distributionTable.begin(); it != distributionTable.end(); it++ ) {

					(*errors[ (it->v) ])[ x + ((it->o) * d) ] += et * it->coeff;

				}

			}
	
		}
	
		// ------------------------------------------------------------------------

		TEST_TErrorDiffusionDither(); // Hidden!

		// ------------------------------------------------------------------------

		int LineCountFromDistributionTable(
			error_distribution_collection_type & distributionTable
		) const {

			int lines = 0;

			error_distribution_collection_type::iterator it;

			for ( it = distributionTable.begin(); it != distributionTable.end(); it++ ) {

				if ( lines < it->v ) {

					lines = it->v;

				}

			}

			return (lines + 1);

		}

		int ReachFromDistributionTable(
			error_distribution_collection_type & distributionTable
		) const {

			int reach = 0;

			error_distribution_collection_type::iterator it;

			for ( it = distributionTable.begin(); it != distributionTable.end(); it++ ) {

				int o = abs( it->o );

				if ( reach < o ) {

					reach = o;

				}

			}

			return reach;

		}

		void AddToDistributionTable(
			error_distribution_collection_type & distributionTable,
			const int v, const int o, const error_term_storage coeff
		) const {

			distributionTable.push_back( SDistributionEntry( v, o, coeff) );

		}

		// ------------------------------------------------------------------------

		void DestroyErrorTables(
			error_collection_collection_type & errors
			) {

			while ( !errors.empty() ) {

				delete errors.back();

				errors.pop_back();

			}

		}

		// ------------------------------------------------------------------------


		// ------------------------------------------------------------------------

	public:
	
		// ------------------------------------------------------------------------
	
		TEST_TErrorDiffusionDither(
			const bool bUseNoise
			,const int nNoiseModulo
			,const bool bUseReset
			,const int nResetModulo
			,const bool bSerpentine
			,const bool bForceDirection
		) {
		
			m_bUseNoise = bUseNoise;
			m_nNoiseModulo = nNoiseModulo;
			m_nNoiseAdjust = m_nNoiseModulo / 2;
			m_bUseReset = bUseReset;
			m_nResetModulo = nResetModulo;
			m_bSerpentine = bSerpentine;
			m_bForceDirection = bForceDirection;
		
		}
	
		~TEST_TErrorDiffusionDither() {}
	
		// ------------------------------------------------------------------------

		bool OrderedDitherInto(
			SURFACE* pDest, SURFACE* pSource,
			const enum DITHER_STYLE style,
			const int output_r_bit_count,
			const int output_g_bit_count,
			const int output_b_bit_count
		) {
			T_Blit(*pDest, 0, 0, *pSource, TCopyROP<SURFACE::pixel_type>() );

			return DitherBuddy_inplace_32bpp(
				pDest->GetDataPtr(), pDest->Width(), pDest->Height(), pDest->Pitch(), 0, 0
				, SRM // TPixelTraits<PIXEL_ARGB8888>::r_isolation_mask
				, RSL // TPixelTraits<PIXEL_ARGB8888>::r_shift
				, output_r_bit_count // TPixelTraits<SURFACE::pixel_type>::r_bit_count
				, SGM // TPixelTraits<PIXEL_ARGB8888>::g_isolation_mask
				, GSL // TPixelTraits<PIXEL_ARGB8888>::g_shift
				, output_g_bit_count // TPixelTraits<SURFACE::pixel_type>::g_bit_count
				, SBM // TPixelTraits<PIXEL_ARGB8888>::b_isolation_mask
				, BSL // TPixelTraits<PIXEL_ARGB8888>::b_shift
				, output_b_bit_count // TPixelTraits<SURFACE::pixel_type>::b_bit_count
				, style
			);
		}
	
		bool DitherInto(
			SURFACE * pDest, SURFACE * pSource, 
			const enum DITHER_STYLE style,
			const int output_r_bit_count,
			const int output_g_bit_count,
			const int output_b_bit_count
		) {

			if (ORDERED_FLAG & style) {
				return OrderedDitherInto(pDest, pSource, style, output_r_bit_count, output_g_bit_count, output_b_bit_count);
			}
	
			// construct our error term collections
			// --------------------------------------------------------------------

			error_collection_collection_type rErrors;
			error_collection_collection_type gErrors;
			error_collection_collection_type bErrors;

			// prepare
			// --------------------------------------------------------------------
	
			int w = pSource->Width();
			int h = pSource->Height();
	
			// Build the error distribution collection
			// --------------------------------------------------------------------

			error_distribution_collection_type errorDistributionTable;

			switch ( style ) {

			default:
			case FLOYD_STEINBERG:
				AddToDistributionTable( errorDistributionTable, 0, +1, (error_term_storage)(7.0 / 16.0) );
				AddToDistributionTable( errorDistributionTable, 1, -1, (error_term_storage)(3.0 / 16.0) );
				AddToDistributionTable( errorDistributionTable, 1, +0, (error_term_storage)(5.0 / 16.0) );
				AddToDistributionTable( errorDistributionTable, 1, +1, (error_term_storage)(1.0 / 16.0) );
				break;

			case JARVIS_JUDICE_NINKE:
				AddToDistributionTable( errorDistributionTable, 0, +1, (error_term_storage)(7.0 / 48.0) );
				AddToDistributionTable( errorDistributionTable, 0, +2, (error_term_storage)(5.0 / 48.0) );
				AddToDistributionTable( errorDistributionTable, 1, -2, (error_term_storage)(3.0 / 48.0) );
				AddToDistributionTable( errorDistributionTable, 1, -1, (error_term_storage)(5.0 / 48.0) );
				AddToDistributionTable( errorDistributionTable, 1, +0, (error_term_storage)(7.0 / 48.0) );
				AddToDistributionTable( errorDistributionTable, 1, +1, (error_term_storage)(5.0 / 48.0) );
				AddToDistributionTable( errorDistributionTable, 1, +2, (error_term_storage)(3.0 / 48.0) );
				AddToDistributionTable( errorDistributionTable, 2, -2, (error_term_storage)(1.0 / 48.0) );
				AddToDistributionTable( errorDistributionTable, 2, -1, (error_term_storage)(3.0 / 48.0) );
				AddToDistributionTable( errorDistributionTable, 2, +0, (error_term_storage)(5.0 / 48.0) );
				AddToDistributionTable( errorDistributionTable, 2, +1, (error_term_storage)(3.0 / 48.0) );
				AddToDistributionTable( errorDistributionTable, 2, +2, (error_term_storage)(1.0 / 48.0) );
				break;

			case STUCKI:
				AddToDistributionTable( errorDistributionTable, 0, +1, (error_term_storage)(8.0 / 42.0) );
				AddToDistributionTable( errorDistributionTable, 0, +2, (error_term_storage)(4.0 / 42.0) );
				AddToDistributionTable( errorDistributionTable, 1, -2, (error_term_storage)(2.0 / 42.0) );
				AddToDistributionTable( errorDistributionTable, 1, -1, (error_term_storage)(4.0 / 42.0) );
				AddToDistributionTable( errorDistributionTable, 1, +0, (error_term_storage)(8.0 / 42.0) );
				AddToDistributionTable( errorDistributionTable, 1, +1, (error_term_storage)(4.0 / 42.0) );
				AddToDistributionTable( errorDistributionTable, 1, +2, (error_term_storage)(2.0 / 42.0) );
				AddToDistributionTable( errorDistributionTable, 2, -2, (error_term_storage)(1.0 / 42.0) );
				AddToDistributionTable( errorDistributionTable, 2, -1, (error_term_storage)(2.0 / 42.0) );
				AddToDistributionTable( errorDistributionTable, 2, +0, (error_term_storage)(4.0 / 42.0) );
				AddToDistributionTable( errorDistributionTable, 2, +1, (error_term_storage)(2.0 / 42.0) );
				AddToDistributionTable( errorDistributionTable, 2, +2, (error_term_storage)(1.0 / 42.0) );
				break;

			case STEVENSON_ARCE:
				AddToDistributionTable( errorDistributionTable, 0, +2, (error_term_storage)(32.0 / 200.0) );
				AddToDistributionTable( errorDistributionTable, 1, -3, (error_term_storage)(12.0 / 200.0) );
				AddToDistributionTable( errorDistributionTable, 1, -2, (error_term_storage)(26.0 / 200.0) );
				AddToDistributionTable( errorDistributionTable, 1, +1, (error_term_storage)(30.0 / 200.0) );
				AddToDistributionTable( errorDistributionTable, 1, +3, (error_term_storage)(30.0 / 200.0) );
				AddToDistributionTable( errorDistributionTable, 2, -2, (error_term_storage)(12.0 / 200.0) );
				AddToDistributionTable( errorDistributionTable, 2, +0, (error_term_storage)(26.0 / 200.0) );
				AddToDistributionTable( errorDistributionTable, 2, +2, (error_term_storage)(12.0 / 200.0) );
				AddToDistributionTable( errorDistributionTable, 3, -3, (error_term_storage)(5.0 / 200.0) );
				AddToDistributionTable( errorDistributionTable, 3, -2, (error_term_storage)(12.0 / 200.0) );
				AddToDistributionTable( errorDistributionTable, 3, +1, (error_term_storage)(12.0 / 200.0) );
				AddToDistributionTable( errorDistributionTable, 3, +3, (error_term_storage)(5.0 / 200.0) );
				break;

			case BURKES:
				AddToDistributionTable( errorDistributionTable, 0, +1, (error_term_storage)(8.0 / 32.0) );
				AddToDistributionTable( errorDistributionTable, 0, +2, (error_term_storage)(4.0 / 32.0) );
				AddToDistributionTable( errorDistributionTable, 1, -2, (error_term_storage)(2.0 / 32.0) );
				AddToDistributionTable( errorDistributionTable, 1, -1, (error_term_storage)(4.0 / 32.0) );
				AddToDistributionTable( errorDistributionTable, 1, +0, (error_term_storage)(8.0 / 32.0) );
				AddToDistributionTable( errorDistributionTable, 1, +1, (error_term_storage)(4.0 / 32.0) );
				AddToDistributionTable( errorDistributionTable, 1, +2, (error_term_storage)(2.0 / 32.0) );
				break;

			case SIERRA:
				AddToDistributionTable( errorDistributionTable, 0, +1, (error_term_storage)(5.0 / 32.0) );
				AddToDistributionTable( errorDistributionTable, 0, +2, (error_term_storage)(3.0 / 32.0) );
				AddToDistributionTable( errorDistributionTable, 1, -2, (error_term_storage)(2.0 / 32.0) );
				AddToDistributionTable( errorDistributionTable, 1, -1, (error_term_storage)(4.0 / 32.0) );
				AddToDistributionTable( errorDistributionTable, 1, +0, (error_term_storage)(5.0 / 32.0) );
				AddToDistributionTable( errorDistributionTable, 1, +1, (error_term_storage)(4.0 / 32.0) );
				AddToDistributionTable( errorDistributionTable, 1, +2, (error_term_storage)(2.0 / 32.0) );
				AddToDistributionTable( errorDistributionTable, 2, -1, (error_term_storage)(2.0 / 32.0) );
				AddToDistributionTable( errorDistributionTable, 2, +0, (error_term_storage)(3.0 / 32.0) );
				AddToDistributionTable( errorDistributionTable, 2, +1, (error_term_storage)(2.0 / 32.0) );
				break;

			case OTHER_FROM_WEB:
#if 1
				AddToDistributionTable( errorDistributionTable, 0, +1, (error_term_storage)(60.0 / 140.0) );
				AddToDistributionTable( errorDistributionTable, 1, -3, (error_term_storage)(5.0 / 140.0) );
				AddToDistributionTable( errorDistributionTable, 1, -2, (error_term_storage)(10.0 / 140.0) );
				AddToDistributionTable( errorDistributionTable, 1, -1, (error_term_storage)(15.0 / 140.0) );
				AddToDistributionTable( errorDistributionTable, 1, +0, (error_term_storage)(20.0 / 140.0) );
				AddToDistributionTable( errorDistributionTable, 1, +1, (error_term_storage)(15.0 / 140.0) );
				AddToDistributionTable( errorDistributionTable, 1, +2, (error_term_storage)(10.0 / 140.0) );
				AddToDistributionTable( errorDistributionTable, 1, +3, (error_term_storage)(5.0 / 140.0) );
#else
				AddToDistributionTable( errorDistributionTable, 0, +1, (error_term_storage)(2.0 / 10.0) );
				AddToDistributionTable( errorDistributionTable, 0, +2, (error_term_storage)(1.0 / 10.0) );
				AddToDistributionTable( errorDistributionTable, 1, -1, (error_term_storage)(1.0 / 10.0) );
				AddToDistributionTable( errorDistributionTable, 1, +0, (error_term_storage)(2.0 / 10.0) );
				AddToDistributionTable( errorDistributionTable, 1, +1, (error_term_storage)(1.0 / 10.0) );
				AddToDistributionTable( errorDistributionTable, 2, -2, (error_term_storage)(1.0 / 10.0) );
				AddToDistributionTable( errorDistributionTable, 2, +0, (error_term_storage)(1.0 / 10.0) );
				AddToDistributionTable( errorDistributionTable, 2, +2, (error_term_storage)(1.0 / 10.0) );
#endif
				break;

			}

#if 0

			// Fill in the floyd-steinberg table
			// --------------------------------------------------------------------

			errorDistributionTable.push_back( SDistributionEntry( 0, +1, (error_term_storage)(7.0 / 16.0) ) );
			errorDistributionTable.push_back( SDistributionEntry( 1, -1, (error_term_storage)(3.0 / 16.0) ) );
			errorDistributionTable.push_back( SDistributionEntry( 1, +0, (error_term_storage)(5.0 / 16.0) ) );
			errorDistributionTable.push_back( SDistributionEntry( 1, +1, (error_term_storage)(1.0 / 16.0) ) );

#endif

			// reserve
			// --------------------------------------------------------------------

			int lines = LineCountFromDistributionTable( errorDistributionTable );

			for ( int tableCounter = 0; tableCounter < lines; tableCounter++ ) {

				// ----------------------------------------------------------------

				error_collection_type * pRErrorTable = new error_collection_type;

				if ( !pRErrorTable ) {

					DestroyErrorTables( rErrors );
					DestroyErrorTables( gErrors );
					DestroyErrorTables( bErrors );

					return false;

				}

				pRErrorTable->reserve( w );

				// ----------------------------------------------------------------

				error_collection_type * pGErrorTable = new error_collection_type;

				if ( !pGErrorTable ) {

					DestroyErrorTables( rErrors );
					DestroyErrorTables( gErrors );
					DestroyErrorTables( bErrors );

					return false;

				}

				pGErrorTable->reserve( w );

				// ----------------------------------------------------------------

				error_collection_type * pBErrorTable = new error_collection_type;

				if ( !pBErrorTable ) {
	
					DestroyErrorTables( rErrors );
					DestroyErrorTables( gErrors );
					DestroyErrorTables( bErrors );

					return false;

				}

				pBErrorTable->reserve( w );

				// ----------------------------------------------------------------

				for ( int i = 0; i < w; i++ ) {

					pRErrorTable->push_back( (error_term_storage)0 );
					pGErrorTable->push_back( (error_term_storage)0 );
					pBErrorTable->push_back( (error_term_storage)0 );

				}

				// ----------------------------------------------------------------

				rErrors.push_back( pRErrorTable );
				gErrors.push_back( pGErrorTable );
				bErrors.push_back( pBErrorTable );

			}
	
			// Get the easy access references
			// --------------------------------------------------------------------

			error_collection_type & rError1 = *(rErrors[ 0 ]);
			error_collection_type & gError1 = *(gErrors[ 0 ]);
			error_collection_type & bError1 = *(bErrors[ 0 ]);

			// --------------------------------------------------------------------

			int reach = ReachFromDistributionTable( errorDistributionTable );

			for ( int y = 0; y < h; y++ ) {
	
				// ----------------------------------------------------------------
		
				typename SURFACE::pixel_iterator sit = pSource->Iterator( 0, y );
		
				// Start by adding this lines data to last lines error term
				// ----------------------------------------------------------------
		
				for ( int x = 0; x < w; x++ ) {

					typename SURFACE::pixel_type p = *sit;

					// ------------------------------------------------------------

					rError1[ x ] = (*rErrors[ 1 ])[ x ] + (error_term_storage)GetSourceR( p );
					gError1[ x ] = (*gErrors[ 1 ])[ x ] + (error_term_storage)GetSourceG( p );
					bError1[ x ] = (*bErrors[ 1 ])[ x ] + (error_term_storage)GetSourceB( p );

					// ------------------------------------------------------------
					int lc = 1;

					for ( ; lc < (lines - 1); lc++ ) {

						(*rErrors[ lc ])[ x ] = (*rErrors[ lc + 1 ])[ x ];
						(*gErrors[ lc ])[ x ] = (*gErrors[ lc + 1 ])[ x ];
						(*bErrors[ lc ])[ x ] = (*bErrors[ lc + 1 ])[ x ];

					}
	
					// ------------------------------------------------------------

					(*rErrors[ lc ])[ x ] = (error_term_storage)0;
					(*gErrors[ lc ])[ x ] = (error_term_storage)0;
					(*bErrors[ lc ])[ x ] = (error_term_storage)0;

					// ------------------------------------------------------------

					++sit;
		
				}

				// ----------------------------------------------------------------

				bool bForwardDir;
				
				if ( m_bSerpentine ) {
					
					bForwardDir = (y & 1) ? true : false;

				} else {

					bForwardDir = m_bForceDirection;

				}
	
				// Okay lets dither the line
				// ----------------------------------------------------------------

				int t = reach;
	
				if ( bForwardDir ) {

					typename SURFACE::pixel_iterator dit = pDest->Iterator( 0, y );

					int tc = 0;

					for ( ; tc < t; tc++ ) {
		
						*dit++ = Make( rError1[ tc ], gError1[ tc ], bError1[ tc ] );

					}

					int i = t;
		
					for ( ; i < (w - t); i++ ) {

						// ------------------------------------------------------------
		
						typename SURFACE::pixel_type p = (*dit = Make( rError1[ i ], gError1[ i ], bError1[ i ] ));
		
						DistributeError( 
							i, y, 1, rErrors, errorDistributionTable, 
							(rError1[ i ] - (error_term_storage)GetDestR( p )) 
						);

						DistributeError( 
							i, y, 1, gErrors, errorDistributionTable,
							(gError1[ i ] - (error_term_storage)GetDestG( p )) 
						);

						DistributeError( 
							i, y, 1, bErrors, errorDistributionTable,
							(bError1[ i ] - (error_term_storage)GetDestB( p )) 
						);
		
						// ------------------------------------------------------------
		
						++dit;
		
					}
		
					for ( tc = 0; tc < t; tc++, i++ ) {

						*dit++ = Make( rError1[ i ], gError1[ i ], bError1[ i ] );

					}

				} else {
	
					SURFACE::reverse_iterator dit = pDest->rIterator( w - 1, y );
	
					int l = w - t;

					int tc = 0;
		
					for ( ; tc < t; tc++ ) {

						int i = w - tc - 1;

						*dit++ = Make( rError1[ i ], gError1[ i ], bError1[ i ] );

					}
		
					for ( int j = t; j < (w - t); j++ ) {
	
						int i = w - j - 1;
		
						// ------------------------------------------------------------
		
						SURFACE::pixel_type p = (*dit = Make( rError1[ i ], gError1[ i ], bError1[ i ] ));
		
						DistributeError( 
							i, y, -1, rErrors, errorDistributionTable, 
							(error_term_storage)(rError1[ i ] - (error_term_storage)GetDestR( p )) 
						);

						DistributeError( 
							i, y, -1, gErrors, errorDistributionTable, 
							(error_term_storage)(gError1[ i ] - (error_term_storage)GetDestG( p )) 
						);

						DistributeError( 
							i, y, -1, bErrors, errorDistributionTable, 
							(error_term_storage)(bError1[ i ] - (error_term_storage)GetDestB( p )) 
						);
		
						// ------------------------------------------------------------
		
						++dit;

					}
		
					for ( tc = 0; tc < t; tc++ ) {

						int i = t - tc;

						*dit++ = Make( rError1[ i ], gError1[ i ], bError1[ i ] );

					}
	
				}

			}

			// ----------------------------------------------------------------

			DestroyErrorTables( rErrors );
			DestroyErrorTables( gErrors );
			DestroyErrorTables( bErrors );

			// ----------------------------------------------------------------

			return true;
	
		}
	
	};

}; // namespace BPT

// ----------------------------------------------------------------------------

#endif // !defined(AFX_SIMPLEERRORDIFFUSION_H__1A9F64FE_5C9C_4876_BC07_1C9C8C16558D__INCLUDED_)
