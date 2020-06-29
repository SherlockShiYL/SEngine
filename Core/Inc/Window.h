#ifndef INCLUDED_CORE_WINDOW_H
#define INCLUDED_CORE_WINDOW_H

namespace S {
namespace Core {

class Window
{
public:
	Window();
	~Window();

	void Initialize(HINSTANCE instance, LPCSTR appName, uint32_t width, uint32_t height);
	void Terminate();

	const uint32_t GetScreenWidth() const;
	const uint32_t GetScreenHeight() const;

	bool ProcessMessage();

	HWND GetWindowHandle() const { return mWindow; }

private:
	HINSTANCE mInstance;
	HWND mWindow;
	std::string mAppName;

	uint32_t mScreenWidth;
	uint32_t mScreenHeight;
};

} // namespace Core
} // namespace S

using WindowMessageHandler = LRESULT(CALLBACK*)(HWND, UINT, WPARAM, LPARAM);

#endif // !INCLUDED_CORE_WINDOW_H