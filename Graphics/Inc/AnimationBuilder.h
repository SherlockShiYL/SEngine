#ifndef INCLUDED_GRAPHICS_ANIMATIONBUILDER_H
#define INCLUDED_GRAPHICS_ANIMATIONBUILDER_H

#include "Animation.h"

namespace S {
namespace Graphics {

class AnimationBuilder
{
public:
	AnimationBuilder() = default;
	AnimationBuilder& AddPositionKey(const Math::Vector3 position, const float time);
	AnimationBuilder& AddScaleKey(const Math::Vector3 scale, const float time);
	AnimationBuilder& AddRotationkey(const Math::Quaternion quaternion, const float time);
	AnimationBuilder& Addkey(const Math::Vector3 position, const Math::Quaternion quaternion, const float time);

	Animation Finalize();

private:
	Animation mAnimation;

};

} // namespace Graphics
} // namespace S

#endif // !INCLUDED_GRAPHICS_ANIMATIONBUILDER_H