#pragma once
#include "PathFindingAlgorithm.h"
#include <vector>
#include <queue>

class BreadthFirstSearchAlgorithm : public PathFindingAlgorithm
{
public:
    ~BreadthFirstSearchAlgorithm() = default;
    void CalculatePath(Vector2D start, Vector2D end, Grid& grid, Agent& agent) override;
};
