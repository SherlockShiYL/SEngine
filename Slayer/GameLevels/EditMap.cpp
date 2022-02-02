#include "EditMap.h"

using namespace S;

EditMap::EditMap()
{
	input = Input::InputSystem::Get();
}

EditMap::~EditMap()
{
}

void EditMap::Load()
{
	tileMap.Load(level, "Data/Maps/ForestMap_Set.txt");
}

void EditMap::Unload()
{
	tileMap.Unload();
}

Transition EditMap::Update(float deltaTime)
{
	if (input->IsMouseDown(Input::MouseButton::RBUTTON))
	{
		mPosition += { static_cast<float>(input->GetMouseMoveX()), static_cast<float>(input->GetMouseMoveY())};
	}
	if (input->IsKeyDown(Input::KeyCode::LCONTROL))
	{
		if (input->IsMouseDown(Input::MouseButton::LBUTTON))
		{
			tileIndex = tileMap.ChooseTile(input->GetMouseScreenX(), input->GetMouseScreenY());
		}
	}
	else if (input->IsMouseDown(Input::MouseButton::LBUTTON))
	{
		tileMap.SetTile((input->GetMouseScreenX() - static_cast<int>(mPosition.x)) / static_cast<int>(tileMap.GetTileSize()), (input->GetMouseScreenY() - static_cast<int>(mPosition.y)) / static_cast<int>(tileMap.GetTileSize()), tileIndex);
	}

	if (input->IsKeyPressed(Input::KeyCode::S))
	{
		tileMap.SaveLevel(level);
	}

	char nn[6];
	_itoa_s((input->GetMouseScreenX() - static_cast<int>(mPosition.x)) / static_cast<int>(tileMap.GetTileSize()), nn, 10);
	Graphics::DrawScreenText(nn, 10.0f, 120.0f, 24.0f, Math::Vector4::Magenta());

	_itoa_s((input->GetMouseScreenY() - static_cast<int>(mPosition.y)) / static_cast<int>(tileMap.GetTileSize()), nn, 10);
	Graphics::DrawScreenText(nn, 10.0f, 160.0f, 24.0f, Math::Vector4::Magenta());

	// State
	Transition nextState = Transition::None;
	if (input->IsKeyPressed(Input::KeyCode::ESCAPE))
	{
		nextState = Transition::GoToFrontend;
	}
	return nextState;
}

void EditMap::Render()
{
	tileMap.Render(mPosition, 1.0f);
	tileMap.RenderMenu();
}
