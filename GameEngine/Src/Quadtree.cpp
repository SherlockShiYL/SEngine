#include "Precompiled.h"
#include "Quadtree.h"

using namespace S;

namespace
{
	Geometry::AABB GetBL(const Geometry::AABB& aabb)
	{
		Math::Vector3 min = aabb.center - Math::Vector3{ aabb.extend.x, aabb.extend.y, aabb.extend.z };
		Math::Vector3 max = aabb.center + Math::Vector3{ 0.0f, aabb.extend.y, 0.0f };
		return Geometry::AABB::FromMinMax(min, max);
	}

	Geometry::AABB GetBR(const Geometry::AABB& aabb)
	{
		Math::Vector3 min = aabb.center - Math::Vector3{ 0.0f, aabb.extend.y, aabb.extend.z };
		Math::Vector3 max = aabb.center + Math::Vector3{ aabb.extend.x, aabb.extend.y, 0.0f };
		return Geometry::AABB::FromMinMax(min, max);
	}

	Geometry::AABB GetTL(const Geometry::AABB& aabb)
	{
		Math::Vector3 min = aabb.center - Math::Vector3{ aabb.extend.x, aabb.extend.y, 0.0f };
		Math::Vector3 max = aabb.center + Math::Vector3{ 0.0f, aabb.extend.y, aabb.extend.z };
		return Geometry::AABB::FromMinMax(min, max);
	}

	Geometry::AABB GetTR(const Geometry::AABB& aabb)
	{
		Math::Vector3 min = aabb.center - Math::Vector3{ 0.0f, aabb.extend.y, 0.0f };
		Math::Vector3 max = aabb.center + Math::Vector3{ aabb.extend.x, aabb.extend.y, aabb.extend.z };
		return Geometry::AABB::FromMinMax(min, max);
	}
}

void Quadtree::Initialize(Graphics::Mesh* mesh, uint32_t levels)
{
	mMesh = mesh;

	// Get the X/Z bound of the mesh, force Y to be -1000 to 1000
	Geometry::AABB bound = Graphics::MeshBuilder::ComputeBound(*mesh);
	bound.center.y = 0.0f;
	bound.extend.y = 50.0f;

	// Generate nodes up to number of levels
	GenerateNodeRecursive(mRoot, bound, levels);

	// Assign triangles
	const auto* vertices = mesh->GetVertices();
	const auto* indices = mesh->GetIndices();
	int f = mesh->GetIndexCount();
	for (uint32_t i = 0; i < mesh->GetIndexCount(); i += 3)
	{
		const auto& a = vertices[indices[i + 0]];
		const auto& b = vertices[indices[i + 1]];
		const auto& c = vertices[indices[i + 2]];

		Visitor visitor = [&a, &b, &c, i](Node* node)
		{
			if (!node->isLeaf)
				return;
		
			if (!Geometry::Intersect(a.position, node->aabb) &&
				!Geometry::Intersect(b.position, node->aabb) &&
				!Geometry::Intersect(c.position, node->aabb))
				return;
		
			node->indices.push_back(i);
		};
		Visit(visitor);
	}
}

void Quadtree::Visit(Visitor& visitor)
{
	VisitRecursive(mRoot.get(), visitor);
}

bool Quadtree::Intersect(Geometry::Ray ray, float& distance) const
{
	if (mRoot == nullptr)
		return false;

	distance = std::numeric_limits<float>::max();
	return IntersectRecursive(mRoot.get(), ray, distance);
}

void Quadtree::GenerateNodeRecursive(std::unique_ptr<Node>& node, Geometry::AABB aabb, uint32_t levels)
{
	node = std::make_unique<Node>();
	node->aabb = aabb;

	if (--levels == 0)
	{
		node->isLeaf = true;
	}
	else
	{
		GenerateNodeRecursive(node->children[BL], GetBL(aabb), levels);
		GenerateNodeRecursive(node->children[BR], GetBR(aabb), levels);
		GenerateNodeRecursive(node->children[TL], GetTL(aabb), levels);
		GenerateNodeRecursive(node->children[TR], GetTR(aabb), levels);
	}
}

void Quadtree::VisitRecursive(Node* node, Visitor& visitor)
{
	visitor(node);

	if (!node->isLeaf)
	{
		for (auto& child : node->children)
		{
			VisitRecursive(child.get(), visitor);
		}
	}
}

bool Quadtree::IntersectRecursive(Node* node, Geometry::Ray ray, float& distance) const
{
	float distEntry, distExit;
	if (!Geometry::Intersect(ray, node->aabb, distEntry, distExit))
		return false;

	static bool showIntersect = true;
	if (showIntersect)
	{
		node->DrawNode();
	}

	if (!node->isLeaf)
	{
		return
			IntersectRecursive(node->children[0].get(), ray, distance) ||
			IntersectRecursive(node->children[1].get(), ray, distance) ||
			IntersectRecursive(node->children[2].get(), ray, distance) ||
			IntersectRecursive(node->children[3].get(), ray, distance);
	}

	bool intersect = false;
	for (size_t i = 0; i < node->indices.size(); ++i)
	{
		uint32_t faceIndex = node->indices[i];
		const auto& a = mMesh->GetVertex(mMesh->GetIndex(faceIndex + 0));
		const auto& b = mMesh->GetVertex(mMesh->GetIndex(faceIndex + 1));
		const auto& c = mMesh->GetVertex(mMesh->GetIndex(faceIndex + 2));

		float d = 0.0f;
		if (Geometry::Intersect(ray, a.position, b.position, c.position, d))
		{
			distance = Math::Min(distance, d);
			intersect = true;
		}
	}
	return intersect;
}
