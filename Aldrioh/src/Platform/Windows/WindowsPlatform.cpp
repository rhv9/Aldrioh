#include "pch.h"
#include "Core/Platform.h"
#include "GLFW/glfw3.h"

#include <Windows.h>
#include <thread>

#include <File/FileResourceManager.h>
#include <Game.h>

#include <filesystem>

float Platform::GetElapsedTime()
{
	return (float)glfwGetTime();
}

#ifdef PLATFORM_WINDOWS

void Platform::File::WatchForFileUpdate(const std::string& pathString, FileUpdateCallback callback)
{
	LOG_CORE_TRACE("File:: Waiting for updates on path: {}", pathString.c_str());

	FileResourceManager* fileResourceManager = Game::Instance().GetFileResourceManager();
	fileResourceManager->AddThread([](std::stop_token token, std::string filePath, FileUpdateCallback callback)
		{
			std::string dirPathString;

			std::filesystem::path pathObj(filePath);
			std::error_code errorCode;
			std::filesystem::file_time_type oldWriteTime = std::filesystem::last_write_time(pathObj, errorCode);
			
			if (errorCode)
			{
				LOG_CORE_ERROR("Failed to get last modified time for file: {}", filePath.c_str());
				LOG_CORE_ERROR("Message: {}", errorCode.message().c_str());
				return;
			}

			dirPathString = pathObj.parent_path().string();

			LPSTR lpDir = const_cast<char*>(dirPathString.c_str());
			DWORD dwWaitStatus;
			HANDLE dwChangeHandle;
			dwChangeHandle = FindFirstChangeNotificationA(lpDir, FALSE, FILE_NOTIFY_CHANGE_LAST_WRITE);

			if (dwChangeHandle == INVALID_HANDLE_VALUE)
			{
				LOG_CORE_ERROR("Failed to create file change handle!");
				return;
			}

			while (true)
			{
				dwWaitStatus = WaitForSingleObject(dwChangeHandle, 1000);
				
				if (token.stop_requested())
				{
					LOG_CORE_TRACE("Requested to close file watch: {}", filePath.c_str());
					break;
				}
				if (dwWaitStatus == WAIT_OBJECT_0)
				{
					errorCode.clear();
					std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Added this as noticed bug of same time being returned, maybe delay in file metadata being updated and event for update being raised
					auto currentWriteTime = std::filesystem::last_write_time(std::filesystem::path(filePath), errorCode);

					if (errorCode)
					{
						LOG_CORE_ERROR("Something went wrong in getting last time: {}", filePath.c_str());
						LOG_CORE_ERROR("Message: {}", errorCode.message().c_str());
						continue;
					}
					if (currentWriteTime > oldWriteTime)
					{
						oldWriteTime = currentWriteTime;
						callback(filePath);
					}
					if (FindNextChangeNotification(dwChangeHandle) == FALSE)
					{
						LOG_CORE_ERROR("ERROR: FindNextChangeNotification function failed.");
						break;
					}
				}
			}

		}, pathString, callback);
}

#endif

