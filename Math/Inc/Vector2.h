#ifndef INCLUDED_MATH_VECTOR2_H
#define INCLUDED_MATH_VECTOR2_H

namespace S {
namespace Math {

struct Vector2
{
	float x{ 0.0f }, y{ 0.0f };
	constexpr Vector2() {}
	constexpr Vector2(float f) :Vector2{ f,f } {}
	constexpr Vector2(float floatX, float floatY) :x{ floatX }, y{ floatY }{}

	static Vector2 Zero()	{ return Vector2(); }
	static Vector2 One()	{ return Vector2(1.0f, 1.0f); }
	static Vector2 XAxis()	{ return Vector2(1.0f, 0.0f); }
	static Vector2 YAxis()	{ return Vector2(0.0f, 1.0f); }

	constexpr Vector2 operator-() const					{ return {-x, -y}; }
	constexpr Vector2 operator+(const Vector2& u) const	{ return { x + u.x,y + u.y }; }
	constexpr Vector2 operator-(const Vector2& u) const	{ return { x - u.x,y - u.y }; }
	constexpr Vector2 operator*(float s) const			{ return { x * s, y * s }; }
	constexpr Vector2& operator+=(const Vector2& u)		{ x += u.x; y += u.y; return *this; }
	constexpr Vector2& operator-=(const Vector2& u)		{ x -= u.x; y -= u.y; return *this; }
	constexpr Vector2& operator*=(float s)				{ x *= s; y *= s; return *this; }
	constexpr Vector2& operator/=(float s)				{ x /= s; y /= s; return *this; }
	Vector2 operator/(float s) const
	{
		ASSERT(s != 0.0f, "Cannot divide by 0");
		return { x / s ,y / s };
	}
};

} // namespace Math
} // namespace S

#endif // !INCLUDED_MATH_VECTOR2_H