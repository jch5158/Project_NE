#include "pch.h"
#include "ThreadManager.h"

ThreadManager::ThreadManager() 
{
	InitTLS();
}

ThreadManager::~ThreadManager()
{
	DestroyTLS();
	Join();
}

void ThreadManager::Launch(std::function<void(void)> callback)
{	
	LockGuard<Mutex> guard(mLock);
	mThreads.push_back(std::thread([=]()-> void 
		{
			InitTLS();
			callback();
			DestroyTLS();
			return;
		}
	));

	return;
}

void ThreadManager::Join()
{
	for (std::thread& t : mThreads)
	{
		t.join();
	}

	mThreads.clear();
	return;
}

void ThreadManager::InitTLS()
{
	initThreadId();

	return;
}

void ThreadManager::DestroyTLS()
{
	return;
}

void ThreadManager::initThreadId()
{
	static std::atomic<uint32> threadId = 1;
	LThreadId = threadId++;
	return;
}
