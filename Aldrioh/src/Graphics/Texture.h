#pragma once

class SubTexture;

struct TextureCoords
{
	glm::vec2 bottomLeft;
	glm::vec2 topRight;

	TextureCoords()
		: bottomLeft({ 0.0f, 0.0f }), topRight({ 1.0f, 1.0f }) {}

	TextureCoords(const glm::vec2& bottomLeftCoords, const glm::vec2& topRightCoords)
		: bottomLeft(bottomLeftCoords), topRight(topRightCoords) {}
};

class Texture
{
public:
	Texture(const std::string& path);
	Texture(uint32_t* pixelData, uint32_t width, uint32_t height, uint32_t channels);
	~Texture();

	void Bind(uint32_t slot = 0) const;
	
	uint32_t GetWidth() const { return width; }
	uint32_t GetHeight() const { return height; }
	glm::vec2 GetSize() const { return glm::vec2{ width, height }; }
	
	uint32_t GetTextureId() const { return textureId; }
	SubTexture GetAsSubTexture();

protected:
	void SetNullTextureId();

protected:
	uint32_t textureId;
	uint32_t width, height;

	int internalFormat, dataFormat;
};
