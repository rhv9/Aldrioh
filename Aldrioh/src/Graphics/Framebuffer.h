#pragma once
#include "Texture.h"

class Framebuffer
{
public:
	Framebuffer(uint32_t width, uint32_t height);
	~Framebuffer();

	void Bind() const;
	void UnBind() const;

	void Resize(uint32_t width, uint32_t height);
	uint32_t GetWidth() const { return textureObj->GetWidth(); }
	uint32_t GetHeight() const { return textureObj->GetHeight(); }

	std::unique_ptr<Texture>& GetTextureObj() { return textureObj; }

private:
	void Init(uint32_t width, uint32_t height);

private:
	unsigned int fbo;
	std::unique_ptr<Texture> textureObj;
	unsigned int deptho;
};
