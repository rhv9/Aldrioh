#include <pch.h>
#include "FileResourceManager.h"

#include <Core/Platform.h>

FileResourceManager::FileResourceManager()
{
	threads.reserve(10);
}

void FileResourceManager::CloseAndJoinThreads()
{
	for (int i = 0; i < threads.size(); ++i)
		if (threads[i].joinable())
			threads[i].request_stop();
	
	Platform::File::SendThreadShutdownSignal();

	for (int i = 0; i < threads.size(); ++i)
		if (threads[i].joinable())
			threads[i].join();
}
