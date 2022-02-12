#include "Frontend.h"

using namespace S;

Frontend::Frontend()
{
}

Frontend::~Frontend()
{

}

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
	if (ImGui::Button("Quit"))
	{
		nextState = Transition::QuitGame;
	}

	return nextState;
}


void Frontend::Render()
{

}