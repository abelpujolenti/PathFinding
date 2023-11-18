#include "PathFindingAlgorithm.h"

#include "Vector2D.h"
#include "Path.h"
#include "Grid.h"

void PathFindingAlgorithm::Backtrack(int xEnd, int yEnd, int width, std::vector<std::vector<int>>& path,
    const Grid& grid, Path& agentPath)
{
    while (path[yEnd][xEnd] > -1) {
        int curr = path[yEnd][xEnd];
        xEnd = curr % width;
        yEnd = curr / width;
        agentPath.addPathPoint(Vector2D(grid.cell2pix(Vector2D(xEnd, yEnd))), grid.GetCellWeight(Vector2D(xEnd, yEnd)));
    }
}