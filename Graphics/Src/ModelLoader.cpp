#include "Precompiled.h"
#include "ModelLoader.h"

#include "AnimationIO.h"
#include "Mesh.h"

using namespace S;
using namespace S::Graphics;

void ModelLoader::LoadOBJ(const char* fileName, float scale, Mesh& mesh)
{
	std::vector<uint32_t> positionIndices, normalIndices, uvIndices;
	std::vector<Math::Vector3> positions;
	std::vector<Math::Vector3> normals;
	std::vector<Math::Vector2> uvs;

	FILE* file = nullptr;
	fopen_s(&file, fileName, "r");
	ASSERT(file, "[ModelLoader] Failed to open file %s.", fileName);

	while (true)
	{
		char buffer[128];
		int res = fscanf_s(file, "%s", buffer, (int)std::size(buffer));
		if (res == EOF)
			break;

		if (strcmp(buffer, "v") == 0)
		{
			Math::Vector3 position;
			fscanf_s(file, "%f %f %f\n", &position.x, &position.y, &position.z);
			positions.push_back(position * scale);
		}
		else if (strcmp(buffer, "vt") == 0)
		{
			Math::Vector3 uv;
			fscanf_s(file, "%f %f %f\n", &uv.x, &uv.y, &uv.z);
			uvs.push_back({ uv.x, uv.y });
		}
		else if (strcmp(buffer, "vn") == 0)
		{
			Math::Vector3 normal;
			fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			normals.push_back(normal);
		}
		else if (strcmp(buffer, "f") == 0)
		{
			std::string vertex1, vertex2, vertex3, vertex4;
			uint32_t v[4], t[4], n[4];
			fgets(buffer, (int)std::size(buffer), file);
			if (sscanf_s(buffer, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n",
				&v[0], &t[0], &n[0],
				&v[1], &t[1], &n[1],
				&v[2], &t[2], &n[2],
				&v[3], &t[3], &n[3]) == 12)
			{
				positionIndices.push_back(v[0]);
				positionIndices.push_back(v[1]);
				positionIndices.push_back(v[2]);
				uvIndices.push_back(t[0]);
				uvIndices.push_back(t[1]);
				uvIndices.push_back(t[2]);
				normalIndices.push_back(n[0]);
				normalIndices.push_back(n[1]);
				normalIndices.push_back(n[2]);

				positionIndices.push_back(v[0]);
				positionIndices.push_back(v[2]);
				positionIndices.push_back(v[3]);
				uvIndices.push_back(t[0]);
				uvIndices.push_back(t[2]);
				uvIndices.push_back(t[3]);
				normalIndices.push_back(n[0]);
				normalIndices.push_back(n[2]);
				normalIndices.push_back(n[3]);
			}
			else if (sscanf_s(buffer, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
				&v[0], &t[0], &n[0],
				&v[1], &t[1], &n[1],
				&v[2], &t[2], &n[2]) == 9)
			{
				positionIndices.push_back(v[0]);
				positionIndices.push_back(v[1]);
				positionIndices.push_back(v[2]);
				uvIndices.push_back(t[0]);
				uvIndices.push_back(t[1]);
				uvIndices.push_back(t[2]);
				normalIndices.push_back(n[0]);
				normalIndices.push_back(n[1]);
				normalIndices.push_back(n[2]);
			}
			else
			{
				ASSERT(false, "[ModelLoader] Failed to load file %s.", fileName);
			}
		}
	}
	fclose(file);

	// Reconstruct vertices
	std::vector<Graphics::Vertex> vertices;
	for (size_t i = 0; i < positionIndices.size(); ++i)
	{
		Graphics::Vertex vertex;
		vertex.position = positions[positionIndices[i] - 1];
		vertex.normal = normals[normalIndices[i] - 1];
		vertex.texcoord.x = uvs[uvIndices[i] - 1].x;
		vertex.texcoord.y = 1.0f - uvs[uvIndices[i] - 1].y;
		vertices.emplace_back(vertex);
	}
	std::vector<uint32_t> indices;
	indices.resize(vertices.size());
	std::iota(indices.begin(), indices.end(), 0);

	// Initialize mesh
	mesh.Allocate((uint32_t)vertices.size(), (uint32_t)indices.size());
	for (size_t i = 0; i < vertices.size(); ++i)
		mesh.GetVertex((uint32_t)i) = vertices[i];
	for (size_t i = 0; i < indices.size(); ++i)
		mesh.GetIndex((uint32_t)i) = indices[i];
}

void ModelLoader::LoadOBJVertex(const char * fileName, float scale, Mesh & mesh)
{
	Vertex* sVertices = nullptr;
	uint32_t* sIndices = nullptr;
	uint32_t numVertices{ 0u };
	uint32_t numIndices{ 0u };

	FILE* file = nullptr;
	fopen_s(&file, fileName, "r");
	ASSERT(file, "[ModelLoader] Failed to open file %s.", fileName);

	while (true)
	{
		char buffer[128] = { 0 };
		int res = fscanf_s(file, "%s", buffer, (int)std::size(buffer));
		if (res == EOF)
			break;

		if (strcmp(buffer, "VertexCount:") == 0)
		{
			fscanf_s(file, "%d\n", &numVertices);
			sVertices = new Vertex[numVertices];
		}
		else if (strcmp(buffer, "IndexCount:") == 0)
		{
			fscanf_s(file, "%d\n", &numIndices);
			sIndices = new uint32_t[numIndices];
		}
		else if (strcmp(buffer, "MaterialIndex:") == 0)
		{
			fscanf_s(file, "%s\n", &buffer, (int)std::size(buffer));

			for (uint32_t i = 0; i < numVertices; ++i)
			{
				fgets(buffer, (int)std::size(buffer), file);
				if (sscanf_s(buffer, "%f %f %f %f %f %f %f %f %f %f %f",
					&sVertices[i].position.x, &sVertices[i].position.y, &sVertices[i].position.z,
					&sVertices[i].normal.x, &sVertices[i].normal.y, &sVertices[i].normal.z,
					&sVertices[i].tangent.x, &sVertices[i].tangent.y, &sVertices[i].tangent.z,
					&sVertices[i].texcoord.x, &sVertices[i].texcoord.y) != 11)
				{
					ASSERT(false, "[ModelLoader] Failed to load file %s.", fileName);
				}
				else
				{
					sVertices[i].position.x *= scale;
					sVertices[i].position.y *= scale;
					sVertices[i].position.z *= scale;
				}
			}

			for (uint32_t i = 0; i < numIndices; i += 3)
			{
				fgets(buffer, (int)std::size(buffer), file);
				if (sscanf_s(buffer, "%d %d %d", &sIndices[i], &sIndices[i + 1], &sIndices[i + 2]) != 3)
				{
					ASSERT(false, "[ModelLoader] Failed to load file %s.", fileName);
				}
			}
		}
	}

	fclose(file);

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

void ModelLoader::LoadSkeleton(const char* fileName, AnimatedModel& model)
{
	BoneMesh tempMesh;
	BoneVertex* sVertices = nullptr;
	uint32_t* sIndices = nullptr;
	uint32_t numMeshes{ 0 };
	uint32_t numVertices{ 0 };
	uint32_t numIndices{ 0 };
	uint32_t numBone{ 0 };
	uint32_t boneIndex{ 0 };

	FILE* file = nullptr;
	fopen_s(&file, fileName, "r");
	ASSERT(file, "[ModelLoader] Failed to open file %s.", fileName);

	while (true)
	{
		char buffer[256];
		int res = fscanf_s(file, "%s", buffer, (int)std::size(buffer));
		if (res == EOF)
			break;

		if (strcmp(buffer, "MeshCount:") == 0)
		{
			fscanf_s(file, "%d\n", &numMeshes);
			model.meshes.resize(numMeshes);
		}
		else if (strcmp(buffer, "VertexCount:") == 0)
		{
			fscanf_s(file, "%d\n", &numVertices);
			sVertices = new BoneVertex[numVertices];
		}
		else if (strcmp(buffer, "IndexCount:") == 0)
		{
			fscanf_s(file, "%d\n", &numIndices);
			sIndices = new uint32_t[numIndices];
		}
		else if (strcmp(buffer, "TotalTextureCount:") == 0)
		{
			uint32_t num;
			fscanf_s(file, "%d\n", &num);
			model.textures.resize(num);
			model.textureIndices.resize(num);
		}
		else if (strcmp(buffer, "MaterialIndex:") == 0)
		{
			int meshIndex;
			fscanf_s(file, "%d\n", &meshIndex);

			fscanf_s(file, "%s", buffer, (int)std::size(buffer));
			ASSERT(strcmp(buffer, "TextureCount:") == 0, "[ModelLoader] No TextureCount.");

			uint32_t num;
			fscanf_s(file, "%d\n", &num);
			if (num != 0)
			{
				model.textureIndices[meshIndex].resize(num);
			}
			else
			{
				model.textureIndices[meshIndex].resize(1);
			}
			for (uint32_t i = 0; i < num; ++i)
			{
				fscanf_s(file, "%d", &model.textureIndices[meshIndex][i]);
			}
			if (num)
			{
				fgets(buffer, (int)std::size(buffer), file);
			}

			for (uint32_t i = 0; i < numVertices; ++i)
			{
				fgets(buffer, (int)std::size(buffer), file);
				if (sscanf_s(buffer, "%f %f %f %f %f %f %f %f %f %f %f %d %d %d %d %f %f %f %f",
					&sVertices[i].position.x, &sVertices[i].position.y, &sVertices[i].position.z,
					&sVertices[i].normal.x, &sVertices[i].normal.y, &sVertices[i].normal.z,
					&sVertices[i].tangent.x, &sVertices[i].tangent.y, &sVertices[i].tangent.z,
					&sVertices[i].texcoord.x, &sVertices[i].texcoord.y,
					&sVertices[i].boneIndices[0], &sVertices[i].boneIndices[1], &sVertices[i].boneIndices[2], &sVertices[i].boneIndices[3],
					&sVertices[i].boneWeight[0], &sVertices[i].boneWeight[1], &sVertices[i].boneWeight[2], &sVertices[i].boneWeight[3]
					) != 19)
				{
					ASSERT(false, "[ModelLoader] Failed to load file %s.", fileName);
				}
			}

			for (uint32_t i = 0; i < numIndices; i += 3)
			{
				fgets(buffer, (int)std::size(buffer), file);
				if (sscanf_s(buffer, "%d %d %d", &sIndices[i], &sIndices[i + 1], &sIndices[i + 2]) != 3)
				{
					ASSERT(false, "[ModelLoader] Failed to load file %s.", fileName);
				}
			}

			model.meshes[meshIndex].Allocate(numVertices, numIndices);

			for (uint32_t i = 0; i < numVertices; ++i)
			{
				model.meshes[meshIndex].GetVertex(i) = sVertices[i];
			}
			for (uint32_t i = 0; i < numIndices; ++i)
			{
				model.meshes[meshIndex].GetIndex(i) = sIndices[i];
			}
			SafeDeleteArray(sVertices);
			SafeDeleteArray(sIndices);
		}
		else if (strcmp(buffer, "TexturePathes:") == 0)
		{
			int num;
			fscanf_s(file, "%d\n", &num);

			for (int i = 0; i < num; ++i)
			{
				char name[128];
				fscanf_s(file, "%s\n", name, (int)std::size(name));
				std::string temp = fileName;
				std::string tempName = temp.substr(0, temp.rfind('/') + 1).c_str();
				tempName.append(name);
				size_t index = tempName.substr(tempName.rfind('.') - (size_t)1, 1)[0] - '0';
				if (index <= 9 && index >= 0)
				{
					model.textures[index].Initialize(tempName.c_str());
				}
			}
		}
		else if (strcmp(buffer, "BoneCount:") == 0)
		{
			fscanf_s(file, "%d\n", &numBone);
			model.bones.resize(numBone);
			for (uint32_t i = 0; i < numBone; ++i)
			{
				model.bones[i] = new Bone;
			}
		}
		else if (strcmp(buffer, "Name:") == 0)
		{
			fscanf_s(file, "%s\n", buffer, (int)std::size(buffer));
			model.bones[boneIndex]->name = buffer;
		}
		else if (strcmp(buffer, "Index:") == 0)
		{
			fscanf_s(file, "%d\n", &model.bones[boneIndex]->index);
		}
		else if (strcmp(buffer, "ParentIndex:") == 0)
		{
			fscanf_s(file, "%d\n", &model.bones[boneIndex]->parentIndex);
		}
		else if (strcmp(buffer, "ChildCount:") == 0)
		{
			uint32_t temp;
			fscanf_s(file, "%d\n", &temp);
			model.bones[boneIndex]->childIndices.resize(temp);
			model.bones[boneIndex]->children.resize(temp);
			for (uint32_t i = 0; i < temp; ++i)
			{
				fscanf_s(file, "%d", &model.bones[boneIndex]->childIndices[i]);
			}
			if (temp)
			{
				fgets(buffer, (int)std::size(buffer), file);
			}
			fgets(buffer, (int)std::size(buffer), file);
			if (sscanf_s(buffer, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f",
				&model.bones[boneIndex]->toParentTransform._1[0],
				&model.bones[boneIndex]->toParentTransform._1[1],
				&model.bones[boneIndex]->toParentTransform._1[2],
				&model.bones[boneIndex]->toParentTransform._1[3],
				&model.bones[boneIndex]->toParentTransform._2[0],
				&model.bones[boneIndex]->toParentTransform._2[1],
				&model.bones[boneIndex]->toParentTransform._2[2],
				&model.bones[boneIndex]->toParentTransform._2[3],
				&model.bones[boneIndex]->toParentTransform._3[0],
				&model.bones[boneIndex]->toParentTransform._3[1],
				&model.bones[boneIndex]->toParentTransform._3[2],
				&model.bones[boneIndex]->toParentTransform._3[3],
				&model.bones[boneIndex]->toParentTransform._4[0],
				&model.bones[boneIndex]->toParentTransform._4[1],
				&model.bones[boneIndex]->toParentTransform._4[2],
				&model.bones[boneIndex]->toParentTransform._4[3]
			) != 16)
			{
				ASSERT(false, "[ModelLoader] Failed to load file %s.", fileName);
			}
			else
			{
				// No Scaling for Bones yet
				//model.bones[boneIndex]->toParentTransform = model.bones[boneIndex]->toParentTransform*Math::Scaling({ scale,scale ,scale });
			}

			fgets(buffer, (int)std::size(buffer), file);
			if (sscanf_s(buffer, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f",
				&model.bones[boneIndex]->offsetTransform._1[0],
				&model.bones[boneIndex]->offsetTransform._1[1],
				&model.bones[boneIndex]->offsetTransform._1[2],
				&model.bones[boneIndex]->offsetTransform._1[3],
				&model.bones[boneIndex]->offsetTransform._2[0],
				&model.bones[boneIndex]->offsetTransform._2[1],
				&model.bones[boneIndex]->offsetTransform._2[2],
				&model.bones[boneIndex]->offsetTransform._2[3],
				&model.bones[boneIndex]->offsetTransform._3[0],
				&model.bones[boneIndex]->offsetTransform._3[1],
				&model.bones[boneIndex]->offsetTransform._3[2],
				&model.bones[boneIndex]->offsetTransform._3[3],
				&model.bones[boneIndex]->offsetTransform._4[0],
				&model.bones[boneIndex]->offsetTransform._4[1],
				&model.bones[boneIndex]->offsetTransform._4[2],
				&model.bones[boneIndex]->offsetTransform._4[3]
			) != 16)
			{
				ASSERT(false, "[ModelLoader] Failed to load file %s.", fileName);
			}
			else
			{
				// No Scaling for Bones yet
				//model.bones[boneIndex]->toParentTransform = model.bones[boneIndex]->toParentTransform*Math::Scaling({ scale,scale ,scale });
			}
			++boneIndex;
		}
		else if (strcmp(buffer, "Animation:") == 0)
		{
			AnimationIO::LoadAnimationClip(file, model.animations, (int)model.bones.size()); // Why is this ; remove the tab?
		}
	}
	model.LinkBones();
	model.PairBoneAnimation();
	fclose(file);
}
