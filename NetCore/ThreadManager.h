#pragma once

#include <thread>
#include <functional>

class ThreadManager
{
public:
	ThreadManager();
	~ThreadManager();

	void Launch(std::function<void(void)> callback);
	void Join();

	static void InitTLS();
	static void DestroyTLS();

private:
	static void initThreadId();

	Mutex mLock;
	std::vector<std::thread> mThreads;
};



