#pragma once
#include "Agent.h"

class Enemy : public Agent
{
private:

    Vector2D _destination;
    
public:
    Enemy();
    ~Enemy();
    void SetDestination(Vector2D destination);
    Vector2D GetDestination() const;
    
};
