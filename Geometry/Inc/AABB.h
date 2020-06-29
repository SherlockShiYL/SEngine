#ifndef INCLUDED_GEOMETRY_AABB_H
#define INCLUDED_GEOMETRY_AABB_H

namespace S {
namespace Geometry {

struct AABB
{
	Math::Vector3 center;
	Math::Vector3 extend;

	AABB() : center{ 0.0f, 0.0f, 0.0f }, extend{ 0.0f, 0.0f, 0.0f } {}
	AABB(const Math::Vector3& center, const Math::Vector3& extend) : center{ center }, extend{ extend } {}

	static AABB FromMinMax(const Math::Vector3& min, const Math::Vector3& max) { return { (min + max) * 0.5f, (max - min) * 0.5f }; }

	void operator+=(const Math::Vector3& point)
	{
		// Contain x
		if (point.x < center.x - extend.x)
		{
			float max = center.x + extend.x;
			center.x = (max + point.x) * 0.5f;
			extend.x = max - center.x;
		}
		else if (point.x > center.x + extend.x)
		{
			float min = center.x - extend.x;
			center.x = (min + point.x) * 0.5f;
			extend.x = center.x - min;
		}

		// Contain y
		if (point.y < center.y - extend.y)
		{
			float max = center.y + extend.y;
			center.y = (max + point.y) * 0.5f;
			extend.y = max - center.y;
		}
		else if (point.y > center.y + extend.y)
		{
			float min = center.y - extend.y;
			center.y = (min + point.y) * 0.5f;
			extend.y = center.y - min;
		}

		// Contain z
		if (point.z < center.z - extend.z)
		{
			float max = center.z + extend.z;
			center.z = (max + point.z) * 0.5f;
			extend.z = max - center.z;
		}
		else if (point.z > center.z + extend.z)
		{
			float min = center.z - extend.z;
			center.z = (min + point.z) * 0.5f;
			extend.z = center.z - min;
		}
	}
};

} // namespace Geometry
} // namespace S

#endif // !INCLUDED_GEOMETRY_AABB_H