#ifndef INCLUDED_GAMEENGINE_QUADTREE_H
#define INCLUDED_GAMEENGINE_QUADTREE_H

namespace S {

class Quadtree
{
public:
	struct Node
	{
		std::array<std::unique_ptr<Node>, 4> children;
		Geometry::AABB aabb;
		bool isLeaf{ false };
		std::vector<uint32_t> indices;

		void DrawNode() const
		{
			Math::Vector3 center = aabb.center;
			float max = Math::Max(Math::Abs(center.x), Math::Max(Math::Abs(center.y), Math::Abs(center.z)));
			Graphics::SimpleDraw::AddAABB(aabb, { center.x,center.y,center.z, 1.0f });
		};
	};

	using Visitor = std::function<void(Node*)>;

	Quadtree() = default;

	void Initialize(S::Graphics::Mesh* mesh, uint32_t levels);

	void Visit(Visitor& visitor);

	bool Intersect(Geometry::Ray ray, float& distance) const;

private:
	enum Quadrant { BL, BR, TL, TR };

	void GenerateNodeRecursive(std::unique_ptr<Node>& node, S::Geometry::AABB aabb, uint32_t levels);
	void VisitRecursive(Node* node, Visitor& visitor);
	bool IntersectRecursive(Node* node, S::Geometry::Ray ray, float& distance) const;

	S::Graphics::Mesh* mMesh{ nullptr };
	std::unique_ptr<Node> mRoot{ nullptr };
};

} // namespace S

#endif // #ifndef INCLUDED_GAMEENGINE_QUADTREE_H