#include <Graphics/Inc/Graphics.h>
#include <assimp/Importer.hpp>	// C++ importer interface
#include <assimp/scene.h>		// Output data structure
#include <assimp/postprocess.h>	// Post processing flags
#include <cstdio>

using namespace S::Graphics;
using namespace S::Math;

using BoneVector = std::vector<Bone*>;
using BoneIndexMap = std::map<std::string, int>;

struct Options
{
	const char* inputFileName{ nullptr };	// Mandatory - no std::optional
	const char* outputFileName{ nullptr };	// Mandatory
};

bool ParseArg(int argc, char* argv[], Options& options)
{
	if (argc < 3)
		return false;

	options.inputFileName = argv[argc - 2];
	options.outputFileName = argv[argc - 1];
	return true;
}

void PrintUsage()
{
	printf(
		"== ModelImporter Help ==\n"
		"\n"
		"Usage:\n"
		"\n"
		"    ModelImporter.exe [Options] <InputFile> <OutputFile>\n"
		"\n"
		"Options:\n"
		"\n"
		"    <none>"
		"\n"
	);
}

Matrix4 Convert(const aiMatrix4x4& aiMatrix)
{
	Matrix4 mat = *(reinterpret_cast<const Matrix4*>(&aiMatrix));
	mat = Transpose(mat);
	return mat;
}

Vector2 Convert3to2(const aiVector3D& aiVector) { return { aiVector.x,aiVector.y }; }
Vector3 Convert(const aiVector3D& aiVector) { return { aiVector.x,aiVector.y,aiVector.z }; }
Quaternion Convert(const aiQuaternion& aiQuat) { return { aiQuat.x,aiQuat.y,aiQuat.z,aiQuat.w }; }

const char* StripPath(const char* filePath)
{
	// Copy from the last '\\' if there is one
	const char* fileName = strrchr(filePath, '\\');
	if (fileName != nullptr)
	{
		return fileName + 1; // Found '\\', +1 to skip
	}

	// If no '\\' found, copy from the last '/' if there is one
	fileName = strrchr(filePath, '/');
	if (fileName != nullptr)
	{
		return fileName + 1; // Found '\', +1 to skip
	}
	
	// If neither is found, the file path is just the name
	return filePath;
}

void WriteMatrix(FILE* file, Matrix4 m)
{
	fprintf(file, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n", 
		m._1[0], m._1[1], m._1[2], m._1[3],
		m._2[0], m._2[1], m._2[2], m._2[3],
		m._3[0], m._3[1], m._3[2], m._3[3],
		m._4[0], m._4[1], m._4[2], m._4[3]);
}

int GetBoneIndex(aiBone* inputBone, BoneVector& bones, BoneIndexMap& boneIndexMap)
{
	auto iter = boneIndexMap.find(inputBone->mName.C_Str());
	if (iter != boneIndexMap.end())
		return iter->second;

	Bone* newBone = new Bone();
	newBone->name = inputBone->mName.C_Str();
	newBone->index = static_cast<int>(bones.size());
	newBone->offsetTransform = Convert(inputBone->mOffsetMatrix);

	bones.push_back(newBone);
	boneIndexMap.emplace(newBone->name, newBone->index);
	return newBone->index;
}

Bone* BuildSkeleton(aiNode& sceneNode, Bone* parent, BoneVector& bones, BoneIndexMap& boneIndexMap)
{
	Bone* bone = nullptr;

	auto iter = boneIndexMap.find(sceneNode.mName.C_Str());
	if (iter == boneIndexMap.end())
	{
		bone = new Bone();
		bone->index = static_cast<int>(bones.size());
		bone->offsetTransform = Matrix4::Identity();

		if (sceneNode.mName.length != 0)
			bone->name = sceneNode.mName.C_Str();
		else
			bone->name = "NoName" + std::to_string(bone->index);

		bones.push_back(bone);
		boneIndexMap.emplace(bone->name, bone->index);
	}
	else
	{
		bone = bones[iter->second];
	}

	bone->parent = parent;
	bone->toParentTransform = Convert(sceneNode.mTransformation);

	for (uint32_t i = 0; i < sceneNode.mNumChildren; ++i)
	{
		Bone* child = BuildSkeleton(*sceneNode.mChildren[i], bone, bones, boneIndexMap);
		bone->children.push_back(child);
	}

	return bone;
}

int main(int argc, char* argv[])
{
	Options options;
	if (!ParseArg(argc, argv, options))
	{
		PrintUsage();
		return 0;
	}

	Assimp::Importer importer;

	const uint32_t flags =
		aiProcessPreset_TargetRealtime_Quality |
		aiProcess_ConvertToLeftHanded;
	const aiScene* scene = importer.ReadFile(options.inputFileName, flags);

	if (scene == nullptr)
	{
		printf("Error: %s\n", importer.GetErrorString());
		return -1;
	}

	FILE* file = nullptr;
	errno_t error = fopen_s(&file, options.outputFileName, "w");
	if (error != 0)
	{
		printf("Error: Failed to write to output file %s.\n", options.outputFileName);
		return -1;
	}

	BoneVector bones;
	BoneIndexMap boneIndexMap;

	printf("Check Animation...\n");
	if(scene->HasAnimations())
		fprintf(file, "HasAnimation: %d\n", 1);
	else
		fprintf(file, "HasAnimation: %d\n", 0);

	// Read mesh data
	if (scene->HasMeshes())
	{
		printf("Reading mesh data...\n");
		fprintf(file, "MeshCount: %d\n", scene->mNumMeshes);
		fprintf(file, "MaterialCount: %d\n", scene->mNumMaterials);
		fprintf(file, "TotalTextureCount: %d\n", scene->mNumTextures);

		for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
		{
			const aiMesh* inputMesh = scene->mMeshes[meshIndex];

			fprintf(file, "VertexCount: %d\n", inputMesh->mNumVertices);
			fprintf(file, "IndexCount: %d\n", inputMesh->mNumFaces * 3);
			fprintf(file, "MaterialIndex: %d\n", inputMesh->mMaterialIndex);
			fprintf(file, "TextureCount: %u\n", scene->mMaterials[inputMesh->mMaterialIndex]->GetTextureCount(aiTextureType_DIFFUSE));
			aiString texturePath;
			if (scene->mMaterials[inputMesh->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS)
			{
				// Only one digit index.
				// TODO: Make it to be dynamic
				if (texturePath.C_Str()[0] == '*')
					fprintf(file, "%d\n", texturePath.C_Str()[1] - '0');
				else
					fprintf(file, "0\n");
			}

			printf("Reading vertices..\n");

			const aiVector3D* positions = inputMesh->mVertices;
			const aiVector3D* normals = inputMesh->mNormals;
			const aiVector3D* tangents = inputMesh->mTangents;
			const aiVector3D* texcoords = inputMesh->HasTextureCoords(0) ? inputMesh->mTextureCoords[0] : nullptr;

			std::vector<BoneVertex> vertices(inputMesh->mNumVertices);
			std::vector<int> indices(inputMesh->mNumFaces * 3);

			for (uint32_t i = 0; i < inputMesh->mNumVertices; ++i)
			{
				auto& vertex = vertices[i];
				vertex.position = Convert(positions[i]);
				vertex.normal = Convert(normals[i]);
				vertex.tangent = Convert(tangents[i]);
				vertex.texcoord = Convert3to2(texcoords[i]);
			}

			printf("Reading indices...\n");

			for (uint32_t i = 0; i < inputMesh->mNumFaces; ++i)
			{
				indices[i * 3] = inputMesh->mFaces[i].mIndices[0];
				indices[i * 3 + 1] = inputMesh->mFaces[i].mIndices[1];
				indices[i * 3 + 2] = inputMesh->mFaces[i].mIndices[2];
			}

			if (inputMesh->HasBones())
			{
				printf("Reading bone weights...\n");
				// Track how many weights have we add to each vertex so far
				std::vector<int> numWeights(vertices.size(), 0);

				for (uint32_t b = 0; b < inputMesh->mNumBones; ++b)
				{
					aiBone* bone = inputMesh->mBones[b];
					int boneIndex = GetBoneIndex(bone, bones, boneIndexMap);

					for (uint32_t w = 0; w < bone->mNumWeights; ++w)
					{
						aiVertexWeight& weight = bone->mWeights[w];
						auto& vertex = vertices[weight.mVertexId];
						auto& weightIndex = numWeights[weight.mVertexId];
						vertex.boneIndices[weightIndex] = boneIndex;
						vertex.boneWeight[weightIndex] = weight.mWeight;
						++weightIndex;
					}
				}
			}

			printf("Saving vertices...\n");

			for (auto& vertex : vertices)
			{
				fprintf(file, "%f %f %f %f %f %f %f %f %f %f %f %i %i %i %i %f %f %f %f\n",
					vertex.position.x, vertex.position.y, vertex.position.z,
					vertex.normal.x, vertex.normal.y, vertex.normal.z,
					vertex.tangent.x, vertex.tangent.y, vertex.tangent.z,
					vertex.texcoord.x ? vertex.texcoord.x : 0.0f,
					vertex.texcoord.y ? vertex.texcoord.y : 0.0f,
					vertex.boneIndices[0], vertex.boneIndices[1], vertex.boneIndices[2], vertex.boneIndices[3],
					vertex.boneWeight[0], vertex.boneWeight[1], vertex.boneWeight[2], vertex.boneWeight[3]);
			}

			printf("Saving indices...\n");

			for (uint32_t i = 0; i < indices.size(); i += 3)
			{
				fprintf(file, "%d %d %d\n",
					indices[i],
					indices[i + 1],
					indices[i + 2]);
			}
		}
	}

	if (scene->HasMaterials())
	{
		printf("Reading materials...\n");

		std::vector<std::string> textureNames;

		for (uint32_t i = 0; i < scene->mNumMaterials; ++i)
		{
			const aiMaterial* material = scene->mMaterials[i];

			const uint32_t textureCount = material->GetTextureCount(aiTextureType_DIFFUSE);
			if (textureCount > 0)
			{
				aiString texturePath;
				if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS)
				{
					// if texture path starts with *, then the texture is embedded.
					if (texturePath.C_Str()[0] == '*')
					{
						std::string fileName = StripPath(options.inputFileName);
						fileName.erase(fileName.length() - 4);//remove ".fbx" or ".obj"
						fileName += "_diffuse_";
						fileName += texturePath.C_Str()[1];

						ASSERT(scene->HasTextures(), "Error: No embedded texture found!");

						int textureIndex = atoi(texturePath.C_Str() + 1);
						ASSERT(textureIndex < (int)scene->mNumTextures, "Error: Invalid texture index!");

						const aiTexture* texture = scene->mTextures[textureIndex];
						ASSERT(texture->mHeight == 0, "Error: Uncompressed texture found!");

						if (texture->CheckFormat("jpg"))
						{
							fileName += ".jpg";
						}
						else if (texture->CheckFormat("png"))
						{
							fileName += ".png";
						}
						else
						{
							printf("Info: unrecognized texture format\n");
							continue;
						}

						printf("Info: extracting embedded texture %s\n", fileName.c_str());

						std::string fullFileName = options.outputFileName;
						fullFileName = fullFileName.substr(0, fullFileName.rfind('\\') + 1);
						fullFileName += fileName;

						FILE* file = nullptr;
						fopen_s(&file, fullFileName.c_str(), "wb");
						size_t written = fwrite(texture->pcData, 1, texture->mWidth, file);
						ASSERT(written == texture->mWidth, "Error: Failed to extract embedded texture!");
						fclose(file);
						textureNames.push_back(fileName);
					}
					else
					{
						const char* fileName = StripPath(texturePath.C_Str());
						printf("Info: adding texture %s\n", fileName);
						textureNames.push_back(fileName);
					}
				}
			}
		}

		printf("Saving texture names...\n");

		fprintf(file, "TexturePathes: %d\n", static_cast<int>(textureNames.size()));
		for (auto& name : textureNames)
		{
			fprintf(file, "%s\n", name.c_str());
		}
	}

	if (scene->HasAnimations())
	{
		printf("Building skeleton...\n");

		BuildSkeleton(*scene->mRootNode, nullptr, bones, boneIndexMap);

		printf("Saving bones...\n");

		fprintf(file, "BoneCount: %d\n", static_cast<int>(bones.size()));
		for (auto& bone : bones)
		{
			fprintf(file, "Name: %s\n", bone->name.c_str());
			fprintf(file, "Index: %d\n", bone->index);
			fprintf(file, "ParentIndex: %d\n", bone->parent ? bone->parent->index : -1);
			fprintf(file, "ChildCount: %d\n", static_cast<int>(bone->children.size()));
			if (!bone->children.empty())
			{
				for (auto children : bone->children)
					fprintf(file, "%d ", children->index);
				fprintf(file, "\n");
			}
			WriteMatrix(file, bone->toParentTransform);
			WriteMatrix(file, bone->offsetTransform);
		}
		printf("Reading animation data....\n");

		std::vector<std::unique_ptr<AnimationClip>> animations;

		// Reading animation data
		for (uint32_t animIndex = 0; animIndex < scene->mNumAnimations; ++animIndex)
		{
			aiAnimation* aiAnim = scene->mAnimations[animIndex];
			std::unique_ptr<AnimationClip> animClip = std::make_unique<AnimationClip>();

			// Check if the animation has a name
			if (aiAnim->mName.length > 0)
			{
				animClip->name = aiAnim->mName.C_Str();
			}
			else
			{
				std::string generatedName = "Anim" + std::to_string(animIndex);
				printf("Warning: Animation %u has no name, renamed to %s\n", animations.size(), generatedName.c_str());
				animClip->name = std::move(generatedName);
			}

			animClip->duration = static_cast<float>(aiAnim->mDuration);
			animClip->ticksPerSecond = static_cast<float>(aiAnim->mTicksPerSecond);

			if (animClip->ticksPerSecond == 0.0f)
			{
				printf("Warning: Animation %s has ticks per second 0.0. Assuming 1.0.\n", animClip->name.c_str());
				animClip->ticksPerSecond = 1.0f;
			}

			printf("Reading bone animations for %s...\n", animClip->name.c_str());

			for (uint32_t boneAnimIndex = 0; boneAnimIndex < aiAnim->mNumChannels; ++boneAnimIndex)
			{
				aiNodeAnim* aiNodeAnim = aiAnim->mChannels[boneAnimIndex];
				std::unique_ptr<BoneAnimation> boneAnim = std::make_unique<BoneAnimation>();

				boneAnim->boneName = aiNodeAnim->mNodeName.C_Str();

				AnimationBuilder builder;
				for (uint32_t keyIndex = 0; keyIndex < aiNodeAnim->mNumPositionKeys; ++keyIndex)
				{
					const auto& key = aiNodeAnim->mPositionKeys[keyIndex];
					builder.AddPositionKey(Convert(key.mValue), static_cast<float>(key.mTime));
				}
				for (uint32_t keyIndex = 0; keyIndex < aiNodeAnim->mNumRotationKeys; ++keyIndex)
				{
					const auto& key = aiNodeAnim->mRotationKeys[keyIndex];
					builder.AddRotationkey(Convert(key.mValue), static_cast<float>(key.mTime));
				}
				for (uint32_t keyIndex = 0; keyIndex < aiNodeAnim->mNumScalingKeys; ++keyIndex)
				{
					const auto& key = aiNodeAnim->mScalingKeys[keyIndex];
					builder.AddScaleKey(Convert(key.mValue), static_cast<float>(key.mTime));
				}

				boneAnim->animation = builder.Finalize();

				animClip->boneAnimations.push_back(std::move(boneAnim));
			}

			animations.push_back(std::move(animClip));
		}

		printf("Saving animations...\n");

		fprintf(file, "Animation:\nAnimationCount: %d\n", static_cast<int>(animations.size()));

		for (auto& animClip : animations)
		{
			AnimationIO::SaveAnimationClip(file, *animClip);
		}
	}

	for (auto& bone : bones)
		SafeDelete(bone);
	bones.clear();
	boneIndexMap.clear();
	printf("All Done!\n");
	fclose(file);
	return 0;
}

/*
Microsoft Windows [Version 10.0.17763.437]
(c) 2018 Microsoft Corporation. All rights reserved.

D:\Sherlock\SEngine\Debug>assimp-vc140-mt.dll
The system cannot execute the specified program.

D:\Sherlock\SEngine\Debug>ModelImporter.exe ../Assets/Models/Duck/duck.fbx ../Assets/Models/Duck/duck.txt
Reading mesh data...
Reading vertices..
Reading indices...
All Done!

D:\Sherlock\SEngine\Debug>*/