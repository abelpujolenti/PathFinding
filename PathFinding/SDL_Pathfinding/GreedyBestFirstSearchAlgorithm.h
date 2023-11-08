﻿#pragma once
#include "PathFindingAlgorithm.h"
#include <queue>

class GreedyBestFirstSearchAlgorithm : public PathFindingAlgorithm
{
public:
    ~GreedyBestFirstSearchAlgorithm() = default;
    void CalculatePath(Vector2D start, Vector2D end, Grid& grid, Agent& agent) override;
    void Backtrack(int xEnd, int yEnd, int width, std::vector<std::vector<int>>& path, Grid& grid, Agent& agent);
    float Distance(Vector2D start, Vector2D goal);
};
