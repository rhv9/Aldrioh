#pragma once

class AnchorPoint
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

	AnchorPoint() = default;
	constexpr AnchorPoint(Value anchorPoint) : value(anchorPoint) {}

	// Allow switch and comparisons.
	constexpr operator Value() const { return value; }

	explicit operator bool() const = delete;
	constexpr bool operator==(AnchorPoint a) const { return value == a.value; }
	constexpr bool operator!=(AnchorPoint a) const { return value != a.value; }

	glm::vec2 ConvertPos(const glm::vec2& pos, const glm::vec2& size, const glm::vec2& containerSize);

private:
	Value value;
};

