#include "Grid.h"

using namespace std;

Grid::Grid(char* filename)
{
	num_cell_x = SRC_WIDTH / CELL_SIZE;
	num_cell_y = SRC_HEIGHT / CELL_SIZE;

	// Initialize the terrain matrix from file (for each cell a zero value indicates it's a wall, positive values indicate terrain cell cost)
	std::ifstream infile(filename);
	std::string line;
	while (std::getline(infile, line))
	{
		vector<int> terrain_row;
		std::stringstream lineStream(line);
		std::string cell;
		while (std::getline(lineStream, cell, ','))
			terrain_row.push_back(atoi(cell.c_str()));
		SDL_assert(terrain_row.size() == num_cell_x);
		_normalLayer.push_back(terrain_row);
		_enemyLayer.push_back(terrain_row);
	}
	SDL_assert(_normalLayer.size() == num_cell_y);
	infile.close();
}

Grid::~Grid()
{
}

int Grid::getNumCellX()
{
	return num_cell_x;
}

int Grid::getNumCellY()
{
	return num_cell_y;
}

std::vector<std::vector<int>> Grid::GetNormalLayer()
{
	return _normalLayer;
}

std::vector<std::vector<int>> Grid::GetEnemyLayer()
{
	return _enemyLayer;
}

Vector2D Grid::cell2pix(Vector2D cell)
{
	int offset = CELL_SIZE / 2;
	return Vector2D(cell.x*CELL_SIZE + offset, cell.y*CELL_SIZE + offset);
}

Vector2D Grid::pix2cell(Vector2D pix)
{
	return Vector2D((float)((int)pix.x / CELL_SIZE), (float)((int)pix.y / CELL_SIZE));
}

bool Grid::isValidCell(Vector2D cell) const
{
	if ((cell.x < 0) || (cell.y < 0) || (cell.y >= _normalLayer.size()) || (cell.x >= _normalLayer[0].size()))
		return false;
	return !(_normalLayer[(unsigned int)cell.y][(unsigned int)cell.x] == 0);
}

int Grid::GetCellWeight(Vector2D cell) const
{
	if ((cell.x < 0) || (cell.y < 0) || (cell.y >= _normalLayer.size()) || (cell.x >= _normalLayer[0].size()))
		return false;
	return _normalLayer[(unsigned int)cell.y][(unsigned int)cell.x];
}

