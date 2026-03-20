#pragma once

namespace Platform {

	namespace File {

		std::string ReadFileAsString(const char* path);

		void WatchForFileUpdate(const std::string& path);
	}

	float GetElapsedTime();

}