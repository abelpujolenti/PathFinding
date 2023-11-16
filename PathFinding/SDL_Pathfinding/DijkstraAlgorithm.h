#pragma once
#include "PathFindingAlgorithm.h"
#include "Agent.h"
#include <queue>
#include <vector>

class DijkstraAlgorithm : public PathFindingAlgorithm
{
public:
    ~DijkstraAlgorithm() = default;
    void CalculatePath(Vector2D start, Vector2D end, Grid& grid, Path& agentPath) override;
    void Backtrack(int xEnd, int yEnd, int width, std::vector<std::vector<int>>& path, Grid& grid, Path& agentPath);
};
