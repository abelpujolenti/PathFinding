#include "PathFindingScene.h"

#include <utility>

#include "AStarAlgorithm.h"
#include "BreadthFirstSearchAlgorithm.h"
#include "DijkstraAlgorithm.h"
#include "GreedyBestFirstSearchAlgorithm.h"

PathFindingScene::PathFindingScene()
{
	draw_grid = false;
	_maze = std::make_unique<Grid>(Grid("../res/maze.csv"));	
	
	loadTextures("../res/maze.png", "../res/coin.png");

	srand((unsigned int)time(NULL));

	_currentPathFindingAlgorithm.reset(new BreadthFirstSearchAlgorithm);

	_player = std::make_shared<Player>();
	_player->loadSpriteTexture("../res/soldier.png", 4);
	_player->setBehavior(new PathFollowing);

	std::vector<std::shared_ptr<Enemy>> auxEnemies;
	auxEnemies.reserve(NUMBER_ENEMIES);
	for (int i = 0; i < NUMBER_ENEMIES; ++i)
	{
		std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>();
		enemy->loadSpriteTexture("../res/zombie1.png", 8);
		enemy->setBehavior(new PathFollowing);
		auxEnemies.push_back(enemy);
	}
	_enemies = auxEnemies;

	InitializeSceneComponents();
}

PathFindingScene::~PathFindingScene()
{
	if (background_texture)
		SDL_DestroyTexture(background_texture);
	if (coin_texture)
		SDL_DestroyTexture(coin_texture);
}

void PathFindingScene::OnTryToChangeAlgorithm(CurrentAlgorithm newAlgorithmTag, PathFindingAlgorithm* newPathFindingAlgorithm)
{
	if (_currentAlgorithmTag == newAlgorithmTag)
	{
		return;
	}
	
	if (_player->getCurrentTargetIndex() == -1)
	{
		ChangeAlgorithmTag(newAlgorithmTag);
		_currentPathFindingAlgorithm.reset(newPathFindingAlgorithm);
	}
	else
	{
		_nextAlgorithmTag = newAlgorithmTag;
		_nextPathFindingAlgorithm.reset(newPathFindingAlgorithm);
	}
}

void PathFindingScene::update(float dtime, SDL_Event *event)
{
	/* Keyboard & Mouse events */
	switch (event->type) {
	case SDL_KEYDOWN:
		if (event->key.keysym.scancode == SDL_SCANCODE_SPACE)
		{
			draw_grid = !draw_grid;
		}
		if (event->key.keysym.scancode == SDL_SCANCODE_1)
		{
			OnTryToChangeAlgorithm(CurrentAlgorithm::BFS, new BreadthFirstSearchAlgorithm);
		}		
		if (event->key.keysym.scancode == SDL_SCANCODE_2)
		{
			OnTryToChangeAlgorithm(CurrentAlgorithm::Dijkstra, new DijkstraAlgorithm);
		}		
		if (event->key.keysym.scancode == SDL_SCANCODE_3)
		{
			OnTryToChangeAlgorithm(CurrentAlgorithm::Greedy, new GreedyBestFirstSearchAlgorithm);
		}		
		if (event->key.keysym.scancode == SDL_SCANCODE_4)
		{
			OnTryToChangeAlgorithm(CurrentAlgorithm::A, new AStarAlgorithm);
		}
		break;
	case SDL_MOUSEBUTTONDOWN:
		if (event->button.button == SDL_BUTTON_LEFT)
		{
			if (_player->getCurrentTargetIndex() == -1)
			{
				RepositionPlayer(event);				
			}
		}
		if (event->button.button == SDL_BUTTON_RIGHT)
		{
			//////////////////////HERE/////////////////////////

			//Call Algorithm function to calculate new path points
			//Remember, function Add Path Point from Agent receive only one Vector2D

			//////////////////////////////////////////////////
			
			_currentPathFindingAlgorithm->CalculatePath(
				_maze->pix2cell(_player->getPosition()),
				_maze->pix2cell(Vector2D((float)(event->button.x), (float)(event->button.y))),
				*_maze,
				*_player
			);
		}
		break;
	default:
		break;
	}

	_player->update(dtime, event);

	for (int i = 0; i < _enemies.size(); ++i)
	{
		_enemies[i]->update(dtime, event);
	}

	// if we have arrived to the coin, replace it in a random cell!
	if (DidPlayerTakeCoin())
	{
		PlaceCoinInNewPosition();
		//ChangePathFindingAlgorithm();  //?????????????
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
	for (int j = 0; j < _maze->getNumCellY(); j++)
	{
		for (int i = 0; i < _maze->getNumCellX(); i++)
		{		
			if (!_maze->isValidCell(Vector2D((float)i, (float)j)))
			{
				SDL_SetRenderDrawColor(TheApp::Instance()->getRenderer(), 0, 0, 255, 255);
				coords = _maze->cell2pix(Vector2D((float)i, (float)j)) - Vector2D((float)CELL_SIZE / 2, (float)CELL_SIZE / 2);
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
	Vector2D coin_coords = _maze->cell2pix(coinPosition);
	int offset = CELL_SIZE / 2;
	SDL_Rect dstrect = {(int)coin_coords.x-offset, (int)coin_coords.y - offset, CELL_SIZE, CELL_SIZE};
	SDL_RenderCopy(TheApp::Instance()->getRenderer(), coin_texture, NULL, &dstrect);
}

void PathFindingScene::InitializeSceneComponents()
{		
	Vector2D rand_cell;
	constexpr int totalAgents = NUMBER_ENEMIES + 1;
	int counter = 0;
	
	while (counter < totalAgents)
	{	
		do
		{
			rand_cell = Vector2D((float)(rand() % _maze->getNumCellX()), (float)(rand() % _maze->getNumCellY()));
		}
		while (!_maze->isValidCell(rand_cell));
		
		if (counter < NUMBER_ENEMIES)
		{
			_enemies[counter]->setPosition(_maze->cell2pix(rand_cell));
			counter++;
			continue;
		}
		_player->setPosition(_maze->cell2pix(rand_cell));
		break;
	}	

	PlaceCoinInNewPosition();
	
}

void PathFindingScene::PlaceCoinInNewPosition()
{	
	coinPosition = Vector2D(-1, -1);
	do
	{
		coinPosition = Vector2D((float)(rand() % _maze->getNumCellX()), (float)(rand() % _maze->getNumCellY()));
	}
	while (!_maze->isValidCell(coinPosition) ||
		Vector2D::Distance(coinPosition, _maze->pix2cell(_player->getPosition()))<3);
		
}

void PathFindingScene::ChangeAlgorithmTag(CurrentAlgorithm newAlgorithm)
{
	InitializeSceneComponents();		
	_player->clearPath();
	_player->setVelocity(Vector2D(0,0));
	_currentAlgorithmTag = newAlgorithm;
}

void PathFindingScene::ChangePathFindingAlgorithm()
{
	if (_currentAlgorithmTag == _nextAlgorithmTag)
	{
		return;
	}
	_currentAlgorithmTag = _nextAlgorithmTag;
	_currentPathFindingAlgorithm = std::move(_nextPathFindingAlgorithm);
}

bool PathFindingScene::DidPlayerTakeCoin() const
{
	return _player->getCurrentTargetIndex() == -1 && _maze->pix2cell(_player->getPosition()) == coinPosition;
}

void PathFindingScene::RepositionPlayer(SDL_Event* event) const
{	
	Vector2D cell = _maze->pix2cell(Vector2D((float)(event->button.x), (float)(event->button.y)));
	if (_maze->isValidCell(cell)) {
		_player->clearPath();
		_player->setPosition(_maze->cell2pix(cell));
	}
}


bool PathFindingScene::loadTextures(char* filename_bg, char* filename_coin)
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
