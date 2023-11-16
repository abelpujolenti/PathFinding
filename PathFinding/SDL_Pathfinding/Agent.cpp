#include "Agent.h"

using namespace std;

Agent::Agent() : sprite_texture(0),
                 _position(Vector2D(100, 100)),
	             _target(Vector2D(1000, 100)),
	             _velocity(Vector2D(0,0)),
	             currentTargetIndex(-1),
				 mass(0.1f),
				 max_force(150),
				 max_velocity(200),
				 orientation(0),
				 sprite_num_frames(0),
	             sprite_w(0),
	             sprite_h(0),
	             draw_sprite(false)
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

void Agent::setTarget(Vector2D target)
{
	_target = target;
}

void Agent::setVelocity(Vector2D velocity)
{
	_velocity = velocity;
}

void Agent::update(float dtime, SDL_Event *event)
{

	//cout << "agent update:" << endl;

	switch (event->type) {
		/* Keyboard & Mouse events */
	case SDL_KEYDOWN:
		if (event->key.keysym.scancode == SDL_SCANCODE_SPACE)
			draw_sprite = !draw_sprite;
		break;
	default:
		break;
	}

	// Apply the steering behavior
	_steeringBehavior->applySteeringForce(this, dtime);
	
	// Update orientation
	if (_velocity.Length())
		orientation = (float)(atan2(_velocity.y, _velocity.x) * RAD2DEG);

	// Trim position values to window size
	if (_position.x < 0) _position.x = TheApp::Instance()->getWinSize().x;
	if (_position.y < 0) _position.y = TheApp::Instance()->getWinSize().y;
	if (_position.x > TheApp::Instance()->getWinSize().x) _position.x = 0;
	if (_position.y > TheApp::Instance()->getWinSize().y) _position.y = 0;
}


void Agent::addPathPoint(Vector2D point)
{
	if (path.points.size() > 0)
		if (path.points[path.points.size() - 1] == point)
			return;

	path.points.push_back(point);
}


int Agent::getCurrentTargetIndex()
{
	return currentTargetIndex;
}

int Agent::getPathSize() const
{
	return path.points.size();
}

Vector2D Agent::getPathPoint(int idx) const
{
	return path.points[idx];
}

void Agent::clearPath()
{
	setCurrentTargetIndex(-1);
	path.points.clear();
}

void Agent::setCurrentTargetIndex(int idx)
{
	currentTargetIndex = idx;
}

void Agent::draw() const
{
	// Path
	for (int i = 0; i < (int)path.points.size(); i++)
	{
		draw_circle(TheApp::Instance()->getRenderer(), (int)(path.points[i].x), (int)(path.points[i].y), 15, 255, 255, 0, 255);
		if (i > 0)
			SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), (int)(path.points[i - 1].x), (int)(path.points[i - 1].y), (int)(path.points[i].x), (int)(path.points[i].y));
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
		draw_circle(TheApp::Instance()->getRenderer(), (int)_position.x, (int)_position.y, 15, 255, 255, 255, 255);
		SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), (int)_position.x, (int)_position.y, (int)(_position.x+15*cos(orientation*DEG2RAD)), (int)(_position.y+15*sin(orientation*DEG2RAD)));
	}

	
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
