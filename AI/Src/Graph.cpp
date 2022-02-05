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

bool Graph::CheckNext(Coord coord, int x, int y)
{
	coord.x += x;
	coord.y += y;

	if (coord.x < 0 || coord.x >= mColumns || coord.y < 0 || coord.y >= mRows)
	{
		return false;
	}
	if (GetNode(coord).blocked)
	{
		return false;
	}
	return true;
}

bool Graph::HVScan(Coord& parentCoord, const Coord end, int x, int y, CostFunc g, HeuristicFunc h, uint32_t& direction)
{
	bool nodeCreated{ false };

	if (CheckNext(parentCoord, x, y))
	{
		while (!nodeCreated)
		{
			parentCoord.x += x;
			parentCoord.y += y;

			if (parentCoord == end)
			{
				return true;
			}
			if (CheckNext(parentCoord, x, y))
			{
				if (!CheckNext(parentCoord, y, x) && CheckNext(parentCoord, y + x, x + y))
				{
					direction |= GetDirection(x, y);
					direction |= GetDirection(x, x);
					direction |= GetDirection(y, y);
					//direction -= SearchDirection::Origin;
					nodeCreated = true;
				}
				if (!CheckNext(parentCoord, -y, -x) && CheckNext(parentCoord, -y + x, -x + y))
				{
					direction |= GetDirection(x, y);
					direction |= GetDirection(x, -x);
					direction |= GetDirection(-y, y);
					//direction -= SearchDirection::Origin;
					nodeCreated = true;
				}
			}
			else
			{
				return nodeCreated;
			}
		}
	}
	return nodeCreated;
}

bool Graph::DiagonalScan(const Coord parentCoord, const Coord end, const int x, const int y, CostFunc g, HeuristicFunc h)
{
	ASSERT(x != 0 && y != 0, "Search Direction is not Diagonal.");

	Coord current{ parentCoord };
	bool nodeCreated{ false };

	if (CheckNext(current, x, y))
	{
		while (!nodeCreated)
		{
			if (CheckNext(current, x, 0) || CheckNext(current, 0, y))
			{
				current.x += x;
				current.y += y;

				if (current == end)
				{
					// Found
					JPSSetNode(parentCoord, current, 0u, g, h);
					return true;
				}

				if (GetNode({ current.x - x,current.y }).blocked)
				{
					if (CheckNext(current, 0, y) && CheckNext(current, -x, y))
					{
						uint32_t tempDirection{ 0 };
						tempDirection |= GetDirection(0, y);
						tempDirection |= GetDirection(-x, y);
						tempDirection |= GetDirection(x, y);
						tempDirection |= GetDirection(x, 0);
						JPSSetNode(parentCoord, current, tempDirection, g, h);
						nodeCreated = true;
					}
				}
				else if (GetNode({ current.x ,current.y - y }).blocked)
				{
					if (CheckNext(current, x, 0) && CheckNext(current, x, -y))
					{
						uint32_t tempDirection{ 0 };
						tempDirection |= GetDirection(x, 0);
						tempDirection |= GetDirection(x, -y);
						tempDirection |= GetDirection(x, y);
						tempDirection |= GetDirection(0, y);
						JPSSetNode(parentCoord, current, tempDirection, g, h);
						nodeCreated = true;
					}
				}
				if (!CheckNext(current, x, y))
				{
					uint32_t tempDirection{ 0u };
					tempDirection |= GetDirection(x, 0);
					tempDirection |= GetDirection(0, y);
					JPSSetNode(parentCoord, current, tempDirection, g, h);
					return false;
				}
				if (!nodeCreated)
				{
					bool tempCreated{ false };
					Coord refCurrent1 = current;
					Coord refCurrent2 = current;
					uint32_t tempDirection1{ 0 };
					uint32_t tempDirection2{ 0 };
					if (HVScan(refCurrent1, end, x, 0, g, h, tempDirection1))
					{
						JPSSetNode(parentCoord, current, GetDirection(x, y), g, h);
						tempCreated = true;
						JPSSetNode(current, refCurrent1, tempDirection1, g, h);
					}
					if (HVScan(refCurrent2, end, 0, y, g, h, tempDirection2))
					{
						if (!tempCreated)
						{
							JPSSetNode(parentCoord, current, GetDirection(x, y), g, h);
							tempCreated = true;
						}
						JPSSetNode(current, refCurrent2, tempDirection2, g, h);
					}
					if (tempCreated)
					{
						return true;
					}
				}
			}
			else
				break;
		}
	}
	return false;
}

Graph::SearchDirection AI::Graph::GetDirection(int x, int y)
{
	switch (x)
	{
	case -1:
		switch (y)
		{
		case -1:
			return SearchDirection::UL;
		case 0:
			return SearchDirection::Left;
		case 1:
			return SearchDirection::DL;
		default:
			return SearchDirection::None;
		}
	case 0:
		switch (y)
		{
		case -1:
			return SearchDirection::Up;
		case 1:
			return SearchDirection::Down;
		default:
			return SearchDirection::None;
		}
	case 1:
		switch (y)
		{
		case -1:
			return SearchDirection::UR;
		case 0:
			return SearchDirection::Right;
		case 1:
			return SearchDirection::DR;
		default:
			return SearchDirection::None;
		}
	}
	return SearchDirection::None;
}

uint32_t AI::Graph::AntiDirection(const uint32_t direction)
{
	uint32_t tempDirection{ direction };

	if (direction&SearchDirection::Up)
	{
		tempDirection -= SearchDirection::Up;
		tempDirection |= SearchDirection::Down;
	}
	if (direction&SearchDirection::Down)
	{
		tempDirection -= SearchDirection::Down;
		tempDirection |= SearchDirection::Up;
	}
	if (direction&SearchDirection::Left)
	{
		tempDirection -= SearchDirection::Left;
		tempDirection |= SearchDirection::Right;
	}
	if (direction&SearchDirection::Right)
	{
		tempDirection -= SearchDirection::Right;
		tempDirection |= SearchDirection::Left;
	}
	if (direction&SearchDirection::UL)
	{
		tempDirection -= SearchDirection::UL;
		tempDirection |= SearchDirection::DR;
	}
	if (direction&SearchDirection::DR)
	{
		tempDirection -= SearchDirection::DR;
		tempDirection |= SearchDirection::UL;
	}
	if (direction&SearchDirection::UR)
	{
		tempDirection -= SearchDirection::UR;
		tempDirection |= SearchDirection::DL;
	}
	if (direction&SearchDirection::DL)
	{
		tempDirection -= SearchDirection::DL;
		tempDirection |= SearchDirection::UR;
	}
	return tempDirection;
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
		//GetNode(current).coord = current;

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
		//GetNode(current).coord = current;

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
	//GetNode(start).opened = true;

	bool found = false;
	while (!found && !mOpenList.empty())
	{
		//SortOpenList();

		Coord current = mOpenList.front().coord;

		mOpenList.pop_front();
		//GetNode(current).coord = current;

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
	//GetNode(start).opened = true;

	bool found = false;
	while (!found && !mOpenList.empty())
	{
		Coord current = mOpenList.front().coord;

		mOpenList.pop_front();
		//GetNode(current).coord = current;

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
	//GetNode(start).opened = true;

	bool found = false;
	while (!found && !mOpenList.empty())
	{
		Coord current = mOpenList.front().coord;

		mOpenList.pop_front();
		//GetNode(current).coord = current;
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

bool AI::Graph::RunJPS(Coord start, Coord end, CostFunc g, HeuristicFunc h)
{
	Reset();

	mOpenList.push_back(GetNode(start));
	GetNode(start).sDirection = SearchDirection::Origin;

	bool found = false;

	while (!found && !mOpenList.empty())
	{
		Coord current = mOpenList.front().coord;
		mOpenList.pop_front();

		if (current == end)
		{
			found = true;
		}
		else
		{
			if (GetNode(current).sDirection & AI::Graph::SearchDirection::Origin)
			{
				DiagonalScan(current, end, -1, -1, g, h);
				DiagonalScan(current, end, -1, 1, g, h);
				DiagonalScan(current, end, 1, -1, g, h);
				DiagonalScan(current, end, 1, 1, g, h);

				Coord refCurrent = current;
				uint32_t tempDirection{ 0u };
				if (HVScan(refCurrent, end, 1, 0, g, h, tempDirection))
				{
					JPSSetNode(current, refCurrent, tempDirection, g, h);
				}

				refCurrent = current;
				tempDirection = 0u;
				if (HVScan(refCurrent, end, -1, 0, g, h, tempDirection))
				{
					JPSSetNode(current, refCurrent, tempDirection, g, h);
				}

				refCurrent = current;
				tempDirection = 0u;
				if (HVScan(refCurrent, end, 0, 1, g, h, tempDirection))
				{
					JPSSetNode(current, refCurrent, tempDirection, g, h);
				}

				refCurrent = current;
				tempDirection = 0u;
				if (HVScan(refCurrent, end, 0, -1, g, h, tempDirection))
				{
					JPSSetNode(current, refCurrent, tempDirection, g, h);
				}
			}
			if (GetNode(current).sDirection & AI::Graph::SearchDirection::Up)
			{
				Coord refCurrent = current;
				uint32_t tempDirection{ 0u };
				if (HVScan(refCurrent, end, 0, -1, g, h, tempDirection))
				{
					JPSSetNode(current, refCurrent, tempDirection, g, h);
				}
			}
			if (GetNode(current).sDirection & AI::Graph::SearchDirection::Down)
			{
				Coord refCurrent = current;
				uint32_t tempDirection{ 0u };
				if (HVScan(refCurrent, end, 0, 1, g, h, tempDirection))
				{
					JPSSetNode(current, refCurrent, tempDirection, g, h);
				}
			}
			if (GetNode(current).sDirection & AI::Graph::SearchDirection::Left)
			{
				Coord refCurrent = current;
				uint32_t tempDirection{ 0u };
				if (HVScan(refCurrent, end, -1, 0, g, h, tempDirection))
				{
					JPSSetNode(current, refCurrent, tempDirection, g, h);
				}
			}
			if (GetNode(current).sDirection & AI::Graph::SearchDirection::Right)
			{
				Coord refCurrent = current;
				uint32_t tempDirection{ 0u };
				if (HVScan(refCurrent, end, 1, 0, g, h, tempDirection))
				{
					JPSSetNode(current, refCurrent, tempDirection, g, h);
				}
			}
			if (GetNode(current).sDirection & AI::Graph::SearchDirection::UR)
			{
				DiagonalScan(current, end, 1, -1, g, h);
			}
			if (GetNode(current).sDirection & AI::Graph::SearchDirection::UL)
			{
				DiagonalScan(current, end, -1, -1, g, h);
			}
			if (GetNode(current).sDirection & AI::Graph::SearchDirection::DR)
			{
				DiagonalScan(current, end, 1, 1, g, h);
			}
			if (GetNode(current).sDirection & AI::Graph::SearchDirection::DL)
			{
				DiagonalScan(current, end, -1, 1, g, h);
			}
		}

		mCloseList.push_back(GetNode(current));
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

void Graph::RenderPath(float tileSize, Math::Vector2 p)
{
	char gText[32];
	if (mCloseList.empty())
	{
		return;
	}
	for (uint32_t i = 0; i < mColumns; ++i)
	{
		for (uint32_t j = 0; j < mRows; ++j)
		{
			if (GetNode(Graph::Coord{ i,j }).parent != nullptr)
			{
				Graphics::DrawScreenLine(Math::Vector2{ i*tileSize + tileSize * 0.5f,j*tileSize + tileSize * 0.5f } + p
				, Math::Vector2{ GetNode(Graph::Coord{i,j}).parent->coord.x*tileSize + tileSize * 0.5f,GetNode(Graph::Coord{ i,j }).parent->coord.y*tileSize + tileSize * 0.5f } + p
				, Math::Vector4::Black());
				sprintf_s(gText, 32, "%.3f", GetNode(Graph::Coord{ i,j }).g);
				Graphics::DrawScreenText(gText, Math::Vector2{ i*tileSize + tileSize * 0.5f, j*tileSize + tileSize * 0.5f } + p, tileSize*0.25f, Math::Vector4::White());
			}
		}
	}
	Coord temp = mCloseList.back().coord;
	while (GetNode(temp).parent != nullptr)
	{
		Graphics::DrawScreenLine(Math::Vector2{ temp.x*tileSize + tileSize * 0.5f,temp.y*tileSize + tileSize * 0.5f } + p
		, Math::Vector2{ GetNode(temp).parent->coord.x*tileSize + tileSize * 0.5f,GetNode(temp).parent->coord.y*tileSize + tileSize * 0.5f } + p
		, Math::Vector4::Red());
		temp = GetNode(temp).parent->coord;
	}
	/*while (!mCloseList.empty())
	{
		X::DrawScreenCircle(X::Math::Vector2{ mCloseList.front().coord.x*64.0f + 32.0f,mCloseList.front().coord.y*64.0f + 32.0f }, 5.0f, X::Math::Vector4::Magenta());
		mCloseList.pop_front();
	}*/
}

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
		node.sDirection = SearchDirection::None;
	}
	mOpenList.clear();
	mCloseList.clear();
}

void AI::Graph::JPSSetNode(Coord parentCoord, Coord current, const uint32_t direction, CostFunc g, HeuristicFunc h)
{
	float temp = GetNode(parentCoord).g + g(parentCoord, current);
	if (GetNode(current).parent == nullptr)
	{
		if (GetNode(current).sDirection & SearchDirection::Origin)
			return;

		GetNode(current).h = h(current);
		GetNode(current).g = temp;
		GetNode(current).parent = &GetNode(parentCoord);
		GetNode(current).sDirection |= direction;
		hInsertOpenList(GetNode(current));
	}
	else if (temp < GetNode(current).g)
	{
		GetNode(current).g = temp;
		GetNode(current).parent = &GetNode(parentCoord);
		GetNode(current).sDirection = 0u;
		GetNode(current).sDirection |= direction;
	}
	//else if (GetNode(current).g + g(current, parentCoord) < GetNode(parentCoord).g) // Should never happen
	//{
	//	GetNode(parentCoord).g = GetNode(current).g + g(current, parentCoord);
	//	GetNode(parentCoord).parent = &GetNode(current);
	//	GetNode(current).sDirection = AntiDirection(direction);
	//}
}