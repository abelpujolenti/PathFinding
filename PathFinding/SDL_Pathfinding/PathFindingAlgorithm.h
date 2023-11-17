#pragma once
#include "Grid.h"
#include "Path.h"


class PathFindingAlgorithm
{
protected:    
    void Backtrack(int xEnd, int yEnd, int width, std::vector<std::vector<int>>& path, Grid& grid, Path& agentPath);
    
public:
    virtual void CalculatePath(Vector2D start, Vector2D end, Grid& grid, Path& path) = 0;
};

inline void PathFindingAlgorithm::Backtrack(int xEnd, int yEnd, int width, std::vector<std::vector<int>>& path,
    Grid& grid, Path& agentPath)
{
    while (path[yEnd][xEnd] > -1) {
        int curr = path[yEnd][xEnd];
        xEnd = curr % width;
        yEnd = curr / width;
        agentPath.addPathPoint(Vector2D(grid.cell2pix(Vector2D(xEnd, yEnd))));
    }
}
