#ifndef INCLUDED_MATH_QUATERNION_H
#define INCLUDED_MATH_QUATERNION_H

namespace S {
namespace Math {

struct Quaternion
{
	float x, y, z, w;

	Quaternion() : x{ 0.0f }, y{ 0.0f }, z{ 0.0f }, w{ 1.0f } {}
	Quaternion(float x, float y, float z, float w) : x{ x }, y{ y }, z{ z }, w{ w } {}

	static Quaternion Zero()							{ return Quaternion{ 0.0f, 0.0f, 0.0f, 0.0f }; }
	static Quaternion Identity()						{ return Quaternion{ 0.0f, 0.0f, 0.0f, 1.0f }; }

	Quaternion operator+(const Quaternion& rhs) const	{ return Quaternion{ x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w }; }
	Quaternion operator-(const Quaternion& rhs) const	{ return Quaternion{ x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w }; }
	Quaternion operator*(const Quaternion& rhs) const
	{
		return Quaternion(w*rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
						w*rhs.y + y * rhs.w + z * rhs.x - x * rhs.z,
						w*rhs.z + z * rhs.w + x * rhs.y - y * rhs.x,
						w*rhs.w - x * rhs.x - y*rhs.y - z * rhs.z);
	}
	Quaternion operator*(float s) const					{ return Quaternion{ x * s, y * s, z * s, w * s }; }
	Quaternion operator/(float s) const					{ return Quaternion{ x / s, y / s, z / s, w / s }; }
};

} // namespace Math
} // namespace S

#endif // !INCLUDED_MATH_QUATERNION_H