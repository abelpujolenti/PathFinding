#include "Player.h"

Player::Player(int numberOfEnemies, Grid maze) : _recalculateOnChangingWeights(numberOfEnemies != 0)
{    
    _currentPathFindingAlgorithm.reset(new BreadthFirstSearchAlgorithm);
    redValueCircle = 255;
    greenValueCircle = 255;
    blueValueCircle = 255;
    loadSpriteTexture("../res/soldier.png", 4);
    setBehavior(new PathFollowing);
    
}

Player::~Player()
{
}

void Player::update(float dtime, SDL_Event* event, std::shared_ptr<Grid> maze)
{
    Move(dtime, event);
    
    switch (event->type) {
    case SDL_KEYDOWN:
        if (event->key.keysym.scancode == SDL_SCANCODE_1)
        {
            OnTryToChangeAlgorithm(CurrentAlgorithm::BFS, new BreadthFirstSearchAlgorithm);
        }		
        if (event->key.keysym.scancode == SDL_SCANCODE_2)
        {
            OnTryToChangeAlgorithm(CurrentAlgorithm::Dijkstra, new DijkstraAlgorithm);
        }		
        if (event->key.keysym.scancode == SDL_SCANCODE_3)
        {
            OnTryToChangeAlgorithm(CurrentAlgorithm::Greedy, new GreedyBestFirstSearchAlgorithm);
        }		
        if (event->key.keysym.scancode == SDL_SCANCODE_4)
        {
            OnTryToChangeAlgorithm(CurrentAlgorithm::A, new AStarAlgorithm);
        }
        break;
    case SDL_MOUSEBUTTONDOWN:
        if (event->button.button == SDL_BUTTON_LEFT)
        {
            if (currentTargetIndex == -1)
            {
                RepositionPlayer(event, maze);				
            }
        }
        if (event->button.button == SDL_BUTTON_RIGHT)
        {
            //////////////////////HERE/////////////////////////

            //Call Algorithm function to calculate new path points
            //Remember, function Add Path Point from Agent receive only one Vector2D

            //////////////////////////////////////////////////
			
            _currentPathFindingAlgorithm->CalculatePath(
                maze->pix2cell(_position),
                maze->pix2cell(Vector2D((float)(event->button.x), (float)(event->button.y))),
                *maze,
                *_path
            );
        }
        break;
    default:
        break;
    }

    if (!_recalculateOnChangingWeights)
    {
        return;
    }
    
}

void Player::OnTryToChangeAlgorithm(CurrentAlgorithm newAlgorithmTag, PathFindingAlgorithm* newPathFindingAlgorithm)
{
    if (_currentAlgorithmTag == newAlgorithmTag)
    {
        return;
    }
	
    if (currentTargetIndex == -1)
    {
        _currentAlgorithmTag = newAlgorithmTag;
        _currentPathFindingAlgorithm.reset(newPathFindingAlgorithm);
    }
    else
    {
        _nextAlgorithmTag = newAlgorithmTag;
        _nextPathFindingAlgorithm.reset(newPathFindingAlgorithm);
    }
}

void Player::RepositionPlayer(SDL_Event* event, std::shared_ptr<Grid> maze)
{	
    Vector2D cell = maze->pix2cell(Vector2D((float)(event->button.x), (float)(event->button.y)));
    if (maze->isValidCell(cell)) {
        clearPath();
        setPosition(maze->cell2pix(cell));
    }
}

void Player::draw() const
{
    // Path
    for (int i = 0; i < (int)_path->points.size(); i++)
    {
        draw_circle(TheApp::Instance()->getRenderer(), (int)(_path->points[i].x), (int)(_path->points[i].y), 15, 255, 255, 0, 255);
        if (i > 0)
            SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), (int)(_path->points[i - 1].x), (int)(_path->points[i - 1].y), (int)(_path->points[i].x), (int)(_path->points[i].y));
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
