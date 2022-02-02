#ifndef INCLUDED_PHYSICS_CIRCLECOLLIDER_H
#define INCLUDED_PHYSICS_CIRCLECOLLIDER_H

namespace S {
namespace Physics {

class CircleCollider
{
public:
	CircleCollider(Math::Vector2* p, float r)
	{
		mPosition = p;
		mRadius = r;
	}

	void Update()
	{
		mPrevPosition = { mPosition->x,mPosition->y };
	}

	bool CheckPositionChanged()
	{
		return mPosition->x == mPrevPosition.x && mPosition->y == mPrevPosition.y;
	}

	void FixPosition()
	{
		mPosition->x = mPrevPosition.x;
		mPosition->y = mPrevPosition.y;
	}

	Geometry::Circle GetCircle()
	{
		return Geometry::Circle{ {mPosition->x,mPosition->y},mRadius };
	}

	Geometry::Circle GetCircle(Math::Vector2 p, float scale)
	{
		return Geometry::Circle{ {mPosition->x*scale + p.x,mPosition->y*scale + p.y},mRadius*scale };
	}

protected:
	friend class ColliderManager;

	Math::Vector2 mPrevPosition;
	float mRadius{ 0.0f };
	Math::Vector2* mPosition;

};

} // namespace Physics
} // namespace S

#endif // !INCLUDED_PHYSICS_CIRCLECOLLIDER_H