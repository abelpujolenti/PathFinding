#include "Enemy.h"

Enemy::Enemy()
{
    redValueCircle = 255;
    greenValueCircle = 0;
    blueValueCircle = 0;
}

Enemy::~Enemy()
{
}

void Enemy::SetDestination(Vector2D destination)
{
    _destination = destination;
}

Vector2D Enemy::GetDestination() const
{
    return _destination;
}
