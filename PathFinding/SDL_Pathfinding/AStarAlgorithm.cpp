#include "AStarAlgorithm.h"

void AStarAlgorithm::CalculatePath(Vector2D start, Vector2D end, Grid& grid, Agent& agent)
{
}

void Astar(Vector2D start, Vector2D goal) {
	//std::priority_queue<std::pair<Vector2D, int>, std::vector<std::pair<Vector2D, int>>, std::greater<std::pair<Vector2D, int>>> openSet;
	//openSet.push({ start , 0});
	//std::map<Vector2D, int> cameFrom;
	//std::map<Vector2D, int> gScore;
	//gScore[start] = 0;
	//std::map<Vector2D, int> fScore;
	//fScore[start] = h(start);

	//while (!openSet.empty()) {
	//	Vector2D current; // = lowest fscore element in openset
	//}
}

int h(Vector2D v) {
	return 1;
}