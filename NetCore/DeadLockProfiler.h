#pragma once

class DeadLockProfiler
{
public:

	DeadLockProfiler();
	~DeadLockProfiler();

	void PushLock(const std::string& tag);
	void PopLock(const std::string& tag);

	void CheckCycle();

private:

	void dfs(int32 here);

	std::unordered_map<std::string, int32> mTagTold;
	std::unordered_map<int32, std::string> mIdToTag;
	std::stack<int32> mLockStack;
	std::map<int32, std::set<int32>> mLockHistory;
	Mutex mLock;

	std::vector<int32> mDiscoveredOrder; 	// ��尡 �߰ߵ� ������ ����ϴ� �迭
	int32 mDiscoveredCount = 0;				// ��尡 �߰ߵ� ����
	std::vector<bool> mFinished;			// dfs(i)�� ����Ǿ����� ����
	std::vector<int32> mParent;
};
