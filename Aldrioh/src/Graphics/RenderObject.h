#pragma once

class RenderObject
{
public:
	glm::vec3 pos;
	glm::vec2 scale;
	float rotation;
};

class SpriteRenderObject : public RenderObject
{
public:
	int spriteId;
};