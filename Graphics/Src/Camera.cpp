#include "Precompiled.h"
#include "Camera.h"

#include "GraphicsSystem.h"

using namespace S;
using namespace S::Graphics;

Camera::Camera()
{
}

void Camera::SetPosition(const Math::Vector3 & position)
{
	mPosition = position;
}

void Camera::SetDirection(const Math::Vector3 & direction)
{
	mDirection = direction;
}

void Camera::SetLookAt(const Math::Vector3 & target)
{
	mDirection = Math::Normalize(target - mPosition);
}

void Camera::Walk(float distance)
{
	mPosition += mDirection * distance;
}

void Camera::Strafe(float distance)
{
	mPosition += Math::Normalize(Math::Cross(Math::Vector3::YAxis(), mDirection)) *distance;
}

void Camera::Rise(float distance)
{
	mPosition.y += distance;
}

void Camera::Yaw(float radian)
{
	Math::Matrix4 matRotate = Math::Matrix4::RotationY(radian);
	mDirection = TransformNormal(mDirection, matRotate);
}

void Camera::Pitch(float radian)
{
	const Math::Vector3 r = Normalize(Cross(Math::Vector3::YAxis(), mDirection));
	const Math::Matrix4 matRotate = Math::MatrixRotationAxis(r, radian);
	const Math::Vector3 newDirection = TransformNormal(mDirection, matRotate);
	
	if (Math::Abs(Dot(newDirection, Math::Vector3::YAxis())) < 0.999f)
	{
		mDirection = newDirection;
	}
}

void Camera::SetFOV(float fov)
{
	fov *= Math::kDegToRad;
	const float kMinFOV = 10.0f * Math::kDegToRad;
	const float kMaxFOV = 150.0f * Math::kDegToRad;
	mFOV = Math::Clamp(fov, kMinFOV, kMaxFOV);
}

void Camera::SetNearPlane(float nearPlane)
{
	mNearPlane = nearPlane;
}

void Camera::SetFarPlane(float farPlane)
{
	mFarPlane = farPlane;
}

Math::Matrix4 Camera::GetViewMatrix() const
{
	const Math::Vector3 l = mDirection;
	const Math::Vector3 r = Math::Normalize(Math::Cross(Math::Vector3::YAxis(), l));
	const Math::Vector3 u = Math::Normalize(Math::Cross(l, r));
	const float dx = -Math::Dot(r, mPosition);
	const float dy = -Math::Dot(u, mPosition);
	const float dz = -Math::Dot(l, mPosition);

	return Math::Matrix4
	{
		r.x,u.x,l.x,0.0f,
		r.y,u.y,l.y,0.0f,
		r.z,u.z,l.z,0.0f,
		dx,dy,dz,1.0f
	};
}

Math::Matrix4 Camera::GetPerspectiveMatrix(float aspectRatio) const
{
	const float aspect = (aspectRatio == 0.0f)
		? GraphicsSystem::Get()->GetAspectRatio()
		: aspectRatio;
	const float h = 1 / tan(mFOV*0.5f);
	const float w = h / aspect;
	const float zf = mFarPlane;
	const float zn = mNearPlane;
	const float d = zf / (zf - zn);

	return Math::Matrix4
	{
		w,0.0f,0.0f,0.0f,
		0.0f,h,0.0f,0.0f,
		0.0f,0.0f,d,1.0f,
		0.0f,0.0f,-zn * d,0.0f
	};
}

Geometry::Ray Camera::ScreenPointToRay(int screenX, int screenY) const
{
	const float screenWidth = static_cast<float>(GraphicsSystem::Get()->GetWidth());
	const float screenHeight = static_cast<float>(GraphicsSystem::Get()->GetHeight());
	const float aspect = screenWidth / screenHeight;
	const float halfWidth = screenWidth * 0.5f;
	const float halfHeight = screenHeight * 0.5f;
	const float tanFOV = tanf(mFOV*0.5f);
	const float dx = tanFOV * (static_cast<float>(screenX) / halfWidth - 1.0f)*aspect;
	const float dy = tanFOV * (1.0f - static_cast<float>(screenY) / halfHeight);

	Geometry::Ray ray;
	ray.org = Math::Vector3::Zero();
	ray.dir = Math::Normalize(Math::Vector3(dx, dy, 1.0f));

	Math::Matrix4 invMatView = Math::Inverse(GetViewMatrix());
	ray.org = Math::TransformCoord(ray.org, invMatView);
	ray.dir = Math::TransformNormal(ray.dir, invMatView);
	return ray;
}

Math::Matrix4 S::Graphics::ComputeViewMatrix(Math::Vector3 right, Math::Vector3 up, Math::Vector3 look, Math::Vector3 position)
{
	const float dx = -Math::Dot(right, position);
	const float dy = -Math::Dot(up, position);
	const float dz = -Math::Dot(look, position);

	return Math::Matrix4
	{
		right.x,up.x,look.x,0.0f,
		right.y,up.y,look.y,0.0f,
		right.z,up.z,look.z,0.0f,
		dx,dy,dz,1.0f
	};
}

Math::Matrix4 S::Graphics::ComputePerspectiveMatrix(float n, float f, float fov, float aspect)
{
	const float h = 1 / tan(fov*0.5f);
	const float w = h / aspect;
	const float zf = f;
	const float zn = n;
	const float d = zf / (zf - zn);

	return Math::Matrix4
	{
		w,0.0f,0.0f,0.0f,
		0.0f,h,0.0f,0.0f,
		0.0f,0.0f,d,1.0f,
		0.0f,0.0f,-zn * d,0.0f
	};
}