#pragma once
#include <vector>
#include "Vector2D.h"

struct Path
{
	std::vector<Vector2D> points;
	static const int ARRIVAL_DISTANCE = 35;

	void addPathPoint(Vector2D point)
	{
		if (!points.empty())
		{
			if (points[points.size() - 1] == point)
			{
				return;
			}			
		}
		

		points.push_back(point);
	}
};
