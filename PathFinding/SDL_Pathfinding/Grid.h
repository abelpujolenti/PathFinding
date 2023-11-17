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

	std::vector<std::vector<int>> _terrain;

public:
	Grid(char* filename);
	~Grid();
	Vector2D cell2pix(Vector2D cell);
	Vector2D pix2cell(Vector2D pix);
	bool isValidCell(Vector2D cell) const;
	int GetCellWeight(Vector2D cell) const;
	void SetCellWeight(Vector2D cell, int weight);
	int getNumCellX() const;
	int getNumCellY() const;
};
