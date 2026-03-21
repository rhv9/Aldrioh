#include <pch.h>
#include <thread>

#include <File/FileResourceManager.h>
#include <Game.h>

#ifdef PLATFORM_LINUX

void Platform::File::InitInternal()
{
	LOG_CORE_CRITICAL("Platform::File::InitInternal There is no implementation for linux!");
}

void Platform::File::WatchForFileUpdate(const std::string& pathString, FileUpdateCallback callback)
{
	LOG_CORE_CRITICAL("Platform::File::WatchForFileUpdate There is no implementation for linux!");
}

void Platform::File::SendThreadShutdownSignal()
{
	LOG_CORE_CRITICAL("Platform::File::SendThreadShutdownSignal There is no implementation for linux!");
}

void Platform::File::CleanupInternal()
{
	LOG_CORE_CRITICAL("Platform::File::CleanupInternal There is no implementation for linux!");
}


#endif