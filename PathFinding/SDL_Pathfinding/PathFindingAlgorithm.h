#pragma once
#include "Vector2D.h"
#include "Grid.h"
#include "Agent.h"

class PathFindingAlgorithm
{
public:
    ~PathFindingAlgorithm() = default;
    virtual void CalculatePath(Vector2D start, Vector2D end, Grid& grid, Agent& agent) = 0;
};
