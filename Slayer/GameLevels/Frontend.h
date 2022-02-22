#ifndef INCLUDED_SLAYER_FRONTEND_H
#define INCLUDED_SLAYER_FRONTEND_H

#include "GameState.h"

class Frontend : public GameState
{
public:
	Frontend();
	~Frontend() override;

	void Load() override;
	void Unload() override;
	Transition Update(float deltaTime) override;
	void Render() override;

private:
	S::Input::InputSystem* input{ nullptr };
	S::MenuManager* menuManager;


};

#endif //#define INCLUDED_SLAYER_FRONTEND_H

