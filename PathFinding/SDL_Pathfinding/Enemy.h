#pragma once
#include "Agent.h"

#include <vector>

#define CENTER_WEIGHT 10

#define FIRST_RING_LENGTH 4
#define FIRST_RING_WEIGHT 1.6f

#define SECOND_RING_LENGTH 8
#define SECOND_RING_WEIGHT 1.4f

#define THIRD_RING_LENGTH 12
#define THIRD_RING_WEIGHT 1.2f

#define TOTAL_CELLS_TO_MODIFY 25

class Enemy : public Agent
{
private:

    std::shared_ptr<Grid> _enemyLayer;

    std::vector<Vector2D> _cellsModified;
    std::vector<float> _weightsAddToCells;
    
    Vector2D CalculateRandomPosition(const Grid& layer) const;
    bool DidReachDestination(const Grid& layer) const;
    void AddWeights(Vector2D position) const;
    void ModifyLayerWeights(Vector2D position);
    
public:
    Enemy(const std::shared_ptr<Grid>& enemyLayer);
    ~Enemy();
    void update(float dtime, SDL_Event* event, const Grid& layer) override;
    void draw() const override;
    void SetDestination(Vector2D destination);
    void SetCellsToModify();
    void SetWeights();
    void SetCurrentCell(Vector2D currentCell) override;
};
