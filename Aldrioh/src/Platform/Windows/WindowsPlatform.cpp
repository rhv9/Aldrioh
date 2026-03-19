#include "pch.h"
#include "Core/Platform.h"
#include "GLFW/glfw3.h"

#include <Windows.h>


float Platform::GetElapsedTime()
{
	return (float)glfwGetTime();
}

#ifdef PLATFORM_WINDOWS

void Platform::File::WatchForFileUpdate(std::string path)
{
	LPSTR lpDir = const_cast<char*>(path.c_str());
	
	DWORD dwWaitStatus;
	HANDLE dwChangeHandles[2];
	TCHAR lpDrive[4];
	TCHAR lpFile[_MAX_FNAME];
	TCHAR lpExt[_MAX_EXT];


}

#endif

