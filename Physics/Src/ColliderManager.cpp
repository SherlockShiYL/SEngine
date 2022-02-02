#include "Precompiled.h"
#include "ColliderManager.h"

#include "CircleCollider.h"

using namespace S;
using namespace S::Physics;

namespace
{
	ColliderManager* sColliderManager = nullptr;
}

void ColliderManager::StaticInitialize()
{
	ASSERT(sColliderManager == nullptr, "[ColliderManager] Manager already initialized!");
	sColliderManager = new ColliderManager();
}

void ColliderManager::StaticTerminate()
{
	if (sColliderManager != nullptr)
	{
		SafeDelete(sColliderManager);
	}
}

ColliderManager* ColliderManager::Get()
{
	ASSERT(sColliderManager != nullptr, "[ColliderManager] No instance registered.");
	return sColliderManager;
}

void ColliderManager::AddCollider(CircleCollider cCollider, uint32_t type)
{
	for (uint32_t i = 0; i < mCircleBounds.size(); ++i)
	{
		if (mCircleBounds[i].first == type)
		{
			mCircleBounds[i].second.push_back(cCollider);
			return;
		}
	}

	std::vector<CircleCollider> temp;
	temp.push_back(cCollider);
	mCircleBounds.push_back({ type,temp });
}

void ColliderManager::Update()
{
	for (uint32_t i = 0; i < mCircleBounds.size(); ++i)
	{
		for (uint32_t j = 0; j < mCircleBounds[i].second.size(); ++j)
		{
			mCircleBounds[i].second[j].Update();
		}
	}
}


void ColliderManager::CheckCollision()
{
	for (uint32_t k = 0; k < mCircleBounds.size(); ++k)
	{
		for (uint32_t i = 0; i + 1 < mCircleBounds[k].second.size(); ++i)
		{
			for (uint32_t j = i + 1; j < mCircleBounds[k].second.size(); ++j)
			{
				if (Geometry::Intersect(mCircleBounds[k].second[i].GetCircle(), mCircleBounds[k].second[j].GetCircle()))
				{
					if (mCircleBounds[k].second[i].CheckPositionChanged())
					{
						Math::Vector2 dir = { mCircleBounds[k].second[j].mPosition->x - mCircleBounds[k].second[i].mPosition->x
										,mCircleBounds[k].second[j].mPosition->y - mCircleBounds[k].second[i].mPosition->y };
						dir *= (mCircleBounds[k].second[j].mRadius + mCircleBounds[k].second[i].mRadius) / Math::Magnitude(dir);
						mCircleBounds[k].second[j].mPosition->x = mCircleBounds[k].second[i].mPosition->x + dir.x;
						mCircleBounds[k].second[j].mPosition->y = mCircleBounds[k].second[i].mPosition->y + dir.y;
					}
					else// if (mCircleBounds[k].second[j].CheckPositionChanged())
					{
						Math::Vector2 dir = { mCircleBounds[k].second[i].mPosition->x - mCircleBounds[k].second[j].mPosition->x 
										,mCircleBounds[k].second[i].mPosition->y - mCircleBounds[k].second[j].mPosition->y };
						dir *= (mCircleBounds[k].second[i].mRadius + mCircleBounds[k].second[j].mRadius) / Math::Magnitude(dir);
						mCircleBounds[k].second[i].mPosition->x = mCircleBounds[k].second[j].mPosition->x + dir.x;
						mCircleBounds[k].second[i].mPosition->y = mCircleBounds[k].second[j].mPosition->y + dir.y;
					}

				}
			}
		}
	}
}

void ColliderManager::Render(Math::Vector2 p, float scale)
{
	for (uint32_t i = 0; i < mCircleBounds.size(); ++i)
	{
		for (uint32_t j = 0; j < mCircleBounds[i].second.size(); ++j)
		{
			Graphics::DrawScreenCircle(mCircleBounds[i].second[j].GetCircle(p, scale), Math::Vector4::Blue());
		}
	}
}
