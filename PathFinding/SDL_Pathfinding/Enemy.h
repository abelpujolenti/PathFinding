#pragma once
#include "Agent.h"

#include "PathFollowing.h"

class Enemy : public Agent
{
private:

    std::vector<std::vector<int>> _normalLayer;
    
    Vector2D _destination;
    
    Vector2D CalculateRandomPosition(std::shared_ptr<Grid> maze) const;
    bool DidReachDestination(std::shared_ptr<Grid> maze) const;
    
public:
    Enemy(Grid maze);
    ~Enemy();
    void update(float dtime, SDL_Event* event, std::shared_ptr<Grid> maze) override;
    void draw() const override;
    void SetDestination(Vector2D destination);    
};
