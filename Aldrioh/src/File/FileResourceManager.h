#pragma once
#include <threads.h>

class FileResourceManager
{
public:
	FileResourceManager();
	template <typename Callable, typename... Args>
	void AddThread(Callable&& func, Args&&... args) {
		threads.emplace_back(std::forward<Callable>(func),
			std::forward<Args>(args)...);
	}
	void CloseAndJoinThreads();

private:
	std::vector<std::jthread> threads;
};