#include "Precompiled.h"
#include "Graph.h"

using namespace S;
using namespace S::AI;

Graph::Graph()
	:mNodes{ nullptr }
	, mColumns{ 0 }
	, mRows{ 0 }
{
}

void Graph::Init(uint32_t columns, uint32_t rows, float size)
{
	mNodes = std::make_unique<Node[]>(columns*rows);
	mColumns = columns;
	mRows = rows;
	tileSize = size;
	for (uint32_t i = 0; i < columns; ++i)
	{
		for (uint32_t j = 0; j < rows; ++j)
		{
			GetNode({ i, j }).coord = { i, j };
		}
	}
}

void Graph::Clear()
{
	mNodes.reset();
	mColumns = 0;
	mRows = 0;
}

void Graph::SetBlock(uint32_t columns, uint32_t rows)
{
	GetNode({ columns,rows }).blocked = true;
}

void Graph::Unblock(uint32_t columns, uint32_t rows)
{
	GetNode({ columns,rows }).blocked = false;
}

void Graph::gInsertOpenList(Node& node)
{
	if (!mOpenList.empty())
	{
		for (auto it = mOpenList.begin(); it != mOpenList.end(); it++)
		{
			if (node.g < GetNode(it->coord).g)
			{
				mOpenList.insert(it, node);
				return;
			}
		}
		mOpenList.push_back(node);
	}
	else
	{
		mOpenList.push_back(node);
	}
}

void Graph::hInsertOpenList(Node& node)
{
	if (!mOpenList.empty())
	{
		for (auto it = mOpenList.begin(); it != mOpenList.end(); ++it)
		{
			if (node.g + node.h < GetNode(it->coord).g + GetNode(it->coord).h)
			{
				mOpenList.insert(it, node);
				break;
			}
		}
		mOpenList.push_back(node);
	}
	else
	{
		mOpenList.push_back(node);
	}
}

/*void Graph::SortOpenList()
{
	// ============Sort way 1============
	//mOpenList.sort(std::less<Node>()); 
	// ============Sort way 2============
	//CompareWeight cw;
	//mOpenList.sort(cw);
	// ============Sort way 3============
	//mOpenList.sort([](Node n1, Node n2) {return n1.g < n2.g; });
}*/

Graph::Node & Graph::GetNode(Coord coord) const
{
	ASSERT(coord.x < mColumns && coord.y < mRows, "[Graph] Invalid coordinates.");
	return mNodes[coord.x + (coord.y*mColumns)];
}

float Graph::GetTileSize()
{
	return tileSize;
}

bool Graph::RunBFS(Coord start, Coord end)
{
	Reset();

	mOpenList.push_back(GetNode(start));
	GetNode(start).opened = true;

	bool found = false;
	while (!found && !mOpenList.empty())
	{
		Coord current = mOpenList.front().coord;
		mOpenList.pop_front();
		GetNode(current).coord = current;

		if (current == end)
		{
			found = true;
		}
		else
		{
			for (int c = -1; c < 2; ++c)
			{
				for (int r = -1; r < 2; ++r)
				{
					if (c == 0 && r == 0)
					{
						continue;
					}
					if (current.x + c >= 0 
						&& current.x + c < mColumns
						&& current.y + r >= 0 
						&& current.y + r < mRows)
					{
						if (!GetNode({ current.x + c,current.y + r }).blocked
							&& !GetNode({ current.x + c,current.y + r }).opened
							&& !GetNode({ current.x + c,current.y + r }).closed)
						{
							mOpenList.push_back(GetNode({ current.x + c,current.y + r }));
							GetNode({ current.x + c,current.y + r }).opened = true;
							GetNode({ current.x + c,current.y + r }).parent = &GetNode(current);
						}
					}
				}
			}
		}

		mCloseList.push_back(GetNode(current));
		GetNode(current).closed = true;
	}
	return found;
}

bool Graph::RunDFS(Coord start, Coord end)
{
	Reset();

	mOpenList.push_back(GetNode(start));
	GetNode(start).opened = true;

	bool found = false;
	while (!found && !mOpenList.empty())
	{
		Coord current = mOpenList.back().coord;
		mOpenList.pop_back();
		GetNode(current).coord = current;

		if (current == end)
		{
			found = true;
		}
		else
		{
			for (int c = -1; c < 2; ++c)
			{
				for (int r = -1; r < 2; ++r)
				{
					if (c == 0 && r == 0)
					{
						continue;
					}
					if (current.x + c >= 0
						&& current.x + c < mColumns
						&& current.y + r >= 0
						&& current.y + r < mRows)
					{
						if (!GetNode({ current.x + c,current.y + r }).blocked
							&& !GetNode({ current.x + c,current.y + r }).opened
							&& !GetNode({ current.x + c,current.y + r }).closed)
						{
							mOpenList.push_back(GetNode({ current.x + c,current.y + r }));
							GetNode({ current.x + c,current.y + r }).opened = true;
							GetNode({ current.x + c,current.y + r }).parent = &GetNode(current);
						}
					}
				}
			}
		}

		mCloseList.push_back(GetNode(current));
		GetNode(current).closed = true;
	}
	return found;
}

bool Graph::RunDijkstra(Coord start, Coord end, CostFunc costFunc)
{
	Reset();

	mOpenList.push_back(GetNode(start));
	GetNode(start).opened = true;

	bool found = false;
	while (!found && !mOpenList.empty())
	{
		//SortOpenList();

		Coord current = mOpenList.front().coord;

		mOpenList.pop_front();
		GetNode(current).coord = current;

		if (current == end)
		{
			found = true;
		}
		else
		{
			for (int c = -1; c < 2; ++c)
			{
				for (int r = -1; r < 2; ++r)
				{
					if (c == 0 && r == 0)
					{
						continue;
					}
					if (current.x + c >= 0
						&& current.x + c < mColumns
						&& current.y + r >= 0
						&& current.y + r < mRows)
					{
						if (!GetNode({ current.x + c,current.y + r }).blocked
							&& !GetNode({ current.x + c,current.y + r }).closed)
						{
							float temp = GetNode(current).g + costFunc(current, { current.x + c,current.y + r });
							if (GetNode({ current.x + c,current.y + r }).parent == nullptr)
							{
								GetNode({ current.x + c,current.y + r }).g = temp;
								GetNode({ current.x + c,current.y + r }).parent = &GetNode(current);
								//mOpenList.push_back(GetNode({ current.x + c,current.y + r })); // if using Sort
								gInsertOpenList(GetNode({ current.x + c,current.y + r }));
							}
							else if (temp < GetNode({ current.x + c,current.y + r }).g)
							{
								GetNode({ current.x + c,current.y + r }).g = temp;
								GetNode({ current.x + c,current.y + r }).parent = &GetNode(current);
							}
							else if (GetNode({ current.x + c,current.y + r }).g + costFunc(current, { current.x + c,current.y + r }) < GetNode(current).g)
							{
								GetNode(current).g = GetNode({ current.x + c,current.y + r }).g + costFunc(current, { current.x + c,current.y + r });
								GetNode(current).parent = &GetNode({ current.x + c,current.y + r });
							}
						}
					}
				}
			}
		}

		mCloseList.push_back(GetNode(current));
		GetNode(current).closed = true;
	}
	return found;
}

bool Graph::RunAStar(Coord start, Coord end, CostFunc g, HeuristicFunc h)
{
	Reset();

	mOpenList.push_back(GetNode(start));
	GetNode(start).opened = true;

	bool found = false;
	while (!found && !mOpenList.empty())
	{
		Coord current = mOpenList.front().coord;

		mOpenList.pop_front();
		GetNode(current).coord = current;

		if (current == end)
		{
			found = true;
		}
		else
		{
			for (int c = -1; c < 2; ++c)
			{
				for (int r = -1; r < 2; ++r)
				{
					if (c == 0 && r == 0)
					{
						continue;
					}
					if (current.x + c >= 0
						&& current.x + c < mColumns
						&& current.y + r >= 0
						&& current.y + r < mRows)
					{
						if (!GetNode({ current.x + c,current.y + r }).blocked
							&& !GetNode({ current.x + c,current.y + r }).closed)
						{
							float temp = GetNode(current).g + g(current, { current.x + c,current.y + r });
							if (GetNode({ current.x + c,current.y + r }).parent == nullptr)
							{
								GetNode({ current.x + c,current.y + r }).h = h({ current.x + c,current.y + r });
								GetNode({ current.x + c,current.y + r }).g = temp;
								GetNode({ current.x + c,current.y + r }).parent = &GetNode(current);
								hInsertOpenList(GetNode({ current.x + c,current.y + r }));
							}
							else if (temp < GetNode({ current.x + c,current.y + r }).g)
							{
								GetNode({ current.x + c,current.y + r }).g = temp;
								GetNode({ current.x + c,current.y + r }).parent = &GetNode(current);
							}
							else if (GetNode({ current.x + c,current.y + r }).g + g(current, { current.x + c,current.y + r }) < GetNode(current).g)
							{
								GetNode(current).g = GetNode({ current.x + c,current.y + r }).g + g(current, { current.x + c,current.y + r });
								GetNode(current).parent = &GetNode({ current.x + c,current.y + r });
							}
						}
					}
				}
			}
		}

		mCloseList.push_back(GetNode(current));
		GetNode(current).closed = true;
	}
	return found;
}

bool Graph::RunDijkstra(Coord start, std::vector<Coord> c, CostFunc costFunc)
{
	Reset();

	mOpenList.push_back(GetNode(start));
	GetNode(start).opened = true;

	bool found = false;
	while (!found && !mOpenList.empty())
	{
		Coord current = mOpenList.front().coord;

		mOpenList.pop_front();
		GetNode(current).coord = current;
		for (size_t i = 0; i < c.size(); ++i)
		{
			if (current == c[i])
			{
				found = true;
			}
		}
		if (found)
		{
			mCloseList.push_back(GetNode(current));
			GetNode(current).closed = true;
			continue;
		}
		else
		{
			for (int c = -1; c < 2; ++c)
			{
				for (int r = -1; r < 2; ++r)
				{
					if (c == 0 && r == 0)
					{
						continue;
					}
					if (current.x + c >= 0
						&& current.x + c < mColumns
						&& current.y + r >= 0
						&& current.y + r < mRows)
					{
						if (!GetNode({ current.x + c,current.y + r }).blocked
							&& !GetNode({ current.x + c,current.y + r }).closed)
						{
							float temp = GetNode(current).g + costFunc(current, { current.x + c,current.y + r });
							if (GetNode({ current.x + c,current.y + r }).parent == nullptr)
							{
								GetNode({ current.x + c,current.y + r }).g = temp;
								GetNode({ current.x + c,current.y + r }).parent = &GetNode(current);
								gInsertOpenList(GetNode({ current.x + c,current.y + r }));
							}
							else if (temp < GetNode({ current.x + c,current.y + r }).g)
							{
								GetNode({ current.x + c,current.y + r }).g = temp;
								GetNode({ current.x + c,current.y + r }).parent = &GetNode(current);
							}
							else if (GetNode({ current.x + c,current.y + r }).g + costFunc(current, { current.x + c,current.y + r }) < GetNode(current).g)
							{
								GetNode(current).g = GetNode({ current.x + c,current.y + r }).g + costFunc(current, { current.x + c,current.y + r });
								GetNode(current).parent = &GetNode({ current.x + c,current.y + r });
							}
						}
					}
				}
			}
		}

		mCloseList.push_back(GetNode(current));
		GetNode(current).closed = true;
	}
	return found;
}

std::vector<Math::Vector2> Graph::GetPosPath()
{
	Graph::Node tempNode = mCloseList.back();
	std::vector<Math::Vector2> tempVec;
	tempVec.insert(tempVec.begin(), { tempNode.coord.x*tileSize + tileSize * 0.5f,tempNode.coord.y*tileSize + tileSize * 0.5f });
	while (tempNode.parent != nullptr)
	{
		tempVec.insert(tempVec.begin(), { tempNode.parent->coord.x*tileSize + tileSize * 0.5f,tempNode.parent->coord.y*tileSize + tileSize * 0.5f });
		tempNode = *tempNode.parent;
	}
	return tempVec;
}

std::vector<Graph::Node> Graph::GetPath()
{
	return std::vector<Node>();
}

std::list<Graph::Node> Graph::GetCloseList()
{
	return mCloseList;
}

//void Graph::RenderPath(float tileSize)
//{
//	char gText[32];
//	if (mCloseList.empty())
//	{
//		return;
//	}
//	for (uint32_t i = 0; i < mColumns; ++i)
//	{
//		for (uint32_t j = 0; j < mRows; ++j)
//		{
//			if (GetNode(Graph::Coord{ i,j }).parent != nullptr)
//			{
//				S::DrawScreenLine(Math::Vector2{ i*tileSize,j*tileSize }
//				, X::Math::Vector2{ GetNode(Graph::Coord{i,j}).parent->coord.x*tileSize,GetNode(Graph::Coord{ i,j }).parent->coord.y*tileSize }
//				, X::Math::Vector4::Black());
//				sprintf_s(gText, 32, "%.3f", GetNode(Graph::Coord{ i,j }).g);
//				X::DrawScreenText(gText, i*tileSize, j*tileSize, tileSize*0.25f, X::Math::Vector4::White());
//			}
//		}
//	}
//	Coord temp = mCloseList.back().coord;
//	while (GetNode(temp).parent != nullptr)
//	{
//		X::DrawScreenLine(X::Math::Vector2{ temp.x*tileSize,temp.y*tileSize }
//		, X::Math::Vector2{ GetNode(temp).parent->coord.x*tileSize,GetNode(temp).parent->coord.y*tileSize }
//		, X::Math::Vector4::Red());
//		temp = GetNode(temp).parent->coord;
//	}
//	/*while (!mCloseList.empty())
//	{
//		X::DrawScreenCircle(X::Math::Vector2{ mCloseList.front().coord.x*64.0f + 32.0f,mCloseList.front().coord.y*64.0f + 32.0f }, 5.0f, X::Math::Vector4::Magenta());
//		mCloseList.pop_front();
//	}*/
//}

void Graph::Reset()
{
	for (uint32_t i = 0; i < mColumns * mRows; ++i)
	{
		auto& node = mNodes[i];
		node.g = 0.0f;
		node.h = 0.0f;
		node.opened = false;
		node.closed = false;
		node.parent = nullptr;
	}
	mOpenList.clear();
	mCloseList.clear();
}