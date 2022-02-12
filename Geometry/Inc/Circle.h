#ifndef INCLUDED_GEOMETRY_CIRCLE_H
#define INCLUDED_GEOMETRY_CIRCLE_H

namespace S {
namespace Geometry {

struct Circle
{
	Math::Vector2 center;
	float radius;

	constexpr Circle() : center{ 0.0f, 0.0f }, radius{ 1.0f } {}
	constexpr Circle(const Math::Vector2& center, float radius) : center{ center }, radius{ radius } {}
	constexpr Circle(float x, float y, float radius) : center{ x, y }, radius{ radius } {}

	constexpr Circle operator+(Math::Vector2 v) const	{ return Circle{ {center.x + v.x,center.y + v.y},radius }; }
};

} // namespace Geometry
} // namespace S

#endif // !INCLUDED_GEOMETRY_CIRCLE_H