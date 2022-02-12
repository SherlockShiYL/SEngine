#include "QuadtreeScene.h"

using namespace S;

void QuadtreeScene::Load()
{
	int amount = 2000;
	input = Input::InputSystem::Get();
	redArrow.reserve(amount);

	aiWorld.GetQuadrant().Initialize({ 1280.0f,720.0f }, 15u, 20u);
	mQuadtree.Initialize({ 0.0f,0.0f,1280.0f,720.0f }, 4u);
	for (int i = 0; i < amount; ++i)
	{
		RedArrow* newArrow = new RedArrow(aiWorld);
		redArrow.push_back(newArrow);
		redArrow[i]->Load();
		mQuadtree.Insert(redArrow[i]);
	}
	aiWorld.AddObstacles(Geometry::Circle{ 300.0f,300.0f,100.0f });
	aiWorld.AddObstacles(Geometry::Circle{ 600.0f,300.0f,100.0f });
	aiWorld.AddObstacles(Geometry::Circle{ 450.0f,500.0f,50.0f });
	aiWorld.AddObstacles(Geometry::Circle{ 800.0f,500.0f,100.0f });

	SetMouseArea();
	SetMode(3);
	SetMode(4);
	/*mPoints.reserve(1000);
	for (uint32_t i = 0; i < 1000; i++)
	{
		mPoints.emplace_back(Geometry::Circle{ {Math::RandomFloat(0.0f,1280.0f),Math::RandomFloat(0.0f,720.0f) },4.0f });
	}
	for (uint32_t i = 0; i < mPoints.size(); i++)
	{
		mQuadtree.Insert(mPoints[i]);
	}*/

}

void QuadtreeScene::Unload()
{
}

Transition QuadtreeScene::Update(float deltaTime)
{
	SetMouseArea();

	++mRunCount;
	std::chrono::steady_clock::time_point begin;
	std::chrono::steady_clock::time_point end;

	for (auto& arrow : redArrow)
	{
		arrow->Update(deltaTime, { (float)input->GetMouseScreenX(),(float)input->GetMouseScreenY() }, true);
	}

	rectCollidedList.clear();

	mQuadtree.QueryRange(rectCollidedList, mMouseRect);

	begin = std::chrono::steady_clock::now();

	circleCollidedList.clear();
	mQuadtree.Update();
	mQuadtree.QueryRange(circleCollidedList, mMouseCircle);

	end = std::chrono::steady_clock::now();
	mQuadtreeTotalTimeCost += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
	mQuadtreeAverageTimeCost = mQuadtreeTotalTimeCost / (float)mRunCount;

	begin = std::chrono::steady_clock::now();

	quadrantCollidedList.clear();
	quadrantCollidedList = aiWorld.GetNeighborhoodQuadrant(mMouseCircle);
	aiWorld.Update();

	end = std::chrono::steady_clock::now();
	mQuadrantTotalTimeCost += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
	mQuadrantAverageTimeCost = mQuadrantTotalTimeCost / (float)mRunCount;



	// ImGui
	if (ImGui::Button("Seek Behavior"))
	{
		SetMode(1);
	}
	if (ImGui::Button("Arrive Behavior"))
	{
		SetMode(2);
	}
	if (ImGui::Button("Wander Behavior"))
	{
		SetMode(3);
	}
	if (ImGui::Button("ObsAvo Behavior"))
	{
		SetMode(4);
	}
	if (ImGui::Button("Flocking Behavior"))
	{
		SetMode(5);
	}


	// State
	Transition nextState = Transition::None;
	if (ImGui::Button("Return"))
	{
		nextState = Transition::GoToFrontend;
	}
	return nextState;
}


void QuadtreeScene::Render()
{
	ImGui::Text("Count: %d", mRunCount);
	ImGui::Text("Quadtree Average: %f", mQuadtreeAverageTimeCost);
	ImGui::Text("Quadrant Average: %f", mQuadrantAverageTimeCost);
	ImGui::Text("Quadtree Total Cost: %f", mQuadtreeTotalTimeCost);
	ImGui::Text("Quadrant Total Cost: %f", mQuadrantTotalTimeCost);

	for (auto& arrow : redArrow)
	{
		arrow->Render();
	}
	//aiWorld.GetQuadrant().DebugRender();
	mQuadtree.DebugRender();

	Graphics::DrawScreenRect(mMouseRect, Math::Vector4::Cyan());
	Graphics::DrawScreenCircle(mMouseCircle, Math::Vector4::Green());

	for (uint32_t i = 0; i < rectCollidedList.size(); i++)
	{
		Graphics::DrawScreenRect({ rectCollidedList[i]->Position().x - 8.0f, rectCollidedList[i]->Position().y - 8.0f ,rectCollidedList[i]->Position().x + 8.0f ,rectCollidedList[i]->Position().y + 8.0f }, Math::Vector4::Cyan());
	}
	for (uint32_t i = 0; i < circleCollidedList.size(); i++)
	{
		Graphics::DrawScreenCircle({ circleCollidedList[i]->Position(),6.0f }, Math::Vector4::Green());
	}
	for (size_t i = 0; i < quadrantCollidedList.size(); i++)
	{
		Graphics::DrawScreenCircle({ quadrantCollidedList[i]->Position(),10.0f }, Math::Vector4::Yellow());
	}
}

void QuadtreeScene::ResetCount()
{
	mRunCount = 0;
	mQuadtreeTotalTimeCost = 0.0f;
	mQuadtreeAverageTimeCost = 0.0f;
	mQuadrantTotalTimeCost = 0.0f;
	mQuadrantAverageTimeCost = 0.0f;
}

void QuadtreeScene::SetMode(int index)
{
	for (auto& arrow : redArrow)
	{
		arrow->ChangeBehavior(index);
	}
	ResetCount();
}

void QuadtreeScene::SetMouseArea()
{
	mMouseRect = { (float)input->GetMouseScreenX() - 100.0f,(float)input->GetMouseScreenY() - 100.0f,(float)input->GetMouseScreenX() + 100.0f,(float)input->GetMouseScreenY() + 100.0f };
	mMouseCircle = { {(float)input->GetMouseScreenX(),(float)input->GetMouseScreenY()},200.0f };
}
