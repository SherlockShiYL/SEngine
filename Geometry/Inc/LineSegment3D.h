#ifndef INCLUDED_GEOMETRY_LINESEGMENT3D_H
#define INCLUDED_GEOMETRY_LINESEGMENT3D_H

namespace S {
namespace Geometry {

struct LineSegment3D
{
	Math::Vector3 from;
	Math::Vector3 to;

	LineSegment3D() : from{ 0.0f, 0.0f, 0.0f }, to{ 0.0f, 0.0f, 0.0f } {}
	LineSegment3D(float x0, float y0, float z0, float x1, float y1, float z1) : from{ x0, y0, z0 }, to{ x1, y1, z1 } {}
	LineSegment3D(const Math::Vector3& v0, const Math::Vector3& v1) : from{ v0 }, to{ v1 } {}
};

} // namespace Geometry
} // namespace S

#endif // !INCLUDED_GEOMETRY_LINESEGMENT3D_H