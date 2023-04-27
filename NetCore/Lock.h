#pragma once

// W -> R은 허용
// R -> W는 비허용
class Lock
{
private:
	enum : uint32
	{
		ACQUIRE_TIMEOUT_TICK = 10000,
		MAX_SPIN_COUNT = 5000,
		WRITE_THREAD_MASK = 0xffff'0000,
		READ_COUNT_MASK = 0x0000'ffff,
		EMPTY_FLAG = 0x0000'0000,
	};

public:
	void WriteLock(const std::string& tag);
	void WriteUnlock(const std::string& tag);
	void ReadLock(const std::string& tag);
	void ReadUnlock(const std::string& tag);

private:
	std::atomic<uint32> mLockState = EMPTY_FLAG;
	uint16 mWriteCount = 0;
};

class ReadLockGuard
{
public:
	ReadLockGuard(Lock& lock, const std::string& tag)
		:mLock(lock)
		, mTag(tag)
	{
		mLock.ReadLock(tag);
	}

	~ReadLockGuard()
	{
		mLock.ReadUnlock(mTag);
	}

private:
	Lock& mLock;
	const std::string mTag;
};

class WriteLockGuard
{
public:
	WriteLockGuard(Lock& lock, const std::string& tag) 
		:mLock(lock)
		, mTag(tag)
	{
		mLock.WriteLock(tag);
	}

	~WriteLockGuard()
	{
		mLock.WriteUnlock(mTag);
	}

private:
	Lock& mLock;
	const std::string mTag;
};