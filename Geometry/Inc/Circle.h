#ifndef INCLUDED_GEOMETRY_CIRCLE_H
#define INCLUDED_GEOMETRY_CIRCLE_H

namespace S {
namespace Geometry {

struct Circle
{
	Math::Vector2 center;
	float radius;

	Circle() : center{ 0.0f, 0.0f }, radius{ 1.0f } {}
	Circle(const Math::Vector2& center, float radius) : center{ center }, radius{ radius } {}
	Circle(float x, float y, float radius) : center{ x, y }, radius{ radius } {}
};

} // namespace Geometry
} // namespace S

#endif // !INCLUDED_GEOMETRY_CIRCLE_H