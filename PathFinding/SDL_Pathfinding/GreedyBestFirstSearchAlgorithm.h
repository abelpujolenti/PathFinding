#pragma once
#include "PathFindingAlgorithm.h"

class GreedyBestFirstSearchAlgorithm : public PathFindingAlgorithm
{
public:
    ~GreedyBestFirstSearchAlgorithm() = default;
    void CalculatePath() override;
};
