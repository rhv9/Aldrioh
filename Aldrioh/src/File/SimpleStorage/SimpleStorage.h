#pragma once

#include "Section.h"

class SimpleStorage
{
public:
	SimpleStorage(std::string_view newFilePath);
	bool HasSection(const std::string& sectionName);
	bool WriteSection(const Section& section) const;

	std::vector<std::pair<std::string, SectionData>> GetSection(const std::string& sectionName);
private:
	const std::string filePath;
	const std::string tempPath;
};