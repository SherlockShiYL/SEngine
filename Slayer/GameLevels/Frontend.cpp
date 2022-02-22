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
	mMainMenu.AddButton(50.0f, 30.0f, { 0.0f,0.0f }, AnchorRow::Center, AnchorColumn::Center, " ", "Begin");
	mMainMenu.AddButton(50.0f, 30.0f, { 0.0f,0.0f }, AnchorRow::Center, AnchorColumn::Center, " ", "Power Up");
	mMainMenu.AddButton(50.0f, 30.0f, { 0.0f,0.0f }, AnchorRow::Center, AnchorColumn::Center, " ", "Quit");
	mMainMenu.AddButton(50.0f, 30.0f, { 0.0f,0.0f }, AnchorRow::Center, AnchorColumn::Center, " ", "000");
	mMainMenu.AddButton(50.0f, 30.0f, { 0.0f,0.0f }, AnchorRow::Center, AnchorColumn::Center, " ", "111");
	mMainMenu.AddButton(50.0f, 30.0f, { 0.0f,0.0f }, AnchorRow::Center, AnchorColumn::Center, " ", "222");
	mMainMenu.AddButton(50.0f, 30.0f, { 0.0f,0.0f }, AnchorRow::Center, AnchorColumn::Center, " ", "333");
	mMainMenu.AddButton(50.0f, 30.0f, { 0.0f,0.0f }, AnchorRow::Center, AnchorColumn::Center, " ", "444");
	mMainMenu.AddButton(50.0f, 30.0f, { 0.0f,0.0f }, AnchorRow::Center, AnchorColumn::Center, " ", "555");
	mMainMenu.AddButton(50.0f, 30.0f, { 0.0f,0.0f }, AnchorRow::Center, AnchorColumn::Center, " ", "666");
	mMainMenu.AddButton(50.0f, 30.0f, { 0.0f,0.0f }, AnchorRow::Center, AnchorColumn::Center, " ", "777");
	mMainMenu.AddButton(50.0f, 30.0f, { 0.0f,0.0f }, AnchorRow::Center, AnchorColumn::Center, " ", "888");
	mMainMenu.AddButton(50.0f, 30.0f, { 0.0f,0.0f }, AnchorRow::Center, AnchorColumn::Center, " ", "999");
	mMainMenu.SetButtonListDisplay({ 0.0f,0.0f }, AnchorRow::Center, AnchorColumn::Left, MenuDisplayOrder::RightDown, 5, 5, 51.0f, 40.0f);

	menuManager->Insert(mMainMenu);

}

void Frontend::Unload()
{
}

Transition Frontend::Update(float deltaTime)
{
	Transition nextState = Transition::None;

	menuManager->Update(deltaTime);



	// TODO: change to button
	/*if (true)
	{
		nextState = Transition::GoToPlay;
	}*/
	/*if (X::IsKeyPressed(X::Keys::TWO))
	{
		nextState = Transition::GoToSinglePlayer;
	}*/
	if (input->IsKeyPressed(Input::KeyCode::ZERO))
	{
		nextState = Transition::GoToDebug;
	}
	if (input->IsKeyPressed(Input::KeyCode::ESCAPE)
		)
	{
		nextState = Transition::QuitGame;
	}

	return nextState;
}


void Frontend::Render()
{

}