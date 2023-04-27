#include "pch.h"
#include "DeadLockProfiler.h"

DeadLockProfiler::DeadLockProfiler()
{
	LockGuard<Mutex> guard(mLock);
}

DeadLockProfiler::~DeadLockProfiler()
{
}

void DeadLockProfiler::PushLock(const std::string& tag)
{
	LockGuard<Mutex> guard(mLock);

	// 아이디를 찾거나 발급
	int32 lockId;
	auto findIter = mTagTold.find(tag);
	if (findIter == mTagTold.end())
	{
		lockId = static_cast<int32>(mTagTold.size());
		mTagTold[tag] = lockId;
		mIdToTag[lockId] = tag;
	}
	else
	{
		lockId = findIter->second;
	}

	// 잡고있는 락이 있다면
	if (mLockStack.empty() == false)
	{
		// 기존에 발견되지 않은 케이스라면 데드락 여부를 확인한다.
		const int32 prevId = mLockStack.top();
		
		// 중복락은 신경쓸 필요가 없음
		if (lockId != prevId)
		{
			auto& history = mLockHistory[prevId];
			if (history.find(lockId) == history.end())
			{
				history.insert(lockId);
				CheckCycle();
			}
		}
	}

	mLockStack.push(lockId);

	return;
}

void DeadLockProfiler::PopLock(const std::string& tag)
{
	LockGuard<Mutex> guard(mLock);

	if (mLockStack.empty())
	{
		CRASH("MULTIPLE_UNLOCK");
	}

	int32 lockId = mTagTold[tag];
	if (mLockStack.top() != lockId)
	{
		CRASH("INVALID_UNLOCK")
	}

	mLockStack.pop();

	return;
}

void DeadLockProfiler::CheckCycle()
{
	const int32 lockCount = static_cast<int32>(mTagTold.size());
	mDiscoveredOrder = std::vector<int32>(lockCount, -1);
	mDiscoveredCount = 0;
	mFinished = std::vector<bool>(lockCount, false);
	mParent = std::vector<int32>(lockCount, -1);

	for (int32 lockId = 0; lockId < lockCount; ++lockId)
	{
		dfs(lockId);
	}

	mDiscoveredOrder.clear();
	mFinished.clear();
	mParent.clear();

	return;
}

void DeadLockProfiler::dfs(int32 here)
{
	if (mDiscoveredOrder[here] != -1)
	{
		return;
	}

	mDiscoveredOrder[here] = mDiscoveredCount++;

	auto findIter = mLockHistory.find(here);
	if (findIter == mLockHistory.end())
	{
		mFinished[here] = true;
		return;
	}

	auto& nextSet = findIter->second;
	for (int32 there : nextSet)
	{
		if (mDiscoveredOrder[there] == -1)
		{
			mParent[there] = here;
			dfs(there);
			continue;
		}

		// here가 there보다 먼저 발견되었다면, there는 here의 후손 즉, 순방향 간선
		if (mDiscoveredOrder[here] < mDiscoveredOrder[there])
			continue;

		// 순방향이 아니고, dfs(there)가 아직 종료하지 않았다면, there는 here의 선조이다.(역방향 간선)
		if (mFinished[there] == false)
		{
			std::cout << mIdToTag[here] << " -> " << mIdToTag[there] << "\n";

			int32 now = here;
			for (;;)
			{
				std::cout << mIdToTag[mParent[now]] << " -> " << mIdToTag[now] << "\n";
				now = mParent[now];
				if (now == there)
					break;
			}

			CRASH("DEADLOCK_DETECTED");
		}
	}

	mFinished[here] = true;

	return;
}
