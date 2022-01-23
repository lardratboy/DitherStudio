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

#if !defined(SIMPLEENTITYSYSTEM_H__4EE3AB65_FB91_4E19_9877_81FB6B6BDBFD__INCLUDED_)
#define SIMPLEENTITYSYSTEM_H__4EE3AB65_FB91_4E19_9877_81FB6B6BDBFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <vector>
#include <typeinfo>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

namespace BPT {

	// ----------------------------------------------------------------------------

	typedef boost::shared_ptr< struct SimpleEntity > SimpleEntity_Ref;
	typedef boost::shared_ptr< struct onEnterFrame_i > onEnterFrame_Ref;

	// ---------------------------------------------------------------------------

#define BPT_ASSERT(x) ASSERT(x)

	template<typename T>
	struct DBUG_Traits
	{
		typedef uint32_t storage_type;

		static uint32_t getID()
		{
			static uint32_t nextID = 0;

			return ++nextID;
		}
	};

	template<typename T,typename Traits = DBUG_Traits<T> >
	struct DBUG
	{
		typedef Traits traits;
		typedef T type;

		typename traits::storage_type id;

		DBUG() : id(Traits::getID())
		{
		}

		static FILE * console()
		{
			return stdout;
		}

		void trace_(const char* format, va_list args)
		{
#if 1
			char szDbugLine[1024];
			sprintf(szDbugLine, "%s[%08x] - ", typeid(T).name(), id);
			OutputDebugString(szDbugLine);
			vsprintf(szDbugLine, format, args);
			OutputDebugString(szDbugLine);
#else
			fprintf(console(), "%s[%08x] - ", typeid(T).name(),id);
			vfprintf(console(), format, args);
#endif
		}

		void trace(const char* format, ...)
		{
			va_list args;
			va_start(args, format);
			trace_(format, args);
			va_end(args);
		}

		void warning(const char* format, ...)
		{
			va_list args;
			va_start(args, format);
			trace_(format, args);
			va_end(args);
		}

		void error(const char* format, ...)
		{
			va_list args;
			va_start(args, format);
			trace_(format, args);
			va_end(args);
		}

	};

	// ---------------------------------------------------------------------------

	struct onEnterFrame_i : public boost::enable_shared_from_this<onEnterFrame_i>
	{
		DBUG<onEnterFrame_i> dbug;
		virtual void operator()(SimpleEntity_Ref mc) = 0;
	};

	// ---------------------------------------------------------------------------

	struct TimeData
	{
		float scale;
		float base;

		TimeData() : scale(1.0f), base(0.0f)
		{
		}
	};

	struct PositionData
	{
		vec3 position;

		PositionData() : position(0.0f)
		{
		}
	};

	struct TransformData
	{
		mat4 combinedTransform;
		bool identity;

		TransformData() : identity(true)
		{
			combinedTransform = glm::identity<mat4>();
		}

		bool IsIdentity() const
		{
			return identity;
		}

	};

	// ---------------------------------------------------------------------------

	struct EntityManager
	{
		static std::vector<SimpleEntity_Ref> detached_;
		static void RegisterDetachment(SimpleEntity_Ref entity);
		static void ProcessDetachments();

		template<typename T=SimpleEntity>
		static SimpleEntity_Ref Create()
		{
			return SimpleEntity_Ref(new T());
		}
	};

	// ---------------------------------------------------------------------------

	struct SimpleEntity : public boost::enable_shared_from_this<SimpleEntity>
	{

		friend struct EntityManager;

	public: // yeah it's gross but it's temporary 

		DBUG<SimpleEntity> dbug;

		typedef std::list<onEnterFrame_Ref> enter_frame_collection_t;

		typedef std::map<int, SimpleEntity_Ref> children_t;

		enter_frame_collection_t onEnterFrame_;

		SimpleEntity_Ref parent_;

		children_t children_;

		TransformData transform_;

		PositionData position_;

		TimeData time_;

	private:

		SimpleEntity(const SimpleEntity&);
		SimpleEntity& operator=(const SimpleEntity&);

	public:

		SimpleEntity();

		virtual ~SimpleEntity();

		virtual void EnterFrame();

		virtual float Time();

	public:

		void Detach();

		void Attach(SimpleEntity_Ref child);

		void Attach(SimpleEntity_Ref child, int depth);

		int GetNextHighestDepth();

		int GetDepthForChild(SimpleEntity_Ref child);

		int GetDepth();

		vec3 GlobalToLocal(vec3 global);

		vec3 LocalToGlobal(vec3 local);

		mat4& GetConcatenatedMatrix(mat4& m);

		mat4& GetLocalTransformMatrix(mat4& m);

	};

} // namespace BPT

#endif // SIMPLEENTITYSYSTEM_H__4EE3AB65_FB91_4E19_9877_81FB6B6BDBFD__INCLUDED_

