#pragma once

class UIAnchorPoint
{
public:
	enum Value : uint8_t
	{
		LEFT_BOTTOM = 0,
		LEFT_CENTER,
		LEFT_TOP,
		CENTER_BOTTOM,
		CENTER,
		CENTER_TOP,
		RIGHT_BOTTOM,
		RIGHT_CENTER,
		RIGHT_TOP,
	};

	UIAnchorPoint() = default;
	constexpr UIAnchorPoint(Value aAnchorPoint) : value(aAnchorPoint) {}

	// Allow switch and comparisons.
	constexpr operator Value() const { return value; }

	explicit operator bool() const = delete;
	constexpr bool operator==(UIAnchorPoint a) const { return value == a.value; }
	constexpr bool operator!=(UIAnchorPoint a) const { return value != a.value; }

	glm::vec2 ConvertPos(const glm::vec2& pos, const glm::vec2& size, const glm::vec2& containerSize);

private:
	Value value;
};

