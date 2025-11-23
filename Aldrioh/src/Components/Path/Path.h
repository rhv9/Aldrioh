#pragma once

struct PathConfig
{
	float speed = 1.0f;
};

class Path
{
public:
	Path() = default;
	Path(const std::vector<glm::vec2>& pathPoints);

	void Init(const std::vector<glm::vec2>& pathPoints);

	constexpr static int MAX_PATHS = 32;

	std::array<glm::vec2, MAX_PATHS> points{};
	std::array<PathConfig, MAX_PATHS - 1> pathConfigs{};
	int maxPaths;

};
