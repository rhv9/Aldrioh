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
	static const uint8_t MAX_NUMBER = 9;

	AnchorPoint() = default;
	constexpr AnchorPoint(Value anchorPoint) : value(anchorPoint) {}
	constexpr AnchorPoint(uint8_t num) { ASSERT(num >= 0 && num < MAX_NUMBER, "AnchorPoint does not exist"); value = static_cast<Value>(num); }

	// Allow switch and comparisons.
	constexpr operator Value() const { return value; }

	explicit operator bool() const = delete;
	constexpr bool operator==(AnchorPoint a) const { return value == a.value; }
	constexpr bool operator!=(AnchorPoint a) const { return value != a.value; }

	glm::vec2 ConvertPos(const glm::vec2& pos, const glm::vec2& size, const glm::vec2& containerSize);
	
	const std::string& ToString() const;

private:
	Value value;
};

