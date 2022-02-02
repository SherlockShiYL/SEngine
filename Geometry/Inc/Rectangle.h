#ifndef INCLUDED_GEOMETRY_RECTANGLE_H
#define INCLUDED_GEOMETRY_RECTANGLE_H

namespace S {
namespace Geometry {

struct Rect
{
	float left{ 0.0f }, top{ 0.0f }, right{ 1.0f }, bottom{ 1.0f };

	constexpr Rect() : left{ 0.0f }, top{ 0.0f }, right{ 1.0f }, bottom{ 1.0f } {}
	constexpr Rect(float left, float top, float right, float bottom) : left{ left }, top{ top }, right{ right }, bottom{ bottom } {}

	constexpr Rect operator*(float s) const { return { left * s, top * s, right * s, bottom * s }; }
};


} // namespace Geometry
} // namespace S

#endif // !INCLUDED_GEOMETRY_RECTANGLE_H