#ifndef INCLUDED_ENGINECODETEST_QUADTREESCENE_H
#define INCLUDED_ENGINECODETEST_QUADTREESCENE_H

#include <GameEngine/Inc/GameEngine.h>
#include "GameState.h"
#include "QuadTree.h"
#include "RedArrow.h"

class QuadtreeScene final : public GameState
{
public:
	void Load() override;
	void Unload() override;
	Transition Update(float deltaTime) override;
	void Render() override;

	void ResetCount();
	void SetMode(int index);
	void SetMouseArea();

private:
	S::Input::InputSystem* input{ nullptr };
	int mode{ 0 };
	int prevMode{ 0 };
	int mRunCount{ 0 };
	float mQuadtreeTotalTimeCost{ 0.0f };
	float mQuadtreeAverageTimeCost{ 0.0f };
	float mQuadrantTotalTimeCost{ 0.0f };
	float mQuadrantAverageTimeCost{ 0.0f };
	S::Geometry::Rect mMouseRect;
	S::Geometry::Circle mMouseCircle;
	//QuadTree mQuadtree{ {0.0f,0.0f,1280.0f,720.0f}, 4u };

	//std::vector<S::Geometry::Circle> mPoints;
	S::AI::AIWorld aiWorld;
	S::AI::AIQuadtree mQuadtree;
	std::vector<RedArrow*> redArrow;
	S::AI::AgentList rectCollidedList;
	S::AI::AgentList circleCollidedList;
	S::AI::AgentList quadrantCollidedList;
};

#endif // !INCLUDED_ENGINECODETEST_QUADTREESCENE_H
