#pragma once

#include "Section.h"

class SimpleStorage
{
public:
	SimpleStorage(std::string_view newFilePath);

	bool WriteSection(const Section& section) const;
	Section GetSection(const std::string& sectionName);
	bool fileExists() const;

private:
	const std::string filePath;
	const std::string tempPath;
};