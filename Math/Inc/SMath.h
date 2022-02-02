#ifndef INCLUDED_MATH_H
#define INCLUDED_MATH_H

#include "Common.h"

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"
#include "Matrix3.h"
#include "Matrix4.h"
#include "PerlinNoise.h"
#include "Core/Inc/Meta.h"

namespace S {
namespace Math {

static std::random_device myRandomDevice{};
static std::mt19937 myRandomEngine{ myRandomDevice() };

extern const float kPi;
extern const float kTwoPi;
extern const float kPiByTwo;
extern const float kRootTwo;
extern const float kRootThree;
extern const float kDegToRad;
extern const float kRadToDeg;

template<typename T> inline T Min(T a, T b)							{ return(a < b) ? a : b; }
template<typename T> inline T Max(T a, T b)							{ return(a > b) ? a : b; }
template<typename T> inline T Clamp(T value, T min, T max)			{ return Max(min, Min(max, value)); }

inline float Abs(float value)										{ return value >= 0.0f ? value : -value; }
inline float Sign(float value)										{ return value >= 0.0f ? 1.0f : -1.0f; }
inline float Sqr(float value)										{ return value * value; }
inline float Sqrt(float value)										{ return sqrtf(value); }

//inline bool Compare(float a, float b, float epsilon = FLT_MIN)	{ return Abs(a - b) <= epsilon; }
inline bool Compare(float a, float b, float epsilon = 0.0000003f)	{ return Abs(a - b) <= epsilon; }
inline bool Compare(Vector2 u, Vector2 v)							{ return Compare(u.x, v.x) && Compare(u.y, v.y); }
inline bool Compare(Vector3 u, Vector3 v)							{ return Compare(u.x, v.x) && Compare(u.y, v.y) && Compare(u.z, v.z); }
inline bool Compare(Vector4 u, Vector4 v)							{ return Compare(u.x, v.x) && Compare(u.y, v.y) && Compare(u.z, v.z) && Compare(u.w, v.w); }

inline bool IsZero(float value)										{ return Compare(value, 0.0f); }
inline bool IsZero(const Vector2& v)								{ return IsZero(v.x) && IsZero(v.y); }
inline bool IsZero(const Vector3& v)								{ return IsZero(v.x) && IsZero(v.y) && IsZero(v.z); }

inline Vector2 PerpendicularLH(const Vector2& v)					{ return { -v.y,v.x }; }
inline Vector2 PerpendicularRH(const Vector2& v)					{ return { v.y,-v.x }; }

inline float MagnitudeSqr(const Vector2& v)							{ return (v.x * v.x) + (v.y * v.y); }
inline float MagnitudeSqr(const Vector3& v)							{ return (v.x * v.x) + (v.y * v.y) + (v.z * v.z); }
inline float Magnitude(const Vector2& v)							{ return Sqrt(MagnitudeSqr(v)); }
inline float Magnitude(const Vector3& v)							{ return Sqrt(MagnitudeSqr(v)); }
inline float MagnitudeXZSqr(const Vector3& v)						{ return (v.x * v.x) + (v.z * v.z); }
inline float MagnitudeXZ(const Vector3& v)							{ return Sqrt(MagnitudeXZSqr(v)); }
inline float Magnitude(const Quaternion& q)							{ return Sqrt((q.x * q.x) + (q.y * q.y) + (q.z * q.z) + (q.w * q.w)); }

inline Vector2 Normalize(const Vector2& v)
{
	ASSERT(!IsZero(v), "[Math] Cannot normalize zero length vector!");
	return v / Magnitude(v); 
}
inline Vector3 Normalize(const Vector3& v)
{
	ASSERT(!IsZero(v), "[Math] Cannot normalize zero length vector!");
	return v / Magnitude(v); 
}
inline Quaternion Normalize(const Quaternion& q)					{ return q / Magnitude(q); }

inline float DistanceSqr(const Vector2& a, const Vector2& b)		{ return MagnitudeSqr(a - b); }
inline float DistanceSqr(const Vector3& a, const Vector3& b)		{ return MagnitudeSqr(a - b); }
inline float Distance(const Vector2& a, const Vector2& b)			{ return Sqrt(DistanceSqr(a, b)); }
inline float Distance(const Vector3& a, const Vector3& b)			{ return Sqrt(DistanceSqr(a, b)); }
inline float DistanceXZSqr(const Vector3& a, const Vector3& b)		{ return MagnitudeXZSqr(a - b); }
inline float DistanceXZ(const Vector3& a, const Vector3& b)			{ return Sqrt(DistanceXZSqr(a, b)); }

inline float Dot(const Vector2& a, const Vector2& b)				{ return (a.x * b.x) + (a.y * b.y); }
inline float Dot(const Vector3& a, const Vector3& b)				{ return (a.x * b.x) + (a.y * b.y) + (a.z * b.z); }
inline float Determinant(const Vector2& a, const Vector2& b)		{ return a.x * b.y - a.y * b.x; }
inline float Angle(const Vector2& a, const Vector2& b)				{ return atan2(Determinant(a, b), Dot(a, b)); }
inline Vector3 Cross(const Vector3& a, const Vector3& b)			{ return Vector3{ (a.y * b.z) - (a.z * b.y), (a.z * b.x) - (a.x * b.z), (a.x * b.y) - (a.y * b.x) }; }
inline Vector2 Project(const Vector2& v, const Vector2& n)			{ return n * (Dot(v, n) / Dot(n, n)); }
inline Vector3 Project(const Vector3& v, const Vector3& n)			{ return n * (Dot(v, n) / Dot(n, n)); }
inline Vector2 Reflect(const Vector2& v, const Vector2& normal)		{ return v - (normal * Dot(v, normal) * 2.0f); }
inline Vector3 Reflect(const Vector3& v, const Vector3& normal)		{ return v - (normal * Dot(v, normal) * 2.0f); }

inline Vector2 GetTranslation(const Matrix3& m)						{ return Vector2{m._31, m._32}; }
inline Vector3 GetTranslation(const Matrix4& m)						{ return Vector3{m._41, m._42, m._43}; }
inline Vector3 GetRight(const Matrix4& m)							{ return Vector3{m._11, m._12, m._13}; }
inline Vector3 GetUp(const Matrix4& m)								{ return Vector3{m._21, m._22, m._23}; }
inline Vector3 GetForward(const Matrix4& m)							{ return Vector3{m._31, m._32, m._33}; }

inline Vector2 Lerp(const Vector2& v0, const Vector2& v1, float t)	{ return v0 + ((v1 - v0) * t); }
inline Vector3 Lerp(const Vector3& v0, const Vector3& v1, float t)	{ return v0 + ((v1 - v0) * t); }
inline Quaternion Lerp(Quaternion q0, Quaternion q1, float t)		{ return q0 * (1.0f - t) + (q1 * t); }

inline Vector2 ConvertToV2(Vector3 v) { return { v.x,v.y }; }
inline Vector2 ConvertToV2(Vector4 v) { return { v.x,v.y }; }
inline Vector3 ConvertToV3(Vector4 v) { return { v.x,v.y,v.z }; }

inline Vector2 Rotate(const Vector2& v, float rad)
{
	const float kCosAngle = cos(rad);
	const float kSinAngle = sin(rad);
	return Vector2
	{
		v.x * kCosAngle - v.y * kSinAngle,
		v.y * kCosAngle + v.x * kSinAngle
	};
}

inline float Determinant(const Matrix3& m)
{
	float det = 0.0f;
	det = (m._11 * (m._22 * m._33 - m._23 * m._32));
	det -= (m._12 * (m._21 * m._33 - m._23 * m._31));
	det += (m._13 * (m._21 * m._32 - m._22 * m._31));
	return det;
}

inline float Determinant(const Matrix4& m)
{
	float det = 0.0f;
	det = (m._11 * (m._22 * (m._33 * m._44 - (m._43 * m._34)) - m._23 * (m._32 * m._44 - (m._42 * m._34)) + m._24 * (m._32 * m._43 - (m._42 * m._33))));
	det -= (m._12 * (m._21 * (m._33 * m._44 - (m._43 * m._34)) - m._23 * (m._31 * m._44 - (m._41 * m._34)) + m._24 * (m._31 * m._43 - (m._41 * m._33))));
	det += (m._13 * (m._21 * (m._32 * m._44 - (m._42 * m._34)) - m._22 * (m._31 * m._44 - (m._41 * m._34)) + m._24 * (m._31 * m._42 - (m._41 * m._32))));
	det -= (m._14 * (m._21 * (m._32 * m._43 - (m._42 * m._33)) - m._22 * (m._31 * m._43 - (m._41 * m._33)) + m._23 * (m._31 * m._42 - (m._41 * m._32))));
	return det;
}

static Matrix3 Translation(const Vector2& v)		{ return { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, v.x, v.y, 1.0f }; }
static Matrix3 Scaling(const Vector2& s)			{ return { s.x, 0.0f, 0.0f, 0.0f, s.y, 0.0f, 0.0f, 0.0f, 1.0f }; }
static Matrix4 Translation(const Vector3& v)		{ return { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, v.x, v.y, v.z, 1.0f }; }
static Matrix4 Scaling(const Vector3& s)			{ return { s.x, 0.0f, 0.0f, 0.0f, 0.0f, s.y, 0.0f, 0.0f, 0.0f, 0.0f, s.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }; }

inline Matrix3 Adjoint(const Matrix3& m)
{
	return Matrix3
	(
		(m._22 * m._33 - m._23 * m._32),
		-(m._12 * m._33 - m._13 * m._32),
		(m._12 * m._23 - m._13 * m._22),

		-(m._21 * m._33 - m._23 * m._31),
		(m._11 * m._33 - m._13 * m._31),
		-(m._11 * m._23 - m._13 * m._21),

		(m._21 * m._32 - m._22 * m._31),
		-(m._11 * m._32 - m._12 * m._31),
		(m._11 * m._22 - m._12 * m._21)
	);
}

inline Matrix4 Adjoint(const Matrix4& m)
{
	return Matrix4
	(
		(m._22 * ((m._33 * m._44) - (m._43 * m._34)) - m._23 * ((m._32 * m._44) - (m._42 * m._34)) + m._24 * ((m._32 * m._43) - (m._42 * m._33))),
		-(m._12 * ((m._33 * m._44) - (m._43 * m._34)) - m._13 * ((m._32 * m._44) - (m._42 * m._34)) + m._14 * ((m._32 * m._43) - (m._42 * m._33))),
		(m._12 * ((m._23 * m._44) - (m._43 * m._24)) - m._13 * ((m._22 * m._44) - (m._42 * m._24)) + m._14 * ((m._22 * m._43) - (m._42 * m._23))),
		-(m._12 * ((m._23 * m._34) - (m._33 * m._24)) - m._13 * ((m._22 * m._34) - (m._32 * m._24)) + m._14 * ((m._22 * m._33) - (m._32 * m._23))),

		-(m._21 * ((m._33 * m._44) - (m._43 * m._34)) - m._31 * ((m._23 * m._44) - (m._24 * m._43)) + m._41 * ((m._23 * m._34) - (m._24 * m._33))),
		(m._11 * ((m._33 * m._44) - (m._43 * m._34)) - m._13 * ((m._31 * m._44) - (m._41 * m._34)) + m._14 * ((m._31 * m._43) - (m._41 * m._33))),
		-(m._11 * ((m._23 * m._44) - (m._43 * m._24)) - m._13 * ((m._21 * m._44) - (m._41 * m._24)) + m._14 * ((m._21 * m._43) - (m._41 * m._23))),
		(m._11 * ((m._23 * m._34) - (m._33 * m._24)) - m._13 * ((m._21 * m._34) - (m._31 * m._24)) + m._14 * ((m._21 * m._33) - (m._31 * m._23))),

		(m._21 * ((m._32 * m._44) - (m._42 * m._34)) - m._31 * ((m._22 * m._44) - (m._42 * m._24)) + m._41 * ((m._22 * m._34) - (m._32 * m._24))),
		-(m._11 * ((m._32 * m._44) - (m._42 * m._34)) - m._31 * ((m._12 * m._44) - (m._42 * m._14)) + m._41 * ((m._12 * m._34) - (m._32 * m._14))),
		(m._11 * ((m._22 * m._44) - (m._42 * m._24)) - m._12 * ((m._21 * m._44) - (m._41 * m._24)) + m._14 * ((m._21 * m._42) - (m._41 * m._22))),
		-(m._11 * ((m._22 * m._34) - (m._32 * m._24)) - m._21 * ((m._12 * m._34) - (m._32 * m._14)) + m._31 * ((m._12 * m._24) - (m._22 * m._14))),

		-(m._21 * ((m._32 * m._43) - (m._42 * m._33)) - m._31 * ((m._22 * m._43) - (m._42 * m._23)) + m._41 * ((m._22 * m._33) - (m._32 * m._23))),
		(m._11 * ((m._32 * m._43) - (m._42 * m._33)) - m._12 * ((m._31 * m._43) - (m._41 * m._33)) + m._13 * ((m._31 * m._42) - (m._41 * m._32))),
		-(m._11 * ((m._22 * m._43) - (m._42 * m._23)) - m._12 * ((m._21 * m._43) - (m._41 * m._23)) + m._13 * ((m._21 * m._42) - (m._41 * m._22))),
		(m._11 * ((m._22 * m._33) - (m._32 * m._23)) - m._12 * ((m._21 * m._33) - (m._31 * m._23)) + m._13 * ((m._21 * m._32) - (m._31 * m._22)))
	);
}

inline Matrix3 Inverse(const Matrix3& m)
{
	const float determinant = Determinant(m);
	const float invDet = 1.0f / determinant;
	return Adjoint(m) * invDet;
}

inline Matrix4 Inverse(const Matrix4& m)
{
	const float determinant = Determinant(m);
	const float invDet = 1.0f / determinant;
	return Adjoint(m) * invDet;
}

inline Matrix3 Transpose(const Matrix3& m)
{
	return Matrix3
	(
		m._11, m._21, m._31,
		m._12, m._22, m._32,
		m._13, m._23, m._33
	);
}

inline Matrix4 Transpose(const Matrix4& m)
{
	return Matrix4
	(
		m._11, m._21, m._31, m._41,
		m._12, m._22, m._32, m._42,
		m._13, m._23, m._33, m._43,
		m._14, m._24, m._34, m._44
	);
}

inline Vector2 TransformCoord(const Vector2& v, const Matrix3& m)
{
	return Vector2
	{
		v.x * m._11 + v.y * m._21 + m._31,
		v.x * m._12 + v.y * m._22 + m._32
	};
}

inline Vector3 TransformCoord(const Vector3& v, const Matrix4& m)
{
	return Vector3
	{
		v.x * m._11 + v.y * m._21 + v.z * m._31 + m._41,
		v.x * m._12 + v.y * m._22 + v.z * m._32 + m._42,
		v.x * m._13 + v.y * m._23 + v.z * m._33 + m._43
	};
}

inline Vector2 TransformNormal(const Vector2& v, const Matrix3& m)
{
	return Vector2
	{
		v.x * m._11 + v.y * m._21,
		v.x * m._12 + v.y * m._22
	};
}

inline Vector3 TransformNormal(const Vector3& v, const Matrix4& m)
{
	return Vector3
	{
		v.x * m._11 + v.y * m._21 + v.z * m._31,
		v.x * m._12 + v.y * m._22 + v.z * m._32,
		v.x * m._13 + v.y * m._23 + v.z * m._33
	};
}

Quaternion QuaternionLookRotation(const Vector3& forward, const Vector3& up);
Quaternion QuaternionRotationAxis(const Vector3& axis, float rad);
Matrix4 MatrixRotationAxis(const Vector3& axis, float rad);
Matrix4 MatrixRotationQuaternion(const Quaternion& q);

Quaternion Slerp(Quaternion q0, Quaternion q1, float t);

Vector3 GetBarycentric(const Vector2& a, const Vector2& b, const Vector2& c, const Vector2& point);
bool PointInTriangle(const Vector2& point, const Vector2& a, const Vector2& b, const Vector2& c);

Vector3 Mean(const Vector3* v, uint32_t count);

inline int Random()
{
	return std::uniform_int_distribution<>{ 0, (std::numeric_limits<int>::max)() }(myRandomEngine);
}

inline int Random(int min, int max)
{
	return std::uniform_int_distribution<>{ min, max }(myRandomEngine);
}

inline float RandomFloat()
{
	return std::uniform_real_distribution<float>{ 0, 1.0f }(myRandomEngine);
}

inline float RandomFloat(float min, float max)
{
	return std::uniform_real_distribution<float>{ min, max }(myRandomEngine);
}

inline double RandomDouble()
{
	return std::uniform_real_distribution<double>{ 0, 1.0 }(myRandomEngine);
}

inline double RandomDouble(double min, double max)
{
	return std::uniform_real_distribution<double>{ min, max }(myRandomEngine);
}

inline Vector2 RandomVector2()
{
	return Math::Vector2
	(
		RandomFloat(),
		RandomFloat()
	);
}

inline Vector2 RandomVector2(const Math::Vector2& min, const Math::Vector2& max)
{
	return Math::Vector2
	(
		RandomFloat(min.x, max.x),
		RandomFloat(min.y, max.y)
	);
}

//inline Vector2 RandomUnitCircle()
//{
//	return Math::Normalize(Math::Vector2(
//		RandomFloat(-1.0f, 1.0f) + 0.001f,
//		RandomFloat(-1.0f, 1.0f))
//	);
//}

inline Vector3 RandomVector3()
{
	return Math::Vector3
	(
		RandomFloat(),
		RandomFloat(),
		RandomFloat()
	);
}

inline Vector3 RandomVector3(const Math::Vector3& min, const Math::Vector3& max)
{
	return Math::Vector3
	(
		RandomFloat(min.x, max.x),
		RandomFloat(min.y, max.y),
		RandomFloat(min.z, max.z)
	);
}

//inline Vector3 RandomUnitSphere()
//{
//	return Math::Normalize(Math::Vector3(
//		RandomFloat(-1.0f, 1.0f) + 0.001f,
//		RandomFloat(-1.0f, 1.0f),
//		RandomFloat(-1.0f, 1.0f))
//	);
//}

} // namespace Math
} // namespace S

META_TYPE_DECLARE(S::Math::Vector2)
META_TYPE_DECLARE(S::Math::Vector3)
META_TYPE_DECLARE(S::Math::Vector4)
META_TYPE_DECLARE(S::Math::Matrix3)
META_TYPE_DECLARE(S::Math::Matrix4)

#endif // !INCLUDED_MATH_H