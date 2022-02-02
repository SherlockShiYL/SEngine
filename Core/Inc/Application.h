#ifndef INCLUDED_CORE_APPLICATION_H
#define INCLUDED_CORE_APPLICATION_H

namespace S {
namespace Core {

class Application
{
public:
	Application();
	virtual ~Application();

	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	void Initialize(HINSTANCE instance, LPCSTR configFileName);
	void Terminate();

	void HookWindow(HWND hWnd);
	void UnhookWindow();

	void Update(bool(*GameLoop)(float));

	void Kill()								{ mRunning = false; }

	HINSTANCE GetInstance() const			{ return mInstance; }
	HWND GetWindow() const					{ return mWindow; }
	const char* GetConfigFileName() const	{ return mConfigFileName.c_str(); }
	bool IsRunning() const					{ return mRunning; }

private:
	virtual void OnInitialize() = 0;
	virtual void OnTerminate() = 0;
	virtual void OnUpdate(bool (*GameLoop)(float)) = 0;

	HINSTANCE mInstance;
	HWND mWindow;
	std::string mConfigFileName;
	bool mRunning;
};

} // namespace Core
} // namespace S

#endif // !INCLUDED_CORE_APPLICATION_H