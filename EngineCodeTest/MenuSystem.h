#ifndef INCLUDED_ENGINECODETEST_MENUSYSTEM_H
#define INCLUDED_ENGINECODETEST_MENUSYSTEM_H

#include <GameEngine/Inc/GameEngine.h>
#include "GameState.h"

class MenuSystem final : public GameState
{
public:
	void Load() override;
	void Unload() override;
	Transition Update(float deltaTime) override;
	void Render() override;


private:
	S::Input::InputSystem* input{ nullptr };
	S::MenuManager* menuManager{ nullptr };

	/*std::unique_ptr<S::Menu> mTLMenu;
	std::unique_ptr<S::Menu> mTCMenu;
	std::unique_ptr<S::Menu> mTRMenu;
	std::unique_ptr<S::Menu> mCLMenu;
	std::unique_ptr<S::Menu> mCCMenu;
	std::unique_ptr<S::Menu> mCRMenu;
	std::unique_ptr<S::Menu> mBLMenu;
	std::unique_ptr<S::Menu> mBCMenu;
	std::unique_ptr<S::Menu> mBRMenu;*/
};

#endif // !INCLUDED_ENGINECODETEST_QUADTREESCENE_H
