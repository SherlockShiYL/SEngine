#include "Precompiled.h"
#include "AIQuadrant.h"
#include "AIWorld.h"

using namespace S;
using namespace AI;

AIQuadrant::AIQuadrant()
	: mWidth{ 0.0f }, mHeight{ 0.0f }, mRows{ 1u }, mColumns{ 1u }
{}

void AIQuadrant::RegisterAgent(Agent* agent)
{
	if (agent->Position().x >= 0.0f && agent->Position().x <= static_cast<float>(mColumns)*mWidth
		&& agent->Position().y >= 0.0f && agent->Position().y <= static_cast<float>(mRows)*mHeight)
	{
		mAgents[static_cast<int>(agent->Position().x*recipWidth) + mRows * static_cast<int>(agent->Position().y*recipHeight)].push_back(agent);
	}
	else
	{
		mAgents[mAgents.size() - 1].push_back(agent);
	}
}

void AIQuadrant::UnregisterAgent(Agent* agent)
{
	if (agent->Position().x >= 0.0f && agent->Position().x <= static_cast<float>(mColumns)*mWidth
		&& agent->Position().y >= 0.0f && agent->Position().y <= static_cast<float>(mRows)*mHeight)
	{
		int index = static_cast<int>(agent->Position().x*recipWidth) + mRows * static_cast<int>(agent->Position().y*recipHeight);
		auto iter = std::find(mAgents[index].begin(), mAgents[index].end(), agent);
		if (iter != mAgents[index].end())
		{
			mAgents[index].erase(iter);
		}
	}
	else
	{
		size_t index = mAgents.size() - 1;
		auto iter = std::find(mAgents[index].begin(), mAgents[index].end(), agent);
		if (iter != mAgents[index].end())
		{
			mAgents[index].erase(iter);
		}
	}
}

void AIQuadrant::RegisterEntity(Entity * entity)
{
	if (entity->Position().x >= 0.0f && entity->Position().x <= static_cast<float>(mColumns)*mWidth
		&& entity->Position().y >= 0.0f && entity->Position().y <= static_cast<float>(mRows)*mHeight)
	{
		mEntities[static_cast<int>(entity->Position().x*recipWidth) + mRows * static_cast<int>(entity->Position().y*recipHeight)].push_back(entity);
	}
	else
	{
		mEntities[mEntities.size() - 1].push_back(entity);
	}
}

void AIQuadrant::UnregisterEntity(Entity * entity)
{
	if (entity->Position().x >= 0.0f && entity->Position().x <= static_cast<float>(mColumns)*mWidth
		&& entity->Position().y >= 0.0f && entity->Position().y <= static_cast<float>(mRows)*mHeight)
	{
		int index = static_cast<int>(entity->Position().x*recipWidth) + mRows * static_cast<int>(entity->Position().y*recipHeight);
		auto iter = std::find(mEntities[index].begin(), mEntities[index].end(), entity);
		if (iter != mEntities[index].end())
		{
			mEntities[index].erase(iter);
		}
	}
	else
	{
		size_t index = mEntities.size() - 1u;
		auto iter = std::find(mEntities[index].begin(), mEntities[index].end(), entity);
		if (iter != mEntities[index].end())
		{
			mEntities[index].erase(iter);
		}
	}
}

void AIQuadrant::Initialize(Math::Vector2 area, uint32_t rows, uint32_t columns)
{
	ASSERT(area.x >= 0, "The size of map should not be negative.");
	ASSERT(area.y >= 0, "The size of map should not be negative.");

	mRows = rows;
	mColumns = columns;
	mWidth = area.x / columns;
	mHeight = area.y / rows;

	recipWidth = 1.0f / mWidth;
	recipHeight = 1.0f / mHeight;

	mAgents.resize(mRows*mColumns + 1);
	mEntities.resize(mRows*mColumns + 1);
}

void AIQuadrant::Initialize(float width, float height, uint32_t columns, uint32_t rows)
{
	mWidth = width;
	mHeight = height;
	mColumns = columns;
	mRows = rows;

	recipWidth = 1.0f / mWidth;
	recipHeight = 1.0f / mHeight;

	mAgents.resize(mRows*mColumns + 1);
	mEntities.resize(mRows*mColumns + 1);
}

void AIQuadrant::Update()
{
	for (uint32_t i = 0; i < mAgents.size() - 1; ++i)
	{
		for (uint32_t j = 0; j < mAgents[i].size(); ++j)
		{
			if (mAgents[i][j]->Position().x >= 0.0f && mAgents[i][j]->Position().x <= static_cast<float>(mColumns)*mWidth
				&& mAgents[i][j]->Position().y >= 0.0f && mAgents[i][j]->Position().y <= static_cast<float>(mRows)*mHeight)
			{
				int index = static_cast<int>(mAgents[i][j]->Position().x*recipWidth) + mColumns * static_cast<int>(mAgents[i][j]->Position().y*recipHeight);

				if (index != i)
				{
					mAgents[index].push_back(mAgents[i][j]);
					std::swap(mAgents[i][j], mAgents[i][mAgents[i].size() - 1]);
					mAgents[i].pop_back();
					--j;
				}
			}
			else
			{
				mAgents[mAgents.size() - 1].push_back(mAgents[i][j]);
				std::swap(mAgents[i][j], mAgents[i][mAgents[i].size() - 1]);
				mAgents[i].pop_back();
				--j;
			}
		}
	}

	size_t size = mAgents.size() - 1u;
	for (uint32_t j = 0; j < mAgents[mAgents.size() - 1].size(); ++j)
	{
		if (mAgents[size][j]->Position().x >= 0.0f && mAgents[size][j]->Position().x <= static_cast<float>(mColumns)*mWidth
			&& mAgents[size][j]->Position().y >= 0.0f && mAgents[size][j]->Position().y <= static_cast<float>(mRows)*mHeight)
		{
			int index = static_cast<int>(mAgents[size][j]->Position().x*recipWidth) + mRows * static_cast<int>(mAgents[size][j]->Position().y*recipHeight);

			mAgents[index].push_back(mAgents[size][j]);
			std::swap(mAgents[size][j], mAgents[size][mAgents[size].size() - 1]);
			mAgents[size].pop_back();
			--j;
		}
	}
}

void AIQuadrant::DebugRender(Math::Vector2 p)
{
	for (uint32_t i = 0; i < mRows; ++i)
	{
		for (uint32_t j = 0; j < mColumns; ++j)
		{
			Graphics::DrawScreenRect(Geometry::Rect{ mWidth*(float)j,(float)i*mHeight,mWidth*(float)(j + 1),mHeight*(float)(i + 1) }, Math::Vector4::White());
			char frames[6];
			_itoa_s(static_cast<int>(mAgents[j + i * mColumns].size()), frames, 10);
			Graphics::DrawScreenText(frames, mWidth*(float)j, (float)i*mHeight, 24.0f, Math::Vector4::White());
		}
	}
}

