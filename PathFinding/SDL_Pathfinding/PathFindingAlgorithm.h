#pragma once
#include <vector>

class Vector2D;
class Grid;
class Path;

class PathFindingAlgorithm
{
protected:    
    void Backtrack(int xEnd, int yEnd, int width, std::vector<std::vector<int>>& path, const Grid& grid, Path& agentPath);
    
public:
    virtual int CalculatePath(Vector2D start, Vector2D end, const Grid& grid, Path& path) = 0;
};
