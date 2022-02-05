#include "PathFinding.h"

using namespace S;

PathFindingScene::PathFindingScene()
{
}

PathFindingScene::~PathFindingScene()
{
	//input = nullptr;
}

void PathFindingScene::Load()
{
	mMode = 0;
	mTileIndex = 0;
	ResetCount();
	mTileMap.Load("../Assets/TileMaps/PathFindingMap.txt", "../Assets/TileMaps/PathFindingMap_Set.txt");
	mGraph.Init(mTileMap.GetMapColumns(), mTileMap.GetMapRows(), mTileMap.GetTileSize());

	for (int i = 0; i < mTileMap.GetMapColumns(); ++i)
	{
		for (int j = 0; j < mTileMap.GetMapRows(); ++j)
		{
			if (mTileMap.GetTile(i, j) == 1)
			{
				mGraph.SetBlock(i, j);
			}
		}
	}

	mStartPointTexture = Graphics::LoadTexture("AI_Generator_Images/Bot/zelda.png");
	mEndPointTexture = Graphics::LoadTexture("AI_Generator_Images/Box00.png");

	input = Input::InputSystem::Get();
}

void PathFindingScene::Unload()
{
	mGraph.Clear();
	mTileMap.Unload();
	input = nullptr;
}

Transition PathFindingScene::Update(float deltaTime)
{
	//ScaleUpdate(deltaTime);
	PlayerInput();

	auto GetDistance = [&](AI::Graph::Coord c1, AI::Graph::Coord c2)
	{
		float extraWeight = 0.0f;
		float diagonalExtra = mDiagonalExtra;
		if (mTileMap.GetTile(c2.x, c2.y) == 1)
		{
			extraWeight += 4.0f;
		}
		if (mTileMap.GetTile(c1.x, c1.y) == 1)
		{
			extraWeight += 4.0f;
		}
		if (c1.x == c2.x || c1.y == c2.y)
		{
			diagonalExtra = 0.0f;
		}
		return 1.0f + extraWeight + diagonalExtra;
	};

	auto JPSGetDistance = [](AI::Graph::Coord c1, AI::Graph::Coord c2)
	{
		return Math::Sqrt(Math::Sqr(static_cast<float>(c1.x) - static_cast<float>(c2.x)) + Math::Sqr(static_cast<float>(c1.y) - static_cast<float>(c2.y)));
	};

	auto GetStraightDistance = [&](AI::Graph::Coord c)
	{
		return Math::Sqrt(((float)mEndCoord.x - (float)c.x)*((float)mEndCoord.x - (float)c.x) + ((float)mEndCoord.y - (float)c.y)*((float)mEndCoord.y - (float)c.y));
	};

	if (ImGui::Selectable("Run BFS", mMode == 1))
	{
		mMode = 1;
		ResetCount();
	}
	if (ImGui::Selectable("Run DFS", mMode == 2))
	{
		mMode = 2;
		ResetCount();
	}
	if (ImGui::Selectable("Run Dijkstra", mMode == 3))
	{
		mMode = 3;
		ResetCount();
	}
	if (ImGui::Selectable("Run AStar", mMode == 4))
	{
		mMode = 4;
		ResetCount();
	}
	if (ImGui::Selectable("Run JPS", mMode == 5))
	{
		mMode = 5;
		ResetCount();
	}

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	switch (mMode)
	{
	case 1:
	{
		mGraph.RunBFS(mStartCoord, mEndCoord);
		break;
	}
	case 2:
	{
		mGraph.RunDFS(mStartCoord, mEndCoord);
		break;
	}
	case 3:
	{
		mGraph.RunDijkstra(mStartCoord, mEndCoord, GetDistance);
		break;
	}
	case 4:
	{
		mGraph.RunAStar(mStartCoord, mEndCoord, GetDistance, GetStraightDistance);
		break;
	}
	case 5:
	{
		mGraph.RunJPS(mStartCoord, mEndCoord, JPSGetDistance, GetStraightDistance);
		break;
	}
	default:
		break;
	}
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	if (mMode > 0)
	{
		mTotalTimeCost += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
		++mRunCount;
		mAverageTimeCost = mTotalTimeCost / (float)mRunCount;
	}

	// Image Button
	ImVec2 buttonSize(mTileMap.GetTileSize() + 4.0f, mTileMap.GetTileSize() + 4.0f);
	float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
	for (int i = 0; i < mTileMap.GetTileCount(); i++)
	{
		if (ImGui::ImageButton(S::Graphics::TextureManager::Get()->GetTexture(mTileMap.GetTextureId(i))->GetShaderResourceView(), buttonSize))
		{
			mTileIndex = i;
		}
		//ImGui::PushID(n);
		//ImGui::Button("Box", buttonSize);
		float last_button_x2 = ImGui::GetItemRectMax().x;
		float next_button_x2 = last_button_x2 + ImGui::GetStyle().ItemSpacing.x + buttonSize.x; // Expected position if next button was on same line
		if (i + 1 < mTileMap.GetTileCount() && next_button_x2 < window_visible_x2)
			ImGui::SameLine();
		//ImGui::PopID();
	}

	if (ImGui::Button("Save Map"))
	{
		mTileMap.SaveLevel("../Assets/TileMaps/PathFindingMap.txt");
	}
	// State
	Transition nextState = Transition::None;
	if (ImGui::Button("Return"))
	{
		nextState = Transition::GoToFrontend;
	}
	return nextState;
}

void PathFindingScene::Render()
{
	mTileMap.Render(mWorldPosition, mWorldScale);

	mGraph.RenderPath(mTileMap.GetTileSize(), mWorldPosition);
	Graphics::DrawSprite(mStartPointTexture,
		Math::Vector2{ ((float)mStartCoord.x*mTileMap.GetTileSize() + 0.5f*mTileMap.GetTileSize())*mWorldScale,
		((float)mStartCoord.y*mTileMap.GetTileSize() + 0.5f*mTileMap.GetTileSize())*mWorldScale } + mWorldPosition,
		0.0f, mWorldScale);

	Graphics::DrawSprite(mEndPointTexture,
		Math::Vector2{ ((float)mEndCoord.x*mTileMap.GetTileSize() + 0.5f*mTileMap.GetTileSize())*mWorldScale,
		((float)mEndCoord.y*mTileMap.GetTileSize() + 0.5f*mTileMap.GetTileSize())*mWorldScale } + mWorldPosition,
		0.0f, mWorldScale);

	ImGui::Text("Average: %f", mAverageTimeCost);
	ImGui::Text("Count: %d", mRunCount);
	ImGui::Text("Total Cost: %f", mTotalTimeCost);
}

void PathFindingScene::PlayerInput()
{
	uint32_t tColumn = (input->GetMouseScreenX() - static_cast<int>(mWorldPosition.x)) / static_cast<int>(mTileMap.GetTileSize());
	uint32_t tRow = (input->GetMouseScreenY() - static_cast<int>(mWorldPosition.y)) / static_cast<int>(mTileMap.GetTileSize());

	if (input->IsKeyDown(S::Input::KeyCode::SPACE))
	{
		mWorldPosition.x += static_cast<float>(input->GetMouseMoveX());
		mWorldPosition.y += static_cast<float>(input->GetMouseMoveY());
	}

	if (input->GetMouseScreenX() / static_cast<int>(mTileMap.GetTileSize()) < 0
		|| input->GetMouseScreenX() / static_cast<int>(mTileMap.GetTileSize()) >= mTileMap.GetMapColumns()
		|| input->GetMouseScreenY() / static_cast<int>(mTileMap.GetTileSize()) < 0
		|| input->GetMouseScreenY() / static_cast<int>(mTileMap.GetTileSize()) >= mTileMap.GetMapRows())
	{
		return;
	}
	if (input->IsKeyDown(S::Input::KeyCode::LCONTROL))
	{
		if (input->IsMouseDown(S::Input::MouseButton::LBUTTON))
		{
			mTileMap.SetTile(tColumn, tRow, mTileIndex);
			if (mTileIndex)
			{
				mGraph.SetBlock(tColumn, tRow);
			}
			else
			{
				mGraph.Unblock(tColumn, tRow);
			}
		}
	}
	else
	{
		if (input->IsMouseDown(S::Input::MouseButton::LBUTTON))
		{
			mStartCoord.x = tColumn;
			mStartCoord.y = tRow;
			ResetCount();
		}
		if (input->IsMouseDown(S::Input::MouseButton::RBUTTON))
		{
			mEndCoord.x = tColumn;
			mEndCoord.y = tRow;
			ResetCount();
		}
	}
}

void PathFindingScene::ScaleUpdate(float deltaTime)
{
	float scaleSpeed = 10.0f;

	mWorldTargetScale += static_cast<float>(input->GetMouseMoveZ())*0.001f;
	if (mWorldTargetScale > 2.0f)
	{
		mWorldTargetScale = 2.0f;
	}
	else if (mWorldTargetScale < 0.5f)
	{
		mWorldTargetScale = 0.5f;
	}

	if (Math::Abs(mWorldScale - mWorldTargetScale) > 0.001)
	{
		mWorldScale += (mWorldTargetScale - mWorldScale)*deltaTime*scaleSpeed;
	}
	else
	{
		mWorldScale = mWorldTargetScale;
	}
}

void PathFindingScene::ResetCount()
{
	mRunCount = 0;
	mTotalTimeCost = 0.0f;
	mAverageTimeCost = 0.0f;
}
