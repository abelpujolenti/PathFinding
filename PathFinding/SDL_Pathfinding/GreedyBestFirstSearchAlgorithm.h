#pragma once
#include "PathFindingAlgorithm.h"

class GreedyBestFirstSearchAlgorithm : public PathFindingAlgorithm
{
public:
    ~GreedyBestFirstSearchAlgorithm() = default;
    void CalculatePath(Vector2D start, Vector2D end, Grid& grid, Agent& agent) override;
};
