#ifndef INCLUDED_GEOMETRY_RAY_H
#define INCLUDED_GEOMETRY_RAY_H

namespace S {
namespace Geometry {

struct Ray
{
	Math::Vector3 org;
	Math::Vector3 dir;

	Ray() : org{ 0.0f, 0.0f, 0.0f }, dir{ 0.0f, 0.0f, 1.0f } {}
	Ray(float x, float y, float z, float dx, float dy, float dz) : org{ x, y, z }, dir{ dx, dy, dz } {}
	Ray(const Math::Vector3& org, const Math::Vector3& dir) : org{ org }, dir{ dir } {}
};

} // namespace Geometry
} // namespace S

#endif // !INCLUDED_GEOMETRY_RAY_H