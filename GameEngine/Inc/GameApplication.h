#ifndef INCLUDED_GAMEENGINE_GAMEAPPLICATION_H
#define INCLUDED_GAMEENGINE_GAMEAPPLICATION_H

namespace S {
class GameApplication :public Core::Application
{
public:
	GameApplication();
	~GameApplication() override;

private:
	void OnInitialize() override;
	void OnTerminate() override;
	void OnUpdate(bool(*GameLoop)(float)) override;

	Core::Window mWindow;
	Graphics::Camera mCamera;
	Core::Timer timer;
	bool mInitialized{ false };
};
} // namespace S

#endif // !INCLUDED_GAMEENGINE_GAMEAPPLICATION_H