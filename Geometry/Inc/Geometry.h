#ifndef INCLUDED_GEOMETRY_H
#define INCLUDED_GEOMETRY_H

#include "Common.h"

#include "AABB.h"
#include "Circle.h"
#include "LineSegment2D.h"
#include "LineSegment3D.h"
#include "OBB.h"
#include "Plane.h"
#include "Ray.h"
#include "Rectangle.h"
#include "Sphere.h"

namespace S {
namespace Geometry {
bool PointInRect(const Math::Vector2& point, const Rect& rect);
bool PointInCircle(const Math::Vector2& point, const Circle& circle);

bool Intersect(const LineSegment2D& a, const LineSegment2D& b);
bool Intersect(const Circle& c0, const Circle& c1);
bool Intersect(const Rect& r0, const Rect& r1);
bool Intersect(const LineSegment2D& l, const Rect& r);
bool Intersect(const LineSegment2D& l, const Circle& c);
bool Intersect(const Circle& c, const LineSegment2D& l);

bool Intersect(const Circle& c, const Rect& r);
bool Intersect(const Rect& r, const Circle& c);

bool Intersect(const Ray& ray, const Math::Vector3& a, const Math::Vector3& b, const Math::Vector3& c, float& distance);
bool Intersect(const Ray& ray, const Plane& plane, float& distance);
bool Intersect(const Ray& ray, const AABB& aabb, float& distEntry, float& distExit);
bool Intersect(const Ray& ray, const OBB& obb, float& distEntry, float& distExit);
bool Intersect(const Ray& ray, const Sphere& sphere);
bool Intersect(const Math::Vector3& point, const AABB& aabb);
bool Intersect(const Math::Vector3& point, const OBB& obb);

//void GetCorners(const OBB& obb, std::vector<Math::Vector3>& corners);
bool GetContactPoint(const Ray& ray, const OBB& obb, Math::Vector3& point, Math::Vector3& normal);

Math::Vector2 GetContactPoint(const LineSegment2D& a, const LineSegment2D& b);
Math::Vector2 GetClosestPoint(const LineSegment2D& l, const Rect& r); // Assume line intersect with rect
Math::Vector3 GetClosestPoint(const Ray& ray, const Math::Vector3& point);
Math::Vector3 GetClosestPoint(const Ray& ray, const Sphere& sphere);

inline bool IsEmpty(const Rect& rect)								{ return rect.right <= rect.left || rect.bottom <= rect.top; }

inline Math::Vector3 GetPoint(const Ray& ray, float distance)		{ return ray.org + (ray.dir * distance); }

} // namespace Geometry
} // namespace S

#endif // #ifndef INCLUDED_GEOMETRY_H