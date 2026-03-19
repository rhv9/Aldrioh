#pragma once

namespace Platform {

	namespace File {

		std::string ReadFileAsString(const char* path);

		void WatchForFileUpdate(std::string path);
	}

	float GetElapsedTime();

}