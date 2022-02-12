#include "Precompiled.h"
#include "AIQuadrant.h"
#include "AIWorld.h"

using namespace S;
using namespace AI;

AIQuadrant::AIQuadrant()
	: recipWidth{ 0.0f }, recipHeight{ 0.0f }, mWidth{ 0.0f }, mHeight{ 0.0f }, mRows{ 1u }, mColumns{ 1u }
{}

void AIQuadrant::RegisterAgent(Agent* agent)
{
	if (agent->Position().x >= 0.0f && agent->Position().x <= static_cast<float>(mColumns)*mWidth
		&& agent->Position().y >= 0.0f && agent->Position().y <= static_cast<float>(mRows)*mHeight)
	{
		mAgentLists[static_cast<int>(agent->Position().x*recipWidth) + (size_t)mRows * static_cast<int>(agent->Position().y*recipHeight)].push_back(agent);
	}
	else
	{
		mAgentLists[mAgentLists.size() - 1].push_back(agent);
	}
}

void AIQuadrant::UnregisterAgent(Agent* agent)
{
	if (agent->Position().x >= 0.0f && agent->Position().x <= static_cast<float>(mColumns)*mWidth
		&& agent->Position().y >= 0.0f && agent->Position().y <= static_cast<float>(mRows)*mHeight)
	{
		int index = static_cast<int>(agent->Position().x*recipWidth) + mRows * static_cast<int>(agent->Position().y*recipHeight);
		auto iter = std::find(mAgentLists[index].begin(), mAgentLists[index].end(), agent);
		if (iter != mAgentLists[index].end())
		{
			mAgentLists[index].erase(iter);
		}
	}
	else
	{
		size_t index = mAgentLists.size() - 1;
		auto iter = std::find(mAgentLists[index].begin(), mAgentLists[index].end(), agent);
		if (iter != mAgentLists[index].end())
		{
			mAgentLists[index].erase(iter);
		}
	}
}

void AIQuadrant::RegisterEntity(Entity * entity)
{
	if (entity->Position().x >= 0.0f && entity->Position().x <= static_cast<float>(mColumns)*mWidth
		&& entity->Position().y >= 0.0f && entity->Position().y <= static_cast<float>(mRows)*mHeight)
	{
		mEntityLists[(size_t)(entity->Position().x*recipWidth) + (size_t)mRows * (size_t)(entity->Position().y*recipHeight)].push_back(entity);
	}
	else
	{
		mEntityLists[mEntityLists.size() - 1].push_back(entity);
	}
}

void AIQuadrant::UnregisterEntity(Entity * entity)
{
	if (entity->Position().x >= 0.0f && entity->Position().x <= static_cast<float>(mColumns)*mWidth
		&& entity->Position().y >= 0.0f && entity->Position().y <= static_cast<float>(mRows)*mHeight)
	{
		int index = static_cast<int>(entity->Position().x*recipWidth) + mRows * static_cast<int>(entity->Position().y*recipHeight);
		auto iter = std::find(mEntityLists[index].begin(), mEntityLists[index].end(), entity);
		if (iter != mEntityLists[index].end())
		{
			mEntityLists[index].erase(iter);
		}
	}
	else
	{
		size_t index = mEntityLists.size() - 1u;
		auto iter = std::find(mEntityLists[index].begin(), mEntityLists[index].end(), entity);
		if (iter != mEntityLists[index].end())
		{
			mEntityLists[index].erase(iter);
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

	mAgentLists.resize((size_t)mRows*mColumns + 1);
	mEntityLists.resize((size_t)mRows*mColumns + 1);
}

void AIQuadrant::Initialize(float width, float height, uint32_t columns, uint32_t rows)
{
	mWidth = width;
	mHeight = height;
	mColumns = columns;
	mRows = rows;

	recipWidth = 1.0f / mWidth;
	recipHeight = 1.0f / mHeight;

	mAgentLists.resize((size_t)mRows*mColumns + 1);
	mEntityLists.resize((size_t)mRows*mColumns + 1);
}

void AIQuadrant::Update()
{
	for (uint32_t i = 0; i < mAgentLists.size() - 1; ++i)
	{
		for (uint32_t j = 0; j < mAgentLists[i].size(); ++j)
		{
			if (mAgentLists[i][j]->Position().x >= 0.0f && mAgentLists[i][j]->Position().x <= static_cast<float>(mColumns)*mWidth
				&& mAgentLists[i][j]->Position().y >= 0.0f && mAgentLists[i][j]->Position().y <= static_cast<float>(mRows)*mHeight)
			{
				int index = static_cast<int>(mAgentLists[i][j]->Position().x*recipWidth) + mColumns * static_cast<int>(mAgentLists[i][j]->Position().y*recipHeight);

				if (index != i)
				{
					mAgentLists[index].push_back(mAgentLists[i][j]);
					std::swap(mAgentLists[i][j], mAgentLists[i][mAgentLists[i].size() - 1]);
					mAgentLists[i].pop_back();
					--j;
				}
			}
			else
			{
				mAgentLists[mAgentLists.size() - 1].push_back(mAgentLists[i][j]);
				std::swap(mAgentLists[i][j], mAgentLists[i][mAgentLists[i].size() - 1]);
				mAgentLists[i].pop_back();
				--j;
			}
		}
	}

	size_t size = mAgentLists.size() - 1u;
	for (uint32_t j = 0; j < mAgentLists[mAgentLists.size() - 1].size(); ++j)
	{
		if (mAgentLists[size][j]->Position().x >= 0.0f && mAgentLists[size][j]->Position().x <= static_cast<float>(mColumns)*mWidth
			&& mAgentLists[size][j]->Position().y >= 0.0f && mAgentLists[size][j]->Position().y <= static_cast<float>(mRows)*mHeight)
		{
			int index = static_cast<int>(mAgentLists[size][j]->Position().x*recipWidth) + mRows * static_cast<int>(mAgentLists[size][j]->Position().y*recipHeight);

			mAgentLists[index].push_back(mAgentLists[size][j]);
			std::swap(mAgentLists[size][j], mAgentLists[size][mAgentLists[size].size() - 1]);
			mAgentLists[size].pop_back();
			--j;
		}
	}
}

void AIQuadrant::DebugRender(Math::Vector2 p)
{
	for (size_t i = 0; i < (size_t)mRows; ++i)
	{
		for (size_t j = 0; j < (size_t)mColumns; ++j)
		{
			Graphics::DrawScreenRect(Geometry::Rect{ mWidth*(float)j,(float)i*mHeight,mWidth*(float)(j + 1),mHeight*(float)(i + 1) }, Math::Vector4::Gray());
			char frames[6];
			_itoa_s(static_cast<int>(mAgentLists[j + i * mColumns].size()), frames, 10);
			Graphics::DrawScreenText(frames, mWidth*(float)j, (float)i*mHeight, 24.0f, Math::Vector4::Gray());
		}
	}
}

