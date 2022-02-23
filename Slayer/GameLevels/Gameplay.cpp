#include "Gameplay.h"
#include "../GameObjects/GameObjectManager.h"

using namespace S;

namespace
{
	bool renderLine{ true };
}
void Gameplay::Load()
{
	input = Input::InputSystem::Get();
	GameObjectManager::StaticInitialize();

	//tileMap.Load("IslandMap.txt", "IslandMap_Set.txt");
	tileMap.Load("Data/Maps/ForestMap.txt", "Data/Maps/ForestMap_Set.txt");

	// Could be a huge data
	gameObjMgr = GameObjectManager::Get();

	gameObjMgr->aiWorld.GetNavGraph().Init(tileMap.GetMapColumns(), tileMap.GetMapRows(), tileMap.GetTileSize());
	gameObjMgr->aiWorld.GetQuadrant().Initialize({ 1.0f,1.0f }, 1u, 1u);

	// Load Blocks
	for (int i = 0; i < tileMap.GetMapColumns(); ++i)
	{
		for (int j = 0; j < tileMap.GetMapRows(); ++j)
		{
			int tIndex = tileMap.GetTile(i, j);
			//if ((tIndex > 11 && tIndex <= 20) || (tIndex > 33 && tIndex <= 38))
			if (tIndex > 10)
			{
				gameObjMgr->aiWorld.GetNavGraph().SetBlock(i, j);
			}
		}
	}

	gameObjMgr->CreateDefaultGameObjects();

	gameObjMgr->CreateEnemies("Data/Home.txt");

	gameObjMgr->GetPlayer()->LoadTexture();
	gameObjMgr->GetPlayer()->Wake();
	//gameObjMgr->CreateBase();

	sCommand.Clear();
	/*Player newPlayer(gameObjMgr->aiWorld, 0, 0);
	DataLoader::LoadPlayer("Data/Player.txt", newPlayer);

	// Load Entities
	/*for (int i = 0; i < 15; ++i)
	{
		TinyBot* newBot = new TinyBot{ aiWorld };
		entityList.push_back(newBot);
		tinyBot.push_back(newBot);
		tinyBot[i]->Load({ 0.0f,0.0f }, 0, 0);
	}*/
}

void Gameplay::Unload()
{
	tileMap.Unload();
	gameObjMgr = nullptr;
	GameObjectManager::StaticTerminate();
}

Transition Gameplay::Update(float deltaTime)
{
	// Draw Frame
	char frames[6];
	_itoa_s(static_cast<int>(1.0f / deltaTime), frames, 10);
	Graphics::DrawScreenText(frames, 10.0f, 20.0f, 24.0f, Math::Vector4::Magenta());

	// Damage Display!
	RunDamageDisplay(deltaTime, mWorldPosition);

	gameObjMgr->Update(deltaTime);

	// Input
	PlayerInput();

	gameObjMgr->GetPlayer()->AddCommand(sCommand);

	// TinyBot
	/*if (test)
	{
		for (int i = 0; i < tinyBot.size(); ++i)
		{
			tinyBot[i]->Update(deltaTime);
		}
	}*/

	// Sort Entities render order
	/*std::sort(entityList.begin(), entityList.end(), [](const AI::Entity *lhs, const AI::Entity *rhs)
	{
		return lhs->Position().y < rhs->Position().y;
	});*/
	/*std::qsort(entityList.data(), entityList.size(), sizeof(AI::Entity), [](const AI::Entity &lhs, const AI::Entity &rhs) -> bool
	{
		return lhs.Position().y < rhs.Position().y;
	});*/


	//ScaleUpdate(deltaTime);

	// State
	Transition nextState = Transition::None;
	if (input->IsKeyPressed(Input::KeyCode::ESCAPE))
	{
		nextState = Transition::GoToFrontend;
	}
	return nextState;
}

void Gameplay::Render()
{
	ImGui::Checkbox("RenderLine", &renderLine);
	Graphics::DrawScreenCircle({ {(float)input->GetMouseScreenX(),(float)input->GetMouseScreenY()},5.0f }, Math::Vector4::Magenta());

	tileMap.Render(mWorldPosition, mWorldScale, renderLine);

	if (gameObjMgr->GetPlayer()->IsActive())
	{
		gameObjMgr->GetPlayer()->Render(mWorldPosition, mWorldScale);
	}

	gameObjMgr->Render(mWorldPosition, mWorldScale);

	/*for (auto& e : entityList)
	{
		e->Render(mPosition);
	}*/


	/*for (auto &s : storages)
	{
		s->Render(mPosition);
	}*/

	/*for (auto& bot : tinyBot)
	{
		bot->DebugRender(mPosition);
	}*/

	
}

void Gameplay::PlayerInput()
{
	sCommand.Clear();
	if (input->IsKeyDown(Input::KeyCode::W))
		sCommand.MoveUp();
	if (input->IsKeyDown(Input::KeyCode::S))
		sCommand.MoveDown();
	if (input->IsKeyDown(Input::KeyCode::A))
		sCommand.MoveLeft();
	if (input->IsKeyDown(Input::KeyCode::D))
		sCommand.MoveRight();
	if (input->IsKeyPressed(Input::KeyCode::J)) // Attack
		sCommand.Attack();
	/*if (S::IsKeyDown(S::Keys::K)) // Ult
	{
		input += 32;
	}
	if (S::IsKeyDown(S::Keys::L)) // Normal Skill
	{
		input += 64;
	}
	if (S::IsKeyDown(S::Keys::U)) // Item left slide
	{
		input += 128;
	}
	if (S::IsKeyDown(S::Keys::O)) // Item right slide
	{
		input += 256;
	}
	if (S::IsKeyDown(S::Keys::I)) // Item Use
	{
		input += 512;
	}
	if (S::IsKeyDown(S::Keys::H)) // Act with environment
	{
		input += 1024;
	}*/
	char commands[600];
	_itoa_s(static_cast<int>(sCommand.GetCommand()), commands, 10);
	Graphics::DrawScreenText(commands, 100.0f, 20.0f, 24.0f, Math::Vector4::Magenta());




	if (input->IsMouseDown(Input::MouseButton::RBUTTON))
	{
		mWorldPosition.x += static_cast<float>(input->GetMouseMoveX());
		mWorldPosition.y += static_cast<float>(input->GetMouseMoveY());
	}
	if (mWorldPosition.x > 0.0f)
		mWorldPosition.x = 0.0f;
	if (mWorldPosition.y > 0.0f)
		mWorldPosition.y = 0.0f;

	//GameObjectManager::Get()->Draw();
}

void Gameplay::RunDamageDisplay(float deltaTime, Math::Vector2 position)
{
	for (uint32_t i = 0; i < mDamageDisplaySize; ++i)
	{
		if (mDamageDisplay[i].IsActive())
		{
			mDamageDisplay[i].Update(deltaTime);
			mDamageDisplay[i].Render(position);
		}
		else
		{
			mDamageDisplay[i] = mDamageDisplay[mDamageDisplaySize - 1];
			--mDamageDisplaySize;
			--i;
		}
	}
}

void Gameplay::ScaleUpdate(float deltaTime)
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

//float Gameplay::GetDistance(AI::Graph::Coord c1, AI::Graph::Coord c2)
//{
//	float extraWeight = 0.0f;
//	float diagonalExtra = sqrtf(2) - 1.0f;
//	if (tileMap.GetTile(c2.x, c2.y) == 1)
//	{
//		extraWeight += 4.0f;
//	}
//	if (tileMap.GetTile(c1.x, c1.y) == 1)
//	{
//		extraWeight += 4.0f;
//	}
//	if (c1.x == c2.x || c1.y == c2.y)
//	{
//		diagonalExtra = 0.0f;
//	}
//	return 1.0f + extraWeight + diagonalExtra;
//}