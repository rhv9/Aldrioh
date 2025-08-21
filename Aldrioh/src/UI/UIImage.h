#pragma once
#include "UIObject.h"
#include <Graphics/SubTexture.h>
#include <Game/SpriteCollection.h>

class UIImage : public UIObject
{
public:
	UIImage(const std::string& name, const glm::vec2& relativePos, const glm::vec2& size);

	virtual void OnUpdate(Timestep ts) override;
	virtual void OnRender(Timestep ts) override;
	virtual UIType GetType() const override { return UIType::Image; }

	void SetSubTexture(SubTexture* subTexture) { this->subTexture = subTexture; }

private:
	SubTexture* subTexture = Sprites::get(Sprites::null);
};