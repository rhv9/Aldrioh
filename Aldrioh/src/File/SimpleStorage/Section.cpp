#include <pch.h>
#include "Section.h"

#include <charconv>

static void leftTrim(std::string& s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
		return !std::isspace(ch);
		}));
}

void rightTrim(std::string& s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
		return !std::isspace(ch);
		}).base(), s.end());
}

std::string SectionDataToString(const SectionData& sectionData)
{
	if (const int* num = std::get_if<int>(&sectionData))
		return std::to_string(*num);
	if (const float* num = std::get_if<float>(&sectionData))
		return std::to_string(*num);
	if (const std::string* msg = std::get_if<std::string>(&sectionData))
		return *msg;
	if (const glm::vec2* vecNum = std::get_if<glm::vec2>(&sectionData))
		return std::format("[{},{}]", vecNum->x, vecNum->y);
	if (const glm::vec3* vecNum = std::get_if<glm::vec3>(&sectionData))
		return std::format("[{},{},{}]", vecNum->x, vecNum->y, vecNum->z);
	if (const glm::vec4* vecNum = std::get_if<glm::vec4>(&sectionData))
		return std::format("[{},{},{},{}]", vecNum->x, vecNum->y, vecNum->z, vecNum->w);

	return "";
}

SectionData StringToSectionData(std::string line)
{
	leftTrim(line);
	rightTrim(line);

	if (line.empty())
		return "";

	// Potentially vector
	if (line[0] == '[' && line[line.size() - 1] == ']')
	{
		std::string view = line.substr(1, line.size() - 2);
		std::stringstream ss(view);
		float values[4];
		int count = 0;

		std::string token;

		while (std::getline(ss, token, ','))
		{
			leftTrim(token);
			rightTrim(token);

			if (count >= 4 || token.empty())
				return line;

			auto [ptr, erc] = std::from_chars(token.data(), token.data() + token.size(), values[count]);
			if (erc != std::errc() || ptr != (token.data() + token.size()))
				return line;
			++count;
		}

		if (count == 1)
			return values[0];
		if (count == 2)
			return glm::vec2{ values[0], values[1] };
		if (count == 3)
			return glm::vec3{ values[0], values[1], values[2] };
		if (count == 4)
			return glm::vec4{ values[0], values[1], values[2], values[3] };
	}

	int num = 0;
	auto [ptr, erc] = std::from_chars(line.data(), line.data() + line.size(), num);
	if (erc == std::errc() && ptr == (line.data() + line.size()))
		return num;

	float floatNum = 0;
	auto [ptr2, erc2] = std::from_chars(line.data(), line.data() + line.size(), floatNum);
	if (erc2 == std::errc() && ptr2 == (line.data() + line.size()))
		return floatNum;

	return line;
}

std::string SectionDataToStringWithType(const SectionData& sectionData)
{
	if (const int* num = std::get_if<int>(&sectionData))
		return std::format("Int({})", std::to_string(*num));
	if (const float* num = std::get_if<float>(&sectionData))
		return std::format("Float({})", std::to_string(*num));
	if (const std::string* msg = std::get_if<std::string>(&sectionData))
		return std::format("String({})", *msg);
	if (const glm::vec2* vecNum = std::get_if<glm::vec2>(&sectionData))
		return std::format("Vec2[{},{}]", vecNum->x, vecNum->y);
	if (const glm::vec3* vecNum = std::get_if<glm::vec3>(&sectionData))
		return std::format("Vec3[{},{},{}]", vecNum->x, vecNum->y, vecNum->z);
	if (const glm::vec4* vecNum = std::get_if<glm::vec4>(&sectionData))
		return std::format("Vec4[{},{},{},{}]", vecNum->x, vecNum->y, vecNum->z, vecNum->w);

	return "";
}

bool Section::HasProperty(const std::string& other) const
{
	for (auto [property, data] : datas)
		if (property == other)
			return true;
	return false;
}

void Section::Debug_Print() const
{
	LOG_CORE_INFO("[{}]", name);
	for (auto [property, data] : datas)
		LOG_CORE_INFO("{}={}", property, SectionDataToStringWithType(data));
}
