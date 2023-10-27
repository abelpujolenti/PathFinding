#pragma once

class PathFindingAlgorithm
{
public:
    ~PathFindingAlgorithm() = default;
    virtual void CalculatePath() = 0;
};
