#include <GameEngine/Inc/GameEngine.h>
#include <AI/Inc/AI.h>

#define MAXCOLLISIONLINES 8
#define MAXCOLLISIONINPUT MAXCOLLISIONLINES + 4

class Car
{
public:
	void Load(float s, float gapDis, float max, S::Math::Vector2 d);

	void Spawn(S::Math::Vector2 p);
	void SetDestination(S::Math::Vector2 d);
	void Reset();
	void ResetCollisionLines();
	void Update(float deltaTime);

	void Render(S::Math::Vector2 p);

	const S::Geometry::Circle GetCircle() const;
	const int GetDestinationIndex() const	{ return mDestinationIndex; }

	void TurnLeft();
	void TurnRight();
	void Accelerate();
	void Decelerate();
	void ResetAction();
	float TurnLeftRate();
	float TurnRightRate();

	void IncreaseDestinationIndex()			{ mDestinationIndex++; }
	void ResetDestinationIndex()			{ mDestinationIndex = 0; }
	void Kill()								{ mIsAlive = false; }
	bool IsAlive() const					{ return mIsAlive; }
	bool IsArrived();
	void CheckMapCollision(S::TileMap& tileMap);

	std::unique_ptr<S::AI::NEAT::NeuralNet> brain;
	size_t fitness{ 0 };

private:
	S::Math::Vector2 mPosition;
	S::Math::Vector2 mDirection{ 1.0f,0.0f };
	S::Math::Vector2 mDestination;
	int mDestinationIndex{ 0 };
	float mSpeed;
	float mAcceleration;
	float mGapDistance;
	float mMaxSpeed;

	float mAngle;
	float mAngleRate{ 0.0f };
	float mRadius{ 15.0f };
	float mStopCountDown{ 5.0f };
	float mCollisionLength{ 200.0f };
	float mCountDown{ 5.0f };

	bool mIsAlive{ true };
	std::vector<S::Math::Vector2> mCollisionDirection;
	std::vector<double> mCollisionInput;

	S::Graphics::TextureId mTexture;
};