#include "Precompiled.h"
#include "Geometry.h"

using namespace S;
using namespace S::Math;

bool Geometry::PointInRect(const Vector2& point, const Rect& rect)
{
	if (point.x > rect.right || point.x < rect.left ||
		point.y > rect.bottom || point.y < rect.top)
	{
		return false;
	}
	return true;
}

bool Geometry::PointInCircle(const Vector2& point, const Circle& circle)
{
	const Math::Vector2 centerToPoint = point - circle.center;
	const float distSqr = Math::Dot(centerToPoint, centerToPoint);
	const float radiusSqr = circle.radius * circle.radius;
	return distSqr < radiusSqr;
}

bool Geometry::Intersect(const LineSegment2D& a, const LineSegment2D& b)
{
	// http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/

	float ua = ((a.to.x - a.from.x) * (b.from.y - a.from.y)) - ((a.to.y - a.from.y) * (b.from.x - a.from.x));
	float ub = ((b.to.x - b.from.x) * (b.from.y - a.from.y)) - ((b.to.y - b.from.y) * (b.from.x - a.from.x));
	float denom = ((a.to.y - a.from.y) * (b.to.x - b.from.x)) - ((a.to.x - a.from.x) * (b.to.y - b.from.y));

	// First check for special cases
	if (denom == 0.0f)
	{
		if (ua == 0.0f && ub == 0.0f)
		{
			// The line segments are the same
			return true;
		}
		else
		{
			// The line segments are parallel
			return false;
		}
	}

	ua /= denom;
	ub /= denom;

	if (ua < 0.0f || ua > 1.0f || ub < 0.0f || ub > 1.0f)
	{
		return false;
	}

	return true;
}

bool Geometry::Intersect(const Circle& c0, const Circle& c1)
{
	const float radSqr = (c0.radius + c1.radius)*(c0.radius + c1.radius);
	return (DistanceSqr(c0.center, c1.center) < radSqr);
}

bool Geometry::Intersect(const Rect& r0, const Rect& r1)
{
	if (r0.left > r1.right)
		return false;
	else if (r0.top > r1.bottom)
		return false;
	else if (r0.right < r1.left)
		return false;
	else if (r0.bottom < r1.top)
		return false;
	return true;
}

bool Geometry::Intersect(const LineSegment2D& l, const Rect& r)
{
	if (Geometry::Intersect(l, LineSegment2D{ r.left, r.bottom, r.left, r.top }))
		return true;
	else if (Geometry::Intersect(l, LineSegment2D{ r.right, r.bottom, r.right, r.top }))
		return true;
	else if (Geometry::Intersect(l, LineSegment2D{ r.left, r.top, r.right, r.top }))
		return true;
	else if (Geometry::Intersect(l, LineSegment2D{ r.left, r.bottom, r.right, r.bottom }))
		return true;
	return false;
}

bool Geometry::Intersect(const LineSegment2D& l, const Circle& c)
{
	return Geometry::Intersect(c, l);
}

bool Geometry::Intersect(const Circle& c, const LineSegment2D& l)
{
	Vector2 startToCenter = c.center - l.from;
	Vector2 startToEnd = l.to - l.from;
	float len = Magnitude(startToEnd);
	Vector2 dir = startToEnd / len;

	// Find the closest point to the line segment
	float projection = Dot(startToCenter, dir);
	Vector2 closestPoint;
	if (projection > len)
	{
		closestPoint = l.to;
	}
	else if (projection < 0.0f)
	{
		closestPoint = l.from;
	}
	else
	{
		closestPoint = l.from + (dir * projection);
	}

	// Check if the closest point is within the circle
	Vector2 closestToCenter = c.center - closestPoint;
	if (MagnitudeSqr(closestToCenter) > Sqr(c.radius))
	{
		return false;
	}
	return true;
}

bool Geometry::Intersect(const Circle& c, const Rect& r)
{
	return Geometry::Intersect(r, c);
}

bool Geometry::Intersect(const Rect& r, const Circle& c)
{
	Vector2 closestPoint;
	closestPoint.x = Clamp(c.center.x, r.left, r.right);
	closestPoint.y = Clamp(c.center.y, r.top, r.bottom);

	const float distance = Distance(closestPoint, c.center);
	if (distance > c.radius)
	{
		return false;
	}
	return true;
}

bool Geometry::Intersect(const Ray& ray, const Vector3& a, const Vector3& b, const Vector3& c, float& distance)
{
	// Reference: https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm

	// Find vectors for two edges sharing V1
	Vector3 e1 = b - a;
	Vector3 e2 = c - a;

	// Begin calculating determinant - also used to calculate u parameter
	Vector3 P = Cross(ray.dir, e2);

	// If determinant is near zero, ray lies in plane of triangle
	float det = Dot(e1, P);
	// NOT CULLING
	if (IsZero(det))
	{
		return false;
	}

	float inv_det = 1.0f / det;

	// Calculate distance from V1 to ray origin
	Vector3 T = ray.org - a;

	// Calculate u parameter and test bound
	float u = Dot(T, P) * inv_det;

	// The intersection lies outside of the triangle
	if (u < 0.0f || u > 1.0f)
	{
		return false;
	}

	// Prepare to test v parameter
	Vector3 Q = Cross(T, e1);

	// Calculate V parameter and test bound
	float v = Dot(ray.dir, Q) * inv_det;

	// The intersection lies outside of the triangle
	if (v < 0.0f || u + v  > 1.0f)
	{
		return false;
	}

	// Ray intersection
	float t = Dot(e2, Q) * inv_det;
	if (t <= 0.0f)
	{
		// No hit, no win
		return false;
	}

	distance = t;
	return true;
}

bool Geometry::Intersect(const Ray& ray, const Plane& plane, float& distance)
{
	const float orgDotN = Dot(ray.org, plane.n);
	const float dirDotN = Dot(ray.dir, plane.n);

	// Check if ray is parallel to the plane
	if (IsZero(dirDotN))
	{
		if (IsZero(orgDotN - plane.d))
		{
			distance = 0.0f;
			return true;
		}
		else
		{
			return false;
		}
	}

	// Compute distance
	distance = (plane.d - orgDotN) / dirDotN;
	return true;
}

bool Geometry::Intersect(const Ray& ray, const AABB& aabb, float& distEntry, float& distExit)
{
	// https://truesculpt.googlecode.com/hg-history/Release%25200.8/Doc/ray_box_intersect.pdf

	// Returns true if ray intersects bounding box
	// Sets d1 to entry distance, d2 to exit distance (along ray.dir)

	Vector3 boxMin = aabb.center - aabb.extend;
	Vector3 boxMax = aabb.center + aabb.extend;
	float tmin, tmax, tymin, tymax, tzmin, tzmax;

	float divx = 1.0f / ray.dir.x;
	float divy = 1.0f / ray.dir.y;
	float divz = 1.0f / ray.dir.z;

	if (divx >= 0.0f)
	{
		tmin = (boxMin.x - ray.org.x) * divx;
		tmax = (boxMax.x - ray.org.x) * divx;
	}
	else
	{
		tmin = (boxMax.x - ray.org.x) * divx;
		tmax = (boxMin.x - ray.org.x) * divx;
	}
	if (divy >= 0.0f)
	{
		tymin = (boxMin.y - ray.org.y) * divy;
		tymax = (boxMax.y - ray.org.y) * divy;
	}
	else
	{
		tymin = (boxMax.y - ray.org.y) * divy;
		tymax = (boxMin.y - ray.org.y) * divy;
	}

	if ((tmin > tymax) || (tymin > tmax))
	{
		return false;
	}

	if (tymin > tmin)
	{
		tmin = tymin;
	}

	if (tymax < tmax)
	{
		tmax = tymax;
	}

	if (divz >= 0.0f)
	{
		tzmin = (boxMin.z - ray.org.z) * divz;
		tzmax = (boxMax.z - ray.org.z) * divz;
	}
	else
	{
		tzmin = (boxMax.z - ray.org.z) * divz;
		tzmax = (boxMin.z - ray.org.z) * divz;
	}

	if ((tmin > tzmax) || (tzmin > tmax))
	{
		return false;
	}

	if (tzmin > tmin)
	{
		tmin = tzmin;
	}

	if (tzmax < tmax)
	{
		tmax = tzmax;
	}

	distEntry = tmin;
	distExit = tmax;
	return true;
}

bool Geometry::Intersect(const Ray& ray, const OBB& obb, float& distEntry, float& distExit)
{
	// Compute the local-to-world/world-to-local matrices
	Matrix4 matTrans = Matrix4::Translation(obb.center.x, obb.center.y, obb.center.z);
	Matrix4 matRot = MatrixRotationQuaternion(obb.orientation);
	Matrix4 matWorld = matRot * matTrans;
	Matrix4 matWorldInv = Inverse(matWorld);

	// Transform the ray into the OBB's local space
	Vector3 org = TransformCoord(ray.org, matWorldInv);
	Vector3 dir = TransformNormal(ray.dir, matWorldInv);

	AABB aabb(Vector3::Zero(), obb.extend);
	return Geometry::Intersect(Ray(org, dir), aabb, distEntry, distExit);
}

bool Geometry::Intersect(const Ray & ray, const Sphere & sphere)
{
	Vector3 oc = ray.org - sphere.center;
	float a = Dot(ray.dir, ray.dir);
	float b = 2.0f * Dot(oc, ray.dir);
	float c = Dot(oc, oc) - sphere.radius * sphere.radius;
	float discriminant = b * b - 4.0f * a*c;
	if (discriminant < 0.0f)
	{
		return false;
	}
	else
	{
		float numerator = -b - sqrt(discriminant);
		if (numerator > 0.0f)
		{
			return true;
		}

		numerator = -b + sqrt(discriminant);
		if (numerator > 0.0f)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool Geometry::Intersect(const Vector3& point, const AABB& aabb)
{
	const Vector3 test = point - aabb.center;
	if (abs(test.x) > aabb.extend.x) return false;
	if (abs(test.y) > aabb.extend.y) return false;
	if (abs(test.z) > aabb.extend.z) return false;
	return true;
}

bool Geometry::Intersect(const Vector3& point, const OBB& obb)
{
	// Compute the local-to-world/world-to-local matrices
	Matrix4 matTrans = Matrix4::Translation(obb.center.x, obb.center.y, obb.center.z);
	Matrix4 matRot = MatrixRotationQuaternion(obb.orientation);
	Matrix4 matWorld = matRot * matTrans;
	Matrix4 matWorldInv = Inverse(matWorld);

	// Transform the point into the OBB's local space
	Vector3 localPoint = TransformCoord(point, matWorldInv);
	AABB aabb(Vector3::Zero(), obb.extend);

	// Test against local AABB
	return Geometry::Intersect(localPoint, aabb);
}

/*void Geometry::GetCorners(const OBB & obb, std::vector<Vector3>& corners)
{
	// Compute the local-to-world matrices
	Matrix4 matTrans = Matrix4::Translation(obb.center.x, obb.center.y, obb.center.z);
	Matrix4 matRot = MatrixRotationQuaternion(obb.orientation);
	Matrix4 matWorld = matRot * matTrans;

	// Create a local AABB
	corners.clear();
	corners.push_back(Vector3(-obb.extend.x, -obb.extend.y, -obb.extend.z));
	corners.push_back(Vector3(-obb.extend.x, -obb.extend.y, obb.extend.z));
	corners.push_back(Vector3(obb.extend.x, -obb.extend.y, obb.extend.z));
	corners.push_back(Vector3(obb.extend.x, -obb.extend.y, -obb.extend.z));
	corners.push_back(Vector3(-obb.extend.x, obb.extend.y, -obb.extend.z));
	corners.push_back(Vector3(-obb.extend.x, obb.extend.y, obb.extend.z));
	corners.push_back(Vector3(obb.extend.x, obb.extend.y, obb.extend.z));
	corners.push_back(Vector3(obb.extend.x, obb.extend.y, -obb.extend.z));

	// Transform AABB into world space to form the OBB
	const uint32_t kNumCorners = (uint32_t)corners.size();
	for (uint32_t i = 0; i < kNumCorners; ++i)
	{
		corners[i] = TransformCoord(corners[i], matWorld);
	}
}*/

bool Geometry::GetContactPoint(const Ray & ray, const OBB & obb, Vector3 & point, Vector3 & normal)
{
	// Compute the local-to-world/world-to-local matrices
	Matrix4 matTrans = Matrix4::Translation(obb.center.x, obb.center.y, obb.center.z);
	Matrix4 matRot = MatrixRotationQuaternion(obb.orientation);
	Matrix4 matWorld = matRot * matTrans;
	Matrix4 matWorldInv = Inverse(matWorld);

	// Transform the ray into the OBB's local space
	Vector3 org = TransformCoord(ray.org, matWorldInv);
	Vector3 dir = TransformNormal(ray.dir, matWorldInv);
	Ray localRay(org, dir);

	Plane planes[] =
	{
		Plane(0.0f,  0.0f, -1.0f, obb.extend.z),
		Plane(0.0f,  0.0f,  1.0f, obb.extend.z),
		Plane(0.0f, -1.0f,  0.0f, obb.extend.y),
		Plane(0.0f,  1.0f,  0.0f, obb.extend.y),
		Plane(-1.0f,  0.0f,  0.0f, obb.extend.x),
		Plane(1.0f,  0.0f,  0.0f, obb.extend.x)
	};

	uint32_t numIntersections = 0;
	for (uint32_t i = 0; i < 6; ++i)
	{
		const float d = Dot(org, planes[i].n);
		if (d > planes[i].d)
		{
			float distance = 0.0f;
			if (Intersect(localRay, planes[i], distance) && distance >= 0.0f)
			{
				Vector3 pt = org + (dir * distance);
				if (abs(pt.x) <= obb.extend.x + 0.01f &&
					abs(pt.y) <= obb.extend.y + 0.01f &&
					abs(pt.z) <= obb.extend.z + 0.01f)
				{
					point = pt;
					normal = planes[i].n;
					++numIntersections;
				}
			}
		}
	}

	if (numIntersections == 0)
	{
		return false;
	}

	point = TransformCoord(point, matWorld);
	normal = TransformNormal(normal, matWorld);
	return true;
}

Vector2 Geometry::GetContactPoint(const LineSegment2D& a, const LineSegment2D& b)
{
	Vector2 result;
	if (Intersect(a, b))
	{
		if (Abs(a.from.x - a.to.x) <= 0.00003f)
		{
			float bSlope = (b.from.y - b.to.y) / (b.from.x - b.to.x);
			float bK = b.from.y - bSlope * b.from.x;
			result.x = a.from.x;
			result.y = result.x*bSlope + bK;
		}
		else
		{
			float aSlope = (a.from.y - a.to.y) / (a.from.x - a.to.x);
			float aK = a.from.y - aSlope * a.from.x;
			if (Abs(b.from.x - b.to.x) <= 0.00003f)
			{
				result.x = b.from.x;
				result.y = result.x*aSlope + aK;
			}
			else
			{
				float bSlope = (b.from.y - b.to.y) / (b.from.x - b.to.x);
				float bK = b.from.y - bSlope * b.from.x;
				result.x = (bK - aK) / (aSlope - bSlope);
				result.y = result.x*aSlope + aK;
			}
		}
	}
	return result;
}

Vector2 Geometry::GetClosestPoint(const LineSegment2D& l, const Rect& r)
{
	float dis = FLT_MAX;
	LineSegment2D line;
	Math::Vector2 result{ FLT_MAX,FLT_MAX }, temp; // Guessing DrawScreenCircle might get ERROR, max float + float
	if (Geometry::Intersect(l, line = LineSegment2D{ r.left, r.bottom, r.left, r.top }))
	{
		result = GetContactPoint(l, line);
		dis = Math::DistanceSqr(l.from, result);
	}
	if (Geometry::Intersect(l, line = LineSegment2D{ r.right, r.bottom, r.right, r.top }))
	{
		temp = GetContactPoint(l, line);
		float d = Math::DistanceSqr(l.from, temp);
		if (d < dis)
		{
			dis = d;
			result = temp;
		}
	}
	if (Geometry::Intersect(l, line = LineSegment2D{ r.left, r.top, r.right, r.top }))
	{
		temp = GetContactPoint(l, line);
		float d = Math::DistanceSqr(l.from, temp);
		if (d < dis)
		{
			dis = d;
			result = temp;
		}
	}
	if (Geometry::Intersect(l, line = LineSegment2D{ r.left, r.bottom, r.right, r.bottom }))
	{
		temp = GetContactPoint(l, line);
		float d = Math::DistanceSqr(l.from, temp);
		if (d < dis)
		{
			dis = d;
			result = temp;
		}
	}
	return result;
}

Vector3 Geometry::GetClosestPoint(const Ray& ray, const Vector3& point)
{
	Vector3 orgToPoint = point - ray.org;
	float d = Dot(orgToPoint, ray.dir);
	return ray.org + (ray.dir * d);
}

Vector3 Geometry::GetClosestPoint(const Ray& ray, const Sphere& sphere)
{
	Vector3 oc = ray.org - sphere.center;
	float a = Dot(ray.dir, ray.dir);
	float b = 2.0f * Dot(oc, ray.dir);
	float c = Dot(oc, oc) - sphere.radius * sphere.radius;
	float discriminant = b * b - 4.0f * a*c;
	if (discriminant < 0.0f)
	{
		return -1.0f;
	}
	else
	{
		float numerator = -b - sqrt(discriminant);
		if (numerator > 0.0f)
		{
			return numerator / (2.0f * a);
		}

		numerator = -b + sqrt(discriminant);
		if (numerator > 0.0f)
		{
			return numerator / (2.0f * a);
		}
		else
		{
			return -1.0f;
		}
	}
}
