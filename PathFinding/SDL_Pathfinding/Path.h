#pragma once
#include <vector>
#include "Vector2D.h"

struct Path
{
	std::vector<Vector2D> points;
	std::vector<int> weights;
	static const int ARRIVAL_DISTANCE = 35;

	void addPathPoint(Vector2D point, int weight)
	{
		if (!points.empty())
		{
			if (points[points.size() - 1] == point)
			{
				return;
			}			
		}
		

		points.push_back(point);
		weights.push_back(weight);
	}
};
