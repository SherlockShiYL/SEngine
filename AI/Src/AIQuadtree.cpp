#include "Precompiled.h"
#include "AIQuadtree.h"

using namespace S;
using namespace AI;

void AIQuadtree::Initialize(Geometry::Rect rect, uint32_t capacity)
{
	mRect = rect;
	mCapacity = capacity;
}

void AIQuadtree::RegisterAgent(Agent* agent)
{
	Insert(agent);
	//mAgents.emplace_back(agent);
}

void AIQuadtree::UnregisterAgent(Agent* agent)
{
	// This might be slow
	auto iter = std::find(mAgents.begin(), mAgents.end(), agent);
	if (iter != mAgents.end())
	{
		mAgents.erase(iter);
	}
}

void AIQuadtree::RegisterEntity(Entity* entity)
{
	Insert(entity);
	//mEntities.emplace_back(entity);
}

void AIQuadtree::UnregisterEntity(Entity* entity)
{
	// This might be slow
	auto iter = std::find(mEntities.begin(), mEntities.end(), entity);
	if (iter != mEntities.end())
	{
		mEntities.erase(iter);
	}
}

void AIQuadtree::Subdivide()
{
	float halfWidth = (mRect.right - mRect.left) / 2 + mRect.left;
	float halfHeight = (mRect.bottom - mRect.top) / 2 + mRect.top;

	mTL = new AIQuadtree();
	mTL->Initialize({ mRect.left, mRect.top, halfWidth, halfHeight }, mCapacity);
	mTR = new AIQuadtree();
	mTR->Initialize({ halfWidth, mRect.top, mRect.right, halfHeight }, mCapacity);
	mBL = new AIQuadtree();
	mBL->Initialize({ mRect.left, halfHeight, halfWidth, mRect.bottom }, mCapacity);
	mBR = new AIQuadtree();
	mBR->Initialize({ halfWidth, halfHeight, mRect.right, mRect.bottom }, mCapacity);
}

bool AIQuadtree::Insert(Agent* agent)
{
	if (!S::Geometry::PointInRect(agent->Position(), mRect))
		return false;

	if (mAgents.size() < mCapacity)
	{
		mAgents.emplace_back(agent);
		return true;
	}
	else
	{
		if (!mTL)
		{
			Subdivide();
		}
		if (mTL->Insert(agent))
		{
			return true;
		}
		else if (mTR->Insert(agent))
		{
			return true;
		}
		else if (mBL->Insert(agent))
		{
			return true;
		}
		else if (mBR->Insert(agent))
		{
			return true;
		}
		return false;
	}
}

bool S::AI::AIQuadtree::Insert(Entity* entity)
{
	if (!S::Geometry::PointInRect(entity->Position(), mRect))
		return false;

	if (mEntities.size() < mCapacity)
	{
		mEntities.emplace_back(entity);
		return true;
	}
	else
	{
		if (!mTL)
		{
			Subdivide();
		}
		if (mTL->Insert(entity))
		{
			return true;
		}
		else if (mTR->Insert(entity))
		{
			return true;
		}
		else if (mBL->Insert(entity))
		{
			return true;
		}
		else if (mBR->Insert(entity))
		{
			return true;
		}
		return false;
	}
}

void AIQuadtree::QueryRange(AgentList& result, Geometry::Rect rect)
{
	if (!Geometry::Intersect(mRect, rect))
		return;

	for (uint32_t i = 0; i < mAgents.size(); i++)
	{
		if (Geometry::PointInRect(mAgents[i]->Position(), rect))
		{
			result.emplace_back(mAgents[i]);
		}
	}
	if (mTL)
	{
		mTL->QueryRange(result, rect);
		mTR->QueryRange(result, rect);
		mBL->QueryRange(result, rect);
		mBR->QueryRange(result, rect);
	}
}

void AIQuadtree::QueryRange(AgentList& result, Geometry::Circle circle)
{
	if (!Geometry::Intersect(mRect, circle))
		return;

	for (uint32_t i = 0; i < mAgents.size(); i++)
	{
		if (Geometry::PointInCircle(mAgents[i]->Position(), circle))
		{
			result.emplace_back(mAgents[i]);
		}
	}
	if (mTL)
	{
		mTL->QueryRange(result, circle);
		mTR->QueryRange(result, circle);
		mBL->QueryRange(result, circle);
		mBR->QueryRange(result, circle);
	}
}

void AIQuadtree::QueryRange(EntityList& result, Geometry::Rect rect)
{
	if (!Geometry::Intersect(mRect, rect))
		return;

	for (uint32_t i = 0; i < mEntities.size(); i++)
	{
		if (Geometry::PointInRect(mEntities[i]->Position(), rect))
		{
			result.emplace_back(mEntities[i]);
		}
	}
	if (mTL)
	{
		mTL->QueryRange(result, rect);
		mTR->QueryRange(result, rect);
		mBL->QueryRange(result, rect);
		mBR->QueryRange(result, rect);
	}
}

void AIQuadtree::QueryRange(EntityList& result, Geometry::Circle circle)
{
	if (!Geometry::Intersect(mRect, circle))
		return;

	for (uint32_t i = 0; i < mEntities.size(); i++)
	{
		if (Geometry::PointInCircle(mEntities[i]->Position(), circle))
		{
			result.emplace_back(mEntities[i]);
		}
	}
	if (mTL)
	{
		mTL->QueryRange(result, circle);
		mTR->QueryRange(result, circle);
		mBL->QueryRange(result, circle);
		mBR->QueryRange(result, circle);
	}
}

bool AIQuadtree::Empty()
{
	return mAgents.empty() && mEntities.empty() && !mTL;
}

void AIQuadtree::CheckValid(AgentList& result)
{
	for (size_t i = 0; i < mAgents.size(); i++)
	{
		if (!Geometry::PointInRect(mAgents[i]->Position(), mRect))
		{
			result.push_back(mAgents[i]);
			if (mAgents[i] != mAgents[mAgents.size() - 1])
				std::swap(mAgents[i], mAgents[mAgents.size() - 1]);
			mAgents.pop_back();
		}
	}

	if (mTL)
	{
		mTL->CheckValid(result);
		mTR->CheckValid(result);
		mBL->CheckValid(result);
		mBR->CheckValid(result);

		if (mTL->Empty() && mTR->Empty() && mBL->Empty() && mBR->Empty())
		{
			SafeDelete(mTL);
			SafeDelete(mTR);
			SafeDelete(mBL);
			SafeDelete(mBR);
		}
	}
}

void AIQuadtree::CheckValid(EntityList& result)
{
	for (size_t i = 0; i < mEntities.size(); i++)
	{
		if (!Geometry::PointInRect(mEntities[i]->Position(), mRect))
		{
			result.push_back(mEntities[i]);
			if (mEntities[i] != mEntities[mEntities.size() - 1])
				std::swap(mEntities[i], mEntities[mEntities.size() - 1]);
			mEntities.pop_back();
		}
	}

	if (mTL)
	{
		mTL->CheckValid(result);
		mTR->CheckValid(result);
		mBL->CheckValid(result);
		mBR->CheckValid(result);

		if (mTL->Empty() && mTR->Empty() && mBL->Empty() && mBR->Empty())
		{
			SafeDelete(mTL);
			SafeDelete(mTR);
			SafeDelete(mBL);
			SafeDelete(mBR);
		}
	}
}

void AIQuadtree::Update()
{
	AgentList tempAgentList;
	CheckValid(tempAgentList);
	for (size_t i = 0; i < tempAgentList.size(); i++)
	{
		Insert(tempAgentList[i]);
	}

	EntityList tempEntityList;
	CheckValid(tempEntityList);
	for (size_t i = 0; i < tempEntityList.size(); i++)
	{
		Insert(tempEntityList[i]);
	}
}

void AIQuadtree::DebugRender(Math::Vector2 p)
{
	if (mTL)
	{
		float halfWidth = (mRect.right - mRect.left) / 2 + mRect.left;
		float halfHeight = (mRect.bottom - mRect.top) / 2 + mRect.top;
		Graphics::DrawScreenLine(halfWidth, mRect.top, halfWidth, mRect.bottom, Math::Vector4::Gray());
		Graphics::DrawScreenLine(mRect.left, halfHeight, mRect.right, halfHeight, Math::Vector4::Gray());
		mTR->DebugRender();
		mTL->DebugRender();
		mBR->DebugRender();
		mBL->DebugRender();
	}
}
