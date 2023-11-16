#pragma once
#include "Agent.h"

#include "PathFollowing.h"

class Enemy : public Agent
{
private:

    Vector2D _destination;
    
public:
    Enemy();
    ~Enemy();
    void AgentUpdate(float dtime, SDL_Event* event, std::shared_ptr<Grid> maze) override;
    bool DidEnemyReachDestination(std::shared_ptr<Grid> maze) const;
    Vector2D CalculateRandomPosition(std::shared_ptr<Grid> maze) const;
    void SetDestination(Vector2D destination);
    Vector2D GetDestination() const;
    void draw() const override;
};
