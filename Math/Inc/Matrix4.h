#ifndef INCLUDED_MATH_MATRIX4_H
#define INCLUDED_MATH_MATRIX4_H

namespace S {
namespace Math {

struct Matrix4
{
	union 
	{
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		struct
		{
			float _1[4];
			float _2[4];
			float _3[4];
			float _4[4];
		};
		struct
		{
			Vector4 v[4];
		};
		struct
		{
			Vector4 v1;
			Vector4 v2;
			Vector4 v3;
			Vector4 v4;
		};
	};

	Matrix4()
		: _11{ 1.0f }, _12{ 0.0f }, _13{ 0.0f }, _14{ 0.0f }
		, _21{ 0.0f }, _22{ 1.0f }, _23{ 0.0f }, _24{ 0.0f }
		, _31{ 0.0f }, _32{ 0.0f }, _33{ 1.0f }, _34{ 0.0f }
		, _41{ 0.0f }, _42{ 0.0f }, _43{ 0.0f }, _44{ 1.0f }
	{}

	Matrix4(
		float _11, float _12, float _13, float _14,
		float _21, float _22, float _23, float _24,
		float _31, float _32, float _33, float _34,
		float _41, float _42, float _43, float _44)
		: _11{ _11 }, _12{ _12 }, _13{ _13 }, _14{ _14 }
		, _21{ _21 }, _22{ _22 }, _23{ _23 }, _24{ _24 }
		, _31{ _31 }, _32{ _32 }, _33{ _33 }, _34{ _34 }
		, _41{ _41 }, _42{ _42 }, _43{ _43 }, _44{ _44 }
	{}

	static Matrix4 Zero()									{ return {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}; }
	static Matrix4 Identity()								{ return {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f}; }
	static Matrix4 Translation(float x, float y, float z)	{ return {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, x, y, z, 1.0f}; }
	static Matrix4 Translation(Vector3 v)					{ return { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, v.x, v.y, v.z, 1.0f }; }
	static Matrix4 RotationX(float rad)						{ return { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, cosf(rad), sinf(rad), 0.0f, 0.0f, -sinf(rad), cosf(rad), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }; }
	static Matrix4 RotationY(float rad)						{ return { cosf(rad), 0.0f, -sinf(rad), 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, sinf(rad), 0.0f, cosf(rad), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }; }
	static Matrix4 RotationZ(float rad)						{ return { cosf(rad), sinf(rad), 0.0f, 0.0f, -sinf(rad), cosf(rad), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }; }
	static Matrix4 Scaling(float s)							{ return {s, 0.0f, 0.0f, 0.0f, 0.0f, s, 0.0f, 0.0f, 0.0f, 0.0f, s, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f}; }
	static Matrix4 Scaling(float sx, float sy, float sz)	{ return {sx, 0.0f, 0.0f, 0.0f, 0.0f, sy, 0.0f, 0.0f, 0.0f, 0.0f, sz, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f}; }
	static Matrix4 Scaling(Vector3 v)						{ return {v.x, 0.0f, 0.0f, 0.0f, 0.0f, v.y, 0.0f, 0.0f, 0.0f, 0.0f, v.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f}; }

	Matrix4 operator-() const
	{
		return {
			-_11, -_12, -_13, -_14,
			-_21, -_22, -_23, -_24,
			-_31, -_32, -_33, -_34,
			-_41, -_42, -_43, -_44 };
	}
	Matrix4 operator+(const Matrix4& rhs) const
	{
		return {
			_11 + rhs._11, _12 + rhs._12, _13 + rhs._13, _14 + rhs._14,
			_21 + rhs._21, _22 + rhs._22, _23 + rhs._23, _24 + rhs._24,
			_31 + rhs._31, _32 + rhs._32, _33 + rhs._33, _34 + rhs._34,
			_41 + rhs._41, _42 + rhs._42, _43 + rhs._43, _44 + rhs._44 };
	}
	Matrix4 operator-(const Matrix4& rhs) const
	{
		return {
			_11 - rhs._11, _12 - rhs._12, _13 - rhs._13, _14 - rhs._14,
			_21 - rhs._21, _22 - rhs._22, _23 - rhs._23, _24 - rhs._24,
			_31 - rhs._31, _32 - rhs._32, _33 - rhs._33, _34 - rhs._34,
			_41 - rhs._41, _42 - rhs._42, _43 - rhs._43, _44 - rhs._44 };
	}
	Matrix4 operator*(const Matrix4& rhs) const
	{
		return {
			(_11 * rhs._11) + (_12 * rhs._21) + (_13 * rhs._31) + (_14 * rhs._41),
			(_11 * rhs._12) + (_12 * rhs._22) + (_13 * rhs._32) + (_14 * rhs._42),
			(_11 * rhs._13) + (_12 * rhs._23) + (_13 * rhs._33) + (_14 * rhs._43),
			(_11 * rhs._14) + (_12 * rhs._24) + (_13 * rhs._34) + (_14 * rhs._44),

			(_21 * rhs._11) + (_22 * rhs._21) + (_23 * rhs._31) + (_24 * rhs._41),
			(_21 * rhs._12) + (_22 * rhs._22) + (_23 * rhs._32) + (_24 * rhs._42),
			(_21 * rhs._13) + (_22 * rhs._23) + (_23 * rhs._33) + (_24 * rhs._43),
			(_21 * rhs._14) + (_22 * rhs._24) + (_23 * rhs._34) + (_24 * rhs._44),

			(_31 * rhs._11) + (_32 * rhs._21) + (_33 * rhs._31) + (_34 * rhs._41),
			(_31 * rhs._12) + (_32 * rhs._22) + (_33 * rhs._32) + (_34 * rhs._42),
			(_31 * rhs._13) + (_32 * rhs._23) + (_33 * rhs._33) + (_34 * rhs._43),
			(_31 * rhs._14) + (_32 * rhs._24) + (_33 * rhs._34) + (_34 * rhs._44),

			(_41 * rhs._11) + (_42 * rhs._21) + (_43 * rhs._31) + (_44 * rhs._41),
			(_41 * rhs._12) + (_42 * rhs._22) + (_43 * rhs._32) + (_44 * rhs._42),
			(_41 * rhs._13) + (_42 * rhs._23) + (_43 * rhs._33) + (_44 * rhs._43),
			(_41 * rhs._14) + (_42 * rhs._24) + (_43 * rhs._34) + (_44 * rhs._44) };
	}
	Matrix4 operator*(float s) const
	{
		return {
			_11 * s, _12 * s, _13 * s, _14 * s,
			_21 * s, _22 * s, _23 * s, _24 * s,
			_31 * s, _32 * s, _33 * s, _34 * s,
			_41 * s, _42 * s, _43 * s, _44 * s };
	}
	Matrix4 operator/(float s) const
	{
		float t = 1.0f / s;
		return {
			_11 * t, _12 * t, _13 * t, _14 * t,
			_21 * t, _22 * t, _23 * t, _24 * t,
			_31 * t, _32 * t, _33 * t, _34 * t,
			_41 * t, _42 * t, _43 * t, _44 * t };
	}
	Matrix4& operator+=(const Matrix4& rhs)
	{
		_11 += rhs._11; _12 += rhs._12; _13 += rhs._13; _14 += rhs._14;
		_21 += rhs._21; _22 += rhs._22; _23 += rhs._23; _24 += rhs._24;
		_31 += rhs._31; _32 += rhs._32; _33 += rhs._33; _34 += rhs._34;
		_41 += rhs._41; _42 += rhs._42; _43 += rhs._43; _44 += rhs._44;
		return *this;
	}
};

} // namespace Math
} // namespace S

#endif // !INCLUDED_MATH_MATRIX4_H