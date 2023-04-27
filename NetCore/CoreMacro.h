#pragma once

// out 파라미터 표기를 위해 사용
#define OUT


/*-----------
	Lock
-----------*/
#define USE_MANY_LOCKS(count)	Lock mLocks[count];
#define USE_LOCK				USE_MANY_LOCKS(1)
#define READ_LOCK_IDX(idx)		ReadLockGuard readLockGuard_##idx(mLocks[idx], typeid(this).name());
#define READ_LOCK				READ_LOCK_IDX(0)
#define WRITE_LOCK_IDX(idx)		WriteLockGuard writeLockGuard_##idx(mLocks[idx], typeid(this).name());
#define WRITE_LOCK				WRITE_LOCK_IDX(0)

 
/*------------
	Crash
-------------*/
#define CRASH(cause) 					   \
{					  					   \
	uint32* crash = nullptr;			   \
	__analysis_assume(crash != nullptr);   \
	*crash = 0xDEADBEEF;				   \
}					  				   

#define ASSERT_CRASH(expr)				\
{										\
	if (!(expr))						\
	{									\
		CRASH("ASSERT_CRASH");			\
		__analysis_assume(expr);		\
	}									\
}