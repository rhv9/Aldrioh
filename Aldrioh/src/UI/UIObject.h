#pragma once
#include "AnchorPoint.h"

class UIManager;

class UIObject
{
public:
	UIObject(const glm::vec2& pos, const glm::vec2& size);
	~UIObject();

	virtual void OnUpdate(Timestep ts);
	virtual void OnRender(const glm::vec2& offset);

	void AddChild(UIObject* object);

	void SetEnabled(bool enabled) { this->enabled = enabled; }
	void Disable() { enabled = false; }
	void Enable() { enabled = true; }
	bool IsEnabled() const { return enabled; }

	float GetWidth() const { return size.x; }
	float GetHeight() const { return size.y; }
	const glm::vec2& GetSize() const { return size; }

	const glm::vec4& GetBackgroundColour() const { return backgroundColour; }
	void SetBackgroundColour(const glm::vec4& colour) { backgroundColour = colour; }

	UIObject* GetParent() const { return parent; }
	void SetParent(UIObject* parent) { this->parent = parent; }

	AnchorPoint GetAnchorPoint() { return anchorPoint; }
	void SetAnchorPoint(AnchorPoint anchorPoint) { this->anchorPoint = anchorPoint; }


private:
	glm::vec2 pos{ 0 };
	glm::vec2 size{ 0 };

	glm::vec4 backgroundColour{ 0 };

	AnchorPoint anchorPoint = AnchorPoint::LEFT_BOTTOM;

	std::vector<UIObject*> children;
	bool enabled = true;

	UIObject* parent = nullptr;
	UIManager* uiManager = nullptr;

	friend class UIManager;
};