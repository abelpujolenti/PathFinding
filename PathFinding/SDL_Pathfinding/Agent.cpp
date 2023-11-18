#include "Agent.h"
#include "Path.h"
#include "Grid.h"
#include "PathFindingAlgorithm.h"

using namespace std;

Agent::Agent(): _path(std::make_unique<Path>()),
				currentTargetIndex(-1),
				mass(0.1f),
				orientation(0),
				max_force(150),
				max_velocity(200),
				sprite_texture(0),
				draw_sprite(false),
				sprite_num_frames(0),
				sprite_w(0),
				sprite_h(0),
				redValueCircle(0),
				greenValueCircle(0),
				blueValueCircle(0)
{
}

Agent::~Agent()
{
	if (sprite_texture)
		SDL_DestroyTexture(sprite_texture);
}

void Agent::setBehavior(SteeringBehavior* steeringBehavior)
{
	_steeringBehavior.reset(steeringBehavior);
}

Vector2D Agent::getPosition() const
{
	return _position;
}

Vector2D Agent::getTarget() const
{
	return _target;
}

Vector2D Agent::getVelocity() const
{
	return _velocity;
}

float Agent::getMaxVelocity() const
{
	return max_velocity;
}

float Agent::getMaxForce() const
{
	return max_force;
}

float Agent::getMass() const
{
	return mass;
}

void Agent::setPosition(Vector2D position)
{
	_position = position;
}

void Agent::SetCurrentCell(Vector2D currentCell)
{
	_currentCell = currentCell;
}

void Agent::setTarget(Vector2D target)
{
	_target = target;
}

void Agent::setVelocity(Vector2D velocity)
{
	_velocity = velocity;
}

int Agent::getCurrentTargetIndex() const
{
	return currentTargetIndex;
}

int Agent::getPathSize() const
{
	return _path->points.size();
}

Vector2D Agent::getPathPoint(int idx) const
{
	return _path->points[idx];
}

void Agent::clearPath()
{
	setCurrentTargetIndex(-1);
	_path->points.clear();
	_path->weights.clear();
}

void Agent::update(float dtime, SDL_Event* event, const Grid& layer)
{
	_currentCell = layer.pix2cell(_position);
	
	switch (event->type) {
		/* Keyboard & Mouse events */
		case SDL_KEYDOWN:
			if (event->key.keysym.scancode == SDL_SCANCODE_SPACE)
			{
				draw_sprite = !draw_sprite;
			}			
		break;
	default:
		break;
	}

	// Apply the steering behavior
	_steeringBehavior->applySteeringForce(this, dtime);
	
	// Update orientation
	if (_velocity.Length())
	{
		orientation = atan2(_velocity.y, _velocity.x) * RAD2DEG;
	}
		

	// Trim position values to window size
	if (_position.x < 0)
	{
		_position.x = TheApp::Instance()->getWinSize().x;
	}
	if (_position.y < 0)
	{
		_position.y = TheApp::Instance()->getWinSize().y;
	}
	if (_position.x > TheApp::Instance()->getWinSize().x)
	{
		_position.x = 0;
	}
	if (_position.y > TheApp::Instance()->getWinSize().y)
	{
		_position.y = 0;
	}
}

void Agent::setCurrentTargetIndex(int idx)
{
	currentTargetIndex = idx;
}

bool Agent::loadSpriteTexture(char* filename, int _num_frames)
{
	if (_num_frames < 1) return false;

	SDL_Surface *image = IMG_Load(filename);
	if (!image) {
		cout << "IMG_Load: " << IMG_GetError() << endl;
		return false;
	}
	sprite_texture = SDL_CreateTextureFromSurface(TheApp::Instance()->getRenderer(), image);

	sprite_num_frames = _num_frames;
	sprite_w = image->w / sprite_num_frames;
	sprite_h = image->h;
	draw_sprite = true;

	if (image)
		SDL_FreeSurface(image);

	return true;
}

void Agent::LoadPath(const Grid& layer) const
{
	_currentPathFindingAlgorithm->CalculatePath(
				layer.pix2cell(_position),
				layer.pix2cell(_destination),
				layer,
				*_path);
}
