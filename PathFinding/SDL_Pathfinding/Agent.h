#pragma once
#include <iostream>
#include <minmax.h>
#include <SDL.h>
#include <SDL_image.h>
#include "SDL_SimpleApp.h"
#include "Path.h"
#include "utils.h"


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
	
private:
	std::unique_ptr<SteeringBehavior> _steeringBehavior;
	Vector2D _position;
	Vector2D _velocity;
	Vector2D _target;

	// Pathfinding
	Path path;
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

public:
	Agent();
	~Agent();
	Vector2D getPosition() const;
	Vector2D getTarget() const;
	Vector2D getVelocity() const;
	Vector2D getPathPoint(int idx) const;
	int getPathSize() const;
	int getCurrentTargetIndex();
	float getMaxVelocity() const;
	float getMaxForce() const;
	float getMass() const;
	void setBehavior(SteeringBehavior* steeringBehavior);
	void setPosition(Vector2D position);
	void setTarget(Vector2D target);
	void setVelocity(Vector2D velocity);
	void setCurrentTargetIndex(int idx);
	void addPathPoint(Vector2D point);
	void clearPath();
	void update(float dtime, SDL_Event *event);
	void draw() const;
	bool loadSpriteTexture(char* filename, int num_frames=1);
	
};
