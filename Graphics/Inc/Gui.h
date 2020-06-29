//====================================================================================================
// Filename:	Gui.h
// Created by:	Peter Chan
//====================================================================================================

#ifndef INCLUDED_GRAPHICS_GUI_H
#define INCLUDED_GRAPHICS_GUI_H

#include <External/ImGui/Inc/imgui.h>

namespace S {
namespace Graphics {
namespace Gui {

	void Initialize(HWND window);
	void Terminate();
	void BeginRender();
	void EndRender();

	inline ImVec2 ToImVec2(Math::Vector2 v) { return ImVec2{ v.x, v.y }; }
	inline ImVec4 ToImVec4(Math::Vector4 v) { return ImVec4{ v.x, v.y, v.z, v.w }; }

} // namespace Gui
} // namespace Graphics
} // namespace S

#endif // #ifndef INCLUDED_GRAPHICS_GUI_H