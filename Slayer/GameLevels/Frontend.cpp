#include "Frontend.h"

using namespace S;

Frontend::Frontend()
{
	input = Input::InputSystem::Get();
	menuManager = MenuManager::Get();
}

Frontend::~Frontend()
{

}

void Frontend::Load()
{
	Menu mMainMenu(Graphics::GetScreenRect(), 300.0f, 450.0f, { 0.0f,0.0f }, AnchorRow::Center, AnchorColumn::Center);
	//Menu mMessageDisplayMenu(Graphics::GetScreenRect(),)
	mMainMenu.AddButton(200.0f, 100.0f, { 0.0f,0.0f }, AnchorRow::Center, AnchorColumn::Center, " ", "Play");
	mMainMenu.AddButton(200.0f, 100.0f, { 0.0f,0.0f }, AnchorRow::Center, AnchorColumn::Center, " ", "Power Up");
	mMainMenu.AddButton(200.0f, 100.0f, { 0.0f,0.0f }, AnchorRow::Center, AnchorColumn::Center, " ", "Quit");
	mMainMenu.SetButtonListDisplay({ 0.0f,0.0f }, AnchorRow::Center, AnchorColumn::Center, MenuDisplayOrder::CenterDownRight, 1, 5, 0.0f, 120.0f);

	menuManager->Insert(mMainMenu);

}

void Frontend::Unload()
{
	menuManager->Clear();
}

Transition Frontend::Update(float deltaTime)
{
	Transition nextState = Transition::None;

	menuManager->Update(deltaTime);

	if (menuManager->Pressed("Play", Input::MouseButton::LBUTTON))
	{
		nextState = Transition::GoToPlay;
	}
	else if (menuManager->Pressed("Quit", Input::MouseButton::LBUTTON))
	{
		nextState = Transition::QuitGame;
	}

	if (input->IsKeyPressed(Input::KeyCode::ZERO))
	{
		nextState = Transition::GoToDebug;
	}

	return nextState;
}


void Frontend::Render()
{

}