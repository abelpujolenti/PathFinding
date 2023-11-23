#pragma once
#include "Agent.h"
#include "CurrentAlgorithm.h"
#include <vector>

class Player : public Agent
{
private:
    
    CurrentAlgorithm _currentAlgorithmTag;
    CurrentAlgorithm _nextAlgorithmTag;
	
	std::unique_ptr<PathFindingAlgorithm> _nextPathFindingAlgorithm;

    bool _recalculateOnChangingWeights;

    std::vector<Vector2D> _pathingPoints;
    
    void OnTryToChangeAlgorithm(CurrentAlgorithm newAlgorithmTag, PathFindingAlgorithm* newPathFindingAlgorithm);
    void RepositionPlayer(SDL_Event* event, const Grid& layer);
    
public:
    void AddPathingPoint(Vector2D pos, const Grid& layer);
    int PathToPoints(const Grid& layer);
    Player(int numberOfEnemies);    
    void update(float dtime, SDL_Event* event, const Grid& layer) override;
    void draw() const override;
    
};
