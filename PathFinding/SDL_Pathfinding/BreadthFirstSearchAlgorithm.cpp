﻿#include "BreadthFirstSearchAlgorithm.h"

#define max(a, b) ((a) > (b) ? (a) : (b))

void Backtrack(int xEnd, int yEnd, int width, int height, std::vector<std::vector<int>>& visited, Agent& agent) {
	while (visited[yEnd][xEnd] != 1) {
		for (int i = max(0, xEnd - 1); i < xEnd + 2 && i < width; i++) {
			for (int j = max(0, yEnd - 1); j < yEnd + 2 && j < height; j++) {
				if (visited[j][i] == visited[yEnd][xEnd] - 1) {
					agent.addPathPoint(Vector2D(i, j));
					xEnd = i;
					yEnd = j;
					i = j = INT_MAX;
				}
			}
		}
	}
}

void BreadthFirstSearchAlgorithm::CalculatePath(Vector2D start, Vector2D end, Grid& grid, Agent& agent)
{
	int width = grid.getNumCellX();
	int height = grid.getNumCellY();
	std::vector<std::vector<int>> visited(width, std::vector<int>(height));
	std::queue<std::pair<int, int>> toVisit;
	toVisit.push({ (int)end.x + (int)end.y * width , 1 });

	int xEnd = start.x;
	int yEnd = start.y;
	while (!toVisit.empty()) {
		int currX = toVisit.front().first % width;
		int currY = toVisit.front().first / width;
		int depth = toVisit.front().second;
		visited[currY][currX] = depth;
		toVisit.pop();

		if (currX == xEnd && currY == yEnd) {
			Backtrack(xEnd, yEnd, width, height, visited, agent);
		}

		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				if (i + j != 1 || !grid.isValidCell(Vector2D(currX + i, currY + j)) || visited[currY + j][currX + i]) continue;
				toVisit.push({ (currX + i) + (currY + j) * width , depth + 1 });
			}
		}
	}
}