#include <pch.h>
#include "SimpleStorage.h"
#include <fstream>

SimpleStorage::SimpleStorage(const std::string_view newFilePath)
{
	filePath = newFilePath;
}

bool SimpleStorage::HasSection(const std::string& sectionName)
{
	return false;
}

void SimpleStorage::WriteToSection()
{
}

bool SimpleStorage::WriteSection(const Section& section) const
{

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
			if (line.length() == 0 || line[0] != '[')
				continue;

			int pos = line.find_first_of(']', 1);
			if (pos == -1)
				continue;

			// Found section
			if (line.substr(1, pos - 1) == sectionName)
			{
				done = true;
				
				while (std::getline(ifs, line))
				{
					pos = line.find_first_of('=');
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
