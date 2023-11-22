#pragma once
#include "PathFindingAlgorithm.h"

class BreadthFirstSearchAlgorithm : public PathFindingAlgorithm
{
public:
    int CalculatePath(Vector2D start, Vector2D end, const Grid& grid, Path& agentPath) override;
};
