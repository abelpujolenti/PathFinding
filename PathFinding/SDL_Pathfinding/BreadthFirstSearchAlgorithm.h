#pragma once
#include "PathFindingAlgorithm.h"
#include <vector>
#include <queue>
#include <set>

class BreadthFirstSearchAlgorithm : public PathFindingAlgorithm
{
public:
    void CalculatePath(Vector2D start, Vector2D end, Grid& grid, Path& agentPath) override;
};
