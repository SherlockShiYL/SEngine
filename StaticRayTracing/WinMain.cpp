#include <GameEngine/Inc/GameEngine.h>
#include <External/ImGui/Inc/imgui.h>

using namespace S;

bool Init(float deltaTime);
bool Menu(float deltaTime);
bool Play(float deltaTime);
bool Generate(float deltaTime);

void CheckCollision();

std::function<bool(float)> Tick = Init;

Input::InputSystem* input{ nullptr };
S::Geometry::Sphere Spheres[15];

bool Init(float deltaTime)
{
	input = Input::InputSystem::Get();
	Tick = Menu;

	Spheres[0] = S::Geometry::Sphere{ {100.0f,000.0f,-100.0f},30.0f };

	return input->IsKeyDown(Input::KeyCode::ESCAPE);
}


bool Menu(float deltaTime)
{
	/*ImGui::InputText("Ip Address:", ipAdd, 20);
	ImGui::Checkbox("Play", &isClient);
	ImGui::Checkbox("Run Server", &isServer);*/
	
	return input->IsKeyDown(Input::KeyCode::ESCAPE);
}

bool Play(float deltaTime)
{

	return input->IsKeyDown(Input::KeyCode::ESCAPE);
}

bool GameLoop(float deltaTime)
{
	return Tick(deltaTime);
}

void Terminate()
{}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	S::GameApplication myApp;

	myApp.Initialize(hInstance, "sconfig.json");

	while (myApp.IsRunning())
	{
		myApp.Update(GameLoop);
	}

	Terminate();
	myApp.Terminate();
	return 0;
}