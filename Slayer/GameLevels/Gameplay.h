#ifndef INCLUND_SLAYER_GAMEPLAY_H
#define INCLUND_SLAYER_GAMEPLAY_H

#define DAMAGEDISPLAYNUM 200

#include "GameState.h"
#include "../DamageDisplay.h"
#include "../Command.h"

class GameObjectManager;

class Gameplay : public GameState
{
public:
	Gameplay();
	~Gameplay() override;

	void Load() override;
	void Unload() override;
	Transition Update(float deltaTime) override;
	void Render() override;

	void PlayerInput(); // Send Input as well
	void RunDamageDisplay(float deltaTime, S::Math::Vector2 position);
	void ScaleUpdate(float deltaTime);
	//float GetDistance(AI::Graph::Coord c1, AI::Graph::Coord c2);

private:
	S::Math::Vector2 mWorldPosition{ -500.0f,-500.0f };

	uint8_t buffer[16384];
	int bytesReceived{ 0 };

	S::TileMap tileMap;
	float mWorldScale{ 1.0f };
	float mWorldTargetScale{ 1.0f };

	int treesX[10];
	int mData{ 0 };
	uint32_t mClientID{ 0 };

	DamageDisplay mDamageDisplay[DAMAGEDISPLAYNUM];
	uint32_t mDamageDisplaySize{ 0 };
	Command sCommand;

	/*AI::EntityList entityList;
	TinyBots tinyBot;
	Storages storages;
	Trees trees;
	Beds beds;
	Boilers boilers;*/

	bool test{ false };

	GameObjectManager* instance{ nullptr };
	S::Input::InputSystem* input{ nullptr };
};

#endif //#define INCLUND_SLAYER_GAMEPLAY_H