#include "Precompiled.h"
#include "SMath.h"

using namespace S;
using namespace S::Math;

const float Math::kPi = 3.14159265358979f;
const float Math::kTwoPi = 6.28318530717958f;
const float Math::kPiByTwo = 1.57079632679489f;
const float Math::kRootTwo = 1.41421356237309f;
const float Math::kRootThree = 1.73205080756887f;
const float Math::kDegToRad = kPi / 180.0f;
const float Math::kRadToDeg = 180.0f / kPi;

META_TYPE_DEFINE(Math::Vector2, "Vector2")
META_TYPE_DEFINE(Math::Vector3, "Vector3")
META_TYPE_DEFINE(Math::Vector4, "Vector4")
META_TYPE_DEFINE(Math::Matrix3, "Matrix3")
META_TYPE_DEFINE(Math::Matrix4, "Matrix4")

Quaternion Math::QuaternionLookRotation(const Vector3& forward, const Vector3& up)
{
	Vector3 z = Normalize(forward);
	Vector3 x = Normalize(Cross(up, z));
	Vector3 y = Normalize(Cross(z, x));
	Matrix4 mat
	(
		x.x, x.y, x.z, 0.0f,
		y.x, y.y, y.z, 0.0f,
		z.x, z.y, z.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	// TODO: Need matrix to quaternion
	// http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/
	return Quaternion();
}

Quaternion Math::QuaternionRotationAxis(const Vector3& axis, float rad)
{
	const float c = cos(rad * 0.5f);
	const float s = sin(rad * 0.5f);
	const Math::Vector3 a = Math::Normalize(axis);
	return Quaternion(a.x * s, a.y * s, a.z * s, c);
}

Matrix4 Math::MatrixRotationAxis(const Vector3& axis, float rad)
{
	const Vector3 u = Normalize(axis);
	const float x = u.x;
	const float y = u.y;
	const float z = u.z;
	const float s = sin(rad);
	const float c = cos(rad);

	return Matrix4
	(
		c + (x * x * (1.0f - c)),
		x * y * (1.0f - c) + z * s,
		x * z * (1.0f - c) - y * s,
		0.0f,

		x * y * (1.0f - c) - z * s,
		c + (y * y * (1.0f - c)),
		y * z * (1.0f - c) + x * s,
		0.0f,

		x * z * (1.0f - c) + y * s,
		y * z * (1.0f - c) - x * s,
		c + (z * z * (1.0f - c)),
		0.0f,

		0.0f,0.0f,0.0f,1.0f
	);
}

Matrix4 Math::MatrixRotationQuaternion(const Quaternion& q)
{
	return Matrix4
	(
		1.0f - (2.0f * q.y * q.y) - (2.0f * q.z * q.z),
		(2.0f * q.x * q.y) + (2.0f * q.z * q.w),
		(2.0f * q.x * q.z) - (2.0f * q.y * q.w),
		0.0f,

		(2.0f * q.x * q.y) - (2.0f * q.z * q.w),
		1.0f - (2.0f * q.x * q.x) - (2.0f * q.z * q.z),
		(2.0f * q.y * q.z) + (2.0f * q.x * q.w),
		0.0f,

		(2.0f * q.x * q.z) + (2.0f * q.y * q.w),
		(2.0f * q.y * q.z) - (2.0f * q.x * q.w),
		1.0f - (2.0f * q.x * q.x) - (2.0f * q.y * q.y),
		0.0f,

		0.0f,
		0.0f,
		0.0f,
		1.0f
	);
}

Quaternion Math::Slerp(Quaternion q0, Quaternion q1, float t)
{
	// Find the dot product
	float dot = (q0.x * q1.x) + (q0.y * q1.y) + (q0.z * q1.z) + (q0.w * q1.w);

	// Determine the direction of the rotation.
	if (dot < 0.0f)
	{
		dot = -dot;
		q1.x = -q1.x;
		q1.y = -q1.y;
		q1.z = -q1.z;
		q1.w = -q1.w;
	}
	else if (dot > 0.999f)
	{
		return Normalize(Lerp(q0, q1, t));
	}

	float theta = acosf(dot);
	float sintheta = sinf(theta);
	float scale0 = sinf(theta * (1.0f - t)) / sintheta;
	float scale1 = sinf(theta * t) / sintheta;

	// Perform the slerp
	return Quaternion
	(
		(q0.x * scale0) + (q1.x * scale1),
		(q0.y * scale0) + (q1.y * scale1),
		(q0.z * scale0) + (q1.z * scale1),
		(q0.w * scale0) + (q1.w * scale1)
	);
}

Vector3 Math::GetBarycentric(const Vector2 & a, const Vector2 & b, const Vector2 & c, const Vector2 & point)
{
	Vector3 lambda;
	float denom = (b.y - c.y) * (a.x - c.x) + (c.x - b.x) * (a.y - c.y);
	lambda.x = ((b.y - c.y) * (point.x - c.x) + (c.x - b.x) * (point.y - c.y)) / denom;
	lambda.y = ((c.y - a.y) * (point.x - c.x) + (a.x - c.x) * (point.y - c.y)) / denom;
	lambda.z = 1.0f - lambda.x - lambda.y;
	return lambda;
}

bool Math::PointInTriangle(const Vector2& point, const Vector2& a, const Vector2& b, const Vector2& c)
{
	Vector3 barycentric = GetBarycentric(a, b, c, point);
	return
		barycentric.x >= 0.0f && barycentric.x <= 1.0f &&
		barycentric.y >= 0.0f && barycentric.y <= 1.0f &&
		barycentric.z >= 0.0f && barycentric.z <= 1.0f;
}

Vector3 Math::Mean(const Vector3* v, uint32_t count)
{
	Vector3 mean(0.0f, 0.0f, 0.0f);
	for (uint32_t i = 0; i < count; ++i)
	{
		mean += v[i];
	}
	return mean / (float)count;
}