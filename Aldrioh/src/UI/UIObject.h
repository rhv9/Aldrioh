#pragma once

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

	uint32_t GetWidth() const { return size.x; }
	uint32_t GetHeight() const { return size.y; }

	const glm::vec4& GetBackgroundColour() const { return backgroundColour; }
	void SetBackgroundColour(const glm::vec4& colour) { backgroundColour = colour; }

	void SetParent(UIObject* parent) { this->parent = parent; }
	UIObject* GetParent() const { return parent; }

private:
	glm::vec2 pos{ 0 };
	glm::vec2 size{ 0 };

	glm::vec4 backgroundColour{ 0 };

	std::vector<UIObject*> children;
	bool enabled = true;

	UIObject* parent = nullptr;
};