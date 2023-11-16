#pragma once
#include "Vector2D.h"
#include "Grid.h"
#include "Path.h"


class PathFindingAlgorithm
{
public:
    ~PathFindingAlgorithm() = default;
    virtual void CalculatePath(Vector2D start, Vector2D end, Grid& grid, Path& path) = 0;
};
