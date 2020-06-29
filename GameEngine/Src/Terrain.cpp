#include "Precompiled.h"
#include "Terrain.h"

#include <External/ImGui/Inc/imgui.h>

using namespace S;

void Terrain::Initialize(const wchar_t* shader, const char* texture, const char* textureSelec, uint32_t row, uint32_t col, uint32_t size, float height)
{
	mRows = row;
	mCols = col;
	mCellsInCol = mRows - 1;
	mCellsInRow = mCols - 1;
	mCells = mCellsInCol * mCellsInRow;

	mConstantBuffer.Initialize();
	mTerrainVertexShader.Initialize(shader, Graphics::Vertex::Format);
	mTerrainPixelShader.Initialize(shader);

	mSampler.Initialize(Graphics::Sampler::Filter::Anisotropic, Graphics::Sampler::AddressMode::Wrap);
	mTexture.Initialize(texture);
	mSelectionTexture.Initialize(textureSelec);

	Graphics::MeshBuilder::GenerateTerrain(mMesh, 100, height, row, col, size);
	mMeshBuffer.Initialize(mMesh, true);

	mQuadtree.Initialize(&mMesh, 5);
}

void Terrain::Terminate()
{
	mMeshBuffer.Terminate();
	mMesh.Destroy();
	mSelectionTexture.Terminate();
	mTexture.Terminate();
	mSampler.Terminate();

	mTerrainPixelShader.Terminate();
	mTerrainVertexShader.Terminate();
	mConstantBuffer.Terminate();
}

bool Terrain::Intersect(Geometry::Ray ray, float& distance) const
{
	return mQuadtree.Intersect(ray, distance);
}

void Terrain::AddProjection(Math::Vector3 p)
{
	data.projectionView = Graphics::ComputeViewMatrix(
		{ 1.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f },
		{ 0.0f, -1.0f, 0.0f },
		p + Math::Vector3{ 0.0f, 5.0f, 0.0f }
	);
	++mNumProj;
}

void Terrain::Render(const Graphics::Camera& camera, float aspect)
{
	static bool showQuadtree = false;
	if (showQuadtree)
	{
		Quadtree::Visitor visitor = [](Quadtree::Node* node) {
			node->DrawNode();
		};
		mQuadtree.Visit(visitor);
	}

	auto world = Math::Matrix4::Identity();
	world = Math::Translation({-400.0f,0.0f,-400.0f});
	auto view = camera.GetViewMatrix();
	auto projection = camera.GetPerspectiveMatrix(aspect);

	data.world = Math::Transpose(world);
	data.view = Math::Transpose(view);
	data.projection = Math::Transpose(projection);
	
	data.projProjection = Graphics::ComputePerspectiveMatrix(0.1f, 100.0f, Math::kPiByTwo, 1.0f);
	data.viewPosition ={ camera.GetPosition(), static_cast<float>(mNumProj) };
	mConstantBuffer.Set(data);
	mConstantBuffer.BindVS();
	mConstantBuffer.BindPS();

	mSampler.BindPS();
	mTexture.BindPS();
	mSelectionTexture.BindPS(1);

	mTerrainVertexShader.Bind();
	mTerrainPixelShader.Bind();

	mMeshBuffer.Render();
	mNumProj = 0;
}

void Terrain::DrawEditorUI()
{
	if (ImGui::CollapsingHeader("Terrain Editor"))
	{
		if (ImGui::SliderFloat("Frequency 0", &mFrequency0, 0.0f, 100.0f) ||
			ImGui::SliderFloat("Frequency 1", &mFrequency1, 0.0f, 100.0f) ||
			ImGui::SliderFloat("Weight 0", &mWeight0, 0.0f, 50.0f) ||
			ImGui::SliderFloat("Weight 1", &mWeight1, 0.0f, 50.0f))
		{
			GenerateVertices();
			mMeshBuffer.UpdateVertices(mMesh.GetVertices(), mMesh.GetVertexCount());
		}
	}
}

void Terrain::GenerateVertices()
{
	// Generate mesh for the terrain
	// +---+---+---+
	// | \ | \ | \ |
	// +---+---+---+
	// | \ | \ | \ |
	// +---+---+---+
	// | \ | \ | \ |
	// +---+---+---+
	//std::unique_ptr<float[]> heightValues = std::make_unique<float[]>(numRows * numCols);
	//
	//FILE* file = nullptr;
	//fopen_s(&file, "../Assets/Heightmaps/heightmap_200x200.raw", "rb");
	//for (uint32_t i = 0; i < numRows * numCols; ++i)
	//{
	//	heightValues[i] = fgetc(file) / 255.0f * 10.0f;
	//}
	//fclose(file);

	Math::PerlinNoise noise;
	for (uint32_t z = 0; z < mRows; ++z)
	{
		for (uint32_t x = 0; x < mCols; ++x)
		{
			uint32_t index = x + (z * mCols);
			auto& vertex = mMesh.GetVertex(index);

			float normX = static_cast<float>(x / mCols);
			float normZ = static_cast<float>(z / mRows);
			float noise0 = mWeight0 * static_cast<float>(noise.Get(static_cast<double>(mFrequency0 * normX), 0.0, static_cast<double>(mFrequency0 * normZ)));
			float noise1 = mWeight1 * static_cast<float>(noise.Get(static_cast<double>(mFrequency1 * normX), 0.0, static_cast<double>(mFrequency1 * normZ)));

			vertex.position =
			{
				 static_cast<float>(x) * 2.0f,
				//heightValues[index],
				vertex.position.y = (noise0 + noise1),
				 static_cast<float>(z) * 2.0f
			};
			vertex.normal = { 0.0f, 1.0f, 0.0f };
			vertex.tangent = { 1.0f, 0.0f, 0.0f };
			vertex.texcoord.x = normX;
			vertex.texcoord.y = normZ;
		}
	}

	Graphics::MeshBuilder::ComputeNormals(mMesh);
}

void Terrain::GenerateIndices()
{
	uint32_t i = 0;
	for (uint32_t z = 0; z < mCellsInCol; ++z)
	{
		for (uint32_t x = 0; x < mCellsInRow; ++x)
		{
			uint32_t lb = x + (z * mCols);
			uint32_t lt = x + ((z + 1) * mCols);
			uint32_t rb = (x + 1) + (z * mCols);
			uint32_t rt = (x + 1) + ((z + 1) * mCols);

			mMesh.GetIndex(i++) = lb;
			mMesh.GetIndex(i++) = lt;
			mMesh.GetIndex(i++) = rb;

			mMesh.GetIndex(i++) = lt;
			mMesh.GetIndex(i++) = rt;
			mMesh.GetIndex(i++) = rb;
		}
	}
}