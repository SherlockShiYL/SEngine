#include "MenuSystem.h"

using namespace S;

void MenuSystem::Load()
{
	input = Input::InputSystem::Get();
	menuManager = MenuManager::Get();

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


	Menu mTLMenu(Geometry::Rect{ 0.0f,0.0f,(float)Graphics::GetScreenWidth(),(float)Graphics::GetScreenHeight() },
		100.0f, 60.0f, { 60.0f,40.0f }, AnchorRow::Top, AnchorColumn::Left);
	Menu mTCMenu(Geometry::Rect{ 0.0f,0.0f,(float)Graphics::GetScreenWidth(),(float)Graphics::GetScreenHeight() },
		100.0f, 60.0f, { 0.0f,40.0f }, AnchorRow::Top, AnchorColumn::Center);
	Menu mTRMenu(Geometry::Rect{ 0.0f,0.0f,(float)Graphics::GetScreenWidth(),(float)Graphics::GetScreenHeight() },
		100.0f, 60.0f, { -60.0f,40.0f }, AnchorRow::Top, AnchorColumn::Right);
	Menu mCLMenu(Geometry::Rect{ 0.0f,0.0f,(float)Graphics::GetScreenWidth(),(float)Graphics::GetScreenHeight() },
		100.0f, 60.0f, { 60.0f,0.0f }, AnchorRow::Center, AnchorColumn::Left);
	Menu mCCMenu(Geometry::Rect{ 0.0f,0.0f,(float)Graphics::GetScreenWidth(),(float)Graphics::GetScreenHeight() },
		100.0f, 60.0f, { 0.0f,0.0f }, AnchorRow::Center, AnchorColumn::Center);
	Menu mCRMenu(Geometry::Rect{ 0.0f,0.0f,(float)Graphics::GetScreenWidth(),(float)Graphics::GetScreenHeight() },
		100.0f, 60.0f, { -60.0f,0.0f }, AnchorRow::Center, AnchorColumn::Right);
	Menu mBLMenu(Geometry::Rect{ 0.0f,0.0f,(float)Graphics::GetScreenWidth(),(float)Graphics::GetScreenHeight() },
		100.0f, 60.0f, { 60.0f,-40.0f }, AnchorRow::Bottom, AnchorColumn::Left);
	Menu mBCMenu(Geometry::Rect{ 0.0f,0.0f,(float)Graphics::GetScreenWidth(),(float)Graphics::GetScreenHeight() },
		100.0f, 60.0f, { 0.0f,-40.0f }, AnchorRow::Bottom, AnchorColumn::Center);
	Menu mBRMenu(Geometry::Rect{ 0.0f,0.0f,(float)Graphics::GetScreenWidth(),(float)Graphics::GetScreenHeight() },
		100.0f, 60.0f, { -60.0f,-40.0f }, AnchorRow::Bottom, AnchorColumn::Right);

	menuManager->Insert(mTLMenu);
	menuManager->Insert(mTCMenu);
	menuManager->Insert(mTRMenu);
	menuManager->Insert(mCLMenu);
	menuManager->Insert(mCCMenu);
	menuManager->Insert(mCRMenu);
	menuManager->Insert(mBLMenu);
	menuManager->Insert(mBCMenu);
	menuManager->Insert(mBRMenu);
	menuManager->Insert(mMainMenu);
}

void MenuSystem::Unload()
{
	menuManager->Clear();
}

Transition MenuSystem::Update(float deltaTime)
{
	menuManager->Update(deltaTime);

	if (!menuManager->IsMouseCollided())
	{

	}

	// State
	Transition nextState = Transition::None;
	if (ImGui::Button("Return"))
	{
		nextState = Transition::GoToFrontend;
	}
	return nextState;
}


void MenuSystem::Render()
{

}