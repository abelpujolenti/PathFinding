#pragma once
#include <iostream>
#include <minmax.h>
#include <SDL.h>
#include <SDL_image.h>

#include "CurrentAlgorithm.h"
#include "SDL_SimpleApp.h"
#include "utils.h"

class PathFindingAlgorithm;
class Path;
class Grid;

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

	std::unique_ptr<PathFindingAlgorithm> _currentPathFindingAlgorithm;
	
	std::unique_ptr<SteeringBehavior> _steeringBehavior;

	Vector2D _currentCell;
	Vector2D _destination;
	
	Vector2D _position;
	Vector2D _velocity;
	Vector2D _target;

	// Pathfinding
	std::unique_ptr<Path> _path;
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
	int LoadPath(const Grid& layer) const;
	
public:
	Agent();
	virtual ~Agent();
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
	virtual void SetCurrentCell(Vector2D currentCell);
	void setTarget(Vector2D target);
	void setVelocity(Vector2D velocity);
	void setCurrentTargetIndex(int idx);
	void clearPath();
	virtual void update(float dtime, SDL_Event *event, const Grid& layer);
	virtual void draw() const = 0;

	
	int num;
	
	
};
