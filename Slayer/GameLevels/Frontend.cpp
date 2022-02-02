#include "Frontend.h"

using namespace S;

Frontend::Frontend()
{
	input = Input::InputSystem::Get();
}

Frontend::~Frontend()
{

}

void Frontend::Load()
{
	menuManager.LoadStartMenu();
}

void Frontend::Unload()
{
}

Transition Frontend::Update(float deltaTime)
{
	Transition nextState = Transition::None;

	menuManager.Update(deltaTime);

	// TODO: change to button
	if (menuManager.GetStartMenu().ButtonCheck(0))
	{
		nextState = Transition::GoToPlay;
	}
	/*if (X::IsKeyPressed(X::Keys::TWO))
	{
		nextState = Transition::GoToSinglePlayer;
	}*/
	if (input->IsKeyPressed(Input::KeyCode::ZERO))
	{
		nextState = Transition::GoToDebug;
	}
	if (input->IsKeyPressed(Input::KeyCode::ESCAPE)
		|| menuManager.GetStartMenu().ButtonCheck(1))
	{
		nextState = Transition::QuitGame;
	}

	return nextState;
}


void Frontend::Render()
{
	menuManager.Render();
}