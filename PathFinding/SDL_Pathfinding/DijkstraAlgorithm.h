﻿#pragma once
#include "PathFindingAlgorithm.h"

class DijkstraAlgorithm : public PathFindingAlgorithm
{
public:
    void CalculatePath(Vector2D start, Vector2D end, const Grid& grid, Path& agentPath) override;
};
