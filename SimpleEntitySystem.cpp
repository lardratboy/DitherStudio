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
#include "SimpleEntitySystem.h"

namespace BPT {

	// ------------------------------------------------------------------------

	std::vector<SimpleEntity_Ref> EntityManager::detached_;

	void EntityManager::RegisterDetachment(SimpleEntity_Ref entity)
	{
		detached_.push_back(entity);
	}

	void EntityManager::ProcessDetachments()
	{
		while (!detached_.empty())
		{
			SimpleEntity_Ref who = detached_.back();

			detached_.pop_back();

			if (!who->parent_)
			{
				for (auto it = who->children_.begin(); who->children_.end() != it; ++it)
				{
					it->second->Detach();
				}
			}
		}
	}

	// ------------------------------------------------------------------------

	SimpleEntity::SimpleEntity()
	{
	}

	SimpleEntity::~SimpleEntity()
	{
	}

	void SimpleEntity::EnterFrame()
	{
		BPT_ASSERT(parent_);

		SimpleEntity_Ref self = shared_from_this(); // (SimpleEntity_Ref)

		auto currentChildren = children_; // TODO actually make a copy on write logic somewhere

		for (auto child = currentChildren.begin(); currentChildren.end() != child; ++child)
		{
			child->second->EnterFrame();
		}

		if (parent_)
		{
			for (auto handler = onEnterFrame_.begin(); onEnterFrame_.end() != handler; ++handler)
			{
				(*(*handler))(self);
			}
		}
	}

	void SimpleEntity::Detach()
	{
		ASSERT(parent_);

		EntityManager::RegisterDetachment(shared_from_this());

		parent_ = SimpleEntity_Ref();
	}

	void SimpleEntity::Attach(SimpleEntity_Ref child, int depth)
	{
		children_t::iterator found = children_.find(depth);

		if (found->second == child) return /* NOP */;

		if (children_.end() != found)
		{
			//console() << "WARNING!!!!! clip " << found->second->_name << " already at depth " << depth << std::endl;

			found->second->Detach();
		}

		if (child->parent_)
		{
			child->Detach();
		}

		child->parent_ = shared_from_this();

		children_[depth] = child;
	}

	void SimpleEntity::Attach(SimpleEntity_Ref child)
	{
		Attach(child, GetNextHighestDepth());
	}

	int SimpleEntity::GetNextHighestDepth()
	{
		auto highest = children_.rbegin();

		if (children_.rend() != highest)
		{
			return highest->first + 1;
		}

		return 0;
	}

	int SimpleEntity::GetDepthForChild(SimpleEntity_Ref child)
	{
		for (auto it = children_.begin(); children_.end() != it; ++it)
		{
			if (child == it->second) return it->first;
		}

		return -1;
	}

	int SimpleEntity::GetDepth()
	{
		if (!parent_) return -1;

		SimpleEntity_Ref self = shared_from_this();

		return parent_->GetDepthForChild(self);
	}

	float SimpleEntity::Time()
	{
		if (!parent_) return time_.base;

		return parent_->Time() * time_.scale + time_.base;
	}

	mat4& SimpleEntity::GetConcatenatedMatrix(mat4& m)
	{
		// this probably should loop and not recurse!!!

		if (parent_)
		{
			mat4 p;

			parent_->GetConcatenatedMatrix(p);

			mat4 t;

			GetLocalTransformMatrix(t);

			return (m = p * t);
		}

		return GetLocalTransformMatrix(m);
	}

	mat4& SimpleEntity::GetLocalTransformMatrix(mat4& m)
	{
		return m = transform_.combinedTransform;
	}

	vec3 SimpleEntity::GlobalToLocal(vec3 global)
	{
		return global;
	}

	vec3 SimpleEntity::LocalToGlobal(vec3 local)
	{
		return local;
	}

	// ------------------------------------------------------------------------

	// ------------------------------------------------------------------------

} // namespace BPT
