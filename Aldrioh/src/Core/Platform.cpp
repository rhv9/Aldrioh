#include <pch.h>
#include "Platform.h"

#include <fstream>


std::string Platform::File::ReadFileAsString(const char* path)
{
	std::ifstream file(path);
	std::stringstream buffer;
	buffer << file.rdbuf();

	return buffer.str();
}
