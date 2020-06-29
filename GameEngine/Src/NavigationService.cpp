#include "Precompiled.h"
#include "NavigationService.h"

using namespace S;

namespace S
{
	enum Direction
	{
		N,
		S,
		E,
		W,
		NE,
		NW,
		SE,
		SW
	};
	struct Node
	{
		Math::Vector3 position;
		Node* neighbors[8];
		Node* parent;
		float g{ 0.0f };
		float h{ 0.0f };
		bool open{ false };
		bool closed{ false };
	};

	struct Graph
	{
	public:
		void Reset()
		{
			for (auto n : mNodes)
			{
				n.parent = nullptr;
				n.g = 0.0f;
				n.h = 0.0f;
				n.open = false;
				n.closed = false;
			}
		}
		std::vector<Node> mNodes;
	};
}
NavigationService::NavigationService()
	: Service(StaticGetTypeId())
{

}
NavigationService::~NavigationService()
{
}

void NavigationService::Setup(uint32_t width, uint32_t height, float scale)
{
	mWidth = width;
	mHeight = height;
	mScale = scale;
}

void NavigationService::Initialize()
{
	mGraph = std::make_unique<Graph>();

	uint32_t numNodes = mWidth * mHeight;
	mGraph->mNodes.resize(numNodes);
	for (uint32_t z = 0; z < mHeight; ++z)
	{
		for (uint32_t x = 0; x < mWidth; ++x)
		{
			uint32_t index = x + (z*mWidth);
			auto& node = mGraph->mNodes[index];

			node.position = Math::Vector3{ static_cast<float>(x*mScale),0.0f,static_cast<float>(z*mScale) };

			bool hasNorth = z + 1 < mHeight;
			bool hasEast = x + 1 < mWidth;
			bool hasSouth = z > 0;
			bool hasWest = x > 0;
			if (hasNorth)
				node.neighbors[N] = mGraph->mNodes.data() + (index + mWidth);
			if (hasSouth)
				node.neighbors[S] = mGraph->mNodes.data() + (index - mWidth);
			if (hasEast)
				node.neighbors[E] = mGraph->mNodes.data() + (index + 1);
			if (hasWest)
				node.neighbors[W] = mGraph->mNodes.data() + (index - 1);
			if (hasNorth&&hasEast)
				node.neighbors[NE] = mGraph->mNodes.data() + (index + mWidth + 1);
			if (hasNorth&&hasWest)
				node.neighbors[NW] = mGraph->mNodes.data() + (index + mWidth - 1);
			if (hasSouth && hasEast)
				node.neighbors[SE] = mGraph->mNodes.data() + (index - mWidth + 1);
			if (hasSouth&&hasWest)
				node.neighbors[SW] = mGraph->mNodes.data() + (index - mWidth - 1);
		}
	}
}

void NavigationService::Terminate()
{
	mGraph->Reset();
}

void NavigationService::Update(float deltaTime)
{

}

void NavigationService::Render()
{
}

void NavigationService::Render2D()
{
}

void NavigationService::RequestPath(PathRequest & request)
{
	mGraph->Reset();

	std::list<Node*> openList;
	std::list<Node*> closedList;

	uint32_t startIndex = static_cast<int>(request.start.x) + static_cast<uint32_t>(request.start.z)*mWidth;
	uint32_t endIndex = (uint32_t)(request.end.x) + (uint32_t)(request.end.z)*mWidth;
	Node* startNode = mGraph->mNodes.data();
	Node* endNode = mGraph->mNodes.data() + endIndex;

	openList.push_back(startNode);
	startNode->open = true;

	bool found = false;
	while (!found && !openList.empty())
	{
		Node* nextNode = openList.front();
		openList.pop_front();
		if (nextNode == endNode)
		{
			found = true;
		}
		else
		{
			for (int i = 0; i < 8; ++i)
			{
				Node* neighborNode = nextNode->neighbors[i];
				if (neighborNode)
				{
					float g = nextNode->g + Math::Distance(nextNode->position, neighborNode->position);
					if (!neighborNode->open)
					{
						float h = Math::Distance(neighborNode->position, endNode->position);
						auto iter = openList.begin();
						while (iter != openList.end())
						{
							if ((*iter)->g + h > g + h)
								break;
							++iter;
						}
						openList.insert(iter, neighborNode);
						neighborNode->parent = nextNode;
						neighborNode->g = g;
						neighborNode->h = h;
					}
					else if (g < neighborNode->g)
					{
						neighborNode->parent = nextNode;
						neighborNode->g = g;
					}
				}
			}
			closedList.push_back(nextNode);
		}
		if (found)
		{

		}
	}
}
