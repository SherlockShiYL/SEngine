#ifndef INCLUDED_ENGINECODETEST_QUADTREE_H
#define INCLUDED_ENGINECODETEST_QUADTREE_H

#include <GameEngine/Inc/GameEngine.h>

class QuadTree
{
public:
	QuadTree(S::Geometry::Rect rect, uint32_t capacity);

	void Subdivide();

	bool Insert(S::Geometry::Circle& c);
	void QueryRange(S::Geometry::Rect rect, std::vector<S::Geometry::Circle>& result);

	void Render(S::Math::Vector2 p = { 0.0f,0.0f });

private:
	uint32_t mCapacity;
	S::Geometry::Rect mRect;
	std::vector<S::Geometry::Circle> mPoints;
	QuadTree* mTL{ nullptr };
	QuadTree* mTR{ nullptr };
	QuadTree* mBL{ nullptr };
	QuadTree* mBR{ nullptr };

};

#endif // !INCLUDED_ENGINECODETEST_QUADTREE_H