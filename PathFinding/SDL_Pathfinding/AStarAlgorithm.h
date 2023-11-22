#pragma once
#include "PathFindingAlgorithm.h"

class AStarAlgorithm : public PathFindingAlgorithm
{
public:
    int CalculatePath(Vector2D start, Vector2D end, const Grid& grid, Path& agentPath) override;
    float Distance(Vector2D start, Vector2D goal);
    float Heuristic(float g, float h, float a, float b);
};
