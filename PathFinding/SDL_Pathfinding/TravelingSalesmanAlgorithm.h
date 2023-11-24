#pragma once
#include "Vector2D.h"
#include "Grid.h"
#include "Path.h"

class TravelingSalesmanAlgorithm
{
private:
    int CalculateAStar(Vector2D start, Vector2D end, const Grid& grid, Path& agentPath);
    std::vector<Vector2D> CalculateGreedyPathOrder(Vector2D start, std::vector<Vector2D>& path);
    void Backtrack(int xEnd, int yEnd, int width, std::vector<std::vector<int>>& path, const Grid& grid, Path& agentPath);
    float Distance(Vector2D start, Vector2D goal);
    float CalculatePathingPointPriority(Vector2D current, Vector2D furthestToEnd, Vector2D start, Vector2D end);
    float Heuristic(float g, float h, float a, float b);
public:
    void CalculatePathThroughPoints(Vector2D start, std::vector<Vector2D>& path, const Grid& grid, Path& agentPath);
};
