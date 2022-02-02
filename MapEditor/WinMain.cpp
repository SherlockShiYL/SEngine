#include <GameEngine/Inc/GameEngine.h>

S::TileMap tileMap;
S::Math::Vector2 worldPosition;
S::Input::InputSystem* input = nullptr;
char selectedMapName[45];
char newMapName[45];
bool titleBegin, playBegin;
int mapIndex{ 0 }, setIndex{ 0 }, tileIndex{ 0 },mapRow{ 0 },mapColumn{ 0 };

WIN32_FIND_DATAA ffd;
HANDLE hFind = INVALID_HANDLE_VALUE;
std::vector<std::string> fileNames;
std::string message;
static const char* path = "../Assets/TileMaps/*";
std::vector<std::string> fileList;


bool Init(float deltaTime);
bool Title(float deltaTime);
bool Play(float deltaTime);
void PlayerInput();

std::function<bool(float)> Tick = Init;
bool Init(float deltaTime)
{
	Tick = Title;
	input = S::Input::InputSystem::Get();

	hFind = FindFirstFileA(path, &ffd);
	if (INVALID_HANDLE_VALUE != hFind)
	{
		// List all the files in the directory with some info about them.
		do
		{
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				//printf(("  %s   <DIR>\n"), ffd.cFileName);
			}
			else
			{
				std::string filename = ffd.cFileName;
				fileList.emplace_back("../Assets/TileMaps/" + filename);
			}
		} while (FindNextFileA(hFind, &ffd) != 0);
	}

	strcpy_s(newMapName, path);

	return input->IsKeyPressed(S::Input::KeyCode::ESCAPE);
}

bool Title(float deltaTime)
{
	S::Graphics::DrawScreenText("Follow the ImGui", 10.0f, 10.0f, 20.0f, S::Math::Vector4::White());
	ImGui::ShowDemoWindow();

	if (ImGui::ListBoxHeader("File List", static_cast<int>(fileList.size()), 4))
	{
		for (uint32_t i = 0; i < fileList.size(); i++)
		{
			if (ImGui::Selectable(fileList[i].c_str(), mapIndex == i))
			{
				mapIndex = i;
			}
		}
		ImGui::ListBoxFooter();
	}

	ImGui::Separator();
	ImGui::Text(fileList[mapIndex].c_str());
	//ImGui::SetNextWindowSize(ImVec2(600, 0));
	if (ImGui::Button("Open"))
	{
		ImGui::OpenPopup("Choose Map Set File:");
	}
	if (ImGui::BeginPopup("Choose Map Set File:"))
	{
		ImGui::PushItemWidth(300);
		if (ImGui::ListBoxHeader("File List", static_cast<int>(fileList.size()), 4))
		{
			for (uint32_t i = 0; i < fileList.size(); i++)
			{
				if (ImGui::Selectable(fileList[i].c_str(), setIndex == i))
				{
					setIndex = i;
				}
			}
			ImGui::ListBoxFooter();
		}
		ImGui::Separator();
		ImGui::Text(fileList[setIndex].c_str());

		if (ImGui::Button("Set", ImVec2(120, 0)))
		{
			tileMap.Load(fileList[mapIndex].c_str(), fileList[setIndex].c_str());
			strcpy_s(selectedMapName, fileList[mapIndex].c_str());
			mapRow = tileMap.GetMapRows();
			mapColumn = tileMap.GetMapColumns();
			ImGui::CloseCurrentPopup();
			Tick = Play;
		}
		ImGui::SetItemDefaultFocus(); // Don't know what this function doing
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	return input->IsKeyPressed(S::Input::KeyCode::ESCAPE);
}

bool Play(float deltaTime)
{
	//ImGui::Begin("Menu", &playBegin, ImGuiWindowFlags_MenuBar);
	//tileMap.Update(deltaTime);
	tileMap.Render(worldPosition, 1.0f);

	if (ImGui::Button("Save"))
	{
		tileMap.SaveLevel(selectedMapName);
	}
	ImGui::SameLine();
	if (ImGui::Button("Save As"))
	{
		ImGui::OpenPopup("New File Name:");
	}
	ImGui::SameLine();

	if (ImGui::Button("Return"))
	{
		ImGui::OpenPopup("Did You Save?");
	}

	//ImGui::InputText("Map Name", selectedMapName, IM_ARRAYSIZE(selectedMapName));
	ImGui::Text(selectedMapName);
	ImGui::Text("%d", tileIndex);
	if (ImGui::BeginPopupModal("Did You Save?"))
	{
		//ImGui::PushItemWidth(300);
		ImGui::Text(fileList[setIndex].c_str());
		if (ImGui::Button("Save and Return", ImVec2(120, 0)))
		{
			tileMap.SaveLevel(selectedMapName);
			ImGui::CloseCurrentPopup();
			Tick = Title;
		}
		if (ImGui::Button("Return", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
			Tick = Title;
		}
		ImGui::SetItemDefaultFocus(); // Don't know what this function doing
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	if (ImGui::BeginPopupModal("New File Name:"))
	{
		ImGui::InputText("Map Name", newMapName, IM_ARRAYSIZE(newMapName));
		//ImGui::Text(newMapName); // Debug only
		if (ImGui::Button("Save", ImVec2(120, 0)))
		{
			tileMap.SaveLevel(newMapName);
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	if (ImGui::ListBoxHeader("File List", static_cast<int>(fileList.size()), 4))
	{
		for (uint32_t i = 0; i < fileList.size(); i++)
		{
			if (ImGui::Selectable(fileList[i].c_str(), mapIndex == i))
			{
				mapIndex = i;
			}
		}
		ImGui::ListBoxFooter();
	}

	ImVec2 buttonSize(tileMap.GetTileSize() + 4.0f, tileMap.GetTileSize() + 4.0f);
	float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
	for (int i = 0; i < tileMap.GetTileCount(); i++)
	{
		if (ImGui::ImageButton(S::Graphics::TextureManager::Get()->GetTexture(tileMap.GetTextureId(i))->GetShaderResourceView(), buttonSize))
		{
			tileIndex = i;
		}
		//ImGui::PushID(n);
		//ImGui::Button("Box", buttonSize);
		float last_button_x2 = ImGui::GetItemRectMax().x;
		float next_button_x2 = last_button_x2 + ImGui::GetStyle().ItemSpacing.x + buttonSize.x; // Expected position if next button was on same line
		if (i + 1 < tileMap.GetTileCount() && next_button_x2 < window_visible_x2)
			ImGui::SameLine();
		//ImGui::PopID();
	}

	if (input->IsMouseDown(S::Input::MouseButton::LBUTTON))
	{
		tileMap.SetTile((input->GetMouseScreenX() - static_cast<int>(worldPosition.x)) / static_cast<int>(tileMap.GetTileSize()),
			(input->GetMouseScreenY() - static_cast<int>(worldPosition.y)) / static_cast<int>(tileMap.GetTileSize()), tileIndex);
	}

	/*ImGui::InputInt("Row", &mapRow);
	ImGui::InputInt("Column", &mapColumn);
	if (ImGui::Button("Apply"))
	{
	}*/




	if (ImGui::CollapsingHeader("Widgets"))
	{
		if (ImGui::TreeNode("Images"))
		{
			ImGuiIO& io = ImGui::GetIO();
			ImGui::TextWrapped("Below we are displaying the font texture (which is the only texture we have access to in this demo). Use the 'ImTextureID' type as storage to pass pointers or identifier to your own texture data. Hover the texture for a zoomed view!");

			// Here we are grabbing the font texture because that's the only one we have access to inside the demo code.
			// Remember that ImTextureID is just storage for whatever you want it to be, it is essentially a value that will be passed to the render function inside the ImDrawCmd structure.
			// If you use one of the default imgui_impl_XXXX.cpp renderer, they all have comments at the top of their file to specify what they expect to be stored in ImTextureID.
			// (for example, the imgui_impl_dx11.cpp renderer expect a 'ID3D11ShaderResourceView*' pointer. The imgui_impl_glfw_gl3.cpp renderer expect a GLuint OpenGL texture identifier etc.)
			// If you decided that ImTextureID = MyEngineTexture*, then you can pass your MyEngineTexture* pointers to ImGui::Image(), and gather width/height through your own functions, etc.
			// Using ShowMetricsWindow() as a "debugger" to inspect the draw data that are being passed to your render will help you debug issues if you are confused about this.
			// Consider using the lower-level ImDrawList::AddImage() API, via ImGui::GetWindowDrawList()->AddImage().
			ImTextureID my_tex_id = io.Fonts->TexID;
			float my_tex_w = (float)io.Fonts->TexWidth;
			float my_tex_h = (float)io.Fonts->TexHeight;

			ImGui::Text("%.0fx%.0f", my_tex_w, my_tex_h);
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImGui::Image(my_tex_id, ImVec2(my_tex_w, my_tex_h), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				float region_sz = 32.0f;
				float region_x = io.MousePos.x - pos.x - region_sz * 0.5f; if (region_x < 0.0f) region_x = 0.0f; else if (region_x > my_tex_w - region_sz) region_x = my_tex_w - region_sz;
				float region_y = io.MousePos.y - pos.y - region_sz * 0.5f; if (region_y < 0.0f) region_y = 0.0f; else if (region_y > my_tex_h - region_sz) region_y = my_tex_h - region_sz;
				float zoom = 4.0f;
				ImGui::Text("Min: (%.2f, %.2f)", region_x, region_y);
				ImGui::Text("Max: (%.2f, %.2f)", region_x + region_sz, region_y + region_sz);
				ImVec2 uv0 = ImVec2((region_x) / my_tex_w, (region_y) / my_tex_h);
				ImVec2 uv1 = ImVec2((region_x + region_sz) / my_tex_w, (region_y + region_sz) / my_tex_h);
				ImGui::Image(my_tex_id, ImVec2(region_sz * zoom, region_sz * zoom), uv0, uv1, ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
				ImGui::EndTooltip();
			}
			ImGui::TextWrapped("And now some textured buttons..");
			static int pressed_count = 0;
			for (int i = 0; i < 8; i++)
			{
				ImGui::PushID(i);
				int frame_padding = -1 + i;     // -1 = uses default padding
				if (ImGui::ImageButton(my_tex_id, ImVec2(32, 32), ImVec2(0, 0), ImVec2(32.0f / my_tex_w, 32 / my_tex_h), frame_padding, ImColor(0, 0, 0, 255)))
					pressed_count += 1;
				ImGui::PopID();
				ImGui::SameLine();
			}
			ImGui::NewLine();
			ImGui::Text("Pressed %d times.", pressed_count);
			ImGui::TreePop();
		}

	}


	//ImGui::End();
	PlayerInput();
	return input->IsKeyPressed(S::Input::KeyCode::ESCAPE);
}

void PlayerInput()
{
	if (input->IsMouseDown(S::Input::MouseButton::RBUTTON))
	{
		worldPosition.x += static_cast<float>(input->GetMouseMoveX());
		worldPosition.y += static_cast<float>(input->GetMouseMoveY());
	}
	if (worldPosition.x > 0.0f)
		worldPosition.x = 0.0f;
	if (worldPosition.y > 0.0f)
		worldPosition.y = 0.0f;
}

bool GameLoop(float deltaTime)
{
	return Tick(deltaTime);
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	S::GameApplication myApp;

	myApp.Initialize(hInstance, "sconfig.json");

	while (myApp.IsRunning())
	{
		myApp.Update(GameLoop);
	}

	myApp.Terminate();

	return 0;
}