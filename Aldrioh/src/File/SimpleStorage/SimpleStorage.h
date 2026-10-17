#pragma once

class SimpleStorage
{
public:
	SimpleStorage(std::string_view newFilePath);
	bool HasSection(const std::string& sectionName);
	void WriteToSection();
	
	void WriteString(std::string_view property, std::string_view msg);
	std::vector<std::pair<std::string, std::string>> GetSection(const std::string& sectionName);
private:
	std::string filePath;
};