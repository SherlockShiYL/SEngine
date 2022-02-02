#ifndef INCLUND_SLAYER_FRONTEND_H
#define INCLUND_SLAYER_FRONTEND_H

#include "GameState.h"
#include "../MenuManager.h"

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
	MenuManager menuManager;

};

#endif //#define INCLUND_SLAYER_FRONTEND_H

