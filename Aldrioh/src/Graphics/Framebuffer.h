#pragma once
#include "Texture.h"

class Framebuffer
{
public:
	Framebuffer(uint32_t width, uint32_t height);
	~Framebuffer();

	void Bind() const;
	void UnBind() const;

	std::unique_ptr<Texture>& GetTextureObj() { return textureObj; }

private:
	unsigned int fbo;
	std::unique_ptr<Texture> textureObj;
	unsigned int deptho;
};
