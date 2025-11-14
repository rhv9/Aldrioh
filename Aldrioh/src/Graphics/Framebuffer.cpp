#include <pch.h>
#include "Framebuffer.h"
#include <glad/glad.h>

Framebuffer::Framebuffer(uint32_t width, uint32_t height)
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	
	// Generate texture
	unsigned int textureo;
	glGenTextures(1, &textureo);
	glBindTexture(GL_TEXTURE_2D, textureo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	// attach it to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureo, 0);
	
	// Wrap into Texture obj
	textureObj = std::make_unique<Texture>(textureo, width, height, GL_RGBA8, GL_RGBA);

	ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Complete!");
	
	UnBind();
}

Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &fbo);
}

void Framebuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void Framebuffer::UnBind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
