﻿#include "DijkstraAlgorithm.h"

#include <queue>
#include "Vector2D.h"
#include "Grid.h"
#include "Path.h"

int DijkstraAlgorithm::CalculatePath(Vector2D start, Vector2D end, const Grid& grid, Path& agentPath)
{
	int visitedNodes = 0;
	int width = grid.getNumCellX();
	int height = grid.getNumCellY();
	std::vector<std::vector<int>> path(height, std::vector<int>(width, -2));
	std::vector<std::vector<float>> costSoFar(height, std::vector<float>(width, -1));
	std::priority_queue<std::pair<float, int>, std::vector<std::pair<float,int>>, std::greater<std::pair<float,int>>> frontier;
	frontier.push(std::make_pair(0, (int)end.x + (int)end.y * width));
	path[end.y][end.x] = -1;

	int xEnd = start.x;
	int yEnd = start.y;

	while (!frontier.empty()) {
		int curr = frontier.top().second;
		int currX = curr % width;
		int currY = curr / width;
		float cost = costSoFar[currY][currX];
		frontier.pop();

		if (currX == xEnd && currY == yEnd) {
			//std::cout << costSoFar[yEnd][xEnd] << std::endl;
			Backtrack(xEnd, yEnd, width, path, grid, agentPath);
			break;
		}

		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				int neighborX = currX + i;
				int neighborY = currY + j;

				int currPath = path[currY][currX];
				int pathX = currPath % width;
				int pathY = currPath / width;
				Vector2D dir = Vector2D(currX, currY) - Vector2D(pathX, pathY);
				Vector2D newDir = Vector2D(neighborX, neighborY) - Vector2D(currX, currY);
				bool isTurning = dir != newDir && currPath > -1;
				float turnCost = 1.f * isTurning;

				bool isDiagonal = abs(i + j) != 1;

				float nextCellWeight = grid.GetCellWeight(Vector2D(neighborX, neighborY));
				float addCost = isDiagonal ? sqrt(nextCellWeight * nextCellWeight * 2) : nextCellWeight;
				float newCost = cost + turnCost + addCost;

				if (
					(i == 0 && j == 0) ||
					!grid.isValidCell(Vector2D(neighborX, neighborY)) ||
					!(path[neighborY][neighborX] == -2 || newCost < costSoFar[neighborY][neighborX])) continue;

				costSoFar[neighborY][neighborX] = newCost;
				frontier.push(std::make_pair(newCost, neighborX + neighborY * width));
				path[neighborY][neighborX] = curr;
				visitedNodes++;
			}
		}
	}
	return visitedNodes;
}