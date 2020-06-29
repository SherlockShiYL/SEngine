#ifndef INCLUDED_GEOMETRY_SPHERE_H
#define INCLUDED_GEOMETRY_SPHERE_H

namespace S {
namespace Geometry {

struct Sphere
{
	Math::Vector3 center;
	float radius;

	Sphere() : center{ 0.0f, 0.0f, 0.0f }, radius{ 1.0f } {}
	Sphere(float x, float y, float z, float radius) : center{ x, y, z }, radius{ radius } {}
	Sphere(const Math::Vector3& center, float radius) : center{ center }, radius{ radius } {}
};

} // namespace Geometry
} // namespace S

#endif // !INCLUDED_GEOMETRY_SPHERE_H