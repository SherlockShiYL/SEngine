#include "DataProcessor.h"

void DataProcessor::LoadWeapon(const char * fileName, Weapon* weapon)
{
}
//void DataProcessor::LoadEnemy(const char * fileName, Enemy*** enemy)
//{
//	FILE* file = nullptr;
//	fopen_s(&file, fileName, "r");
//	XASSERT(file, "[DataLoader] Failed to open file %s.", fileName);
//
//	int amount{ 0 }; // How many enemies
//	while (true)
//	{
//		char buffer[128];
//		int res = fscanf_s(file, "%s", buffer, static_cast<unsigned int>(std::size(buffer)));
//		if (res == EOF)
//			break;
//
//		if (strcmp(buffer, "Amount:") == 0)
//		{
//			fscanf_s(file, "%d\n", &amount);
//		}
//		if (strcmp(buffer, "Name:") == 0)
//		{
//			char name[128];
//			fscanf_s(file, "%s\n", name, static_cast<unsigned int>(std::size(name)));
//			enemy->SetName(name);
//		}
//		else if (strcmp(buffer, "HP:") == 0)
//		{
//			int hp;
//			fscanf_s(file, "%d\n", &hp);
//			enemy->SetBasicHP(hp);
//		}
//		else if (strcmp(buffer, "Speed:") == 0)
//		{
//			float speed;
//			fscanf_s(file, "%f\n", &speed);
//			enemy->SetSpeed(speed);
//		}
//		else if (strcmp(buffer, "Damage:") == 0)
//		{
//			int damage;
//			fscanf_s(file, "%d\n", &damage);
//			enemy->SetBasicDamage(damage);
//		}
//		else if (strcmp(buffer, "Position:") == 0)
//		{
//			X::Math::Vector2 position;
//			fscanf_s(file, "%f %f\n", &position.x, &position.y);
//			enemy->SetPosition(position);
//		}
//	}
//	fclose(file);
//	//while (true)
//	//{
//	//	char buffer[128];
//	//	int res = fscanf_s(file, "%s", buffer, std::size(buffer));
//	//	if (res == EOF)
//	//		break;
//
//	//	if (strcmp(buffer, "v") == 0)
//	//	{
//	//		Math::Vector3 position;
//	//		fscanf_s(file, "%f %f %f\n", &position.x, &position.y, &position.z);
//	//		positions.push_back(position * scale);
//	//	}
//	//	else if (strcmp(buffer, "vt") == 0)
//	//	{
//	//		Math::Vector3 uv;
//	//		fscanf_s(file, "%f %f %f\n", &uv.x, &uv.y, &uv.z);
//	//		uvs.push_back({ uv.x, uv.y });
//	//	}
//	//	else if (strcmp(buffer, "vn") == 0)
//	//	{
//	//		Math::Vector3 normal;
//	//		fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
//	//		normals.push_back(normal);
//	//	}
//	//	else if (strcmp(buffer, "f") == 0)
//	//	{
//	//		std::string vertex1, vertex2, vertex3, vertex4;
//	//		uint32_t v[4], t[4], n[4];
//	//		fgets(buffer, std::size(buffer), file);
//	//		if (sscanf_s(buffer, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n",
//	//			&v[0], &t[0], &n[0],
//	//			&v[1], &t[1], &n[1],
//	//			&v[2], &t[2], &n[2],
//	//			&v[3], &t[3], &n[3]) == 12)
//	//		{
//	//			positionIndices.push_back(v[0]);
//	//			positionIndices.push_back(v[1]);
//	//			positionIndices.push_back(v[2]);
//	//			uvIndices.push_back(t[0]);
//	//			uvIndices.push_back(t[1]);
//	//			uvIndices.push_back(t[2]);
//	//			normalIndices.push_back(n[0]);
//	//			normalIndices.push_back(n[1]);
//	//			normalIndices.push_back(n[2]);
//
//	//			positionIndices.push_back(v[0]);
//	//			positionIndices.push_back(v[2]);
//	//			positionIndices.push_back(v[3]);
//	//			uvIndices.push_back(t[0]);
//	//			uvIndices.push_back(t[2]);
//	//			uvIndices.push_back(t[3]);
//	//			normalIndices.push_back(n[0]);
//	//			normalIndices.push_back(n[2]);
//	//			normalIndices.push_back(n[3]);
//	//		}
//	//		else if (sscanf_s(buffer, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
//	//			&v[0], &t[0], &n[0],
//	//			&v[1], &t[1], &n[1],
//	//			&v[2], &t[2], &n[2]) == 9)
//	//		{
//	//			positionIndices.push_back(v[0]);
//	//			positionIndices.push_back(v[1]);
//	//			positionIndices.push_back(v[2]);
//	//			uvIndices.push_back(t[0]);
//	//			uvIndices.push_back(t[1]);
//	//			uvIndices.push_back(t[2]);
//	//			normalIndices.push_back(n[0]);
//	//			normalIndices.push_back(n[1]);
//	//			normalIndices.push_back(n[2]);
//	//		}
//	//		else
//	//		{
//	//			ASSERT(false, "[ModelLoader] Failed to load file %s.", fileName);
//	//		}
//	//	}
//	//}
//	//fclose(file);
//
//	//// Reconstruct vertices
//	//std::vector<Graphics::Vertex> vertices;
//	//for (size_t i = 0; i < positionIndices.size(); ++i)
//	//{
//	//	Graphics::Vertex vertex;
//	//	vertex.position = positions[positionIndices[i] - 1];
//	//	vertex.normal = normals[normalIndices[i] - 1];
//	//	vertex.texcoord.x = uvs[uvIndices[i] - 1].x;
//	//	vertex.texcoord.y = 1.0f - uvs[uvIndices[i] - 1].y;
//	//	vertices.emplace_back(vertex);
//	//}
//	//std::vector<uint32_t> indices;
//	//indices.resize(vertices.size());
//	//std::iota(indices.begin(), indices.end(), 0);
//
//	//// Initialize mesh
//	//mesh.Allocate(vertices.size(), indices.size());
//	//for (size_t i = 0; i < vertices.size(); ++i)
//	//	mesh.GetVertex(i) = vertices[i];
//	//for (size_t i = 0; i < indices.size(); ++i)
//	//	mesh.GetIndex(i) = indices[i];
//}

void DataProcessor::LoadPlayer(const char * fileName, Player* player)
{
	FILE* file = nullptr;
	fopen_s(&file, fileName, "r");
	ASSERT(file, "[DataLoader] Failed to open file %s.", fileName);

	while (true)
	{
		char buffer[128];
		int res = fscanf_s(file, "%s", buffer, static_cast<unsigned int>(std::size(buffer)));
		if (res == EOF)
			break;

		if (strcmp(buffer, "Name:") == 0)
		{
			char name[128];
			fscanf_s(file, "%s\n", name, static_cast<unsigned int>(std::size(name)));
			player->SetName(name);
		}
		else if (strcmp(buffer, "HP:") == 0)
		{
			int hp;
			fscanf_s(file, "%d\n", &hp);
			player->SetHP(hp);
			player->SetBasicHP(hp);
		}
		else if (strcmp(buffer, "Speed:") == 0)
		{
			float speed;
			fscanf_s(file, "%f\n", &speed);
			player->SetSpeed(speed);
		}
		else if (strcmp(buffer, "Damage:") == 0)
		{
			int damage;
			fscanf_s(file, "%d\n", &damage);
			player->SetBasicDamage(damage);
			player->SetDamage(damage);
		}
	}
	fclose(file);
}

//void DataProcessor::SaveLevel(char * fileName, Enemy * enemy)
//{
//
//}
