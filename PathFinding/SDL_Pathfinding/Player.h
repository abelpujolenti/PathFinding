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
    
public:
    Player();
    ~Player();
    void AgentUpdate(float dtime, SDL_Event* event, std::shared_ptr<Grid> maze) override;
    void OnTryToChangeAlgorithm(CurrentAlgorithm newAlgorithmTag, PathFindingAlgorithm* newPathFindingAlgorithm);
    void RepositionPlayer(SDL_Event* event, std::shared_ptr<Grid> maze);
    void draw() const override;
};
