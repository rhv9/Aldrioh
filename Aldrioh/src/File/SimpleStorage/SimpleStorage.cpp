#include <pch.h>
#include "SimpleStorage.h"
#include <fstream>

SimpleStorage::SimpleStorage(const std::string_view newFilePath) : filePath(newFilePath), tempPath(filePath + "_temp")
{
}

bool SimpleStorage::HasSection(const std::string& sectionName)
{
	return false;
}

void SimpleStorage::WriteToSection()
{
}

std::string extractSectionName(const std::string& line)
{
	if (line.length() != 0 && line[0] != '[')
	{
		int pos = line.find_first_of(']', 1);
		if (pos != -1)
			return line.substr(1, pos - 1);
	}
	return "";
}

bool SimpleStorage::WriteSection(const Section& section) const
{
	std::ifstream inputStream(filePath);
	std::ofstream tempStream(tempPath);
	
	std::string line;

	if (inputStream.is_open() && tempStream.is_open())
	{
		bool foundSection = false;
		while (std::getline(inputStream, line))
		{
			std::string extractedSectionName = extractSectionName(line);

			if (extractedSectionName == section.sectionName)
			{
				foundSection = true;
				break;
			}
			else
				tempStream << line << '\n';
		}

		if (foundSection)
		{
			for (auto [property, sectionData] : section.datas)
			{
				tempStream << property;

				
			}
		}
	}

	return false;
}

void SimpleStorage::WriteString(std::string_view property, std::string_view msg)
{
	std::ofstream ofs(filePath);
	
	ofs << property;
	ofs << '=';
	ofs << msg;
	ofs << '\n';
}

std::vector<std::pair<std::string, std::string>> SimpleStorage::GetSection(const std::string& sectionName)
{
	std::ifstream ifs(filePath);
	std::string line;
	std::vector<std::pair<std::string, std::string>> keyValues;
	bool done = false;
	if (ifs.is_open())
	{
		while (std::getline(ifs, line))
		{
			std::string lineSectionName = extractSectionName(line);
			// Found section
			if (lineSectionName == sectionName)
			{
				done = true;
				
				while (std::getline(ifs, line))
				{
					int pos = line.find_first_of('=');
					if (pos == -1)
						continue;
					std::string left = line.substr(0, pos);
					std::string right = line.substr(pos + 1, line.length());
					
					keyValues.emplace_back(left, right);
				}
			}

			if (done)
				break;
		}
	}
	else
		LOG_CORE_ERROR("Could not load section in file: {}: {}", filePath, sectionName);

	return keyValues;
}
