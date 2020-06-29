#include "Precompiled.h"
#include "MeshBuilder.h"

using namespace S;
using namespace S::Graphics;

void MeshBuilder::GenerateCubePC(MeshPC& mesh)
{
	VertexPC sVertices[] =
	{
		// FRONT
		{{1.0f,1.0f,1.0f},Math::Vector4::Orange()},		//x y z			+++
		{{-1.0f,1.0f,1.0f},Math::Vector4::Orange()},	//-x y z		-++
		{{-1.0f,-1.0f,1.0f},Math::Vector4::Orange()},	//-x -y z		--+
		{{1.0f,-1.0f,1.0f},Math::Vector4::Orange()},	//x -y z		+-+

		// BACK
		{{-1.0f,1.0f,-1.0f},Math::Vector4::Magenta()},	//-x y -z		-+-
		{{1.0f,1.0f,-1.0f},Math::Vector4::Magenta()},	//x y -z		++-
		{{1.0f,-1.0f,-1.0f},Math::Vector4::Magenta()},	//x -y -z		+--
		{{-1.0f,-1.0f,-1.0f},Math::Vector4::Magenta()},	//-x -y -z		---

		// LEFT
		{{1.0f,1.0f,-1.0f},Math::Vector4::Cyan()},		//x y -z		++-
		{{1.0f,1.0f,1.0f},Math::Vector4::Cyan()},		//x y z			+++
		{{1.0f,-1.0f,1.0f},Math::Vector4::Cyan()},		//x -y z		+-+
		{{1.0f,-1.0f,-1.0f},Math::Vector4::Cyan()},		//x -y -z		+--

		// RIGHT
		{{-1.0f,1.0f,1.0f},Math::Vector4::Green()},		//-x y z		-++
		{{-1.0f,1.0f,-1.0f},Math::Vector4::Green()},	//-x y -z		-+-
		{{-1.0f,-1.0f,-1.0f},Math::Vector4::Green()},	//-x -y -z		---
		{{-1.0f,-1.0f,1.0f},Math::Vector4::Green()},	//-x -y z		--+

		// TOP
		{{-1.0f,1.0f,1.0f},Math::Vector4::White()},		//-x y z		-++
		{{1.0f,1.0f,1.0f},Math::Vector4::White()},		//x y z			+++
		{{1.0f,1.0f,-1.0f},Math::Vector4::White()},		//x y -z		++-
		{{-1.0f,1.0f,-1.0f},Math::Vector4::White()},		//-x y -z		-+-

		// BOTTOM
		{{-1.0f,-1.0f,-1.0f},Math::Vector4::Yellow()},	//-x -y -z		---
		{{1.0f,-1.0f,-1.0f},Math::Vector4::Yellow()},	//x -y -z		+--
		{{1.0f,-1.0f,1.0f},Math::Vector4::Yellow()},	//x -y z		+-+
		{{-1.0f,-1.0f,1.0f},Math::Vector4::Yellow()},	//-x -y z		--+
	};

	uint32_t sIndices[] =
	{
		0,1,2,
		0,2,3,
		4,5,6,
		4,6,7,
		8,9,10,
		8,10,11,
		12,13,14,
		12,14,15,
		16,17,18,
		16,18,19,
		20,21,22,
		20,22,23
	};

	uint32_t numVertices = sizeof(sVertices) / sizeof(sVertices[0]);
	uint32_t numIndices = sizeof(sIndices) / sizeof(sIndices[0]);

	mesh.Allocate(numVertices, numIndices);

	for (uint32_t i = 0; i < numVertices; ++i)
	{
		mesh.GetVertex(i) = sVertices[i];
	}
	for (uint32_t i = 0; i < numIndices; ++i)
	{
		mesh.GetIndex(i) = sIndices[i];
	}
}

void MeshBuilder::GenerateSpherePC(MeshPC & mesh, uint32_t slices, uint32_t rings)
{
	uint32_t numVertices = (slices + 1)*rings;
	uint32_t numIndices = slices * (rings - 1) * 6;
	const float sliceStep = Math::kTwoPi / static_cast<float>(slices);
	const float ringStep = Math::kPi / static_cast<float>(rings - 1);

	VertexPC* sVertices = new VertexPC[numVertices];
	uint32_t* sIndices = new uint32_t[numIndices];

	uint32_t index = 0;
	float jStep = 1.0f / static_cast<float>(rings - 1);
	for (uint32_t i = 0; i <= slices; ++i)
	{
		for (uint32_t j = 0; j < rings; ++j)
		{
			sVertices[index].position.y = cosf(ringStep*j);
			float _x = sinf(ringStep*j);
			sVertices[index].position.x = cosf(sliceStep*i)*_x;
			sVertices[index].position.z = sinf(sliceStep*i)*_x;
			sVertices[index].color = { 1.0f - 1.0f*jStep*j,0.0f,1.0f*jStep*j,1.0f };
			++index;
		}
	}

	index = 0;
	for (uint32_t i = 0; i < slices; ++i)
	{
		for (uint32_t j = 0; j < rings - 1; ++j)
		{
			sIndices[index++] = i * rings + j;
			sIndices[index++] = (i + 1) * rings + j;
			sIndices[index++] = (i + 1) * rings + j + 1;
			sIndices[index++] = i * rings + j;
			sIndices[index++] = (i + 1) * rings + j + 1;
			sIndices[index++] = i * rings + j + 1;
		}
	}

	mesh.Allocate(numVertices, numIndices);

	for (uint32_t i = 0; i < numVertices; ++i)
	{
		mesh.GetVertex(i) = sVertices[i];
	}
	for (uint32_t i = 0; i < numIndices; ++i)
	{
		mesh.GetIndex(i) = sIndices[i];
	}

	SafeDeleteArray(sVertices);
	SafeDeleteArray(sIndices);
}

void MeshBuilder::GenerateSpherePNC(MeshPNC & mesh, uint32_t slices, uint32_t rings)
{
	uint32_t numVertices = (slices + 1)*rings;
	uint32_t numIndices = slices * (rings - 1) * 6;
	const float sliceStep = Math::kTwoPi / static_cast<float>(slices);
	const float ringStep = Math::kPi / static_cast<float>(rings - 1);

	VertexPNC* sVertices = new VertexPNC[numVertices];
	uint32_t* sIndices = new uint32_t[numIndices];

	uint32_t index = 0;
	for (uint32_t i = 0; i <= slices; ++i)
	{
		for (uint32_t j = 0; j < rings; ++j)
		{
			sVertices[index].position.y = cosf(ringStep*j);
			float _x = sinf(ringStep*j);
			sVertices[index].position.x = cosf(sliceStep*i)*_x;
			sVertices[index].position.z = sinf(sliceStep*i)*_x;
			sVertices[index].color = Math::Vector4::Orange();
			sVertices[index].normal = sVertices[index].position;
			++index;
		}
	}

	index = 0;
	for (uint32_t i = 0; i < slices; ++i)
	{
		for (uint32_t j = 0; j < rings - 1; ++j)
		{
			sIndices[index++] = i * rings + j;
			sIndices[index++] = (i + 1) * rings + j;
			sIndices[index++] = (i + 1) * rings + j + 1;
			sIndices[index++] = i * rings + j;
			sIndices[index++] = (i + 1) * rings + j + 1;
			sIndices[index++] = i * rings + j + 1;
		}
	}

	mesh.Allocate(numVertices, numIndices);

	for (uint32_t i = 0; i < numVertices; ++i)
	{
		mesh.GetVertex(i) = sVertices[i];
	}
	for (uint32_t i = 0; i < numIndices; ++i)
	{
		mesh.GetIndex(i) = sIndices[i];
	}

	SafeDeleteArray(sVertices);
	SafeDeleteArray(sIndices);
}

void MeshBuilder::GenerateCubePT(MeshPT & mesh)
{
	VertexPT sVertices[] =
	{
		// FRONT
		{{1.0f,1.0f,1.0f},{0.0f,0.0f}},		//x y z			+++
		{{-1.0f,1.0f,1.0f},{1.0f,0.0f}},	//-x y z		-++
		{{-1.0f,-1.0f,1.0f},{1.0f,1.0f}},	//-x -y z		--+
		{{1.0f,-1.0f,1.0f},{0.0f,1.0f}},	//x -y z		+-+

		// BACK
		{{-1.0f,1.0f,-1.0f},{0.0f,0.0f}},	//-x y -z		-+-
		{{1.0f,1.0f,-1.0f},{1.0f,0.0f}},	//x y -z		++-
		{{1.0f,-1.0f,-1.0f},{1.0f,1.0f}},	//x -y -z		+--
		{{-1.0f,-1.0f,-1.0f},{0.0f,1.0f}},	//-x -y -z		---

		// LEFT
		{{1.0f,1.0f,-1.0f},{0.0f,0.0f}},	//x y -z		++-
		{{1.0f,1.0f,1.0f},{1.0f,0.0f}},		//x y z			+++
		{{1.0f,-1.0f,1.0f},{1.0f,1.0f}},	//x -y z		+-+
		{{1.0f,-1.0f,-1.0f},{0.0f,1.0f}},	//x -y -z		+--

		// RIGHT
		{{-1.0f,1.0f,1.0f},{0.0f,0.0f}},	//-x y z		-++
		{{-1.0f,1.0f,-1.0f},{1.0f,0.0f}},	//-x y -z		-+-
		{{-1.0f,-1.0f,-1.0f},{1.0f,1.0f}},	//-x -y -z		---
		{{-1.0f,-1.0f,1.0f},{0.0f,1.0f}},	//-x -y z		--+

		// TOP
		{{-1.0f,1.0f,1.0f},{0.0f,0.0f}},	//-x y z		-++
		{{1.0f,1.0f,1.0f},{1.0f,0.0f}},		//x y z			+++
		{{1.0f,1.0f,-1.0f},{1.0f,1.0f}},	//x y -z		++-
		{{-1.0f,1.0f,-1.0f},{0.0f,1.0f}},	//-x y -z		-+-

		// BOTTOM
		{{-1.0f,-1.0f,-1.0f},{0.0f,0.0f}},	//-x -y -z		---
		{{1.0f,-1.0f,-1.0f},{1.0f,0.0f}},	//x -y -z		+--
		{{1.0f,-1.0f,1.0f},{1.0f,1.0f}},	//x -y z		+-+
		{{-1.0f,-1.0f,1.0f},{0.0f,1.0f}},	//-x -y z		--+
	};

	uint32_t sIndices[] =
	{
		0,1,2,
		0,2,3,
		4,5,6,
		4,6,7,
		8,9,10,
		8,10,11,
		12,13,14,
		12,14,15,
		16,17,18,
		16,18,19,
		20,21,22,
		20,22,23
	};

	const uint32_t numVertices = sizeof(sVertices) / sizeof(sVertices[0]);
	const uint32_t numIndices = sizeof(sIndices) / sizeof(sIndices[0]);

	mesh.Allocate(numVertices, numIndices);

	for (int i = 0; i < numVertices; ++i)
	{
		mesh.GetVertex(i) = sVertices[i];
	}
	for (int i = 0; i < numIndices; ++i)
	{
		mesh.GetIndex(i) = sIndices[i];
	}
}

void MeshBuilder::GenerateSpherePT(MeshPT & mesh, uint32_t slices, uint32_t rings)
{
	uint32_t numVertices = (slices + 1)*rings;
	uint32_t numIndices = slices * (rings - 1) * 6;
	const float sliceStep = Math::kTwoPi / static_cast<float>(slices);
	const float ringStep = Math::kPi / static_cast<float>(rings - 1);

	VertexPT* sVertices = new VertexPT[numVertices];
	uint32_t* sIndices = new uint32_t[numIndices];

	uint32_t index = 0;
	float iStep = 1.0f / static_cast<float>(slices);
	float jStep = 1.0f / static_cast<float>(rings - 1);
	for (uint32_t i = 0; i <= slices; ++i)
	{
		for (uint32_t j = 0; j < rings; ++j)
		{
			sVertices[index].position.y = cosf(ringStep*j);
			float _x = sinf(ringStep*j);
			sVertices[index].position.x = cosf(sliceStep*i)*_x;
			sVertices[index].position.z = sinf(sliceStep*i)*_x;
			sVertices[index].texcoord = { iStep*i,jStep*j };
			++index;
		}
	}

	index = 0;
	for (uint32_t i = 0; i < slices; ++i)
	{
		for (uint32_t j = 0; j < rings - 1; ++j)
		{
			sIndices[index++] = i * rings + j;
			sIndices[index++] = (i + 1) * rings + j;
			sIndices[index++] = (i + 1) * rings + j + 1;
			sIndices[index++] = i * rings + j;
			sIndices[index++] = (i + 1) * rings + j + 1;
			sIndices[index++] = i * rings + j + 1;
		}
	}

	mesh.Allocate(numVertices, numIndices);

	for (uint32_t i = 0; i < numVertices; ++i)
	{
		mesh.GetVertex(i) = sVertices[i];
	}
	for (uint32_t i = 0; i < numIndices; ++i)
	{
		mesh.GetIndex(i) = sIndices[i];
	}

	SafeDeleteArray(sVertices);
	SafeDeleteArray(sIndices);
}

void MeshBuilder::GenerateSpherePNT(MeshPNT & mesh, uint32_t slices, uint32_t rings)
{
	uint32_t numVertices = (slices + 1)*rings;
	uint32_t numIndices = slices * (rings - 1) * 6;
	const float sliceStep = Math::kTwoPi / static_cast<float>(slices);
	const float ringStep = Math::kPi / static_cast<float>(rings - 1);

	VertexPNT* sVertices = new VertexPNT[numVertices];
	uint32_t* sIndices = new uint32_t[numIndices];

	uint32_t index = 0;
	float iStep = 1.0f / static_cast<float>(slices);
	float jStep = 1.0f / static_cast<float>(rings - 1);
	for (uint32_t i = 0; i <= slices; ++i)
	{
		for (uint32_t j = 0; j < rings; ++j)
		{
			sVertices[index].position.y = cosf(ringStep*j);
			float _x = sinf(ringStep*j);
			sVertices[index].position.x = cosf(sliceStep*i)*_x;
			sVertices[index].position.z = sinf(sliceStep*i)*_x;
			sVertices[index].texcoord = { iStep*i,jStep*j };
			sVertices[index].normal = sVertices[index].position;
			++index;
		}
	}

	index = 0;
	for (uint32_t i = 0; i < slices; ++i)
	{
		for (uint32_t j = 0; j < rings - 1; ++j)
		{
			sIndices[index++] = i * rings + j;
			sIndices[index++] = (i + 1) * rings + j;
			sIndices[index++] = (i + 1) * rings + j + 1;
			sIndices[index++] = i * rings + j;
			sIndices[index++] = (i + 1) * rings + j + 1;
			sIndices[index++] = i * rings + j + 1;
		}
	}

	mesh.Allocate(numVertices, numIndices);

	for (uint32_t i = 0; i < numVertices; ++i)
	{
		mesh.GetVertex(i) = sVertices[i];
	}
	for (uint32_t i = 0; i < numIndices; ++i)
	{
		mesh.GetIndex(i) = sIndices[i];
	}
	SafeDeleteArray(sVertices);
	SafeDeleteArray(sIndices);
}

void MeshBuilder::GenerateSphere(Mesh & mesh, uint32_t slices, uint32_t rings)
{
	uint32_t numVertices = (slices + 1)*rings;
	uint32_t numIndices = slices * (rings - 1) * 6;
	const float sliceStep = Math::kTwoPi / static_cast<float>(slices);
	const float ringStep = Math::kPi / static_cast<float>(rings - 1);

	Vertex* sVertices = new Vertex[numVertices];
	uint32_t* sIndices = new uint32_t[numIndices];

	uint32_t index = 0;
	float iStep = 1.0f / static_cast<float>(slices);
	float jStep = 1.0f / static_cast<float>(rings - 1);
	for (uint32_t i = 0; i <= slices; ++i)
	{
		for (uint32_t j = 0; j < rings; ++j)
		{
			sVertices[index].position.y = cosf(ringStep*j);
			float _x = sinf(ringStep*j);
			sVertices[index].position.x = cosf(sliceStep*i)*_x;
			sVertices[index].position.z = sinf(sliceStep*i)*_x;
			sVertices[index].texcoord = { iStep*i,jStep*j };
			sVertices[index].normal = sVertices[index].position;
			++index;
		}
	}

	index = 0;
	for (uint32_t i = 0; i <= slices; ++i)
	{
		int temp = index + 1;
		sVertices[temp].tangent = Math::Normalize({ -sVertices[temp].normal.z,0.0f,sVertices[temp].normal.x });
		for (uint32_t j = 0; j < rings; ++j)
		{
			sVertices[index++].tangent = Math::Normalize({ -sVertices[temp].normal.z,0.0f,sVertices[temp].normal.x });
		}
	}

	index = 0;
	for (uint32_t i = 0; i < slices; ++i)
	{
		for (uint32_t j = 0; j < rings - 1; ++j)
		{
			sIndices[index++] = i * rings + j;
			sIndices[index++] = (i + 1) * rings + j;
			sIndices[index++] = (i + 1) * rings + j + 1;
			sIndices[index++] = i * rings + j;
			sIndices[index++] = (i + 1) * rings + j + 1;
			sIndices[index++] = i * rings + j + 1;
		}
	}

	mesh.Allocate(numVertices, numIndices);

	for (uint32_t i = 0; i < numVertices; ++i)
	{
		mesh.GetVertex(i) = sVertices[i];
	}
	for (uint32_t i = 0; i < numIndices; ++i)
	{
		mesh.GetIndex(i) = sIndices[i];
	}
	SafeDeleteArray(sVertices);
	SafeDeleteArray(sIndices);
}

void MeshBuilder::GenerateCube(Mesh & mesh)
{
	Vertex sVertices[] =
	{
		// FRONT
		{{1.0f,1.0f,1.0f},{0.0f,0.0f,1.0f},{-1.0f,0.0f,0.0f},{0.0f,0.0f}},		//x y z			+++
		{{-1.0f,1.0f,1.0f},{0.0f,0.0f,1.0f},{-1.0f,0.0f,0.0f},{1.0f,0.0f}},		//-x y z		-++
		{{-1.0f,-1.0f,1.0f},{0.0f,0.0f,1.0f},{-1.0f,0.0f,0.0f},{1.0f,1.0f}},	//-x -y z		--+
		{{1.0f,-1.0f,1.0f},{0.0f,0.0f,1.0f},{-1.0f,0.0f,0.0f},{0.0f,1.0f}},		//x -y z		+-+

		// BACK
		{{-1.0f,1.0f,-1.0f},{0.0f,0.0f,-1.0f},{1.0f,0.0f,0.0f},{0.0f,0.0f}},	//-x y -z		-+-
		{{1.0f,1.0f,-1.0f},{0.0f,0.0f,-1.0f},{1.0f,0.0f,0.0f},{1.0f,0.0f}},		//x y -z		++-
		{{1.0f,-1.0f,-1.0f},{0.0f,0.0f,-1.0f},{1.0f,0.0f,0.0f},{1.0f,1.0f}},	//x -y -z		+--
		{{-1.0f,-1.0f,-1.0f},{0.0f,0.0f,-1.0f},{1.0f,0.0f,0.0f},{0.0f,1.0f}},	//-x -y -z		---

		// LEFT
		{{1.0f,1.0f,-1.0f},{1.0f,0.0f,0.0f},{0.0f,1.0f,0.0f},{0.0f,0.0f}},		//x y -z		++-
		{{1.0f,1.0f,1.0f},{1.0f,0.0f,0.0f},{0.0f,1.0f,0.0f},{1.0f,0.0f}},		//x y z			+++
		{{1.0f,-1.0f,1.0f},{1.0f,0.0f,0.0f},{0.0f,1.0f,0.0f},{1.0f,1.0f}},		//x -y z		+-+
		{{1.0f,-1.0f,-1.0f},{1.0f,0.0f,0.0f},{0.0f,1.0f,0.0f},{0.0f,1.0f}},		//x -y -z		+--

		// RIGHT
		{{-1.0f,1.0f,1.0f},{-1.0f,0.0f,0.0f},{0.0f,-1.0f,0.0f},{0.0f,0.0f}},	//-x y z		-++
		{{-1.0f,1.0f,-1.0f},{-1.0f,0.0f,0.0f},{0.0f,-1.0f,0.0f},{1.0f,0.0f}},	//-x y -z		-+-
		{{-1.0f,-1.0f,-1.0f},{-1.0f,0.0f,0.0f},{0.0f,-1.0f,0.0f},{1.0f,1.0f}},	//-x -y -z		---
		{{-1.0f,-1.0f,1.0f},{-1.0f,0.0f,0.0f},{0.0f,-1.0f,0.0f},{0.0f,1.0f}},	//-x -y z		--+

		// TOP
		{{-1.0f,1.0f,1.0f},{0.0f,1.0f,0.0f},{-1.0f,0.0f,0.0f},{0.0f,0.0f}},		//-x y z		-++
		{{1.0f,1.0f,1.0f},{0.0f,1.0f,0.0f},{-1.0f,0.0f,0.0f},{1.0f,0.0f}},		//x y z			+++
		{{1.0f,1.0f,-1.0f},{0.0f,1.0f,0.0f},{-1.0f,0.0f,0.0f},{1.0f,1.0f}},		//x y -z		++-
		{{-1.0f,1.0f,-1.0f},{0.0f,1.0f,0.0f},{-1.0f,0.0f,0.0f},{0.0f,1.0f}},	//-x y -z		-+-

		// BOTTOM
		{{-1.0f,-1.0f,-1.0f},{0.0f,-1.0f,0.0f},{-1.0f,0.0f,0.0f},{0.0f,0.0f}},	//-x -y -z		---
		{{1.0f,-1.0f,-1.0f},{0.0f,-1.0f,0.0f},{-1.0f,0.0f,0.0f},{1.0f,0.0f}},	//x -y -z		+--
		{{1.0f,-1.0f,1.0f},{0.0f,-1.0f,0.0f},{-1.0f,0.0f,0.0f},{1.0f,1.0f}},	//x -y z		+-+
		{{-1.0f,-1.0f,1.0f},{0.0f,-1.0f,0.0f},{-1.0f,0.0f,0.0f},{0.0f,1.0f}},	//-x -y z		--+
	};

	uint32_t sIndices[] =
	{
		0,1,2,
		0,2,3,
		4,5,6,
		4,6,7,
		8,9,10,
		8,10,11,
		12,13,14,
		12,14,15,
		16,17,18,
		16,18,19,
		20,21,22,
		20,22,23
	};

	uint32_t numVertices = sizeof(sVertices) / sizeof(sVertices[0]);
	uint32_t numIndices = sizeof(sIndices) / sizeof(sIndices[0]);

	mesh.Allocate(numVertices, numIndices);

	for (uint32_t i = 0; i < numVertices; ++i)
	{
		mesh.GetVertex(i) = sVertices[i];
	}
	for (uint32_t i = 0; i < numIndices; ++i)
	{
		mesh.GetIndex(i) = sIndices[i];
	}
}

void MeshBuilder::GeneratePlane(Mesh & mesh, uint32_t rows, uint32_t columns, float size)
{
	//std::unique_ptr<float[]> heightValues = std::make_unique<float[]>(rows*columns);

	// Create a PerlinNoise object with a random permutation vector generated with seed
	//Math::PerlinNoise pn(seed);

	const uint32_t numVertices = rows * columns;
	const uint32_t numIndices = (rows - 1)*(columns - 1) * 6;

	Vertex* sVertices = new Vertex[numVertices];
	uint32_t* sIndices = new uint32_t[numIndices];

	uint32_t index = 0;
	float iStep = 1.0f / static_cast<float>(rows - 1);
	float jStep = 1.0f / static_cast<float>(columns - 1);
	// bottom left to top right
	for (uint32_t i = 0; i < rows; ++i)
	{
		for (uint32_t j = 0; j < columns; ++j)
		{
			sVertices[index].position.x = static_cast<float>(j*size);
			sVertices[index].position.y = 0.0f;
			sVertices[index].position.z = static_cast<float>(i*size);
			sVertices[index].texcoord = { jStep*j,iStep*i };
			sVertices[index].normal = { 0.0f,0.0f,0.0f };
			sVertices[index].tangent = { 1.0f,0.0f,0.0f };
			++index;
		}
	}

	index = 0;
	for (uint32_t i = 0; i < rows; ++i)
	{
		for (uint32_t j = 0; j < columns; ++j)
		{
			if (j != 0 && j != columns - 1 && i != 0 && i != rows - 1)
			{
				sVertices[index].normal += S::Math::Cross(S::Math::Normalize(sVertices[index - 1].position - sVertices[index].position), S::Math::Normalize(sVertices[index + columns].position - sVertices[index].position));
				sVertices[index].normal += S::Math::Cross(S::Math::Normalize(sVertices[index + columns].position - sVertices[index].position), S::Math::Normalize(sVertices[index + 1].position - sVertices[index].position));
				sVertices[index].normal += S::Math::Cross(S::Math::Normalize(sVertices[index + 1].position - sVertices[index].position), S::Math::Normalize(sVertices[index - columns].position - sVertices[index].position));
				sVertices[index].normal += S::Math::Cross(S::Math::Normalize(sVertices[index - columns].position - sVertices[index].position), S::Math::Normalize(sVertices[index - 1].position - sVertices[index].position));
			}
			else if (j == 0 && i != 0 && i != rows - 1)
			{
				sVertices[index].normal += S::Math::Cross(S::Math::Normalize(sVertices[index + columns].position - sVertices[index].position), S::Math::Normalize(sVertices[index + 1].position - sVertices[index].position));
				sVertices[index].normal += S::Math::Cross(S::Math::Normalize(sVertices[index + 1].position - sVertices[index].position), S::Math::Normalize(sVertices[index - columns].position - sVertices[index].position));
			}
			else if (j == columns - 1 && i != 0 && i != rows - 1)
			{
				sVertices[index].normal += S::Math::Cross(S::Math::Normalize(sVertices[index - 1].position - sVertices[index].position), S::Math::Normalize(sVertices[index + columns].position - sVertices[index].position));
				sVertices[index].normal += S::Math::Cross(S::Math::Normalize(sVertices[index - columns].position - sVertices[index].position), S::Math::Normalize(sVertices[index - 1].position - sVertices[index].position));
			}
			else if (i == 0 && j != 0 && j != columns - 1)
			{
				sVertices[index].normal += S::Math::Cross(S::Math::Normalize(sVertices[index - 1].position - sVertices[index].position), S::Math::Normalize(sVertices[index + columns].position - sVertices[index].position));
				sVertices[index].normal += S::Math::Cross(S::Math::Normalize(sVertices[index + columns].position - sVertices[index].position), S::Math::Normalize(sVertices[index + 1].position - sVertices[index].position));
			}
			else if (i == rows - 1 && j != 0 && j != columns - 1)
			{
				sVertices[index].normal += S::Math::Cross(S::Math::Normalize(sVertices[index + 1].position - sVertices[index].position), S::Math::Normalize(sVertices[index - columns].position - sVertices[index].position));
				sVertices[index].normal += S::Math::Cross(S::Math::Normalize(sVertices[index - columns].position - sVertices[index].position), S::Math::Normalize(sVertices[index - 1].position - sVertices[index].position));
			}
			else if (j == 0 && i == 0)
			{
				sVertices[index].normal += S::Math::Cross(S::Math::Normalize(sVertices[index + columns].position - sVertices[index].position), S::Math::Normalize(sVertices[index + 1].position - sVertices[index].position));
			}
			else if (j == 0 && i == rows - 1)
			{
				sVertices[index].normal += S::Math::Cross(S::Math::Normalize(sVertices[index + 1].position - sVertices[index].position), S::Math::Normalize(sVertices[index - columns].position - sVertices[index].position));
			}
			else if (j == columns - 1 && i == 0)
			{
				sVertices[index].normal += S::Math::Cross(S::Math::Normalize(sVertices[index - 1].position - sVertices[index].position), S::Math::Normalize(sVertices[index + columns].position - sVertices[index].position));
			}
			else if (j == columns - 1 && i == rows - 1)
			{
				sVertices[index].normal += S::Math::Cross(S::Math::Normalize(sVertices[index - columns].position - sVertices[index].position), S::Math::Normalize(sVertices[index - 1].position - sVertices[index].position));
			}

			sVertices[index].normal = S::Math::Normalize(sVertices[index].normal);
			++index;
		}
	}

	index = 0;
	for (uint32_t i = 0; i < rows - 1; ++i)
	{
		for (uint32_t j = 0; j < columns - 1; ++j)
		{
			sIndices[index++] = j + (i*columns);				// lb
			sIndices[index++] = j + ((i + 1)*columns);			// lt
			sIndices[index++] = (j + 1) + (i *columns);		// rb
			sIndices[index++] = j + ((i + 1)*columns);			// lt
			sIndices[index++] = (j + 1) + ((i + 1) *columns);	// rt
			sIndices[index++] = (j + 1) + (i *columns);		// rb
		}
	}

	mesh.Allocate(numVertices, numIndices);

	for (uint32_t i = 0; i < numVertices; ++i)
	{
		mesh.GetVertex(i) = sVertices[i];
	}
	for (uint32_t i = 0; i < numIndices; ++i)
	{
		mesh.GetIndex(i) = sIndices[i];
	}
	SafeDeleteArray(sVertices);
	SafeDeleteArray(sIndices);
}

void MeshBuilder::GenerateCylinder(BoneMesh & mesh, float height, float radius, uint32_t slices, uint32_t rings)
{
	uint32_t numVertices = (slices + 1)*rings;
	uint32_t numIndices = slices * (rings - 1) * 6;
	const float sliceStep = Math::kTwoPi / static_cast<float>(slices);
	const float ringStep = height / static_cast<float>(rings - 1);

	BoneVertex* sVertices = new BoneVertex[numVertices];
	uint32_t* sIndices = new uint32_t[numIndices];

	uint32_t index = 0;
	float iStep = 1.0f / static_cast<float>(slices);
	float jStep = 1.0f / static_cast<float>(rings - 1);
	for (uint32_t i = 0; i <= slices; ++i)
	{
		for (uint32_t j = 0; j < rings; ++j)
		{
			sVertices[index].position.y = height - ringStep * j;
			sVertices[index].position.x = cosf(sliceStep*i);
			sVertices[index].position.z = sinf(sliceStep*i);
			sVertices[index].texcoord = { iStep*i,jStep*j };
			sVertices[index].normal = sVertices[index].position;
			if (sVertices[index].position.y < height*0.3f)
			{
				sVertices[index].boneWeight[0] = 1.0f;
			}
			else if (sVertices[index].position.y < height*0.4f)
			{
				sVertices[index].boneIndices[1] = 1;
				sVertices[index].boneWeight[0] = (height*0.4f - sVertices[index].position.y) / 0.4f;
				sVertices[index].boneWeight[1] = 1.0f - sVertices[index].boneWeight[0];
			}
			else if (sVertices[index].position.y < height*0.6f)
			{
				sVertices[index].boneIndices[0] = 1;
				sVertices[index].boneWeight[0] = 1.0f;
			}
			else if (sVertices[index].position.y < height*0.7f)
			{
				sVertices[index].boneIndices[0] = 1;
				sVertices[index].boneIndices[1] = 2;
				sVertices[index].boneWeight[0] = (height*0.7f - sVertices[index].position.y) / 0.4f;
				sVertices[index].boneWeight[1] = 1.0f - sVertices[index].boneWeight[0];
			}
			else
			{
				sVertices[index].boneIndices[0] = 2;
				sVertices[index].boneWeight[0] = 1.0f;
			}

			++index;
		}
	}

	index = 0;
	for (uint32_t i = 0; i <= slices; ++i)
	{
		int temp = index + 1;
		sVertices[temp].tangent = Math::Normalize({ -sVertices[temp].normal.z,0.0f,sVertices[temp].normal.x });
		for (uint32_t j = 0; j < rings; ++j)
		{
			sVertices[index++].tangent = Math::Normalize({ -sVertices[temp].normal.z,0.0f,sVertices[temp].normal.x });
		}
	}

	index = 0;
	for (uint32_t i = 0; i < slices; ++i)
	{
		for (uint32_t j = 0; j < rings - 1; ++j)
		{
			sIndices[index++] = i * rings + j;
			sIndices[index++] = (i + 1) * rings + j;
			sIndices[index++] = (i + 1) * rings + j + 1;
			sIndices[index++] = i * rings + j;
			sIndices[index++] = (i + 1) * rings + j + 1;
			sIndices[index++] = i * rings + j + 1;
		}
	}

	mesh.Allocate(numVertices, numIndices);

	for (uint32_t i = 0; i < numVertices; ++i)
	{
		mesh.GetVertex(i) = sVertices[i];
	}
	for (uint32_t i = 0; i < numIndices; ++i)
	{
		mesh.GetIndex(i) = sIndices[i];
	}
	SafeDeleteArray(sVertices);
	SafeDeleteArray(sIndices);
}

void MeshBuilder::GenerateSphereByCube(Mesh& mesh, uint32_t cubeslices)
{
	uint32_t numVertices = (cubeslices + 1)*(cubeslices + 1) * 6;
	uint32_t numIndices = cubeslices * cubeslices * 36;
	const float sliceStep = 1.0f / static_cast<float>(cubeslices);

	Vertex* sVertices = new Vertex[numVertices];
	uint32_t* sIndices = new uint32_t[numIndices];

	uint32_t index = 0;
	float Step = 1.0f / static_cast<float>(cubeslices);
	for (uint32_t k = 0; k < 6; ++k)
	{
		switch (k)
		{
		case 0:
			// Front
			for (uint32_t i = 0; i <= cubeslices; ++i)
			{
				for (uint32_t j = 0; j <= cubeslices; ++j)
				{
					float x = 1.0f - sliceStep * static_cast<float>(i) * 2.0f;
					float y = 1.0f - sliceStep * static_cast<float>(j) * 2.0f;
					float z = 1.0f;
					sVertices[index].position.x = x * Math::Sqrt(1.0f - y * y*0.5f - z * z*0.5f + y * y*z*z / 3.0f);
					sVertices[index].position.y = y * Math::Sqrt(1.0f - z * z*0.5f - x * x*0.5f + x * x*z*z / 3.0f);
					sVertices[index].position.z = z * Math::Sqrt(1.0f - y * y*0.5f - x * x*0.5f + x * x*y*y / 3.0f);
					sVertices[index].texcoord = { 1.0f - sliceStep * i,sliceStep*j };
					sVertices[index].normal = sVertices[index].position;
					++index;
				}
			}
			break;
		case 1:
			// Back
			for (uint32_t i = 0; i <= cubeslices; ++i)
			{
				for (uint32_t j = 0; j <= cubeslices; ++j)
				{
					float x = sliceStep * static_cast<float>(i) * 2.0f - 1.0f;
					float y = 1.0f - sliceStep * static_cast<float>(j) * 2.0f;
					float z = -1.0f;
					sVertices[index].position.x = x * Math::Sqrt(1.0f - y * y*0.5f - z * z*0.5f + y * y*z*z / 3.0f);
					sVertices[index].position.y = y * Math::Sqrt(1.0f - z * z*0.5f - x * x*0.5f + x * x*z*z / 3.0f);
					sVertices[index].position.z = z * Math::Sqrt(1.0f - y * y*0.5f - x * x*0.5f + x * x*y*y / 3.0f);
					sVertices[index].texcoord = { 1.0f - sliceStep * i,sliceStep*j };
					sVertices[index].normal = sVertices[index].position;
					++index;
				}
			}
			break;
		case 2:
			// Left
			for (uint32_t i = 0; i <= cubeslices; ++i)
			{
				for (uint32_t j = 0; j <= cubeslices; ++j)
				{
					float x = 1.0f;
					float y = 1.0f - sliceStep * static_cast<float>(j) * 2.0f;
					float z = sliceStep * static_cast<float>(i) * 2.0f - 1.0f;
					sVertices[index].position.x = x * Math::Sqrt(1.0f - y * y*0.5f - z * z*0.5f + y * y*z*z / 3.0f);
					sVertices[index].position.y = y * Math::Sqrt(1.0f - z * z*0.5f - x * x*0.5f + x * x*z*z / 3.0f);
					sVertices[index].position.z = z * Math::Sqrt(1.0f - y * y*0.5f - x * x*0.5f + x * x*y*y / 3.0f);
					sVertices[index].texcoord = { 1.0f - sliceStep * i,sliceStep*j };
					sVertices[index].normal = sVertices[index].position;
					++index;
				}
			}
			break;
		case 3:
			// Right
			for (uint32_t i = 0; i <= cubeslices; ++i)
			{
				for (uint32_t j = 0; j <= cubeslices; ++j)
				{
					float x = -1.0f;
					float y = 1.0f - sliceStep * static_cast<float>(j) * 2.0f;
					float z = 1.0f - sliceStep * static_cast<float>(i) * 2.0f;
					sVertices[index].position.x = x * Math::Sqrt(1.0f - y * y*0.5f - z * z*0.5f + y * y*z*z / 3.0f);
					sVertices[index].position.y = y * Math::Sqrt(1.0f - z * z*0.5f - x * x*0.5f + x * x*z*z / 3.0f);
					sVertices[index].position.z = z * Math::Sqrt(1.0f - y * y*0.5f - x * x*0.5f + x * x*y*y / 3.0f);
					sVertices[index].texcoord = { 1.0f - sliceStep * i,sliceStep*j };
					sVertices[index].normal = sVertices[index].position;
					++index;
				}
			}
			break;
		case 4:
			// Top
			for (uint32_t i = 0; i <= cubeslices; ++i)
			{
				for (uint32_t j = 0; j <= cubeslices; ++j)
				{
					float x = sliceStep * static_cast<float>(i) * 2.0f - 1.0f;
					float y = 1.0f;
					float z = 1.0f - sliceStep * static_cast<float>(j) * 2.0f;
					sVertices[index].position.x = x * Math::Sqrt(1.0f - y * y*0.5f - z * z*0.5f + y * y*z*z / 3.0f);
					sVertices[index].position.y = y * Math::Sqrt(1.0f - z * z*0.5f - x * x*0.5f + x * x*z*z / 3.0f);
					sVertices[index].position.z = z * Math::Sqrt(1.0f - y * y*0.5f - x * x*0.5f + x * x*y*y / 3.0f);
					sVertices[index].texcoord = { sliceStep * i,1.0f - sliceStep * j };
					sVertices[index].normal = sVertices[index].position;
					++index;
				}
			}
			break;
		case 5:
			// Botom
			for (uint32_t i = 0; i <= cubeslices; ++i)
			{
				for (uint32_t j = 0; j <= cubeslices; ++j)
				{
					float x = 1.0f - sliceStep * static_cast<float>(i) * 2.0f;
					float y = -1.0f;
					float z = 1.0f - sliceStep * static_cast<float>(j) * 2.0f;
					sVertices[index].position.x = x * Math::Sqrt(1.0f - y * y*0.5f - z * z*0.5f + y * y*z*z / 3.0f);
					sVertices[index].position.y = y * Math::Sqrt(1.0f - z * z*0.5f - x * x*0.5f + x * x*z*z / 3.0f);
					sVertices[index].position.z = z * Math::Sqrt(1.0f - y * y*0.5f - x * x*0.5f + x * x*y*y / 3.0f);
					sVertices[index].texcoord = { 1.0f - sliceStep * i,sliceStep * j };
					sVertices[index].normal = sVertices[index].position;
					++index;
				}
			}
			break;
		default:
			break;
		}		
	}

	index = 0;
	for (uint32_t k = 0; k < 6; ++k)
	{
		for (uint32_t i = 0; i <= cubeslices; ++i)
		{
			for (uint32_t j = 0; j <= cubeslices; ++j)
			{
				//sVertices[index++].tangent = Math::Normalize({ -sVertices[index].normal.z,0.0f,sVertices[index].normal.x });
			}
		}
	}
	index = 0;
	for (int k = 0; k < 6; ++k)
	{
		for (uint32_t i = 0; i < cubeslices; ++i)
		{
			for (uint32_t j = 0; j < cubeslices; ++j)
			{
				sIndices[index++] = i * (cubeslices + 1) + j + k * (cubeslices + 1)*(cubeslices + 1);
				sIndices[index++] = (i + 1) * (cubeslices + 1) + j + k * (cubeslices + 1)*(cubeslices + 1);
				sIndices[index++] = (i + 1) * (cubeslices + 1) + j + 1 + k * (cubeslices + 1)*(cubeslices + 1);
				sIndices[index++] = i * (cubeslices + 1) + j + k * (cubeslices + 1)*(cubeslices + 1);
				sIndices[index++] = (i + 1) * (cubeslices + 1) + j + 1 + k * (cubeslices + 1)*(cubeslices + 1);
				sIndices[index++] = i * (cubeslices + 1) + j + 1 + k * (cubeslices + 1) * (cubeslices + 1);
			}
		}
	}
	mesh.Allocate(numVertices, numIndices);

	for (uint32_t i = 0; i < numVertices; ++i)
	{
		mesh.GetVertex(i) = sVertices[i];
	}
	for (uint32_t i = 0; i < numIndices; ++i)
	{
		mesh.GetIndex(i) = sIndices[i];
	}
	SafeDeleteArray(sVertices);
	SafeDeleteArray(sIndices);
	
}

void MeshBuilder::GenerateSkySphere(MeshPT & mesh, uint32_t slices, uint32_t rings)
{
	uint32_t numVertices = (slices + 1)*rings;
	uint32_t numIndices = numVertices * 6;
	const float sliceStep = Math::kTwoPi / static_cast<float>(slices);
	const float ringStep = Math::kPi / static_cast<float>(rings - 1);

	VertexPT* sVertices = new VertexPT[numVertices];
	uint32_t* sIndices = new uint32_t[numIndices];

	uint32_t index = 0;
	float iStep = 1.0f / static_cast<float>(slices);
	float jStep = 1.0f / static_cast<float>(rings - 1);
	for (uint32_t i = 0; i <= slices; ++i)
	{
		for (uint32_t j = 0; j < rings; ++j)
		{
			sVertices[index].position.y = cosf(ringStep*j)*10000.0f;
			float _x = sinf(ringStep*j);
			sVertices[index].position.x = cosf(sliceStep*i)*_x*10000.0f;
			sVertices[index].position.z = sinf(sliceStep*i)*_x*10000.0f;
			sVertices[index].texcoord = { iStep*i,jStep*j };
			++index;
		}
	}

	index = 0;
	for (uint32_t i = 0; i < slices; ++i)
	{
		for (uint32_t j = 0; j < rings - 1; ++j)
		{
			sIndices[index++] = i * rings + j;
			sIndices[index++] = (i + 1) * rings + j;
			sIndices[index++] = (i + 1) * rings + j + 1;
			sIndices[index++] = i * rings + j;
			sIndices[index++] = (i + 1) * rings + j + 1;
			sIndices[index++] = i * rings + j + 1;
		}
	}

	mesh.Allocate(numVertices, numIndices);

	for (uint32_t i = 0; i < numVertices; ++i)
	{
		mesh.GetVertex(i) = sVertices[i];
	}
	for (uint32_t i = 0; i < numIndices; ++i)
	{
		mesh.GetIndex(i) = sIndices[i];
	}

	SafeDeleteArray(sVertices);
	SafeDeleteArray(sIndices);
}

void Graphics::MeshBuilder::GenerateSkyBox(MeshPT & mesh, float size)
{
	float rate = 1.0f / 3.0f;
	float correction = 1.0f / 1024.0f*rate;
	VertexPT sVertices[] =
	{
		// FRONT
		{{size,size,size},{0.5f, rate + correction}},			//x y z			+++
		{{-size,size,size},{0.25f,rate + correction}},			//-x y z		-++
		{{-size,-size,size},{0.25f,2.0f*rate - correction}},	//-x -y z		--+
		{{size,-size,size},{0.5f,2.0f*rate - correction}},		//x -y z		+-+

		// BACK
		{{-size,size,-size},{1.0f,rate + correction}},			//-x y -z		-+-
		{{size,size,-size},{0.75f,rate + correction}},			//x y -z		++-
		{{size,-size,-size},{0.75f,2.0f*rate - correction}},	//x -y -z		+--
		{{-size,-size,-size},{1.0f,2.0f*rate - correction}},	//-x -y -z		---

		// RIGHT
		{{size,size,-size},{0.75f,rate + correction}},			//x y -z		++-
		{{size,size,size},{0.5f,rate + correction}},			//x y z			+++
		{{size,-size,size},{0.5f,2.0f*rate - correction}},		//x -y z		+-+
		{{size,-size,-size},{0.75f,2.0f*rate - correction}},	//x -y -z		+--

		// LEFT
		{{-size,size,size},{0.25f,rate + correction}},			//-x y z		-++
		{{-size,size,-size},{0.0f,rate + correction}},			//-x y -z		-+-
		{{-size,-size,-size},{0.0f,2.0f*rate - correction}},	//-x -y -z		---
		{{-size,-size,size},{0.25f,2.0f*rate - correction}},	//-x -y z		--+

		// TOP
		{{-size,size,size},{0.25f,rate - correction}},			//-x y z		-++
		{{size,size,size},{0.5f,rate - correction}},			//x y z			+++
		{{size,size,-size},{0.5f,0.0f + correction}},			//x y -z		++-
		{{-size,size,-size},{0.25f,0.0f + correction}},			//-x y -z		-+-

		// BOTTOM
		{{-size,-size,-size},{0.25f,1.0f - correction}},		//-x -y -z		---
		{{size,-size,-size},{0.5f,1.0f - correction}},			//x -y -z		+--
		{{size,-size,size},{0.5f,2.0f*rate + correction}},		//x -y z		+-+
		{{-size,-size,size},{0.25f,2.0f*rate + correction}}		//-x -y z		--+
	};

	uint32_t sIndices[] =
	{
		2,1,0,
		3,2,0,
		6,5,4,
		7,6,4,
		10,9,8,
		11,10,8,
		14,13,12,
		15,14,12,
		18,17,16,
		19,18,16,
		22,21,20,
		23,22,20
	};

	const uint32_t numVertices = sizeof(sVertices) / sizeof(sVertices[0]);
	const uint32_t numIndices = sizeof(sIndices) / sizeof(sIndices[0]);

	mesh.Allocate(numVertices, numIndices);

	for (int i = 0; i < numVertices; ++i)
	{
		mesh.GetVertex(i) = sVertices[i];
	}
	for (int i = 0; i < numIndices; ++i)
	{
		mesh.GetIndex(i) = sIndices[i];
	}
}

void MeshBuilder::GenerateTerrain(Mesh & mesh, uint32_t seed, float height, uint32_t rows, uint32_t columns, uint32_t size)
{
	std::unique_ptr<float[]> heightValues = std::make_unique<float[]>(rows*columns);

	// Create a PerlinNoise object with a random permutation vector generated with seed
	Math::PerlinNoise pn(seed);

	const uint32_t numVertices = rows * columns;
	const uint32_t numIndices = (rows - 1)*(columns - 1) * 6;

	Vertex* sVertices = new Vertex[numVertices];
	uint32_t* sIndices = new uint32_t[numIndices];

	uint32_t index = 0;
	float rStep = 1.0f / static_cast<float>(rows - 1);
	float cStep = 1.0f / static_cast<float>(columns - 1);
	// bottom left to top right
	for (uint32_t i = 0; i < rows; ++i)
	{
		for (uint32_t j = 0; j < columns; ++j)
		{
			double tempC = (double)j / ((double)columns);
			double tempR = (double)i / ((double)rows);

			// Typical Perlin noise
			double n = pn.Get(10.0 * tempC, 10.0 * tempR, 0.8);

			// Wood like structure
			//n = 20 * pn.noise(x, y, 0.8);
			//n = n - floor(n);

			sVertices[index].position.x = static_cast<float>(j*size);
			sVertices[index].position.y = static_cast<float>(n)*height;
			sVertices[index].position.z = static_cast<float>(i*size);
			sVertices[index].texcoord = { cStep*j,rStep*i };
			sVertices[index].normal = { 0.0f,0.0f,0.0f };
			sVertices[index].tangent = { 1.0f,0.0f,0.0f };
			++index;
		}
	}

	index = 0;
	for (uint32_t i = 0; i < rows; ++i)
	{
		for (uint32_t j = 0; j < columns; ++j)
		{
			if (j != 0 && j != columns - 1 && i != 0 && i != rows - 1)
			{
				sVertices[index].normal += S::Math::Cross(S::Math::Normalize(sVertices[index - 1].position - sVertices[index].position), S::Math::Normalize(sVertices[index + columns].position - sVertices[index].position));
				sVertices[index].normal += S::Math::Cross(S::Math::Normalize(sVertices[index + columns].position - sVertices[index].position), S::Math::Normalize(sVertices[index + 1].position - sVertices[index].position));
				sVertices[index].normal += S::Math::Cross(S::Math::Normalize(sVertices[index + 1].position - sVertices[index].position), S::Math::Normalize(sVertices[index - columns].position - sVertices[index].position));
				sVertices[index].normal += S::Math::Cross(S::Math::Normalize(sVertices[index - columns].position - sVertices[index].position), S::Math::Normalize(sVertices[index - 1].position - sVertices[index].position));
			}
			else if (j == 0 && i != 0 && i != rows - 1)
			{
				sVertices[index].normal += S::Math::Cross(S::Math::Normalize(sVertices[index + columns].position - sVertices[index].position), S::Math::Normalize(sVertices[index + 1].position - sVertices[index].position));
				sVertices[index].normal += S::Math::Cross(S::Math::Normalize(sVertices[index + 1].position - sVertices[index].position), S::Math::Normalize(sVertices[index - columns].position - sVertices[index].position));
			}
			else if (j == columns - 1 && i != 0 && i != rows - 1)
			{
				sVertices[index].normal += S::Math::Cross(S::Math::Normalize(sVertices[index - 1].position - sVertices[index].position), S::Math::Normalize(sVertices[index + columns].position - sVertices[index].position));
				sVertices[index].normal += S::Math::Cross(S::Math::Normalize(sVertices[index - columns].position - sVertices[index].position), S::Math::Normalize(sVertices[index - 1].position - sVertices[index].position));
			}
			else if (i == 0 && j != 0 && j != columns - 1)
			{
				sVertices[index].normal += S::Math::Cross(S::Math::Normalize(sVertices[index - 1].position - sVertices[index].position), S::Math::Normalize(sVertices[index + columns].position - sVertices[index].position));
				sVertices[index].normal += S::Math::Cross(S::Math::Normalize(sVertices[index + columns].position - sVertices[index].position), S::Math::Normalize(sVertices[index + 1].position - sVertices[index].position));
			}
			else if (i == rows - 1 && j != 0 && j != columns - 1)
			{
				sVertices[index].normal += S::Math::Cross(S::Math::Normalize(sVertices[index + 1].position - sVertices[index].position), S::Math::Normalize(sVertices[index - columns].position - sVertices[index].position));
				sVertices[index].normal += S::Math::Cross(S::Math::Normalize(sVertices[index - columns].position - sVertices[index].position), S::Math::Normalize(sVertices[index - 1].position - sVertices[index].position));
			}
			else if (j == 0 && i == 0)
			{
				sVertices[index].normal += S::Math::Cross(S::Math::Normalize(sVertices[index + columns].position - sVertices[index].position), S::Math::Normalize(sVertices[index + 1].position - sVertices[index].position));
			}
			else if (j == 0 && i == rows - 1)
			{
				sVertices[index].normal += S::Math::Cross(S::Math::Normalize(sVertices[index + 1].position - sVertices[index].position), S::Math::Normalize(sVertices[index - columns].position - sVertices[index].position));
			}
			else if (j == columns - 1 && i == 0)
			{
				sVertices[index].normal += S::Math::Cross(S::Math::Normalize(sVertices[index - 1].position - sVertices[index].position), S::Math::Normalize(sVertices[index + columns].position - sVertices[index].position));
			}
			else if (j == columns - 1 && i == rows - 1)
			{
				sVertices[index].normal += S::Math::Cross(S::Math::Normalize(sVertices[index - columns].position - sVertices[index].position), S::Math::Normalize(sVertices[index - 1].position - sVertices[index].position));
			}

			sVertices[index].normal = S::Math::Normalize(sVertices[index].normal);
			++index;
		}
	}

	index = 0;
	for (uint32_t i = 0; i < rows - 1; ++i)
	{
		for (uint32_t j = 0; j < columns - 1; ++j)
		{
			sIndices[index++] = j + (i*columns);				// lb
			sIndices[index++] = j + ((i + 1)*columns);			// lt
			sIndices[index++] = (j + 1) + (i *columns);		// rb
			sIndices[index++] = j + ((i + 1)*columns);			// lt
			sIndices[index++] = (j + 1) + ((i + 1) *columns);	// rt
			sIndices[index++] = (j + 1) + (i *columns);		// rb
		}
	}

	mesh.Allocate(numVertices, numIndices);

	for (uint32_t i = 0; i < numVertices; ++i)
	{
		mesh.GetVertex(i) = sVertices[i];
	}
	for (uint32_t i = 0; i < numIndices; ++i)
	{
		mesh.GetIndex(i) = sIndices[i];
	}
	SafeDeleteArray(sVertices);
	SafeDeleteArray(sIndices);
}

void MeshBuilder::ComputeNormals(Mesh& mesh)
{
	std::vector<Math::Vector3> normals;
	normals.resize(mesh.GetVertexCount());

	for (uint32_t i = 0; i < mesh.GetIndexCount(); i += 3)
	{
		uint32_t i0 = mesh.GetIndex(i + 0);
		uint32_t i1 = mesh.GetIndex(i + 1);
		uint32_t i2 = mesh.GetIndex(i + 2);
		const auto& a = mesh.GetVertex(i0);
		const auto& b = mesh.GetVertex(i1);
		const auto& c = mesh.GetVertex(i2);
		Math::Vector3 ab = b.position - a.position;
		Math::Vector3 ac = c.position - a.position;
		Math::Vector3 normal = Math::Normalize(Math::Cross(ab, ac));
		normals[i0] += normal;
		normals[i1] += normal;
		normals[i2] += normal;
	}

	for (uint32_t i = 0; i < mesh.GetVertexCount(); ++i)
	{
		mesh.GetVertex(i).normal = Math::Normalize(normals[i]);
	}
}

Geometry::AABB MeshBuilder::ComputeBound(const Mesh& mesh)
{
	if (mesh.GetVertexCount() == 0)
		return Geometry::AABB();

	Math::Vector3 min = mesh.GetVertex(0).position;
	Math::Vector3 max = mesh.GetVertex(0).position;

	for (uint32_t v = 1; v < mesh.GetVertexCount(); ++v)
	{
		const auto& vertex = mesh.GetVertex(v);

		min.x = Math::Min(min.x, vertex.position.x);
		min.y = Math::Min(min.y, vertex.position.y);
		min.z = Math::Min(min.z, vertex.position.z);

		max.x = Math::Max(max.x, vertex.position.x);
		max.y = Math::Max(max.y, vertex.position.y);
		max.z = Math::Max(max.z, vertex.position.z);
	}

	return Geometry::AABB::FromMinMax(min, max);
}