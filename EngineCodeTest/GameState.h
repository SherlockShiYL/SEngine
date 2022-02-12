#ifndef INCLUND_ENGINECODETEST_GAMESTATE_H
#define INCLUND_ENGINECODETEST_GAMESTATE_H

#include <GameEngine/Inc/GameEngine.h>
#include <AI/Inc/AI.h>

enum class Transition
{
	None,
	GoToFrontend, // Menu
	GoToPathFindingScene,
	GoToQuadtree,
	QuitGame
};

class GameState
{
public:
	virtual ~GameState(){}

	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual Transition Update(float deltaTime) = 0;
	virtual void Render() = 0;

private:

};

#endif //#ifndef INCLUND_ENGINECODETEST_GAMESTATE_H