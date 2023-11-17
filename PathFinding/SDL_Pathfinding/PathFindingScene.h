#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
#include "Scene.h"
#include "PathFollowing.h"
#include "Grid.h"
#include "Enemy.h"
#include "Player.h"

#define NUMBER_ENEMIES 5

class PathFindingScene : public Scene
{	
private:
	
	SDL_Texture *background_texture;
	SDL_Texture *coin_texture;
	
	std::shared_ptr<Player> _player;

	std::vector<std::shared_ptr<Enemy>> _enemies;

	std::shared_ptr<Grid> _maze;

	Vector2D coinPosition;

	bool draw_grid;

	bool loadTextures(char* filename_bg, char* filename_coin);
	
	void drawMaze() const;
	void drawCoin() const;
	void InitializeSceneComponents();
	void PlaceCoinInNewPosition();
	bool DidPlayerTakeCoin() const;
	Vector2D CalculateRandomPosition() const;
	
public:
	PathFindingScene();
	~PathFindingScene() override;
	void update(float dtime, SDL_Event *event) override;
	void draw() override;
	const char* getTitle() override;
};
