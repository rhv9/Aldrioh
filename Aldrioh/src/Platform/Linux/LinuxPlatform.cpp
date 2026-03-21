#include <pch.h>
#include <thread>

#include <File/FileResourceManager.h>
#include <Game.h>

#ifdef PLATFORM_LINUX

void Platform::File::WatchForFileUpdate(const std::string& pathString, FileUpdateCallback callback)
{
	LOG_CORE_CRITICAL("Platform::File::WatchForFileUpdate There is no implementation for linux!");
}

#endif