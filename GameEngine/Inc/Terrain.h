#ifndef INCLUDED_GAMEENGINE_TERRAIN_H
#define INCLUDED_GAMEENGINE_TERRAIN_H

#include "Quadtree.h"

namespace S {

class Terrain
{
public:
	Terrain() = default;

	void Initialize(const wchar_t* shader, const char* texture, const char* textureSelec, uint32_t row = 256u, uint32_t col = 256u, uint32_t size = 1u, float height = 5.0f);
	void Terminate();

	bool Intersect(Geometry::Ray ray, float& distance) const;

	void AddProjection(Math::Vector3 p);

	void Render(const Graphics::Camera& camera, float aspect = 0.0f);

	void DrawEditorUI();

private:
	void GenerateVertices();
	void GenerateIndices();

	struct ConstantData
	{
		Math::Matrix4 world;
		Math::Matrix4 view;
		Math::Matrix4 projection;
		Math::Matrix4 projectionView;
		Math::Matrix4 projProjection;
		Graphics::DirectionalLight l{
		{ 1.0f,0.0f,0.0f },
		{ 1.0f,1.0f,1.0f,0.0f },
		{ 1.0f,1.0f,1.0f,0.0f },
		{ 1.0f,1.0f,1.0f,0.0f } };
		Graphics::Material m{
		{ 1.0f,1.0f,1.0f,0.0f },
		{ 1.0f,1.0f,1.0f,0.0f },
		{ 1.0f,1.0f,1.0f,0.0f },
		7.0f };
		Math::Vector4 viewPosition;
	};

	ConstantData data;

	Graphics::TypedConstantBuffer<ConstantData> mConstantBuffer;
	Graphics::VertexShader mTerrainVertexShader;
	Graphics::PixelShader mTerrainPixelShader;

	Graphics::Texture mTexture;
	Graphics::Sampler mSampler;
	Graphics::Texture mSelectionTexture;

	Graphics::Mesh mMesh;
	Graphics::MeshBuffer mMeshBuffer;

	Quadtree mQuadtree;

	float mFrequency0{ 5.0f };
	float mFrequency1{ 40.0f };
	float mWeight0{ 25.0f };
	float mWeight1{ 1.0f };
	int mNumProj{ 0 };

	uint32_t mRows{ 256 };
	uint32_t mCols{ 256 };
	uint32_t mCellsInCol{ 255 };
	uint32_t mCellsInRow{ 255 };
	uint32_t mCells{ 65536 };
};

} // namespace S

#endif // #ifndef INCLUDED_GAMEENGINE_TERRAIN_H