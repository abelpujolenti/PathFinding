#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>

#include "Vector2D.h"
#include "SDL_SimpleApp.h"

class Grid
{
private:

	int num_cell_x;
	int num_cell_y;

	std::vector<std::vector<int>> _normalLayer;
	std::vector<std::vector<int>> _enemyLayer;

public:
	
	Grid(char* filename);
	~Grid();
	Vector2D cell2pix(Vector2D cell);
	Vector2D pix2cell(Vector2D pix);
	bool isValidCell(Vector2D cell) const;
	int GetCellWeight(Vector2D cell) const;
	int getNumCellX();
	int getNumCellY();
	std::vector<std::vector<int>> GetNormalLayer();
	std::vector<std::vector<int>> GetEnemyLayer();
};
