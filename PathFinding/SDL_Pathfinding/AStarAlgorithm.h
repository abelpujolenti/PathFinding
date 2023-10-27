#pragma once
#include "PathFindingAlgorithm.h"

class AStarAlgorithm : public PathFindingAlgorithm
{
public:
    ~AStarAlgorithm() = default;
    void CalculatePath() override;
};
