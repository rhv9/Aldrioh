#pragma once

namespace Platform {

	namespace File {

		std::string ReadFileAsString(const char* path);

		using FileUpdateCallback = std::function<void(const std::string& filePath)>;
		void WatchForFileUpdate(const std::string& path, FileUpdateCallback callback);
		
		void InitInternal();
		void SendThreadShutdownSignal();
		void CleanupInternal();
	}

	float GetElapsedTime();

}