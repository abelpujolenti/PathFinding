#pragma once
#include "PathFindingAlgorithm.h"
#include <queue>

class AStarAlgorithm : public PathFindingAlgorithm
{
public:
    void CalculatePath(Vector2D start, Vector2D end, Grid& grid, Path& agentPath) override;
    float Distance(Vector2D start, Vector2D goal);
    float Heuristic(float g, float h, float a, float b);
};
