#include <pch.h>
#include "Path.h"

Path::Path(const std::vector<glm::vec2>& pathPoints)
{
	Init(pathPoints);
}

void Path::Init(const std::vector<glm::vec2>& pathPoints)
{
	maxPaths = pathPoints.size() - 1;

	for (int i = 0; i < pathPoints.size(); ++i)
	{
		points[i] = pathPoints[i];
	}
}
