#ifndef INCLUDE_AI_H
#define INCLUDE_AI_H

namespace S {
namespace AI {

class Graph
{
public:
	struct Coord
	{
		uint32_t x{ 0 }, y{ 0 };

		bool operator==(const Coord& other) const
		{
			return x == other.x && y == other.y;
		}
	};

	struct Node
	{
		Coord coord;
		Node* parent{ nullptr };
		float g{ 0.0f }, h{ 0.0f };
		bool opened{ false };
		bool closed{ false };
		bool blocked{ false };
		// ============Sort way 1============
		//bool operator<(const Node& rhs)const { return g < rhs.g; }
	};

	// ============Sort way 2============
	/*struct CompareWeight
	{
		bool operator()(Node & l, Node & r)
		{
			return l.g < r.g;
		}
	};*/

	using CostFunc = std::function<float(Coord, Coord)>;
	using HeuristicFunc = std::function<float(Coord)>;

	Graph();

	void Init(uint32_t columns, uint32_t rows, float size);
	void Clear();
	void SetBlock(uint32_t columns, uint32_t rows);
	void Unblock(uint32_t columns, uint32_t rows);
	void gInsertOpenList(Node& node);
	void hInsertOpenList(Node& node);
	//void SortOpenList(); // Sort is not better than Ordered Insert

	Node& GetNode(Coord coord) const;
	float GetTileSize();

	bool RunBFS(Coord start, Coord end);
	bool RunDFS(Coord start, Coord end);
	bool RunDijkstra(Coord start, Coord end, CostFunc costFunc);
	bool RunAStar(Coord start, Coord end, CostFunc g, HeuristicFunc h);
	bool RunDijkstra(Coord start, std::vector<Coord> c, CostFunc costFunc);

	std::vector<Math::Vector2> GetPosPath();
	std::vector<Node> GetPath();
	std::list<Node> GetCloseList();

	uint32_t GetColumns() const { return mColumns; }
	uint32_t GetRows() const { return mRows; }

	//void RenderPath(float tileSize);

private:
	void Reset();

	// Graph State
	std::unique_ptr<Node[]> mNodes;
	float tileSize;
	uint32_t mColumns;
	uint32_t mRows;

	// Search State
	std::list<Node> mOpenList;
	std::list<Node> mCloseList;
};

} // namespace AI
} // namespace S

#endif // !INCLUDE_AI_H