#pragma once

class RenderObject
{
public:
	glm::vec3 pos;
	glm::vec2 scale;
	float rotation;
	glm::vec4 colour;
	float flags;
};

class SpriteRenderObject : public RenderObject
{
public:
	int spriteId;

};