#pragma once

namespace Platform {

	namespace File {

		std::string ReadFileAsString(const char* path);
	}

	float GetElapsedTime();

}