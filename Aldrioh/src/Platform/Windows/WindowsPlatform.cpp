#include "pch.h"
#include "Core/Platform.h"
#include "GLFW/glfw3.h"

#include <Windows.h>
#include <threads.h>

#include <File/FileResourceManager.h>
#include <Game.h>

float Platform::GetElapsedTime()
{
	return (float)glfwGetTime();
}

#ifdef PLATFORM_WINDOWS

void Platform::File::WatchForFileUpdate(const std::string& path)
{
	LOG_CORE_INFO("Waiting for updates on path: {}", path.c_str());
	FileResourceManager* fileResourceManager = Game::Instance().GetFileResourceManager();
	fileResourceManager->AddThread([](std::stop_token token, std::string path)
		{
			LPSTR lpDir = const_cast<char*>(path.c_str());

			DWORD dwWaitStatus;
			HANDLE dwChangeHandle;
			TCHAR lpDrive[4];
			TCHAR lpFile[_MAX_FNAME];
			TCHAR lpExt[_MAX_EXT];

			dwChangeHandle = FindFirstChangeNotificationA(lpDir, FALSE, FILE_NOTIFY_CHANGE_FILE_NAME);

			if (dwChangeHandle == INVALID_HANDLE_VALUE)
			{
				LOG_CORE_ERROR("Failed to create file change handle!");
				return;
			}

			while (true)
			{
				dwWaitStatus = WaitForSingleObject(dwChangeHandle, 1000);
				
				if (token.stop_requested())
					break;
				if (dwWaitStatus == WAIT_OBJECT_0)
				{
					LOG_CORE_INFO("CHANGED NOTICED NO WAY!!");
					if (FindNextChangeNotification(dwChangeHandle) == FALSE)
					{
						LOG_CORE_ERROR("ERROR: FindNextChangeNotification function failed.");
						break;
					}
				}
			}

		}, path);
}

#endif

