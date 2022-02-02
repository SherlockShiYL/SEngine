#ifndef INCLUDED_SLAYER_EDITMAP_H
#define INCLUDED_SLAYER_EDITMAP_H

#include "GameState.h"

class EditMap : public GameState
{
public:
	EditMap();
	~EditMap() override;

	void Load() override;
	void Unload() override;
	Transition Update(float deltaTime) override;
	void Render() override;

private:
	S::TileMap tileMap;
	S::Math::Vector2 mPosition{ 0.0f,0.0f };
	int tileIndex{ 0 };
	const char* level = "Data/Maps/ForestMap.txt";
	S::Input::InputSystem* input{ nullptr };
};


#endif // !INCLUDED_SLAYER_EDITMAP_H