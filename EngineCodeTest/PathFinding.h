#ifndef INCLUND_ENGINECODETEST_GAMEPLAY_H
#define INCLUND_ENGINECODETEST_GAMEPLAY_H

#include <GameEngine/Inc/GameEngine.h>
#include <AI/Inc/AI.h>
#include "GameState.h"

class GameObjectManager;

class PathFindingScene : public GameState
{
public:
	PathFindingScene();
	~PathFindingScene() override;

	void Load() override;
	void Unload() override;
	Transition Update(float deltaTime) override;
	void Render() override;

	void PlayerInput();
	void ScaleUpdate(float deltaTime);


private:
	void ResetCount();

	S::Input::InputSystem* input{ nullptr };

	bool mStartPoint = false, mEndPoint = false;
	int mMode{ 0 };
	int mRunCount{ 0 };
	int mTileIndex{ 0 };
	S::AI::Graph::Coord mStartCoord, mEndCoord;
	float mTotalTimeCost{ 0.0f };
	float mAverageTimeCost{ 0.0f };

	S::Graphics::TextureId mStartPointTexture;
	S::Graphics::TextureId mEndPointTexture;

	float mWorldScale{ 1.0f };
	float mWorldTargetScale{ 1.0f };

	S::Math::Vector2 mWorldPosition;

	S::AI::Graph mGraph;
	S::TileMap mTileMap;

};

#endif //#define INCLUND_ENGINECODETEST_GAMEPLAY_H