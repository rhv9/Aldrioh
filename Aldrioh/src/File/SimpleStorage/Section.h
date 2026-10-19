#pragma once

enum class PropertyType : uint8_t
{
	INT = 0,
	FLOAT,
	STRING,
	VEC2,
	VEC3,
	VEC4,
};


using SectionData = std::variant<int, float, std::string, glm::vec2, glm::vec3, glm::vec4>;

std::string SectionDataToString(const SectionData& sectionData)
{

}

struct Section
{
	std::string sectionName;
	std::vector<std::pair<std::string, SectionData>> datas;
};