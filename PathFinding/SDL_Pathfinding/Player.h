#pragma once
#include "Agent.h"
#include "CurrentAlgorithm.h"
#include "TravelingSalesmanAlgorithm.h"
#include <vector>

class Player : public Agent
{
private:
    
    CurrentAlgorithm _currentAlgorithmTag;
    CurrentAlgorithm _nextAlgorithmTag;
	
	std::unique_ptr<PathFindingAlgorithm> _nextPathFindingAlgorithm;

    TravelingSalesmanAlgorithm _travelingSalesman;

    bool _recalculateOnChangingWeights;
    
    void OnTryToChangeAlgorithm(CurrentAlgorithm newAlgorithmTag, PathFindingAlgorithm* newPathFindingAlgorithm);
    void RepositionPlayer(SDL_Event* event, const Grid& layer);
    
public:
    std::vector<Vector2D> pathingPoints;
    
    void AddPathingPoint(Vector2D pos, const Grid& layer);
    int PathToPoints(const Grid& layer);
    Player(int numberOfEnemies);    
    void update(float dtime, SDL_Event* event, const Grid& layer) override;
    void draw() const override;
    
};
