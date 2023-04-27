#pragma once

class DeadLockProfiler
{
public:

	DeadLockProfiler();
	~DeadLockProfiler();

	DeadLockProfiler(const DeadLockProfiler&) = delete;
	DeadLockProfiler& operator=(const DeadLockProfiler&) = delete;

	void PushLock(const std::string& tag);
	void PopLock(const std::string& tag);

	void CheckCycle();

private:

	void dfs(int32 here);

	static thread_local std::stack<int32> mLockStack;

	std::unordered_map<std::string, int32> mTagTold;
	std::unordered_map<int32, std::string> mIdToTag;
	std::map<int32, std::set<int32>> mLockHistory;
	Mutex mLock; 

	std::vector<int32> mDiscoveredOrder; 	// 노드가 발견된 순서를 기록하는 배열
	int32 mDiscoveredCount = 0;				// 노드가 발견된 순서
	std::vector<bool> mFinished;			// dfs(i)가 종료되었는지 여부
	std::vector<int32> mParent;
};

