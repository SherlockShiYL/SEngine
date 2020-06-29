#ifndef INCLUDED_GRAPHICS_BONE_H
#define INCLUDED_GRAPHICS_BONE_H

#include "Common.h"

namespace S{
namespace Graphics{

struct Bone
{
	std::string name;
	int index{ 0 };

	Bone* parent{ nullptr };
	int parentIndex{ -1 };

	std::vector<Bone*> children;
	std::vector<int> childIndices;

	Math::Matrix4 toParentTransform;
	Math::Matrix4 offsetTransform;
};


} // namespace Graphics
} // namespace S

#endif // !INCLUDED_GRAPHICS_BONE_H