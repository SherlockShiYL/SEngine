#include "Frontend.h"

using namespace S;

void Frontend::Load()
{
}

void Frontend::Unload()
{
}

Transition Frontend::Update(float deltaTime)
{
	Transition nextState = Transition::None;

	if (ImGui::Button("Path Finding Scene"))
	{
		nextState = Transition::GoToPathFindingScene;
	}
	if (ImGui::Button("Quadtree"))
	{
		nextState = Transition::GoToQuadtree;
	}
	if (ImGui::Button("Menu System"))
	{
		nextState = Transition::GoToMenuSystem;
	}
	if (ImGui::Button("Quit"))
	{
		nextState = Transition::QuitGame;
	}

	return nextState;
}


void Frontend::Render()
{

}