#ifndef INCLUDED_GRAPHICS_MESHBUILDER_H
#define INCLUDED_GRAPHICS_MESHBUILDER_H

#include "Mesh.h"

namespace S {
namespace Graphics {

class MeshBuilder
{
public:
	// Add new vertex type "VertexPT", which will have position and uv
	// Added helper functions to MeshBuilder to generate different meshes

	static void GenerateCubePC(MeshPC& mesh);
	static void GenerateSpherePC(MeshPC& mesh, uint32_t slices = 32, uint32_t rings = 17);
	static void GenerateSpherePNC(MeshPNC& mesh, uint32_t slices = 32, uint32_t rings = 17);
	static void GenerateCubePT(MeshPT& mesh);
	static void GenerateSpherePT(MeshPT& mesh, uint32_t slices = 32, uint32_t rings = 17);
	static void GenerateSpherePNT(MeshPNT& mesh, uint32_t slices = 32, uint32_t rings = 17);
	static void GenerateSphere(Mesh& mesh, uint32_t slices = 32, uint32_t rings = 17);
	static void GenerateCube(Mesh& mesh);
	static void GeneratePlane(Mesh& mesh, uint32_t rows = 256, uint32_t columns = 256, float size = 1.0f);
	static void GenerateCylinder(BoneMesh& mesh, float height = 2.0f, float radius = 1.0f, uint32_t slices = 32, uint32_t rings = 17);

	static void GenerateSphereByCube(Mesh& mesh, uint32_t cubeslices = 10u);
	static void GenerateSkySphere(MeshPT& mesh, uint32_t slices = 32, uint32_t rings = 17);
	static void GenerateSkyBox(MeshPT& mesh, float size);
	static void GenerateTerrain(Mesh& mesh, uint32_t seed = 233u, float height = 10.0f, uint32_t rows = 256, uint32_t columns = 256, uint32_t size = 1u);

	static void ComputeNormals(Mesh& mesh);
	static Geometry::AABB ComputeBound(const Mesh& mesh);
};

// usage:
//void main()
//{
//	MeshPC mesh;
//	MeshBuilder::GenerateCubePC(mesh);
//}

} // namespace Graphics
} // namespace S

#endif // !INCLUDED_GRAPHICS_MESHBUILDER_H