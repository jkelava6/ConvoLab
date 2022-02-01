#pragma once

#include <Core/Threading/ThreadInclude.h> // FCppThread

#include <cassert> // assert()

CORE_THREADING_NAMESPACE_BEGIN

extern FCppThread::id MainThreadId;
#if _DEBUG
#define THREAD_GUARD() FCoreThreading::ThreadGuard()
#else
#define THREAD_GUARD()
#endif
#define SET_THIS_THREAD_AS_MAIN() FCoreThreading::MainThreadId = std::this_thread::get_id()

extern int DefaultWaitTime;

extern int InitializeThreadPool(int ThreadCount);
extern int ThreadPoolCount();

extern int QueueTaskBlocking(int PoolId, FunctionPointer(void, Task, void*), void* Arg = nullptr);
extern bool AllTasksDone(int PoolId);
extern void WaitForAllTasks(int PoolId);

extern void Sleep(int Milliseconds);

extern void ThreadGuard();

CORE_THREADING_NAMESPACE_END
