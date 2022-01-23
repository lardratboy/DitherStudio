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


#if !defined(AFX_TSTRUCTUREDDATA_H__A95A4664_6C6D_4489_80CC_B3826DDD0299__INCLUDED_)
#define AFX_TSTRUCTUREDDATA_H__A95A4664_6C6D_4489_80CC_B3826DDD0299__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ----------------------------------------------------------------------------

#include <windows.h>
#include <mmsystem.h>
#include <list>
#include <vector>
#include <utility>

// ----------------------------------------------------------------------------

namespace BPT {

/*

	NOTES:

	Data storage.

		Chunk data is stored using an output_iterator.  I need to build
		an efficient growable container that doesn't copy the data when
		the container is expanded, it needs to grow very efficiently
		because the container is going to be used to store lots of data.


*/

	// ========================================================================
	// CHUNKID standard id helper class.
	// ========================================================================

	//
	//	FOURCC_STANDARD_CHUNK_IDS
	//

	class FOURCC_STANDARD_CHUNK_IDS {

	public:

		static FOURCC QUICK_ROOT_CHUNK() {

			return MAKEFOURCC( 'Q', 'U', 'I', 'K' );

		}

		static FOURCC QUICK_INDEX_CHUNK() {

			return MAKEFOURCC( 'Q', 'I', 'D', 'X' );

		}

	};

	// ========================================================================
	// CHUNK_HEADER
	// ========================================================================

	struct SFourccChunkHeader {

		// Helper types
		// --------------------------------------------------------------------

		typedef FOURCC CHUNKID;
		typedef int SIZETYPE;

		// Structure data
		// --------------------------------------------------------------------

		CHUNKID m_Id;
		SIZETYPE m_Size;

		// Constructor
		// --------------------------------------------------------------------

		SFourccChunkHeader( const CHUNKID & id, const SIZETYPE size = 0 ) 
			: m_Id( id ), m_Size( size ) { /* Empty */ }

		// Standard chunk ID's?
		// --------------------------------------------------------------------

	};

	// ========================================================================
	// FIXUP CODE
	// ========================================================================

	// Since an output function needs access to fixup values should they be
	// defined externally to the TChunk class?

	// ========================================================================
	// CHUNK CODE
	// ========================================================================

	//
	//	TChunk
	//

	template<
		typename STORAGE_ITERATOR,
		typename CHUNK_HEADER = SFourccChunkHeader,
		typename STANDARD_CHUNKS = FOURCC_STANDARD_CHUNK_IDS
	>
	class TChunk {

	public:

		// --------------------------------------------------------------------

		typedef TChunk<STORAGE_ITERATOR,CHUNK_HEADER,STANDARD_CHUNKS> this_type;
		typedef this_type * pointer;
		typedef this_type & reference;
		typedef const this_type & const_reference;
		typedef const this_type * const_pointer;

		typedef typename CHUNK_HEADER::SIZETYPE SIZETYPE;
		typedef typename CHUNK_HEADER::CHUNKID CHUNKID;

		// Fixup types
		// --------------------------------------------------------------------

		enum FIXUP_SIZE_TYPE {

			WORD_FIXUP
			,DWORD_FIXUP

		};

		enum FIXUP_TYPE {

			CHUNK_RELATIVE
			,ROOT_RELATIVE

		};

		class CFixupValue {

		private:

			FIXUP_SIZE_TYPE m_Type;
			pointer m_Chunk;
			int m_Offset;
			int m_ID;

		public:

			CFixupValue(
				const FIXUP_SIZE_TYPE & type, const int id, const int offset,
				pointer chunk
			) : m_Type( type ), m_ID( id ), m_Offset( offset ), m_Chunk( chunk ) {

				/* Empty */

			}

			FIXUP_SIZE_TYPE type() const {

				return m_Type;

			}

			int offset() const {

				return m_Offset;

			}

			int id() const {

				return m_ID;

			}

			pointer chunk() const {

				return m_Chunk;

			}

		};

	private:

		// --------------------------------------------------------------------

		typedef std::list< pointer > CHILDCOLLECTION;

		// --------------------------------------------------------------------

		CHILDCOLLECTION m_Children;
		int m_ChunkFixupID;
		pointer m_Parent;
		int m_DataOffset;
		CHUNK_HEADER m_ChunkHeader;

		// The root block 'owns' this data. But all children have a pointer
		// to it!  This has to be a design pattern of some type. 
		// --------------------------------------------------------------------

		class CSharedData {

		private:

			std::list< CFixupValue * > m_FixupValues;
			STORAGE_ITERATOR m_DataIT;
			int m_NextFixupID;

		public:

			CSharedData( STORAGE_ITERATOR * pDataIT )
				: m_NextFixupID( 1 ), m_DataIT( *pDataIT ) {

				/* Empty */

			}

			int GetNextFixupID() {

				return m_NextFixupID++;

			}

			void OutputByte( const unsigned char v ) {

				*m_DataIT = v;
				++m_DataIT;

			}

			bool NoteFixupValue(
				const FIXUP_SIZE_TYPE & type, const int id, 
				const int offset, pointer chunk ) {

				CFixupValue * pFixup = CFixupValue( type, id, offset, chunk );
	
				if ( !pFixup ) {

					return false;

				}
	
				m_FixupValues.push_back( pFixup );

				return true;

			}

		};

		CSharedData * m_pSharedData;

		// --------------------------------------------------------------------

		TChunk(); // Hidden

		int SizeOverhead() const {

			return sizeof(CHUNK_HEADER);

		}

		CSharedData * GetSharedDataObject() {

			return m_pRootData;

		}

		int GetNextFixupID() {

			return GetSharedDataObject()->GetNextFixupID();

		}

		int GetCurrentDataOffset() const {

			return m_DataOffset;

		}

		void NoteFixupValueForChunk( const FIXUP_SIZE_TYPE & type, const int fixupID, const int dataOffset ) {

			GetSharedDataObject()->NotefixupValue( type, id, dataOffset, this );

		}

		// private construction
		// --------------------------------------------------------------------

		TChunk( const CHUNKID & id, pointer parent ) : 
			m_ChunkHeader( id ), m_Parent( parent ), m_ChunkFixupID( 0 ),
			m_pSharedData( 0 ), m_DataOffset( 0 )  {

			// associate the root data for this chunk
			// ----------------------------------------------------------------

			if ( parent ) {

				m_pSharedData = parent->GetSharedDataObject();
				m_ChunkFixupID = GetNextFixupID();

			}

		}

	public:

		// Factory
		// --------------------------------------------------------------------

		bool InitRootChunkData( STORAGE_ITERATOR * pDataIT = 0 ) {

			// really should be an assert!

			if ( m_Parent ) {

				return false;

			}

			m_pSharedData = new CSharedData( pDataIT );

			return (0 != m_pSharedData);

		}

		static pointer Factory( const CHUNKID & id, STORAGE_ITERATOR * pDataIT = 0 ) {

			pointer what = new this_type( id, static_cast<pointer>( 0 ) );

			if ( what ) {

				if ( !what->InitRootChunkData( pDataIT ) ) {

					delete what;
					return static_cast<pointer>( 0 );

				}

			}

			return what;

		}

		// Destruction, remove all children from the child collection,
		// assume that the data storage is managed by something else.
		// --------------------------------------------------------------------

		~TChunk() {

			// Delete all the children chunks

			while ( !m_Children.empty() ) {
	
				delete m_Children->back();
				m_Children->pop_back();
	
			}

			// if this chunk is the root chunk then delete the shared data

			if ( !m_Parent ) {

				delete m_pSharedData;

			}

		}

		// Create child (factory)
		// --------------------------------------------------------------------

		pointer CreateChild( const CHUNKID & id ) {

			pointer ptr = new this_type( id, this );

			if ( ptr ) {

				m_pChildren.push_back( ptr );

			}

			return ptr;

		}

		// Query
		// --------------------------------------------------------------------

		int DataSize() const {

			if ( m_Children ) {

				CHILDCOLLECTION::iterator it = m_Children.begin();

				int sum = 0;

				for ( ; it != m_Children.end(); it++ ) {

					sum += (*it)->Size();

				}

				return sum;

			}
			
			return GetCurrentDataOffset();

		}

		int Size() const {

			return DataSize() + SizeOverhead();

		}

		// Output helpers
		// --------------------------------------------------------------------

		reference operator<<( const unsigned char v ) {

			// Store off this byte to the shared data object,
			// ----------------------------------------------------------------

			GetSharedDataObject()->OutputByte( v );

			// Keep track of our own offset/size variable.
			// ----------------------------------------------------------------

			++m_DataOffset;

			return *this;

		}

		template< class T >
		reference operator<<( T & t ) {

			const unsigned char * ptr = static_cast< const unsigned char * >( &t );

			for ( size_t i = 0; i < sizeof(T); i++ ) {

				*this << *ptr++;

			}

			return *this;

		}

		template< class InIt >
		reference operator<<( std::pair< InIt, InIt> range ) {

			while ( range.first != range.second ) {

				*this << *range.first++;

			}

			return *this;

		}

		// Endian helper functions??
		// --------------------------------------------------------------------

		// Fixup related methods
		// --------------------------------------------------------------------

		int GetFixupID() const {

			return m_ChunkFixupID;

		}

		void WriteFixupValue( const FIXUP_SIZE_TYPE type, const int fixupID ) {

			// Append the fixup to the fixup list

			NoteFixupValueForChunk( type, fixupID, GetCurrentDataOffset() );

			// Reserve the space in the data for this chunk

			if ( WORD_FIXUP == type ) {

				WORD value = 0;
				*this << value;

			} else if ( DWORD_FIXUP == type ) {

				DWORD value = 0;
				*this << value;

			}

		}

		int DefineFixup( const FIXUP_TYPE type, const int ida = 0, const int idb = 0) {


// NEEDS WORK!
// NEEDS WORK!
// NEEDS WORK!
// NEEDS WORK!

			// Get a fixup id for this one
			// ----------------------------------------------------------------

			int fixupID = GetNextFixupID();

			// Build and add the fixup to the fixup's to be built collection
			// ----------------------------------------------------------------

			if ( CHUNK_RELATIVE == type ) {

				if ( ida && idb ) {

					// Chunk to chunk relative

				} else {

					// This chunk relative

					int dataOffset = GetCurrentDataOffset();

				}

			}

// NEEDS WORK!
// NEEDS WORK!
// NEEDS WORK!
// NEEDS WORK!

			return fixupID;

		}

		// Quick access helper builder
		// --------------------------------------------------------------------

		class QuickChunkInfo {

		private:

			pointer m_pQuickChunk;
			pointer m_pIndexChunk;
			pointer m_pOwnerChunk;

		public:

			// Creation
			// ----------------------------------------------------------------

			QuickChunkInfo() : m_pQuickChunk( 0 ), m_pIndexChunk( 0 ), m_pOwnerChunk( 0 )
				{ /* Empty */ }

			~QuickChunkInfo() {

				Release();

			}

			// Chunk management
			// ----------------------------------------------------------------

			void Release() {

				m_pQuickChunk = 0;
				m_pIndexChunk = 0;
				m_pOwnerChunk = 0;

			}

			bool Build( pointer pOwner ) {

				Release();

				m_pOwnerChunk = pOwner;

				// Create the root block
				// -------------------------------------------------------------

				m_pQuickChunk = pOwner->CreateChild( STANDARD_CHUNKS::QUICK_ROOT_CHUNK() );

				if ( !m_pQuickChunk ) {

					Release();
					return false;

				}
	
				// Create the index block
				// -------------------------------------------------------------
	
				m_pIndexChunk = m_pQuickChunk->CreateChild( STANDARD_CHUNKS::QUICK_INDEX_CHUNK() );
	
				if ( !m_pIndexChunk ) {

					Release();
					return false;

				}

				return true;
	
			}

			// Create a managed chunk
			// ----------------------------------------------------------------

			pointer CreateChild( const CHUNKID & id ) {

				pointer pChunk = pQuickChunk->CreateChild( id );

				if ( pChunk ) {

					// Create a relative fixup value

					fixupID = m_pOwnerChunk->DefineFixup(
						CHUNK_RELATIVE, pQuickChunk->GetFixupID(), pChunk->GetFixupID()
					);

					// write out the fixup into the index table
				
					m_pIndexChunk->WriteFixupValue( DWORD_FIXUP, fixupID );

				}

				return pChunk;

			}

		};

		QuickChunkInfo *
		CreateQuickAcessChunk() {

			QuickChunkInfo * pInfo = new QuickChunkInfo;

			if ( !pInfo ) {

				return static_cast<QuickChunkInfo *>( 0 );

			}

			if ( !pInfo->Build( this ) ) {

				delete pInfo;
				return static_cast<QuickChunkInfo *>( 0 );

			}

			return info;

		}

	};

}; // namespace BPT

// ----------------------------------------------------------------------------

#endif // !defined(AFX_TSTRUCTUREDDATA_H__A95A4664_6C6D_4489_80CC_B3826DDD0299__INCLUDED_)
