#ifndef INCLUDED_GEOMETRY_LINESEGMENT2D_H
#define INCLUDED_GEOMETRY_LINESEGMENT2D_H

namespace S {
namespace Geometry {

struct LineSegment2D
{
	Math::Vector2 from;
	Math::Vector2 to;

	LineSegment2D() : from{ 0.0f, 0.0f }, to{ 0.0f, 0.0f } {}
	LineSegment2D(float x0, float y0, float x1, float y1) : from{ x0, y0 }, to{ x1, y1 } {}
	LineSegment2D(const Math::Vector2& v0, const Math::Vector2& v1) : from{ v0 }, to{ v1 } {}
};

} // namespace Geometry
} // namespace S

#endif // !INCLUDED_GEOMETRY_LINESEGMENT2D_H