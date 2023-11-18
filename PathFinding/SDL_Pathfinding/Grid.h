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

	std::vector<std::vector<float>> _terrain;

public:
	Grid(const char* filename);
	~Grid();
	Vector2D cell2pix(Vector2D cell) const;
	Vector2D pix2cell(Vector2D pix) const;
	bool isValidCell(Vector2D cell) const;
	float GetCellWeight(Vector2D cell) const;
	void ModifyCellWeight(Vector2D cell, float weight);
	int getNumCellX() const;
	int getNumCellY() const;
};
