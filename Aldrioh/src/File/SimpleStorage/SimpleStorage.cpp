#include <pch.h>
#include "SimpleStorage.h"
#include <fstream>

SimpleStorage::SimpleStorage(const std::string_view newFilePath) : filePath(newFilePath), tempPath(filePath + "temp")
{
}

bool SimpleStorage::HasSection(const std::string& sectionName)
{
	return false;
}

std::string extractSectionName(const std::string& line)
{
	if (!line.empty() && line[0] == '[')
	{
		int pos = line.find_first_of(']', 1);
		if (pos != -1)
			return line.substr(1, pos - 1);
	}
	return "";
}

void leftTrim(std::string& s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
		return !std::isspace(ch);
		}));
}

bool SimpleStorage::WriteSection(const Section& section) const
{
	std::ifstream inputStream(filePath);
	std::ofstream tempStream(tempPath);
	
	std::string line;

	if (inputStream.is_open() && tempStream.is_open())
	{
		bool foundSection = false;
		bool previousLineEmpty = false;

		while (std::getline(inputStream, line))
		{
			previousLineEmpty = line.empty();
			// Found section
			if (extractSectionName(line) == section.name)
			{
				foundSection = true;
				tempStream << line << '\n';

				for (auto [property, sectionData] : section.datas)
					tempStream << std::format("{}={}\n", property, SectionDataToString(sectionData));

				// Go through original properties already saved and add only unmodified ones
				while (std::getline(inputStream, line))
				{
					leftTrim(line);
					if (line.empty() || line[0] == '[')
						break;

					if (!section.HasProperty(line.substr(0, line.find_first_of('='))))
						tempStream << line << '\n';
				}

				// The else if allows sections to not have a gap between them
				if (line.empty())
					tempStream << '\n';
				else if (line[0] == '[')
					tempStream << line << '\n';
			}
			else
				tempStream << line << '\n';
		}

		if (!foundSection)
		{
			if (!previousLineEmpty)
				tempStream << '\n';

			tempStream << std::format("[{}]\n", section.name);
			for (auto [property, sectionData] : section.datas)
				tempStream << std::format("{}={}\n", property, SectionDataToString(sectionData));
		}
	}

	return true;
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
