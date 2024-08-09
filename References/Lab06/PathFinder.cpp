#include "PathFinder.h"
#include "PathNode.h"
#include "Game.h"
#include <string>
#include <fstream>
#include "SDL2/SDL.h"
#include "CSVHelper.h"
#include <unordered_map>
#include <set>
#include <vector>
#include <algorithm>

PathNode* PathFinder::NextNode(std::set<PathNode*>& openSet,
							   std::map<PathNode*, NodeInfo>& nodeInfo)
{
	bool first = true;
	PathNode* nextNode = nullptr;
	for (PathNode* node : openSet)
	{
		if (first)
		{
			nextNode = node;
			first = false;
		}
		else if (nodeInfo[node].mF < nodeInfo[nextNode].mF)
			nextNode = node;
	}

	return nextNode;
}

bool PathFinder::CalculatePath(class PathNode* start, class PathNode* end,
							   std::vector<class PathNode*>& outPath)
{
	// Setup for A* algorithm
	std::map<PathNode*, NodeInfo> nodeInfo;
	std::set<PathNode*> openSet;

	PathNode* currentNode = start;
	nodeInfo[currentNode].mIsClosed = true;
	do
	{
		for (PathNode* neighbor : currentNode->mAdjacent)
		{
			if (!nodeInfo[neighbor].mIsClosed)
			{
				if (openSet.find(neighbor) != openSet.end())
				{
					float newG = nodeInfo[currentNode].mG +
								 Vector2::Distance(currentNode->GetPosition(),
												   neighbor->GetPosition());
					if (newG < nodeInfo[neighbor].mG)
					{
						nodeInfo[neighbor].mParent = currentNode;
						nodeInfo[neighbor].mG = newG;
						nodeInfo[neighbor].mF = nodeInfo[neighbor].mG + nodeInfo[neighbor].mH;
					}
				}
				else
				{
					nodeInfo[neighbor].mParent = currentNode;
					nodeInfo[neighbor].mH = Vector2::Distance(end->GetPosition(),
															  neighbor->GetPosition());
					nodeInfo[neighbor].mG = nodeInfo[currentNode].mG +
											Vector2::Distance(currentNode->GetPosition(),
															  neighbor->GetPosition());
					nodeInfo[neighbor].mF = nodeInfo[neighbor].mG + nodeInfo[neighbor].mH;
					openSet.insert(neighbor);
				}
			}
		}

		if (openSet.empty())
			break;

		currentNode = NextNode(openSet, nodeInfo);
		openSet.erase(currentNode);
		nodeInfo[currentNode].mIsClosed = true;
	}
	while (currentNode != end);

	if (currentNode == end)
	{
		outPath.clear();
		while (currentNode != start)
		{
			outPath.push_back(currentNode);
			currentNode = nodeInfo[currentNode].mParent;
		}
		return true;
	}
	else
		return false;
}

// DANGER - DO NOT EDIT THE FUNCTIONS BELOW THIS!!
PathFinder::PathFinder(class Game* game)
: Actor(game)
{
	InitializePathNodes();
}

PathNode* PathFinder::GetPathNode(size_t row, size_t col)
{
	return mPathNodeGrid[row][col];
}

void PathFinder::SetIsBlocked(size_t row, size_t col, bool isBlocked)
{
	size_t numRows = mPathNodeGrid.size();
	size_t numCols = mPathNodeGrid[0].size();
	PathNode* p = mPathNodeGrid[row][col];
	if (p != nullptr && p->IsBlocked() != isBlocked)
	{
		mPathNodeGrid[row][col]->SetIsBlocked(isBlocked);

		// Now that its status has changed, we need to fix adjacency
		// If we're being changed to not blocked, tell our neighbors to add us
		if (!isBlocked)
		{
			// Otherwise, tell valid neighbors to add us
			PathNode* above = row > 0 ? mPathNodeGrid[row - 1][col] : nullptr;
			if (above && !above->IsBlocked())
			{
				p->mAdjacent.emplace_back(above);
				above->mAdjacent.emplace_back(p);
			}

			PathNode* below = row < numRows - 1 ? mPathNodeGrid[row + 1][col] : nullptr;
			if (below && !below->IsBlocked())
			{
				p->mAdjacent.emplace_back(below);
				below->mAdjacent.emplace_back(p);
			}

			PathNode* left = col > 0 ? mPathNodeGrid[row][col - 1] : nullptr;
			if (left && !left->IsBlocked())
			{
				p->mAdjacent.emplace_back(left);
				left->mAdjacent.emplace_back(p);
			}

			PathNode* right = col < numCols - 1 ? mPathNodeGrid[row][col + 1] : nullptr;
			if (right && !right->IsBlocked())
			{
				p->mAdjacent.emplace_back(right);
				right->mAdjacent.emplace_back(p);
			}
		}
		else
		{
			// Otherwise, remove adjaceny from ourselves/neighbors
			for (auto neighbor : p->mAdjacent)
			{
				auto iter = std::find(neighbor->mAdjacent.begin(), neighbor->mAdjacent.end(), p);
				if (iter != neighbor->mAdjacent.end())
				{
					neighbor->mAdjacent.erase(iter);
				}
			}

			// Now clear out our adjacency list
			p->mAdjacent.clear();
		}
	}
}

void PathFinder::DebugDrawPaths(SDL_Renderer* renderer)
{
	size_t numRows = mPathNodeGrid.size();
	size_t numCols = mPathNodeGrid[0].size();

	Vector2 cameraPos = mGame->GetCameraPos();
	SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
	for (size_t i = 0; i < numRows; i++)
	{
		for (size_t j = 0; j < numCols; j++)
		{
			if (auto p = mPathNodeGrid[i][j])
			{
				for (auto n : p->mAdjacent)
				{
					SDL_RenderDrawLine(renderer, static_cast<int>(p->GetPosition().x - cameraPos.x),
									   static_cast<int>(p->GetPosition().y - cameraPos.y),
									   static_cast<int>(n->GetPosition().x - cameraPos.x),
									   static_cast<int>(n->GetPosition().y - cameraPos.y));
				}
			}
		}
	}
}

void PathFinder::InitializePathNodes()
{
	// First load in the CSV for the tile to path node map
	std::unordered_map<int, bool> tileToPathMap;
	std::ifstream pathFile("Assets/Map/Paths.csv");
	if (pathFile.is_open())
	{
		std::string line;
		// Skip 1st line
		std::getline(pathFile, line);
		while (!pathFile.eof())
		{
			std::getline(pathFile, line);
			std::vector<std::string> v = CSVHelper::Split(line);
			if (v.size() > 1)
			{
				bool pathable = v[1] == "True";
				tileToPathMap.emplace(std::stoi(v[0]), pathable);
			}
		}
	}

	// Now create the path nodes
	size_t row = 0;

	std::ifstream tileFile("Assets/Map/Tiles.csv");
	if (tileFile.is_open())
	{
		std::string line;
		while (!tileFile.eof())
		{
			std::getline(tileFile, line);
			std::vector<std::string> v = CSVHelper::Split(line);
			if (v.size() > 1)
			{
				mPathNodeGrid.emplace_back(std::vector<PathNode*>(v.size()));
				for (size_t col = 0; col < v.size(); col++)
				{
					auto iter = tileToPathMap.find(std::stoi(v[col]));
					if (iter != tileToPathMap.end())
					{
						PathNode* p = new PathNode(mGame, row, col);
						p->SetIsBlocked(!iter->second);
						p->SetPosition(Vector2(16.0f + col * 32.0f, 16.0f + row * 32.0f));
						mPathNodeGrid[row][col] = p;
					}
				}
				row++;
			}
		}
	}

	// Now hook up edges
	size_t numRows = mPathNodeGrid.size();
	size_t numCols = mPathNodeGrid[0].size();
	for (size_t i = 0; i < numRows; i++)
	{
		for (size_t j = 0; j < numCols; j++)
		{
			if (auto p = mPathNodeGrid[i][j])
			{
				// Skip adding neighbors for this node if it's blocked
				if (p->IsBlocked())
				{
					continue;
				}

				PathNode* above = i > 0 ? mPathNodeGrid[i - 1][j] : nullptr;
				if (above && !above->IsBlocked())
				{
					p->mAdjacent.emplace_back(above);
				}

				PathNode* below = i < numRows - 1 ? mPathNodeGrid[i + 1][j] : nullptr;
				if (below && !below->IsBlocked())
				{
					p->mAdjacent.emplace_back(below);
				}

				PathNode* left = j > 0 ? mPathNodeGrid[i][j - 1] : nullptr;
				if (left && !left->IsBlocked())
				{
					p->mAdjacent.emplace_back(left);
				}

				PathNode* right = j < numCols - 1 ? mPathNodeGrid[i][j + 1] : nullptr;
				if (right && !right->IsBlocked())
				{
					p->mAdjacent.emplace_back(right);
				}
			}
		}
	}
}
