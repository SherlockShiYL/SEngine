#ifndef INCLUDED_MATH_VECTOR4_H
#define INCLUDED_MATH_VECTOR4_H

namespace S {
namespace Math {

struct Vector4
{
	union
	{
		struct
		{
			float x, y, z, w;
		};
		struct
		{
			float f[4];
		};
	};
	constexpr Vector4() : x{ 0.0f }, y{ 0.0f }, z{ 0.0f }, w{ 0.0f } {}
	constexpr Vector4(float f) :Vector4{ f,f,f,f } {}
	constexpr Vector4(float floatX, float floatY, float floatZ, float floatW) :x{ floatX }, y{ floatY }, z{ floatZ }, w{ floatW } {}
	constexpr Vector4(const Vector3& v, float w) : x{ v.x }, y{ v.y }, z{ v.z }, w{ w }{}

	constexpr Vector4 operator-() const					{ return {-x, -y, -z, -w}; }
	constexpr Vector4 operator+(const Vector4& u) const	{ return { x + u.x,y + u.y,z + u.z,w + u.w }; }
	constexpr Vector4 operator-(const Vector4& u) const	{ return { x - u.x,y - u.y,z - u.z,w - u.w }; }
	constexpr Vector4 operator*(float s) const			{ return { x * s,y * s,z * s,w * s }; }
	constexpr Vector4& operator+=(const Vector4& u)		{ x += u.x; y += u.y; z += u.z; w += u.w; return *this; }
	constexpr Vector4& operator-=(const Vector4& u)		{ x -= u.x; y -= u.y; z -= u.z; w -= u.w; return *this; }
	constexpr Vector4& operator*=(float s)				{ x *= s; y *= s; z *= s; w *= s; return *this; }
	constexpr Vector4& operator/=(float s)				{ x /= s; y /= s; z /= s; w /= s; return *this; }
	Vector4 operator/(float s) const
	{
		ASSERT(s != 0.0f, "Cannot divide by 0");
		return { x / s ,y / s ,z / s, w / s }; 
	}

	static Vector4 Black()						{ return {0.0f, 0.0f, 0.0f, 1.0f}; }
	static Vector4 Gray()						{ return {0.5f, 0.5f, 0.5f, 1.0f}; }
	static Vector4 DarkGray()					{ return { 0.25f, 0.25f, 0.25f, 1.0f }; }
	static Vector4 White()						{ return {1.0f, 1.0f, 1.0f, 1.0f}; }
	static Vector4 LightRed()					{ return {1.0f, 0.5f, 0.5f, 1.0f}; }
	static Vector4 Red()						{ return {1.0f, 0.0f, 0.0f, 1.0f}; }
	static Vector4 LightGreen()					{ return {0.5f, 1.0f, 0.5f, 1.0f}; }
	static Vector4 Green()						{ return {0.0f, 1.0f, 0.0f, 1.0f}; }
	static Vector4 LightBlue()					{ return {0.5f, 0.5f, 1.0f, 1.0f}; }
	static Vector4 Blue()						{ return {0.0f, 0.0f, 1.0f, 1.0f}; }
	static Vector4 DarkBlue()					{ return {0.0f, 0.0f, 0.5f, 1.0f}; }
	static Vector4 Yellow()						{ return {1.0f, 1.0f, 0.0f, 1.0f}; }
	static Vector4 Orange()						{ return {1.0f, 0.5f, 0.0f, 1.0f}; }
	static Vector4 Cyan()						{ return {0.0f, 1.0f, 1.0f, 1.0f}; }
	static Vector4 Magenta()					{ return {1.0f, 0.0f, 1.0f, 1.0f}; }

	void SetEachElement(const float data)
	{
		x = data;
		y = data;
		z = data;
		w = data;
	}
};

} // namespace Math
} // namespace S

#endif // !INCLUDED_MATH_VECTOR4_H