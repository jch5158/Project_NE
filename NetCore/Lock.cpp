#include "pch.h"
#include "Lock.h"

void Lock::WriteLock(const std::string& tag)
{
#ifdef _DEBUG
	GDeadLockProfiler->PushLock(tag);
#endif

	const uint32 lockThreadId = (mLockState.load() & WRITE_THREAD_MASK) >> 16;
	if (lockThreadId == LThreadId)
	{
		++mWriteCount;
		return;
	}

	const uint64 beginTick = GetTickCount64();
	const uint32 desired = (LThreadId << 16) & WRITE_THREAD_MASK;
	for (;;)
	{
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; ++spinCount)
		{
			uint32 expected = EMPTY_FLAG;
			if (mLockState.compare_exchange_strong(OUT expected, desired))
			{
				++mWriteCount;
				return;
			}
		}

		if (GetTickCount64() - beginTick >= ACQUIRE_TIMEOUT_TICK)
		{
			CRASH("LOCK_TIMEOUT");
		}

		std::this_thread::yield();
	}

	return;
}

void Lock::WriteUnlock(const std::string& tag)
{
#ifdef _DEBUG
	GDeadLockProfiler->PopLock(tag);
#endif

	if ((mLockState.load() & READ_COUNT_MASK) != 0)
	{
		CRASH("INVALID_UNLOCK_ORDER");
		return;
	}

	const uint16 writeCount = --mWriteCount;
	if (writeCount == 0)
	{
		mLockState.store(EMPTY_FLAG);
	}

	return;
}

void Lock::ReadLock(const std::string& tag)
{
#ifdef _DEBUG
	GDeadLockProfiler->PushLock(tag);
#endif

	const uint32 lockThreadId = (mLockState.load() & WRITE_THREAD_MASK) >> 16;
	if (lockThreadId == LThreadId)
	{
		mLockState.fetch_add(1);
		return;
	}

	const uint64 beginTick = GetTickCount64();
	for (;;)
	{
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; ++spinCount)
		{
			uint32 expected = mLockState.load() & READ_COUNT_MASK;
			if (mLockState.compare_exchange_strong(OUT expected, expected + 1))
			{
				return;
			}
		}

		if (GetTickCount64() - beginTick >= ACQUIRE_TIMEOUT_TICK)
		{
			CRASH("LOCK_TIMEOUT");
		}

		std::this_thread::yield();
	}

	return;
}

void Lock::ReadUnlock(const std::string& tag)
{
#ifdef _DEBUG
	GDeadLockProfiler->PopLock(tag);
#endif

	if ((mLockState.fetch_sub(1) & READ_COUNT_MASK) == 0)
	{
		CRASH("MULTIPLE_UNLOCK");
	}

	return;
}
