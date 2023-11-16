#pragma once
#include "PathFindingAlgorithm.h"
#include <vector>
#include <queue>
#include <set>

class BreadthFirstSearchAlgorithm : public PathFindingAlgorithm
{
public:
    ~BreadthFirstSearchAlgorithm() = default;
    void CalculatePath(Vector2D start, Vector2D end, Grid& grid, Agent& agent) override;
    void Backtrack(int xEnd, int yEnd, int width, std::vector<std::vector<int>>& path, Grid& grid, Agent& agent);
};
