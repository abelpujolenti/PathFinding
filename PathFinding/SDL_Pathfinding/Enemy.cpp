#include "Enemy.h"

#include "AStarAlgorithm.h"
#include "PathFollowing.h"
#include "Grid.h"
#include "Path.h"

Enemy::Enemy(const std::shared_ptr<Grid>& enemyLayer) : _enemyLayer(enemyLayer)
{    
    _currentPathFindingAlgorithm.reset(new AStarAlgorithm);
    redValueCircle = 255;
    greenValueCircle = 0;
    blueValueCircle = 0;
    loadSpriteTexture("../res/zombie1.png", 8);
    setBehavior(new PathFollowing);

    SetCellsToModify();
    SetWeights();    
}

Enemy::~Enemy()
{
}

void Enemy::update(float dtime, SDL_Event* event, const Grid& layer)
{
    Agent::update(dtime, event, layer);

    Vector2D position = layer.pix2cell(_position);

    if (_currentCell != position)
    {               
        ModifyLayerWeights(position);
        _currentCell = position;        
    }
    
    if (DidReachDestination(layer))
    {
        _destination = layer.cell2pix(CalculateRandomPosition(layer));
        LoadPath(layer);
    }
}

Vector2D Enemy::CalculateRandomPosition(const Grid& layer) const
{	
    Vector2D rand_cell;
	
    do
    {
        rand_cell = Vector2D((float)(rand() % layer.getNumCellX()), (float)(rand() % layer.getNumCellY()));
    } while (!layer.isValidCell(rand_cell));

    return rand_cell;
}

bool Enemy::DidReachDestination(const Grid& layer) const
{    
    return currentTargetIndex == -1 && layer.pix2cell(_position) == layer.pix2cell(_destination);
}

void Enemy::SetDestination(Vector2D destination)
{
    _destination = destination;
}

void Enemy::SetCurrentCell(Vector2D currentCell)
{
    Agent::SetCurrentCell(currentCell);
    
    AddWeights(_currentCell);
}

void Enemy::AddWeights(Vector2D position) const
{    
    for (int i = 0; i < _cellsModified.size(); ++i)
    {
        Vector2D cellDesired = position + _cellsModified[i];

        if (cellDesired.x >= 0 && cellDesired.x <= _enemyLayer->getNumCellX() &&
            cellDesired.y >= 0 && cellDesired.y <= _enemyLayer->getNumCellY())
        {
            const int currentCellWeight = _enemyLayer->GetCellWeight(cellDesired);
            
            if (currentCellWeight != 0)
            {
                _enemyLayer->ModifyCellWeight(cellDesired, _weightsAddToCells[i]);    
            }                
        }    
    }
}

void Enemy::ModifyLayerWeights(Vector2D position)
{
    for (int i = 0; i < _cellsModified.size(); ++i)
    {
        Vector2D cellDesired = _currentCell + _cellsModified[i];
        if (cellDesired.x > 0 && cellDesired.x < _enemyLayer->getNumCellX() &&
            cellDesired.y > 0 && cellDesired.y < _enemyLayer->getNumCellY())
        {
            const int currentCellWeight = _enemyLayer->GetCellWeight(cellDesired);
            
            if (currentCellWeight != 0)
            {
                _enemyLayer->ModifyCellWeight(cellDesired, -_weightsAddToCells[i]);    
            }                
        }       
    }

    AddWeights(position);
}

void Enemy::SetCellsToModify()
{
    _cellsModified.reserve(TOTAL_CELLS_TO_MODIFY);

    //THIRD RING
    _cellsModified.push_back(Vector2D(-3, 0));
    _cellsModified.push_back(Vector2D(-2, 1));
    _cellsModified.push_back(Vector2D(-1, 2));
    _cellsModified.push_back(Vector2D(0, 3));
    _cellsModified.push_back(Vector2D(1, 2));
    _cellsModified.push_back(Vector2D(2, 1));
    _cellsModified.push_back(Vector2D(3, 0));
    _cellsModified.push_back(Vector2D(2, -1));
    _cellsModified.push_back(Vector2D(1, -2));
    _cellsModified.push_back(Vector2D(0, -3));
    _cellsModified.push_back(Vector2D(-1, -2));
    _cellsModified.push_back(Vector2D(-2, -1));

    //SECOND RING
    _cellsModified.push_back(Vector2D(-2, 0));
    _cellsModified.push_back(Vector2D(-1, 1));
    _cellsModified.push_back(Vector2D(0, 2));
    _cellsModified.push_back(Vector2D(1, 1));
    _cellsModified.push_back(Vector2D(2, 0));
    _cellsModified.push_back(Vector2D(1, -1));
    _cellsModified.push_back(Vector2D(0, -2));
    _cellsModified.push_back(Vector2D(-1, -1));

    //FIRST RING
    _cellsModified.push_back(Vector2D(-1, 0));
    _cellsModified.push_back(Vector2D(0, 1));
    _cellsModified.push_back(Vector2D(1, 0));
    _cellsModified.push_back(Vector2D(0, -1));

    //CENTER
    _cellsModified.push_back(Vector2D(0, 0));
    
}

void Enemy::SetWeights()
{
    _weightsAddToCells.reserve(TOTAL_CELLS_TO_MODIFY);
    
    for (int i = 0; i < THIRD_RING_LENGTH; ++i)
    {
        _weightsAddToCells.push_back(THIRD_RING_WEIGHT);
    }
    
    for (int i = 0; i < SECOND_RING_LENGTH; ++i)
    {
        _weightsAddToCells.push_back(SECOND_RING_WEIGHT);
    }

    for (int i = 0; i < FIRST_RING_LENGTH; ++i)
    {
        _weightsAddToCells.push_back(FIRST_RING_WEIGHT);
    }

    _weightsAddToCells.push_back(CENTER_WEIGHT);
}

void Enemy::draw() const
{
    
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
