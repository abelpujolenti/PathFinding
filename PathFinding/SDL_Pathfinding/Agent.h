#pragma once
#include <iostream>
#include <minmax.h>
#include <SDL.h>
#include <SDL_image.h>

#include "SDL_SimpleApp.h"
#include "utils.h"
#include "AStarAlgorithm.h"
#include "BreadthFirstSearchAlgorithm.h"
#include "DijkstraAlgorithm.h"
#include "GreedyBestFirstSearchAlgorithm.h"


class Agent
{
	
public:
	class SteeringBehavior
	{
	public:
		SteeringBehavior() {};
		virtual ~SteeringBehavior() {};
		virtual void applySteeringForce(Agent *agent, float dtime) {};
	};
	
protected:

	std::shared_ptr<Grid> _enemyLayer;

	std::unique_ptr<PathFindingAlgorithm> _currentPathFindingAlgorithm;
	
	std::unique_ptr<SteeringBehavior> _steeringBehavior;
	
	Vector2D _position;
	Vector2D _velocity;
	Vector2D _target;

	// Pathfinding
	std::shared_ptr<Path> _path;
	int currentTargetIndex;

	float mass;
	float orientation;
	float max_force;
	float max_velocity;

	SDL_Texture *sprite_texture;
	bool draw_sprite;
	int sprite_num_frames;
	int sprite_w;
	int sprite_h;
	int redValueCircle;
	int greenValueCircle;
	int blueValueCircle;
	
	void setBehavior(SteeringBehavior* steeringBehavior);
	bool loadSpriteTexture(char* filename, int num_frames=1);
	void Move(float dtime, SDL_Event *event);
	
public:
	Agent(const std::shared_ptr<Grid>& enemyLayer);
	~Agent();
	Vector2D getPosition() const;
	Vector2D getTarget() const;
	Vector2D getVelocity() const;
	Vector2D getPathPoint(int idx) const;
	int getPathSize() const;
	int getCurrentTargetIndex() const;
	float getMaxVelocity() const;
	float getMaxForce() const;
	float getMass() const;
	void setPosition(Vector2D position);
	void setTarget(Vector2D target);
	void setVelocity(Vector2D velocity);
	void setCurrentTargetIndex(int idx);
	void clearPath();
	virtual void update(float dtime, SDL_Event *event, std::shared_ptr<Grid> maze) = 0;
	virtual void draw() const = 0;
	
};
