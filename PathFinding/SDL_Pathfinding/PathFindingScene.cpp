#include "PathFindingScene.h"

#include <utility>

PathFindingScene::PathFindingScene()
{
	draw_grid = false;
	_maze = std::make_unique<Grid>(Grid("../res/maze.csv"));	
	
	loadTextures("../res/maze.png", "../res/coin.png");

	srand((unsigned int)time(NULL));
	
	_player = std::make_shared<Player>();

	std::vector<std::shared_ptr<Enemy>> auxEnemies;
	auxEnemies.reserve(NUMBER_ENEMIES);
	for (int i = 0; i < NUMBER_ENEMIES; ++i)
	{
		std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>();
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
				default:
					break;
			}			
			
			break;
		default:
			break;
	}

	std::weak_ptr<Grid> mazeWeakPointer = _maze;
	if (std::shared_ptr<Grid> mazeLock = mazeWeakPointer.lock())
	{		
		_player->AgentUpdate(dtime, event, mazeLock);

		for (int i = 0; i < _enemies.size(); ++i)
		{
			_enemies[i]->AgentUpdate(dtime, event, mazeLock);
		}	
	}	

	// if we have arrived to the coin, replace it in a random cell!
	if (DidPlayerTakeCoin())
	{
		PlaceCoinInNewPosition();
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
	const Vector2D coin_coords = _maze->cell2pix(coinPosition);
	const int offset = CELL_SIZE / 2;
	const SDL_Rect dstrect = {(int)coin_coords.x-offset, (int)coin_coords.y - offset, CELL_SIZE, CELL_SIZE};
	SDL_RenderCopy(TheApp::Instance()->getRenderer(), coin_texture, NULL, &dstrect);
}

void PathFindingScene::InitializeSceneComponents()
{		
	Vector2D rand_cell;
	constexpr int totalAgents = NUMBER_ENEMIES + 1;
	
	for (int i = 0; i < totalAgents; i++)
	{
		rand_cell = CalculateRandomPosition();
		
		if (i < NUMBER_ENEMIES)
		{
			const Vector2D cell = _maze->cell2pix(rand_cell);			
			_enemies[i]->setPosition(cell);
			_enemies[i]->SetDestination(cell);
			_enemies[i]->clearPath();
		}
		_player->setPosition(_maze->cell2pix(rand_cell));
	}

	PlaceCoinInNewPosition();	
}

void PathFindingScene::PlaceCoinInNewPosition()
{	
	do
	{
		coinPosition = Vector2D((float)(rand() % _maze->getNumCellX()), (float)(rand() % _maze->getNumCellY()));
	}
	while (!_maze->isValidCell(coinPosition) ||
		Vector2D::Distance(coinPosition, _maze->pix2cell(_player->getPosition()))<3);
		
}

bool PathFindingScene::DidPlayerTakeCoin() const
{
	return _player->getCurrentTargetIndex() == -1 && _maze->pix2cell(_player->getPosition()) == coinPosition;
}

Vector2D PathFindingScene::CalculateRandomPosition() const
{
	Vector2D rand_cell;
	
	do
	{
		rand_cell = Vector2D((float)(rand() % _maze->getNumCellX()), (float)(rand() % _maze->getNumCellY()));
	} while (!_maze->isValidCell(rand_cell));

	return rand_cell;
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
