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

std::string SectionDataToString(const SectionData& sectionData);
SectionData StringToSectionData(std::string line);
std::string SectionDataToStringWithType(const SectionData& sectionData);

struct Section
{
	std::string name;
	std::vector<std::pair<std::string, SectionData>> datas;

	bool HasProperty(const std::string& other) const;
	void Debug_Print() const;
	bool HasProperties() const { return datas.size(); }
};