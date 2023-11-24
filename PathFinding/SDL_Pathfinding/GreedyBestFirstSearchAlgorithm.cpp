﻿#include "GreedyBestFirstSearchAlgorithm.h"

#include <queue>
#include "Vector2D.h"
#include "Grid.h"
#include "Path.h"

float GreedyBestFirstSearchAlgorithm::Distance(Vector2D start, Vector2D goal)
{
	float dx = abs(start.x - goal.x);
	float dy = abs(start.y - goal.y);
	if (dx < dy) {
		float temp = dy;
		dy = dx;
		dx = temp;
	}
	return dy * 1.4142 + dx - dy;
}

int GreedyBestFirstSearchAlgorithm::CalculatePath(Vector2D start, Vector2D end, const Grid& grid, Path& agentPath)
{
	int visitedNodes = 0;
	int width = grid.getNumCellX();
	int height = grid.getNumCellY();
	std::vector<std::vector<int>> path(height, std::vector<int>(width, -2));
	std::priority_queue<std::pair<float, int>, std::vector<std::pair<float, int>>, std::greater<std::pair<float, int>>> frontier;
	frontier.push(std::make_pair(0, (int)end.x + (int)end.y * width));
	path[end.y][end.x] = -1;

	int xEnd = start.x;
	int yEnd = start.y;

	while (!frontier.empty()) {
		int curr = frontier.top().second;
		int currX = curr % width;
		int currY = curr / width;
		frontier.pop();

		if (currX == xEnd && currY == yEnd) {
			Backtrack(xEnd, yEnd, width, path, grid, agentPath);
			break;
		}

		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				int neighborX = currX + i;
				int neighborY = currY + j;

				if (
					(i == 0 && j == 0) ||
					!grid.isValidCell(Vector2D(neighborX, neighborY)) ||
					!(path[neighborY][neighborX] == -2)) continue;

				frontier.push(std::make_pair(Distance(Vector2D(neighborX, neighborY), Vector2D(xEnd, yEnd)), neighborX + neighborY * width));
				path[neighborY][neighborX] = curr;
				visitedNodes++;
			}
		}
	}
	return visitedNodes;
}