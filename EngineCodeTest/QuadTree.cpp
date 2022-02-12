#include "QuadTree.h"

using namespace S;

QuadTree::QuadTree(Geometry::Rect rect, uint32_t capacity)
	:mRect{ rect }, mCapacity{ capacity }
{
	mPoints.reserve(capacity);
}

void QuadTree::Subdivide()
{
	float halfWidth = (mRect.right - mRect.left) / 2 + mRect.left;
	float halfHeight = (mRect.bottom - mRect.top) / 2 + mRect.top;

	mTL = new QuadTree({ mRect.left, mRect.top, halfWidth, halfHeight }, mCapacity);
	mTR = new QuadTree({ halfWidth, mRect.top, mRect.right, halfHeight }, mCapacity);
	mBL = new QuadTree({ mRect.left, halfHeight, halfWidth, mRect.bottom }, mCapacity);
	mBR = new QuadTree({ halfWidth, halfHeight, mRect.right, mRect.bottom }, mCapacity);
}

bool QuadTree::Insert(S::Geometry::Circle& c)
{
	if (!S::Geometry::PointInRect(c.center, mRect))
		return false;

	if (mPoints.size() < mCapacity)
	{
		mPoints.emplace_back(c);
		return true;
	}
	else
	{
		if (!mTL)
		{
			Subdivide();
		}
		if (mTL->Insert(c))
		{
			return true;
		}
		else if (mTR->Insert(c))
		{
			return true;
		}
		else if (mBL->Insert(c))
		{
			return true;
		}
		else if (mBR->Insert(c))
		{
			return true;
		}
		return false;
	}
}

void QuadTree::QueryRange(Geometry::Rect rect, std::vector<S::Geometry::Circle>& result)
{
	if (!Geometry::Intersect(mRect, rect))
	{
		return;
	}

	for (uint32_t i = 0; i < mPoints.size(); i++)
	{
		if (Geometry::PointInRect(mPoints[i].center, rect))
		{
			result.emplace_back(mPoints[i]);
		}
	}

	if (mTL)
	{
		mTL->QueryRange(rect, result);
		mTR->QueryRange(rect, result);
		mBL->QueryRange(rect, result);
		mBR->QueryRange(rect, result);
	}

}

void QuadTree::Render(S::Math::Vector2 p)
{
	Graphics::DrawScreenRect(mRect + p, Math::Vector4::Gray());
	for (uint32_t i = 0; i < mPoints.size(); i++)
	{
		Graphics::DrawScreenCircle({ mPoints[i].center + p,mPoints[i].radius }, Math::Vector4::Magenta());
	}
	if (mTL)
	{
		mTR->Render();
		mTL->Render();
		mBR->Render();
		mBL->Render();
	}
}
