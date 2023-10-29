#pragma once
#include "PathFindingAlgorithm.h"

class AStarAlgorithm : public PathFindingAlgorithm
{
public:
    ~AStarAlgorithm() = default;
    void CalculatePath(Vector2D start, Vector2D end, Grid& grid, Agent& agent) override;
};
