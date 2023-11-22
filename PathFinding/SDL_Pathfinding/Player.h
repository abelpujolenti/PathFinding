#pragma once
#include "Agent.h"
#include "CurrentAlgorithm.h"

class Player : public Agent
{
private:
    
    CurrentAlgorithm _currentAlgorithmTag;
    CurrentAlgorithm _nextAlgorithmTag;
	
	std::unique_ptr<PathFindingAlgorithm> _nextPathFindingAlgorithm;

    bool _recalculateOnChangingWeights;
    
    void OnTryToChangeAlgorithm(CurrentAlgorithm newAlgorithmTag, PathFindingAlgorithm* newPathFindingAlgorithm);
    void RepositionPlayer(SDL_Event* event, const Grid& layer);
    
public:
    int PathTowardsPosition(Vector2D position, const Grid& layer);
    Player(int numberOfEnemies);    
    void update(float dtime, SDL_Event* event, const Grid& layer) override;
    void draw() const override;
    
};
