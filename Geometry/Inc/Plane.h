#ifndef INCLUDED_GEOMETRY_PLANE_H
#define INCLUDED_GEOMETRY_PLANE_H

namespace S {
namespace Geometry {

struct Plane
{
	Math::Vector3 n;
	float d;

	Plane() : n{ 0.0f, 1.0f, 0.0f }, d{ 0.0f } {}
	Plane(float a, float b, float c, float d) : n{ a, b, c }, d{ d } {}
};

} // namespace Geometry
} // namespace S

#endif // !INCLUDED_GEOMETRY_PLANE_H