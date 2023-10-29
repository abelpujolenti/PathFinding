#pragma once
#include "PathFindingAlgorithm.h"
#include "Agent.h"

class DijkstraAlgorithm : public PathFindingAlgorithm
{
public:
    ~DijkstraAlgorithm() = default;
    void CalculatePath(Vector2D start, Vector2D end, Grid& grid, Agent& agent) override;
};
