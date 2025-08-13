#include <pch.h>
#include "AnchorPoint.h"

glm::vec2 UIAnchorPoint::ConvertPos(const glm::vec2& pos, const glm::vec2& size, const glm::vec2& containerSize)
{
	glm::vec2 result = pos;
	switch (this->value)
	{
	case UIAnchorPoint::LEFT_BOTTOM:
		return result;
	case UIAnchorPoint::LEFT_CENTER:
		result.y = pos.y + (containerSize.y - size.y) / 2.0f;
		return result;
	case UIAnchorPoint::LEFT_TOP:
		result.y = (containerSize.y) - pos.y - size.y;
		return result;

	case UIAnchorPoint::CENTER_BOTTOM:
		result.x = pos.x + (containerSize.x - size.x) / 2.0f;
		return result;
	case UIAnchorPoint::CENTER_TOP:
		result.x = pos.x + (containerSize.x - size.x) / 2.0f;
		result.y = (containerSize.y) - pos.y - size.y;
		return result;
	case UIAnchorPoint::CENTER:
		result.x = pos.x + (containerSize.x - size.x) / 2.0f;
		result.y = pos.y + (containerSize.y - size.y) / 2.0f;
		return result;

	case UIAnchorPoint::RIGHT_BOTTOM:
		result.x = containerSize.x - pos.x - size.x;
		return result;
	case UIAnchorPoint::RIGHT_CENTER:
		result.x = containerSize.x - pos.x - size.x;
		result.y = pos.y + (containerSize.y - size.y) / 2.0f;
		return result;
	case UIAnchorPoint::RIGHT_TOP:
		result.x = containerSize.x - pos.x - size.x;
		result.y = (containerSize.y) - pos.y - size.y;
		return result;

	default:
		return result;
	}

	return result;
}
