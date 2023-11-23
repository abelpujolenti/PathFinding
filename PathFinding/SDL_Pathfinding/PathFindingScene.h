#pragma once
#include <memory>
#include <vector>
#include "Scene.h"
#include "Vector2D.h"


#define NUMBER_ENEMIES 0
#define PATH_MAZE_CSV "../res/maze.csv"
#define PATH_MAZE_PNG "../res/maze.png"
#define PATH_COIN "../res/coin.png"

class Grid;
class Player;
class Enemy;
class PathFollowing;

class PathFindingScene : public Scene
{	
private:
	
	SDL_Texture *background_texture;
	SDL_Texture *coin_texture;
	
	std::unique_ptr<Player> _player;

	std::vector<std::unique_ptr<Enemy>> _enemies;

	std::unique_ptr<Grid> _normalLayer;
	std::shared_ptr<Grid> _enemyLayer;

	Vector2D coinPosition;
	std::vector<Vector2D> predefinedCoinSpots;
	Vector2D predefinedPlayerStart;
	int predefinedCoinSpotIndex;
	std::vector<int> visitedNodeInstances;

	bool autoMode = false;

	bool draw_grid;

	bool loadTextures(const char* filename_bg, const char* filename_coin);
	
	void drawMaze() const;
	void drawCoin() const;
	void InitializeSceneComponents();
	void ResetEnemyLayer() const;
	void PlaceCoinInNewPosition();
	bool DidPlayerTakeCoin() const;
	Vector2D CalculateRandomPosition() const;
	void GeneratePredefinedSpots();
	void ToggleAutoMode();
	void SetNextPredefinedCoinPosition();
	void DebugVisitedNodeInstances();
	void GenerateRandomPathPoints(int amount);
	
public:
	PathFindingScene();
	~PathFindingScene() override;
	void update(float dtime, SDL_Event *event) override;
	void draw() override;
	const char* getTitle() override;
};
