#pragma once
#include "Agent.h"
#include "CurrentAlgorithm.h"

#include "PathFollowing.h"

class Player : public Agent
{
private:
    
    CurrentAlgorithm _currentAlgorithmTag;
    CurrentAlgorithm _nextAlgorithmTag;
	
	std::unique_ptr<PathFindingAlgorithm> _nextPathFindingAlgorithm;

    bool _recalculateOnChangingWeights;
    
    void OnTryToChangeAlgorithm(CurrentAlgorithm newAlgorithmTag, PathFindingAlgorithm* newPathFindingAlgorithm);
    void RepositionPlayer(SDL_Event* event, std::shared_ptr<Grid> maze);
    
public:
    Player(int numberOfEnemies, const std::shared_ptr<Grid>& enemyLayer);
    ~Player();
    void update(float dtime, SDL_Event* event, std::shared_ptr<Grid> maze) override;
    void draw() const override;
    
};
