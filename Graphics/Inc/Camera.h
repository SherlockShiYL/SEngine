#ifndef INCLUDED_GRAPHICS_CAMERA_H
#define INCLUDED_GRAPHICS_CAMERA_H

namespace S {
namespace Graphics {

	Math::Matrix4 ComputeViewMatrix(Math::Vector3 right, Math::Vector3 up, Math::Vector3 look, Math::Vector3 position);
	Math::Matrix4 ComputePerspectiveMatrix(float n, float f, float fov, float aspect);

// This camera assumes y-axis as up direction always
class Camera
{
public:
	Camera();

	void SetPosition(const Math::Vector3& position);
	void SetDirection(const Math::Vector3& direction);
	void SetLookAt(const Math::Vector3& target);

	void Walk(float distance);
	void Strafe(float distance);
	void Rise(float distance);

	void Yaw(float radian);
	void Pitch(float radian);

	void SetFOV(float fov);
	void SetNearPlane(float nearPlane);
	void SetFarPlane(float farPlane);

	Math::Vector3 GetPosition() const	{ return mPosition; }
	Math::Vector3 GetDirection() const	{ return mDirection; }
	float GetFOV()						{ return mFOV; }
	float GetNearPlane()				{ return mNearPlane; }
	float GetFarPlane()					{ return mFarPlane; }

	Math::Matrix4 GetViewMatrix() const;
	Math::Matrix4 GetPerspectiveMatrix(float aspectRatio = 0.0f) const;

	Geometry::Ray ScreenPointToRay(int screenX, int screenY) const;

private:
	Math::Vector3 mPosition;
	Math::Vector3 mDirection{ 0.0f,0.0f,1.0f };

	float mFOV{ 60.0f*Math::kDegToRad };
	float mNearPlane{ 0.01f };
	float mFarPlane{ 10000.0f };
};

} // namespace Graphics
} // namespace S

#endif // !INCLUDED_GRAPHICS_CAMERA_H