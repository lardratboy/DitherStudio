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

#if !defined(AFX_TPSDFILECODE_H__9D2B4C1A_1D42_4364_A93E_65D59EF0F741__INCLUDED_)
#define AFX_TPSDFILECODE_H__9D2B4C1A_1D42_4364_A93E_65D59EF0F741__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ----------------------------------------------------------------------------

#include <list>
#include <utility>
#include "THelpers.h"

// ----------------------------------------------------------------------------

namespace BPT {

   	class CFileIO {

   	private:

   		FILE * m_Handle;

   	public:

   		CFileIO() : m_Handle( 0 ) { /* Empty */ }

   		~CFileIO() {

   			if ( m_Handle ) {

   				Close();

   			}

   		}

   		bool Open( const char * filename, const char * access ) {

   			m_Handle = fopen( filename, access );
   
   			return (0 != m_Handle);
   
   		}
   
   		void Close() {

   			if ( m_Handle ) {

   				fclose( m_Handle );
   				m_Handle = 0;

   			}
   
   		}
   
   		void Seek( const int pos, const int mode ) {
   
   			if ( m_Handle ) {

   				fseek( m_Handle, pos, mode );

   			}
   
   		}
   
   		int ReadByte() { 
   
   			if ( m_Handle ) {

   				return (unsigned)fgetc( m_Handle ) & 255;

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
			fread(ptr, 1, nBytes, m_Handle);

			return ptr;
		}

   		BYTE * LoadData( const int nBytes ) {

   			if ( m_Handle ) {

   				void * ptr = ::CoTaskMemAlloc( nBytes );
   
   				if ( 0 == ptr ) {
   
   					return 0;
   
   				}
   
   				fread( ptr, 1, nBytes, m_Handle );

   				return static_cast<BYTE *>( ptr );

   			}

   			return static_cast<BYTE *>( 0 );

   		}

   		void UnloadData( BYTE * ptr ) {

   			if ( ptr ) {

   				::CoTaskMemFree( static_cast<void *>( ptr ) );

   			}

   		}

   	};

	// (PSD-FILE-LOAD-FUNCTIONS) ----------------------------------------------

	//
	//	TPSDFileLoader<>
	//

	template<
		typename COLOR_SURFACE,
		typename CHANNEL_SURFACE = SURF_8BPP,
		typename CHANNEL_COLLECTION = std::list< CHANNEL_SURFACE * >
	> 
	class TPSDFileLoader {

	public:

		// Helper types
		// --------------------------------------------------------------------

		typedef TPSDFileLoader<COLOR_SURFACE,CHANNEL_SURFACE,CHANNEL_COLLECTION> this_type;
		typedef COLOR_SURFACE color_surface_type;
		typedef CHANNEL_SURFACE channel_surface_type;
		typedef CHANNEL_COLLECTION channel_collection_type;
		typedef std::pair< COLOR_SURFACE *, CHANNEL_COLLECTION * > loaded_data;

	private:

		// --------------------------------------------------------------------

		enum {

			PHOTOSHOP_MAGIC_NUMBER		= 0x38425053
			,RGB_COLOR_MODE				= 3
			,VALID_VERSION_NUMBER		= 1
			,VALID_CHANNEL_BIT_SIZE		= 8
			,COMPRESSION_NONE			= 0
			,COMPRESSION_PACKBITS		= 1

		};

		// --------------------------------------------------------------------

		class Unpack8BitChannel {

		public:

			Unpack8BitChannel() { /* Empty */ }

			template< typename SURFACE >
			bool operator()(
				SURFACE * surface, const int surfaceChannel,
				const BYTE * compressedChannelDataPtr
			) {
	
				int pixelsToProcess = (surface->Width() * surface->Height());
				int resetCount = surface->Width();
				int pixelsLeft = resetCount;
				int y = 0;
	
				SURFACE::pixel_iterator it = surface->Iterator( 0, y++ );
	
				while ( 0 < pixelsToProcess ) {
	
					int len = static_cast<int>( *compressedChannelDataPtr++ );
	
					if ( 128 > len ) { // Literal
	
						++len;
	
						pixelsToProcess -= len;
	
						if ( 0 > pixelsToProcess ) {
	
							len += pixelsToProcess;
							pixelsToProcess = 0;
	
						}
	
						while ( 0 < len ) {

							// Process as many pixels in this span as possible
							// ------------------------------------------------

							int count = min( pixelsLeft, len );

							pixelsLeft -= count;

							len -= count;

							while ( 0 <= --count ) {

								it->SetChannel( surfaceChannel, *compressedChannelDataPtr++ );

								++it;

							}
	
							// If we're at the end of the span move
							// ------------------------------------------------

							if ( (0 == pixelsLeft) && pixelsToProcess ) {
	
								it = surface->Iterator( 0, y++ );
								pixelsLeft = resetCount;
	
							}
	
						}
	
					} else if ( 128 < len ) { // Run
	
						len = (len ^ 0xff) + 2;
	
						pixelsToProcess -= len;
	
						if ( 0 > pixelsToProcess ) {
	
							len += pixelsToProcess;
							pixelsToProcess = 0;
	
						}
	
						int value = static_cast<int>( *compressedChannelDataPtr++ );
	
						while ( 0 < len ) {

							// Process as many pixels in this span as possible
							// ------------------------------------------------
	
							int count = min( pixelsLeft, len );

							pixelsLeft -= count;

							len -= count;

							while ( 0 <= --count ) {

								it->SetChannel( surfaceChannel, value );

								++it;

							}

							// If we're at the end of the span move
							// ------------------------------------------------
	
							if ( (0 == pixelsLeft) && pixelsToProcess ) {
	
								it = surface->Iterator( 0, y++ );
								pixelsLeft = resetCount;
	
							}
	
						}
	
					}
	
				}
	
				return true;
	
			}

		};

		// --------------------------------------------------------------------

		class ReadRaw8BitChannel {

		public:

			ReadRaw8BitChannel() { /* EMPTY */ }

			template< typename SURFACE >
			bool operator()(
				SURFACE * surface, const int surfaceChannel,
				const BYTE * channelDataPtr
			) {
	
				int cx = surface->Width();
				int cy = surface->Height();
	
				for ( int y = 0; y < cy; y++ ) {
	
					SURFACE::pixel_iterator it = surface->Iterator( 0, y );
	
					for ( int x = 0; x < cx; x++ ) {
	
						it->SetChannel( surfaceChannel, *channelDataPtr++ );
	
						++it;
	
					}
	
				}
	
				return true;
	
			}

		};

		// --------------------------------------------------------------------

		template< typename IO, typename DECODER, typename SIZECOLLECTION >
		bool LoadChannelsHandler(
			IO & input, DECODER & decode, SIZECOLLECTION & dataSizeTable,
			COLOR_SURFACE * colorSurface, CHANNEL_COLLECTION * channelCollection,
			int loadChannelCount
		) {

   			// Prepare for channel data load loop
   			// ----------------------------------------------------------------

   			int colorChannelsLeft = colorSurface->ChannelCount();

   			int nextChannel = 0;

   			CHANNEL_COLLECTION::iterator channelIT;

   			if ( channelCollection ) {

   				channelIT = channelCollection->begin();

   			}

   			SIZECOLLECTION::iterator sizeIT = dataSizeTable.begin();

   			// Read each channel
   			// -------------------------------------------------------------

   			for ( int channel = 0; channel < loadChannelCount; channel++ ) {

   				// read the entire channel into ram
   				// --------------------------------------------------------

   				int channelDataSize = *sizeIT++;

   				BYTE * data = input.LoadData( channelDataSize );

   				if ( !data ) {

					return false;

   				}

   				// Determine which surface & channel to write data into
   				// --------------------------------------------------------

   				if ( colorChannelsLeft ) {

   					decode( colorSurface, nextChannel, data );

   					if ( --colorChannelsLeft ) {

   						++nextChannel;

   					}

   				} else if ( channelCollection ) {

   					decode( *channelIT++, 0, data );

   				}

   				// Unload the loaded data
   				// --------------------------------------------------------

   				input.UnloadData( data );

   			}

			return true;

		}

		// --------------------------------------------------------------------
		
	public:

		// --------------------------------------------------------------------

		template< typename IO > loaded_data
		LoadFromAbstraction( IO & input, const bool loadAllChannels = true ) {

			// Read & Validate the header
			// ----------------------------------------------------------------

			if ( PHOTOSHOP_MAGIC_NUMBER != input.Read_m32() ) {

				return loaded_data( 0, 0 );

			}

			if ( VALID_VERSION_NUMBER != input.Read_m16() ) {

				return loaded_data( 0, 0 );

			}

			// Skip 6 bytes of pad
			// ----------------------------------------------------------------

			input.Read_m32(); // Pad4 (should be zero)
			input.Read_m16(); // Pad2 (should be zero)

			// Read the channel count
			// ----------------------------------------------------------------

			int channelCount = input.Read_m16();

			// Get the bitmap dimensions
			// ----------------------------------------------------------------

			SIZE imageSize;
			imageSize.cy = input.Read_m32();
			imageSize.cx = input.Read_m32();

			// Read the size of the channel data
			// ----------------------------------------------------------------

			if ( VALID_CHANNEL_BIT_SIZE != input.Read_m16() ) {

				return loaded_data( 0, 0 );

			}

			if ( RGB_COLOR_MODE != input.Read_m16() ) {

				return loaded_data( 0, 0 );

			}

			// Skip unprocessed data blocks
			// ----------------------------------------------------------------

			int colorModelDataSize = input.Read_m32();

			if ( colorModelDataSize ) {

				input.Seek( colorModelDataSize, SEEK_CUR );

			}

			int imageResourceDataSize = input.Read_m32();

			if ( imageResourceDataSize ) {

				input.Seek( imageResourceDataSize, SEEK_CUR );

			}

			int layerAndMaskInfoDataSize = input.Read_m32();

			if ( layerAndMaskInfoDataSize ) {

				input.Seek( layerAndMaskInfoDataSize, SEEK_CUR );

			}

			// Finally get down to business and read some data
			// ----------------------------------------------------------------

			int compressionType = input.Read_m16();

			if ( (COMPRESSION_NONE != compressionType) && 
				(COMPRESSION_PACKBITS != compressionType) ) {

				return loaded_data( 0, 0 );

			}

			// Create all of the surfaces to store the bitmap data
			// ----------------------------------------------------------------

			COLOR_SURFACE * colorSurface = new COLOR_SURFACE;

			if ( !colorSurface ) {

				return loaded_data( 0, 0 );

			}

			if ( !colorSurface->Create( imageSize ) ) {

				delete colorSurface;
				return loaded_data( 0, 0 );

			}

			// Are there extra channels?
			// ----------------------------------------------------------------

			int extraChannelCount = channelCount - colorSurface->ChannelCount();

			CHANNEL_COLLECTION * channelCollection = 0;

			if ( loadAllChannels && (0 < extraChannelCount) ) {

				channelCollection = new CHANNEL_COLLECTION;

				if ( !channelCollection ) {

					goto HANDLE_ERROR;

				}

				for ( int i = 0; i < extraChannelCount; i++ ) {

					CHANNEL_SURFACE * pChannel = new CHANNEL_SURFACE;

					if ( !pChannel ) {

						goto HANDLE_ERROR;

					}

					if ( !pChannel->Create( imageSize ) ) {

						delete pChannel;
						goto HANDLE_ERROR;

					}

					channelCollection->push_back( pChannel );

				}

			}

			// Determine the number of channels to actually process
			// ----------------------------------------------------------------

			int loadChannelCount;

			if ( channelCollection ) {

				loadChannelCount = channelCount;

			} else {

				loadChannelCount = min( channelCount, colorSurface->ChannelCount() );

			}

			// Get down to business and read each channel
			// ----------------------------------------------------------------

			if ( COMPRESSION_NONE == compressionType ) {

				// Build the dataSizeTable
				// ------------------------------------------------------------

				typedef std::vector<int> SizeCollection;

				SizeCollection dataSizeTable;

				int channelDataSize = imageSize.cx * imageSize.cy;

				for ( int i = 0; i < channelCount; i++ ) {

					dataSizeTable.push_back( channelDataSize );

				}

				// Call the handler
				// ------------------------------------------------------------

				if ( !LoadChannelsHandler(
					input, ReadRaw8BitChannel(), dataSizeTable,
					colorSurface, channelCollection, loadChannelCount ) ) {

					goto HANDLE_ERROR;

				}

			} else {

				// Read in the compressed channel data size table
				// Assume reading entire compressed channel as one big chunk
				// -------------------------------------------------------------

				typedef std::vector<int> SizeCollection;

				SizeCollection dataSizeTable;

				for ( int i = 0; i < channelCount; i++ ) {

					int channelDataSize = 0;

					for ( int y = 0; y < imageSize.cy; y++ ) {

						channelDataSize += input.Read_m16();

					}

					dataSizeTable.push_back( channelDataSize );

				}

				// Call the handler
				// ------------------------------------------------------------

				if ( !LoadChannelsHandler(
					input, Unpack8BitChannel(), dataSizeTable,
					colorSurface, channelCollection, loadChannelCount ) ) {

					goto HANDLE_ERROR;

				}

			}

			// Finally return our loaded data
			// ----------------------------------------------------------------

			return loaded_data( colorSurface, channelCollection );

			// ----------------------------------------------------------------

		HANDLE_ERROR:

			if ( colorSurface ) {

				delete colorSurface;

			}

			if ( channelCollection ) {

				CHANNEL_COLLECTION::iterator it;

				for ( it = channelCollection->begin(); 
					it != channelCollection->end(); it++ ) {

					delete (*it);

				}

				delete channelCollection;

			}

			return loaded_data( 0, 0 );

		}

		// --------------------------------------------------------------------

		loaded_data
		LoadFromFile( const char * filename, const bool loadAllChannels = true ) {

			// Open the file return error if not opened.
			// ----------------------------------------------------------------

			CFileIO inFile;

			if ( !inFile.Open( filename, "rb" ) ) {

				return loaded_data( 0, 0 );

			}

			return LoadFromAbstraction( inFile, loadAllChannels );

		}

	};

}; // namespace BPT

#endif // !defined(AFX_TPSDFILECODE_H__9D2B4C1A_1D42_4364_A93E_65D59EF0F741__INCLUDED_)
