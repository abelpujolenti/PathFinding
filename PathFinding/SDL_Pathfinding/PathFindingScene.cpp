#include "PathFindingScene.h"

#include <utility>
#include <fstream>
#include <time.h>
#include <iostream>

#include "Enemy.h"
#include "Grid.h"
#include "Player.h"

PathFindingScene::PathFindingScene() :
	_normalLayer{std::make_unique<Grid>(PATH_MAZE_CSV)},
	_enemyLayer{std::make_shared<Grid>(PATH_MAZE_CSV)}
{
	loadTextures(PATH_MAZE_PNG, PATH_COIN);

	srand((unsigned int)time(NULL));
	
	_player = std::make_unique<Player>(NUMBER_ENEMIES);

	std::vector<std::unique_ptr<Enemy>> auxEnemies;
	auxEnemies.reserve(NUMBER_ENEMIES);
	for (int i = 0; i < NUMBER_ENEMIES; ++i)
	{
		std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>(_enemyLayer);
		auxEnemies.push_back(std::move(enemy));
	}
	_enemies = std::move(auxEnemies);

	InitializeSceneComponents();
	GeneratePredefinedSpots();
}

PathFindingScene::~PathFindingScene()
{
	if (background_texture)
		SDL_DestroyTexture(background_texture);
	if (coin_texture)
		SDL_DestroyTexture(coin_texture);
}

void PathFindingScene::update(float dtime, SDL_Event *event)
{
	/* Keyboard & Mouse events */
	
	switch (event->type) {
		case SDL_KEYDOWN:
			switch (event->key.keysym.scancode) {
				case SDL_SCANCODE_SPACE:
					draw_grid = !draw_grid;	
					break;

				case SDL_SCANCODE_1:
				case SDL_SCANCODE_2:
				case SDL_SCANCODE_3:
				case SDL_SCANCODE_4:
					if (_player->getPathSize() == 0)
					{
						InitializeSceneComponents();
					}
					break;
				case SDL_SCANCODE_A:
					ToggleAutoMode();
					break;
				case SDL_SCANCODE_D:
					DebugVisitedNodeInstances();
					break;
				default:
					break;
			}			
			
			break;
		default:
			break;
	}

	for (int i = 0; i < _enemies.size(); ++i)
	{
		_enemies[i]->update(dtime, event, *_normalLayer);
	}	

	_player->update(dtime, event, *_enemyLayer);

	// if we have arrived to the coin, replace it in a random cell!
	if (DidPlayerTakeCoin())
	{
		if (autoMode)
		{
			SetNextPredefinedCoinPosition();
		}
		else
		{
			PlaceCoinInNewPosition();
		}
	}
	
}

void PathFindingScene::draw()
{
	drawMaze();
	drawCoin();

	if (draw_grid)
	{
		SDL_SetRenderDrawColor(TheApp::Instance()->getRenderer(), 255, 255, 255, 127);
		for (int i = 0; i < SRC_WIDTH; i+=CELL_SIZE)
		{
			SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), i, 0, i, SRC_HEIGHT);
		}
		for (int j = 0; j < SRC_HEIGHT; j = j += CELL_SIZE)
		{
			SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), 0, j, SRC_WIDTH, j);
		}
	}

	_player->draw();

	for (int i = 0; i < _enemies.size(); ++i)
	{
		_enemies[i]->draw();		
	}
	
}

const char* PathFindingScene::getTitle()
{
	return "SDL Path Finding :: PathFinding Demo";
}

void PathFindingScene::drawMaze() const
{
	SDL_SetRenderDrawColor(TheApp::Instance()->getRenderer(), 0, 0, 255, 255);
	SDL_Rect rect;
	Vector2D coords;
	for (int j = 0; j < _normalLayer->getNumCellY(); j++)
	{
		for (int i = 0; i < _normalLayer->getNumCellX(); i++)
		{		
			if (!_normalLayer->isValidCell(Vector2D((float)i, (float)j)))
			{
				SDL_SetRenderDrawColor(TheApp::Instance()->getRenderer(), 0, 0, 255, 255);
				coords = _normalLayer->cell2pix(Vector2D((float)i, (float)j)) - Vector2D((float)CELL_SIZE / 2, (float)CELL_SIZE / 2);
				rect = { (int)coords.x, (int)coords.y, CELL_SIZE, CELL_SIZE };
				SDL_RenderFillRect(TheApp::Instance()->getRenderer(), &rect);
			} else {
				// Do not draw if it is not necessary (bg is already black)
			}
					
			
		}
	}
	//Alternative: render a backgroud texture:
	//SDL_RenderCopy(TheApp::Instance()->getRenderer(), background_texture, NULL, NULL );
}

void PathFindingScene::drawCoin() const
{
	const Vector2D coin_coords = _normalLayer->cell2pix(coinPosition);
	const int offset = CELL_SIZE / 2;
	const SDL_Rect dstrect = {(int)coin_coords.x-offset, (int)coin_coords.y - offset, CELL_SIZE, CELL_SIZE};
	SDL_RenderCopy(TheApp::Instance()->getRenderer(), coin_texture, NULL, &dstrect);
}

void PathFindingScene::InitializeSceneComponents()
{		
	srand(time(NULL));
	Vector2D rand_cell;
	constexpr int totalAgents = NUMBER_ENEMIES + 1;

	//ResetEnemyLayer();
	
	for (int i = 0; i < totalAgents; i++)
	{
		rand_cell = CalculateRandomPosition();
		
		if (i < NUMBER_ENEMIES)
		{
			const Vector2D cell = _normalLayer->cell2pix(rand_cell);			
			_enemies[i]->setPosition(cell);
			_enemies[i]->SetDestination(cell);
			_enemies[i]->SetCurrentCell(rand_cell);
			_enemies[i]->clearPath();
		}
		_player->setPosition(_normalLayer->cell2pix(rand_cell));
		_player->SetCurrentCell(rand_cell);
	}

	PlaceCoinInNewPosition();	
}

void PathFindingScene::ResetEnemyLayer() const
{
	*_enemyLayer = Grid{PATH_MAZE_CSV};
}

void PathFindingScene::PlaceCoinInNewPosition()
{
	do
	{
		coinPosition = Vector2D((float)(rand() % _normalLayer->getNumCellX()), (float)(rand() % _normalLayer->getNumCellY()));
	}
	while (!_normalLayer->isValidCell(coinPosition) ||
		Vector2D::Distance(coinPosition, _normalLayer->pix2cell(_player->getPosition()))<3);
		
}

bool PathFindingScene::DidPlayerTakeCoin() const
{
	return _player->getCurrentTargetIndex() == -1 && _normalLayer->pix2cell(_player->getPosition()) == coinPosition;
}

Vector2D PathFindingScene::CalculateRandomPosition() const
{
	Vector2D rand_cell;
	
	do
	{
		rand_cell = Vector2D((float)(rand() % _normalLayer->getNumCellX()), (float)(rand() % _normalLayer->getNumCellY()));
	} while (!_normalLayer->isValidCell(rand_cell));

	return rand_cell;
}

void PathFindingScene::GeneratePredefinedSpots()
{
	for (int i = 0; i < 20; i++) {
		predefinedCoinSpots.push_back(CalculateRandomPosition());
	}
	predefinedPlayerStart = CalculateRandomPosition();
}

void PathFindingScene::ToggleAutoMode()
{
	autoMode = !autoMode;
	if (autoMode) {
		_player->setPosition(_normalLayer->cell2pix(predefinedPlayerStart));
		visitedNodeInstances.clear();
		predefinedCoinSpotIndex = 0;
		SetNextPredefinedCoinPosition();
	}
}

void PathFindingScene::SetNextPredefinedCoinPosition()
{
	if (predefinedCoinSpotIndex >= predefinedCoinSpots.size()) {
		autoMode = false;
		DebugVisitedNodeInstances();
		return;
	}
	coinPosition = predefinedCoinSpots[predefinedCoinSpotIndex];
	int visitedNodes = _player->PathTowardsPosition(_normalLayer->cell2pix(coinPosition), *_normalLayer);
	if (visitedNodes > 0) {
		visitedNodeInstances.push_back(visitedNodes);
	}
	predefinedCoinSpotIndex++;
}

void PathFindingScene::DebugVisitedNodeInstances()
{
	float average = 0.f;
	int max = visitedNodeInstances[0];
	int min = visitedNodeInstances[0];
	for (int i = 0; i < visitedNodeInstances.size(); i++) {
		if (visitedNodeInstances[i] < min) {
			min = visitedNodeInstances[i];
		}
		if (visitedNodeInstances[i] > max) {
			max = visitedNodeInstances[i];
		}
		average += visitedNodeInstances[i];
	}
	average /= visitedNodeInstances.size();
	std::cout << std::endl;
	std::cout << "Minimum: " << min << std::endl;
	std::cout << "Maximum: " << max << std::endl;
	std::cout << "Average: " << average << std::endl;
}

bool PathFindingScene::loadTextures(const char* filename_bg, const char* filename_coin)
{
	SDL_Surface *image = IMG_Load(filename_bg);
	if (!image) {
		std::cout << "IMG_Load: " << IMG_GetError() << std::endl;
		return false;
	}
	background_texture = SDL_CreateTextureFromSurface(TheApp::Instance()->getRenderer(), image);

	if (image)
		SDL_FreeSurface(image);

	image = IMG_Load(filename_coin);
	if (!image) {
		std::cout << "IMG_Load: " << IMG_GetError() << std::endl;
		return false;
	}
	coin_texture = SDL_CreateTextureFromSurface(TheApp::Instance()->getRenderer(), image);

	if (image)
		SDL_FreeSurface(image);

	return true;
}
