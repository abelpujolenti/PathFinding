#pragma once
#include "PathFindingAlgorithm.h"

class BreadthFirstSearchAlgorithm : public PathFindingAlgorithm
{
public:
    ~BreadthFirstSearchAlgorithm() = default;
    void CalculatePath() override;
};
