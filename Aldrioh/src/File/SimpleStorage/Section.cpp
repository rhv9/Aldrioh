#include <pch.h>
#include "Section.h"

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

bool Section::HasProperty(const std::string& other) const
{
	for (auto [property, data] : datas)
		if (property == other)
			return true;
	return false;
}
