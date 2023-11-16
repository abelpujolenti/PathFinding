#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
#include "Scene.h"
#include "PathFollowing.h"
#include "Grid.h"
#include "CurrentAlgorithm.h"
#include "PathFindingAlgorithm.h"
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

	std::unique_ptr<Grid> _maze;

	CurrentAlgorithm _currentAlgorithmTag;
	CurrentAlgorithm _nextAlgorithmTag;

	std::unique_ptr<PathFindingAlgorithm> _currentPathFindingAlgorithm;
	
	std::unique_ptr<PathFindingAlgorithm> _nextPathFindingAlgorithm;

	Vector2D coinPosition;

	bool draw_grid;

	bool loadTextures(char* filename_bg, char* filename_coin);
	
	void drawMaze() const;
	void drawCoin() const;
	void InitializeSceneComponents();
	void PlaceCoinInNewPosition();
	void OnTryToChangeAlgorithm(CurrentAlgorithm newAlgorithmTag, PathFindingAlgorithm* newPathFindingAlgorithm);
	void ChangeAlgorithmTag(CurrentAlgorithm newAlgorithm);
	void ChangePathFindingAlgorithm();
	bool DidPlayerTakeCoin() const;
	void RepositionPlayer(SDL_Event* event) const;
	
public:
	PathFindingScene();
	~PathFindingScene() override;
	void update(float dtime, SDL_Event *event) override;
	void draw() override;
	const char* getTitle() override;
};
