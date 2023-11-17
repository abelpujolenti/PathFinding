#pragma once
#include "PathFindingAlgorithm.h"
#include "Agent.h"
#include <queue>
#include <vector>

class DijkstraAlgorithm : public PathFindingAlgorithm
{
public:
    void CalculatePath(Vector2D start, Vector2D end, Grid& grid, Path& agentPath) override;
};
