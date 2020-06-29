#ifndef INCLUDED_GEOMETRY_OBB_H
#define INCLUDED_GEOMETRY_OBB_H

namespace S {
namespace Geometry {

struct OBB
{
	Math::Vector3 center{ 0.0f, 0.0f, 0.0f };
	Math::Vector3 extend{ 1.0f, 1.0f, 1.0f };
	Math::Quaternion orientation{ 0.0f, 0.0f, 0.0f, 1.0f };

	OBB()
		: center{ 0.0f, 0.0f, 0.0f }
		, extend{ 1.0f, 1.0f, 1.0f }
		, orientation{ 0.0f, 0.0f, 0.0f, 1.0f }
	{}

	OBB(float x, float y, float z, float sx, float sy, float sz)
		: center{ x, y, z }
		, extend{ sx, sy, sz }
		, orientation{ 0.0f, 0.0f, 0.0f, 1.0f }
	{}

	OBB(float x, float y, float z, float sx, float sy, float sz, const Math::Quaternion& orientation)
		: center{ x, y, z }
		, extend{ sx, sy, sz }
		, orientation{ orientation }
	{}

	OBB(const Math::Vector3& center, const Math::Vector3& extend, const Math::Quaternion& orientation)
		: center{ center }
		, extend{ extend }
		, orientation{ orientation }
	{}

	/*OBB(float x, float y, float z, float sx, float sy, float sz, float ax, float ay, float az, float angle)
		: center{ x, y, z }
		, extend{ sx, sy, sz }
		, orientation{ Quaternion::RotationAxis(Math::Vector3(ax, ay, az), angle) }
	{}*/
};

} // namespace Geometry
} // namespace S

#endif // !INCLUDED_GEOMETRY_OBB_H