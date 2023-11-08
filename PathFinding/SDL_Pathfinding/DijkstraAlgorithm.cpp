﻿#include "DijkstraAlgorithm.h"


void DijkstraAlgorithm::Backtrack(int xEnd, int yEnd, int width, std::vector<std::vector<int>>& path, Grid& grid, Agent& agent)
{
	while (path[yEnd][xEnd] > -1) {
		int curr = path[yEnd][xEnd];
		xEnd = curr % width;
		yEnd = curr / width;
		agent.addPathPoint(Vector2D(grid.cell2pix(Vector2D(xEnd, yEnd))));
	}
}

void DijkstraAlgorithm::CalculatePath(Vector2D start, Vector2D end, Grid& grid, Agent& agent)
{
	int width = grid.getNumCellX();
	int height = grid.getNumCellY();
	std::vector<std::vector<int>> path(height, std::vector<int>(width, -2));
	std::vector<std::vector<float>> costSoFar(height, std::vector<float>(width, -1));
	std::priority_queue<std::pair<int, int>, std::vector<std::pair<int,int>>, std::greater<std::pair<int,int>>> frontier;
	frontier.push(std::make_pair(0, (int)end.x + (int)end.y * width));
	path[end.y][end.x] = -1;

	int xEnd = start.x;
	int yEnd = start.y;

	while (!frontier.empty()) {
		int cost = frontier.top().first;
		int curr = frontier.top().second;
		int currX = curr % width;
		int currY = curr / width;
		frontier.pop();

		if (currX == xEnd && currY == yEnd) {
			std::cout << costSoFar[yEnd][xEnd] << std::endl;
			Backtrack(xEnd, yEnd, width, path, grid, agent);
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
			}
		}
	}
}