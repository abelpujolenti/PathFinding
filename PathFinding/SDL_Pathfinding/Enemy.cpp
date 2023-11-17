#include "Enemy.h"

Enemy::Enemy()
{
    _currentPathFindingAlgorithm.reset(new AStarAlgorithm);
    redValueCircle = 255;
    greenValueCircle = 0;
    blueValueCircle = 0;
    loadSpriteTexture("../res/zombie1.png", 8);
    setBehavior(new PathFollowing);
}

Enemy::~Enemy()
{
}

void Enemy::AgentUpdate(float dtime, SDL_Event* event, std::shared_ptr<Grid> maze)
{
    update(dtime, event);
    
    std::cout << DidEnemyReachDestination(maze) << std::endl;

    if (DidEnemyReachDestination(maze))
    {        
        _destination = maze->cell2pix(CalculateRandomPosition(maze));
        _currentPathFindingAlgorithm->CalculatePath(
            maze->pix2cell(_position),
            maze->pix2cell(_destination),
            *maze,
            *path);
    }
}

Vector2D Enemy::CalculateRandomPosition(std::shared_ptr<Grid> maze) const
{	
    Vector2D rand_cell;
	
    do
    {
        rand_cell = Vector2D((float)(rand() % maze->getNumCellX()), (float)(rand() % maze->getNumCellY()));
    } while (!maze->isValidCell(rand_cell));

    return rand_cell;
}

bool Enemy::DidEnemyReachDestination(std::shared_ptr<Grid> maze) const
{    
    return currentTargetIndex == -1 && maze->pix2cell(_position) == maze->pix2cell(_destination);
}

void Enemy::SetDestination(Vector2D destination)
{
    _destination = destination;
}

Vector2D Enemy::GetDestination() const
{
    return _destination;
}

void Enemy::draw() const
{
    // Path
    for (int i = 0; i < (int)path->points.size(); i++)
    {
        draw_circle(TheApp::Instance()->getRenderer(), (int)(path->points[i].x), (int)(path->points[i].y), 15, 255, 255, 0, 255);
        if (i > 0)
            SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), (int)(path->points[i - 1].x), (int)(path->points[i - 1].y), (int)(path->points[i].x), (int)(path->points[i].y));
    }
    
    if (draw_sprite)
    {
        Uint32 sprite;
		
        if (_velocity.Length() < 5.0)
            sprite = 1;
        else
            sprite = (int)(SDL_GetTicks() / (-0.1*_velocity.Length() + 250)) % sprite_num_frames;
		
        SDL_Rect srcrect = { (int)sprite * sprite_w, 0, sprite_w, sprite_h };
        SDL_Rect dstrect = { (int)_position.x - (sprite_w / 2), (int)_position.y - (sprite_h / 2), sprite_w, sprite_h };
        SDL_Point center = { sprite_w / 2, sprite_h / 2 };
        SDL_RenderCopyEx(TheApp::Instance()->getRenderer(), sprite_texture, &srcrect, &dstrect, orientation+90, &center, SDL_FLIP_NONE);
    }
    else 
    {
        draw_circle(TheApp::Instance()->getRenderer(), (int)_position.x, (int)_position.y, 15,
            redValueCircle, greenValueCircle, blueValueCircle, 255);
        SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), (int)_position.x, (int)_position.y,
            (int)(_position.x+15*cos(orientation*DEG2RAD)), (int)(_position.y+15*sin(orientation*DEG2RAD)));
    }
}
