#ifndef INCLUDED_MATH_VECTOR3_H
#define INCLUDED_MATH_VECTOR3_H

namespace S {
namespace Math {

struct Vector3
{
	union
	{
		struct
		{
			float x, y, z;
		};
		struct
		{
			float f[3];
		};
	};
	constexpr Vector3() :x{ 0.0f }, y{ 0.0f }, z{ 0.0f } {}
	constexpr Vector3(float f) : Vector3{ f,f,f } {}
	constexpr Vector3(float floatX, float floatY, float floatZ) :x{ floatX }, y{ floatY }, z{ floatZ } {}

	static Vector3 Zero()	{ return Vector3(); }
	static Vector3 One()	{ return Vector3(1.0f, 1.0f, 1.0f); }
	static Vector3 XAxis()	{ return Vector3(1.0f, 0.0f, 0.0f); }
	static Vector3 YAxis()	{ return Vector3(0.0f, 1.0f, 0.0f); }
	static Vector3 ZAxis()	{ return Vector3(0.0f, 0.0f, 1.0f); }

	constexpr Vector3 operator-() const					{ return {-x, -y, -z}; }
	constexpr Vector3 operator+(const Vector3& u) const	{ return { x + u.x,y + u.y,z + u.z }; }
	constexpr Vector3 operator-(const Vector3& u) const	{ return { x - u.x,y - u.y,z - u.z }; }
	constexpr Vector3 operator*(float s) const			{ return { x * s,y * s,z * s }; }
	constexpr Vector3& operator+=(const Vector3& u)		{ x += u.x; y += u.y; z += u.z; return *this; }
	constexpr Vector3& operator-=(const Vector3& u)		{ x -= u.x; y -= u.y; z -= u.z; return *this; }
	constexpr Vector3& operator*=(float s)				{ x *= s; y *= s; z *= s; return *this; }
	constexpr Vector3& operator/=(float s)				{ x /= s; y /= s; z /= s; return *this; }
	Vector3 operator/(float s) const
	{
		ASSERT(s != 0.0f, "Cannot divide by 0");
		return { x / s ,y / s ,z / s }; 
	}
};

} // namespace Math
} // namespace S

#endif // !INCLUDED_MATH_VECTOR3_H
