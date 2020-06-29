#ifndef INCLUDED_GRAPHICS_KEYFRAME_H
#define INCLUDED_GRAPHICS_KEYFRAME_H

namespace S {
namespace Graphics {

template <class T>
struct Keyframe
{
	T mValue;
	float mTime; // Number of Frame
};
//struct Keyframe
//{
//	Math::Vector3 mPosition;
//	Math::Quaternion mRotation;
//	Math::Vector3 mScale;
//	float mTime;
//};

} // namespace Graphics
} // namespace S

#endif // !INCLUDED_GRAPHICS_KEYFRAME_H