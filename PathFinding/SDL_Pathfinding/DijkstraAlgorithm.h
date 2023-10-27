#pragma once
#include "PathFindingAlgorithm.h"

class DijkstraAlgorithm : public PathFindingAlgorithm
{
public:
    ~DijkstraAlgorithm() = default;
    void CalculatePath() override;
};
