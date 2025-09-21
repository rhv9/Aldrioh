#pragma once
#include "AnchorPoint.h"

#include <Events/Events.h>

enum UIType
{
	Object,
	Text,
	Image,
	Button,
};

class UIManager;

#define UIOBJECT_EVENT(event) virtual void On##event##Event(##event##EventArg& e) {} \
void On##event##EventChildren(##event##EventArg& e) \
{ \
	for (UIObject* child : children)\
	{\
		if (child->IsEnabled())\
		{\
			child->On##event##Event(e);\
			child->On##event##EventChildren(e);\
		}\
	}\
}\

class UIObject
{
public:
	UIObject(const std::string& name, const glm::vec2& relativePos, const glm::vec2& size);
	UIObject() = default;
	virtual ~UIObject();

	virtual void OnUpdate(Timestep ts);
	virtual void OnRender(Timestep ts);

	virtual UIType GetType() const { return UIType::Object; }

	void AddChild(UIObject* child);

	void SetEnabled(bool enabled) { this->enabled = enabled; }
	void Disable() { enabled = false; }
	void Enable() { enabled = true; }
	bool IsEnabled() const { return enabled; }

	float GetWidth() const { return size.x; }
	float GetHeight() const { return size.y; }
	const glm::vec2& GetSize() const { return size; }
	void SetSize(const glm::vec2& size) { this->size = size; RecalculateRenderPos(); }

	void SetRelativePos(const glm::vec2& newRelativePos) { relativePos = newRelativePos; RecalculateRenderPos(); }
	const glm::vec2& GetRelativePos() const { return relativePos; }
	float GetRelativePosX() const { return relativePos.x; }
	float GetRelativePosY() const { return relativePos.y; }

	const glm::vec2& GetRenderPos() const { return renderPos; }

	const glm::vec4& GetBackgroundColour() const { return backgroundColour; }
	void SetBackgroundColour(const glm::vec4& colour) { backgroundColour = colour; }

	UIObject* GetParent() const { return parent; }
	void SetParent(UIObject* parent) { this->parent = parent; }

	AnchorPoint GetAnchorPoint() const { return anchorPoint; }
	void SetAnchorPoint(AnchorPoint anchorPoint) { this->anchorPoint = anchorPoint; RecalculateRenderPos(); }

	UIManager* GetUIManager();


	bool IsMouseHovering();
	bool IsPosWithin(const glm::vec2& pos) const;

	// Event Related
	UIOBJECT_EVENT(MouseMove)
	UIOBJECT_EVENT(MouseButton)

private:
	void RenderChildren(Timestep ts);

	void RecalculateRenderPos();
	void SetUIManager(UIManager* uiManager);


protected:
	std::string name = "unnamed";

	glm::vec2 relativePos{ 0 }, renderPos{ 0 };
	glm::vec2 size{ 0 };
	AnchorPoint anchorPoint = AnchorPoint::LEFT_BOTTOM;
	glm::vec4 backgroundColour{ 0 };

	std::vector<UIObject*> children;
	UIObject* parent = nullptr;

	bool enabled = true;

private:
	UIManager* uiManager = nullptr;

	friend class UIManager;
};