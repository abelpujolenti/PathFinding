#include "BreadthFirstSearchAlgorithm.h"

void BreadthFirstSearchAlgorithm::Backtrack(int xEnd, int yEnd, int width, std::vector<std::vector<int>>& path, Grid& grid, Agent& agent) {
	while (path[yEnd][xEnd] > -1) {
		int curr = path[yEnd][xEnd];
		xEnd = curr % width;
		yEnd = curr / width;
		agent.addPathPoint(Vector2D(grid.cell2pix(Vector2D(xEnd, yEnd))));
	}
}

void BreadthFirstSearchAlgorithm::CalculatePath(Vector2D start, Vector2D end, Grid& grid, Agent& agent)
{
	int width = grid.getNumCellX();
	int height = grid.getNumCellY();
	std::vector<std::vector<int>> path(height, std::vector<int>(width, -2));
	std::queue<int> toVisit;
	toVisit.push((int)end.x + (int)end.y * width);
	path[end.y][end.x] = -1;

	int xEnd = start.x;
	int yEnd = start.y;

	while (!toVisit.empty()) {
		int curr = toVisit.front();
		int currX = curr % width;
		int currY = curr / width;
		toVisit.pop();

		if (currX == xEnd && currY == yEnd) {
			while (path[yEnd][xEnd] > -1) {
				int curr = path[yEnd][xEnd];
				xEnd = curr % width;
				yEnd = curr / width;
				agent.addPathPoint(Vector2D(grid.cell2pix(Vector2D(xEnd, yEnd))));
			}
			break;
		}

		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				int neighborX = currX + i;
				int neighborY = currY + j;
				if (abs(i + j) != 1 || !grid.isValidCell(Vector2D(neighborX, neighborY)) || path[neighborY][neighborX] != -2) continue;
				toVisit.push(neighborX + neighborY * width);
				path[neighborY][neighborX] = curr;
			}
		}
	}
}