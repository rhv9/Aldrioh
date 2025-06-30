#pragma once
#include "Tiles.h"
class TexturedTiles : public Tiles
{
public:
	TexturedTiles(const int spriteId);
	virtual ~TexturedTiles() override;

	virtual void OnUpdate(Timestep ts, const TileMetaData& metadata) override;
	virtual void OnRender(Timestep ts, const TileMetaData& metadata) override;

	const int GetSpriteId() const { return spriteId; }

private:
	int spriteId;
};