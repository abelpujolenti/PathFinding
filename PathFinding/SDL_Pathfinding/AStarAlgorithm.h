#pragma once
#include "PathFindingAlgorithm.h"
#include <queue>

class AStarAlgorithm : public PathFindingAlgorithm
{
public:
    ~AStarAlgorithm() = default;
    void CalculatePath(Vector2D start, Vector2D end, Grid& grid, Agent& agent) override;
    void Backtrack(int xEnd, int yEnd, int width, std::vector<std::vector<int>>& path, Grid& grid, Agent& agent);
    float Distance(Vector2D start, Vector2D goal);
    float Heuristic(float g, float h, float a, float b);
};
