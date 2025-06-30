#pragma once

class Level;

struct TileMetaData
{
	glm::vec2 pos;
	Level* level;
};


class Tiles
{
public:
	Tiles() {}
	virtual ~Tiles() {}
	virtual void OnUpdate(Timestep ts, const TileMetaData& metadata) = 0;
	virtual void OnRender(Timestep ts, const TileMetaData& metadata) = 0;

private:
};
