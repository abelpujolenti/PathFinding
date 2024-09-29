#include "TravelingSalesmanAlgorithm.h"

#include <queue>
#include "Vector2D.h"
#include "Grid.h"
#include "Path.h"
#include <algorithm>

float TravelingSalesmanAlgorithm::Distance(Vector2D start, Vector2D goal)
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

float TravelingSalesmanAlgorithm::CalculatePathingPointPriority(Vector2D current, Vector2D furthestToEnd, Vector2D pathPoint, Vector2D end)
{
	float ret = Distance(end, furthestToEnd) + Distance(furthestToEnd, current) + Distance(pathPoint, current);
	return ret;
}

float TravelingSalesmanAlgorithm::Heuristic(float g, float h, float a, float b)
{
	return g * a + h * b;
}

void TravelingSalesmanAlgorithm::Backtrack(int xEnd, int yEnd, int width, std::vector<std::vector<int>>& path, const Grid& grid, Path& agentPath)
{
	while (path[yEnd][xEnd] > -1) {
		int curr = path[yEnd][xEnd];
		xEnd = curr % width;
		yEnd = curr / width;
		agentPath.addPathPoint(Vector2D(grid.cell2pix(Vector2D(xEnd, yEnd))), grid.GetCellWeight(Vector2D(xEnd, yEnd)));
	}
}

void TravelingSalesmanAlgorithm::CalculatePathThroughPoints(Vector2D start, std::vector<Vector2D>& path, const Grid& grid, Path& agentPath)
{
	std::vector<Vector2D> pathOrdered(CalculateGreedyPathOrder(start, path));
	for (int i = 1; i < pathOrdered.size(); i++) {
		Vector2D first = grid.pix2cell(pathOrdered[i - 1]);
		Vector2D last = grid.pix2cell(pathOrdered[i]);
		CalculateAStar(first, last, grid, agentPath);
	}
}

std::vector<Vector2D> TravelingSalesmanAlgorithm::CalculateGreedyPathOrder(Vector2D start, std::vector<Vector2D>& path)
{
	std::vector<Vector2D> ret;
	ret.push_back(start);

	std::vector<std::pair<float, int >> distanceFromEnd;
	std::vector<std::pair<float, int >> distanceFromStart;
	for (int i = 0; i < path.size(); i++) {
		distanceFromStart.push_back(std::make_pair(Distance(path[i], start), i));
	}
	std::sort(distanceFromStart.begin(), distanceFromStart.end());
	Vector2D end = path[distanceFromStart.back().second];
	for (int i = 0; i < path.size(); i++) {
		distanceFromEnd.push_back(std::make_pair(Distance(path[i], end), i));
	}
	std::sort(distanceFromEnd.begin(), distanceFromEnd.end());

	std::vector<Vector2D> tempPath;
	for (int i = 0; i < distanceFromEnd.size(); i++) {
		tempPath.push_back(path[distanceFromEnd[distanceFromEnd.size() - 1 - i].second]);
	}

	while (!tempPath.empty()) {
		int nextNode = 0;
		float minPriority = -1;
		for (int i = 0; i < tempPath.size(); i++) {
			float priority = CalculatePathingPointPriority(tempPath[i], tempPath.front(), ret.back(), end);
			if (priority < minPriority || minPriority == -1) {
				minPriority = priority;
				nextNode = i;
			}
		}
		ret.push_back(tempPath[nextNode]);
		tempPath.erase(tempPath.begin() + nextNode);
	}

	return ret;
}

int TravelingSalesmanAlgorithm::CalculateAStar(Vector2D start, Vector2D end, const Grid& grid, Path& agentPath)
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
				float distance = Distance(Vector2D(neighborX, neighborY), Vector2D(xEnd, yEnd));
				frontier.push(std::make_pair(Heuristic(cost, distance, 0.5f, 0.5f), neighborX + neighborY * width));
				path[neighborY][neighborX] = curr;
				visitedNodes++;
			}
		}
	}
	return visitedNodes;
}