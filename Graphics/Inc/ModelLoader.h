#ifndef INCLUDED_GRAPHICS_MODELLOADER_H
#define INCLUDED_GRAPHICS_MODELLOADER_H

#include "Mesh.h"
#include "Model.h"

namespace S {
namespace Graphics {

class ModelLoader
{
public:
	static void LoadOBJ(const char* fileName, float scale, Mesh& mesh);
	static void LoadOBJVertex(const char* fileName, float scale, Mesh& mesh);
	static void LoadSkeleton(const char* fileName, AnimatedModel& model);
};

} // namespace Graphics
} // namespace Engine

#endif // #ifndef INCLUDED_GRAPHICS_MODELLOADER_H